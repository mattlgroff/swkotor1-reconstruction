// CAurGUIStringInternal / CAurGUIImageInternal virtuals
// (0x00458F70 .. 0x0045BD70; vtables 0x00741878 and 0x0074183C).

#include <string.h>
#include "h/scene.h"

extern "C" {
__declspec(dllimport) void __stdcall glBegin(unsigned int mode);
__declspec(dllimport) void __stdcall glEnd();
__declspec(dllimport) void __stdcall glEnable(unsigned int cap);
__declspec(dllimport) void __stdcall glDisable(unsigned int cap);
__declspec(dllimport) void __stdcall glColor4ubv(const unsigned char* v);
__declspec(dllimport) void __stdcall glVertex4f(float x, float y, float z, float w);
}

extern short g_nGUILayer;                       // 0x007B9460
extern short g_aGUIRects[][5];                  // 0x007B9468 {x, y, w, h, bClear}
extern int g_bAurNoGUIDraw;                     // 0x007BB4D0
extern int g_nGUIImageFlag;                     // 0x007A689C
extern unsigned int g_nGLDepthTest;             // 0x0078D45C
extern unsigned int g_nGLTexture2D;             // 0x0078D458
extern unsigned int g_nGLBlend;                 // 0x0078D448
extern unsigned int g_nGLLineStrip;             // 0x0073F358

void AurTextureRelease(CAurObject* pTexture);   // 0x00420D70

class GUIVector {
public:
    float x, y, z;

    GUIVector() { x = 0.0f; y = 0.0f; z = 0.0f; }
    GUIVector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
};

struct CAurFontInfo {
    int m_n0;
    float m_fScale;                             // +0x04
};

class CAurFont : public CAurObject {
public:
    virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34();
    virtual CAurFontInfo* GetInfo();            // +0x38
    virtual void v3C();
    virtual int GetHeight();                    // +0x40
};

CAurFont* AurTextureGetReference(char* szName, int n);   // 0x00423A60

class FontPtr {
public:
    CAurFont* m_p;

    FontPtr() { m_p = 0; }
    ~FontPtr()
    {
        if (m_p)
            m_p->RemoveNoThrow((SafePointerBase*)this);
    }
    void Reset()
    {
        if (m_p)
            m_p->Remove((SafePointerBase*)this);
        m_p = 0;
    }
    FontPtr& operator=(CAurFont* p);            // 0x00447FA0
};

template <class T>
class GUIList : public CExoArrayList<T> {
};


class CAurGUIImage {
public:
    virtual void Draw() = 0;
    ~CAurGUIImage() {}
};

class CAurGUIStringInternal : public CAurGUIImage {
public:
    int m_nX;                                   // +0x04
    int m_nY;                                   // +0x08
    int m_nW;                                   // +0x0c
    int m_nH;                                   // +0x10
    char* m_szString;                           // +0x14
    FontPtr m_pFont;                            // +0x18
    GUIVector m_vColor;                         // +0x1c
    GUIList<float> m_lA;                        // +0x28
    GUIList<int> m_lB;                          // +0x34
    int m_n40;                                  // +0x40
    short m_n44;                                // +0x44
    unsigned char m_nFlags;                     // +0x46

