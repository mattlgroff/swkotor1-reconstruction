// Client quest/world journal containers (nwcjournal.o).
#ifndef M_SW_C_JOURNAL_H
#define M_SW_C_JOURNAL_H

class CExoString {
public:
    char* m_sString;
    unsigned long m_nBufferLength;

    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    CExoString(const CExoString& s);                // 0x005E5B00
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    CExoString& operator=(const char* s);           // 0x005E5140
    int operator==(const CExoString& s) const;      // 0x005E5200
    int operator<(const CExoString& s) const;       // 0x005E5410
    int operator>(const CExoString& s) const;       // 0x005E5490
    char* CStr() const;                             // 0x005E5670
    void Format(const char* fmt, ...);              // 0x005E5680
};

class CResRef {
public:
    CResRef();
    char m_resRef[16];
};

struct STR_RES {
    CExoString m_sText;                             // 0x00
    CResRef m_cSound;                               // 0x08
    float m_fSoundLength;                           // 0x18
};

class CTlkTable {
public:
    int Fetch(unsigned long nStrRef, STR_RES& res, unsigned char nGender); // 0x0041E550
};

extern CTlkTable* g_pTlkTable;                      // 0x007A3A08
extern unsigned char g_nGender;                     // 0x0073ECD0

class CExoLocString {
public:
    CExoLocString();
    CExoLocString(const CExoLocString&);
    ~CExoLocString();
    int GetString(unsigned long nLanguage, CExoString* pString, unsigned char nGender) const; // 0x005EA130
    void* m_pList;
    void* m_pPad;
};

template <class T>
class CExoArrayList {
public:
    T* element;                                     // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08

    void Allocate(int n);
    void Add(T t);
    void Clear()
    {
        if (element) {
            delete[] element;
            element = 0;
            array_size = 0;
        }
        num = 0;
    }
    void AddAndDispose(T t);
    void DelIndex(int i);
};

// Sorted index lists: growth and append are inline.
template <>
class CExoArrayList<int> {
public:
    int* element;                                   // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08

    void Allocate(int n);                           // 0x005E03C0
    void Clear()
    {
        if (element) {
            delete[] element;
            element = 0;
            array_size = 0;
        }
        num = 0;
    }
    void Add(int t)
    {
        if (num == array_size) {
            Allocate(array_size ? array_size * 2 : 16);
        }
        element[num++] = t;
    }
    void DelIndex(int i)
    {
        num--;
        for (; i < num; i++) {
            element[i] = element[i + 1];
        }
    }
};

class CWorldJournalEntry {
public:
    CWorldJournalEntry() {}
    CWorldJournalEntry(const CWorldJournalEntry& e); // 0x004C4080

    CExoString m_sName;                             // 0x00
    CExoString m_sText;                             // 0x08
    unsigned long m_nCalendarDay;                   // 0x10
    unsigned long m_nTimeOfDay;                     // 0x14
    unsigned long m_n18;                            // 0x18
    unsigned long m_nID;                            // 0x1C
    unsigned long m_bStrRef : 1;                    // 0x20 bit 0
};

class CWorldTimer {
public:
    int CompareWorldTimes(unsigned long nDay1, unsigned long nTime1,
                          unsigned long nDay2, unsigned long nTime2); // 0x004ADFA0
};

class CSWCCMessageData {
public:
    CSWCCMessageData();                             // 0x004D69F0
    void SetString(int nIndex, const CExoString& s); // 0x004D6AE0
    char pad_000[0x34];
};

class CGuiInGame {
public:
    void UpdateStatus(unsigned char nType, int nValue); // 0x0062EEB0
};

class CClientExoApp {
public:
    unsigned long GetClientLanguage();              // 0x005ED4C0
    CWorldTimer* GetWorldTimer();                   // 0x005ED4F0
    int GetLoadBarState();                          // 0x005ED970
    void CallUnknown5FCD10(int nType, CSWCCMessageData* pData, const CExoString& s); // 0x005ED360
    CGuiInGame* GetInGameGui();                     // 0x005ED690
};

