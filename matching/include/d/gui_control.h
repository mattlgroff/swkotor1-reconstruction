// CSWGuiControl and the control classes (0x00418010-0x0041d5b0).
// Layouts from include/kotor/b0041_gui_controls.h and the constructors /
// destructors (x86 confirmed). Standalone header (does not include d/gui.h:
// its CSWGuiControl is a slot-only stub).
#ifndef D_GUI_CONTROL_H
#define D_GUI_CONTROL_H

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
    char* CStr() const;                             // 0x005e5c40
};

struct CResStruct {
    unsigned long m_nStructIndex;
};

class CResGFF {
public:
    int GetStructFromStruct(CResStruct* pOut, CResStruct* pStruct, char* szLabel); // 0x00411a10
    unsigned char ReadFieldBYTE(CResStruct* pStruct, char* szLabel, int& bSuccess, unsigned char nDefault); // 0x00411a60
    int ReadFieldINT(CResStruct* pStruct, char* szLabel, int& bSuccess, int nDefault); // 0x00411c90
};

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList(int nSize = 0);
    ~CExoArrayList();                               // 0x0056b6f0
    void Allocate(int nSize);
    void Add(T t);
    void DelIndex(int i);
    int IndexOf(T t);
    T& operator[](int i) { return element[i]; }
};

struct CSWGuiExtent {
    int left;
    int top;
    int width;
    int height;
    CSWGuiExtent() { left = top = width = height = 0; }
    void Load(CResGFF* pGFF, CResStruct* pStruct);  // 0x00409dc0
};

class CSWGuiControl;
class CSWGuiListBox;
class CSWGuiPanel;

// vtable 0x0073df20 (slots 0x00-0x30).
class CSWGuiObject {
public:
    CSWGuiObject() {}
    virtual ~CSWGuiObject() {}                                  // 0x00
    virtual void SetExtent(const CSWGuiExtent& extent);         // 0x04 (0x00409e90)
    virtual void SetActiveControl(CSWGuiControl* pControl, int bSound); // 0x08
    virtual void HandleMouseOver(int n);                        // 0x0c
    virtual int v10(int a, int b);                              // 0x10
    virtual void OnCaptureLost();                               // 0x14
    virtual void HandleLMouseDown();                            // 0x18
    virtual void HandleLMouseUp();                              // 0x1c
    virtual void v20();
    virtual void HandleRMouseUp();                              // 0x24
    virtual CSWGuiPanel* GetPanel();                            // 0x28
    virtual CSWGuiListBox* GetListBox();                        // 0x2c
    virtual void v30();
    CSWGuiExtent m_extent;          // 0x04
};

class CSWGuiManager;

class CSWGuiPanel : public CSWGuiObject {
public:
    virtual void v34(); virtual void v38();
    virtual void HandleInputEvent(int nEvent, int bDown);       // 0x3c
    CSWGuiControl* GetControl(int nIndex);          // 0x0040ba00
    void GetLocalMouseCoords(int* pX, int* pY);     // 0x0040ba20
    int m_bUpdateFonts;                             // 0x14
    CSWGuiManager* m_pManager;                      // 0x18
};

