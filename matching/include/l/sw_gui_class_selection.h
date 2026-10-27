// Worker L matching headers: the class selection screen (swguiclassselection.o,
// 0x006DB6D0-0x006DCE80). CSWGuiClassSelection (layout "ClassSel", vtable
// 0x00758020, 0x1560 bytes) and its six character slots CSWGuiClassSelChar
// (descriptive name, 0x25C bytes: button, 3D model view, creature, timer).
// Control/panel layouts are copied (trimmed) from m/sw_gui_main_menu.h. This
// header stands alone: its CSWGuiLabel destructor is inline (the class
// selection destructor inlines the four label destructors).
#ifndef L_SW_GUI_CLASS_SELECTION_H
#define L_SW_GUI_CLASS_SELECTION_H

#ifdef L_CLASS_SELECTION_DTORS
#define L_CLASSSEL_VTABLE __declspec(novtable)
#else
#define L_CLASSSEL_VTABLE
#endif

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
    void __cdecl Format(char* sFormat, ...);        // 0x005E5680
    int GetLength() const;                          // 0x005E5790
};

class CResRef {
public:
    CResRef(const char* s);                         // 0x00406D80
    CResRef(const CExoString& s);                   // 0x00406D60
    char m_resRef[16];
};

void* operator new(unsigned int nSize);
void operator delete(void* p);

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
    void Allocate(int nSize);                       // 0x005E03C0 (int/ulong instance)
    void Add(T t)
    {
        if (num == array_size) {
            Allocate(array_size ? array_size * 2 : 16);
        }
        int n = num;
        element[n] = t;
        num = n + 1;
    }
    void DelIndex(int nIndex)
    {
        num--;
        for (; nIndex < num; nIndex++) {
            element[nIndex] = element[nIndex + 1];
        }
    }
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

extern Vector g_vDifficultyExitColor;               // 0x007A23B4
extern Vector g_vAbilityColorGranted;               // 0x007A23C0

// Engine frame step (descriptive name of 0x00401C10).
void __cdecl RunFrame(float fDelta, int bRunServer, int bKeepFrame);  // 0x00401C10

class CExoResMan {
public:
    int Exists(const CResRef& cResRef, unsigned short nType, unsigned long* pSize); // 0x00408BC0
    int AddResourceImageFile(const CExoString& sFile);      // 0x004087C0
    int CleanDirectory(const CExoString& sAlias, int bDirs, int bRecurse);   // 0x00409460
    char pad_0x0[0x34];
    unsigned char m_bFlag0 : 1;                     // 0x34 bit 0 (descriptive)
};

extern CExoResMan* g_pExoResMan;                    // 0x007A39E8

class CExoInput {
public:
    void CoolDownEvent(unsigned long nEvent, unsigned long nMilliseconds);  // 0x005DF4B0
};

extern CExoInput* g_pExoInput;                      // 0x007A39E4

class CTlkTable {
public:
    CExoString GetSimpleString(unsigned long nStrRef);  // 0x0041E8F0
};

extern CTlkTable* g_pTlkTable;                      // 0x007A3A08

class C2DA {
public:
    C2DA(CResRef cResRef, int bPrivate);            // 0x00413CC0
    virtual ~C2DA();
    int Load2DArray();                              // 0x004143B0
    int GetINTEntry(int nRow, const CExoString& sColumn, int* pValue);  // 0x00413660
    char pad_0x4[0x20];
    int m_nNumRows;                                 // 0x24
    char pad_0x28[0x54 - 0x28];
};

class CGuiInGame {
public:
    void AdjustHideCount(int nDelta);               // 0x0062B410
};

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

