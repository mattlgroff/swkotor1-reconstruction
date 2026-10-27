// CSWGuiInGameMap map-note label and return-to-Ebon-Hawk prompt
// (swguiingamemap.o). The Mac map attributes ReturnToEbonHawk to 0x006929B0,
// the start the function table gives this range; the callback signature
// (CSWGuiControl*, RET 4) belongs to 0x00692B00, which is missing from the
// table. Other names are descriptive.

class CExoString {
public:
    char* m_sString;
    unsigned long m_nBufferLength;
    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    CExoString operator+(const CExoString& s) const;  // 0x005E5D10
    int GetLength() const;                          // 0x005E5790
};

class CExoLocString {
public:
    int GetString(unsigned long nLanguage, CExoString* pString, unsigned char nGender) const;  // 0x005EA130
    char pad_0x0[0x8];
};

class CSWSWaypoint {
public:
    char pad_0x0[0x230];
    CExoLocString m_sMapNote;                       // 0x230
};

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54();
    virtual CSWSWaypoint* AsSWSWaypoint();          // 0x58
};

class CTlkTable {
public:
    CExoString GetSimpleString(unsigned long nStrRef);  // 0x0041E8F0
};

extern CTlkTable* g_pTlkTable;                      // 0x007A3A08

class CVirtualMachine {
public:
    int RunScript(CExoString* psScript, unsigned long oidSelf, int bOid);  // 0x005D0FC0
};

extern CVirtualMachine* g_pVirtualMachine;          // 0x007A3A00

class CSWGuiTextParams {
public:
    void SetText(const CExoString& sText);          // 0x00415E00
};

class CSWGuiControl {
public:
    char pad_0x0[0x4c];
    int m_bEnabled;                                 // 0x4c (placeholder name)
    char pad_0x50[0x58 - 0x50];
    unsigned long m_nId;                            // 0x58 (placeholder name)
};

class CSWGuiObject {
public:
    virtual ~CSWGuiObject();
};

class CSWGuiPanel : public CSWGuiObject {
public:
    virtual ~CSWGuiPanel();                         // 0x0040CF70
    virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30();
    virtual void Draw(float fDelta);                // 0x34 (0x0040B760)
    virtual void v38();
    virtual void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x00409E60)
    virtual void v40(); virtual void v44(); virtual void v48();
    virtual void OnPanelRemoved();                  // 0x4c (0x0040C170)
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68();
    char pad_0x4[0x18 - 0x4];
    class CSWGuiManager* m_pManager;                // 0x18
    char pad_0x1c[0x64 - 0x1c];
};

class CSWGuiMessageBox : public CSWGuiPanel {
public:
    virtual void SetMessageStrRef(unsigned long nStrRef);  // 0x6c

    void SetAllowCancel(int bCancel);               // 0x00627130
    void SetCallback(CSWGuiObject* pObject, void (CSWGuiObject::*pfnCallback)(CSWGuiControl*),
                     CSWGuiControl* pControl);      // 0x00624A40

    unsigned long m_bAccepted : 1;                  // 0x64 bit 0
};

class CSWGuiManager {
public:
    void AddPanel(CSWGuiPanel* pPanel, int a, int b);  // 0x0040BC70
    void PlayGuiSound(char nSound);                 // 0x0040A140
    void HandleMouseMove(int x, int y);             // 0x0040C1E0 (descriptive)
    int m_nMouseX;                                  // 0x00 (descriptive)
    int m_nMouseY;                                  // 0x04 (descriptive)
};

// Map control (0x248 bytes); the map-note methods are in
// sw_gui_map_hider.cpp / sw_gui_map_hider_notes.cpp.
class CSWGuiMapHider {
public:
    void ClearMapNotes();                           // 0x00692E30
    void SetMapNote(unsigned long oidNote);         // 0x006932A0
    CExoString GetNextMapNote();                    // 0x00692E80
    CExoString GetPrevMapNote();                    // 0x00693090
    char pad_0x0[0x248];
};

class CGuiInGame {
public:
    int HideSWInGameGui(int bHide);                 // 0x0062CBA0
    void ShowPartySelection(const CExoString& sExitScript, int bFromScript, int nForced1, int nForced2);  // 0x0062DD20
    char pad_0x0[0x98];
    CSWGuiMessageBox* m_pMessageBox;                // 0x98
    char pad_0x9c[0xc04 - 0x9c];
    int m_bRunningScript;                           // 0xc04 (placeholder name)
    CSWGuiMessageBox* GetMessageBox() { return m_pMessageBox; }
};

