// CSWCJournal sorting controls.

class CSWCJournalList {
public:
    void SortList();                                  // 0x00676530
    char pad_0x0[0x18];
    unsigned long m_nSort;
    char pad_0x1c[4];
};

class CSWCJournal {
public:
    void EnableSort();
    void DisableSort();
    void SetActiveSortBy(unsigned long nSort);
    void SetDoneSortBy(unsigned long nSort);

    CSWCJournalList m_DoneList;
    CSWCJournalList m_ActiveList;
    unsigned long m_nFlags;
};

// FUNCTION: SWKOTOR 0x00675DB0
void CSWCJournal::EnableSort()
{
    m_nFlags |= 1;
}

// FUNCTION: SWKOTOR 0x00675DC0
void CSWCJournal::DisableSort()
{
    m_nFlags &= 0xfffffffe;
}

// FUNCTION: SWKOTOR 0x00676BC0
void CSWCJournal::SetActiveSortBy(unsigned long nSort)
{
    if (nSort != 0 && nSort != 1 && nSort != 2 && nSort != 3) {
        return;
    }
    m_ActiveList.m_nSort = nSort;
    if ((m_nFlags & 1) != 0) {
        m_ActiveList.SortList();
    }
}

// FUNCTION: SWKOTOR 0x00676BF0
void CSWCJournal::SetDoneSortBy(unsigned long nSort)
{
    if (nSort != 0 && nSort != 1 && nSort != 2 && nSort != 3) {
        return;
    }
    m_DoneList.m_nSort = nSort;
    if ((m_nFlags & 1) != 0) {
        m_DoneList.SortList();
    }
}
