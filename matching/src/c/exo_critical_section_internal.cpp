// CExoCriticalSectionInternal: a CRITICAL_SECTION wrapper (b005e0_exo_foundation_leaves).

struct _RTL_CRITICAL_SECTION {
    char data[0x18];
};

extern "C" {
__declspec(dllimport) void __stdcall InitializeCriticalSection(_RTL_CRITICAL_SECTION* p);  // IAT 0x0073D178
__declspec(dllimport) void __stdcall DeleteCriticalSection(_RTL_CRITICAL_SECTION* p);      // IAT 0x0073D174
__declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION* p);       // IAT 0x0073D170
__declspec(dllimport) void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION* p);       // IAT 0x0073D16C
}

class CExoCriticalSectionInternal {
public:
    _RTL_CRITICAL_SECTION m_cCriticalSection;       // 0x00

    CExoCriticalSectionInternal();
    ~CExoCriticalSectionInternal();
    void EnterCriticalSection();
    void LeaveCriticalSection();
};

// FUNCTION: SWKOTOR 0x005EC250
CExoCriticalSectionInternal::CExoCriticalSectionInternal()
{
    ::InitializeCriticalSection(&m_cCriticalSection);
}

// FUNCTION: SWKOTOR 0x005EC260
CExoCriticalSectionInternal::~CExoCriticalSectionInternal()
{
    ::DeleteCriticalSection(&m_cCriticalSection);
}

// FUNCTION: SWKOTOR 0x005EC270
void CExoCriticalSectionInternal::EnterCriticalSection()
{
    ::EnterCriticalSection(&m_cCriticalSection);
}

// FUNCTION: SWKOTOR 0x005EC280
void CExoCriticalSectionInternal::LeaveCriticalSection()
{
    ::LeaveCriticalSection(&m_cCriticalSection);
}
