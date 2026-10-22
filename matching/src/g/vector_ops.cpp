// Vector scalar operators, the 3x3 Matrix constructors, the
// emitter post-process hook and the CAurMeshBuffer constructor.

class Vector {
public:
    float x, y, z;

    Vector() { x = 0; y = 0; z = 0; }
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
};

// FUNCTION: SWKOTOR 0x00437a60 SYMBOL: ??D@YA?AVVector@@MABV0@@Z
Vector operator*(float f, const Vector& v)
{
    return Vector(f * v.x, f * v.y, f * v.z);
}

// FUNCTION: SWKOTOR 0x00437a90 SYMBOL: ??K@YA?AVVector@@ABV0@M@Z
Vector operator/(const Vector& v, float f)
{
    if (f != 0.0f) {
        f = 1.0f / f;
        return Vector(f * v.x, f * v.y, f * v.z);
    }
    return v;
}

class Matrix {
public:
    Vector m_vRow0;
    Vector m_vRow1;
    Vector m_vRow2;

    Matrix();
    Matrix(Vector vRow0, Vector vRow1, Vector vRow2);
};

// FUNCTION: SWKOTOR 0x00437af0 SYMBOL: ??0Matrix@@QAE@XZ
Matrix::Matrix()
{
    m_vRow0 = Vector(1.0f, 0.0f, 0.0f);
    m_vRow1 = Vector(0.0f, 1.0f, 0.0f);
    m_vRow2 = Vector(0.0f, 0.0f, 1.0f);
}

// FUNCTION: SWKOTOR 0x00437ba0 SYMBOL: ??0Matrix@@QAE@VVector@@00@Z
Matrix::Matrix(Vector vRow0, Vector vRow1, Vector vRow2)
{
    m_vRow0 = vRow0;
    m_vRow1 = vRow1;
    m_vRow2 = vRow2;
}

class Model {
public:
    char pad00[0x80];
    float m_fAnimScale;                     // +0x80
};

class MaxTree {
public:
    Model* AsModel();                       // 0x0043e1c0
};

class MdlNodeEmitter {
public:
    char pad00[0x08];
    MaxTree* m_pTree;                       // +0x08

    void PostProcess();
};

// FUNCTION: SWKOTOR 0x00437c10
void MdlNodeEmitter::PostProcess()
{
    Model* pModel = m_pTree->AsModel();
    if (pModel && pModel->m_fAnimScale < 40.0f)
        pModel->m_fAnimScale = 40.0f;
}

class CAurMeshBuffer {
public:
    virtual ~CAurMeshBuffer();
    int m_n04;                              // +0x04
    int m_n08;                              // +0x08

    CAurMeshBuffer();
};

// FUNCTION: SWKOTOR 0x0044ee00
CAurMeshBuffer::CAurMeshBuffer()
{
    m_n08 = 0;
    m_n04 = 0;
}
