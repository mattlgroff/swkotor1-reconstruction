// image helpers between worker E's image.cpp entries
// (0x0045DA80 .. 0x0045E320).

#include <stdio.h>
#include <string.h>

struct TGAFileHeader {
    unsigned char nIDLength;
    unsigned char nColorMapType;
    unsigned char nImageType;
    unsigned char aColorMapSpec[5];
    unsigned short nXOrigin;
    unsigned short nYOrigin;
    unsigned short nWidth;
    unsigned short nHeight;
    unsigned char nPixelDepth;
    unsigned char nDescriptor;
};

// FUNCTION: SWKOTOR 0x0045DA80
void ImageSwapRB(unsigned char* pData, int nWidth, int nHeight, int nBpp)
{
    if (nBpp < 3)
        return;
    int nSize = nWidth * nHeight * nBpp;
    for (int i = 0; i < nSize; i += nBpp) {
        unsigned char c = pData[i];
        pData[i] = pData[i + 2];
        pData[i + 2] = c;
    }
}

// FUNCTION: SWKOTOR 0x0045E270
int ImageGetS3TCSize(int nWidth, int nHeight, int nFormat)
{
    int nSize = 0;
    int nBlockSize = nFormat == 4 ? 16 : 8;
    while (nWidth || nHeight) {
        if (!nWidth)
            nWidth = 1;
        if (!nHeight)
            nHeight = 1;
        nSize += (nWidth + 3) / 4 * ((nHeight + 3) / 4) * nBlockSize;
        nWidth >>= 1;
        nHeight >>= 1;
    }
    return nSize;
}

// FUNCTION: SWKOTOR 0x0045E2E0
void ImageReadTGAHeader(FILE* f, int* pWidth, int* pHeight, int* pBpp)
{
    TGAFileHeader header;
    fread(&header, sizeof(header), 1, f);
    *pWidth = header.nWidth;
    *pHeight = header.nHeight;
    *pBpp = header.nPixelDepth >> 3;
}

// FUNCTION: SWKOTOR 0x0045E320
unsigned char* ImageReadTGAImage(FILE* f, int nWidth, int nHeight, int nBpp)
{
    int nSize = nWidth * nHeight * nBpp;
    unsigned char* pData = new unsigned char[nSize];
    fread(pData, 1, nSize, f);
    return pData;
}

int ImageReadS3TCHeader(FILE* f, int* pWidth, int* pHeight, int* pFormat, int* pMipMaps,
                        float* pAlphaMean);                             // 0x0045E0F0

// Reads only the header of a 16-bit (.4pc) image.
// FUNCTION: SWKOTOR 0x0045E010
int ImageRead16bitHeader(char* szName, int* pWidth, int* pHeight, int* pBpp, int* pFormat)
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
    fclose(f);
    return 1;
}

// FUNCTION: SWKOTOR 0x0045E180
int ImageReadS3TCHeader(char* szName, int* pWidth, int* pHeight, int* pFormat, int* pMipMaps,
                        float* pAlphaMean)
{
    if (!szName || !*szName)
        return 0;
    *pMipMaps = 0;
    *pWidth = 0;
    *pHeight = 0;
    *pFormat = 0;
    char szPath[256];
    sprintf(szPath, "%s", szName);
    FILE* f = fopen(szPath, "rb");
    if (!f) {
        sprintf(szPath, "%s.dds", szName);
        f = fopen(szPath, "rb");
        if (!f)
            return 0;
    }
    ImageReadS3TCHeader(f, pWidth, pHeight, pFormat, pMipMaps, pAlphaMean);
    fclose(f);
    return 1;
}

// FUNCTION: SWKOTOR 0x0045E440
int ImageReadHeader(char* szName, int* pWidth, int* pHeight, int* pBpp)
{
    if (!szName || !*szName)
        return 0;
    *pWidth = 0;
    *pHeight = 0;
    *pBpp = 0;
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
    fclose(f);
    return 1;
}
