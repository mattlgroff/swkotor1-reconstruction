// embedded GUI parts (0x00414a50-0x004175c0): CSWGuiDrawable and
// its border / text / edit text / image / scene subclasses plus their
// parameter blocks. Layouts from include/kotor/b0041_gui_params.h and the
// constructors (x86 confirmed). Standalone header.
#ifndef D_GUI_PARTS_H
#define D_GUI_PARTS_H

class CExoString {
public:
    char* m_sString;
    int m_nBufferLength;
    CExoString();                                   // 0x005b3190
    CExoString(const char* sString);                // 0x005e5a90
    ~CExoString();                                  // 0x005e5c20
    CExoString& operator=(const CExoString& s);     // 0x005e5c50
    CExoString& operator=(const char* s);           // 0x005e5140
    CExoString operator+(const CExoString& s) const; // 0x005e5d10
    CExoString SubString(int nStart, int nCount) const; // 0x005e6270
    void Format(const char* sFormat, ...);          // 0x005e5680
    char* CStr() const;                             // 0x005e5670
    int GetLength() const;                          // 0x005e5790
};

class CResRef {
public:
    char m_resRef[16];
    CResRef();                                      // 0x00405ed0
    CResRef(const char* szResRef);                  // 0x00406d80
    CResRef(const CExoString& sResRef);             // 0x00406d60
    CResRef operator=(const CResRef& cResRef);      // 0x00406120
    int operator!=(const CResRef& cResRef) const;   // 0x00406dc0
    int IsValid() const;                            // 0x00406030
    void CopyToString(char* pStr) const;            // 0x00405fb0
};

class Vector {
public:
    float x, y, z;
    Vector() { x = 0.0f; y = 0.0f; z = 0.0f; }
    Vector(float fX, float fY, float fZ)
    {
        x = fX;
        y = fY;
        z = fZ;
    }
};

struct CResStruct {
    unsigned long m_nStructIndex;
};

class CResGFF;

struct CSWGuiExtent {
    int left;
    int top;
    int width;
    int height;
    CSWGuiExtent() { left = top = width = height = 0; }
    CSWGuiExtent(int nLeft, int nTop, int nWidth, int nHeight)
    {
        left = nLeft;
        top = nTop;
        width = nWidth;
        height = nHeight;
    }
};

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList(int nSize = 0);
    ~CExoArrayList();                               // 0x0056b6f0
    void Allocate(int nSize);                       // 0x005e03c0
    void Add(T t);                                  // 0x00671c00
    void DelIndex(int i);                           // 0x006857a0
    void Insert(T t, int nPos);
    T& operator[](int i) { return element[i]; }
};

// Engine-side objects (vtables outside this range).
class CAurGUIImage {
public:
    virtual void v00();
    virtual void Release(int bDelete);              // 0x04
    virtual void v08();
    virtual int GetHeight();                        // 0x0c
    virtual int GetWidth();                         // 0x10
    virtual void v14();
    virtual void Draw(int nX, int nY, int nWidth, int nHeight, unsigned char nFlip,
                      float fRotate, const Vector* pColor, float fAlpha); // 0x18
};

class CAurGUIString {
public:
    virtual void v00();
    virtual void Release(int bDelete);              // 0x04
    virtual void v08();
    virtual void SetColor(const Vector* pColor);    // 0x0c
    virtual void v10(); virtual void v14();
    virtual void SetText(const char* sText);        // 0x18
    virtual void SetAlignment(unsigned char nAlign); // 0x1c
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40();
    virtual void SetFont(const char* sFont);        // 0x44
    virtual int GetFontHeight();                    // 0x48
    virtual void Rebuild(int n);                    // 0x4c
    virtual int GetLineCount(int nWidth);           // 0x50
    virtual void v54(); virtual void v58(); virtual void v5c(); virtual void v60();
    virtual void SetPosition(int x, int y);         // 0x64
    virtual void v68(); virtual void v6c(); virtual void v70();
    virtual void SetSize(int w, int h);             // 0x74
    virtual void v78();
    virtual void SetAlpha(float fAlpha);            // 0x7c
};

