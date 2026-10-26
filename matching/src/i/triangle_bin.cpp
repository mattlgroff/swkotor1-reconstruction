// CAurTriangleBin construction / teardown and the grass sort comparator
// (aurgrass.o). GrassSortTris is descriptive; the rest follow the class.
#include "i/triangle_bin.h"

// FUNCTION: SWKOTOR 0x004A7690
int GrassSortTris(const void* a, const void* b)
{
    CAurTriangleListElem e1 = *(const CAurTriangleListElem*)a;
    CAurTriangleListElem e2 = *(const CAurTriangleListElem*)b;
    if ((unsigned int)e1.m_pPart < (unsigned int)e2.m_pPart) {
        return -1;
    }
    if ((unsigned int)e1.m_pPart > (unsigned int)e2.m_pPart) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004A82B0
CAurTriangleBin::CAurTriangleBin()
{
    polygon_count = 0;
    flags = 0;
    m_pTexture.Reset();
    m_pVertices = 0;
    m_pIndices = 0;
    m_n40 = 0;
    m_n44 = 0;
    m_n10 = -1;
}

// FUNCTION: SWKOTOR 0x004A8350
CAurTriangleBin::~CAurTriangleBin()
{
    if (flags & 1) {
        delete[] m_pIndices;
        m_pIndices = 0;
        delete[] m_pVertices;
        m_pVertices = 0;
        m_n40 = 0;
        m_n44 = 0;
        flags ^= 1;
    }
    CAurTexture* pTexture = m_pTexture.m_p;
    m_pTexture.Reset();
    AurTextureRelease(pTexture);
}

// FUNCTION: SWKOTOR 0x004A8460
void CAurTriangleBin::DestroyGrassPolys()
{
    if (~flags & 4) {
        return;
    }
    if (flags & 1) {
        delete[] m_pIndices;
        m_pIndices = 0;
        m_n40 = 0;
        m_n44 = 0;
        delete[] m_pVertices;
        m_pVertices = 0;
        flags ^= 1;
    }
    if (flags & 8) {
        m_lGrass58.SetSize(0);
        m_lGrassTextures.SetSize(0);
    }
    flags ^= 4;
}
