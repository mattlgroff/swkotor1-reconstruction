// CAurTextureBasic image construction and Aur texture globals
// (0x0041e9b0-0x0041feb5). Layout from matching/src/pilot3/aurtexture.cpp and
// docs/batches/b0041_aur_texture.md; 0x0041f6a0 LoadImage is in pilot3.

extern "C" void __cdecl free(void* p);
extern "C" int __cdecl tolower(int c);
extern "C" unsigned int __cdecl strlen(const char* s);
extern "C" char* __cdecl strcpy(char* d, const char* s);
extern "C" void* __cdecl memset(void* p, int c, unsigned int n);
extern "C" void* __cdecl memcpy(void* d, const void* s, unsigned int n);

typedef void* (*PFN_LOADIMAGEPLT)(char* szName, int* pWidth, int* pHeight, int* pUnused);
typedef void (*PFN_IMAGEHOOK)(void* pData, int nWidth, int nHeight, int nBpp, int nUnused,
                              int nMipMaps, int nFormat, unsigned char bAlpha);
typedef void (*PFN_RELEASEHOOK)(void* pData, ...);
typedef void* (*PFN_LOADIMAGES3TC)(char* szName, int* pWidth, int* pHeight, char* pBpp, int* pCompressed,
                                    int* pMipMaps, int* pDataSize, float* pTXI, char* pMipMapCount);
typedef void* (*PFN_LOADIMAGEDDS)(char* szName, int* pWidth, int* pHeight, int* pBpp, int* pCompressed,
                                  float* pTXI);
typedef void* (*PFN_LOADIMAGETGA)(char* szName, int* pWidth, int* pHeight, float* pTXI, int* pFormat);
typedef void* (*PFN_LOADIMAGE)(char* szName, int* pWidth, int* pHeight, int* pBpp);
typedef void (__stdcall *PFN_GLACTIVETEXTURE)(unsigned int nTexture);

extern PFN_LOADIMAGES3TC g_pfnLoadImageS3TC;    // 0x0078d420
extern PFN_LOADIMAGEDDS g_pfnLoadImageDDS;      // 0x0078d40c
extern PFN_LOADIMAGETGA g_pfnLoadImageTGA;      // 0x0078d414
extern PFN_LOADIMAGE g_pfnLoadImageRaw;         // 0x0078d404
extern int g_bAurTextureDisableDDS;             // 0x007a4700
extern float g_fAurTextureDynamicMemoryRatio;   // 0x0078d3f0
extern PFN_LOADIMAGEPLT g_pfnLoadImagePLT;      // 0x0078d41c
extern PFN_IMAGEHOOK g_pfnImageHook;            // 0x007a46f8
extern PFN_RELEASEHOOK g_pfnReleaseHook;        // 0x007a46fc
extern unsigned char* g_pLayerTable;            // 0x007a4704
extern unsigned char* g_pLayerAlpha;            // 0x007a4708
extern unsigned char g_bBumpMaps;               // 0x007a46d4
extern unsigned char g_aBumpTable[];            // 0x007a4764
extern int g_nAurTextureMaxTexID;               // 0x007a46bc
extern int g_nActiveTextureStage;               // 0x007a6898
extern PFN_GLACTIVETEXTURE g_pfnglActiveTextureARB; // 0x007bb80c

void ImageGamma(int nWidth, int nHeight, int nBpp, void* pData, float fGamma);  // 0x0045dd90
void* ImageMakeLayered(void* pData, int nWidth, int nHeight, unsigned char* pTable,
                       unsigned char* pAlpha, int* pScheme, int nScheme);       // 0x0045de10
void ImageBumpMap(void* pData, int nWidth, int nHeight, int nBpp, unsigned char* pTable); // 0x0045f0d0
int AurHasMultiTexture();                       // 0x0045f710
int AurHasDDS();                                // 0x0045f950

struct Vector {
    float x, y, z;
};

class CAurTextureBasic;

