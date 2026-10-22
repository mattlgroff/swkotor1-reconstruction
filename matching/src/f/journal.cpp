// SJournalEntry, CSWSJournal and their CExoArrayList instances
// (b005c_f2_journal_store, 0x005C53E0-0x005C5A30).

class CExoString {
public:
    CExoString();
    CExoString(const CExoString &s);
    ~CExoString();
    CExoString &operator=(const CExoString &s);
    int operator==(const CExoString &s) const;
    char *m_sString;
    unsigned long m_nBufferLength;
};

class CExoLocString {
public:
    CExoLocString();
    CExoLocString(const CExoLocString &s);
    ~CExoLocString();
    CExoLocString &operator=(const CExoLocString &s);
    void *m_pList;
    void *m_pPad;
};

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;

    CExoArrayList() { num = 0; array_size = 0; element = 0; }
    ~CExoArrayList()
    {
        delete[] element;
        element = 0;
    }
    void Allocate(int n);
    void DelIndex(int nIndex);
    void Add(T t)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 16);
        element[num++] = t;
    }
};

struct SJournalEntry {
    CExoLocString m_sName;           // 0x00
    CExoLocString m_sText;           // 0x08
    unsigned long m_nCalendarDay;    // 0x10
    unsigned long m_nTimeOfDay;      // 0x14
    CExoString m_sPlotID;            // 0x18
    int m_nState;                    // 0x20
    int m_nPriority;                 // 0x24
    unsigned short m_nPictureIndex;  // 0x28
    unsigned long m_bQuestCompleted : 1; // 0x2C bit 0
    unsigned long m_bQuestDisplayed : 1; // 0x2C bit 1
    unsigned long m_bUpdated : 1;        // 0x2C bit 2
    unsigned long m_nPlanetID;       // 0x30
    int m_n34;                       // 0x34
    unsigned long m_nQuestXP;        // 0x38

    SJournalEntry();
    SJournalEntry(const SJournalEntry &e);
    ~SJournalEntry();
    SJournalEntry &operator=(const SJournalEntry &e);
    void Update(SJournalEntry e, unsigned long nMask);
};

// FUNCTION: SWKOTOR 0x005C53E0 SYMBOL: ??0SJournalEntry@@QAE@XZ
SJournalEntry::SJournalEntry()
    : m_nCalendarDay(0xffffffff), m_nTimeOfDay(0xffffffff)
{
    m_nState = -1;
    m_nPriority = -1;
    m_bQuestCompleted = 0;
    m_bQuestDisplayed = 0;
    m_bUpdated = 0;
    m_n34 = -1;
    m_nPictureIndex = 0xffff;
    m_nPlanetID = 0;
    m_nQuestXP = 0;
}

// FUNCTION: SWKOTOR 0x005C5470
SJournalEntry &SJournalEntry::operator=(const SJournalEntry &e)
{
    m_sName = e.m_sName;
    m_sText = e.m_sText;
    m_nCalendarDay = e.m_nCalendarDay;
    m_nTimeOfDay = e.m_nTimeOfDay;
    m_sPlotID = e.m_sPlotID;
    m_nState = e.m_nState;
    m_nPriority = e.m_nPriority;
    m_nPictureIndex = e.m_nPictureIndex;
    m_bQuestCompleted = e.m_bQuestCompleted;
    m_bQuestDisplayed = e.m_bQuestDisplayed;
    m_bUpdated = e.m_bUpdated;
    m_nPlanetID = e.m_nPlanetID;
    m_n34 = e.m_n34;
    m_nQuestXP = e.m_nQuestXP;
    return *this;
}

// FUNCTION: SWKOTOR 0x005C5500
SJournalEntry::~SJournalEntry()
{
}

// FUNCTION: SWKOTOR 0x005C5560 SYMBOL: ??0SJournalEntry@@QAE@ABU0@@Z
SJournalEntry::SJournalEntry(const SJournalEntry &e)
    : m_sName(e.m_sName), m_sText(e.m_sText), m_nCalendarDay(e.m_nCalendarDay),
      m_nTimeOfDay(e.m_nTimeOfDay), m_sPlotID(e.m_sPlotID)
{
    m_nState = e.m_nState;
    m_nPriority = e.m_nPriority;
    m_nPictureIndex = e.m_nPictureIndex;
    m_bQuestCompleted = e.m_bQuestCompleted;
    m_bQuestDisplayed = e.m_bQuestDisplayed;
    m_bUpdated = e.m_bUpdated;
    m_nPlanetID = e.m_nPlanetID;
    m_n34 = e.m_n34;
    m_nQuestXP = e.m_nQuestXP;
}