// 12-byte entry of the control event list (+0x38): a panel member callback.
typedef void (CSWGuiPanel::*PFN_GUI_CALLBACK)(CSWGuiControl* pControl);
struct CSWGuiControlEvent {
    CSWGuiPanel* m_pObject;
    PFN_GUI_CALLBACK m_pfnCallback;
    int m_nEvent;
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

class CResRef {
public:
    CResRef();                                      // 0x00405ed0
    char m_resRef[16];
};

class CSWGuiImageParams {
public:
    CSWGuiImageParams(const CResRef& cResRef, int b1, int n2, float f10, int n6, float f14,
                      const Vector& vColor);        // 0x00414d30
    void SetImage(const CResRef& cResRef, int bForce); // 0x00415900
    CResRef m_cResRef;                              // 0x00
    float m_f10;                                    // 0x10
    float m_fAlpha;                                 // 0x14
    char pad_0x18[0x0c];
    unsigned long m_b0 : 1;                         // 0x24 bit 0
    unsigned long m_b1 : 1;                         //      bit 1
    unsigned long m_nOrient : 4;                    //      bits 2-5
    unsigned long m_n6 : 6;                         //      bits 6-11
    void* m_pImage;                                 // 0x28
};

extern Vector g_vDisabledColor;                 // 0x0078d3b4
extern Vector g_vHilightColor;                  // 0x0078d3c0

class CSWGuiManager {
public:
    int m_nMouseX;                                  // 0x00
    int m_nMouseY;                                  // 0x04
    CSWGuiControl* m_pMouseOverControl;             // 0x08
    unsigned long m_bMouseDown : 1;                 // 0x0c bit 0
    CSWGuiControl* m_pCaptureControl;               // 0x10
    char pad_0x14[0x04];
    CSWGuiControl* m_pFocusControl;                 // 0x18
    unsigned long m_bLeftDown : 1;                  // 0x1c bit 0
    void PlayGuiSound(char nSound);                 // 0x0040a140
    void SetCapture(CSWGuiControl* pControl, int nType); // 0x0040a1c0
    void ReleaseCapture(CSWGuiControl* pControl);   // 0x0040a200
    void ResetToolTipTimer();                       // 0x0040b4d0
    void RemoveToolTip();                           // 0x0040c150
    CSWGuiPanel* PopModalPanel();                    // 0x0040be00
};

extern CSWGuiManager* g_pGuiManager;            // 0x007a39f4

CSWGuiExtent IntersectExtents(const CSWGuiExtent& a, const CSWGuiExtent& b); // 0x0040a4a0

class CExoInput {
public:
    void StartTextInput();                          // 0x005df520
    void StopTextInput();                           // 0x005df530
};

extern CExoInput* g_pExoInput;                  // 0x007a39e4

class CSWGuiBorderParams {
public:
    char pad_0x00[0x60];
};

class CSWGuiTextParams {
public:
    void ResetColor();                              // 0x00414e80
    void SetColor(const Vector* pColor);            // 0x00414e10
    void UpdateFont();                              // 0x00415df0
    char pad_0x00[0x38];
    unsigned long m_bFlag0 : 1;                     // 0x38
    unsigned long m_bFlag1 : 1;
    unsigned long m_bFlag2 : 1;
    unsigned long m_bFlag3 : 1;
    unsigned long m_bFlag4 : 1;
    unsigned long m_bFlag5 : 1;
    unsigned long m_bHilight : 1;                   //      bit 6
    char pad_0x3c[0x18];
};

// Embedded GUI parts (slot 0x10 is Load(gff, struct, name)).
class CSWGuiBorder {
public:
    CSWGuiBorder();                                 // 0x004167f0
    ~CSWGuiBorder();                                // 0x004151c0
    virtual void v00();
    virtual void SetExtent(const CSWGuiExtent& extent); // 0x04
    virtual void v08();
    virtual void Draw(float fDelta);                // 0x0c
    virtual void Load(CResGFF* pGFF, CResStruct* pStruct, const CExoString& sName); // 0x10
    CSWGuiExtent GetInnerExtent();                  // 0x00415240
    void Initialize(const CSWGuiExtent& extent, CSWGuiBorderParams* pParams); // 0x00415190
    char pad_0x04[0x10];
    CSWGuiBorderParams m_cParams;                   // 0x14
};

class CSWGuiText {
public:
    CSWGuiText();                                   // 0x00417280
    void Initialize(const CSWGuiExtent& extent, CSWGuiTextParams* pParams, float fScale); // 0x00417310
    virtual ~CSWGuiText();                          // 0x00415ff0
    virtual void SetExtent(const CSWGuiExtent& extent); // 0x04
    virtual void v08();
    virtual void Draw(float fDelta);                // 0x0c
    virtual void Load(CResGFF* pGFF, CResStruct* pStruct, const CExoString& sName); // 0x10
    char pad_0x04[0x14];
    CSWGuiTextParams m_cParams;                     // 0x18
    int m_n6c;                                      // 0x6c
};

class CSWGuiEditText : public CSWGuiText {
public:
    CSWGuiEditText();                               // 0x004173d0
    void SetFocus(int bFocus);                      // 0x00416530
    void RemoveLastChar();                          // 0x004162e0
    void AddNewChar(const char* sChar);             // 0x004163a0
    __forceinline ~CSWGuiEditText() {}
    short m_nMaxLength;             // 0x70
    unsigned long m_bFlag0 : 1;     // 0x74
    CExoString m_sText;             // 0x78
};

class CAurGUIImage {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual int GetHeight();                        // 0x0c
    virtual int GetWidth();                         // 0x10
};

class CSWGuiImage {
public:
    CSWGuiImage();                                  // 0x00415a30
    ~CSWGuiImage();                                 // 0x00415ad0
    virtual void v00();
    virtual void SetExtent(const CSWGuiExtent& extent); // 0x04
    virtual void v08();
    virtual void Draw(float fDelta);                // 0x0c
    virtual void Load(CResGFF* pGFF, CResStruct* pStruct, const CExoString& sName); // 0x10
    void Initialize(const CSWGuiExtent& extent, CSWGuiImageParams* pParams); // 0x00416e90
    CSWGuiExtent GetImageSize();                    // 0x00415b30
    CSWGuiExtent m_extent;                          // 0x04
    char pad_0x14[0x10];
    CAurGUIImage* m_pImage;                         // 0x24
    CSWGuiImageParams m_cParams;                    // 0x28
};

class CSWGuiNavigable;
class CSWGuiListBox;
class CSWGuiScrollBar;

// vtable 0x0073e488 (slots 0x00-0x94).
class CSWGuiControl : public CSWGuiObject {
public:
    CSWGuiControl();
    virtual ~CSWGuiControl();                                   // 0x00
    virtual void HandleMouseOver(int bOver);                    // 0x0c
    virtual void HandleLMouseDown();                            // 0x18
    virtual void HandleLMouseUp();                              // 0x1c
    virtual void HandleRMouseUp();                              // 0x24
    virtual int GetHeight();                                    // 0x34
    virtual void Draw(float fDelta);                            // 0x38
    virtual void HandleInputEvent(int nEvent, int bDown);       // 0x3c
    virtual void SetFlag0(int b);                               // 0x40
    virtual CSWGuiControl* HitCheckMouse(int x, int y);         // 0x44
    virtual void Load(CResGFF* pGFF, CResStruct* pStruct);      // 0x48
    virtual CSWGuiNavigable* GetNavigation();                   // 0x4c
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual int v60(); virtual void v64(); virtual void v68();
    virtual CSWGuiScrollBar* v6c();                             // 0x6c (scroll bar part, or 0)
    virtual void v70(); virtual void v74(); virtual void v78();
    virtual void HandleFocusChange(int bFocus);                 // 0x7c
    virtual int IsSelectable();                                 // 0x80
    virtual void HandleRMouseDown();                            // 0x84
    virtual void SetEnabled(int b);                             // 0x88
    virtual void UpdateFont();                                  // 0x8c
    virtual int DisplayToolTip();                               // 0x90
    virtual void v94();

