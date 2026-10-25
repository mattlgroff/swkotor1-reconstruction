// CSWCModule dialogue-camera queries and transitions.

class CExoString {
public:
    char* m_sString;
    unsigned long m_nBufferLength;
    char* CStr() const;                             // 0x005E5670
};

class CCameraBehavior {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18();
    virtual void* GetEventTime(const char* szEvent, int* pStart, int* pEnd); // 0x1c
};

class CSWCModule {
public:
    int IsCameraAnimated();

    char pad_0x0[0x12c];
    CCameraBehavior* m_pCamera;
    CExoString m_sCameraName;
};

// FUNCTION: SWKOTOR 0x006412B0
// NONMATCH: MSVC7 reloads the vtable after CStr instead of using the saved
// vtable slot for dispatch; best semantic reconstruction remains nonexact.
int CSWCModule::IsCameraAnimated()
{
    CCameraBehavior* pCamera = m_pCamera;
    if (pCamera) {
        void** pVtable = *(void***)pCamera;
        const char* szName = m_sCameraName.CStr();
        pCamera = m_pCamera;
        if (pVtable && pCamera) {
            return pCamera->GetEventTime(szName, 0, 0) != 0 ? 1 : 0;
        }
    }
    return 0;
}
