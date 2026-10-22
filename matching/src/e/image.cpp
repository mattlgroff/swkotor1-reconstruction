// image helpers (0x0045D990 .. 0x0045F55F).

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// FUNCTION: SWKOTOR 0x0045D990
void ImageFlipY(unsigned char* pData, int nWidth, int nHeight, int nBpp)
{
    int nSize = nWidth * nHeight * nBpp;
    unsigned char* pTemp = new unsigned char[nSize];
    for (int y = 0; y < nHeight; y++) {
        for (int x = 0; x < nWidth; x++) {
            memcpy(pTemp + ((nHeight - 1 - y) * nWidth + x) * nBpp,
                   pData + (y * nWidth + x) * nBpp, nBpp);
        }
    }
    memcpy(pData, pTemp, nSize);
    delete pTemp;
}

// NOTE: NONMATCH - original recomputes (srcRow + sx) * bpp per tap and keeps sx in the
// bpp arg slot; ours strength-reduces the source pointer.
// FUNCTION: SWKOTOR 0x0045DAD0
unsigned char* ImageScale(unsigned char* pSrc, int nSrcWidth, int nSrcHeight, int nBpp,
                          int nDstWidth, int nDstHeight)
{
    int nRowSize = nBpp * nDstWidth;
    int nSize = nRowSize * nDstHeight;
    if (!nSize)
        return 0;
    unsigned char* pDst = new unsigned char[nSize];
    int nAccSize = nRowSize * 4;
    unsigned int* pAcc = (unsigned int*)new unsigned char[nAccSize];
    memset(pAcc, 0, nAccSize);

    int nRowLeft = nDstHeight;
    int nRowNeed = nSrcHeight;
    int nSrcRow = 0;
    int nDstRow = 0;
    for (int y = 0; y < nDstHeight;) {
        int nWeightY = nRowNeed;
        if (nRowLeft < nRowNeed)
            nWeightY = nRowLeft;
        int nColNeed = nSrcWidth;
        int nColLeft = nDstWidth;
        int sx = 0;
        for (int dx = 0; dx < nDstWidth;) {
            if (nColLeft >= nColNeed) {
                for (int c = 0; c < nBpp; c++)
                    pAcc[dx * nBpp + c] += pSrc[(sx + nSrcRow) * nBpp + c] * nColNeed * nWeightY;
                nColLeft -= nColNeed;
                if (!nColLeft) {
                    sx++;
                    nColLeft = nDstWidth;
                }
                dx++;
                nColNeed = nSrcWidth;
            }
            else {
                for (int c = 0; c < nBpp; c++)
                    pAcc[dx * nBpp + c] += pSrc[(sx + nSrcRow) * nBpp + c] * nWeightY * nColLeft;
                nColNeed -= nColLeft;
                nColLeft = nDstWidth;
                sx++;
            }
        }
        if (nRowLeft >= nRowNeed) {
            int nDiv = nSrcWidth * nSrcHeight;
            for (int dx = 0; dx < nDstWidth; dx++) {
                for (int c = 0; c < nBpp; c++)
                    pDst[(nDstRow + dx) * nBpp + c] =
                        (unsigned char)((pAcc[dx * nBpp + c] + nDiv / 2) / nDiv);
            }
            nRowLeft -= nRowNeed;
            if (!nRowLeft) {
                nSrcRow += nSrcWidth;
                nRowLeft = nDstHeight;
            }
            y++;
            nRowNeed = nSrcHeight;
            nDstRow += nDstWidth;
            memset(pAcc, 0, nAccSize);
        }
        else {
            nRowNeed -= nRowLeft;
            nRowLeft = nDstHeight;
            nSrcRow += nSrcWidth;
        }
    }
    delete pAcc;
    return pDst;
}

// FUNCTION: SWKOTOR 0x0045DD90
void ImageGammaCorrect(int nWidth, int nHeight, int nBpp, unsigned char* pData, float fGamma)
{
    if (fGamma != 1.0f) {
        int nSize = nWidth * nHeight * nBpp;
        for (int i = 0; i < nSize; i++) {
            float fInv = 1.0f / fGamma;
            pData[i] = (unsigned char)(int)(pow(pData[i] * (1.0f / 255.0f), fInv) * 255.0);
        }
    }
}

