// Worker L matching headers: the title screen movie list CSWGuiTitleMovies
// (layout "titlemovie", vtable 0x00758130, 0x654 bytes; 0x006DCE80-0x006DDB08),
// opened from CSWGuiMainMenu::OnMoviesPicked. Control/panel layouts are
// copied (trimmed) from m/sw_gui_main_menu.h and l/sw_gui_class_selection.h.
// This header stands alone: its CSWGuiLabel destructor is inline (the panel
// destructor inlines it).
#ifndef L_SW_GUI_TITLE_MOVIES_H
#define L_SW_GUI_TITLE_MOVIES_H

class CExoString {
public:
    char* m_sString;
    int m_nBufferLength;
    CExoString();                                   // 0x005B3190
    CExoString(const char* sString);                // 0x005E5A90
    CExoString(const CExoString& sString);          // 0x005E5B00
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    CExoString operator+(const CExoString& s) const;    // 0x005E5D10
    void __cdecl Format(char* sFormat, ...);        // 0x005E5680
    int GetLength() const;                          // 0x005E5790
    CExoString SubString(int nStart, int nCount) const; // 0x005E6270
};

class CResRef {
public:
    CResRef(const char* s);                         // 0x00406D80
    CResRef(const CExoString& s);                   // 0x00406D60
    char m_resRef[16];
};

void* operator new(unsigned int nSize);
void operator delete(void* p);
// No operator new[]/delete[] declarations: without them this compiler lowers
// new T[n] / delete[] p (trivial T) to ??2/??3, as the original does; with
// them it calls ??_U/??_V (0x006FC0D2/0x006FC0CD) instead.

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
    void Insert(T t, int nIndex);                   // 0x005E8C60 (CExoString)
    void Allocate(int nSize);                       // 0x005E03C0 (folded)
    void SetSize(int nSize)
    {
        if (nSize == 0) {
            T* pArray = element;
            num = 0;
            if (pArray) {
                delete[] pArray;
                element = 0;
                array_size = 0;
            }
        } else {
            Allocate(nSize);
        }
        num = nSize;
    }
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
    __forceinline void InsertInline(T t, int nIndex)
    {
        int n = num;
        if (n == array_size) {
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
        element[num] = t;
        num++;
        for (; n > nIndex; n--) {
            element[n] = element[n - 1];
        }
        element[nIndex] = t;
    }
};

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
};

extern Vector g_vPazaakButtonHilight;               // 0x007A23B4
extern Vector g_vAbilityColorGranted;               // 0x007A23C0

class CTlkTable {
public:
    CExoString GetSimpleString(unsigned long nStrRef);  // 0x0041E8F0
};

extern CTlkTable* g_pTlkTable;                      // 0x007A3A08

class C2DA {
public:
    int GetINTEntry(const CExoString& sRow, const CExoString& sColumn, int* pValue);  // 0x00414110
};

class CTwoDimArrays {
public:
    C2DA* GetMovies() { return m_pMovies; }
    char pad_0x0[0x11c];
    C2DA* m_pMovies;                                // 0x11c movies.2da (c names it m_pTexturePacks)
};

class CSWRules {
public:
    CTwoDimArrays* Get2DArrays() { return m_p2DArrays; }
    char pad_0x0[0xb8];
    CTwoDimArrays* m_p2DArrays;                     // 0xb8
};

extern CSWRules* g_pRules;                          // 0x007A3A28

class CClientOptions {
public:
    // c names 0x0061D3D0 GetTexturePackBit; it reads the movies.2da row bit.
    int GetMovieShown(const CResRef& cMovie);       // 0x0061D3D0
};

class CClientExoApp {
public:
    CClientOptions* GetClientOptions();             // 0x005ED700
    void StartMenuMusic(int a, int nVolume, int c); // 0x005ED9B0
    void SetStartMenuMusicOnMovieEnd(int b);        // 0x005ED9E0
    void PlayMovie(const CExoString& sMovie, int* pResult, int a3, int a4);  // 0x005EDB30
    void StartCreditSequence(const Vector& vColor, const CExoString& sMovie);  // 0x005EE1C0
};

class CAppManager {
public:
    void* m_p00;
    CClientExoApp* m_pClientExoApp;                 // 0x04
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CSWGuiControl;
class CSWGuiPanel;
class CSWGuiButton;

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
    CSWGuiPanel* PopModalPanel();                   // 0x0040BE00
};

class CSWGuiTextParams {
public:
    void SetText(const CExoString& sText);          // 0x00415E00
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
    virtual void v50(); virtual void v54();
    virtual CSWGuiButton* GetButton();                          // 0x58 (descriptive)
    virtual void v5c();
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
    int m_n48;                                      // 0x48
    int m_n4c;                                      // 0x4c
    int m_n50;                                      // 0x50
    char pad_0x54[0x4];
    void* m_pUserData;                              // 0x58 (descriptive)

    void AddEvent(int nEvent, CSWGuiPanel* pObject,
                  void (CSWGuiPanel::*pfnCallback)(CSWGuiControl*));   // 0x0041AB20
};

// vtable 0x0073e5b8; the panel destructor inlines ~CSWGuiLabel.
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
#ifdef L_GUI_BUTTON_INLINE_CTOR
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
    CSWGuiControl* GetControl(int nIndex);          // 0x0041A5A0
    void AddControls(const CExoArrayList<CSWGuiControl*>& lstControls, int a, int b, int c); // 0x0041C1D0
    void ClearControls();                           // 0x00419A90
    char pad_0x5c[0x28c - 0x5c];
    CSWGuiExtent m_cItemExtent;                     // 0x28c
    char pad_0x29c[0x2a0 - 0x29c];
    int m_nControls;                                // 0x2a0
    char pad_0x2a4[0x2cc - 0x2a4];
    CSWGuiButton* m_pProtoItem;                     // 0x2cc
    char pad_0x2d0[0x2e0 - 0x2d0];
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
    unsigned long m_nFlags;                         // 0x44
    char pad_0x48[0x1c];
};

typedef void (CSWGuiPanel::*PFN_GUI_CALLBACK)(CSWGuiControl*);

// vtable 0x00758130.
class CSWGuiTitleMovies : public CSWGuiPanel {
public:
    CSWGuiTitleMovies(CSWGuiManager* pManager);     // 0x006DD910
    virtual ~CSWGuiTitleMovies();                   // 0x006DCFE0
    virtual void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006DCE80)

    void OnMoviePicked(CSWGuiControl* pControl);    // 0x006DCF10
    void PopulateMovies();                          // 0x006DD100

    CSWGuiListBox m_lbMovies;                       // 0x064 LB_MOVIES
    CSWGuiLabel m_lblTitle;                         // 0x344 LBL_TITLE
    CSWGuiButton m_btnBack;                         // 0x484 BTN_BACK
    CExoArrayList<CExoString> m_lstMovieNames;      // 0x648
};

#endif
