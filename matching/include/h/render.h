// shared declarations of the PartTriMesh render paths
// (0x0046F900-0x00477830).
#ifndef MATCHING_H_RENDER_H
#define MATCHING_H_RENDER_H

#include "h/renderpaths.h"
#include "h/glcaps.h"

namespace GLRender {
void DrawElements(int eMode, int nCount, int nOffset, void* pData);    // 0x00425AB0
void ActivateTextureStage(int nStage);                              // 0x00425250
void ActivateTextureUnit(int nStage);                               // 0x0041FE80
}

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;
};

class CAurTexture {
public:
    virtual void v00(); virtual void v04(); virtual void v08();
    virtual unsigned char HasCubeMap();         // +0x0c
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7c();
    virtual void v80(); virtual void v84(); virtual void v88();
    virtual int GetKind();                      // +0x8c
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9c();
    virtual void va0(); virtual void va4(); virtual void va8(); virtual void vac();
    virtual void vb0();
    virtual void Bind();                        // +0xb4

    unsigned char IsProcedural();   // 0x00420600
};

class PartDynamic {
public:
    char m_pad00[0xc4];
    unsigned short m_bAlphaAnimated;    // +0xc4
    char m_padC6[0xd8 - 0xc6];
    float m_fAlpha;                 // +0xd8
    char m_padDC[0x11c - 0xdc];
    float m_fSelfIllum;             // +0x11c
    char m_pad120[0x1a8 - 0x120];
    CAurTexture* m_pEnvMap;         // +0x1a8
};

class Material {
public:
    CAurTexture* m_pTexture0;       // +0x00
    CAurTexture* m_pTexture1;       // +0x04
    char m_pad08[0x0c - 0x08];
    CAurTexture* m_pTexture3;       // +0x0c
    CAurTexture* m_pBumpMap;        // +0x10
    CAurTexture* m_pEnvMap;         // +0x14
    char m_pad18[0x2c - 0x18];
    int m_nSrcBlends;               // +0x2c (m_lSrcBlend.num)
    char m_pad30[0x44 - 0x30];
    int m_nRenderType;              // +0x44
    float m_fWaterAlpha;            // +0x48

    void BindTexture0();            // 0x0047ADF0
    void BindTexture1();            // 0x0047AE60
    void BindEnvMap();              // 0x0047AEB0
    unsigned char HasEnvMap();      // 0x0047B0E0
    unsigned char HasBumpMap();     // 0x0047B8F0
    unsigned char IsBumpyShiny();   // 0x0047B0C0
    unsigned char HasNormalMap();   // 0x0041F250
    void SetBlendingMode(int nMode);    // 0x0047AF70
};

class LightmapNode {
public:
    char m_pad00[0x53];
    unsigned char m_bFogged;        // +0x53
    char m_pad54[0xac - 0x54];
    int m_nLightmap;                // +0xac
};

class LightmapOwner {
public:
    LightmapNode* GetNode();        // 0x0043E1C0
};

class MdlNodeTriMesh;
class LightmapData {
public:
    char m_pad00[8];
    LightmapOwner* m_pOwner;        // +0x08
    char m_pad0C[0x14c - 0x0c];
    void* m_pTexCoords;             // +0x14c
    void* m_pTexCoords2;            // +0x150
    char m_pad154[0x180 - 0x154];
    short m_nStage;                 // +0x180
    char m_pad182[0x184 - 0x182];
    unsigned char m_bLightmapped;   // +0x184
    char m_pad185[0x194 - 0x185];
    int m_nOffset;                  // +0x194

    unsigned char HasTangentBases();    // 0x0043E410
};

class MdlNode {
public:
    char m_pad00[8];
    LightmapOwner* m_pOwner;        // +0x08

    LightmapData* GetLightmap();    // 0x0043E400
    void Prepare();                 // 0x0043E380
};

void SetLightmapTexCoordToStage(void* a, void* b, int nLightmap, int nStage,
                                LightmapData* pData, int nOffset);  // 0x00425280
void SetDecalTexCoordToStage0(void* a, void* b, int nLightmap, int nStage,
                              LightmapData* pData, int nOffset);    // 0x004253D0
void _glBlendFunc(int nSrc, int nDst);          // 0x00401670
void DoEnvMapTransform(CAurTexture* pTexture, unsigned int nUnit);      // 0x0046F780
void UndoEnvMapTransform(CAurTexture* pTexture, unsigned int nUnit);    // 0x0046F870

class MdlNodeTriMesh {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28();
    virtual MdlNodeTriMesh* GetTriMesh();       // +0x2c

