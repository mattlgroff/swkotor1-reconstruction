// CSWGuiMessageBox constructor (swguiingame.o, vtable 0x0074FDB0, 0x984
// bytes). Standalone layout matching gui_message_lifecycle_family.cpp.
struct CSWGuiExtent {
    int left,top,width,height;
    CSWGuiExtent() { left = top = width = height = 0; }
};
class CSWGuiControl;
class CSWGuiPanel;
class CSWGuiManager;
class CExoString { public: char* m_sString; unsigned long m_nLength; CExoString(const char* s); ~CExoString(); };
class CResRef { public: char m_resRef[16]; CResRef(const char* s); };
class CSWGuiText {
public:
    virtual void v00(); virtual void v04(); virtual int GetIdealHeight();
    char pad_0x4[0x6c];
    int GetFontHeight(); // 0x00414EE0
};
class CSWGuiControl {
public:
    virtual ~CSWGuiControl();
    virtual void SetExtent(const CSWGuiExtent& extent);
    CSWGuiExtent m_extent; char pad_0x14[0x30];
    union { unsigned long m_nFlags; struct { unsigned long m_bOther:1; unsigned long m_bEnabled:1; unsigned long m_nOther:30; }; };
    char pad_0x48[8];
    void AddEvent(int nEvent,CSWGuiPanel* pObject,void (CSWGuiPanel::*pfnCallback)(CSWGuiControl*)); // 0x0041AB20
    void SetEnabled(int b) { m_bEnabled=b; }
};
class CSWGuiLabel : public CSWGuiControl {
public:
    CSWGuiLabel(); virtual ~CSWGuiLabel();
    virtual void v08(); virtual void v0c(); virtual void v10(); virtual void v14(); virtual void v18();
    virtual void v1c(); virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c(); virtual void v40();
    virtual void v44(); virtual void v48(); virtual void v4c(); virtual void v50(); virtual void v54();
    virtual void v58(); virtual void v5c(); virtual void v60(); virtual void v64(); virtual void v68();
    virtual void v6c(); virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7c();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8c(); virtual void v90();
    virtual void v94();
    virtual void CopyStyle(const CSWGuiExtent& extent,CSWGuiLabel* pFrom,float fScale); // +0x98
    char pad_0x50[0x80]; CSWGuiText m_text;
};
class CSWGuiButton : public CSWGuiControl {
public:
    CSWGuiButton(); virtual ~CSWGuiButton();
    char pad_0x50[0x34]; int m_nTextMargin; char pad_0x88[0xcc]; CSWGuiText m_text;
};
class CSWGuiListBox : public CSWGuiControl {
public:
    CSWGuiListBox(); virtual ~CSWGuiListBox();
    char pad_0x50[0xb4]; CSWGuiControl m_scrollBar; char pad_0x154[0x138];
    CSWGuiExtent m_innerExtent; char pad_0x29c[4]; int m_nControls; char pad_0x2a4[0x10];
    int m_nItemSize; char pad_0x2b8[4]; unsigned long m_nListFlags;
    unsigned char m_nPadding; unsigned char m_bWrapText; char pad_0x2c2[0xa];
    CSWGuiLabel* m_pPrototype; char pad_0x2d0[0x10];
    void UpdateScrollBar(int nWidth,int bSetExtent); // 0x004181F0
    int IsOversized() { return m_nPadding + m_nItemSize > m_innerExtent.height && m_nControls > 0 && !(m_nListFlags & 0x100); }
};
class CSWGuiPanel {
public:
    CSWGuiPanel(CSWGuiManager* pManager); // 0x0040B570
    virtual ~CSWGuiPanel();
    virtual void SetExtent(const CSWGuiExtent& extent);
    CSWGuiExtent m_extent; int m_n14; CSWGuiManager* m_pManager;
    char pad_0x1c[0x60-0x1c]; unsigned char m_nOpenSound; char pad_0x61[3];
    void StartLoadFromLayout(const CResRef& cLayout); // 0x0040A680
    void InitControl(CSWGuiControl* pControl,const CExoString& sName,int bAdd); // 0x0040B930
    void UnloadResource(); // 0x0040B8F0
    void AddControl(CSWGuiControl* pControl); // 0x0040B9A0
    void CenterPanel(); // 0x0040A600
};
class CSWGuiMessageBox : public CSWGuiPanel {
public:
    CSWGuiMessageBox(CSWGuiManager* pManager);
    virtual ~CSWGuiMessageBox();
    union { unsigned long m_nFlags; struct { unsigned long m_bAccepted:1,m_bCancel:1,m_bButtons:1,m_bHidden:1,m_bIcon:1,m_nRest:27; }; };
    void (CSWGuiMessageBox::*m_callback)(CSWGuiControl*); CSWGuiMessageBox* m_pCallbackObject; int m_nCallbackArgument;
    CSWGuiLabel m_message,m_icon; CSWGuiButton m_ok,m_cancel; CSWGuiListBox m_list;
    CSWGuiExtent m_originalExtent,m_listExtent; int m_nSavedInputClass,m_bForceOk;
    void OnAccept(CSWGuiControl*); void OnCancel(CSWGuiControl*);
    void FixMessageLabel(); void AddMessageLabel();
};

