// CSWSArea::LoadRooms: builds the area's room array from its layout and
// welds/links the walkmeshes (0x00504870), plus the CSWSRoom vector deleting
// destructor emitted right after it.
#include "a/sws_area.h"

class CSWRoom {
public:
    void SetRoomInfo(const CResRef& cName, const Vector& vPosition);    // 0x005C7D70
};

class CSWSRoom : public CSWRoom {
public:
    CSWSRoom();                                     // 0x005793B0
    ~CSWSRoom();                                    // 0x005794B0
    int LoadWalkMesh(int bArg);                     // 0x00579520
    void UpdateWalkMeshPosition();                  // 0x005796B0 (placeholder name)
    int WeldRoom(CSWSRoom* pOther, float fTolerance);   // 0x005796D0 (placeholder name)
    int SetRoomAdjacency(CSWSRoom* pOther, int nIndex, int nOtherIndex); // 0x00579740
    void ComputeMeshNormals();                      // 0x00579730

    char pad_0x0[0x4c];
};

class CLYT {
public:
    int GetNumRooms();                              // 0x005A4C50 (folded getter)
    CResRef GetRoomName(int nIndex);                // 0x005DE470 (placeholder name)
    Vector GetRoomPosition(int nIndex);             // 0x005DE4A0
};

// FUNCTION: SWKOTOR 0x00504870
CSWSRoom* CSWSArea::LoadRooms(CLYT* pLayout)
{
    int nRooms = pLayout->GetNumRooms();
    if (nRooms == 0) {
        return NULL;
    }
    CSWSRoom* pRooms = new CSWSRoom[nRooms];
    int i;
    for (i = 0; i < nRooms; i++) {
        pRooms[i].SetRoomInfo(pLayout->GetRoomName(i), pLayout->GetRoomPosition(i));
        pRooms[i].LoadWalkMesh(0);
        pRooms[i].UpdateWalkMeshPosition();
    }
    for (i = 0; i < nRooms; i++) {
        for (int j = i + 1; j < nRooms; j++) {
            pRooms[i].WeldRoom(&pRooms[j], 0.01f);
        }
    }
    for (i = 0; i < nRooms; i++) {
        for (int j = i + 1; j < nRooms; j++) {
            pRooms[i].SetRoomAdjacency(&pRooms[j], i, j);
        }
    }
    for (i = 0; i < nRooms; i++) {
        pRooms[i].ComputeMeshNormals();
    }
    return pRooms;
}

// FUNCTION: SWKOTOR 0x00504A10 SYMBOL: ??_ECSWSRoom@@QAEPAXI@Z
void DeleteAreaRooms(CSWSRoom* pRooms)
{
    delete[] pRooms;
}
