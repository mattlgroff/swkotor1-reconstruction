// CSWSModule container helpers: CResHelper<CResIFO, 2014> dtor,
// NWPlayerCharacterList_st dtor and the CExoArrayList instantiations over
// CWorldJournalEntry / CSWSModuleStringEntry (0x004C4C60-0x004C8470).
#include "i/sws_module.h"

template <class T, int nType>
CResHelper<T, nType>::~CResHelper()
{
    if (m_pRes && m_cResRef != "") {
        if (m_bAutoRequest) {
            m_pRes->CancelRequest();
            m_bAutoRequest = 0;
        }
        if (!g_pExoResMan->ReleaseResObject(m_pRes)) {
            if (m_pRes) {
                delete m_pRes;
            }
            m_pRes = 0;
        }
    }
}

// FUNCTION: SWKOTOR 0x004C4C60 SYMBOL: ??1?$CResHelper@VCResIFO@@$0HNO@@@UAE@XZ
template CResHelper<CResIFO, 2014>::~CResHelper();

// FUNCTION: SWKOTOR 0x004C5DF0 SYMBOL: ??1NWPlayerCharacterList_st@@QAE@XZ
void DeletePlayerCharacterEntry(NWPlayerCharacterList_st* p)
{
    delete p;
}

// FUNCTION: SWKOTOR 0x004C5000 SYMBOL: ?Allocate@?$CExoArrayList@VCWorldJournalEntry@@@@QAEXH@Z
template void CExoArrayList<CWorldJournalEntry>::Allocate(int n);

template <>
void CExoArrayList<CWorldJournalEntry>::Add(CWorldJournalEntry t)
{
    if (num == array_size) {
        Allocate(array_size ? array_size * 2 : 16);
    }
    element[num++] = t;
}

// FUNCTION: SWKOTOR 0x004C65E0 SYMBOL: ?Add@?$CExoArrayList@VCWorldJournalEntry@@@@QAEXVCWorldJournalEntry@@@Z
void AddWorldJournalEntry(CExoArrayList<CWorldJournalEntry>* pList, const CWorldJournalEntry& e)
{
    pList->Add(e);
}

// FUNCTION: SWKOTOR 0x004C6740 SYMBOL: ?Allocate@?$CExoArrayList@VCSWSModuleStringEntry@@@@QAEXH@Z
template void CExoArrayList<CSWSModuleStringEntry>::Allocate(int n);

// FUNCTION: SWKOTOR 0x004C8470 SYMBOL: ??1?$CExoArrayList@VCSWSModuleStringEntry@@@@QAE@XZ
template CExoArrayList<CSWSModuleStringEntry>::~CExoArrayList();
