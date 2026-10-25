// CSWCJournal's active/done index lookups and full-update sort callback.

struct CSWCJournalEntry {
    char data[0x3c];
};

class CSWCJournalList {
public:
    CSWCJournalEntry* element;
    int num;
    int array_size;
    unsigned long* m_pIndices;
    int m_nNumIndices;
    int m_nIndexCapacity;
    int m_nSort;
    int m_bChanged;

    void SortList();                                // 0x00676530
};

class CSWCJournal {
public:
    CSWCJournalEntry* GetDoneEntryAt(int nIndex);
    CSWCJournalEntry* GetActiveEntryAt(int nIndex);
    void CallFullUpdateComplete();

    CSWCJournalList m_DoneList;                     // 0x00
    CSWCJournalList m_ActiveList;                   // 0x20
    unsigned long m_nFlags;                         // 0x40
};

// FUNCTION: SWKOTOR 0x00676310
CSWCJournalEntry* CSWCJournal::GetActiveEntryAt(int nIndex)
{
    if (nIndex < 0 || nIndex >= m_ActiveList.num) {
        return 0;
    }
    return &m_ActiveList.element[m_ActiveList.m_pIndices[nIndex]];
}

// FUNCTION: SWKOTOR 0x00676340
CSWCJournalEntry* CSWCJournal::GetDoneEntryAt(int nIndex)
{
    if (nIndex < 0 || nIndex >= m_DoneList.num) {
        return 0;
    }
    return &m_DoneList.element[m_DoneList.m_pIndices[nIndex]];
}

// FUNCTION: SWKOTOR 0x00676BA0
void CSWCJournal::CallFullUpdateComplete()
{
    m_ActiveList.SortList();
    m_DoneList.SortList();
}
