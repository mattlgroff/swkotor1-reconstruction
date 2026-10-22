// CClientOptions setters, defaults and constructor 0x0061D1C0-0x0061DB60
// (worker C r10).

#include <string.h>

#include "c/exo_base.h"

class CResRef {
public:
    char m_resRef[16];

    void CopyToString(CExoString& s) const;         // 0x00405F70
};

extern CExoString g_sDefaultTexturePack;            // 0x008338D8

class C2DA {
public:
    int GetRowIndex(const CExoString& sRow);       // 0x004138B0
};

class CTwoDimArrays {
public:
    char pad_0x0[0x11c];
    C2DA* m_pTexturePacks;                          // 0x11c

    C2DA* GetTexturePacks() { return m_pTexturePacks; }
};

class CSWRules {
public:
    char pad_0x0[0xb8];
    CTwoDimArrays* m_p2DArrays;                     // 0xb8

    CTwoDimArrays* Get2DArrays() { return m_p2DArrays; }
};

extern CSWRules* g_pRules;                          // 0x007A3A28

class CExoSound {
public:
    void SetMusicVolume(float fVolume, int b);      // 0x005D5C50
    void SetSoundEffectVolume(float fVolume);       // 0x005D5CD0
    void SetDialogVolume(float fVolume);            // 0x005D5D50
    int GetMaxSoundQuality();                       // 0x005D6460
};

extern CExoSound* g_pExoSound;                      // 0x007A39EC

class CSWGuiManager {
public:
    char pad_0x0[0x48];
    float m_fToolTipDelay;                          // 0x48

    void UpdateAllFonts();                          // 0x0040B420
};

extern CSWGuiManager* g_pGuiManager;                // 0x007A39F4

struct Vector {
    float x, y, z;
};

class CSWCCreature {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9C();
    virtual void vA0(); virtual void vA4(); virtual void vA8(); virtual void vAC();
    virtual void vB0();
    virtual void ApplyVisualEffect(int nEffect, int a, unsigned long oidTarget,
                                   unsigned long oidSource, int b, int c,
                                   const Vector& vOffset);  // 0xb4
    virtual void RemoveVisualEffect(int nEffect);  // 0xb8
    virtual void vBC(); virtual void vC0(); virtual void vC4(); virtual void vC8();
    virtual int HasVisualEffect(int nEffect);       // 0xcc
};

class CSWParty {
public:
    int m_nMembers;                                 // 0x00

    CSWCCreature* GetMember(int n);                 // 0x006346C0
};

class CClientExoApp {
public:
    CSWParty* GetParty();                           // 0x005ED8B0
    void SetTexturePack(int nPack);                 // 0x005ED8F0
    void SetMouseMode(unsigned char nMode);         // 0x005EE150
    void SaveOptions();                             // 0x005EDF70
};

class CAppManager {
public:
    void* m_pUnknown00;
    CClientExoApp* m_pClientExoApp;                 // 0x04
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

extern float g_fFarClipOverride;                    // 0x007A2320
extern float g_fFarClipOverride2;                   // 0x007A2324
extern float g_fFarClipOverride3;                   // 0x007A2328

void AurEnableShadows();                            // 0x0044EE20
void AurEnableGrass();                              // 0x0044EE30
void AurDisableGrass();                             // 0x0044EE40
void AurEnableSoftShadows();                        // 0x0044EE50
void AurDisableSoftShadows();                       // 0x0044EE60
void AurEnableFrameBufferEffects();                 // 0x0044EE70
void AurDisableFrameBufferEffects();                // 0x0044EE80
void AurSetVSyncState(int b);                       // 0x0044EE90
void AurDisableShadows();                           // 0x0044F080
void AurSetAnisotropy(int n);                       // 0x0044F2C0
void AurSetAntiAliasingMode(int n);                 // 0x0044F2F0
unsigned char AurCheckFrameBufferModificationsAvailable();  // 0x0045FAB0

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