// FUNCTION: SWKOTOR 0x00626DF0
CSWGuiMessageBox::CSWGuiMessageBox(CSWGuiManager* pManager) : CSWGuiPanel(pManager)
{
    m_bCancel = 1;
    m_bButtons = 1;
    m_bHidden = 0;
    m_bIcon = 0;
    {
        CResRef cLayout("confirm");
        StartLoadFromLayout(cLayout);
    }
    { CExoString sName("BTN_OK"); InitControl(&m_ok,sName,1); }
    { CExoString sName("BTN_CANCEL"); InitControl(&m_cancel,sName,1); }
    { CExoString sName("LB_MESSAGE"); InitControl(&m_list,sName,1); }
    UnloadResource();
    m_ok.AddEvent(0x27,this,(void (CSWGuiPanel::*)(CSWGuiControl*))&CSWGuiMessageBox::OnAccept);
    m_cancel.AddEvent(0x27,this,(void (CSWGuiPanel::*)(CSWGuiControl*))&CSWGuiMessageBox::OnCancel);
    m_list.m_bWrapText = 1;
    {
        CSWGuiExtent extent;
        extent.left = 0;
        extent.top = 10;
        extent.width = 0x20;
        extent.height = 0x20;
        m_icon.SetExtent(extent);
    }
    m_icon.SetEnabled(0);
    AddControl(&m_icon);
    if (m_list.m_pPrototype) m_message.CopyStyle(m_list.m_pPrototype->m_extent,m_list.m_pPrototype,1.0f);
    CenterPanel();
    m_originalExtent = m_extent;
    m_listExtent = m_list.m_extent;
    m_bAccepted = 0;
    m_callback = 0;
    m_pCallbackObject = 0;
    m_nCallbackArgument = 0;
    m_nSavedInputClass = 0;
    m_bForceOk = 0;
    m_nOpenSound = 0xf;
}

// FUNCTION: SWKOTOR 0x006253A0
void CSWGuiMessageBox::FixMessageLabel()
{
    CSWGuiExtent panel = m_originalExtent;
    CSWGuiExtent list = m_listExtent;
    CSWGuiExtent button;
    if (m_bIcon) {
        panel.height += 0x20;
        list.top += 0x20;
        panel.top -= 0x10;
    }
    if (m_bButtons && !m_bHidden) {
        if (!m_bCancel) {
            panel.height -= m_cancel.m_extent.height + 2;
            panel.top += (m_cancel.m_extent.height + 2) / 2;
        }
    } else {
        int nHeight = m_ok.m_extent.height;
        panel.height -= nHeight;
        panel.top += nHeight / 2;
    }
    if (m_ok.m_bEnabled) {
        button = m_ok.m_extent;
        button.width = 100;
        m_ok.SetExtent(button);
        while (m_ok.m_text.GetIdealHeight() != m_ok.m_text.GetFontHeight()) {
            button.width += 10;
            m_ok.SetExtent(button);
        }
        button.width += 2 * m_ok.m_nTextMargin;
        m_ok.SetExtent(button);
        if (panel.width <= button.width - 2) panel.width = button.width - 2;
    }
    if (m_cancel.m_bEnabled) {
        button = m_cancel.m_extent;
        button.width = 100;
        m_cancel.SetExtent(button);
        while (m_cancel.m_text.GetIdealHeight() != m_cancel.m_text.GetFontHeight()) {
            button.width += 10;
            m_cancel.SetExtent(button);
        }
        button.width += 2 * m_cancel.m_nTextMargin;
        m_cancel.SetExtent(button);
        if (panel.width <= button.width - 2) panel.width = button.width - 2;
    }
    list.width += panel.width - m_originalExtent.width;
    SetExtent(panel);
    m_list.SetExtent(list);
    m_list.UpdateScrollBar(0x10,1);
    AddMessageLabel();
    int nScrollWidth = m_list.m_scrollBar.m_extent.width;
    m_list.UpdateScrollBar(0,1);
    if (m_list.IsOversized()) {
        int nLineHeight = m_message.m_text.GetFontHeight();
        while (m_list.IsOversized() && (panel.width < 0x1b8 || panel.height < 0x118)) {
            if (panel.height > 0xa0 && panel.width < 0x1b8) {
                list.width += 0x28;
                panel.width += 0x28;
                panel.left -= 0x14;
                m_list.SetExtent(list);
                AddMessageLabel();
                if (!m_list.IsOversized()) break;
            }
            if (panel.height < 0x118) {
                list.height += nLineHeight;
                panel.height += nLineHeight;
                panel.top -= nLineHeight / 2;
                m_list.SetExtent(list);
                AddMessageLabel();
            }
        }
        SetExtent(panel);
    }
    if (m_list.IsOversized()) {
        m_list.UpdateScrollBar(nScrollWidth,1);
        AddMessageLabel();
    }
    int nCenter = m_extent.width / 2;
    button = m_icon.m_extent;
    button.left = nCenter - button.width / 2;
    m_icon.SetExtent(button);
    const CSWGuiExtent& ok = m_ok.m_extent;
    button = ok;
    button.top = m_list.m_extent.top + m_list.m_extent.height + 4;
    button.left = nCenter - button.width / 2;
    m_ok.SetExtent(button);
    button = m_cancel.m_extent;
    button.top = ok.top + ok.height + 2;
    button.left = nCenter - button.width / 2;
    m_cancel.SetExtent(button);
    CenterPanel();
}
