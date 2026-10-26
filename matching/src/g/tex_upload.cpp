// GLRender texture upload helpers called by CAurTextureBasic::glImage
// (0x00420970): 2D and cube-map textures, raw and processed (pre-built
// mipmap chains, S3TC). Behavioural reference:
// src/units/0549/066b636c54-upload.cpp.
#include "g/aur_texture.h"

extern "C" void* __cdecl memcpy(void* dst, const void* src, unsigned int n);
#pragma intrinsic(memcpy)

extern "C" {
__declspec(dllimport) void __stdcall glTexImage2D(unsigned int target, int level,
    int internalformat, int width, int height, int border, unsigned int format,
    unsigned int type, const void* pixels);
__declspec(dllimport) void __stdcall glTexSubImage2D(unsigned int target, int level,
    int xoffset, int yoffset, int width, int height, unsigned int format,
    unsigned int type, const void* pixels);
int __stdcall gluBuild2DMipmaps(unsigned int target, int components, int width,
    int height, unsigned int format, unsigned int type, const void* data);
}

#define GL_TEXTURE_2D 0x0de1
#define GL_UNSIGNED_BYTE 0x1401
#define GL_UNSIGNED_SHORT_4_4_4_4 0x8033
#define GL_RGB 0x1907
#define GL_RGBA 0x1908
#define GL_LUMINANCE 0x1909
#define GL_TEXTURE_CUBE_MAP 0x8513
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X 0x8515
#define GL_CLAMP_TO_EDGE 0x812f
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_TEXTURE_WRAP_R 0x8072
#define GL_NEAREST 0x2600
#define GL_CLAMP 0x2900
#define GL_REPEAT 0x2901
#define GL_GENERATE_MIPMAP_SGIS 0x8191
#define GL_NEAREST_MIPMAP_NEAREST 0x2700

extern int g_aCubeFaceOrder[6];         // 0x0078db50
extern int g_aCubeFaceRotation[6];      // 0x0078db38

unsigned char* ImageGetSubImage(unsigned char* pImage, int nWidth, int nHeight,
                                int nBpp, int x, int y, int w, int h);   // 0x0045f000
unsigned char* ImageScale(unsigned char* pImage, int nWidth, int nHeight, int nBpp,
                          int nNewWidth, int nNewHeight);                  // 0x0045dad0
unsigned char* ImageRotate(unsigned char* pImage, int w, int h, int nBpp,
                           int nRotation);                               // 0x0041fbd0

typedef void (__stdcall* PFNCOMPRESSEDTEXIMAGE2D)(unsigned int target, int level,
    unsigned int internalformat, int width, int height, int border, int imageSize,
    const void* data);
extern PFNCOMPRESSEDTEXIMAGE2D g_pglCompressedTexImage2DARB;   // 0x007bb774
extern unsigned int g_aGLCompressedFormats[];                  // 0x0073f36c

int AurBumpMappingAvailable();                                           // 0x0045fb10
int AurSGIMipmapAvailable();                                             // 0x0045f910
int BuildTextureNormalMap(void* pImage, float fScale, int* pWidth, int* pHeight,
                          int nBpp, int c, unsigned char bNormalMap);    // 0x00420ea0
void AurBuild2DMipMaps(int w, int h, int nBpp, unsigned char* pImage, int bUpdate);
int GetCompressedContactSheetOffset(int nCount, int nSize, int nFormat);  // 0x00424d70