class CAurObject {
public:
    virtual void Destroy(int bDelete);              // 0x00
    virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20();
    virtual void v24(); virtual void v28(); virtual void v2c(); virtual void v30();
    virtual void v34(); virtual void v38(); virtual void v3c(); virtual void v40();
    virtual void v44(); virtual void v48();
    virtual void AttachToScene(void* pScene);       // 0x4c
};

class CAurScene {
public:
    virtual void Destroy(int bDelete);              // 0x00
    virtual void SetActive(int bActive);            // 0x04
    virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18();
    virtual void Update(float fDelta);              // 0x1c
    virtual void SetLighting(int a, int b, int c, int d, float f); // 0x20
    virtual void v24(); virtual void v28(); virtual void v2c(); virtual void v30();
    virtual void v34(); virtual void v38(); virtual void v3c(); virtual void v40();
    virtual void v44(); virtual void v48(); virtual void v4c(); virtual void v50();
    virtual void v54(); virtual void v58(); virtual void v5c(); virtual void v60();
    virtual void v64(); virtual void v68(); virtual void v6c(); virtual void v70();
    virtual void v74(); virtual void v78(); virtual void v7c(); virtual void v80();
    virtual void v84(); virtual void v88();
    virtual void Initialize();                      // 0x8c
};

class CAurCamera {
public:
    virtual void Destroy(int bDelete);              // 0x00
    virtual void v04();
    virtual void Render();                          // 0x08
    virtual void v0c(); virtual void v10();
    virtual void SetScene(CAurScene* pScene);       // 0x14
    virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2c(); virtual void v30(); virtual void v34();
    virtual void v38(); virtual void v3c();
    virtual void SetClipPlanes(float fNear, float fFar); // 0x40
};

CAurScene* __cdecl NewCAurScene(const char* sName);     // 0x00458e70
CAurCamera* __cdecl NewCAurCamera(const char* sName);   // 0x0045d930
CAurObject* __cdecl NewCAurObject(const char* sModel, const char* sName, int a, int b); // 0x00449cc0
extern Vector g_vGuiSceneAmbient;                       // 0x0078d3d8

extern "C" CAurGUIImage* NewCAurGUIImage(const char* sName);   // 0x0045a260
CAurGUIString* NewCAurGUIString(const char* sText, const char* sFont,
                                const Vector& vColor, float fScale); // 0x0045bdf0

class CSWGuiBorder;
class CSWGuiText;
class CSWGuiImage;

// vtable 0x0073e324.
class CSWGuiDrawable {
public:
    CSWGuiDrawable() {}
    virtual ~CSWGuiDrawable() {}                    // 0x00 (0x00414f80)
    virtual void SetExtent(const CSWGuiExtent& extent); // 0x04 (0x00409e90)
    virtual int GetHeight();                        // 0x08
    virtual void Draw(float fDelta) = 0;            // 0x0c
    virtual void Load(CResGFF* pGFF, CResStruct* pStruct, const CExoString& sName) = 0; // 0x10
    CSWGuiExtent m_extent;                          // 0x04
};

class CSWGuiBorderParams {
public:
    CSWGuiBorderParams(const CResRef& cCorner, const CResRef& cEdge, const CResRef& cFill,
                       int nStyle, int nCornerSize, float fEdgeScale, const Vector& vColor,
                       int nPulse);
    CSWGuiBorderParams& operator=(const CSWGuiBorderParams& p);
    void SetCornerImage(const CResRef& cResRef, int bForce);
    void SetEdgeImage(const CResRef& cResRef, int bForce);
    void SetFillImage(const CResRef& cResRef, int bForce);
    void SetCornerImage(const CResRef& cResRef) { SetCornerImage(cResRef, 0); }
    void SetEdgeImage(const CResRef& cResRef) { SetEdgeImage(cResRef, 0); }
    void SetFillImage(const CResRef& cResRef) { SetFillImage(cResRef, 0); }
    void SetBorder(CSWGuiBorder* pBorder);
    int GetCornerSize();
    int GetInset() { return m_nPulse < GetCornerSize() ? m_nPulse : GetCornerSize(); }
    void SetFlags(int b2, int b3, int b4);
    float GetF08() const { return m_f08; }
    void SetStyle(int nStyle) { m_nStyle = nStyle; }
    float GetEdgeScale() const { return m_fEdgeScale; }

