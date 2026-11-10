// CSWGuiMainInterface constructor (in-game HUD panel, 0xC8CC bytes, vtable
// 0x00753F50). Member offsets as include/k/sw_gui_main_interface.h; control
// classes trimmed to sized members as sw_gui_in_game_map_ctor.cpp. The
// local map-view label's destructor is inlined, so CSWGuiLabel's is an
// inline virtual destructor here (out-of-line copy 0x00419D10).

class CExoString {
public:
    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    ~CExoString();                                  // 0x005E5C20
    void Format(const char* szFormat, ...);         // 0x005E5680
    char* m_sString;
    unsigned long m_nBufferLength;
};

class CResRef {
public:
    CResRef(const char* s);                         // 0x00406D80
    char m_resRef[16];
};

struct CSWGuiExtent {
    CSWGuiExtent() {}
    CSWGuiExtent(int nLeft, int nTop, int nWidth, int nHeight)
    {
        left = nLeft;
        top = nTop;
        width = nWidth;
        height = nHeight;
    }
    int left;
    int top;
    int width;
    int height;
};

// Zero-initialised extent (control and map rectangles).
struct CSWGuiRect : public CSWGuiExtent {
    CSWGuiRect() { left = top = width = height = 0; }
};

template <class T>
class CExoArrayList {
public:
    CExoArrayList() { num = 0; array_size = 0; element = 0; }
    ~CExoArrayList()
    {
        delete[] element;
        element = 0;
    }
    T* element;
    int num;
    int array_size;
};

class CTlkTable {
public:
    CExoString GetSimpleString(unsigned long nStrRef);  // 0x0041E8F0
};

extern CTlkTable* g_pTlkTable;                      // 0x007A3A08

class CAurGUIImage;
CAurGUIImage* NewCAurGUIImage(char* sName);         // 0x0045A260

extern unsigned long g_nScreenWidth;                // 0x0078D1D4
extern unsigned long g_nScreenHeight;               // 0x0078D1D8

class CSWGuiManager {
public:
    void GetSafeMargins(int* pnX, int* pnY);        // 0x00409EE0
    char pad_0x0[0x6c];
    short m_nWidth;                                 // 0x6c
    short m_nHeight;                                // 0x6e
};

class CSWGuiControl;
class CSWGuiPanel;
class CSWGuiMainInterface;

class CSWGuiObject {
public:
    virtual ~CSWGuiObject();
    virtual void SetExtent(const CSWGuiExtent& extent);  // 0x04
    CSWGuiRect m_extent;                            // 0x04
};

class CSWGuiControl : public CSWGuiObject {
public:
    virtual ~CSWGuiControl();                       // 0x00419A20
    void AddEvent(int nEvent, CSWGuiPanel* pObject,
                  void (CSWGuiPanel::*pfnCallback)(CSWGuiControl*));   // 0x0041AB20
    void SetVisible(int b) { m_bVisible = b; }
    void SetNavigable(int b) { m_bNavigable = b; }
    void SetFlag5(int b) { m_bFlag5 = b; }
    void SetToolTip(int nStrRef, int nKey)
    {
        m_nToolTipStrRef = nStrRef;
        m_nKeyBinding = nKey;
    }

    char pad_0x14[0x24 - 0x14];
    int m_nToolTipStrRef;                           // 0x24
    char pad_0x28[0x8];
    int m_nKeyBinding;                              // 0x30
    char pad_0x34[0x44 - 0x34];
    unsigned long m_bActive : 1;                    // 0x44 bit 0
    unsigned long m_bVisible : 1;                   //      bit 1
    unsigned long m_bNavigable : 1;                 //      bit 2
    unsigned long m_bFlag3 : 1;
    unsigned long m_bFlag4 : 1;
    unsigned long m_bFlag5 : 1;                     //      bit 5
    char pad_0x48[0x5c - 0x48];
};

class CSWGuiBorderParams {
public:
    char pad_0x0[0x1c];
    unsigned long m_bFlag0 : 1;                     // 0x1c bit 0
    unsigned long m_bFlag1 : 1;                     //      bit 1
    char pad_0x20[0x54 - 0x20];
};

// 0x74 bytes.
class CSWGuiBorder {
public:
    virtual ~CSWGuiBorder();                        // 0x004151C0
    char pad_0x4[0x10];
    CSWGuiBorderParams m_cParams;                   // 0x14
    char pad_0x68[0x74 - 0x68];
};

