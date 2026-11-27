// sw_combat_information.h -- CSWCombatInformation
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/m/sw_c_creature_stats_lifecycle.cpp); the other copies' members fall on its members.
#ifndef SHARED_SW_COMBAT_INFORMATION_H
#define SHARED_SW_COMBAT_INFORMATION_H


class CSWCombatInformation {
public:
    CSWCombatInformation();                         // 0x00552280
    ~CSWCombatInformation();                        // 0x00550E00
    char pad_0x0[0x48];

    // declared by other copies
    void SetCombatInformation(CSWCombatInformation *pInfo);
};

#endif
