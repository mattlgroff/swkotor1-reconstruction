// TextureController image copy, the Perlin noise controller
// (0x0045FD70 .. 0x0045FF40) and the random noise controller update.

#include <math.h>
#include <stdlib.h>

#include "h/texctl.h"

unsigned char* ImageBuildPerlin(int nSize, float fAmplitude);        // 0x0045E6B0
unsigned char ImageGetPerlinPixel(unsigned char** pLevels, int nRow, int nCol,
                                  int nBase, int nLevels, int nSize);   // 0x0045E710, descriptive

// vtable 0x0073ED40 (Update 0x004615A0, ParseField 0x00463310).
class PerlinTextureController : public TextureController {
public:
    virtual void Update();                          // 0x004615A0
    virtual void Init();                            // 0x0045FDD0
    virtual void BuildPerlinImage(unsigned char* pImage, int nSize); // +0x1c

    unsigned char* m_pNoise;                // +0x38
    unsigned char* m_pGradient;             // +0x3c
    float m_fBlend;                         // +0x40
};

// Copies the owner texture's pixels into the distortion buffer.
// FUNCTION: SWKOTOR 0x0045FD70
void TextureController::Distort()
{
    int nSize = GetImageSize(m_pTexture);
    m_pDistortData = new unsigned char[nSize];
    memcpy(m_pDistortData, m_pTexture->GetData(), nSize);
}

// NONMATCH (1 insn): the original loads m_pTexture before the m_fBlend
// store (same scheduling as CycleTIDTextureController::Update).
// FUNCTION: SWKOTOR 0x0045FDD0
void PerlinTextureController::Init()
{
    if (m_bDistort)
        Distort();
    m_fBlend = 0.0f;
    int nSize = GetPixelCount(m_pTexture);
    m_pNoise = new unsigned char[nSize];
    m_pGradient = new unsigned char[nSize];
    BuildPerlinImage(m_pNoise, m_pTexture->GetWidth());
    BuildPerlinImage(m_pGradient, m_pTexture->GetWidth());
}

// FUNCTION: SWKOTOR 0x0045FE50
void PerlinTextureController::BuildPerlinImage(unsigned char* pImage, int nSize)
{
    int nLevelSize = 4;
    float fAmplitude = 1.0f;
    int nLevels = 1;
    if (nSize >= 8) {
        do
            nLevels++;
        while ((4 << nLevels) <= nSize);
    }
    unsigned char** pLevels = new unsigned char*[nLevels];
    int i;
    for (i = 0; i < nLevels; i++, nLevelSize *= 2) {
        pLevels[i] = ImageBuildPerlin(nLevelSize, fAmplitude);
        fAmplitude *= 0.5f;
    }
    for (int r = 0; r < nSize; r++) {
        unsigned char* pRow = pImage + r * nSize;
        for (int c = 0; c < nSize; c++)
            pRow[c] = ImageGetPerlinPixel(pLevels, r, c, 4, nLevels, nSize);
    }
    for (i = 0; i < nLevels; i++)
        delete[] pLevels[i];
    delete[] pLevels;
}

extern int g_nAurFrameCounter;          // 0x007A46F4

// FUNCTION: SWKOTOR 0x00462800
void RandomTextureController::Update()
{
    if (m_nFrame == g_nAurFrameCounter)
        return;
    m_nFrame = g_nAurFrameCounter;
    if (!m_p3c)
        return;
    int nWidth = m_pTexture->GetWidth();
    int nHeight = m_pTexture->GetHeight();
    for (int y = 0; y < nHeight; y++) {
        for (int x = 0; x < nWidth; x++)
            m_p38[y * nWidth + x] = (unsigned char)(rand() % 256);
    }
    if (m_bDistort) {
        unsigned char* pImage = DistortImage(m_p38);
        m_pTexture->UpdateImage(1, pImage);
    } else {
        m_p3c = Height2Channel(m_p38, m_p3c);
        m_pTexture->UpdateImage(1, m_p3c);
    }
}

// FUNCTION: SWKOTOR 0x00460000
void WaveTextureController::Init()
{
    int nWidth = m_pTexture->GetWidth();
    int nHeight = m_pTexture->GetHeight();
    int nSize = nHeight * nWidth;
    if (m_bDistort)
        Distort();
    m_p38 = new float[nSize];
    m_p3c = new float[nSize];
    unsigned char* pData = m_pTexture->GetData();
    for (int y = 0; y < nHeight; y++) {
        for (int x = 0; x < nWidth; x++) {
            int i = y * nWidth + x;
            m_p3c[i] = 127.0f;
            if (pData)
                m_p38[i] = pData[m_pTexture->GetBytesPerPixel() * i];
            else
                m_p38[i] = 127.0f;
            m_p38[i] = (float)(rand() % 256);
        }
    }
}

