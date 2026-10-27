// Worker M matching header: the in-game quest journal panel
// CSWGuiInGameJournal (0xFC8 bytes, vtable 0x00751960), 0x00644720-0x00646069.
// Control/panel layouts are copied (trimmed) from m/sw_gui_main_menu.h, which
// follows d/gui_control.h; member offsets were checked against the playable
// reference (src/units/1407 panels_menus, src/units/1746 claude_gui_journal).
#ifndef M_SW_GUI_IN_GAME_JOURNAL_H
#define M_SW_GUI_IN_GAME_JOURNAL_H

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
    int GetLength() const;                          // 0x005E5790
};

class CResRef {
public:
    CResRef(const char* s);                         // 0x00406D80
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
    void AddUnique(T t);                            // 0x006449D0 (unsigned long)
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
};

struct CSWGuiExtent {
    int left;
    int top;
    int width;
    int height;
};

class Vector {
public:
    float x, y, z;
};

extern Vector g_vDifficultyExitColor;               // 0x007A23B4
extern Vector g_vAbilityColorGranted;               // 0x007A23C0

class CTlkTable {
public:
    CExoString GetSimpleString(unsigned long nStrRef);  // 0x0041E8F0
};

extern CTlkTable* g_pTlkTable;                      // 0x007A3A08

class CExoLocString {
public:
    int GetString(unsigned long nLanguage, CExoString* pString, unsigned char nGender) const; // 0x005EA130
    void* m_pList;
    void* m_pPad;
};

class C2DA {
public:
    int GetINTEntry(int nRow, const CExoString& sColumn, int* pValue);  // 0x00413660
};

class C2DAs {
public:
    char pad_0x0[0x104];
    C2DA* m_pPlanets;                               // 0x104 (planets.2da)
    C2DA* GetPlanets() { return m_pPlanets; }
};

class CSWRules {
public:
    char pad_0x0[0xb8];
    C2DAs* m_p2DArrays;                             // 0xb8
    C2DAs* Get2DArrays() { return m_p2DArrays; }
};

extern CSWRules* g_pRules;                          // 0x007A3A28

// A quest journal entry as returned by CSWGuiInGameJournal::GetQuestEntryAt.
struct SJournalEntry {
    CExoLocString m_sName;                          // 0x00
    CExoLocString m_sText;                          // 0x08
    char pad_0x10[0x2c - 0x10];
    unsigned long m_bQuestCompleted : 1;            // 0x2c bit 0
    unsigned long m_bQuestDisplayed : 1;            //      bit 1
    unsigned long m_bUpdated : 1;                   //      bit 2
    char pad_0x30[4];
    int m_nPlanet;                                  // 0x34 (planets.2da row, -1 if none)
};

class CSWCJournal {
public:
    unsigned long GetDoneSortBy();                  // 0x00644760 (folded getter)
    unsigned long GetActiveSortBy();                // 0x00712FC0 (folded getter)
    void SetActiveSortBy(unsigned long nSort);      // 0x00676BC0
    void SetDoneSortBy(unsigned long nSort);        // 0x00676BF0
    void ClearActiveListChanged();                  // 0x00644770
    void ClearDoneListChanged();                    // 0x00644780
};

class CGuiInGame {
public:
    void SetJournalOption(unsigned long nValue);    // 0x00644740
    int HideSWInGameGui(int bArg);                  // 0x0062CBA0
    char pad_0x0[0xbc4];
    unsigned long m_bJournalDone : 1;               // 0xbc4 bit 0
};

class CClientExoApp {
public:
    CSWCJournal* GetQuestJournal();                 // 0x005ED320
    CGuiInGame* GetInGameGui();                     // 0x005ED690
    unsigned long GetClientLanguage();              // 0x005ED4C0
    int SetInputClass(int nClass, int bForce);      // 0x005EDA60
};