    MdlNode* m_pNode;               // +0x04
    char m_pad08[0x40 - 0x08];
    PartDynamic* m_pDynamic;        // +0x40
    Material* m_pMaterial;          // +0x44
    char m_pad48[0x84 - 0x48];
    float m_fAlpha;                 // +0x84
    char m_pad88[0x100 - 0x88];
    CExoArrayList<int> m_lStripLengths;     // +0x100
    char m_pad10C[0x118 - 0x10C];
    CExoArrayList<void*> m_lStrips;         // +0x118
    char m_pad124[0x128 - 0x124];
    void* m_pIndices;               // +0x128
    unsigned int m_nIndices;        // +0x12c
    int m_eMode;                    // +0x130
    char m_pad134[0x160 - 0x134];
    int m_nTexCoords;               // +0x160
    char m_pad164[0x184 - 0x164];
    unsigned char m_bLightmapped;   // +0x184
    char m_pad185[0x194 - 0x185];
    int m_nIndexOffset;             // +0x194
};

class PartTriMesh {
public:
    char m_pad00[4];
    MdlNodeTriMesh* m_pMesh;        // +0x04
    MdlNodeTriMesh* m_pObject;      // +0x08
};

extern int g_bRenderFlat;           // 0x0078E614
extern int g_bRenderFlatKeepState;  // 0x0078E61C
extern int g_bRenderLightMapped;    // 0x0078E620
extern int g_bRegisterCombiners;    // 0x007B92A0
extern int g_nFlatParts;            // 0x00827FB4
extern int g_nFlatTriangles;        // 0x00827FB0
extern unsigned int g_eTexCoordArray;   // 0x0073F398
extern unsigned int g_aBlendFactors[];  // 0x0073F270

typedef void (__cdecl* PFNCOMBINER)(unsigned int, CAurTexture*, CAurTexture*, CAurTexture*, CAurTexture*);
extern PFNCOMBINER g_pfnCombiner;   // 0x007A47E0
typedef void (__stdcall* PFNACTIVETEXTURE)(unsigned int);
extern PFNACTIVETEXTURE g_pfnActiveTexture; // 0x007BB6B0
extern unsigned int g_eNormalArray;     // 0x0073F39C
extern unsigned int g_eColorArray;      // 0x0073F3A0
extern unsigned int g_eVertexArray;     // 0x0073F3A4

extern "C" __declspec(dllimport) void __stdcall glBlendFunc(unsigned int, unsigned int);

extern "C" __declspec(dllimport) void __stdcall glTexEnvi(unsigned int, unsigned int, int);
extern "C" __declspec(dllimport) void __stdcall glTexGeni(unsigned int, unsigned int, int);

// GL enums kept in .data.
extern unsigned int g_eGLTexture2D;     // 0x0078D44C
extern unsigned int g_eGLTexture2DB;    // 0x0078D458
extern unsigned int g_eGLLighting;      // 0x0078D45C
extern unsigned int g_eGLTexGenS;       // 0x0078D460
extern unsigned int g_eGLTexGenT;       // 0x0078D464
extern unsigned int g_eGLTexGenR;       // 0x0078D468
extern unsigned int g_eGLTexture3D;     // 0x0078D46C
extern unsigned int g_eGLTexCubeMap;    // 0x0078D478

extern int g_bRenderLightMappedNoShaders;   // 0x0078E628

extern int g_bRenderEnvMapped;      // 0x0078E640
extern int g_bRenderEMLM;           // 0x0078E644
extern int g_bOnePassEnvMap;        // 0x0078E638
extern int g_bEnvMapTexturesOnly;   // 0x0078E63C
extern int g_bEnvMapBlendAdd;       // 0x00827FD4
extern unsigned int g_nActiveTextureUnit;   // 0x007A6898
typedef void (__stdcall* PFNCLIENTACTIVETEXTURE)(unsigned int);
extern PFNCLIENTACTIVETEXTURE g_pfnClientActiveTexture;    // 0x007BB80C
typedef void (__cdecl* PFNCOMBINERCONST)(Vector*, float);
extern PFNCOMBINERCONST g_pfnCombinerConst; // 0x007A6820

unsigned char AurTextureEnvMapDisabled();   // 0x004204D0
namespace GLRender {
void BindTexture(int nTarget, int nTexture);    // 0x00420440
}

class VertexProgram {
public:
    VertexProgram(const char* szProgram, int nContext);    // 0x004A27D0
    void Bind(void* pMesh, int bLightmap);                  // 0x004A24D0

    char m_data[0x201c];
};

void SetCombinerConstant(int nIndex, float r, float g, float b, float a);   // 0x0044F820

void RenderFlat(PartTriMesh* pPart);    // 0x0046F900

#endif
