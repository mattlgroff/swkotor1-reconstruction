// Compact CSWCItem field access used by matching leaves.
#ifndef M_SW_C_ITEM_H
#define M_SW_C_ITEM_H

class CSWCCreature {
public:
    char pad_0x0[0x4];
    unsigned long m_oidSelf;                  // 0x04
};

class CSWCItem {
public:
    char pad_0x0[0x160];
    unsigned long m_oidEquippedBy;            // 0x160

    void SetEquippedBy(CSWCCreature* pCreature); // 0x00646600
};

#endif
