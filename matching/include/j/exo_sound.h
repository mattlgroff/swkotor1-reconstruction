// Worker J matching headers: the public sound facades (CExoSound,
// CExoSoundSource, CExoStreamingSoundSource, 0x005D5870-0x005D64E0) and the
// internal classes they forward to (0x005D5FE0-0x005D6BC0, 0x005D8xxx-0x005DCxxx).
#ifndef J_EXO_SOUND_H
#define J_EXO_SOUND_H

class CResRef {
public:
    CResRef(const char* s);                         // 0x00406D80
    char m_resRef[16];
};

struct Vector {
    float x, y, z;
};

int __cdecl operator==(const Vector& a, const Vector& b);    // 0x004AA980

extern int g_bSoundDisabled;                        // 0x00832894
extern int g_n74C5F4;                               // 0x0074C5F4
extern int g_n74C5FC;                               // 0x0074C5FC

class CExoTimers {
public:
    unsigned __int64 GetHighResolutionTimer();      // 0x005E9260
};

class CExoBase {
public:
    char pad_0x0[0x4];
    CExoTimers* m_pcExoTimers;                      // 0x04
};

extern CExoBase* g_pExoBase;                        // 0x007A39E0

// Miles Sound System 3D listener API (IAT 0x0073D4A4-0x0073D4B0).
typedef void* HPROVIDER;
typedef void* H3DPOBJECT;
extern "C" {
__declspec(dllimport) H3DPOBJECT __stdcall AIL_open_3D_listener(HPROVIDER lib);
__declspec(dllimport) void __stdcall AIL_close_3D_listener(H3DPOBJECT listener);
__declspec(dllimport) void __stdcall AIL_set_3D_orientation(H3DPOBJECT obj,
    float fx, float fy, float fz, float ux, float uy, float uz);
__declspec(dllimport) void __stdcall AIL_set_3D_position(H3DPOBJECT obj,
    float x, float y, float z);
}

// One priority group (CExoSoundInternal +0x4C table, 0x18 bytes).
class CPriorityGroup {
public:
    unsigned char GetMaxPlayer();                   // 0x005D6690
    unsigned char GetByte5();                       // 0x005D66A0

    int m_n0;                                       // 0x00
    unsigned char m_nMaxPlayer;                     // 0x04
    unsigned char m_n5;                             // 0x05
    unsigned char m_n6;                             // 0x06
    unsigned char m_nVolume;                        // 0x07
    unsigned long m_n8;                             // 0x08
    unsigned long m_nC;                             // 0x0c
    char pad_0x10[0x8];
};

class CExoSoundSourceInternal;
class CExoStreamingSoundSourceInternal;

// Size 0x14C.
class CExoSoundInternal {
public:
    CExoSoundInternal();                            // 0x005D86E0
    ~CExoSoundInternal();                           // 0x005D9400

    unsigned char GetProperty0x46();                // 0x005D5FE0
    unsigned long GetProperty0x58();                // 0x005D6000
    float GetMusicVolume(int bCurrent);             // 0x005D6010
    float GetSoundEffectVolume(int bCurrent);       // 0x005D6030
    float GetDialogVolume(int bCurrent);            // 0x005D6050
    unsigned char GetNumber2DVoices();              // 0x005D6070
    CPriorityGroup* GetPriorityGroup(unsigned char nGroup);     // 0x005D6080
    void SetListenerOrientation(const Vector& vFront, const Vector& vUp);  // 0x005D6540
    void SetListenerPosition(const Vector& vPos);               // 0x005D6600
    float GetDesiredAmbientScale();                 // 0x005D66B0
    unsigned long DesiredEnvironment();             // 0x005D66C0
    void SetMusicVolume(float fVolume, int nMode, int bApply);  // 0x005D6940
    void SetSoundEffectVolume(float fVolume, int nMode);        // 0x005D69D0
    void SetDialogVolume(float fVolume, int nMode);             // 0x005D69F0
    void MuteSound();                               // 0x005D6A10
    void UnMuteSound();                             // 0x005D6A50
    int GetSourceVolumeType(CExoSoundSourceInternal* pSource);  // 0x005D6A90
    int GetStreamVolumeType(CExoStreamingSoundSourceInternal* pSource);    // 0x005D6AD0
    float GetVolume(int nType, int bCurrent);       // 0x005D6B10
    int IsSourceAudible(CExoSoundSourceInternal* pSource);      // 0x005D6B70
    int IsStreamAudible(CExoStreamingSoundSourceInternal* pSource);    // 0x005D6BC0

    int PlayOneShotSound(unsigned long a, unsigned long b, unsigned long c,
                         unsigned long d, unsigned long e, unsigned long f);    // 0x005D7550
    int PlayOneShotSoundAt(unsigned long a, Vector vPos, unsigned long c,
                           unsigned long d, unsigned long e, unsigned long f,
                           unsigned long g);        // 0x005D7680
    void ApplyVolumes(int nFlag);                   // 0x005D8110
    void StopAllOneShots();                         // 0x005D8220
    void SetSoundMode(unsigned long nMode);         // 0x005D8560
    void Render();                                  // 0x005D9590
    void Initialize(int n3DVoices, int n2DVoices, unsigned long a, unsigned long b); // 0x005DA8A0