class CAppManager {
public:
    void* m_p00;
    CClientExoApp* m_pClientExoApp;                 // 0x04
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

extern int g_nJournalSortOrder;                     // 0x00833A90
extern int g_bJournalUpdated;                       // 0x00833A94
extern unsigned long g_anJournalSortStrRefs[4];     // 0x007A2474 (descriptive)

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
    char pad_0x00[0x0c];
    float m_fAlpha;                                 // 0x0c
    Vector m_vColor;                                // 0x10
    char pad_0x1c[0x44];
};

class CSWGuiBorder {
public:
    virtual ~CSWGuiBorder();
    CSWGuiExtent m_extent;                          // 0x04
    CSWGuiBorderParams m_cParams;                   // 0x14
};

class CSWGuiText {
public:
    virtual ~CSWGuiText();
    virtual void v04();
    virtual int GetHeight();                        // 0x08
    int GetFontHeight();                            // 0x00414EE0
    char pad_0x04[0x14];
    CSWGuiTextParams m_cParams;                     // 0x18
    int m_n6c;                                      // 0x6c
};

// vtable 0x0073e488 (slots 0x00-0x94).
class CSWGuiControl : public CSWGuiObject {
public:
    virtual ~CSWGuiControl();                                   // 0x00 (0x00419A20)
    virtual int GetHeight();                                    // 0x34
    virtual void Draw(float fDelta);                            // 0x38
    virtual void HandleInputEvent(int nEvent, int bDown);       // 0x3c
    virtual void SetActive(int bActive);                        // 0x40 (descriptive)
    virtual void v44(); virtual void v48(); virtual void v4c();
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
            unsigned long m_bSelectable : 1;        //      bit 2
            unsigned long m_nFlagsRest : 29;
        };
    };
    int m_n48;                                      // 0x48
    int m_n4c;                                      // 0x4c
    char pad_0x50[0xc];

    void AddEvent(int nEvent, CSWGuiPanel* pObject,
                  void (CSWGuiPanel::*pfnCallback)(CSWGuiControl*));   // 0x0041AB20
};

// vtable 0x0073e5b8.
class CSWGuiLabel : public CSWGuiControl {
public:
    CSWGuiLabel();                                  // 0x0041ACD0
    virtual ~CSWGuiLabel();                         // 0x00419D10
    virtual void CopyStyle(const CSWGuiExtent& extent, CSWGuiControl* pFrom, float fScale); // 0x98
    CSWGuiBorder m_cBorder;                         // 0x5c
    CSWGuiText m_cText;                             // 0xd0
};

// vtable 0x0073e520.
class CSWGuiNavigable : public CSWGuiControl {
public:
    int m_aNavigation[4];                           // 0x5c
};

// vtable 0x0073e658.
class CSWGuiButton : public CSWGuiNavigable {
public:
    CSWGuiButton();                                 // 0x0041C0F0
    virtual ~CSWGuiButton();                        // 0x00419DA0
    virtual void CopyStyle(const CSWGuiExtent& extent, CSWGuiControl* pFrom); // 0x98
    CSWGuiBorder m_cBorder;                         // 0x6c
    CSWGuiBorder m_cHilight;                        // 0xe0
    CSWGuiText m_cText;                             // 0x154
};

