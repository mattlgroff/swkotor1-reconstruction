// CSWGuiInGameMenu callbacks and CSWGuiInGameMessages input/view switching
// (swguiingame.o). Standalone minimal layouts; the panel base is 0x64 bytes.
struct CSWGuiExtent { int left,top,width,height; CSWGuiExtent() {} CSWGuiExtent(int l,int t,int w,int h) { left=l; top=t; width=w; height=h; } };
class CSWGuiControl;
class CSWGuiPanel;
class CSWGuiManager { public: void PlayGuiSound(char nSound); }; // 0x0040A140
class CExoString {
public:
    char* m_sString; unsigned long m_nLength;
    CExoString(); CExoString(const char* s); ~CExoString();
    CExoString& operator=(const CExoString& s);
    CExoString operator+(const CExoString& s) const;
    int GetLength() const; // 0x005E5790
};
class CTlkTable { public: CExoString GetSimpleString(unsigned long nStrRef); };
extern CTlkTable* g_pTlkTable;
struct Vector { float x,y,z; };
template <class T> class CExoArrayList {
public:
    T* m_pData; int m_nNum; int m_nAlloc;
    CExoArrayList() { m_nNum = 0; m_nAlloc = 0; m_pData = 0; }
    ~CExoArrayList() { delete[] m_pData; }
    T& operator[](int i) { return m_pData[i]; }
    void Allocate(int nSize); // 0x005E03C0
    void SetSize(int nSize)
    {
        if (nSize == 0) {
            if (m_pData) { delete[] m_pData; m_pData = 0; m_nAlloc = 0; }
        } else {
            Allocate(nSize);
        }
        m_nNum = nSize;
    }
};
class CSWGuiBorderParams {
public:
    char pad_0x0[0x10]; Vector m_vColor;
    unsigned long m_nStyle:2,m_bPulse:1,m_b3:1,m_b4:1,m_nRest:27;
    char pad_0x20[0x34];
    void SetFlags(int b2,int b3,int b4); // 0x00414D00
    int GetCornerSize(); // 0x00414CD0
};
class CSWGuiTextParams {
public:
    char pad_0x0[0x58];
    void SetText(const CExoString& s); void SetStrRef(int n); void UpdateFont();
    void SetColor(const Vector& vColor); // 0x00414E10
};
class CSWGuiText { public: virtual void v00(); virtual void v04(); virtual int GetIdealHeight(); char pad_0x4[0x14]; CSWGuiTextParams m_params; };
class CSWGuiControl {
public:
    virtual ~CSWGuiControl();
    virtual void SetExtent(const CSWGuiExtent& extent);
    virtual void v08(); virtual void v0c(); virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c(); virtual void v30();
    virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void SetSelected(int bSelected);
    virtual void v44(); virtual void v48(); virtual void v4c(); virtual void v50(); virtual void v54();
    virtual void v58(); virtual void v5c(); virtual void v60(); virtual void v64(); virtual void v68();
    virtual void v6c(); virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7c();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8c(); virtual void v90();
    virtual void v94(); virtual void v98(); virtual void v9c();
    virtual void CopyFrom(const CSWGuiExtent& extent,CSWGuiControl* pPrototype);
    virtual void SetHilightable(int bHilight);
    CSWGuiExtent m_extent; char pad_0x14[0x10];
    unsigned long m_nHintStrRef; char pad_0x28[8]; int m_nHintId; char pad_0x34[0x10]; // descriptive
    union { unsigned long m_nFlags; struct { unsigned long m_bOther:1; unsigned long m_bEnabled:1; unsigned long m_bSelectable:1; unsigned long m_nOther:29; }; };
    char pad_0x48[4]; int m_bDown;
    void SetEnabled(int b) { m_bEnabled=b; }
    void ClearSelectable() { m_nFlags &= ~4ul; } // descriptive inline
    void SetHint(unsigned long nStrRef,int nId) { m_nHintStrRef=nStrRef; m_nHintId=nId; }
    void AddEvent(int nEvent,CSWGuiPanel* pObject,void (CSWGuiPanel::*pfnCallback)(CSWGuiControl*)); // 0x0041AB20
};
class CSWGuiLabel : public CSWGuiControl { public: CSWGuiLabel(); virtual ~CSWGuiLabel(); char pad_0x50[0x98]; CSWGuiTextParams m_text; };
class CSWGuiLabelHilight : public CSWGuiControl {
public:
    CSWGuiLabelHilight(); virtual ~CSWGuiLabelHilight(); // 0x0041B5C0 / 0x0041B610
    char pad_0x50[0x20]; CSWGuiBorderParams m_border;
    char pad_0xc4[0xc]; CSWGuiText m_text;
    char pad_0x140[0x14]; CSWGuiBorderParams m_hilightBorder;
    char pad_0x1a8[0xc];
    int GetIdealHeight() { return m_text.GetIdealHeight() + 2 * m_hilightBorder.GetCornerSize(); }
};
class CSWGuiButton : public CSWGuiControl { public: CSWGuiButton(); virtual ~CSWGuiButton(); char pad_0x50[0x11c]; CSWGuiTextParams m_text; };
class CSWGuiListBox : public CSWGuiControl {
public:
    CSWGuiListBox(); virtual ~CSWGuiListBox(); // 0x0041BE40 / 0x0041B060
    void ClearControls(); // 0x00419A90
    char pad_0x50[0x23c]; CSWGuiExtent m_innerExtent;
    char pad_0x29c[0x20]; unsigned long m_nListFlags; unsigned char m_nPadding; char pad_0x2c1[0xb];
    CSWGuiLabelHilight* m_pPrototype; char pad_0x2d0[0x10];
    void AddControls(const CExoArrayList<CSWGuiControl*>& lControls,int n1,int n2,int n3); // 0x0041C1D0
    void SetSelectedControl(int nIndex,int bArg); // 0x0041C040
};
class CResRef { public: char m_resRef[16]; CResRef(const char* s); };
class CSWGuiPanel {
public:
    CSWGuiPanel(CSWGuiManager* pManager); // 0x0040B570
    virtual ~CSWGuiPanel();
    virtual void v04();
    virtual void SetActiveControl(CSWGuiControl* pControl,int bArg);
    virtual void v0c(); virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c(); virtual void v30();
    virtual void v34(); virtual void v38(); virtual void HandleInputEvent(int nEvent,int bDown);
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c(); virtual void v50();
    virtual void OnExitClicked(CSWGuiControl* pControl);   // +0x54, descriptive
    virtual void OnSwitchClicked(CSWGuiControl* pControl); // +0x58, descriptive
    void StartLoadFromLayout(const CResRef& cLayout); // 0x0040A680
    void InitControl(CSWGuiControl* pControl,const CExoString& sName,int bAdd); // 0x0040B930
    void UnloadResource(); // 0x0040B8F0
    CSWGuiExtent m_extent; int m_n14; CSWGuiManager* m_pManager; int m_n1c;
    CExoArrayList<CSWGuiControl*> m_controls;
    char pad_0x2c[0x44-0x2c]; unsigned long m_nFlags; char pad_0x48[0x64-0x48];
};
class CItemRepository { public: char pad_0x0[4]; unsigned long m_nItems; };
class CSWPartyTable { public: CItemRepository* GetPartyItems(); }; // 0x00563340
class CServerExoApp { public: CSWPartyTable* GetPartyTable(); }; // 0x004AEE70
class CSWCJournal { public: int HasNewEntries(); }; // 0x00676470
class CGuiInGame {
public:
    char pad_0x0[0xbc8]; unsigned char m_bDialogMessages;
    void PrevSWInGameGui(); void NextSWInGameGui(); void SwitchToSWInGameGui(int nPanel);
    int HideSWInGameGui(int bArg);
    void AppendToMsgBuffer(const CExoString& s,unsigned long nColor,unsigned char nType);
    int UpdateMessageGUI();
};
class CClientExoApp {
public:
    CGuiInGame* GetInGameGui(); CSWCJournal* GetQuestJournal(); int SetInputClass(int nClass,int bArg);
};
class CAppManager { public: char pad_0x0[4]; CClientExoApp* m_pClient; CServerExoApp* m_pServer; };
extern CAppManager* g_pAppManager;

