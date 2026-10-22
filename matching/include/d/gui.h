// CSWGuiManager / CSWGuiPanel and the GUI object base.
// Layouts from include/kotor/b0040_gui_manager_panel.h and
// docs/batches/b0040_gui_manager_panel.md (x86 confirmed).
// Standalone header: GUI CExoArrayList members are out-of-line calls
// (ctor 0x00416770, dtor 0x0056b6f0, Add 0x00671c00, DelIndex 0x006857a0),
// unlike the inline list of d/exo_key_table.h.
#ifndef D_GUI_H
#define D_GUI_H

extern "C" unsigned int __cdecl strlen(const char* s);
extern "C" int __cdecl memcmp(const void* a, const void* b, unsigned int n);
extern "C" int __cdecl strcmp(const char* a, const char* b);
#pragma intrinsic(strlen, memcmp, strcmp)

class CExoString {
public:
    char* m_sString;
    int m_nBufferLength;
    CExoString();                                   // 0x005b3190
    CExoString(const char* sString);                // 0x005e5a90
    CExoString(const CExoString& sString);          // 0x005e5b00
    ~CExoString();                                  // 0x005e5c20
    CExoString& operator=(const CExoString& s);     // 0x005e5c50
    CExoString& operator=(const char* s);           // 0x005e5140
    CExoString operator+(const CExoString& s) const; // 0x005e5d10
};

class CResRef {
public:
    char m_resRef[16];
    CResRef();                                      // 0x00405ed0
    CResRef(const CResRef& cResRef);
    CResRef(const char* szResRef);                  // 0x00406d80
    CResRef(const CExoString& sResRef);             // 0x00406d60
    CResRef operator=(const char* szResRef);        // 0x00406290
    char* GetResRefStr() const;                     // 0x00405fe0
};

struct CResStruct {
    unsigned long m_nStructIndex;
};

struct CResList : public CResStruct {
    char m_pLabel[16];
};

class Vector {
public:
    float x, y, z;
};

class CRes {
public:
    virtual ~CRes();
    void* Demand();                                 // 0x00409b10
    int Release();                                  // 0x00409b80
};

class CResGFF : public CRes {
public:
    CResGFF(unsigned short nType, char* pFileType, const CResRef& cResRef); // 0x00410630
    void GetTopLevelStruct(CResStruct* pStruct);    // 0x00411240
    int GetList(CResList* pList, CResStruct* pStruct, char* szLabel); // 0x004118c0
    int GetStructFromStruct(CResStruct* pOut, CResStruct* pStruct, char* szLabel); // 0x00411a10
    int ReadFieldINT(CResStruct* pStruct, char* szLabel, int& bSuccess, int nDefault); // 0x00411c90
    float ReadFieldFLOAT(CResStruct* pStruct, char* szLabel, int& bSuccess, float fDefault); // 0x00411d00
    CResRef ReadFieldCResRef(CResStruct* pStruct, char* szLabel, int& bSuccess, const CResRef& cDefault); // 0x00411e10
    Vector ReadFieldVector(CResStruct* pStruct, char* szLabel, int& bSuccess, const Vector& vDefault); // 0x004122a0
    char pad_0x04[0x9c];
};

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList(int nSize = 0);
    ~CExoArrayList();
    void Add(T t);
    void DelIndex(int i);
    int IndexOf(T t);
    void Insert(T t, int nIndex);
    T& operator[](int i) { return element[i]; }
};

class C2DA {
public:
    C2DA(CResRef cResRef, int bAutoRequest);        // 0x00413cc0
    virtual ~C2DA();
    int Load2DArray();                              // 0x004143b0
    int GetCExoStringEntry(int nRow, const CExoString& sColumn, CExoString* pValue); // 0x00413270
    char pad_0x04[0x20];
    int m_nNumRows;                                 // 0x24
    char pad_0x28[0x2c];
};

class CExoSound {
public:
    virtual ~CExoSound();
};

class CExoSoundSource : public CExoSound {
public:
    CExoSoundSource(const CResRef& cResRef);             // 0x005d60e0
    void SetPriorityGroup(unsigned char n);         // 0x005d5900
    void Play();                                    // 0x005d5930
    int pad_0x04;
};

