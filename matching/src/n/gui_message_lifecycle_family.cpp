// Native message-box controls, text, and panel lifetime. Standalone layout.
struct CSWGuiExtent { int left,top,width,height; };
class CSWGuiControl;
class CExoString { public: char* m_sString; unsigned long m_nLength; CExoString(const CExoString&); ~CExoString(); int GetLength() const; };
class CSWGuiManager;
class CSWGuiPanel {
public:
    virtual ~CSWGuiPanel();
    virtual void SetExtent(const CSWGuiExtent& e); virtual void SetActiveControl(CSWGuiControl*,int);
    virtual void v0c(); virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c(); virtual void v30();
    virtual void v34(); virtual void v38(); virtual void HandleInputEvent(int,int);
    virtual void v40(); virtual void v44(); virtual void OnPanelAdded(); virtual void OnPanelRemoved();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c(); virtual void v60();
    virtual void v64(); virtual void UpdateFonts();
    CSWGuiExtent m_extent; int m_bUpdateFonts; CSWGuiManager* m_pManager;
    char pad_0x1c[0x44-0x1c];
    unsigned long m_nPanelFlags;
    char pad_0x48[0x18]; unsigned char m_nOpenSound; char pad_0x61[3];
};
class CSWGuiTextParams {
public:
    CExoString m_sText; char pad_0x8[0x50];
    void SetText(const CExoString& s); void UpdateFont(); void SetStrRef(int n);
};
class CSWGuiText {
public: virtual void v00(); virtual void v04(); virtual int GetIdealHeight();
    char pad_0x4[0x14]; CSWGuiTextParams m_params;
    int GetFontHeight();
};
class CSWGuiLabel {
public:
    virtual ~CSWGuiLabel(); virtual void SetExtent(const CSWGuiExtent&);
    CSWGuiExtent m_extent;
    char pad_0x14[0x30];
    union { unsigned long m_nFlags; struct { unsigned long m_bOther:1; unsigned long m_bEnabled:1; unsigned long m_nOther:30; }; };
    char pad_0x48[0x88]; CSWGuiText m_text;
    void SetEnabled(int b) { m_bEnabled=b; }
};
class CSWGuiButton {
public:
    virtual ~CSWGuiButton();
    CSWGuiExtent m_extent; char pad_0x14[0x30];
    union { unsigned long m_nFlags; struct { unsigned long m_bOther:1; unsigned long m_bEnabled:1; unsigned long m_nOther:30; }; };
    char pad_0x48[4]; int m_bDown;
    char pad_0x50[0x11c]; CSWGuiTextParams m_text;
    void SetEnabled(int b) { m_bEnabled=b; }
};
class CSWGuiListBox {
public: virtual ~CSWGuiListBox(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20();
    virtual void v24(); virtual void v28(); virtual void v2c(); virtual void v30(); virtual void v34();
    virtual void v38(); virtual void HandleInputEvent(int,int);
    char pad_0x4[0x288]; CSWGuiExtent m_innerExtent;
    char pad_0x29c[0x24]; unsigned char m_nPadding; char pad_0x2c1[0x1f];
    void AddControls(CSWGuiControl**,int,int,int,int); void SetActiveControl(CSWGuiControl*,int);
};
class CSWGuiMessageBox : public CSWGuiPanel {
public:
    CSWGuiMessageBox(); virtual ~CSWGuiMessageBox();
    virtual void SetMessageStrRef(unsigned long); virtual void SetMessage(CExoString s);
    virtual void SetOkStrRef(unsigned long); virtual void SetCancelStrRef(unsigned long);
    virtual void HideButtons(); virtual void ResetButtonMessage();
    union { unsigned long m_nFlags; struct { unsigned long m_bAccepted:1,m_bCancel:1,m_bButtons:1,m_bHidden:1,m_bIcon:1,m_nRest:27; }; };
    void (CSWGuiMessageBox::*m_callback)(CSWGuiControl*); CSWGuiMessageBox* m_pCallbackObject; int m_nCallbackArgument;
    CSWGuiLabel m_message,m_icon; CSWGuiButton m_ok,m_cancel; CSWGuiListBox m_list;
    CSWGuiExtent m_originalExtent,m_listExtent; int m_nSavedInputClass,m_bForceOk;
    void UpdateControls(); void FixMessageLabel(); void AddMessageLabel();
    void HandleInputEvent(int,int);
    void OnPanelAdded(); void OnPanelRemoved();
    void OnAccept(CSWGuiControl*); void OnCancel(CSWGuiControl*);
};
class CGuiInGame { public: void EndControllerPauseState(); };
class CClientExoApp {
public:
    int GetInputClass(); int SetInputClass(int,int); CGuiInGame* GetInGameGui();
    void SetUnknown1a8(int,int,int); // 0x005EDE10: verified three integer stores
};
class CAppManager { public: char pad_0x0[4]; CClientExoApp* m_pClient; };
extern CAppManager* g_pAppManager;
class CSWGuiManager {
public:
    CSWGuiPanel* PopModalPanel(); void PlayGuiSound(char);
    void SaveMousePosition(); void RestoreOldMousePosition(); void MoveMouseToPosition(int,int);
    void AddPanel(CSWGuiPanel*,int,int);
};
extern CSWGuiManager* g_pGuiManager;
class CTlkTable { public: void ParseTokens(CExoString*); CExoString GetSimpleString(unsigned long); };
extern CTlkTable* g_pTlkTable;
// FUNCTION: SWKOTOR 0x00625200
void CSWGuiMessageBox::UpdateControls() {
    m_ok.SetEnabled((m_nFlags>>2)&1);
    m_cancel.SetEnabled((m_nFlags&2) && (m_nFlags&4));
    m_icon.SetEnabled((m_nFlags>>4)&1);
}
// FUNCTION: SWKOTOR 0x00625260
void CSWGuiMessageBox::SetOkStrRef(unsigned long n) { m_ok.m_text.SetStrRef(n); }
// FUNCTION: SWKOTOR 0x00625270
void CSWGuiMessageBox::SetCancelStrRef(unsigned long n) { m_cancel.m_text.SetStrRef(n); }
// FUNCTION: SWKOTOR 0x00625280
void CSWGuiMessageBox::HideButtons() { m_bHidden=1; m_ok.SetEnabled(0); m_cancel.SetEnabled(0); }
// FUNCTION: SWKOTOR 0x006252A0
void CSWGuiMessageBox::ResetButtonMessage() { m_ok.SetEnabled(1); m_cancel.SetEnabled(1); m_bHidden=0; m_ok.m_text.SetStrRef(0x62c); m_cancel.m_text.SetStrRef(0x62d); }
// FUNCTION: SWKOTOR 0x006271A0
void CSWGuiMessageBox::SetMessage(CExoString s) { g_pTlkTable->ParseTokens(&s); m_message.m_text.m_params.SetText(s); m_message.m_text.m_params.UpdateFont(); FixMessageLabel(); }
// FUNCTION: SWKOTOR 0x006249D0
void CSWGuiMessageBox::SetMessageStrRef(unsigned long n) { SetMessage(g_pTlkTable->GetSimpleString(n)); }
// FUNCTION: SWKOTOR 0x00625A00
void CSWGuiMessageBox::OnPanelRemoved() { CSWGuiPanel::OnPanelRemoved(); if(m_nSavedInputClass>=0) g_pAppManager->m_pClient->SetInputClass(m_nSavedInputClass,1); g_pGuiManager->RestoreOldMousePosition(); m_bForceOk=0; }
// NONMATCH: bounded source-shape attempts retained; see worker-n.md.
// FUNCTION: SWKOTOR 0x006258F0
void CSWGuiMessageBox::OnPanelAdded() {
    int old=g_pAppManager->m_pClient->GetInputClass(); CSWGuiPanel::OnPanelAdded(); m_nSavedInputClass=-1;
    if(old!=2) { m_nSavedInputClass=old; g_pAppManager->m_pClient->SetInputClass(2,0); }
    SetActiveControl(0,0); SetActiveControl((CSWGuiControl*)&m_ok,0);
    CSWGuiExtent panel=m_extent; CSWGuiExtent target;
    if(m_nFlags&2) { if(m_bForceOk) target=m_ok.m_extent; else target=m_cancel.m_extent; } else target=m_ok.m_extent;
    g_pGuiManager->SaveMousePosition();
    g_pGuiManager->MoveMouseToPosition(target.width/2+panel.left+target.left,target.height/2+target.top+panel.top);
}
// FUNCTION: SWKOTOR 0x00625A80
void CSWGuiMessageBox::OnAccept(CSWGuiControl*) { HandleInputEvent(0x1f6,m_ok.m_bDown); }
// FUNCTION: SWKOTOR 0x00625AA0
void CSWGuiMessageBox::OnCancel(CSWGuiControl*) { HandleInputEvent(0x1f7,m_cancel.m_bDown); }
class CSWGuiControllerLossBox : public CSWGuiMessageBox {
public: void OnPanelAdded(); void OnAccept(CSWGuiControl*);
};
// FUNCTION: SWKOTOR 0x00625A40
void CSWGuiControllerLossBox::OnAccept(CSWGuiControl*) { g_pAppManager->m_pClient->GetInGameGui()->EndControllerPauseState(); }
// FUNCTION: SWKOTOR 0x00627220
void CSWGuiControllerLossBox::OnPanelAdded() { m_nFlags&=~2ul; m_nOpenSound=2; UpdateControls(); SetMessageStrRef(0xbd0e); m_pCallbackObject=this; m_callback=(void(CSWGuiMessageBox::*)(CSWGuiControl*))&CSWGuiControllerLossBox::OnAccept; m_nCallbackArgument=0; CSWGuiMessageBox::OnPanelAdded(); }
// Descriptive name: original class at 0x00627260 has no proven symbol.
class CSWGuiModalWarningBox : public CSWGuiMessageBox {
public: void OnPanelAdded(); void OnAccept(CSWGuiControl*);
};
// FUNCTION: SWKOTOR 0x00625A60
void CSWGuiModalWarningBox::OnAccept(CSWGuiControl*) { g_pAppManager->m_pClient->SetUnknown1a8(1,0,1); }
// FUNCTION: SWKOTOR 0x00627260
void CSWGuiModalWarningBox::OnPanelAdded() { m_nFlags&=~2ul; m_nOpenSound=2; UpdateControls(); SetMessageStrRef(0xa56f); m_pCallbackObject=this; m_callback=(void(CSWGuiMessageBox::*)(CSWGuiControl*))&CSWGuiModalWarningBox::OnAccept; m_nCallbackArgument=0; m_pManager->AddPanel(this,1,1); }

// FUNCTION: SWKOTOR 0x006250F0
void CSWGuiMessageBox::HandleInputEvent(int event,int down) {
    if(down) {
        switch(event) {
        case 0x39: m_list.HandleInputEvent(0x31,1); break;
        case 0x3a: m_list.HandleInputEvent(0x32,1); break;
        case 0x28: case 0x2e: case 0x1f7:
            if(m_nFlags&2) m_bAccepted=0; else m_bAccepted=1;
            goto finish;
        case 0x1f6: m_bAccepted=1;
        finish:
            m_nFlags=(m_nFlags&~0x10ul)|6;
            ResetButtonMessage();
            m_nPanelFlags&=~0x100ul;
            if((m_nPanelFlags&0x600)!=0x400) m_nPanelFlags=(m_nPanelFlags&~0x400ul)|0x200;
            m_pManager->PopModalPanel(); m_pManager->PlayGuiSound(0);
            if(m_pCallbackObject && m_callback) (m_pCallbackObject->*m_callback)((CSWGuiControl*)m_nCallbackArgument);
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(event,down);
}
// FUNCTION: SWKOTOR 0x006252F0
void CSWGuiMessageBox::AddMessageLabel() {
    CSWGuiExtent extent=m_message.m_extent;
    CSWGuiListBox* list=&m_list;
    extent.width=list->m_innerExtent.width-2*list->m_nPadding;
    CSWGuiLabel* label=&m_message;
    label->SetExtent(extent);
    if(m_message.m_text.m_params.m_sText.GetLength()>0) extent.height=m_message.m_text.GetIdealHeight();
    else extent.height=m_message.m_text.GetFontHeight();
    label->SetExtent(extent);
    CSWGuiControl* control=(CSWGuiControl*)label;
    list->AddControls(&control,1,0,0,0); list->SetActiveControl((CSWGuiControl*)label,0);
}
// Descriptive class name for the panel input bridge at 0x00624970, whose original name is not mapped.
class CSWGuiInGameInputPanel : public CSWGuiPanel { public: void HandleInputEvent(int,int); };
// FUNCTION: SWKOTOR 0x00624970
void CSWGuiInGameInputPanel::HandleInputEvent(int event,int down) {
    switch(event) {
    case 0xf3: if(down&1) HandleInputEvent(0x35,1); break;
    case 0xf4: if(down&1) HandleInputEvent(0x36,1); break;
    }
    CSWGuiPanel::HandleInputEvent(event,down);
}