class CSWGuiInGameMenu : public CSWGuiPanel {
public:
    CSWGuiInGameMenu(CSWGuiManager* pManager);
    CSWGuiLabelHilight m_equipment,m_inventory,m_character,m_map,m_abilities,m_journal,m_options,m_messages;
    CSWGuiButton m_equipmentButton,m_inventoryButton,m_characterButton,m_mapButton,m_abilitiesButton,m_journalButton,m_optionsButton,m_messagesButton;
    void SetActiveControlID(int nIndex,int bArg);
    void Update(float fDelta);
    void OnShoulderLeft(CSWGuiControl* pControl); void OnShoulderRight(CSWGuiControl* pControl);
    void OnEquipButtonPressed(CSWGuiControl*); void OnInvButtonPressed(CSWGuiControl*);
    void OnCharButtonPressed(CSWGuiControl*); void OnMapButtonPressed(CSWGuiControl*);
    void OnAbiButtonPressed(CSWGuiControl*); void OnJouButtonPressed(CSWGuiControl*);
    void OnOptButtonPressed(CSWGuiControl*); void OnMsgButtonPressed(CSWGuiControl*);
};

typedef void (CSWGuiPanel::*GuiCallback)(CSWGuiControl*);

// FUNCTION: SWKOTOR 0x00627980
CSWGuiInGameMenu::CSWGuiInGameMenu(CSWGuiManager* pManager) : CSWGuiPanel(pManager)
{
    {
        CResRef cLayout("top");
        StartLoadFromLayout(cLayout);
    }
    { CExoString sName("LBLH_EQU"); InitControl(&m_equipment,sName,1); }
    { CExoString sName("LBLH_INV"); InitControl(&m_inventory,sName,1); }
    { CExoString sName("LBLH_CHA"); InitControl(&m_character,sName,1); }
    { CExoString sName("LBLH_MAP"); InitControl(&m_map,sName,1); }
    { CExoString sName("LBLH_ABI"); InitControl(&m_abilities,sName,1); }
    { CExoString sName("LBLH_JOU"); InitControl(&m_journal,sName,1); }
    { CExoString sName("LBLH_OPT"); InitControl(&m_options,sName,1); }
    { CExoString sName("LBLH_MSG"); InitControl(&m_messages,sName,1); }
    { CExoString sName("BTN_EQU"); InitControl(&m_equipmentButton,sName,1); }
    { CExoString sName("BTN_INV"); InitControl(&m_inventoryButton,sName,1); }
    { CExoString sName("BTN_CHAR"); InitControl(&m_characterButton,sName,1); }
    { CExoString sName("BTN_MAP"); InitControl(&m_mapButton,sName,1); }
    { CExoString sName("BTN_ABI"); InitControl(&m_abilitiesButton,sName,1); }
    { CExoString sName("BTN_JOU"); InitControl(&m_journalButton,sName,1); }
    { CExoString sName("BTN_OPT"); InitControl(&m_optionsButton,sName,1); }
    { CExoString sName("BTN_MSG"); InitControl(&m_messagesButton,sName,1); }
    UnloadResource();
    m_equipment.AddEvent(0x35,this,(GuiCallback)&CSWGuiInGameMenu::OnShoulderLeft);
    m_inventory.AddEvent(0x35,this,(GuiCallback)&CSWGuiInGameMenu::OnShoulderLeft);
    m_character.AddEvent(0x35,this,(GuiCallback)&CSWGuiInGameMenu::OnShoulderLeft);
    m_map.AddEvent(0x35,this,(GuiCallback)&CSWGuiInGameMenu::OnShoulderLeft);
    m_abilities.AddEvent(0x35,this,(GuiCallback)&CSWGuiInGameMenu::OnShoulderLeft);
    m_journal.AddEvent(0x35,this,(GuiCallback)&CSWGuiInGameMenu::OnShoulderLeft);
    m_options.AddEvent(0x35,this,(GuiCallback)&CSWGuiInGameMenu::OnShoulderLeft);
    m_messages.AddEvent(0x35,this,(GuiCallback)&CSWGuiInGameMenu::OnShoulderLeft);
    m_equipment.AddEvent(0x36,this,(GuiCallback)&CSWGuiInGameMenu::OnShoulderRight);
    m_inventory.AddEvent(0x36,this,(GuiCallback)&CSWGuiInGameMenu::OnShoulderRight);
    m_character.AddEvent(0x36,this,(GuiCallback)&CSWGuiInGameMenu::OnShoulderRight);
    m_map.AddEvent(0x36,this,(GuiCallback)&CSWGuiInGameMenu::OnShoulderRight);
    m_abilities.AddEvent(0x36,this,(GuiCallback)&CSWGuiInGameMenu::OnShoulderRight);
    m_journal.AddEvent(0x36,this,(GuiCallback)&CSWGuiInGameMenu::OnShoulderRight);
    m_options.AddEvent(0x36,this,(GuiCallback)&CSWGuiInGameMenu::OnShoulderRight);
    m_messages.AddEvent(0x36,this,(GuiCallback)&CSWGuiInGameMenu::OnShoulderRight);
    CSWGuiButton* pEquipButton = &m_equipmentButton;
    pEquipButton->AddEvent(0x27,this,(GuiCallback)&CSWGuiInGameMenu::OnEquipButtonPressed);
    CSWGuiButton* pInvButton = &m_inventoryButton;
    pInvButton->AddEvent(0x27,this,(GuiCallback)&CSWGuiInGameMenu::OnInvButtonPressed);
    m_characterButton.AddEvent(0x27,this,(GuiCallback)&CSWGuiInGameMenu::OnCharButtonPressed);
    m_mapButton.AddEvent(0x27,this,(GuiCallback)&CSWGuiInGameMenu::OnMapButtonPressed);
    m_abilitiesButton.AddEvent(0x27,this,(GuiCallback)&CSWGuiInGameMenu::OnAbiButtonPressed);
    m_journalButton.AddEvent(0x27,this,(GuiCallback)&CSWGuiInGameMenu::OnJouButtonPressed);
    m_optionsButton.AddEvent(0x27,this,(GuiCallback)&CSWGuiInGameMenu::OnOptButtonPressed);
    m_messagesButton.AddEvent(0x27,this,(GuiCallback)&CSWGuiInGameMenu::OnMsgButtonPressed);
    pEquipButton->ClearSelectable();
    pInvButton->ClearSelectable();
    m_characterButton.ClearSelectable();
    m_mapButton.ClearSelectable();
    m_abilitiesButton.ClearSelectable();
    m_journalButton.ClearSelectable();
    m_optionsButton.ClearSelectable();
    m_messagesButton.ClearSelectable();
    CSWGuiPanel::SetActiveControl(pEquipButton,0);
    m_nFlags |= 0x60;
    pEquipButton->SetHint(0xbc5b,0xd1);
    pInvButton->SetHint(0xbc5c,0xd2);
    m_characterButton.SetHint(0xbc61,0xd3);
    m_mapButton.SetHint(0xbc5d,0xd7);
    m_abilitiesButton.SetHint(0xbc60,0xd4);
    m_journalButton.SetHint(0xbc5a,0xd6);
    m_optionsButton.SetHint(0xbc5e,0xd8);
    m_messagesButton.SetHint(0xbc5f,0xd5);
}

