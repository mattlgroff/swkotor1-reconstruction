// sw_boolean_array.h -- CSWBooleanArray
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/f/vartable.cpp); the other copies' members fall on its members.
#ifndef SHARED_SW_BOOLEAN_ARRAY_H
#define SHARED_SW_BOOLEAN_ARRAY_H


class CSWBooleanArray {
public:
    CSWBooleanArray() { for (int i = 0; i < 3; i++) m_a[i] = 0; }
    unsigned long m_a[3];
};

#endif