extern int g_nImageDecodeCalls;     // 0x007B9334
extern int g_bImageDecodeDisabled;  // 0x007BB510

// FUNCTION: SWKOTOR 0x0045DE10
unsigned int* ImageDecode(unsigned char* pSrc, int nWidth, int nHeight,
                          unsigned int (*pPalettes)[256], int nUnused, int* pIndices,
                          int nCount)
{
    g_nImageDecodeCalls++;
    if (g_bImageDecodeDisabled)
        return 0;
    unsigned int aTable[15][256];
    unsigned int* pOut = new unsigned int[nWidth * nHeight];
    for (int i = 0; i < nCount; i++)
        memcpy(aTable[i], pPalettes[pIndices[i]], sizeof(aTable[i]));
    unsigned int* pDst = pOut;
    for (int y = 0; y < nHeight; y++) {
        for (int x = 0; x < nWidth; x++) {
            unsigned char* p = pSrc + (y * nWidth + x) * 2;
            *pDst++ = aTable[0][(p[1] << 8) + p[0]];
        }
    }
    return pOut;
}

// FUNCTION: SWKOTOR 0x0045DF10
unsigned char* ImageRead16bit(char* szName, int* pWidth, int* pHeight, int* pBpp, int* pFormat)
{
    char szPath[256];
    strcpy(szPath, szName);
    FILE* f = fopen(szPath, "rb");
    if (!f) {
        strcat(szPath, ".4pc");
        f = fopen(szPath, "rb");
        if (!f)
            return 0;
    }
    fread(pWidth, 4, 1, f);
    fread(pHeight, 4, 1, f);
    fread(pBpp, 4, 1, f);
    fread(pFormat, 4, 1, f);
    unsigned char* pData = new unsigned char[*pWidth * *pHeight * 2];
    fread(pData, 1, *pWidth * *pHeight * 2, f);
    fclose(f);
    return pData;
}

// FUNCTION: SWKOTOR 0x0045E0F0
int ImageReadS3TCHeader(FILE* f, int* pWidth, int* pHeight, int* pFormat, int* pMipMaps,
                        float* pAlphaMean)
{
    if (!f)
        return 0;
    *pMipMaps = 0;
    *pWidth = 0;
    *pHeight = 0;
    *pFormat = 0;
    *pAlphaMean = -1.0f;
    fread(pWidth, 4, 1, f);
    fread(pHeight, 4, 1, f);
    fread(pFormat, 4, 1, f);
    fread(pMipMaps, 4, 1, f);
    fread(pAlphaMean, 4, 1, f);
    return 1;
}

int ImageReadTGAHeader(FILE* f, int* pWidth, int* pHeight, int* pBpp);             // 0x0045E2E0
unsigned char* ImageReadTGAImage(FILE* f, int nWidth, int nHeight, int nBpp);      // 0x0045E320
void ImageSwapRB(unsigned char* pData, int nWidth, int nHeight, int nBpp);         // 0x0045DA80

// FUNCTION: SWKOTOR 0x0045E350
unsigned char* ImageReadTGA(char* szName, int* pWidth, int* pHeight, int* pBpp)
{
    if (!szName || !*szName)
        return 0;
    char szPath[256];
    sprintf(szPath, "%s", szName);
    FILE* f = fopen(szPath, "rb");
    if (!f) {
        sprintf(szPath, "%s.tga", szName);
        f = fopen(szPath, "rb");
        if (!f)
            return 0;
    }
    ImageReadTGAHeader(f, pWidth, pHeight, pBpp);
    unsigned char* pData = ImageReadTGAImage(f, *pWidth, *pHeight, *pBpp);
    fclose(f);
    ImageSwapRB(pData, *pWidth, *pHeight, *pBpp);
    return pData;
}

// FUNCTION: SWKOTOR 0x0045E430
unsigned char* ImageLoadTGA(char* szName, int* pWidth, int* pHeight, int* pBpp)
{
    return ImageReadTGA(szName, pWidth, pHeight, pBpp);
}

struct PLTHeader {
    char szSignature[16];
    int nWidth;
    int nHeight;
};