// vtable 0x0073f0a8 (CAurTexture) / 0x0073f160 (CAurTextureBasic). Most
// slots forward to the CAurTextureBasic behind GetTarget().
class CAurTexture {
public:
    virtual ~CAurTexture();                         // 0x00
    virtual CAurTextureBasic* GetBasic();           // 0x04 0x0063e7f0 (basic: 0x00641db0)
    virtual CAurTexture* GetTarget();               // 0x08 0x004205c0
    virtual bool IsCubeMap();                       // 0x0c
    virtual bool IsMissing();                       // 0x10 0x004205d0
    virtual bool IsEnvMapAlpha();                   // 0x14
    virtual bool IsBumpMapAlpha();                  // 0x18
    virtual bool HasAlpha();                        // 0x1c
    virtual bool IsAlphaBlended();                  // 0x20
    virtual bool GetFlagDE();                       // 0x24
    virtual bool IsConstructed();                   // 0x28
    virtual bool IsBumpMap();                       // 0x2c
    virtual bool GetAlpha();                        // 0x30
    virtual bool GetFlagE6();                       // 0x34
    virtual int GetField38();                       // 0x38
    virtual void* GetData();                        // 0x3c
    virtual char* GetResName();                     // 0x40
    virtual int GetNumFrames();                     // 0x44
    virtual int GetWidth();                         // 0x48
    virtual int GetHeight();                        // 0x4c
    virtual int GetBytesPerPixel();                 // 0x50
    virtual int GetNumMipMaps();                    // 0x54
    virtual int GetNumX();                          // 0x58
    virtual int GetNumY();                          // 0x5c
    virtual int GetFieldC0();                       // 0x60
    virtual int GetDataSize();                      // 0x64
    virtual float GetField28();                     // 0x68
    virtual bool GetFlagDC();                       // 0x6c
    virtual bool GetFlagDD();                       // 0x70
    virtual bool GetFlagDF();                       // 0x74
    virtual float GetField18();                     // 0x78
    virtual float GetField1C();                     // 0x7c
    virtual float GetBumpMapScaling();              // 0x80
    virtual float GetEnvMapScaling();               // 0x84
    virtual Vector GetVector4C();                   // 0x88
    virtual int GetFormat();                        // 0x8c
    virtual int GetTXIDataSize();                   // 0x90
    virtual void* GetTXIData();                     // 0x94
    virtual int CalculateFormat();                  // 0x98
    virtual void SetFormat(int nFormat);            // 0x9c
    virtual void SetTXIData(void* pData, int nSize);    // 0xa0
    virtual void SetFlagE6(bool bValue);   // 0xa4
    virtual void SetFrame(int nFrame);              // 0xa8
    virtual void vac();                             // 0xac 0x00421430
    virtual void FreeImageData();                   // 0xb0
    virtual void vb4();                             // 0xb4 0x004214b0

    char pad_0x4[0xc];
    CAurTexture* m_pTarget;     // +0x10
    char pad_0x14[4];
    float m_f18;                // +0x18
    float m_f1c;                // +0x1c
    float m_fOrder;             // +0x20 ordering priority / TXI value
};

class CAurTextureBasic : public CAurTexture {
public:
    void* LoadImage();                          // 0x0041f6a0
    void* LoadImagePLT();
    void ConstructImage();
    void SetScheme(int* pScheme, int nScheme);
    virtual void FreeImageData();               // 0xb0

    float m_fGamma;             // +0x24
    float m_f28;                // +0x28
    float m_fEnvMapScaling;     // +0x2c
    float m_fBumpMapScaling;    // +0x30
    int m_bProcedural;          // +0x34
    int m_n38;                  // +0x38
    int* m_pScheme;             // +0x3c
    int* m_pFrameTIDs;          // +0x40
    void* m_pData;              // +0x44
    void* m_pTXIData;           // +0x48
    Vector m_v4c;               // +0x4c
    int m_nTID;                 // +0x58
    int m_nDataSize;            // +0x5c
    int m_nMipMaps;             // +0x60
    int m_nTXIDataSize;         // +0x64
    int m_nWidth;               // +0x68
    int m_nHeight;              // +0x6c
    int m_nBpp;                 // +0x70
    int m_nType;                // +0x74
    char m_szResName[0x20];     // +0x78
    char m_szName[0x28];        // +0x98
    short m_nC0;                // +0xc0
    short m_nC2;                // +0xc2
    short m_nC4;                // +0xc4
    short m_nFrames;            // +0xc6
    short m_nProcWidth;         // +0xc8
    short m_nProcHeight;        // +0xca
    short m_nScheme;            // +0xcc
    short m_nMipMapCount;       // +0xce
    char pad_0xd0[4];
    int m_nBlending;            // +0xd4
    bool m_bEnvMapAlpha;   // +0xd8
    bool m_bBumpMapAlpha;  // +0xd9
    bool m_bBumpMap;   // +0xda
    unsigned char m_bCubeMap;   // +0xdb
    bool m_bDC;        // +0xdc
    bool m_bDD;        // +0xdd
    bool m_bDE;        // +0xde
    bool m_bDF;        // +0xdf
    bool m_bConstructed;   // +0xe0
    unsigned char m_bMissing;   // +0xe1
    char pad_0xe2;
    unsigned char m_bHooked;    // +0xe3
    char pad_0xe4;
    bool m_bAlpha;     // +0xe5
    bool m_bE6;        // +0xe6
};