    CSWGuiControl* m_pParentControl;                // 0x14
    CExoArrayList<CSWGuiControl*> m_lstChildren;    // 0x18
    int m_nToolTipStrRef;                           // 0x24
    CExoString m_sToolTip;                          // 0x28
    int m_nKeyBinding;                              // 0x30
    CSWGuiObject* m_pOwner;                         // 0x34
    CExoArrayList<CSWGuiControlEvent> m_lstEvents;  // 0x38
    unsigned long m_bFlag0 : 1;                     // 0x44 bit 0
    unsigned long m_bFlag1 : 1;                     //      bit 1
    unsigned long m_bNavigable : 1;                 //      bit 2
    unsigned long m_bSelectable : 1;                //      bit 3
    unsigned long m_bFlag4 : 1;                     //      bit 4
    unsigned long m_bFlag5 : 1;                     //      bit 5
    unsigned long m_bToolTipShown : 1;              //      bit 6
    unsigned long m_nFlagsRest : 25;
    int m_n48;                                      // 0x48
    int m_n4c;                                      // 0x4c
    int m_nId;                                      // 0x50
    unsigned char m_nSound;                         // 0x54
    unsigned char m_nClickSound;                    // 0x55
    int m_n58;                                      // 0x58

    void AddChildControl(CSWGuiControl* pControl);
    void Load(CSWGuiObject* pOwner, CResGFF* pGFF, CResStruct* pStruct, CExoString& sName);
    int HasChildControl(CSWGuiControl* pControl);
    CSWGuiControl* GetOwner();
    void HideToolTip();                             // 0x004176c0
    void AddEvent(int nEvent, CSWGuiPanel* pObject, PFN_GUI_CALLBACK pfnCallback);
};

// vtable 0x0073e520.
class CSWGuiNavigable : public CSWGuiControl {
public:
    CSWGuiNavigable();
    virtual ~CSWGuiNavigable() {}
    virtual void HandleMouseOver(int n);                        // 0x0c
    virtual void HandleInputEvent(int nEvent, int bDown);       // 0x3c
    virtual void Load(CResGFF* pGFF, CResStruct* pStruct);      // 0x48
    virtual CSWGuiNavigable* GetNavigation();                   // 0x4c
    void SetNavigation(int nDir, int nId);          // 0x00417660
    int GetNavigationId(int nDir);                  // 0x00417680