// FUNCTION: SWKOTOR 0x005C5740
void SJournalEntry::Update(SJournalEntry e, unsigned long nMask)
{
    m_sPlotID = e.m_sPlotID;
    if ((nMask & 0x1) || (nMask & 0x8000))
        m_sName = e.m_sName;
    if ((nMask & 0x2) || (nMask & 0x8000))
        m_sText = e.m_sText;
    if ((nMask & 0x4) || (nMask & 0x8000))
        m_nCalendarDay = e.m_nCalendarDay;
    if ((nMask & 0x8) || (nMask & 0x8000))
        m_nTimeOfDay = e.m_nTimeOfDay;
    if ((nMask & 0x10) || (nMask & 0x8000))
        m_nState = e.m_nState;
    if ((nMask & 0x20) || (nMask & 0x8000))
        m_nPriority = e.m_nPriority;
    if ((nMask & 0x80) || (nMask & 0x8000))
        m_bQuestCompleted = e.m_bQuestCompleted;
    if (nMask & 0x8200)
        m_nPlanetID = e.m_nPlanetID;
    if (nMask & 0x8400)
        m_n34 = e.m_n34;
    if (nMask & 0x8800)
        m_nQuestXP = e.m_nQuestXP;
    m_bUpdated = 1;
}

template <class T>
void CExoArrayList<T>::Allocate(int n)
{
    T *pOld = element;
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
void CExoArrayList<T>::DelIndex(int nIndex)
{
    num--;
    for (; nIndex < num; nIndex++) {
        element[nIndex] = element[nIndex + 1];
    }
}

// FUNCTION: SWKOTOR 0x005C5640 SYMBOL: ?DelIndex@?$CExoArrayList@USJournalEntry@@@@QAEXH@Z
// FUNCTION: SWKOTOR 0x005C5680 SYMBOL: ?Allocate@?$CExoArrayList@USJournalEntry@@@@QAEXH@Z
template void CExoArrayList<SJournalEntry>::DelIndex(int nIndex);
template void CExoArrayList<SJournalEntry>::Allocate(int n);

class CSWSJournal {
public:
    CExoArrayList<SJournalEntry> m_lstEntries;   // 0x00

    CSWSJournal();
    ~CSWSJournal();
    unsigned long GetQuestEntryXP(const CExoString &sPlotID);
    int GetState(const CExoString &sPlotID);
};

// FUNCTION: SWKOTOR 0x005C5A30
CSWSJournal::CSWSJournal()
{
}

// FUNCTION: SWKOTOR 0x005C58F0
CSWSJournal::~CSWSJournal()
{
}

// FUNCTION: SWKOTOR 0x005C5850
unsigned long CSWSJournal::GetQuestEntryXP(const CExoString &sPlotID)
{
    int nEntries = m_lstEntries.num;
    for (int i = 0; i < nEntries; i++) {
        if (m_lstEntries.element[i].m_sPlotID == sPlotID)
            return m_lstEntries.element[i].m_nQuestXP;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005C58A0
int CSWSJournal::GetState(const CExoString &sPlotID)
{
    int nEntries = m_lstEntries.num;
    for (int i = 0; i < nEntries; i++) {
        if (m_lstEntries.element[i].m_sPlotID == sPlotID)
            return m_lstEntries.element[i].m_nState;
    }
    return 0;
}

struct CSWSPlayerJournalQuestUpdates {
    unsigned short m_nUpdateType;    // 0x00
    CExoString m_sPlotID;            // 0x04
};

// FUNCTION: SWKOTOR 0x005C59A0 SYMBOL: ?Add@?$CExoArrayList@UCSWSPlayerJournalQuestUpdates@@@@QAEXUCSWSPlayerJournalQuestUpdates@@@Z
template void CExoArrayList<CSWSPlayerJournalQuestUpdates>::Add(CSWSPlayerJournalQuestUpdates t);
