// CExoCriticalSection facade destructor (b005e0_exo_foundation_leaves).

class CExoCriticalSectionInternal {
public:
    char pad_0x0[0x18];

    CExoCriticalSectionInternal();                  // 0x005EC250
    ~CExoCriticalSectionInternal();                 // 0x005EC260
};

class CExoCriticalSection {
public:
    CExoCriticalSectionInternal* m_pcExoCriticalSectionInternal;   // 0x00

    CExoCriticalSection();
    ~CExoCriticalSection();
};

// FUNCTION: SWKOTOR 0x005E9290
CExoCriticalSection::CExoCriticalSection()
{
    m_pcExoCriticalSectionInternal = new CExoCriticalSectionInternal();
}

// FUNCTION: SWKOTOR 0x005E9320
CExoCriticalSection::~CExoCriticalSection()
{
    if (m_pcExoCriticalSectionInternal) {
        delete m_pcExoCriticalSectionInternal;
    }
    m_pcExoCriticalSectionInternal = 0;
}
