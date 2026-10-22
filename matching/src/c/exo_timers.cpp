// CExoTimers facade (b005e4_exo_pimpl_ctors).

class CExoTimersInternal {
public:
    char pad_0x0[0x20];

    CExoTimersInternal();                           // 0x005EC160
};

class CExoTimers {
public:
    CExoTimersInternal* m_pcExoTimersInternal;      // 0x00

    CExoTimers();
};

// FUNCTION: SWKOTOR 0x005E91E0
CExoTimers::CExoTimers()
{
    m_pcExoTimersInternal = new CExoTimersInternal();
}
