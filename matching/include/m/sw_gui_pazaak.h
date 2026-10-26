// Worker M matching headers: the Pazaak GUI (0x0067CA50-0x0067FFA0).
//   CSWGuiActivatedButton (0x1D4, vtable 0x00753120), CSWGuiPazaakCard
//   (0x31C, vtable 0x007531C0), CSWGuiTutorial (0x99C, CSWGuiMessageBox base,
//   vtable 0x00753260), CSWGuiWagerPopup (0xCAC, vtable 0x007534C8),
//   CSWGuiPazaakStart (0x8928, vtable 0x007532E8), CSWGuiPazaakGame (0x86E8,
//   vtable 0x00753358) and the rules object CSWPazaak (0x230).
// Control layouts follow matching/include/d/gui_control.h, message box layout
// matching/include/l/sw_gui_tutorial_box.h (copied, trimmed).
#ifndef M_SW_GUI_PAZAAK_H
#define M_SW_GUI_PAZAAK_H

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
    char* CStr() const;                             // 0x005E5670
};

struct CResStruct {
    unsigned long m_nStructIndex;
};

class CResGFF;

class CResRef {
public:
    CResRef();                                      // 0x00405ED0
    CResRef(const char* s);                         // 0x00406D80
    CResRef operator=(const char* s);               // 0x00406290
    char* GetResRef() const;                        // 0x00641DB0
    char m_resRef[16];
};

void* operator new(unsigned int nSize);
void operator delete(void* p);
void* operator new[](unsigned int nSize);
void operator delete[](void* p);

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList(int nSize = 0)
    {
        num = 0;
        array_size = nSize;
        element = 0;
        if (array_size) {
            element = new T[array_size];
        }
    }
    ~CExoArrayList()
    {
        if (element) {
            delete[] element;
        }
    }
    void Allocate(int nSize);
    void Add(T t)
    {
        if (num == array_size) {
            Allocate(array_size ? array_size * 2 : 16);
        }
        element[num++] = t;
    }
};

// The wager popup's (unused) list frees its storage unconditionally.
template <>
inline CExoArrayList<int>::~CExoArrayList()
{
    delete[] element;
    element = 0;
}

struct CSWGuiExtent {
    int left;
    int top;
    int width;
    int height;
};

class Vector {
public:
    Vector() {}
    Vector(float fX, float fY, float fZ)
    {
        x = fX;
        y = fY;
        z = fZ;
    }
    float x, y, z;
    Vector operator*(float f) const { return Vector(x * f, y * f, z * f); }
    Vector operator+(const Vector& o) const { return Vector(x + o.x, y + o.y, z + o.z); }
};

class CSWGuiControl;
class CSWGuiPanel;

// vtable 0x0073df20 (slots 0x00-0x30).
class CSWGuiObject {
public:
    virtual ~CSWGuiObject();                                    // 0x00
    virtual void SetExtent(const CSWGuiExtent& extent);         // 0x04
    virtual void SetActiveControl(CSWGuiControl* pControl, int bSound); // 0x08
    virtual void HandleMouseOver(int n);                        // 0x0c
    virtual int HandleMouseCapturedMovement(int x, int y);      // 0x10
    virtual void OnCaptureLost();                               // 0x14
    virtual void HandleLMouseDown();                            // 0x18
    virtual void HandleLMouseUp();                              // 0x1c
    virtual void v20();
    virtual void HandleRMouseUp();                              // 0x24
    virtual CSWGuiPanel* GetPanel();                            // 0x28
    virtual void* GetListBox();                                 // 0x2c
    virtual void v30();
    CSWGuiExtent m_extent;                          // 0x04
};

class CSWGuiManager {
public:
    int m_nMouseX;                                  // 0x00
    int m_nMouseY;                                  // 0x04
    CSWGuiControl* m_pMouseOverControl;             // 0x08
    int m_nCaptureType;                             // 0x0c
    CSWGuiControl* m_pCaptureControl;               // 0x10
    char pad_0x14[0x40];
    CSWGuiControl* m_pDragControl;                  // 0x54

