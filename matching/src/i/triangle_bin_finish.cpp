// CAurTriangleBin::FinishGeometry (0x004A67B0): bounding centre and
// radius from the min/max corners.
#include "i/triangle_bin.h"

Vector3 operator/(const Vector3& v, float f);           // 0x00437A90 (cdecl)
float Magnitude(const Vector3& v);                      // 0x004AB100 (cdecl)

inline Vector3 operator+(const Vector3& a, const Vector3& b)
{
    return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline Vector3 operator-(const Vector3& a, const Vector3& b)
{
    return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

// FUNCTION: SWKOTOR 0x004A67B0
void CAurTriangleBin::FinishGeometry()
{
    m_v2C = (m_v14 + m_v20) / 2.0f;
    m_fRadius = Magnitude(m_v14 - m_v20) * 0.5f;
}
