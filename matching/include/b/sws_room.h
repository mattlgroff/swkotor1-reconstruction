// CSWRoom / CSWSRoom: a room of an area and its server walkmesh
// (b0057_area_map_room). Only what worker B uses of CSWRoomSurfaceMesh
// (worker F's range) is declared here.
#ifndef B_SWS_ROOM_H
#define B_SWS_ROOM_H

#include "b/vector.h"
#include "b/res_gff.h"

// Collision query result (0x20 bytes on the stack of ClearLineOfSight).
class CSWCollisionResult {
public:
    CSWCollisionResult(unsigned long nMask);            // 0x00581320
    void ClearAll(unsigned long nMask);                 // 0x00580780

    unsigned long m_nMask;      // +0x00
    char pad_0x4[0x4];
    int m_bHit;                 // +0x08
    int m_nFace;                // +0x0C
    Vector m_vHit;              // +0x10
    char pad_0x1c[0x4];
};

class CSWRoomSurfaceMesh {
public:
    CSWRoomSurfaceMesh();                                               // 0x00582AE0
    ~CSWRoomSurfaceMesh();                                              // 0x00581FE0
    virtual void v00();
    virtual void v01();
    virtual void SetPosition(Vector* pPosition);                        // vtable +0x08

    float ComputeHeight(int nArg, Vector vPosition);                    // 0x005805E0
    int WeldEdgeVertices(CSWRoomSurfaceMesh* pOther, int nArg,
                         Vector* pPosition, Vector* pOtherPosition);    // 0x00580D50
    int SetAdjacency(CSWRoomSurfaceMesh* pOther, int nArg1, int nArg2,
                     Vector* pPosition, Vector* pOtherPosition);        // 0x00580E90
    int GetAdjacentSecond(int nIndex);                                  // 0x00581150
    int CheckAABBLineOfSight(Vector vStart, Vector vEnd,
                             CSWCollisionResult* pResult);              // 0x005814D0
    int CheckAABBWalkable(Vector vStart, Vector vEnd,
                          CSWCollisionResult* pResult);                 // 0x00581530
    int CheckAABBAll(Vector vStart, Vector vEnd,
                     CSWCollisionResult* pResult);                      // 0x00581610
    float ComputeHeightAABB(Vector vPosition, int nArg);                // 0x00581B30
    int IsWalkable(float fX1, float fY1, float fZ1, float fX2, float fY2, float fZ2,
                   int nArg);                                           // 0x00584220
    int LoadMesh(int nArg);                                             // 0x00596670
    void ComputeNormals();                                              // 0x00596C10
    int NoNonWalkPolys(float fArg1, float fArg2, float fArg3, float fArg4,
                       float fArg5, int nArg6);                         // 0x005972B0

    // +0x00 vtable
    char pad_0x4[0x8];
    CResRef m_cResRef;          // +0x0C
    char pad_0x1c[0x4];
    Vector m_vOffset;           // +0x20
    char pad_0x2c[0x3c];
    Vector* m_pFaceNormals;     // +0x68
    char pad_0x6c[0xe8 - 0x6c];
};

class CSWRoom {
public:
    CSWRoom();                                                          // 0x005C7D10
    virtual ~CSWRoom();                                                 // 0x005C7D00

    Vector m_vPosition;         // +0x04
    float m_fOrientation[4];    // +0x10
    CResRef m_cResRef;          // +0x20
    int m_n30;                  // +0x30
    float m_f34;                // +0x34
    int m_bNoWalkMesh;          // +0x38
};

class CSWSRoom : public CSWRoom {
public:
    CSWSRoom();                                                         // 0x005793B0
    virtual ~CSWSRoom();                                                // 0x005794B0
    int GetAdjacentRoom(int nIndex);                                    // 0x00579420
    float ComputeHeight(Vector vPosition, int nArg);                    // 0x00579430
    float ComputeHeightSecondary(int nArg, Vector vPosition);           // 0x00579470
    int LoadWalkMesh(int nArg);                                         // 0x00579520
    int CheckAABBWalkable(Vector vStart, Vector vEnd,
                          CSWCollisionResult* pResult);                 // 0x00579560
    int CheckAABBAll(Vector vStart, Vector vEnd,
                     CSWCollisionResult* pResult);                      // 0x00579610
    int IsWalkable(float fX1, float fY1, float fZ1, float fX2, float fY2, float fZ2,
                   int nArg);                                           // 0x00579670
    int NoNonWalkPolysOnRoom(float fArg1, float fArg2, float* pArg3, float* pArg4,
                             float* pArg5, int nArg6);                  // 0x00579680
    void UpdateWalkMeshPosition();                                      // 0x005796B0
    int WeldRoom(CSWSRoom* pOther, int nArg);                           // 0x005796D0
    void ComputeMeshNormals();                                          // 0x00579730
    int SetRoomAdjacency(CSWSRoom* pOther, int nArg1, int nArg2);       // 0x00579740
    int ClearLineOfSight(Vector vStart, Vector vEnd, Vector* pHit, int nUnused); // 0x005797A0
    Vector GetFaceNormal(int nFace);                                    // 0x00579840

    CSWRoomSurfaceMesh* m_pWalkMesh;    // +0x3C
    char pad_0x40[0xc];
};

#endif
