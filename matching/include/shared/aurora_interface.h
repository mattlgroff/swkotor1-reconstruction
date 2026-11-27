// aurora_interface.h -- CAuroraInterface
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/o/aurora_textures.h); the other copies' members fall on its members.
#ifndef SHARED_AURORA_INTERFACE_H
#define SHARED_AURORA_INTERFACE_H


class CAurora16BitTexture;
class CAuroraCompressedTexture;
class CAuroraLayeredTexture;
class CAuroraModel;
class CAuroraModelX;
class CAuroraProcessedTexture;
class CAuroraTXI;
class CAuroraTexture;
class CAuroraVisibility;
struct CExoLinkedListNode;
class CExoString;

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