    // 0x5c: up, left, down, right (Get/SetNavigation 0x00417680/0x00417660).
    int m_aNavigation[4];
};

// vtable 0x0073e5b8.
class CSWGuiLabel : public CSWGuiControl {
public:
    CSWGuiLabel();
    virtual ~CSWGuiLabel();
    virtual void SetExtent(const CSWGuiExtent& extent);         // 0x04
    virtual void Draw(float fDelta);                            // 0x38
    virtual void Load(CResGFF* pGFF, CResStruct* pStruct);      // 0x48
    virtual void SetEnabled(int b);                             // 0x88
    virtual void UpdateFont();                                  // 0x8c
    virtual void CopyStyle(const CSWGuiExtent& extent, CSWGuiLabel* pFrom, float fScale); // 0x98
    virtual void SetStyle(const CSWGuiExtent& extent, CSWGuiTextParams* pText, CSWGuiBorderParams* pBorder,
                          float fScale);                        // 0x9c
    CSWGuiBorder m_cBorder;                         // 0x5c
    CSWGuiText m_cText;                             // 0xd0
};

// vtable 0x0073e8e8.
class CSWGuiLabelHilight : public CSWGuiLabel {
public:
    CSWGuiLabelHilight();
    virtual ~CSWGuiLabelHilight();
    virtual void SetExtent(const CSWGuiExtent& extent);         // 0x04
    virtual void Draw(float fDelta);                            // 0x38
    virtual void Load(CResGFF* pGFF, CResStruct* pStruct);      // 0x48
    virtual void CopyStyle(const CSWGuiExtent& extent, CSWGuiLabelHilight* pFrom); // 0xa0
    virtual void SetStyle(const CSWGuiExtent& extent, CSWGuiTextParams* pText, CSWGuiBorderParams* pBorder,
                          CSWGuiBorderParams* pHilight);        // 0xa4
    CSWGuiBorder m_cHilight;                        // 0x140
};

// vtable 0x0073e658.
class CSWGuiButton : public CSWGuiNavigable {
public:
    CSWGuiButton();
    virtual ~CSWGuiButton();
    virtual void HandleInputEvent(int nEvent, int bDown);       // 0x3c
    virtual void SetExtent(const CSWGuiExtent& extent);         // 0x04
    virtual void Draw(float fDelta);                            // 0x38
    virtual void SetFlag0(int b);                               // 0x40
    virtual void Load(CResGFF* pGFF, CResStruct* pStruct);      // 0x48
    virtual void SetEnabled(int b);                             // 0x88
    virtual void UpdateFont();                                  // 0x8c
    virtual void CopyStyle(const CSWGuiExtent& extent, CSWGuiButton* pFrom); // 0x98
    virtual void SetStyle(const CSWGuiExtent& extent, CSWGuiTextParams* pText, CSWGuiBorderParams* pBorder,
                          CSWGuiBorderParams* pHilight);        // 0x9c
    CSWGuiBorder m_cBorder;                         // 0x6c
    CSWGuiBorder m_cHilight;                        // 0xe0
    CSWGuiText m_cText;                             // 0x154
};

// vtable 0x0073eb88.
class CSWGuiButtonToggle : public CSWGuiButton {
public:
    CSWGuiButtonToggle();
    virtual ~CSWGuiButtonToggle();
    virtual void SetExtent(const CSWGuiExtent& extent);         // 0x04
    virtual void Draw(float fDelta);                            // 0x38
    virtual void HandleInputEvent(int nEvent, int bDown);       // 0x3c
    virtual void Load(CResGFF* pGFF, CResStruct* pStruct);      // 0x48
    virtual void CopyStyle(const CSWGuiExtent& extent, CSWGuiButtonToggle* pFrom); // 0xa0
    virtual void SetStyle(const CSWGuiExtent& extent, CSWGuiTextParams* pText, CSWGuiBorderParams* pBorder,
                          CSWGuiBorderParams* pHilight, CSWGuiBorderParams* pSelected,
                          CSWGuiBorderParams* pHilightSelected); // 0xa4
    int m_nToggleEvent;                             // 0x1c4
    unsigned long m_bSelected : 1;                  // 0x1c8
    CSWGuiBorder m_cSelected;                       // 0x1cc
    CSWGuiBorder m_cHilightSelected;                // 0x240
};

class CSWGuiRadioButton : public CSWGuiButton {
public:
    virtual void HandleMouseOver(int bOver);                    // 0x0c
    virtual void HandleLMouseUp();                              // 0x1c
    virtual void HandleLMouseDown();                            // 0x18
};

// vtable 0x0073e6f8.
class CSWGuiScrollBar : public CSWGuiControl {
public:
    CSWGuiScrollBar();
    virtual ~CSWGuiScrollBar();
    virtual void SetExtent(const CSWGuiExtent& extent);         // 0x04
    virtual void Draw(float fDelta);                            // 0x38
    virtual void Load(CResGFF* pGFF, CResStruct* pStruct);      // 0x48
    int m_nMaxValue;                                // 0x5c
    int m_nCurValue;                                // 0x60
    int m_nVisibleValue;                            // 0x64
    unsigned long m_bDrawMode : 1;                  // 0x68 bit 0
    unsigned long m_bAlwaysShow : 1;                //      bit 1
    CSWGuiBorder m_cBorder;                         // 0x6c
    CSWGuiImage m_cThumb;                           // 0xe0
    CSWGuiImage m_cDir;                             // 0x134

