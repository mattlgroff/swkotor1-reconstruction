// Material bind / blend helpers (0x0047ABC0-0x0047B0E0). The rest
// of Material (ParseField, ctor, dtor, texture setup) is worker E's
// (matching/src/e/material.cpp); this is an own copy of the layout.
#ifndef MATCHING_H_MATERIAL_H
#define MATCHING_H_MATERIAL_H

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList() { num = 0; array_size = 0; element = 0; }
    ~CExoArrayList() { if (element) delete[] element; }
    T& operator[](int i) { return element[i]; }
};

class Vector {
public:
    float x, y, z;
};

class CAurTexture {
public:
    virtual void v00(); virtual void v04();
    virtual CAurTexture* GetTexture();          // +0x08
    virtual unsigned char HasCubeMap();         // +0x0c
    virtual unsigned char IsInvalid();          // +0x10
    virtual void v14();
    virtual unsigned char IsBumpTexture();      // +0x18
    virtual void v1c(); virtual void v20();
    virtual unsigned char HasAlpha();           // +0x24
    virtual void v28(); virtual void v2c(); virtual void v30(); virtual void v34();
    virtual void v38(); virtual void v3c(); virtual void v40(); virtual void v44();
    virtual void v48(); virtual void v4c(); virtual void v50(); virtual void v54();
    virtual void v58(); virtual void v5c(); virtual void v60(); virtual void v64();
    virtual void v68(); virtual void v6c(); virtual void v70(); virtual void v74();
    virtual void v78(); virtual void v7c(); virtual void v80(); virtual void v84();
    virtual void v88(); virtual void v8c(); virtual void v90(); virtual void v94();
    virtual void v98(); virtual void v9c(); virtual void va0(); virtual void va4();
    virtual void va8(); virtual void vac(); virtual void vb0();
    virtual void Bind();                        // +0xb4

    unsigned char IsProcedural();               // 0x00420600
    int GetTID();
};

void AurTextureRelease(CAurTexture* p);                 // 0x00420D70
CAurTexture* AurTextureGetNULL();                       // 0x004230C0
int AurTextureGetTID(CAurTexture* p);                   // 0x00420BF0
inline int CAurTexture::GetTID() { return AurTextureGetTID(this); }

class CAurTextureRef {
public:
    CAurTexture* m_p;
};
void AurTextureSetField(CAurTexture* p, char* szField); // 0x00422D20
float* AurMakeColor(Vector* pColor, float* pAlpha);     // 0x004AAAB0

extern "C" int __cdecl sprintf(char*, const char*, ...);

extern "C" __declspec(dllimport) void __stdcall glBlendFunc(unsigned int, unsigned int);
extern "C" __declspec(dllimport) void __stdcall glDepthMask(unsigned char);
extern "C" __declspec(dllimport) void __stdcall glAlphaFunc(unsigned int, float);
extern "C" __declspec(dllimport) void __stdcall glMaterialfv(unsigned int, unsigned int,
                                                            const float*);

extern int g_bUseLightMapTexture;       // 0x0078E5FC
extern unsigned int g_aBlendFactors[];  // 0x0073F270
extern int g_bAlphaTest;                // 0x00798A8C
extern float g_fAlphaRef;               // 0x00798A90
extern unsigned int g_eAlphaFunc;       // 0x0073F2B0
extern float g_fMaterialAlpha;          // 0x0078E390
extern unsigned int g_eMaterialFace;    // 0x0073F2EC
extern unsigned int g_eMaterialDiffuse; // 0x0073F300

// Validates a texture reference; the results are dropped by the callers.
static inline CAurTexture* CheckTexture(CAurTexture* p)
{
    if (!p)
        return AurTextureGetNULL();
    if (p->IsInvalid()) {
        CAurTexture* pNull = AurTextureGetNULL();
        AurTextureRelease(p);
        return pNull;
    }
    return p;
}

class Material {
public:
    CAurTextureRef m_tTexture;      // +0x00
    CAurTextureRef m_tTexture1;     // +0x04
    CAurTextureRef m_tTexture2;     // +0x08
    CAurTextureRef m_tTexture3;     // +0x0c
    CAurTextureRef m_tBumpMap;      // +0x10
    CAurTextureRef m_tEnvMap;       // +0x14
    CAurTextureRef m_tLightMap;     // +0x18
    char* m_szBumpMap;              // +0x1c
    char* m_szBumpyShiny;           // +0x20
    char* m_szEnvMap;               // +0x24
    CExoArrayList<int> m_lSrcBlend; // +0x28
    CExoArrayList<int> m_lDstBlend; // +0x34
    unsigned char m_bDecal;         // +0x40
    int m_nRenderType;              // +0x44
    float m_fWaterAlpha;            // +0x48

    int BindTexture0();
    int BindTexture1();
    int BindEnvMap();
    int GetTextureTID();
    void SetDownSampling(int nMax);
    int IsBumpMapBumpTexture();
    void SetBlendingMode(int i);
    void SetGUIBlendingMode();
    void SetMaterialColor(Vector vColor, float fAlpha);
    bool HasCubeEnvMap();
    bool HasEnvMap();
};

#endif
