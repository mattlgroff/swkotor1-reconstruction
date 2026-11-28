// Worker K matching headers: client area ambient sound (CSWCAmbientSound,
// vtable 0x007541C8, abstract base vtable 0x00747470),
// 0x0068D3D0-0x0068F0D9.
#ifndef K_SW_C_AMBIENT_SOUND_H
#define K_SW_C_AMBIENT_SOUND_H

#include "shared/exo_string.h"

class CResRef {
public:
    char m_resRef[16];
    CResRef();                                      // 0x00405ED0
    CResRef(const CExoString& s);                   // 0x00406D60
    CResRef operator=(const CExoString& s);         // 0x004061F0
    CResRef operator=(const CResRef& r);            // 0x00406120
    void CopyToString(CExoString& s) const;         // 0x00405F70
};

class CExoStreamingSoundSource {
public:
    CExoStreamingSoundSource();                     // 0x005D5A50
    ~CExoStreamingSoundSource();                    // 0x005D6200

    int IsPlaying();                                // 0x005D5AF0
    int Play(const CResRef& cResRef, int nFormat, int a, int bLooping, int b);  // 0x005D5B10
    void SetLooping(int bLooping);                  // 0x005D5B40
    void SetVolume(unsigned char nVolume);          // 0x005D5B50
    void Stop();                                    // 0x005D5B80
    void FadeAndStop(unsigned long nMs);            // 0x005D5B90
    void SetPriorityGroup(unsigned char nGroup);    // 0x005D5BA0
    CResRef GetFilePlaying();                       // 0x005D6230
    void Set3D(int b3D);                            // 0x005D6290
    void SetPosition(const class Vector& v);       // 0x005D62C0
    void SetDistances(float fMin, float fMax);      // 0x005D62A0

    void* m_pInternal;                              // 0x00
};

class CExoSound {
public:
    int GetCurrentMode();                           // 0x005D6490
    int GetProperty0x144();                         // 0x005D64E0
};

extern CExoSound* g_pExoSound;                      // 0x007A39EC
#include "shared/globals.h"

class CWorldTimer {
public:
    void GetWorldTime(unsigned long* pCalendarDay, unsigned long* pTimeOfDay);   // 0x004ADE40
    int SubtractWorldTimes(unsigned long nDay1, unsigned long nTime1,
                           unsigned long nDay2, unsigned long nTime2,
                           unsigned long* pDay, unsigned long* pTime);           // 0x004AE460
};

class CSWCModule {
public:
    char pad_0x0[0xb4];
    unsigned char m_nDayState;                      // 0xb4
};

class CClientOptions {
public:
    char pad_0x0[0x73];
    unsigned char m_nMusicFlags;                    // 0x73
};

class CClientExoApp {
public:
    CWorldTimer* GetPauseTimer();                   // 0x005ED500
    CSWCModule* GetModule();                        // 0x005ED530
    CClientOptions* GetClientOptions();             // 0x005ED700
};

#include "shared/app_manager.h"


class C2DA {
public:
    int GetCExoStringEntry(int nRow, const CExoString& sColumn, CExoString* pValue);  // 0x00413270
};

class C2DArrays {
public:
    char pad_0x0[0x50];
    C2DA* m_pAmbientMusic;                          // 0x50
    C2DA* m_pAmbientSound;                          // 0x54
};

class CSWRules {
public:
    char pad_0x0[0xb8];
    C2DArrays* m_p2DArrays;                         // 0xb8
};


class CNWMessage {
public:
    int ReadBOOL();                                 // 0x004D66F0
    unsigned char ReadBYTE(int nBits);              // 0x004D6720
    unsigned long ReadDWORD(int nBits);             // 0x004D6780
};

// One 2DA row of AmbientMusic / AmbientSound: the looping resource plus
// up to three stingers. Size 0x20.
#include "shared/swc_ambient_sound_list.h"

// Shared with the server side (CSWSAmbientSound uses the same fields).
#include "shared/sw_ambient_sound.h"

class CSWCArea;

class CSWCAmbientSound : public CSWAmbientSound {
public:
    CSWCAmbientSound(CSWCArea* pArea);        // 0x0068DF40
    ~CSWCAmbientSound();                            // 0x0068E0D0

    virtual void PlayMusic(int bPlay);
    virtual void SetMusicDelay(int nDelay);
    virtual void SetMusicDayTrack(int nTrack);
    virtual void SetMusicNightTrack(int nTrack);
    virtual void PlayBattleMusic(int bPlay);
    virtual void SetBattleMusicTrack(int nTrack);
    virtual void PlayAmbientSound(int bPlay);
    virtual void SetAmbientDayTrack(int nTrack);
    virtual void SetAmbientNightTrack(int nTrack);

    void Activate();
    void SetAmbientDayVolume(int nVolume);
    void SetAmbientNightVolume(int nVolume);
    void UpdateDayNight();
    void LoadSoundList(int bMusic, int nRow, CSWCAmbientSoundList* pList);
    void PlayStinger(CSWCAmbientSoundList* pList);
    void ResetTimer();
    unsigned long GetElapsedTime();
    void PlayTruncated(int bPlay, int nKind);
    void StopAll(int bImmediate);
    void LoadFromMessage(CNWMessage* pMessage);
    void Update();

    CSWCAmbientSoundList m_lstMusicDay;             // 0x2c
    CSWCAmbientSoundList m_lstMusicNight;           // 0x4c
    CSWCAmbientSoundList m_lstMusicBattle;          // 0x6c
    CSWCAmbientSoundList m_lstAmbientDay;           // 0x8c
    CSWCAmbientSoundList m_lstAmbientNight;         // 0xac
    unsigned long m_nMusicDelayMs;                  // 0xcc
    unsigned long m_nTimer;                         // 0xd0
    unsigned long m_nTimerDay;                      // 0xd4
    unsigned long m_nTimerTime;                     // 0xd8
    unsigned long m_nUnknownDC;                     // 0xdc
    CExoStreamingSoundSource* m_pMusicSource;       // 0xe0
    CExoStreamingSoundSource* m_pAmbientSource;     // 0xe4
    CSWCArea* m_pArea;                        // 0xe8
    int m_bMusicEnabled;                            // 0xec
    int m_bAmbientEnabled;                          // 0xf0
    int m_bActive;                                  // 0xf4
    int m_bDay;                                     // 0xf8
    int m_nMusicMode;                               // 0xfc
    int m_nPlayingKind;                             // 0x100
    CExoStreamingSoundSource* m_pStingerSource;     // 0x104
    int m_bAmbientFailed;                           // 0x108
};

#endif
