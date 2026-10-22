// Worker K matching headers: the in-game main interface (CSWGuiMainInterface,
// vtable 0x00753F50), its target action menu (embedded at +0xbc), the
// per-action button groups and the party character slots,
// 0x00684DF0-0x0068B170. Control layouts follow
// matching/include/l/sw_gui_dialog.h and k/sw_gui_in_game_credits.h.
#ifndef K_SW_GUI_MAIN_INTERFACE_H
#define K_SW_GUI_MAIN_INTERFACE_H

class CExoString {
public:
    char* m_sString;
    int m_nBufferLength;
    CExoString();                                   // 0x005B3190
    CExoString(const char* sString);                // 0x005E5A90
    CExoString(const CExoString& sString);          // 0x005E5B00
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    CExoString& operator=(const char* s);           // 0x005E5140
    char* CStr() const;                             // 0x005E5670
    void Format(const char* szFormat, ...);         // 0x005E5680
};

class CResRef {
public:
    CResRef();                                      // 0x00405ED0
    CResRef(const char* s);                         // 0x00406D80
    CResRef(const CExoString& s);                   // 0x00406D60
    int IsValid() const;                            // 0x00406030
    int operator!=(const char* s) const;            // 0x00406060
    int operator!=(const CResRef& r) const;         // 0x00406DC0
    char m_resRef[16];
};

class Vector {
public:
    Vector() {}
    float x, y, z;
};

struct CSWGuiExtent {
    int left;
    int top;
    int width;
    int height;
};

// AurGUI layer helpers (cdecl).
void AurGUIStartLayer();                            // 0x004591B0
int AurGUISetupViewport(int x, int y, int w, int h, Vector* pColor, int n, float fAlpha); // 0x004592F0
void AurGUIStopLayer();                             // 0x004592B0
void AurGUICloseViewport();                         // 0x00459580
extern Vector g_vGuiViewportColor;                  // 0x0078D3D8

class CSWGuiControl;

class CSWGuiManager {
public:
    char pad_0x0[0x60];
    float m_fFrameTime;                             // 0x60
    char pad_0x64[0x8];
    float GetFrameTime() { return m_fFrameTime; }
    short m_nWidth;                                 // 0x6c
    short m_nHeight;                                // 0x6e
    void PlayGuiSound(char nSound);                 // 0x0040A140
    void AddToolTip(CExoString* pText);             // 0x0040B490
    void UpdateToolTip(CExoString* pText);          // 0x0040A340
};

extern CSWGuiManager* g_pGuiManager;                // 0x007A39F4

class CSWSCombatRound {
public:
    int RemoveLastAction();                         // 0x004D37B0
};

class CSWSCreature {
public:
    void ClearAllActions(int bClearCombat);         // 0x004CCD80
    char pad_0x0[0x9c8];
    CSWSCombatRound* m_pCombatRound;                // 0x9c8
};

class CSWCCreature;
class CGameObject;
class CGuiInGame;

class CSWParty {
public:
    CSWCCreature* GetPlayerCharacter();             // 0x00635460
};

class CClientOptions {
public:
    char pad_0x0[0x14];
    unsigned long m_nFlags0 : 9;                    // 0x14
    unsigned long m_bHideMap : 1;                   //      bit 9
    unsigned long m_nFlags10 : 5;
    unsigned long m_bTutorialPause : 1;             //      bit 15
    unsigned long m_nFlagsRest : 16;
    int ShowMinimap() { return !m_bHideMap; }
};

class CClientExoApp {
public:
    CGameObject* GetGameObject(unsigned long oid);  // 0x005ED580
    CGuiInGame* GetInGameGui();                     // 0x005ED690
    int GetPauseOnPersonalAction();                 // 0x005EDEF0
    int GetCombatMode();                            // 0x005EDE70
    CClientOptions* GetOptions();                   // 0x005ED700
    int ShowTutorialWindow(int n, unsigned long a, unsigned long b, int c);  // 0x005EDF40
    void SetTutorialPaused(int bPaused, int nReason);   // 0x005EDEE0
    CSWParty* GetParty();                           // 0x005ED8B0
    CSWGuiManager* GetGuiManager();                 // 0x005EDA40
    CExoString GetGUIString(unsigned long nStrRef); // 0x005EDEB0
};

