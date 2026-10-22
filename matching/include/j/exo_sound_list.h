// Worker J matching headers: CExoSoundInternal source lists (0x005D6C10-
// 0x005D6D2F, 0x005D78F0-0x005D7BC6). Standalone view of the sound classes:
// j/exo_sound.h keeps the getter layout (its 0x46 byte overlaps the +0x44
// list pointer used here), so the two headers are never included together.
#ifndef J_EXO_SOUND_LIST_H
#define J_EXO_SOUND_LIST_H

typedef void* CExoLinkedListPosition;

class CExoLinkedListInternal {
public:
    CExoLinkedListInternal();                       // 0x005E9AA0
    ~CExoLinkedListInternal();                      // 0x005E9AB0
    void* GetAtPos(CExoLinkedListPosition pos);     // 0x005E9B60
    void* GetNext(CExoLinkedListPosition& pos);     // 0x005E9B90
    void* Remove(CExoLinkedListPosition pos);       // 0x005E9C10
    int Contains(void* p);                          // 0x005E9D40
    CExoLinkedListPosition AddHead(void* p);        // 0x005E9D70
    void* GetHead();

    void* m_pHead;                                  // 0x00
    void* m_pTail;                                  // 0x04
    unsigned long m_nCount;                         // 0x08
};

struct CExoLinkedListNode {
    CExoLinkedListNode* pPrev;                      // 0x00
    CExoLinkedListNode* pNext;                      // 0x04
    void* pObject;                                  // 0x08
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedList() { m_pcExoLinkedListInternal = new CExoLinkedListInternal(); }
    ~CExoLinkedList() { delete m_pcExoLinkedListInternal; }

    CExoLinkedListPosition AddHead(T* p) { return m_pcExoLinkedListInternal->AddHead(p); }
    int Contains(T* p) { return m_pcExoLinkedListInternal->Contains(p); }
    T* GetHead()
    {
        CExoLinkedListInternal* pInternal = m_pcExoLinkedListInternal;
        return (T*)pInternal->GetHead();
    }
    CExoLinkedListPosition GetHeadPos() { return m_pcExoLinkedListInternal->m_pHead; }
    T* Remove(CExoLinkedListPosition pos) { return (T*)m_pcExoLinkedListInternal->Remove(pos); }
    unsigned long GetCount() { return m_pcExoLinkedListInternal->m_nCount; }
    T* GetAtPos(CExoLinkedListPosition pos) { return (T*)m_pcExoLinkedListInternal->GetAtPos(pos); }
    T* GetNext(CExoLinkedListPosition& pos) { return (T*)m_pcExoLinkedListInternal->GetNext(pos); }

    CExoLinkedListInternal* m_pcExoLinkedListInternal;
};

inline void* CExoLinkedListInternal::GetHead()
{
    CExoLinkedListNode* pNode = (CExoLinkedListNode*)m_pHead;
    if (!pNode) {
        return 0;
    }
    return pNode->pObject;
}

// Size 0x18.
class CPriorityGroup {
public:
    CPriorityGroup();
    ~CPriorityGroup();                              // 0x004015A0 (folded empty)
    void AddPlaying() { m_nPlaying++; }
    void RemovePlaying() { m_nPlaying--; }
    void SetInterrupt(int n) { m_n0 = n; }
    void SetMaxPlaying(unsigned char n) { m_nMaxPlayer = n; }
    void SetPriority(unsigned char n) { m_n6 = n; }
    void SetVolume(unsigned char n) { m_nVolume = n; }
    void SetVolumeDist(float fMin, float fMax)
    {
        m_fF8 = fMin;
        m_fC = fMax;
    }
    void SetPlaybackVariance(float f)
    {
        m_fPlaybackVariance = !(0.0f > f) ? (1.0f < f ? 1.0f : f) : 0.0f;
    }
    void SetFadeTime(unsigned short n) { m_n14 = n; }

    int m_n0;                                       // 0x00
    unsigned char m_nMaxPlayer;                     // 0x04
    unsigned char m_nPlaying;                       // 0x05
    unsigned char m_n6;                             // 0x06
    unsigned char m_nVolume;                        // 0x07
    float m_fF8;                                    // 0x08
    float m_fC;                                     // 0x0c
    union {
        unsigned long m_n10;                        // 0x10
        float m_fPlaybackVariance;                  // 0x10
    };
    unsigned short m_n14;                           // 0x14
};