    CAurGUIStringInternal(char* szText, char* szFont, GUIVector vColor, int n);
    virtual void Draw();                        // +0x00
    virtual ~CAurGUIStringInternal();           // +0x04
    virtual GUIVector GetColor();               // +0x08
    virtual void SetColor(const GUIVector& v);  // +0x0c
    virtual int GetLength();                    // +0x10
    virtual void SetStringSize(short nSize);    // +0x14
    virtual void SetString(char* sz);           // +0x18
    virtual void SetFlags(unsigned char n);     // +0x1c
    virtual unsigned char GetFlags();           // +0x20
    virtual void SetVAlign(unsigned char n);    // +0x24
    virtual int GetVAlign();                    // +0x28
    virtual void SetHAlign(unsigned char n);    // +0x2c
    virtual int GetHAlign();                    // +0x30
    virtual void SetCursor(short n);            // +0x34
    virtual short GetCursor();                  // +0x38
    virtual void ClearCursor();                 // +0x3c
    virtual int GetFontHeight();                // +0x40
    virtual void SetFont(char* szFont);         // +0x44
    virtual int GetFontScale();                 // +0x48
    virtual void v4C();
    virtual void v50();
    virtual int v54();
    virtual void GetPosition(float* pX, float* pY);      // +0x58
    virtual void GetPixelPosition(int* pX, int* pY);     // +0x5c
    virtual void SetPosition(float fX, float fY);        // +0x60
    virtual void SetPixelPosition(int nX, int nY);       // +0x64
    virtual void GetSize(float* pW, float* pH);          // +0x68
    virtual void GetPixelSize(int* pW, int* pH);         // +0x6c
    virtual void SetSize(float fW, float fH);            // +0x70
    virtual void SetPixelSize(int nW, int nH);           // +0x74
    virtual void DrawBounds(int a, int b, int c, int d); // +0x78
    virtual void Render(float fAlpha);                   // +0x7c
};

// FUNCTION: SWKOTOR 0x0045BBD0
void CAurGUIStringInternal::Draw()
{
    Render(1.0f);
}

// FUNCTION: SWKOTOR 0x0045BAE0
GUIVector CAurGUIStringInternal::GetColor()
{
    return m_vColor;
}

// FUNCTION: SWKOTOR 0x0045BB00
void CAurGUIStringInternal::SetColor(const GUIVector& v)
{
    m_vColor = v;
}

// FUNCTION: SWKOTOR 0x0045BB20
int CAurGUIStringInternal::GetLength()
{
    return strlen(m_szString);
}

// FUNCTION: SWKOTOR 0x00458F70
void CAurGUIStringInternal::SetString(char* sz)
{
    if (strlen(sz) > strlen(m_szString)) {
        delete m_szString;
        m_szString = new char[strlen(sz) + 1];
    }
    strcpy(m_szString, sz);
}

// FUNCTION: SWKOTOR 0x0045BB30
void CAurGUIStringInternal::SetFlags(unsigned char n)
{
    m_nFlags = n;
}

unsigned char CAurGUIStringInternal::GetFlags()
{
    return m_nFlags;
}

// FUNCTION: SWKOTOR 0x0045BB40
void CAurGUIStringInternal::SetVAlign(unsigned char n)
{
    m_nFlags = (m_nFlags & 7) | n;
}

// FUNCTION: SWKOTOR 0x0045BB60
int CAurGUIStringInternal::GetVAlign()
{
    return m_nFlags & 0x38;
}

// FUNCTION: SWKOTOR 0x0045BB70
void CAurGUIStringInternal::SetHAlign(unsigned char n)
{
    m_nFlags = (m_nFlags & 0x38) | n;
}

// FUNCTION: SWKOTOR 0x0045BB90
int CAurGUIStringInternal::GetHAlign()
{
    return m_nFlags & 7;
}

// FUNCTION: SWKOTOR 0x0045BBA0
void CAurGUIStringInternal::SetCursor(short n)
{
    m_n44 = n;
}

// FUNCTION: SWKOTOR 0x0045BBB0
short CAurGUIStringInternal::GetCursor()
{
    return m_n44;
}

// FUNCTION: SWKOTOR 0x0045BBC0
void CAurGUIStringInternal::ClearCursor()
{
    m_n44 = -1;
}

// FUNCTION: SWKOTOR 0x0045BC60
int CAurGUIStringInternal::GetFontHeight()
{
    if (m_pFont.m_p)
        return m_pFont.m_p->GetHeight();
    return 0;
}

// FUNCTION: SWKOTOR 0x0045BD70
void CAurGUIStringInternal::SetFont(char* szFont)
{
    CAurFont* pFont = m_pFont.m_p;
    if (pFont) {
        m_pFont.Reset();
        AurTextureRelease(pFont);
    }
    if (szFont == 0)
        m_pFont.Reset();
    else
        m_pFont = AurTextureGetReference(szFont, 0);
    m_lA.num = 0;
}

// FUNCTION: SWKOTOR 0x00459610
int CAurGUIStringInternal::GetFontScale()
{
    return (int)(m_pFont.m_p->GetInfo()->m_fScale * 100.0f + 0.5f);
}