// FUNCTION: SWKOTOR 0x0041e9b0
void AurTextureSetDynamicMemoryRatio(float fRatio)
{
    g_fAurTextureDynamicMemoryRatio = fRatio;
}

// FUNCTION: SWKOTOR 0x0041e9c0
void AurTextureSortOrdered(CAurTexture** ppList, int nCount)
{
    for (int i = 1; i < nCount; i++) {
        CAurTexture* pTexture = ppList[i];
        int j = i - 1;
        while (pTexture->m_fOrder > ppList[j]->m_fOrder) {
            CAurTexture* pSwap = ppList[j + 1];
            ppList[j + 1] = ppList[j];
            ppList[j] = pSwap;
            if (--j < 0) {
                break;
            }
        }
    }
}

#define AUR_BASIC() GetTarget()->GetBasic()

// FUNCTION: SWKOTOR 0x0041ea20
void* AurGetImageWrapper(char* szName, int* pWidth, int* pHeight, int* pBpp, int* pCompressed,
                         int* pMipMaps, int* pDataSize, int* pFormat, float* pTXI,
                         unsigned char* pAlpha, short* pMipMapCount, int bAllowDDS)
{
    if (g_bAurTextureDisableDDS) {
        bAllowDDS = 0;
    }
    char nBpp;
    char nMipMapCount;
    *pCompressed = 0;
    *pMipMaps = 0;
    *pDataSize = 0;
    void* pData = g_pfnLoadImageS3TC(szName, pWidth, pHeight, &nBpp, pCompressed, pMipMaps, pDataSize,
                                     pTXI, &nMipMapCount);
    if (pData) {
        *pBpp = nBpp;
        *pMipMapCount = nMipMapCount;
        *pAlpha = 1;
        if (*pCompressed) {
            switch (*pBpp) {
            case 3:
                *pFormat = 7;
                break;
            case 4:
                *pFormat = 8;
                break;
            }
        }
        else {
            switch (*pBpp) {
            case 1:
                *pFormat = 3;
                break;
            case 3:
                *pFormat = 1;
                break;
            case 4:
                *pFormat = 0;
                break;
            }
        }
        return pData;
    }
    if (bAllowDDS && AurHasDDS()) {
        pData = g_pfnLoadImageDDS(szName, pWidth, pHeight, pBpp, pCompressed, pTXI);
        if (pData) {
            switch (*pBpp) {
            case 4:
                *pFormat = 8;
                break;
            case 3:
                *pFormat = 7;
                break;
            }
            return pData;
        }
    }
    pData = g_pfnLoadImageTGA(szName, pWidth, pHeight, pTXI, pFormat);
    *pBpp = 2;
    if (pData) {
        return pData;
    }
    pData = g_pfnLoadImageRaw(szName, pWidth, pHeight, pBpp);
    if (pData) {
        switch (*pBpp) {
        case 1:
            *pFormat = 3;
            break;
        case 3:
            *pFormat = 1;
            break;
        case 4:
            *pFormat = 0;
            break;
        }
    }
    return pData;
}

// FUNCTION: SWKOTOR 0x0041ebd0
void AurTextureFreeLayers()
{
    free(g_pLayerTable);
    g_pLayerTable = 0;
    g_pLayerAlpha = 0;
}

// FUNCTION: SWKOTOR 0x0041ebf0
bool CAurTexture::IsCubeMap()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic) {
        return pBasic->m_bCubeMap;
    }
    return false;
}

// FUNCTION: SWKOTOR 0x0041ec10
bool CAurTexture::IsConstructed()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic) {
        return pBasic->m_bConstructed;
    }
    return false;
}

// FUNCTION: SWKOTOR 0x0041ec30
bool CAurTexture::GetAlpha()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic) {
        return pBasic->m_bAlpha;
    }
    return false;
}

// FUNCTION: SWKOTOR 0x0041ec50
bool CAurTexture::GetFlagE6()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic) {
        return pBasic->m_bE6;
    }
    return false;
}

// FUNCTION: SWKOTOR 0x0041ec70
void CAurTexture::SetFlagE6(bool bValue)
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic) {
        pBasic->m_bE6 = bValue;
    }
}

