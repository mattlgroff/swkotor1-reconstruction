// CExoIni facade (b005e3_exo_file_wrappers, b005e4_exo_pimpl_ctors).

class CExoIniInternal {
public:
    char pad_0x0[0xc];

    CExoIniInternal();                              // 0x005EA540
    ~CExoIniInternal();                             // 0x005EB500
};

class CExoIni {
public:
    CExoIniInternal* m_pcExoIniInternal;            // 0x00

    CExoIni();
    ~CExoIni();
};

// FUNCTION: SWKOTOR 0x005E6750
CExoIni::CExoIni()
{
    m_pcExoIniInternal = new CExoIniInternal();
}

// FUNCTION: SWKOTOR 0x005E67E0
CExoIni::~CExoIni()
{
    if (m_pcExoIniInternal) {
        delete m_pcExoIniInternal;
    }
    m_pcExoIniInternal = 0;
}
