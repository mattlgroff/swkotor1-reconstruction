// CSWCSortedJournalArrayList destructor (worker C s1).

class CExoString {
public:
    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04

    ~CExoString();                                  // 0x005E5C20
};

struct SJournalEntry {
    char pad_0x0[0x3c];

    ~SJournalEntry();                               // 0x005C5500
};

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

// Owns the sort scratch buffer at +0x0c.
struct CSWCSortScratch {
    int* m_pnIndices;                               // 0x00

    ~CSWCSortScratch()
    {
        delete m_pnIndices;
        m_pnIndices = 0;
    }
};

class CSWCSortedJournalArrayList : public CExoArrayList<SJournalEntry> {
public:
    ~CSWCSortedJournalArrayList();

    CSWCSortScratch m_cSortScratch;                 // 0x0c (descriptive)
};

// FUNCTION: SWKOTOR 0x005FCB50
CSWCSortedJournalArrayList::~CSWCSortedJournalArrayList()
{
}
