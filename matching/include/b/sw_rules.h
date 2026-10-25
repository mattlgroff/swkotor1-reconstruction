// CSWRules / CTwoDimArrays (only the members worker B sources touch).
#ifndef B_SW_RULES_H
#define B_SW_RULES_H

#include "b/res_gff.h"

class CTwoDimArrays {
public:
    C2DA* GetIPRPCostTable(unsigned char nTable);           // 0x005C49A0
    C2DA* GetIPRPDamageCost() { return m_pIPRPDamageCost; }
    C2DA* GetIPRPOnHit() { return m_pIPRPOnHit; }
    C2DA* GetItemValue() { return m_pItemValue; }

    C2DA* GetDifficultyOptions() { return m_pDifficultyOptions; }

    char pad_0x0[0x7c];
    C2DA* m_pDifficultyOptions; // 0x7c (MULTIPLIER; name guessed)
    char pad_0x80[0xb0 - 0x80];
    C2DA* m_pIPRPDamageCost;    // 0xb0 (NumDice / Die)
    char pad_0xb4[0xcc - 0xb4];
    C2DA* m_pIPRPOnHit;         // 0xcc (EffectChance / DurationRounds)
    char pad_0xd0[0xe0 - 0xd0];
    C2DA* m_pRepute;            // 0xe0
    char pad_0xe4[0x8];
    C2DA* m_pPlotXP;            // 0xec
    C2DA* m_pPercentXP;         // 0xf0
    char pad_0xf4[0x114 - 0xf4];
    C2DA* m_pItemValue;         // 0x114 (MAXSINGLEITEMVALUE)
};

class CSWSpell {
public:
    char pad_0x0[0x124];
    int m_n124;                                             // 0x124 (talent category)
};

class CSWSpellArray {
public:
    CSWSpell* GetSpell(int nSpell);                         // 0x0059B6D0
};

class CSWFeat {
public:
    char pad_0x0[0x10];
    int m_n10;                                              // 0x10 (talent category)
    char pad_0x14[0x28 - 0x14];
    unsigned char m_b28_0 : 4;
    unsigned char m_bValid : 1;                             // 0x28 bit 4
    char pad_0x29[0x48 - 0x29];
};

// vtable 0x00748BB4
class CSWRules {
public:
    CSWRules() {}
    virtual ~CSWRules();                                    // 0x00551440

    char pad_0x4[0x34 - 0x4];
    class CSWBaseItemArray* m_pBaseItemArray; // 0x34
    char pad_0x38[0x8c - 0x38];
    class CSWSpellArray* m_pSpellArray; // 0x8c
    class CSWFeat* m_pFeats;    // 0x90 (0x48-byte entries)
    char pad_0x94[0xa4 - 0x94];
    unsigned short m_nNumFeats; // 0xa4
    char pad_0xa6[0xaa - 0xa6];
    unsigned char m_nAA;        // 0xaa
    char pad_0xab[0xb8 - 0xab];
    CTwoDimArrays* m_p2DArrays; // 0xb8
    unsigned char m_aDifficulty[5][6]; // 0xbc
    char pad_0xda[0x120 - 0xda];

    unsigned short RollDice(unsigned char nDice, unsigned char nSides); // 0x00550B60
    unsigned char GetFlagIndex(unsigned char nFlag);         // 0x00550B90 (name guessed)
    class CSWFeat* GetFeat(unsigned short nFeat);            // 0x00550C00
    int GetDifficultyValue(int nColumn, int nRow);          // 0x00550C30 (name guessed)
    void LoadDifficultyInfo();                              // 0x00550C50
    unsigned long GetWeightedDamageAmount(unsigned long nRoll, unsigned long nMax, unsigned long bAttackerPC, unsigned long bDefenderPC, unsigned long nPercent); // 0x00550D10

    CTwoDimArrays* Get2DArrays() { return m_p2DArrays; }
    class CSWBaseItemArray* GetBaseItemArray() { return m_pBaseItemArray; }
};

extern CSWRules* g_pRules;      // 0x007A3A28

#endif
