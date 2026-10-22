// CExoArrayList<MaxFace_PostProcess>::Add, the out-of-line
// instance (0x004647F0).

#define H_ALLOCATE_OUT_OF_LINE
#define H_ADD_OUT_OF_LINE
#include "h/texctl.h"

struct MaxFace_PostProcess {
    int* pVerts;            // +0x00 int[3]
    int* pTVerts;           // +0x04 int[12]
    int nSmoothGroup;       // +0x08
    int nMaterial;          // +0x0c
    int* pNormals;          // +0x10 int[3]

    ~MaxFace_PostProcess();                 // 0x0043E0F0
};

template void CExoArrayList<MaxFace_PostProcess>::Add(MaxFace_PostProcess value);

// FUNCTION: SWKOTOR 0x004647F0 SYMBOL: ?Add@?$CExoArrayList@UMaxFace_PostProcess@@@@QAEXUMaxFace_PostProcess@@@Z
