// CSWSAreaMap (0x00578C30-0x005792D0): explored-area bitmap, map pixel and
// grid conversions, party member map positions.
#include "b/sws_area_map.h"
#include <stdlib.h>
#include <string.h>

// FUNCTION: SWKOTOR 0x00578BE0
CSWSAreaMap::CSWSAreaMap()
{
    m_pBits = NULL;
    m_fScaleX = 0.0f;
    m_fScaleY = 0.0f;
    m_fOriginX = 0.0f;
    m_fOriginY = 0.0f;
    m_nWords = 0;
    m_nGridWidth = 0;
    m_nGridHeight = 0;
    for (int i = 0; i < 3; i++) {
        m_nPartyMapX[i] = -1;
        m_nPartyMapY[i] = -1;
        m_fPartyRotation[i] = 0.0f;
    }
}

// FUNCTION: SWKOTOR 0x00578C30
CSWSAreaMap::~CSWSAreaMap()
{
    if (m_pBits != NULL) {
        delete m_pBits;
        m_pBits = NULL;
        m_nWords = 0;
    }
}

// FUNCTION: SWKOTOR 0x00578C60
void CSWSAreaMap::Initialize(int bEnable, int nGridWidth, int nNorth,
                             float fWorldX0, float fWorldY0, float fWorldX1, float fWorldY1,
                             int nMapX0, int nMapY0, int nMapX1, int nMapY1,
                             int nDivisor)
{
    if (!bEnable)
        return;
    if (nGridWidth > 0x58 || nGridWidth == 0)
        nGridWidth = 0x58;
    m_nGridWidth = nGridWidth;
    int nGridHeight = (int)(nGridWidth * 0.58181816f);
    m_nGridHeight = nGridHeight;
    if (nGridHeight < 1)
        nGridHeight = 1;
    m_nNorth = nNorth;
    m_nGridHeight = nGridHeight;
    m_fInvDivisor = 1.0f / nDivisor;
    if (m_pBits == NULL) {
        m_nWords = (nGridHeight + 1) * (nGridWidth + 1) / 32 + 1;
        m_pBits = new unsigned long[m_nWords];
        memset(m_pBits, 0, m_nWords * 4);
        float fTemp;
        if (m_nNorth == 3) {
            fTemp = fWorldX0;
            fWorldX0 = fWorldY0;
            fWorldY0 = fTemp;
            fWorldX0 *= -1.0f;
            fTemp = fWorldX1;
            fWorldX1 = fWorldY1;
            fWorldY1 = fTemp;
            fWorldX1 *= -1.0f;
        }
        if (m_nNorth == 1) {
            fWorldX0 *= -1.0f;
            fWorldY0 *= -1.0f;
            fWorldX1 *= -1.0f;
            fWorldY1 *= -1.0f;
        }
        if (m_nNorth == 2) {
            fTemp = fWorldX0;
            fWorldX0 = fWorldY0;
            fWorldY0 = fTemp;
            fWorldY0 *= -1.0f;
            fTemp = fWorldX1;
            fWorldX1 = fWorldY1;
            fWorldY1 = fTemp;
            fWorldY1 *= -1.0f;
        }
        m_fScaleX = (fWorldX1 - fWorldX0) / (nMapX1 - nMapX0);
        m_fScaleY = (fWorldY1 - fWorldY0) / (nMapY1 - nMapY0);
        m_fOriginX = fWorldX0 - nMapX0 * m_fScaleX;
        m_fOriginY = fWorldY0 - nMapY0 * m_fScaleY;
    }
}

// FUNCTION: SWKOTOR 0x00578E00
int CSWSAreaMap::GetMapPixelFromWorldCoord(Vector vWorld, int* pMapX, int* pMapY)
{
    float fX = vWorld.x;
    float fY = vWorld.y;
    float fTemp;
    if (m_nNorth == 3) {
        fTemp = fX;
        fX = fY;
        fY = fTemp;
        fX *= -1.0f;
    }
    if (m_nNorth == 1) {
        fX *= -1.0f;
        fY *= -1.0f;
    }
    if (m_nNorth == 2) {
        fTemp = fX;
        fX = fY;
        fY = fTemp;
        fY *= -1.0f;
    }
    *pMapX = (int)((fX - m_fOriginX) / m_fScaleX + 0.5f);
    *pMapY = (int)((fY - m_fOriginY) / m_fScaleY + 0.5f);
    if (*pMapX >= 0 && *pMapX <= 440 && *pMapY >= 0 && *pMapY <= 256)
        return 1;
    *pMapX = -1;
    *pMapY = -1;
    return 0;
}

// FUNCTION: SWKOTOR 0x00578ED0
float CSWSAreaMap::GetMapRotateCCWFromWorldOrientation(Vector vOrientation)
{
    float fYaw = GetVectorYaw(vOrientation);
    switch (m_nNorth) {
    case 1:
        return fYaw + 180.0f;
    case 2:
        return fYaw + 90.0f;
    case 3:
        return fYaw + 270.0f;
    }
    return fYaw;
}

