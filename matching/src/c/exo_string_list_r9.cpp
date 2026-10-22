// CExoStringList default constructor and Merge (b005e7, r9).

#include <stdlib.h>
#include <string.h>

extern "C" __declspec(dllimport) int __stdcall MessageBoxA(void* hWnd, const char* sText, const char* sCaption, unsigned int nType);   // IAT 0x0073D418

class CExoString {
public:
    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04

    CExoString(const char* s);                      // 0x005E5A90
    ~CExoString();                                  // 0x005E5C20
    char* CStr() const;                             // 0x005E5670
};

class CExoStringList {
public:
    CExoString** m_pStrings;                        // 0x00
    int m_nCount;                                   // 0x04
    int m_bSorted;                                  // 0x08
    int m_bAllowDuplicates;                         // 0x0c
    int m_nSize;                                    // 0x10

    CExoStringList();
    CExoStringList(int bAllowDuplicates, int bSorted, int nSize);
    void Add(CExoString* pString);                  // 0x005E98B0
    CExoStringList* Merge(CExoStringList* pOther, int bAllowDuplicates, int bSorted);
    void Introduce(CExoString* pString, int nPosition);   // 0x005E94A0
    void AddSorted(CExoString* pString);
};

// FUNCTION: SWKOTOR 0x005E9530 SYMBOL: ??0CExoStringList@@QAE@XZ
CExoStringList::CExoStringList()
{
    m_pStrings = 0;
    m_nCount = 0;
    m_bSorted = 0;
    m_bAllowDuplicates = 0;
    m_nSize = 0;
}

// Matched as 0x005E9550 in exo_string_list.cpp; defined here so it inlines.
CExoStringList::CExoStringList(int bAllowDuplicates, int bSorted, int nSize)
{
    m_pStrings = (CExoString**)calloc(nSize, sizeof(CExoString*));
    m_nCount = 0;
    m_bSorted = bSorted;
    m_bAllowDuplicates = bAllowDuplicates;
    m_nSize = nSize;
}

// FUNCTION: SWKOTOR 0x005E9980
CExoStringList* CExoStringList::Merge(CExoStringList* pOther, int bAllowDuplicates, int bSorted)
{
    CExoStringList* pNew = new CExoStringList(bAllowDuplicates, bSorted, m_nSize);
    for (int i = 0; i < m_nCount; i++) {
        pNew->Add(new CExoString(m_pStrings[i]->CStr()));
    }
    if (pOther) {
        for (int j = 0; j < pOther->m_nCount; j++) {
            pNew->Add(new CExoString(pOther->m_pStrings[j]->CStr()));
        }
    }
    return pNew;
}

// Binary-search insert; m_bSorted (+0x08) is the duplicate mode here
// (0 = insert duplicates, 2 = report them).
// FUNCTION: SWKOTOR 0x005E95F0
void CExoStringList::AddSorted(CExoString* pString)
{
    // NONMATCH (146 diff insns): register allocation only; the original keeps
    // this in ebx, nDir/nMid in edi/ebp and reloads pString from the stack,
    // ours keeps pString in edi and a zero in ebx.
    int bDone = 0;
    int nHigh = 0;
    int nLow = 0;
    int nCompare = 0;
    int nDir;
    int nMid;
    if (m_nCount > 0) {
        nCompare = strcmp(pString->CStr(), m_pStrings[0]->CStr());
        if (nCompare < 0) {
            Introduce(pString, 0);
            bDone = 1;
        } else if (nCompare == 0) {
            if (m_bSorted == 0) {
                Introduce(pString, 0);
            } else if (m_bSorted == 2) {
                MessageBoxA(0, "Duplicate string found.", "Error: ExoStringList", 1);
            }
            bDone = 1;
        }
    }
    if (!bDone && m_nCount > 0) {
        nHigh = m_nCount - 1;
        nCompare = strcmp(pString->CStr(), m_pStrings[m_nCount - 1]->CStr());
        if (nCompare > 0) {
            Introduce(pString, m_nCount);
            bDone = 1;
        } else if (nCompare == 0) {
            if (m_bSorted == 0) {
                Introduce(pString, m_nCount);
            } else if (m_bSorted == 2) {
                MessageBoxA(0, "Duplicate string found.", "Error: ExoStringList", 1);
            }
            bDone = 1;
        }
    }
    nMid = nDir = m_nCount / 2;
    while (!bDone) {
        if (nHigh - nLow <= 1) {
            break;
        }
        nCompare = strcmp(pString->CStr(), m_pStrings[nMid]->CStr());
        if (nCompare < 0) {
            nHigh = nMid;
            nDir = -1;
        } else if (nCompare > 0) {
            nLow = nMid;
            nDir = 1;
        } else {
            if (m_bSorted == 0) {
                Introduce(pString, nMid);
            } else if (m_bSorted == 2) {
                MessageBoxA(0, "Duplicate string found.", "Error: ExoStringList", 1);
            }
            bDone = 1;
        }
        nMid += nDir * ((nHigh - nLow) / 2);
    }
    if (!bDone) {
        if (nCompare > 0) {
            Introduce(pString, nMid + 1);
        } else {
            Introduce(pString, nMid);
        }
    }
}
