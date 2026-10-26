// CSWCWorldJournal: sorted world journal entries (nwcjournal.o).
#include "m/sw_c_journal.h"

// FUNCTION: SWKOTOR 0x00676370 SYMBOL: ?SortItemsByDate@CSWCWorldJournal@@QAEHHHH@Z
int CSWCWorldJournal::SortItemsByDate(int a, int b, int bNoName)
{
    int nResult = g_pAppManager->GetClientExoApp()->GetWorldTimer()->CompareWorldTimes(
        m_lstEntries.element[a].m_nCalendarDay, m_lstEntries.element[a].m_nTimeOfDay,
        m_lstEntries.element[b].m_nCalendarDay, m_lstEntries.element[b].m_nTimeOfDay);
    if (nResult == 0 && !bNoName) {
        return SortItemsByName(a, b, 1);
    }
    return nResult;
}

// FUNCTION: SWKOTOR 0x006763D0 SYMBOL: ?SortItemsByName@CSWCWorldJournal@@QAEHHHH@Z
int CSWCWorldJournal::SortItemsByName(int a, int b, int bNoDate)
{
    if (m_lstEntries.element[a].m_sText < m_lstEntries.element[b].m_sText) {
        return 1;
    }
    if (m_lstEntries.element[a].m_sText > m_lstEntries.element[b].m_sText) {
        return -1;
    }
    if (!bNoDate) {
        return g_pAppManager->GetClientExoApp()->GetWorldTimer()->CompareWorldTimes(
            m_lstEntries.element[a].m_nCalendarDay, m_lstEntries.element[a].m_nTimeOfDay,
            m_lstEntries.element[b].m_nCalendarDay, m_lstEntries.element[b].m_nTimeOfDay);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00676E60
void CSWCWorldJournal::DeleteEntryStrRef(unsigned long nStrRef)
{
    int nCount = m_lstEntries.num;
    for (int i = 0; i < nCount; i++) {
        if (m_lstEntries.element[i].m_bStrRef && m_lstEntries.element[i].m_nID == nStrRef) {
            m_lstEntries.DelIndex(i);
            for (int j = 0; j < nCount; j++) {
                if (m_lstSorted.element[j] == i) {
                    m_lstSorted.DelIndex(j);
                    nCount = m_lstSorted.num;
                }
                if (j != nCount && m_lstSorted.element[j] > i) {
                    m_lstSorted.element[j]--;
                }
            }
        }
    }
    m_bChanged = 1;
}

// NONMATCH: MSVC7 keeps the count in EBP and the ID in EBX; the original keeps
// the count in EDX (dead across the call) and re-reads the ID. Four shapes tried.
// FUNCTION: SWKOTOR 0x00676F00
void CSWCWorldJournal::DeleteEntry(unsigned long nID)
{
    int nCount = m_lstEntries.num;
    for (int i = 0; i < nCount; i++) {
        int nIndex = m_lstSorted.element[i];
        if (!m_lstEntries.element[nIndex].m_bStrRef && m_lstEntries.element[nIndex].m_nID == nID) {
            m_lstEntries.DelIndex(nIndex);
            int nRemoved = m_lstSorted.element[i];
            m_lstSorted.DelIndex(i);
            nCount = m_lstSorted.num;
            for (int j = 0; j < nCount; j++) {
                if (m_lstSorted.element[j] > nRemoved) {
                    m_lstSorted.element[j]--;
                }
            }
        }
    }
    m_bChanged = 1;
}

// FUNCTION: SWKOTOR 0x00676FA0
void CSWCWorldJournal::DeleteAllEntries()
{
    m_lstEntries.Clear();
    m_lstSorted.Clear();
    m_bChanged = 1;
}

// FUNCTION: SWKOTOR 0x00677000
void CSWCWorldJournal::SortList()
{
    int (CSWCWorldJournal::*pCompare)(int, int, int);
    if (!m_nSortBy) {
        pCompare = &CSWCWorldJournal::SortItemsByDate;
    }
    else {
        pCompare = &CSWCWorldJournal::SortItemsByName;
    }
    int nCount = m_lstSorted.num;
    for (int i = 0; i < nCount; i++) {
        int nBest = i;
        for (int j = i + 1; j < nCount; j++) {
            if ((this->*pCompare)(m_lstSorted.element[nBest], m_lstSorted.element[j], 0) < 0) {
                nBest = j;
            }
        }
        int nTemp = m_lstSorted.element[i];
        m_lstSorted.element[i] = m_lstSorted.element[nBest];
        m_lstSorted.element[nBest] = nTemp;
    }
}

// FUNCTION: SWKOTOR 0x00677130
void CSWCWorldJournal::AddEntryStrRef(unsigned long nName, unsigned long nText,
                                      unsigned long nDay, unsigned long nTime)
{
    STR_RES sName;
    STR_RES sText;
    if (!g_pTlkTable->Fetch(nName, sName, g_nGender)) {
        sName.m_sText = "";
    }
    if (!g_pTlkTable->Fetch(nText, sText, g_nGender)) {
        sText.m_sText = "";
    }
    CWorldJournalEntry cEntry;
    cEntry.m_nID = nName;
    cEntry.m_sName = sName.m_sText;
    cEntry.m_sText = sText.m_sText;
    cEntry.m_nCalendarDay = nDay;
    cEntry.m_bStrRef = 1;
    cEntry.m_nTimeOfDay = nTime;
    int nIndex = m_lstEntries.num;
    m_lstEntries.Add(cEntry);
    m_lstSorted.Add(nIndex);
    SortList();
    m_bChanged = 1;
}

// FUNCTION: SWKOTOR 0x00677300
void CSWCWorldJournal::AddEntry(unsigned long nID, const CExoString& sName, const CExoString& sText,
                                unsigned long nDay, unsigned long nTime)
{
    CWorldJournalEntry cEntry;
    cEntry.m_nID = nID;
    cEntry.m_sName = sName;
    cEntry.m_sText = sText;
    cEntry.m_nCalendarDay = nDay;
    cEntry.m_nTimeOfDay = nTime;
    cEntry.m_bStrRef = 0;
    int nIndex = m_lstEntries.num;
    m_lstEntries.Add(cEntry);
    m_lstSorted.Add(nIndex);
    SortList();
    m_bChanged = 1;
}
