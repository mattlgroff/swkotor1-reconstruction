// CSWRoomSurfaceMesh and its CExoArrayList instances
// (b0058_f2_surfmesh_placeable).

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;

    void Allocate(int s);
    void Add(T t);
    void Insert(T t, int pos);
    CExoArrayList<T> &operator=(const CExoArrayList<T> &other);
};

template <class T>
void CExoArrayList<T>::Allocate(int s)
{
    T *old = element;
    array_size = s;
    element = new T[s];
    for (int i = 0; i < num; i++)
        element[i] = old[i];
    if (old)
        delete[] old;
}

template <class T>
void CExoArrayList<T>::Add(T t)
{
    if (num == array_size)
        Allocate(array_size ? array_size * 2 : 16);
    element[num++] = t;
}

template <class T>
void CExoArrayList<T>::Insert(T t, int pos)
{
    int i = num;
    Add(t);
    for (; i > pos; i--)
        element[i] = element[i - 1];
    element[pos] = t;
}

template <class T>
CExoArrayList<T> &CExoArrayList<T>::operator=(const CExoArrayList<T> &other)
{
    num = 0;
    for (int i = 0; i < other.num; i++)
        Add(other.element[i]);
    return *this;
}

class Vector {
public:
    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    Vector &operator*=(float f);
    Vector &operator+=(const Vector &v);
    float x, y, z;
};
Vector Normalize(const Vector &v);
double PlaneHeightAtPoint(const Vector &vNormal, float fDistance, const Vector &vPoint);

class CSWRoomSurfaceMeshAABBNode {
public:
    CSWRoomSurfaceMeshAABBNode();
    ~CSWRoomSurfaceMeshAABBNode() {}
    CSWRoomSurfaceMeshAABBNode &operator=(const CSWRoomSurfaceMeshAABBNode &o)
    {
        m_vMin = o.m_vMin;
        m_vMax = o.m_vMax;
        m_nFace = o.m_nFace;
        m_nPlane = o.m_nPlane;
        m_nSplit = o.m_nSplit;
        m_pLeft = o.m_pLeft;
        m_pRight = o.m_pRight;
        return *this;
    }
    Vector m_vMin;            // +0x00
    Vector m_vMax;            // +0x0c
    int m_nFace;              // +0x18
    int m_nPlane;             // +0x1c
    int m_nSplit;             // +0x20
    CSWRoomSurfaceMeshAABBNode *m_pLeft;    // +0x24
    CSWRoomSurfaceMeshAABBNode *m_pRight;   // +0x28
};

struct CSWRoomFacePair {
    CSWRoomFacePair() { m_nFirst = -1; m_nSecond = -1; }
    CSWRoomFacePair &operator=(const CSWRoomFacePair &o)
    {
        m_nFirst = o.m_nFirst;
        m_nSecond = o.m_nSecond;
        return *this;
    }
    int m_nFirst;
    int m_nSecond;
};

struct CSWCollisionResult {
    unsigned long m_nMask;     // +0x00
    char pad_0x4[0x4];
    int m_bHit;                // +0x08
    int m_nFace;               // +0x0c
    Vector m_vHit;             // +0x10
};

class C3DMatrix;

struct CSWRoomTriangle {
    int m_nVertexA;
    int m_nVertexB;
    int m_nVertexC;
};

class CSWCollisionMesh {
public:
    void TransformToWorld(C3DMatrix *pMatrix);
};

class CSWRoomSurfaceMesh : public CSWCollisionMesh {
public:
    int GetAdjacentSecond(int nIndex);
    void GetTriangleIndices(int nUnused, int nFace, int *pA, int *pB, int *pC);
    double ComputeHeight(int nFace, Vector vPoint);
    int CheckAABBNode(CSWRoomSurfaceMeshAABBNode *pNode, Vector *pStart, Vector *pEnd,
                      CSWCollisionResult *pResult);
    void TransformAABBNode(CSWRoomSurfaceMeshAABBNode *pNode, C3DMatrix *pMatrix);
    int CheckAABBLineOfSight(Vector vStart, Vector vEnd, CSWCollisionResult *pResult);
    int CheckAABBWalkable(Vector vStart, Vector vEnd, CSWCollisionResult *pResult);
    void TransformToWorld(C3DMatrix *pMatrix);

