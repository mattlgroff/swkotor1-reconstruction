// Pilot 4 arm A: declarations for CSWSCreature::MaxAttackRange (0x004FB0F0).
#ifndef P4A_FB0F0_H
#define P4A_FB0F0_H

class CSWBaseItem {
public:
    char pad_0x0[0x1a];
    unsigned char m_nRangedWeapon;                          // 0x1a
    char pad_0x1b[0x34 - 0x1b];
    float m_fMaxRange;                                      // 0x34

    int IsRangedWeapon() { return m_nRangedWeapon > 0; }
};

class CSWItem {
public:
    CSWBaseItem* GetBaseItem();                             // 0x005B4790
};

class CSWSItem : public CSWItem {
};

class CSWInventory {
public:
    CSWSItem* GetItemInSlot(unsigned long nSlot);           // 0x005A4C20
};

class CSWSCreature {
public:
    float DesiredAttackRange(unsigned long oTarget, int a2);           // 0x004F1310
    float MaxAttackRange(unsigned long oTarget, int a2, int bRangedOnly); // 0x004FB0F0

    char pad_0x0[0xa2c];
    CSWInventory* m_pInventory;                             // 0xa2c
};

#endif
