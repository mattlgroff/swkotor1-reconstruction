struct Vector {
    Vector() { x = 0.0f; y = 0.0f; z = 0.0f; }
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    float x;
    float y;
    float z;
};

class CSWRoomSurfaceMeshHitInfo {
public:
    unsigned long m_nMask;          // +0x0
    unsigned long m_nField4;        // +0x4
    unsigned long m_nHit;           // +0x8
    int m_nFace;                    // +0xc
    Vector m_vPoint;                // +0x10
    unsigned long m_nBit;           // +0x1c

    void ClearOutput();
    void ClearAll(unsigned long nMask);
};

// FUNCTION: SWKOTOR 0x00580750
void CSWRoomSurfaceMeshHitInfo::ClearOutput()
{
    m_nHit = 0;
    m_nBit = 0;
    m_vPoint = Vector(0.0f, 0.0f, 0.0f);
}

// FUNCTION: SWKOTOR 0x00580780
void CSWRoomSurfaceMeshHitInfo::ClearAll(unsigned long nMask)
{
    m_nMask = nMask;
    m_nFace = -1;
    m_nField4 = 0;
    m_nHit = 0;
    m_nBit = 0;
    m_vPoint = Vector(0.0f, 0.0f, 0.0f);
}

struct Quaternion {
    float w;
    float x;
    float y;
    float z;
};

class CSWRoomSurfaceMeshOrientation {
public:
    char pad_0x0[0x38];
    Quaternion m_qOrientation;      // +0x38

    void SetOrientation(const Quaternion *pOrientation);
};

// FUNCTION: SWKOTOR 0x00580700
void CSWRoomSurfaceMeshOrientation::SetOrientation(const Quaternion *pOrientation)
{
    m_qOrientation = *pOrientation;
}