class CClientExoApp {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual CNetLayer* GetNetLayer();               // 0x10
    CGuiInGame* GetGuiInGame();                     // 0x005ED690
    void BusyWaitMenuMusic();                       // 0x005EE120
    CExoString GetClientName();                     // 0x005ED490
    CSWCMessage* GetMessage();                      // 0x005ED6F0
    void SetPlayerCharacterName(const CExoString& sName);   // 0x005EDAA0
    int SetInputClass(int nClass, int bForce);      // 0x005EDA60
    CExoString GetLoadMusicByModuleName(const CExoString& sModule);   // 0x005EDD00
    void SetLoadMusic(const CExoString& sMusic);    // 0x005EDD30
    void SetLoadScreenByModuleName(const CExoString& sModule);  // 0x005EDCF0
    void ShowLoadScreen(int a, int b, int c);       // 0x005EDA80
    void HideLoadScreen(int a, int b);              // 0x005EDA90
    int GetNextLoadScreenHintSTRREF(const CExoString& sModule, unsigned char nType); // 0x005EE090
    void SetLoadScreenHint(int nHint);              // 0x005EDF00
    void SetLoadBarProgress(int nProgress, int bDraw);  // 0x005EDD40
    void StartMenuMusic(int a, int nVolume, int c); // 0x005ED9B0
    void DisplayMainMenu(int bShow);                // 0x005ED420
};

class CAppManager {
public:
    void CreateServer(int bLoad);                   // 0x00401380
    void* m_p00;
    CClientExoApp* m_pClientExoApp;                 // 0x04
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CAurObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7c();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8c();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9c();
    virtual void vA0(); virtual void vA4(); virtual void vA8(); virtual void vAC();
    virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC();
    virtual void vC0(); virtual void vC4(); virtual void vC8(); virtual void vCC();
    virtual void vD0(); virtual void vD4(); virtual void vD8(); virtual void vDC();
    virtual void vE0(); virtual void vE4(); virtual void vE8(); virtual void vEC();
    virtual void vF0(); virtual void vF4(); virtual void vF8(); virtual void vFC();
    virtual void v100();
    virtual class CAurScene* GetScene();            // 0x104 (descriptive)
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

class CSWCAnimBase {
public:
    virtual void v00(); virtual void v04();
    virtual CAurObject* GetModel(unsigned char nPart);  // 0x08 (descriptive)
};

class CSWCCreatureStats {
public:
    void SetClass(unsigned char nIndex, unsigned char nClass);  // 0x00647770
    char pad_0x0[0x31];
    unsigned char m_nGender;                        // 0x31
    char pad_0x32[0x5c - 0x32];
    unsigned char m_n5c;                            // 0x5c
    unsigned char m_n5d;                            // 0x5d
    unsigned char m_n5e;                            // 0x5e
    char pad_0x5f[0x8c - 0x5f];
    unsigned short m_nAppearance;                   // 0x8c
    char pad_0x8e[0x120 - 0x8e];
    unsigned short m_n120;                          // 0x120
};

// Creature appearance block (0x3C bytes, +0x21C in the creature).
struct CSWCCreatureAppearance {
    unsigned char m_nType;                          // 0x00
    unsigned char m_n01;                            // 0x01
    char pad_0x2[0x14 - 0x2];
    int m_n14;                                      // 0x14
    unsigned short m_nAppearance;                   // 0x18
    unsigned char m_n1a;                            // 0x1a
    unsigned char m_nGender;                        // 0x1b
    char pad_0x1c[0x3c - 0x1c];
};

class CSWCCreature {
public:
    CSWCCreature();                                 // 0x00616A20
    virtual ~CSWCCreature();                        // 0x00
    virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7c();
    virtual void v80(); virtual void v84(); virtual void v88();
    virtual void SetPosition(const Vector& vPosition);  // 0x8c
    virtual void v90();
    virtual void AttachToScene(CAurScene* pScene);  // 0x94
    virtual void v98(); virtual void v9c();
    virtual void vA0(); virtual void vA4(); virtual void vA8(); virtual void vAC();
    virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC();
    virtual void vC0(); virtual void vC4(); virtual void vC8(); virtual void vCC();
    virtual void vD0(); virtual void vD4(); virtual void vD8(); virtual void vDC();
    virtual void vE0(); virtual void vE4(); virtual void vE8(); virtual void vEC();
    virtual void vF0();
    virtual void SetPortraitId(unsigned short nPortrait);   // 0xf4

