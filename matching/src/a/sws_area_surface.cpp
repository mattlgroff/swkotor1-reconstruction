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
