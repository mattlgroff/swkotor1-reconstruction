// GLRender helpers (render pools, client-array setup, small vector
// math), 0x00423a90-0x00427450.
#ifndef G_GLRENDER_H
#define G_GLRENDER_H

#include "g/request_pool.h"

extern "C" {
__declspec(dllimport) void __stdcall glDisable(unsigned int cap);
__declspec(dllimport) void __stdcall glColor4f(float r, float g, float b, float a);
__declspec(dllimport) void __stdcall glMatrixMode(unsigned int mode);
__declspec(dllimport) void __stdcall glLoadIdentity();
__declspec(dllimport) void __stdcall glMultMatrixf(const float* m);
__declspec(dllimport) void __stdcall glRotatef(float a, float x, float y, float z);
__declspec(dllimport) void __stdcall glTranslatef(float x, float y, float z);
__declspec(dllimport) void __stdcall glVertexPointer(int size, unsigned int type, int stride, const void* p);
__declspec(dllimport) void __stdcall glColorPointer(int size, unsigned int type, int stride, const void* p);
__declspec(dllimport) void __stdcall glNormalPointer(unsigned int type, int stride, const void* p);
__declspec(dllimport) void __stdcall glTexCoordPointer(int size, unsigned int type, int stride, const void* p);
__declspec(dllimport) void __stdcall glDrawElements(unsigned int mode, int count, unsigned int type, const void* p);
__declspec(dllimport) void __stdcall glDrawArrays(unsigned int mode, int first, int count);
__declspec(dllimport) void __stdcall glBegin(unsigned int mode);
__declspec(dllimport) void __stdcall glEnd();
__declspec(dllimport) void __stdcall glVertex3fv(const float* v);
__declspec(dllimport) void __stdcall glColor4ubv(const unsigned char* v);
__declspec(dllimport) void* __stdcall wglGetCurrentContext();
__declspec(dllimport) void* __stdcall wglGetCurrentDC();
double __cdecl acos(double x);
double __cdecl sin(double x);
}
#pragma intrinsic(acos, sin)

#define GL_FLOAT 0x1406
#define GL_UNSIGNED_BYTE 0x1401
#define GL_UNSIGNED_SHORT 0x1403
#define GL_QUADS 7
#define GL_TEXTURE0_ARB 0x84c0

// GL extension entry points (function-pointer globals).
typedef void(__stdcall* PFNGLACTIVETEXTUREARB)(unsigned int tex);
extern PFNGLACTIVETEXTUREARB glClientActiveTextureARB;     // 0x007bb6b0
extern PFNGLACTIVETEXTUREARB glActiveTextureARB;           // 0x007bb80c

int HasMultiTexture();     // 0x0045f710
int HasVertexBuffers();    // 0x0045fc90
void ActivateTextureStage(int nStage);   // 0x00425250

class Vector {
public:
    float x, y, z;

    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
};

class Quaternion {
public:
    Quaternion() {}
    Quaternion(const Quaternion& o) { w = o.w; x = o.x; y = o.y; z = o.z; }
    float w, x, y, z;

    Vector axis() const;
};

class Matrix {
public:
    Matrix() {}
    float m[16];
};

Quaternion operator-(const Quaternion& q);                       // 0x004a9b50
Matrix MakeMatrix(const Vector& v, const Quaternion& q);         // 0x004aa9c0

// WGL_ARB_pixel_format / WGL_ARB_pbuffer entry points.
typedef int(__stdcall* PFNWGLCHOOSEPIXELFORMATARB)(void* hdc, const int* piAttribs,
                                                   const float* pfAttribs, unsigned int nMax,
                                                   int* piFormats, unsigned int* pnFormats);
typedef void*(__stdcall* PFNWGLCREATEPBUFFERARB)(void* hdc, int iFormat, int iWidth,
                                                 int iHeight, const int* piAttribs);
extern PFNWGLCHOOSEPIXELFORMATARB wglChoosePixelFormatARB;   // 0x007bb730
extern PFNWGLCREATEPBUFFERARB wglCreatePbufferARB;           // 0x007bb530

typedef void*(__stdcall* PFNWGLGETPBUFFERDCARB)(void* hPbuffer);
extern PFNWGLGETPBUFFERDCARB wglGetPbufferDCARB;             // 0x007bb6b8
extern "C" {
__declspec(dllimport) void __stdcall glGenTextures(int n, unsigned int* textures);
__declspec(dllimport) void __stdcall glBindTexture(unsigned int target, unsigned int texture);
__declspec(dllimport) void* __stdcall wglCreateContext(void* hdc);
__declspec(dllimport) int __stdcall wglShareLists(void* hglrc1, void* hglrc2);
__declspec(dllimport) int __stdcall wglMakeCurrent(void* hdc, void* hglrc);
}

// One 0x1c-byte imposter render target.
struct Imposter {
    unsigned char m_bUsed;   // +0x00
    unsigned int m_nTexture; // +0x04
    void* m_hRC;             // +0x08
    void* m_hPbuffer;        // +0x0c
    void* m_hDC;             // +0x10
    void* m_pVertices;       // +0x14
    int m_nVertices;         // +0x18

    Imposter() { m_bUsed = 0; }
};

class ImposterManager {
public:
    Imposter* m_pImposters;  // +0x00
    int m_nImposters;        // +0x04

    int AllocateImposter(int nIndex, int nVertices, const void* pVertices);
    int SetUp(int nVertices, const void* pVertices);    // 0x00432090
};
extern ImposterManager g_ImposterManager;          // 0x0078db30

extern int g_bUseViewMatrix;                       // 0x0078d494

extern int g_nCurrentStage;                        // 0x007a6898
extern int g_aCurrentStageTID[6];                  // 0x007a683c
extern int g_nFilmNoiseTexture;                    // 0x007a68d0
extern void* g_hGLContext;                         // 0x007a6854
extern void* g_hGLDC;                              // 0x007a47e4
extern int g_n827fa0;                              // 0x00827fa0
extern int g_n827fa4;                              // 0x00827fa4
extern unsigned int g_aPrimitiveModes[];           // 0x0073f334

// Just the texture-coordinate fields of a trimesh node.
class MdlNodeTriMesh {
public:
    char pad00[0x14c];
    int m_nVertexStride;                    // +0x14c
    char pad150[0x160 - 0x150];
    int m_nTVertOffset;                     // +0x160
    int m_nTVertOffset1;                    // +0x164
};

void SetLightmapTexCoordToStage0(int a, int b, int nId, int nCount, MdlNodeTriMesh* pNode,
                                 int nOffset);   // 0x00425280
void SetDecalTexCoordToStage0(int a, int b, int nId, int nCount, MdlNodeTriMesh* pNode,
                              int nOffset);      // 0x004253d0

#endif
