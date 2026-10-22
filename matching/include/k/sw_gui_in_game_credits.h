// Worker K matching headers: the in-game credits panel CSWGuiInGameCredits
// (vtable 0x007541F0, 0x0068F0E0-0x0068FCB0). Panel / control layouts follow
// matching/include/l/sw_gui_dialog.h (copied, trimmed to what is used here).
#ifndef K_SW_GUI_IN_GAME_CREDITS_H
#define K_SW_GUI_IN_GAME_CREDITS_H

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
    CExoString operator+(const CExoString& s) const;    // 0x005E5D10
    char* CStr() const;                             // 0x005E5670
};

class CResRef {
public:
    CResRef(const char* s);                         // 0x00406D80
    CResRef();                                      // 0x00405ED0
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

struct CSWGuiZeroExtent : public CSWGuiExtent {
    CSWGuiZeroExtent() { left = top = width = height = 0; }
};

struct STR_RES {
    CExoString sString;                             // 0x00
    CResRef cSoundResRef;                           // 0x08
    float fSoundLength;                             // 0x18
};

class CTlkTable {
public:
    int Fetch(unsigned long nStrRef, STR_RES& strRes, unsigned char nGender);  // 0x0041E550
};

extern CTlkTable* g_pTlkTable;                      // 0x007A3A08
extern unsigned char g_nGender;                     // 0x0073ECD0

class C2DA {
public:
    C2DA(CResRef cResRef, int bPrivate);            // 0x00413CC0
    int Load2DArray();                              // 0x004143B0
    int GetINTEntry(int nRow, const CExoString& sColumn, int* pValue);   // 0x00413660
    char pad_0x0[0x24];
    int m_nNumRows;                                 // 0x24
    int GetNumRows() { return m_nNumRows; }
    char pad_0x28[0x2c];
};

class CExoStreamingSoundSource {
public:
    CExoStreamingSoundSource();                     // 0x005D5A50
    ~CExoStreamingSoundSource();                    // 0x005D6200
    int IsPlaying();                                // 0x005D5AF0
    int Play(const CResRef& cResRef, int a, int b, int c, int d);  // 0x005D5B10
    void SetPriorityGroup(unsigned char nGroup);    // 0x005D5BA0
    void* m_pInternal;
};

class CSWGuiPanel;

class CSWGuiManager {
public:
    char pad_0x0[0x6c];
    short m_nWidth;                                 // 0x6c
    short m_nHeight;                                // 0x6e
    void SetSize(int nWidth, int nHeight);          // 0x0040BE70
};

extern CSWGuiManager* g_pGuiManager;                // 0x007A39F4

class CSWGuiTextParams {
public:
    void SetText(const CExoString& sText);          // 0x00415E00
    void SetBaseFont(const CResRef& cFont);         // 0x00415DD0
    void SetStrRef(int nStrRef);                    // 0x00415E50
    void SetAlignment(int nAlign);                  // 0x00415F20
    CExoString m_sText;                             // 0x00
    char pad_0x8[0x20];
    float m_fAlpha;                                 // 0x28
    char pad_0x2c[0x2c];
};

class CSWGuiLabel;

class CSWGuiControl {
public:
    CSWGuiControl();
    virtual ~CSWGuiControl();                       // 0x00
    virtual void SetExtent(const CSWGuiExtent& extent);     // 0x04
    virtual void v08();
    virtual void HandleMouseOver(int bOver);        // 0x0c
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38();
    virtual void HandleInputEvent(int nEvent, int bDown);   // 0x3c
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual CSWGuiLabel* GetLabel();                // 0x50
    virtual void v54();
    virtual int IsSelectable();                     // 0x58
    virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94();

    CSWGuiExtent m_extent;                          // 0x04
    char pad_0x14[0x30];
    unsigned long m_nFlags;                         // 0x44
    char pad_0x48[0x14];

    void AddEvent(int nEvent, CSWGuiPanel* pObject,
                  void (CSWGuiPanel::*pfnCallback)(CSWGuiControl*));   // 0x0041AB20
};

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

class CSWGuiLabel : public CSWGuiControl {
public:
    CSWGuiLabel();                                  // 0x0041ACD0
    ~CSWGuiLabel();                                 // 0x00419D10
    virtual void CopyStyle(const CSWGuiExtent& extent, CSWGuiControl* pFrom, float fScale); // 0x98
    char pad_0x5c[0x74];
    CSWGuiLabelText m_cTextBox;                     // 0xd0
    CSWGuiTextParams m_cText;                       // 0xe8
};

class CSWGuiListBox : public CSWGuiControl {
public:
    CSWGuiListBox();                                // 0x0041BE40
    ~CSWGuiListBox();                               // 0x0041B060
    void ClearControls();                           // 0x00419A90
    void SetActiveControl(CSWGuiControl* pControl, int bSound);     // 0x0041C160
    void AddControls(CSWGuiControl** ppControls, int nCount, int a, int b, int c);   // 0x0041C2C0
    char pad_0x5c[0x104];
    int m_nScrollMax;                               // 0x160
    int m_nScrollPos;                               // 0x164
    char pad_0x168[0x150];
    float m_fScale;                                 // 0x2b8
    char pad_0x2bc[0x10];
    CSWGuiControl* m_pProtoItem;                    // 0x2cc
    char pad_0x2d0[0x10];
};

// vtable 0x0073e010.
class CSWGuiPanel {
public:
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
    char pad_0x1c[0x28];
    unsigned long m_nFlags;                         // 0x44
    unsigned char m_nResolution;                    // 0x48
    float m_fAlpha;                                 // 0x4c
    Vector m_vColor;                                // 0x50
    void* m_pBorder;                                // 0x5c
    unsigned char m_nOpenSound;                     // 0x60

