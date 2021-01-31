
/**
 * @file ldanim.h
 * 
 * @brief Classe de gestion du macro langage de pilotage des LEDs
 * 
 * On fourni une chaine avec un MACROPROGRAMME a cette classe
 * Elle gere le pilotage des LEDs et l'execution du programme
*/
class LdAnim
{
private:
    void* m_leds;     ///< Tableau des LEDs
    int m_nbLeds;     ///< Nombre de LEDs
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
    LdAnim(void* leds, int nbLeds);

    void init(char* prog, int len);
    void tick(void);

    void getDumpStr(char* strOut, int maxLen);
};