class CSWGuiTextParams {
public:
    void SetText(const CExoString& sText);          // 0x00415E00
    char pad_0x0[0x58];
};

// 0x70 bytes.
class CSWGuiText {
public:
    virtual ~CSWGuiText();                          // 0x00415FF0
    char pad_0x4[0x14];
    CSWGuiTextParams m_cParams;                     // 0x18
};

// 0x140 bytes, vtable 0x0073e5b8.
class CSWGuiLabel : public CSWGuiControl {
public:
    CSWGuiLabel();                                  // 0x0041ACD0
    __forceinline virtual ~CSWGuiLabel() {}
    virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94();
    virtual void CopyStyle(const CSWGuiExtent& extent, CSWGuiLabel* pFrom, float fScale);  // 0x98
    CSWGuiBorder m_cBorder;                         // 0x5c
    CSWGuiText m_cText;                             // 0xd0
};

// 0x1b4 bytes.
class CSWGuiLabelHilight : public CSWGuiLabel {
public:
    CSWGuiLabelHilight();                           // 0x0041B5C0
    virtual ~CSWGuiLabelHilight();                  // 0x0041B610
    char pad_0x140[0x1b4 - 0x140];
};

// 0x1c4 bytes, vtable 0x0073e658.
class CSWGuiButton : public CSWGuiControl {
public:
    CSWGuiButton();                                 // 0x0041C0F0
    virtual ~CSWGuiButton();                        // 0x00419DA0
    char pad_0x5c[0x1c4 - 0x5c];
};

// 0x2b4 bytes.
class CSWGuiButtonToggle : public CSWGuiButton {
public:
    CSWGuiButtonToggle();                           // 0x0041CA30
    virtual ~CSWGuiButtonToggle();                  // 0x0041CAD0
    char pad_0x1c4[0x2b4 - 0x1c4];
};

// Personal action list (CExoArrayList of target actions), 0xc bytes.
struct CSWGuiTargetActionList {
    CSWGuiTargetActionList();                       // 0x005A76D0
    ~CSWGuiTargetActionList();                      // 0x005FBFB0
    char pad_0x0[0xc];
};

// 0x1af0 bytes.
class CSWGuiTargetActionMenu {
public:
    CSWGuiTargetActionMenu();                       // 0x0068BE40
    ~CSWGuiTargetActionMenu();                      // 0x0068B0C0
    void Initialize(CSWGuiPanel* pPanel);           // 0x0068BF50
    char pad_0x0[0x15bc];
    CSWGuiExtent m_rect15bc;                        // 0x15bc
    char pad_0x15cc[0x1af0 - 0x15cc];
};

// 0xea8 bytes.
class CSWGuiMainInterfaceChar {
public:
    CSWGuiMainInterfaceChar();                      // 0x00689C00
    ~CSWGuiMainInterfaceChar();                     // 0x00689CC0
    void Initialize(CSWGuiMainInterface* pPanel, int nIndex);   // 0x006898A0
    char pad_0x0[0xea8];
};

// 0x71c bytes.
class CSWGuiMainInterfaceAction {
public:
    CSWGuiMainInterfaceAction();                    // 0x00689200
    ~CSWGuiMainInterfaceAction();                   // 0x006893A0
    void Initialize(CSWGuiPanel* pPanel, int nIndex, int bTarget);  // 0x0068B9D0
    char pad_0x0[0x71c];
};

// 0x14c bytes.
class CSWGuiMainInterfaceStatus {
public:
    CSWGuiMainInterfaceStatus();                    // 0x00686AA0
    ~CSWGuiMainInterfaceStatus();                   // 0x0068AD10
    void Init(CSWGuiPanel* pPanel, unsigned char nStatus);  // 0x00686920
    char pad_0x0[0x14c];
};

class CSWGuiPanel : public CSWGuiObject {
public:
    CSWGuiPanel(CSWGuiManager* pManager);           // 0x0040B570
    virtual ~CSWGuiPanel();                         // 0x0040CF70
    virtual void SetExtent(const CSWGuiExtent& extent);  // 0x04 (0x0040A5A0)

    void StartLoadFromLayout(const CResRef& cResRef);   // 0x0040A680
    void UnloadResource();                          // 0x0040B8F0
    void InitControl(CSWGuiControl* pControl, const CExoString& sName, int bAdd);  // 0x0040B930
    void AddControl(CSWGuiControl* pControl);       // 0x0040B9A0

