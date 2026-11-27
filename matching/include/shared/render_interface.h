// render_interface.h -- RenderInterface
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/c/sw_c_creature_model_s7.cpp); the other copies' members fall on its members.
#ifndef SHARED_RENDER_INTERFACE_H
#define SHARED_RENDER_INTERFACE_H


class RenderInterface {
public:
    static int IsResident(char* sModel);            // 0x0070D980
};

#endif
