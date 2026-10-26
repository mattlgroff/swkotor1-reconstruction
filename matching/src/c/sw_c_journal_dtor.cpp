// CSWCJournal destructor (worker C s1). The sorted-list destructor stays an
// out-of-line call (0x005FCB50), so it lives in sw_c_sorted_journal_dtor.cpp.

class CExoString {
public:
    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04

    ~CExoString();                                  // 0x005E5C20
};

struct CSWCJournalString {
    unsigned long m_nId;                            // 0x00
    CExoString m_sText;                             // 0x04

    ~CSWCJournalString();                           // 0x0059EA30 (shared string-holder destructor)
};

struct SJournalEntry;

template <class T>
class CExoArrayList {
public:
    T* element;                                     // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08

    ~CExoArrayList()
    {
        if (element) {
            delete[] element;
        }
        element = 0;
    }
};

class CSWCSortedJournalArrayList {
public:
    ~CSWCSortedJournalArrayList();                  // 0x005FCB50

    SJournalEntry* element;                         // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08
    int* m_pnSortIndices;                           // 0x0c
};

class CSWCJournal {
public:
    ~CSWCJournal();

    CSWCSortedJournalArrayList m_lstQuests;         // 0x00 (descriptive)
    char pad_0x10[0x10];
    CSWCSortedJournalArrayList m_lstEntries;        // 0x20 (descriptive)
    char pad_0x30[0x14];
    CExoArrayList<CSWCJournalString> m_lstStrings;  // 0x44 (descriptive)
};

// FUNCTION: SWKOTOR 0x00602C10
CSWCJournal::~CSWCJournal()
{
}
