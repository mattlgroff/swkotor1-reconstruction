// CResRef members.
#include "d/exo_key_table.h"

extern "C" void* __cdecl memcpy(void* dst, const void* src, unsigned int n);
extern "C" void* __cdecl memset(void* dst, int c, unsigned int n);
extern "C" int __cdecl memcmp(const void* a, const void* b, unsigned int n);
#pragma intrinsic(memcpy, memset, memcmp)

inline CResRef::CResRef(const CResRef& cResRef)
{
    memcpy(m_resRef, cResRef.m_resRef, 16);
}

// FUNCTION: SWKOTOR 0x00405f70 SYMBOL: ?CopyToString@CResRef@@QBEXAAVCExoString@@@Z
void CResRef::CopyToString(CExoString& sStr) const
{
    char szResRef[17];
    memcpy(szResRef, m_resRef, 16);
    szResRef[16] = 0;
    sStr = szResRef;
}

// FUNCTION: SWKOTOR 0x00406170 SYMBOL: ??4CResRef@@QAE?AV0@AAY0BA@$$CBD@Z
CResRef CResRef::operator=(const char (&pResRef)[16])
{
    memcpy(m_resRef, pResRef, 16);
    for (unsigned int i = 0; i < 16; i++) {
        if (m_resRef[i] == 0) {
            memset(&m_resRef[i + 1], 0, 15 - i);
            break;
        }
        m_resRef[i] = (char)tolower((unsigned char)m_resRef[i]);
    }
    return *this;
}

// FUNCTION: SWKOTOR 0x004061f0 SYMBOL: ??4CResRef@@QAE?AV0@ABVCExoString@@@Z
CResRef CResRef::operator=(const CExoString& sResRef)
{
    unsigned int nLength = sResRef.GetLength();
    memset(m_resRef, 0, 16);
    if (nLength > 0) {
        char* pStr = sResRef.CStr();
        memcpy(m_resRef, pStr, nLength > 16 ? 16 : nLength);
        for (unsigned int i = 0; i < (nLength > 16 ? 16 : nLength); i++) {
            m_resRef[i] = (char)tolower((unsigned char)m_resRef[i]);
        }
    }
    return *this;
}

// FUNCTION: SWKOTOR 0x00406300 SYMBOL: ??YCResRef@@QAE?AV0@ABVCExoString@@@Z
CResRef CResRef::operator+=(const CExoString& sString)
{
    CExoString sResRef;
    char szResRef[17];
    memcpy(szResRef, m_resRef, 16);
    szResRef[16] = 0;
    sResRef = szResRef;
    sResRef = sResRef + sString;
    sResRef = sResRef.LowerCase();
    memset(m_resRef, 0, 16);
    if (sResRef.GetLength() > 0) {
        unsigned int nLength;
        if ((unsigned int)sResRef.GetLength() > 16) {
            nLength = 16;
        } else {
            nLength = sResRef.GetLength();
        }
        memcpy(m_resRef, sResRef.CStr(), nLength);
    }
    return *this;
}

// FUNCTION: SWKOTOR 0x00406d60 SYMBOL: ??0CResRef@@QAE@ABVCExoString@@@Z
CResRef::CResRef(const CExoString& sResRef)
{
    *this = sResRef;
}

// FUNCTION: SWKOTOR 0x00406de0 SYMBOL: ??9CResRef@@QBEHABVCExoString@@@Z
int CResRef::operator!=(const CExoString& sResRef) const
{
    sResRef.IsEmpty();
    char* pStr = sResRef.CStr();
    unsigned int nLength = sResRef.GetLength() + 1;
    if (nLength > 16) {
        nLength = 16;
    }
    return memcmp(pStr, m_resRef, nLength) != 0;
}

// FUNCTION: SWKOTOR 0x00405ed0 SYMBOL: ??0CResRef@@QAE@XZ
CResRef::CResRef()
{
    memset(m_resRef, 0, 16);
}