    void PlayGuiSound(char nSound);                 // 0x0040A140
    void ReleaseCapture(CSWGuiControl* pControl);   // 0x0040A200
    void AddPanel(CSWGuiPanel* pPanel, int nFlags, int bPlaySound);   // 0x0040BC70
    int PanelExists(CSWGuiPanel* pPanel);           // 0x0040BD70
    CSWGuiPanel* PopModalPanel();                   // 0x0040BE00
    int RemovePanel(CSWGuiPanel* pPanel);           // 0x0040C830
};

extern CSWGuiManager* g_pGuiManager;                // 0x007A39F4

class CSWGuiTextParams {
public:
    void SetColor(const Vector& vColor);            // 0x00414E10
    void SetText(const CExoString& sText);          // 0x00415E00
    void SetStrRef(int nStrRef);                    // 0x00415E50
    CExoString m_sText;                             // 0x00
    char pad_0x08[0x4c];
};

class CSWGuiBorderParams {
public:
    void SetFillImage(const CResRef& cResRef, int bForce);  // 0x00414C00
    void SetFlags(int b2, int b3, int b4);          // 0x00414D00
    CResRef GetFillImage() { return m_cFill; }
    char pad_0x00[0x10];
    Vector m_vColor;                                // 0x10
    char pad_0x1c[0x24];
    CResRef m_cFill;                                // 0x40
    char pad_0x50[0x10];
};

class CSWGuiBorder {
public:
    virtual ~CSWGuiBorder();
    virtual void SetExtent(const CSWGuiExtent& extent); // 0x04
    virtual void v08();
    virtual void Draw(float fDelta);                // 0x0c
    virtual void Load(CResGFF* pGFF, CResStruct* pStruct, const CExoString& sName); // 0x10
    CSWGuiExtent m_extent;                          // 0x04
    CSWGuiBorderParams m_cParams;                   // 0x14
};

class CSWGuiText {
public:
    virtual ~CSWGuiText();
    virtual void SetExtent(const CSWGuiExtent& extent); // 0x04
    virtual void v08();
    virtual void Draw(float fDelta);                // 0x0c
    virtual void Load(CResGFF* pGFF, CResStruct* pStruct, const CExoString& sName); // 0x10
    char pad_0x04[0x14];
    CSWGuiTextParams m_cParams;                     // 0x18
    int m_n6c;                                      // 0x6c
};

// vtable 0x0073e488 (slots 0x00-0x94).
class CSWGuiControl : public CSWGuiObject {
public:
    virtual ~CSWGuiControl();                                   // 0x00
    virtual void HandleLMouseDown();                            // 0x18 (0x0041AB90)
    virtual void HandleLMouseUp();                              // 0x1c (0x0041ABD0)
    virtual int GetHeight();                                    // 0x34
    virtual void Draw(float fDelta);                            // 0x38
    virtual void HandleInputEvent(int nEvent, int bDown);       // 0x3c
    virtual void SetActive(int b);                              // 0x40
    virtual CSWGuiControl* HitCheckMouse(int x, int y);         // 0x44
    virtual void Load(CResGFF* pGFF, CResStruct* pStruct);      // 0x48
    virtual void* GetNavigation();                              // 0x4c
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual int v60(); virtual void v64(); virtual void v68();
    virtual void* v6c();
    virtual void v70(); virtual void v74(); virtual void v78();
    virtual void HandleFocusChange(int bFocus);                 // 0x7c
    virtual int GetIsSelectable();                              // 0x80
    virtual void HandleRMouseDown();                            // 0x84 (0x0041AC30)
    virtual void SetEnabled(int b);                             // 0x88
    virtual void UpdateFont();                                  // 0x8c
    virtual int DisplayToolTip();                               // 0x90
    virtual void DrawDragDrop(int x, int y);                    // 0x94