// FUNCTION: SWKOTOR 0x00624BD0
void CSWGuiInGameMenu::SetActiveControlID(int nIndex,int bArg)
{
    if (nIndex >= 0 && nIndex < m_controls.m_nNum) SetActiveControl(m_controls.m_pData[nIndex],bArg);
}
// FUNCTION: SWKOTOR 0x00624C00
void CSWGuiInGameMenu::OnShoulderLeft(CSWGuiControl* pControl)
{
    if (pControl->m_bDown) g_pAppManager->m_pClient->GetInGameGui()->PrevSWInGameGui();
}
// FUNCTION: SWKOTOR 0x00624C30
void CSWGuiInGameMenu::OnShoulderRight(CSWGuiControl* pControl)
{
    if (pControl->m_bDown) g_pAppManager->m_pClient->GetInGameGui()->NextSWInGameGui();
}
// FUNCTION: SWKOTOR 0x00624C60
void CSWGuiInGameMenu::Update(float)
{
    if (g_pAppManager->m_pServer->GetPartyTable()->GetPartyItems()->m_nItems > 0) {
        if (!m_inventory.m_border.m_bPulse) m_inventory.m_border.SetFlags(1,0,1);
    } else {
        m_inventory.m_border.SetFlags(0,0,0);
    }
    if (g_pAppManager->m_pClient->GetQuestJournal()->HasNewEntries()) {
        if (!m_journal.m_border.m_bPulse) m_journal.m_border.SetFlags(1,0,1);
    } else {
        m_journal.m_border.SetFlags(0,0,0);
    }
}
// FUNCTION: SWKOTOR 0x00624CF0
void CSWGuiInGameMenu::OnEquipButtonPressed(CSWGuiControl*) { g_pAppManager->m_pClient->GetInGameGui()->SwitchToSWInGameGui(0); }
// FUNCTION: SWKOTOR 0x00624D10
void CSWGuiInGameMenu::OnInvButtonPressed(CSWGuiControl*) { g_pAppManager->m_pClient->GetInGameGui()->SwitchToSWInGameGui(1); }
// FUNCTION: SWKOTOR 0x00624D30
void CSWGuiInGameMenu::OnCharButtonPressed(CSWGuiControl*) { g_pAppManager->m_pClient->GetInGameGui()->SwitchToSWInGameGui(2); }
// FUNCTION: SWKOTOR 0x00624D50
void CSWGuiInGameMenu::OnMapButtonPressed(CSWGuiControl*) { g_pAppManager->m_pClient->GetInGameGui()->SwitchToSWInGameGui(6); }
// FUNCTION: SWKOTOR 0x00624D70
void CSWGuiInGameMenu::OnAbiButtonPressed(CSWGuiControl*) { g_pAppManager->m_pClient->GetInGameGui()->SwitchToSWInGameGui(3); }
// FUNCTION: SWKOTOR 0x00624D90
void CSWGuiInGameMenu::OnJouButtonPressed(CSWGuiControl*) { g_pAppManager->m_pClient->GetInGameGui()->SwitchToSWInGameGui(5); }
// FUNCTION: SWKOTOR 0x00624DB0
void CSWGuiInGameMenu::OnOptButtonPressed(CSWGuiControl*) { g_pAppManager->m_pClient->GetInGameGui()->SwitchToSWInGameGui(7); }
// FUNCTION: SWKOTOR 0x00624DD0
void CSWGuiInGameMenu::OnMsgButtonPressed(CSWGuiControl*) { g_pAppManager->m_pClient->GetInGameGui()->SwitchToSWInGameGui(4); }

