// CClientExoAppInternal::SetInputClass (clientinput.o). Standalone layout:
// +0x18 module, +0x40 in-game GUI, +0x9C input class, +0x28C pending player
// id, +0x2A0 player control.
struct Vector {
    float x,y,z;
    Vector() {}
    Vector(float fx,float fy,float fz) { x = fx; y = fy; z = fz; }
};
class CExoInput { public: void ClearEvents(); /* 0x005DF4E0 */ };
extern CExoInput* g_pExoInput; // 0x007A39E4
class CGameObject { public: virtual ~CGameObject(); unsigned long m_idSelf; };
class CSWSCreature : public CGameObject {};
class CServerExoApp { public: CSWSCreature* GetPlayerCreature(); /* 0x004B2820 */ };
class CClientExoApp { public: unsigned long ServerToClientObjectId(unsigned long oid); /* 0x005EDA50 */ };
class CAppManager { public: void* m_p0; CClientExoApp* m_pClientExoApp; CServerExoApp* m_pServerExoApp; };
extern CAppManager* g_pAppManager; // 0x007A39FC
extern int g_nDefaultCombatMode; // 0x0074F224
class CSWCCreature { public: char pad_0x0[0x30]; Vector m_vPosition; void SetCombatMode(int nMode); /* 0x0060F090 */ };
class CSWMiniCamera {
public:
    char pad_0x0[0x124]; int m_n124; Vector m_vPosition;
};
class CSWMiniGame {
public:
    CSWMiniCamera* SetCamera(CSWMiniCamera* p); // 0x00671670
    void Go(); // 0x006714E0
};
class CSWCArea { public: char pad_0x0[0x264]; CSWMiniGame* m_pMiniGame; };
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
    virtual CSWMiniCamera* GetCamera(int nId); // +0x80
};
class CSWCModule { public: char pad_0x0[0x40]; CAurScene* m_pScene; char pad_0x44[4]; CSWCArea* m_pArea; };
class CGuiInGame {
public:
    char pad_0x0[0x30]; int m_bInputActive; int m_nMode;
    void SetSWGuiStatus(int nStatus,int bArg); // 0x0062AA00
};
class CSWPlayerControl { public: void SetEnabled(int b); /* 0x006792E0 */ };
class CClientExoAppInternal {
public:
    char pad_0x0[0x18]; CSWCModule* m_pModule;
    char pad_0x1c[0x24]; CGuiInGame* m_pGuiInGame;
    char pad_0x44[0x58]; int m_nInputClass;
    char pad_0xa0[0x1ec]; unsigned long m_oidPendingPlayer;
    char pad_0x290[0x10]; CSWPlayerControl* m_pPlayerControl;
    CSWCCreature* GetPlayerCreature(); // 0x005F1420
    void SetPlayerCreature(unsigned long oid); // 0x005F2BB0
    void IndicateObject(int a,int b); // 0x005F2C60
    void AttachPlayerGameObjectToArea(); // 0x005F5CF0
    void SetInputState(int nMask); // 0x0061F9C0
    void ClearInputState(int nMask); // 0x0061FA00
    void SetInputClass(int nClass,int bForce);
};

// FUNCTION: SWKOTOR 0x006200E0
void CClientExoAppInternal::SetInputClass(int nClass,int bForce)
{
    if (m_nInputClass == nClass) return;
    int nOld = m_nInputClass;
    m_nInputClass = nClass;
    g_pExoInput->ClearEvents();
    switch (nClass) {
    case 0:
        if (m_oidPendingPlayer != 0x7f000000) {
            CSWSCreature* pCreature = g_pAppManager->m_pServerExoApp->GetPlayerCreature();
            if (pCreature) {
                unsigned long oid = pCreature->m_idSelf;
                SetPlayerCreature(g_pAppManager->m_pClientExoApp->ServerToClientObjectId(oid));
                m_oidPendingPlayer = 0x7f000000;
                AttachPlayerGameObjectToArea();
            }
        }
        if (m_pModule && m_pModule->m_pScene) {
            Vector v(0.0f,0.0f,0.0f);
            if (GetPlayerCreature()) v = GetPlayerCreature()->m_vPosition;
            CSWMiniCamera* pCamera = m_pModule->m_pScene->GetCamera(0x106a);
            if (pCamera) {
                pCamera->m_n124 = 0;
                pCamera->m_vPosition = v;
            }
        }
        if (GetPlayerCreature()) GetPlayerCreature()->SetCombatMode(g_nDefaultCombatMode);
        if (m_pPlayerControl) m_pPlayerControl->SetEnabled(1);
        ClearInputState(1);
        if (m_pGuiInGame) m_pGuiInGame->SetSWGuiStatus(4,1);
        m_pGuiInGame->m_bInputActive = 0;
        break;
    case 3:
        IndicateObject(0,0);
        break;
    case 2:
        if (m_pGuiInGame && m_pGuiInGame->m_nMode == 1) {
            if (bForce) m_pGuiInGame->SetSWGuiStatus(3,1);
            ClearInputState(1);
        }
        m_pGuiInGame->m_bInputActive = 1;
        break;
    case 1:
        CSWMiniGame* pMiniGame;
        if (m_pModule && m_pModule->m_pArea && (pMiniGame = m_pModule->m_pArea->m_pMiniGame) != 0) {
            IndicateObject(0,0);
            SetInputState(1);
            if (m_pGuiInGame) m_pGuiInGame->SetSWGuiStatus(2,0);
            pMiniGame->SetCamera((CSWMiniCamera*)m_pModule->m_pScene);
            pMiniGame->Go();
        } else {
            SetInputClass(nOld,1);
        }
        break;
    }
}