struct CSWGuiExtent {
    int left;
    int top;
    int width;
    int height;
    void Load(CResGFF* pGFF, CResStruct* pStruct);
};

class CSWGuiControl;
class CSWGuiManager;
class CSWGuiPanel;

// vtable 0x0073df20 (slots 0x00-0x30).
class CSWGuiObject {
public:
    CSWGuiObject() { m_extent.left = m_extent.top = m_extent.width = m_extent.height = 0; }
    virtual ~CSWGuiObject();                                    // 0x00 (0x00409e80)
    virtual void SetExtent(const CSWGuiExtent& extent);         // 0x04 (0x00409e90)
    virtual void SetActiveControl(CSWGuiControl* pControl, int bSound); // 0x08 (0x005b5e90)
    virtual void v0c(int n);                                    // 0x0c (0x0060e760)
    virtual int v10(int a, int b);                              // 0x10 (0x00409eb0)
    virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24();                     // 0x14-0x24 (0x004015a0)
    virtual void v28(); virtual void v2c(); virtual void v30(); // 0x28-0x30 (0x0063e7f0)
    CSWGuiExtent m_extent;          // 0x04
};

class CSWGuiNavigation {
public:
    int GetNavigation(int nDirection);              // 0x00417680
    void SetNavigation(int nDirection, CSWGuiControl* pControl); // 0x00417660
};

class CSWGuiScrollBar {
public:
};

// Control vtable slots used here.
class CSWGuiControl {
public:
    virtual ~CSWGuiControl();                                   // 0x00
    virtual void SetExtent(CSWGuiExtent* pExtent);              // 0x04
    virtual void v08();
    virtual void SetHighlight(int bHighlight);                  // 0x0c
    virtual void v10();
    virtual void OnCaptureLost();                               // 0x14
    virtual void HandleLMouseDown();                            // 0x18
    virtual void HandleLMouseUp();                              // 0x1c
    virtual void v20();
    virtual void HandleRMouseUp();                              // 0x24
    virtual void v28(); virtual void v2c(); virtual void v30(); virtual void v34();
    virtual void Draw(float fDelta);                            // 0x38
    virtual void HandleInputEvent(int nEvent, int bDown);       // 0x3c
    virtual void v40();
    virtual CSWGuiControl* HitCheckMouse(int x, int y);         // 0x44
    virtual void v48();
    virtual CSWGuiNavigation* GetNavigation();                  // 0x4c
    virtual void v50(); virtual void v54();
    virtual int IsSelectable();                                 // 0x58
    virtual void v5c(); virtual void v60(); virtual void v64(); virtual void v68();
    virtual void v6c();
    virtual CSWGuiControl* GetScrollBar();                      // 0x70
    virtual void v74(); virtual void v78();
    virtual void SetFocus(int bFocus);                          // 0x7c
    virtual int IsControl();                                    // 0x80
    virtual void HandleRMouseDown();                            // 0x84
    virtual void v88();
    virtual void OnPanelLoaded();                               // 0x8c
    virtual int HasToolTip();                                   // 0x90
    virtual void DrawAt(int x, int y);                          // 0x94
    virtual void v98();
    virtual void HandleFocusEvent(int nArg);                    // 0x9c

    CSWGuiExtent m_extent;          // 0x04
    char pad_0x14[0x20];
    CSWGuiPanel* m_pParent;         // 0x34
    char pad_0x38[0x0c];
    unsigned long m_nFlags;         // 0x44
    char pad_0x48[0x8];
    int m_nIndex;                   // 0x50
    char m_nSound;                  // 0x54 (unused)
    unsigned char m_nClickSound;    // 0x55

    void Initialize(class CSWGuiPanel* pPanel, CResGFF* pGFF, CResStruct* pStruct, int nArg); // 0x00418840
    CSWGuiControl* GetOwner();                      // 0x00418a50
    CSWGuiControl* GetToolTipControl(int n);        // 0x0041a5e0
    void RefreshScroll();                           // 0x0041c400
};

class CSWGuiBorderParams {
public:
    void SetFillImage(const CResRef& cResRef, int nArg); // 0x00414c00
    void SetFillImage(const CResRef& cResRef) { SetFillImage(cResRef, 0); }
};