struct CFeedbackMessage { CExoString m_sText; int m_n08; unsigned char m_nType; };
struct CDialogMessage { CExoString m_sText; CExoString m_sSpeaker; };
extern Vector g_vFeedbackColor;     // 0x007A23B4, descriptive
extern Vector g_vFeedbackAltColor;  // 0x007A23D8, descriptive
extern Vector g_vHilightColor;      // 0x007A23C0, descriptive
class CSWGuiInGameMessages : public CSWGuiPanel {
public:
    CSWGuiInGameMessages(CSWGuiManager* pManager);
    virtual ~CSWGuiInGameMessages();
    CSWGuiListBox m_feedbackList,m_dialogList;
    CSWGuiLabel m_title;
    CSWGuiLabelHilight* m_pFeedbackItems; CSWGuiLabelHilight* m_pDialogItems;
    CSWGuiButton m_switchButton,m_exitButton;
    void ShowFeedback(); void ShowDialog();
    void AddFeedbackMessages(const CFeedbackMessage* pMessages,unsigned long nCount);
    void AddDialogMessages(const CDialogMessage* pMessages,unsigned long nCount);
    void HandleInputEvent(int nEvent,int bDown);
};

// FUNCTION: SWKOTOR 0x00626400
CSWGuiInGameMessages::CSWGuiInGameMessages(CSWGuiManager* pManager) : CSWGuiPanel(pManager)
{
    CSWGuiExtent extent;
    extent.left = extent.top = extent.width = extent.height = 0;
    {
        CResRef cLayout("messages");
        StartLoadFromLayout(cLayout);
    }
    {
        CExoString sName("LB_MESSAGES");
        InitControl(&m_feedbackList,sName,1);
    }
    {
        CExoString sName("LBL_MESSAGES");
        InitControl(&m_title,sName,1);
    }
    {
        CExoString sName("LB_DIALOG");
        InitControl(&m_dialogList,sName,1);
    }
    {
        CExoString sName("BTN_SHOW");
        InitControl(&m_switchButton,sName,1);
    }
    {
        CExoString sName("BTN_EXIT");
        InitControl(&m_exitButton,sName,1);
    }
    UnloadResource();
    CSWGuiLabelHilight* pFeedbackPrototype = m_feedbackList.m_pPrototype;
    pFeedbackPrototype->m_hilightBorder.m_vColor = g_vHilightColor;
    pFeedbackPrototype->m_hilightBorder.SetFlags(1,1,0);
    m_pFeedbackItems = new CSWGuiLabelHilight[0x40];
    for (unsigned int i = 0; i < 0x40; ++i) m_pFeedbackItems[i].CopyFrom(extent,pFeedbackPrototype);
    CSWGuiLabelHilight* pDialogPrototype = m_dialogList.m_pPrototype;
    pDialogPrototype->m_hilightBorder.m_vColor = g_vHilightColor;
    pDialogPrototype->m_hilightBorder.SetFlags(1,1,0);
    m_pDialogItems = new CSWGuiLabelHilight[0x40];
    for (unsigned int j = 0; j < 0x40; ++j) m_pDialogItems[j].CopyFrom(extent,pDialogPrototype);
    CSWGuiListBox* pList = &m_feedbackList;
    pList->m_nListFlags |= 0x400;
    CSWGuiButton* pButton = &m_switchButton;
    pButton->AddEvent(0x27,this,&CSWGuiPanel::OnSwitchClicked);
    pButton->m_bSelectable = 0;
    pButton = &m_exitButton;
    pButton->AddEvent(0x27,this,&CSWGuiPanel::OnExitClicked);
    pButton->m_bSelectable = 0;
    pList->SetHilightable(1);
    m_dialogList.SetHilightable(1);
    CSWGuiPanel::SetActiveControl(pList,0);
}

