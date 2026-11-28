// exo_loc_string_st.h -- ExoLocString_st
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/c/exo_array_list_dtor.cpp); the other copies' members fall on its members.
#ifndef SHARED_EXO_LOC_STRING_ST_H
#define SHARED_EXO_LOC_STRING_ST_H

#include "shared/exo_string.h"

struct ExoLocString_st {
    int m_nLanguage;                                // 0x00
    CExoString m_sString;                           // 0x04

    ~ExoLocString_st();                             // 0x0059EA20
};

#endif
