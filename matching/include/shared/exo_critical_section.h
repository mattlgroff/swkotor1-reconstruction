// exo_critical_section.h -- CExoCriticalSection
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/c/exo_file_thread.cpp); the other copies' members fall on its members.
#ifndef SHARED_EXO_CRITICAL_SECTION_H
#define SHARED_EXO_CRITICAL_SECTION_H

#include "shared/exo_critical_section_internal.h"

class CExoCriticalSection {
public:
    CExoCriticalSectionInternal* m_pcExoCriticalSectionInternal;   // 0x00

    CExoCriticalSection();                          // 0x005E9290
    ~CExoCriticalSection();                         // 0x005E9320
    void EnterCriticalSection();                    // 0x005E9300
    void LeaveCriticalSection();                    // 0x005E9310
};

#endif
