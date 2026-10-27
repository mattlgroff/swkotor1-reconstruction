// CResSSF: the sound set resource (restype 2060, .ssf; vtable 0x00758000).
// Descriptive class name (bucket m's sw_sound_set.cpp uses the same).

class CRes {
public:
    CRes();                                         // 0x00406460
    virtual ~CRes();                                // 0x00409D40
    virtual void* GetData();                        // 0x04 (0x00406490)
    virtual int v08();                              // 0x08
    virtual int OnResourceFreed();                  // 0x0c
    virtual int OnResourceServiced();               // 0x10

    char pad_0x4[0xc];
    void* m_pResource;                              // 0x10
    char pad_0x14[0x14];
};

class CResSSF : public CRes {
public:
    CResSSF();                                      // 0x006DB650
    virtual ~CResSSF();                             // 0x006DB670
    virtual int OnResourceFreed();                  // 0x0c (0x006DB680)
    virtual int OnResourceServiced();               // 0x10 (0x006DB690)

    int m_bLoaded;                                  // 0x28 (descriptive)
};

// FUNCTION: SWKOTOR 0x006DB650
CResSSF::CResSSF()
{
    m_bLoaded = 0;
}

// FUNCTION: SWKOTOR 0x006DB670
CResSSF::~CResSSF()
{
}

// FUNCTION: SWKOTOR 0x006DB680
int CResSSF::OnResourceFreed()
{
    m_bLoaded = 0;
    return 1;
}

// FUNCTION: SWKOTOR 0x006DB690
int CResSSF::OnResourceServiced()
{
    if (!m_pResource) {
        return 0;
    }
    if (m_bLoaded) {
        return 1;
    }
    m_bLoaded = 1;
    return 1;
}

// FUNCTION: SWKOTOR 0x006DB6B0 SYMBOL: ??_GCResSSF@@UAEPAXI@Z
// Emitted with the destructor above.
