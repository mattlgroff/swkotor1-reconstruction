// CSWCCreature members, worker C s8 (translation-unit-local layout).

#include "c/exo_base.h"

struct Vector {
    Vector() {}
    Vector(float a, float b, float c) : x(a), y(b), z(c) {}
    float x, y, z;
};

class CAurObject;

// Model of a creature part (CGob); only the slots used here.
class CGob {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14();
    virtual void PlayAnimation(const char* sName, float fSpeed, int nFlags, int b);   // 0x18 (descriptive)
    virtual void GetAnimationLength(const char* sName, int b, float* pLength);       // 0x1c (descriptive)
};

class CSWCAnimBase {
public:
    virtual void v00(); virtual void v04();
    virtual CGob* GetPartModel(int nPart);          // 0x08 (descriptive)
};

// Model returned by CSWGuiScene::GetModel.
class CAurSceneModel {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14();
    virtual void PlayAnimation(const char* sName, float fSpeed, int nFlags, int b);   // 0x18 (descriptive)
    virtual void GetAnimationLength(const char* sName, int b, float* pLength);       // 0x1c (descriptive)
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9C();
    virtual void vA0(); virtual void vA4(); virtual void vA8(); virtual void vAC();
    virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC();
    virtual void vC0(); virtual void vC4(); virtual void vC8(); virtual void vCC();
    virtual void vD0(); virtual void vD4(); virtual void vD8(); virtual void vDC();
    virtual void vE0(); virtual void vE4(); virtual void vE8(); virtual void vEC();
    virtual void vF0(); virtual void vF4(); virtual void vF8(); virtual void vFC();
    virtual void v100(); virtual void v104(); virtual void v108();
    virtual int HasHook(const char* sHook);         // 0x10c (descriptive)
};

struct CAurCameraLight {
    char pad_0x0[0x3c];
    Vector m_vPosition;                             // 0x3c (descriptive)
    int m_bEnabled;                                 // 0x48 (descriptive)
};

class CAurCamera {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40();
    virtual void SetFieldOfView(float fFov);        // 0x44 (descriptive)
    virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70();
    virtual void AttachToHook(CAurSceneModel* pObject, const char* sHook, int a);   // 0x74 (descriptive)
    virtual void v78(); virtual void v7C();
    virtual CAurCameraLight* GetLight(int nId);     // 0x80 (descriptive)
};

class CSWGuiScene {
public:
    CAurSceneModel* GetModel(int n);                // 0x00416750

    void* m_pVtable;                                // 0x00
    char pad_0x04[0x18 - 0x4];
    CAurCamera* m_pCamera;                          // 0x18
};

class CSWGui3DSceneView {
public:
    char pad_0x0[0x5c];
    CSWGuiScene m_cScene;                           // 0x5c
};

class CSWSCreature {
public:
    char pad_0x0[0x18];
    CExoString m_sTag;                              // 0x18
};

class CSWSObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual CSWSCreature* AsSWSCreature();          // 0x30
};

class CServerExoApp;

class CAppManager {
public:
    void* m_pUnknown00;
    void* m_pClientExoApp;                          // 0x04
    CServerExoApp* m_pServerExoApp;                 // 0x08

    CServerExoApp* GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC
extern int g_nForcedGuiAlignment;                   // 0x007A22F8 (descriptive, -1 = unset)

class CSWCCreatureStats {
public:
    char pad_0x0[0x80];
    short m_nGoodEvil;                              // 0x80

    short GetGoodEvil() { return m_nGoodEvil; }
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
    virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC();
    virtual void vC0(); virtual void vC4(); virtual void vC8(); virtual void vCC();
    virtual void vD0(); virtual void vD4();
    virtual float GetCameraHeight();                // 0xd8 (descriptive)

    void Setup3DScene(CSWGui3DSceneView& cView, CSWCCreature* pSource, CSWSCreature* pServer, int bPlay);

    CSWSObject* GetServerObject();                  // 0x0063D4B0 (CSWCObjectServerLink)
    CSWSCreature* GetServerCreature()
    {
        return GetServerObject() ? GetServerObject()->AsSWSCreature() : 0;
    }
    CSWCAnimBase* GetAnimationBase();               // 0x0063C070 (CSWCObject)

    unsigned long m_idSelf;                         // 0x04
    char pad_0x8[0x2f8 - 0x8];
    CSWCCreatureStats* m_pStats;                    // 0x2f8
};

// Poses the creature's GUI model (character sheet / level-up 3D view): plays
// the alignment idle on head and body, the alignment animation on the scene
// model, and attaches the scene camera to the creature's camera hook.
// FUNCTION: SWKOTOR 0x006100F0
void CSWCCreature::Setup3DScene(CSWGui3DSceneView& cView, CSWCCreature* pSource, CSWSCreature* pServer, int bPlay)
{
    short nGoodEvil;
    if (pSource) {
        nGoodEvil = pSource->m_pStats->m_nGoodEvil;
    } else {
        nGoodEvil = m_pStats->m_nGoodEvil;
    }
    int nAlignment = nGoodEvil;
    if (g_nForcedGuiAlignment != -1) {
        nAlignment = g_nForcedGuiAlignment;
    }
    CExoString sAnimation;
    CExoString sAlignment;
    if (nAlignment == 0) {
        sAlignment = "evil";
    } else if (nAlignment == 100) {
        sAlignment = "good";
    } else {
        sAlignment.Format("align%d", nAlignment / 5 + 1);
    }
    if (!pServer && g_pAppManager->GetServerExoApp()) {
        pServer = pSource ? pSource->GetServerCreature() : GetServerCreature();
    }
    if (pServer && (pServer->m_sTag == "t3m4" || pServer->m_sTag == "hk47")) {
        sAnimation = "pause1";
    } else if (nAlignment < 40) {
        sAnimation = "evil";
    } else if (nAlignment >= 60) {
        sAnimation = "good";
    } else {
        sAnimation = "neutral";
    }
    CGob* pPart = GetAnimationBase()->GetPartModel(0xff);
    if (pPart && bPlay) {
        pPart->PlayAnimation(sAnimation.CStr(), 1.0f, 2, 0);
    }
    pPart = GetAnimationBase()->GetPartModel(0xfe);
    if (pPart && bPlay) {
        pPart->PlayAnimation(sAnimation.CStr(), 1.0f, 2, 0);
    }
    CAurSceneModel* pModel = cView.m_cScene.GetModel(0);
    if (pModel) {
        float fLength;
        pModel->GetAnimationLength(sAlignment.CStr(), 0, &fLength);
        pModel->PlayAnimation((fLength != -1.0f ? sAlignment : sAnimation).CStr(), 1.0f, 2, 0);
        CExoString sHook;
        if (!pServer) {
            sHook = "camerahook";
        } else if (pServer->m_sTag == "t3m4") {
            sHook = "camerahookt";
        } else if (pServer->m_sTag == "hk47") {
            sHook = "camerahookh";
        } else if (pServer->m_sTag == "zaalbar") {
            sHook = "camerahookz";
        } else {
            sHook = "camerahook";
        }
        if (!pModel->HasHook(sHook.CStr())) {
            sHook = "camerahook";
        }
        cView.m_cScene.m_pCamera->AttachToHook(pModel, sHook.CStr(), 1);
        cView.m_cScene.m_pCamera->SetFieldOfView(22.726f);
        CAurCameraLight* pLight = cView.m_cScene.m_pCamera->GetLight(0x3ea);
        if (pLight) {
            pLight->m_bEnabled = 1;
            pLight->m_vPosition = Vector(0.0f, 0.0f, GetCameraHeight());
        }
    }
}