class CSWGuiBorder {
public:
    CSWGuiBorder();                                 // 0x004167f0
    virtual ~CSWGuiBorder();
    virtual void SetExtent(CSWGuiExtent* pExtent);  // 0x04
    virtual void v08();
    virtual void Draw(float fDelta);                // 0x0c
    virtual void Load(CResGFF* pGFF, CResStruct* pStruct, const CExoString& sName); // 0x10
    char pad_0x04[0x10];
    CSWGuiBorderParams m_cParams;                   // 0x14
    char pad_0x15[0x5f];
};

// AurGUI layer helpers (cdecl).
void AurGUIStartLayer();                            // 0x004591b0
int AurGUISetupViewport(int x, int y, int w, int h, Vector* pColor, int n, float fAlpha); // 0x004592f0
void AurGUIStopLayer();                             // 0x004592b0
void AurGUICloseViewport();                         // 0x00459580

// vtable 0x0073e010.
class CSWGuiPanel : public CSWGuiObject {
public:
    CSWGuiPanel(CSWGuiManager* pManager);
    virtual ~CSWGuiPanel();                                     // 0x00
    virtual void SetExtent(const CSWGuiExtent& extent);         // 0x04
    virtual void SetActiveControl(CSWGuiControl* pControl, int bSound); // 0x08
    virtual void v28(); virtual void v2c(); virtual void v30(); // 0x28-0x30
    virtual void Draw(float fDelta);                            // 0x34
    virtual void Update(float fDelta);                          // 0x38
    virtual void HandleInputEvent(int nEvent, int bDown);       // 0x3c
    virtual CSWGuiControl* HitCheckMouse(int x, int y);         // 0x40
    virtual CSWGuiControl* GetFocusControl();                   // 0x44
    virtual void OnPanelAdded();                                // 0x48
    virtual void OnPanelRemoved();                              // 0x4c
    virtual void HandleInputEvent27(int n);                     // 0x50
    virtual void HandleInputEvent28(int n);                     // 0x54
    virtual void HandleInputEvent29(int n);                     // 0x58
    virtual void HandleInputEvent2A(int n);                     // 0x5c
    virtual void HandleInputEvent2B(int n);                     // 0x60
    virtual CExoString GetFullScreenBG();                       // 0x64
    virtual void UpdateFonts();                                 // 0x68

    int m_bUpdateFonts;                             // 0x14
    CSWGuiManager* m_pManager;                      // 0x18
    CSWGuiControl* m_pActiveControl;                // 0x1c
    CExoArrayList<CSWGuiControl*> m_lstControls;    // 0x20
    CResGFF* m_pGFF;                                // 0x2c
    CResList m_cControls;                           // 0x30
    union {
        unsigned long m_nFlags;                     // 0x44
        struct {
            unsigned long m_bCentred : 1;           // bit 0
            unsigned long m_bLoaded : 1;            // bit 1
            unsigned long m_bVisible : 1;           // bit 2
            unsigned long m_nDrawMode : 2;          // bits 3-4
            unsigned long m_bCentreX : 1;           // bit 5
            unsigned long m_bCentreY : 1;           // bit 6
            unsigned long m_bDraw : 1;              // bit 7
            unsigned long m_bFlag8 : 1;             // bit 8
            unsigned long m_nFadeState : 2;         // bits 9-10
            unsigned long m_nFlagsHigh : 21;
        };
    };
    unsigned char m_nResolution;                    // 0x48
    float m_fAlpha;                                 // 0x4c
    Vector m_vColor;                                // 0x50
    CSWGuiBorder* m_pBorder;                        // 0x5c
    unsigned char m_nOpenSound;                     // 0x60

    void StartLoadFromLayout(const CResRef& cResRef);
    void SetBackground(const CResRef& cResRef);
    void SetupNavigation();                         // 0x0040a890
    void SetVisible(int bVisible);                  // 0x0040b9d0
    void GetAbsoluteExtent(CSWGuiExtent* pExtent);
    void CenterPanel();
    void SetAlpha(float fAlpha);                    // 0x00404b00
    void UnloadResource();
    void InitControl(CSWGuiControl* pControl, int nArg, int bAdd);
    void AddControl(CSWGuiControl* pControl);
    CSWGuiControl* GetControl(int nIndex);
};

