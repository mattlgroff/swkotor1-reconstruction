// sw_party_member_entry.h -- CSWPartyMemberEntry
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/c/client_exo_app_internal_combat_s9.cpp); the other copies' members fall on its members.
#ifndef SHARED_SW_PARTY_MEMBER_ENTRY_H
#define SHARED_SW_PARTY_MEMBER_ENTRY_H


struct CSWPartyMemberEntry {
    char pad_0x0[0x20];
    unsigned long  m_oidCreature;                // 0x20
    char pad_0x24[0x5c];
    unsigned long m_oidTarget;                      // 0x80 (descriptive)
    unsigned long m_nCombatMessage;                 // 0x84 (descriptive)
};

#endif
