// CSWCCreatureStats / CSWCLevelUpStats leaves from nwccreaturestats.o and the
// two CSWSpell chain-code helpers placed in front of them.

#include "m/sw_c_creature_stats.h"

class CSWSpellChain {
public:
    unsigned long GetChainCode(int nIndex);         // 0x0059B6A0
    int GetChainCodeUnits(int nIndex);
    int GetSortKey(int nIndex);
};

// Descriptive name: the units digit of the spell's chain code.
// FUNCTION: SWKOTOR 0x006474D0
int CSWSpellChain::GetChainCodeUnits(int nIndex)
{
    return GetChainCode(nIndex) - GetChainCode(nIndex) / 10 * 10;
}

// FUNCTION: SWKOTOR 0x00647510
int CSWSpellChain::GetSortKey(int nIndex)
{
    return (int)(GetChainCode(nIndex) - GetChainCode(nIndex) / 1000 * 1000) / 10;
}

// Descriptive name: stores the unmodified skill rank (+0x88).
// FUNCTION: SWKOTOR 0x00647590
void CSWCCreatureStats::SetBaseSkillRank(unsigned short nSkill, unsigned char nRank)
{
    if (nSkill < g_pRules->m_nNumSkills) {
        m_pSkillRanks[nSkill] = (char)nRank;
    }
}

// Descriptive name: ability modifier of an arbitrary score.
// FUNCTION: SWKOTOR 0x00647630
char CSWCCreatureStats::GetAbilityMod(unsigned char nScore)
{
    return (char)floor(nScore * 0.5) - 5;
}

// FUNCTION: SWKOTOR 0x006478A0
void CSWCLevelUpStats::SetINTBase(unsigned char nINT)
{
    m_nINTBase = nINT;
    m_nINT = g_pRules->m_lstRaces[m_nRace].m_nINTAdjust + nINT;
}

// FUNCTION: SWKOTOR 0x006478D0
void CSWCLevelUpStats::SetCHABase(unsigned char nCHA)
{
    m_nCHABase = nCHA;
    m_nCHA = g_pRules->m_lstRaces[m_nRace].m_nCHAAdjust + nCHA;
}

// FUNCTION: SWKOTOR 0x006495F0
CSWCCreatureStats_ClassInfo::~CSWCCreatureStats_ClassInfo()
{
    ClearKnownSpells();
}
