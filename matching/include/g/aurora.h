// Aurora (graphics front end) API state, 0x0044d8d0-0x0044f880.
#ifndef G_AURORA_H
#define G_AURORA_H

// --- Win32 / OpenGL imports (declared by hand, 2002-era headers) ----------
extern "C" {
__declspec(dllimport) void __stdcall glEnable(unsigned int cap);
__declspec(dllimport) void __stdcall glDisable(unsigned int cap);
__declspec(dllimport) unsigned int __stdcall glGetError();
const unsigned char* __stdcall gluErrorString(unsigned int err);
struct _devicemodeA;
__declspec(dllimport) long __stdcall ChangeDisplaySettingsA(_devicemodeA* pMode,
                                                            unsigned long dwFlags);
__declspec(dllimport) int __stdcall EnumDisplaySettingsA(const char* szDevice,
                                                         unsigned long nMode,
                                                         _devicemodeA* pMode);
__declspec(dllimport) int __stdcall wglMakeCurrent(void* hDC, void* hRC);
}

#define GL_LIGHT0 0x4000
#define CDS_TEST 2
#define CDS_FULLSCREEN 4

// DEVMODEA is 0x9c bytes.
struct _devicemodeA {
    char dmDeviceName[32];          // +0x00
    unsigned short dmSpecVersion;   // +0x20
    unsigned short dmDriverVersion; // +0x22
    unsigned short dmSize;          // +0x24
    unsigned short dmDriverExtra;   // +0x26
    unsigned long dmFields;         // +0x28
    char pad2c[0x68 - 0x2c];
    unsigned long dmBitsPerPel;     // +0x68
    unsigned long dmPelsWidth;      // +0x6c
    unsigned long dmPelsHeight;     // +0x70
    unsigned long dmDisplayFlags;   // +0x74
    unsigned long dmDisplayFrequency; // +0x78
    char pad7c[0x9c - 0x7c];
};

// vtable 0x007414f0 (slots +0x04..+0x24 are _purecall there; not modelled as pure).
class CAurora {
public:
    virtual ~CAurora();
    virtual int GetDisplayStyle();                   // 0x04
    virtual void Slot2();                            // 0x08 Initialize
    virtual bool GetValidMode(unsigned long* pWidth, unsigned long* pHeight,
                                       unsigned char* pBitsPerPel,
                                       unsigned long* pFrequency,
                                       unsigned char bApply);   // 0x0c
    virtual void Slot4();                            // 0x10
    virtual void Slot5();                            // 0x14
    virtual void Slot6();                            // 0x18
    virtual void Slot7();                            // 0x1c
    virtual void Slot8();                            // 0x20
    virtual int SetGamma(float fGamma, int bTemporary); // 0x24
};

// Members of the single Aurora instance (vtable 0x00741518, 0xb8 bytes).
class __declspec(novtable) CAurInternal : public CAurora {
public:
    CAurInternal();
    void RestoreDesktopDisplayMode();
    void RestoreAuroraDisplayMode();
    static void DestroyCAuroraInstance();

    _devicemodeA m_devMode;      // +0x04
    char pad_a0[0xa8 - 0xa0];
    int m_bFullScreen;           // +0xa8
    int m_nUnknownAC;            // +0xac
    float m_fGamma;              // +0xb0
    int m_nUnknownB4;            // +0xb4
};

class CAurInternalGL : public CAurInternal {
public:
    CAurInternalGL();
    virtual ~CAurInternalGL();
    virtual int GetDisplayStyle();                      // 0x04 0x0044d6d0
    virtual bool GetValidMode(unsigned long* pWidth, unsigned long* pHeight,
                                       unsigned char* pBitsPerPel,
                                       unsigned long* pFrequency,
                                       unsigned char bApply);   // 0x0c 0x0044d6e0
    virtual int SetGamma(float fGamma, int bTemporary); // 0x24 0x0044d990
    virtual void* GetContext();                         // 0x28 0x0044d970
    virtual int MakeCurrent(void* hDC, void* hRC);      // 0x2c 0x0044d980
    void Shutdown(int bRelease, int bRestore); // 0x0044ea30
};

#endif
