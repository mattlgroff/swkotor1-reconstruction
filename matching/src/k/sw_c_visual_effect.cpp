// CSWCVisualEffect: client area visual effect object (object type 0x0B,
// vtable 0x00754278); class name from its LoadModel diagnostic string.
#include <stdio.h>
#include <string.h>
#include "k/sw_c_client_object.h"

class CExoSoundSource {
public:
    CExoSoundSource(const CResRef&);                                 // 0x005D60E0
    virtual ~CExoSoundSource();
    void SetPriorityGroup(unsigned char);                            // 0x005D5900
    void Set3D(int);                                                 // 0x005D5910
    unsigned long Play();                                            // 0x005D5930
    void SetLooping(int);                                            // 0x005D59D0
    void SetPosition(const Vector&, float);                          // 0x005D59E0
    void Stop();                                                     // 0x005D5A20
    char pad_0x4[4];
};

class CExoSound {
public:
    int Play3DOneShotSound(const CResRef&, Vector, float, unsigned char, unsigned long,
                           unsigned char, float, float);             // 0x005D5E10
};

extern CExoSound* g_pExoSound;                                       // 0x007A39EC

class C2DA {
public:
    int GetCExoStringEntry(const CExoString&, const CExoString&, CExoString*); // 0x00413DE0
    int GetCExoStringEntry(const CExoString&, int, CExoString*);     // 0x00413EC0
    int GetFLOATEntry(const CExoString&, int, float*);               // 0x00414080
    int GetINTEntry(const CExoString&, int, int*);                   // 0x00414260
};

class C2DAs {
public:
    char pad_0x0[0x88];
    C2DA* m_pVisualEffects;                                          // +0x88
    C2DA* GetVisualEffects() { return m_pVisualEffects; }
};

class CSWRules {
public:
    char pad_0x0[0xb8];
    C2DAs* m_p2DArrays;                                              // +0xB8
    C2DAs* Get2DArrays() { return m_p2DArrays; }
};

extern CSWRules* g_pRules;                                           // 0x007A3A28

struct CClientOptions {
    unsigned char m_nGraphicsQuality;                                // +0x0 (bit 7 skips LowQuality)
};

extern int g_nVisualEffectCol_Imp_HeadCon_Node;                      // 0x007A21B4
extern int g_nVisualEffectCol_Imp_Impact_Node;                       // 0x007A21B8
extern int g_nVisualEffectCol_Imp_Root_M_Node;                       // 0x007A21BC
extern int g_nVisualEffectCol_Imp_Root_S_Node;                       // 0x007A21C0
extern int g_nVisualEffectCol_Imp_Root_L_Node;                       // 0x007A21C4
extern int g_nVisualEffectCol_Imp_Root_H_Node;                       // 0x007A21C8
extern int g_nVisualEffectCol_LowQuality;                            // 0x007A21CC
extern int g_nVisualEffectCol_OrientWithGround;                      // 0x007A21D0
extern int g_nVisualEffectCol_ShakeType;                             // 0x007A21D4
extern int g_nVisualEffectCol_ShakeDuration;                         // 0x007A21D8
extern int g_nVisualEffectCol_ShakeDelay;                            // 0x007A21DC
extern float g_fVisualEffectAnimSpeed40;                             // 0x007A2674 (0.8)

class CSWCAnimBaseVisualEffect : public CSWCAnimBase {
public:
    CSWCAnimBaseVisualEffect();                                      // 0x006E5CE0
    char pad_0x4[0xf0 - 0x4];
};

class CClientExoAppVFX : public CClientExoApp {
public:
    CClientOptions* GetClientOptions();                              // 0x005ED700
    void LookUpAndPerformRumbleWithCutOff(int, int, const Vector*);  // 0x005EDF60
};

extern unsigned char g_nObjectTypeAreaOfEffect;                      // 0x0074663A