    int m_bUpdateFonts;                             // 0x14
    CSWGuiManager* m_pManager;                      // 0x18
    char pad_0x1c[0x44 - 0x1c];
    unsigned long m_nPanelFlags;                    // 0x44
    char pad_0x48[0x64 - 0x48];
};

class CSWGuiMainInterface : public CSWGuiPanel {
public:
    CSWGuiMainInterface(CSWGuiManager* pManager);
    virtual ~CSWGuiMainInterface();                 // 0x0068B170
    void SetMessageText(const CExoString& sText);   // 0x00685560
    void ShowActionQueue(int bShow);                // 0x006874C0
    void UpdateMessage();                           // 0x00686E20
    void OnPauseClicked(CSWGuiControl* pControl);           // 0x00688590 (descriptive)
    void OnSoloClicked(CSWGuiControl* pControl);            // 0x00688610 (descriptive)
    void OnStealthClicked(CSWGuiControl* pControl);         // 0x00688640 (descriptive)
    void OnMenuButtonPressed(CSWGuiControl* pControl);      // 0x006883C0 (descriptive)
    void OnCancelCombatClicked(CSWGuiControl* pControl);    // 0x0068B050
    void OnClearTargetClicked(CSWGuiControl* pControl);     // 0x0068B0A0

    int m_nTargetId;                                // 0x64
    int m_n68;                                      // 0x68
    float m_fTargetTime;                            // 0x6c
    int m_n70;                                      // 0x70
    CSWGuiTargetActionList m_aPersonal[6];          // 0x74
    CSWGuiTargetActionMenu m_cTargetMenu;           // 0xbc
    int m_anPersonalSelected[6];                    // 0x1bac
    int m_nLastPersonalSlot;                        // 0x1bc4
    CSWGuiLabel m_aPartyLabels[3];                  // 0x1bc8
    CSWGuiMainInterfaceChar m_aParty[3];            // 0x1f88
    CSWGuiMainInterfaceChar m_cLeader;              // 0x4b80
    float m_fSlideTime;                             // 0x5a28
    CSWGuiLabel m_lbl5a2c;                          // 0x5a2c
    CSWGuiLabel m_lbl5b6c;                          // 0x5b6c
    int m_n5cac;                                    // 0x5cac
    float m_fTargetMenuTime;                        // 0x5cb0
    CExoArrayList<CSWGuiControl*> m_lstControls;    // 0x5cb4
    CSWGuiLabel m_lblMap;                           // 0x5cc0
    CSWGuiLabel m_lblMapBorder;                     // 0x5e00
    CSWGuiLabel m_lblMapOverlay;                    // 0x5f40
    CSWGuiRect m_rectMap;                           // 0x6080
    int m_n6090;                                    // 0x6090
    int m_n6094;                                    // 0x6094
    CSWGuiButton m_btnMapArrow;                     // 0x6098
    CSWGuiLabel m_aActionIcons[4];                  // 0x625c
    CSWGuiLabel m_aQueueLabels[3];                  // 0x675c
    CSWGuiLabelHilight m_hlQueue;                   // 0x6b1c
    CSWGuiButton m_btnQueue0;                       // 0x6cd0
    CSWGuiButton m_btnQueue1;                       // 0x6e94
    CSWGuiButton m_btnQueue2;                       // 0x7058
    CSWGuiLabel m_lblQueueBG;                       // 0x721c
    CSWGuiLabel m_lblDescription;                   // 0x735c
    CSWGuiLabel m_lblDescriptionBG;                 // 0x749c
    CSWGuiLabel m_lblDescriptionAlt;                // 0x75dc
    int m_bDescriptionHidden;                       // 0x771c
    float m_fMessageDuration;                       // 0x7720 (descriptive)
    float m_fQueueTime;                             // 0x7724
    unsigned long m_nCombatMessage;                 // 0x7728 (descriptive)
    CSWGuiMainInterfaceAction m_aPersonalActions[6];    // 0x772c
    CSWGuiLabel m_lblMessage;                       // 0xa1d4
    CSWGuiLabel m_lblMessageBG;                     // 0xa314
    int m_nMessageBottom;                           // 0xa454
    unsigned char m_nQueueShift;                    // 0xa458
    unsigned long m_bInterfaceFlag0 : 1;            // 0xa45c bit 0
    unsigned long m_bQueueShown : 1;                //        bit 1
    CSWGuiMainInterfaceStatus m_aStatus[9];         // 0xa460
    CSWGuiButton m_btnMinimap;                      // 0xb00c
    CSWGuiButton m_btnB1d0;                         // 0xb1d0
    CSWGuiButton m_btnB394;                         // 0xb394
    CSWGuiButton m_btnB558;                         // 0xb558
    CSWGuiButton m_btnB71c;                         // 0xb71c
    CSWGuiButton m_btnB8e0;                         // 0xb8e0
    CSWGuiButton m_btnBaa4;                         // 0xbaa4
    CSWGuiButton m_btnBc68;                         // 0xbc68
    CSWGuiLabel m_lblBe2c;                          // 0xbe2c
    CSWGuiLabel m_lblBf6c;                          // 0xbf6c
    CSWGuiButtonToggle m_tglC0ac;                   // 0xc0ac
    CSWGuiButtonToggle m_tglC360;                   // 0xc360
    CSWGuiButtonToggle m_tglC614;                   // 0xc614
    CAurGUIImage* m_pExtra;                         // 0xc8c8
};

