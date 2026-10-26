// CSWSArea surface-material queries.
#include "a/sws_area.h"

class CSWSRoomSurfaceMesh {
public:
    int GetSurfaceMaterial(Vector vPosition); // 0x00581930
    int GetSurfaceMaterialWalkCheckOnly(Vector vPosition); // 0x00581A10
};

class CSWSRoom {
public:
    char pad_0x0[0x3c];
    CSWSRoomSurfaceMesh* m_pSurfaceMesh; // 0x3c
};

class C2DA {
public:
    int GetINTEntry(int nRow, const CExoString& sColumn, int* pValue);  // 0x00413660
};

class C2DAs {
public:
    char pad_0x0[0x14];
    C2DA* m_pSurfaceMaterial;                       // 0x14 (placeholder name)
};

class CSWRules {
public:
    char pad_0x0[0xb8];
    C2DAs* m_p2DArrays;                             // 0xb8
};
extern CSWRules* g_pRules;                          // 0x007A3A28

// FUNCTION: SWKOTOR 0x005063C0
int CSWSArea::GetSurfaceMaterial(Vector vPosition)
{
    CSWSRoom* pRoom = GetRoom(&vPosition, 0);
    if (pRoom) {
        CSWSRoomSurfaceMesh* pMesh = pRoom->m_pSurfaceMesh;
        return pMesh->GetSurfaceMaterial(vPosition);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00506400
int CSWSArea::PositionWalkable(Vector vPosition)
{
    CSWSRoom* pRoom = GetRoom(&vPosition, 0);
    if (pRoom) {
        CSWSRoomSurfaceMesh* pMesh = pRoom->m_pSurfaceMesh;
        int nMaterial = pMesh->GetSurfaceMaterialWalkCheckOnly(vPosition);
        int bWalk;
        g_pRules->m_p2DArrays->m_pSurfaceMaterial->GetINTEntry(nMaterial, CExoString("Walk"), &bWalk);
        return bWalk != 0;
    }
    return 0;
}