class CSWCVisualEffect : public CSWCObject {
public:
    CSWCVisualEffect(int bRegister);
    virtual ~CSWCVisualEffect();
    virtual void AIUpdate();                                         // 0x00690980
    virtual int LoadModel(const CResRef&, unsigned char, int);       // 0x006903D0

    int LoadEffect(unsigned long);                                   // 0x00690010
    void Announce();                                                 // 0x00690940
    void SetAnimation(unsigned short);                               // 0x00690510

    CExoSoundSource* m_pSound;                                       // +0xFC
    unsigned long m_nFadeTime;                                       // +0x100
    unsigned long m_nDelay;                                          // +0x104
    int m_nEffect;                                                   // +0x108
    unsigned short m_nAnimation;                                     // +0x10C
    char pad_0x10e[2];
    int m_bFading;                                                   // +0x110
    int m_bDone;                                                     // +0x114
    int m_bHasImpact;                                                // +0x118
    int m_bDestroyWhenDone;                                          // +0x11C
    int m_bOrient;                                                   // +0x120
    int m_bHideWhenDone;                                             // +0x124
    int m_bNoFade;                                                   // +0x128
    int m_bRenderOrder;                                              // +0x12C
};

// FUNCTION: SWKOTOR 0x0068FE70
CSWCVisualEffect::CSWCVisualEffect(int bRegister) : CSWCObject(0x0b)
{
    unsigned long day, tick;
    CClientExoApp* app = g_pAppManager->m_pClientExoApp;
    app->GetWorldTimer()->GetWorldTime(&day, &tick);
    m_nDay = day;
    m_nTick = tick;
    m_nFadeTime = 0;
    m_bFading = 0;
    m_bDone = 0;
    m_bHasImpact = 0;
    m_bDestroyWhenDone = 1;
    m_nDelay = 0;
    m_nAnimation = 0;
    if (bRegister) {
        g_pAppManager->GetClientExoApp()->GetGameObjectArray()->AddInternalObject(&m_oid, this, 0);
        g_pAppManager->GetClientExoApp()->GetClientAIMaster()->AddObject(this);
    }
    m_pSound = 0;
    m_bOrient = 0;
    m_bHideWhenDone = 0;
    m_bNoFade = 0;
    m_bRenderOrder = 0;
    m_nEffect = -1;
}

// FUNCTION: SWKOTOR 0x0068FF70
CSWCVisualEffect::~CSWCVisualEffect()
{
    g_pAppManager->m_pClientExoApp->GetGameObjectArray()->Delete(m_oid);
    if (m_pSound)
        delete m_pSound;
    if (m_pModel) {
        m_pModel->UnloadModel();
        delete m_pModel;
        m_pModel = 0;
    }
}