    CSWGuiControl* m_pParentControl;                // 0x14
    CExoArrayList<CSWGuiControl*> m_lstChildren;    // 0x18
    int m_nToolTipStrRef;                           // 0x24
    CExoString m_sToolTip;                          // 0x28
    int m_nKeyBinding;                              // 0x30
    CSWGuiObject* m_pOwner;                         // 0x34
    char m_lstEvents[0x0c];                         // 0x38
    union {
        unsigned long m_nFlags;                     // 0x44
        struct {
            unsigned long m_bFlag0 : 1;             //      bit 0
            unsigned long m_bFlag1 : 1;             //      bit 1
            unsigned long m_bNavigable : 1;         //      bit 2
            unsigned long m_bSelectable : 1;        //      bit 3
            unsigned long m_bFlag4 : 1;             //      bit 4
            unsigned long m_bFlag5 : 1;             //      bit 5
            unsigned long m_nFlagsRest : 26;
        };
    };
    int m_n48;                                      // 0x48
    int m_n4c;                                      // 0x4c
    int m_nId;                                      // 0x50
    unsigned char m_nSound;                         // 0x54
    unsigned char m_nClickSound;                    // 0x55
    int m_n58;                                      // 0x58

    void SetFlag5(int b) { m_bFlag5 = b; }
    void SetNavigable(int b) { m_bNavigable = b; }
    void AddEvent(int nEvent, CSWGuiPanel* pObject,
                  void (CSWGuiPanel::*pfnCallback)(CSWGuiControl*));   // 0x0041AB20
};

// vtable 0x0073e520.
class CSWGuiNavigable : public CSWGuiControl {
public:
    virtual void HandleInputEvent(int nEvent, int bDown);       // 0x3c (0x0041A9D0)
    void SetNavigation(int nDir, CSWGuiControl* pControl);      // 0x00417660
    int m_aNavigation[4];                           // 0x5c
};

// vtable 0x0073e5b8.
class CSWGuiLabel : public CSWGuiControl {
public:
    CSWGuiLabel();                                  // 0x0041ACD0
    virtual ~CSWGuiLabel();                         // 0x00419D10
    virtual void CopyStyle(const CSWGuiExtent& extent, CSWGuiLabel* pFrom, float fScale); // 0x98
    virtual void SetStyle(const CSWGuiExtent& extent, CSWGuiTextParams* pText, CSWGuiBorderParams* pBorder,
                          float fScale);                        // 0x9c
    CSWGuiBorder m_cBorder;                         // 0x5c
    CSWGuiText m_cText;                             // 0xd0
};

// vtable 0x0073e658.
class CSWGuiButton : public CSWGuiNavigable {
public:
    CSWGuiButton();                                 // 0x0041C0F0
    virtual ~CSWGuiButton();                        // 0x00419DA0
    virtual void HandleInputEvent(int nEvent, int bDown);       // 0x3c (0x0041AD40)
    virtual void Draw(float fDelta);                            // 0x38 (0x00417AB0)
    virtual void Load(CResGFF* pGFF, CResStruct* pStruct);      // 0x48 (0x0041C930)
    virtual void SetEnabled(int b);                             // 0x88 (0x00418DB0)
    virtual void CopyStyle(const CSWGuiExtent& extent, CSWGuiButton* pFrom); // 0x98
    virtual void SetStyle(const CSWGuiExtent& extent, CSWGuiTextParams* pText, CSWGuiBorderParams* pBorder,
                          CSWGuiBorderParams* pHilight);        // 0x9c
    CSWGuiBorder m_cBorder;                         // 0x6c
    CSWGuiBorder m_cHilight;                        // 0xe0
    CSWGuiText m_cText;                             // 0x154
};

// A Pazaak card {value, flag}; the constructor only clears the flag.
struct CPazaakCard {
    CPazaakCard() { m_bFlag = 0; }
    CPazaakCard(int nValue, int bFlag)
    {
        m_nValue = nValue;
        m_bFlag = bFlag;
    }
    int operator!=(CPazaakCard cCard) { return m_nValue != cCard.m_nValue || m_bFlag != cCard.m_bFlag; }