// FUNCTION: SWKOTOR 0x00458FE0
void CAurGUIStringInternal::GetPosition(float* pX, float* pY)
{
    *pX = (float)m_nX / g_aGUIRects[g_nGUILayer][2];
    *pY = (float)m_nY / g_aGUIRects[g_nGUILayer][3];
}

// FUNCTION: SWKOTOR 0x0045BBE0
void CAurGUIStringInternal::GetPixelPosition(int* pX, int* pY)
{
    *pX = m_nX;
    *pY = m_nY;
}

// FUNCTION: SWKOTOR 0x00459030
void CAurGUIStringInternal::SetPosition(float fX, float fY)
{
    m_nX = (int)(g_aGUIRects[g_nGUILayer][2] * fX);
    m_nY = (int)(g_aGUIRects[g_nGUILayer][3] * fY);
}

// FUNCTION: SWKOTOR 0x0045BC00
void CAurGUIStringInternal::SetPixelPosition(int nX, int nY)
{
    m_nX = nX;
    m_nY = nY;
}

// FUNCTION: SWKOTOR 0x00459090
void CAurGUIStringInternal::GetSize(float* pW, float* pH)
{
    *pW = (float)m_nW / g_aGUIRects[g_nGUILayer][2];
    *pH = (float)m_nH / g_aGUIRects[g_nGUILayer][3];
}

// FUNCTION: SWKOTOR 0x0045BC20
void CAurGUIStringInternal::GetPixelSize(int* pW, int* pH)
{
    *pW = m_nW;
    *pH = m_nH;
}

// FUNCTION: SWKOTOR 0x004590E0
void CAurGUIStringInternal::SetSize(float fW, float fH)
{
    m_nW = (int)(g_aGUIRects[g_nGUILayer][2] * fW);
    m_nH = (int)(g_aGUIRects[g_nGUILayer][3] * fH);
}

// FUNCTION: SWKOTOR 0x0045BC40
void CAurGUIStringInternal::SetPixelSize(int nW, int nH)
{
    m_nW = nW;
    m_nH = nH;
}

// FUNCTION: SWKOTOR 0x00459630
void CAurGUIStringInternal::DrawBounds(int a, int b, int c, int d)
{
    if (g_bAurNoGUIDraw)
        return;
    glDisable(g_nGLDepthTest);
    glDisable(g_nGLTexture2D);
    glDisable(g_nGLBlend);
    float fY0 = 1.0f - m_nY / (float)g_aGUIRects[g_nGUILayer][3];
    float fY1 = 1.0f - (m_nY + m_nH) / (float)g_aGUIRects[g_nGUILayer][3];
    float fX0 = m_nX / (float)g_aGUIRects[g_nGUILayer][2];
    float fX1 = (m_nX + m_nW) / (float)g_aGUIRects[g_nGUILayer][2];
    glBegin(g_nGLLineStrip);
    {
        unsigned int nColor = 0xffe5a566;
        glColor4ubv((unsigned char*)&nColor);
    }
    glVertex4f(fX0, fY0, 0.0f, 1.0f);
    glVertex4f(fX0, fY1, 0.0f, 1.0f);
    glVertex4f(fX1, fY1, 0.0f, 1.0f);
    glVertex4f(fX1, fY0, 0.0f, 1.0f);
    glVertex4f(fX0, fY1, 0.0f, 1.0f);
    glEnd();
    glEnable(g_nGLDepthTest);
    glEnable(g_nGLBlend);
    glEnable(g_nGLTexture2D);
}

// ---------------------------------------------------------------------------
// CAurGUIImageInternal

class CAurGUITexture {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44();
    virtual int GetWidth();                     // +0x48
    virtual int GetHeight();                    // +0x4c
};

class CAurGUIImageData {
public:
    CAurGUITexture* m_pTexture;                 // +0x00
    char pad_0x4[0x48];

    CAurGUIImageData();                         // 0x0047B290
    ~CAurGUIImageData();                        // 0x0047B980
    void Bind();                                // 0x0047ADF0
    void Apply();                               // 0x0047B000
    int GetTextureId();                         // 0x0047AF00
    void Load(char* szName, int a, int b, int c);   // 0x0047B560
};

