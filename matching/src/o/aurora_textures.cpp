// CAurora* resource wrappers and their CResHelper instantiations,
// 0x0070DA60-0x00710B30.
#include "o/aurora_textures.h"

// ------------------------------------------------- CAuroraLayeredTexture

// FUNCTION: SWKOTOR 0x0070DA60
int CAuroraLayeredTexture::GetTextureAttrib(unsigned long* pWidth, unsigned long* pHeight, unsigned long* pLayers)
{
    if (m_pRes && m_pRes->GetDemands() >= 1) {
        return m_pRes->GetPLTAttrib(pWidth, pHeight, pLayers);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0070DAA0
int CAuroraLayeredTexture::Load()
{
    if (!m_pRes->Demand()) {
        return 0;
    }
    unsigned long nWidth;
    unsigned long nHeight;
    unsigned long nLayers;
    m_pRes->GetPLTAttrib(&nWidth, &nHeight, &nLayers);
    unsigned char* pData = m_pRes->GetPLTDataPtr();
    if (!pData) {
        m_pRes->Release();
        return 0;
    }
    CResHelperInfo* pInfo = (CResHelperInfo*)(pData + nWidth * nHeight * 2 + 4);
    pInfo->m_pResHelper = this;
    pInfo->m_nResType = 6;
    return 1;
}

// Folded into 0x0070F380; inlined below.
unsigned char* CAuroraLayeredTexture::GetTextureData()
{
    if (m_pRes && m_pRes->GetDemands() >= 1) {
        return m_pRes->GetPLTDataPtr();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0070DB10
CResHelperInfo* CAuroraLayeredTexture::GetPtrToResHelperInfo()
{
    if (!m_pRes || m_pRes->GetDemands() < 1) {
        return 0;
    }
    unsigned char* pData = GetTextureData();
    unsigned long nWidth;
    unsigned long nHeight;
    unsigned long nLayers;
    GetTextureAttrib(&nWidth, &nHeight, &nLayers);
    return (CResHelperInfo*)(pData + nWidth * nHeight * 2 + 4);
}

// FUNCTION: SWKOTOR 0x0070DB90 SYMBOL: ??1?$CResHelper@VCResPLT@@$05@@UAE@XZ
// FUNCTION: SWKOTOR 0x0070DBF0 SYMBOL: ?SetResRef@?$CResHelper@VCResPLT@@$05@@QAEXABVCResRef@@H@Z
// FUNCTION: SWKOTOR 0x0070DD40 SYMBOL: ??_G?$CResHelper@VCResPLT@@$05@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x0070DD60
CAuroraLayeredTexture::CAuroraLayeredTexture(CResRef cResRef, int bAutoRequest)
{
    SetResRef(cResRef, bAutoRequest);
}

// FUNCTION: SWKOTOR 0x0070DDF0
CAuroraLayeredTexture::~CAuroraLayeredTexture()
{
}

// FUNCTION: SWKOTOR 0x0070DE00 SYMBOL: ??_GCAuroraLayeredTexture@@UAEPAXI@Z

// -------------------------------------------------------- CAuroraTexture

// FUNCTION: SWKOTOR 0x0070EBB0
int CAuroraTexture::GetTextureAttrib(unsigned long* pWidth, unsigned long* pHeight, unsigned char* pBitsPerPixel)
{
    if (m_pRes && m_pRes->GetDemands() >= 1) {
        return m_pRes->GetTGAAttrib(pWidth, pHeight, pBitsPerPixel);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0070EBF0
int CAuroraTexture::Load()
{
    if (!m_pRes->Demand()) {
        return 0;
    }
    unsigned long nWidth;
    unsigned long nHeight;
    unsigned char nBitsPerPixel;
    m_pRes->GetTGAAttrib(&nWidth, &nHeight, &nBitsPerPixel);
    unsigned char* pData;
    if ((nBitsPerPixel != 8 && nBitsPerPixel != 24 && nBitsPerPixel != 32)
        || (pData = m_pRes->GetTGADataPtr()) == 0) {
        m_pRes->Release();
        return 0;
    }
    CResHelperInfo* pInfo = (CResHelperInfo*)(pData + (nBitsPerPixel >> 3) * nWidth * nHeight + 4);
    pInfo->m_pResHelper = this;
    pInfo->m_nResType = 3;
    return 1;
}

// FUNCTION: SWKOTOR 0x0070EC70
void CAuroraTexture::Unload()
{
    if (m_pRes && m_pRes->GetDemands() >= 1) {
        unsigned long nWidth;
        unsigned long nHeight;
        unsigned char nBitsPerPixel;
        m_pRes->GetTGAAttrib(&nWidth, &nHeight, &nBitsPerPixel);
        unsigned long nBytesPerPixel = nBitsPerPixel >> 3;
        unsigned long nPixels = nWidth * nHeight;
        unsigned char* pPixel = m_pRes->GetTGADataPtr();
        for (unsigned long i = 0; i < nPixels; i++) {
            unsigned char nTemp = pPixel[0];
            pPixel[0] = pPixel[2];
            pPixel[2] = nTemp;
            pPixel += nBytesPerPixel;
        }
        m_pRes->Release();
    }
}

// FUNCTION: SWKOTOR 0x0070ECE0
int CAuroraTexture::ReadTextureHeader(unsigned long* pWidth, unsigned long* pHeight, unsigned long* pBytesPerPixel)
{
    TGAHeader cHeader;
    if (!m_pRes) {
        return 0;
    }
    if (!m_pRes->ReadRaw(sizeof(TGAHeader), (char*)&cHeader)) {
        return 0;
    }
    *pWidth = cHeader.m_nWidth;
    *pHeight = cHeader.m_nHeight;
    *pBytesPerPixel = cHeader.m_nBitsPerPixel >> 3;
    return 1;
}

// Folded into 0x0070F3E0; inlined below.
unsigned char* CAuroraTexture::GetTextureData()
{
    if (m_pRes && m_pRes->GetDemands() >= 1) {
        return m_pRes->GetTGADataPtr();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0070ED40
CResHelperInfo* CAuroraTexture::GetPtrToResHelperInfo()
{
    if (!m_pRes || m_pRes->GetDemands() < 1) {
        return 0;
    }
    unsigned char* pData = GetTextureData();
    unsigned long nWidth;
    unsigned long nHeight;
    unsigned char nBitsPerPixel;
    GetTextureAttrib(&nWidth, &nHeight, &nBitsPerPixel);
    return (CResHelperInfo*)(pData + (nBitsPerPixel >> 3) * nWidth * nHeight + 4);
}

// FUNCTION: SWKOTOR 0x0070EDD0 SYMBOL: ??1?$CResHelper@VCResTGA@@$02@@UAE@XZ
// FUNCTION: SWKOTOR 0x0070EE30 SYMBOL: ?SetResRef@?$CResHelper@VCResTGA@@$02@@QAEXABVCResRef@@H@Z
// FUNCTION: SWKOTOR 0x0070EF80 SYMBOL: ??_G?$CResHelper@VCResTGA@@$02@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x0070EFA0
CAuroraTexture::CAuroraTexture(CResRef cResRef, int bAutoRequest)
{
    SetResRef(cResRef, bAutoRequest);
}

// FUNCTION: SWKOTOR 0x0070F030
CAuroraTexture::~CAuroraTexture()
{
}

// FUNCTION: SWKOTOR 0x0070F040 SYMBOL: ??_GCAuroraTexture@@UAEPAXI@Z

// ----------------------------------------------------- CAuroraVisibility

// FUNCTION: SWKOTOR 0x0070F060
int CAuroraVisibility::Load()
{
    if (!m_pRes->Demand()) {
        return 0;
    }
    CResHelperInfo* pInfo = (CResHelperInfo*)(m_pRes->GetVISDataPtr() - sizeof(CResHelperInfo));
    pInfo->m_pResHelper = this;
    pInfo->m_nResType = 3001;
    return 1;
}

// FUNCTION: SWKOTOR 0x0070F090 SYMBOL: ??1?$CResHelper@VCResVIS@@$0LLJ@@@UAE@XZ
// FUNCTION: SWKOTOR 0x0070F0F0 SYMBOL: ?SetResRef@?$CResHelper@VCResVIS@@$0LLJ@@@QAEXABVCResRef@@H@Z
// FUNCTION: SWKOTOR 0x0070F250 SYMBOL: ??_G?$CResHelper@VCResVIS@@$0LLJ@@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x0070F270
CAuroraVisibility::CAuroraVisibility(CResRef cResRef, int bAutoRequest)
{
    SetResRef(cResRef, bAutoRequest);
}

// FUNCTION: SWKOTOR 0x0070F300
CAuroraVisibility::~CAuroraVisibility()
{
}

// FUNCTION: SWKOTOR 0x0070F310 SYMBOL: ??_GCAuroraVisibility@@UAEPAXI@Z

// ----------------------------------------------- CAuroraProcessedTexture

// FUNCTION: SWKOTOR 0x0070F330
int CAuroraProcessedTexture::GetProcessedTextureAttrib(int* pWidth, int* pHeight, char* pFormat,
                                                       int* pCompressedSize, int* pDataSize, int* pTXISize,
                                                       float* pAlphaTest, char* pMipMaps)
{
    if (m_pRes && m_pRes->GetDemands() >= 1) {
        return m_pRes->GetTPCAttrib(pWidth, pHeight, pFormat, pCompressedSize, pDataSize, pTXISize,
                                    pAlphaTest, pMipMaps);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0070F380
unsigned char* CAuroraProcessedTexture::GetProcessedTextureDataPtr()
{
    if (m_pRes && m_pRes->GetDemands() >= 1) {
        return m_pRes->GetTPCDataPtr();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0070F3B0
int CAuroraProcessedTexture::GetProcessedTextureDataSize()
{
    if (m_pRes && m_pRes->GetDemands() >= 1) {
        return m_pRes->GetTPCDataSize();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0070F3E0
unsigned char* CAuroraProcessedTexture::GetProcessedTextureTXIPtr()
{
    if (m_pRes && m_pRes->GetDemands() >= 1) {
        return m_pRes->GetTXIDataPtr();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0070F410
int CAuroraProcessedTexture::GetProcessedTextureTXISize()
{
    if (m_pRes && m_pRes->GetDemands() >= 1) {
        return m_pRes->GetTXIDataSize();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0070F440
int CAuroraProcessedTexture::Load()
{
    int nFaces = 1;
    if (!m_pRes->Demand()) {
        return 0;
    }
    int nWidth;
    int nHeight;
    char nFormat;
    int nCompressedSize;
    int nDataSize;
    int nTXISize;
    float fAlphaTest;
    char nMipMaps;
    m_pRes->GetTPCAttrib(&nWidth, &nHeight, &nFormat, &nCompressedSize, &nDataSize, &nTXISize, &fAlphaTest,
                         &nMipMaps);
    unsigned char* pData;
    if ((nFormat != 1 && nFormat != 3 && nFormat != 4) || (pData = m_pRes->GetTPCDataPtr()) == 0) {
        m_pRes->Release();
        return 0;
    }
    int nSize = nCompressedSize;
    if (nSize) {
        int nBlockSize;
        if (nFormat == 3) {
            nBlockSize = 8;
        } else if (nFormat == 4) {
            nBlockSize = 16;
        }
        int nW = nWidth;
        int nH = nHeight;
        if (nH / nW == 6) {
            nFaces = 6;
            nH /= 6;
        }
        for (int i = nMipMaps - 1; i > 0; i--) {
            nH >>= 1;
            nW >>= 1;
            nSize += ((nH + 3) / 4) * ((nW + 3) / 4) * nBlockSize;
        }
        if (nFaces == 6) {
            nSize *= 6;
        }
    } else {
        nSize = nDataSize;
    }
    CResHelperInfo* pInfo = (CResHelperInfo*)(pData + m_pRes->GetTXIDataSize() + nSize + 4);
    pInfo->m_pResHelper = this;
    pInfo->m_nResType = 3007;
    return 1;
}

// FUNCTION: SWKOTOR 0x0070F590
int CAuroraProcessedTexture::ReadProcessedTextureHeader(int* pWidth, int* pHeight, char* pFormat,
                                                        int* pCompressedSize, int* pDataSize, float* pAlphaTest,
                                                        char* pMipMaps)
{
    unsigned char aHeader[0x80];
    if (!m_pRes) {
        return 0;
    }
    if (!m_pRes->ReadRaw(sizeof(aHeader), (char*)aHeader)) {
        return 0;
    }
    TPCHeader* pHeader = (TPCHeader*)aHeader;
    int nWidth = pHeader->m_nWidth;
    *pWidth = nWidth;
    int nHeight = pHeader->m_nHeight;
    *pHeight = nHeight;
    *pAlphaTest = pHeader->m_fAlphaTest;
    *pMipMaps = pHeader->m_nMipMaps;
    if (pHeader->m_nEncoding & 1) {
        *pFormat = 1;
    } else if (pHeader->m_nEncoding & 2) {
        *pFormat = 3;
    } else if (pHeader->m_nEncoding & 4) {
        *pFormat = 4;
    }
    if (pHeader->m_nDataSize == 0) {
        *pDataSize = 0;
        *pCompressedSize = 0;
        for (int i = 0; i < pHeader->m_nMipMaps; i++) {
            *pDataSize += *pFormat * nHeight * nWidth;
            nWidth >>= 1;
            nHeight >>= 1;
        }
    } else {
        *pDataSize = 0;
        *pCompressedSize = pHeader->m_nDataSize;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0070F690
int CAuroraProcessedTexture::GetTextureSize()
{
    int nFaces = 1;
    int nDataSize;
    int nWidth;
    int nHeight;
    char nFormat;
    int nCompressedSize;
    int nTXISize;
    float fAlphaTest;
    char nMipMaps;
    GetProcessedTextureAttrib(&nWidth, &nHeight, &nFormat, &nCompressedSize, &nDataSize, &nTXISize, &fAlphaTest,
                              &nMipMaps);
    int nSize = nCompressedSize;
    if (nSize) {
        int nBlockSize;
        if (nFormat == 3) {
            nBlockSize = 8;
        } else if (nFormat == 4) {
            nBlockSize = 16;
        }
        if (nHeight / nWidth == 6) {
            nFaces = 6;
            nHeight /= 6;
        }
        for (int i = nMipMaps - 1; i > 0; i--) {
            nHeight >>= 1;
            nWidth >>= 1;
            nSize += ((nHeight + 3) / 4) * ((nWidth + 3) / 4) * nBlockSize;
        }
        if (nFaces == 6) {
            return nSize * 6;
        }
    } else {
        return nDataSize;
    }
    return nSize;
}

// FUNCTION: SWKOTOR 0x0070F7A0 SYMBOL: ??1?$CResHelper@VCResTPC@@$0LLP@@@UAE@XZ
// FUNCTION: SWKOTOR 0x0070F800 SYMBOL: ?SetResRef@?$CResHelper@VCResTPC@@$0LLP@@@QAEXABVCResRef@@H@Z
// FUNCTION: SWKOTOR 0x0070F960 SYMBOL: ??_G?$CResHelper@VCResTPC@@$0LLP@@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x0070F980
CAuroraProcessedTexture::CAuroraProcessedTexture(CResRef cResRef, int bAutoRequest)
{
    SetResRef(cResRef, bAutoRequest);
}

// FUNCTION: SWKOTOR 0x0070FA10
CAuroraProcessedTexture::~CAuroraProcessedTexture()
{
}

// FUNCTION: SWKOTOR 0x0070FA20
CResHelperInfo* CAuroraProcessedTexture::GetPtrToResHelperInfo()
{
    if (!m_pRes || m_pRes->GetDemands() < 1) {
        return 0;
    }
    unsigned char* pData = GetProcessedTextureDataPtr();
    int nTXISize = GetProcessedTextureTXISize();
    return (CResHelperInfo*)(pData + GetTextureSize() + nTXISize + 4);
}

// FUNCTION: SWKOTOR 0x0070FAA0 SYMBOL: ??_GCAuroraProcessedTexture@@UAEPAXI@Z

// ------------------------------------------------------------ CAuroraTXI

// FUNCTION: SWKOTOR 0x0070FAC0
int CAuroraTXI::Load()
{
    if (!m_pRes->Demand()) {
        return 0;
    }
    CResHelperInfo* pInfo = (CResHelperInfo*)(m_pRes->GetTXIDataPtr() - sizeof(CResHelperInfo));
    pInfo->m_pResHelper = this;
    pInfo->m_nResType = 2022;
    return 1;
}

// Folded into 0x007107B0; inlined below.
char* CAuroraTXI::GetTXIData()
{
    if (m_pRes && m_pRes->GetDemands() >= 1) {
        return m_pRes->GetTXIDataPtr();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0070FAF0
CResHelperInfo* CAuroraTXI::GetPtrToResHelperInfo()
{
    if (!m_pRes || m_pRes->GetDemands() < 1) {
        return 0;
    }
    return (CResHelperInfo*)(GetTXIData() - sizeof(CResHelperInfo));
}

// FUNCTION: SWKOTOR 0x0070FB30 SYMBOL: ??1?$CResHelper@VCResTXI@@$0HOG@@@UAE@XZ
// FUNCTION: SWKOTOR 0x0070FB90 SYMBOL: ?SetResRef@?$CResHelper@VCResTXI@@$0HOG@@@QAEXABVCResRef@@H@Z
// FUNCTION: SWKOTOR 0x0070FCF0 SYMBOL: ??_G?$CResHelper@VCResTXI@@$0HOG@@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x0070FD10
CAuroraTXI::CAuroraTXI(CResRef cResRef, int bAutoRequest)
{
    SetResRef(cResRef, bAutoRequest);
}

// FUNCTION: SWKOTOR 0x0070FDA0
CAuroraTXI::~CAuroraTXI()
{
}

// FUNCTION: SWKOTOR 0x0070FDB0 SYMBOL: ??_GCAuroraTXI@@UAEPAXI@Z

// --------------------------------------------------------- CAuroraModelX

// FUNCTION: SWKOTOR 0x0070FDD0
int CAuroraModelX::Load()
{
    if (!m_pRes->Demand()) {
        return 0;
    }
    CResHelperInfo* pInfo = (CResHelperInfo*)(m_pRes->GetMDXDataPtr() - sizeof(CResHelperInfo));
    pInfo->m_pResHelper = this;
    pInfo->m_nResType = 3008;
    return 1;
}

// FUNCTION: SWKOTOR 0x0070FE00 SYMBOL: ??1?$CResHelper@VCResMDX@@$0LMA@@@UAE@XZ
// FUNCTION: SWKOTOR 0x0070FE60 SYMBOL: ?SetResRef@?$CResHelper@VCResMDX@@$0LMA@@@QAEXABVCResRef@@H@Z
// FUNCTION: SWKOTOR 0x0070FFC0 SYMBOL: ??_G?$CResHelper@VCResMDX@@$0LMA@@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x0070FFE0
CAuroraModelX::CAuroraModelX(CResRef cResRef, int bAutoRequest)
{
    SetResRef(cResRef, bAutoRequest);
}

// FUNCTION: SWKOTOR 0x00710070
CAuroraModelX::~CAuroraModelX()
{
}

// FUNCTION: SWKOTOR 0x00710080 SYMBOL: ??_GCAuroraModelX@@UAEPAXI@Z

// ---------------------------------------------------------- CAuroraModel

// Folded with the other wrappers' size getters.
// FUNCTION: SWKOTOR 0x007100A0
int CAuroraModel::GetModelDataSize()
{
    if (m_pRes && m_pRes->GetDemands() >= 1) {
        return m_pRes->GetMDLSize();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x007100D0
int CAuroraModel::Load()
{
    if (!m_pRes->Demand()) {
        return 0;
    }
    CResHelperInfo* pInfo = (CResHelperInfo*)(m_pRes->GetMDLDataPtr() - sizeof(CResHelperInfo));
    pInfo->m_pResHelper = this;
    pInfo->m_nResType = 2002;
    return 1;
}

// Folded with every other wrapper's Unload.
// FUNCTION: SWKOTOR 0x00710100
void CAuroraModel::Unload()
{
    if (m_pRes && m_pRes->GetDemands() >= 1) {
        m_pRes->Release();
    }
}

// FUNCTION: SWKOTOR 0x00710120 SYMBOL: ??1?$CResHelper@VCResMDL@@$0HNC@@@UAE@XZ
// FUNCTION: SWKOTOR 0x00710180 SYMBOL: ?SetResRef@?$CResHelper@VCResMDL@@$0HNC@@@QAEXABVCResRef@@H@Z
// FUNCTION: SWKOTOR 0x007102E0 SYMBOL: ??_G?$CResHelper@VCResMDL@@$0HNC@@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x00710300
CAuroraModel::CAuroraModel(CResRef cResRef, int bAutoRequest)
{
    SetResRef(cResRef, bAutoRequest);
}

// FUNCTION: SWKOTOR 0x00710390
CAuroraModel::~CAuroraModel()
{
}

// FUNCTION: SWKOTOR 0x007103A0 SYMBOL: ??_GCAuroraModel@@UAEPAXI@Z

// ---------------------------------------------- CAuroraCompressedTexture

// FUNCTION: SWKOTOR 0x007103C0
int CAuroraCompressedTexture::GetCompressedTextureAttrib(unsigned long* pWidth, unsigned long* pHeight,
                                                         unsigned char* pBytesPerPixel, unsigned long* pDataSize,
                                                         float* pAlphaMean)
{
    if (m_pRes && m_pRes->GetDemands() >= 1) {
        return m_pRes->GetDDSAttrib(pWidth, pHeight, pBytesPerPixel, pDataSize, pAlphaMean);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00710400
int CAuroraCompressedTexture::Load()
{
    if (!m_pRes->Demand()) {
        return 0;
    }
    CResHelperInfo* pInfo =
        (CResHelperInfo*)(m_pRes->GetDDSDataPtr() - sizeof(DDSHeader) - sizeof(CResHelperInfo));
    pInfo->m_pResHelper = this;
    pInfo->m_nResType = 2033;
    return 1;
}

// FUNCTION: SWKOTOR 0x00710430
int CAuroraCompressedTexture::ReadTextureHeader(unsigned long* pWidth, unsigned long* pHeight,
                                                unsigned long* pBytesPerPixel, unsigned long* pDataSize,
                                                float* pAlphaMean)
{
    DDSHeader cHeader;
    if (!m_pRes) {
        return 0;
    }
    if (!m_pRes->ReadRaw(sizeof(DDSHeader), (char*)&cHeader)) {
        return 0;
    }
    *pWidth = cHeader.m_nWidth;
    *pHeight = cHeader.m_nHeight;
    *pBytesPerPixel = cHeader.m_nBytesPerPixel;
    *pDataSize = cHeader.m_nDataSize;
    *pAlphaMean = cHeader.m_fAlphaMean;
    return 1;
}

// Folded into 0x007107B0; inlined below.
unsigned char* CAuroraCompressedTexture::GetTextureData()
{
    if (m_pRes && m_pRes->GetDemands() >= 1) {
        return m_pRes->GetDDSDataPtr();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00710490
CResHelperInfo* CAuroraCompressedTexture::GetPtrToResHelperInfo()
{
    if (!m_pRes || m_pRes->GetDemands() < 1) {
        return 0;
    }
    return (CResHelperInfo*)(GetTextureData() - sizeof(DDSHeader) - sizeof(CResHelperInfo));
}

// FUNCTION: SWKOTOR 0x007104D0 SYMBOL: ??1?$CResHelper@VCResDDS@@$0HPB@@@UAE@XZ
// FUNCTION: SWKOTOR 0x00710530 SYMBOL: ?SetResRef@?$CResHelper@VCResDDS@@$0HPB@@@QAEXABVCResRef@@H@Z
// FUNCTION: SWKOTOR 0x00710690 SYMBOL: ??_G?$CResHelper@VCResDDS@@$0HPB@@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x007106B0
CAuroraCompressedTexture::CAuroraCompressedTexture(CResRef cResRef, int bAutoRequest)
{
    SetResRef(cResRef, bAutoRequest);
}

// FUNCTION: SWKOTOR 0x00710740
CAuroraCompressedTexture::~CAuroraCompressedTexture()
{
}

// FUNCTION: SWKOTOR 0x00710750 SYMBOL: ??_GCAuroraCompressedTexture@@UAEPAXI@Z

// --------------------------------------------------- CAurora16BitTexture

// FUNCTION: SWKOTOR 0x00710770
int CAurora16BitTexture::GetTextureAttrib(unsigned long* pWidth, unsigned long* pHeight, float* pAlphaMean,
                                          int* pDataSize)
{
    if (m_pRes && m_pRes->GetDemands() >= 1) {
        return m_pRes->Get4PCAttrib(pWidth, pHeight, pAlphaMean, pDataSize);
    }
    return 0;
}

// Folded with the other wrappers' data pointer getters.
// FUNCTION: SWKOTOR 0x007107B0
unsigned char* CAurora16BitTexture::Get16BitTextureDataPtr()
{
    if (m_pRes && m_pRes->GetDemands() >= 1) {
        return m_pRes->Get4PCDataPtr();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x007107E0
int CAurora16BitTexture::Load()
{
    if (!m_pRes->Demand()) {
        return 0;
    }
    CResHelperInfo* pInfo =
        (CResHelperInfo*)(m_pRes->Get4PCDataPtr() - sizeof(FourPCHeader) - sizeof(CResHelperInfo));
    pInfo->m_pResHelper = this;
    pInfo->m_nResType = 2059;
    return 1;
}

// FUNCTION: SWKOTOR 0x00710810
int CAurora16BitTexture::ReadTextureHeader(unsigned long* pWidth, unsigned long* pHeight, float* pAlphaMean,
                                           int* pDataSize)
{
    FourPCHeader cHeader;
    if (!m_pRes) {
        return 0;
    }
    if (!m_pRes->ReadRaw(sizeof(FourPCHeader), (char*)&cHeader)) {
        return 0;
    }
    *pWidth = cHeader.m_nWidth;
    *pHeight = cHeader.m_nHeight;
    *pAlphaMean = cHeader.m_fAlphaMean;
    *pDataSize = cHeader.m_nDataSize;
    return 1;
}

// FUNCTION: SWKOTOR 0x00710870
CResHelperInfo* CAurora16BitTexture::GetPtrToResHelperInfo()
{
    if (!m_pRes || m_pRes->GetDemands() < 1) {
        return 0;
    }
    return (CResHelperInfo*)(Get16BitTextureDataPtr() - sizeof(FourPCHeader) - sizeof(CResHelperInfo));
}

// FUNCTION: SWKOTOR 0x007108B0 SYMBOL: ??1?$CResHelper@VCRes4PC@@$0IAL@@@UAE@XZ
// FUNCTION: SWKOTOR 0x00710910 SYMBOL: ?SetResRef@?$CResHelper@VCRes4PC@@$0IAL@@@QAEXABVCResRef@@H@Z
// FUNCTION: SWKOTOR 0x00710A70 SYMBOL: ??_G?$CResHelper@VCRes4PC@@$0IAL@@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x00710A90
CAurora16BitTexture::CAurora16BitTexture(CResRef cResRef, int bAutoRequest)
{
    SetResRef(cResRef, bAutoRequest);
}

// FUNCTION: SWKOTOR 0x00710B20
CAurora16BitTexture::~CAurora16BitTexture()
{
}

// FUNCTION: SWKOTOR 0x00710B30 SYMBOL: ??_GCAurora16BitTexture@@UAEPAXI@Z

template class CResHelper<CResPLT, 6>;
template class CResHelper<CResTGA, 3>;
template class CResHelper<CResVIS, 3001>;
template class CResHelper<CResTPC, 3007>;
template class CResHelper<CResTXI, 2022>;
template class CResHelper<CResMDX, 3008>;
template class CResHelper<CResMDL, 2002>;
template class CResHelper<CResDDS, 2033>;
template class CResHelper<CRes4PC, 2059>;
