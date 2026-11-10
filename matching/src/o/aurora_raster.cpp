// CAuroraInterface raster accessors, 0x0070C6E0-0x0070C870. A separate
// file from the Get*/Release*Internal helpers, which stay calls here.
#include <string.h>

#include "o/aurora_textures.h"

// FUNCTION: SWKOTOR 0x0070C6E0
unsigned char* CAuroraInterface::GetLayeredRaster(char* szName, unsigned long* pWidth, unsigned long* pHeight,
                                                  unsigned long* pLayers)
{
    CAuroraLayeredTexture* pTexture;
    {
        CExoString sName(szName);
        pTexture = GetLayeredTexture(sName);
    }
    if (!pTexture) {
        return 0;
    }
    unsigned long nWidth;
    unsigned long nHeight;
    unsigned long nLayers;
    pTexture->GetTextureAttrib(&nWidth, &nHeight, &nLayers);
    *pWidth = nWidth;
    *pHeight = nHeight;
    *pLayers = nLayers;
    return pTexture->GetTextureData();
}

// FUNCTION: SWKOTOR 0x0070C790
unsigned char* CAuroraInterface::GetRaster(char* szName, unsigned long* pWidth, unsigned long* pHeight,
                                           unsigned long* pBytesPerPixel)
{
    CAuroraTexture* pTexture;
    {
        CExoString sName(szName);
        pTexture = GetTexture(sName);
    }
    if (!pTexture) {
        return 0;
    }
    unsigned long nWidth;
    unsigned long nHeight;
    unsigned char nBitsPerPixel;
    pTexture->GetTextureAttrib(&nWidth, &nHeight, &nBitsPerPixel);
    *pWidth = nWidth;
    *pHeight = nHeight;
    *pBytesPerPixel = nBitsPerPixel >> 3;
    return pTexture->GetTextureData();
}

// FUNCTION: SWKOTOR 0x0070C850
void CAuroraInterface::ReleaseLayeredRaster(void* pData, int nWidth, int nHeight)
{
    CResHelperInfo* pInfo = (CResHelperInfo*)((unsigned char*)pData + nWidth * nHeight * 2 + 4);
    ReleaseLayeredTextureInternal((CAuroraLayeredTexture*)pInfo->m_pResHelper);
}

// FUNCTION: SWKOTOR 0x0070C870
void CAuroraInterface::ReleaseRaster(void* pData, int nWidth, int nHeight, int nBytesPerPixel, int nCompressedSize,
                                     int nDataSize, int nTXISize, unsigned char bProcessed)
{
    CResHelperInfo* pInfo;
    if (bProcessed) {
        int nSize = nCompressedSize;
        if (!nSize) {
            nSize = nDataSize;
        }
        pInfo = (CResHelperInfo*)((unsigned char*)pData + nSize + nTXISize + 4);
        if (pInfo->m_nResType == 3007) {
            ReleaseProcessedTextureInternal((CAuroraProcessedTexture*)pInfo->m_pResHelper);
        }
    } else if (nCompressedSize) {
        pInfo = (CResHelperInfo*)((unsigned char*)pData - sizeof(DDSHeader) - sizeof(CResHelperInfo));
        if (pInfo->m_nResType == 2033) {
            ReleaseCompressedTextureInternal((CAuroraCompressedTexture*)pInfo->m_pResHelper);
        }
    } else if (nBytesPerPixel == 2) {
        pInfo = (CResHelperInfo*)((unsigned char*)pData - sizeof(FourPCHeader) - sizeof(CResHelperInfo));
        if (pInfo->m_nResType == 2059) {
            Release16BitTextureInternal((CAurora16BitTexture*)pInfo->m_pResHelper);
        }
    } else {
        pInfo = (CResHelperInfo*)((unsigned char*)pData + nWidth * nHeight * nBytesPerPixel + 4);
        if (pInfo->m_nResType == 3) {
            ReleaseTextureInternal((CAuroraTexture*)pInfo->m_pResHelper);
        }
    }
}