// FUNCTION: SWKOTOR 0x0041ec90
void* CAurTexture::GetTXIData()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic) {
        return pBasic->m_pTXIData;
    }
    return 0;
}


// FUNCTION: SWKOTOR 0x0041ecb0
void CAurTexture::SetTXIData(void* pData, int nSize)
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic && pData && nSize > 0) {
        if (pBasic->m_pTXIData) {
            free(pBasic->m_pTXIData);
            pBasic->m_pTXIData = 0;
        }
        pBasic->m_pTXIData = new char[nSize];
        memcpy(pBasic->m_pTXIData, pData, nSize);
        pBasic->m_nTXIDataSize = nSize;
    }
}

// FUNCTION: SWKOTOR 0x0041ed20
int CAurTexture::GetTXIDataSize()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic) {
        return pBasic->m_nTXIDataSize;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0041ed40
bool CAurTexture::IsBumpMap()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic) {
        return pBasic->m_bBumpMap;
    }
    return false;
}

// FUNCTION: SWKOTOR 0x0041ed60
Vector CAurTexture::GetVector4C()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    Vector vResult;
    vResult.x = 0.0f;
    vResult.y = 0.0f;
    vResult.z = 0.0f;
    if (pBasic) {
        return pBasic->m_v4c;
    }
    return vResult;
}

// FUNCTION: SWKOTOR 0x0041edd0
float CAurTexture::GetField1C()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic) {
        return pBasic->m_f1c;
    }
    return 0.0f;
}

// FUNCTION: SWKOTOR 0x0041edf0
float CAurTexture::GetBumpMapScaling()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    float fScaling = 0.0f;
    if (pBasic) {
        if (pBasic->IsBumpMapAlpha()) {
            fScaling = pBasic->m_fBumpMapScaling;
        }
        else {
            fScaling = 0.0f;
        }
    }
    return fScaling;
}

// FUNCTION: SWKOTOR 0x0041ee30
float CAurTexture::GetEnvMapScaling()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    float fScaling = 0.0f;
    if (pBasic) {
        if (pBasic->IsEnvMapAlpha()) {
            fScaling = pBasic->m_fEnvMapScaling;
        }
        else {
            fScaling = 0.0f;
        }
    }
    return fScaling;
}

// FUNCTION: SWKOTOR 0x0041ee70
bool CAurTexture::IsEnvMapAlpha()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic) {
        return pBasic->HasAlpha() && pBasic->m_bEnvMapAlpha;
    }
    return false;
}

// FUNCTION: SWKOTOR 0x0041eeb0
bool CAurTexture::IsBumpMapAlpha()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic) {
        return pBasic->HasAlpha() && pBasic->m_bBumpMapAlpha;
    }
    return false;
}

// FUNCTION: SWKOTOR 0x0041eef0
bool CAurTexture::HasAlpha()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic) {
        return (pBasic->m_nBlending && pBasic->GetAlpha())
            || (pBasic->m_nBlending == 1 && pBasic->m_nBpp == 1)
            || (pBasic->m_nBlending == 2 && pBasic->m_nBpp == 4);
    }
    return false;
}

// FUNCTION: SWKOTOR 0x0041ef50
bool CAurTexture::IsAlphaBlended()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic) {
        return pBasic->m_nBlending == 2 && pBasic->m_nBpp == 4;
    }
    return false;
}

// FUNCTION: SWKOTOR 0x0041ef80
bool CAurTexture::GetFlagDE()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic) {
        return pBasic->m_bDE;
    }
    return false;
}

// FUNCTION: SWKOTOR 0x0041efa0
int CAurTexture::GetField38()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic) {
        return pBasic->m_n38;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0041efc0
void* CAurTexture::GetData()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic) {
        return pBasic->m_pData;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0041efe0
char* CAurTexture::GetResName()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic) {
        return pBasic->m_szResName;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0041f000
int CAurTexture::GetNumFrames()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic) {
        return pBasic->m_nFrames;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0041f020
int CAurTexture::GetWidth()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic) {
        return pBasic->m_nWidth;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0041f040
int CAurTexture::GetHeight()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic) {
        return pBasic->m_nHeight;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0041f060
int CAurTexture::GetBytesPerPixel()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic) {
        return pBasic->m_nBpp;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0041f080
int CAurTexture::GetNumMipMaps()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic) {
        return pBasic->m_nMipMapCount;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0041f0a0
int CAurTexture::GetNumX()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic) {
        return pBasic->m_nC2;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0041f0c0
int CAurTexture::GetNumY()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic) {
        return pBasic->m_nC4;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0041f0e0
bool CAurTexture::GetFlagDC()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic) {
        return pBasic->m_bDC;
    }
    return false;
}