// FUNCTION: SWKOTOR 0x00624BB0 SYMBOL: ??_9@$BFE@AE
// vcall thunk for &CSWGuiPanel::OnExitClicked (+0x54), emitted by the ctor.
// FUNCTION: SWKOTOR 0x00624BC0 SYMBOL: ??_9@$BFI@AE
// vcall thunk for &CSWGuiPanel::OnSwitchClicked (+0x58), emitted by the ctor.

// FUNCTION: SWKOTOR 0x006267C0 SYMBOL: ??_ECSWGuiLabelHilight@@UAEPAXI@Z
// Vector deleting destructor (not in functions.csv), emitted for delete[].

// FUNCTION: SWKOTOR 0x00626820
CSWGuiInGameMessages::~CSWGuiInGameMessages()
{
    m_dialogList.ClearControls();
    m_feedbackList.ClearControls();
    delete[] m_pDialogItems;
    delete[] m_pFeedbackItems;
}

// FUNCTION: SWKOTOR 0x00628240 SYMBOL: ??_GCSWGuiInGameMessages@@UAEPAXI@Z
// Scalar deleting destructor, emitted with the vtable.

// FUNCTION: SWKOTOR 0x00624DF0
void CSWGuiInGameMessages::ShowFeedback()
{
    m_feedbackList.SetEnabled(0);
    m_dialogList.SetEnabled(1);
    m_switchButton.m_text.SetStrRef(0xa49e);
    SetActiveControl(&m_dialogList,0);
    g_pAppManager->m_pClient->GetInGameGui()->m_bDialogMessages = 0;
    CExoString sTitle;
    {
        CExoString sSeparator(" - ");
        sTitle = g_pTlkTable->GetSimpleString(0x61b) + sSeparator + g_pTlkTable->GetSimpleString(0x173);
    }
    m_title.m_text.SetText(sTitle);
}
// FUNCTION: SWKOTOR 0x00624F70
void CSWGuiInGameMessages::ShowDialog()
{
    m_feedbackList.SetEnabled(1);
    m_dialogList.SetEnabled(0);
    m_switchButton.m_text.SetStrRef(0xa49f);
    SetActiveControl(&m_feedbackList,0);
    g_pAppManager->m_pClient->GetInGameGui()->m_bDialogMessages = 1;
    CExoString sTitle;
    {
        CExoString sSeparator(" - ");
        sTitle = g_pTlkTable->GetSimpleString(0x61b) + sSeparator + g_pTlkTable->GetSimpleString(0xa4b7);
    }
    m_title.m_text.SetText(sTitle);
}