    virtual void CopyStyle(const CSWGuiExtent& extent, CSWGuiScrollBar* pFrom); // 0x98
    virtual void SetStyle(const CSWGuiExtent& extent, CSWGuiBorderParams* pBorder, const CResRef& cDir,
                          const CResRef& cThumb);       // 0x9c

    int HitCheckScrollbar(int x, int y);            // 0x004197c0
    int CanScroll();                                // 0x00418010
    int IsVisible();                                // 0x00418030
    void SetMaxValue(int nMax, unsigned char bFlag); // 0x00418070
    void SetCurValue(int nValue);                   // 0x004180a0
    void SetVisibleValue(int nValue);               // 0x004180d0
};

// vtable 0x0073e7a0.
class CSWGuiProgressBar : public CSWGuiControl {
public:
    CSWGuiProgressBar();
    virtual ~CSWGuiProgressBar();
    virtual void SetExtent(const CSWGuiExtent& extent);         // 0x04
    virtual void Draw(float fDelta);                            // 0x38
    virtual void Load(CResGFF* pGFF, CResStruct* pStruct);      // 0x48
    virtual void CopyStyle(const CSWGuiExtent& extent, CSWGuiProgressBar* pFrom); // 0x98
    virtual void SetStyle(const CSWGuiExtent& extent, CSWGuiBorderParams* pBorder,
                          CSWGuiBorderParams* pProgress);       // 0x9c
    int m_nMaxValue;                                // 0x5c
    int m_nCurValue;                                // 0x60
    unsigned long m_bStartFromLeft : 1;             // 0x64
    CSWGuiBorder m_cBorder;                         // 0x68
    CSWGuiBorder m_cProgress;                       // 0xdc