// FUNCTION: SWKOTOR 0x00578F10
int CSWSAreaMap::GetGridPixelFromMapPixel(int nMapX, int nMapY, int* pGridX, int* pGridY)
{
    if (nMapX <= 440 && nMapX >= 0 && nMapY <= 256 && nMapY >= 0) {
        float fCellHeight = 256.0f / m_nGridHeight;
        *pGridX = (int)(nMapX / (440.0f / m_nGridWidth) + 0.5f);
        *pGridY = (int)(nMapY / fCellHeight + 0.5f);
        return 1;
    }
    *pGridX = -1;
    *pGridY = -1;
    return 0;
}

// FUNCTION: SWKOTOR 0x00578FA0
void CSWSAreaMap::SetAllExplored()
{
    if (m_pBits != NULL)
        memset(m_pBits, 0xff, m_nWords * 4);
}

// FUNCTION: SWKOTOR 0x00578FC0
void CSWSAreaMap::SetExploredData(unsigned long* pData, unsigned long nWords,
                                  int nGridWidth, int nGridHeight)
{
    if (m_nWords > 0 && nGridWidth == m_nGridWidth && nGridHeight == m_nGridHeight
        && nWords == m_nWords)
        memcpy(m_pBits, pData, nWords * 4);
}

// FUNCTION: SWKOTOR 0x00579000
void CSWSAreaMap::SetMapPointExplored(int nMapX, int nMapY)
{
    if (nMapX > 440 || nMapX < 0 || nMapY > 256 || nMapY < 0)
        return;
    int nGridX = 0;
    int nGridY = 0;
    if (!GetGridPixelFromMapPixel(nMapX, nMapY, &nGridX, &nGridY))
        return;
    int nBit = (m_nGridWidth + 1) * nGridY + nGridX;
    int nWord = nBit / 32;
    if (nWord > (int)m_nWords)
        return;
    m_pBits[nWord] |= 1 << (nBit % 32);
}

// FUNCTION: SWKOTOR 0x00579090
unsigned long* CSWSAreaMap::GetExploredData(unsigned long* pWords, int* pGridWidth,
                                            int* pGridHeight)
{
    if (m_nWords <= 0)
        return NULL;
    *pWords = m_nWords;
    *pGridWidth = m_nGridWidth;
    *pGridHeight = m_nGridHeight;
    return m_pBits;
}

// FUNCTION: SWKOTOR 0x005790C0
void CSWSAreaMap::SetPartyMemberWorldLocation(int nIndex, Vector vWorld)
{
    if (m_nWords > 0 && nIndex < 3) {
        int nMapX = 0;
        int nMapY = 0;
        if (GetMapPixelFromWorldCoord(vWorld, &nMapX, &nMapY)) {
            m_nPartyMapX[nIndex] = nMapX;
            m_nPartyMapY[nIndex] = nMapY;
        }
    }
}

// FUNCTION: SWKOTOR 0x00579130
void CSWSAreaMap::SetPartyMemberWorldOrientation(int nIndex, Vector vOrientation)
{
    if (m_nWords > 0 && nIndex < 3)
        m_fPartyRotation[nIndex] = GetMapRotateCCWFromWorldOrientation(vOrientation);
}

// FUNCTION: SWKOTOR 0x005791B0
int CSWSAreaMap::GetPartyMemberMapPixel(int nIndex, int* pMapX, int* pMapY)
{
    if (!IsPartyIndexValid(nIndex))
        return 0;
    if (m_nPartyMapX[nIndex] == -1)
        return 0;
    *pMapX = m_nPartyMapX[nIndex];
    *pMapY = m_nPartyMapY[nIndex];
    return 1;
}

// FUNCTION: SWKOTOR 0x005791F0
float CSWSAreaMap::GetPartyMemberMapRotation(int nIndex)
{
    if (!IsPartyIndexValid(nIndex))
        return 0.0f;
    return m_fPartyRotation[nIndex];
}

// FUNCTION: SWKOTOR 0x00579210
int CSWSAreaMap::IsWorldPointExplored(Vector vWorld)
{
    unsigned long nWords = m_nWords;
    if (nWords <= 0)
        return 0;
    int nMapX, nMapY;
    int nGridX, nGridY = 0;
    if (!GetMapPixelFromWorldCoord(vWorld, &nMapX, &nMapY))
        return 0;
    if (!GetGridPixelFromMapPixel(nMapX, nMapY, &nGridX, &nGridY))
        return 0;
    int nBit = (m_nGridWidth + 1) * nGridY + nGridX;
    int nWord = nBit / 32;
    if (nWord > (int)nWords)
        return 0;
    return ((1 << (nBit % 32)) & m_pBits[nWord]) != 0;
}

// FUNCTION: SWKOTOR 0x005792D0
void CSWSAreaMap::SetWorldPointExplored(Vector vWorld, int nRadius)
{
    if (m_nWords > 0) {
        int nMapX = 0;
        int nMapY = 0;
        if (GetMapPixelFromWorldCoord(vWorld, &nMapX, &nMapY)) {
            for (int i = -nRadius; i <= nRadius; i++) {
                int nSpan = nRadius - abs(i);
                for (int j = -nSpan; j <= nSpan; j++) {
                    int nX = (440 / m_nGridWidth) * i + nMapX;
                    int nY = (256 / m_nGridHeight) * j + nMapY;
                    if (nX < 0)
                        nX = 0;
                    if (nY < 0)
                        nY = 0;
                    if (nX > 440)
                        nX = 440;
                    if (nY > 256)
                        nY = 256;
                    SetMapPointExplored(nX, nY);
                }
            }
        }
    }
}