    int m_nValue;
    int m_bFlag;
};

// 0x70 bytes, no vtable (ctor 0x006E4B50).
class CPazaakPlayer {
public:
    CPazaakPlayer();                                // 0x006E4B50
    int GetTotal();                                 // 0x006E4360
    void AddGameCard(CPazaakCard cCard);            // 0x006E45F0
    CPazaakCard GetBoardCard(int i) { return m_aBoard[i]; }
    CPazaakCard GetHandCard(int i) { return m_aHand[i]; }
    CPazaakCard m_aHand[4];                         // 0x00
    CPazaakCard m_aBoard[9];                        // 0x20
    int m_bStanding;                                // 0x68
    int m_nSetsWon;                                 // 0x6c
};

// 0x230 bytes, no vtable (ctor 0x006E4C00).
class CSWPazaak {
public:
    CSWPazaak();                                    // 0x006E4C00
    void ClearGameBoard();                          // 0x006E4640
    CPazaakCard DrawCard();                         // 0x006E4800
    int CanUsePlayerSidedeckCard(int nIndex);       // 0x006E4840
    int UsePlayerSidedeckCard(int nIndex);          // 0x006E4860
    int PlayAITurn(CPazaakPlayer* pAI, CPazaakPlayer* pPlayer, int nState);   // 0x006E4C60
    CPazaakPlayer* GetPlayer() { return &m_cPlayer; }
    void DrawPlayerCard();                          // 0x0067CAE0
    int DoAITurn();                                 // 0x0067CB10

    int m_nWager;                                   // 0x000
    unsigned long m_oidOpponent;                    // 0x004
    CPazaakPlayer m_cPlayer;                        // 0x008
    CPazaakPlayer m_cAI;                            // 0x078
    CPazaakCard m_aDeck[40];                        // 0x0e8
    int m_nDeckIndex;                               // 0x228
    int m_nAIState;                                 // 0x22c
};

extern const char* g_szPazaakCardText[28];          // 0x007A2600

extern Vector g_vPazaakButtonColor;                 // 0x007A2408
extern Vector g_vPazaakButtonHilight;               // 0x007A23B4

// vtable 0x00753120; no constructor of its own (the owners inline it).
class CSWGuiActivatedButton : public CSWGuiButton {
public:
    virtual void Draw(float fDelta);                            // 0x38 (0x0067CB70)
    virtual void SetEnabled(int b);                             // 0x88 (0x0067CC90)
    void SetColor(const Vector& vColor);            // 0x0067CA50
    void Flash(const Vector& vColor)
    {
        m_fTimer = 0.8f;
        m_vColor = vColor;
    }
    float m_fTimer;                                 // 0x1c4
    Vector m_vColor;                                // 0x1c8
};

// vtable 0x007531C0.
class CSWGuiPazaakCard : public CSWGuiButton {
public:
    CSWGuiPazaakCard();                             // 0x0067DDD0
    virtual ~CSWGuiPazaakCard();                    // 0x0067DE80
    virtual int HandleMouseCapturedMovement(int x, int y);      // 0x10 (0x0067D060)
    virtual void HandleLMouseDown();                            // 0x18 (0x0067D040)
    virtual void HandleLMouseUp();                              // 0x1c (0x0067D0D0)
    virtual void Draw(float fDelta);                            // 0x38 (0x0067CA80)
    virtual void HandleInputEvent(int nEvent, int bDown);       // 0x3c
    virtual void Load(CResGFF* pGFF, CResStruct* pStruct);      // 0x48 (0x0067CD00)
    virtual int GetIsSelectable();                              // 0x80 (0x0067DE60)
    virtual void DrawDragDrop(int x, int y);                    // 0x94 (0x0067CF00)
    void SetCard(CPazaakCard cCard);                // 0x0067CD30
    CPazaakCard GetCard() { return m_cCard; }

