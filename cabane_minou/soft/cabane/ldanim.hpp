#ifndef LDANIM_HEADER_INCLUDED
#define LDANIM_HEADER_INCLUDED

#include "ldstrip.hpp"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef WIN32
  #include <windows.h>
  #define millis() GetTickCount()
#endif

/**
 * @brief Liste des MACRO instructions
 * @ {
*/
#define INSTR_NOP     'N'
#define INSTR_CLRALL  'Z'
#define INSTR_SETALL  'A'
#define INSTR_START   'S'
#define INSTR_END     'E'
#define INSTR_CLR     'C'
#define INSTR_SETCOL  'L'
#define INSTR_SETR    'R'
#define INSTR_SETG    'G'
#define INSTR_SETB    'B'
#define INSTR_SET     'O'
#define INSTR_DELAY_S 'D'
#define INSTR_DEL_MS  'X'
#define INSTR_WAIT    'W'
#define INSTR_STOP    '.'
#define INSTR_REPEAT  '*'
/**
 * @ }
*/

/**
 * @file ldanim.h
 * 
 * @brief Classe de gestion du macro langage de pilotage des LEDs
 * 
 * On fourni une chaine avec un MACROPROGRAMME a cette classe
 * Elle gere le pilotage des LEDs et l'execution du programme
*/

template <int PIN_DATA,int POW_5V, int NBRE>
class LdAnim : public LdStrip<PIN_DATA,POW_5V,NBRE>
{
private:
    bool m_running;
    char* m_pProg;    ///< Chaine du programme MACRO LANGAGE
    int m_iProgLen;   ///< Taille du programme

    char* m_pPos;     ///< Position courante dans le programme

    char m_instr;     ///< Instruction en cours
    long m_delay_ms;  ///< Dureee de l'instruction en cours
    long m_tick0_ms;  ///< Tick en ms du debut d'execution de l'instruction en cours
    char m_r;         ///< Partie rouge de la couleur courante
    char m_g;         ///< Partie verte de la couleur courante
    char m_b;         ///< Partie blue de la couleur courante
    char m_start;     ///< Premiere LED de la selection en cours
    char m_end;       ///< Derniere LED de la selection en cours

    /**
    * @brief Recupere la prochaine instruction et la decode
    */
    void fetch_next(void)
    {
      if (m_pPos - m_pProg < m_iProgLen)
      {
          decode();
      }
    }

    /**
     * @brief Decode l'instruction a l'emplacement courant
    */
    void decode()
    {
      if (m_pPos == NULL)
          return;

      /// On recupere l'instruction (position courante)
      char bNextInstr = (char)(*m_pPos);
      m_instr = bNextInstr;

      logStep();

      /// Si Stop, on retourne pour ne pas aller plus loin
      if (bNextInstr == INSTR_STOP)
          return;

      /// On avance
      m_pPos++;
      if (m_pPos > m_pProg+m_iProgLen)
      {
          /// Si on est a la fin du programme, on force l'instruction STOP
          m_instr = INSTR_STOP;
          return;
      }

      /// On prepare le contexte pour cette instruction
      switch (m_instr)
      {
        /// Pour le start, on memorise l'index de LED a la fois pour start et end
        case INSTR_START:
        {
            int num;
            if (decodeNum(&num) == true)
            {
                if ((num >= 0) && (num < NBRE))
                {
                    m_start = num;
                    m_end = num;
                }
            }
            break;
        }
        /// Pour le end, on memorise seulement le end
        case INSTR_END:
        {
            int num;
            if (decodeNum(&num) == true)
            {
                if ((num >= 0) && (num < NBRE))
                {
                    m_end = num;
                }
            }
            break;
        }
        /// On momorise la couleur (les composantes en fonction du raccourci donne)
        case INSTR_SETCOL:
        {
            char col = (*m_pPos);
            m_pPos++;

            switch (col)
            {
              case 'r':m_r = 127; m_g = 0; m_b = 0; break;
              case 'g':m_r = 0; m_g = 127; m_b = 0; break;
              case 'b':m_r = 0; m_g = 0; m_b = 127; break;
              case 'w':m_r = 127; m_g = 127; m_b = 127; break;
              case 'y':m_r = 127; m_g = 127; m_b = 0; break;
              case 'o':m_r = 127; m_g = 82; m_b = 0; break;
              case 'p':m_r = 127; m_g = 9; m_b = 73; break;
              case 'c':m_r = 0; m_g = 127; m_b = 127; break;
              case 'v':m_r = 127; m_g = 0; m_b = 127; break;
            }

            break;
        }
        /// Memorisation de la composante Rouge
        case INSTR_SETR:
        {
            int num = 0;
            if (decodeNum(&num) == true)
                m_r = num;

            break;
        }
        /// Memorisation de la composante Verte
        case INSTR_SETG:
        {
            int num = 0;
            if (decodeNum(&num) == true)
                m_g = num;

            break;
        }
        /// Memorisation de la composante Bleue
        case INSTR_SETB:
        {
            int num = 0;
            if (decodeNum(&num) == true)
                m_b = num;

            break;
        }
        /// Memorisation du delai demande (exprime en secondes)
        case INSTR_DELAY_S:
        {
            int num = 0;
            if (decodeNum(&num) == true)
                m_delay_ms = num * 1000L;
            break;
        }
        /// Memorisation du delai demande (exprime en millis)
        case INSTR_DEL_MS:
        {
            int num = 0;
            if (decodeNum(&num) == true)
                m_delay_ms = num;
            break;
        }
        /// Debut de l'attente: On prend la valeur courante du tick en ms
        case INSTR_WAIT:
        {
            m_tick0_ms = millis();
            break;
        }
      }
    }