class CClientExoApp {
public:
    CGuiInGame* GetInGameGui();                     // 0x005ED690
    int DismissInGameGUI();                         // 0x005EDC00
    void SetInputClass(int nClass, int bForce);     // 0x005EDA60
    unsigned long GetClientLanguage();              // 0x005ED4C0
    int ShowTutorialWindow(int nWindow, unsigned long oid1, unsigned long oid2, int nArg);  // 0x005EDF40
};

class CSWPartyTable {
public:
    char pad_0x0[0x108];
    unsigned long m_nEbonHawkStrRef;                // 0x108 (placeholder name)
};

class CServerExoApp {
public:
    CSWPartyTable* GetPartyTable();                 // 0x004AEE70
    CGameObject* GetGameObject(unsigned long oid);  // 0x004AE750
};

class CAppManager {
public:
    char pad_0x0[0x4];
    CClientExoApp* m_pClientExoApp;                 // 0x04
    CServerExoApp* m_pServerExoApp;                 // 0x08
    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
    CServerExoApp* GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CSWGuiInGameMap : public CSWGuiPanel {
public:
    void SetMapNoteText(const CExoString& sNote);
    void ReturnToEbonHawk(CSWGuiControl* pControl);
    void OnReturnToEbonHawk(CSWGuiControl* pControl);
    virtual void Draw(float fDelta);                // 0x00692810
    virtual void HandleInputEvent(int nEvent, int bDown);
    virtual void OnPanelRemoved();
    void OnMapNoteClicked(CSWGuiControl* pControl);
    void OnUpArrowPressed(CSWGuiControl* pControl);     // 0x006927B0 (descriptive)
    void OnDownArrowPressed(CSWGuiControl* pControl);   // 0x006927C0 (descriptive)

    char pad_0x64[0x3cc - 0x64];
    CSWGuiTextParams m_cMapNoteText;                // 0x3cc (descriptive)
    char pad_0x3cd[0xe38 - 0x3cd];
    CSWGuiMapHider m_cMapHider;                     // 0xe38
    char pad_0x1080[0x10d4 - 0x1080];
    unsigned long m_nNoReturnStrRef;                // 0x10d4 (descriptive)
    unsigned char m_nMapFlags;                      // 0x10d8 (descriptive)
};

// Arrow button callbacks forward to the panel's input handler; not in the
// function table.
// FUNCTION: SWKOTOR 0x006927B0
void CSWGuiInGameMap::OnUpArrowPressed(CSWGuiControl* pControl)
{
    HandleInputEvent(0x31, 1);
}

// FUNCTION: SWKOTOR 0x006927C0
void CSWGuiInGameMap::OnDownArrowPressed(CSWGuiControl* pControl)
{
    HandleInputEvent(0x32, 1);
}

// FUNCTION: SWKOTOR 0x006929B0
void CSWGuiInGameMap::SetMapNoteText(const CExoString& sNote)
{
    CExoString sText;
    if (sNote.GetLength() == 0) {
        m_cMapNoteText.SetText("");
        return;
    }
    sText = g_pTlkTable->GetSimpleString(349) + ": " + sNote;
    m_cMapNoteText.SetText(sText);
}

// Not in the function table.
// FUNCTION: SWKOTOR 0x00692B00
void CSWGuiInGameMap::ReturnToEbonHawk(CSWGuiControl* pControl)
{
    if (g_pAppManager->GetClientExoApp()->GetInGameGui()->GetMessageBox()->m_bAccepted &&
        g_pAppManager->GetClientExoApp()->DismissInGameGUI()) {
        g_pAppManager->GetClientExoApp()->GetInGameGui()->m_bRunningScript = 1;
        CExoString sScript("k_sup_gohawk");
        g_pVirtualMachine->RunScript(&sScript, 0, 0);
        g_pAppManager->GetClientExoApp()->GetInGameGui()->m_bRunningScript = 0;
    }
}

// NONMATCH (6): register tie only; the else arm loads the app manager into
// ECX (ours EAX). Direct-member, accessor and test-form variants agree.
// FUNCTION: SWKOTOR 0x00692BC0
void CSWGuiInGameMap::OnReturnToEbonHawk(CSWGuiControl* pControl)
{
    CSWGuiMessageBox* pMessageBox;
    if (m_nMapFlags & 2) {
        pMessageBox = g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pMessageBox;
        pMessageBox->SetAllowCancel(1);
        unsigned long nStrRef = g_pAppManager->GetServerExoApp()->GetPartyTable()->m_nEbonHawkStrRef;
        pMessageBox->SetMessageStrRef(nStrRef);
        pMessageBox->SetCallback(this, (void (CSWGuiObject::*)(CSWGuiControl*))&CSWGuiInGameMap::ReturnToEbonHawk, 0);
    }
    else {
        if (m_nNoReturnStrRef == 0) {
            m_nNoReturnStrRef = 38451;
        }
        pMessageBox = g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pMessageBox;
        pMessageBox->SetAllowCancel(0);
        pMessageBox->SetMessageStrRef(m_nNoReturnStrRef);
        pMessageBox->SetCallback(0, 0, 0);
    }
    m_pManager->AddPanel(pMessageBox, 1, 1);
}

// FUNCTION: SWKOTOR 0x00693BA0
void CSWGuiInGameMap::OnPanelRemoved()
{
    m_cMapHider.ClearMapNotes();
    CSWGuiPanel::OnPanelRemoved();
}

// Not in the function table.
// FUNCTION: SWKOTOR 0x00693BC0
void CSWGuiInGameMap::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x29:
            m_pManager->PlayGuiSound(0);
            if (!g_pAppManager->GetClientExoApp()->ShowTutorialWindow(0x27, 0x7f000000, 0x7f000000, 0)) {
                OnReturnToEbonHawk(0);
            }
            break;
        case 0x27:
            m_pManager->PlayGuiSound(0);
            if (m_nMapFlags & 1) {
                g_pAppManager->GetClientExoApp()->GetInGameGui()->ShowPartySelection("", 0, -1, -1);
            }
            else {
                CSWGuiMessageBox* pMessageBox = g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pMessageBox;
                pMessageBox->SetAllowCancel(0);
                pMessageBox->SetMessageStrRef(m_nNoReturnStrRef);
                pMessageBox->SetCallback(0, 0, 0);
                m_pManager->AddPanel(pMessageBox, 1, 1);
            }
            break;
        case 0x28:
        case 0x2d:
        case 0x2e:
        case 0xdf:
            if (g_pAppManager->GetClientExoApp()->GetInGameGui()->HideSWInGameGui(0)) {
                g_pAppManager->GetClientExoApp()->SetInputClass(0, 1);
            }
            break;
        case 0x31:
        case 0x3d:
            m_pManager->PlayGuiSound(1);
            SetMapNoteText(m_cMapHider.GetPrevMapNote());
            break;
        case 0x32:
        case 0x3e:
            m_pManager->PlayGuiSound(1);
            SetMapNoteText(m_cMapHider.GetNextMapNote());
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// NONMATCH (19): register tie only; the original keeps this in EDI and the
// note id in EBX (ours swapped). Member, local and manager-local shapes agree.
// Not in the function table: event 0x27 callback of the map-note buttons
// that CSWGuiMapHider::Draw creates.
// FUNCTION: SWKOTOR 0x00693E70
void CSWGuiInGameMap::OnMapNoteClicked(CSWGuiControl* pControl)
{
    if (!pControl->m_bEnabled) {
        return;
    }
    CExoString sNote;
    unsigned long oid = pControl->m_nId;
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oid);
    if (pObject && pObject->AsSWSWaypoint()) {
        CClientExoApp* pClient = g_pAppManager->GetClientExoApp();
        CSWSWaypoint* pWaypoint = pObject->AsSWSWaypoint();
        pWaypoint->m_sMapNote.GetString(pClient->GetClientLanguage(), &sNote, 0);
    }
    SetMapNoteText(sNote);
    m_cMapHider.SetMapNote(oid);
    CSWGuiInGameMap::Draw(0.001f);
    m_pManager->HandleMouseMove(m_pManager->m_nMouseX, m_pManager->m_nMouseY);
}
