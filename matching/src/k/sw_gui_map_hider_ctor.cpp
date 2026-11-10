// CSWGuiMapHider constructor (map control, 0x248 bytes, vtable 0x00754738).
// Member layout as sw_gui_map_hider_dtor.cpp; image / border parameter
// layouts as matching/include/d/gui_parts.h.

class CExoString {
public:
    CExoString();                                   // 0x005B3190
    ~CExoString();                                  // 0x005E5C20
    char* m_sString;
    unsigned long m_nBufferLength;
};

class CResRef {
public:
    CResRef();                                      // 0x00405ED0
    CResRef(const char* s);                         // 0x00406D80
    char m_resRef[16];
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

struct CSWGuiExtent {
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

typedef void* CExoLinkedListPosition;

class CExoLinkedListInternal {
public:
    ~CExoLinkedListInternal();                      // 0x005E9AB0
    void* m_pHead;
    void* m_pTail;
    unsigned long m_nCount;
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedList();                               // 0x004B25D0
    ~CExoLinkedList() { delete m_pList; }
    CExoLinkedListInternal* m_pList;
};

template <class T>
class CExoArrayList {
public:
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
    T* element;
    int num;
    int array_size;
};

class CAurGUIImage;
CAurGUIImage* __cdecl NewCAurGUIImage(char* sName);  // 0x0045A260

class CSWGuiImageParams {
public:
    CSWGuiImageParams(const CResRef& cResRef, int b1, int n2, float f10, int n6, float f14,
                      const Vector& vColor);        // 0x00414D30
    CResRef m_cResRef;                              // 0x00
    float m_f10;                                    // 0x10
    float m_f14;                                    // 0x14
    Vector m_vColor;                                // 0x18
    unsigned long m_b0 : 1;                         // 0x24 bit 0
    unsigned long m_b1 : 1;                         //      bit 1
    unsigned long m_n2 : 4;                         //      bits 2-5
    unsigned long m_n6 : 6;                         //      bits 6-11
    void* m_pImage;                                 // 0x28
    void SetB0(int b) { m_b0 = b; }
};

class CSWGuiTextParams {
public:
    CSWGuiTextParams();                             // 0x00417050
    CExoString m_sText;                             // 0x00
    char pad_0x08[0x4c];
};

class CSWGuiBorderParams {
public:
    CSWGuiBorderParams(const CResRef& cCorner, const CResRef& cEdge, const CResRef& cFill,
                       int nStyle, int nCornerSize, float fEdgeScale, const Vector& vColor,
                       int nPulse);                 // 0x00415000
    char pad_0x00[0x10];
    Vector m_vColor;                                // 0x10
    char pad_0x1c[0x38];
};

class CSWGuiDrawable {
public:
    virtual ~CSWGuiDrawable();
    CSWGuiExtent m_extent;                          // 0x04
};

class CSWGuiBorder : public CSWGuiDrawable {
public:
    CSWGuiBorderParams m_cParams;                   // 0x14
    char pad_0x68[0x74 - 0x68];
};

// 0x54 bytes, vtable 0x0073e390.
class CSWGuiImage {
public:
    CSWGuiImage();                                  // 0x00415A30
    virtual ~CSWGuiImage();
    void Initialize(const CSWGuiExtent& extent, const CSWGuiImageParams& params);  // 0x00416E90
    char pad_0x4[0x28 - 0x4];
    CSWGuiImageParams m_cParams;                    // 0x28
};

class CSWGuiControl {
public:
    CSWGuiControl();                                // 0x0041AA80
    virtual ~CSWGuiControl();                       // 0x00419A20
    char pad_0x4[0x58];
};

// 0x1c4 bytes, vtable 0x0073e658.
class CSWGuiButton : public CSWGuiControl {
public:
    CSWGuiButton();                                 // 0x0041C0F0
    virtual ~CSWGuiButton();                        // 0x00419DA0
    virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94(); virtual void v98();
    virtual void SetStyle(const CSWGuiExtent& extent, CSWGuiTextParams* pText,
                          CSWGuiBorderParams* pBorder, CSWGuiBorderParams* pHilight);  // 0x9c
    char pad_0x5c[0x6c - 0x5c];
    CSWGuiBorder m_cBorder;                         // 0x6c
    CSWGuiBorder m_cHilight;                        // 0xe0
    char pad_0x154[0x1c4 - 0x154];
};

struct CSWGuiMapNote {
    unsigned long m_oidNote;
};

extern Vector g_vGuiColor23F0;                      // 0x007A23F0
extern Vector g_vAbilityColorGranted;               // 0x007A23C0

class CSWGuiMapHider : public CSWGuiControl {
public:
    CSWGuiMapHider();
    virtual ~CSWGuiMapHider();                      // 0x006942D0
    void ClearMapNotes();                           // 0x00692E30

