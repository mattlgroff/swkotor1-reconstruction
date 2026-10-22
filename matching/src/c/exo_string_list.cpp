// CExoStringList::Introduce (b005e7_exo_string_array_misc).

#include <stdlib.h>

class CExoString {
public:
    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04

    ~CExoString();                                  // 0x005E5C20
};

class CExoStringList {
public:
    CExoString** m_pStrings;                        // 0x00
    int m_nCount;                                   // 0x04
    int m_bSorted;                                  // 0x08
    int m_bAllowDuplicates;                         // 0x0c
    int m_nSize;                                    // 0x10

    CExoStringList(int bAllowDuplicates, int bSorted, int nSize);
    ~CExoStringList();
    void Clear();
    void Introduce(CExoString* pString, int nPosition);
};

// FUNCTION: SWKOTOR 0x005E94A0
void CExoStringList::Introduce(CExoString* pString, int nPosition)
{
    int nNewSize = m_nSize;
    if (m_nCount >= nNewSize) {
        nNewSize += 5;
    }
    if (nNewSize > m_nSize) {
        CExoString** pNew = (CExoString**)calloc(nNewSize, sizeof(CExoString*));
        for (int i = 0; i < m_nSize; i++) {
            pNew[i] = m_pStrings[i];
        }
        free(m_pStrings);
        m_pStrings = pNew;
    }
    for (int i = m_nCount; i > nPosition; i--) {
        m_pStrings[i] = m_pStrings[i - 1];
    }
    m_pStrings[nPosition] = pString;
    m_nSize = nNewSize;
    m_nCount++;
}

// FUNCTION: SWKOTOR 0x005E9550
CExoStringList::CExoStringList(int bAllowDuplicates, int bSorted, int nSize)
{
    m_pStrings = (CExoString**)calloc(nSize, sizeof(CExoString*));
    m_nSize = nSize;
    m_bSorted = bSorted;
    m_nCount = 0;
    m_bAllowDuplicates = bAllowDuplicates;
}

// FUNCTION: SWKOTOR 0x005E9590
void CExoStringList::Clear()
{
    for (int i = 0; i < m_nSize; i++) {
        if (m_pStrings[i]) {
            delete m_pStrings[i];
            m_pStrings[i] = 0;
        }
    }
    m_nCount = 0;
}

// FUNCTION: SWKOTOR 0x005E9860
CExoStringList::~CExoStringList()
{
    if (m_pStrings) {
        for (int i = 0; i < m_nCount; i++) {
            if (m_pStrings[i]) {
                delete m_pStrings[i];
            }
        }
        free(m_pStrings);
    }
}