    CSWGuiLabel m_lblFace;                          // 0x1c4
    CPazaakCard m_cCard;                            // 0x304
    int m_nDownX;                                   // 0x30c
    int m_nDownY;                                   // 0x310
    int m_bDraggable;                               // 0x314
    int m_bCanSelect;                               // 0x318
};

// vtable 0x0073e010.
class CSWGuiPanel : public CSWGuiObject {
public:
    CSWGuiPanel(CSWGuiManager* pManager);           // 0x0040B570
    virtual ~CSWGuiPanel();                         // 0x0040CF70
    virtual void Draw(float fDelta);                // 0x34
    virtual void v38();
    virtual void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x00409E60)
    virtual void v40();
    virtual CSWGuiControl* GetFocusControl();       // 0x44
    virtual void OnPanelAdded();                    // 0x48 (0x0040B870)
    virtual void OnPanelRemoved();                  // 0x4c
    virtual void v50();
    virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60();
    virtual CExoString GetFullScreenBG();           // 0x64
    virtual void UpdateFonts();                     // 0x68

    void GetAbsoluteExtent(CSWGuiExtent* pExtent);  // 0x0040AA00
    void StartLoadFromLayout(const CResRef& cResRef);   // 0x0040A680
    void InitControl(CSWGuiControl* pControl, const CExoString& sName, int bAdd);  // 0x0040B930
    void UnloadResource();                          // 0x0040B8F0
    void OnBButtonPressed(CSWGuiControl* pControl); // 0x0040B650
    void SetCloseCallback(CSWGuiPanel* pObject, void (CSWGuiPanel::*pfnCallback)(CSWGuiControl*),
                          int nArg);                // 0x00624A40
    void MarkForDelete() { m_nFlags = (m_nFlags & ~0x300) | 0x400; }

    int m_bUpdateFonts;                             // 0x14
    CSWGuiManager* m_pManager;                      // 0x18
    CSWGuiControl* m_pActiveControl;                // 0x1c
    char pad_0x20[0x24];
    unsigned long m_nFlags;                         // 0x44
    unsigned char m_nResolution;                    // 0x48
    float m_fAlpha;                                 // 0x4c
    Vector m_vColor;                                // 0x50
    void* m_pBorder;                                // 0x5c
    unsigned char m_nOpenSound;                     // 0x60
};

// vtable 0x00754A38 family; ctor 0x00626DF0, dtor 0x006270A0.
class CSWGuiMessageBox : public CSWGuiPanel {
public:
    CSWGuiMessageBox(CSWGuiManager* pManager);      // 0x00626DF0
    ~CSWGuiMessageBox();                            // 0x006270A0
    void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006250F0)
    virtual void SetMessageStrRef(unsigned long nStrRef);   // 0x6c
    virtual void SetMessage(CExoString sMessage);   // 0x70
    virtual void SetOkStrRef(unsigned long nStrRef);        // 0x74
    virtual void SetCancelStrRef(unsigned long nStrRef);    // 0x78
    virtual void v7c();
    virtual void v80();

    void SetAllowCancel(int bCancel);               // 0x00627130

    unsigned long m_bAccepted : 1;                  // 0x64 bit 0
    char pad_0x68[0xc];
    CSWGuiLabel m_lblMessage;                       // 0x74
    char pad_0x1b4[0x140];
    CSWGuiButton m_btnOk;                           // 0x2f4
    CSWGuiButton m_btnCancel;                       // 0x4b8
    char pad_0x67c[0x308];
};

