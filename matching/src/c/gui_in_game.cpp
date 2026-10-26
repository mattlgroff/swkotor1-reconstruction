// CGuiInGame (b005e2_client_app_lifecycle, b0062 message leaves, b0062b,
// b0063 client gui panels, b0064 hide handlers, b0065 virtual leaves).

#include "c/client_exo_app.h"

class CSWGuiManagerPanels {
public:
    void AddPanel(CSWGuiPanel* pPanel, int a, int b);   // 0x0040BC70
    int PanelExists(CSWGuiPanel* pPanel);               // 0x0040BD70
    void PopModalPanel();                               // 0x0040BE00
    void RemovePanel(CSWGuiPanel* pPanel);              // 0x0040C830
};

class CSWGuiPanelV : public CSWGuiPanel {
public:
    void SetVisible(int b);                             // 0x0040B9D0
};

class CSWGuiInGameMessages {
public:
    int SetMessageA(int a, int b);                      // 0x00626920
    int SetMessageB(int a, int b);                      // 0x00626B10
};

class CSWGuiBarkBubble : public CSWGuiPanel {
public:
    void SetBark(unsigned long oid, const CExoString& text, const CResRef& sound, int b);           // 0x006A9920
};

class CSWGuiFade : public CSWGuiPanelV {
public:
    void StartFade(int a, int b, int c, int d);         // 0x006244E0
};

class CSWGuiExamine : public CSWGuiPanel {
public:
    virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68();
    virtual void SetObject(unsigned long oid);          // 0x6c

    char pad_0x4[0x980];
    int m_bFromInventory;                               // 0x984
};

class CSWGuiContainer : public CSWGuiPanel {
public:
    void SetContainer(int a, int b);                    // 0x006B8130
};

class CSWGuiSubItemCreate : public CSWGuiPanel {
public:
    void SetItem(int a);                                // 0x006BDEA0
    void OnHide();                                      // 0x006BE2B0
};

struct CStatusSummary {
    int m_n[6];
};

class CSWGuiStatusPanel {
public:
    char pad_0x0[0x64];
    CStatusSummary m_summary;                           // 0x64
};

class CExoStreamingSoundSource {
public:
    ~CExoStreamingSoundSource();                        // 0x005D6200
};

class CSWSCreature {
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
    virtual void v90();
    virtual int GetIsInConversation();                  // 0x94

    int IsDebilitated();                                // 0x005B4880
    int IsDead();                                       // 0x004EF890
};

class CGuiInGame {
public:
    int IsAnimationPlayingInDialog();
    int ShowItemCreateMenu();
    void ShowBarkBubble(unsigned long oid, const CExoString& text, const CResRef& sound, int b);
    void StartGlobalFade(int a, int b, int c, int d);
    int UpdateMessageGUI();
    int GetPartyAccessPanelUp(int bOnlyFirst);
    int InitializeShotCamera();
    int ShowSubItemCreateMenu(int a);
    void FadeAndStopDialogAmbientTrack();
    void ClearDialogNonDependantList();
    void HideItemCreateMenu();
    void HideSubItemCreateMenu();
    void ShowExamineBox(unsigned long oid, int bFromInventory);
    void HideExamineBox();
    int ShowContainerGui(int a, int b);
    int HideContainerGui();
    void HideLoadModuleDebugMenu();
    void HidePowersFeatsSkillsDebugMenu();
    void UnlockDialogParticipantOrientations();
    void SetStatusSummary(const CStatusSummary& s);
    int CanPlayDialogAnimations(unsigned long oid);

