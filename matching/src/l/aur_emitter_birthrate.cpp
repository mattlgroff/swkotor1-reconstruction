// Existing callers identify CAurEmitter::SetBirthrateA/B. The timer is
// adjusted only while below the previous interval and nonzero.
// Best attempt: each setter differs only in the integer-load scheduling
// around the second floating comparison; x87 operation order is preserved.
class CAurEmitter {
public:
    char pad_0x0[0x4c];
    float m_fBirthrateA;
    float m_fTimerA;
    char pad_0x54[0x20];
    float m_fBirthrateB;
    float m_fTimerB;
    void SetBirthrateA(float fRate);
    void SetBirthrateB(float fRate);
};

// FUNCTION: SWKOTOR 0x006DAF60
void CAurEmitter::SetBirthrateA(float fRate)
{
    if (m_fTimerA < m_fBirthrateA) {
        if (m_fTimerA != 0.0f) {
            m_fTimerA -= m_fBirthrateA - fRate;
        }
        m_fBirthrateA = fRate;
    } else {
        m_fBirthrateA = fRate;
    }
}

// FUNCTION: SWKOTOR 0x006DAFA0
void CAurEmitter::SetBirthrateB(float fRate)
{
    if (m_fTimerB < m_fBirthrateB) {
        if (m_fTimerB != 0.0f) {
            m_fTimerB -= m_fBirthrateB - fRate;
        }
        m_fBirthrateB = fRate;
    } else {
        m_fBirthrateB = fRate;
    }
}
