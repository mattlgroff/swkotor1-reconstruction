// CSWCCreatureStats skill-rank access and reset.
#include <string.h>

class CSWRules {
public:
    char pad_0x0[0xab];
    unsigned char m_nNumSkills;
};

extern CSWRules* g_pRules;

class CSWCCreatureStats {
public:
    void SetSkillRank(unsigned short nSkill, unsigned char nRank);
    void ClearSkills();
    char GetSkillRank(unsigned short nSkill);

    char pad_0x0[0x84];
    char* m_pModifiedSkillRanks;                    // 0x84
    char* m_pSkillRanks;                            // 0x88
};

// FUNCTION: SWKOTOR 0x00647560
void CSWCCreatureStats::SetSkillRank(unsigned short nSkill, unsigned char nRank)
{
    if (nSkill < g_pRules->m_nNumSkills) {
        m_pModifiedSkillRanks[nSkill] = (char)nRank;
    }
}

// FUNCTION: SWKOTOR 0x006475E0
void CSWCCreatureStats::ClearSkills()
{
    memset(m_pModifiedSkillRanks, 0, g_pRules->m_nNumSkills);
    memset(m_pSkillRanks, 0, g_pRules->m_nNumSkills);
}

// FUNCTION: SWKOTOR 0x006477B0
char CSWCCreatureStats::GetSkillRank(unsigned short nSkill)
{
    if (nSkill >= g_pRules->m_nNumSkills) {
        return 0;
    }
    return m_pModifiedSkillRanks[nSkill];
}
