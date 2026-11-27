// nearest_struct.h -- CNearestStruct
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/c/client_exo_app_leaves_s3.cpp); the other copies' members fall on its members.
#ifndef SHARED_NEAREST_STRUCT_H
#define SHARED_NEAREST_STRUCT_H


struct CNearestStruct {
    unsigned long m_oidObject;                      // 0x00
    float m_fDistance;                              // 0x04
    unsigned long m_nType;                          // 0x08
};

#endif