#include "j/exo_string.h"

class CResRef {
public:
    CResRef(const char* s);                         // 0x00406D80
    CResRef(const CResRef& r);
    void CopyToString(CExoString& s) const;         // 0x00405F70
    char m_resRef[16];
};

class C2DA {
public:
    C2DA(CResRef cResRef, int bPrivate);            // 0x00413CC0
    virtual ~C2DA();
    int Load2DArray();                              // 0x004143B0
    int GetINTEntry(int nRow, const CExoString& sColumn, int* pValue);       // 0x00413660
    int GetFLOATEntry(int nRow, const CExoString& sColumn, float* pValue);   // 0x00413430
    int GetCExoStringEntry(int nRow, const CExoString& sColumn, CExoString* pValue);   // 0x00413270
    void Unload2DArray();                           // 0x004139E0

    char pad_0x4[0x20];
    int m_nNumRows;                                 // 0x24
    char pad_0x28[0x2c];
    int GetNumRows() { return m_nNumRows; }
};

class CExoStringList {
public:
    CExoStringList();                               // 0x005E9530
    ~CExoStringList();                              // 0x005E9860
    void Clear();                                   // 0x005E9590
    CExoString* GetString(int nIndex);              // 0x005E95E0
    void Add(CExoString* pString);                  // 0x005E98B0
    int GetCount();                                 // 0x00675DD0 (folded getter)

    char pad_0x0[0xc];
    int m_bSorted;                                  // 0x0c
    char pad_0x10[0x4];
};

class CExoIni {
public:
    CExoIni();                                      // 0x005E6750
    ~CExoIni();                                     // 0x005E67E0
    void* m_pIniInternal;
};

class CExoSoundSourceInternal;
typedef void* HSAMPLE;
typedef void* HPROVIDER;
__declspec(dllimport) void __stdcall AIL_set_3D_sample_effects_level(HSAMPLE hSample, float fLevel);
__declspec(dllimport) int __stdcall AIL_3D_room_type(HPROVIDER hProvider);
__declspec(dllimport) void __stdcall AIL_quick_shutdown();
__declspec(dllimport) long __stdcall AIL_set_preference(unsigned long nNumber, long nValue);

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
__declspec(dllimport) void __stdcall AIL_close_3D_provider(HPROVIDER hProvider);
typedef unsigned long HPROENUM;
__declspec(dllimport) int __stdcall AIL_enumerate_3D_providers(HPROENUM* next, HPROVIDER* dest, char** name);
__declspec(dllimport) int __stdcall AIL_open_3D_provider(HPROVIDER lib);
__declspec(dllimport) void __stdcall AIL_release_sample_handle(HSAMPLE hSample);
__declspec(dllimport) void __stdcall AIL_release_3D_sample_handle(HSAMPLE hSample);

// One hardware voice slot (CExoSoundInternal +0x2C / +0x30 arrays, 0x10 bytes).
struct CExoSoundVoice {
    CExoSoundSourceInternal* m_pSource;             // 0x00
    HSAMPLE m_hSample;                              // 0x04
    int m_bReserved;                                // 0x08
    unsigned long m_nC;                             // 0x0c
};

class CExoSoundSourceInternal {
public:
    virtual ~CExoSoundSourceInternal();
    int IsHardwarePlayingSoundPleaseDontUseThis();  // 0x005DB0E0
    void Pause();                                   // 0x005DB150
    void Resume();                                  // 0x005DB1C0
    void ShutDownSource();                          // 0x005DB210
    int GetIgnorePause();                           // 0x0063E7F0 (folded return 0)
    void SetVolume(unsigned char nVolume, int nMs, float fScale);   // 0x005DB800

