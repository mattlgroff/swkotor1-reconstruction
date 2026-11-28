// rumble_channel.h -- CRumbleChannel
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/k/sw_c_rumble_pattern.cpp); the other copies' members fall on its members.
#ifndef SHARED_RUMBLE_CHANNEL_H
#define SHARED_RUMBLE_CHANNEL_H


class CRumbleChannel {
public:
    float* m_pfMagnitudes;                          // 0x00
    float* m_pfTimes;                               // 0x04
    int m_nCount;                                   // 0x08
    int m_nCursor;                                  // 0x0C

    float GetMagnitude(float fTime, int bLoop);     // 0x0068FCB0
    float GetTime(int nIndex)
    {
        if (nIndex >= 0 && nIndex < m_nCount)
            return m_pfTimes[nIndex];
        return 0.0f;
    }
};

#endif
