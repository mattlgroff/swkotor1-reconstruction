// CSWRoomSurfaceMesh: walkmesh adjacency, perimeter, AABB queries, ctor / dtor
// (0x00580340-0x00582AE0). Names come from symbols.csv where callers already
// use them; the others follow the Mac build names (mac_names.csv) or describe
// the code.

#include "f/room_surface_mesh.h"

union LARGE_INTEGER {
    struct {
        unsigned long LowPart;
        long HighPart;
    } u;
    __int64 QuadPart;
};

extern "C" {
__declspec(dllimport) int __stdcall QueryPerformanceCounter(LARGE_INTEGER *p);     // IAT 0x0073D1A8
__declspec(dllimport) int __stdcall QueryPerformanceFrequency(LARGE_INTEGER *p);   // IAT 0x0073D1AC
}

float g_fAABBCheckTime;

static LARGE_INTEGER GetPerformanceFrequency()
{
    LARGE_INTEGER nFrequency;
    QueryPerformanceFrequency(&nFrequency);
    return nFrequency;
}

// Microseconds from the performance counter; times CheckAABBAll.
// FUNCTION: SWKOTOR 0x00580340
__int64 GetPerformanceMicroseconds()
{
    static LARGE_INTEGER s_nFrequency = GetPerformanceFrequency();
    LARGE_INTEGER nCounter;
    QueryPerformanceCounter(&nCounter);
    return nCounter.QuadPart * 1000000 / s_nFrequency.QuadPart;
}

// FUNCTION: SWKOTOR 0x00580720
CSWRoomSurfaceMeshAABBNode::CSWRoomSurfaceMeshAABBNode()
{
    m_nLeft = -1;
    m_nRight = -1;
    m_nFace = -1;
    m_nPlane = 4;
    m_nSplitMask = 0;
}

// FUNCTION: SWKOTOR 0x005807C0
int CSWRoomSurfaceMesh::LoadMeshBinary(CResBWM *pRes)
{
    if (!CSWCollisionMesh::LoadMeshBinary(pRes))
        return 0;
    m_pAdjacency = pRes->m_pHeader ? (CSWCollisionFace *)(pRes->m_pHeader->m_nAdjacencyOffset + pRes->m_pData) : NULL;
    m_nAABBRoot = pRes->m_pHeader ? pRes->m_pHeader->m_nAABBRoot : 0;
    m_lPerimeter.SetExternal(pRes->m_pHeader ? (CSWRoomEdge *)(pRes->m_pHeader->m_nPerimeterOffset + pRes->m_pData) : NULL,
                             pRes->m_pHeader ? pRes->m_pHeader->m_nPerimeterEdges : 0);
    m_lAABB.SetExternal(pRes->m_pHeader ? (CSWRoomSurfaceMeshAABBNode *)(pRes->m_pHeader->m_nAABBOffset + pRes->m_pData) : NULL,
                        pRes->m_pHeader ? pRes->m_pHeader->m_nAABBs : 0);
    m_lRooms.SetExternal(pRes->m_pHeader ? (int *)(pRes->m_pHeader->m_nRoomOffset + pRes->m_pData) : NULL,
                         pRes->m_pHeader ? pRes->m_pHeader->m_nRooms : 0);
    return 1;
}

// FUNCTION: SWKOTOR 0x005808E0
CSWWalkMeshHeader::CSWWalkMeshHeader()
{
}