class CAurGUIImageInternal : public CAurGUIImage {
public:
    CAurGUIImageData* m_pData;                  // +0x04

    CAurGUIImageInternal();                     // 0x00459790
    CAurGUIImageInternal(char* szName);         // 0x004598A0
    virtual void Draw();                        // +0x00
    virtual ~CAurGUIImageInternal();            // +0x04
    virtual void SetImage(char* szName);        // +0x08
    virtual int GetHeight();                    // +0x0c
    virtual int GetWidth();                     // +0x10
    virtual void DrawRect(float fX, float fY, float fW, float fH,
        int a, int b, GUIVector* pColor, float fAlpha);     // +0x14
    virtual void DrawPixelRect(int nX, int nY, int nW, int nH,
        int a, int b, GUIVector* pColor, float fAlpha);     // +0x18
    virtual void DrawAlpha(float fAlpha);       // +0x1c
    virtual void DrawRect2(float fX, float fY, float fW, float fH,
        int a, int b, GUIVector* pColor, float fAlpha);     // +0x20
    virtual void DrawPixelRect2(int nX, int nY, int nW, int nH,
        int a, int b, GUIVector* pColor, float fAlpha);     // +0x24
};

// FUNCTION: SWKOTOR 0x00459790 SYMBOL: ??0CAurGUIImageInternal@@QAE@XZ
CAurGUIImageInternal::CAurGUIImageInternal()
{
    m_pData = new CAurGUIImageData;
}

// FUNCTION: SWKOTOR 0x00459800
void CAurGUIImageInternal::DrawPixelRect2(int nX, int nY, int nW, int nH,
    int a, int b, GUIVector* pColor, float fAlpha)
{
    if (g_bAurNoGUIDraw)
        return;
    DrawRect2(nX / (float)g_aGUIRects[g_nGUILayer][2], nY / (float)g_aGUIRects[g_nGUILayer][3],
        nW / (float)g_aGUIRects[g_nGUILayer][2], nH / (float)g_aGUIRects[g_nGUILayer][3],
        a, b, pColor, fAlpha);
}

// FUNCTION: SWKOTOR 0x00459880
void CAurGUIImageInternal::SetImage(char* szName)
{
    m_pData->Load(szName, 0, 0, 0);
}

// FUNCTION: SWKOTOR 0x004598A0 SYMBOL: ??0CAurGUIImageInternal@@QAE@PAD@Z
CAurGUIImageInternal::CAurGUIImageInternal(char* szName)
{
    m_pData = new CAurGUIImageData;
    m_pData->Load(szName, 0, 0, 0);
}

// FUNCTION: SWKOTOR 0x00459920
void CAurGUIImageInternal::DrawPixelRect(int nX, int nY, int nW, int nH,
    int a, int b, GUIVector* pColor, float fAlpha)
{
    if (g_bAurNoGUIDraw)
        return;
    g_nGUIImageFlag = 0;
    DrawRect(nX / (float)g_aGUIRects[g_nGUILayer][2], nY / (float)g_aGUIRects[g_nGUILayer][3],
        nW / (float)g_aGUIRects[g_nGUILayer][2], nH / (float)g_aGUIRects[g_nGUILayer][3],
        a, b, pColor, fAlpha);
}

// FUNCTION: SWKOTOR 0x0045A090
int CAurGUIImageInternal::GetHeight()
{
    return m_pData->m_pTexture->GetHeight();
}

// FUNCTION: SWKOTOR 0x0045A0A0
int CAurGUIImageInternal::GetWidth()
{
    return m_pData->m_pTexture->GetWidth();
}

// FUNCTION: SWKOTOR 0x0045A0B0 SYMBOL: ?Draw@CAurGUIImageInternal@@UAEXXZ
void CAurGUIImageInternal::Draw()
{
    CAurGUITexture* pTexture = m_pData->m_pTexture;
    GUIVector v(-1.0f, -1.0f, -1.0f);
    DrawPixelRect(0, 0, pTexture->GetWidth(), pTexture->GetHeight(), 0, 0, &v, 1.0f);
}

