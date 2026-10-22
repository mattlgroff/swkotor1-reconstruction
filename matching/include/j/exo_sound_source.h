// Worker J matching headers: CExoSoundSourceInternal (0x005DAE50-0x005DBB7E)
// and CExoStreamingSoundSourceInternal (0x005DBB90-0x005DCE22). Standalone
// view of the sound classes on top of j/res.h's CResHelper (j/exo_sound.h
// and j/exo_sound_list.h define their own CResRef / CPriorityGroup, so the
// three headers are never included together).
#ifndef J_EXO_SOUND_SOURCE_H
#define J_EXO_SOUND_SOURCE_H

#include "j/res.h"

struct Vector {
    Vector() { x = 0; y = 0; z = 0; }
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    float x, y, z;
};

inline Vector operator-(Vector a, Vector b)
{
    return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
}

float __cdecl FUN_004ab100(const Vector* v);        // 0x004AB100 (magnitude)

extern unsigned char g_nDefaultSourceVolume;        // 0x0074C5E8

class CExoTimers {
public:
    unsigned long GetLowResolutionTimer();          // 0x005E9250
    unsigned __int64 GetHighResolutionTimer();      // 0x005E9260
};

class CExoAliasList {
public:
    CExoString ResolveFileName(const CExoString& sName, unsigned short nResType);  // 0x005E68A0
};

class CExoBase {
public:
    char pad_0x0[0x4];
    CExoTimers* m_pcExoTimers;                      // 0x04
    char pad_0x8[0x4];
    CExoAliasList* m_pcExoAliasList;                // 0x0c
};

extern CExoBase* g_pExoBase;                        // 0x007A39E0

class CClientExoApp {
public:
    int GetCreditSequenceInProgress();              // 0x005EE1E0
};

class CAppManager {
public:
    char pad_0x0[0x4];
    CClientExoApp* m_pClientExoApp;                 // 0x04
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

// Miles Sound System (IAT 0x0073D4xx-0x0073D5xx).
typedef void* HSAMPLE;
typedef void* H3DSAMPLE;
typedef void* HSTREAM;
extern "C" {
__declspec(dllimport) void __stdcall AIL_set_3D_position(void* obj, float x, float y, float z);
__declspec(dllimport) void __stdcall AIL_set_3D_sample_obstruction(H3DSAMPLE s, float f);
__declspec(dllimport) unsigned long __stdcall AIL_sample_status(HSAMPLE s);
__declspec(dllimport) unsigned long __stdcall AIL_3D_sample_status(H3DSAMPLE s);
__declspec(dllimport) void __stdcall AIL_stop_sample(HSAMPLE s);
__declspec(dllimport) long __stdcall AIL_sample_position(HSAMPLE s);
__declspec(dllimport) void __stdcall AIL_stop_3D_sample(H3DSAMPLE s);
__declspec(dllimport) long __stdcall AIL_3D_sample_offset(H3DSAMPLE s);
__declspec(dllimport) void __stdcall AIL_end_sample(HSAMPLE s);
__declspec(dllimport) void __stdcall AIL_end_3D_sample(H3DSAMPLE s);
__declspec(dllimport) void __stdcall AIL_set_3D_sample_playback_rate(H3DSAMPLE s, long n);
__declspec(dllimport) void __stdcall AIL_set_sample_playback_rate(HSAMPLE s, long n);
__declspec(dllimport) void __stdcall AIL_set_3D_sample_volume(H3DSAMPLE s, float f);
__declspec(dllimport) void __stdcall AIL_set_sample_volume_levels(HSAMPLE s, float l, float r);
__declspec(dllimport) void __stdcall AIL_close_stream(HSTREAM s);
__declspec(dllimport) unsigned long __stdcall AIL_stream_status(HSTREAM s);
__declspec(dllimport) void __stdcall AIL_set_stream_loop_count(HSTREAM s, long n);
__declspec(dllimport) void __stdcall AIL_pause_stream(HSTREAM s, long n);
__declspec(dllimport) long __stdcall AIL_stream_position(HSTREAM s);
__declspec(dllimport) void __stdcall AIL_set_stream_position(HSTREAM s, long n);
__declspec(dllimport) void __stdcall AIL_set_stream_volume_levels(HSTREAM s, float l, float r);
__declspec(dllimport) void __stdcall AIL_init_sample(HSAMPLE s);
__declspec(dllimport) int __stdcall AIL_set_sample_file(HSAMPLE s, void* pFile, long nBlock);
__declspec(dllimport) void __stdcall AIL_set_sample_loop_count(HSAMPLE s, long n);
__declspec(dllimport) void __stdcall AIL_set_sample_position(HSAMPLE s, unsigned long n);
__declspec(dllimport) void __stdcall AIL_resume_sample(HSAMPLE s);
}

// A hardware voice slot (CExoSoundInternal 2D / 3D voice tables).
struct CExoSoundVoice {
    void* m_pOwner;                                 // 0x00
    void* m_hSample;                                // 0x04
};

// Size 0x18.
class CPriorityGroup {
public:
    float GetMaxDistance() { return m_fMaxDistance; }
    float GetMinDistance() { return m_fMinDistance; }
    float GetPitchVariance() { return m_fPitchVariance; }

