// sw_class_skill.h -- CSWClassSkill
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/f/class_getters.cpp); the other copies' members fall on its members.
#ifndef SHARED_SW_CLASS_SKILL_H
#define SHARED_SW_CLASS_SKILL_H


struct CSWClassSkill {
    unsigned short m_nSkill;         // +0x0
    char pad_0x2[0x2];
    int m_nValue;                    // +0x4
    int m_nLevel;                    // +0x8
};

#endif
