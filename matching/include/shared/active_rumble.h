// active_rumble.h -- CActiveRumble
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/c/client_exo_app.h); the other copies' members fall on its members.
#ifndef SHARED_ACTIVE_RUMBLE_H
#define SHARED_ACTIVE_RUMBLE_H


struct CActiveRumble {
    int m_nPattern;                                 // 0x00
    union {
        int m_nTime;                                // 0x04
        float m_fTime;                              // 0x04 (elapsed seconds)
    };
    int m_bActive;                                  // 0x08
};

#endif
