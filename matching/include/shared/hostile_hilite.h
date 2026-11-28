// hostile_hilite.h -- CHostileHilite
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/c/client_exo_app.h); the other copies' members fall on its members.
#ifndef SHARED_HOSTILE_HILITE_H
#define SHARED_HOSTILE_HILITE_H


struct CHostileHilite {
    unsigned long m_oidTarget;                      // 0x00
    float m_fTimeLeft;                              // 0x04
};

#endif