// Descriptive names: AddFeedbackMessages fills the 0x64 list, AddDialogMessages
// the 0x344 list. (The established ShowFeedback/ShowDialog names are kept.)
// NONMATCH: item-pointer register tie in the ideal-height step (mov ecx/lea eax
// versus mov eax/add eax); member, pointer and split-call shapes tried.
// FUNCTION: SWKOTOR 0x00626920
void CSWGuiInGameMessages::AddFeedbackMessages(const CFeedbackMessage* pMessages,unsigned long nCount)
{
    CExoArrayList<CSWGuiControl*> lControls;
    CSWGuiListBox* pList = &m_feedbackList;
    CSWGuiExtent extent(0,0,pList->m_innerExtent.width - 2 * pList->m_nPadding,10);
    lControls.SetSize(nCount);
    for (unsigned long i = 0; i < nCount; ++i) {
        m_pFeedbackItems[i].m_text.m_params.SetText(pMessages[i].m_sText);
        m_pFeedbackItems[i].m_text.m_params.UpdateFont();
        Vector vColor;
        switch (pMessages[i].m_nType) {
        case 1: vColor = g_vFeedbackAltColor; break;
        default: vColor = g_vFeedbackColor; break;
        }
        m_pFeedbackItems[i].m_text.m_params.SetColor(vColor);
        m_pFeedbackItems[i].SetExtent(extent);
        extent.height = m_pFeedbackItems[i].GetIdealHeight();
        m_pFeedbackItems[i].SetExtent(extent);
        m_pFeedbackItems[i].SetSelected(0);
        lControls[i] = &m_pFeedbackItems[i];
    }
    if (nCount) {
        pList->AddControls(lControls,0,0,1);
        pList->SetSelectedControl(nCount - 1,0);
    }
}

