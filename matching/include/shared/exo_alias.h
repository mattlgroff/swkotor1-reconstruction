// exo_alias.h -- CExoAlias
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/c/exo_alias_list_internal_s5.cpp); the other copies' members fall on its members.
#ifndef SHARED_EXO_ALIAS_H
#define SHARED_EXO_ALIAS_H

#include "shared/exo_string.h"

class CExoAlias {
public:
    CExoString m_sAlias;                            // 0x00
    CExoString m_sPath;                             // 0x08
    CExoAlias* m_pNext;                             // 0x10
    CExoAlias* m_pPrev;                             // 0x14

    CExoAlias();                                    // 0x005EB600
};

#endif
