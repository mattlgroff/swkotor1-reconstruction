// CSWSRoom (0x005793B0-0x00579840): server room, forwarding to its walkmesh.
#include "b/sws_room.h"

inline Vector operator+(const Vector& a, const Vector& b)
{
    return Vector(a.x + b.x, a.y + b.y, a.z + b.z);
}

// FUNCTION: SWKOTOR 0x005793B0
CSWSRoom::CSWSRoom()
{
    m_pWalkMesh = new CSWRoomSurfaceMesh();
}

// FUNCTION: SWKOTOR 0x00579420
int CSWSRoom::GetAdjacentRoom(int nIndex)
{
    return m_pWalkMesh->GetAdjacentSecond(nIndex);
}

// FUNCTION: SWKOTOR 0x00579430
float CSWSRoom::ComputeHeight(Vector vPosition, int nArg)
{
    float fHeight = 0.0f;
    if (m_pWalkMesh != NULL)
        fHeight = m_pWalkMesh->ComputeHeightAABB(vPosition, nArg);
    return fHeight;
}

// FUNCTION: SWKOTOR 0x00579470
float CSWSRoom::ComputeHeightSecondary(int nArg, Vector vPosition)
{
    float fHeight = 0.0f;
    if (m_pWalkMesh != NULL)
        fHeight = m_pWalkMesh->ComputeHeight(nArg, vPosition);
    return fHeight;
}

// FUNCTION: SWKOTOR 0x005794B0
CSWSRoom::~CSWSRoom()
{
    if (m_pWalkMesh != NULL)
        delete m_pWalkMesh;
    m_pWalkMesh = NULL;
}

// FUNCTION: SWKOTOR 0x00579520
int CSWSRoom::LoadWalkMesh(int nArg)
{
    if (m_bNoWalkMesh)
        return 0;
    m_pWalkMesh->m_cResRef = m_cResRef;
    return m_pWalkMesh->LoadMesh(nArg);
}

// FUNCTION: SWKOTOR 0x00579560
int CSWSRoom::CheckAABBWalkable(Vector vStart, Vector vEnd, CSWCollisionResult* pResult)
{
    int bResult = 0;
    CSWRoomSurfaceMesh* pMesh = m_pWalkMesh;
    if (pMesh->CheckAABBWalkable(vStart, vEnd, pResult)) {
        bResult = 1;
        pResult->m_vHit = pResult->m_vHit + m_vPosition + m_pWalkMesh->m_vOffset;
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x00579610
int CSWSRoom::CheckAABBAll(Vector vStart, Vector vEnd, CSWCollisionResult* pResult)
{
    int bResult = 0;
    if (m_pWalkMesh->CheckAABBAll(vStart, vEnd, pResult))
        bResult = 1;
    return bResult;
}

// FUNCTION: SWKOTOR 0x00579670
int CSWSRoom::IsWalkable(float fX1, float fY1, float fZ1, float fX2, float fY2, float fZ2,
                         int nArg)
{
    return m_pWalkMesh->IsWalkable(fX1, fY1, fZ1, fX2, fY2, fZ2, nArg);
}

// FUNCTION: SWKOTOR 0x00579680
int CSWSRoom::NoNonWalkPolysOnRoom(float fArg1, float fArg2, float* pArg3, float* pArg4,
                                   float* pArg5, int nArg6)
{
    CSWRoomSurfaceMesh* pMesh = m_pWalkMesh;
    return pMesh->NoNonWalkPolys(fArg1, fArg2, *pArg3, *pArg4, *pArg5, nArg6);
}

// FUNCTION: SWKOTOR 0x005796B0
void CSWSRoom::UpdateWalkMeshPosition()
{
    if (!m_bNoWalkMesh)
        m_pWalkMesh->SetPosition(&m_vPosition);
}

// FUNCTION: SWKOTOR 0x005796D0
int CSWSRoom::WeldRoom(CSWSRoom* pOther, int nArg)
{
    if (m_bNoWalkMesh || pOther->m_bNoWalkMesh)
        return 0;
    Vector vOther = pOther->m_vPosition;
    return m_pWalkMesh->WeldEdgeVertices(pOther->m_pWalkMesh, nArg, &m_vPosition, &vOther);
}

// FUNCTION: SWKOTOR 0x00579730
void CSWSRoom::ComputeMeshNormals()
{
    if (!m_bNoWalkMesh)
        m_pWalkMesh->ComputeNormals();
}

// FUNCTION: SWKOTOR 0x00579740
int CSWSRoom::SetRoomAdjacency(CSWSRoom* pOther, int nArg1, int nArg2)
{
    if (m_bNoWalkMesh || pOther->m_bNoWalkMesh)
        return 0;
    Vector vOther = pOther->m_vPosition;
    return m_pWalkMesh->SetAdjacency(pOther->m_pWalkMesh, nArg1, nArg2, &m_vPosition, &vOther);
}

// FUNCTION: SWKOTOR 0x005797A0
int CSWSRoom::ClearLineOfSight(Vector vStart, Vector vEnd, Vector* pHit, int nUnused)
{
    if (m_bNoWalkMesh)
        return 1;
    CSWCollisionResult cResult(-1);
    cResult.ClearAll(-1);
    int bBlocked = m_pWalkMesh->CheckAABBLineOfSight(vStart, vEnd, &cResult);
    if (bBlocked && pHit != NULL)
        *pHit = cResult.m_vHit;
    return !bBlocked;
}

// FUNCTION: SWKOTOR 0x00579840
Vector CSWSRoom::GetFaceNormal(int nFace)
{
    if (nFace >= 0 && m_pWalkMesh->m_pFaceNormals != NULL)
        return m_pWalkMesh->m_pFaceNormals[nFace];
    return Vector(0.0f, 0.0f, 1.0f);
}