// FUNCTION: SWKOTOR 0x00423f90
void CreateCubeTexture(CAurTextureBasic* t, int bUpdate, int nIndex)
{
    for (int nFace = 0; nFace < 6; nFace++) {
        int nWidth = t->GetWidth();
        int nHeight = t->GetHeight();
        int nBpp = t->GetBytesPerPixel();
        int nFaceHeight = nHeight / 6;
        unsigned int nTarget = g_aCubeFaceOrder[nFace] + GL_TEXTURE_CUBE_MAP_POSITIVE_X;
        unsigned char* pFace = (unsigned char*)t->GetImage()
                               + nFaceHeight * nBpp * nWidth * nFace;
        int w = nWidth / t->GetNumX();
        int h = nFaceHeight / t->GetNumY();
        int x = (nIndex % t->GetNumX()) * w;
        int y = (nIndex / t->GetNumX()) * h;
        unsigned char* pBuffer;
        if (t->GetNumX() == 1 && t->GetNumY() == 1)
            pBuffer = pFace;
        else
            pBuffer = ImageGetSubImage(pFace, nWidth, nFaceHeight, nBpp, x, y, w, h);
        unsigned char* pRotated = ImageRotate(pBuffer, w, h, t->GetBytesPerPixel(),
                                              g_aCubeFaceRotation[nFace]);
        int nComponents = t->GetBytesPerPixel();
        unsigned int nFormat;
        if (nComponents == 4)
            nFormat = GL_RGBA;
        else if (nComponents == 3)
            nFormat = GL_RGB;
        else
            nFormat = (nComponents == 2) + GL_LUMINANCE;
        if (t->GetMipMap())
            gluBuild2DMipmaps(nTarget, nComponents, w, h, nFormat, GL_UNSIGNED_BYTE, pRotated);
        else if ((unsigned char)bUpdate)
            glTexSubImage2D(nTarget, 0, 0, 0, w, h, nFormat, GL_UNSIGNED_BYTE, pRotated);
        else
            glTexImage2D(nTarget, 0, nComponents, w, h, 0, nFormat, GL_UNSIGNED_BYTE, pRotated);
        if (pRotated != pBuffer)
            delete pRotated;
        if (pBuffer != pFace)
            delete pBuffer;
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, (t->GetFilter() != 0) + GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,
                    t->GetMipMap() ? (t->GetFilter() ? 3 : 0) + GL_NEAREST_MIPMAP_NEAREST
                                   : (t->GetFilter() != 0) + GL_NEAREST);
}