    char pad_0x0[0xc];
    CSWGuiPanel* m_pPartyAccessPanel;                   // 0xc
    CSWGuiPanel* m_pPartyAccessPanel2;                  // 0x10
    char pad_0x14[0x8];
    CSWGuiInGameMessages* m_pMessages;                  // 0x1c
    char pad_0x20[0x18];
    CSWGuiManagerPanels* m_pGuiManager;                 // 0x38
    char pad_0x3c[0x10];
    CSWGuiBarkBubble* m_pBarkBubble;                    // 0x4c
    CSWGuiExamine* m_pExamineBox;                       // 0x50
    CSWGuiContainer* m_pContainerGui;                   // 0x54
    CSWGuiPanel* m_pItemCreatePanel;                    // 0x58
    CSWGuiSubItemCreate* m_pSubItemCreatePanel;         // 0x5c
    char pad_0x60[0xc];
    CSWGuiFade* m_pGlobalFade;                          // 0x6c
    CSWGuiPanel* m_pLoadModuleDebugMenu;                // 0x70
    CSWGuiPanel* m_pPowersFeatsSkillsDebugMenu;         // 0x74
    char pad_0x78[0x30];
    CSWGuiStatusPanel* m_pStatusPanel;                  // 0xa8
    CStatusSummary* m_pPendingStatusSummary;            // 0xac
    char pad_0xb0[0x14];
    unsigned long m_nShotCamera;                        // 0xc4
    char pad_0xc8[0x30];
    int m_nMessageA;                                    // 0xf8
    int m_nMessageB;                                    // 0xfc
    int m_nMessageC;                                    // 0x100
    int m_nMessageD;                                    // 0x104
    int m_bItemCreateAvailable;                         // 0x108
    char pad_0x10c[0x64];
    unsigned long m_oidDialogOwner;                     // 0x170
    unsigned long m_oidDialogSpeaker;                   // 0x174
    char pad_0x178[0x20];
    unsigned long m_nDialogParticipants;                // 0x198
    char pad_0x19c[0x4];
    unsigned long* m_pDialogParticipants;               // 0x1a0
    char pad_0x1a4[0x97c];
    int m_bShotCameraInitialized;                       // 0xb20
    char pad_0xb24[0x28];
    CExoLinkedList<CExoString> m_lstDialogNonDependant; // 0xb4c
    char pad_0xb50[0x6c];
    CExoStreamingSoundSource* m_pDialogAmbientTrack;    // 0xbbc
};

// FUNCTION: SWKOTOR 0x0062D570
int CGuiInGame::IsAnimationPlayingInDialog()
{
    int bPlaying = 0;
    for (unsigned long i = 0; i < m_nDialogParticipants; i++) {
        CSWCCreature* pCreature = g_pAppManager->GetClientExoApp()->GetGameObject(m_pDialogParticipants[i])->AsSWCCreature();
        if (pCreature) {
            bPlaying = pCreature->IsAnimationPlaying(pCreature->m_nAnimation);
            if (bPlaying) {
                return 1;
            }
        }
    }
    return bPlaying;
}

// FUNCTION: SWKOTOR 0x0062D280
int CGuiInGame::ShowItemCreateMenu()
{
    int bAvailable = m_bItemCreateAvailable;
    if (bAvailable) {
        m_pGuiManager->AddPanel(m_pItemCreatePanel, 1, 1);
        bAvailable = g_pAppManager->GetClientExoApp()->SetInputClass(2, 1);
    }
    return bAvailable;
}

// FUNCTION: SWKOTOR 0x0062AB30
void CGuiInGame::ShowBarkBubble(unsigned long oid, const CExoString& text, const CResRef& sound, int b)
{
    if (m_bItemCreateAvailable) {
        m_pBarkBubble->SetBark(oid, text, sound, b);
        m_pGuiManager->AddPanel(m_pBarkBubble, 4, 1);
    }
}

// FUNCTION: SWKOTOR 0x0062ABF0
void CGuiInGame::StartGlobalFade(int a, int b, int c, int d)
{
    if (m_pGlobalFade) {
        m_pGuiManager->AddPanel(m_pGlobalFade, 4, 1);
        m_pGlobalFade->SetVisible(1);
        m_pGlobalFade->StartFade(a, b, c, d);
    }
}

// FUNCTION: SWKOTOR 0x0062AD60
int CGuiInGame::UpdateMessageGUI()
{
    m_pMessages->SetMessageA(m_nMessageA, m_nMessageC);
    return m_pMessages->SetMessageB(m_nMessageB, m_nMessageD);
}