class CSWGuiListBox : public CSWGuiControl {
public:
    CSWGuiListBox();                                // 0x0041BE40
    virtual ~CSWGuiListBox();                       // 0x0041B060
    virtual void v98(); virtual void v9c(); virtual void va0();
    virtual void SetHilightable(int bHilight);      // 0xa4
    void ClearControls();                           // 0x00419A90
    void AddControls(const CExoArrayList<CSWGuiControl*>& lstControls, int a, int b, int c); // 0x0041C1D0
    void AddControls(CSWGuiControl** pControls, int nCount, int a, int b, int c); // 0x0041C2C0
    void SelectControl(CSWGuiControl* pControl, int bArg);  // 0x0041C160
    void SetSelectedControl(int nIndex, int bArg);  // 0x0041C040
    char pad_0x5c[0x28c - 0x5c];
    CSWGuiExtent m_cItemExtent;                     // 0x28c
    char pad_0x29c[0x2bc - 0x29c];
    unsigned long m_nListFlags;                     // 0x2bc
    unsigned char m_nBorder;                        // 0x2c0
    char pad_0x2c1[0x2cc - 0x2c1];
    CSWGuiControl* m_pProtoItem;                    // 0x2cc
    char pad_0x2d0[0x2e0 - 0x2d0];
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
    virtual void OnBButtonPressed(CSWGuiControl* pControl);     // 0x54 (descriptive)
    virtual void OnXButtonPressed(CSWGuiControl* pControl);     // 0x58 (descriptive)
    virtual void OnYButtonPressed(CSWGuiControl* pControl);     // 0x5c (descriptive)
    virtual void OnBlackButtonPressed(CSWGuiControl* pControl); // 0x60 (descriptive)
    virtual CExoString GetFullScreenBG();           // 0x64
    virtual void UpdateFonts();                     // 0x68

    void StartLoadFromLayout(const CResRef& cResRef);   // 0x0040A680
    void InitControl(CSWGuiControl* pControl, const CExoString& sName, int bAdd);  // 0x0040B930
    void UnloadResource();                          // 0x0040B8F0

    int m_bUpdateFonts;                             // 0x14
    CSWGuiManager* m_pManager;                      // 0x18
    CSWGuiControl* m_pActiveControl;                // 0x1c
    char pad_0x20[0x24];
    unsigned long m_nFlags;                         // 0x44
    char pad_0x48[0x1c];
};

class CSWGuiQuestItem : public CSWGuiPanel {
public:
    CSWGuiQuestItem(CSWGuiManager* pManager);       // 0x006D26C0
    virtual void v6c(); virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7c();
    char pad_0x64[0xa70 - 0x64];
};

// vtable 0x007518C0; one 0x1CC-byte list entry per quest.
class CSWGuiJournalItemEntry : public CSWGuiButton {
public:
    CSWGuiJournalItemEntry()
    {
        m_nIndex = 0;
        m_bEncouraged = 0;
    }
    void SetEncouraged(int bEncouraged);            // 0x00644790
    int m_nIndex;                                   // 0x1c4
    unsigned long m_bEncouraged : 1;                // 0x1c8 bit 0
};

// vtable 0x00751960.
class CSWGuiInGameJournal : public CSWGuiPanel {
public:
    CSWGuiInGameJournal(CSWGuiManager* pManager);   // 0x00644A40
    virtual ~CSWGuiInGameJournal();                 // 0x00644850
    virtual void Draw(float fDelta);                // 0x34 (0x00645670)
    virtual void HandleInputEvent(int nEvent, int bDown);   // 0x3c (0x006456E0)
    virtual void OnPanelAdded();                    // 0x48 (0x00645E00)
    virtual void OnPanelRemoved();                  // 0x4c (0x00646070)
    virtual SJournalEntry* GetQuestEntryAt(int nIndex);     // 0x6c (0x00644960)

    void OnControlEntered(CSWGuiControl* pControl); // 0x00645100
    void PopulateItemListBox();                     // 0x00645330
    void MarkNewQuestsObserved();                   // 0x00645610

    CSWGuiLabel m_lblTitle;                         // 0x064
    CSWGuiListBox m_lbDescription;                  // 0x1a4
    CSWGuiLabel m_lblDescription;                   // 0x484
    CSWGuiListBox m_lbItems;                        // 0x5c4
    CSWGuiButton m_btnQuestItems;                   // 0x8a4
    CSWGuiButton m_btnSwapText;                     // 0xa68
    CSWGuiButton m_btnSort;                         // 0xc2c
    CSWGuiButton m_btnExit;                         // 0xdf0
    CSWGuiQuestItem* m_pQuestItem;                  // 0xfb4
    CSWGuiControl* m_pFB8;                          // 0xfb8 (unknown control, never set here)
    CExoArrayList<unsigned long> m_lstViewed;       // 0xfbc
};

#endif
