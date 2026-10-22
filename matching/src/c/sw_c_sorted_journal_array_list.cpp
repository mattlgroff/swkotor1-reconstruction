// CSWCSortedJournalArrayList (b0062 message leaves).

struct CSWCJournalEntry {
    char pad_0x0[0x10];
    unsigned long m_nDate;                          // 0x10
    unsigned long m_nTime;                          // 0x14
    char pad_0x18[0x24];
};

class CSWCJournalClock {
public:
    int CompareDates(unsigned long nDateA, unsigned long nTimeA,
                     unsigned long nDateB, unsigned long nTimeB);   // 0x004ADFA0
};

class CClientExoApp {
public:
    CSWCJournalClock* GetJournalClock();            // 0x005ED4F0
};

class CAppManager {
public:
    void* m_pUnknown00;                             // 0x00
    CClientExoApp* m_pClientExoApp;                 // 0x04

    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CSWCSortedJournalArrayList {
public:
    int SortItemsByDate(int a, int b);

    CSWCJournalEntry* element;                      // 0x00
};

// FUNCTION: SWKOTOR 0x00675ED0
int CSWCSortedJournalArrayList::SortItemsByDate(int a, int b)
{
    return g_pAppManager->GetClientExoApp()->GetJournalClock()->CompareDates(
        element[a].m_nDate, element[a].m_nTime, element[b].m_nDate, element[b].m_nTime);
}