class CAppManager {
public:
    void* m_p00;
    CClientExoApp* m_pClientExoApp;                 // 0x04

    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CSWCWorldJournal {
public:
    int SortItemsByDate(int a, int b, int bNoName);
    int SortItemsByName(int a, int b, int bNoDate);
    void DeleteEntryStrRef(unsigned long nStrRef);
    void DeleteEntry(unsigned long nID);
    void DeleteAllEntries();
    void SortList();
    void AddEntryStrRef(unsigned long nName, unsigned long nText,
                        unsigned long nDay, unsigned long nTime);
    void AddEntry(unsigned long nID, const CExoString& sName, const CExoString& sText,
                  unsigned long nDay, unsigned long nTime);

    CExoArrayList<CWorldJournalEntry> m_lstEntries; // 0x00
    CExoArrayList<int> m_lstSorted;                 // 0x0C
    unsigned char m_nSortBy;                        // 0x18
    unsigned long m_bChanged : 1;                   // 0x1C bit 0
};

struct SJournalEntry {
    CExoLocString m_sName;                          // 0x00
    CExoLocString m_sText;                          // 0x08
    unsigned long m_nCalendarDay;                   // 0x10
    unsigned long m_nTimeOfDay;                     // 0x14
    CExoString m_sPlotID;                           // 0x18
    int m_nState;                                   // 0x20
    unsigned long m_n24;                            // 0x24
    unsigned short m_nPictureIndex;                 // 0x28
    unsigned long m_bQuestCompleted : 1;            // 0x2C bit 0
    unsigned long m_bQuestDisplayed : 1;            // 0x2C bit 1
    unsigned long m_bUpdated : 1;                   // 0x2C bit 2
    unsigned long m_nPlanetID;                      // 0x30
    int m_n34;                                      // 0x34
    unsigned long m_nQuestXP;                       // 0x38

    SJournalEntry();                                // 0x005C53E0
    SJournalEntry(const SJournalEntry& e);          // 0x005C5560
    ~SJournalEntry();                               // 0x005C5500
    SJournalEntry& operator=(const SJournalEntry& e); // 0x005C5470
    void Update(SJournalEntry e, unsigned long nMask); // 0x005C5740
};

struct CSWCJournalPendingUpdate {
    CExoString m_sText;
    unsigned long m_nCompleted;
};

class CSWCSortedJournalArrayList {
public:
    int SortItemsByDate(int a, int b);              // 0x00675ED0
    int SortItemsByName(int a, int b);
    int SortItemsByPlanet(int a, int b);
    int SortItemsByPriority(int a, int b);
    void SortList();

    CExoArrayList<SJournalEntry> m_lstEntries;      // 0x00
    CExoArrayList<int> m_lstSorted;                 // 0x0C
    unsigned long m_nSortBy;                        // 0x18
    int m_bChanged;                                 // 0x1C
};

class CSWCJournal {
public:
    int HasNewEntries();
    void Add(SJournalEntry entry, unsigned short nFlags);
    void JournalUpdated(int bDefer, int bCompleted, const CExoString& sText);

    CSWCSortedJournalArrayList m_DoneList;          // 0x00
    CSWCSortedJournalArrayList m_ActiveList;        // 0x20
    unsigned long m_nFlags;                         // 0x40
    CExoArrayList<CSWCJournalPendingUpdate> m_lstPending; // 0x44
};

// Windows call evidence names the client quest journal CSWCQuestJournal.
class CSWCQuestJournal : public CSWCJournal {
public:
    void Remove(const CExoString& sPlotID);
    void SetJournalPicture(const CExoString& sPlotID, unsigned short nPicture);
    void ClearQuests();
};

extern int g_bJournalUpdated;                       // 0x00833A94

#endif
