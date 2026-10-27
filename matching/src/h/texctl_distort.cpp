// TextureController::DistortImage (0x00460D70): offsets each pixel of the
// distortion copy by an amount read from a height map.

#include "h/texctl.h"

float CosLookup(float fAngle);              // 0x004AADF0
float SinLookup(float fAngle);              // 0x004AAE30

extern unsigned char g_aDistortImage[];     // 0x007BB870

static inline void FloatToInt(int* pOut, float f)
{
    __asm {
        fld f
        mov edx, pOut
        frndint
        fistp dword ptr [edx]
    }
}

// NONMATCH (42 insns): same statements; the original re-initialises the
// row pointer and index per branch and keeps the column in memory.
// FUNCTION: SWKOTOR 0x00460D70
unsigned char* TextureController::DistortImage(unsigned char* pHeight)
{
    int nBpp = m_pTexture->GetBytesPerPixel();
    int nWidth = m_pTexture->GetWidth();
    int nHeight = m_pTexture->GetHeight();
    int y, x, dx, dy;
    int nRow = 0;
    unsigned char* pRow = g_aDistortImage;
    if (m_nDistortAngle) {
        for (y = 0; y < nHeight; y++) {
            unsigned char* pDst = pRow;
            for (x = 0; x < nWidth; x++) {
                float fAngle = pHeight[nRow + x] * 0.024543693f;
                FloatToInt(&dx, CosLookup(fAngle) * m_fDistortionAmplitude - 0.5f);
                FloatToInt(&dy, SinLookup(fAngle) * m_fDistortionAmplitude - 0.5f);
                int nOffset = ((y + nHeight + dx) & (nHeight - 1)) * nWidth + ((x + dy + nWidth) & (nWidth - 1));
                memcpy(pDst, m_pDistortData + nOffset * nBpp, nBpp);
                pDst += nBpp;
            }
            nRow += nWidth;
            pRow += nWidth * nBpp;
        }
        return g_aDistortImage;
    }
    for (y = 0; y < nHeight; y++) {
        unsigned char* pDst = pRow;
        for (x = 0; x < nWidth; x++) {
            float f = (pHeight[nRow + x] - 0x7f) * 0.0078125f;
            FloatToInt(&dx, f * m_fDistortionAmplitude - 0.5f);
            FloatToInt(&dy, f * m_fDistortionAmplitude - 0.5f);
            int nOffset = ((y + dx) & (nHeight - 1)) * nWidth + ((dy + x) & (nWidth - 1));
            memcpy(pDst, m_pDistortData + nOffset * nBpp, nBpp);
            pDst += nBpp;
        }
        nRow += nWidth;
        pRow += nWidth * nBpp;
    }
    return g_aDistortImage;
}
