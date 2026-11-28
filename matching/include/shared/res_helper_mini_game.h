// res_helper_mini_game.h -- CResHelperMiniGame
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/m/sw_mini_game_object.h); the other copies' members fall on its members.
#ifndef SHARED_RES_HELPER_MINI_GAME_H
#define SHARED_RES_HELPER_MINI_GAME_H

#include "shared/res_ref.h"

class CResHelperMiniGame {
public:
    CResHelperMiniGame()
    {
        m_pRes = 0;
        m_cResRef = "";
        m_bAutoRequest = 0;
    }
    virtual ~CResHelperMiniGame();                  // 0x00506BD0
    void SetResRef(const CResRef& cResRef, int bAutoRequest);   // 0x00506C30 (shared template)

    int m_bAutoRequest;                             // 0x04
    void* m_pRes;                                   // 0x08
    CResRef m_cResRef;                              // 0x0c
};

#endif
