// Worker M matching headers: the load screen and the main menu
// (0x0067A6D0-0x0067CA4F).
//   CSWGuiLoadScreen (0x6B8, vtable 0x00752DC0), CSWGui3DSceneView (0x90,
//   vtable 0x00752E30) and CSWGuiMainMenu (0x1414, vtable 0x00752F70).
// Control/panel layouts are copied (trimmed) from m/sw_gui_pazaak.h, which
// follows d/gui_control.h; member offsets were checked against the playable
// reference (src/units/0487 gui_control_load, 0492 mainmenu, 0610
// load_screen, 0880 gui_m48_controls).
#ifndef M_SW_GUI_MAIN_MENU_H
#define M_SW_GUI_MAIN_MENU_H

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
    int operator<(const CExoString& s) const;       // 0x005E5500
    int operator==(const CExoString& s) const;      // 0x005E5200
    char* CStr() const;                             // 0x005E5670
    void __cdecl Format(char* sFormat, ...);        // 0x005E5680
    int GetLength() const;                          // 0x005E5790
    CExoString LowerCase() const;                   // 0x005E6080
    CExoString SubString(int nStart, int nCount) const; // 0x005E6270
    int Find(const CExoString& s, int nStart) const;    // 0x005E5EB0
};

class CResRef {
public:
    CResRef(const char* s);                         // 0x00406D80
    CResRef(const CExoString& s);                   // 0x00406D60
    char m_resRef[16];
};

void* operator new(unsigned int nSize);
void operator delete(void* p);
void* operator new[](unsigned int nSize);
void operator delete[](void* p);

template <class T>
class CExoArrayList;

class CExoAliasList {
public:
    CExoString* GetAliasPath(const CExoString& sAlias, int nIndex);  // 0x005E6890
};

class CExoBase {
public:
    void GetDirectoryList(CExoArrayList<CExoString>* pList, const CExoString& sDir, unsigned short nType,
                          int bSort, int bStripExt);  // 0x005E6640
    char pad_0x0[0xc];
    CExoAliasList* m_pAliasList;                    // 0x0c
};

extern CExoBase* g_pExoBase;                        // 0x007A39E0
extern unsigned char g_nLiveFolderCount;            // 0x0073ECD2 (descriptive)

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList()
    {
        num = 0;
        array_size = 0;
        element = 0;
    }
    ~CExoArrayList()
    {
        delete[] element;
        element = 0;
    }
    void Add(T t);                                  // 0x004059A0 (CExoString)
    __forceinline void AddInline(T t)
    {
        if (num == array_size) {
            array_size = array_size ? array_size * 2 : 16;
            T* pOld = element;
            element = new T[array_size];
            for (int i = 0; i < num; i++) {
                element[i] = pOld[i];
            }
            if (pOld) {
                delete[] pOld;
            }
        }
        element[num++] = t;
    }
    void DelIndex(int nIndex)
    {
        num--;
        for (; nIndex < num; nIndex++) {
            element[nIndex] = element[nIndex + 1];
        }
    }
    void Clear()
    {
        if (element) {
            delete[] element;
            element = 0;
            array_size = 0;
        }
        num = 0;
    }
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
};

class Quaternion {
public:
    Quaternion(float fX, float fY, float fZ, float fW)
    {
        x = fX;
        y = fY;
        z = fZ;
        w = fW;
    }
    float x, y, z, w;
};

extern Vector g_vBlack;                             // 0x00833910
extern Vector g_vAbilityColorGranted;               // 0x007A23C0
extern Vector g_vPazaakButtonHilight;               // 0x007A23B4
extern int g_nScreenWidth;                          // 0x0078D1D4
extern int g_nScreenHeight;                         // 0x0078D1D8

void AurGUIStartLayer();                            // 0x004591B0
int AurGUISetupViewport(int nX, int nY, int nWidth, int nHeight, Vector* pColor, int bClear,
                        float fAlpha);              // 0x004592F0
void AurGUICloseViewport();                         // 0x00459580
void AurGUIStopLayer();                             // 0x004592B0

class CExoResMan {
public:
    int Exists(const CResRef& cResRef, unsigned short nType, unsigned long* pSize); // 0x00408BC0
    int AddResourceDirectory(const CExoString& sDir);       // 0x00408800
    int RemoveResourceDirectory(const CExoString& sDir);    // 0x004088D0
    int AddResourceImageFile(const CExoString& sFile);      // 0x004087C0
    int GetFreeDiskSpace(const CExoString& sAlias, unsigned __int64* pFree);  // 0x004067B0
    int CreateDirectory2(const CExoString& sDir);   // 0x004068C0
    int Leaf9480(const CExoString& sDir, int a, int b);     // 0x00409480
    int Leaf9460(const CExoString& sDir, int a, int b);     // 0x00409460
    int RemoveFile(const CExoString& sFile, unsigned short nType);  // 0x00406B20
    char pad_0x0[0x34];
    unsigned char m_nFlags;                         // 0x34
};

