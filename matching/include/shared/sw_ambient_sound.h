// sw_ambient_sound.h -- CSWAmbientSound
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/k/sw_c_ambient_sound.h); the other copies' members fall on its members.
#ifndef SHARED_SW_AMBIENT_SOUND_H
#define SHARED_SW_AMBIENT_SOUND_H


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

#endif
