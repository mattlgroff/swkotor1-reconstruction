// Worker L matching headers: conversation GUI (CSWGuiDialog and its
// subclasses, the letterbox panel, CSWGuiDialogTop), 0x006A6D70-0x006A8110.
// Panel/control layouts follow matching/include/d/gui.h.
#ifndef L_SW_GUI_DIALOG_H
#define L_SW_GUI_DIALOG_H

class CExoString {
public:
    char* m_sString;
    int m_nBufferLength;
    CExoString();                                   // 0x005B3190
    CExoString(const char* sString);                // 0x005E5A90
    CExoString(int n);                              // 0x005E5BC0
    CExoString(const CExoString& sString);          // 0x005E5B00
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    CExoString& operator=(const char* s);           // 0x005E5140
    CExoString operator+(const CExoString& s) const;    // 0x005E5D10
    int IsEmpty() const;                          // 0x005E58A0
    int AsINT() const;                            // 0x005E5630
    int GetLength() const;                          // 0x005E5790
    char* CStr() const;                             // 0x005E5670
    void Format(const char* szFormat, ...);         // 0x005E5680
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

struct CSWGuiPoint {
    int x;
    int y;
};

struct CSWGuiZeroExtent : public CSWGuiExtent {
    CSWGuiZeroExtent() { left = top = width = height = 0; }
};

class CExoSoundSource {
public:
    virtual ~CExoSoundSource();
    void Play();                                    // 0x005D5930
    void Stop();                                    // 0x005D5A20
};

class CSWGuiPanel;
template <class T> class CExoArrayListL;
class CSWGuiControl;
class CResRef;

class CSWGuiManager {
public:
    char pad_0x0[0x8];
    CSWGuiControl* m_pMouseOverControl;             // 0x08
    char pad_0xc[0x54];
    float m_fFrameTime;                             // 0x60
    char pad_0x64[0x8];
    short m_nWidth;                                 // 0x6c
    short m_nHeight;                                // 0x6e

