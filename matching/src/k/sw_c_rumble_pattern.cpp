// Controller rumble patterns: two keyframed magnitude channels (left/right).
// CRumbleChannel and its method names are descriptive; the layout follows the
// 0x0068FCB0 / 0x0068FDD0 accesses (CRumblePattern +0x20 is m_bExclusive in
// include/c/client_exo_app.h, used here as the looping flag).

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

class CRumblePattern {
public:
    CRumbleChannel m_cLeft;                         // 0x00
    CRumbleChannel m_cRight;                        // 0x10
    int m_bExclusive;                               // 0x20

    int GetIntensity(float fTime, float* pfLeft, float* pfRight); // 0x0068FDD0
};

// FUNCTION: SWKOTOR 0x0068FCB0
float CRumbleChannel::GetMagnitude(float fTime, int bLoop)
{
    if (!m_pfTimes || !m_pfMagnitudes)
        return 0.0f;
    if (bLoop) {
        while (fTime > m_pfTimes[m_nCount - 1])
            fTime -= m_pfTimes[m_nCount - 1];
    } else if (fTime > m_pfTimes[m_nCount - 1]) {
        return 0.0f;
    }
    int found = 0;
    int i;
    for (i = m_nCursor; i < m_nCount - 1; i++) {
        if (found)
            break;
        if (fTime >= m_pfTimes[i] && fTime < m_pfTimes[i + 1]) {
            found = 1;
            m_nCursor = i;
        }
    }
    for (i = 0; i < m_nCursor; i++) {
        if (found)
            break;
        if (fTime >= m_pfTimes[i] && fTime < m_pfTimes[i + 1]) {
            found = 1;
            m_nCursor = i;
        }
    }
    if (!found)
        return 0.0f;
    return (fTime - m_pfTimes[m_nCursor]) / (m_pfTimes[m_nCursor + 1] - m_pfTimes[m_nCursor]) *
               (m_pfMagnitudes[m_nCursor + 1] - m_pfMagnitudes[m_nCursor]) +
           m_pfMagnitudes[m_nCursor];
}

// FUNCTION: SWKOTOR 0x0068FDD0
int CRumblePattern::GetIntensity(float fTime, float* pfLeft, float* pfRight)
{
    *pfLeft = m_cLeft.GetMagnitude(fTime, m_bExclusive);
    *pfRight = m_cRight.GetMagnitude(fTime, m_bExclusive);
    if (m_bExclusive)
        return 1;
    if (fTime < m_cLeft.GetTime(m_cLeft.m_nCount - 1))
        return 1;
    if (fTime < m_cRight.GetTime(m_cRight.m_nCount - 1))
        return 1;
    return 0;
}
