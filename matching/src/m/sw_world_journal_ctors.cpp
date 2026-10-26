// Journal object constructors, with fields laid out at their observed offsets.

template <class T>
class CExoArrayList {
public:
    CExoArrayList()
    {
        num = 0;
        array_size = 0;
        element = 0;
    }
    ~CExoArrayList() { if (element) delete[] element; }

    T* element;
    int num;
    int array_size;
};

class CSWCWorldJournalEntry {
public:
    ~CSWCWorldJournalEntry();
};

class CSWCWorldJournal {
public:
    CSWCWorldJournal();

    CExoArrayList<CSWCWorldJournalEntry> m_entries;
    CExoArrayList<unsigned long> m_sortedIndices;
    unsigned char m_n18;
    unsigned char m_n19[3];
    unsigned long m_n1C;
};

// NONMATCH: field stores agree; original includes an EH registration frame for member cleanup.
// FUNCTION: SWKOTOR 0x00676DA0 SYMBOL: ??0CSWCWorldJournal@@QAE@XZ
CSWCWorldJournal::CSWCWorldJournal()
    : m_entries()
    , m_sortedIndices()
{
    m_n18 = 0;
    m_n1C &= ~1UL;
}

class CSWCQuestJournal {
public:
    CSWCQuestJournal();

    unsigned long m_n00;
    unsigned long m_n04;
    unsigned long m_n08;
    unsigned long m_n0C;
    unsigned long m_n10;
    unsigned long m_n14;
    unsigned long m_n18;
    unsigned long m_n1C;
    unsigned long m_n20;
    unsigned long m_n24;
    unsigned long m_n28;
    unsigned long m_n2C;
    unsigned long m_n30;
    unsigned long m_n34;
    unsigned long m_n38;
    unsigned long m_n3C;
    unsigned long m_n40;
    unsigned long m_n44;
    unsigned long m_n48;
    unsigned long m_n4C;
};

// NONMATCH: field stores agree; original includes an EH registration frame for member cleanup.
// FUNCTION: SWKOTOR 0x006770C0 SYMBOL: ??0CSWCQuestJournal@@QAE@XZ
CSWCQuestJournal::CSWCQuestJournal()
{
    m_n04 = 0;
    m_n08 = 0;
    m_n00 = 0;
    m_n10 = 0;
    m_n14 = 0;
    m_n0C = 0;
    m_n18 = 0;
    m_n1C = 0;
    m_n24 = 0;
    m_n28 = 0;
    m_n20 = 0;
    m_n30 = 0;
    m_n34 = 0;
    m_n2C = 0;
    m_n38 = 0;
    m_n3C = 0;
    m_n48 = 0;
    m_n4C = 0;
    m_n44 = 0;
    m_n40 |= 1;
}