    int m_n0;                                       // 0x00
    unsigned char m_nMaxPlayer;                     // 0x04
    unsigned char m_nPlaying;                       // 0x05
    unsigned char m_n6;                             // 0x06
    unsigned char m_nVolume;                        // 0x07
    float m_fMaxDistance;                           // 0x08
    float m_fMinDistance;                           // 0x0c
    float m_fPitchVariance;                         // 0x10
    unsigned short m_n14;                           // 0x14
};

typedef void* CExoLinkedListPosition;

class CExoLinkedListInternal {
public:
    void* GetAtPos(CExoLinkedListPosition pos);     // 0x005E9B60
    void* GetNext(CExoLinkedListPosition& pos);     // 0x005E9B90

    void* m_pHead;                                  // 0x00
    void* m_pTail;                                  // 0x04
    unsigned long m_nCount;                         // 0x08
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedListPosition GetHeadPos() { return m_pcExoLinkedListInternal->m_pHead; }
    T* GetAtPos(CExoLinkedListPosition pos) { return (T*)m_pcExoLinkedListInternal->GetAtPos(pos); }
    T* GetNext(CExoLinkedListPosition& pos) { return (T*)m_pcExoLinkedListInternal->GetNext(pos); }

    CExoLinkedListInternal* m_pcExoLinkedListInternal;
};

// Miles 3D sample / room API used by the voice binders (IAT 0x0073D4A0-0x0073D4F8).
typedef void* HPROVIDER;
extern "C" {
__declspec(dllimport) void __stdcall AIL_set_3D_sample_file(H3DSAMPLE s, void* pFile);
__declspec(dllimport) void __stdcall AIL_set_3D_sample_loop_count(H3DSAMPLE s, long n);
__declspec(dllimport) void __stdcall AIL_set_3D_sample_distances(H3DSAMPLE s, float fMax, float fMin);
__declspec(dllimport) void __stdcall AIL_set_3D_sample_offset(H3DSAMPLE s, unsigned long n);
__declspec(dllimport) void __stdcall AIL_set_3D_sample_effects_level(H3DSAMPLE s, float f);
__declspec(dllimport) int __stdcall AIL_3D_room_type(HPROVIDER lib);
__declspec(dllimport) void __stdcall AIL_resume_3D_sample(H3DSAMPLE s);

// Miles AILSOUNDINFO.
struct AILSOUNDINFO {
    long format;                                    // 0x00
    void* data_ptr;                                 // 0x04
    unsigned long data_len;                         // 0x08
    unsigned long rate;                             // 0x0c
    long bits;                                      // 0x10
    long channels;                                  // 0x14
    unsigned long samples;                          // 0x18
    unsigned long block_size;                       // 0x1c
    void* initial_ptr;                              // 0x20
};
__declspec(dllimport) int __stdcall AIL_WAV_info(void* data, AILSOUNDINFO* info);
__declspec(dllimport) void __stdcall AIL_start_stream(HSTREAM s);
__declspec(dllimport) void __stdcall AIL_stream_ms_position(HSTREAM s, long* pTotal, long* pCurrent);
}

float __stdcall FUN_0050ccf0(Vector vFrom, Vector vTo);     // 0x0050CCF0 (obstruction ray test)

class CExoSoundSourceInternal;
class CExoStreamingSoundSourceInternal;

extern "C" int __cdecl toupper(int c);

// Public streaming facade (size 4).
class CExoStreamingSoundSource {
public:
    CExoStreamingSoundSource();                     // 0x005D5A50
    ~CExoStreamingSoundSource();                    // 0x005D6200
    unsigned long Play(const CResRef& cRes, int a, int b, int c, int d);   // 0x005D5B10
    void SetVolume(unsigned char n);                // 0x005D5B50
    void Stop();                                    // 0x005D5B80
    void SetPriorityGroup(unsigned char n);         // 0x005D5BA0
    void Set3D(int b);                              // 0x005D6290
    void SetDistances(float fMin, float fMax);      // 0x005D62A0
    void SetPosition(const Vector& v);              // 0x005D62C0

