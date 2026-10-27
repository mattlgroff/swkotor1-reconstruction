// GUI layer helpers, GL wrappers and CAurGUIString bits
// (0x00451D70, 0x00458EF0 .. 0x0045BE80).

#include <string.h>

extern "C" {
__declspec(dllimport) void __stdcall glBegin(unsigned int mode);
__declspec(dllimport) void __stdcall glEnd();
__declspec(dllimport) void __stdcall glVertex3fv(const float* v);
__declspec(dllimport) void __stdcall glScalef(float x, float y, float z);
__declspec(dllimport) void __stdcall glViewport(int x, int y, int w, int h);
__declspec(dllimport) void __stdcall glColor4ubv(const unsigned char* v);
__declspec(dllimport) void __stdcall glGetIntegerv(unsigned int pname, int* params);
__declspec(dllimport) void __stdcall glEnable(unsigned int cap);
__declspec(dllimport) void __stdcall glDisable(unsigned int cap);
__declspec(dllimport) void __stdcall glMatrixMode(unsigned int mode);
__declspec(dllimport) void __stdcall glPushMatrix();
__declspec(dllimport) void __stdcall glPopMatrix();
__declspec(dllimport) void __stdcall glLoadIdentity();
__declspec(dllimport) void __stdcall glOrtho(double l, double r, double b, double t,
    double n, double f);
__declspec(dllimport) void __stdcall glBlendFunc(unsigned int s, unsigned int d);
__declspec(dllimport) void __stdcall glVertex4f(float x, float y, float z, float w);
__declspec(dllimport) void __stdcall glClear(unsigned int mask);
}

#define GL_VIEWPORT 0x0ba2

class Vector {
public:
    float x, y, z;
};

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;
};

extern unsigned int g_aPrimitiveModes[];        // 0x0073F334
extern short g_nGUILayer;                       // 0x007B9460
extern int g_bAurNoGUIDraw;                     // 0x007BB4D0
extern float g_afGUIAlpha[];                    // 0x007B9248
extern short g_aGUIRects[][5];                  // 0x007B9468 {x, y, w, h, bClear}
extern unsigned int g_nGLProjection;            // 0x0073F290
extern unsigned int g_nGLModelView;             // 0x0073F298
extern unsigned int g_nGLDepthTest;             // 0x0078D45C
extern unsigned int g_nGLLighting;              // 0x0078D474
extern unsigned int g_nGLBlend;                 // 0x0078D448
extern unsigned int g_nGLSrcAlpha;              // 0x0073F270
extern unsigned int g_nGLOneMinusSrcAlpha;      // 0x0073F274
extern unsigned int g_nGLTexture2D;             // 0x0078D458
extern int g_nScreenWidth;                      // 0x0078D1D4
extern int g_nScreenHeight;                     // 0x0078D1D8

int operator!=(const Vector& a, const Vector& b);   // 0x0043E1F0

// Rounds to the nearest integer with the current FPU mode.
inline void AurRoundToInt(int* pOut, float f)
{
    __asm {
        fld f
        mov edx, pOut
        frndint
        fistp dword ptr [edx]
    }
}

class CAurGUIImage {
public:
    virtual ~CAurGUIImage();                    // 0x00459140
};

class CAurGUIStringInternal : public CAurGUIImage {
public:
    char pad_0x4[0x10];
    char* m_szString;                           // +0x14

    void SetStringSize(short nSize);            // 0x00458F10
};

// FUNCTION: SWKOTOR 0x00451D70
void DrawFromVerts(int nMode, CExoArrayList<Vector>* pVerts)
{
    glBegin(g_aPrimitiveModes[nMode]);
    for (int i = 0; i < pVerts->num; i++)
        glVertex3fv(&pVerts->element[i].x);
    glEnd();
}

// FUNCTION: SWKOTOR 0x00458EF0
float AurGUIGetAlpha()
{
    float f = 1.0f;
    for (int i = 0; i <= g_nGUILayer; i++)
        f *= g_afGUIAlpha[i];
    return f;
}

// FUNCTION: SWKOTOR 0x00458F10
void CAurGUIStringInternal::SetStringSize(short nSize)
{
    if (nSize > (short)strlen(m_szString)) {
        char* sz = new char[nSize + 1];
        strcpy(sz, m_szString);
        delete m_szString;
        m_szString = sz;
    }
    else
        m_szString[nSize] = 0;
}

// FUNCTION: SWKOTOR 0x00459140
CAurGUIImage::~CAurGUIImage()
{
}

// FUNCTION: SWKOTOR 0x00459150
void _glScalef(float x, float y, float z)
{
    glScalef(x, y, z);
}

// FUNCTION: SWKOTOR 0x00459170
void AurSetViewport(int x, int y, int w, int h)
{
    glViewport(x, y, w, h);
}

