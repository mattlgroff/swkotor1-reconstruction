// sw_pazaak.h -- CSWPazaak
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/l/sw_pazaak.h); the other copies' members fall on its members.
#ifndef SHARED_SW_PAZAAK_H
#define SHARED_SW_PAZAAK_H

#include "shared/pazaak_card.h"
#include "shared/pazaak_player.h"

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

    // declared by other copies
    CPazaakPlayer* GetPlayer();
    void DrawPlayerCard();
    int DoAITurn();
};

#endif
