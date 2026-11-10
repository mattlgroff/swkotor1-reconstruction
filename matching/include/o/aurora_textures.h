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

class CAuroraInterface {
public:
    CAuroraInterface();
    ~CAuroraInterface();
    static unsigned char* GetResource(const char* szName, const char* szType, int* pSize);
    static int LoadTexturePack(unsigned char nIndex, const CExoString& sName);
    static int UnloadTexturePack(unsigned char nIndex);
    static CExoString GetCurrentTexturePack(unsigned char nIndex);
    static int SetTexturePack(unsigned char nIndex, const CExoString& sName);
    static void RequestModel(char* szName);
    static unsigned char* GetLayeredRaster(char* szName, unsigned long* pWidth, unsigned long* pHeight,
                                           unsigned long* pLayers);
    static unsigned char* GetRaster(char* szName, unsigned long* pWidth, unsigned long* pHeight,
                                    unsigned long* pBytesPerPixel);
    static void ReleaseLayeredRaster(void* pData, int nWidth, int nHeight);
    static void ReleaseResource(void* pData, int nOffset);
    static int ReadRasterHeader(char* szName, unsigned long* pWidth, unsigned long* pHeight,
                                unsigned long* pBytesPerPixel);
    static int ReadProcessedRasterHeader(char* szName, int* pWidth, int* pHeight, char* pFormat,
                                         int* pCompressedSize, int* pDataSize, float* pAlphaTest,
                                         char* pMipMaps);
    static int ReadCompressedRasterHeader(char* szName, unsigned long* pWidth, unsigned long* pHeight,
                                          unsigned long* pBytesPerPixel, unsigned long* pDataSize,
                                          float* pAlphaMean);
    static int Get16BitRasterHeader(char* szName, unsigned long* pWidth, unsigned long* pHeight,
                                    float* pAlphaMean, int* pDataSize);
    static void ReleaseRaster(void* pData, int nWidth, int nHeight, int nBytesPerPixel, int nCompressedSize,
                              int nDataSize, int nTXISize, unsigned char bProcessed);
    static unsigned char* GetCompressedRaster(char* szName, unsigned long* pWidth, unsigned long* pHeight,
                                              unsigned long* pBytesPerPixel, unsigned long* pDataSize,
                                              float* pAlphaMean);
    static unsigned char* GetProcessedRaster(char* szName, int* pWidth, int* pHeight, char* pFormat,
                                             int* pCompressedSize, int* pDataSize, int* pTXISize,
                                             float* pAlphaTest, char* pMipMaps);
    static unsigned char* Get16BitRaster(char* szName, unsigned long* pWidth, unsigned long* pHeight,
                                         float* pAlphaMean, int* pDataSize);
    static CAuroraModel* GetModelResHelper(const CExoString& sName, int bAutoRequest, int* pbCached,
                                           CExoLinkedListNode** ppNode);      // 0x0070DE20
    static CAuroraModel* GetModelInternal(const CExoString& sName, int bAutoRequest, int* pbCached,
                                          CExoLinkedListNode** ppNode);
    static void ReleaseModelInternal(CAuroraModel* pModel);
    static CAuroraTexture* GetTextureInternal(const CExoString& sName);
    static CAurora16BitTexture* Get16BitTextureInternal(const CExoString& sName);
    static CAuroraLayeredTexture* GetLayeredTextureInternal(const CExoString& sName);
    static CAuroraCompressedTexture* GetCompressedTextureInternal(const CExoString& sName);
    static CAuroraProcessedTexture* GetProcessedTextureInternal(const CExoString& sName);
    static CAuroraTXI* GetTXIInternal(const CExoString& sName);
    static CAuroraVisibility* GetVisibilityInternal(const CExoString& sName);
    static CAuroraModelX* GetModelXInternal(const CExoString& sName);
    static CAuroraTexture* GetTexture(const CExoString& sName);
    static void ReleaseTextureInternal(CAuroraTexture* pTexture);
    static CAurora16BitTexture* Get16BitTexture(const CExoString& sName);
    static void Release16BitTextureInternal(CAurora16BitTexture* pTexture);
    static CAuroraLayeredTexture* GetLayeredTexture(const CExoString& sName);
    static void ReleaseLayeredTextureInternal(CAuroraLayeredTexture* pTexture);
    static CAuroraCompressedTexture* GetCompressedTexture(const CExoString& sName);
    static void ReleaseCompressedTextureInternal(CAuroraCompressedTexture* pTexture);
    static CAuroraProcessedTexture* GetProcessedTexture(const CExoString& sName);
    static void ReleaseProcessedTextureInternal(CAuroraProcessedTexture* pTexture);
    static CAuroraTXI* GetTXI(const CExoString& sName);
    static void ReleaseTXIInternal(CAuroraTXI* pTXI);
    static CAuroraVisibility* GetVisibility(const CExoString& sName);
    static void ReleaseVisibilityInternal(CAuroraVisibility* pVisibility);
    static CAuroraModelX* GetModelX(const CExoString& sName);
    static void ReleaseModelXInternal(CAuroraModelX* pModelX);
};

#endif
