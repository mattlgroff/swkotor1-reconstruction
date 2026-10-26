// CSWCModule dialogue-camera queries and transitions.

class CExoString {
public:
    char* m_sString;
    unsigned long m_nBufferLength;
    CExoString();                                   // 0x005B3190
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    char* CStr() const;                             // 0x005E5670
};

class CResRef {
public:
    int operator!=(const char* szName) const;       // 0x00406060
    void CopyToString(CExoString& sOut) const;      // 0x00405F70

    char m_pResRef[16];
};

class CAurObject;
CAurObject* NewCAurObject(const char* szModel, const char* szParent, int nFlags, int nUnused); // 0x00449CC0

class Vector {
public:
    float x, y, z;

    Vector() { x = 0.0f; y = 0.0f; z = 0.0f; }
};

class Quaternion {
public:
    float w, x, y, z;

    Quaternion() { z = 0.0f; y = 0.0f; x = 0.0f; w = 1.0f; }
};

class Scene;

#define M_DLG_VSLOTS4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();

// Dialogue camera model (an Aurora object) owned at module +0x12c.
class CCameraBehavior {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14();
    virtual void PlayAnimation(const char* szAnim, float fSpeed, int nMode, int nUnused);  // 0x18
    virtual void* GetEventTime(const char* szEvent, float* pStart, float* pEnd);          // 0x1c
    M_DLG_VSLOTS4(v20) M_DLG_VSLOTS4(v30)
    virtual void v40a(); virtual void v40b(); virtual void v40c();
    virtual void AttachToScene(Scene* pScene);                                             // 0x4c
    virtual void v50(); virtual void v54(); virtual void v58();
    virtual Vector SetPosition(Vector vPosition);                                          // 0x5c
    virtual Quaternion SetOrientation(Quaternion qOrientation);                            // 0x60
    virtual void v64(); virtual void v68(); virtual void v6C(); virtual void v70(); virtual void v74(); virtual void v78();
    virtual void v7C(); virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C(); virtual void v90();
    virtual void v94(); virtual void v98(); virtual void v9C(); virtual void vA0(); virtual void vA4(); virtual void vA8();
    virtual void vAC(); virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC(); virtual void vC0();
    virtual void vC4(); virtual void vC8(); virtual void vCC(); virtual void vD0(); virtual void vD4(); virtual void vD8();
    virtual void vDC(); virtual void vE0(); virtual void vE4(); virtual void vE8(); virtual void vEC(); virtual void vF0();
    virtual void vF4(); virtual void vF8(); virtual void vFC(); virtual void v100(); virtual void v104(); virtual void v108();
    virtual void v10C(); virtual void v110(); virtual void v114(); virtual void v118(); virtual void v11C(); virtual void v120();
    virtual void v124(); virtual void v128(); virtual void v12C();
    virtual void StartAnimations();                                                        // 0x130, descriptive
    virtual void v134();
    virtual void ResetAnimations();                                                        // 0x138, descriptive
};

class CSWCameraSettings;
class CAurBehavior;
class CSWCameraOnAStick {
public:
    void SaveCameraSettings(CSWCameraSettings* pSettings);  // 0x0063BA80
};

class Gob;
class CSWCModuleCamera {
public:
    M_DLG_VSLOTS4(m00) M_DLG_VSLOTS4(m10) M_DLG_VSLOTS4(m20) M_DLG_VSLOTS4(m30)
    virtual void SetClipPlanes(float fNear, float fFar);                                   // 0x40
    virtual void m44(); virtual void m48(); virtual void m4c();
    M_DLG_VSLOTS4(m50) M_DLG_VSLOTS4(m60)
    virtual void m70();
    virtual void AttachCamera(CCameraBehavior* pCamera, const char* szHook, int nFlags);    // 0x74
    virtual void m78(); virtual void m7c();
    virtual CSWCameraOnAStick* GetBehavior(int nType);                                     // 0x80
};

class CSWCArea {
public:
    char pad_0x0[0x184];
    Scene* m_pScene;                                // 0x184
};

class CSWCObject;
class CGameObject {
public:
    virtual void g00(); virtual void g04(); virtual void g08();
    virtual CSWCObject* AsSWCObject();              // 0x0c
};

class CGuiInGame {
public:
    CExoString GetDialogCameraAnimationName(unsigned short nAnimation);  // 0x006288F0, descriptive
    int IsLoopingDialogStuntAnimation(unsigned short nAnimation);        // 0x0062AB70
    void SetDeferredFOVCallForAnimatedCutscenes(float fFov);             // 0x0062B470
};

