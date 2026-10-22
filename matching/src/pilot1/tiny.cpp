// Pilot group 1: tiny leaves.

class CAuroraInstance;

class CAurInternal {
public:
    CAuroraInstance* CreateInstance();
    CAuroraInstance* NewCAuroraInstance();
};

// FUNCTION: SWKOTOR 0x0044ED60
CAuroraInstance* CAurInternal::NewCAuroraInstance()
{
    return CreateInstance();
}

class CClientExoAppInternal {
public:
    char pad_0x0[0x90];
    int m_nCaptureEventPlayBack;    // 0x90
};

class CClientExoApp {
public:
    void* vtable_dummy;             // 0x0 (placeholder for vtable)
    CClientExoAppInternal* m_pcInternal; // 0x4
    int GetCaptureEventPlayBack();
};

// FUNCTION: SWKOTOR 0x005ED480
int CClientExoApp::GetCaptureEventPlayBack()
{
    return m_pcInternal->m_nCaptureEventPlayBack;
}
