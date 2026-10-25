// CSWSArea surface-material query.
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
