// CSWGuiInGameGalaxyMap planet cycling (swguiingamemap.o). The two cycling
// methods and the party-table planet queries have descriptive names.

class CExoString {
public:
    char* m_sString;
    unsigned long m_nBufferLength;
    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
};

class CResRef {
public:
    void CopyToString(CExoString& sString) const;   // 0x00405F70
    int IsValid() const;                            // 0x00406030
    char m_resRef[16];
};

class CTlkTable {
public:
    CExoString GetSimpleString(unsigned long nStrRef);  // 0x0041E8F0
};

extern CTlkTable* g_pTlkTable;                      // 0x007A3A08
extern int g_bDo3DGui;                              // 0x0078D1E4 (descriptive)

class CSWGuiTextParams {
public:
    void SetText(const CExoString& sText);          // 0x00415E00
};

class CAurObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14();
    virtual void PlayAnimation(const char* sAnim, float fSpeed, int a, int b);  // 0x18 (descriptive)
};

class CAurCamera {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40();
    virtual void SetFieldOfView(float fFov);        // 0x44 (descriptive)
    virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70();
    virtual void AttachToHook(CAurObject* pObject, const char* sHook, int a);   // 0x74 (descriptive)
};

class CSWGuiScene {
public:
    virtual ~CSWGuiScene();                         // 0x004165E0
    void RemoveModel(int nIndex, int bDelete);      // 0x00416710
    CAurObject* AddModel(const CExoString& sModel, int nPart);  // 0x00417620
    char pad_0x04[0x10];
    void* m_pScene;                                 // 0x14
    CAurCamera* m_pCamera;                          // 0x18
    char pad_0x1c[0x20 - 0x1c];
    unsigned long m_nModels;                        // 0x20 (descriptive)
    char pad_0x24[0x34 - 0x24];
};

// One galaxy-map planet entry (descriptive).
struct CSWGuiGalaxyPlanet {
    unsigned long m_nNameStrRef;
    unsigned long m_nDescriptionStrRef;
    CResRef m_cModel;
};

class CSWPartyTable {
public:
    int GetIsPlanetSelectable(int nPlanet);         // 0x00563B00 (descriptive name)
    int GetIsPlanetAvailable(int nPlanet);          // 0x00563B40 (descriptive name)
    void SetSelectedPlanet(int nPlanet);            // 0x00563B60

    char pad_0x0[0xe0];
    int m_nSelectedPlanet;                          // 0xe0
};

class CServerExoApp {
public:
    CSWPartyTable* GetPartyTable();                 // 0x004AEE70
};

class CVirtualMachine {
public:
    int RunScript(CExoString* psScript, unsigned long oidSelf, int bOid);  // 0x005D0FC0
};

extern CVirtualMachine* g_pVirtualMachine;          // 0x007A3A00

class CGuiInGame {
public:
    void HideGalaxyMapGui();                        // 0x0062D1D0
    char pad_0x0[0xc04];
    int m_bRunningScript;                           // 0xc04 (placeholder name)
};

class CClientExoApp {
public:
    CGuiInGame* GetInGameGui();                     // 0x005ED690
};

class CAppManager {
public:
    char pad_0x0[0x4];
    CClientExoApp* m_pClientExoApp;                 // 0x04
    CServerExoApp* m_pServerExoApp;                 // 0x08
    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

class CSWGuiManager {
public:
    void PlayGuiSound(char nSound);                 // 0x0040A140
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CSWGuiButton {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void SetSelected(int bSelected);        // 0x40 (placeholder name)

    char pad_0x4[0x44 - 0x4];
    unsigned long m_bFlag0 : 1;                     // 0x44 bit 0 (placeholder name)
    unsigned long m_bAvailable : 1;                 // 0x44 bit 1 (descriptive)
    char pad_0x48[0x1c4 - 0x48];
};

// Planet button as seen by the click handler (descriptive members).
class CSWGuiControl {
public:
    char pad_0x0[0x4c];
    int m_bEnabled;                                 // 0x4c (placeholder name)
    char pad_0x50[0x58 - 0x50];
    int m_nId;                                      // 0x58 (placeholder name)
};

class CSWGuiPanel {
public:
    virtual ~CSWGuiPanel();                         // 0x00 (0x0040CF70)
    virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30();
    virtual void Draw(float fDelta);                // 0x34 (0x0040B760)
    virtual void v38();
    virtual void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x00409E60)
    virtual void v40(); virtual void v44();
    virtual void OnPanelAdded();                    // 0x48 (0x0040B870)
    virtual void OnPanelRemoved();                  // 0x4c (0x0040C170)

