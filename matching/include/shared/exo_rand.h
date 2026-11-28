// exo_rand.h -- CExoRand
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/c/exo_rand.cpp); the other copies' members fall on its members.
#ifndef SHARED_EXO_RAND_H
#define SHARED_EXO_RAND_H


class CExoRandInternal;

class CExoRand {
public:
    CExoRandInternal* m_pcExoRandInternal;          // 0x00

    CExoRand();
    ~CExoRand();

    // declared by other copies
    unsigned long Rand();
};

#endif