    CAurGUIImage* m_pMarker;                        // 0x5c (descriptive)
    CSWGuiImage* m_pMapImage;                       // 0x60 (descriptive)
    CSWGuiImage* m_pHiderImage;                     // 0x64 (descriptive)
    CSWGuiButton m_cMarker;                         // 0x68 (descriptive)
    CExoArrayList<CSWGuiControl*> m_aControls;      // 0x22c
    CExoLinkedList<CSWGuiMapNote> m_lMapNotes;      // 0x238
    CExoLinkedListPosition m_posSelectedNote;       // 0x23c
    char pad_0x240[0x244 - 0x240];
    int m_bMapNoteFlag;                             // 0x244 (descriptive)
};

typedef char KMapHiderSize[(sizeof(CSWGuiMapHider) == 0x248) ? 1 : -1];

// FUNCTION: SWKOTOR 0x00693F60
// NONMATCH: 22 diff insns, register choice only: cl keeps &m_cMarker in ebx
// for the SetStyle call where the original keeps the EH state 3 in bl.
CSWGuiMapHider::CSWGuiMapHider()
{
    ClearMapNotes();
    m_bMapNoteFlag = 0;
    m_pMarker = NewCAurGUIImage("blackdot");
    m_pMapImage = new CSWGuiImage;
    m_pHiderImage = new CSWGuiImage;
    if (m_pMapImage) {
        CResRef cImage("mm_barrow");
        m_pMapImage->Initialize(CSWGuiExtent(0, 0, 0x20, 0x20),
                                CSWGuiImageParams(cImage, 0, 0, 0.0f, 0x12, 1.0f, Vector(1.0f, 1.0f, 1.0f)));
        m_pMapImage->m_cParams.SetB0(1);
    }
    if (m_pHiderImage) {
        CResRef cImage("lbl_mapcircle");
        m_pHiderImage->Initialize(CSWGuiExtent(0, 0, 0x10, 0x10),
                                  CSWGuiImageParams(cImage, 0, 0, 0.0f, 0x12, 1.0f, Vector(1.0f, 1.0f, 1.0f)));
        m_pHiderImage->m_cParams.m_vColor = g_vGuiColor23F0;
    }
    {
        CResRef cHilight("whitetarget");
        CResRef cBorder("whitetarget");
        m_cMarker.SetStyle(CSWGuiExtent(0, 0, 0x14, 0x14), &CSWGuiTextParams(),
                          &CSWGuiBorderParams(CResRef(), CResRef(), cBorder, 2, 0, 1.0f,
                                              Vector(1.0f, 1.0f, 1.0f), 0),
                          &CSWGuiBorderParams(CResRef(), CResRef(), cHilight, 2, 0, 1.0f,
                                              Vector(1.0f, 1.0f, 1.0f), 0));
    }
    m_cMarker.m_cBorder.m_cParams.m_vColor = g_vAbilityColorGranted;
    m_cMarker.m_cHilight.m_cParams.m_vColor = g_vAbilityColorGranted;
}