extern CExoResMan* g_pExoResMan;                    // 0x007A39E8

class CTlkTable {
public:
    CExoString GetSimpleString(unsigned long nStrRef);  // 0x0041E8F0
    void ParseTokens(CExoString* pString);          // 0x0041DD30
    void SetCustomToken(int nToken, const CExoString& sValue);  // 0x0041DB50
};

extern CTlkTable* g_pTlkTable;                      // 0x007A3A08

class CExoSound {
public:
    void SetSoundMode(int nMode);                   // 0x005D5E80
};

extern CExoSound* g_pExoSound;                      // 0x007A39EC
extern int g_nSoundModeBack;                        // 0x0074C5EC

class CExoIni {
public:
    CExoIni();                                      // 0x005E6750
    ~CExoIni();                                     // 0x005E67E0
    void* m_pcExoIniInternal;
};

// Does not use its receiver; callers leave ECX untouched.
class CSWPartyTable {
public:
    static void ResetCurrentSessionStartTime();     // 0x00563CF0
};

class CGuiInGame;
class CSWGuiMessageBox;

class CNetLayer {
public:
    int StartConnectToSession(int nSession, const CExoString& sPlayerName, const CExoString& sPassword,
                              int nTimeout, int nType);  // 0x005D5040
    int SendMessageToPlayer(unsigned long nPlayer, unsigned char* pData, unsigned long nSize,
                            unsigned long nFlags);  // 0x005D4CB0
};

extern unsigned long PLAYERID_SERVER;               // 0x0074C528 (0xFFFFFFFD)
extern unsigned char g_nConnectionType;             // 0x0074C540 (descriptive)

class CSWCMessage {
public:
    int SendPlayerToServer_01_00();                 // 0x006775E0
};

class CWorldTimer {
public:
    void PauseWorldTimer();                         // 0x004ADFF0
};

class CClientExoApp {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual CNetLayer* GetNetLayer();               // 0x10
    CGuiInGame* GetInGameGui();                     // 0x005ED690
    void ExitProgram();                             // 0x005ED450
    void ShowMouse(int bShow);                      // 0x005EE180
    void BusyWaitMenuMusic();                       // 0x005EE120
    CWorldTimer* GetWorldTimer();                   // 0x005ED4F0
    CExoString GetClientName();                     // 0x005ED490
    CSWCMessage* GetMessage();                      // 0x005ED6F0
    void SetPlayerCharacterName(const CExoString& sName);   // 0x005EDAA0
    int SetInputClass(int nClass, int bForce);      // 0x005EDA60
    void DisableInput();                            // 0x005EDBE0
    CExoString GetLoadMusicByModuleName(const CExoString& sModule);   // 0x005EDD00
    void SetLoadMusic(const CExoString& sMusic);    // 0x005EDD30
    void SetLoadScreenByModuleName(const CExoString& sModule);  // 0x005EDCF0
    void ShowLoadScreen(int a, int b, int c);       // 0x005EDA80
    int GetNextLoadScreenHintSTRREF(const CExoString& sModule, unsigned char nType); // 0x005EE090
    void SetLoadScreenHint(int nHint);              // 0x005EDF00
};

class CServerExoApp {
public:
    CWorldTimer* GetWorldTimer();                   // 0x004AEDE0
};

class CAppManager {
public:
    void CreateServer(int bLoad);                   // 0x00401380
    void* m_p00;
    CClientExoApp* m_pClientExoApp;                 // 0x04
    CServerExoApp* m_pServerExoApp;                 // 0x08
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CGuiInGame {
public:
    void AppendToDialogBuffer(const CExoString& sSpeaker, const CExoString& sText);  // 0x0062B680
    void AdjustHideCount(int nDelta);               // 0x0062B410
    char pad_0x0[0x98];
    CSWGuiMessageBox* m_pMessageBox;                // 0x98
};

class CSWGuiControl;
class CSWGuiPanel;
class CResGFF;
struct CResStruct;

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
    char pad_0x08[0x64];
    short m_nWidth;                                 // 0x6c
    short m_nHeight;                                // 0x6e

