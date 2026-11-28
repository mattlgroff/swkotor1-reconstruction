// exo_critical_section_internal.h -- CExoCriticalSectionInternal
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/c/exo_critical_section_internal.cpp); the other copies' members fall on its members.
#ifndef SHARED_EXO_CRITICAL_SECTION_INTERNAL_H
#define SHARED_EXO_CRITICAL_SECTION_INTERNAL_H

#include "shared/_rtl_critical_section.h"

class CExoCriticalSectionInternal {
public:
    _RTL_CRITICAL_SECTION m_cCriticalSection;       // 0x00

    CExoCriticalSectionInternal();
    ~CExoCriticalSectionInternal();
    void EnterCriticalSection();
    void LeaveCriticalSection();
};

#endif
