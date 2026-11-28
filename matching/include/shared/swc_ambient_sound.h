// swc_ambient_sound.h -- CSWCAmbientSound
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/k/sw_c_ambient_sound.h); the other copies' members fall on its members.
#ifndef SHARED_SWC_AMBIENT_SOUND_H
#define SHARED_SWC_AMBIENT_SOUND_H

#include "shared/sw_ambient_sound.h"
#include "shared/swc_ambient_sound_list.h"

class CExoStreamingSoundSource;
class CNWMessage;
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

    // declared by other copies
    void UpdateTimeOfDay();
};

#endif
