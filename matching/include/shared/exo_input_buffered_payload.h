// exo_input_buffered_payload.h -- CExoInputBufferedPayload
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/c/exo_input.h); the other copies' members fall on its members.
#ifndef SHARED_EXO_INPUT_BUFFERED_PAYLOAD_H
#define SHARED_EXO_INPUT_BUFFERED_PAYLOAD_H


class CExoInputBufferedPayload {
public:
    CExoInputBufferedPayload() { m_n4 = 0; m_n8 = 0; m_nC = 0; }
    virtual void Unknown00();

    unsigned long m_n4;                             // 0x04
    unsigned long m_n8;                             // 0x08
    unsigned long m_nC;                             // 0x0c
    unsigned long m_n10;                            // 0x10
    unsigned long m_n14;                            // 0x14
    unsigned long m_nValue1;                        // 0x18
    unsigned long m_nValue2;                        // 0x1c
};

#endif
