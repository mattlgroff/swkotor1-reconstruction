// CSWCVisualEffect: client area visual effect object (object type 0x0B,
// vtable 0x00754278); class name from its LoadModel diagnostic string.
#include <stdio.h>
#include "k/sw_c_client_object.h"

class CExoSoundSource {
public:
    virtual ~CExoSoundSource();
    void SetPosition(const Vector&, float);                          // 0x005D59E0
};

class CSWCAnimBaseVisualEffect : public CSWCAnimBase {
public:
    CSWCAnimBaseVisualEffect();                                      // 0x006E5CE0
    char pad_0x4[0xf0 - 0x4];
};

class CClientExoAppVFX : public CClientExoApp {
public:
    void LookUpAndPerformRumbleWithCutOff(int, int, const Vector*);  // 0x005EDF60
};

extern unsigned char g_nObjectTypeAreaOfEffect;                      // 0x0074663A

class CSWCVisualEffect : public CSWCObject {
public:
    CSWCVisualEffect(int bRegister);
    virtual ~CSWCVisualEffect();
    virtual void AIUpdate();                                         // 0x00690980
    virtual int LoadModel(const CResRef&, unsigned char, int);       // 0x006903D0

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
        sprintf(message, "CSWCVisualEffect::LoadModel: Failed to load visual effect model %s",
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
