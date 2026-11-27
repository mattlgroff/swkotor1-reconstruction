// gui_vector.h -- GUIVector
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/h/guivirt.cpp); the other copies' members fall on its members.
#ifndef SHARED_GUI_VECTOR_H
#define SHARED_GUI_VECTOR_H


class GUIVector {
public:
    float x, y, z;

    GUIVector() { x = 0.0f; y = 0.0f; z = 0.0f; }
    GUIVector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
};

#endif
