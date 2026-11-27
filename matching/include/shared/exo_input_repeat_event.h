// exo_input_repeat_event.h -- CExoInputRepeatEvent
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/c/exo_input.h); the other copies' members fall on its members.
#ifndef SHARED_EXO_INPUT_REPEAT_EVENT_H
#define SHARED_EXO_INPUT_REPEAT_EVENT_H


struct CExoInputRepeatEvent {
    unsigned long m_nEvent;                         // 0x00
    unsigned long m_nInitialDelay;                  // 0x04
    unsigned long m_nRepeatDelay;                   // 0x08
    unsigned long m_nUnknown0c;                     // 0x0c
    unsigned long m_nUnknown10;                     // 0x10
    unsigned long m_nUnknown14;                     // 0x14
    unsigned long m_nUnknown18;                     // 0x18
};

#endif
