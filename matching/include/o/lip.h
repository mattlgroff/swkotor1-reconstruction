// Worker O matching headers: CLIP, the lip-sync keyframe track
// (0x0070C280-0x0070C6C0), a CResHelper over CResLIP.
#ifndef O_LIP_H
#define O_LIP_H

#include "o/aurora_res_helper.h"

enum LIPViseme {
    LIP_VISEME_EE = 0
};

// Keyframes follow the "LIP V1.0" header, length and count as packed
// 5-byte records: float time, then one viseme byte.
// vtable 0x0075FB10
class CLIP : public CResHelper<CResLIP, 3004> {
public:
    CLIP();
    virtual ~CLIP();
    float GetAnimLength();
    LIPViseme GetVisemeIndex(unsigned long nIndex);
    float GetTimeIndex(unsigned long nIndex);
    int LoadLip(CExoString sLip);

    int m_n1C;                                      // 0x1c
    int m_bLoaded;                                  // 0x20
    unsigned long m_nKeyFrames;                     // 0x24
    float m_fAnimLength;                            // 0x28
    char* m_pKeyFrames;                             // 0x2c
};

#endif