// FUNCTION: SWKOTOR 0x0045E510
unsigned short* ImageReadPLT(char* szName, int* pWidth, int* pHeight)
{
    char szPath[32];
    PLTHeader header;
    sprintf(szPath, "%s.plt", szName);
    FILE* f = fopen(szPath, "rb");
    if (!f)
        return 0;
    fread(&header, sizeof(header), 1, f);
    *pWidth = header.nWidth;
    *pHeight = header.nHeight;
    unsigned short* pData = (unsigned short*)new unsigned char[header.nWidth * header.nHeight * 2];
    if (!pData)
        return 0;
    fread(pData, 1, header.nWidth * header.nHeight * 2, f);
    fclose(f);
    return pData;
}

static inline void FloatToInt(int* pOut, float f)
{
    __asm {
        fld f
        mov edx, pOut
        frndint
        fistp dword ptr [edx]
    }
}

// FUNCTION: SWKOTOR 0x0045E5B0
unsigned char ImageInterpolate(unsigned char* pData, float x, float y, int nWidth, int nHeight)
{
    int ix = (int)floor(x);
    int iy = (int)floor(y);
    float fx = x - ix;
    int ix1 = (ix + 1) % nWidth;
    float fy = y - iy;
    int iy1 = (iy + 1) % nHeight;
    float f = (pData[iy * nWidth + ix] * (1.0f - fx) + pData[iy * nWidth + ix1] * fx) * (1.0f - fy) +
              (pData[iy1 * nWidth + ix] * (1.0f - fx) + pData[iy1 * nWidth + ix1] * fx) * fy;
    int n;
    FloatToInt(&n, f);
    return (unsigned char)n;
}

// FUNCTION: SWKOTOR 0x0045E6B0
unsigned char* ImageBuildPerlin(int nSize, float fScale)
{
    int n = nSize * nSize;
    unsigned char* pData = new unsigned char[n];
    for (int i = 0; i < n; i++)
        pData[i] = (unsigned char)(int)((rand() % 128 - 64) * fScale + 127.0f);
    return pData;
}

// FUNCTION: SWKOTOR 0x0045E710
// NOTE: NONMATCH - return zero-extends with xor eax,eax / mov al,bl in the original (ours: movzx).
int ImageGetPerlinPixel(unsigned char** pLayers, int x, int y, int nScale, int nLayers, int nSize)
{
    int nSum = 0;
    int nCur = nScale;
    float fx = (float)x / nSize;
    float fy = (float)y / nSize;
    for (int i = 0; i < nLayers; i++) {
        nSum += ImageInterpolate(pLayers[i], fx * nCur, fy * nCur, nCur, nCur) - 127;
        nCur *= 2;
    }
    return (unsigned char)nSum + 127;
}

// FUNCTION: SWKOTOR 0x0045E790
float ImageGetAlphaMean(unsigned char* pData, int nWidth, int nHeight, int nBpp)
{
    if (nBpp < 4)
        return 1.0f;
    float fSum = 0.0f;
    int n = nWidth * nHeight;
    for (int i = 0; i < n; i++)
        fSum += pData[i * nBpp + 3] * (1.0f / 255.0f);
    return fSum / n;
}

extern float g_aLowpassKernel[3][3];    // 0x0078E450

// FUNCTION: SWKOTOR 0x0045E8B0
void ImageLowpassLowQualityBlur(unsigned char* pDst, unsigned char* pSrc, int nWidth,
                                int nHeight, int nPart)
{
    for (int y = nHeight * nPart / 3; y < (nPart + 1) * nHeight / 3; y++) {
        for (int x = 0; x < nWidth; x++) {
            for (int k = 0; k < 3; k++) {
                int yy = (y + nHeight - 1 + k) % nHeight;
                for (int j = 0; j < 3; j++) {
                    if ((k == 0 || k == 2) && (j == 0 || j == 2))
                        continue;
                    pDst[y * nWidth + x] += (int)(pSrc[yy * nWidth + (x + nWidth - 1 + j) % nWidth] *
                                                  g_aLowpassKernel[k][j]);
                }
            }
        }
    }
}

extern unsigned char g_aLowpassShift[3][3];     // 0x0078E474