// nOffset >= 0: a header-prefixed resource tagged just before its data;
// otherwise a processed texture tagged -nOffset bytes after it.
// FUNCTION: SWKOTOR 0x0070CBD0
void CAuroraInterface::ReleaseResource(void* pData, int nOffset)
{
    CResHelperInfo* pInfo;
    if (nOffset >= 0) {
        unsigned char* pTag = (unsigned char*)pData - sizeof(CResHelperInfo);
        unsigned short nType = *(unsigned short*)pTag;
        pTag += sizeof(unsigned short);
        switch (nType) {
        case 2022:
            ReleaseTXIInternal(*(CAuroraTXI**)pTag);
            break;
        case 2002:
            ReleaseModelInternal(*(CAuroraModel**)pTag);
            break;
        case 3001:
            ReleaseVisibilityInternal(*(CAuroraVisibility**)pTag);
            break;
        case 3008:
            ReleaseModelXInternal(*(CAuroraModelX**)pTag);
            break;
        }
    } else {
        pInfo = (CResHelperInfo*)((unsigned char*)pData - nOffset + 3);
        switch (pInfo->m_nResType) {
        case 3006:
        case 3007:
            ReleaseProcessedTextureInternal((CAuroraProcessedTexture*)pInfo->m_pResHelper);
            break;
        }
    }
}

// FUNCTION: SWKOTOR 0x0070CC60
int CAuroraInterface::ReadRasterHeader(char* szName, unsigned long* pWidth, unsigned long* pHeight,
                                       unsigned long* pBytesPerPixel)
{
    if (strlen(szName) > 16) {
        return 0;
    }
    CAuroraTexture cTexture(CResRef(szName), 0);
    return cTexture.ReadTextureHeader(pWidth, pHeight, pBytesPerPixel);
}

// FUNCTION: SWKOTOR 0x0070CD10
int CAuroraInterface::ReadProcessedRasterHeader(char* szName, int* pWidth, int* pHeight, char* pFormat,
                                                int* pCompressedSize, int* pDataSize, float* pAlphaTest,
                                                char* pMipMaps)
{
    if (strlen(szName) > 16) {
        return 0;
    }
    CAuroraProcessedTexture cTexture(CResRef(szName), 0);
    return cTexture.ReadProcessedTextureHeader(pWidth, pHeight, pFormat, pCompressedSize, pDataSize,
                                               pAlphaTest, pMipMaps);
}

// FUNCTION: SWKOTOR 0x0070CDD0
int CAuroraInterface::ReadCompressedRasterHeader(char* szName, unsigned long* pWidth, unsigned long* pHeight,
                                                 unsigned long* pBytesPerPixel, unsigned long* pDataSize,
                                                 float* pAlphaMean)
{
    if (strlen(szName) > 16) {
        return 0;
    }
    CAuroraCompressedTexture cTexture(CResRef(szName), 0);
    return cTexture.ReadTextureHeader(pWidth, pHeight, pBytesPerPixel, pDataSize, pAlphaMean);
}

// FUNCTION: SWKOTOR 0x0070CE80
int CAuroraInterface::Get16BitRasterHeader(char* szName, unsigned long* pWidth, unsigned long* pHeight,
                                           float* pAlphaMean, int* pDataSize)
{
    if (strlen(szName) > 16) {
        return 0;
    }
    CAurora16BitTexture cTexture(CResRef(szName), 0);
    return cTexture.ReadTextureHeader(pWidth, pHeight, pAlphaMean, pDataSize);
}