    char pad_0x4[0x8];
    CResRef m_cResRef;                              // 0x0c
    unsigned long m_n1C;                            // 0x1c
    char pad_0x20[0x4];
    int m_bLooping;                                 // 0x24
    int m_bDeleteWhenDone;                          // 0x28
    char pad_0x2c[0x8];
    unsigned char m_nList;                          // 0x34
    unsigned char m_nPriorityGroup;                 // 0x35
    char pad_0x36[0x6];
    CExoSoundVoice* m_pVoice;                       // 0x3c
    CExoLinkedListPosition m_pos;                   // 0x40
    char pad_0x44[0x41];
    unsigned char m_nVolume;                        // 0x85
};

class CExoStreamingSoundSourceInternal {
public:
    void Stop();                                    // 0x005DC380
    void Pause();                                   // 0x005DC3C0
    void Resume();                                  // 0x005DCDB0
    void SetVolume(unsigned char nVolume, int nMs, float fScale);   // 0x005DC930

    char pad_0x0[0x4];
    int m_bLooping;                                 // 0x04
    unsigned char m_nPriorityGroup;                 // 0x08
    char pad_0x9[0x13];
    CExoLinkedListPosition m_pos;                   // 0x1c
    char pad_0x20[0x26];
    unsigned char m_nVolume;                        // 0x46
};

extern int g_bSoundPaused;                          // 0x00832890
extern int g_nSoundModeBack;                        // 0x0074C5EC
extern int g_nSoundModeMuted;                       // 0x0074C5F0
extern int g_nSoundModeQuiet1;                      // 0x0074C5F4
extern int g_nSoundModeQuiet2;                      // 0x0074C5F8
extern int g_nSoundModeQuiet3;                      // 0x0074C5FC
extern int g_nSoundModePauseAll;                    // 0x0074C600
extern int g_nSoundModeResumeAll;                   // 0x0074C604

// One +0x108 entry, built from soundprovider.2da.
struct CSoundProviderInfo {
    int m_nProvider;                                // 0x00 (index in m_lProviders)
    float m_f2D3DBias;                              // 0x04
    int m_bHardware;                                // 0x08
    int m_bEAX;                                     // 0x0c
};

struct CSoundVector {
    CSoundVector() { x = 0; y = 0; z = 0; }
    float x, y, z;
};

// +0xEC / +0x108: {data, num, size} arrays.
struct CSoundArray {
    void Allocate(int n);                           // 0x005E03C0
    void Add(void* p)
    {
        if (num == array_size) {
            Allocate(array_size ? array_size * 2 : 16);
        }
        element[num] = p;
        num++;
    }
    CSoundArray()
    {
        num = 0;
        array_size = 0;
        element = 0;
    }
    ~CSoundArray()
    {
        delete[] element;
        element = 0;
    }
    void Clear()
    {
        num = 0;
        if (element) {
            delete[] element;
            element = 0;
            array_size = 0;
        }
    }
    void** element;                                 // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08
};

extern int g_bSoundDisabled;                        // 0x00832894

class CExoSoundInternal {
public:
    CExoSoundInternal();                            // 0x005D86E0
    ~CExoSoundInternal();                           // 0x005D9400
    void InitPriorityGroups();                      // 0x005D7BD0
    void LoadProviderSettings();                    // 0x005D9140
    void EnumerateProviders();                      // 0x005DA6D0
    void ShutDown();                                // 0x005D6D30
    CExoLinkedListPosition RemoveFromPlayingList(CExoSoundSourceInternal* pSource);
    CExoLinkedListPosition RemoveFromNonPlayingLoopingList(CExoSoundSourceInternal* pSource);
    CExoLinkedListPosition RemoveFromNonPlayingStreamingLoopingList(CExoStreamingSoundSourceInternal* pSource);
    CExoLinkedListPosition RemoveFromDelayedOneShotList(CExoSoundSourceInternal* pSource);
    CExoLinkedListPosition RemoveFromStreamingList(CExoStreamingSoundSourceInternal* pSource);
    void AddToPlayingList(CExoSoundSourceInternal* pSource);
    void AddToNonPlayingLoopingList(CExoSoundSourceInternal* pSource);
    CExoLinkedListPosition AddToNonPlayingStreamingLoopingList(CExoStreamingSoundSourceInternal* pSource);
    void AddToDelayedOneShotList(CExoSoundSourceInternal* pSource);
    void MuteSound();                               // 0x005D6A10
    void UnMuteSound();                             // 0x005D6A50
    void InterruptPlayingSound(unsigned char nGroup);           // 0x005D7FE0
    void InterruptPlayingStream(unsigned char nGroup);          // 0x005D80A0
    void StopAllOneShots();                         // 0x005D8220
    int PauseAllSounds(int bAll);                   // 0x005D82C0
    int ResumeAllSounds();                          // 0x005D83F0
    int AddToStreamingList(CExoStreamingSoundSourceInternal* pSource);  // 0x005D8470
    void SetSoundMode(int nMode);                   // 0x005D8560
    CExoSoundVoice* GetFree3DVoice(unsigned char nGroup);       // 0x005D8EB0
    CExoSoundVoice* GetFree2DVoice(unsigned char nGroup);       // 0x005D8FF0
    void ApplyVolumes(int nMs);                     // 0x005D8110
    float GetEffectsLevel() { return m_fEffectsLevel; }

