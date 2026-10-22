// CSWSClient lifetime (CSWSSysAdmin in sysadmin.cpp), door / placeable surface meshes,
// CSWCollisionMesh::WorldToLocal, CResMDL / CResBWM / CResDWK (0x005802F0-0x005CEB50).

#include <string.h>

class Vector {
public:
    Vector() { x = 0.0f; y = 0.0f; z = 0.0f; }
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    Vector operator-(const Vector &v) const { return Vector(x - v.x, y - v.y, z - v.z); }
    float x, y, z;
};

class Quaternion {
public:
    Quaternion() { x = y = z = 0.0f; w = 1.0f; }
    float w, x, y, z;
};

Quaternion operator~(const Quaternion &q);
Vector operator*(const Quaternion &q, const Vector &v);

// ---------------------------------------------------------------------------
// CSWSClient / CSWSSysAdmin

class CSWSClient {
public:
    CSWSClient(unsigned long nPlayerId);
    virtual ~CSWSClient();

    unsigned long m_nPlayerId;       // +0x04
    int m_n08;                       // +0x08
};

// FUNCTION: SWKOTOR 0x005CDA10
CSWSClient::CSWSClient(unsigned long nPlayerId)
{
    m_nPlayerId = nPlayerId;
    m_n08 = 0;
}

// FUNCTION: SWKOTOR 0x005CDA30
CSWSClient::~CSWSClient()
{
}

// ---------------------------------------------------------------------------
// CSWCollisionMesh and the door / placeable surface meshes

class CRes {
public:
    CRes();
    virtual ~CRes();

    char pad_0x4[0xc];
    char *m_pResource;               // +0x10
    char pad_0x14[0x4];
    int m_nSize;                     // +0x18
    char pad_0x1c[0x4];
    int m_n20;                       // +0x20
    char pad_0x24[0x4];
};

class CSWCollisionMesh {
public:
    CSWCollisionMesh(unsigned short nA, unsigned short nB);
    virtual ~CSWCollisionMesh();
    Vector WorldToLocal(const Vector &v);

    int m_bTransformed;              // +0x04
    char pad_0x8[0x18];
    Vector m_vPosition;              // +0x20
    Vector m_vOffset;                // +0x2c
    Quaternion m_qOrientation;       // +0x38
    char pad_0x48[0x28];
    Vector m_avUse[2];               // +0x70
};

// FUNCTION: SWKOTOR 0x00596A00
Vector CSWCollisionMesh::WorldToLocal(const Vector &v)
{
    if (m_bTransformed)
        return v;
    Vector vLocal(v.x - m_vPosition.x, v.y - m_vPosition.y, v.z - m_vPosition.z);
    return ~m_qOrientation * vLocal - m_vOffset;
}

class CSWPlaceableSurfaceMesh : public CSWCollisionMesh {
public:
    CSWPlaceableSurfaceMesh();
    virtual ~CSWPlaceableSurfaceMesh();
    void SetUsePoint(int n, const Vector &v);
};

// FUNCTION: SWKOTOR 0x005CDA60
void CSWPlaceableSurfaceMesh::SetUsePoint(int n, const Vector &v)
{
    m_avUse[n] = WorldToLocal(v);
}

// FUNCTION: SWKOTOR 0x005CDAA0
CSWPlaceableSurfaceMesh::CSWPlaceableSurfaceMesh() : CSWCollisionMesh(2002, 2053)
{
}

class CSWDoorSurfaceMesh : public CSWCollisionMesh {
public:
    CSWDoorSurfaceMesh();
    virtual ~CSWDoorSurfaceMesh();
    void SetUsePoint(unsigned char n, Vector v);
};

// FUNCTION: SWKOTOR 0x005CE070
void CSWDoorSurfaceMesh::SetUsePoint(unsigned char n, Vector v)
{
    m_avUse[n] = WorldToLocal(v);
}

// FUNCTION: SWKOTOR 0x005CE0B0
CSWDoorSurfaceMesh::CSWDoorSurfaceMesh() : CSWCollisionMesh(2002, 2052)
{
}

// FUNCTION: SWKOTOR 0x005CE0D0
CSWDoorSurfaceMesh::~CSWDoorSurfaceMesh()
{
}

// ---------------------------------------------------------------------------
// CResDWK use points

struct CResDWKHeader {
    char pad_0x0[0x24];
    Vector m_avUse[2];               // +0x24
};

class CResDWK : public CRes {
public:
    Vector GetUsePoint(int n);

    char pad_0x28[0x8];
    char *m_pData;                   // +0x30
    CResDWKHeader *m_pHeader;        // +0x34
};

// FUNCTION: SWKOTOR 0x005CE010
Vector CResDWK::GetUsePoint(int n)
{
    return m_pHeader ? m_pHeader->m_avUse[n] : Vector(0.0f, 0.0f, 0.0f);
}

// ---------------------------------------------------------------------------
// CResMDL / CResBWM

class CResMDL : public CRes {
public:
    CResMDL();
    virtual ~CResMDL();

    int m_n28;                       // +0x28
    int m_n2C;                       // +0x2c
    int m_n30;                       // +0x30
};

// FUNCTION: SWKOTOR 0x005CEA50
CResMDL::CResMDL()
{
    m_n28 = 0;
    m_n2C = 0;
    m_n30 = 0;
    m_n20 = 1;
}

// FUNCTION: SWKOTOR 0x005CEA80
CResMDL::~CResMDL()
{
}

// FUNCTION: SWKOTOR 0x005CEA90 SYMBOL: ??_GCResMDL@@UAEPAXI@Z

class CResBWM : public CRes {
public:
    CResBWM();
    virtual ~CResBWM();
    virtual int OnResourceServiced();
    int IsBinary();

    int m_bLoaded;                   // +0x28
    int m_nDataSize;                 // +0x2c
    char *m_pData;                   // +0x30
    char *m_pHeader;                 // +0x34
};

// FUNCTION: SWKOTOR 0x005CEAB0
CResBWM::CResBWM()
{
    m_bLoaded = 0;
}

// FUNCTION: SWKOTOR 0x005CEAD0
CResBWM::~CResBWM()
{
}

// FUNCTION: SWKOTOR 0x005CEAE0
int CResBWM::OnResourceServiced()
{
    if (!m_pResource)
        return 0;
    if (m_bLoaded)
        return 1;
    m_nDataSize = m_nSize;
    m_pData = m_pResource;
    m_pHeader = m_pResource;
    if (strncmp(m_pResource, "BWM V1.0", 8))
        m_pHeader = 0;
    m_bLoaded = 1;
    return 1;
}

// FUNCTION: SWKOTOR 0x005CEB30
int CResBWM::IsBinary()
{
    return strncmp(m_pData, "BWM V1.0", 8) == 0;
}

// FUNCTION: SWKOTOR 0x005CEB50 SYMBOL: ??_GCResBWM@@UAEPAXI@Z