class CAppManager {
public:
    void* m_p00;
    CClientExoApp* m_pClientExoApp;                 // 0x04
    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CSWGuiTextParams {
public:
    void SetText(const CExoString& sText);          // 0x00415E00
    CExoString m_sText;                             // 0x00
    char pad_0x8[0x20];
    float m_fAlpha;                                 // 0x28
    char pad_0x2c[0x2c];
};

// Border/fill parameters embedded in controls.
class CSWGuiBorderParams {
public:
    void SetFillImage(const CResRef& cResRef, int bForce);  // 0x00414C00
    void SetFlags(int b2, int b3, int b4);          // 0x00414D00
    char pad_0x0[0xc];
    float m_fAlpha;                                 // 0x0c
    Vector m_vColor;                                // 0x10
    char pad_0x1c[0x24];
    CResRef m_cFillImage;                           // 0x40
    char pad_0x50[0x10];
};

class CSWGuiControl {
public:
    virtual ~CSWGuiControl();                       // 0x00
    virtual void SetExtent(const CSWGuiExtent& extent);     // 0x04
    virtual void v08();
    virtual void HandleMouseOver(int bOver);        // 0x0c
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34();
    virtual void Draw(float fDelta);                // 0x38
    virtual void HandleInputEvent(int nEvent, int bDown);   // 0x3c
    virtual void v40();
    virtual int IsMouseOver(int nX, int nY);        // 0x44
    virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88();
    virtual void UpdateFonts();                     // 0x8c
    virtual void v90(); virtual void v94();

    void SetVisible(int bVisible) { m_bVisible = bVisible; }
    __forceinline void MoveTop(int nDelta)
    {
        CSWGuiExtent ext = m_extent;
        ext.top += nDelta;
        SetExtent(ext);
    }

    CSWGuiExtent m_extent;                          // 0x04
    char pad_0x14[0x30];
    union {
        unsigned long m_nFlags;                     // 0x44
        struct {
            unsigned long m_bActive : 1;            //      bit 0
            unsigned long m_bVisible : 1;           //      bit 1
            unsigned long m_nFlagsRest : 30;
        };
    };
    int m_n48;                                      // 0x48
    int m_bMouseClick;                              // 0x4c
    char pad_0x50[0x8];
    int m_nId;                                      // 0x58
};

// Embedded text box with its own vtable (label +0xd0).
class CSWGuiLabelText {
public:
    virtual void v00();
    virtual void SetExtent(const CSWGuiExtent& extent);     // 0x04
    virtual int GetHeight();                        // 0x08
    char pad_0x4[0x14];
};

// 0x140 bytes.
class CSWGuiLabel : public CSWGuiControl {
public:
    CSWGuiLabel();                                  // 0x0041ACD0
    ~CSWGuiLabel();                                 // 0x00419D10
    char pad_0x5c[0x14];
    CSWGuiBorderParams m_cBorder;                   // 0x70
    CSWGuiLabelText m_cTextBox;                     // 0xd0
    CSWGuiTextParams m_cText;                       // 0xe8
};

// 0x1c4 bytes.
class CSWGuiButton : public CSWGuiControl {
public:
    CSWGuiButton();                                 // 0x0041C0F0
    ~CSWGuiButton();                                // 0x00419DA0
    void Update(float fDelta);                      // 0x48 (0x00417AB0)
    char pad_0x5c[0x24];
    CSWGuiBorderParams m_cBorder;                   // 0x80
    char pad_0xe0[0x14];
    CSWGuiBorderParams m_cHilite;                   // 0xf4
    char pad_0x154[0x70];
};

// 0x150 bytes.
class CSWGuiProgressBar : public CSWGuiControl {
public:
    CSWGuiProgressBar();                            // 0x0041AED0
    ~CSWGuiProgressBar();                           // 0x0041AF70
    void SetMaxValue(int nValue);                   // 0x00417F90
    void SetCurValue(int nValue);                   // 0x00417FB0
    char pad_0x5c[0x94];
    CSWGuiBorderParams m_cProgress;                 // 0xf0
};

// One action column of the target menu (three per menu), 0x71c bytes:
// four buttons (icon frame, icon, up / down arrows) and the flash timer.
class CSWGuiMainInterfaceAction {
public:
    CSWGuiControl* HitCheckMouse(int nX, int nY);   // 0x00684DF0
    int IsActive();                                 // 0x00685840
    void Draw(float fDelta);                        // 0x00685870
    void Update(float fDelta);                      // 0x006858E0
    void SetIcon(const CResRef& cIcon, int a2, int a3, int bEnabled);  // 0x00685960

