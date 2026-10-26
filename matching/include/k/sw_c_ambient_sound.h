// Worker K matching headers: client area ambient sound (CSWCAmbientSound,
// vtable 0x007541C8, abstract base vtable 0x00747470),
// 0x0068D3D0-0x0068F0D9.
#ifndef K_SW_C_AMBIENT_SOUND_H
#define K_SW_C_AMBIENT_SOUND_H

class CExoString {
public:
    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04

    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    CExoString& operator=(const char* s);           // 0x005E5140
    int operator!=(const char* s) const;            // 0x005E5390
    int CompareNoCase(const CExoString& s) const;   // 0x005E6450
    CExoString UpperCase() const;                   // 0x005E6360
};

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
    void SetPosition(const struct Vector& v);       // 0x005D62C0
    void SetDistances(float fMin, float fMax);      // 0x005D62A0

    void* m_pInternal;                              // 0x00
};

class CExoSound {
public:
    int GetCurrentMode();                           // 0x005D6490
    int GetProperty0x144();                         // 0x005D64E0
};

extern CExoSound* g_pExoSound;                      // 0x007A39EC
extern const int g_nSoundModeMovie;                 // 0x0074C5F8
extern const int g_nSoundModeMenu;                  // 0x0074C5FC

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

class CAppManager {
public:
    char pad_0x0[0x4];
    CClientExoApp* m_pClientExoApp;                 // 0x04
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

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

extern CSWRules* g_pRules;                          // 0x007A3A28

class CNWMessage {
public:
    int ReadBOOL();                                 // 0x004D66F0
    unsigned char ReadBYTE(int nBits);              // 0x004D6720
    unsigned long ReadDWORD(int nBits);             // 0x004D6780
};

// One 2DA row of AmbientMusic / AmbientSound: the looping resource plus
// up to three stingers. Size 0x20.
class CSWCAmbientSoundList {
public:
    CSWCAmbientSoundList();                         // 0x0068D3D0
    ~CSWCAmbientSoundList();                        // 0x0068D430

    CExoString m_sResource;                         // 0x00
    CExoString m_sStinger1;                         // 0x08
    CExoString m_sStinger2;                         // 0x10
    CExoString m_sStinger3;                         // 0x18
};

// Shared with the server side (CSWSAmbientSound uses the same fields).
class CSWAmbientSound {
public:
    CSWAmbientSound()
    {
        m_bMusicPlaying = 1;
        m_nMusicDelay = 5000;
        m_nMusicDay = 2;
        m_nMusicNight = 3;
        m_bBattleMusicPlaying = 0;
        m_nMusicBattle = 1;
        m_bAmbientPlaying = 1;
        m_nAmbientSndDay = 1;
        m_nAmbientSndNight = 2;
        m_nAmbientSndDayVol = 0;
        m_nAmbientSndNitVol = 0;
    }
    ~CSWAmbientSound() {}

    virtual void PlayMusic(int bPlay) = 0;                  // 0x00
    virtual void SetMusicDelay(int nDelay) = 0;             // 0x04
    virtual void SetMusicDayTrack(int nTrack) = 0;          // 0x08
    virtual void SetMusicNightTrack(int nTrack) = 0;        // 0x0c
    virtual void PlayBattleMusic(int bPlay) = 0;            // 0x10
    virtual void SetBattleMusicTrack(int nTrack) = 0;       // 0x14
    virtual void PlayAmbientSound(int bPlay) = 0;           // 0x18
    virtual void SetAmbientDayTrack(int nTrack) = 0;        // 0x1c
    virtual void SetAmbientNightTrack(int nTrack) = 0;      // 0x20

    int m_bMusicPlaying;                            // 0x04
    int m_nMusicDelay;                              // 0x08
    int m_nMusicDay;                                // 0x0c
    int m_nMusicNight;                              // 0x10
    int m_bBattleMusicPlaying;                      // 0x14
    int m_nMusicBattle;                             // 0x18
    int m_bAmbientPlaying;                          // 0x1c
    int m_nAmbientSndDay;                           // 0x20
    int m_nAmbientSndNight;                         // 0x24
    unsigned char m_nAmbientSndDayVol;              // 0x28
    unsigned char m_nAmbientSndNitVol;              // 0x29
};

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
