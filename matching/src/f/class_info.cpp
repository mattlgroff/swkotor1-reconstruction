// CSWSCreatureStats_ClassInfo ctor / dtor (0x005A76E0, 0x005AC9F0).

extern "C" void *memset(void *p, int c, unsigned int n);

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;

    CExoArrayList()
    {
        num = 0;
        array_size = 0;
        element = 0;
    }
    ~CExoArrayList()
    {
        if (element)
            delete[] element;
        element = 0;
    }
    void SetSize0()
    {
        if (element) {
            delete[] element;
            element = 0;
            array_size = 0;
        }
        num = 0;
    }
};

class CSWSCreatureStats_ClassInfo {
public:
    CSWSCreatureStats_ClassInfo();
    ~CSWSCreatureStats_ClassInfo();

    CExoArrayList<unsigned long> m_pKnownSpellList[1];      // +0x0
    CExoArrayList<unsigned long> m_pMemorizedSpellList[1];  // +0xc
    unsigned char m_nSpellsKnownPerLevel[1];  // +0x18
    unsigned char m_nSpellsPerDayLeft[1];     // +0x19
    unsigned char m_nSpellsPerDay[1];         // +0x1a
    unsigned char m_nClass;                   // +0x1b
    unsigned char m_nLevel;                   // +0x1c
    unsigned char m_nNegativeLevels;          // +0x1d
    unsigned char m_nSpecialization;          // +0x1e
    unsigned char m_nDomain;                  // +0x1f
    unsigned char m_bUnknown20;               // +0x20
    char pad_0x21[0x3];
    int m_n24;                                // +0x24
};

// FUNCTION: SWKOTOR 0x005A76E0
CSWSCreatureStats_ClassInfo::~CSWSCreatureStats_ClassInfo()
{
    for (int i = 0; i < 1; i++)
        m_pKnownSpellList[i].SetSize0();
}

// FUNCTION: SWKOTOR 0x005AC9F0
CSWSCreatureStats_ClassInfo::CSWSCreatureStats_ClassInfo()
{
    m_nClass = 0;
    m_nLevel = 0;
    m_nNegativeLevels = 0;
    m_n24 = 0;
    m_nSpecialization = 0;
    m_nDomain = 0;
    m_bUnknown20 = 1;
    memset(m_nSpellsPerDayLeft, 0, sizeof(m_nSpellsPerDayLeft));
    memset(m_nSpellsPerDay, 0, sizeof(m_nSpellsPerDay));
    memset(m_nSpellsKnownPerLevel, 0, sizeof(m_nSpellsKnownPerLevel));
}
