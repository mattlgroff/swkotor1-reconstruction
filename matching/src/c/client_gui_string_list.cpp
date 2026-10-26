// CClientExoAppInternal GUI string cache (worker C s1): a sorted
// CExoArrayList<CGuiStringStruct> at +0x358 keyed by TLK strref.

#include "c/client_exo_app.h"

class CTlkTable {
public:
    CExoString GetSimpleString(unsigned long nStrRef);  // 0x0041E8F0
    void ParseTokens(CExoString* pString);              // 0x0041DD30
};

extern CTlkTable* g_pTlkTable;                      // 0x007A3A08

template <class T>
void CExoArrayList<T>::Allocate(int n)
{
    T* pOld = element;
    array_size = n;
    element = new T[n];
    for (int i = 0; i < num; i++) {
        element[i] = pOld[i];
    }
    if (pOld) {
        delete[] pOld;
    }
}

template <class T>
void CExoArrayList<T>::Insert(T t, int pos)
{
    int i = num;
    Add(t);
    for (; i > pos; i--) {
        element[i] = element[i - 1];
    }
    element[pos] = t;
}

template void CExoArrayList<CGuiStringStruct>::Allocate(int n);
template void CExoArrayList<CGuiStringStruct>::Add(CGuiStringStruct t);
template void CExoArrayList<CGuiStringStruct>::Insert(CGuiStringStruct t, int pos);

// FUNCTION: SWKOTOR 0x005F5020 SYMBOL: ?Allocate@?$CExoArrayList@UCGuiStringStruct@@@@QAEXH@Z

// FUNCTION: SWKOTOR 0x005F50F0 SYMBOL: ?Add@?$CExoArrayList@UCGuiStringStruct@@@@QAEXUCGuiStringStruct@@@Z

// FUNCTION: SWKOTOR 0x005F7D00 SYMBOL: ?Insert@?$CExoArrayList@UCGuiStringStruct@@@@QAEXUCGuiStringStruct@@H@Z

// FUNCTION: SWKOTOR 0x005FB540
CExoString CClientExoAppInternal::GetGUIString(int nId)
{
    // NONMATCH (14 insns): ebp/edi swapped for the search bounds.
    int bFound = 0;
    int nHigh = m_lstGuiStrings.num - 1;
    int nMiddle = 0;
    int nLow = 0;
    CExoString sResult;

    while (nLow < nHigh && !bFound) {
        nMiddle = (nHigh - nLow) / 2 + nLow;
        if (m_lstGuiStrings.element[nMiddle].m_nStrRef == (unsigned long)nId) {
            sResult = m_lstGuiStrings.element[nMiddle].m_sString;
            bFound = 1;
        } else if (m_lstGuiStrings.element[nMiddle].m_nStrRef < (unsigned long)nId) {
            nHigh = nMiddle;
        } else {
            nLow = nMiddle + 1;
        }
    }

    if (!bFound) {
        CGuiStringStruct cEntry;
        sResult = g_pTlkTable->GetSimpleString(nId);
        cEntry.m_nStrRef = nId;
        cEntry.m_sString = sResult;
        if (m_lstGuiStrings.num > 0 && m_lstGuiStrings.element[nMiddle].m_nStrRef < (unsigned long)nId) {
            nMiddle++;
        }
        m_lstGuiStrings.Insert(cEntry, nMiddle);
    }

    g_pTlkTable->ParseTokens(&sResult);
    return sResult;
}