class CSWGuiToolTip {
public:
    CSWGuiToolTip(CSWGuiManager* pManager);         // 0x006277c0
    virtual ~CSWGuiToolTip();
    // panel slots 0x04..0x64
    virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20();
    virtual void v24(); virtual void v28(); virtual void v2c(); virtual void v30();
    virtual void Draw(float fDelta); virtual void v38(); virtual void v3c(); virtual void v40();
    virtual void v44(); virtual void v48(); virtual void v4c(); virtual void v50();
    virtual void v54(); virtual void v58(); virtual void v5c(); virtual void v60();
    virtual void v64();
    virtual void UpdateFonts();                     // 0x68
    void SetText(int nText);                        // 0x00624ae0
    void Show();                                    // 0x00624af0
    char pad_0x04[0x1a0];
};

class CAurObject {
public:
    virtual ~CAurObject();
};

class CSWGuiMouse {
public:
    virtual ~CSWGuiMouse();                                     // 0x00
    virtual void SetExtent(CSWGuiExtent* pExtent);              // 0x04
    virtual void v08(); virtual void Draw(float fDelta); virtual void v10(); virtual void v14();
    virtual void PlayAnimation(char* sName, float fSpeed, int n1, int n2); // 0x18
    virtual void v1c(); virtual void v20(); virtual void v24(); virtual void v28();
    virtual void v2c(); virtual void v30();
    virtual void SetTexture(char* sPart, char* sTexture);       // 0x34
    void SetScale(float fScale);                                // 0x0043e150
};

class CClientOptions {
public:
    char pad_0x00[0x0c];
    float m_fGuiScale;              // 0x0c
    float GetGuiScale() { return m_fGuiScale; }
    int m_n10;                      // 0x10
    unsigned long m_nFlags;         // 0x14 (0x400: tooltips)
    char pad_0x18[0x68];
    unsigned char m_nFontSize;      // 0x80
};

class CFontManager {
public:
    void UpdateFonts();             // 0x0062b490
};

class CClientExoAppInternal {
public:
    void SetMousePosition(int x, int y);            // 0x005f4ea0
};

class CExoInput {
public:
    void SetMousePosition(int x, int y);            // 0x005df5d0
};

class CClientExoApp {
public:
    void* m_p00;
    CClientExoAppInternal* m_pInternal;             // 0x04
    CClientOptions* GetOptions();   // 0x005ed700
    CFontManager* GetFontManager(); // 0x005ed690
    int GetInputMode();             // 0x005eda70
    int GetInFreeLook();            // 0x005ee230
};

class CAppManager {
public:
    void* m_pServer;
    CClientExoApp* m_pClientExoApp; // 0x04
};

extern CAppManager* g_pAppManager;              // 0x007a39fc
extern CSWGuiManager* g_pGuiManager;            // 0x007a39f4
extern CExoInput* g_pExoInput;                  // 0x007a39e4
extern int g_nScreenWidth;                      // 0x0078d1d4
extern int g_nScreenHeight;                     // 0x0078d1d8
extern char* g_aCursorNames[];                  // 0x0078d240
extern CExoString g_aResolutionSuffixes[];      // 0x007a3d50
extern int g_bDrawSafeRegion;                   // 0x007a3d4c

void GuiBeginFrame();                           // 0x0044ed90
void GuiUpdateFrame(float fDelta);              // 0x00418300
void GuiUpdateBorders(float fDelta);            // 0x00414a50

class CSWGuiManager {
public:
    CSWGuiManager();
    ~CSWGuiManager();

