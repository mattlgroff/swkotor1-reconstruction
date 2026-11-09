// CClientOptions::SaveOptions (clientoptions.o, Mac name), worker C s10:
// writes every option to swkotor.ini. The member names are the ones
// client_options_r10.cpp uses; several are older guesses that the ini keys
// here correct (see the key written for each member).

class CExoString {
public:
    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04

    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    ~CExoString();                                  // 0x005E5C20
    char* CStr() const;                             // 0x005E5670
    void Format(const char* format, ...);           // 0x005E5680
};

class CExoIni {
public:
    CExoIni();                                      // 0x005E6750
    ~CExoIni();                                     // 0x005E67E0
    int WriteIniEntry(const CExoString& sValue, const CExoString& sFile, const CExoString& sSection,
                      const CExoString& sKey);      // 0x005E67D0

    void* m_pcExoIniInternal;                       // 0x00
};

class CClientOptions {
public:
    unsigned long m_b0Bit0 : 1;                     // 0x00
    unsigned long m_bMouseLook : 1;
    int m_b0Bit2 : 1;
    int m_b0Bit3 : 1;
    int m_b0Bit4 : 1;
    int m_b0Bit5 : 1;
    int m_b0Bit6 : 1;
    unsigned char m_bUnknown04;                     // 0x04
    unsigned long m_b8Bit0 : 1;                     // 0x08
    unsigned long m_b8Bit1 : 1;
    unsigned long m_b8Bit2 : 1;
    unsigned long m_b8Bit3 : 1;
    unsigned long m_b8Bit4 : 1;
    float m_fToolTipDelay;                          // 0x0c
    int m_n10;                                      // 0x10
    unsigned long m_b14Bit0 : 1;                    // 0x14
    unsigned long m_b14Bit1 : 1;
    unsigned long m_b14Bit2 : 1;
    unsigned long m_b14Bit3 : 1;
    unsigned long m_b14Bit4 : 1;
    unsigned long m_b14Bit5 : 1;
    unsigned long m_b14Bit6 : 1;
    unsigned long m_b14Bit7 : 1;
    unsigned long m_bLargeFont : 1;                 // bit 8
    unsigned long m_b14Bit9 : 1;
    unsigned long m_b14Bit10 : 1;
    unsigned long m_b14Bit11 : 1;
    unsigned long m_b14Bit12 : 1;
    unsigned long m_b14Bit13 : 1;
    unsigned long m_b14Bit14 : 1;
    unsigned long m_b14Bit15 : 1;
    unsigned long m_b14Bit16 : 1;
    unsigned char m_nGamma;                         // 0x18
    float m_fGammaMin;                              // 0x1c
    float m_fGammaMax;                              // 0x20
    int m_bShadows;                                 // 0x24
    int m_bSoftShadows;                             // 0x28
    int m_bGrass;                                   // 0x2c
    int m_bFrameBuffer;                             // 0x30
    int m_nAnisotropy;                              // 0x34
    int m_n38;                                      // 0x38
    unsigned char m_nTexturePack;                   // 0x3c
    unsigned char m_b3D;                            // 0x3d
    int m_bVSync;                                   // 0x40
    unsigned char m_nAntiAliasing;                  // 0x44
    unsigned char m_nMusicVolume;                   // 0x45
    unsigned char m_nVoiceVolume;                   // 0x46
    unsigned char m_nSoundVolume;                   // 0x47
    unsigned char m_nMovieVolume;                   // 0x48
    unsigned char m_nSoundQuality;                  // 0x49
    int m_n4C;                                      // 0x4c
    unsigned char m_nBrightness;                    // 0x50
    int m_n54;                                      // 0x54
    unsigned char m_n58;                            // 0x58
    unsigned long m_b5CBit0 : 1;                    // 0x5c
    unsigned char m_aTexturePackBits[11];           // 0x60
    unsigned char m_n6B;                            // 0x6b
    unsigned char m_n6C;                            // 0x6c
    unsigned char m_n6D;                            // 0x6d
    unsigned char m_n6E;                            // 0x6e
    unsigned char m_n6F;                            // 0x6f
    unsigned char m_n70;                            // 0x70
    unsigned char m_n71;                            // 0x71
    unsigned char m_n72;                            // 0x72
    unsigned char m_n73;                            // 0x73
    int m_n74;                                      // 0x74
    float m_f78;                                    // 0x78
    float m_f7C;                                    // 0x7c
    unsigned char m_nFontSize;                      // 0x80
    float m_f84;                                    // 0x84
    int m_n88;                                      // 0x88
    int m_n8C;                                      // 0x8c
    int m_n90;                                      // 0x90
    float m_fFarClip;                               // 0x94
    float m_fFarClip2;                              // 0x98
    float m_fFarClip3;                              // 0x9c

