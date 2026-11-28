// sws_object_spell_view.h -- CSWSObjectSpellView
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/b/vm_commands_get_object.cpp); the other copies' members fall on its members.
#ifndef SHARED_SWS_OBJECT_SPELL_VIEW_H
#define SHARED_SWS_OBJECT_SPELL_VIEW_H


struct CSWSObjectSpellView {
    char pad_0x0[0x1bc];
    unsigned long m_oidSpellTarget;                         // 0x1bc
    int m_nSpellId;                                         // 0x1c0
    char pad_0x1c4[0x10];
    unsigned char  m_nLastSpellCastClass;        // 0x1d4
    char pad_0x1d5[0x3];
    unsigned long m_oidLastSpellCaster;                     // 0x1d8
    int m_nLastSpellId;                                     // 0x1dc
    int m_bLastSpellHarmful;                                // 0x1e0
};

#endif
