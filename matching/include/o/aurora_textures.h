// Worker O matching headers: the CAurora* resource wrappers
// (0x0070DA60-0x00710B30). Each one is a CResHelper over its CRes class and
// tags the loaded payload with a CResHelperInfo back pointer.
#ifndef O_AURORA_TEXTURES_H
#define O_AURORA_TEXTURES_H

#include "o/aurora_res_helper.h"

// vtable 0x0075FB50
class CAuroraLayeredTexture : public CResHelper<CResPLT, 6> {
public:
    CAuroraLayeredTexture(CResRef cResRef, int bAutoRequest);
    virtual ~CAuroraLayeredTexture();
    int GetTextureAttrib(unsigned long* pWidth, unsigned long* pHeight, unsigned long* pLayers);
    int Load();
    void Unload();                                  // 0x00710100 (shared)
    CResHelperInfo* GetPtrToResHelperInfo();
    unsigned char* GetTextureData();                // 0x0070F380 (shared)
};

// vtable 0x0075FB58
class CAuroraTexture : public CResHelper<CResTGA, 3> {
public:
    CAuroraTexture(CResRef cResRef, int bAutoRequest);
    virtual ~CAuroraTexture();
    int GetTextureAttrib(unsigned long* pWidth, unsigned long* pHeight, unsigned char* pBitsPerPixel);
    int Load();
    void Unload();
    int ReadTextureHeader(unsigned long* pWidth, unsigned long* pHeight, unsigned long* pBytesPerPixel);
    CResHelperInfo* GetPtrToResHelperInfo();
    unsigned char* GetTextureData();                // 0x0070F3E0 (shared)
    int GetTextureDataSize();                       // 0x0070F410 (shared)
};

// vtable 0x0075FB60
class CAuroraVisibility : public CResHelper<CResVIS, 3001> {
public:
    CAuroraVisibility(CResRef cResRef, int bAutoRequest);
    virtual ~CAuroraVisibility();
    int Load();
    void Unload();                                  // 0x00710100 (shared)
    CResHelperInfo* GetPtrToResHelperInfo();        // 0x0070FAF0 (shared)
    int GetVisibilityDataSize();                    // 0x007100A0 (shared)
    unsigned char* GetVisibilityData();             // 0x007107B0 (shared)
};

// vtable 0x0075FB68
class CAuroraProcessedTexture : public CResHelper<CResTPC, 3007> {
public:
    CAuroraProcessedTexture(CResRef cResRef, int bAutoRequest);
    virtual ~CAuroraProcessedTexture();
    int GetProcessedTextureAttrib(int* pWidth, int* pHeight, char* pFormat, int* pCompressedSize,
                                  int* pDataSize, int* pTXISize, float* pAlphaTest, char* pMipMaps);
    unsigned char* GetProcessedTextureDataPtr();
    int GetProcessedTextureDataSize();
    unsigned char* GetProcessedTextureTXIPtr();
    int GetProcessedTextureTXISize();
    int Load();
    void Unload();                                  // 0x00710100 (shared)
    int ReadProcessedTextureHeader(int* pWidth, int* pHeight, char* pFormat, int* pCompressedSize,
                                   int* pDataSize, float* pAlphaTest, char* pMipMaps);
    int GetTextureSize();
    CResHelperInfo* GetPtrToResHelperInfo();
};

// vtable 0x0075FB70
class CAuroraTXI : public CResHelper<CResTXI, 2022> {
public:
    CAuroraTXI(CResRef cResRef, int bAutoRequest);
    virtual ~CAuroraTXI();
    int Load();
    void Unload();                                  // 0x00710100 (shared)
    CResHelperInfo* GetPtrToResHelperInfo();
    char* GetTXIData();                             // 0x007107B0 (shared)
    int GetTXIDataSize();                           // 0x007100A0 (shared)
};

// vtable 0x0075FB78
class CAuroraModelX : public CResHelper<CResMDX, 3008> {
public:
    CAuroraModelX(CResRef cResRef, int bAutoRequest);
    virtual ~CAuroraModelX();
    int Load();
    void Unload();                                  // 0x00710100 (shared)
    CResHelperInfo* GetPtrToResHelperInfo();        // 0x0070FAF0 (shared)
    int GetModelXDataSize();                        // 0x007100A0 (shared)
    unsigned char* GetModelXData();                 // 0x007107B0 (shared)
};

// vtable 0x0075FB80
class CAuroraModel : public CResHelper<CResMDL, 2002> {
public:
    CAuroraModel(CResRef cResRef, int bAutoRequest);
    virtual ~CAuroraModel();
    int GetModelDataSize();
    int Load();
    void Unload();
    CResHelperInfo* GetPtrToResHelperInfo();        // 0x0070FAF0 (shared)
    unsigned char* GetModelData();                  // 0x007107B0 (shared)
};

// vtable 0x0075FB88
class CAuroraCompressedTexture : public CResHelper<CResDDS, 2033> {
public:
    CAuroraCompressedTexture(CResRef cResRef, int bAutoRequest);
    virtual ~CAuroraCompressedTexture();
    int GetCompressedTextureAttrib(unsigned long* pWidth, unsigned long* pHeight, unsigned char* pBytesPerPixel,
                                   unsigned long* pDataSize, float* pAlphaMean);
    int Load();
    void Unload();                                  // 0x00710100 (shared)
    int ReadTextureHeader(unsigned long* pWidth, unsigned long* pHeight, unsigned long* pBytesPerPixel,
                          unsigned long* pDataSize, float* pAlphaMean);
    CResHelperInfo* GetPtrToResHelperInfo();
    unsigned char* GetTextureData();                // 0x007107B0 (shared)
};

// vtable 0x0075FB90
class CAurora16BitTexture : public CResHelper<CRes4PC, 2059> {
public:
    CAurora16BitTexture(CResRef cResRef, int bAutoRequest);
    virtual ~CAurora16BitTexture();
    int GetTextureAttrib(unsigned long* pWidth, unsigned long* pHeight, float* pAlphaMean, int* pDataSize);
    unsigned char* Get16BitTextureDataPtr();
    int Load();
    void Unload();                                  // 0x00710100 (shared)
    int ReadTextureHeader(unsigned long* pWidth, unsigned long* pHeight, float* pAlphaMean, int* pDataSize);
    CResHelperInfo* GetPtrToResHelperInfo();
};

class CExoLinkedListNode;

class RenderInterface {
public:
    static int IsResident(char* szName);
};

#include "shared/aurora_interface.h"

#endif
