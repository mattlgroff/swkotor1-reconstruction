// CGuiInGame::SetComputerCameraShot (guiingame.o). Standalone layout copied
// from gui_in_game_set_shot.cpp.
struct Vector { float x,y,z; };
class Quaternion {
public:
    float w,x,y,z;
    Quaternion() { z = 0.0f; y = 0.0f; x = 0.0f; w = 1.0f; }
};
Quaternion EulerToQuaternion(float fYaw,float fPitch,float fRoll); // 0x004ACAC0
float GetRoll(const Quaternion& q); // 0x004AA020
float GetPitch(const Quaternion& q); // 0x004A9F40
float GetEulerRoll(Quaternion q); // 0x004ACB90, descriptive
struct CSWPlaceableCamera {
    unsigned long m_nId; Vector m_vPosition; Quaternion m_qOrientation;
    float m_fPitch,m_fHeight,m_fFieldOfView,m_fF2C;
};
class CAurCamera { public: void SetShot(int nType,Vector vPosition,Quaternion qOrientation,float fFieldOfView); /* 0x006BB590 */ };
class CAurScene {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7c();
    virtual CAurCamera* GetCamera(int nId); // +0x80
};
class CSWCModule { public: char pad_0x0[0x40]; CAurScene* m_pScene; CAurScene* GetScene() { return m_pScene; } };
class CClientExoApp {
public:
    CSWCModule* GetModule(); // 0x005ED530
    void EnableVideoEffect(int nEffect); // 0x005EDF10
};
class CAppManager { public: void* m_p0; CClientExoApp* m_pClientExoApp; CClientExoApp* GetClientExoApp() { return m_pClientExoApp; } };
extern CAppManager* g_pAppManager; // 0x007A39FC
class CSWGuiPanel {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74();
    virtual void SetSpeaker(unsigned long oid);     // +0x78, descriptive
    char pad_0x4[0x19c0 - 0x4];
    unsigned long m_oidOwner;                       // +0x19C0, descriptive
};
class CSWGuiManager {
public:
    void AddPanel(CSWGuiPanel* pPanel, int nMode, int bArg);   // 0x0040BC70
    int RemovePanel(CSWGuiPanel* pPanel);           // 0x0040C830
};
class CGuiInGame {
public:
    char pad_0x0[0x38]; CSWGuiManager* m_pGuiManager; CSWGuiPanel* m_pDialog; char pad_0x40[4]; CSWGuiPanel* m_pDialogComputer;
    CSWGuiPanel* m_pDialogCamera;
    char pad_0x4c[0x68]; int m_nGlobalDialogState;
    char pad_0xb8[0x2c]; int m_bShotSet; Vector m_vShotPosition; float m_fShotF4;
    char pad_0xf8[0x68]; int m_n160;
    char pad_0x164[0x4c]; int m_nPlaceableCameras; CSWPlaceableCamera m_cameras[50];
    char pad_0xb14[0xc]; int m_bShotCameraInitialized;
    int InitializeShotCamera(); // 0x0062BF30
    void SetComputerCameraShot(int nShot,int n160,int nEffect); // 0x0062C030
    void SetShotFromScript(int nShot,int nEffect);
};

// FUNCTION: SWKOTOR 0x0062C030
void CGuiInGame::SetComputerCameraShot(int nShot,int n160,int nEffect)
{
    if (!m_nPlaceableCameras) return;
    m_pGuiManager->RemovePanel(m_pDialog);
    m_pGuiManager->AddPanel(m_pDialogCamera,0,1);
    m_pDialogCamera->SetSpeaker(m_pDialog->m_oidOwner);
    CAurCamera* pCamera = g_pAppManager->m_pClientExoApp->GetModule()->GetScene()->GetCamera(0x106d);
    if (pCamera) {
        int bFound = 0;
        int i;
        for (i = 0; i < m_nPlaceableCameras; i++) {
            if (m_cameras[i].m_nId == (unsigned long)nShot) {
                bFound = 1;
                break;
            }
        }
        Vector vPosition;
        vPosition.x = 0.0f; vPosition.y = 0.0f; vPosition.z = 0.0f;
        Quaternion q;
        float fFov = 55.0f;
        float fF2C = 0.0f;
        if (bFound) {
            vPosition = m_cameras[i].m_vPosition;
            vPosition.z += m_cameras[i].m_fHeight;
            q = m_cameras[i].m_qOrientation;
            q = EulerToQuaternion(GetPitch(q),GetRoll(q) + m_cameras[i].m_fPitch,GetEulerRoll(q));
            fFov = m_cameras[i].m_fFieldOfView;
            fF2C = m_cameras[i].m_fF2C;
        }
        pCamera->SetShot(6,vPosition,q,fFov);
        m_bShotSet = 1;
        m_vShotPosition = vPosition;
        m_fShotF4 = fF2C;
        if (nEffect == -1)
            nEffect = 0;
        if (nEffect != -2)
            g_pAppManager->m_pClientExoApp->EnableVideoEffect(nEffect);
    }
    m_n160 = n160;
}