    unsigned long m_n0;                             // 0x00
    unsigned long m_n4;                             // 0x04
    char pad_0x8[0x4];
    int m_bMuted;                                   // 0x0c
    unsigned char m_n10;                            // 0x10
    char pad_0x11[0xf];
    unsigned long m_nDesiredEnvironment;            // 0x20
    float m_fDesiredAmbientScale;                   // 0x24
    unsigned char m_nNumber3DVoices;                // 0x28
    unsigned char m_nNumber2DVoices;                // 0x29
    char pad_0x2a[0x1c];
    unsigned char m_n46;                            // 0x46
    char pad_0x47[0x5];
    CPriorityGroup* m_pPriorityGroups;              // 0x4c
    unsigned long m_nFadeStart;                     // 0x50
    unsigned long m_nFadeOutTime;                   // 0x54
    unsigned long m_n58;                            // 0x58
    unsigned long m_nFadeInTime;                    // 0x5c
    float m_fScale60;                               // 0x60
    float m_fDialogVolume;                          // 0x64
    char pad_0x68[0x4];
    float m_fMusicVolumeCurrent;                    // 0x6c
    float m_fSoundEffectVolumeCurrent;              // 0x70
    float m_fDialogVolumeCurrent;                   // 0x74
    float m_fMusicVolume;                           // 0x78
    float m_fSoundEffectVolume;                     // 0x7c
    Vector m_vListenerFront;                        // 0x80
    Vector m_vListenerUp;                           // 0x8c
    Vector m_vListenerPosition;                     // 0x98
    char pad_0xa4[0x54];
    HPROVIDER m_hProvider;                          // 0xf8
    char pad_0xfc[0x4];
    unsigned long m_n100;                           // 0x100
    unsigned long m_n104;                           // 0x104
    char pad_0x108[0x10];
    int m_aModes[10];                               // 0x118
    int m_nMode;                                    // 0x140
    unsigned long m_n144;                           // 0x144
    char pad_0x148[0x4];
};

// Size 0xA0.
class CExoSoundSourceInternal {
public:
    CExoSoundSourceInternal(CExoSoundInternal* pSound);         // 0x005DAF50
    virtual ~CExoSoundSourceInternal();

    void SetResRef(const CResRef& cResRef, int bFlag);          // 0x005D5E90
    unsigned long GetProperty0x28();                // 0x005D64F0
    unsigned long GetOneShotDelay();                // 0x005D6500
    unsigned long GetProperty0x64();                // 0x005D6510
    unsigned char GetProperty0x85();                // 0x005D6520
    int IsHardwarePlayingSoundPleaseDontUseThis();  // 0x005DB0E0
    void SetLooping(int bLooping);                  // 0x005DB130
    void Stop();                                    // 0x005DB2B0
    void SetPosition(const Vector& vPos);           // 0x005DB2C0
    void Set3D(int b3D);                            // 0x005DB320
    int Play(int a, int b);                         // 0x005DB4D0
    void SetVolume(unsigned char nVolume, int nMs, float fScale);   // 0x005DB800
    int SetFixedVariance(float fVariance);          // 0x005DB980
    void SetPitchVariance(float fVariance);         // 0x005DBA40
    void SetPriorityGroup(unsigned char nGroup);    // 0x005DBB20

    char pad_0x4[0x20];
    unsigned long m_n24;                            // 0x24
    unsigned long m_n28;                            // 0x28
    char pad_0x2c[0x9];
    unsigned char m_nPriorityGroup;                 // 0x35
    char pad_0x36[0xe];
    CExoSoundInternal* m_pSound;                    // 0x44
    char pad_0x48[0x10];
    unsigned long m_n58;                            // 0x58
    unsigned long m_nOneShotDelay;                  // 0x5c
    char pad_0x60[0x4];
    unsigned long m_n64;                            // 0x64
    char pad_0x68[0x4];
    unsigned long m_n6C;                            // 0x6c
    unsigned long m_n70;                            // 0x70
    char pad_0x74[0x11];
    unsigned char m_n85;                            // 0x85
    char pad_0x86[0x1a];
};

// Size 0x84.
class CExoStreamingSoundSourceInternal {
public:
    CExoStreamingSoundSourceInternal(CExoSoundInternal* pSound);    // 0x005DBBE0
    ~CExoStreamingSoundSourceInternal();            // 0x005DBCA0