// FUNCTION: SWKOTOR 0x0062B4D0
int CGuiInGame::GetPartyAccessPanelUp(int bOnlyFirst)
{
    if (m_pGuiManager) {
        if (m_pGuiManager->PanelExists(m_pPartyAccessPanel)) {
            return 1;
        }
        if (!bOnlyFirst && m_pGuiManager->PanelExists(m_pPartyAccessPanel2)) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0062BF30
int CGuiInGame::InitializeShotCamera()
{
    if (!m_bShotCameraInitialized) {
        CSWCModule* pModule = g_pAppManager->GetClientExoApp()->GetModule();
        if (pModule) {
            pModule->InitializeShotCamera(m_nShotCamera);
        }
        m_bShotCameraInitialized = 1;
    }
    return m_bShotCameraInitialized;
}

// FUNCTION: SWKOTOR 0x0062D320
int CGuiInGame::ShowSubItemCreateMenu(int a)
{
    int bAvailable = m_bItemCreateAvailable;
    if (bAvailable) {
        m_pSubItemCreatePanel->SetItem(a);
        m_pGuiManager->AddPanel(m_pSubItemCreatePanel, 1, 1);
        bAvailable = g_pAppManager->GetClientExoApp()->SetInputClass(2, 1);
    }
    return bAvailable;
}

// FUNCTION: SWKOTOR 0x0062FB90
void CGuiInGame::FadeAndStopDialogAmbientTrack()
{
    if (m_pDialogAmbientTrack) {
        g_pAppManager->GetClientExoApp()->FadeAndStopStreamingSoundSource(m_pDialogAmbientTrack, 500);
        if (m_pDialogAmbientTrack) {
            delete m_pDialogAmbientTrack;
        }
        m_pDialogAmbientTrack = 0;
    }
}

// FUNCTION: SWKOTOR 0x0062F8B0
void CGuiInGame::ClearDialogNonDependantList()
{
    while (!m_lstDialogNonDependant.IsEmpty()) {
        CExoString* pString = m_lstDialogNonDependant.RemoveHead();
        if (pString) {
            delete pString;
        }
    }
}

// FUNCTION: SWKOTOR 0x0062D2B0
void CGuiInGame::HideItemCreateMenu()
{
    if (m_bItemCreateAvailable && m_pGuiManager->PanelExists(m_pItemCreatePanel)) {
        m_pGuiManager->PopModalPanel();
        m_pGuiManager->RemovePanel(m_pItemCreatePanel);
        if (g_pAppManager->GetClientExoApp()->GetMiniGame()) {
            g_pAppManager->GetClientExoApp()->SetInputClass(1, 1);
        } else {
            g_pAppManager->GetClientExoApp()->SetInputClass(0, 1);
        }
    }
}

// FUNCTION: SWKOTOR 0x0062D360
void CGuiInGame::HideSubItemCreateMenu()
{
    if (m_bItemCreateAvailable && m_pGuiManager->PanelExists(m_pSubItemCreatePanel)) {
        m_pGuiManager->PopModalPanel();
        m_pGuiManager->RemovePanel(m_pSubItemCreatePanel);
        if (g_pAppManager->GetClientExoApp()->GetMiniGame()) {
            g_pAppManager->GetClientExoApp()->SetInputClass(1, 1);
        } else {
            g_pAppManager->GetClientExoApp()->SetInputClass(0, 1);
        }
        m_pSubItemCreatePanel->OnHide();
    }
}

// FUNCTION: SWKOTOR 0x0062D3E0
void CGuiInGame::ShowExamineBox(unsigned long oid, int bFromInventory)
{
    if (m_bItemCreateAvailable) {
        m_pExamineBox->SetObject(oid);
        m_pExamineBox->m_bFromInventory = bFromInventory;
        m_pGuiManager->AddPanel(m_pExamineBox, 1, 1);
        g_pAppManager->m_pClientExoApp->SetInputClass(2, 1);
    }
}

// FUNCTION: SWKOTOR 0x0062D440
void CGuiInGame::HideExamineBox()
{
    if (m_bItemCreateAvailable && m_pGuiManager->PanelExists(m_pExamineBox)) {
        m_pGuiManager->PopModalPanel();
        m_pGuiManager->RemovePanel(m_pExamineBox);
        if (m_pExamineBox->m_bFromInventory == 1) {
            g_pAppManager->GetClientExoApp()->SetInputClass(2, 1);
        } else {
            g_pAppManager->GetClientExoApp()->SetInputClass(0, 1);
        }
    }
}

// FUNCTION: SWKOTOR 0x0062D4B0
int CGuiInGame::ShowContainerGui(int a, int b)
{
    int bAvailable = m_bItemCreateAvailable;
    if (bAvailable) {
        m_pContainerGui->SetContainer(a, b);
        m_pGuiManager->AddPanel(m_pContainerGui, 1, 1);
        g_pAppManager->GetClientExoApp()->SetInputClass(2, 1);
        bAvailable = g_pAppManager->GetClientExoApp()->SetPausedByCombat(1, 3, 0);
    }
    return bAvailable;
}

// FUNCTION: SWKOTOR 0x0062D510
int CGuiInGame::HideContainerGui()
{
    int bAvailable = m_bItemCreateAvailable;
    if (bAvailable) {
        bAvailable = m_pGuiManager->PanelExists(m_pContainerGui);
        if (bAvailable) {
            m_pGuiManager->PopModalPanel();
            m_pGuiManager->RemovePanel(m_pContainerGui);
            g_pAppManager->GetClientExoApp()->SetInputClass(0, 1);
            bAvailable = g_pAppManager->GetClientExoApp()->SetPausedByCombat(0, 0, 0);
        }
    }
    return bAvailable;
}

// FUNCTION: SWKOTOR 0x0062DA70
void CGuiInGame::HideLoadModuleDebugMenu()
{
    if (m_bItemCreateAvailable && m_pGuiManager->PanelExists(m_pLoadModuleDebugMenu)) {
        m_pGuiManager->PopModalPanel();
        m_pGuiManager->RemovePanel(m_pLoadModuleDebugMenu);
        if (g_pAppManager->GetClientExoApp()->GetMiniGame()) {
            g_pAppManager->GetClientExoApp()->SetInputClass(1, 1);
        } else {
            g_pAppManager->GetClientExoApp()->SetInputClass(0, 1);
        }
    }
}

// FUNCTION: SWKOTOR 0x0062DAE0
void CGuiInGame::HidePowersFeatsSkillsDebugMenu()
{
    if (m_bItemCreateAvailable && m_pGuiManager->PanelExists(m_pPowersFeatsSkillsDebugMenu)) {
        m_pGuiManager->PopModalPanel();
        m_pGuiManager->RemovePanel(m_pPowersFeatsSkillsDebugMenu);
        if (g_pAppManager->GetClientExoApp()->GetMiniGame()) {
            g_pAppManager->GetClientExoApp()->SetInputClass(1, 1);
        } else {
            g_pAppManager->GetClientExoApp()->SetInputClass(0, 1);
        }
    }
}

// FUNCTION: SWKOTOR 0x0062DB50
void CGuiInGame::UnlockDialogParticipantOrientations()
{
    CGameObject* pObject = g_pAppManager->m_pClientExoApp->GetGameObject(m_oidDialogOwner);
    if (pObject && pObject->AsSWCCreature()) {
        pObject->AsSWCCreature()->SetLockOrientation(OBJECT_INVALID, 0);
    }
    unsigned long oid = m_oidDialogSpeaker;
    pObject = g_pAppManager->m_pClientExoApp->GetGameObject(oid);
    if (pObject && pObject->AsSWCCreature()) {
        pObject->AsSWCCreature()->SetLockOrientation(OBJECT_INVALID, 0);
    }
}

// FUNCTION: SWKOTOR 0x0062F040
void CGuiInGame::SetStatusSummary(const CStatusSummary& s)
{
    if (m_pStatusPanel) {
        m_pStatusPanel->m_summary = s;
        return;
    }
    if (!m_pPendingStatusSummary) {
        m_pPendingStatusSummary = new CStatusSummary;
    }
    *m_pPendingStatusSummary = s;
}

// FUNCTION: SWKOTOR 0x0062F0E0
int CGuiInGame::CanPlayDialogAnimations(unsigned long oid)
{
    if (oid != OBJECT_INVALID) {
        unsigned long oidServer = g_pAppManager->GetServerExoApp()->ClientToServerObjectID(oid);
        CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidServer);
        if (!pObject) {
            return 0;
        }
        if (pObject->AsSWSCreature()) {
            CSWSCreature* pCreature = pObject->AsSWSCreature();
            if (pCreature->IsDebilitated() || pCreature->GetIsInConversation() || pCreature->IsDead()) {
                return 0;
            }
        }
    }
    return 1;
}
