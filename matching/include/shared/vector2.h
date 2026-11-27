// vector2.h -- Vector2
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/e/animmesh.cpp); the other copies' members fall on its members.
#ifndef SHARED_VECTOR2_H
#define SHARED_VECTOR2_H


class Vector2 {
public:
    float x, y;

    Vector2() {}
    Vector2(float fX, float fY) { x = fX; y = fY; }
    Vector2(const Vector2& v) { x = v.x; y = v.y; }
    Vector2& operator=(const Vector2& v) { x = v.x; y = v.y; return *this; }
    Vector2 operator+(const Vector2& v) const { return Vector2(x + v.x, y + v.y); }
    Vector2 operator*(float f) const { return Vector2(x * f, y * f); }
};

#endif