    char pad_0x0[0x4c];
    int m_bTransformed;                         // +0x4c
    char pad_0x50[0x8];
    int m_nFaceCount;                           // +0x58
    char pad_0x5c[0x4];
    CSWRoomTriangle *m_pFaceIndices;            // +0x60
    char pad_0x64[0x4];
    Vector *m_pFaceNormals;                     // +0x68
    float *m_pFaceDistances;                    // +0x6c
    char pad_0x70[0x1c];
    CSWRoomFacePair *m_pPairs;                  // +0x8c
    char pad_0x90[0x20];
    int m_bHasAABB;                             // +0xb0
    char pad_0xb4[0x20];
    CSWRoomSurfaceMeshAABBNode *m_pAABBRoot;    // +0xd4
    unsigned long m_nLOSMask;                   // +0xd8
    char pad_0xdc[0x4];
    unsigned long m_nWalkMask;                  // +0xe0
};

// FUNCTION: SWKOTOR 0x00581150
int CSWRoomSurfaceMesh::GetAdjacentSecond(int nIndex)
{
    return m_pPairs[nIndex].m_nSecond;
}

// FUNCTION: SWKOTOR 0x005806C0
void CSWRoomSurfaceMesh::GetTriangleIndices(int nUnused, int nFace, int *pA, int *pB, int *pC)
{
    if (nFace < m_nFaceCount) {
        *pA = m_pFaceIndices[nFace].m_nVertexA;
        *pB = m_pFaceIndices[nFace].m_nVertexB;
        *pC = m_pFaceIndices[nFace].m_nVertexC;
    }
}

// FUNCTION: SWKOTOR 0x005805E0
double CSWRoomSurfaceMesh::ComputeHeight(int nFace, Vector vPoint)
{
    Vector vLocal(vPoint.x, vPoint.y, -9.0f);
    return PlaneHeightAtPoint(m_pFaceNormals[nFace], m_pFaceDistances[nFace], vLocal);
}

// FUNCTION: SWKOTOR 0x00581170
template void CExoArrayList<class CSWRoomSurfaceMeshAABBNode>::Allocate(int s);

// FUNCTION: SWKOTOR 0x00581290
template void CExoArrayList<class CSWRoomSurfaceMeshAABBNode>::Add(CSWRoomSurfaceMeshAABBNode t);

// FUNCTION: SWKOTOR 0x00581BD0 SYMBOL: ??4?$CExoArrayList@VCSWRoomSurfaceMeshAABBNode@@@@QAEAAV0@ABV0@@Z
template CExoArrayList<CSWRoomSurfaceMeshAABBNode> &
CExoArrayList<CSWRoomSurfaceMeshAABBNode>::operator=(const CExoArrayList<CSWRoomSurfaceMeshAABBNode> &);

// FUNCTION: SWKOTOR 0x00581C30
template void CExoArrayList<class Vector>::Insert(Vector t, int pos);

// FUNCTION: SWKOTOR 0x00581CD0
template void CExoArrayList<struct CSWRoomFacePair>::Allocate(int s);

// FUNCTION: SWKOTOR 0x005814D0
int CSWRoomSurfaceMesh::CheckAABBLineOfSight(Vector vStart, Vector vEnd, CSWCollisionResult *pResult)
{
    if (!m_bHasAABB)
        return 0;
    pResult->m_nMask = m_nLOSMask;
    if (CheckAABBNode(m_pAABBRoot, &vStart, &vEnd, pResult) > 0) {
        pResult->m_bHit = 1;
        pResult->m_vHit = vEnd;
    }
    return pResult->m_bHit;
}

// FUNCTION: SWKOTOR 0x00581530
int CSWRoomSurfaceMesh::CheckAABBWalkable(Vector vStart, Vector vEnd, CSWCollisionResult *pResult)
{
    if (!m_bHasAABB)
        return 0;
    pResult->m_nMask = m_nWalkMask;
    int nHit = CheckAABBNode(m_pAABBRoot, &vStart, &vEnd, pResult);
    if (pResult->m_nFace == -1) {
        Vector vNudge = Normalize(Vector(1.0f, 1.0f, 0.0f));
        vNudge *= 0.001f;
        vStart += vNudge;
        vEnd += vNudge;
        nHit = CheckAABBNode(m_pAABBRoot, &vStart, &vEnd, pResult);
    }
    if (nHit > 0) {
        pResult->m_bHit = 1;
        pResult->m_vHit = vEnd;
    }
    return pResult->m_bHit;
}

// FUNCTION: SWKOTOR 0x00581AF0
void CSWRoomSurfaceMesh::TransformToWorld(C3DMatrix *pMatrix)
{
    if (m_bTransformed)
        return;
    CSWCollisionMesh::TransformToWorld(pMatrix);
    if (m_bHasAABB)
        TransformAABBNode(m_pAABBRoot, pMatrix);
}