    int m_nCornerSize;                              // 0x00
    int m_nPulse;                                   // 0x04
    float m_f08;                                    // 0x08
    float m_fEdgeScale;                             // 0x0c
    Vector m_vColor;                                // 0x10
    unsigned long m_nStyle : 2;                     // 0x1c bits 0-1
    unsigned long m_b2 : 1;                         //      bit 2
    unsigned long m_b3 : 1;                         //      bit 3
    unsigned long m_b4 : 1;                         //      bit 4
    CResRef m_cCorner;                              // 0x20
    CResRef m_cEdge;                                // 0x30
    CResRef m_cFill;                                // 0x40
    CSWGuiBorder* m_pBorder;                        // 0x50
};

// vtable 0x0073e338.
class CSWGuiBorder : public CSWGuiDrawable {
public:
    CSWGuiBorder();
    virtual ~CSWGuiBorder();
    virtual void Draw(float fDelta);                // 0x0c (0x004168c0)
    virtual void Load(CResGFF* pGFF, CResStruct* pStruct, const CExoString& sName); // 0x10
    void Initialize(const CSWGuiExtent& extent, const CSWGuiBorderParams& params);
    CSWGuiExtent GetInnerExtent();                  // 0x00415240
    void DrawFillTiled(int nHeight, int nWidth, int nX, int nY, float fAlpha,
                       const Vector* pColor);       // 0x004155b0
    void DrawFillCentered(int nHeight, int nWidth, int nX, int nY, float fAlpha,
                          const Vector* pColor);    // 0x004157f0
    int GetCornerHeight()
    {
        CAurGUIImage* pCorner = m_pCorner;
        if (pCorner && m_pEdge) {
            return pCorner->GetHeight();
        }
        return 0;
    }

    CSWGuiBorderParams m_cParams;                   // 0x14
    CAurGUIImage* m_pCorner;                        // 0x68
    CAurGUIImage* m_pEdge;                          // 0x6c
    CAurGUIImage* m_pFill;                          // 0x70
};

class CSWGuiImageParams {
public:
    CSWGuiImageParams(const CResRef& cResRef, int b1, int n2, float f10, int n6, float f14,
                      const Vector& vColor);
    CSWGuiImageParams& operator=(const CSWGuiImageParams& p);
    void SetImage(const CResRef& cResRef, int bForce);
    void SetImage(const CResRef& cResRef) { SetImage(cResRef, 0); }
    void SetN6(int n);
    void SetB1(int b);
    void SetOwner(CSWGuiImage* pImage);
    float GetRotation() const { return m_f10; }
    float GetF14() const { return m_f14; }

    CResRef m_cResRef;                              // 0x00
    float m_f10;                                    // 0x10
    float m_f14;                                    // 0x14
    Vector m_vColor;                                // 0x18
    unsigned long m_b0 : 1;                         // 0x24 bit 0
    unsigned long m_b1 : 1;                         //      bit 1
    unsigned long m_n2 : 4;                         //      bits 2-5
    unsigned long m_n6 : 6;                         //      bits 6-11
    CSWGuiImage* m_pImage;                          // 0x28
};

// vtable 0x0073e390.
class CSWGuiImage : public CSWGuiDrawable {
public:
    CSWGuiImage();
    virtual ~CSWGuiImage();
    virtual void SetExtent(const CSWGuiExtent& extent); // 0x04 (0x00415b70)
    virtual void Draw(float fDelta);                // 0x0c (0x00415c90)
    virtual void Load(CResGFF* pGFF, CResStruct* pStruct, const CExoString& sName); // 0x10
    void Initialize(const CSWGuiExtent& extent, const CSWGuiImageParams& params);

    CSWGuiExtent m_cInner;                          // 0x14
    CAurGUIImage* m_pImage;                         // 0x24
    CSWGuiImageParams m_cParams;                    // 0x28
};

class CSWGuiTextParams {
public:
    CSWGuiTextParams();
    CSWGuiTextParams(const CExoString& sText, const CResRef& cFont, const Vector& vColor,
                     int nAlign, float fAlpha);
    CSWGuiTextParams& operator=(const CSWGuiTextParams& p);
    void SetColor(const Vector& vColor);
    void SetDefaultColor(const Vector& vColor);
    void RestoreDefaultColor();
    void SetFont(const CResRef& cFont);
    void ApplyFont();
    void SetBaseFont(const CResRef& cFont);
    void UpdateFont();
    void SetText(const CExoString& sText);
    void SetStrRef(int nStrRef);
    void SetAlignment(int nAlign);
    void SetOwner(CSWGuiText* pText);
    void SetHilight(int b) { m_bHilight = b; }
    float GetAlpha() const { return m_fAlpha; }