    CSWGuiMainInterfaceAction();                    // 0x00689200
    ~CSWGuiMainInterfaceAction();                   // 0x006893A0
    void SetFrame(int bShow, int bArrows);          // 0x00689280
    void StartFlash()
    {
        m_nFlash = 2;
        m_fFlashTime = 0.1f;
    }

    CSWGuiButton m_btnFrame;                        // 0x000
    CSWGuiButton m_btnIcon;                         // 0x1c4
    CSWGuiButton m_btnUp;                           // 0x388
    CSWGuiButton m_btnDown;                         // 0x54c
    float m_fFlashTime;                             // 0x710
    unsigned char m_nFlash;                         // 0x714
    int m_n718;                                     // 0x718
};

class CSWGuiTargetActionMenu;

// Target menu entry (0x38 bytes).
// Action handlers are pointers to members of a multiply-inherited client
// object class (8-byte PMF: function, this adjustment).
class CSWCActionTargetBase0 {
public:
    int m_n0;
};
class CSWCActionTargetBase1 {
public:
    int m_n0;
};
class CSWCActionTarget : public CSWCActionTargetBase0, public CSWCActionTargetBase1 {
};

#pragma pack(push, 4)
struct CSWGuiTargetAction {
    void Reset();                                   // 0x0063C000
    CExoString m_sName;                             // 0x00
    int m_nId;                                      // 0x08
    void (CSWCActionTarget::*m_pfnAction)(int nId, CSWCCreature* pPC);   // 0x0c
    char pad_0x14[0x8];
    unsigned long m_oidTarget;                      // 0x1c
    CResRef m_cIcon;                                // 0x20
    union {
        unsigned long m_nFlags;                     // 0x30
        struct {
            unsigned long m_bEnabled : 1;           //      bit 0
            unsigned long m_nDisabledReason : 4;    //      bits 1-4
            unsigned long m_nFlagsRest : 27;
        };
    };
    int m_nCount;                                   // 0x34
};
#pragma pack(pop)

struct CSWGuiTargetActionList {
    ~CSWGuiTargetActionList();                      // 0x005FBFB0
    CSWGuiTargetAction* m_pData;                    // 0x00
    int m_nCount;                                   // 0x04
    int m_nAlloc;                                   // 0x08
};

class CSWCObject;

class CSWCCreature {
public:
    void StartPersonalAction();                     // 0x0063D490
    CSWSCreature* GetServerCreature();              // 0x0060FB20
    char GetTargetInterfaceTargetType(CSWCObject* pTarget);    // 0x0060FC00
    void SetAttackTarget(int b);                    // 0x00610A10
    void GetTargetActions(CSWCObject* pTarget, int nMenu, CSWGuiTargetActionList* pList);  // 0x00619C20
    void ClearTargetActions();                      // 0x0063D470
    char pad_0x0[0x4];
    unsigned long m_idSelf;                         // 0x04
    char pad_0x8[0x438];
    unsigned char m_nCombatFlags;                   // 0x440
};

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08();
    virtual CSWCActionTarget* AsActionTarget();     // 0x0c
};

class CGuiInGame {
public:
    void SetPaused(int bPaused, int nReason);       // 0x0062DEF0
};

class CSWCObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual CExoString GetDisplayName();            // 0x70
    virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94();
    virtual short GetMaxHitPoints(int bBase);       // 0x98
    virtual short GetCurrentHitPoints(int bBase);   // 0x9c

    CSWCObject* GetClientObject();                  // 0x0063D4B0
    void GetNameInfo(CExoString* pName, int* pbSkip);   // 0x0063D300
};

// The main interface's target action menu (+0xbc), unnamed in the binary.
class CSWGuiTargetActionMenu {
public:
    ~CSWGuiTargetActionMenu();                      // 0x0068B0C0
    void UpdateTimers(float fDelta);                // 0x00684E70
    char GetControlType(CSWGuiControl* pControl);   // 0x00684ED0
    void SetBars(int bFriend, int bPoison);         // 0x006859E0
    void SetNameLabel(const CExoString& sName);     // 0x00685AF0
    void SetTarget(CSWCObject* pTarget);            // 0x00685CB0
    void Draw(float fDelta);                        // 0x00685ED0
    CSWGuiControl* HitCheckMouse(int nX, int nY);   // 0x00685F90
    void SelectNextAction(int nMenu);               // 0x006865B0
    void SelectPrevAction(int nMenu);               // 0x00686680
    void UpdateActions(CSWCCreature* pPC, CSWCObject* pTarget);    // 0x00686090
    void PopulateMenus(CSWCCreature* pPC, int bDisabled, CSWCObject* pTarget);  // 0x00689410

    CSWGuiTargetActionList m_aLists[3];             // 0x00
    int m_anSelected[4][3];                         // 0x24
    CSWGuiMainInterfaceAction m_aActions[3];        // 0x54
    int m_n15a8;                                    // 0x15a8
    CSWGuiExtent m_extent;                          // 0x15ac
    char pad_0x15bc[0x10];
    CSWGuiLabel m_lblName;                          // 0x15cc
    CSWGuiLabel m_lblNameBG;                        // 0x170c
    CSWGuiLabel m_lblBarBG;                         // 0x184c
    CSWGuiProgressBar m_pbHealth;                   // 0x198c
    float m_fNameTimer;                             // 0x1adc
    float m_fHiliteTimer;                           // 0x1ae0
    unsigned long m_nNameStrRef;                    // 0x1ae4
    unsigned char m_nBarGap;                        // 0x1ae8
    unsigned char m_nActionGap;                     // 0x1ae9
    char m_nSelectedRow;                            // 0x1aea
    char m_nSelectedMenu;                           // 0x1aeb
    union {
        unsigned long m_nMenuFlags;                 // 0x1aec
        struct {
            unsigned long m_bShown : 1;             //      bit 0
            unsigned long m_bMouseOver : 1;         //      bit 1
            unsigned long m_nMenuFlagsRest : 30;
        };
    };
};

class CTlkTable {
public:
    CExoString GetSimpleString(unsigned long nStrRef);  // 0x0041E8F0
};

extern CTlkTable* g_pTlkTable;                      // 0x007A3A08

class CSWGuiPanel {
public:
    virtual ~CSWGuiPanel();                         // 0x0040CF70
    void UpdateFonts();                             // 0x68 (0x0040A9C0)
    void Draw(float fDelta);                        // 0x34 (0x0040B760)
    void InitControl(CSWGuiControl* pControl, const CExoString& sName, int bAdd);  // 0x0040B930

    CSWGuiExtent m_extent;                          // 0x04
    int m_bUpdateFonts;                             // 0x14
    CSWGuiManager* m_pManager;                      // 0x18
    char pad_0x1c[0x28];
    unsigned long m_nPanelFlags;                    // 0x44
    int m_n48;                                      // 0x48
    float m_fAlpha;                                 // 0x4c
    Vector m_vColor;                                // 0x50
    char pad_0x5c[0x8];
};

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;
    __forceinline ~CExoArrayList()
    {
        delete element;
        element = 0;
    }
    void DelIndex(int nIndex);                      // <CSWGuiControl*>: 0x006857A0
};