    CClientOptions();
    void SetToolTipDelay(float f)
    {
        m_fToolTipDelay = f;
        if (g_pGuiManager) {
            g_pGuiManager->m_fToolTipDelay = f;
        }
    }
    void ApplySoundOptions();
    void SetTexturePackBit(const CExoString& sName, int bSet);
    int GetTexturePackBit(const CResRef& cTexture);
    void SetGameDefaults();
    void SetControlDefaults();
    void SetGraphicsDefaults();
    void SetSoundDefaults();
    void SetSoundQualityDefault();
    float GetGamma();
    float GetBrightness();
    float GetFarClip();
    float GetFarClip2();
    float GetFarClip3();
    void SetMouseLook(int b);
    void SetShadows(int b);
    void SetSoftShadows(int b);
    void SetGrass(int b);
    void SetAntiAliasing(unsigned char n);
    void SetTexturePack(int n);
    void SetFrameBuffer(int b);
    void SetAnisotropy(int n);
    void SetVSync(int b);
    void SetLargeFont(int b);
    void SetFeedbackDefaults();
    void SetDisplayDefaults();
    void SetBrightnessDefaults();
    void SetFarClipDefaults();
    void SetDefaults();
};

// FUNCTION: SWKOTOR 0x0061D1C0
void CClientOptions::ApplySoundOptions()
{
    if (g_pExoSound) {
        g_pExoSound->SetMusicVolume(m_nMusicVolume * 0.01f, 0);
        g_pExoSound->SetDialogVolume(m_nVoiceVolume * 0.01f);
        g_pExoSound->SetSoundEffectVolume(m_nSoundVolume * 0.01f);
    }
}

// FUNCTION: SWKOTOR 0x0061D230
void CClientOptions::SetTexturePackBit(const CExoString& sName, int bSet)
{
    int nRow = g_pRules->Get2DArrays()->GetTexturePacks()->GetRowIndex(sName);
    if (nRow < 0) {
        char* pDot = strrchr(sName.CStr(), '.');
        char* pSlash = strrchr(sName.CStr(), '\\');
        if (pDot == NULL || pSlash == NULL) {
            return;
        }
        int nLength = strlen(sName.CStr());
        int nDot = pDot - sName.CStr() + 1;
        int nSlash = pSlash - sName.CStr() + 1;
        CExoString sBase = sName.Right(nLength - nSlash);
        sBase = sBase.Left(nDot - nSlash - 1);
        nRow = (unsigned char)g_pRules->Get2DArrays()->GetTexturePacks()->GetRowIndex(sBase);
        if (nRow < 0) {
            return;
        }
    }
    unsigned int nByte = (unsigned int)nRow >> 3;
    if (nByte < 11) {
        int nBit = nRow - nByte * 8;
        if (bSet) {
            m_aTexturePackBits[nByte] |= (unsigned char)(1 << nBit);
        } else {
            m_aTexturePackBits[nByte] &= ~(unsigned char)(1 << nBit);
        }
        g_pAppManager->m_pClientExoApp->SaveOptions();
    }
}

// FUNCTION: SWKOTOR 0x0061D3D0
int CClientOptions::GetTexturePackBit(const CResRef& cTexture)
{
    CExoString sName;
    cTexture.CopyToString(sName);
    if (sName.CompareNoCase(g_sDefaultTexturePack)) {
        return 1;
    }
    int nRow = g_pRules->Get2DArrays()->GetTexturePacks()->GetRowIndex(sName);
    if (nRow < 0) {
        return 0;
    }
    unsigned char nByte = nRow / 8;
    if (nByte >= 11) {
        return 0;
    }
    unsigned char nBit = nRow - nByte * 8;
    return m_aTexturePackBits[nByte] & (1 << nBit);
}

// FUNCTION: SWKOTOR 0x0061D4E0
void CClientOptions::SetGameDefaults()
{
    m_bUnknown04 = 1;
    m_b8Bit0 = 0;
    m_b8Bit1 = 0;
    m_b8Bit2 = 1;
    m_b8Bit3 = 0;
    m_b8Bit4 = 1;
}

// FUNCTION: SWKOTOR 0x0061D500
void CClientOptions::SetControlDefaults()
{
    m_b14Bit11 = 0;
    m_b14Bit12 = 1;
    m_b14Bit13 = 1;
    m_b14Bit14 = 1;
    m_b14Bit15 = 0;
    m_b14Bit16 = 1;
}

// FUNCTION: SWKOTOR 0x0061D520
void CClientOptions::SetGraphicsDefaults()
{
    m_bFrameBuffer = 1;
    m_nAnisotropy = 1;
    m_n38 = 1;
    m_nTexturePack = 0;
    m_bVSync = 0;
    m_nAntiAliasing = 0;
    m_bSoftShadows = 1;
}