// FUNCTION: SWKOTOR 0x0045EA70
void ImageLowpassLowQualityBlur2(unsigned char* pData, int nWidth, int nHeight)
{
    static unsigned char* s_pBuffer = new unsigned char[nWidth * nHeight];
    static int s_nHeight = nHeight;
    static int s_nWidth = nWidth;
    if (s_nHeight != nHeight || s_nWidth != nWidth) {
        delete s_pBuffer;
        s_pBuffer = new unsigned char[nWidth * nHeight];
        s_nHeight = nHeight;
        s_nWidth = nWidth;
    }
    memset(s_pBuffer, 0, nWidth * nHeight);
    for (int y = 0; y < nHeight; y++) {
        for (int x = 0; x < nWidth; x++) {
            for (int k = 0; k < 3; k++) {
                int yy = (y + nHeight - 1 + k) % nHeight;
                for (int j = 0; j < 3; j++) {
                    if ((k == 0 || k == 2) && (j == 0 || j == 2))
                        continue;
                    s_pBuffer[y * nWidth + x] +=
                        pData[yy * nWidth + (x + nWidth - 1 + j) % nWidth] >> g_aLowpassShift[k][j];
                }
            }
        }
    }
    memcpy(pData, s_pBuffer, nWidth * nHeight);
}

// NOTE: NONMATCH - only the reload order of x / j after the bpp loop differs.
// FUNCTION: SWKOTOR 0x0045ECB0
void ImageLowpassBlur(unsigned char* pData, int nWidth, int nHeight, int nBpp)
{
    float aKernel[3][3] = {
        { 0.025f, 0.1f, 0.025f },
        { 0.1f, 0.5f, 0.1f },
        { 0.025f, 0.1f, 0.025f },
    };
    int nSize = nWidth * nHeight * nBpp;
    unsigned char* pTemp = new unsigned char[nSize];
    memset(pTemp, 0, nSize);
    for (int y = 0; y < nHeight; y++) {
        for (int x = 0; x < nWidth; x++) {
            for (int k = 0; k < 3; k++) {
                int yy = (y + nHeight - 1 + k) % nHeight;
                for (int j = 0; j < 3; j++) {
                    int xx = (x + nWidth - 1 + j) % nWidth;
                    for (int c = 0; c < nBpp; c++)
                        pTemp[(y * nWidth + x) * nBpp + c] +=
                            (int)(pData[(yy * nWidth + xx) * nBpp + c] * aKernel[k][j]);
                }
            }
        }
    }
    memcpy(pData, pTemp, nSize);
    delete pTemp;
}

// FUNCTION: SWKOTOR 0x0045F000
unsigned char* ImageGetSubImage(unsigned char* pData, int nWidth, int nHeight, int nBpp,
                                int nX, int nY, int nSubWidth, int nSubHeight)
{
    unsigned char* pSub = new unsigned char[nBpp * nSubWidth * nSubHeight];
    for (int y = 0; y < nSubHeight; y++) {
        for (int x = 0; x < nSubWidth; x++) {
            memcpy(pSub + (y * nSubWidth + x) * nBpp,
                   pData + ((nY + y) * nWidth + nX + x) * nBpp, nBpp);
        }
    }
    return pSub;
}

// NOTE: NONMATCH - original shares one temp slot for alpha / int->float / FloatToInt arg
// and computes the three a*c products before the adds (frame 0x54 vs ours 0x4c).
// FUNCTION: SWKOTOR 0x0045F0D0
void ImageFillAlpha(unsigned char* pData, int nWidth, int nHeight, int nBpp, float* pColor)
{
    if (nBpp != 4)
        return;
    for (int y = 0; y < nHeight; y++) {
        for (int x = 0; x < nWidth; x++) {
            int i = (y * nWidth + x) * 4;
            float aSrc[4];
            float aBack[3];
            float aOut[3];
            aSrc[0] = pData[i] * (1.0f / 255.0f);
            aSrc[1] = pData[i + 1] * (1.0f / 255.0f);
            aSrc[2] = pData[i + 2] * (1.0f / 255.0f);
            aSrc[3] = pData[i + 3] * (1.0f / 255.0f);
            float fInv = 1.0f - aSrc[3];
            aBack[0] = fInv * pColor[0];
            aBack[1] = fInv * pColor[1];
            aBack[2] = fInv * pColor[2];
            aSrc[0] *= aSrc[3];
            aSrc[1] *= aSrc[3];
            aSrc[2] *= aSrc[3];
            aOut[0] = aSrc[0] + aBack[0];
            aOut[1] = aSrc[1] + aBack[1];
            aOut[2] = aSrc[2] + aBack[2];
            int nR, nG, nB;
            FloatToInt(&nR, aOut[0] * 255.0f);
            FloatToInt(&nG, aOut[1] * 255.0f);
            FloatToInt(&nB, aOut[2] * 255.0f);
            pData[i] = nR;
            pData[i + 1] = nG;
            pData[i + 2] = nB;
            pData[i + 3] = 255;
        }
    }
}