    CSWCAnimBase* GetAnimBase();                    // 0x0063C070
    void SetSoundSet(unsigned short nSoundSet);     // 0x0060B7C0
    void SetAppearance(void* pAppearance, int nPart, int bLoad);    // 0x006134C0
    void CGPauseCycle(float fDelta, int bReset);    // 0x0060F7C0
    void SaveCharGenCreature();                     // 0x006123E0

    char pad_0x4[0x21c - 0x4];
    CSWCCreatureAppearance* m_pAppearance;          // 0x21c
    char pad_0x220[0x2f8 - 0x220];
    CSWCCreatureStats* m_pStats;                    // 0x2f8
    char pad_0x2fc[0x44c - 0x2fc];
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
    void PlayGuiSound(char nSound);                 // 0x0040A140
    void AddPanel(CSWGuiPanel* pPanel, int nFlags, int bPlaySound);   // 0x0040BC70
};

class CSWGuiTextParams {
public:
    void SetText(const CExoString& sText);          // 0x00415E00
    void SetStrRef(unsigned long nStrRef);          // 0x00415E50
    CExoString m_sText;                             // 0x00
    char pad_0x08[0x4c];
};

class CSWGuiBorderParams {
public:
    void SetFlags(int b2, int b3, int b4);          // 0x00414D00
    char pad_0x00[0x10];
    Vector m_vColor;                                // 0x10
    char pad_0x1c[0x44];
};

class CSWGuiBorder {
public:
    CSWGuiBorder();                                 // 0x004167F0
    virtual ~CSWGuiBorder();                        // 0x004151C0
    CSWGuiExtent m_extent;                          // 0x04
    CSWGuiBorderParams m_cParams;                   // 0x14
};

class CSWGuiText {
public:
    CSWGuiText();                                   // 0x00417280
    virtual ~CSWGuiText();                          // 0x00415FF0
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
            unsigned long m_bFlag0 : 1;             //      bit 0 (selected)
            unsigned long m_bVisible : 1;           //      bit 1
            unsigned long m_bNavigable : 1;         //      bit 2
            unsigned long m_nFlagsRest : 29;
        };
    };
    int m_nEvent;                                   // 0x48
    int m_n4c;                                      // 0x4c
    int m_n50;                                      // 0x50
    unsigned char m_nSound;                         // 0x54
    char pad_0x55[0x7];

    void AddEvent(int nEvent, CSWGuiPanel* pObject,
                  void (CSWGuiPanel::*pfnCallback)(CSWGuiControl*));   // 0x0041AB20
};

// vtable 0x0073e5b8; the class selection destructor inlines ~CSWGuiLabel.
class CSWGuiLabel : public CSWGuiControl {
public:
    CSWGuiLabel();                                  // 0x0041ACD0
    virtual ~CSWGuiLabel() {}                       // 0x00419D10
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
    CSWGuiButton();                                 // 0x0041C0F0
    virtual ~CSWGuiButton();                        // 0x00419DA0
    CSWGuiBorder m_cBorder;                         // 0x6c
    CSWGuiBorder m_cHilight;                        // 0xe0
    CSWGuiText m_cText;                             // 0x154
};

extern int g_bDo3DGui;                              // 0x0078D1E4 (descriptive)

class CSWGuiScene {
public:
    CSWGuiScene();                                  // 0x004174B0
    virtual ~CSWGuiScene();                         // 0x004165E0
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

// vtable 0x0073e010; ctor 0x0040B570, dtor 0x0040CF70.
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
    virtual void OnAButtonPressed(CSWGuiControl* pControl);     // 0x50
    virtual void OnBButtonPressed(CSWGuiControl* pControl);     // 0x54
    virtual void v58(); virtual void v5c();
    virtual void v60();
    virtual CExoString GetFullScreenBG();           // 0x64
    virtual void UpdateFonts();                     // 0x68