struct CClientOptions {
    char pad_0x0[0x6d];
    unsigned char m_nCameraMode;                    // 0x6d
};

class CClientExoApp {
public:
    CGameObject* GetGameObject(unsigned long oid);  // 0x005ED580
    CGuiInGame* GetGuiInGame();                     // 0x005ED690
    CClientOptions* GetClientOptions();             // 0x005ED700
};

class CAppManager {
public:
    void* pad;
    CClientExoApp* m_pClientExoApp;
};
extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CSWCameraSettings {
public:
    char pad_0x0[0x20];
};

class CSWCModule {
public:
    int IsCameraAnimated();
    void SetCameraModel(CResRef* pResRef);
    float SetAnimatedCamera(unsigned short nAnimation, float fFov, unsigned long oidObject);

    char pad_0x0[0x40];
    CSWCModuleCamera* m_pModuleCamera;              // 0x40
    char pad_0x44[0x48 - 0x44];
    CSWCArea* m_pArea;                              // 0x48
    char pad_0x4c[0x10c - 0x4c];
    CSWCameraSettings m_cSavedCamera;               // 0x10c
    CCameraBehavior* m_pCamera;
    CExoString m_sCameraName;
};

// FUNCTION: SWKOTOR 0x006412B0
int CSWCModule::IsCameraAnimated()
{
    if (m_pCamera)
        return m_pCamera->GetEventTime(m_sCameraName.CStr(), 0, 0) != 0;
    return 0;
}

// FUNCTION: SWKOTOR 0x00640F70
void CSWCModule::SetCameraModel(CResRef* pResRef)
{
    if (m_pCamera)
        return;
    if (*pResRef != "") {
        CExoString sModel;
        pResRef->CopyToString(sModel);
        m_pCamera = (CCameraBehavior*)NewCAurObject(sModel.CStr(), "", 0, 0);
    }
}


// FUNCTION: SWKOTOR 0x00641010
float CSWCModule::SetAnimatedCamera(unsigned short nAnimation, float fFov, unsigned long oidObject)
{
    if (oidObject == 0x7f000000 || !m_pCamera)
        return 0.0f;

    unsigned char nMode = g_pAppManager->m_pClientExoApp->GetClientOptions()->m_nCameraMode;
    if (nMode == 3) {
        CSWCameraOnAStick* pStick = m_pModuleCamera->GetBehavior(0x106a);
        if (pStick)
            pStick->SaveCameraSettings(&m_cSavedCamera);
    }

    CSWCModuleCamera* pModuleCamera = m_pModuleCamera;
    pModuleCamera->SetClipPlanes(0.1f, 10000.0f);
    Scene* pScene = m_pArea->m_pScene;
    m_pCamera->AttachToScene(pScene);

    CGameObject* pObject = g_pAppManager->m_pClientExoApp->GetGameObject(oidObject);
    if (!pObject || !pObject->AsSWCObject())
        return 0.0f;
    pObject->AsSWCObject();

    Vector vPosition;
    Quaternion qOrientation;
    m_pCamera->SetPosition(vPosition);
    m_pCamera->SetOrientation(qOrientation);
    pModuleCamera->AttachCamera(m_pCamera, "camerahook", 0);

    CGuiInGame* pGui = g_pAppManager->m_pClientExoApp->GetGuiInGame();
    m_sCameraName = pGui->GetDialogCameraAnimationName(nAnimation);
    m_pCamera->StartAnimations();
    m_pCamera->ResetAnimations();

    float fStart;
    float fLength;
    m_pCamera->GetEventTime(m_sCameraName.CStr(), &fStart, &fLength);
    if (fLength <= 0.0f)
        fLength = 0.0f;
    if (pGui->IsLoopingDialogStuntAnimation(nAnimation))
        m_pCamera->PlayAnimation(m_sCameraName.CStr(), 1.0f, 2, 0);
    else
        m_pCamera->PlayAnimation(m_sCameraName.CStr(), 1.0f, 3, 0);
    if (fFov > 0.0f) {
        CClientExoApp* pApp = g_pAppManager->m_pClientExoApp;
        pApp->GetGuiInGame()->SetDeferredFOVCallForAnimatedCutscenes(fFov);
    }
    return fLength;
}
