// Pilot 4 arm B: 0x0050E270, reset of a block of vectors and ids.

class Vector {
public:
    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    float x, y, z;
};

class CSWSPathData {
public:
    void Clear();
    char pad_0x0[0x4];
    Vector m_v04;                               // 0x04
    Vector m_v10;                               // 0x10
    Vector m_v1c;                               // 0x1c
    Vector m_v28;                               // 0x28
    Vector m_v34;                               // 0x34
    Vector m_v40;                               // 0x40
    int m_n4c;                                  // 0x4c
    int m_n50;                                  // 0x50
    Vector m_v54;                               // 0x54
    int m_n60;                                  // 0x60
    int m_n64;                                  // 0x64
    Vector m_v68;                               // 0x68
};

// FUNCTION: SWKOTOR 0x0050E270
void CSWSPathData::Clear()
{
    m_v04 = Vector(0.0f, 0.0f, 0.0f);
    m_v10 = Vector(0.0f, 0.0f, 0.0f);
    m_v1c = Vector(0.0f, 0.0f, 0.0f);
    m_v28 = Vector(0.0f, 0.0f, 0.0f);
    m_v34 = Vector(0.0f, 0.0f, 0.0f);
    m_v40 = Vector(0.0f, 0.0f, 0.0f);
    m_n4c = -1;
    m_n50 = -1;
    m_v54 = Vector(0.0f, 0.0f, 0.0f);
    m_n60 = -1;
    m_n64 = -1;
    m_v68 = Vector(0.0f, 0.0f, 0.0f);
}