struct TGAHeader {
    unsigned char nIDLength;
    unsigned char nColorMapType;
    unsigned char nImageType;
    unsigned char aColorMapSpec[5];
    short nXOrigin;
    short nYOrigin;
    short nWidth;
    short nHeight;
    unsigned char nPixelDepth;
    unsigned char nDescriptor;
};

// FUNCTION: SWKOTOR 0x0045F2A0
void ImageWriteTGA(char* szName, int nWidth, int nHeight, int nBpp, unsigned char* pData)
{
    TGAHeader header;
    header.nIDLength = 0;
    header.nColorMapType = 0;
    header.nImageType = 2;
    header.aColorMapSpec[0] = 0;
    header.aColorMapSpec[1] = 0;
    header.aColorMapSpec[2] = 0;
    header.aColorMapSpec[3] = 0;
    header.aColorMapSpec[4] = 0;
    header.nXOrigin = 0;
    header.nYOrigin = 0;
    header.nWidth = nWidth;
    header.nHeight = nHeight;
    header.nPixelDepth = nBpp * 8;
    header.nDescriptor = 0;
    FILE* f = fopen(szName, "wb");
    fwrite(&header, sizeof(header), 1, f);
    int nSize = nWidth * nHeight * nBpp;
    unsigned char* pTemp = new unsigned char[nSize];
    memcpy(pTemp, pData, nSize);
    ImageFlipY(pTemp, nWidth, nHeight, nBpp);
    ImageSwapRB(pTemp, nWidth, nHeight, nBpp);
    fwrite(pTemp, 1, nSize, f);
    delete pTemp;
    fclose(f);
}

int ImageGetS3TCSize(int nWidth, int nHeight, int nFormat);     // 0x0045E270

// NOTE: NONMATCH - register allocation: original keeps w/h/size/p in ebx/edi/ebp/ecx.
// FUNCTION: SWKOTOR 0x0045F390 SYMBOL: ?ImageReadS3TC@@YAPAEPAU_iobuf@@PAH111PAM@Z
unsigned char* ImageReadS3TC(FILE* f, int* pWidth, int* pHeight, int* pFormat, int* pSize,
                             float* pAlphaMean)
{
    if (!f)
        return 0;
    fread(pWidth, 4, 1, f);
    fread(pHeight, 4, 1, f);
    fread(pFormat, 4, 1, f);
    fread(pSize, 4, 1, f);
    fread(pAlphaMean, 4, 1, f);
    int nBlockSize = *pFormat == 4 ? 16 : 8;
    unsigned char* pData = new unsigned char[ImageGetS3TCSize(*pWidth, *pHeight, *pFormat)];
    fread(pData, 1, *pSize, f);
    unsigned char* p = pData;
    int nSize = *pSize;
    int nW = *pWidth;
    int nH = *pHeight;
    for (;;) {
        nW >>= 1;
        nH >>= 1;
        if (!nW && !nH)
            break;
        if (!nW)
            nW = 1;
        if (!nH)
            nH = 1;
        p += nSize;
        nSize = ((nH + 3) / 4) * ((nW + 3) / 4) * nBlockSize;
        fread(p, 1, nSize, f);
    }
    return pData;
}

// FUNCTION: SWKOTOR 0x0045F4A0 SYMBOL: ?ImageReadS3TC@@YAPAEPADPAH111PAM@Z
unsigned char* ImageReadS3TC(char* szName, int* pWidth, int* pHeight, int* pFormat, int* pSize,
                             float* pAlphaMean)
{
    if (!szName || !*szName)
        return 0;
    char szPath[256];
    sprintf(szPath, "%s", szName);
    FILE* f = fopen(szPath, "rb");
    if (!f) {
        sprintf(szPath, "%s.dds", szName);
        f = fopen(szPath, "rb");
        if (!f)
            return 0;
    }
    unsigned char* pData = ImageReadS3TC(f, pWidth, pHeight, pFormat, pSize, pAlphaMean);
    fclose(f);
    return pData;
}
