// sws_combat_data.h -- CSWSCombatData
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/c/client_exo_app_internal_r5.cpp); the other copies' members fall on its members.
#ifndef SHARED_SWS_COMBAT_DATA_H
#define SHARED_SWS_COMBAT_DATA_H


struct CSWSCombatData {
    char pad_0x0[0x34];
    unsigned long m_oidTarget;                      // 0x34

    // declared by other copies
    ~CSWSCombatData();
};

#endif
