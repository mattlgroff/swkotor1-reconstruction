// sws2_d_arrays.h -- CSWS2DArrays
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/i/sws_object.h); the other copies' members fall on its members.
#ifndef SHARED_SWS2_D_ARRAYS_H
#define SHARED_SWS2_D_ARRAYS_H


class C2DA;

struct CSWS2DArrays {
    char pad_0x0[0xc];
    C2DA * m_pAppearanceTable;                   // 0xc
    char pad_0x10[0x28];
    C2DA* m_pPortraits;                             // 0x38 (placeholder name)
};

#endif
