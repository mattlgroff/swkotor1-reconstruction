// CSWCJournal / CSWCSortedJournalArrayList: client quest journal (nwcjournal.o).
#include "m/sw_c_journal.h"

// FUNCTION: SWKOTOR 0x00675F10
int CSWCSortedJournalArrayList::SortItemsByName(int a, int b)
{
    CExoString sNameA;
    CExoString sNameB;
    unsigned long nLanguage = g_pAppManager->GetClientExoApp()->GetClientLanguage();
    m_lstEntries.element[a].m_sName.GetString(nLanguage, &sNameA, 0);
    m_lstEntries.element[b].m_sName.GetString(nLanguage, &sNameB, 0);
    if (sNameA < sNameB) {
        return 1;
    }
    if (sNameA > sNameB) {
        return -1;
    }
    return 0;
}

// Descriptive name: orders by the unsigned field at +0x24, ties by date.
// FUNCTION: SWKOTOR 0x00676050
int CSWCSortedJournalArrayList::SortItemsByPlanet(int a, int b)
{
    unsigned long nA = m_lstEntries.element[a].m_n24;
    unsigned long nB = m_lstEntries.element[b].m_n24;
    if (nA > nB) {
        return -1;
    }
    if (nA < nB) {
        return 1;
    }
    return SortItemsByDate(a, b);
}

// Descriptive name: orders by the signed field at +0x34, ties by date.
// FUNCTION: SWKOTOR 0x006760A0
int CSWCSortedJournalArrayList::SortItemsByPriority(int a, int b)
{
    int nA = m_lstEntries.element[a].m_n34;
    int nB = m_lstEntries.element[b].m_n34;
    if (nA > nB) {
        return 1;
    }
    if (nA < nB) {
        return -1;
    }
    return SortItemsByDate(a, b);
}

// FUNCTION: SWKOTOR 0x00676530
void CSWCSortedJournalArrayList::SortList()
{
    int (CSWCSortedJournalArrayList::*pCompare)(int, int);
    int nCount;
    if (m_nSortBy == 0) {
        pCompare = &CSWCSortedJournalArrayList::SortItemsByDate;
    }
    else if (m_nSortBy == 1) {
        pCompare = &CSWCSortedJournalArrayList::SortItemsByName;
    }
    else if (m_nSortBy == 2) {
        pCompare = &CSWCSortedJournalArrayList::SortItemsByPlanet;
    }
    else {
        pCompare = &CSWCSortedJournalArrayList::SortItemsByPriority;
    }
    nCount = m_lstEntries.num;
    for (int i = 0; i < nCount; i++) {
        int nBest = i;
        for (int j = i + 1; j < nCount; j++) {
            if ((this->*pCompare)(m_lstSorted.element[nBest], m_lstSorted.element[j]) < 0) {
                nBest = j;
            }
        }
        int nTemp = m_lstSorted.element[i];
        m_lstSorted.element[i] = m_lstSorted.element[nBest];
        m_lstSorted.element[nBest] = nTemp;
    }
    m_bChanged = 1;
}