    void PlayGuiSound(char nSound);                 // 0x0040A140
    CSWGuiPanel* PopModalPanel();                    // 0x0040BE00
    void GetSafeMargins(int* pX, int* pY);          // 0x00409EE0
    static CExoString GetScreenResolutionString(int nWidth, int nHeight);  // 0x0040A3E0
    void AddPanel(CSWGuiPanel* pPanel, int nFlags, int bPlaySound);   // 0x0040BC70
    int PanelExists(CSWGuiPanel* pPanel);           // 0x0040BD70
    void RemovePanel(CSWGuiPanel* pPanel);          // 0x0040C830
};

// Text parameters embedded in labels/controls.
class CSWGuiTextParams {
public:
    void SetColor(const Vector& vColor);            // 0x00414E10
    void SetText(const CExoString& sText);          // 0x00415E00
    void SetBaseFont(const CResRef& cFont);         // 0x00415DD0
    void SetStrRef(int nStrRef);                    // 0x00415E50
    CSWGuiTextParams& operator=(const CSWGuiTextParams& p); // 0x004171E0
    CExoString m_sText;                             // 0x00
    char pad_0x8[0x30];
    unsigned long m_nFlags; // 0x38
    char pad_0x3c[0x1c];
};

class CSWGuiControl;
class CSWGuiSlider;
class CSWGuiLabel;
class CSWGuiLabelHilight;

class CSWGuiControl {
public:
    CSWGuiControl();
    virtual ~CSWGuiControl();                       // 0x00
    virtual void SetExtent(const CSWGuiExtent& extent);     // 0x04
    virtual void SetActiveControlV(CSWGuiControl* pControl, int bSound);   // 0x08 (list boxes)
    virtual void HandleMouseOver(int bOver);        // 0x0c
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38();
    virtual void HandleInputEvent(int nEvent, int bDown);   // 0x3c
    virtual void v40();
    virtual CSWGuiControl* HitCheckMouse(int x, int y); // 0x44 (0x004187C0)
    virtual void v48(); virtual void v4C();
    virtual CSWGuiLabel* GetLabel();                // 0x50
    virtual CSWGuiLabelHilight* GetLabelHilight();          // 0x54
    virtual int IsSelectable();                     // 0x58
    virtual void v5C();
    virtual void v60(); virtual CSWGuiSlider* GetSlider(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void SetEnabled(int bEnabled);  /* 0x88 */ virtual void v8C();
    virtual void v90(); virtual void v94();

    CSWGuiExtent m_extent;                          // 0x04
    CSWGuiControl* m_pParentControl;               // 0x14
    char pad_0x18[0x1c];
    void* m_pOwner;                                // 0x34
    char pad_0x38[0xc];
    union {
        unsigned long m_nFlags;                     // 0x44
        struct {
            unsigned long m_bFlag0 : 1;             //      bit 0
            unsigned long m_bVisible : 1;           //      bit 1
            unsigned long m_bNavigable : 1;         //      bit 2
            unsigned long m_nFlagsRest : 29;
        };
    };
    int m_n48;                                      // 0x48
    int m_n4c;                                      // 0x4c
    int m_n50;                                      // 0x50
    char pad_0x54[0x4];
    int m_n58;                                      // 0x58

    int GetUserData() { return m_n50; }
    void AddEvent(int nEvent, CSWGuiPanel* pObject,
                  void (CSWGuiPanel::*pfnCallback)(CSWGuiControl*));   // 0x0041AB20
    void AddChildControl(CSWGuiControl* pControl);                    // 0x0041ACC0
};

// The dialog clears its controls' visible bit through a control pointer
// (the original stores each flag word before loading the next one).
inline void HideControl(CSWGuiControl* pControl) { pControl->m_bVisible = 0; }

// Embedded text box with its own vtable (label +0xd0).
class CSWGuiLabelText {
public:
    virtual void v00();
    virtual void SetExtent(const CSWGuiExtent& extent);     // 0x04
    virtual int GetHeight();                        // 0x08
    virtual void Draw(float fDelta);                // 0x0c
    int GetLineHeight();                            // 0x00414EE0
    char pad_0x4[0x14];
};

// Border/fill parameters embedded in controls (+0x70 in a label).
class CSWGuiBorderParams {
public:
    CSWGuiBorderParams(const CResRef& cCorner, const CResRef& cEdge, const CResRef& cFill,
                       int nStyle, int nCornerSize, float fEdgeScale, const Vector& vColor,
                       int nPulse);                // 0x00415000
    CSWGuiBorderParams& operator=(const CSWGuiBorderParams& p); // 0x004150C0
    void SetBorder(class CSWGuiBorder* pBorder);
    int GetCornerSize();                            // 0x00414CD0
    void SetFlags(int b2, int b3, int b4);          // 0x00414D00
    void SetFillImage(const CResRef& cResRef, int bForce);  // 0x00414C00
    void SetCornerImage(const CResRef& cResRef, int bForce);    // 0x00414AE0
    void SetEdgeImage(const CResRef& cResRef, int bForce);  // 0x00414B70
    char pad_0x0[0xc];
    float m_fAlpha;                                 // 0x0c
    Vector m_vColor;                                // 0x10
    unsigned long m_nStyle : 2;                     // 0x1c bits 0-1
    char pad_0x20[0x40];
};

class CSWGuiBorder;

class CSWGuiLabel : public CSWGuiControl {
public:
    // The border (0x5c, a CSWGuiBorder) is declared as padding + params.
    CSWGuiBorder& GetBorder() { return *(CSWGuiBorder*)pad_0x5c; }
    CSWGuiLabel();                                  // 0x0041ACD0
    ~CSWGuiLabel();                                 // 0x00419D10
    virtual void CopyStyle(const CSWGuiExtent& extent, CSWGuiControl* pFrom, float fScale); // 0x98
    char pad_0x5c[0x14];
    CSWGuiBorderParams m_cBorder;                   // 0x70
    CSWGuiLabelText m_cTextBox;                     // 0xd0
    CSWGuiTextParams m_cText;                       // 0xe8
};

class CSWGuiListBox : public CSWGuiControl {
public:
    CSWGuiListBox();
    ~CSWGuiListBox();                               // 0x0041B060
    void ClearControls();                           // 0x00419A90
    void SetActiveControl(CSWGuiControl* pControl, int bSound);     // 0x0041C160
    void AddControls(CSWGuiControl** ppControls, int nCount, int a, int b, int c);   // 0x0041C2C0
    void AddControls(const CExoArrayListL<CSWGuiControl*>& lstControls, int bOwned, int bRestoreExtents,
                     int bUnequal);                 // 0x0041C1D0
    int IsOversized()
    {
        if (m_nPadding + m_nItemSize > m_nAreaHeight && m_nControls > 0 && !(m_nListFlags & 0x100)) {
            return 1;
        }
        return 0;
    }
    CSWGuiControl* GetSelectedControl();            // 0x0041A5D0
    CSWGuiControl* GetControl(int nIndex);          // 0x0041A5A0
    void SelectControl(int nIndex) { SetActiveControlV(GetControl(nIndex), 0); }
    char pad_0x5c[0x24];
    CSWGuiPoint m_ptLocalMouse;                     // 0x80
    char pad_0x88[0x20c];
    int m_nAreaWidth;                               // 0x294 (m_cArea.width)
    int m_nAreaHeight;                              // 0x298 (m_cArea.height)
    CSWGuiControl** m_ppControls;                   // 0x29c
    int m_nControls;                                // 0x2a0
    char pad_0x2a4[0x10];
    int m_nItemSize;                                // 0x2b4
    float m_fScale;                                 // 0x2b8
    unsigned long m_nListFlags;                     // 0x2bc
    unsigned char m_nPadding;                       // 0x2c0
    unsigned char m_n2c1;                           // 0x2c1
    short m_nPages;                                 // 0x2c2
    short m_nVisible;                               // 0x2c4
    short m_nSelected;                              // 0x2c6
    char pad_0x2c8[0x4];
    CSWGuiControl* m_pProtoItem;                    // 0x2cc
    char pad_0x2d0[0x10];
};

// vtable 0x0073e010.
class CSWGuiPanel {
public:
    CSWGuiPanel();
    CSWGuiPanel(CSWGuiManager* pManager);           // 0x0040B570
    virtual ~CSWGuiPanel();                         // 0x00 (0x0040CF70)
    virtual void SetExtent(const CSWGuiExtent& extent);     // 0x04 (0x0040A5A0)
    virtual void SetActiveControl(CSWGuiControl* pControl, int bSound);  // 0x08
    virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30();
    virtual void Draw(float fDelta);                // 0x34 (0x0040B760)
    virtual void v38();
    virtual void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x00409E60)
    virtual void v40();
    virtual CSWGuiControl* GetFocusControl();       // 0x44
    virtual void OnPanelAdded();                    // 0x48 (0x0040B870)
    virtual void OnPanelRemoved();                  // 0x4c (0x0040C170)
    virtual void v50();
    virtual void HandleInputEvent28(int n);         // 0x54 (0x0040B650)
    virtual void v58(); virtual void v5C();
    virtual void v60();
    virtual CExoString GetFullScreenBG();           // 0x64
    virtual void UpdateFonts();                     // 0x68