// FUNCTION: SWKOTOR 0x00459190
void AurSetColor(unsigned int nColor)
{
    unsigned int c = nColor;
    glColor4ubv((unsigned char*)&c);
}

// FUNCTION: SWKOTOR 0x004591B0
void AurGUIStartLayer()
{
    int aViewport[4];
    glGetIntegerv(GL_VIEWPORT, aViewport);
    g_aGUIRects[0][0] = aViewport[0];
    g_aGUIRects[0][1] = aViewport[1];
    g_aGUIRects[0][2] = aViewport[2];
    g_aGUIRects[0][3] = aViewport[3];
    g_aGUIRects[0][4] = 0;
    g_afGUIAlpha[0] = 1.0f;
    g_nGUILayer = 0;
    glDisable(g_nGLDepthTest);
    glDisable(g_nGLLighting);
    glMatrixMode(g_nGLProjection);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
    glMatrixMode(g_nGLModelView);
    glLoadIdentity();
    glEnable(g_nGLBlend);
    glBlendFunc(g_nGLSrcAlpha, g_nGLOneMinusSrcAlpha);
}

// FUNCTION: SWKOTOR 0x004592B0
void AurGUIStopLayer()
{
    g_nGUILayer = -1;
    glMatrixMode(g_nGLProjection);
    glPopMatrix();
    glMatrixMode(g_nGLModelView);
    glEnable(g_nGLDepthTest);
    glEnable(g_nGLLighting);
}

// NONMATCH: the original keeps the frame stores in source order (ours
// hoists the layer and clear-flag stores) and loads the layer without
// clearing edx first; registers, colour packing and quad match.
// FUNCTION: SWKOTOR 0x004592F0
int AurGUISetupViewport(int x, int y, int w, int h, Vector* pColor, unsigned char bClear, float fAlpha)
{
    if (g_bAurNoGUIDraw)
        return 0;
    if (x + w < 0 || y + h < 0)
        return 0;
    if (w <= 0 || h <= 0)
        return 0;
    if (x >= g_nScreenWidth || y >= g_nScreenHeight)
        return 0;
    short nX = g_aGUIRects[g_nGUILayer][0] + x;
    short nY = g_aGUIRects[g_nGUILayer][1] + y;
    short nLayer = g_nGUILayer + 1;
    g_aGUIRects[nLayer][0] = nX;
    g_afGUIAlpha[nLayer] = fAlpha;
    g_aGUIRects[nLayer][1] = nY;
    g_aGUIRects[nLayer][2] = w;
    g_nGUILayer = nLayer;
    g_aGUIRects[nLayer][3] = h;
    g_aGUIRects[nLayer][4] = bClear;
    AurSetViewport(nX, g_aGUIRects[0][3] - nY - h, w, h);
    int r, g, b, a;
    AurRoundToInt(&r, pColor->x * 255.0f);
    AurRoundToInt(&g, pColor->y * 255.0f);
    AurRoundToInt(&b, pColor->z * 255.0f);
    AurRoundToInt(&a, fAlpha * 255.0f);
    unsigned int nColor = (((a << 8 | b) << 8 | g) << 8) | r;
    Vector vNone;
    vNone.x = -1.0f;
    vNone.y = -1.0f;
    vNone.z = -1.0f;
    if (*pColor != vNone) {
        glDisable(g_nGLTexture2D);
        glDisable(g_nGLDepthTest);
        glBegin(g_aPrimitiveModes[6]);
        AurSetColor(nColor);
        glVertex4f(0.0f, 0.0f, 0.0f, 1.0f);
        glVertex4f(1.0f, 0.0f, 0.0f, 1.0f);
        glVertex4f(1.0f, 1.0f, 0.0f, 1.0f);
        glVertex4f(0.0f, 1.0f, 0.0f, 1.0f);
        glEnd();
        glEnable(g_nGLTexture2D);
        glEnable(g_nGLDepthTest);
    }
    if (bClear) {
        glClear(0x500);
        glEnable(g_nGLLighting);
    }
    return 1;
}

// The frames are a short[][5] table: indexing a 10-byte struct array
// emits scaled field loads instead of the original row offset.
// FUNCTION: SWKOTOR 0x00459580
void AurGUICloseViewport()
{
    if (g_bAurNoGUIDraw)
        return;

    g_nGUILayer--;
    int h = g_aGUIRects[g_nGUILayer][3];
    int w = g_aGUIRects[g_nGUILayer][2];
    int y = g_aGUIRects[0][3] - g_aGUIRects[g_nGUILayer][1] - h;
    int x = g_aGUIRects[g_nGUILayer][0];
    glViewport(x, y, w, h);

    if (g_aGUIRects[g_nGUILayer][4])
        glEnable(g_nGLLighting);
    else
        glDisable(g_nGLLighting);
}
