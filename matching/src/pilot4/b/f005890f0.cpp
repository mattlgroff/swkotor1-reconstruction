// Pilot 4 arm B: 0x005890F0 CSWSDoor::NoNonWalkPolysInDoor.

class Vector {
public:
    float x, y, z;
};

class Quaternion {
public:
    float w, x, y, z;
};

class CSWCollisionMesh {
public:
    int NoNonWalkPolys(float fX1, float fY1, float fX2, float fY2, float fZ,
                       int bFlag);              // 0x005972B0
    char pad_0x0[0x20];
    Vector m_vPosition;                         // +0x20
    char pad_0x2c[0xc];
    Quaternion m_qOrientation;                  // +0x38
    void SetOrientation(Quaternion q) { m_qOrientation = q; }
};

class CSWSDoor {
public:
    int NoNonWalkPolysInDoor(float fX1, float fY1, float &fX2, float &fY2,
                             float &fZ, int bFlag);
    char pad_0x0[0x90];
    Vector m_vPosition;                         // +0x90
    char pad_0x9c[0x230];
    unsigned char m_nOpenState;                 // +0x2cc
    char pad_0x2cd[0x5f];
    CSWCollisionMesh *m_apWalkMeshes[3];        // +0x32c
    char pad_0x338[0x78];
    Quaternion m_qOrientation;                  // +0x3b0
};

// FUNCTION: SWKOTOR 0x005890F0
int CSWSDoor::NoNonWalkPolysInDoor(float fX1, float fY1, float &fX2, float &fY2,
                                   float &fZ, int bFlag)
{
    if (!m_apWalkMeshes[m_nOpenState]) {
        return 0;
    }
    m_apWalkMeshes[m_nOpenState]->m_vPosition = m_vPosition;
    m_apWalkMeshes[m_nOpenState]->SetOrientation(m_qOrientation);
    return m_apWalkMeshes[m_nOpenState]->NoNonWalkPolys(fX1, fY1, fX2, fY2, fZ, bFlag);
}
