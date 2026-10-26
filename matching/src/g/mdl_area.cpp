// MdlNodeTriMesh surface area from the locked vertex pool (0x004388a0).
// Name not recorded; behavioural reference:
// src/units/0945/447e76485a-mdl_postprocess.cpp.
template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    T& operator[](int i) { return element[i]; }
};

class Vector {
public:
    float x, y, z;

    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
};

inline Vector operator-(const Vector& a, const Vector& b)
{
    return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vector __cdecl cross(const Vector& a, const Vector& b);    // 0x004ab210
float __cdecl Magnitude(const Vector& v);                    // 0x004ab100
void* __cdecl PoolLock(int nId, int a, int b, int c);        // 0x00423f10
void __cdecl PoolUnlock(int nId);                            // 0x00423f30

class MdlFace {
public:
    char pad00[0x1a];
    short m_anVertex[3];                    // +0x1a
};

class Model {
public:
    char pad00[0xac];
    int m_nVertexPool;                      // +0xac
};

class MaxTree {
public:
    Model* AsModel();                       // 0x0043e1c0
};

class MdlNodeTriMesh {
public:
    char pad00[0x08];
    MaxTree* m_pTree;                       // +0x08
    char pad0c[0x14c - 0x0c];
    int m_nVertexStride;                    // +0x14c
    char pad150[0x194 - 0x150];
    int m_nVertexOffset;                    // +0x194

    void ComputeSurfaceArea(CExoArrayList<MdlFace>* pFaces, float* pArea);
};

// FUNCTION: SWKOTOR 0x004388a0
// NONMATCH (80): x87 scheduling. The original keeps v0/v1 on the FPU
// stack, copies v2 partly as integers and spills single components; the
// address order is (index * stride + offset) + base. Not retried.
void MdlNodeTriMesh::ComputeSurfaceArea(CExoArrayList<MdlFace>* pFaces, float* pArea)
{
    *pArea = 0;
    char* pBase = (char*)PoolLock(m_pTree->AsModel()->m_nVertexPool, 0, 0, 8);
    int nOffset = m_nVertexOffset;
    for (int i = 0; i < pFaces->num; i++) {
        MdlFace& face = (*pFaces)[i];
        Vector v0 = *(Vector*)(pBase + nOffset + face.m_anVertex[0] * m_nVertexStride);
        Vector v1 = *(Vector*)(pBase + nOffset + face.m_anVertex[1] * m_nVertexStride);
        Vector v2 = *(Vector*)(pBase + nOffset + face.m_anVertex[2] * m_nVertexStride);
        Vector c = cross(v2 - v0, v1 - v0);
        *pArea += Magnitude(c) * 0.5f;
    }
    PoolUnlock(m_pTree->AsModel()->m_nVertexPool);
}
