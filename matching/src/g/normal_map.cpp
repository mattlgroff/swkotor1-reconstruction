// Normal-map textures: height field conversion, box-filtered normal mipmaps
// and the light attenuation lookup texture. Behavioural reference:
// src/units/0874/24f0969a61-normal_map.cpp,
// src/units/0790/e4997cee66-renderer_gl_loader_lowers.cpp.
#include "g/aur_texture.h"

extern "C" {
__declspec(dllimport) void __stdcall glTexImage2D(unsigned int target, int level,
    int internalformat, int width, int height, int border, unsigned int format,
    unsigned int type, const void* pixels);
__declspec(dllimport) void __stdcall glTexSubImage2D(unsigned int target, int level,
    int xoffset, int yoffset, int width, int height, unsigned int format,
    unsigned int type, const void* pixels);
__declspec(dllimport) void __stdcall glGenTextures(int n, unsigned int* textures);
void* __cdecl memcpy(void* dst, const void* src, unsigned int n);
}
#pragma intrinsic(memcpy)

#define GL_TEXTURE_2D 0x0de1
#define GL_UNSIGNED_BYTE 0x1401
#define GL_RGBA 0x1908
#define GL_RGBA8 0x8058
#define GL_INTENSITY8 0x8051
#define GL_CLAMP_TO_EDGE 0x812f
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_BGRA_EXT 0x80e1
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_LINEAR 0x2601
#define GL_LINEAR_MIPMAP_LINEAR 0x2703

unsigned int g_nAttenuationTexture;   // 0x007a68a0

unsigned char* ConvertHeightFieldToNormalMap(void* pData, int nWidth, int nHeight,
    int nMapWidth, int nMapHeight, float fScale);                       // 0x0041fec0

#include <math.h>

// Halves a normal map: alpha-weighted average of each 2x2 block of unpacked
// normals (wrapping at the right/bottom edge), renormalised, with the
// length kept in alpha. Frees the source.
// NONMATCH (166): x87 scheduling; the original spills the four alpha
// weights to the frame while cl keeps them on the FPU stack (a float[4]
// array is scalarised the same way).
// FUNCTION: SWKOTOR 0x00420030
unsigned char* DownSampleNormalMap(unsigned char* pData, int nWidth, int nHeight,
                                   int nNewWidth, int nNewHeight)
{
    unsigned char* pOut = new unsigned char[nNewWidth * nNewHeight * 4];
    for (int y = 0; y < nHeight; y += 2) {
        for (int x = 0; x < nWidth; x += 2) {
            int nRight = (x + 1) % nWidth;
            int nNext = (y + 1) % nWidth;
            unsigned char* p00 = &pData[(y * nWidth + x) * 4];
            unsigned char* p10 = &pData[(y * nWidth + nRight) * 4];
            unsigned char* p01 = &pData[(nNext * nWidth + x) * 4];
            unsigned char* p11 = &pData[(nNext * nWidth + nRight) * 4];
            float a00 = p00[3] * (1.0f / 255.0f);
            float a10 = p10[3] * (1.0f / 255.0f);
            float a01 = p01[3] * (1.0f / 255.0f);
            float a11 = p11[3] * (1.0f / 255.0f);
            float z = (p10[2] * (2.0f / 255.0f) - 1.0f) * a10
                    + (p00[2] * (2.0f / 255.0f) - 1.0f) * a00
                    + (p01[2] * (2.0f / 255.0f) - 1.0f) * a01
                    + (p11[2] * (2.0f / 255.0f) - 1.0f) * a11;
            float ny = (p00[1] * (2.0f / 255.0f) - 1.0f) * a00
                     + (p10[1] * (2.0f / 255.0f) - 1.0f) * a10
                     + (p01[1] * (2.0f / 255.0f) - 1.0f) * a01
                     + (p11[1] * (2.0f / 255.0f) - 1.0f) * a11;
            float nx = (p00[0] * (2.0f / 255.0f) - 1.0f) * a00
                     + (p10[0] * (2.0f / 255.0f) - 1.0f) * a10
                     + (p01[0] * (2.0f / 255.0f) - 1.0f) * a01
                     + (p11[0] * (2.0f / 255.0f) - 1.0f) * a11;
            if (nNewWidth == 1 || nNewHeight == 1 || (nNewWidth == 2 && nNewHeight == 2)) {
                z = 0.0f;
                ny = 0.0f;
                nx = 1.0f;
            } else if (nNewWidth == 4 && nNewHeight == 4) {
                z *= 0.5f;
                ny *= 0.5f;
            }
            float fLength = (float)sqrt(nx * nx + ny * ny + z * z);
            if (fLength == 0.0) {
                z = 0.0f;
                ny = 0.0f;
                nx = 1.0f;
            } else {
                float fInv = 1.0f / fLength;
                z *= fInv;
                ny = fInv * ny;
                nx = fInv * nx;
            }
            unsigned char* pDst = &pOut[((y >> 1) * nNewWidth + (x >> 1)) * 4];
            pDst[2] = (unsigned char)(z * 127.0f + 128.0f);
            pDst[1] = (unsigned char)(ny * 127.0f + 128.0f);
            pDst[0] = (unsigned char)(nx * 127.0f + 128.0f);
            float fAlpha = fLength * 0.25f;
            if (fAlpha > 1.0)
                pDst[3] = 0xff;
            else
                pDst[3] = (unsigned char)(fAlpha * 255.0f);
        }
    }
    delete pData;
    return pOut;
}

