// CSWGuiStatusSummary constructor (swguiingame.o, vtable 0x0074FF68,
// 0x1B44 bytes). Standalone layout: summary record +0x64, nine icon labels
// +0x300, nine description labels +0xE40, OK button +0x1980.
struct CSWGuiExtent { int left,top,width,height; };
class CSWGuiControl;
class CSWGuiPanel;
class CSWGuiManager;
class CExoString { public: char* m_sString; unsigned long m_nLength; CExoString(const char* s); ~CExoString(); };
class CResRef { public: char m_resRef[16]; CResRef(const char* s); };
class CSWGuiControl {
public:
    virtual ~CSWGuiControl();
    CSWGuiExtent m_extent; char pad_0x14[0x30];
    union { unsigned long m_nFlags; struct { unsigned long m_bOther:1; unsigned long m_bEnabled:1; unsigned long m_bSelectable:1; unsigned long m_nOther:29; }; };
    char pad_0x48[8];
    void AddEvent(int nEvent,CSWGuiPanel* pObject,void (CSWGuiPanel::*pfnCallback)(CSWGuiControl*)); // 0x0041AB20
};
class CSWGuiLabel : public CSWGuiControl { public: CSWGuiLabel(); virtual ~CSWGuiLabel(); char pad_0x50[0xf0]; };
class CSWGuiButton : public CSWGuiControl { public: CSWGuiButton(); virtual ~CSWGuiButton(); char pad_0x50[0x174]; };
class CSWGuiPanel {
public:
    CSWGuiPanel(CSWGuiManager* pManager); // 0x0040B570
    virtual ~CSWGuiPanel();
    virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10(); virtual void v14();
    virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24(); virtual void v28();
    virtual void v2c(); virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void OnCloseClicked(CSWGuiControl* pControl); // +0x50
    CSWGuiExtent m_extent; int m_n14; CSWGuiManager* m_pManager;
    char pad_0x1c[0x44-0x1c]; unsigned long m_nFlags; char pad_0x48[0x64-0x48];
    void StartLoadFromLayout(const CResRef& cLayout); // 0x0040A680
    void InitControl(CSWGuiControl* pControl,const CExoString& sName,int bAdd); // 0x0040B930
    void UnloadResource(); // 0x0040B8F0
};
struct CStatusSummary {
    unsigned long m_nFlags;
    unsigned char m_nLightShift,m_nDarkShift; char pad_0x6[2];
    int m_nCredits;
    unsigned long m_nXp,m_nStealthXp;
    int m_nSuppressEntries;
};
class CSWGuiStatusSummary : public CSWGuiPanel {
public:
    CSWGuiStatusSummary(CSWGuiManager* pManager);
    CStatusSummary m_summary; char pad_0x7c[4];
    CSWGuiLabel m_heading,m_total;
    CSWGuiLabel m_icons[9],m_descriptions[9];
    CSWGuiButton m_ok;
};

// FUNCTION: SWKOTOR 0x006272A0
CSWGuiStatusSummary::CSWGuiStatusSummary(CSWGuiManager* pManager) : CSWGuiPanel(pManager)
{
    {
        CResRef cLayout("statussummary");
        StartLoadFromLayout(cLayout);
    }
    { CExoString sName("LBL_JOURNAL"); InitControl(&m_icons[0],sName,1); }
    { CExoString sName("LBL_JOURNAL_DESC"); InitControl(&m_descriptions[0],sName,1); }
    { CExoString sName("LBL_CREDITS"); InitControl(&m_icons[1],sName,1); }
    { CExoString sName("LBL_CREDITS_DESC"); InitControl(&m_descriptions[1],sName,1); }
    { CExoString sName("LBL_XP"); InitControl(&m_icons[2],sName,1); }
    { CExoString sName("LBL_XP_DESC"); InitControl(&m_descriptions[2],sName,1); }
    { CExoString sName("LBL_STEALTH"); InitControl(&m_icons[3],sName,1); }
    { CExoString sName("LBL_STEALTH_DESC"); InitControl(&m_descriptions[3],sName,1); }
    { CExoString sName("LBL_DARKSIDE"); InitControl(&m_icons[4],sName,1); }
    { CExoString sName("LBL_DARKSIDE_DESC"); InitControl(&m_descriptions[4],sName,1); }
    { CExoString sName("LBL_LIGHTSIDE"); InitControl(&m_icons[5],sName,1); }
    { CExoString sName("LBL_LIGHTSIDE_DESC"); InitControl(&m_descriptions[5],sName,1); }
    { CExoString sName("LBL_NETSHIFT"); InitControl(&m_icons[6],sName,1); }
    { CExoString sName("LBL_NETSHIFT_DESC"); InitControl(&m_descriptions[6],sName,1); }
    { CExoString sName("LBL_RECEIVED"); InitControl(&m_icons[7],sName,1); }
    { CExoString sName("LBL_RECEIVED_DESC"); InitControl(&m_descriptions[7],sName,1); }
    { CExoString sName("LBL_LOST"); InitControl(&m_icons[8],sName,1); }
    { CExoString sName("LBL_LOST_DESC"); InitControl(&m_descriptions[8],sName,1); }
    { CExoString sName("BTN_OK"); InitControl(&m_ok,sName,1); }
    UnloadResource();
    CSWGuiButton* pOk = &m_ok;
    pOk->m_bSelectable = 0;
    pOk->AddEvent(0x27,this,&CSWGuiPanel::OnCloseClicked);
    m_summary.m_nFlags &= ~0x1fful;
    m_summary.m_nCredits = 0;
    m_summary.m_nLightShift = 0;
    m_summary.m_nDarkShift = 0;
    m_summary.m_nXp = 0;
    m_summary.m_nStealthXp = 0;
    m_summary.m_nSuppressEntries = 0;
    m_nFlags |= 1;
}

// FUNCTION: SWKOTOR 0x00624BA0 SYMBOL: ??_9@$BFA@AE
// vcall thunk for &CSWGuiPanel::OnCloseClicked (+0x50), emitted by the ctor.