    void SetMaxValue(int n);                        // 0x00417f90
    void SetCurValue(int n);                        // 0x00417fb0
    void SetStartFromLeft(int b);                   // 0x00417fe0
};

// vtable 0x0073e9d0.
class CSWGuiSlider : public CSWGuiNavigable {
public:
    CSWGuiSlider();
    virtual ~CSWGuiSlider();
    virtual void SetExtent(const CSWGuiExtent& extent);         // 0x04
    virtual int v10(int a, int b);                              // 0x10
    virtual void HandleLMouseDown();                            // 0x18
    virtual void Draw(float fDelta);                            // 0x38
    virtual void HandleInputEvent(int nEvent, int bDown);       // 0x3c
    virtual void CopyStyle(const CSWGuiExtent& extent, CSWGuiSlider* pFrom); // 0x98
    virtual void SetStyle(const CSWGuiExtent& extent, CSWGuiBorderParams* pBorder, CSWGuiBorderParams* pThumb,
                          const CResRef& cImage);       // 0x9c
    void SetMaxValue(int nMax);                     // 0x00417ec0
    void SetValue(int nValue);                      // 0x00417ee0
    int HitCheckSlider(int x, int y);               // 0x00419060
    int m_nHitPart;                                 // 0x6c
    int m_nMaxValue;                                // 0x70
    int m_nValue;                                   // 0x74
    unsigned char m_nSound;                         // 0x78
    CSWGuiBorder m_cBorder;                         // 0x7c
    CSWGuiBorder m_cThumb;                          // 0xf0
    CSWGuiImage m_cImage;                           // 0x164
};

// vtable 0x0073eac8.
class CSWGuiEditbox : public CSWGuiNavigable {
public:
    CSWGuiEditbox();
    virtual ~CSWGuiEditbox();
    virtual void SetExtent(const CSWGuiExtent& extent);         // 0x04
    virtual void HandleLMouseUp();                              // 0x1c
    virtual void Draw(float fDelta);                            // 0x38
    virtual void HandleFocusChange(int bFocus);                 // 0x7c
    virtual int IsSelectable();                                 // 0x80
    virtual void UpdateFont();                                  // 0x8c
    virtual void Initialize(const CSWGuiExtent& extent, CSWGuiTextParams* pText, CSWGuiBorderParams* pBorder); // 0x98
    virtual void HandleChar(int nChar);             // 0x9c
    void SetFocus();
    virtual void Load(CResGFF* pGFF, CResStruct* pStruct);      // 0x48
    CSWGuiBorder m_cBorder;                         // 0x6c
    CSWGuiEditText m_cText;                         // 0xe0
};

// 12-byte member of CSWGuiListBox (+0x2d0), zeroed by its inline ctor.
struct CSWGuiListBoxScroll {
    int m_n0;
    int m_n4;
    int m_n8;
    CSWGuiListBoxScroll()
    {
        m_n0 = 0;
        m_n4 = 0;
        m_n8 = 0;
    }
};

// vtable 0x0073e840.
class CSWGuiListBox : public CSWGuiNavigable {
public:
    CSWGuiListBox();
    virtual ~CSWGuiListBox();
    virtual void SetExtent(const CSWGuiExtent& extent);         // 0x04
    virtual void Draw(float fDelta);                            // 0x38

