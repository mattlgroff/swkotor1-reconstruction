// CExoString core members (b005e7_exo_string_array_misc), worker C r8.

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "c/exo_base.h"

extern char g_sExoEmptyString[];                    // 0x008328A4 "" (.data)
extern char* g_pExoStringFormatBuffer;              // 0x008328A8
extern unsigned long g_nExoStringFormatBufferSize;  // 0x008328AC

CExoString::CExoString()
{
    m_sString = 0;
    m_nBufferLength = 0;
}

// FUNCTION: SWKOTOR 0x005E5140 SYMBOL: ??4CExoString@@QAEAAV0@PBD@Z
CExoString& CExoString::operator=(const char* s)
{
    if (m_sString && (s == 0 || strlen(s) + 1 > m_nBufferLength)) {
        m_nBufferLength = 0;
        delete[] m_sString;
        m_sString = 0;
    }
    if (s && (int)strlen(s) > 0) {
        if (m_sString == 0) {
            m_nBufferLength = strlen(s) + 1;
            m_sString = new char[m_nBufferLength];
        }
        strcpy(m_sString, s);
    } else if (m_sString) {
        delete[] m_sString;
        m_sString = 0;
        m_nBufferLength = 0;
    }
    return *this;
}

// FUNCTION: SWKOTOR 0x005E5200 SYMBOL: ??8CExoString@@QBEHABV0@@Z
int CExoString::operator==(const CExoString& s) const
{
    if (m_sString && s.m_sString) {
        return strcmp(m_sString, s.m_sString) == 0;
    }
    if (m_sString == 0 && s.m_sString == 0) {
        return 1;
    }
    if (m_sString && *m_sString == 0) {
        return 1;
    }
    if (s.m_sString && *s.m_sString == 0) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005E5290 SYMBOL: ??8CExoString@@QBEHPBD@Z
int CExoString::operator==(const char* s) const
{
    if (m_sString && s) {
        return strcmp(m_sString, s) == 0;
    }
    if (m_sString == 0 && s == 0) {
        return 1;
    }
    if (m_sString && *m_sString == 0) {
        return 1;
    }
    if (s && *s == 0) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005E5310 SYMBOL: ??9CExoString@@QBEHABV0@@Z
int CExoString::operator!=(const CExoString& s) const
{
    if (m_sString && s.m_sString) {
        return strcmp(m_sString, s.m_sString) != 0;
    }
    if ((m_sString && *m_sString) || (s.m_sString && *s.m_sString)) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005E5390 SYMBOL: ??9CExoString@@QBEHPBD@Z
int CExoString::operator!=(const char* s) const
{
    if (m_sString && s) {
        return strcmp(m_sString, s) != 0;
    }
    if ((m_sString && *m_sString) || (s && *s)) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005E5410
int CExoString::operator<(const CExoString& s) const
{
    if (m_sString && s.m_sString) {
        return strcmp(m_sString, s.m_sString) < 0;
    }
    if (m_sString == 0 && s.m_sString && *s.m_sString) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005E5490
int CExoString::operator>(const CExoString& s) const
{
    if (m_sString && s.m_sString) {
        return strcmp(m_sString, s.m_sString) > 0;
    }
    if ((m_sString && *m_sString) && (s.m_sString == 0 || *s.m_sString == 0)) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005E5500
int CExoString::operator<=(const CExoString& s) const
{
    if (m_sString && s.m_sString) {
        return strcmp(m_sString, s.m_sString) <= 0;
    }
    if (!((m_sString == 0 && s.m_sString == 0) || (m_sString && *m_sString == 0) || s.m_sString)) {
        return 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x005E5580
int CExoString::operator>=(const CExoString& s) const
{
    if (m_sString && s.m_sString) {
        return strcmp(m_sString, s.m_sString) >= 0;
    }
    if (!((m_sString == 0 && s.m_sString == 0) || (s.m_sString && *s.m_sString == 0) || m_sString)) {
        return 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x005E5600
char CExoString::operator[](int n) const
{
    if (n >= 0 && m_sString && strlen(m_sString) > (unsigned int)n) {
        return m_sString[n];
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005E5650
float CExoString::AsFLOAT() const
{
    if (m_sString) {
        return (float)atof(m_sString);
    }
    return 0.0f;
}

// FUNCTION: SWKOTOR 0x005E5670
char* CExoString::CStr() const
{
    if (m_sString) {
        return m_sString;
    }
    return g_sExoEmptyString;
}

// FUNCTION: SWKOTOR 0x005E5680
void CExoString::Format(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    if (g_pExoStringFormatBuffer == 0 && g_nExoStringFormatBufferSize == 0) {
        g_nExoStringFormatBufferSize = 0x400;
        g_pExoStringFormatBuffer = new char[0x400];
    }
    int n = _vsnprintf(g_pExoStringFormatBuffer, g_nExoStringFormatBufferSize, format, args);
    if (n < -1) {
        return;
    }
    while (n < 0) {
        g_nExoStringFormatBufferSize += 0x400;
        if (g_pExoStringFormatBuffer) {
            delete[] g_pExoStringFormatBuffer;
        }
        g_pExoStringFormatBuffer = new char[g_nExoStringFormatBufferSize];
        n = _vsnprintf(g_pExoStringFormatBuffer, g_nExoStringFormatBufferSize, format, args);
    }
    if (m_sString && (unsigned long)(n + 1) > m_nBufferLength) {
        m_nBufferLength = 0;
        delete[] m_sString;
        m_sString = 0;
    }
    if (m_sString == 0) {
        m_nBufferLength = n + 1;
        m_sString = new char[n + 1];
    }
    strncpy(m_sString, g_pExoStringFormatBuffer, n);
    m_sString[n] = 0;
}

// FUNCTION: SWKOTOR 0x005E5790
int CExoString::GetLength() const
{
    if (m_sString) {
        return strlen(m_sString);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005E58A0
int CExoString::IsEmpty() const
{
    if (m_sString == 0 || *m_sString == 0) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005E5A90 SYMBOL: ??0CExoString@@QAE@PBD@Z
CExoString::CExoString(const char* s)
{
    if (s && strlen(s)) {
        m_nBufferLength = strlen(s) + 1;
        m_sString = new char[m_nBufferLength];
        strcpy(m_sString, s);
    } else {
        m_sString = 0;
        m_nBufferLength = 0;
    }
}

// FUNCTION: SWKOTOR 0x005E5B00 SYMBOL: ??0CExoString@@QAE@ABV0@@Z
CExoString::CExoString(const CExoString& s)
{
    if (s.m_sString && strlen(s.m_sString)) {
        m_nBufferLength = strlen(s.m_sString) + 1;
        m_sString = new char[m_nBufferLength];
        strcpy(m_sString, s.m_sString);
    } else {
        m_sString = 0;
        m_nBufferLength = 0;
    }
}

// FUNCTION: SWKOTOR 0x005E5B70 SYMBOL: ??0CExoString@@QAE@PBDH@Z
CExoString::CExoString(const char* s, int nLength)
{
    if (nLength > 0) {
        m_nBufferLength = nLength + 1;
        m_sString = new char[nLength + 1];
        strncpy(m_sString, s, nLength);
        m_sString[nLength] = 0;
    } else {
        m_sString = 0;
        m_nBufferLength = 0;
    }
}

// FUNCTION: SWKOTOR 0x005E5C20
CExoString::~CExoString()
{
    if (m_sString) {
        m_nBufferLength = 0;
        delete[] m_sString;
        m_sString = 0;
    }
}

// FUNCTION: SWKOTOR 0x005E5C50 SYMBOL: ??4CExoString@@QAEAAV0@ABV0@@Z
CExoString& CExoString::operator=(const CExoString& s)
{
    if (this != &s) {
        if (m_sString && (s.m_sString == 0 || strlen(s.m_sString) + 1 > m_nBufferLength)) {
            m_nBufferLength = 0;
            delete[] m_sString;
            m_sString = 0;
        }
        int nLength;
        if (s.m_sString && (nLength = strlen(s.m_sString)) > 0) {
            if (m_sString == 0) {
                m_nBufferLength = nLength + 1;
                m_sString = new char[m_nBufferLength];
            }
            strcpy(m_sString, s.m_sString);
        } else if (m_sString) {
            delete[] m_sString;
            m_sString = 0;
            m_nBufferLength = 0;
        }
    }
    return *this;
}

// FUNCTION: SWKOTOR 0x005E5D10
CExoString CExoString::operator+(const CExoString& s) const
{
    CExoString sResult;
    int nOther = s.m_sString ? strlen(s.m_sString) : 0;
    int nThis = m_sString ? strlen(m_sString) : 0;
    if (nOther == 0) {
        if (nThis == 0) {
            return sResult;
        }
        sResult.m_nBufferLength = nThis + 1;
        sResult.m_sString = new char[nThis + 1];
        strcpy(sResult.m_sString, m_sString);
        return sResult;
    }
    if (nThis == 0) {
        sResult.m_nBufferLength = nOther + 1;
        sResult.m_sString = new char[nOther + 1];
        strcpy(sResult.m_sString, s.m_sString);
        return sResult;
    }
    sResult.m_nBufferLength = nThis + nOther + 1;
    sResult.m_sString = new char[nThis + nOther + 1];
    strcpy(sResult.m_sString, m_sString);
    strcat(sResult.m_sString, s.m_sString);
    return sResult;
}

// FUNCTION: SWKOTOR 0x005E5EB0 SYMBOL: ?Find@CExoString@@QBEHABV1@H@Z
int CExoString::Find(const CExoString& s, int nStart) const
{
    if (m_sString == 0 || s.m_sString == 0 || nStart < 0) {
        return -1;
    }
    char* p = m_sString;
    for (int i = 0; i < nStart; i++) {
        if (*p == 0) {
            return -1;
        }
        p++;
    }
    int j = 0;
    while (p[j] != 0) {
        if (s.m_sString[j] == 0) {
            return p - m_sString;
        }
        if (p[j] != s.m_sString[j]) {
            p++;
            j = 0;
        } else {
            j++;
        }
    }
    if (s.m_sString[j] == 0) {
        return p - m_sString;
    }
    return -1;
}

// FUNCTION: SWKOTOR 0x005E5F20 SYMBOL: ?Find@CExoString@@QBEHDH@Z
int CExoString::Find(char c, int nStart) const
{
    char* p = m_sString;
    int n = nStart;
    if (p == 0 || nStart < 0) {
        return -1;
    }
    for (int i = 0; i < nStart; i++) {
        if (*p == 0) {
            return -1;
        }
        p++;
    }
    while (*p) {
        if (*p == c) {
            return n;
        }
        n++;
        p++;
    }
    return -1;
}

// FUNCTION: SWKOTOR 0x005E5F70
CExoString CExoString::Left(int nCount) const
{
    CExoString sResult;
    if (m_sString == 0) {
        return sResult;
    }
    unsigned int nLength = strlen(m_sString);
    if (nCount < 0) {
        return sResult;
    }
    if (nLength == 0) {
        return sResult;
    }
    unsigned int nSize = nCount;
    if (nSize > nLength) {
        nSize = nLength;
    }
    sResult.m_nBufferLength = nSize + 1;
    sResult.m_sString = new char[nSize + 1];
    strncpy(sResult.m_sString, m_sString, nSize);
    sResult.m_sString[nSize] = 0;
    return sResult;
}

// FUNCTION: SWKOTOR 0x005E6080
CExoString CExoString::LowerCase() const
{
    CExoString sResult;
    if (m_sString == 0) {
        return sResult;
    }
    sResult.m_nBufferLength = strlen(m_sString) + 1;
    sResult.m_sString = new char[sResult.m_nBufferLength];
    int i;
    for (i = 0; m_sString[i] != 0; i++) {
        if (m_sString[i] >= 'A' && m_sString[i] <= 'Z') {
            sResult.m_sString[i] = m_sString[i] + 0x20;
        } else {
            sResult.m_sString[i] = m_sString[i];
        }
    }
    sResult.m_sString[i] = 0;
    return sResult;
}

// FUNCTION: SWKOTOR 0x005E6170
CExoString CExoString::Right(int nCount) const
{
    CExoString sResult;
    if (m_sString == 0) {
        return sResult;
    }
    unsigned int nLength = strlen(m_sString);
    if (nCount < 0) {
        return sResult;
    }
    if (nLength == 0) {
        return sResult;
    }
    unsigned int nSize = nCount;
    if (nSize > nLength) {
        nSize = nLength;
    }
    sResult.m_nBufferLength = nSize + 1;
    sResult.m_sString = new char[nSize + 1];
    strncpy(sResult.m_sString, m_sString + nLength - nSize, nSize);
    sResult.m_sString[nSize] = 0;
    return sResult;
}

// FUNCTION: SWKOTOR 0x005E6270
CExoString CExoString::SubString(int nStart, int nCount) const
{
    CExoString sResult;
    if (m_sString == 0) {
        return sResult;
    }
    unsigned int nLength = strlen(m_sString);
    if (nStart < 0 || (unsigned int)nStart >= nLength || nCount == 0) {
        return sResult;
    }
    if (nCount < 0) {
        nCount = nLength - nStart;
    }
    sResult.m_nBufferLength = nCount + 1;
    sResult.m_sString = new char[nCount + 1];
    strncpy(sResult.m_sString, m_sString + nStart, nCount);
    sResult.m_sString[nCount] = 0;
    return sResult;
}

// FUNCTION: SWKOTOR 0x005E6360
CExoString CExoString::UpperCase() const
{
    CExoString sResult;
    if (m_sString == 0) {
        return sResult;
    }
    sResult.m_nBufferLength = strlen(m_sString) + 1;
    sResult.m_sString = new char[sResult.m_nBufferLength];
    int i;
    for (i = 0; m_sString[i] != 0; i++) {
        if (m_sString[i] >= 'a' && m_sString[i] <= 'z') {
            sResult.m_sString[i] = m_sString[i] - 0x20;
        } else {
            sResult.m_sString[i] = m_sString[i];
        }
    }
    sResult.m_sString[i] = 0;
    return sResult;
}

// FUNCTION: SWKOTOR 0x005E6450
// NONMATCH (14 diff insns): esi/edi swapped (s / this) and the original
// loads this->m_sString before the s == 0 branch (shared by both paths).
int CExoString::CompareNoCase(const CExoString& s) const
{
    int nLength = s.m_sString ? strlen(s.m_sString) : 0;
    if (s.m_sString == 0 && m_sString == 0) {
        return 1;
    }
    if (s.m_sString == 0 || m_sString == 0) {
        return 0;
    }
    if (nLength != (int)strlen(m_sString)) {
        return 0;
    }
    const char* p1 = s.m_sString;
    const char* p2 = m_sString;
    for (int i = 0; i < nLength; i++, p1++, p2++) {
        char a = *p1;
        char b = *p2;
        if (a != b) {
            if (a >= 'A' && a <= 'Z') {
                if (a + 0x20 != b) {
                    return 0;
                }
            } else if (b >= 'A' && b <= 'Z') {
                if (b + 0x20 != a) {
                    return 0;
                }
            } else {
                return 0;
            }
        }
    }
    return 1;
}