// FUNCTION: SWKOTOR 0x00424230
void CreateProcessedCubeTexture(CAurTextureBasic* t, int bUpdate, int nIndex)
{
    int nWidth = t->GetWidth();
    int nHeight = t->GetHeight() / 6;
    int nBpp = t->GetBytesPerPixel();
    int bMipMap = t->GetMipMap();
    unsigned char* pData = (unsigned char*)t->GetImage();
    unsigned int nTarget = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
    for (int nFace = 0; nFace < 6; nFace++, nTarget++) {
        if (t->GetCompressedSize() > 0) {
            int nLevel = 0;
            int nBlock = (nBpp == 4) ? 16 : 8;
            g_pglCompressedTexImage2DARB(nTarget, 0,
                g_aGLCompressedFormats[t->GetCompressionFormat()], nWidth, nHeight, 0,
                t->GetCompressedSize(), pData);
            if (bMipMap) {
                int nSize = t->GetCompressedSize();
                int w = nWidth;
                int h = nHeight;
                for (;;) {
                    w >>= 1;
                    h >>= 1;
                    if (w == 0) {
                        if (h == 0)
                            break;
                        w = 1;
                    }
                    if (h == 0)
                        h = 1;
                    nLevel++;
                    pData += nSize;
                    nSize = ((h + 3) / 4) * ((w + 3) / 4) * nBlock;
                    g_pglCompressedTexImage2DARB(nTarget, nLevel,
                        g_aGLCompressedFormats[t->GetCompressionFormat()], w, h, 0, nSize, pData);
                }
                pData += nSize;
            }
        } else {
            unsigned int nFormat = (nBpp == 4) + GL_RGB;
            if (bMipMap) {
                int nLevel = 0;
                if ((unsigned char)bUpdate)
                    glTexSubImage2D(nTarget, 0, 0, 0, nWidth, nHeight, nFormat, GL_UNSIGNED_BYTE, pData);
                else
                    glTexImage2D(nTarget, 0, nBpp, nWidth, nHeight, 0, nFormat, GL_UNSIGNED_BYTE, pData);
                int nSize = nBpp * nHeight * nWidth;
                int w = nWidth >> 1;
                int h = nHeight >> 1;
                for (int n = t->GetNumMipMaps() - 1; n > 0; n--) {
                    if (w == 0)
                        w = 1;
                    if (h == 0)
                        h = 1;
                    nLevel++;
                    pData += nSize;
                    nSize = h * w * nBpp;
                    if ((unsigned char)bUpdate)
                        glTexSubImage2D(nTarget, nLevel, 0, 0, w, h, nFormat, GL_UNSIGNED_BYTE, pData);
                    else
                        glTexImage2D(nTarget, nLevel, nBpp, w, h, 0, nFormat, GL_UNSIGNED_BYTE, pData);
                    w >>= 1;
                    h >>= 1;
                }
                pData += nSize;
            } else if ((unsigned char)bUpdate) {
                glTexSubImage2D(nTarget, 0, 0, 0, nWidth, nHeight, nFormat, GL_UNSIGNED_BYTE, pData);
            } else {
                glTexImage2D(nTarget, 0, nBpp, nWidth, nHeight, 0, nFormat, GL_UNSIGNED_BYTE, pData);
            }
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, (t->GetFilter() != 0) + GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,
                    t->GetMipMap() ? (t->GetFilter() ? 3 : 0) + GL_NEAREST_MIPMAP_NEAREST
                                   : (t->GetFilter() != 0) + GL_NEAREST);
}

// Builds the mipmap chain of a 2D texture by hand (box filter; 4444 for two
// bytes per pixel), squaring non-square images first.
// NONMATCH (143): the original keeps the per-column addresses unreduced
// (recomputed from the column each pass, frame 0x38); cl strength-reduces
// them into pointers. Tried running row offsets, pointer temporaries and
// y/x index expressions.
// FUNCTION: SWKOTOR 0x00424580
void AurBuild2DMipMaps(int w, int h, int nBpp, unsigned char* pImage, int bUpdate)
{
    int nWidth = w;
    int nHeight = h;
    unsigned char* pSrc;
    if (w != h) {
        int nSize = w > h ? w : h;
        pSrc = ImageScale(pImage, w, h, nBpp, nSize, nSize);
        nWidth = nSize;
        nHeight = nSize;
    } else {
        int nBytes = w * h * nBpp;
        pSrc = new unsigned char[nBytes];
        memcpy(pSrc, pImage, nBytes);
    }
    int nLevel = 0;
    unsigned int nType = nBpp == 2 ? GL_UNSIGNED_SHORT_4_4_4_4 : GL_UNSIGNED_BYTE;
    unsigned int nFormat;
    if (nBpp == 4)
        nFormat = GL_RGBA;
    else if (nBpp == 3)
        nFormat = GL_RGB;
    else
        nFormat = (nBpp != 2) + GL_RGBA;
    if (!(unsigned char)bUpdate)
        glTexImage2D(GL_TEXTURE_2D, 0, nBpp, nWidth, nHeight, 0, nFormat, nType, pSrc);
    else
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, nWidth, nHeight, nFormat, nType, pSrc);
    while (nHeight * nWidth > 1) {
        int nHalfHeight = nHeight / 2;
        int nHalfWidth = nWidth / 2;
        unsigned char* pDst = new unsigned char[nHalfWidth * nHalfHeight * nBpp];
        for (int y = 0; y < nHalfHeight; y++) {
            for (int x = 0; x < nHalfWidth; x++) {
                int a = (y * nWidth + x) * 2;
                int b = (y * 2 + 1) * nWidth + x * 2;
                int d = y * nHalfWidth + x;
                if (nBpp == 2) {
                    for (int i = 0; i < 2; i++) {
                        int s0 = pSrc[b * 2 + i + 2];
                        int s1 = pSrc[b * 2 + i];
                        int s2 = pSrc[a * 2 + i + 2];
                        int s3 = pSrc[a * 2 + i];
                        int nHigh = ((s0 >> 4) + (s1 >> 4) + (s2 >> 4) + (s3 >> 4)) / 4;
                        int nLow = ((s0 & 15) + (s1 & 15) + (s2 & 15) + (s3 & 15)) / 4;
                        pDst[d * 2 + i] = (unsigned char)(nHigh << 4) | (unsigned char)nLow;
                    }
                } else {
                    for (int c = 0; c < nBpp; c++)
                        pDst[d * nBpp + c] = (unsigned char)((pSrc[(a + 1) * nBpp + c]
                            + pSrc[b * nBpp + c] + pSrc[(b + 1) * nBpp + c] + pSrc[a * nBpp + c]) / 4);
                }
            }
        }
        delete[] pSrc;
        nWidth >>= 1;
        nHeight >>= 1;
        nLevel++;
        pSrc = pDst;
        if (!(unsigned char)bUpdate)
            glTexImage2D(GL_TEXTURE_2D, nLevel, nBpp, nWidth, nHeight, 0, nFormat, nType, pSrc);
        else
            glTexSubImage2D(GL_TEXTURE_2D, nLevel, 0, 0, nWidth, nHeight, nFormat, nType, pSrc);
    }
    delete[] pSrc;
}