    static CExoString __cdecl GetScreenResolutionString(int nWidth, int nHeight);  // 0x0040A3E0
    void PlayGuiSound(char nSound);                 // 0x0040A140
    void AddPanel(CSWGuiPanel* pPanel, int nFlags, int bPlaySound);   // 0x0040BC70
};

class CSWGuiTextParams {
public:
    void SetColor(const Vector& vColor);            // 0x00414E10
    void SetText(const CExoString& sText);          // 0x00415E00
    CExoString m_sText;                             // 0x00
    char pad_0x08[0x4c];
};

class CSWGuiBorderParams {
public:
    void SetFillImage(const CResRef& cResRef, int bForce);  // 0x00414C00
    int GetCornerSize();                            // 0x00414CD0
    char pad_0x00[0x1c];
    unsigned long m_bFlag0 : 1;                     // 0x1c bit 0
    unsigned long m_bFlag1 : 1;                     //      bit 1
    char pad_0x20[0x40];
};

class CSWGuiBorder {
public:
    CSWGuiBorder();                                 // 0x004167F0
    virtual ~CSWGuiBorder();
    CSWGuiExtent m_extent;                          // 0x04
    CSWGuiBorderParams m_cParams;                   // 0x14
};

class CSWGuiText {
public:
    CSWGuiText();                                   // 0x00417280
    virtual ~CSWGuiText();
    virtual void v04();
    virtual int GetHeight();                        // 0x08
    char pad_0x04[0x14];
    CSWGuiTextParams m_cParams;                     // 0x18
    int m_n6c;                                      // 0x6c
};

// vtable 0x0073e488 (slots 0x00-0x94).
class CSWGuiControl : public CSWGuiObject {
public:
    CSWGuiControl();                                // 0x0041AA80
    virtual ~CSWGuiControl();                                   // 0x00 (0x00419A20)
    virtual int GetHeight();                                    // 0x34
    virtual void Draw(float fDelta);                            // 0x38
    virtual void HandleInputEvent(int nEvent, int bDown);       // 0x3c
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7c();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8c();
    virtual void v90(); virtual void v94();

    CSWGuiControl* m_pParentControl;                // 0x14
    char pad_0x18[0x2c];
    union {
        unsigned long m_nFlags;                     // 0x44
        struct {
            unsigned long m_bFlag0 : 1;             //      bit 0
            unsigned long m_bVisible : 1;           //      bit 1
            unsigned long m_nFlagsRest : 30;
        };
    };
    int m_n48;                                      // 0x48
    int m_n4c;                                      // 0x4c
    char pad_0x50[0xc];

    void SetVisible(int b) { m_bVisible = b; }
    void AddEvent(int nEvent, CSWGuiPanel* pObject,
                  void (CSWGuiPanel::*pfnCallback)(CSWGuiControl*));   // 0x0041AB20
};

class CSWGuiProgressBar : public CSWGuiControl {
public:
    CSWGuiProgressBar();                            // 0x0041AED0
    virtual ~CSWGuiProgressBar();                   // 0x0041AF70
    void SetCurValue(int nValue);                   // 0x00417FB0
    char pad_0x5c[0x150 - 0x5c];
};

// vtable 0x0073e5b8.
class CSWGuiLabel : public CSWGuiControl {
public:
    CSWGuiLabel();                                  // 0x0041ACD0
    virtual ~CSWGuiLabel();                         // 0x00419D10
    CSWGuiBorder m_cBorder;                         // 0x5c
    CSWGuiText m_cText;                             // 0xd0
};

// vtable 0x0073e520.
class CSWGuiNavigable : public CSWGuiControl {
public:
    CSWGuiNavigable();                              // 0x0041B8B0
    int m_aNavigation[4];                           // 0x5c
};

// vtable 0x0073e658.
class CSWGuiButton : public CSWGuiNavigable {
public:
#ifdef M_GUI_BUTTON_INLINE_CTOR
    CSWGuiButton() {}
#else
    CSWGuiButton();                                 // 0x0041C0F0
#endif
    virtual ~CSWGuiButton();                        // 0x00419DA0
    virtual void CopyStyle(const CSWGuiExtent& extent, CSWGuiButton* pFrom); // 0x98
    CSWGuiBorder m_cBorder;                         // 0x6c
    CSWGuiBorder m_cHilight;                        // 0xe0
    CSWGuiText m_cText;                             // 0x154
};