    /**
     * @brief Recupere et decode un nombre a partir de la position courante
     * @param i_pint Nombre retourne
     * @return true si un nombre a bien ete decode
    */
    bool decodeNum(int* i_pint)
    {
      char* pDec = m_pPos;
      while (isNumDigit(*pDec) == true)
      {
          pDec++;
      }

      if (pDec == m_pPos)
          return false;

      char strVal[5];
      memcpy(strVal, m_pPos, pDec - m_pPos);
      strVal[pDec - m_pPos] = 0;
      (*i_pint) = atoi(strVal);
      m_pPos = pDec;

      #ifndef WIN32
          Serial.println((*i_pint));
      #else
          printf("%d\n", (*i_pint));
      #endif

      return true;
    }

    /**
     * @brief Indique si le caractere est un nombre
     * @param c Caractere
     * @return true si c'est un numero
    */
    bool isNumDigit(char c)
    {
      if ((c >= '0') && (c <= '9'))
      {
          return true;
      }
      else
      {
          return false;
      }
    }

    /**
     * @brief Pilote la LED
     * @param inx Numero de LED
     * @param r Composante rouge (0-127)
     * @param g Composante verte (0-127)
     * @param b Composante bleue (0-127)
    */
    /*void setLed(char inx, char r, char g, char b)
    {
      #ifndef WIN32
      this->leds[inx] = CRGB(r, g, b);
      #else
      #endif
    }*/

    /**
     * @brief Log de l'etape en cours sur port serie (ou printf si WIN32)
    */
    void logStep()
    {
      char strMsg[30];

      sprintf(strMsg, "Ins: %c: {%d -> %d: (%d,%d,%d): %ld ms}",
          m_instr,
          m_start,
          m_end,
          m_r,
          m_g,
          m_b,
          m_delay_ms
      );

      #ifndef WIN32
          Serial.println(strMsg);
      #else
          printf("%s\n",strMsg);
      #endif
    }

public:    
    LdAnim()
    {
      m_pProg = NULL;
      m_pPos = NULL;
      m_instr = 0;
      m_delay_ms = 1000;
      m_tick0_ms = millis();
      m_r = 127;
      m_g = 0;
      m_b = 0;
      m_iProgLen = 0;
      m_start = 0;
      m_end = 0;   
      m_running=false;   
    }

    /**
     * @brief Initialisation d'un MACRO programme
     * @param prog Programme a executer
     * @param len Taille en octets du programme
    */
    void setprog(char* prog, int len)
    {
      m_tick0_ms = millis();
      m_pProg = prog;
      m_pPos = prog;
      m_iProgLen = len;

      fetch_next();
    }

    /**
     * @brief Tick a appeler aussi souvent que possible
     *
     * Gere la condition de passage a l'instruction suivante
    */
    void tick(void)
    {
      if (m_running==false)
      {
        return;
      }

      switch (m_instr)
      {
        case INSTR_NOP:
        case INSTR_START:
        case INSTR_END:
        case INSTR_SETCOL:
        case INSTR_SETR:
        case INSTR_SETG:
        case INSTR_SETB:
        case INSTR_DELAY_S:
        case INSTR_DEL_MS:
        {
            /// Passage inconditionnel a l'instruction suivante
            fetch_next();
            break;
        }

        /// Toutes les LEDs eteintes puis suivant
        case INSTR_CLRALL:
        {
            for (int i = 0; i < NBRE; i++)
            {
              this->setLed(i, 0, 0, 0);
            }
            fetch_next();
            break;
        }
        /// Toutes les LEDs allumees avec le RGB courant puis suivant
        case INSTR_SETALL:
        {
            for (int i = 0; i < NBRE; i++)
            {
                this->setLed(i, m_r, m_g, m_b);
            }
            fetch_next();
            break;
        }
        /// Effacer les LEDs selectionnees puis suivant
        case INSTR_CLR:
        {
            for (int i = m_start; i <= m_end; i++)
            {
              this->setLed(i, 0, 0, 0);
            }
            fetch_next();
            break;
        }
        /// Allumer les LEDs selectionnees avec le RGB courant puis suivant
        case INSTR_SET:
        {
            for (int i = m_start; i <= m_end; i++)
            {
              this->setLed(i, m_r, m_g, m_b);
            }
            fetch_next();
            break;
        }
        /// Attendre la fin du delai puis suivant
        case INSTR_WAIT:
        {
            long t = millis();
            long delta = 0;
            if (t > m_tick0_ms)
                delta = t - m_tick0_ms;
            else
                delta = 0xFFFFFFFF - m_tick0_ms + t;

            if (delta > m_delay_ms)
                fetch_next();

            break;
        }
        /// Ne rien plus faire
        case INSTR_STOP:
        {
            break;
        }
        /// Revenir au debut du programme
        case INSTR_REPEAT:
        {
            m_pPos = m_pProg;
            fetch_next();
            break;
        }
        }

    #ifndef WIN32
        FastLED.show();
    #endif
    }

    void start(void)
    {
      m_running=true;
    }

    void stop(void)
    {
      m_running=false;
    }

    bool isRunning(void)
    {
      return m_running;
    }

    /**
     * @brief Retourne une chaine decrivant l'instruction en cours
     * @param strOut Buffer de sortie (chaine)
     * @param maxLen Taille allouee pour le buffer
    */
    void getDumpStr(char* strOut, int maxLen)
    {
      char strMsg[30];

      sprintf(strMsg, "Ins: %c: {%d -> %d: (%d,%d,%d): %d ms}",
          m_instr,
          m_start,
          m_end,
          m_r,
          m_g,
          m_b,
          m_delay_ms
      );

      strncpy(strOut, strMsg, maxLen);
      strOut[maxLen - 1] = 0;
    }
};

#endif