typedef char KMainInterfaceSize[(sizeof(CSWGuiMainInterface) == 0xc8cc) ? 1 : -1];

extern CSWGuiMainInterface* g_pMainInterface;       // 0x00833BB4 (descriptive)

typedef void (CSWGuiPanel::*CSWGuiCallback)(CSWGuiControl*);

// FUNCTION: SWKOTOR 0x0068C100
CSWGuiMainInterface::CSWGuiMainInterface(CSWGuiManager* pManager)
    : CSWGuiPanel(pManager)
{
    int i;
    CExoString sName;
    CSWGuiLabel lblMapView;
    g_pMainInterface = this;
    switch (pManager->m_nWidth) {
    case 0x400:
        StartLoadFromLayout("mipc210x7");
        break;
    case 0x500:
        switch (pManager->m_nHeight) {
        case 0x3c0:
            StartLoadFromLayout("mipc212x9");
            break;
        case 0x400:
            StartLoadFromLayout("mipc212x10");
            break;
        }
        break;
    case 0x640:
        StartLoadFromLayout("mipc216x12");
        break;
    default:
        StartLoadFromLayout("mipc28x6");
        break;
    }
    m_nPanelFlags |= 1;
    for (i = 0; i < 6; i++) {
        m_aPersonalActions[i].Initialize(this, i, 0);
    }
    for (i = 0; i < 3; i++) {
        m_aParty[i].Initialize(this, i + 1);
    }
    m_cLeader.Initialize(this, 4);
    for (i = 0; i < 4; i++) {
        sName.Format("LBL_QUEUE%d", i);
        InitControl(&m_aActionIcons[i], sName, 1);
    }
    for (i = 0; i < 9; i++) {
        m_aStatus[i].Init(this, i);
    }
    m_cTargetMenu.Initialize(this);
    for (i = 0; i < 3; i++) {
        sName.Format("LBL_MOULDING%d", i + 1);
        InitControl(&m_aPartyLabels[i], sName, 1);
    }
    for (i = 0; i < 3; i++) {
        sName.Format("LBL_COMBATBG%d", i + 1);
        InitControl(&m_aQueueLabels[i], sName, 1);
    }
    InitControl(&m_btnQueue0, "BTN_CLEARONE", 1);
    InitControl(&m_btnQueue1, "BTN_CLEARONE2", 1);
    InitControl(&m_btnQueue2, "BTN_CLEARALL", 1);
    InitControl(&m_lblMessage, "LBL_ACTIONDESC", 1);
    InitControl(&m_lblMessageBG, "LBL_ACTIONDESCBG", 1);
    InitControl(&m_lblMap, "LBL_MAPBORDER", 1);
    InitControl(&m_lblMapOverlay, "LBL_ARROW", 0);
    InitControl(&m_lblMapBorder, "LBL_MAP", 0);
    InitControl(&lblMapView, "LBL_MAPVIEW", 0);
    InitControl(&m_btnMapArrow, "BTN_MINIMAP", 1);
    InitControl(&m_lblDescription, "LBL_CMBTMODEMSG", 1);
    m_lblDescriptionAlt.CopyStyle(m_lblDescription.m_extent, &m_lblDescription, 1.0f);
    InitControl(&m_lblDescriptionBG, "LBL_CMBTMSGBG", 1);
    m_lblDescription.SetVisible(0);
    m_lblDescriptionBG.SetVisible(0);
    m_lblDescriptionAlt.SetVisible(0);
    m_lblDescriptionAlt.m_cText.m_cParams.SetText(g_pTlkTable->GetSimpleString(0xbd4a));
    InitControl(&m_btnMinimap, "BTN_EQU", 1);
    InitControl(&m_btnB1d0, "BTN_INV", 1);
    InitControl(&m_btnB394, "BTN_CHAR", 1);
    InitControl(&m_btnB558, "BTN_ABI", 1);
    InitControl(&m_btnB71c, "BTN_MSG", 1);
    InitControl(&m_btnB8e0, "BTN_JOU", 1);
    InitControl(&m_btnBaa4, "BTN_MAP", 1);
    InitControl(&m_btnBc68, "BTN_OPT", 1);
    InitControl(&m_lblBe2c, "LBL_MENUBG", 1);
    InitControl(&m_lblBf6c, "LBL_ARROW_MARGIN", 1);
    InitControl(&m_tglC0ac, "TB_PAUSE", 1);
    InitControl(&m_tglC360, "TB_SOLO", 1);
    InitControl(&m_tglC614, "TB_STEALTH", 1);
    UnloadResource();
    {
        CSWGuiExtent extMenu;
        int nTop = m_lblBe2c.m_extent.top + m_lblBe2c.m_extent.height;
        extMenu.height = m_tglC0ac.m_extent.top - nTop;
        extMenu.width = m_pManager->m_nWidth - 2 * m_lblBf6c.m_extent.left;
        extMenu.top = nTop;
        extMenu.left = m_lblBf6c.m_extent.left;
        m_cTargetMenu.m_rect15bc = extMenu;
    }
    AddControl(&m_lblDescriptionAlt);
    if (pManager->m_nWidth >= 0x400) {
        CSWGuiExtent extent = m_extent;
        extent.height = g_nScreenHeight;
        extent.width = g_nScreenWidth;
        SetExtent(extent);
    }
    int nMarginX, nMarginY;
    CSWGuiManager* pGuiManager = m_pManager;
    pGuiManager->GetSafeMargins(&nMarginX, &nMarginY);
    m_n6094 = m_lblMapBorder.m_extent.height;
    m_n6090 = m_lblMapBorder.m_extent.width;
    m_rectMap = lblMapView.m_extent;
    CSWGuiExtent extArrow = m_lblMapOverlay.m_extent;
    extArrow.left = (m_rectMap.width - extArrow.width) / 2;
    extArrow.top = (m_rectMap.height - extArrow.height) / 2;
    m_nQueueShift = 0;
    m_fQueueTime = -1.0f;
    m_lblMapOverlay.SetExtent(extArrow);
    m_lbl5a2c.m_cBorder.m_cParams.m_bFlag0 = 0;
    m_lbl5a2c.m_cBorder.m_cParams.m_bFlag1 = 1;
    m_lbl5a2c.SetFlag5(1);
    AddControl(&m_lbl5a2c);
    m_lbl5b6c.SetExtent(CSWGuiExtent(0, 0, 0x10, 0x10));
    m_lbl5b6c.m_cBorder.m_cParams.m_bFlag0 = 0;
    m_lbl5b6c.m_cBorder.m_cParams.m_bFlag1 = 1;
    m_lbl5b6c.SetFlag5(1);
    m_lbl5b6c.SetVisible(0);
    AddControl(&m_lbl5b6c);
    m_n5cac = 0x7f000000;
    m_nTargetId = 0x7f000000;
    m_n68 = 0x7f000000;
    m_fTargetMenuTime = -1.0f;
    m_fTargetTime = -1.0f;
    for (i = 0; i < 6; i++) {
        m_anPersonalSelected[i] = -1;
    }
    m_bQueueShown = 1;
    m_fSlideTime = 0.0f;
    ShowActionQueue(0);
    m_nCombatMessage = 0xbc50;
    m_bDescriptionHidden = 0;
    m_btnMinimap.AddEvent(0x27, this, (CSWGuiCallback)&CSWGuiMainInterface::OnMenuButtonPressed);
    m_btnB1d0.AddEvent(0x27, this, (CSWGuiCallback)&CSWGuiMainInterface::OnMenuButtonPressed);
    m_btnB394.AddEvent(0x27, this, (CSWGuiCallback)&CSWGuiMainInterface::OnMenuButtonPressed);
    m_btnB558.AddEvent(0x27, this, (CSWGuiCallback)&CSWGuiMainInterface::OnMenuButtonPressed);
    m_btnB8e0.AddEvent(0x27, this, (CSWGuiCallback)&CSWGuiMainInterface::OnMenuButtonPressed);
    m_btnB71c.AddEvent(0x27, this, (CSWGuiCallback)&CSWGuiMainInterface::OnMenuButtonPressed);
    m_btnBaa4.AddEvent(0x27, this, (CSWGuiCallback)&CSWGuiMainInterface::OnMenuButtonPressed);
    m_btnBc68.AddEvent(0x27, this, (CSWGuiCallback)&CSWGuiMainInterface::OnMenuButtonPressed);
    m_btnMapArrow.AddEvent(0x27, this, (CSWGuiCallback)&CSWGuiMainInterface::OnMenuButtonPressed);
    m_btnMinimap.SetNavigable(0);
    m_btnB1d0.SetNavigable(0);
    m_btnB394.SetNavigable(0);
    m_btnB558.SetNavigable(0);
    m_btnB8e0.SetNavigable(0);
    m_btnB71c.SetNavigable(0);
    m_btnBaa4.SetNavigable(0);
    m_btnBc68.SetNavigable(0);
    m_btnMapArrow.SetNavigable(0);
    m_btnQueue0.AddEvent(0x27, this, (CSWGuiCallback)&CSWGuiMainInterface::OnCancelCombatClicked);
    m_btnQueue1.AddEvent(0x27, this, (CSWGuiCallback)&CSWGuiMainInterface::OnCancelCombatClicked);
    m_btnQueue2.AddEvent(0x27, this, (CSWGuiCallback)&CSWGuiMainInterface::OnClearTargetClicked);
    m_btnQueue0.SetNavigable(0);
    m_btnQueue1.SetNavigable(0);
    m_btnQueue2.SetNavigable(0);
    m_lblBf6c.SetVisible(0);
    m_lblBf6c.SetNavigable(0);
    m_tglC0ac.AddEvent(0x27, this, (CSWGuiCallback)&CSWGuiMainInterface::OnPauseClicked);
    m_tglC360.AddEvent(0x27, this, (CSWGuiCallback)&CSWGuiMainInterface::OnSoloClicked);
    m_tglC614.AddEvent(0x27, this, (CSWGuiCallback)&CSWGuiMainInterface::OnStealthClicked);
    m_tglC0ac.SetNavigable(0);
    m_tglC360.SetNavigable(0);
    m_tglC614.SetNavigable(0);
    m_pExtra = NewCAurGUIImage("blackdot");
    m_nLastPersonalSlot = 0;
    m_lblDescription.m_cText.m_cParams.SetText("");
    m_nMessageBottom = m_lblMessage.m_extent.height + m_lblMessage.m_extent.top;
    SetMessageText("");
    UpdateMessage();
    m_btnMinimap.SetToolTip(0xbc5b, 0xd1);
    m_btnB1d0.SetToolTip(0xbc5c, 0xd2);
    m_btnB394.SetToolTip(0xbc61, 0xd3);
    m_btnBaa4.SetToolTip(0xbc5d, 0xd7);
    m_btnMapArrow.SetToolTip(0xbc5d, 0xd7);
    m_btnB558.SetToolTip(0xbc60, 0xd4);
    m_btnB8e0.SetToolTip(0xbc5a, 0xd6);
    m_btnBc68.SetToolTip(0xbc5e, 0xd8);
    m_btnB71c.SetToolTip(0xbc5f, 0xd5);
    m_tglC0ac.SetToolTip(0xbb93, 0xf1);
    m_tglC360.SetToolTip(0xbba3, 0xcf);
    m_tglC614.SetToolTip(0xf7, 0x108);
    m_btnQueue0.SetToolTip(0xbd48, 0xf5);
    m_btnQueue1.SetToolTip(0xbd48, 0xf5);
    m_btnQueue2.SetToolTip(0xbd86, 0xf0);
}
