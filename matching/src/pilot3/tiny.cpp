// Pilot group 3: tiny functions.

class CExoTimersInternal {
public:
    ~CExoTimersInternal();
};

class CExoTimers {
public:
    ~CExoTimers();
    CExoTimersInternal *m_pcExoTimersInternal;
};

// FUNCTION: SWKOTOR 0x005E9270
CExoTimers::~CExoTimers()
{
    delete m_pcExoTimersInternal;
}

class CServerExoAppInternal {
public:
    char pad_0x0[0x100c4];
    int m_bLoadFromSaveGame;
};

class CServerExoApp {
public:
    int GetLoadFromSaveGame();
    void *m_vtbl_pad;
    CServerExoAppInternal *m_pcExoAppInternal;
};

// FUNCTION: SWKOTOR 0x004AF050
int CServerExoApp::GetLoadFromSaveGame()
{
    return m_pcExoAppInternal->m_bLoadFromSaveGame;
}
