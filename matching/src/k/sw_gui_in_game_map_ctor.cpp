// CSWGuiInGameMap constructor (0x10DC bytes, vtable 0x00754830). Member
// offsets as sw_gui_in_game_map.cpp; control classes as
// include/k/sw_gui_pazaak.h, trimmed to sized members.

class CExoString {
public:
    CExoString(const char* s);                      // 0x005E5A90
    ~CExoString();                                  // 0x005E5C20
    char* m_sString;
    unsigned long m_nBufferLength;
};

class CResRef {
public:
    CResRef(const char* s);                         // 0x00406D80
    char m_resRef[16];
};

struct CSWGuiExtent {
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

class CSWGuiControl;
class CSWGuiPanel;
class CSWGuiManager;

class CSWGuiObject {
public:
    virtual ~CSWGuiObject();
    virtual void SetExtent(const CSWGuiExtent& extent);  // 0x04
    char pad_0x4[0x10];
};

class CSWGuiControl : public CSWGuiObject {
public:
    virtual ~CSWGuiControl();                       // 0x00419A20
    void AddEvent(int nEvent, CSWGuiPanel* pObject,
                  void (CSWGuiPanel::*pfnCallback)(CSWGuiControl*));   // 0x0041AB20
    void SetNavigable(int b) { m_bNavigable = b; }
    void SetFlag1(int b) { m_bFlag1 = b; }

    char pad_0x14[0x44 - 0x14];
    unsigned long m_bFlag0 : 1;                     // 0x44 bit 0
    unsigned long m_bFlag1 : 1;                     //      bit 1
    unsigned long m_bNavigable : 1;                 //      bit 2
    char pad_0x48[0x5c - 0x48];
};

// 0x140 bytes, vtable 0x0073e5b8.
class CSWGuiLabel : public CSWGuiControl {
public:
    CSWGuiLabel();                                  // 0x0041ACD0
    virtual ~CSWGuiLabel();                         // 0x00419D10
    char pad_0x5c[0x140 - 0x5c];
};

// 0x1c4 bytes, vtable 0x0073e658.
class CSWGuiButton : public CSWGuiControl {
public:
    CSWGuiButton();                                 // 0x0041C0F0
    virtual ~CSWGuiButton();                        // 0x00419DA0
    char pad_0x5c[0x1c4 - 0x5c];
};

// 0x248 bytes, vtable 0x00754738.
class CSWGuiMapHider : public CSWGuiControl {
public:
    CSWGuiMapHider();                               // 0x00693F60
    virtual ~CSWGuiMapHider();                      // 0x006942D0
    char pad_0x5c[0x248 - 0x5c];
};

// 0x54 bytes, vtable 0x0073e390.
class CSWGuiImage {
public:
    CSWGuiImage();                                  // 0x00415A30
    virtual ~CSWGuiImage();                         // 0x00415AD0
    virtual void SetExtent(const CSWGuiExtent& extent);  // 0x04
    char pad_0x4[0x54 - 0x4];
};

class CSWGuiPanel : public CSWGuiObject {
public:
    CSWGuiPanel(CSWGuiManager* pManager);           // 0x0040B570
    virtual ~CSWGuiPanel();                         // 0x0040CF70
    virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void OnAButtonPressed(CSWGuiControl* pControl);  // 0x50 (descriptive)
    virtual void OnBButtonPressed(CSWGuiControl* pControl);  // 0x54 (descriptive)
    virtual void OnXButtonPressed(CSWGuiControl* pControl);  // 0x58 (descriptive)

    void StartLoadFromLayout(const CResRef& cResRef);   // 0x0040A680
    void UnloadResource();                          // 0x0040B8F0
    void InitControl(CSWGuiControl* pControl, const CExoString& sName, int bAdd);  // 0x0040B930
    void AddControl(CSWGuiControl* pControl);       // 0x0040B9A0