    int m_n6c;                                      // 0x6c
    int m_n70;                                      // 0x70
    int m_n74;                                      // 0x74
    int m_n78;                                      // 0x78
    int m_n7c;                                      // 0x7c
    int m_nLocalMouseX;                             // 0x80
    int m_nLocalMouseY;                             // 0x84
    int m_n88;                                      // 0x88 repeat event
    union {
        int m_n8c;                                  // 0x8c
        float m_fRepeatTimer;
    };
    CSWGuiBorder m_cBorder;                         // 0x90
    CSWGuiScrollBar m_cScrollBar;                   // 0x104
    CSWGuiExtent m_cArea;                           // 0x28c
    CExoArrayList<CSWGuiControl*> m_lstControls;    // 0x29c
    CExoArrayList<CSWGuiExtent> m_lstExtents;       // 0x2a8
    int m_nItemSize;                                // 0x2b4
    float m_fScale;                                 // 0x2b8
    union {
        unsigned long m_nFlags;                     // 0x2bc
        struct {
            unsigned long m_bListFlag0 : 1;                 // 0x2bc bit 0
            unsigned long m_bFlag1 : 1;                     //       bit 1
            unsigned long m_bFlag2 : 1;                     //       bit 2
            unsigned long m_bDirty : 1;                     //       bit 3
            unsigned long m_bScrollBarOnLeft : 1;           //       bit 4
            unsigned long m_bFlag5 : 1;                     //       bit 5
            unsigned long m_bFlag6 : 1;                     //       bit 6
            unsigned long m_bScrolledToTop : 1;             //       bit 7
            unsigned long m_bUnequal : 1;                   //       bit 8
            unsigned long m_bFlag9 : 1;                     //       bit 9
            unsigned long m_bFlag10 : 1;                    //       bit 10
            unsigned long m_bFlag11 : 1;                    //       bit 11
            unsigned long m_bKeepSelectionVisible : 1;      //       bit 12
        };
    };
    unsigned char m_n2c0;                           // 0x2c0
    char m_n2c1;                                    // 0x2c1
    short m_nPages;                                 // 0x2c2
    short m_nVisible;                               // 0x2c4
    short m_nSelected;                              // 0x2c6
    short m_nFirstVisible;                          // 0x2c8
    short m_nLastVisible;                           // 0x2ca
    CSWGuiControl* m_pProtoItem;                    // 0x2cc
    CSWGuiListBoxScroll m_cScroll;                  // 0x2d0
    CSWGuiControl* m_pLastControl;                  // 0x2dc

    virtual void SetActiveControl(CSWGuiControl* pControl, int bSound); // 0x08
    virtual void HandleLMouseDown();                            // 0x18
    virtual void HandleLMouseUp();                              // 0x1c
    virtual void SetFlag0(int b);                               // 0x40
    virtual int IsSelectable();                                 // 0x80
    void SelectControl(CSWGuiControl* pControl, int bSound); // 0x0041c160
    virtual void UpdateFont();                                  // 0x8c
    virtual int DisplayToolTip();                               // 0x90
    CSWGuiControl* HitCheckMouseLocal(int* pIndex); // 0x0041a5e0
    void UpdateScrollBar(int nWidth, int bSetExtent); // 0x004181f0
    int IsOversized();                              // 0x0041a290
    int GetPagesToScroll();                         // 0x004182b0
    CSWGuiControl* GetControl(int nIndex);          // 0x0041a5a0
    CSWGuiControl* GetSelectedControl();            // 0x0041a5d0
    void SetSelectedControl(int nIndex, int bSound); // 0x0041c040
    void SetPadding(unsigned char nPadding);        // 0x0041c190
    void AddControls(const CExoArrayList<CSWGuiControl*>& lstControls, int bOwned, int bRestoreExtents,
                     int bUnequal);                 // 0x0041c1d0
    void AddControls(CSWGuiControl** pControls, int nCount, int bOwned, int bRestoreExtents,
                     int bUnequal);                 // 0x0041c2c0
    void SetScrollBarOnLeft(int bLeft);             // 0x00418270
    void ClearControls();                           // 0x00419a90
    virtual void CopyStyle(const CSWGuiExtent& extent, CSWGuiListBox* pFrom); // 0x98
    virtual void SetStyle(const CSWGuiExtent& extent, CSWGuiBorderParams* pBorder,
                          int nPadding);                // 0x9c
    virtual void SetScrollBarStyle(CSWGuiExtent& extent, CSWGuiBorderParams* pBorder, const CResRef& cDir,
                                   const CResRef& cThumb, int bLeft); // 0xa0
    virtual void SetListFlag0(int b);               // 0xa4 (0x0041b120)
    void OrganizeControls();                        // 0x0041b140
};

#endif
