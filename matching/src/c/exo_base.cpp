// CExoBase destructor (b005e7_exo_string_array_misc).

class CExoBaseInternal { public: CExoBaseInternal(); ~CExoBaseInternal(); char pad[0xc]; };   // 0x005E8BA0 / 0x005E8BB0
class CExoIni { public: CExoIni(); ~CExoIni(); void* m_p; };                     // 0x005E6750 / 0x005E67E0
class CExoAliasList { public: CExoAliasList(); ~CExoAliasList(); void* m_p; };   // 0x005E6810 / 0x005E69B0
class CExoTimers { public: CExoTimers(); ~CExoTimers(); void* m_p; };            // 0x005E91E0 / 0x005E9270
class CExoDebug { public: CExoDebug(); ~CExoDebug(); char pad[0x10]; };          // 0x005E9370 / 0x005E9430
class CExoRand { public: CExoRand(); ~CExoRand(); void* m_p; };                  // 0x005EA420 / 0x005EA4A0

class CExoBase {
public:
    CExoIni* m_pcExoIni;                            // 0x00
    CExoTimers* m_pcExoTimers;                      // 0x04
    CExoDebug* m_pcExoDebug;                        // 0x08
    CExoAliasList* m_pcExoAliasList;                // 0x0c
    CExoRand* m_pcExoRand;                          // 0x10
    CExoBaseInternal* m_pcExoBaseInternal;          // 0x14

    CExoBase();
    ~CExoBase();
};

// FUNCTION: SWKOTOR 0x005E6500
CExoBase::CExoBase()
{
    m_pcExoDebug = new CExoDebug();
    m_pcExoTimers = new CExoTimers();
    m_pcExoAliasList = new CExoAliasList();
    m_pcExoIni = new CExoIni();
    m_pcExoRand = new CExoRand();
    m_pcExoBaseInternal = new CExoBaseInternal();
}

// FUNCTION: SWKOTOR 0x005E66A0
CExoBase::~CExoBase()
{
    if (m_pcExoBaseInternal) {
        delete m_pcExoBaseInternal;
        m_pcExoBaseInternal = 0;
    }
    if (m_pcExoIni) {
        delete m_pcExoIni;
        m_pcExoIni = 0;
    }
    if (m_pcExoAliasList) {
        delete m_pcExoAliasList;
        m_pcExoAliasList = 0;
    }
    if (m_pcExoTimers) {
        delete m_pcExoTimers;
        m_pcExoTimers = 0;
    }
    if (m_pcExoDebug) {
        delete m_pcExoDebug;
        m_pcExoDebug = 0;
    }
    if (m_pcExoRand) {
        delete m_pcExoRand;
        m_pcExoRand = 0;
    }
}