    CExoString m_sText;                             // 0x00
    int m_nStrRef;                                  // 0x08
    CResRef m_cFont;                                // 0x0c
    Vector m_vColor;                                // 0x1c
    float m_fAlpha;                                 // 0x28
    Vector m_vDefaultColor;                         // 0x2c
    unsigned long m_nAlign : 6;                     // 0x38 bits 0-5
    unsigned long m_bHilight : 1;                   //      bit 6
    char m_szBaseFont[0x14];                        // 0x3c
    CSWGuiText* m_pText;                            // 0x50
};

// vtable 0x0073e3a4.
class CSWGuiText : public CSWGuiDrawable {
public:
    CSWGuiText();
    virtual ~CSWGuiText();
    virtual void SetExtent(const CSWGuiExtent& extent); // 0x04
    virtual int GetHeight();                        // 0x08
    virtual void Draw(float fDelta);                // 0x0c
    virtual void Load(CResGFF* pGFF, CResStruct* pStruct, const CExoString& sName); // 0x10
    void Initialize(const CSWGuiExtent& extent, const CSWGuiTextParams& params, float fScale);
    void Rebuild();
    int GetFontHeight();
    CSWGuiExtent GetTextExtent();

    CAurGUIString* m_pString;                       // 0x14
    CSWGuiTextParams m_cParams;                     // 0x18
    int m_nMaxWidth;                                // 0x6c
};

// vtable 0x0073e444.
class CSWGuiEditText : public CSWGuiText {
public:
    CSWGuiEditText();
    void RemoveLastChar();
    void AddNewChar(const char* sChar);
    void SetText(const CExoString& sText);
    void SetCaretVisible(int bVisible);

    short m_nMaxLength;                             // 0x70
    unsigned long m_bCaret : 1;                     // 0x74
    CExoString m_sText;                             // 0x78
};

// vtable 0x0073e3f0.
class CSWGuiScene : public CSWGuiDrawable {
public:
    CSWGuiScene();
    virtual ~CSWGuiScene();
    virtual void Draw(float fDelta);                // 0x0c (0x00414fb0)
    CAurObject* AddModel(const CExoString& sModel, int nIndex);
    int RemoveModel(int nIndex, int bDelete);
    CAurObject* GetModel(int nIndex);
    int AddModel(CAurObject* pModel, int nIndex, int bAttach);

    CAurScene* m_pScene;                          // 0x14
    CAurCamera* m_pCamera;                            // 0x18
    CExoArrayList<CAurObject*> m_lstModels;         // 0x1c
    Vector m_vAmbient;                              // 0x28
};

struct STR_RES {
    STR_RES() {}
    CExoString sString;                             // 0x00
    CResRef cSoundResRef;                           // 0x08
    float fSoundLength;                             // 0x18
};

class CTlkTable {
public:
    int Fetch(unsigned long nStrRef, STR_RES& strRes, unsigned char nGender); // 0x0041e550
};

extern CTlkTable* g_pTlkTable;                      // 0x007a3a08
extern unsigned char g_nGender;                     // 0x0073ecd0

class CSWGuiSafePanel;
class CSWGuiSafeLabel;

class CSWGuiManager {
public:
    void GetUpdatedFontName(char* sFont);           // 0x0040b360
    void DrawPCBG(float fDelta);                    // 0x0040ae80
    void DrawSafeRegion(float fDelta);              // 0x0040bec0
    char pad_0x00[0x6c];
    short m_nWidth;                                 // 0x6c
    short m_nHeight;                                // 0x6e
    char pad_0x70[0x04];
    CExoString m_sResolution;                       // 0x74
    CSWGuiSafeLabel* m_pBackground;                 // 0x7c
    CSWGuiSafePanel* m_pSafePanel;                  // 0x80
    CSWGuiSafeLabel* m_pSafeLabel;                  // 0x84
};

extern CSWGuiManager* g_pGuiManager;                // 0x007a39f4

#endif