// NONMATCH: item-pointer register tie in the ideal-height step (mov ecx/lea eax
// versus mov eax/add eax); member, pointer and split-call shapes tried.
// FUNCTION: SWKOTOR 0x00626B10
void CSWGuiInGameMessages::AddDialogMessages(const CDialogMessage* pMessages,unsigned long nCount)
{
    CExoArrayList<CSWGuiControl*> lControls;
    CSWGuiListBox* pList = &m_dialogList;
    CSWGuiExtent extent(0,0,pList->m_innerExtent.width - 2 * pList->m_nPadding,10);
    CExoString sSpeaker;
    lControls.SetSize(nCount);
    for (unsigned long i = 0; i < nCount; ++i) {
        sSpeaker = pMessages[i].m_sSpeaker;
        if (sSpeaker.GetLength() > 0) {
            CExoString sSeparator(": ");
            m_pDialogItems[i].m_text.m_params.SetText(sSpeaker + sSeparator + pMessages[i].m_sText);
        } else
            m_pDialogItems[i].m_text.m_params.SetText(pMessages[i].m_sText);
        m_pDialogItems[i].m_text.m_params.UpdateFont();
        m_pDialogItems[i].SetExtent(extent);
        extent.height = m_pDialogItems[i].GetIdealHeight();
        m_pDialogItems[i].SetExtent(extent);
        m_pDialogItems[i].SetSelected(0);
        lControls[i] = &m_pDialogItems[i];
    }
    if (nCount) {
        pList->AddControls(lControls,0,0,1);
        pList->SetSelectedControl(nCount - 1,0);
    }
}

extern int g_bMessageCheatShown; // 0x008338E8, descriptive

// FUNCTION: SWKOTOR 0x00628260
void CSWGuiInGameMessages::HandleInputEvent(int nEvent,int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x29:
            m_pManager->PlayGuiSound(0);
            if (m_feedbackList.m_bEnabled) ShowFeedback(); else ShowDialog();
            break;
        case 0x28: case 0x2d: case 0x2e: case 0xdf:
            if (g_pAppManager->m_pClient->GetInGameGui()->HideSWInGameGui(0))
                g_pAppManager->m_pClient->SetInputClass(0,1);
            break;
        }
        static int s_nStep;
        int aSequence[4] = { 0x29,0x2f,0x27,0x29 };
        if (nEvent != aSequence[s_nStep]) s_nStep = 0;
        if (nEvent == aSequence[s_nStep] && ++s_nStep == 4) {
            char szText[] = "Pvpfh!kw,!Ekexkh!";
            s_nStep = 0;
            for (unsigned int i = 0; i < 0x11; ++i) szText[i] -= (char)(i & 3);
            {
                CExoString sText(szText);
                CClientExoApp* pClient = g_pAppManager->m_pClient;
                pClient->GetInGameGui()->AppendToMsgBuffer(sText,0x80,0);
            }
            g_bMessageCheatShown = 1;
            g_pAppManager->m_pClient->GetInGameGui()->UpdateMessageGUI();
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent,bDown);
}
