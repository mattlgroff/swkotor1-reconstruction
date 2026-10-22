// Vector: three floats (engine math type).
#ifndef B_VECTOR_H
#define B_VECTOR_H

struct Vector {
    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }

    float x;
    float y;
    float z;
};

int operator==(const Vector& a, const Vector& b);     // 0x004AA980
Vector operator-(const Vector& a, const Vector& b);   // 0x00423BC0

#endif
