// CExoRandInternal constructor and GetString (s5).

#include <string.h>
#include "c/exo_base.h"

union LARGE_INTEGER {
    struct {
        unsigned long LowPart;
        long HighPart;
    };
    __int64 QuadPart;
};

extern "C" __declspec(dllimport) int __stdcall QueryPerformanceCounter(LARGE_INTEGER* p);   // IAT 0x0073D1A8
extern "C" __declspec(dllimport) unsigned long __stdcall GetTickCount();                    // IAT 0x0073D0F8

#pragma pack(push, 4)
class CExoRandInternal {
public:
    unsigned long m_z[4];                           // 0x00
    unsigned long m_c[4];                           // 0x10
    unsigned long m_ks[4];                          // 0x20
    unsigned long m_k[4];                           // 0x30
    unsigned long m_q[4];                           // 0x40
    unsigned long m_s[4];                           // 0x50
    int m_bSeeded;                                  // 0x60
    unsigned char* m_pPool;                         // 0x64 (0x80-byte entropy pool)
    unsigned short m_nPoolPos;                      // 0x68
    LARGE_INTEGER m_liCounter[4];                   // 0x6c
    unsigned long m_nTickCount;                     // 0x8c

    CExoRandInternal();
    unsigned long Rand();                           // 0x005ECCA0
    unsigned long ReSeed(unsigned long s0, unsigned long s1, unsigned long s2, unsigned long s3);   // 0x005ECD40
    CExoString GetString(unsigned short nLength);
    void GenSeed();                                 // 0x005ED080
};
#pragma pack(pop)

// NONMATCH (48 diff insns): the original keeps the zero-extended length in
// edx / a spill slot across the XOR loops and spills the chunk offset; ours
// re-reads the parameter and keeps the offset in edi. An explicit int copy
// of the length or int loop counters do not reproduce it.
// FUNCTION: SWKOTOR 0x005ECEA0
CExoString CExoRandInternal::GetString(unsigned short nLength)
{
    unsigned short nChunks = 0x80 / nLength;
    unsigned short nRemainder = 0x80 % nLength;
    char* pBuffer = new char[nLength + 1];
    CExoString sResult;
    unsigned short i;
    unsigned short j;
    memcpy(pBuffer, m_pPool, nLength);
    for (i = 1; i < nChunks; i++) {
        for (j = 0; j < nLength; j++) {
            pBuffer[j] ^= m_pPool[i * nLength + j];
        }
    }
    for (j = 0; j < nRemainder; j++) {
        pBuffer[j] ^= m_pPool[nChunks * nLength + j];
    }
    for (j = 0; j < nLength; j++) {
        if (pBuffer[j] == 0) {
            pBuffer[j] = (char)(Rand() % 250 + 1);
        }
    }
    pBuffer[nLength] = 0;
    if (m_nPoolPos + nLength < 0x80) {
        memcpy(m_pPool + m_nPoolPos, pBuffer, nLength);
        m_nPoolPos += nLength;
    } else {
        memcpy(m_pPool + m_nPoolPos, pBuffer, 0x80 - m_nPoolPos);
        memcpy(m_pPool, pBuffer + 0x80 - m_nPoolPos, m_nPoolPos + nLength - 0x80);
        m_nPoolPos += nLength - 0x80;
    }
    sResult = pBuffer;
    delete[] pBuffer;
    return sResult;
}

// FUNCTION: SWKOTOR 0x005ED0E0
CExoRandInternal::CExoRandInternal()
{
    CExoString sSeed;
    m_bSeeded = 0;
    m_z[0] = 0x1745D12D;
    m_z[1] = 0x249248ED;
    m_z[2] = 0x333332F9;
    m_z[3] = 0x383EF9FA;
    m_k[0] = 31;
    m_k[1] = 29;
    m_k[2] = 28;
    m_k[3] = 25;
    m_q[0] = 6;
    m_q[1] = 2;
    m_q[2] = 13;
    m_q[3] = 3;
    m_s[0] = 20;
    m_s[1] = 12;
    m_s[2] = 7;
    m_s[3] = 15;
    m_nPoolPos = 0;
    m_pPool = new unsigned char[0x80];
    GenSeed();
    sSeed = GetString(0x10);
    unsigned long s0 = *(unsigned long*)sSeed.SubString(0, 4).CStr();
    unsigned long s1 = *(unsigned long*)sSeed.SubString(4, 4).CStr();
    unsigned long s2 = *(unsigned long*)sSeed.SubString(8, 4).CStr();
    unsigned long s3 = *(unsigned long*)sSeed.SubString(12, 4).CStr();
    ReSeed(s0, s1, s2, s3);
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    m_liCounter[0] = li;
    m_liCounter[1] = li;
    m_liCounter[2] = li;
    m_liCounter[3] = li;
    m_nTickCount = GetTickCount();
}
