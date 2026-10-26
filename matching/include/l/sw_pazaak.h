// Worker L matching header: the Pazaak rules objects in swpazaak.o
// (0x006E4360-0x006E4C60). CPazaakCard {value, flag}, CPazaakPlayer (0x70,
// no vtable) and CSWPazaak (0x230, no vtable). Layout copied from
// matching/include/m/sw_gui_pazaak.h (bucket m), trimmed.
#ifndef L_SW_PAZAAK_H
#define L_SW_PAZAAK_H

class CExoString {
public:
    char* m_sString;
    int m_nBufferLength;
    CExoString();                                   // 0x005B3190
    ~CExoString();                                  // 0x005E5C20
    char operator[](int nIndex) const;              // 0x005E5600
};

class CResRef {
public:
    CResRef(const char* s);                         // 0x00406D80
    // Declared, not defined: the argument is built in place.
    CResRef(const CResRef& c);
    char m_resRef[16];
};

class C2DA {
public:
    C2DA(CResRef cResRef, int bPrivate);            // 0x00413CC0
    virtual ~C2DA();                                // 0x00413D80
    int Load2DArray();                              // 0x004143B0
    void Unload2DArray();                           // 0x004139E0
    int GetCExoStringEntry(int nRow, int nColumn, CExoString* pValue); // 0x00413190
    char pad_0x04[0x20];
    int m_nNumRows;                                 // 0x24
    char pad_0x28[0x54 - 0x28];
};

extern "C" int __cdecl rand(void);                  // 0x006FC447

// Card value by card code: 0..5 +1..+6, 6..11 -1..-6, 12..17 flip +-1..6,
// 18..27 the main-deck 1..10. Descriptive name.
extern int g_nPazaakCardValues[28];                 // 0x007A26B8

// A Pazaak card {value, flag}; the constructor only clears the flag.
struct CPazaakCard {
    CPazaakCard() { m_bFlag = 0; }
    CPazaakCard(int nValue, int bFlag)
    {
        m_nValue = nValue;
        m_bFlag = bFlag;
    }
    int m_nValue;
    int m_bFlag;
};

// 0x70 bytes, no vtable (ctor 0x006E4B50).
class CPazaakPlayer {
public:
    CPazaakPlayer();                                // 0x006E4B50
    int GetTotal();                                 // 0x006E4360
    void ChooseSidedeck(CPazaakCard* pCards);       // 0x006E4580
    void AddGameCard(CPazaakCard cCard);            // 0x006E45F0
    CPazaakCard m_aHand[4];                         // 0x00
    CPazaakCard m_aBoard[9];                        // 0x20
    int m_bStanding;                                // 0x68
    int m_nSetsWon;                                 // 0x6c
};

// 0x230 bytes, no vtable (ctor 0x006E4C00).
class CSWPazaak {
public:
    CSWPazaak();                                    // 0x006E4C00
    void ClearGameBoard();                          // 0x006E4640
    void ShuffleDeck();                             // 0x006E4730 (descriptive)
    CPazaakCard DrawCard();                         // 0x006E4800
    int CanUsePlayerSidedeckCard(int nIndex);       // 0x006E4840
    int UsePlayerSidedeckCard(int nIndex);          // 0x006E4860
    int DetermineWinner();                          // 0x006E48A0 (descriptive)
    int FindPotentialSidecardTotal(CPazaakPlayer* pPlayer, int* pIndex, int bLower); // 0x006E4910 (descriptive)
    void LoadInitialSideDeck(int nDeck, CPazaakCard* pCards);   // 0x006E4A40
    int PlayAITurn(CPazaakPlayer* pAI, CPazaakPlayer* pPlayer, int nState);   // 0x006E4C60

    int m_nWager;                                   // 0x000
    unsigned long m_oidOpponent;                    // 0x004
    CPazaakPlayer m_cPlayer;                        // 0x008
    CPazaakPlayer m_cAI;                            // 0x078
    CPazaakCard m_aDeck[40];                        // 0x0e8
    int m_nDeckIndex;                               // 0x228
    int m_nAIState;                                 // 0x22c
};

#endif // L_SW_PAZAAK_H
