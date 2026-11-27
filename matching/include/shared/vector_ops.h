// vector_ops.h -- inline arithmetic operators on Vector
//
// The original declares these as free functions; the Mac symbol tables name
// operator/(Vector const&, float), and the executable keeps out-of-line
// copies of several (0x00423B90-0x00423BF0, 0x00437A60, 0x00437A90). A unit
// that calls those copies declares the prototype instead of including this.
#ifndef SHARED_VECTOR_OPS_H
#define SHARED_VECTOR_OPS_H

#include "shared/vector.h"

inline Vector operator+(const Vector& a, const Vector& b)
{
    return Vector(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline Vector operator-(const Vector& a, const Vector& b)
{
    return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline Vector operator-(const Vector& v)
{
    return Vector(-v.x, -v.y, -v.z);
}

inline Vector operator*(const Vector& v, float f)
{
    return Vector(v.x * f, v.y * f, v.z * f);
}

// dot product
inline float operator*(const Vector& a, const Vector& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

#endif