// vtable 0x00753260.
class CSWGuiTutorial : public CSWGuiMessageBox {
public:
    CSWGuiTutorial(CSWGuiManager* pManager);        // 0x0067DEF0
    void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x0067D260)
    void PlaceRightOf(CSWGuiControl* pControl);     // 0x0067D110
    void PlaceAbove(CSWGuiControl* pControl);       // 0x0067D1C0
    void ShowHelp(unsigned int nMessage, int nButtonA, unsigned int nButtonB);  // 0x0067DFC0
    void ShowMultipleHelp(const unsigned int* pMessages, int nCount);  // 0x0067E0B0

    CExoArrayList<unsigned int> m_lstMessages;      // 0x984
    int m_nMessage;                                 // 0x990
    int m_bEnabled;                                 // 0x994
    CSWGuiControl* m_pAnchor;                       // 0x998
};

// vtable 0x007533C8; inline constructor only (auto-repeat button).
class CSWGuiSpeedButton : public CSWGuiButton {
public:
    CSWGuiSpeedButton()
    {
        m_fRepeatDelay = 0.2f;
        m_fInitialDelay = 0.5f;
    }
    virtual void HandleLMouseDown();                            // 0x18 (0x0067E800)
    virtual void Draw(float fDelta);                            // 0x38 (0x0067E860)
    virtual void HandleRMouseDown();                            // 0x84 (0x0067E830)
    float m_fRepeatDelay;                           // 0x1c4
    float m_fInitialDelay;                          // 0x1c8
    float m_fTimer;                                 // 0x1cc
    unsigned long m_bLeftButton : 1;                // 0x1d0
};

class CSWParty;

class CGuiInGame {
public:
    char pad_0x0[0x98];
    CSWGuiMessageBox* m_pMessageBox;                // 0x98
};


class CClientExoApp {
public:
    CGuiInGame* GetInGameGui();                     // 0x005ED690
    CSWParty* GetParty();                           // 0x005ED8B0
    void EndPazaakGame(int bWon, int nWager, int bRestart, unsigned long oidOpponent);   // 0x005EDDD0
};

class CSWCCreature {
public:
    char pad_0x0[0x4];
    unsigned long m_idSelf;                         // 0x04
};

class CSWParty {
public:
    CSWCCreature* GetPC();                          // 0x00635460
};

class CSWSCreatureStats {
public:
    CExoString GetFullName();                       // 0x005A4C60
};

class CSWSCreature {
public:
    int GetGold();                                  // 0x004EDD60
    char pad_0x0[0xa74];
    CSWSCreatureStats* m_pStats;                    // 0xa74
    CSWSCreatureStats* GetStats() { return m_pStats; }
};

class CExoSound {
public:
    int PlayOneShotSound(const CResRef& cResRef, unsigned char nGroup,
                         unsigned long nDelay, unsigned char nVolume,
                         float fFixedVariance, float fPitchVariance);  // 0x005D5E00
};

extern CExoSound* g_pExoSound;                      // 0x007A39EC

class CSWPartyTable {
public:
    char pad_0x0[0x120];
    int m_aPazaakCards[18];                         // 0x120
    int m_aPazaakSideList[10];                      // 0x168
};

class CServerExoApp {
public:
    CSWPartyTable* GetPartyTable();                 // 0x004AEE70
    unsigned long ClientToServerObjectID(unsigned long oidClient);  // 0x004AEA30
    CSWSCreature* GetCreatureByGameObjectID(unsigned long oid);     // 0x004AE770
    unsigned long GetPlayerCreatureId();            // 0x004AEA40
};

class CTlkTable {
public:
    CExoString GetSimpleString(unsigned long nStrRef);  // 0x0041E8F0
};

extern CTlkTable* g_pTlkTable;                      // 0x007A3A08

class CAppManager {
public:
    void* m_p00;
    CClientExoApp* m_pClientExoApp;                 // 0x04
    CServerExoApp* m_pServerExoApp;                 // 0x08
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CSWGuiPazaakStart;

// vtable 0x007534C8.
class CSWGuiWagerPopup : public CSWGuiPanel {
public:
    CSWGuiWagerPopup(CSWGuiManager* pManager, CSWGuiPazaakStart* pStart, int nMaxWager);  // 0x0067F000
    void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x0067E150)
    void OnLessButton(CSWGuiControl* pControl);     // 0x0067CB40
    void OnMoreButton(CSWGuiControl* pControl);     // 0x0067CB50
    void OnWagerButton(CSWGuiControl* pControl);    // 0x0067CB60
    void UpdateWagerText();                         // 0x0067D340