// Status icon label of the main interface (b0065: Update 0x00686A20),
// 0x14c bytes. The destructor stores no vtable pointer (a tail jump to the
// label destructor), hence novtable.
class __declspec(novtable) CSWGuiMainInterfaceStatus : public CSWGuiLabel {
public:
    CSWGuiMainInterfaceStatus();                    // 0x00686AA0
    ~CSWGuiMainInterfaceStatus();                   // 0x0068AD10
    void Flash()
    {
        if (!m_bVisible) {
            m_bVisible = 1;
        }
        m_fTimeLeft = 4.0f;
        m_nStatusFlags |= 1;
    }
    void Init(CSWGuiPanel* pPanel, unsigned char nStatus);  // 0x00686920
    void Update(float fDelta);                      // 0x00686A20 (matching/src/c)
    float m_fTimeLeft;                              // 0x140
    unsigned long m_nStatusFlags;                   // 0x144
    unsigned char m_nStatus;                        // 0x148
};

class CSWGuiMainInterfaceChar;
class CSWGuiMainInterfaceStatus;

// Party portrait button with a tool tip (vtable 0x00753BD0).
class CSWGuiCharButton : public CSWGuiButton {
public:
    CSWGuiCharButton();                             // 0x00686AB0 (matching/src/c)
    void BuildToolTip();                            // 0x00684F50
    int ShowToolTip();                              // 0xa0 (0x00686750)
    void Update(float fDelta);                      // 0x48 (0x00686790)

    CSWGuiMainInterfaceChar* m_pChar;               // 0x1c4
    CExoString m_sToolTip;                          // 0x1c8
};

// One party member slot of the main interface.
class CSWGuiMainInterfaceChar {
public:
    CSWGuiMainInterfaceChar();                      // 0x00689C00
    ~CSWGuiMainInterfaceChar();                     // 0x00689CC0
    void SlideIn(float fTime);                      // 0x006851B0
    void DrawStack(float fDelta, int bBig, int bTop);   // 0x00685330
    void SetHealth(int nCurrent, int nMax, int bFriendly);  // 0x006857D0
    void Show(int bShow, unsigned char nParts, unsigned char nStackA, unsigned char nStackB); // 0x006867F0
    void DrawStacks(float fDelta, int bBig)
    {
        if (m_aLabels[5].m_bVisible) {
            if (m_nStackA > 0) {
                DrawStack(fDelta, bBig, 1);
            }
            if (m_nStackB > 0) {
                DrawStack(fDelta, bBig, 0);
            }
        }
    }
    void Hide()
    {
        m_aLabels[5].SetVisible(0);
        m_aLabels[0].SetVisible(0);
        m_aBars[0].SetVisible(0);
        m_aBars[1].SetVisible(0);
        m_aLabels[1].SetVisible(0);
        m_aLabels[2].SetVisible(0);
        m_aLabels[3].SetVisible(0);
        m_aLabels[4].SetVisible(0);
        m_nStackA = 0;
        m_nStackB = 0;
        m_btnChar.SetVisible(0);
    }

    int m_nCurHP;                                   // 0x00
    int m_nMaxHP;                                   // 0x04
    int m_nCurFP;                                   // 0x08
    int m_nMaxFP;                                   // 0x0c
    int m_nDefense;                                 // 0x10
    int m_nLevel;                                   // 0x14
    CExoString m_sName;                             // 0x18
    CExoString m_sClass;                            // 0x20
    CSWGuiLabel m_aLabels[6];                       // 0x28
    CSWGuiProgressBar m_aBars[2];                   // 0x7a8
    CSWGuiLabel m_lblStackA;                        // 0xa48
    CSWGuiLabel m_lblStackB;                        // 0xb88
    CSWGuiCharButton m_btnChar;                     // 0xcc8
    short m_nFromY;                                 // 0xe98
    short m_nFromX;                                 // 0xe9a
    short m_nToY;                                   // 0xe9c
    short m_nToX;                                   // 0xe9e
    unsigned char m_nStackA;                        // 0xea0
    unsigned char m_nStackB;                        // 0xea1
    int m_bDirty;                                   // 0xea4
};

extern int g_bClearTargetOnAction;                 // 0x008338F8
extern int g_bDrawGui;                              // 0x007A2288