// NONMATCH (10): only the S3TC level size swaps which half-size is formed
// first (`lea eax, [edi + 3]` height first in the original); operand order,
// level copies, a size helper and declaration order did not move it.
// FUNCTION: SWKOTOR 0x00424980
void CreateTexture(CAurTextureBasic* t, int bUpdate, int nIndex)
{
    int nWidth;
    int nHeight = t->GetHeight();
    int nBpp = t->GetBytesPerPixel();
    unsigned char* pImage = (unsigned char*)t->GetImage();
    int nNumX = t->GetNumX();
    int nNumY = t->GetNumY();
    nWidth = t->GetWidth();
    int w = nWidth / nNumX;
    int h = nHeight / nNumY;
    int x = (nIndex % nNumX) * w;
    int y = (nIndex / nNumX) * h;
    unsigned char* pBuffer;
    if (nNumX == 1 && nNumY == 1)
        pBuffer = pImage;
    else
        pBuffer = ImageGetSubImage(pImage, nWidth, nHeight, nBpp, x, y, w, h);
    if (t->IsLightmap()) {
        if (AurBumpMappingAvailable())
            BuildTextureNormalMap(pBuffer, t->GetBumpMapScaling(), &w, &h, nBpp, 0,
                                  t->IsNormalMap());
        if (pBuffer != pImage)
            delete pBuffer;
        return;
    }
    int bMipMap = t->GetMipMap();
    if (t->GetCompressedSize() > 0) {
        int nLevel = 0;
        int nBlock = (nBpp == 4) ? 16 : 8;
        unsigned char* pData = (unsigned char*)t->GetImage();
        g_pglCompressedTexImage2DARB(GL_TEXTURE_2D, 0,
            g_aGLCompressedFormats[t->GetCompressionFormat()], nWidth, nHeight, 0,
            t->GetCompressedSize(), t->GetImage());
        if (t->GetMipMap()) {
            int nSize = t->GetCompressedSize();
            for (;;) {
                nWidth >>= 1;
                nHeight >>= 1;
                if (nWidth == 0) {
                    if (nHeight == 0)
                        break;
                    nWidth = 1;
                }
                if (nHeight == 0)
                    nHeight = 1;
                nLevel++;
                pData += nSize;
                nSize = ((nHeight + 3) / 4) * ((nWidth + 3) / 4) * nBlock;
                g_pglCompressedTexImage2DARB(GL_TEXTURE_2D, nLevel,
                    g_aGLCompressedFormats[t->GetCompressionFormat()], nWidth, nHeight, 0,
                    nSize, pData);
            }
        }
    } else {
        unsigned int nFormat;
        if (nBpp == 4)
            nFormat = GL_RGBA;
        else if (nBpp == 3)
            nFormat = GL_RGB;
        else
            nFormat = (nBpp != 2) + GL_RGBA;
        unsigned int nType;
        int nInternal;
        if (nBpp == 2) {
            nType = GL_UNSIGNED_SHORT_4_4_4_4;
            nInternal = 4;
        } else {
            nType = GL_UNSIGNED_BYTE;
            nInternal = nBpp;
        }
        if (bMipMap) {
            if (AurSGIMipmapAvailable()) {
                glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, 1);
                glTexImage2D(GL_TEXTURE_2D, 0, nInternal, w, h, 0, nFormat, nType, pBuffer);
            } else if ((unsigned char)bUpdate || nBpp == 2) {
                AurBuild2DMipMaps(w, h, nBpp, pBuffer, bUpdate);
            } else {
                gluBuild2DMipmaps(GL_TEXTURE_2D, nInternal, w, h, nFormat, nType, pBuffer);
            }
        } else if ((unsigned char)bUpdate) {
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, nFormat, GL_UNSIGNED_BYTE, pBuffer);
        } else {
            glTexImage2D(GL_TEXTURE_2D, 0, nInternal, w, h, 0, nFormat, nType, pBuffer);
        }
    }
    unsigned int nClamp = t->GetClamp();
    int bFilter = t->GetFilter();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (nClamp & 1) ? GL_CLAMP : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (nClamp & 2) ? GL_CLAMP : GL_REPEAT);
    int nMag = (bFilter != 0) + GL_NEAREST;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, nMag);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    bMipMap ? (bFilter ? 3 : 0) + GL_NEAREST_MIPMAP_NEAREST : nMag);
    if (pBuffer != pImage)
        delete pBuffer;
}