    CSWGuiLabel m_lblTitle;                         // 0x064
    CSWGuiLabel m_lblMaximum;                       // 0x1a4
    CSWGuiLabel m_lblWagerVal;                      // 0x2e4
    CSWGuiSpeedButton m_btnLess;                    // 0x424
    CSWGuiSpeedButton m_btnMore;                    // 0x5f8
    CSWGuiButton m_btnWager;                        // 0x7cc
    CSWGuiButton m_btnQuit;                         // 0x990
    CSWGuiLabel m_lblBG;                            // 0xb54
    int m_nWager;                                   // 0xc94
    int m_nMaxWager;                                // 0xc98
    CExoArrayList<int> m_lstUnused;                 // 0xc9c
    CSWGuiPazaakStart* m_pStart;                    // 0xca8
};

// vtable 0x007532E8.
class CSWGuiPazaakStart : public CSWGuiPanel {
public:
    ~CSWGuiPazaakStart();                           // 0x0067E2B0
    void OnPanelAdded();                            // 0x48 (0x0067F750)
    void HandleWagerExit(int nWager);               // 0x0067D3B0
    void HandleQuitDialog(CSWGuiControl* pControl); // 0x0067D3D0
    int IsOtherCardActiveControl();                 // 0x0067D450
    int IsSidedeckCardActiveControl();              // 0x0067D480
    // Inline forms of the two tests above (ShowDisabledButtons expands them).
    int IsAvailCard(CSWGuiControl* pControl, int nId)
    {
        if (nId < 0 || nId >= 18) {
            return 0;
        }
        return pControl == &m_aAvail[nId];
    }
    int IsChosenCard(CSWGuiControl* pControl, int nId)
    {
        if (nId < 0 || nId >= 10) {
            return 0;
        }
        return pControl == &m_aChosen[nId];
    }
    void ShowDisabledButtons();                     // 0x0067E400
    void OnChosenCardEntered(CSWGuiControl* pControl);  // 0x0067E530
    int AddChosenCard(int nKind, int nSlot);        // 0x0067FB10
    int RemoveChosenCard(int nSlot);                // 0x0067FD10
    void HandleAvailCardDrop(CSWGuiControl* pControl);  // 0x0067FE80
    void HandleChosenCardDrop(CSWGuiControl* pControl); // 0x0067FED0

    CSWGuiLabel m_lblTitle;                         // 0x0064
    CSWGuiPazaakCard m_aAvail[18];                  // 0x01a4
    CSWGuiLabel m_aAvailNum[18];                    // 0x399c
    CSWGuiPazaakCard m_aChosen[10];                 // 0x501c
    CSWGuiActivatedButton m_btnSwitch;              // 0x6f34
    CSWGuiActivatedButton m_btnAccept;              // 0x7108
    CSWGuiLabel m_lblLText;                         // 0x72dc
    CSWGuiLabel m_lblRText;                         // 0x741c
    int m_aAvailCount[18];                          // 0x755c
    CPazaakCard m_aChosenCards[10];                 // 0x75a4
    CSWGuiWagerPopup* m_pWagerPopup;                // 0x75f4
    CSWGuiControl* m_pLastChosen;                   // 0x75f8
    CSWGuiControl* m_pLastAvail;                    // 0x75fc
    int m_bAllChosen;                               // 0x7600
    CSWPazaak* m_pPazaak;                           // 0x7604
    CSWGuiMessageBox m_cMessageBox;                 // 0x7608
    CSWGuiTutorial m_cTutorial;                     // 0x7f8c
};

extern Vector g_vPazaakFlashColor;                  // 0x007A23CC
extern int g_bPazaakShowOpponentHand;               // 0x00833BB0