// FUNCTION: SWKOTOR 0x00581370
int CSWRoomSurfaceMesh::SaveBinaryMesh(const CExoString &sFile)
{
    int bResult = CSWCollisionMesh::SaveBinaryMesh(sFile);
    if (bResult == 1) {
        FILE *pFile = fopen(sFile.CStr(), "rb+");
        CSWWalkMeshHeader cHeader;
        fread(&cHeader, sizeof(cHeader), 1, pFile);
        fseek(pFile, 0, SEEK_END);
        cHeader.m_nAABBs = m_lAABB.num;
        cHeader.m_nAABBOffset = ftell(pFile);
        cHeader.m_nAABBRoot = m_nAABBRoot;
        fwrite(m_lAABB.element, sizeof(CSWRoomSurfaceMeshAABBNode), cHeader.m_nAABBs, pFile);
        cHeader.m_nAdjacencyOffset = ftell(pFile);
        fwrite(m_pAdjacency, sizeof(int), cHeader.m_nWalkableFaces * 3, pFile);
        cHeader.m_nPerimeterEdges = m_lPerimeter.num;
        cHeader.m_nPerimeterOffset = ftell(pFile);
        fwrite(m_lPerimeter.element, sizeof(CSWRoomEdge), cHeader.m_nPerimeterEdges, pFile);
        cHeader.m_nRooms = m_lRooms.num;
        cHeader.m_nRoomOffset = ftell(pFile);
        fwrite(m_lRooms.element, sizeof(int), cHeader.m_nRooms, pFile);
        fseek(pFile, 0, SEEK_SET);
        fwrite(&cHeader, sizeof(cHeader), 1, pFile);
        fclose(pFile);
        return 1;
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x00581610
int CSWRoomSurfaceMesh::CheckAABBAll(Vector vStart, Vector vEnd, CSWCollisionResult *pResult)
{
    if (!m_lAABB.num)
        return 0;
    __int64 nStart = GetPerformanceMicroseconds();
    pResult->m_nMask = m_nAllMask;
    int nHit = CheckAABBNode(m_nAABBRoot, &vStart, &vEnd, pResult);
    if (pResult->m_nFace == -1) {
        Vector vNudge = Normalize(Vector(1.0f, 1.0f, 0.0f));
        vNudge *= 0.001f;
        vStart += vNudge;
        vEnd += vNudge;
        nHit = CheckAABBNode(m_nAABBRoot, &vStart, &vEnd, pResult);
    }
    if (nHit > 0) {
        pResult->m_bHit = 1;
        pResult->m_vHit = vEnd;
    }
    g_fAABBCheckTime = (float)(GetPerformanceMicroseconds() - nStart);
    return pResult->m_bHit;
}

// FUNCTION: SWKOTOR 0x00581D60 SYMBOL: ??1?$CExoArrayList@VCSWRoomSurfaceMeshAABBNode@@@@QAE@XZ
template CExoArrayList<CSWRoomSurfaceMeshAABBNode>::~CExoArrayList();

// FUNCTION: SWKOTOR 0x00580620
void CSWRoomSurfaceMesh::GetAdjacentFaces(int nUnused, int nFace, int *pA, int *pB, int *pC)
{
    if (nFace < m_nFaces && m_pAdjacency) {
        int i = nFace * 3;
        int n = ((int *)m_pAdjacency)[i];
        *pA = n == -1 ? -1 : n / 3;
        n = ((int *)m_pAdjacency)[i + 1];
        *pB = n == -1 ? -1 : n / 3;
        n = ((int *)m_pAdjacency)[i + 2];
        *pC = n == -1 ? -1 : n / 3;
    }
}

// FUNCTION: SWKOTOR 0x00580AF0
void CSWRoomSurfaceMesh::TransformAABBNode(int nNode, const Vector &vPosition)
{
    if (nNode == -1)
        return;
    m_lAABB.element[nNode].m_vMax = m_vOffset + m_lAABB.element[nNode].m_vMax + vPosition;
    m_lAABB.element[nNode].m_vMin = m_lAABB.element[nNode].m_vMin + m_vOffset + vPosition;
    TransformAABBNode(m_lAABB.element[nNode].m_nLeft, vPosition);
    TransformAABBNode(m_lAABB.element[nNode].m_nRight, vPosition);
}

// FUNCTION: SWKOTOR 0x00580920
int CSWRoomSurfaceMesh::CheckAABBNode(int nNode, Vector *pStart, Vector *pEnd, CSWCollisionResult *pResult)
{
    if (!BoxIntersect(pStart, pEnd, &m_lAABB.element[nNode].m_vMin, &m_lAABB.element[nNode].m_vMax))
        return 0;
    if (m_lAABB.element[nNode].m_nFace != -1) {
        int nFace = m_lAABB.element[nNode].m_nFace;
        unsigned long nBit = 1 << m_pnMaterials[nFace];
        int nHit = 0;
        if (!(nBit & pResult->m_nMask))
            return 0;
        Vector avTriangle[3];
        Vector vHit;
        int nIndex = nFace * 3;
        avTriangle[0] = m_pvVertices[((int *)m_pFaces)[nIndex]];
        avTriangle[1] = m_pvVertices[((int *)m_pFaces)[nIndex + 1]];
        avTriangle[2] = m_pvVertices[((int *)m_pFaces)[nIndex + 2]];
        if (PolyHit(avTriangle, 3, pStart, pEnd, pEnd, &vHit)) {
            pResult->m_nFace = nFace;
            pResult->m_nBit = nBit;
            return 1;
        }
        return nHit;
    }
    if (pResult->m_n04 & m_lAABB.element[nNode].m_nSplitMask) {
        return CheckAABBNode(m_lAABB.element[nNode].m_nLeft, pStart, pEnd, pResult) +
               CheckAABBNode(m_lAABB.element[nNode].m_nRight, pStart, pEnd, pResult);
    }
    return CheckAABBNode(m_lAABB.element[nNode].m_nRight, pStart, pEnd, pResult) +
           CheckAABBNode(m_lAABB.element[nNode].m_nLeft, pStart, pEnd, pResult);
}

// FUNCTION: SWKOTOR 0x00580BE0
int CSWRoomSurfaceMesh::GetPerimeterVertex(int nEdge, Vector &vOut)
{
    if (nEdge < m_lPerimeter.num) {
        vOut = m_pvVertices[((int *)m_pFaces)[m_lPerimeter.element[nEdge].m_nVertexSlot]];
        return 1;
    }
    vOut = Vector(0.0f, 0.0f, 0.0f);
    return 0;
}

// FUNCTION: SWKOTOR 0x00580C60
int CSWRoomSurfaceMesh::GetEdgeVertices(int nEdge, Vector &vStart, Vector &vEnd)
{
    int nCount = m_lPerimeter.num;
    if (nEdge < nCount) {
        vStart = m_pvVertices[((int *)m_pFaces)[m_lPerimeter.element[nEdge].m_nVertexSlot]];
        int nNext = nEdge + 1 == nCount ? 0 : nEdge + 1;
        vEnd = m_pvVertices[((int *)m_pFaces)[m_lPerimeter.element[nNext].m_nVertexSlot]];
        return 1;
    }
    vStart = Vector(0.0f, 0.0f, 0.0f);
    vEnd = Vector(0.0f, 0.0f, 0.0f);
    return 0;
}

// FUNCTION: SWKOTOR 0x00580D50
int CSWRoomSurfaceMesh::WeldEdgeVertices(CSWRoomSurfaceMesh *pOther, float fTolerance, Vector *pUnused1, Vector *pUnused2)
{
    if (m_bLoaded)
        return 0;
    int nWelded = 0;
    Vector vThis;
    Vector vOther;
    for (int i = 0; i < m_lPerimeter.num; i++) {
        GetPerimeterVertex(i, vThis);
        for (int j = 0; j < pOther->m_lPerimeter.num; j++) {
            pOther->GetPerimeterVertex(j, vOther);
            Vector vDiff = vThis - vOther;
            if (vDiff.x * vDiff.x + vDiff.y * vDiff.y + vDiff.z * vDiff.z < fTolerance) {
                if (j < pOther->m_lPerimeter.num)
                    pOther->m_pvVertices[((int *)pOther->m_pFaces)[pOther->m_lPerimeter.element[j].m_nVertexSlot]] = vThis;
                nWelded++;
            }
        }
    }
    return nWelded;
}

// FUNCTION: SWKOTOR 0x00580E90
int CSWRoomSurfaceMesh::SetAdjacency(CSWRoomSurfaceMesh *pOther, int nThisRoom, int nOtherRoom, Vector *pUnused1, Vector *pUnused2)
{
    if (m_bLoaded)
        return 0;
    int nCount = 0;
    Vector vStart;
    Vector vEnd;
    Vector vOtherStart;
    Vector vOtherEnd;
    Vector vFirst;
    int nRoom = 0;
    for (int i = 0; i < m_lPerimeter.num; i++) {
        GetEdgeVertices(i, vStart, vEnd);
        if (i == 0)
            vFirst = vStart;
        if (i == m_lRooms.element[nRoom] - 1) {
            nRoom++;
            Vector vTemp = vEnd;
            vEnd = vFirst;
            vFirst = vTemp;
        }
        Vector vOtherFirst;
        int nOtherRoomIndex = 0;
        for (int j = 0; j < pOther->m_lPerimeter.num; j++) {
            pOther->GetEdgeVertices(j, vOtherStart, vOtherEnd);
            if (j == 0)
                vOtherFirst = vOtherStart;
            if (j == pOther->m_lRooms.element[nOtherRoomIndex] - 1) {
                nOtherRoomIndex++;
                Vector vTemp = vOtherEnd;
                vOtherEnd = vOtherFirst;
                vOtherFirst = vTemp;
            }
            Vector vDiff1 = vStart - vOtherEnd;
            Vector vDiff2 = vEnd - vOtherStart;
            if (vDiff1.x * vDiff1.x + vDiff1.y * vDiff1.y + vDiff1.z * vDiff1.z < 0.0001 &&
                vDiff2.x * vDiff2.x + vDiff2.y * vDiff2.y + vDiff2.z * vDiff2.z < 0.0001) {
                m_lPerimeter.element[i].m_nTransition = nOtherRoom;
                pOther->m_lPerimeter.element[j].m_nTransition = nThisRoom;
                nCount++;
            }
        }
    }
    return nCount;
}

// FUNCTION: SWKOTOR 0x00581930
int CSWRoomSurfaceMesh::GetSurfaceMaterial(Vector vPosition)
{
    Vector vLocal = WorldToLocal(vPosition);
    CSWCollisionResult cResult(0xffffffff);
    Vector vStart(vLocal.x, vLocal.y, 1000.0f);
    Vector vEnd(vLocal.x, vLocal.y, -1000.0f);
    CheckAABBAll(vStart, vEnd, &cResult);
    if (cResult.m_nFace == -1)
        return 0;
    return m_pnMaterials[cResult.m_nFace];
}

// FUNCTION: SWKOTOR 0x00581A10
int CSWRoomSurfaceMesh::GetSurfaceMaterialWalkCheckOnly(Vector vPosition)
{
    Vector vLocal = WorldToLocal(vPosition);
    CSWCollisionResult cResult(0xffffffff);
    Vector vStart(vLocal.x, vLocal.y, 1000.0f);
    Vector vEnd(vLocal.x, vLocal.y, -1000.0f);
    CheckAABBWalkable(vStart, vEnd, &cResult);
    if (cResult.m_nFace == -1)
        return 0;
    return m_pnMaterials[cResult.m_nFace];
}

// FUNCTION: SWKOTOR 0x00581720
int CSWRoomSurfaceMesh::FindTriangle(const Vector &vPosition, int bWalkCheck, Vector vNudge)
{
    Vector vLocal = WorldToLocal(vPosition);
    Vector vStart(vLocal.x, vLocal.y, 1000.0f);
    Vector vEnd(vLocal.x, vLocal.y, -1000.0f);
    CSWCollisionResult cResult(0xffffffff);
    if (bWalkCheck)
        CheckAABBWalkable(vStart, vEnd, &cResult);
    else
        CheckAABBAll(vStart, vEnd, &cResult);
    if (cResult.m_nFace == -1) {
        Vector vDelta = Normalize(vNudge);
        vDelta *= 0.001f;
        vStart += vDelta;
        vEnd += vDelta;
        if (bWalkCheck)
            CheckAABBWalkable(vStart, vEnd, &cResult);
        else
            CheckAABBAll(vStart, vEnd, &cResult);
    }
    return cResult.m_nFace;
}

// FUNCTION: SWKOTOR 0x00581B30
float CSWRoomSurfaceMesh::ComputeHeightAABB(Vector vPosition, int bWalkCheck)
{
    int nFace = FindTriangle(vPosition, bWalkCheck, Vector(1.0f, 1.0f, 0.0f));
    if (nFace == -1)
        return -9.0f;
    Vector vLocal(vPosition.x, vPosition.y, -9.0f);
    return (float)PlaneHeightAtPoint(m_pvNormals[nFace], m_pfPlaneDistances[nFace], vLocal);
}

// FUNCTION: SWKOTOR 0x00582180
void CSWRoomSurfaceMesh::GetResRef(CResRef *pResRef)
{
    *pResRef = m_cResRef;
}

// FUNCTION: SWKOTOR 0x005821A0
unsigned long CSWRoomSurfaceMesh::GetWalkCheckMask()
{
    return m_nWalkCheckMask;
}

// FUNCTION: SWKOTOR 0x005821B0
unsigned long CSWRoomSurfaceMesh::GetLineOfSightMask()
{
    return m_nLineOfSightMask;
}

// FUNCTION: SWKOTOR 0x005821C0
unsigned long CSWRoomSurfaceMesh::GetWalkMask()
{
    return m_nWalkMask;
}

// FUNCTION: SWKOTOR 0x005821D0
unsigned long CSWRoomSurfaceMesh::GetAllMask()
{
    return m_nAllMask;
}

// FUNCTION: SWKOTOR 0x00582AE0
CSWRoomSurfaceMesh::CSWRoomSurfaceMesh() : CSWCollisionMesh(2002, 2016)
{
    m_pAdjacency = NULL;
    m_lAABB = CSWMeshList<CSWRoomSurfaceMeshAABBNode>();
    m_nLineOfSightMask = 0;
    m_nWalkMask = 0;
    m_nWalkCheckMask = 0;
    m_nAllMask = 0;
    for (int i = 0; i < 32; i++) {
        CExoString sLabel;
        int nValue;
        g_pRules->Get2DArrays()->m_pSurfaceMat->GetCExoStringEntry(i, CExoString("Label"), &sLabel);
        g_pRules->Get2DArrays()->m_pSurfaceMat->GetINTEntry(i, CExoString("LineOfSight"), &nValue);
        if (nValue == 1)
            m_nLineOfSightMask += 1 << i;
        g_pRules->Get2DArrays()->m_pSurfaceMat->GetINTEntry(i, CExoString("Walk"), &nValue);
        if (nValue == 1)
            m_nWalkMask += 1 << i;
        g_pRules->Get2DArrays()->m_pSurfaceMat->GetINTEntry(i, CExoString("WalkCheck"), &nValue);
        if (nValue == 1)
            m_nWalkCheckMask += 1 << i;
        m_nAllMask += 1 << i;
    }
}

// FUNCTION: SWKOTOR 0x00581D90
void CSWRoomSurfaceMesh::AddCrossing(const Vector &vUnused, const Vector &vDirection, const Vector &vPoint, int &nCrossings, int nData)
{
    int i = 0;
    int nCount = m_lCrossings.num;
    for (; i < nCount; i++) {
        if (m_lCrossings.element[i].x * vDirection.x + m_lCrossings.element[i].y * vDirection.y + m_lCrossings.element[i].z * vDirection.z > vPoint.x * vDirection.x + vPoint.y * vDirection.y + vPoint.z * vDirection.z)
            break;
    }
    m_lCrossings.Insert(vPoint, i);
    m_lCrossingData.Insert(nData, i);
    nCrossings++;
}

// FUNCTION: SWKOTOR 0x00581FE0
CSWRoomSurfaceMesh::~CSWRoomSurfaceMesh()
{
    if (m_bLoaded) {
        m_pAdjacency = NULL;
        m_lAABB.element = NULL;
        m_lAABB.num = 0;
        m_lAABB.array_size = 0;
        m_lAABB.m_bExternal = 0;
        m_lPerimeter.element = NULL;
        m_lPerimeter.num = 0;
        m_lPerimeter.array_size = 0;
        m_lPerimeter.m_bExternal = 0;
        m_lRooms.m_bExternal = 0;
        m_lRooms.element = NULL;
        m_lRooms.array_size = 0;
        m_lRooms.num = 0;
    } else {
        delete[] m_pAdjacency;
        m_pAdjacency = NULL;
        m_lPerimeter.Clear();
        m_lRooms.Clear();
    }
}