    CExoStreamingSoundSourceInternal* m_pInternal;  // 0x00
};

class CExoSoundInternal {
public:
    int GetSourceVolumeType(CExoSoundSourceInternal* pSource);  // 0x005D6A90
    float GetVolume(int nType, int bCurrent);       // 0x005D6B10
    void Resume3DSource(CExoSoundSourceInternal* pSource, CExoSoundVoice* pVoice); // 0x005D71C0
    void Resume2DSource(CExoSoundSourceInternal* pSource, CExoSoundVoice* pVoice); // 0x005D7480
    void RemoveFromPlayingList(CExoSoundSourceInternal* pSource);           // 0x005D78F0
    void RemoveFromNonPlayingLoopingList(CExoSoundSourceInternal* pSource); // 0x005D79C0
    void RemoveFromNonPlayingStreamingLoopingList(CExoStreamingSoundSourceInternal* pSource); // 0x005D7A00
    void RemoveFromDelayedOneShotList(CExoSoundSourceInternal* pSource);    // 0x005D7A70
    void RemoveFromStreamingList(CExoStreamingSoundSourceInternal* pSource); // 0x005D7AB0
    void AddToDelayedOneShotList(CExoSoundSourceInternal* pSource);         // 0x005D7BB0
    Vector GetListenerVector80();                   // 0x005DBBB0
    unsigned long GetNextSourceId()
    {
        unsigned long nId = m_nNextSourceId;
        m_nNextSourceId = nId + 1;
        return nId;
    }
    int IsSourceMuted(CExoSoundSourceInternal* pSource);        // 0x006054E0
    int IsStreamMuted(CExoStreamingSoundSourceInternal* pSource);   // 0x006054E0 (same body)
    int GetStreamVolumeType(CExoStreamingSoundSourceInternal* pSource);    // 0x005D6AD0

    int PlayOneShotSoundAt(const CResRef& cResRef, Vector vPos, unsigned char nGroup, unsigned long nDelay,
                           unsigned char nVolume, float fFixedVariance, float fPitchVariance); // 0x005D7680
    int PlayOneShotSound(const CResRef& cResRef, unsigned char nGroup, unsigned long nDelay,
                         unsigned char nVolume, float fFixedVariance, float fPitchVariance); // 0x005D7550
    int FindCachedObstruction(const Vector* pPos, Vector* pListener, unsigned long* pTime,
                              float* pObstruction);    // 0x005D6EF0
    int IsSourceAudible(CExoSoundSourceInternal* pSource);      // 0x005D6B70
    void AddToPlayingList(CExoSoundSourceInternal* pSource);    // 0x005D7B40
    void AddToNonPlayingLoopingList(CExoSoundSourceInternal* pSource);  // 0x005D7B80
    void InterruptPlayingSound(unsigned char nGroup);           // 0x005D7FE0
    CExoSoundVoice* GetFree3DVoice(unsigned char nGroup);       // 0x005D8EB0
    CExoSoundVoice* GetFree2DVoice(unsigned char nGroup);       // 0x005D8FF0
    int IsStreamAudible(CExoStreamingSoundSourceInternal* pSource);     // 0x005D6BC0
    void AddToNonPlayingStreamingLoopingList(CExoStreamingSoundSourceInternal* pSource); // 0x005D7BA0
    void InterruptPlayingStream(unsigned char nGroup);          // 0x005D80A0
    int AddToStreamingList(CExoStreamingSoundSourceInternal* pSource);  // 0x005D8470

