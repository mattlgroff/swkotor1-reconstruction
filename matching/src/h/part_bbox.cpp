// World-space bounding box of a static tile part (0x00451DC0).

class Vector {
public:
    float x, y, z;

    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
};

class Quaternion {
public:
    float w, x, y, z;
};

Vector operator*(const Quaternion& q, const Vector& v);    // 0x004A9BB0

inline Vector operator+(const Vector& a, const Vector& b)
{
    Vector v;
    v.x = a.x + b.x;
    v.y = a.y + b.y;
    v.z = a.z + b.z;
    return v;
}
float Round(float fValue, float fStep);                     // 0x004A96E0
Vector MinVector(Vector a, Vector b);                       // 0x004AAE70
Vector MaxVector(Vector a, Vector b);                       // 0x004AAEE0
void* PoolLock(int nId, int nUnused1, int nUnused2, int nFlags);   // 0x00423F10
void PoolUnlock(int nId);                                   // 0x00423F30

class Model {
public:
    char pad_0x0[0xac];
    int m_nVertexPool;                      // +0xac
};

class MaxTree {
public:
    Model* AsModel();                       // 0x0043E1C0
};

class MdlNodeTriMesh {
public:
    char pad_0x0[8];
    MaxTree* m_pTree;                       // +0x08
    char pad_0xc[0x140];
    int m_nVertexStride;                    // +0x14c
    char pad_0x150[4];
    int m_nPositionOffset;                  // +0x154
    char pad_0x158[0x28];
    short m_nVertices;                      // +0x180
    char pad_0x182[0x12];
    int m_nPoolOffset;                      // +0x194
};

class PartVertices {
public:
    char pad_0x0[4];
    MdlNodeTriMesh* m_pNode;                // +0x04
};

class Part {
public:
    char pad_0x0[8];
    Vector m_vPosition;                     // +0x08
    Quaternion m_qOrientation;              // +0x14
    char pad_0x24[0x28];
    Vector m_vBMin;                         // +0x4c
    Vector m_vBMax;                         // +0x58
    char pad_0x64[0xc];
    PartVertices* m_pVertices;              // +0x70
};

// NONMATCH: identical apart from where the caller cleanup `add esp, 0x10`
// after the MinVector call is scheduled (two instructions earlier in the
// original).
// FUNCTION: SWKOTOR 0x00451DC0
void ComputePartBBox(Part* pPart)
{
    MdlNodeTriMesh* pNode = pPart->m_pVertices->m_pNode;
    Vector vMax(-100000.0f, -100000.0f, -100000.0f);
    Vector vMin(100000.0f, 100000.0f, 100000.0f);
    int nPool = pNode->m_pTree->AsModel()->m_nVertexPool;
    char* pPool = (char*)PoolLock(nPool, 0, 0, 8);
    int nOffset = pNode->m_nPoolOffset;
    for (int i = 0; i < pNode->m_nVertices; i++) {
        char* pVertex = pPool + nOffset + pNode->m_nVertexStride * i;
        Vector* p = (Vector*)(pVertex + pNode->m_nPositionOffset);
        Vector v(p->x, p->y, p->z);
        Vector w = pPart->m_vPosition + pPart->m_qOrientation * v;
        Vector s(Round(w.x, 0.001f), Round(w.y, 0.001f), Round(w.z, 0.001f));
        vMin = MinVector(vMin, s);
        vMax = MaxVector(vMax, s);
    }
    PoolUnlock(pNode->m_pTree->AsModel()->m_nVertexPool);
    pPart->m_vBMin = vMin;
    pPart->m_vBMax = vMax;
}
