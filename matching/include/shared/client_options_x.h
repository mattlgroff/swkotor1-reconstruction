// client_options_x.h -- CClientOptionsX
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/c/sw_c_creature_r6.cpp); the other copies' members fall on its members.
#ifndef SHARED_CLIENT_OPTIONS_X_H
#define SHARED_CLIENT_OPTIONS_X_H


struct CClientOptionsX {
    char pad_0x0[0x14];
    unsigned char  m_nFlags;                     // 0x14
    char pad_0x15[0x57];
    unsigned char m_nShadows;                       // 0x6c
    unsigned char m_nCameraMode;                    // 0x6d
};

#endif