class CSWGuiListBox : public CSWGuiControl {
public:
    CSWGuiListBox();                                // 0x0041BE40
    virtual ~CSWGuiListBox();                       // 0x0041B060
    void ClearControls();                           // 0x00419A90
    CSWGuiControl* GetControl(int nIndex);          // 0x0041A5A0
    void AddControls(const CExoArrayList<CSWGuiControl*>& lstControls, int a, int b, int c); // 0x0041C1D0
    char pad_0x5c[0x28c - 0x5c];
    CSWGuiExtent m_cItemExtent;                     // 0x28c
    char pad_0x29c[0x2a0 - 0x29c];
    int m_nControls;                                // 0x2a0
    char pad_0x2a4[0x2c6 - 0x2a4];
    short m_nSelected;                              // 0x2c6
    char pad_0x2c8[0x2cc - 0x2c8];
    CSWGuiButton* m_pProtoItem;                     // 0x2cc
    char pad_0x2d0[0x2e0 - 0x2d0];
};

class CAurObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10();
    virtual void SetAnimationSpeed(float fSpeed);   // 0x14 (descriptive)
    virtual void PlayAnimation(const char* sAnim, float fSpeed, int a, int b);  // 0x18 (descriptive)
};

class CAurScene {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void SetRoom(const char* sRoom, const Vector& vPosition,
                         const Quaternion& qOrientation);   // 0x70 (descriptive)
};

class CAurCamera {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40();
    virtual void SetFieldOfView(float fFov);        // 0x44 (descriptive)
    virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70();
    virtual void AttachToHook(CAurObject* pObject, const char* sHook, int a);   // 0x74 (descriptive)
};

extern int g_bDo3DGui;                              // 0x0078D1E4 (descriptive)

class CSWGuiScene {
public:
    CSWGuiScene();                                  // 0x004174B0
    virtual ~CSWGuiScene();                         // 0x004165E0
    virtual void v04();
    virtual void v08();
    virtual void Draw(float fDelta);                // 0x0c
    CAurObject* AddModel(const CExoString& sModel, int nPart);  // 0x00417620
    char pad_0x04[0x10];
    CAurScene* m_pScene;                            // 0x14
    CAurCamera* m_pCamera;                          // 0x18
    char pad_0x1c[0x18];
};

// vtable 0x00752E30.
class CSWGui3DSceneView : public CSWGuiControl {
public:
    CSWGui3DSceneView() {}
    virtual ~CSWGui3DSceneView();                   // 0x0067AC50
    virtual void Draw(float fDelta);                // 0x38 (0x0067ACB0)
    CSWGuiScene m_cScene;                           // 0x5c
};

// vtable 0x0073f??? (CSWGuiPanel); ctor 0x0040B570, dtor 0x0040CF70.
class CSWGuiPanel : public CSWGuiObject {
public:
    CSWGuiPanel(CSWGuiManager* pManager);           // 0x0040B570
    virtual ~CSWGuiPanel();                         // 0x0040CF70
    virtual void SetActiveControl(CSWGuiControl* pControl, int bSound);  // 0x08 (0x0040A630)
    virtual void Draw(float fDelta);                // 0x34 (0x0040B760)
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

    void SetBackground(const CResRef& cResRef);     // 0x0040A870
    void StartLoadFromLayout(const CResRef& cResRef);   // 0x0040A680
    void InitControl(CSWGuiControl* pControl, const CExoString& sName, int bAdd);  // 0x0040B930
    void UnloadResource();                          // 0x0040B8F0
    void MarkForDelete() { m_nFlags = (m_nFlags & ~0x300) | 0x400; }
    int IsMarkedForDelete() { return (m_nFlags & 0x600) == 0x400; }

    int m_bUpdateFonts;                             // 0x14
    CSWGuiManager* m_pManager;                      // 0x18
    CSWGuiControl* m_pActiveControl;                // 0x1c
    char pad_0x20[0x24];
    unsigned long m_nFlags;                         // 0x44
    char pad_0x48[0x1c];
};

class CSWGuiMessageBox : public CSWGuiPanel {
public:
    void SetAllowCancel(int bCancel);               // 0x00627130
    void SetCloseCallback(CSWGuiPanel* pObject, void (CSWGuiPanel::*pfnCallback)(CSWGuiControl*),
                          int nArg);                // 0x00624A40
    virtual void SetMessageStrRef(unsigned long nStrRef);   // 0x6c
    virtual void SetMessage(CExoString sMessage);   // 0x70
};

// vtable 0x00752DC0; the destructor is compiler-generated.
class CSWGuiLoadScreen : public CSWGuiPanel {
public:
    CSWGuiLoadScreen(CSWGuiManager* pManager);      // 0x0067A710
    virtual void Draw(float fDelta);                // 0x34 (0x0067AB00)
    void SetImage(const CResRef& cImage);           // 0x0067A6D0
    void SetHint(int nHint);                        // 0x0067A9C0
    void AppendHintToDialogBuffer();                // 0x0067AA80