    int m_nMouseX;                                  // 0x00
    int m_nMouseY;                                  // 0x04
    CSWGuiControl* m_pMouseOverControl;             // 0x08
    int m_nCaptureType;                             // 0x0c
    CSWGuiControl* m_pCaptureControl;               // 0x10
    CSWGuiControl* m_pLastMouseOverControl;         // 0x14
    CSWGuiControl* m_pFocusControl;                 // 0x18
    unsigned long m_bLeftDown : 1;                  // 0x1c bit 0
    unsigned long m_bRightDown : 1;                 //      bit 1
    unsigned long m_bCentred : 1;                   //      bit 2
    unsigned long m_bToolTipShown : 1;              //      bit 3
    unsigned long m_nFlagsRest : 28;
    CSWGuiMouse* m_pMouseObject;                    // 0x20
    CSWGuiMouse* m_pMouse;                          // 0x24
    CResRef m_cCursor;                              // 0x28
    int m_nCursor;                                  // 0x38
    CSWGuiToolTip* m_pToolTip;                      // 0x3c
    CSWGuiControl* m_pToolTipOwner;                 // 0x40
    float m_fToolTipTimer;                          // 0x44
    float m_fGuiScale;                              // 0x48
    int m_bToolTipLocked;                           // 0x4c
    CSWGuiControl* m_pToolTipControl;               // 0x50
    CSWGuiControl* m_pDragControl;                  // 0x54
    int m_nSavedMouseX;                             // 0x58
    int m_nSavedMouseY;                             // 0x5c
    int m_n60;                                      // 0x60
    int m_n64;                                      // 0x64
    int m_n68;                                      // 0x68
    short m_nWidth;                                 // 0x6c
    short m_nHeight;                                // 0x6e
    unsigned char m_nOpaquePanels;                  // 0x70
    unsigned char m_nTransparentPanels;             // 0x71
    unsigned char m_n72;                            // 0x72
    CExoString m_sResolution;                       // 0x74
    CAurObject* m_pBackground;                      // 0x7c
    CAurObject* m_p80;                              // 0x80
    CAurObject* m_p84;                              // 0x84
    CExoArrayList<CSWGuiPanel*> m_lstPanels;        // 0x88
    CExoArrayList<CSWGuiPanel*> m_lstModalPanels;   // 0x94
    CExoSoundSource** m_pGuiSounds;                 // 0xa0
    char m_nGuiSounds;                              // 0xa4

    int LoadGuiSounds();
    void UnloadGuiSounds();
    void PlayGuiSound(char nSound);
    int HandleLMouseUp();
    int HandleRMouseUp();
    void SetCapture(CSWGuiControl* pControl, int nType);
    void ReleaseCapture(CSWGuiControl* pControl);
    void DeactivateSoftwareMouse();
    void HideSoftwareMouse();
    void SetSoftwareMouseCursor(int nCursor, int bCentred, int bPressed);
    void ChangeToolTipText(int nText);
    void ReloadToolTipPanel();
    void SaveMousePosition();
    static CExoString GetScreenResolutionString(int nWidth, int nHeight);
    int GetModalIndex(CSWGuiPanel* pPanel);
    int GetPanelIndex(CSWGuiPanel* pPanel);
    int HitCheckMouse(int x, int y, CSWGuiPanel** ppPanel, CSWGuiControl** ppControl, int nUnused);
    void DetermineNeedToDraw();
    int IsPanelOnTop(CSWGuiPanel* pPanel);
    void DrawPCBG(float fDelta);
    CSWGuiMouse* ActivateSoftwareMouse();
    CSWGuiMouse* ShowSoftwareMouse();
    int SetFocusControl(int nArg);
    void UpdateMouseOverControl();
    void GetUpdatedFontName(char* sName);
    void UpdateAllFonts();
    void DisplayToolTip(int nText);
    void ResetToolTipTimer();
    void SetTemporarilyDisableToolTip(int bDisable);
    void AddPanel(CSWGuiPanel* pPanel, int nFlags, int bPlaySound);
    void SendPanelToBack(CSWGuiPanel* pPanel);
    int PanelExists(CSWGuiPanel* pPanel);
    int PushModalPanel(CSWGuiPanel* pPanel);
    CSWGuiPanel* PopModalPanel();
    void SetSize(int nWidth, int nHeight);
    void HideAllPanels();
    void RemoveToolTip();
    void HandleMouseMove(int x, int y);
    int HandleLMouseDown(int bDown);
    int HandleRMouseDown();
    int HandleZMouse(int nDelta);
    void MoveMouseToPosition(int x, int y);
    void RestoreOldMousePosition();
    int RemovePanel(CSWGuiPanel* pPanel);
    void GetSafeMargins(int* pX, int* pY);
    void DrawSafeRegion(float fDelta);              // 0x0040bec0
    void Draw(float fDelta);
    void Update(float fDelta);
};

CSWGuiExtent IntersectExtents(const CSWGuiExtent& a, const CSWGuiExtent& b); // 0x0040a4a0
CSWGuiExtent UnionExtents(const CSWGuiExtent& a, const CSWGuiExtent& b);     // 0x0040a520

#endif