    int SaveOptions();
};

// FUNCTION: SWKOTOR 0x0061B780
int CClientOptions::SaveOptions()
{
    CExoIni cIni;
    CExoString sValue;
    sValue.Format("%d", m_b8Bit4);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Game Options", "Combat Movement");
    sValue.Format("%d", m_b8Bit3);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Game Options", "Reverse Minigame YAxis");
    sValue.Format("%d", m_n54);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Game Options", "Reverse Mouse Buttons");
    sValue.Format("%d", m_n58);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Game Options", "EnableScreenShot");
    sValue.Format("%d", m_b5CBit0);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Game Options", "GUIsInScreenShot");
    sValue.Format("%d", m_b8Bit2);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Game Options", "AutoSave");
    sValue.Format("%d", m_b8Bit1);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Game Options", "Mouse Look");
    sValue.Format("%d", m_b8Bit0);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Game Options", "Auto Level Up NPCs");
    sValue.Format("%d", m_bUnknown04);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Game Options", "Difficulty Level");
    sValue.Format("%d", m_nBrightness);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Game Options", "Mouse Sensitivity");
    sValue.Format("%d", m_n10);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Game Options", "Enable Mouse Teleporting To Buttons");
    sValue.Format("%d", m_b14Bit5);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Game Options", "Status Summary");
    sValue.Format("%d", m_b14Bit4);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Game Options", "Floating Numbers");
    sValue.Format("%d", m_b14Bit3);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Game Options", "Mini Map");
    sValue.Format("%d", m_b14Bit2);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Game Options", "Subtitles");
    sValue.Format("%d", m_b14Bit1);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Game Options", "Tutorial Popups");
    sValue.Format("%d", m_b14Bit0);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Game Options", "Hide Unequippable");
    sValue.Format("%d", m_bLargeFont);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Game Options", "Use Small Fonts");
    sValue.Format("%d", m_b14Bit9);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Game Options", "Hide InGame GUI");
    sValue.Format("%d", m_b14Bit16);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Autopause Options", "New Target Selected");
    sValue.Format("%d", m_b14Bit15);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Autopause Options", "Action Menu");
    sValue.Format("%d", m_b14Bit14);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Autopause Options", "Party Killed");
    sValue.Format("%d", m_b14Bit12);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Autopause Options", "Enemy Sighted");
    sValue.Format("%d", m_b14Bit13);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Autopause Options", "Mine Sighted");
    sValue.Format("%d", m_b14Bit11);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Autopause Options", "End Of Combat Round");
    sValue.Format("%d", m_nGamma);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Graphics Options", "Brightness");
    sValue.Format("%d", m_bShadows);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Graphics Options", "Shadows");
    sValue.Format("%d", m_bSoftShadows);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Graphics Options", "Soft Shadows");
    sValue.Format("%d", m_bGrass);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Graphics Options", "Grass");
    sValue.Format("%d", m_nTexturePack);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Graphics Options", "Texture Quality");
    sValue.Format("%d", m_nAntiAliasing);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Graphics Options", "Anti Aliasing");
    sValue.Format("%d", m_bFrameBuffer);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Graphics Options", "Frame Buffer");
    sValue.Format("%d", m_nAnisotropy);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Graphics Options", "Anisotropy");
    sValue.Format("%d", m_bVSync);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Graphics Options", "V-Sync");
    sValue.Format("%d", m_nMovieVolume);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Sound Options", "Movie Volume");
    sValue.Format("%d", m_nSoundVolume);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Sound Options", "Sound Effects Volume");
    sValue.Format("%d", m_nVoiceVolume);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Sound Options", "Voiceover Volume");
    sValue.Format("%d", m_nMusicVolume);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Sound Options", "Music Volume");
    CExoString sKey;
    for (int i = 0; i < 11; i++) {
        sKey.Format("Movie%2d", i);
        sValue.Format("%d", m_aTexturePackBits[i]);
        cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Movies Shown", sKey.CStr());
    }
    sValue.Format("%f", m_fFarClip);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Game Options", "Keyboard Camera DPS");
    sValue.Format("%f", m_fFarClip2);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Game Options", "Keyboard Camera Acceleration");
    sValue.Format("%f", m_fFarClip3);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Game Options", "Keyboard Camera Deceleration");
    sValue.Format("%d", m_b14Bit10);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Game Options", "Enable Tooltips");
    return 1;
}
