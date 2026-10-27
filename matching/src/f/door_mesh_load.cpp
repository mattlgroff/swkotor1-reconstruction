// CSWDoorSurfaceMesh::LoadMeshBinary (round s8, 0x005CE770). Kept apart from
// res_mesh.cpp: the original calls the out-of-line CResDWK::GetUsePoint that
// res_mesh.cpp defines, which cl would otherwise inline.

class Vector {
public:
    Vector() { x = 0.0f; y = 0.0f; z = 0.0f; }
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    float x, y, z;
};

class CRes {
public:
    virtual ~CRes();
    char pad_0x4[0x24];
};

class CResBWM : public CRes {
public:
};

struct CResDWKHeader {
    char pad_0x0[0x24];
    Vector m_avUse[2];               // +0x24
};

class CResDWK : public CRes {
public:
    Vector GetUsePoint(int n);
    Vector PeekUsePoint(int n) { return m_pHeader ? m_pHeader->m_avUse[n] : Vector(0.0f, 0.0f, 0.0f); }

    char pad_0x28[0x8];
    char *m_pData;                   // +0x30
    CResDWKHeader *m_pHeader;        // +0x34
};

class CSWCollisionMesh {
public:
    virtual ~CSWCollisionMesh();
    int LoadMeshBinary(CResBWM *pRes);

    char pad_0x4[0x6c];
    Vector m_avUse[2];               // +0x70
};

class CSWDoorSurfaceMesh : public CSWCollisionMesh {
public:
    int LoadMeshBinary(CResBWM *pRes);

    int m_nDoorState;                // +0x88
};

// FUNCTION: SWKOTOR 0x005CE770
int CSWDoorSurfaceMesh::LoadMeshBinary(CResBWM *pRes)
{
    CSWCollisionMesh::LoadMeshBinary(pRes);
    CResDWK *pDWK = (CResDWK *)pRes;
    Vector v = pDWK->PeekUsePoint(0);
    if (v.x != 0.0f || v.y != 0.0f || v.z != 0.0f)
        m_avUse[0] = pDWK->GetUsePoint(0);
    v = pDWK->PeekUsePoint(1);
    if (v.x != 0.0f || v.y != 0.0f || v.z != 0.0f)
        m_avUse[1] = pDWK->GetUsePoint(1);
    return 1;
}