    int m_n0;                                       // 0x00
    int m_bUseEAX;                                  // 0x04
    int m_n8;                                       // 0x08
    int m_nC;                                       // 0x0c
    unsigned char m_n10;                            // 0x10
    float m_f14;                                    // 0x14
    float m_f18;                                    // 0x18
    float m_f1C;                                    // 0x1c
    int m_n20;                                      // 0x20
    float m_f24;                                    // 0x24
    unsigned char m_nNumber2DVoicesX2;              // 0x28
    unsigned char m_nNumber3DVoices;                // 0x29
    unsigned char m_n2A;                            // 0x2a
    CExoSoundVoice* m_p2DVoices;                    // 0x2c
    CExoSoundVoice* m_p3DVoices;                    // 0x30
    CExoLinkedList<CExoSoundSourceInternal> m_lDelayedOneShots;                 // 0x34
    CExoLinkedList<CExoSoundSourceInternal> m_lNonPlayingLooping;               // 0x38
    CExoLinkedList<CExoStreamingSoundSourceInternal> m_lNonPlayingStreamingLooping; // 0x3c
    CExoLinkedList<CExoSoundSourceInternal> m_lPlaying;                         // 0x40
    CExoLinkedList<CExoStreamingSoundSourceInternal> m_lStreaming;              // 0x44
    CExoLinkedList<CExoSoundSourceInternal> m_lPendingDelete;                   // 0x48
    CPriorityGroup* m_pPriorityGroups;              // 0x4c
    int m_n50;                                      // 0x50
    int m_n54;                                      // 0x54
    int m_n58;                                      // 0x58
    int m_n5C;                                      // 0x5c
    float m_f60;                                    // 0x60
    float m_f64;                                    // 0x64
    float m_fEffectsLevel;                          // 0x68
    float m_f6C;                                    // 0x6c
    float m_f70;                                    // 0x70
    float m_f74;                                    // 0x74
    float m_f78;                                    // 0x78
    float m_f7C;                                    // 0x7c
    CSoundVector m_v80;                             // 0x80
    CSoundVector m_v8C;                             // 0x8c
    CSoundVector m_v98;                             // 0x98
    CExoLinkedListPosition m_posIterator;           // 0xa4
    int m_nIteratorList;                            // 0xa8
    int m_nAC;                                      // 0xac
    int m_nB0;                                      // 0xb0
    int m_nB4;                                      // 0xb4
    int m_nB8;                                      // 0xb8
    int m_nBC;                                      // 0xbc
    int m_nC0;                                      // 0xc0
    int m_nC4;                                      // 0xc4
    int m_nC8;                                      // 0xc8
    int m_nCC;                                      // 0xcc
    CExoString m_sProviderName;                     // 0xd0
    CExoStringList m_lProviders;                    // 0xd8
    CSoundArray m_aArrayEC;                         // 0xec
    HPROVIDER m_hProvider;                          // 0xf8
    char pad_0xfc[0x8];
    int m_nMaxEAX;                                  // 0x104
    CSoundArray m_aArray108;                        // 0x108
    unsigned long m_nStartTime;                     // 0x114
    int m_aModes[10];                               // 0x118
    int m_nMode;                                    // 0x140
    int m_bModePaused;                              // 0x144
    int m_n148;                                     // 0x148
};

#endif