// FUNCTION: SWKOTOR 0x0061D540
void CClientOptions::SetSoundDefaults()
{
    m_nMusicVolume = 85;
    m_nVoiceVolume = 85;
    m_nSoundVolume = 85;
    m_nMovieVolume = 100;
}

// FUNCTION: SWKOTOR 0x0061D550
void CClientOptions::SetSoundQualityDefault()
{
    m_nSoundQuality = g_pExoSound->GetMaxSoundQuality() > 3 ? 3 : g_pExoSound->GetMaxSoundQuality();
    m_n4C = 0;
}

// FUNCTION: SWKOTOR 0x0061D590
float CClientOptions::GetGamma()
{
    return m_fGammaMax - (m_fGammaMax - m_fGammaMin) * (m_nGamma * 0.01f);
}

// FUNCTION: SWKOTOR 0x0061D5B0
float CClientOptions::GetBrightness()
{
    float f = m_nBrightness * 0.01f;
    return f * 45.0f + 10.0f;
}

// FUNCTION: SWKOTOR 0x0061D5E0
float CClientOptions::GetFarClip()
{
    if (g_fFarClipOverride != -1.0f) {
        return g_fFarClipOverride;
    }
    return m_fFarClip;
}

// FUNCTION: SWKOTOR 0x0061D610
float CClientOptions::GetFarClip2()
{
    if (g_fFarClipOverride2 != -1.0f) {
        return g_fFarClipOverride2;
    }
    return m_fFarClip2;
}

// FUNCTION: SWKOTOR 0x0061D640
float CClientOptions::GetFarClip3()
{
    if (g_fFarClipOverride3 != -1.0f) {
        return g_fFarClipOverride3;
    }
    return m_fFarClip3;
}

// FUNCTION: SWKOTOR 0x0061D670
void CClientOptions::SetMouseLook(int b)
{
    if (g_pAppManager && g_pAppManager->m_pClientExoApp) {
        if (b) {
            g_pAppManager->m_pClientExoApp->SetMouseMode(0);
        } else {
            g_pAppManager->m_pClientExoApp->SetMouseMode(1);
        }
        m_bMouseLook = b;
    }
}

// FUNCTION: SWKOTOR 0x0061D6B0
void CClientOptions::SetShadows(int b)
{
    m_bShadows = b;
    if (b) {
        AurEnableShadows();
    } else {
        AurDisableShadows();
    }
}

// FUNCTION: SWKOTOR 0x0061D6D0
void CClientOptions::SetSoftShadows(int b)
{
    m_bSoftShadows = b;
    if (b) {
        AurEnableSoftShadows();
    } else {
        AurDisableSoftShadows();
    }
}

// FUNCTION: SWKOTOR 0x0061D6F0
void CClientOptions::SetGrass(int b)
{
    m_bGrass = b;
    if (b) {
        AurEnableGrass();
    } else {
        AurDisableGrass();
    }
}

// FUNCTION: SWKOTOR 0x0061D710
void CClientOptions::SetAntiAliasing(unsigned char n)
{
    m_nAntiAliasing = n;
    AurSetAntiAliasingMode(n);
}

// FUNCTION: SWKOTOR 0x0061D730
void CClientOptions::SetTexturePack(int n)
{
    m_nTexturePack = n;
    g_pAppManager->m_pClientExoApp->SetTexturePack(n);
}

