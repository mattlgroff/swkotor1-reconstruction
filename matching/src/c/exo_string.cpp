// CExoString leaves (b005e7_exo_string_array_misc).

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "c/exo_base.h"

extern char g_sExoEmptyString[];                    // 0x008328A4 "" (.data)

// FUNCTION: SWKOTOR 0x005E57B0
void CExoString::Insert(const CExoString& s, int nPosition)
{
    if (s.m_sString == 0) {
        return;
    }
    unsigned int nInsert = strlen(s.m_sString);
    unsigned int nLength = m_sString ? strlen(m_sString) : 0;
    if (nInsert == 0 || nPosition < 0 || nLength <= (unsigned int)nPosition) {
        return;
    }
    char* sNew = new char[nLength + nInsert + 1];
    sNew[0] = 0;
    char* sOld = m_sString;
    m_sString = sNew;
    if (sOld) {
        strncpy(sNew, sOld, nPosition);
    }
    m_sString[nPosition] = 0;
    strcat(m_sString, s.m_sString);
    strcat(m_sString, sOld + nPosition);
    if (sOld) {
        delete[] sOld;
    }
}

// FUNCTION: SWKOTOR 0x005E58C0
int CExoString::CompareNoCase(const char* s) const
{
    const char* sOwn = m_sString;
    if (s == 0 && sOwn == 0) {
        return 1;
    }
    if (s == 0 || sOwn == 0) {
        return 0;
    }
    return _stricmp(s, sOwn) == 0;
}

// FUNCTION: SWKOTOR 0x005E5900
int CExoString::ComparePrefixNoCase(const CExoString& s, int nSize) const
{
    const char* p = s.m_sString ? s.m_sString : g_sExoEmptyString;
    const char* sOwn = m_sString;
    if (p == 0 && sOwn == 0) {
        return 1;
    }
    if (p == 0 || sOwn == 0) {
        return 0;
    }
    return _strnicmp(p, sOwn, nSize) == 0;
}

// FUNCTION: SWKOTOR 0x005E5950
int CExoString::StripNonAlphaNumeric(int bLower, int bAllowEmail, int bAllowPunctuation)
{
    int bRemoved = 0;
    if (m_sString == 0) {
        return 0;
    }
    unsigned int nLength = strlen(m_sString);
    if (nLength == 0) {
        return 0;
    }
    char sAllowed[128];
    if (bLower) {
        strcpy(sAllowed, "abcdefghijklmnopqrstuvwxyz0123456789'"
                         "\xdf\xe9\xc9\xe8\xc8\xc7\xe7\xf9\xd9\xf1\xd1\xe0\xc0\xc4\xe4\xdc\xfc\xd6\xf6_-");
    } else {
        strcpy(sAllowed, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    }
    if (bAllowEmail) {
        strcat(sAllowed, "@.");
    }
    if (bAllowPunctuation) {
        strcat(sAllowed, ".',\xdf\xe9\xc9\xe8\xc8\xc7\xe7\xf9\xd9\xf1\xd1\xe0\xc0\xc4\xe4\xdc\xfc\xd6\xf6_- ");
    }
    for (unsigned int i = 0; i < nLength; i++) {
        if (bLower) {
            char c = m_sString[i];
            if (c >= 'A' && c <= 'Z') {
                m_sString[i] = c + 0x20;
            }
        }
        if (strchr(sAllowed, m_sString[i]) == 0) {
            for (unsigned int j = i; j < nLength; j++) {
                m_sString[j] = m_sString[j + 1];
            }
            i--;
            nLength--;
            bRemoved = 1;
        }
    }
    return bRemoved;
}

// FUNCTION: SWKOTOR 0x005E5BC0
CExoString::CExoString(int n)
{
    char sBuffer[36];
    sprintf(sBuffer, "%i", n);
    m_nBufferLength = strlen(sBuffer) + 1;
    m_sString = new char[m_nBufferLength];
    strcpy(m_sString, sBuffer);
}

// FUNCTION: SWKOTOR 0x005E5630
int CExoString::AsINT() const
{
    if (m_sString) {
        return atol(m_sString);
    }
    return 0;
}
