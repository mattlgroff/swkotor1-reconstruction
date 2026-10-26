#ifndef C_SW_C_CREATURE_ABILITY_LIST_H
#define C_SW_C_CREATURE_ABILITY_LIST_H

// Constructor at 0x006475c0 proves writes at +0 (dword zero), +4 (dword one),
// and +8 (byte zero). Unknown field names remain offset-based.
class CSWSStats_SpellLikeAbility {
public:
    unsigned long m_nSpell;
    int m_nValue4;
    unsigned char m_nValue8;
    char pad_0x9[3];
    CSWSStats_SpellLikeAbility();
};

#endif