    float GetSourceVolumeScale() { return m_fSourceVolumeScale; }
    float GetEffectsLevel() { return m_fEffectsLevel; }

    char pad_0x0[0x4];
    int m_bUseEAX;                                  // 0x04
    void* m_hDriver;                                // 0x08
    char pad_0xc[0x8];
    float m_fSourceVolumeScale;                     // 0x14
    char pad_0x18[0x28];
    CExoLinkedList<CExoSoundSourceInternal> m_lPlaying;             // 0x40
    CExoLinkedList<CExoStreamingSoundSourceInternal> m_lStreaming;  // 0x44
    char pad_0x48[0x4];
    CPriorityGroup* m_pPriorityGroups;              // 0x4c
    char pad_0x50[0x8];
    unsigned long m_nNextSourceId;                  // 0x58
    char pad_0x5c[0x4];
    float m_fScale60;                               // 0x60
    char pad_0x64[0x4];
    float m_fEffectsLevel;                          // 0x68
    char pad_0x6c[0x14];
    Vector m_vListener80;                           // 0x80
    char pad_0x8c[0xc];
    Vector m_vListenerPosition;                     // 0x98
    char pad_0xa4[0x14];
    unsigned long m_nMaxObstructionTests;           // 0xb8
    char pad_0xbc[0x8];
    unsigned long m_nObstructionTime;               // 0xc4
    unsigned long m_nObstructionTests;              // 0xc8
    char pad_0xcc[0x2c];
    HPROVIDER m_hProvider;                          // 0xf8
    char pad_0xfc[0x48];
    int m_bModePaused;                              // 0x144
    CExoStreamingSoundSource* m_pOneShotStream;     // 0x148
};

// Size 0xA0, vftable 0x0074D2E8 (CResHelper<CResWave,4> base 0x0074D2E4).
class CExoSoundSourceInternal : public CResHelper<CResWave, 4> {
public:
    CExoSoundSourceInternal(CExoSoundInternal* pSound);         // 0x005DAF50
    virtual ~CExoSoundSourceInternal();                         // 0x005DB480

    int Demand();                                   // 0x005DAE50
    int Release();                                  // 0x005DAE90
    void DoObstructionWork();                       // 0x005DAEB0
    int IsInAudibleRange();                         // 0x005DB050
    int IsHardwarePlayingSoundPleaseDontUseThis();  // 0x005DB0E0
    void SetLooping(int bLooping);                  // 0x005DB130
    void Pause();                                   // 0x005DB150
    void Resume();                                  // 0x005DB1C0
    void ShutDownSource();                          // 0x005DB210
    void Stop();                                    // 0x005DB2B0
    void SetPosition(const Vector& vPos);           // 0x005DB2C0
    void Set3D(int b3D);                            // 0x005DB320
    void SetOneShotDelay(unsigned long nDelay);     // 0x005DB340
    void StoreRestartSoundPosition();               // 0x005DB380
    void ApplyPitchVariance();                      // 0x005DB3D0
    void ApplyFixedVariance();                      // 0x005DB450
    int Play(int a, int b);                         // 0x005DB4D0
    void SetVolume(unsigned char nVolume, int nMs, float fScale);   // 0x005DB800
    int SetFixedVariance(float fVariance);          // 0x005DB980
    void SetPitchVariance(float fVariance);         // 0x005DBA40
    void SetPriorityGroup(unsigned char nGroup);    // 0x005DBB20
    void SetDistances(float fMin, float fMax)
    {
        m_fMinDistance = fMin;
        m_fMaxDistance = fMax;
    }

