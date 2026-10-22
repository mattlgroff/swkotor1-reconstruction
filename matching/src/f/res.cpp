// CRes subclasses and CLYT (b0058_f_lifetime_leaves).

extern "C" __declspec(dllimport) void __stdcall AIL_mem_free_lock(void *ptr);

class CRes {
public:
    CRes();
    virtual ~CRes();
    virtual int OnResourceFreed();
    int Release();

    char pad_0x4[0xc];
    void *m_pResource;       // +0x10
    char pad_0x14[0x14];
};

class CResWave : public CRes {
public:
    CResWave();
    virtual ~CResWave();
    virtual int OnResourceFreed();

    int m_n28;               // +0x28
    int m_n2C;               // +0x2c
    int m_n30;               // +0x30
    int m_n34;               // +0x34
    int m_n38;               // +0x38
};

// FUNCTION: SWKOTOR 0x005DF1B0
CResWave::CResWave()
{
    m_n30 = 0;
    m_n28 = 0;
    m_n2C = 0;
    m_n34 = 0;
    m_n38 = 0;
}

// FUNCTION: SWKOTOR 0x005DF1F0
int CResWave::OnResourceFreed()
{
    if (m_n28) {
        AIL_mem_free_lock(m_pResource);
        m_pResource = 0;
    }
    m_n30 = 0;
    m_n28 = 0;
    m_n2C = 0;
    m_n34 = 0;
    m_n38 = 0;
    return 1;
}

class CResLYT : public CRes {
public:
    CResLYT();
    virtual ~CResLYT();

    int m_n28;               // +0x28
    int m_n2C;               // +0x2c
    int m_n30;               // +0x30
};

// FUNCTION: SWKOTOR 0x005DF3A0
CResLYT::CResLYT()
{
    m_n28 = 0;
    m_n2C = 0;
    m_n30 = 0;
}

class CLYT {
public:
    void UnloadLayout();

    char pad_0x0[0x8];
    CRes *m_pRes;            // +0x08
    char pad_0xc[0x14];
    int m_bLoaded;           // +0x20
};

// FUNCTION: SWKOTOR 0x005DE450
void CLYT::UnloadLayout()
{
    if (m_bLoaded) {
        m_pRes->Release();
        m_bLoaded = 0;
    }
}
