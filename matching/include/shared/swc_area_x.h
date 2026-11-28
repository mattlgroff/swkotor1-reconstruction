// swc_area_x.h -- CSWCAreaX
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/c/sw_c_creature_vslots_s4.cpp); the other copies' members fall on its members.
#ifndef SHARED_SWC_AREA_X_H
#define SHARED_SWC_AREA_X_H


class CSWSAreaX;
class Vector;

struct CSWCAreaX {
    char pad_0x0[0x104];
    unsigned long m_nId104;                         // 0x104

    // declared by other copies
    CSWSAreaX* GetServerArea();
    void UpdatePosition(const Vector& v);
};

#endif
