// client_unknown700.h -- CClientUnknown700
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/c/client_exo_app.h); the other copies' members fall on its members.
#ifndef SHARED_CLIENT_UNKNOWN700_H
#define SHARED_CLIENT_UNKNOWN700_H


class CClientUnknown700 {
public:
    int Unknown61b780();                            // 0x0061B780
    char pad_0x0[0xc];
    float m_fToolTipDelay;                          // 0x0c
    char pad_0x10[0x4];
    unsigned char m_nFeedbackFlags;                 // 0x14 (descriptive; 0x10 = floaty text)

    float GetToolTipDelay() { return m_fToolTipDelay; }
    float GetGamma(int b);                          // 0x0061D590

    // declared by other copies
    float GetValue();
};

#endif
