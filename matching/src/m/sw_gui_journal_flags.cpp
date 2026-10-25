// Small GUI journal state setters.

class CGuiInGame {
public:
    void SetJournalOption(unsigned long nValue);

    char pad_0x0[0xbc4];
    unsigned long m_nJournalOption;
};

class CSWCJournal {
public:
    void ClearActiveListChanged();
    void ClearDoneListChanged();

    char pad_0x0[0x1c];
    unsigned long m_bDoneListChanged;
    char pad_0x20[0x1c];
    unsigned long m_bActiveListChanged;
};

// FUNCTION: SWKOTOR 0x00644740
void CGuiInGame::SetJournalOption(unsigned long nValue)
{
    m_nJournalOption = m_nJournalOption ^ ((m_nJournalOption ^ nValue) & 1);
}

// FUNCTION: SWKOTOR 0x00644770
void CSWCJournal::ClearActiveListChanged()
{
    m_bActiveListChanged = 0;
}

// FUNCTION: SWKOTOR 0x00644780
void CSWCJournal::ClearDoneListChanged()
{
    m_bDoneListChanged = 0;
}
