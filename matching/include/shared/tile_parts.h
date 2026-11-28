// tile_parts.h -- TileParts
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/h/scenebsp.cpp); the other copies' members fall on its members.
#ifndef SHARED_TILE_PARTS_H
#define SHARED_TILE_PARTS_H


template <class T> class CExoArrayList;
class TilePart;

class TileParts {
public:
    void * m_pMainLight1;                        // 0x0
    void * m_pMainLight2;                        // 0x4
    TilePart * m_pSecondLight1;                  // 0x8
    TilePart * m_pSecondLight2;                  // 0xc
    char pad_0x10[0x8];
    CExoArrayList<TilePart*> m_lParts;  // +0x18
};

#endif