    CSWGuiProgressBar m_pbProgress;                 // 0x064
    CSWGuiLabel m_lblHint;                          // 0x1b4
    CSWGuiLabel m_lblLogo;                          // 0x2f4
    CSWGuiLabel m_lblLoading;                       // 0x434
    int m_nHint;                                    // 0x574
    CSWGuiLabel m_lblImage;                         // 0x578
};

class CSWGuiClassSelection : public CSWGuiPanel {
public:
    CSWGuiClassSelection(CSWGuiManager* pManager, const CExoString& sModule);   // 0x006DC3C0
    char pad_0x64[0x1560 - 0x64];
};

class CSWGuiSaveLoad : public CSWGuiPanel {
public:
    CSWGuiSaveLoad(CSWGuiManager* pManager, int bSave, int nArg);   // 0x006CC680
    char pad_0x64[0x1160 - 0x64];
};

class CSWGuiTitleMovies : public CSWGuiPanel {
public:
    CSWGuiTitleMovies(CSWGuiManager* pManager);     // 0x006DD910
    char pad_0x64[0x654 - 0x64];
};

class CSWGuiOptionsMain : public CSWGuiPanel {
public:
    CSWGuiOptionsMain(CSWGuiManager* pManager);     // 0x006E3E80
    char pad_0x64[0x1220 - 0x64];
};

// A module list entry: the name plus four words (0x18 bytes); its destructor
// folds into the identical ~CExoString thunk 0x0059EA30.
struct CSWGuiModuleEntry {
    ~CSWGuiModuleEntry();                           // 0x0059EA30
    CExoString m_sName;
    char pad_0x8[0x10];
};

// vtable 0x00752F70.
class CSWGuiMainMenu : public CSWGuiPanel {
public:
    CSWGuiMainMenu(CSWGuiManager* pManager);        // 0x0067C4C0
    virtual ~CSWGuiMainMenu();                      // 0x0067B500
    virtual void Draw(float fDelta);                // 0x34 (0x0067AC10)
    virtual void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x0067B380)
    virtual void OnPanelAdded();                    // 0x48 (0x0067B6C0)

    void LoadFromLayout(CSWGuiManager* pManager);   // 0x0067ACE0
    void OnNewGamePicked(CSWGuiControl* pControl);        // 0x0067AFB0
    void OnLoadSaveGame(CSWGuiControl* pControl);       // 0x0067B1A0
    void OnMoviesPicked(CSWGuiControl* pControl);         // 0x0067B250
    void OnOptionsPicked(CSWGuiControl* pControl);        // 0x0067B2F0
    void OnEnterButton(CSWGuiControl* pControl);    // 0x0067B450
    void OnExitButton(CSWGuiControl* pControl);    // 0x0067B470
    void LaunchDashboard(CSWGuiControl* pControl);  // 0x0067B490
    void OnQuitButtonPressed(CSWGuiControl* pControl);           // 0x0067B4A0
    void ClearModuleNames();                        // 0x0067B930
    void OnModuleSelected(CSWGuiControl* pControl); // 0x0067B9D0
    void PopulateModules();                         // 0x0067BC40
    void ShowWarpList();                            // 0x0067C3F0
    void OnWarpButtonPressed(CSWGuiControl* pControl);           // 0x0067C4B0

    CSWGuiListBox m_lbModules;                      // 0x064
    CExoArrayList<CExoString> m_lstModuleNames;     // 0x344
    int m_nState;                                   // 0x350
    CExoArrayList<CSWGuiModuleEntry> m_lstModules;  // 0x354
    CSWGui3DSceneView m_lbl3DView;                  // 0x360
    CSWGuiButton m_btnNewGame;                      // 0x3f0
    CSWGuiButton m_btnLoadGame;                     // 0x5b4
    CSWGuiButton m_btnMovies;                       // 0x778
    CSWGuiButton m_btnOptions;                      // 0x93c
    CSWGuiButton m_btnB00;                          // 0xb00
    CSWGuiLabel m_lblNewContent;                    // 0xcc4
    CSWGuiLabel m_alblLogo[2];                      // 0xe04 (gamelogo, menubg)
    CSWGuiButton m_btnExit;                         // 0x1084
    CSWGuiButton m_btnWarp;                         // 0x1248
    int m_bActive;                                  // 0x140c
    int m_nDrawCount;                               // 0x1410
};

#endif