    unsigned char GetPriorityGroupIndex();          // 0x005D5FF0
    unsigned long GetProperty0x34();                // 0x005D6530
    CResRef* GetFilePlaying();                      // 0x005D6670
    unsigned long GetFileFormat();                 // 0x005D6680
    void SetReverbEnabled(int bEnabled);            // 0x005DBB90
    int IsPlaying();                                // 0x005DC300
    void SetLooping(int bLooping);                  // 0x005DC330
    void Stop();                                    // 0x005DC380
    void Pause();                                   // 0x005DC3C0
    void FadeAndStop(unsigned long nMs);            // 0x005DC480
    void SetVolume(unsigned char nVolume, int nMs, float fScale);   // 0x005DC930
    int Play(unsigned long a, unsigned long b, unsigned long c,
             unsigned long d, unsigned long e);     // 0x005DCA30
    void SetPriorityGroup(unsigned char nGroup);    // 0x005DCD50
    void Resume();                                  // 0x005DCDB0
    int GetPlayFailed();                            // 0x0063E7F0

    char pad_0x0[0x4];
    unsigned long m_n4;                             // 0x04
    unsigned char m_nPriorityGroup;                 // 0x08
    CResRef m_cFilePlaying;                         // 0x09
    char pad_0x19[0x1b];
    unsigned long m_n34;                            // 0x34
    char pad_0x38[0xc];
    unsigned short m_nFileFormat;                   // 0x44
    char pad_0x46[0x2];
    unsigned long m_n48;                            // 0x48
    unsigned long m_n4C;                            // 0x4c
    char pad_0x50[0x8];
    unsigned long m_n58;                            // 0x58
    Vector m_v5C;                                   // 0x5c
    char pad_0x68[0x1c];
};

// Public facade, vftable 0x0074C608.
class CExoSoundSource {
public:
    CExoSoundSource();                              // 0x005D5870
    CExoSoundSource(const CResRef& cResRef);        // 0x005D60E0
    virtual ~CExoSoundSource();

    void SetPriorityGroup(unsigned char nGroup);
    void Set3D(int b3D);
    int IsHardwarePlayingSoundPleaseDontUseThis();
    int Play();
    void SetVolume(unsigned char nVolume);
    void SetPitchVariance(float fVariance);
    void SetLooping(int bLooping);
    void SetPosition(const Vector& vPos, float fHeight);
    void Stop();
    int SetFixedVariance(float fVariance);
    unsigned long GetProperty0x24();
    void SetProperty0x70(unsigned long a, unsigned long b);
    void SetResRef(const CResRef& cResRef, int bFlag);

    CExoSoundSourceInternal* m_pInternal;           // 0x04
};

class CExoStreamingSoundSource {
public:
    CExoStreamingSoundSource();                     // 0x005D5A50
    ~CExoStreamingSoundSource();                    // 0x005D6200

    int IsPlaying();
    int GetPlayFailed();
    int Play(unsigned long a, unsigned long b, unsigned long c,
             unsigned long d, unsigned long e);
    void Pause();
    void Resume();
    void SetLooping(int bLooping);
    void SetVolume(unsigned char nVolume);
    void Stop();
    void FadeAndStop(unsigned long nMs);
    void SetPriorityGroup(unsigned char nGroup);
    void SetReverbEnabled(int bEnabled);
    CResRef GetFilePlaying();
    unsigned long GetProperty0x04();
    void SetProperty0x58(unsigned long n);
    void SetProperty0x48(unsigned long a, unsigned long b);
    void SetProperty0x5C(const Vector& v);

    CExoStreamingSoundSourceInternal* m_pInternal;  // 0x00
};

class CExoSound {
public:
    CExoSound(unsigned long a, unsigned long b, unsigned long c, unsigned long d); // 0x005D5BC0
    ~CExoSound();                                   // 0x005D62F0

    void SetMusicVolume(float fVolume, int bApply);
    void SetSoundEffectVolume(float fVolume);
    void SetDialogVolume(float fVolume);
    void Render();
    void SetListenerOrientation(const Vector& a, const Vector& b);
    void SetListenerPosition(const Vector& a);
    int PlayOneShotSound(unsigned long a, unsigned long b, unsigned long c,
                         unsigned long d, unsigned long e, unsigned long f);
    int PlayOneShotSoundAt(unsigned long a, Vector vPos, float fHeight,
                           unsigned long c, unsigned long d, unsigned long e,
                           unsigned long f, unsigned long g);
    void StopAllOneShots();
    void SetSoundMode(unsigned long nMode);
    void SetScale60(float fScale);
    float GetScale60();
    unsigned char GetNumber3DVoices();
    unsigned char GetNumber2DVoices();
    void GetListenerPosition(Vector* pOut);
    void ReInitialize(unsigned long a, unsigned long b);
    void SetProperty0x00(unsigned long n);
    void SetProperty0x04(unsigned long n);
    unsigned long GetProperty0x100();
    unsigned long GetProperty0x104();
    void SetDesiredEnvironment(unsigned long n);
    void SetDesiredAmbientScale(float f);
    int GetCurrentMode();
    void GetPriorityGroupInfo(unsigned char nGroup, unsigned long* pC, unsigned long* p8);
    unsigned long GetProperty0x144();

    CExoSoundInternal* m_pInternal;                 // 0x00
};

extern CExoSound* g_pExoSound;                      // 0x007A39EC

#endif
