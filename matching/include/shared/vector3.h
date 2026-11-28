// vector3.h -- Vector3
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/i/triangle_bin.h); the other copies' members fall on its members.
#ifndef SHARED_VECTOR3_H
#define SHARED_VECTOR3_H


class Vector3 {
public:
    Vector3()
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }
    Vector3(float fX, float fY, float fZ)
    {
        x = fX;
        y = fY;
        z = fZ;
    }

    float x;
    float y;
    float z;
};

#endif