// Uploads a normal map (converted from a height field unless it already is
// one) with its whole mipmap chain.
// FUNCTION: SWKOTOR 0x004260a0
void BuildNormalMap(void* pData, int nWidth, int nHeight, int nMapWidth,
                    int nMapHeight, float fScale, int bSubImage, unsigned char bNormalMap)
{
    unsigned char* pBuffer;
    if (bNormalMap) {
        int nBytes = nWidth * nHeight * 4;
        pBuffer = new unsigned char[nBytes];
        memcpy(pBuffer, pData, nBytes);
    } else {
        pBuffer = ConvertHeightFieldToNormalMap(pData, nWidth, nHeight, nMapWidth,
                                                nMapHeight, fScale);
    }
    int nLevel = 0;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight, 0, GL_BGRA_EXT,
                 GL_UNSIGNED_BYTE, pBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, g_nAnisotropyRequested);
    while (nWidth > 1 || nHeight > 1) {
        nLevel++;
        int w = nWidth >> 1;
        int h = nHeight >> 1;
        if (w == 0)
            w = 1;
        if (h == 0)
            h = 1;
        pBuffer = DownSampleNormalMap(pBuffer, nWidth, nHeight, w, h);
        if (bSubImage) {
            glTexSubImage2D(GL_TEXTURE_2D, nLevel, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pBuffer);
        } else {
            glTexImage2D(GL_TEXTURE_2D, nLevel, GL_RGBA8, w, h, 0, GL_BGRA_EXT,
                         GL_UNSIGNED_BYTE, pBuffer);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, g_nAnisotropyRequested);
        }
        nWidth = w;
        nHeight = h;
    }
    delete[] pBuffer;
}

// Binds a 2D texture on the active unit and records it.
static inline void BindTexture2D(unsigned int nTexture)
{
    glBindTexture(g_nGLTexture2D, nTexture);
    glTexParameteri(g_nGLTexture2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, g_nAnisotropyRequested);
    g_aBoundTextures[g_nActiveTextureUnit] = nTexture;
}

// 64x64 radial falloff 1 / (1 + 4.5 d^2) used for the light attenuation
// lookup; the border texels are black.
// FUNCTION: SWKOTOR 0x004263c0
void InitAttenuationTexture()
{
    unsigned char aPixels[64][64][4];
    for (int r = 0; r < 64; r++) {
        float x = (r - 32) * 0.125f;
        float x2 = x * x;
        for (int c = 0; c < 64; c++) {
            float y = (c - 32) * 0.125f;
            float v = 1.0f / ((y * y + x2) * 4.5f + 1.0f);
            if (c == 0 || r == 0 || c == 63 || r == 63)
                v = 0;
            unsigned char b = (unsigned char)(v * 255.0f);
            aPixels[r][c][0] = b;
            aPixels[r][c][1] = b;
            aPixels[r][c][2] = b;
            aPixels[r][c][3] = 0xff;
        }
    }
    glGenTextures(1, &g_nAttenuationTexture);
    BindTexture2D(g_nAttenuationTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_INTENSITY8, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, aPixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, g_nAnisotropyRequested);
}