// FUNCTION: SWKOTOR 0x00690010
int CSWCVisualEffect::LoadEffect(unsigned long nEffect)
{
    CExoString sModel;
    CExoString sRow((int)nEffect);
    int bOrient;
    if (!g_pRules->Get2DArrays()->GetVisualEffects()->GetINTEntry(
            sRow, g_nVisualEffectCol_OrientWithGround, &bOrient))
        bOrient = 0;
    if (bOrient)
        m_bOrient = 1;
    else
        m_bOrient = 0;
    int bFound = 0;
    if ((((CClientExoAppVFX*)g_pAppManager->m_pClientExoApp)->GetClientOptions()->m_nGraphicsQuality & 0x80) == 0
        && g_pRules->Get2DArrays()->GetVisualEffects()->GetCExoStringEntry(
            sRow, g_nVisualEffectCol_LowQuality, &sModel))
        bFound = 1;
    else if (g_pRules->Get2DArrays()->GetVisualEffects()->GetCExoStringEntry(
            sRow, g_nVisualEffectCol_Imp_HeadCon_Node, &sModel))
        bFound = 1;
    else if (g_pRules->Get2DArrays()->GetVisualEffects()->GetCExoStringEntry(
            sRow, g_nVisualEffectCol_Imp_Impact_Node, &sModel))
        bFound = 1;
    else if (g_pRules->Get2DArrays()->GetVisualEffects()->GetCExoStringEntry(
            sRow, g_nVisualEffectCol_Imp_Root_M_Node, &sModel))
        bFound = 1;
    else if (g_pRules->Get2DArrays()->GetVisualEffects()->GetCExoStringEntry(
            sRow, g_nVisualEffectCol_Imp_Root_S_Node, &sModel))
        bFound = 1;
    else if (g_pRules->Get2DArrays()->GetVisualEffects()->GetCExoStringEntry(
            sRow, g_nVisualEffectCol_Imp_Root_L_Node, &sModel))
        bFound = 1;
    else if (g_pRules->Get2DArrays()->GetVisualEffects()->GetCExoStringEntry(
            sRow, g_nVisualEffectCol_Imp_Root_H_Node, &sModel))
        bFound = 1;
    int nShakeType;
    float fDuration, fDelay;
    if (!g_pRules->Get2DArrays()->GetVisualEffects()->GetINTEntry(
            sRow, g_nVisualEffectCol_ShakeType, &nShakeType))
        nShakeType = 0;
    if (!g_pRules->Get2DArrays()->GetVisualEffects()->GetFLOATEntry(
            sRow, g_nVisualEffectCol_ShakeDuration, &fDuration))
        fDuration = 0.0f;
    if (!g_pRules->Get2DArrays()->GetVisualEffects()->GetFLOATEntry(
            sRow, g_nVisualEffectCol_ShakeDelay, &fDelay))
        fDelay = 0.0f;
    if (nShakeType == 1) {
        CClientExoApp* app = g_pAppManager->m_pClientExoApp;
        app->GetModule()->BumpCamera((int)(fDelay * 1000.0f));
    } else if (nShakeType == 2) {
        CClientExoApp* app = g_pAppManager->m_pClientExoApp;
        app->GetModule()->ShakeCamera((int)(fDelay * 1000.0f), (int)(fDuration * 1000.0f));
    }
    if (bFound) {
        CResRef res(sModel);
        if (!LoadModel(res, 0xff, 8))
            return 0;
    }
    if (nEffect == 0x27 || nEffect == 0x28)
        UnHilite(3);
    m_nEffect = nEffect;
    return 1;
}

// FUNCTION: SWKOTOR 0x00690510
void CSWCVisualEffect::SetAnimation(unsigned short nAnimation)
{
    m_nDelay = 0;
    m_nAnimation = nAnimation;
    CExoString sRow(m_nEffect);
    float fSpeed;
    if (m_nEffect == 0x28)
        fSpeed = g_fVisualEffectAnimSpeed40;
    else
        fSpeed = 1.0f;
    if (nAnimation == 1) {
        CExoString sSound;
        if (!g_pRules->Get2DArrays()->GetVisualEffects()->GetCExoStringEntry(sRow, "SoundImpact", &sSound))
            sSound = "";
        g_pExoSound->Play3DOneShotSound(sSound, m_vPosition, 1.5f, 13, 0, 0, 0.0f, 0.0f);
        if (m_pModel) {
            m_nDelay = (unsigned long)m_pModel->GetAnimationEventTime(1);
            if (m_bRenderOrder)
                m_pModel->PlayAnim(1, fSpeed, 0, 0);
            else
                m_pModel->PlayAnim(1, fSpeed, 1, 0);
        }
    } else if (nAnimation == 3) {
        CExoString sSound;
        if (!g_pRules->Get2DArrays()->GetVisualEffects()->GetCExoStringEntry(sRow, "SoundCessation", &sSound))
            sSound = "";
        g_pExoSound->Play3DOneShotSound(sSound, m_vPosition, 1.5f, 13, 0, 0, 0.0f, 0.0f);
        if (m_pSound) {
            m_pSound->Stop();
            if (m_pSound)
                delete m_pSound;
            m_pSound = 0;
        }
        if (m_pModel) {
            m_nDelay = (unsigned long)m_pModel->GetAnimationEventTime(3);
            if (m_bRenderOrder)
                m_pModel->PlayAnim(3, fSpeed, 0, 0);
            else
                m_pModel->PlayAnim(3, fSpeed, 1, 0);
        }
    } else {
        CExoString sSound;
        if (!g_pRules->Get2DArrays()->GetVisualEffects()->GetCExoStringEntry(sRow, "SoundDuration", &sSound))
            sSound = "";
        m_pSound = new CExoSoundSource(sSound);
        if (m_pSound) {
            m_pSound->Set3D(1);
            m_pSound->SetPriorityGroup(13);
            m_pSound->SetLooping(1);
            m_pSound->Play();
        }
        if (m_pModel)
            m_pModel->PlayAnim(nAnimation, fSpeed, 0, 0);
    }
}