    int m_b3D;                                      // 0x1c
    char pad_0x20[0x4];
    int m_bLooping;                                 // 0x24
    unsigned long m_n28;                            // 0x28
    int m_bPlayFailed;                              // 0x2c
    int m_bDemanded;                                // 0x30
    unsigned char m_nList;                          // 0x34
    unsigned char m_nPriorityGroup;                 // 0x35
    char pad_0x36[0x2];
    CExoSoundVoice* m_p2DVoice;                     // 0x38
    CExoSoundVoice* m_p3DVoice;                     // 0x3c
    void* m_pos;                                    // 0x40
    CExoSoundInternal* m_pSound;                    // 0x44
    unsigned long m_nSampleRate;                    // 0x48
    char pad_0x4c[0x4];
    unsigned long m_nFixedRate;                     // 0x50
    unsigned long m_nPitchRate;                     // 0x54
    unsigned long m_n58;                            // 0x58
    unsigned long m_nOneShotDelay;                  // 0x5c
    unsigned long m_nId;                            // 0x60
    unsigned long m_nDelayStart;                    // 0x64
    float m_fFixedVariance;                         // 0x68
    float m_fMaxDistance;                           // 0x6c
    float m_fMinDistance;                           // 0x70
    float m_fPitchVariance;                         // 0x74
    Vector m_vPosition;                             // 0x78
    unsigned char m_nGroupVolume;                   // 0x84
    unsigned char m_nVolume;                        // 0x85
    char pad_0x86[0x2];
    long m_nRestartPosition;                        // 0x88
    Vector m_v8C;                                   // 0x8c
    unsigned long m_n98;                            // 0x98
    float m_fObstruction;                           // 0x9c
};

// Size 0x84.
class CExoStreamingSoundSourceInternal {
public:
    CExoStreamingSoundSourceInternal(CExoSoundInternal* pSound);    // 0x005DBBE0
    ~CExoStreamingSoundSourceInternal();            // 0x005DBCA0

    void SetReverbEnabled(int bEnabled);            // 0x005DBB90
    int IsInAudibleRange();                         // 0x005DC4D0
    int IsPlaying();                                // 0x005DC300
    void SetLooping(int bLooping);                  // 0x005DC330
    void Stop();                                    // 0x005DC380
    void Pause();                                   // 0x005DC3C0
    void FadeAndStop(unsigned long nMs);            // 0x005DC480
    void SetVolume(unsigned char nVolume, int nMs, float fScale);   // 0x005DC930
    void ApplyPositionalVolume();                   // 0x005DC560
    void SetPriorityGroup(unsigned char nGroup);    // 0x005DCD50
    void SetDistances(float fMin, float fMax)
    {
        m_fMinDistance = fMin;
        m_fMaxDistance = fMax;
    }
    void Resume();                                  // 0x005DCDB0
    void OpenStream();                              // 0x005DBD30
    unsigned long Play(const CResRef& cResRef, unsigned short nResType, int bVoice, int bMusic,
                       int nUnused);                // 0x005DCA30

    HSTREAM m_hStream;                              // 0x00
    int m_bLooping;                                 // 0x04
    unsigned char m_nPriorityGroup;                 // 0x08
    CResRef m_cFilePlaying;                         // 0x09
    char pad_0x19[0x3];
    int m_bInStreamingList;                         // 0x1c
    CExoSoundInternal* m_pSound;                    // 0x20
    CExoString m_sFile;                             // 0x24
    unsigned long m_nFadeTime;                      // 0x2c
    unsigned long m_nId;                            // 0x30
    unsigned long m_nFadeStart;                     // 0x34
    void* m_pBuffer;                                // 0x38
    char pad_0x3c[0x4];
    long m_nPausePosition;                          // 0x40
    unsigned short m_nResType;                      // 0x44
    unsigned char m_nVolume;                        // 0x46
    char pad_0x47[0x1];
    float m_fMinDistance;                           // 0x48
    float m_fMaxDistance;                           // 0x4c
    unsigned char m_nGroupVolume;                   // 0x50
    char pad_0x51[0x3];
    float m_f54;                                    // 0x54
    int m_b3D;                                      // 0x58
    Vector m_vPosition;                             // 0x5c
    int m_bReverbEnabled;                           // 0x68
    int m_bReverbChanged;                           // 0x6c
    Vector m_v70;                                   // 0x70
    unsigned long m_n7C;                            // 0x7c
    unsigned long m_n80;                            // 0x80
};

#endif