// FUNCTION: SWKOTOR 0x00405ef0 SYMBOL: ??0CResRef@@QAE@PBDE@Z
CResRef::CResRef(const char* pResRef, unsigned char nLength)
{
    if (pResRef) {
        int nCopy = nLength < 16 ? nLength : 16;
        unsigned char nPad = 16 - nCopy;
        memset(&m_resRef[16 - nPad], 0, nPad);
        memcpy(m_resRef, pResRef, 16 - nPad);
    } else {
        memset(m_resRef, 0, 16);
    }
}

// FUNCTION: SWKOTOR 0x00405fb0 SYMBOL: ?CopyToString@CResRef@@QBEXPAD@Z
void CResRef::CopyToString(char* pStr) const
{
    memcpy(pStr, m_resRef, 16);
    pStr[16] = 0;
}

char g_aResRefStr[4][17];    // 0x007a3d00
int g_nResRefStr;            // 0x007a3d48

// FUNCTION: SWKOTOR 0x00405fe0
char* CResRef::GetResRefStr() const
{
    g_nResRefStr = (g_nResRefStr + 1) % 4;
    char* pStr = g_aResRefStr[g_nResRefStr];
    memcpy(pStr, m_resRef, 16);
    g_aResRefStr[g_nResRefStr][16] = 0;
    return pStr;
}

// FUNCTION: SWKOTOR 0x00406030
int CResRef::IsValid() const
{
    return m_resRef[0] != 0;
}

// FUNCTION: SWKOTOR 0x00406040
unsigned char CResRef::GetLength() const
{
    unsigned char i;
    for (i = 0; i < 16; i++) {
        if (m_resRef[i] == 0) {
            break;
        }
    }
    return i;
}

// FUNCTION: SWKOTOR 0x00406060 SYMBOL: ??9CResRef@@QBEHPBD@Z
int CResRef::operator!=(const char* szResRef) const
{
    for (unsigned int i = 0; i < 16; i++) {
        int c = tolower(szResRef[i]);
        if (c != tolower((unsigned char)m_resRef[i])) {
            return 1;
        }
        if (c == 0) {
            break;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004060b0 SYMBOL: ??8CResRef@@QBEHABV0@@Z
int CResRef::operator==(const CResRef& cResRef) const
{
    return memcmp(cResRef.m_resRef, m_resRef, 16) == 0;
}

// FUNCTION: SWKOTOR 0x004060d0 SYMBOL: ??8CResRef@@QBEHPBD@Z
int CResRef::operator==(const char* szResRef) const
{
    for (unsigned int i = 0; i < 16; i++) {
        int c = tolower(szResRef[i]);
        if (c != tolower((unsigned char)m_resRef[i])) {
            return 0;
        }
        if (c == 0) {
            break;
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00406120 SYMBOL: ??4CResRef@@QAE?AV0@ABV0@@Z
CResRef CResRef::operator=(const CResRef& cResRef)
{
    for (unsigned int i = 0; i < 16; i++) {
        m_resRef[i] = (char)tolower((unsigned char)cResRef.m_resRef[i]);
    }
    return *this;
}

// FUNCTION: SWKOTOR 0x00406290 SYMBOL: ??4CResRef@@QAE?AV0@PBD@Z
CResRef CResRef::operator=(const char* szResRef)
{
    memset(m_resRef, 0, 16);
    if (szResRef) {
        for (unsigned int i = 0; i < 16; i++) {
            if (szResRef[i] == 0) {
                break;
            }
            m_resRef[i] = (char)tolower(szResRef[i]);
        }
    }
    return *this;
}

// FUNCTION: SWKOTOR 0x00406d80 SYMBOL: ??0CResRef@@QAE@PBD@Z
CResRef::CResRef(const char* szResRef)
{
    *this = szResRef;
}

// FUNCTION: SWKOTOR 0x00406dc0 SYMBOL: ??9CResRef@@QBEHABV0@@Z
int CResRef::operator!=(const CResRef& cResRef) const
{
    return memcmp(cResRef.m_resRef, m_resRef, 16) != 0;
}