    void StartLoadFromLayout(const CResRef& cResRef);   // 0x0040A680
    void InitControl(CSWGuiControl* pControl, const CExoString& sName, int bAdd);  // 0x0040B930
    void UnloadResource();                          // 0x0040B8F0
    void AddControl(CSWGuiControl* pControl);       // 0x0040B9A0
    void OnCloseClicked(CSWGuiControl* pControl);   // 0x00624BA0

    void Close() { m_nFlags = (m_nFlags & ~0x300) | 0x400; }
};

class CGuiInGame {
public:
    void FadeAndStopDialogAmbientTrack();           // 0x0062FB90
    void AdjustHideCount(int nOperation);           // 0x0062B410
};

class CClientExoAppInternal {
public:
    char pad_0x0[0x4bc];
    int m_bEvilEndingHackInProgress;                // 0x4bc
    void SetEvilEndingHackInProgress(int b);        // 0x005F4E50
};

class CClientExoApp {
public:
    void* m_p00;
    CClientExoAppInternal* m_pInternal;             // 0x04
    CClientExoAppInternal* GetInternal() { return m_pInternal; }
    CGuiInGame* GetInGameGui();                     // 0x005ED690
    void SetInputClass(int nClass, int bForce);     // 0x005EDA60
    int GetInputClass();                            // 0x005EDA70
    void HideMouse(int nMask);                      // 0x005EE170
    void ShowMouse(int nMask);                      // 0x005EE180
    void StopCreditSequence();                      // 0x005EE1D0
};

class CServerExoApp {
public:
    int GetPauseState(unsigned char nState);        // 0x004AE980
};

class CAppManager {
public:
    void* m_p00;
    CClientExoApp* m_pClientExoApp;                 // 0x04
    CServerExoApp* m_pServerExoApp;                 // 0x08
    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC
extern int g_nScreenWidth;                          // 0x0078D1D4
extern int g_nScreenHeight;                         // 0x0078D1D8
void ResumeTimers();                                // 0x00403DE0

// Screen state saved while the evil-ending credits force 640x480.
extern int g_nCreditsSavedScreenHeight;             // 0x00833BB8
extern int g_nCreditsSavedGuiHeight;                // 0x00833BBC
extern int g_nCreditsSavedGuiWidth;                 // 0x00833BC0
extern int g_nCreditsSavedScreenWidth;              // 0x00833BC4
extern float g_fCreditsScrollRate;                  // 0x007A2670 (305.0)

// vtable 0x007541F0; ??_G 0x0068F6D0 (matching/src/c).
class CSWGuiInGameCredits : public CSWGuiPanel {
public:
    CSWGuiInGameCredits(CSWGuiManager* pManager, const CExoString& sMusic);  // 0x0068F8D0
    ~CSWGuiInGameCredits();                         // 0x0068F0E0
    void Draw(float fDelta);                        // 0x34 (0x0068F880)
    void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x0068F350)
    void OnPanelAdded();                            // 0x48 (0x0068F260)

    void Show(unsigned long nStrRef, int bLast);    // 0x0068F3A0
    void UpdateScroll(float fDelta);                // 0x0068F630
    void SetNextText();                             // 0x0068F6F0
    void UpdateFading(float fDelta);                // 0x0068F790

    CSWGuiListBox m_lbCredits;                      // 0x64
    CSWGuiLabel m_lblText;                          // 0x344
    CSWGuiLabel m_lblBackground;                    // 0x484
    CExoString m_sMusic;                            // 0x5c4
    CExoStreamingSoundSource* m_pMusic;             // 0x5cc
    unsigned long* m_pStrRefs;                      // 0x5d0
    float m_fScrollTime;                            // 0x5d4
    float m_fFadeTime;                              // 0x5d8
    int m_nPrevInputClass;                          // 0x5dc
    unsigned char m_nCurLine;                       // 0x5e0
    int m_bFading;                                  // 0x5e4
    int m_bFadeIn;                                  // 0x5e8
    unsigned char m_nLines;                         // 0x5ec
    float m_fScrollStep;                            // 0x5f0
};

#endif