    CSWGuiExtent m_extent;                          // 0x04
    int m_bUpdateFonts;                             // 0x14
    CSWGuiManager* m_pManager;                      // 0x18
    CSWGuiControl* m_pControl1c;                    // 0x1c
    char pad_0x20[0x24];
    unsigned long m_nFlags;                         // 0x44
    unsigned char m_nResolution;                    // 0x48
    float m_fAlpha;                                 // 0x4c
    Vector m_vColor;                                // 0x50
    void* m_pBorder;                                // 0x5c
    unsigned char m_nOpenSound;                     // 0x60

    void StartLoadFromLayout(const CResRef& cResRef);   // 0x0040A680
    void InitControl(CSWGuiControl* pControl, const CExoString& sName, int bAdd);  // 0x0040B930
    void UnloadResource();                          // 0x0040B8F0
    void SetToDestroy(int bDestroy);                // 0x00624A00
    void CenterPanel(); // 0x0040A600
    void SetVisible(int bVisible);                   // 0x0040B9D0
    void SetCloseCallback(CSWGuiPanel* pObject, void (CSWGuiPanel::*pfnCallback)(CSWGuiControl*),
                          int nArg);                // 0x00624A40

    // throw(): the letterbox ctor keeps EH state 0 across this call.
    void SetAlpha(float fAlpha) throw();            // 0x00404B00
};

class CSWGuiLetterbox;
class CSWGuiInGamePartySelection;
class CSWGuiInGameMap;
class CSWGuiMessageBox;

class CGuiInGame {
public:
    char pad_0x0[0x60];
    CSWGuiLetterbox* m_pLetterbox;                  // 0x60
    CSWGuiLetterbox* m_pLetterboxBottom;            // 0x64
    char pad_0x68[0x28];
    CSWGuiInGamePartySelection* m_pPartySelection;  // 0x90
    void* m_p94;                                    // 0x94
    CSWGuiMessageBox* m_pMessageBox;                // 0x98
    char pad_0x9c[0xb14];
    int m_bMessagePending;                          // 0xbb0
    char pad_0xbb4[0xc];
    unsigned char m_nAbilityMode;                   // 0xbc0