    char pad_0x14[0x64 - 0x14];
};

class CSWGuiInGameMap : public CSWGuiPanel {
public:
    CSWGuiInGameMap(CSWGuiManager* pManager);
    virtual ~CSWGuiInGameMap();                     // 0x00695BF0
    void OnUpArrowPressed(CSWGuiControl* pControl);     // 0x006927B0 (descriptive)
    void OnDownArrowPressed(CSWGuiControl* pControl);   // 0x006927C0 (descriptive)

    CSWGuiLabel m_lblMap;                           // 0x64
    CSWGuiLabel m_lblArea;                          // 0x1a4
    CSWGuiLabel m_lblMapNote;                       // 0x2e4
    CSWGuiLabel m_lblCompass;                       // 0x424
    CSWGuiButton m_btnReturn;                       // 0x564
    CSWGuiButton m_btnPartySelect;                  // 0x728
    CSWGuiButton m_btnExit;                         // 0x8ec
    CSWGuiButton m_btnUp;                           // 0xab0
    CSWGuiButton m_btnDown;                         // 0xc74
    CSWGuiMapHider m_cMapHider;                     // 0xe38
    CSWGuiImage m_cMapImage;                        // 0x1080
    unsigned long m_nNoReturnStrRef;                // 0x10d4 (descriptive)
    unsigned long m_nMapFlags;                      // 0x10d8 (descriptive)
};

typedef char KInGameMapSize[(sizeof(CSWGuiInGameMap) == 0x10dc) ? 1 : -1];

typedef void (CSWGuiPanel::*CSWGuiCallback)(CSWGuiControl*);

// FUNCTION: SWKOTOR 0x00694D50
CSWGuiInGameMap::CSWGuiInGameMap(CSWGuiManager* pManager)
    : CSWGuiPanel(pManager)
{
    {
        CResRef cLayout("map");
        StartLoadFromLayout(cLayout);
    }
    {
        CExoString sName("LBL_Map");
        InitControl(&m_lblMap, sName, 0);
    }
    {
        CExoString sName("LBL_MapNote");
        InitControl(&m_lblMapNote, sName, 1);
    }
    {
        CExoString sName("LBL_Area");
        InitControl(&m_lblArea, sName, 1);
    }
    {
        CExoString sName("LBL_COMPASS");
        InitControl(&m_lblCompass, sName, 1);
    }
    {
        CExoString sName("BTN_RETURN");
        InitControl(&m_btnReturn, sName, 1);
    }
    {
        CExoString sName("BTN_PRTYSLCT");
        InitControl(&m_btnPartySelect, sName, 1);
    }
    {
        CExoString sName("BTN_EXIT");
        InitControl(&m_btnExit, sName, 1);
    }
    {
        CExoString sName("BTN_UP");
        InitControl(&m_btnUp, sName, 1);
    }
    {
        CExoString sName("BTN_DOWN");
        InitControl(&m_btnDown, sName, 1);
    }
    UnloadResource();
    m_cMapImage.SetExtent(CSWGuiExtent(0, 0, 0x200, 0x100));
    m_cMapHider.SetExtent(CSWGuiExtent(0, 0, 0x1b8, 0x100));
    AddControl(&m_cMapHider);
    m_cMapHider.SetFlag1(0);
    m_nMapFlags |= 3;
    m_nNoReturnStrRef = 0;
    m_btnReturn.SetNavigable(0);
    m_btnReturn.AddEvent(0x27, this, &CSWGuiPanel::OnXButtonPressed);
    m_btnPartySelect.SetNavigable(0);
    m_btnPartySelect.AddEvent(0x27, this, &CSWGuiPanel::OnAButtonPressed);
    m_btnExit.SetNavigable(0);
    m_btnExit.AddEvent(0x27, this, &CSWGuiPanel::OnBButtonPressed);
    m_btnUp.SetNavigable(0);
    m_btnUp.AddEvent(0x27, this, (CSWGuiCallback)&CSWGuiInGameMap::OnUpArrowPressed);
    m_btnDown.SetNavigable(0);
    m_btnDown.AddEvent(0x27, this, (CSWGuiCallback)&CSWGuiInGameMap::OnDownArrowPressed);
}