// A texture format is used only when its key-table location (Exists'
// out value) outranks every lower format's: 2, then 4, then 3, then 1.
static inline int IsPreferredSource(unsigned long nMine, unsigned long nA, unsigned long nB, unsigned long nC)
{
    if (nMine == 2) {
        return 1;
    }
    if (nA == 2 || nB == 2 || nC == 2) {
        return 0;
    }
    if (nMine == 4) {
        return 1;
    }
    if (nA == 4 || nB == 4 || nC == 4) {
        return 0;
    }
    if (nMine == 3) {
        return 1;
    }
    if (nA == 3 || nB == 3 || nC == 3) {
        return 0;
    }
    if (nMine == 1) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0070D380
unsigned char* CAuroraInterface::GetCompressedRaster(char* szName, unsigned long* pWidth, unsigned long* pHeight,
                                                     unsigned long* pBytesPerPixel, unsigned long* pDataSize,
                                                     float* pAlphaMean)
{
    CResRef cResRef(szName);
    unsigned long nDDS = 0;
    unsigned long n4PC = 0;
    unsigned long nTGA = 0;
    unsigned long nTPC = 0;
    if (!g_pExoResMan->Exists(cResRef, 2033, &nDDS)) {
        nDDS = 0;
    }
    if (!g_pExoResMan->Exists(cResRef, 2059, &n4PC)) {
        n4PC = 0;
    }
    if (!g_pExoResMan->Exists(cResRef, 3, &nTGA)) {
        nTGA = 0;
    }
    if (!IsPreferredSource(nDDS, n4PC, nTGA, nTPC)) {
        return 0;
    }
    CAuroraCompressedTexture* pTexture;
    {
        CExoString sName(szName);
        pTexture = GetCompressedTexture(sName);
    }
    if (!pTexture) {
        return 0;
    }
    unsigned long nWidth;
    unsigned long nHeight;
    unsigned char nBytesPerPixel;
    unsigned long nDataSize;
    float fAlphaMean;
    pTexture->GetCompressedTextureAttrib(&nWidth, &nHeight, &nBytesPerPixel, &nDataSize, &fAlphaMean);
    *pWidth = nWidth;
    *pHeight = nHeight;
    *pBytesPerPixel = nBytesPerPixel;
    *pDataSize = nDataSize;
    *pAlphaMean = fAlphaMean;
    return pTexture->GetTextureData();
}

// FUNCTION: SWKOTOR 0x0070D510
unsigned char* CAuroraInterface::GetProcessedRaster(char* szName, int* pWidth, int* pHeight, char* pFormat,
                                                    int* pCompressedSize, int* pDataSize, int* pTXISize,
                                                    float* pAlphaTest, char* pMipMaps)
{
    CResRef cResRef(szName);
    unsigned long nDDS = 0;
    unsigned long n4PC = 0;
    unsigned long nTGA = 0;
    unsigned long nTPC = 0;
    if (!g_pExoResMan->Exists(cResRef, 3007, &nTPC)) {
        nTPC = 0;
    }
    if (!g_pExoResMan->Exists(cResRef, 2033, &nDDS)) {
        nDDS = 0;
    }
    if (!g_pExoResMan->Exists(cResRef, 2059, &n4PC)) {
        n4PC = 0;
    }
    if (!g_pExoResMan->Exists(cResRef, 3, &nTGA)) {
        nTGA = 0;
    }
    if (!IsPreferredSource(nTPC, nDDS, n4PC, nTGA)) {
        return 0;
    }
    CAuroraProcessedTexture* pTexture;
    {
        CExoString sName(szName);
        pTexture = GetProcessedTexture(sName);
    }
    if (!pTexture) {
        return 0;
    }
    pTexture->GetProcessedTextureAttrib(pWidth, pHeight, pFormat, pCompressedSize, pDataSize, pTXISize,
                                        pAlphaTest, pMipMaps);
    return pTexture->GetProcessedTextureDataPtr();
}

// FUNCTION: SWKOTOR 0x0070D6B0
unsigned char* CAuroraInterface::Get16BitRaster(char* szName, unsigned long* pWidth, unsigned long* pHeight,
                                                float* pAlphaMean, int* pDataSize)
{
    CResRef cResRef(szName);
    unsigned long nDDS = 0;
    unsigned long n4PC = 0;
    unsigned long nTGA = 0;
    unsigned long nTPC = 0;
    if (!g_pExoResMan->Exists(cResRef, 2059, &n4PC)) {
        n4PC = 0;
    }
    if (!g_pExoResMan->Exists(cResRef, 3, &nTGA)) {
        nTGA = 0;
    }
    if (!IsPreferredSource(n4PC, nTGA, nDDS, nTPC)) {
        return 0;
    }
    CAurora16BitTexture* pTexture;
    {
        CExoString sName(szName);
        pTexture = Get16BitTexture(sName);
    }
    if (!pTexture) {
        return 0;
    }
    unsigned long nWidth;
    unsigned long nHeight;
    float fAlphaMean;
    int nDataSize;
    pTexture->GetTextureAttrib(&nWidth, &nHeight, &fAlphaMean, &nDataSize);
    *pWidth = nWidth;
    *pHeight = nHeight;
    *pAlphaMean = fAlphaMean;
    *pDataSize = nDataSize;
    return pTexture->Get16BitTextureDataPtr();
}