    void InitializeMap(); // 0x0062B0A0
    void HideLetterbox();                           // 0x0062AB90
    int IsDialogForced();                           // 0x0062ABD0
    int HideSWInGameGui(int bHide);                 // 0x0062CBA0
    void SelectReply(int nReply, int nReplies, int bSound);    // 0x0062ADA0
    void UpdateDialog();                            // 0x006339C0
    int IsMinimapHidden();                          // 0x0062F910
    unsigned char GetCombatBarHeight();             // 0x0062F8F0
    CSWGuiInGameMap* GetMapPanel(int nPanel, int b);    // 0x0062B060
};

class CClientOptions {
public:
    void SetControlDefaults(); // 0x0061D500
    void SetShadows(int bEnabled); // 0x0061D6B0
    void SetSoftShadows(int bEnabled); // 0x0061D6D0
    void SetGrass(int bEnabled); // 0x0061D6F0
    void SetLargeFont(int bLarge); // 0x0061D910
    void SetAutoLevelUpNPCs(int bAuto); // 0x0061B740
    void SetGameDefaults(); // 0x0061D4E0
    void SetGraphicsDefaults(); // 0x0061D520 (advanced graphics)
    void SetSoundDefaults(); // 0x0061D540
    void SetSoundQualityDefault(); // 0x0061D550
    void SetFeedbackDefaults(); // 0x0061D950
    void SetDisplayDefaults(); // 0x0061D9A0
    void SetBrightnessDefaults(); // 0x0061D9D0 (mouse)

    char pad_0x0[4];
    unsigned char m_nDifficulty;
    char pad_0x5[3];
    union {
        unsigned long m_nGameplayFlags;
        struct {
            unsigned long m_bGame0 : 1;
            unsigned long m_bGame1 : 1;
            unsigned long m_bGame2 : 1;
            unsigned long m_bGame3 : 1;
            unsigned long m_bGame4 : 1;
            unsigned long m_bGameRemaining : 27;
        };
    };
    char pad_0xc[8];
    union {
        unsigned long m_nFlags;
        struct {
            unsigned long m_bFeedback0 : 1;
            unsigned long m_bFeedback1 : 1;
            unsigned long m_bFeedback2 : 1;
            unsigned long m_bFeedback3 : 1;
            unsigned long m_bFeedback4 : 1;
            unsigned long m_bFeedback5 : 1;
            unsigned long m_bFeedback6 : 1;
            unsigned long m_bFeedback7 : 1;
            unsigned long m_bFeedback8 : 1;
            unsigned long m_bFeedback9 : 1;
            unsigned long m_bFeedback10 : 1;
            unsigned long m_bFeedbackRemaining : 21;
        };
    };                         // 0x14
    char pad_0x18[0xc];
    int m_bShadows; // 0x24
    char pad_0x28[4];
    int m_bGrass; // 0x2c
    char pad_0x30[0x20];
    unsigned char m_nMouseSensitivity; // 0x50
};

class CClientExoAppInternal;
class CWorldTimer;
class CGameObject;
class CSWCCreature;
class CSWCDoor;
class CSWCPlaceable;
class CSWParty;
class CSWSClient;
class CSWSCreature;

class CClientExoApp {
public:
    void* m_p00;
    CClientExoAppInternal* m_pInternal;             // 0x04
    inline CWorldTimer* GetWorldTimer();

