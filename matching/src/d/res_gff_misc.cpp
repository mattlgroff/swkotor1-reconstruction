// CResGFF struct accessors (0x004111c0-0x00411240) and the CRC-32
// table helpers (0x00410520 / 0x00410560, used by CTlkTable).
#include "d/res_gff.h"

extern unsigned long g_aCRCTable[256];          // 0x007a4178

#pragma auto_inline(off)

// FUNCTION: SWKOTOR 0x00410520
void InitializeCRCTable()
{
    for (unsigned long i = 0; i <= 0xff; i++) {
        unsigned long c = i;
        for (int k = 8; k; k--) {
            if (c & 1) {
                c = (c >> 1) ^ 0xedb88320;
            } else {
                c >>= 1;
            }
        }
        g_aCRCTable[i] = c;
    }
}

// FUNCTION: SWKOTOR 0x00410560
unsigned long ComputeCRC(const char* pData, unsigned long nLength)
{
    unsigned long nCRC = 0;
    for (unsigned long i = 0; i < nLength && pData[i]; i++) {
        nCRC = g_aCRCTable[(pData[i] ^ nCRC) & 0xff] ^ (nCRC >> 8);
    }
    return nCRC;
}

// FUNCTION: SWKOTOR 0x004111c0
unsigned long CResGFF::GetStructID(CResStruct* pStruct)
{
    CResGFFStruct* pGFFStruct = GetStruct(pStruct);
    if (!pGFFStruct) {
        return 0xffffffff;
    }
    return pGFFStruct->m_nType;
}

// FUNCTION: SWKOTOR 0x00411200
unsigned long CResGFF::GetElementCount(CResStruct* pStruct)
{
    CResGFFStruct* pGFFStruct = GetStruct(pStruct);
    if (!pGFFStruct) {
        return 0xffffffff;
    }
    return pGFFStruct->m_nFieldCount;
}

// FUNCTION: SWKOTOR 0x00411240
int CResGFF::GetTopLevelStruct(CResStruct* pStruct)
{
    if (!pStruct) {
        return 0;
    }
    pStruct->m_nStructIndex = 0;
    return 1;
}
