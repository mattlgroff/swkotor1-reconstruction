// Binds all client arrays of an interleaved trimesh vertex buffer
// (position, normal, colour, four UV sets, tangent basis and skin
// weights). Behavioural reference: src/units/0735/b922350f9b-f2_gl.cpp.
#include "g/request_pool.h"

extern "C" {
__declspec(dllimport) void __stdcall glVertexPointer(int size, unsigned int type, int stride, const void* p);
__declspec(dllimport) void __stdcall glColorPointer(int size, unsigned int type, int stride, const void* p);
__declspec(dllimport) void __stdcall glNormalPointer(unsigned int type, int stride, const void* p);
__declspec(dllimport) void __stdcall glTexCoordPointer(int size, unsigned int type, int stride, const void* p);
}

#define GL_FLOAT 0x1406
#define GL_UNSIGNED_BYTE 0x1401
#define GL_TEXTURE0_ARB 0x84c0

typedef void(__stdcall* PFNGLACTIVETEXTUREARB)(unsigned int tex);
extern PFNGLACTIVETEXTUREARB glClientActiveTextureARB;     // 0x007bb6b0
typedef void(__stdcall* PFNVERTEXATTRIBPTR)(unsigned int index, int size,
    unsigned int type, unsigned char normalized, int stride, const void* p);
typedef void(__stdcall* PFNVERTEXATTRIBPTRNV)(unsigned int index, int size,
    unsigned int type, int stride, const void* p);
typedef void(__stdcall* PFNVERTEXATTRIBARRAY)(unsigned int index);
extern PFNVERTEXATTRIBPTR g_pglVertexAttribPointerARB;      // 0x007bb7d8
extern PFNVERTEXATTRIBPTRNV g_pglVertexAttribPointerNV;     // 0x007bb854
extern PFNVERTEXATTRIBARRAY g_pfnEnableVertexAttribArray;   // 0x007bb79c
extern PFNVERTEXATTRIBARRAY g_pfnDisableVertexAttribArray;  // 0x007bb770
extern int g_bARBVertexProgram;                            // 0x0078e5ec
extern int g_bNVVertexProgram;                             // 0x0078e8c0
extern int g_nCurrentStage;                                // 0x007a6898

int HasMultiTexture();              // 0x0045f710
int HasVertexBuffers();             // 0x0045fc90
int AurVertexProgramsAvailable();   // 0x0045f770

// Skin weight / bone index offsets in the interleaved vertex.
class MdlNodeSkin {
public:
    char pad00[0x1a8];
    int m_nWeightOffset;            // +0x1a8
    int m_nBoneOffset;              // +0x1ac
};

class MdlNode {
public:
    MdlNodeSkin* AsMdlNodeSkin();   // 0x0043e3f0
};

// Interleaved vertex layout of a trimesh node.
class MdlNodeTriMesh : public MdlNode {
public:
    char pad00[0x14c];
    int m_nVertexStride;            // +0x14c
    char pad150[0x154 - 0x150];
    int m_nVertexOffset;            // +0x154
    int m_nNormalOffset;            // +0x158
    int m_nColorOffset;             // +0x15c
    int m_nTVertOffset[4];          // +0x160
    int m_nTangentOffset;           // +0x170
};

static inline void SetClientStage(int nStage)
{
    if (HasMultiTexture())
        glClientActiveTextureARB(GL_TEXTURE0_ARB + nStage);
    g_nCurrentStage = nStage;
}

static inline void VertexAttribPointer(unsigned int nIndex, int nSize, unsigned int eType,
                                       int nStride, const void* p)
{
    if (g_bARBVertexProgram)
        g_pglVertexAttribPointerARB(nIndex, nSize, eType, 0, nStride, p);
    else if (g_bNVVertexProgram && AurVertexProgramsAvailable())
        g_pglVertexAttribPointerNV(nIndex, nSize, eType, nStride, p);
}

// FUNCTION: SWKOTOR 0x00425520
void AurVARBindVertices(int a, int nFlags, int nId, int nCount, MdlNodeTriMesh* pNode, int nOffset)
{
    char* pBase;
    if (HasVertexBuffers()) {
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, g_pRequestPoolManager->GetBuffer(nId));
        pBase = (char*)nOffset;
    } else {
        pBase = (char*)g_pRequestPoolManager->GetBuffer(nId) + nOffset;
    }
    unsigned char nMask = (unsigned char)nFlags;
    if (nMask & 1)
        glVertexPointer(3, GL_FLOAT, pNode->m_nVertexStride, pBase + pNode->m_nVertexOffset);
    if (nMask & 0x20)
        glNormalPointer(GL_FLOAT, pNode->m_nVertexStride, pBase + pNode->m_nNormalOffset);
    if (nMask & 0x40)
        glColorPointer(4, GL_UNSIGNED_BYTE, pNode->m_nVertexStride, pBase + pNode->m_nColorOffset);
    if (nMask & 2) {
        SetClientStage(0);
        glTexCoordPointer(2, GL_FLOAT, pNode->m_nVertexStride, pBase + pNode->m_nTVertOffset[0]);
    }
    if (nMask & 4) {
        SetClientStage(1);
        glTexCoordPointer(2, GL_FLOAT, pNode->m_nVertexStride, pBase + pNode->m_nTVertOffset[1]);
    }
    if (nMask & 8) {
        SetClientStage(2);
        glTexCoordPointer(2, GL_FLOAT, pNode->m_nVertexStride, pBase + pNode->m_nTVertOffset[2]);
    }
    if (nMask & 0x10) {
        SetClientStage(3);
        glTexCoordPointer(2, GL_FLOAT, pNode->m_nVertexStride, pBase + pNode->m_nTVertOffset[3]);
    }
    SetClientStage(0);
    if (!AurVertexProgramsAvailable())
        return;
    if (nMask & 0x80) {
        VertexAttribPointer(0xc, 3, GL_FLOAT, pNode->m_nVertexStride, pBase + pNode->m_nTangentOffset);
        VertexAttribPointer(0xd, 3, GL_FLOAT, pNode->m_nVertexStride, pBase + pNode->m_nTangentOffset + 0xc);
        VertexAttribPointer(0xe, 3, GL_FLOAT, pNode->m_nVertexStride, pBase + pNode->m_nTangentOffset + 0x18);
        g_pfnEnableVertexAttribArray(0xc);
        g_pfnEnableVertexAttribArray(0xd);
        g_pfnEnableVertexAttribArray(0xe);
    } else {
        g_pfnDisableVertexAttribArray(0xc);
        g_pfnDisableVertexAttribArray(0xd);
        g_pfnDisableVertexAttribArray(0xe);
    }
    if (pNode->AsMdlNodeSkin()) {
        VertexAttribPointer(1, 4, GL_FLOAT, pNode->m_nVertexStride,
                            pBase + pNode->AsMdlNodeSkin()->m_nWeightOffset);
        VertexAttribPointer(4, 4, GL_FLOAT, pNode->m_nVertexStride,
                            pBase + pNode->AsMdlNodeSkin()->m_nBoneOffset);
        g_pfnEnableVertexAttribArray(1);
        g_pfnEnableVertexAttribArray(4);
    } else {
        g_pfnDisableVertexAttribArray(1);
        g_pfnDisableVertexAttribArray(4);
    }
}