    CGameObject* GetGameObject(unsigned long oid);  // 0x005ED580
    CSWCCreature* GetCreatureByGameObjectID(unsigned long oid);    // 0x005ED5B0
    CSWCPlaceable* GetPlaceableByGameObjectID(unsigned long oid);  // 0x005ED5F0
    CSWCDoor* GetDoorByGameObjectID(unsigned long oid);            // 0x005ED600
    CSWParty* GetParty();                           // 0x005ED8B0
    unsigned long ServerToClientObjectId(unsigned long oid);   // 0x005EDA50
    int GetPausedByCombat();                        // 0x005EDC10
    int SetPausedByCombat(int bPaused, unsigned char nFlag, int bForce);  // 0x005EDC20
    int GetCombatMode();                            // 0x005EDE70
    void PlayerFlourishWeapons();                   // 0x005EDE90
    CGuiInGame* GetInGameGui();                     // 0x005ED690
    CClientOptions* GetOptions();                   // 0x005ED700
    CSWGuiManager* GetGuiManager();                 // 0x005EDA40
    void SetInputClass(int nClass, int bForce);     // 0x005EDA60
    void SetUnknown1a8(int a, int b, int c);        // 0x005EDE10
    void ShowTutorialWindow(int, int, int, int); // 0x005EDF40
    void SaveOptions();                             // 0x005EDF70
    CSWCCreature* GetPlayerCreature();              // 0x005ED540
    int ChangeCharacterToNextLivingPartyMember(int a, int b, int c);  // 0x005EDF80
};

class CServerInfo {
public:
    char pad_0x0[0x104];
    unsigned long m_bPartyMode : 1;                 // 0x104 bit 0
};

class CSWPartyTable {
public:
    unsigned long GetNPCObject(int nNPC, int bCreate, int bRevive);    // 0x00564700
};

class CServerExoApp {
public:
    unsigned long ClientToServerObjectID(unsigned long oid);    // 0x004AEA30
    void SkipPastDialog(unsigned long oid);         // 0x004AE970
    CSWSClient* GetClientObjectByObjectID(unsigned long oid);  // 0x004AE910
    unsigned long GetPlayerCreatureID();            // 0x004AEA40
    CSWSCreature* GetCreatureByGameObjectID(unsigned long oid);    // 0x004AE770
    void SetMoveToModulePending(int bPending);      // 0x004AECC0
    void SetMoveToModuleString(const CExoString& sModule);  // 0x004AECD0
    CServerInfo* GetServerInfo();                   // 0x004AEC90
    CSWPartyTable* GetPartyTable();                 // 0x004AEE70
};

class CAppManager {
public:
    void* m_p00;
    CClientExoApp* m_pClientExoApp;                 // 0x04
    CServerExoApp* m_pServerExoApp;                 // 0x08
    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
    CServerExoApp* GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC
extern int g_nScreenWidth;                          // 0x0078D1D4
extern int g_nScreenHeight;                         // 0x0078D1D8
extern CExoString g_sComputerBGPrefix;              // 0x007A3D70
extern Vector g_vDefaultReplyColor;                 // 0x007A23C0

// Conversation panel; implicit destructor 0x006A6F30.
class CSWGuiDialog : public CSWGuiPanel {
public:
    virtual int SelectReply(int nReply);            // 0x6c (0x006A7110)
    virtual void ClearReplies();                    // 0x70 (0x006A7160)
    virtual void AddReply(unsigned long a1, int a2, int a3, int a4, int bLast);  // 0x74 (0x006A86A0)
    virtual void SetSpeaker(unsigned long oid);     // 0x78 (0x006A7C90)
    virtual void SetDialogMessage(const CExoString& sMessage);  // 0x7c (0x006A7010)
    virtual void ShowMessage();                     // 0x80 (0x006A70A0)
    virtual void SelectCurrentReply(int nUnused);   // 0x84 (0x006A7390)

    void HandleInputEvent(int nEvent, int bDown);   // 0x006A7230
    void SetForced();                               // 0x006A6D70
    int GetReplyIndex(int nReply);                  // 0x006A6D80
    void SetReplyColor(CSWGuiControl* pControl);    // 0x006A6FE0
    static void __stdcall ResetReplyColor(CSWGuiControl* pControl);   // 0x006A6FC0

    int m_n64;                                      // 0x64
    int m_nSelected;                                // 0x68
    int m_nReplies;                                 // 0x6c
    CSWGuiLabel m_aReplies[20];                     // 0x70
    int m_aReplyIndex[20];                          // 0x1970
    unsigned long m_oidSpeaker;                     // 0x19c0
    CSWGuiListBox m_lbReplies;                      // 0x19c4
    CSWGuiLabel m_lblMessage;                       // 0x1ca4
    Vector m_vReplyColor;                           // 0x1de4
    CExoString m_sMessage;                          // 0x1df0
    unsigned long m_bMessageShown : 1;              // 0x1df8 bit 0
    unsigned long m_bMessageHidden : 1;             //        bit 1
    unsigned long m_bForced : 1;                    //        bit 2
};

class CDialogCamera {
public:
    virtual ~CDialogCamera();
};

// Three labels (no vtable); ctor 0x006A8050, dtor 0x006A80B0.
class CSWGuiReplyEntry {
public:
    CSWGuiReplyEntry();
    ~CSWGuiReplyEntry();