// FUNCTION: SWKOTOR 0x00460120
void LifeTextureController::Init()
{
    unsigned char pixel[4];
    pixel[0] = 0;
    pixel[1] = 0;
    pixel[2] = 0;
    pixel[3] = 0xff;
    int nWidth = m_pTexture->GetWidth();
    int nHeight = m_pTexture->GetHeight();
    int nBpp = m_pTexture->GetBytesPerPixel();
    unsigned char* pImage = m_pTexture->GetData();
    m_pCells = new unsigned char[nBpp * nHeight * nWidth];
    for (int y = 0; y < nHeight; y++) {
        for (int x = 0; x < nWidth; x++) {
            pixel[0] = 0;
            pixel[1] = 0;
            pixel[2] = 0;
            pixel[3] = 0xff;
            if (rand() % 100 > 60)
                pixel[0] = 0xff;
            int i = (y * nWidth + x) * nBpp;
            memcpy(pImage + i, pixel, nBpp);
            memcpy(m_pCells + i, pixel, nBpp);
        }
    }
}

// NONMATCH: the original loads the neighbour count into eax on both paths
// of the survival test, which shifts the following register choices; the
// neighbour index must be one expression or cl hoists y + height.
// FUNCTION: SWKOTOR 0x00460250
void LifeTextureController::Update()
{
    if (m_nFrame > g_nAurFrameCounter - 5)
        return;
    m_nFrame = g_nAurFrameCounter;
    unsigned char pixel[4];
    pixel[0] = 0;
    pixel[1] = 0;
    pixel[2] = 0;
    pixel[3] = 0xff;
    int nWidth = m_pTexture->GetWidth();
    int nHeight = m_pTexture->GetHeight();
    int nBpp = m_pTexture->GetBytesPerPixel();
    int nSize = nBpp * nHeight * nWidth;
    unsigned char* pOld = new unsigned char[nSize];
    memcpy(pOld, m_pCells, nSize);
    for (int y = 0; y < nHeight; y++) {
        for (int x = 0; x < nWidth; x++) {
            int nCount = 0;
            int nAlive = 0;
            int dy;
            for (dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    int i = ((x + dx + nWidth) % nWidth + (y + dy + nHeight) % nHeight * nWidth) * nBpp;
                    if (dx == 0 && dy == 0)
                        nAlive = pOld[i];
                    else if (pOld[i] > 0)
                        nCount++;
                }
            }
            int nOffset = (y * nWidth + x) * nBpp;
            if ((nAlive && nCount == 2) || nCount == 3) {
                int v = 0;
                if (nBpp > 2) {
                    v = (m_pCells + nOffset)[dy] + 4;
                    if (v > 0xff)
                        v = 0xff;
                }
                pixel[0] = 0xff;
                pixel[1] = (unsigned char)v;
                pixel[2] = (unsigned char)v;
            } else {
                unsigned char* pCell = nOffset + m_pCells;
                pixel[0] = 0;
                pixel[1] = 0;
                if (*pCell != 0)
                    pixel[2] = 0x80;
                else if (nBpp > 2) {
                    int v = pCell[2];
                    if (v - 4 < 0)
                        pixel[2] = 0;
                    else
                        pixel[2] = v - 4;
                }
            }
            memcpy(m_pCells + nOffset, pixel, nBpp);
        }
    }
    unsigned char* pImage = m_pTexture->GetData();
    pImage = new unsigned char[nSize];
    memcpy(pImage, m_pCells, nSize);
    m_pTexture->UpdateImage(1, 0);
    m_pTexture->FreeImage();
    delete[] pOld;
}

float FUN_004aadf0(float f);                    // 0x004AADF0 (table cosine)
extern float g_fFrameTime;                      // 0x0078E574
extern float g_fRingAmplitude;                  // 0x0078E560, descriptive
extern float g_fRingFrequency;                  // 0x0078E564, descriptive
extern float g_fRingPhaseScale;                 // 0x0078E568, descriptive
extern float g_fRingPhaseSpeed;                 // 0x0078E56C, descriptive

static inline float Clamp(float v, float fMin, float fMax)
{
    if (!(fMax < v) && fMin > v)
        return fMin;
    if (fMax < v)
        return fMax;
    return v;
}