// FUNCTION: SWKOTOR 0x006760F0
void CSWCQuestJournal::Remove(const CExoString& sPlotID)
{
    int nActive = m_ActiveList.m_lstEntries.num;
    for (int i = 0; i < nActive; i++) {
        if (m_ActiveList.m_lstEntries.element[i].m_sPlotID == sPlotID) {
            m_ActiveList.m_lstEntries.DelIndex(i);
            for (int j = 0; j < nActive; j++) {
                if (m_ActiveList.m_lstSorted.element[j] == i) {
                    m_ActiveList.m_lstSorted.DelIndex(j);
                    nActive = m_ActiveList.m_lstSorted.num;
                }
                if (j != nActive && m_ActiveList.m_lstSorted.element[j] > i) {
                    m_ActiveList.m_lstSorted.element[j]--;
                }
            }
            m_ActiveList.m_bChanged = 1;
            return;
        }
    }
    int nDone = m_DoneList.m_lstEntries.num;
    for (int n = 0; n < nDone; n++) {
        if (m_DoneList.m_lstEntries.element[n].m_sPlotID == sPlotID) {
            m_DoneList.m_lstEntries.num--;
            for (int k = n; k < m_DoneList.m_lstEntries.num; k++) {
                m_DoneList.m_lstEntries.element[k] = m_DoneList.m_lstEntries.element[k + 1];
            }
            for (int m = 0; m < nDone; m++) {
                if (m_DoneList.m_lstSorted.element[m] == n) {
                    m_DoneList.m_lstSorted.DelIndex(m);
                    nDone = m_DoneList.m_lstSorted.num;
                }
                if (m != nDone && m_DoneList.m_lstSorted.element[m] > n) {
                    m_DoneList.m_lstSorted.element[m]--;
                }
            }
            m_DoneList.m_bChanged = 1;
            return;
        }
    }
}

// FUNCTION: SWKOTOR 0x00676260
void CSWCQuestJournal::SetJournalPicture(const CExoString& sPlotID, unsigned short nPicture)
{
    int i;
    int nCount = m_DoneList.m_lstEntries.num;
    for (i = 0; i < nCount; i++) {
        if (m_DoneList.m_lstEntries.element[i].m_sPlotID == sPlotID) {
            m_DoneList.m_lstEntries.element[i].m_nPictureIndex = nPicture;
            m_DoneList.m_lstEntries.element[i].m_bUpdated = 1;
            m_DoneList.m_bChanged = 1;
            return;
        }
    }
    nCount = m_ActiveList.m_lstEntries.num;
    for (i = 0; i < nCount; i++) {
        if (m_ActiveList.m_lstEntries.element[i].m_sPlotID == sPlotID) {
            m_ActiveList.m_lstEntries.element[i].m_nPictureIndex = nPicture;
            m_ActiveList.m_lstEntries.element[i].m_bUpdated = 1;
            m_ActiveList.m_bChanged = 1;
            return;
        }
    }
}

// Descriptive name: any active entry flagged as updated.
// FUNCTION: SWKOTOR 0x00676470
int CSWCJournal::HasNewEntries()
{
    int nCount = m_ActiveList.m_lstEntries.num;
    for (int i = 0; i < nCount; i++) {
        if (m_ActiveList.m_lstEntries.element[i].m_bUpdated) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00676B00
void CSWCQuestJournal::ClearQuests()
{
    m_DoneList.m_lstEntries.Clear();
    m_DoneList.m_lstSorted.Clear();
    m_ActiveList.m_lstEntries.Clear();
    m_ActiveList.m_lstSorted.Clear();
}

// FUNCTION: SWKOTOR 0x00676C20
void CSWCJournal::JournalUpdated(int bDefer, int bCompleted, const CExoString& sText)
{
    if (g_pAppManager->GetClientExoApp()->GetLoadBarState() && bDefer) {
        CSWCJournalPendingUpdate cUpdate;
        cUpdate.m_sText = sText;
        cUpdate.m_nCompleted = bCompleted;
        m_lstPending.Add(cUpdate);
        return;
    }
    CSWCCMessageData* pData = new CSWCCMessageData();
    pData->SetString(0, sText);
    {
        CExoString sEmpty("");
        g_pAppManager->GetClientExoApp()->CallUnknown5FCD10(0xb8, pData, sEmpty);
    }
    g_pAppManager->GetClientExoApp()->GetInGameGui()->UpdateStatus(0, 0);
    if (bCompleted) {
        g_pAppManager->GetClientExoApp()->GetInGameGui()->UpdateStatus(0xb, 0);
    }
    else {
        g_pAppManager->GetClientExoApp()->GetInGameGui()->UpdateStatus(0xa, 0);
    }
    g_bJournalUpdated = 1;
}
