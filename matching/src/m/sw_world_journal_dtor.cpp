// World-journal lifetime: arrays of entries and sorted indices.
#include "shared/swc_world_journal_entry.h"
template <class T> class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;
    ~CExoArrayList() { delete[] element; element = 0; }
};
class CSWCWorldJournal {
public:
    ~CSWCWorldJournal();
    CExoArrayList<CSWCWorldJournalEntry> m_entries;
    CExoArrayList<unsigned long> m_sortedIndices;
    unsigned char m_n18;
    unsigned char m_pad[3];
    unsigned long m_n1C;
};

// FUNCTION: SWKOTOR 0x00676DE0
CSWCWorldJournal::~CSWCWorldJournal()
{
}
