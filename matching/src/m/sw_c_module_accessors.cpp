// CSWCModule area access and time-of-day state updates.

class CSWCMiniGame;

class CSWCAmbientSound {
public:
    void UpdateTimeOfDay();                           // 0x0068D690
};

class CSWCArea {
public:
    void SetAsDay();                                  // 0x006058C0
    void SetAsNight();                                // 0x006059C0
    void SetAsDawn();                                 // 0x00606CF0
    void SetAsDusk();                                 // 0x00606C50
    void UpdateSoundObjectsTimeOfDay();               // 0x00605B10

    char pad_0x0[0x1dc];
    CSWCAmbientSound* m_pAmbientSound;
    char pad_0x1e0[0x84];
    CSWCMiniGame* m_pMiniGame;                        // 0x264
};

class CSWCModule {
public:
    CSWCMiniGame* GetMiniGame();
    void SetTimeOfDayState(unsigned char nState, unsigned long nMilliseconds);

    char pad_0x0[0x48];
    CSWCArea* m_pArea;
    char pad_0x4c[0x54];
    unsigned char m_nMinutesPerHour;
    char pad_0xa1[0x13];
    unsigned char m_nTimeOfDayState;
    char pad_0xb5[3];
    float m_fWorldTime;
    float m_fMinutesPerHour;
};

// FUNCTION: SWKOTOR 0x00641980
CSWCMiniGame* CSWCModule::GetMiniGame()
{
    if (m_pArea) {
        return m_pArea->m_pMiniGame;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00640640
void CSWCModule::SetTimeOfDayState(unsigned char nState, unsigned long nMilliseconds)
{
    if (m_pArea) {
        switch (nState) {
        case 1:
            m_pArea->SetAsDay();
            break;
        case 2:
            m_pArea->SetAsNight();
            break;
        case 3:
            m_pArea->SetAsNight();
            m_pArea->SetAsDawn();
            break;
        case 4:
            m_pArea->SetAsDay();
            m_pArea->SetAsDusk();
            break;
        }
    }

    if (m_nTimeOfDayState != nState) {
        CSWCArea* pArea = m_pArea;
        m_nTimeOfDayState = nState;
        if (pArea) {
            pArea->UpdateSoundObjectsTimeOfDay();
            m_pArea->m_pAmbientSound->UpdateTimeOfDay();
        }
    }

    m_fWorldTime = (float)nMilliseconds * *((float*)0x0073d708);
    m_fMinutesPerHour = (float)m_nMinutesPerHour * *((float*)0x00748734);
}