// 0x1b4 bytes (matching/src/d: dtor 0x0041B610).
class CSWGuiLabelHilight : public CSWGuiLabel {
public:
    ~CSWGuiLabelHilight();                          // 0x0041B610
    char pad_0x140[0x74];
};

// 0x2b4 bytes (matching/src/d: dtor 0x0041CAD0).
class CSWGuiButtonToggle : public CSWGuiButton {
public:
    ~CSWGuiButtonToggle();                          // 0x0041CAD0
    char pad_0x1c4[0xf0];
};

class CSWGuiMainInterfaceExtra {
public:
    virtual void v00();
    virtual ~CSWGuiMainInterfaceExtra();            // 0x04
};

// The in-game main interface panel (vtable 0x00753F50, ??_G 0x0068D3B0).
class CSWGuiMainInterface : public CSWGuiPanel {
public:
    void SetMessageText(const CExoString& sText);   // 0x00685560
    void SetTargetId(int nId, int a2);              // 0x006855F0
    void ShowActionIcon(int* pCount, const CResRef& cIcon, const CResRef& cAlt,
                        int bEnabled, int bForce);  // 0x00685610
    void HideActionDescription();                   // 0x006856E0
    void ShowActionDescription();                   // 0x00685710
    void UpdateFonts();                             // 0x68 (0x00685760)
    void ShowActionQueue(int bShow);                // 0x006874C0
    void UpdateMessage();                           // 0x00686E20
    CSWParty* GetParty();                           // 0x00687FD0
    void ShowStatus(unsigned char nStatus);         // 0x00687FE0
    void RemoveControl(CSWGuiControl* pControl);    // 0x00688060
    void CancelCombat();                            // 0x006880C0
    void CheckCombatTutorial();                     // 0x00688790
    void RefreshTargetActions();                    // 0x006887D0
    void SelectNextPersonalAction(int nSlot);       // 0x00688820
    void SelectPrevPersonalAction(int nSlot);       // 0x006888E0
    void GetMapExtent(CSWGuiExtent* pExtent);       // 0x00688DB0
    void DoPersonalAction(int nSlot, int a2);       // 0x0068AD60
    void OnPersonalPrevClicked(CSWGuiControl* pControl);    // 0x0068AF70
    void OnPersonalNextClicked(CSWGuiControl* pControl);    // 0x0068AFE0
    void OnCancelCombatClicked(CSWGuiControl* pControl);    // 0x0068B050
    void OnClearTargetClicked(CSWGuiControl* pControl);     // 0x0068B0A0
    ~CSWGuiMainInterface();                         // 0x0068B170
    void Draw(float fDelta);                        // 0x34 (0x0068B4A0)
    void UpdateTarget();                            // 0x00687860
    void UpdateActions();                           // 0x00689D80
    void UpdateQueue();                             // 0x0068A010
    void UpdateParty(float fFrameTime);             // 0x0068A310
    void DrawMap(float fDelta);                     // 0x0068AB10

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
    int m_nMapLeft;                                 // 0x6080
    int m_nMapTop;                                  // 0x6084
    int m_nMapWidth;                                // 0x6088
    int m_nMapHeight;                               // 0x608c
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
    int m_n7720;                                    // 0x7720
    float m_fQueueTime;                             // 0x7724
    int m_n7728;                                    // 0x7728
    CSWGuiMainInterfaceAction m_aPersonalActions[6];    // 0x772c
    CSWGuiLabel m_lblMessage;                       // 0xa1d4
    CSWGuiLabel m_lblMessageBG;                     // 0xa314
    int m_nMessageBottom;                           // 0xa454
    unsigned char m_nQueueShift;                    // 0xa458
    union {
        unsigned long m_nInterfaceFlags;            // 0xa45c
        struct {
            unsigned long m_bInterfaceFlag0 : 1;    //      bit 0
            unsigned long m_bQueueShown : 1;        //      bit 1
            unsigned long m_nInterfaceFlagsRest : 30;
        };
    };
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
    CSWGuiMainInterfaceExtra* m_pExtra;             // 0xc8c8
};

#endif
