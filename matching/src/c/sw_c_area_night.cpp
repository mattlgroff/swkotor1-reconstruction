// CSWCArea::SetAsNight / RemovePlaceableObjectLight and the `lockcamera`
// console helper (b0061_client_area_creature_misc), plus GetSurfaceMaterial /
// StartSounds / SetLightningDone (b00604_area_creature_x87_leftovers). Kept
// apart from sw_c_area.cpp so SetAsDay <-> SetAsNight stay tail calls.

#include "c/exo_base.h"

extern "C" void __cdecl free(void* p);

struct Vector {
    float x, y, z;
};

extern float g_fFogDensity;                         // 0x0078E6A0
extern float g_fAmbientScale;                       // 0x0078E408
extern int g_bLockCamera;                           // 0x007A22B8
extern float g_fLightningAmbient;                   // 0x00832C70
extern const int g_nAreaSoundMode;                  // 0x0074C5EC

extern "C" int __cdecl rand();

class CExoSound {
public:
    void SetSoundMode(int nMode);                   // 0x005D5E80
};

extern CExoSound* g_pExoSound;                      // 0x007A39EC

class CSWCAmbientSound {
public:
    void Activate();                                // 0x0068D4A0
};

class CLightningObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48();
    virtual void SetVisible(int b);                 // 0x4c
};

class CSWSArea {
public:
    int GetSurfaceMaterial(Vector v);               // 0x005063C0
};

class CSWSModule {
public:
    CSWSArea* GetArea();                            // 0x004C30F0
};

class CServerExoApp {
public:
    CSWSModule* GetModule();                        // 0x004AE6B0
};

class CSWCCreature {
public:
    void GetCreatureLight(int b);                   // 0x00614AA0
};

class CDeletable {
public:
    virtual ~CDeletable();
};

struct CPlaceableLight {
    unsigned long m_oidPlaceable;                   // 0x00
    CDeletable* m_pLight;                           // 0x04
};

class CSWCCameraObject {
public:
    char pad_0x0[0x58];
    float m_f58;                                    // 0x58
    char pad_0x5c[0x8];
    float m_f64;                                    // 0x64
};

class CSWCCameraNode {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18();
    virtual CSWCCameraObject* GetCamera();          // 0x1c
};

class CSceneHolder {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual CSWCCameraNode* GetCameraNode(unsigned long nId);   // 0x80
};

class CSWCModule {
public:
    char pad_0x0[0x40];
    CSceneHolder* m_pScene;                         // 0x40
};

class CClientExoApp {
public:
    CSWCModule* GetModule();                        // 0x005ED530
    CSWCCreature* GetPlayerCreature();              // 0x005ED540
    void StopMenuMusic();                           // 0x005ED9C0
    void StopLoadMusic();                           // 0x005ED9D0
};

