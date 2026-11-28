// _rtl_critical_section.h -- _RTL_CRITICAL_SECTION
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/c/exo_critical_section_internal.cpp); the other copies' members fall on its members.
#ifndef SHARED__RTL_CRITICAL_SECTION_H
#define SHARED__RTL_CRITICAL_SECTION_H


struct _RTL_CRITICAL_SECTION {
    char data[0x18];
};

#endif