// NONMATCH: the original compares the lower bound against a duplicated
// st(0) (`fld st(0)` ... `fstp st(0)`); the rest of the pass matches.
// FUNCTION: SWKOTOR 0x00460A90
void RingDistortTextureController::Update()
{
    if (m_nFrame == g_nAurFrameCounter)
        return;
    if (!m_p38)
        return;
    m_nFrame = g_nAurFrameCounter;
    int nWidth = m_pTexture->GetWidth();
    int nHeight = m_pTexture->GetHeight();
    m_pTexture->GetBytesPerPixel();
    float fInv = 1.0f / nWidth;
    int i = 0;
    for (int y = 0; y < nHeight; y++) {
        float fy = (y + 1.0f) * fInv;
        for (int x = 0; x < nWidth; x++) {
            float fDy = fy - 0.5f;
            float fDy2 = fDy * fDy;
            float fx = (x + 1.0f) * fInv - 0.5f;
            float fRadius = (float)sqrt(fx * fx + fDy2);
            float a = FUN_004aadf0(g_fRingPhaseScale * m_fPhase + g_fRingFrequency * fRadius)
                * g_fRingAmplitude + 0.5f;
            float b = FUN_004aadf0(g_fRingPhaseScale * m_fPhase + g_fRingFrequency * fRadius)
                * g_fRingAmplitude + 0.5f;
            float va = Clamp(a * 255.0f, 0.0f, 255.0f);
            float vb = Clamp(b * 255.0f, 0.0f, 255.0f);
            m_p38[i + 2] = 0xff;
            m_p38[i + 1] = (unsigned char)vb;
            m_p38[i] = (unsigned char)va;
            m_p38[i + 3] = 0xff;
            i += 4;
        }
    }
    m_pTexture->UpdateImage(1, m_p38);
    m_fPhase = g_fFrameTime * g_fRingPhaseSpeed + m_fPhase;
    while (m_fPhase > 6.2831855f)
        m_fPhase -= 6.2831855f;
}

// NONMATCH: prologue register choice only; the original multiplies the
// texture size straight after GetHeight and keeps the grid size in edi.
// FUNCTION: SWKOTOR 0x00460560
void ArturoTextureController::Init()
{
    int nWidth = m_pTexture->GetWidth();
    int nPixels = m_pTexture->GetHeight() * nWidth;
    int nCells = m_nArturoHeight * m_nArturoWidth;
    m_fTime = 0.0f;
    if (m_bDistort)
        Distort();
    m_p44 = new float[nCells];
    m_p48 = new float[nCells];
    m_p40 = new float[nCells];
    m_p50 = (float*)new unsigned char[nPixels];
    for (int y = 0; y < m_nArturoHeight; y++) {
        for (int x = 0; x < m_nArturoWidth; x++) {
            float fx = (float)x / m_nArturoWidth * 2.0f - 1.0f;
            float fy = (float)y / m_nArturoHeight * 2.0f - 1.0f;
            m_p48[m_nArturoWidth * y + x] = 0.0f;
            float fRadius = (float)sqrt(fy * fy + fx * fx);
            m_p44[m_nArturoWidth * y + x] = (float)sin((1.0f - fRadius) * 3.1415927f) * 127.0f;
        }
    }
    BuildArturoImage();
    BuildArturoImage();
}

// NONMATCH (2 insns): the grid size product copies the height register
// (original: the width register) before the imul.
// FUNCTION: SWKOTOR 0x0045FF40
void WaterTextureController::Init()
{
    int nWidth = m_nWaterWidth;
    int nHeight = m_nWaterHeight;
    int nSize = nWidth * nHeight;
    m_nCounter = 0;
    if (m_bDistort)
        Distort();
    m_p48 = new float[nSize];
    m_p40 = new float[nSize];
    m_p44 = new float[nSize];
    for (int y = 0; y < nHeight; y++) {
        for (int x = 0; x < nWidth; x++) {
            int i = y * nWidth + x;
            m_p48[i] = 0.0f;
            m_p40[i] = 127.0f;
        }
    }
    int nTexWidth = m_pTexture->GetWidth();
    int nTexHeight = m_pTexture->GetHeight();
    m_p5c = new unsigned char[nTexHeight * nTexWidth];
    m_p60 = new unsigned char[nTexHeight * nTexWidth];
    m_fAnglePhase = m_fForcePhase = 0.0f;
}