class CAppManager {
public:
    void* m_pUnknown00;
    CClientExoApp* m_pClientExoApp;                 // 0x04
    CServerExoApp* m_pServerExoApp;                 // 0x08

    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CSWCArea {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10();
    virtual void SetSunColor(int b, Vector v);      // 0x14
    virtual void SetMoonColor(int b, Vector v);     // 0x18
    virtual void v1C();
    virtual void SetFog(unsigned long a, unsigned long b, Vector* pColor);  // 0x20

    void SetAsDay();                                // 0x006058C0
    void SetAsNight();
    void RemovePlaceableObjectLight(unsigned long oid, int bDestroy);
    int GetSurfaceMaterial(Vector v);
    void StartSounds();
    void SetLightningDone();

    char pad_0x4[0x70];
    unsigned long m_nNightFogA;                     // 0x74
    unsigned long m_nNightFogB;                     // 0x78
    char pad_0x7c[0x24];
    int m_bIsNight;                                 // 0xa0
    int m_bDayNightPending;                         // 0xa4
    char pad_0xa8[0xc];
    unsigned char m_nAmbientScale;                  // 0xb4
    char pad_0xb5[0x73];
    Vector m_vNightMoonColor;                       // 0x128
    Vector m_vNightSunColor;                        // 0x134
    Vector m_vNightFogColor;                        // 0x140
    char pad_0x14c[0x3c];
    int m_bLightingDirty;                           // 0x188
    char pad_0x18c[0x18];
    CLightningObject* m_pLightning;                 // 0x1a4
    char pad_0x1a8[0xc];
    int m_nUnknown1b4;                              // 0x1b4
    int m_nUnknown1b8;                              // 0x1b8
    char pad_0x1bc[0x4];
    int m_bUnknown1c0;                              // 0x1c0
    char pad_0x1c4[0x18];
    CSWCAmbientSound* m_pAmbientSound;              // 0x1dc
    float m_fLightningTimer;                        // 0x1e0
    char pad_0x1e4[0x34];
    CExoArrayList<CPlaceableLight*> m_lstPlaceableLights;  // 0x218
};

// FUNCTION: SWKOTOR 0x006059C0
void CSWCArea::SetAsNight()
{
    if (!m_bIsNight && !m_bDayNightPending) {
        SetAsDay();
        return;
    }
    g_fFogDensity = 0.0f;
    g_fAmbientScale = (float)m_nAmbientScale * 0.01f;
    m_bUnknown1c0 = 1;
    m_nUnknown1b4 = 0;
    m_nUnknown1b8 = 0;
    m_bDayNightPending = 1;
    SetFog(m_nNightFogA, m_nNightFogB, &m_vNightFogColor);
    SetMoonColor(1, m_vNightMoonColor);
    SetSunColor(1, m_vNightSunColor);
    if (m_bLightingDirty) {
        CSWCCreature* pPlayer = g_pAppManager->GetClientExoApp()->GetPlayerCreature();
        if (pPlayer) {
            pPlayer->GetCreatureLight(1);
        }
    }
}

// FUNCTION: SWKOTOR 0x00605B50
void CSWCArea::RemovePlaceableObjectLight(unsigned long oid, int bDestroy)
{
    int nCount = m_lstPlaceableLights.num;
    for (int i = 0; i < nCount; i++) {
        if (m_lstPlaceableLights.element[i] && m_lstPlaceableLights.element[i]->m_oidPlaceable == oid) {
            if (bDestroy && m_lstPlaceableLights.element[i]->m_pLight) {
                delete m_lstPlaceableLights.element[i]->m_pLight;
            }
            free(m_lstPlaceableLights.element[i]);
            m_lstPlaceableLights.DelIndex(i);
            return;
        }
    }
}

// FUNCTION: SWKOTOR 0x0060B2A0
// NONMATCH: the original moves the camera into ECX and builds the stored
// constants in EAX/EDX (`xor eax, eax` doubles as 0.0f); we keep it in EAX.
int lockcamera(char* sArgs)
{
    CSWCModule* pModule = g_pAppManager->m_pClientExoApp->GetModule();
    if (pModule && pModule->m_pScene) {
        CSWCCameraNode* pNode = pModule->m_pScene->GetCameraNode(0xffffffff);
        if (pNode) {
            CSWCCameraObject* pCamera = pNode->GetCamera();
            if (pCamera && sArgs) {
                switch (*sArgs - '0') {
                case 0:
                    pCamera->m_f58 = pCamera->m_f64 = 0.0f;
                    return g_bLockCamera = 0;
                case 1:
                    pCamera->m_f58 = pCamera->m_f64 = 1.0f;
                    return g_bLockCamera = 1;
                }
            }
        }
    }
    return g_bLockCamera;
}

// FUNCTION: SWKOTOR 0x00604BF0
int CSWCArea::GetSurfaceMaterial(Vector v)
{
    if (g_pAppManager->m_pServerExoApp) {
        CSWSModule* pModule = g_pAppManager->m_pServerExoApp->GetModule();
        if (pModule) {
            CSWSArea* pArea = pModule->GetArea();
            if (pArea) {
                return pArea->GetSurfaceMaterial(v);
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00604D70
void CSWCArea::StartSounds()
{
    g_pExoSound->SetSoundMode(g_nAreaSoundMode);
    g_pAppManager->m_pClientExoApp->StopMenuMusic();
    g_pAppManager->m_pClientExoApp->StopLoadMusic();
    m_pAmbientSound->Activate();
}

// FUNCTION: SWKOTOR 0x006053C0
void CSWCArea::SetLightningDone()
{
    g_fAmbientScale = g_fLightningAmbient;
    m_fLightningTimer = (float)(rand() % 5000) * 0.001f + 5.0f;
    if (m_pLightning) {
        m_pLightning->SetVisible(0);
    }
}
