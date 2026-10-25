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

struct GUIRect {
    short x;
    short y;
    short w;
    short h;
    short bClear;
};

extern unsigned int g_aPrimitiveModes[];        // 0x0073F334
extern short g_nGUILayer;                       // 0x007B9460
extern int g_bAurNoGUIDraw;                     // 0x007BB4D0
extern float g_afGUIAlpha[];                    // 0x007B9248
extern GUIRect g_aGUIRects[];                   // 0x007B9468
extern unsigned int g_nGLProjection;            // 0x0073F290
extern unsigned int g_nGLModelView;             // 0x0073F298
extern unsigned int g_nGLDepthTest;             // 0x0078D45C
extern unsigned int g_nGLLighting;              // 0x0078D474
extern unsigned int g_nGLBlend;                 // 0x0078D448
extern unsigned int g_nGLSrcAlpha;              // 0x0073F270
extern unsigned int g_nGLOneMinusSrcAlpha;      // 0x0073F274

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
    g_aGUIRects[0].x = aViewport[0];
    g_aGUIRects[0].y = aViewport[1];
    g_aGUIRects[0].w = aViewport[2];
    g_aGUIRects[0].h = aViewport[3];
    g_aGUIRects[0].bClear = 0;
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

// NONMATCH (16 insns): MSVC folds the frame index into scaled field loads;
// original materializes the 10-byte frame offset before reading its fields.
// FUNCTION: SWKOTOR 0x00459580
void AurGUICloseViewport()
{
    if (g_bAurNoGUIDraw)
        return;

    g_nGUILayer--;
    GUIRect* previous = &g_aGUIRects[g_nGUILayer];
    int h = previous->h;
    int w = previous->w;
    int y = g_aGUIRects[0].h - previous->y - h;
    int x = previous->x;
    glViewport(x, y, w, h);

    previous = &g_aGUIRects[g_nGUILayer];
    if (previous->bClear)
        glEnable(g_nGLLighting);
    else
        glDisable(g_nGLLighting);
}
