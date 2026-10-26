// CAurTriangleListElem and CExoArrayList<CAurTriangleListElem>
// (aurgrass.o). No Mac names for these; the class name follows the
// playable build.
#include "i/triangle_list.h"

// FUNCTION: SWKOTOR 0x004A7240 SYMBOL: ??0CAurTriangleListElem@@QAE@V?$CExoArrayList@M@@PAVMaxFace@@PAVPartTriMesh@@H@Z
CAurTriangleListElem::CAurTriangleListElem(CExoArrayList<float> lCorners, MaxFace* pFace, PartTriMesh* pPart,
                                           int nCount)
{
    m_lCorners = lCorners;
    m_pFace = pFace;
    m_pPart = pPart;
    m_nCount = nCount;
}

// FUNCTION: SWKOTOR 0x004A72D0
CAurTriangleListElem::~CAurTriangleListElem()
{
    m_lCorners.SetSize(0);
    m_pFace = 0;
    m_pPart = 0;
    m_nCount = 0;
}

// Not in functions.csv; the vector constructor iterator in Allocate uses it.
// FUNCTION: SWKOTOR 0x004A7220 SYMBOL: ??0CAurTriangleListElem@@QAE@XZ
// FUNCTION: SWKOTOR 0x004A7340 SYMBOL: ??0?$CExoArrayList@M@@QAE@ABV0@@Z
// FUNCTION: SWKOTOR 0x004A73F0 SYMBOL: ?Allocate@?$CExoArrayList@VCAurTriangleListElem@@@@QAEXH@Z
// FUNCTION: SWKOTOR 0x004A77C0 SYMBOL: ?Add@?$CExoArrayList@VCAurTriangleListElem@@@@QAEXVCAurTriangleListElem@@@Z
// FUNCTION: SWKOTOR 0x004A7790 SYMBOL: ??1?$CExoArrayList@VCAurTriangleListElem@@@@QAE@XZ
// FUNCTION: SWKOTOR 0x004A81B0 SYMBOL: ??0?$CExoArrayList@VCAurTriangleListElem@@@@QAE@ABV0@@Z
template class CExoArrayList<CAurTriangleListElem>;
template CExoArrayList<float>::CExoArrayList(const CExoArrayList<float>&);
