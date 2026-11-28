// pazaak_player.h -- CPazaakPlayer
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/k/sw_gui_pazaak.h); the other copies' members fall on its members.
#ifndef SHARED_PAZAAK_PLAYER_H
#define SHARED_PAZAAK_PLAYER_H

#include "shared/pazaak_card.h"

class CPazaakPlayer {
public:
    CPazaakPlayer();                                // 0x006E4B50
    int GetTotal();                                 // 0x006E4360
    void AddGameCard(CPazaakCard cCard);            // 0x006E45F0
    void ChooseSidedeck(CPazaakCard* pCards);       // 0x006E4580
    CPazaakCard GetBoardCard(int i) { return m_aBoard[i]; }
    CPazaakCard GetHandCard(int i) { return m_aHand[i]; }
    CPazaakCard m_aHand[4];                         // 0x00
    CPazaakCard m_aBoard[9];                        // 0x20
    int m_bStanding;                                // 0x68
    int m_nSetsWon;                                 // 0x6c
};

#endif
