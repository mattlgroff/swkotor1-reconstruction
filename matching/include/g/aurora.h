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
}

#define GL_LIGHT0 0x4000
#define CDS_TEST 2
#define CDS_FULLSCREEN 4

// DEVMODEA is 0x9c bytes.
struct _devicemodeA {
    char pad[0x9c];
};

// vtable 0x007414f0
class CAurora {
public:
    virtual ~CAurora();
    virtual void Slot1();
    virtual void Slot2();
    virtual void Slot3();
    virtual void Slot4();
    virtual void Slot5();
    virtual void Slot6();
    virtual void Slot7();
    virtual void Slot8();
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
    void Shutdown(int a, int b); // 0x0044ea30
};

#endif
