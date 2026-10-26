// math3d.o leftovers past the vector cluster (0x004AC110-0x004ACCA0).
// Vector here zero-initialises by default, as the original's dead
// stack-zeroing at 0x004AC11D shows.
#ifndef I_MATH3D_H
#define I_MATH3D_H

class Vector {
public:
    Vector()
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }
    Vector(float fX, float fY, float fZ)
    {
        x = fX;
        y = fY;
        z = fZ;
    }

    void Normalize();                                   // 0x004AB130

    float x;
    float y;
    float z;
};

class Quaternion {
public:
    Quaternion()
    {
        z = 0.0f;
        y = 0.0f;
        x = 0.0f;
        w = 1.0f;
    }
    Quaternion(float fW, float fX, float fY, float fZ)
    {
        w = fW;
        x = fX;
        y = fY;
        z = fZ;
    }

    float w;
    float x;
    float y;
    float z;
};

class Plane {
public:
    void Transform(const Vector& vTranslation, const Quaternion& qRotation);  // 0x004AB4C0

    Vector n;
    float d;
};

inline Vector operator-(const Vector& v)
{
    return Vector(-v.x, -v.y, -v.z);
}

Quaternion operator-(const Quaternion& q);                                 // 0x004A9B50
Vector operator*(const Quaternion& q, const Vector& v);                    // 0x004A9BB0

inline Vector operator*(const Vector& v, float f)
{
    return Vector(v.x * f, v.y * f, v.z * f);
}

inline Vector operator*(float f, const Vector& v)
{
    return Vector(v.x * f, v.y * f, v.z * f);
}

inline float operator*(const Vector& a, const Vector& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vector operator+(const Vector& a, const Vector& b)
{
    return Vector(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline Vector operator-(const Vector& a, const Vector& b)
{
    return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vector planelineintersection(const Vector& vNormal, float fDist,
                             const Vector& vStart, const Vector& vEnd);   // 0x004AB250
Vector segmentproject(const Vector& a, const Vector& b, const Vector& p);  // 0x004AB360
Quaternion RotationArc(Vector vFrom, Vector vTo);                          // 0x004AB630
Vector FUN_004ab210(const Vector& a, const Vector& b);                     // 0x004AB210 cross
int operator==(const Vector& a, const Vector& b);                          // 0x004AA980

extern const float g_fZero;                         // 0x0073D700 (0.0f)
extern const float g_fOne;                          // 0x0073D764 (1.0f)

#endif
