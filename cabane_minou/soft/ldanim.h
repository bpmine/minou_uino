#ifndef LDANIM_HEADER_INCLUDED
#define LDANIM_HEADER_INCLUDED

#include "ldstrip.hpp"

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

    void fetch_next(void);  
    void decode();
    bool decodeNum(int* i_pint);
    bool isNumDigit(char c);

    void setLed(char inx, char r, char g, char b);

    void logStep();

public:    
    LdAnim()
    {
      m_pProg = NULL;
      m_pPos = NULL;
      m_instr = 0;
      m_delay_ms = 1000;
      m_tick0_ms = 0;
      m_r = 127;
      m_g = 0;
      m_b = 0;
      m_iProgLen = 0;
      m_start = 0;
      m_end = 0;   
      m_running=false;   
    }

    void init(char* prog, int len);
    void tick(void);

    void start(void);
    void stop(void);

    void getDumpStr(char* strOut, int maxLen);
};

#endif
