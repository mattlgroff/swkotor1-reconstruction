// CSWCCreatureStats (b0062 message leaves).

class CCombatInformation {
public:
    ~CCombatInformation();                          // 0x00550E00
};

class CSWCCreatureStats {
public:
    void SetCombatInformation(CCombatInformation* pInfo);

    char pad_0x0[0x68];
    CCombatInformation* m_pCombatInformation;       // 0x68
    int m_bCombatInformationSet;                    // 0x6c
};

// FUNCTION: SWKOTOR 0x00647A90
void CSWCCreatureStats::SetCombatInformation(CCombatInformation* pInfo)
{
    if (m_pCombatInformation) {
        delete m_pCombatInformation;
    }
    if (!m_bCombatInformationSet) {
        m_bCombatInformationSet = 1;
    }
    m_pCombatInformation = pInfo;
}
