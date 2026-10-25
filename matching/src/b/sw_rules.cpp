// CSWRules small helpers (0x00550B60-0x00550C50).
#include "b/sw_rules.h"
#include <stdlib.h>

// FUNCTION: SWKOTOR 0x00550B60
unsigned short CSWRules::RollDice(unsigned char nDice, unsigned char nSides)
{
    int nTotal = 0;
    for (int i = 0; i < nDice; i++) {
        nTotal += rand() % nSides + 1;
    }
    return (unsigned short)nTotal;
}

// FUNCTION: SWKOTOR 0x00550B90
unsigned char CSWRules::GetFlagIndex(unsigned char nFlag)
{
    switch (nFlag) {
    case 1:
        return 2;
    case 2:
    case 16:
    case 32:
        return 1;
    case 4:
        return 3;
    case 8:
        return 4;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00550C00
CSWFeat* CSWRules::GetFeat(unsigned short nFeat)
{
    if (nFeat < m_nNumFeats && m_pFeats[nFeat].m_bValid) {
        return &m_pFeats[nFeat];
    }
    return NULL;
}

// FUNCTION: SWKOTOR 0x00550C30
int CSWRules::GetDifficultyValue(int nColumn, int nRow)
{
    return m_aDifficulty[nRow][nColumn];
}

// FUNCTION: SWKOTOR 0x00550C50
void CSWRules::LoadDifficultyInfo()
{
    int nValue;
    C2DA* p2DA = new C2DA("DiffSettings", 0);
    p2DA->Load2DArray();
    for (int nRow = 1; nRow <= 5; nRow++) {
        for (int nColumn = 0; nColumn < 6; nColumn++) {
            m_aDifficulty[nRow - 1][nColumn] =
                p2DA->GetINTEntry(nColumn, nRow, &nValue) ? nValue : 0;
        }
    }
    if (p2DA) {
        delete p2DA;
    }
}

// FUNCTION: SWKOTOR 0x00550D10
unsigned long CSWRules::GetWeightedDamageAmount(unsigned long nRoll, unsigned long nMax, unsigned long bAttackerPC, unsigned long bDefenderPC, unsigned long nPercent)
{
    int nPercentSigned = (int)nPercent;
    int nMaxSigned = (int)nMax;
    int nRollSigned = (int)nRoll;
    if (bAttackerPC == 1) {
        if (nPercent != 0) {
            int nFloor = (int)(nMax * nPercent) / 100;
            if (nFloor == nMaxSigned) {
                return nMax;
            }
            int nSpread = rand() % (100 - nPercentSigned);
            int nWeighted = nRollSigned;
            nWeighted += (int)((nSpread + nPercentSigned) * nMax) / 100;
            if (nWeighted < nMaxSigned) {
                return (unsigned long)nWeighted;
            }
            return nMax;
        }
        goto return_roll;
    }
    if (bDefenderPC != 1 || nPercent == 0) {
        goto return_roll;
    }
    if (nPercentSigned < 100) {
        int nWeighted = nRollSigned - (int)((100 - nPercent) * nMax) / 100;
        if (nWeighted >= 1) {
            return (unsigned long)nWeighted;
        }
        return 1;
    }
    if (nPercentSigned == 100) {
        goto return_roll;
    }
    int nSpread = rand() % (nPercentSigned - 100);
    return (unsigned long)(nRollSigned + (int)(nSpread * nMax) / 100);
return_roll:
    return nRoll;
}