    CSWGuiLabel m_lblA;                             // 0x000
    CSWGuiLabel m_lblB;                             // 0x140
    CSWGuiLabel m_lblC;                             // 0x280
};

// vtable 0x00755800.
class CSWGuiDialogTop;

class CSWGuiDialogCinematic : public CSWGuiDialog {
public:
    ~CSWGuiDialogCinematic();                       // 0x006A7C20
    void SetExtent(const CSWGuiExtent& extent);     // 0x006A7CA0
    void ShowMessage();                             // 0x006A7D30
    int SelectReply(int nReply);                    // 0x006A7E20
    void ClearReplies();                            // 0x006A7EF0
    void AddReply(unsigned long a1, int a2, int a3, int a4, int bLast);   // 0x006A8DE0

    CSWGuiDialogTop* m_pTop;                        // 0x1dfc
};

// vtable 0x00755888.
class CSWGuiDialogComputer : public CSWGuiDialog {
public:
    void HandleInputEvent(int nEvent, int bDown);   // 0x006A81E0
    void OnPanelAdded();                            // 0x006A6DE0
    void OnPanelRemoved();                          // 0x006A6E00
    CExoString GetFullScreenBG();                   // 0x006A6E20

    ~CSWGuiDialogComputer();                        // 0x006A8110
    void AddReply(unsigned long a1, int a2, int a3, int a4, int bLast);   // 0x006A94E0
    void SetDialogMessage(const CExoString& sMessage);  // 0x006A9520
    void LayoutEntries();                           // 0x006A8240

    CSWGuiReplyEntry m_aEntries[4];                 // 0x1dfc
    CSWGuiListBox m_lbEntries;                      // 0x2cfc
    CSWGuiLabel m_aLabels[4];                       // 0x2fdc
    CSWGuiLabel m_lblTitle;                         // 0x34dc
    CExoSoundSource* m_pSound;                      // 0x361c
};

// vtable 0x00755958.
class CSWGuiDialogComputerCamera : public CSWGuiDialog {
public:
    ~CSWGuiDialogComputerCamera();                  // 0x006A8520

    CSWGuiLabel m_lblA;                             // 0x1dfc
    CSWGuiLabel m_lblB;                             // 0x1f3c
    CDialogCamera* m_pCamera;                       // 0x207c
};

extern Vector g_vBlack;                             // 0x00833910

// Cinematic letterbox bars; vtable 0x00755A78; implicit dtor 0x006A8A10.
class CSWGuiLetterbox : public CSWGuiPanel {
public:
    CSWGuiLetterbox(CSWGuiManager* pManager);       // 0x006A8930
    void SetFullScreen();                           // 0x006A8A60
    void SetExtent(const CSWGuiExtent& extent);     // 0x006A7440
    void Draw(float fDelta);                        // 0x006A7680

    int IsFadeDone();                               // 0x006A6DA0
    void SetBarExtent(CSWGuiExtent extent);         // 0x006A73B0
    void ShowTop();                                 // 0x006A74B0
    void ShowBottom();                              // 0x006A7540
    void ResetFade();                               // 0x006A75E0
    void StartFade(int bFadeIn, float fStart, float fDuration, const Vector& vColor);  // 0x006A7620

    CSWGuiZeroExtent m_barExtent;                   // 0x64
    CSWGuiZeroExtent m_targetExtent;                // 0x74
    int m_nMode;                                    // 0x84
    int m_bDone;                                    // 0x88
    CExoString m_s8C;                               // 0x8c
    float m_fStart;                                 // 0x94
    float m_fDuration;                              // 0x98
    float m_fTime;                                  // 0x9c
    int m_bFadeIn;                                  // 0xa0
};

class CSWGuiDialogTop : public CSWGuiPanel {
public:
    void SetReply(const CExoString& sReply);        // 0x006A7AE0

    CSWGuiLabel m_lblReply;                         // 0x64
};

#endif
