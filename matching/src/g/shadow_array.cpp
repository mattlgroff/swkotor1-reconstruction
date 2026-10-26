// Stencil shadow volumes: building the extruded silhouette vertex array and
// drawing it with the edge strip indices. Behavioural reference:
// src/units/0806/e9d88e6c4d-m47_modelb_shadow.cpp.
#include "g/glrender.h"

extern "C" {
__declspec(dllimport) void __stdcall glEnable(unsigned int cap);
__declspec(dllimport) void __stdcall glStencilOp(unsigned int fail, unsigned int zfail, unsigned int zpass);
__declspec(dllimport) void __stdcall glStencilMask(unsigned int mask);
__declspec(dllimport) void __stdcall glStencilFunc(unsigned int func, int ref, unsigned int mask);
__declspec(dllimport) void __stdcall glFrontFace(unsigned int mode);
}

#define GL_TRIANGLE_STRIP 5
#define GL_CULL_FACE 0x0b44
#define GL_CW 0x900
#define GL_CCW 0x901
#define GL_FRONT 0x404
#define GL_BACK 0x405
#define GL_ALWAYS 0x207
#define GL_KEEP 0x1e00
#define GL_INCR 0x1e02
#define GL_DECR 0x1e03
#define GL_INCR_WRAP_EXT 0x8507
#define GL_DECR_WRAP_EXT 0x8508
#define GL_STENCIL_TEST_TWO_SIDE_EXT 0x8910
#define GL_ARRAY_BUFFER_ARB 0x8892
#define GL_ELEMENT_ARRAY_BUFFER_ARB 0x8893

typedef void(__stdcall* PFNGLACTIVESTENCILFACEEXT)(unsigned int face);
extern PFNGLACTIVESTENCILFACEEXT glActiveStencilFaceEXT;   // 0x007bb624

// One silhouette edge: the two vertex indices.
struct ShadowEdge {
    unsigned short a;
    unsigned short b;
};

extern ShadowEdge g_aShadowEdges[];            // 0x007a69c0
extern unsigned short g_aShadowVertexMarks[];  // 0x007a4820
extern void* g_pShadowVertices;                // 0x007a47e8
extern int g_bShadowTwoSidedStencil;           // 0x007a68dc
extern int g_nShadowTriangles;                 // 0x00827fb0

int AurGLCaps9F0();                            // 0x0045f9f0 (two-sided stencil)

// Draws the shadow volume of the last BuildShadowArray: one strip of six
// indices per silhouette edge, capped by the light point (2n) and the far
// point (2n + 1).
// FUNCTION: SWKOTOR 0x00426660
void RenderShadowArray(int nVertices, int nEdges, int nUnused)
{
    static unsigned short* s_pIndices = new unsigned short[0x1800];
    if (!nEdges)
        return;
    int nNear = nVertices + nVertices;
    unsigned short* pIndices = s_pIndices;
    int nFar = nNear + 1;
    unsigned short* p = pIndices;
    for (int i = 0; i < nEdges; i++) {
        int a = g_aShadowVertexMarks[g_aShadowEdges[i].a] * 2 - 2;
        int b = g_aShadowVertexMarks[g_aShadowEdges[i].b] * 2 - 2;
        if (i % 2 == 0) {
            *p++ = nNear;
            *p++ = a;
            *p++ = b;
            *p++ = a + 1;
            *p++ = b + 1;
            *p++ = nFar;
        } else {
            *p++ = nFar;
            *p++ = b + 1;
            *p++ = a + 1;
            *p++ = b;
            *p++ = a;
            *p++ = nNear;
        }
    }
    if (HasVertexBuffers())
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
    glVertexPointer(3, GL_FLOAT, 12, g_pShadowVertices);
    if (g_bShadowTwoSidedStencil && AurGLCaps9F0()) {
        glDisable(GL_CULL_FACE);
        glEnable(GL_STENCIL_TEST_TWO_SIDE_EXT);
        glActiveStencilFaceEXT(GL_BACK);
        glStencilOp(GL_KEEP, GL_KEEP, GL_DECR_WRAP_EXT);
        glStencilMask(0xffffffff);
        glStencilFunc(GL_ALWAYS, 0, 0xffffffff);
        glActiveStencilFaceEXT(GL_FRONT);
        glStencilOp(GL_KEEP, GL_KEEP, GL_INCR_WRAP_EXT);
        glStencilMask(0xffffffff);
        if (HasVertexBuffers())
            glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
        glDrawElements(GL_TRIANGLE_STRIP, nEdges * 6, GL_UNSIGNED_SHORT, pIndices);
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glDisable(GL_STENCIL_TEST_TWO_SIDE_EXT);
    } else {
        glFrontFace(GL_CW);
        glStencilOp(GL_KEEP, GL_INCR, GL_KEEP);
        if (HasVertexBuffers())
            glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
        glDrawElements(GL_TRIANGLE_STRIP, nEdges * 6, GL_UNSIGNED_SHORT, pIndices);
        glFrontFace(GL_CCW);
        glStencilOp(GL_KEEP, GL_DECR, GL_KEEP);
        if (HasVertexBuffers())
            glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
        glDrawElements(GL_TRIANGLE_STRIP, nEdges * 6, GL_UNSIGNED_SHORT, pIndices);
    }
    g_nShadowTriangles += nEdges * 8;
}