// NONMATCH (113): register / stack-slot tie (the original keeps the mip
// flag in ebp and w/h in [esp+14h]/[esp+10h]); control flow and calls match.
// FUNCTION: SWKOTOR 0x00424dd0
void CreateProcessedTexture(CAurTextureBasic* t, int bUpdate, int nIndex)
{
    int w, h;
    int nHeight = t->GetHeight();
    int nBpp = t->GetBytesPerPixel();
    unsigned char* pImage = (unsigned char*)t->GetImage();
    int nNumX = t->GetNumX();
    int nNumY = t->GetNumY();
    w = t->GetWidth() / nNumX;
    h = nHeight / nNumY;
    unsigned char* pData;
    if (nNumX == 1 && nNumY == 1)
        pData = pImage;
    else
        pData = pImage + GetCompressedContactSheetOffset(nIndex, w, nBpp);
    int bMipMap = t->GetMipMap();
    if (t->IsLightmap() && nBpp == 1) {
        if (AurBumpMappingAvailable())
            BuildTextureNormalMap(pData, t->GetBumpMapScaling(), &w, &h, nBpp, 0,
                                  t->IsNormalMap());
        if (pData != pImage)
            delete pData;
        return;
    }
    if (t->GetCompressedSize() > 0) {
        int nLevel = 0;
        unsigned char* pCursor = pData;
        int nBlock = (nBpp == 4) ? 16 : 8;
        int nSize = ((h + 3) / 4) * ((w + 3) / 4) * nBlock;
        g_pglCompressedTexImage2DARB(GL_TEXTURE_2D, 0,
            g_aGLCompressedFormats[t->GetCompressionFormat()], w, h, 0, nSize, pData);
        if (t->GetMipMap()) {
            int lw = w;
            int lh = h;
            for (;;) {
                lw >>= 1;
                lh >>= 1;
                if (lw == 0) {
                    if (lh == 0)
                        break;
                    lw = 1;
                }
                if (lh == 0)
                    lh = 1;
                nLevel++;
                pCursor += nSize;
                nSize = ((lh + 3) / 4) * ((lw + 3) / 4) * nBlock;
                g_pglCompressedTexImage2DARB(GL_TEXTURE_2D, nLevel,
                    g_aGLCompressedFormats[t->GetCompressionFormat()], lw, lh, 0, nSize, pCursor);
            }
        }
    } else {
        unsigned int nFormat;
        if (nBpp == 4)
            nFormat = GL_RGBA;
        else if (nBpp == 3)
            nFormat = GL_RGB;
        else
            nFormat = (nBpp != 2) + GL_RGBA;
        unsigned int nType;
        int nInternal;
        if (nBpp == 2) {
            nType = GL_UNSIGNED_SHORT_4_4_4_4;
            nInternal = 4;
        } else {
            nType = GL_UNSIGNED_BYTE;
            nInternal = nBpp;
        }
        if (bMipMap) {
            int nLevel = 0;
            unsigned char* pPixels = (unsigned char*)t->GetImage();
            if ((unsigned char)bUpdate)
                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, nFormat, GL_UNSIGNED_BYTE, pPixels);
            else
                glTexImage2D(GL_TEXTURE_2D, 0, nInternal, w, h, 0, nFormat, nType, pPixels);
            if (t->GetMipMap()) {
                int lh = h;
                int lw = w;
                int nSize = lh * lw * nBpp;
                lw >>= 1;
                lh >>= 1;
                for (int n = t->GetNumMipMaps() - 1; n > 0; n--) {
                    if (lw == 0)
                        lw = 1;
                    if (lh == 0)
                        lh = 1;
                    nLevel++;
                    pPixels += nSize;
                    nSize = lh * lw * nBpp;
                    if ((unsigned char)bUpdate)
                        glTexSubImage2D(GL_TEXTURE_2D, nLevel, 0, 0, lw, lh, nFormat, nType, pPixels);
                    else
                        glTexImage2D(GL_TEXTURE_2D, nLevel, nInternal, lw, lh, 0, nFormat, nType, pPixels);
                    lw >>= 1;
                    lh >>= 1;
                }
            }
        } else if ((unsigned char)bUpdate) {
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, nFormat, GL_UNSIGNED_BYTE, pData);
        } else {
            glTexImage2D(GL_TEXTURE_2D, 0, nInternal, w, h, 0, nFormat, nType, pData);
        }
    }
    if (!t->IsLightmap()) {
        unsigned int nClamp = t->GetClamp();
        int bFilter = t->GetFilter();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (nClamp & 1) ? GL_CLAMP : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (nClamp & 2) ? GL_CLAMP : GL_REPEAT);
        int nMag = (bFilter != 0) + GL_NEAREST;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, nMag);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        bMipMap ? (bFilter ? 3 : 0) + GL_NEAREST_MIPMAP_NEAREST : nMag);
    }
}
