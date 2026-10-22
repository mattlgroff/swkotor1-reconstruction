// Particle helpers (0x0048D870, 0x0048D9E0).

#include <stdlib.h>
#include <math.h>

class Vector {
public:
    float x, y, z;

    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    Vector operator+(const Vector& v) const { return Vector(x + v.x, y + v.y, z + v.z); }
    Vector operator-(const Vector& v) const { return Vector(x - v.x, y - v.y, z - v.z); }
    Vector operator*(float f) const { return Vector(x * f, y * f, z * f); }
};

class Quaternion {
public:
    float w, x, y, z;

    // Rotated +Z axis.
    Vector GetZAxis() const
    {
        return Vector(2.0f * (x * z + y * w),
                      2.0f * (y * z - w * x),
                      1.0f - 2.0f * (y * y + x * x));
    }
};

class Emitter {
public:
    char pad_0x0[0x128];
    Vector m_vXAxis;            // +0x128
    Vector m_vYAxis;            // +0x134
    Vector m_vTarget;           // +0x140
    char pad_0x14c[0x18];
    Vector m_vPosition;         // +0x164
    Vector m_vLastPosition;     // +0x170
    char pad_0x17c[0x38];
    Quaternion m_qOrientation;  // +0x1b4
};

class Particle {
public:
    void* m_pVtbl;              // +0x00
    Emitter* m_pEmitter;        // +0x04
    Vector m_vPosition;         // +0x08
    float pad_0x14;
    float m_fSpeed;             // +0x18
    Vector m_vVelocity;         // +0x1c

    void randomPosition(int nWidth, int nHeight);
    void inheritVelocity(float fDeltaTime);
};

// FUNCTION: SWKOTOR 0x0048D870
void Particle::randomPosition(int nWidth, int nHeight)
{
    nWidth = nWidth > 0 ? (rand() * rand()) % (int)(nWidth * 0.5f) : 0;
    nHeight = nHeight > 0 ? (rand() * rand()) % (int)(nHeight * 0.5f) : 0;
    if (rand() % 2 > 0)
        nWidth = -nWidth;
    if (rand() % 2 > 0)
        nHeight = -nHeight;
    Emitter* pEmitter = m_pEmitter;
    m_vPosition = pEmitter->m_vXAxis * (nWidth * 0.0001f) + pEmitter->m_vPosition
                  + pEmitter->m_vYAxis * (nHeight * 0.0001f);
}

// NONMATCH (9 insns): the GetZAxis x term computes x*z before y*w whatever
// the source order, and the normalize multiplies y/z through the stored
// pointer instead of this.
// FUNCTION: SWKOTOR 0x0048D9E0
void Particle::inheritVelocity(float fDeltaTime)
{
    Emitter* pEmitter = m_pEmitter;
    Vector vTarget = pEmitter->m_vTarget - pEmitter->m_qOrientation.GetZAxis();
    Vector vOffset = pEmitter->m_vPosition - m_vPosition;
    float fInv = fDeltaTime > 0.0f ? 1.0f / fDeltaTime : 0.01f;
    float fDist = (float)sqrt(vOffset.x * vOffset.x + vOffset.y * vOffset.y + vOffset.z * vOffset.z);
    m_vVelocity = (pEmitter->m_vPosition - pEmitter->m_vLastPosition) * fInv
                  + vTarget * fDist * fInv;
    m_fSpeed = (float)sqrt(m_vVelocity.x * m_vVelocity.x + m_vVelocity.y * m_vVelocity.y
                     + m_vVelocity.z * m_vVelocity.z);
    if (m_fSpeed != 0.0f)
        m_vVelocity = m_vVelocity * (1.0f / m_fSpeed);
    else
        m_vVelocity = Vector(0.0f, 0.0f, 0.0f);
}