// FUNCTION: SWKOTOR 0x006903D0
int CSWCVisualEffect::LoadModel(const CResRef& res, unsigned char, int)
{
    if (!m_pModel) {
        m_pModel = new CSWCAnimBaseVisualEffect;
        if (!m_pModel)
            return 0;
    }
    res.GetResRefStr();
    if (!m_pModel->LoadModel(res, 0xff)) {
        char message[256];
        sprintf(message, "CSWCVisualEffect::LoadModel: Failed to load visual effect model '%s'.",
                res.GetResRefStr());
        return 0;
    }
    CAurObject* model = m_pModel->GetModel(0xff);
    if (!model)
        return 0;
    model->SetObjectType(g_nObjectTypeAreaOfEffect);
    model->SetClientObject(this);
    model->SetRenderMode(3);
    Vector color;
    color.x = 1.0f;
    color.y = 1.0f;
    color.z = 1.0f;
    Hilite(&color, 3, 0, 0);
    if (m_bRenderOrder)
        model->EnableRenderOrder();
    return 1;
}

// FUNCTION: SWKOTOR 0x00690940
void CSWCVisualEffect::Announce()
{
    ((CClientExoAppVFX*)g_pAppManager->m_pClientExoApp)
        ->LookUpAndPerformRumbleWithCutOff(m_nEffect, 1, &m_vPosition);
}

// Not in the function table (vtable slot 0x74 of 0x00754278).
// FUNCTION: SWKOTOR 0x00690980
void CSWCVisualEffect::AIUpdate()
{
    if (m_bDone)
        return;
    unsigned long day, tick;
    g_pAppManager->GetClientExoApp()->GetWorldTimer()->GetWorldTime(&day, &tick);
    unsigned long elapsedDays, elapsed;
    CClientExoApp* app = g_pAppManager->m_pClientExoApp;
    app->GetWorldTimer()->SubtractWorldTimes(
        day, tick, m_nDay, m_nTick, &elapsedDays, &elapsed);
    m_nElapsed = elapsed;
    m_nDay = day;
    m_nTick = tick;
    if (m_pSound)
        m_pSound->SetPosition(m_vPosition, 1.5f);
    if (m_nDelay > 0) {
        if (m_nDelay > m_nElapsed) {
            m_nDelay -= m_nElapsed;
        } else {
            m_nDelay = 0;
            if (m_nAnimation == 1) {
                if (m_bHasImpact) {
                    m_nFadeTime = m_bNoFade ? 0 : 3000;
                    m_bFading = 1;
                } else {
                    SetAnimation(2);
                }
            } else if (m_nAnimation == 3) {
                m_nFadeTime = 3000;
                m_bFading = 1;
            }
        }
    }
    if (m_bFading) {
        if (m_nFadeTime <= m_nElapsed) {
            if (m_bDestroyWhenDone) {
                delete this;
                return;
            }
            if (m_bHideWhenDone)
                SetDisplayed(0);
            m_bDone = 1;
            m_bFading = 0;
        } else {
            m_nFadeTime -= m_nElapsed;
        }
    }
}

// FUNCTION: SWKOTOR 0x00690960 SYMBOL: ??_GCSWCVisualEffect@@UAEPAXI@Z
// CSWCVisualEffect::`scalar deleting destructor' (compiler generated).