// FUNCTION: SWKOTOR 0x0045A110
void CAurGUIImageInternal::DrawAlpha(float fAlpha)
{
    CAurGUITexture* pTexture = m_pData->m_pTexture;
    GUIVector v(-1.0f, -1.0f, -1.0f);
    DrawPixelRect(0, 0, pTexture->GetWidth(), pTexture->GetHeight(), 0, 0, &v, fAlpha);
}

// ---------------------------------------------------------------------------
// Batched GUI image drawing, destructor, factory

extern int g_nGUIBufferCount;                   // 0x007BB4D4
extern int g_nGUIBatchTexture;                  // 0x007BB4D8
extern CAurGUIImageData* g_pGUIBatchData;       // 0x007BB4DC
extern unsigned char g_bGUIBatchTriangles;      // 0x007BB4E0
extern float g_afGUIAlpha[];                    // 0x007B9248
extern float g_fGUIFlushAlpha;                  // 0x0078E43C
extern char g_aGUIVertexBuffer[];               // 0x007B94D0

extern "C" __declspec(dllimport) void __stdcall glColor4f(float r, float g, float b, float a);

void AurGUIResetState();                                // 0x00421EE0
void AurDrawGUIQuads(void* pBuffer, int nQuads);        // 0x00431EB0
void AurDrawGUITriangles(void* pBuffer, int nTris);     // 0x00431CC0

inline float GUIGetAlpha()
{
    float f = 1.0f;
    for (int i = 0; i <= g_nGUILayer; i++)
        f *= g_afGUIAlpha[i];
    return f;
}

class CAurGUIImageBatch {
public:
    static void FlushBuffer(float fAlpha);
};

// FUNCTION: SWKOTOR 0x0045A170
void CAurGUIImageBatch::FlushBuffer(float fAlpha)
{
    if (g_nGUIBufferCount > 0) {
        glDisable(g_nGLDepthTest);
        g_nGUIImageFlag = 0;
        AurGUIResetState();
        if (g_pGUIBatchData) {
            g_pGUIBatchData->Bind();
            g_pGUIBatchData->Apply();
        }
        if (!g_bGUIBatchTriangles) {
            glColor4f(1.0f, 1.0f, 1.0f, GUIGetAlpha() * fAlpha);
            AurDrawGUIQuads(g_aGUIVertexBuffer, g_nGUIBufferCount / 5);
        }
        else
            AurDrawGUITriangles(g_aGUIVertexBuffer, g_nGUIBufferCount / 6);
        glEnable(g_nGLDepthTest);
    }
    g_nGUIBufferCount = 0;
}

// FUNCTION: SWKOTOR 0x0045A260
CAurGUIImage* NewCAurGUIImage(char* szName)
{
    if (szName == 0)
        return new CAurGUIImageInternal();
    return new CAurGUIImageInternal(szName);
}

// FUNCTION: SWKOTOR 0x0045B200
CAurGUIImageInternal::~CAurGUIImageInternal()
{
    if (g_nGUIBatchTexture == m_pData->GetTextureId()) {
        CAurGUIImageBatch::FlushBuffer(g_fGUIFlushAlpha);
        g_nGUIBatchTexture = 0;
        g_pGUIBatchData = 0;
    }
    delete m_pData;
}

// FUNCTION: SWKOTOR 0x0045B970 SYMBOL: ??_GCAurGUIImageInternal@@UAEPAXI@Z

// ---------------------------------------------------------------------------

// FUNCTION: SWKOTOR 0x0045B990
CAurGUIStringInternal::CAurGUIStringInternal(char* szText, char* szFont, GUIVector vColor, int n)
{
    if (szFont == 0)
        m_pFont.Reset();
    else
        m_pFont = AurTextureGetReference(szFont, 0);
    m_szString = new char[strlen(szText) + 1];
    strcpy(m_szString, szText);
    m_lB.Add(strlen(szText));
    m_vColor = vColor;
    m_nX = 0;
    m_nY = 0;
    m_nW = 0;
    m_nH = 0;
    m_n44 = -1;
    m_nFlags = 9;
    m_n40 = n;
}
