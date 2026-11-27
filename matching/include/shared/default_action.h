// default_action.h -- CDefaultAction
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/n/client_input_mouse_click.cpp); the other copies' members fall on its members.
#ifndef SHARED_DEFAULT_ACTION_H
#define SHARED_DEFAULT_ACTION_H


class CDefaultActionTarget;
class CSWCCreature;

struct CDefaultAction {
    char pad_0x0[8];
    unsigned long m_nArgument;
    void (CDefaultActionTarget::*m_pfnAction)(unsigned long nArgument,CSWCCreature* pPlayer);
    char pad_0x14[8];
    unsigned long m_oidTarget;
};

#endif