    char pad_0x4[0x18 - 0x4];
    CSWGuiManager* m_pManager;                      // 0x18
    char pad_0x1c[0x64 - 0x1c];
};

class CSWGuiInGameGalaxyMap : public CSWGuiPanel {
public:
    virtual void Draw(float fDelta);
    virtual void HandleInputEvent(int nEvent, int bDown);
    virtual void OnPanelAdded();
    virtual void OnPanelRemoved();
    void OnPlanetClicked(CSWGuiControl* pControl);
    void SelectNextPlanet();
    void SelectPreviousPlanet();
    void DisplayPlanet(int nPlanet, int bSelect);   // 0x006933A0

    CSWGuiButton m_aPlanetButtons[16];              // 0x64
    char pad_0x1ca4[0x1d8c - 0x1ca4];
    CSWGuiTextParams m_cPlanetName;                 // 0x1d8c (descriptive)
    char pad_0x1d8d[0x1ecc - 0x1d8d];
    CSWGuiTextParams m_cPlanetDescription;          // 0x1ecc (descriptive)
    char pad_0x1ecd[0x2398 - 0x1ecd];
    CSWGuiScene m_cScene;                           // 0x2398
    CSWGuiGalaxyPlanet m_aPlanets[16];              // 0x23cc
    int m_nDisplayedPlanet;                         // 0x254c (descriptive)
};

// FUNCTION: SWKOTOR 0x006933A0
void CSWGuiInGameGalaxyMap::DisplayPlanet(int nPlanet, int bSelect)
{
    CExoString sName;
    CExoString sDescription;
    if (nPlanet == -1 || nPlanet == m_nDisplayedPlanet) {
        return;
    }
    if (m_aPlanets[nPlanet].m_nNameStrRef) {
        sName = g_pTlkTable->GetSimpleString(m_aPlanets[nPlanet].m_nNameStrRef);
    }
    if (m_aPlanets[nPlanet].m_nDescriptionStrRef) {
        sDescription = g_pTlkTable->GetSimpleString(m_aPlanets[nPlanet].m_nDescriptionStrRef);
    }
    m_cPlanetDescription.SetText(sDescription);
    m_cPlanetName.SetText(sName);
    if (m_cScene.m_nModels > 0) {
        m_cScene.RemoveModel(0, 1);
    }
    if (m_aPlanets[nPlanet].m_cModel.IsValid()) {
        CExoString sModel;
        m_aPlanets[nPlanet].m_cModel.CopyToString(sModel);
        CAurCamera* pCamera = m_cScene.m_pCamera;
        if (g_bDo3DGui) {
            CAurObject* pModel = m_cScene.AddModel(sModel, -1);
            if (pModel) {
                if (bSelect) {
                    pModel->PlayAnimation("zoomin", 1.0f, 1, 0);
                    pModel->PlayAnimation("rotate", 1.0f, 0x20, 0);
                }
                pCamera->AttachToHook(pModel, "camerahook", 0);
                pCamera->SetFieldOfView(22.726f);
            }
        }
    }
    m_nDisplayedPlanet = nPlanet;
}

// FUNCTION: SWKOTOR 0x00694BF0
void CSWGuiInGameGalaxyMap::SelectNextPlanet()
{
    CSWPartyTable* pTable = g_pAppManager->m_pServerExoApp->GetPartyTable();
    if (pTable->m_nSelectedPlanet == -1) {
        return;
    }
    (&m_aPlanetButtons[pTable->m_nSelectedPlanet])->SetSelected(0);
    for (int nPlanet = pTable->m_nSelectedPlanet + 1; nPlanet != pTable->m_nSelectedPlanet; nPlanet++) {
        if (nPlanet >= 16) {
            nPlanet = 0;
        }
        if (pTable->GetIsPlanetAvailable(nPlanet) && pTable->GetIsPlanetSelectable(nPlanet)) {
            pTable->SetSelectedPlanet(nPlanet);
            break;
        }
    }
    (&m_aPlanetButtons[pTable->m_nSelectedPlanet])->SetSelected(1);
    DisplayPlanet(pTable->m_nSelectedPlanet, 1);
}

// FUNCTION: SWKOTOR 0x00694CA0
void CSWGuiInGameGalaxyMap::SelectPreviousPlanet()
{
    CSWPartyTable* pTable = g_pAppManager->m_pServerExoApp->GetPartyTable();
    if (pTable->m_nSelectedPlanet == -1) {
        return;
    }
    (&m_aPlanetButtons[pTable->m_nSelectedPlanet])->SetSelected(0);
    for (int nPlanet = pTable->m_nSelectedPlanet - 1; nPlanet != pTable->m_nSelectedPlanet; nPlanet--) {
        if (nPlanet < 0) {
            nPlanet = 15;
        }
        if (pTable->GetIsPlanetAvailable(nPlanet) && pTable->GetIsPlanetSelectable(nPlanet)) {
            pTable->SetSelectedPlanet(nPlanet);
            break;
        }
    }
    (&m_aPlanetButtons[pTable->m_nSelectedPlanet])->SetSelected(1);
    DisplayPlanet(pTable->m_nSelectedPlanet, 1);
}

// FUNCTION: SWKOTOR 0x006935A0
void CSWGuiInGameGalaxyMap::OnPlanetClicked(CSWGuiControl* pControl)
{
    if (pControl->m_bEnabled == 0) {
        return;
    }
    CSWPartyTable* pTable = g_pAppManager->m_pServerExoApp->GetPartyTable();
    if (pTable->m_nSelectedPlanet != -1 && pTable->m_nSelectedPlanet == pControl->m_nId) {
        return;
    }
    if (pTable->m_nSelectedPlanet != -1) {
        (&m_aPlanetButtons[pTable->m_nSelectedPlanet])->SetSelected(0);
    }
    pTable->SetSelectedPlanet(pControl->m_nId);
    DisplayPlanet(pTable->m_nSelectedPlanet, 1);
}

// Not in the function table: starts after OnPlanetClicked's padding.
// FUNCTION: SWKOTOR 0x00693610
void CSWGuiInGameGalaxyMap::Draw(float fDelta)
{
    CSWPartyTable* pTable = g_pAppManager->m_pServerExoApp->GetPartyTable();
    if (pTable->m_nSelectedPlanet != -1) {
        (&m_aPlanetButtons[pTable->m_nSelectedPlanet])->SetSelected(1);
    }
    CSWGuiPanel::Draw(fDelta);
}

// FUNCTION: SWKOTOR 0x00694B40
void CSWGuiInGameGalaxyMap::OnPanelAdded()
{
    CSWPartyTable* pTable = g_pAppManager->m_pServerExoApp->GetPartyTable();
    if (pTable->m_nSelectedPlanet == -1) {
        for (int nPlanet = 0; nPlanet < 16; nPlanet++) {
            if (pTable->GetIsPlanetAvailable(nPlanet) && pTable->GetIsPlanetSelectable(nPlanet)) {
                pTable->SetSelectedPlanet(nPlanet);
                break;
            }
        }
    }
    if (pTable->m_nSelectedPlanet != -1) {
        (&m_aPlanetButtons[pTable->m_nSelectedPlanet])->SetSelected(1);
    }
    DisplayPlanet(pTable->m_nSelectedPlanet, 1);
    for (int i = 0; i < 16; i++) {
        m_aPlanetButtons[i].m_bAvailable = pTable->GetIsPlanetAvailable(i);
    }
    CSWGuiPanel::OnPanelAdded();
}

// Not in the function table.
// FUNCTION: SWKOTOR 0x006927D0
void CSWGuiInGameGalaxyMap::OnPanelRemoved()
{
    for (int i = 0; i < 16; i++) {
        m_aPlanetButtons[i].SetSelected(0);
    }
    m_nDisplayedPlanet = -1;
    CSWGuiPanel::OnPanelRemoved();
}

// Not in the function table.
// FUNCTION: SWKOTOR 0x00695980
void CSWGuiInGameGalaxyMap::HandleInputEvent(int nEvent, int bDown)
{
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
    if (!bDown) {
        return;
    }
    switch (nEvent) {
    case 0x27:
    case 0x2d:
        m_pManager->PlayGuiSound(0);
        if (g_pVirtualMachine) {
            g_pAppManager->GetClientExoApp()->GetInGameGui()->m_bRunningScript = 1;
            CExoString sScript("k_sup_galaxymap");
            g_pVirtualMachine->RunScript(&sScript, 0x7f000000, 0);
            g_pAppManager->GetClientExoApp()->GetInGameGui()->m_bRunningScript = 0;
        }
        g_pAppManager->GetClientExoApp()->GetInGameGui()->HideGalaxyMapGui();
        break;
    case 0x28:
    case 0x2e:
    case 0xdf:
        m_pManager->PlayGuiSound(0);
        g_pAppManager->GetClientExoApp()->GetInGameGui()->HideGalaxyMapGui();
        break;
    case 0x2f:
    case 0x31:
    case 0x3d:
    case 0x3f:
        m_pManager->PlayGuiSound(1);
        SelectPreviousPlanet();
        break;
    case 0x30:
    case 0x32:
    case 0x3e:
    case 0x40:
        m_pManager->PlayGuiSound(1);
        SelectNextPlanet();
        break;
    }
}
