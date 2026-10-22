// immediate / client-array GL draw helpers (textured and coloured
// vertex lists, font strings, console overlay), 0x004318f0-0x00432780 and
// 0x0044cb90-0x0044d00e.
#ifndef G_GL_DRAW_H
#define G_GL_DRAW_H

#include "g/glrender.h"

extern "C" {
size_t __cdecl strlen(const char* s);
__declspec(dllimport) void __stdcall glEnable(unsigned int cap);
__declspec(dllimport) void __stdcall glEnableClientState(unsigned int a);
__declspec(dllimport) void __stdcall glDisableClientState(unsigned int a);
__declspec(dllimport) void __stdcall glTexCoord2f(float s, float t);
__declspec(dllimport) void __stdcall glVertex3f(float x, float y, float z);
__declspec(dllimport) void __stdcall glColor3f(float r, float g, float b);
__declspec(dllimport) void __stdcall glNormal3fv(const float* v);
__declspec(dllimport) void __stdcall glPushAttrib(unsigned int mask);
__declspec(dllimport) void __stdcall glPopAttrib();
__declspec(dllimport) void __stdcall glPushMatrix();
__declspec(dllimport) void __stdcall glPopMatrix();
__declspec(dllimport) void __stdcall glListBase(unsigned int base);
__declspec(dllimport) void __stdcall glCallLists(int n, unsigned int type, const void* lists);
__declspec(dllimport) void __stdcall glOrtho(double l, double r, double b, double t,
                                             double n, double f);
__declspec(dllimport) void __stdcall glRasterPos2f(float x, float y);
__declspec(dllimport) void __stdcall glBlendFunc(unsigned int s, unsigned int d);
__declspec(dllimport) void __stdcall glGetIntegerv(unsigned int pname, int* p);
__declspec(dllimport) void __stdcall glGetFloatv(unsigned int pname, float* p);
}

#pragma intrinsic(strlen)

#define GL_VERTEX_ARRAY 0x8074
#define GL_NORMAL_ARRAY 0x8075
#define GL_COLOR_ARRAY 0x8076
#define GL_TEXTURE_COORD_ARRAY 0x8078
#define GL_TRIANGLES 4
#define GL_QUADS 7

// 0x18-byte position / colour / texture-coordinate vertex.
struct VertexPCT {
    float v[3];             // +0x00
    unsigned char c[4];     // +0x0c
    float s, t;             // +0x10
};

// 0x14-byte position / texture-coordinate vertex.
struct VertexPT {
    float v[3];             // +0x00
    float s, t;             // +0x0c
};

// 0x10-byte position / colour vertex.
struct VertexPC {
    float v[3];             // +0x00
    unsigned char c[4];     // +0x0c
};

extern int g_bFontInitialised;                        // 0x007b90f0
extern unsigned int g_nFontListBase;                  // 0x007a47c0
void AurInitFont();                                   // 0x0044cc40
void RenderSquares(int nId);                          // 0x00426000
extern CExoArrayList<int> g_aRenderSquares;           // 0x007a693c
void AurDrawString(char* sz);                         // 0x00432040

void _glOrtho(double l, double r, double b, double t, double n, double f); // 0x00404b60
extern unsigned int g_aGLBlendFactors[];              // 0x0073f270
extern unsigned int g_aGLMatrixModes[];               // 0x0073f290
extern unsigned int g_nGLCap1;                        // 0x0078d474
extern unsigned int g_nGLCap4;                        // 0x0078d448

class CAurFontTexture {
public:
    virtual void V00();
    virtual void V04();
    virtual void V08();
    virtual void V0c();
    virtual void V10();
    virtual void V14();
    virtual void V18();
    virtual void V1c();
    virtual void V20();
    virtual void V24();
    virtual void V28();
    virtual void V2c();
    virtual void V30();
    virtual void V34();
    virtual void V38();
    virtual void V3c();
    virtual void V40();
    virtual void V44();
    virtual void V48();
    virtual void V4c();
    virtual void V50();
    virtual void V54();
    virtual void V58();
    virtual void V5c();
    virtual void V60();
    virtual void V64();
    virtual void V68();
    virtual void V6c();
    virtual void V70();
    virtual void V74();
    virtual void V78();
    virtual void V7c();
    virtual void V80();
    virtual void V84();
    virtual void V88();
    virtual void V8c();
    virtual void V90();
    virtual void V94();
    virtual void V98();
    virtual void V9c();
    virtual void Va0();
    virtual void Va4();
    virtual void Va8();
    virtual void Vac();
    virtual void Vb0();
    virtual void Bind();                              // 0xb4
};

class CAurFontRender {
public:
    char pad_0[0x1c];
    CAurFontTexture* m_pTexture;                      // +0x1c

    void Draw(char* sz, int nLength);                 // 0x004a1770
};

extern CExoArrayList<CAurFontRender*> g_aFonts;       // 0x0083052c
extern int g_nCurrentFont;                            // 0x0078e358

#endif
