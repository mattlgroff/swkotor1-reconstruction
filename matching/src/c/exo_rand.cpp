// CExoRand facade destructor (b005e0_exo_foundation_leaves).

class CExoRandInternal {
public:
    char pad_0x0[0x90];

    CExoRandInternal();                             // 0x005ED0E0
    ~CExoRandInternal();                            // 0x005ECC80
};

class CExoRand {
public:
    CExoRandInternal* m_pcExoRandInternal;          // 0x00

    CExoRand();
    ~CExoRand();
};

// FUNCTION: SWKOTOR 0x005EA420
CExoRand::CExoRand()
{
    m_pcExoRandInternal = new CExoRandInternal();
}

// FUNCTION: SWKOTOR 0x005EA4A0
CExoRand::~CExoRand()
{
    if (m_pcExoRandInternal) {
        delete m_pcExoRandInternal;
        m_pcExoRandInternal = 0;
    }
}