// FUNCTION: SWKOTOR 0x0041f100
bool CAurTexture::GetFlagDD()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic) {
        return pBasic->m_bDD;
    }
    return false;
}

// FUNCTION: SWKOTOR 0x0041f120
bool CAurTexture::GetFlagDF()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic) {
        return pBasic->m_bDF;
    }
    return false;
}

// FUNCTION: SWKOTOR 0x0041f140
int CAurTexture::GetFieldC0()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic) {
        return pBasic->m_nC0;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0041f160
float CAurTexture::GetField18()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic) {
        return pBasic->m_f18;
    }
    return 0.0f;
}

// FUNCTION: SWKOTOR 0x0041f180
float CAurTexture::GetField28()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic) {
        return pBasic->m_f28;
    }
    return 1.0f;
}

// FUNCTION: SWKOTOR 0x0041f1a0
int CAurTexture::GetDataSize()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic) {
        return pBasic->m_nDataSize;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0041f1c0
int CAurTexture::GetFormat()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic && pBasic->m_nDataSize) {
        return pBasic->m_nType;
    }
    return 7;
}

// FUNCTION: SWKOTOR 0x0041f1f0
int CAurTexture::CalculateFormat()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic) {
        if (!pBasic->m_nDataSize && pBasic->m_nType == 7) {
            pBasic->m_nType = pBasic->m_nBpp == 4 ? 0 : pBasic->m_nBpp == 3 ? 1 : pBasic->m_nBpp == 2 ? 2 : 3;
        }
        return pBasic->m_nType;
    }
    return 7;
}

// FUNCTION: SWKOTOR 0x0041f260
void CAurTexture::SetFormat(int nFormat)
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic && !pBasic->m_nDataSize) {
        pBasic->m_nType = nFormat;
    }
}

// FUNCTION: SWKOTOR 0x0041f310
void CAurTexture::FreeImageData()
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic) {
        pBasic->FreeImageData();
    }
}

// FUNCTION: SWKOTOR 0x0041f3b0
void CAurTexture::SetFrame(int nFrame)
{
    CAurTextureBasic* pBasic = AUR_BASIC();
    if (pBasic) {
        pBasic->m_nTID = pBasic->m_pFrameTIDs[nFrame];
    }
}

// FUNCTION: SWKOTOR 0x0041f4d0
unsigned short AurTextureCalculateHashID(char* szName, short nInts, int* pInts)
{
    int nHash = 0;
    int nLength = strlen(szName);
    int i;
    for (i = 0; i < nLength; i++) {
        int nValue;
        if (nHash == 0) {
            nValue = tolower(szName[i]);
        }
        else {
            nValue = tolower(szName[i]) * nHash;
        }
        nHash = nValue % 0xfe4d;
    }
    if (nInts > 0) {
        for (i = 0; i < nInts; i++) {
            int nValue = pInts[i];
            if (nHash) {
                nValue *= nHash;
            }
            nHash = nValue % 0xfe4d;
        }
    }
    return (unsigned short)nHash;
}

// FUNCTION: SWKOTOR 0x0041f570
void CAurTextureBasic::FreeImageData()
{
    if (!m_pData) {
        return;
    }
    int nFaces = 1;
    int nBlockSize;
    if (m_nBpp == 3) {
        nBlockSize = 8;
    }
    else if (m_nBpp == 4) {
        nBlockSize = 16;
    }
    int nWidth = m_nWidth;
    int nHeight = m_nHeight;
    if (nHeight / nWidth == 6) {
        nFaces = 6;
        nHeight /= 6;
    }
    int nSize = m_nDataSize;
    if (nSize) {
        for (int i = 0; i < m_nMipMapCount - 1; i++) {
            nHeight >>= 1;
            nWidth >>= 1;
            nSize += ((nHeight + 3) / 4) * ((nWidth + 3) / 4) * nBlockSize;
        }
        if (nFaces == 6) {
            nSize *= 6;
        }
    }
    m_bE6 = 0;
    if (m_bHooked) {
        g_pfnImageHook(m_pData, m_nWidth, m_nHeight, m_nBpp, nSize, m_nMipMaps, m_nTXIDataSize, m_bAlpha);
        m_bHooked = 0;
        m_pData = 0;
    }
    else {
        free(m_pData);
        m_pData = 0;
    }
}