// vtable 0x00753358.
class CSWGuiPazaakGame : public CSWGuiPanel {
public:
    ~CSWGuiPazaakGame();                            // 0x0067E540
    void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x0067E8F0)
    void OnPanelAdded();                            // 0x48 (0x0067FFA0)
    void RefreshDisplay();                          // 0x0067D4B0
    void ReconnectSidedeckButtons();                // 0x0067DA30
    void HandleQuitDialog(CSWGuiControl* pControl); // 0x0067DB50
    void SetHandEnabled(int bEnabled);              // 0x0067DBB0
    void HandleXButton(CSWGuiControl* pControl);    // 0x0067DC90
    void HandleBlackButton(CSWGuiControl* pControl);    // 0x0067DCC0
    void HandleFlipHandCard();                      // 0x0067DCF0
    void HandleHandCardRightClick(CSWGuiControl* pControl); // 0x0067DD80
    void HandleFlipButton(CSWGuiControl* pControl); // 0x0067DDA0
    void StartNewGame();                            // 0x0067E770
    void HandleEndDialog(CSWGuiControl* pControl);  // 0x0067E9A0
    void HandleTutorialClose(CSWGuiControl* pControl);  // 0x0067EA10
    void HandleContinue(CSWGuiControl* pControl);   // 0x0067EC20
    void HandleStand(CSWGuiControl* pControl);      // 0x0067ED00
    void HandlePlayHandCard(int nIndex);            // 0x0067EDE0
    void HandleHandCardDrop(CSWGuiControl* pControl);   // 0x0067EF00
    void HandleHandCardDoubleClick(CSWGuiControl* pControl);    // 0x0067EF80
    CSWPazaak* GetPazaak() { return m_pPazaak; }

    CSWGuiLabel m_lblPlayerName;                    // 0x0064
    CSWGuiLabel m_lblNpcName;                       // 0x01a4
    CSWGuiLabel m_lblPlayerTotal;                   // 0x02e4
    CSWGuiLabel m_lblNpcTotal;                      // 0x0424
    CSWGuiLabel m_lblPlayerSideDeck;                // 0x0564
    CSWGuiLabel m_lblNpcSideDeck;                   // 0x06a4
    CSWGuiLabel m_lblPlayerTurn;                    // 0x07e4
    CSWGuiLabel m_lblNpcTurn;                       // 0x0924
    CSWGuiLabel m_aPlayerScore[3];                  // 0x0a64
    CSWGuiLabel m_aNpcScore[3];                     // 0x0e24
    CSWGuiPazaakCard m_aPlayerBoard[9];             // 0x11e4
    CSWGuiPazaakCard m_aPlayerHand[4];              // 0x2de0
    CSWGuiPazaakCard m_aNpcBoard[9];                // 0x3a50
    CSWGuiPazaakCard m_aNpcHand[4];                 // 0x564c
    CSWGuiButton m_aFlip[4];                        // 0x62bc
    CSWGuiLabel m_lblFlipIcon;                      // 0x69cc
    CSWGuiLabel m_lblFlipLegend;                    // 0x6b0c
    CSWGuiActivatedButton m_btnEndTurn;             // 0x6c4c
    CSWGuiActivatedButton m_btnStand;               // 0x6e20
    CSWGuiActivatedButton m_btnPlay;                // 0x6ff4
    CSWGuiActivatedButton m_btnFlip;                // 0x71c8
    CSWGuiMessageBox m_cMessageBox;                 // 0x739c
    CSWGuiTutorial m_cTutorial;                     // 0x7d20
    const char* m_aSounds[4];                       // 0x86bc
    const char* m_szEndSound;                       // 0x86cc
    CSWPazaak* m_pPazaak;                           // 0x86d0
    int m_nState;                                   // 0x86d4
    float m_fTimer;                                 // 0x86d8
    int m_nTutorialShown;                           // 0x86dc
    int m_nPendingCard;                             // 0x86e0
    int m_nHandMode;                                // 0x86e4
};

#endif