    void StartLoadFromLayout(const CResRef& cResRef);   // 0x0040A680
    void InitControl(CSWGuiControl* pControl, const CExoString& sName, int bAdd);  // 0x0040B930
    void UnloadResource();                          // 0x0040B8F0
    void MarkForDelete() { m_nFlags = (m_nFlags & ~0x300) | 0x400; }

    int m_bUpdateFonts;                             // 0x14
    CSWGuiManager* m_pManager;                      // 0x18
    CSWGuiControl* m_pActiveControl;                // 0x1c
    char pad_0x20[0x24];
    union {
        unsigned long m_nFlags;                     // 0x44
        struct {
            unsigned long m_nFlagsLow : 7;
            unsigned long m_bFlag7 : 1;             //      bit 7 (descriptive)
            unsigned long m_nFlagsHigh : 24;
        };
    };
    char pad_0x48[0x1c];
};

// The six class choices (0x007A2684, 8 bytes each).
struct CSWGuiClassChoice {
    unsigned char m_nClass;                         // 0x0
    unsigned char m_nGender;                        // 0x1
    unsigned char m_nSoundSet;                      // 0x2
    unsigned char m_n3;                             // 0x3
    unsigned long m_nDescription;                   // 0x4 strref
};

extern CSWGuiClassChoice g_aClassChoices[6];        // 0x007A2684 (descriptive)
extern const unsigned char g_nClassScout;           // 0x0074B31A (1, descriptive)
extern const unsigned char g_nClassScoundrel;       // 0x0074B31B (2, descriptive)

class CSWGuiClassSelection;

class CSWGuiMainCharGen : public CSWGuiPanel {
public:
    CSWGuiMainCharGen(CSWGuiManager* pManager, CSWCCreature* pCreature,
                      CSWGuiClassSelection* pClassSel);   // 0x006EB420
    char pad_0x64[0x22dc - 0x64];
};

// One class choice (descriptive): its button, 3D model view, creature and
// the zoom timer. Its ctor and dtor have empty bodies (member-wise only).
struct CSWGuiClassSelChar {
    CSWGuiClassSelChar();                           // 0x006DC350
    ~CSWGuiClassSelChar();                          // 0x006DB6D0
    CSWGuiButton m_btnSelect;                       // 0x000 BTN_SEL%d
    CSWGui3DSceneView m_cModel;                     // 0x1c4 3D_MODEL%d
    CSWCCreature* m_pCreature;                      // 0x254
    float m_fTimer;                                 // 0x258
};

class L_CLASSSEL_VTABLE CSWGuiClassSelection : public CSWGuiPanel {
public:
    CSWGuiClassSelection(CSWGuiManager* pManager, const CExoString& sModule);   // 0x006DC3C0
    virtual ~CSWGuiClassSelection();                // 0x006DB720
    virtual void Draw(float fDelta);                // 0x34 (0x006DC030)
    virtual void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006DBD30)
    virtual void OnPanelAdded();                    // 0x48 (0x006DB8F0)

    void OnSelectButton(CSWGuiControl* pControl);   // 0x006DB9B0
    void OnEnterButton(CSWGuiControl* pControl);    // 0x006DBA70
    void BeginPlayGame();                           // 0x006DBDF0

    int m_n64;                                      // 0x64
    CSWCCreature* m_pSelected;                      // 0x68
    CSWGuiClassSelChar m_aChars[6];                 // 0x6c
    CSWGuiLabel m_lblCharGen;                       // 0xe94 LBL_CHAR_GEN
    CSWGuiLabel m_lblInstruction;                   // 0xfd4 LBL_INSTRUCTION
    CSWGuiLabel m_lblDesc;                          // 0x1114 LBL_DESC
    CSWGuiLabel m_lblClass;                         // 0x1254 LBL_CLASS
    CSWGuiButton m_btnBack;                         // 0x1394 BTN_BACK
    CExoString m_sModule;                           // 0x1558
};

#endif