// FUNCTION: SWKOTOR 0x0061D750
void CClientOptions::SetFrameBuffer(int b)
{
    CSWParty* pParty = g_pAppManager->m_pClientExoApp->GetParty();
    m_bFrameBuffer = b;
    if (b) {
        AurEnableFrameBufferEffects();
        if (pParty) {
            for (unsigned char i = 0; i < pParty->m_nMembers; i++) {
                CSWCCreature* pCreature = pParty->GetMember(i);
                if (pCreature && pCreature->HasVisualEffect(0x1f42) &&
                    AurCheckFrameBufferModificationsAvailable()) {
                    pCreature->RemoveVisualEffect(0x1f42);
                    Vector v;
                    v.x = 0.0f;
                    v.y = 0.0f;
                    v.z = 0.0f;
                    pCreature->ApplyVisualEffect(0x1f40, 0, 0x7f000000, 0x7f000000, 0, 0, v);
                }
            }
        }
    } else {
        AurDisableFrameBufferEffects();
        if (pParty) {
            for (unsigned char i = 0; i < pParty->m_nMembers; i++) {
                CSWCCreature* pCreature = pParty->GetMember(i);
                if (pCreature && pCreature->HasVisualEffect(0x1f40) &&
                    AurCheckFrameBufferModificationsAvailable()) {
                    pCreature->RemoveVisualEffect(0x1f40);
                    Vector v;
                    v.x = 0.0f;
                    v.y = 0.0f;
                    v.z = 0.0f;
                    pCreature->ApplyVisualEffect(0x1f42, 0, 0x7f000000, 0x7f000000, 0, 0, v);
                }
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x0061D8C0
void CClientOptions::SetAnisotropy(int n)
{
    m_nAnisotropy = n;
    AurSetAnisotropy(n);
}

// FUNCTION: SWKOTOR 0x0061D8E0
void CClientOptions::SetVSync(int b)
{
    m_bVSync = b;
    if (b) {
        AurSetVSyncState(1);
    } else {
        AurSetVSyncState(0);
    }
}

// FUNCTION: SWKOTOR 0x0061D910
void CClientOptions::SetLargeFont(int b)
{
    m_bLargeFont = b;
    m_nFontSize = b ? 1 : 2;
    if (g_pGuiManager) {
        g_pGuiManager->UpdateAllFonts();
    }
}

// FUNCTION: SWKOTOR 0x0061D950
void CClientOptions::SetFeedbackDefaults()
{
    m_b14Bit0 = 0;
    m_b14Bit1 = 1;
    m_b14Bit2 = 1;
    m_b14Bit3 = 1;
    m_b14Bit4 = 1;
    m_b14Bit5 = 1;
    m_b14Bit6 = 0;
    m_b14Bit7 = 1;
    m_bLargeFont = 0;
    m_b14Bit9 = 0;
    m_b14Bit10 = 1;
    SetLargeFont(m_bLargeFont);
}

// FUNCTION: SWKOTOR 0x0061D9A0
void CClientOptions::SetDisplayDefaults()
{
    m_nGamma = 57;
    m_fGammaMin = 0.23f;
    m_fGammaMax = 2.0f;
    m_bShadows = 1;
    m_bGrass = 1;
    m_b3D = 1;
}

// FUNCTION: SWKOTOR 0x0061D9D0
void CClientOptions::SetBrightnessDefaults()
{
    m_n54 = 0;
    m_nBrightness = 44;
}

// FUNCTION: SWKOTOR 0x0061D9E0
void CClientOptions::SetFarClipDefaults()
{
    if (g_fFarClipOverride != -1.0f) {
        g_fFarClipOverride = 200.0f;
    }
    m_fFarClip = 200.0f;
    if (g_fFarClipOverride2 != -1.0f) {
        g_fFarClipOverride2 = 500.0f;
    }
    m_fFarClip2 = 500.0f;
    if (g_fFarClipOverride3 != -1.0f) {
        g_fFarClipOverride3 = 2000.0f;
    }
    m_fFarClip3 = 2000.0f;
}

// FUNCTION: SWKOTOR 0x0061DA60
void CClientOptions::SetDefaults()
{
    SetGameDefaults();
    SetFeedbackDefaults();
    SetControlDefaults();
    SetDisplayDefaults();
    SetGraphicsDefaults();
    SetSoundDefaults();
    SetBrightnessDefaults();
    m_n58 = 0;
    m_b5CBit0 = 1;
    SetFarClipDefaults();
    memset(m_aTexturePackBits, 0, sizeof(m_aTexturePackBits));
    m_n90 = 0;
    m_n88 = 0;
    m_n8C = 0;
}

// FUNCTION: SWKOTOR 0x0061DB60
CClientOptions::CClientOptions()
{
    m_b0Bit0 = 0;
    m_b0Bit2 = 1;
    m_b0Bit3 = 0;
    m_b0Bit4 = 1;
    m_b0Bit6 = 1;
    m_n6D = 3;
    m_n6E = 1;
    m_n73 = 16;
    m_n6B = 0;
    m_n72 = 2;
    m_n6C = 2;
    m_f78 = 3.0f;
    m_f7C = 1.0f;
    m_n6F = 3;
    m_n70 = 2;
    m_n71 = 1;
    m_nFontSize = 2;
    m_f84 = 5.0f;
    SetToolTipDelay(1.0f);
    m_b14Bit10 = 1;
    m_n10 = 1;
    SetDefaults();
}
