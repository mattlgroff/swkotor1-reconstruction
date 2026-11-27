// aabb.h -- AABB
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/g/mdl_ctor.h); the other copies' members fall on its members.
#ifndef SHARED_AABB_H
#define SHARED_AABB_H


class AABB {
public:
    char pad00[0x18];
    AABB* m_pLeft;              // +0x18
    AABB* m_pRight;             // +0x1c

    ~AABB()
    {
        if (m_pLeft)
            delete m_pLeft;
        if (m_pRight)
            delete m_pRight;
    }
};

#endif