// FUNCTION: SWKOTOR 0x0041f9b0
void* CAurTextureBasic::LoadImagePLT()
{
    FreeImageData();
    if (m_szName[0] == 0) {
        strcpy(m_szName, m_szResName);
    }
    int nUnused;
    void* pData = g_pfnLoadImagePLT(m_szName, &m_nWidth, &m_nHeight, &nUnused);
    m_nBpp = 2;
    m_pData = pData;
    m_nType = 0;
    if (g_pfnImageHook && pData) {
        m_bHooked = 1;
        return pData;
    }
    m_bHooked = 0;
    return pData;
}

// FUNCTION: SWKOTOR 0x0041fa30
void CAurTextureBasic::ConstructImage()
{
    ImageGamma(m_nWidth, m_nHeight, m_nBpp, m_pData, m_fGamma);
    if (m_pScheme) {
        LoadImagePLT();
        void* pImage = m_pData;
        m_nDataSize = 0;
        if (pImage) {
            m_pData = ImageMakeLayered(pImage, m_nWidth, m_nHeight, g_pLayerTable, g_pLayerAlpha,
                                       m_pScheme, m_nScheme);
            if (m_bHooked) {
                g_pfnReleaseHook(pImage, m_nWidth, m_nHeight);
                m_bHooked = 0;
            }
            else {
                free(pImage);
            }
            m_nBpp = 4;
        }
        m_fOrder = 1.0f;
    }
    else {
        LoadImage();
    }
    if (!m_pData) {
        if (m_nBlending) {
            m_nBpp = 1;
        }
        m_nWidth = m_nWidth < 2 ? 2 : m_nWidth;
        m_nHeight = m_nHeight < 2 ? 2 : m_nHeight;
    }
    if (!m_pData) {
        if (m_bProcedural) {
            m_nWidth = m_nProcWidth;
            m_nHeight = m_nProcHeight;
            m_nBpp = 4;
            m_pData = new unsigned char[m_nHeight * m_nWidth * 4];
            memset(m_pData, 0x7f, m_nBpp * m_nHeight * m_nWidth);
        }
        else {
            m_bMissing = 1;
        }
    }
    if (m_bBumpMap && g_bBumpMaps) {
        ImageBumpMap(m_pData, m_nWidth, m_nHeight, m_nBpp, g_aBumpTable);
    }
    m_bConstructed = 1;
}

// FUNCTION: SWKOTOR 0x0041fbd0
unsigned char* ImageQuarterRotated(unsigned char* pSrc, int nWidth, int nHeight, int nBpp, int nQuarter)
{
    unsigned char* pDst = new unsigned char[nWidth * nHeight * nBpp];
    nQuarter %= 4;
    if (nQuarter < 0) {
        nQuarter += 4;
    }
    unsigned char* pRow = pSrc;
    for (int y = 0; y < nHeight; y++) {
        unsigned char* pPixel = pRow;
        for (int x = 0; x < nWidth; x++) {
            int aRow[4];
            int aCol[4];
            aRow[0] = y;
            aRow[1] = x;
            aRow[2] = nHeight - 1 - y;
            aRow[3] = nWidth - 1 - x;
            aCol[0] = x;
            aCol[1] = nHeight - 1 - y;
            aCol[2] = nWidth - 1 - x;
            aCol[3] = y;
            memcpy(pDst + (aRow[nQuarter] * nWidth + aCol[nQuarter]) * nBpp, pPixel, nBpp);
            pPixel += nBpp;
        }
        pRow += nWidth * nBpp;
    }
    return pDst;
}

// FUNCTION: SWKOTOR 0x0041fd00
void CAurTextureBasic::SetScheme(int* pScheme, int nScheme)
{
    if (!m_pScheme) {
        m_pScheme = new int[256];
    }
    for (int i = 0; i < 256; i++) {
        m_pScheme[i] = i < nScheme ? pScheme[i] : 0;
    }
    m_nScheme = (short)nScheme;
}

class GLRender {
public:
    static void ActivateTextureStage(int nStage);
};

// FUNCTION: SWKOTOR 0x0041fe80
void GLRender::ActivateTextureStage(int nStage)
{
    g_nActiveTextureStage = nStage;
    if (AurHasMultiTexture()) {
        g_pfnglActiveTextureARB(g_nActiveTextureStage + 0x84c0);
    }
}

// FUNCTION: SWKOTOR 0x0041feb0
int AurTextureGetMaxTexID()
{
    return g_nAurTextureMaxTexID;
}
