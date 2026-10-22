// CSWCCreatureStats (b00647_creature_levelup_stats).

#include "k/sw_c_creature_stats.h"
#include <string.h>

#pragma auto_inline(off)

// FUNCTION: SWKOTOR 0x006477E0
int CSWCCreatureStats::GetCanUseSkill(unsigned short nSkill)
{
    if (nSkill >= g_pRules->m_nNumSkills) {
        return 0;
    }
    if (g_pRules->m_lstSkills[nSkill].m_nFlags & 2) {
        for (unsigned char i = 0; i < m_nNumMultiClasses; i++) {
            if (GetClassObject(i)->IsSkillUseable(nSkill)) {
                return 1;
            }
        }
        return 0;
    }
    return GetSkillRank(nSkill) > 0;
}

// FUNCTION: SWKOTOR 0x00647AE0
int CSWCCreatureStats::CalculateNumAttacks()
{
    int nBAB = 0;
    for (int i = 0; i < m_nNumMultiClasses; i++) {
        nBAB += GetClassObject(i)->GetAttackBonus(GetClassLevel(i));
    }
    int nAttacks = (nBAB + 4) / 5;
    if (nAttacks == 0) {
        nAttacks = 1;
    }
    return nAttacks;
}

// FUNCTION: SWKOTOR 0x00648150
char CSWCCreatureStats::GetSTRMod()
{
    return (char)floor(m_nSTR * 0.5) - 5;
}

// FUNCTION: SWKOTOR 0x00648180
char CSWCCreatureStats::GetDEXMod()
{
    return (char)((char)floor(m_nDEX * 0.5) - 5) + m_nArmorDexBonus;
}

// FUNCTION: SWKOTOR 0x006481C0
char CSWCCreatureStats::GetCONMod()
{
    return (char)floor(m_nCON * 0.5) - 5;
}

// FUNCTION: SWKOTOR 0x006481F0
char CSWCCreatureStats::GetINTMod()
{
    return (char)floor(m_nINT * 0.5) - 5;
}

// FUNCTION: SWKOTOR 0x00648220
char CSWCCreatureStats::GetWISMod()
{
    return (char)floor(m_nWIS * 0.5) - 5;
}

// FUNCTION: SWKOTOR 0x00648250
char CSWCCreatureStats::GetCHAMod()
{
    return (char)floor(m_nCHA * 0.5) - 5;
}

// FUNCTION: SWKOTOR 0x00648280
char CSWCCreatureStats::GetINTBaseMod()
{
    return (char)floor(m_nINTBase * 0.5) - 5;
}

// FUNCTION: SWKOTOR 0x00648430
unsigned char CSWCCreatureStats::GetTotalLevel()
{
    int nLevel = 0;
    for (int i = 0; i < m_nNumMultiClasses; i++) {
        nLevel += GetClassLevel(i);
    }
    return nLevel;
}

// FUNCTION: SWKOTOR 0x00648830
char CSWCCreatureStats::GetSkillModifier(unsigned short nSkill)
{
    switch (g_pRules->m_lstSkills[nSkill].m_nKeyAbility) {
    case 0:
        return GetSTRMod();
    case 1:
        return GetDEXMod();
    case 2:
        return GetCONMod();
    case 3:
        return GetINTMod();
    case 4:
        return GetWISMod();
    case 5:
        return GetCHAMod();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00649380
void CSWCCreatureStats::UpdateAbilityModifiedSkillRanks()
{
    for (unsigned short i = 0; i < g_pRules->m_nNumSkills; i++) {
        if (!(g_pRules->m_lstSkills[i].m_nFlags & 2) && GetSkillRank(i) == 0) {
            SetModifiedSkillRank(i, 0);
        } else {
            SetModifiedSkillRank(i, GetSkillRank(i) + GetSkillModifier(i));
        }
    }
}

// FUNCTION: SWKOTOR 0x00649D60
// NONMATCH (9 diff insns): the original sign-extends the first STR modifier
// (`movsx eax, al`) before the byte store; no source form tried keeps it.
void CSWCCreatureStats::InitializeCombatInformation()
{
    CSWCCombatInformation* pInfo = m_pCombatInformation;
    if (pInfo) {
        pInfo->m_nNumAttacks = CalculateNumAttacks();
        pInfo->SetOnHandAttackMod((char)floor(m_nSTR * 0.5) - 5);
        pInfo->m_nOnHandDamageMod = (char)floor(m_nSTR * 0.5) - 5;
        {
        int nValue;
        g_pRules->m_p2DArrays->m_pRacialTypes->GetINTEntry(
            g_pRules->m_lstRaces[m_nRace].m_nRacialTypeRow, g_nRacialTypeColumn, &nValue);
        }
        pInfo->m_n08 = 0;
        {
        int nValue;
        g_pRules->m_p2DArrays->m_pRacialTypes->GetINTEntry(
            g_pRules->m_lstRaces[m_nRace].m_nRacialTypeRow, g_nRacialTypeColumn, &nValue);
        }
        pInfo->m_n09 = 0;
        pInfo->m_n0a = 1;
    }
}

// FUNCTION: SWKOTOR 0x00649F90
void CSWCCreatureStats::AddKnownSpell(unsigned char nClassIndex, unsigned long nSpell)
{
    if (nClassIndex < 2) {
        if (g_pRules->m_pSpellArray->GetSpell(nSpell)) {
            m_ClassInfo[nClassIndex].AddKnownSpell(0, nSpell);
        }
    }
}

// FUNCTION: SWKOTOR 0x00649FE0
void CSWCCreatureStats::RemoveKnownSpell(unsigned char nClassIndex, unsigned long nSpell)
{
    if (nClassIndex < 2) {
        if (g_pRules->m_pSpellArray->GetSpell(nSpell)) {
            CExoArrayList<unsigned long>* pList = m_ClassInfo[nClassIndex].m_pKnownSpellList;
            for (int i = 0; i < pList->num; i++) {
                if (pList->element[i] == nSpell) {
                    if (i >= 0) {
                        m_ClassInfo[nClassIndex].RemoveKnownSpell(nSpell);
                    }
                    return;
                }
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x006482B0
int CSWCCreatureStats::HasFeat(unsigned short nFeat)
{
    int nFound = 0;
    for (int i = 0; i < m_lstFeats.num; i++) {
        if (m_lstFeats.element[i] == nFeat) {
            nFound++;
        }
    }
    if (nFound) {
        return 1;
    }
    nFound = 0;
    for (int j = 0; j < m_lstBonusFeats.num; j++) {
        if (m_lstBonusFeats.element[j] == nFeat) {
            nFound++;
        }
    }
    if (!nFound) {
        return 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00648320
void CSWCCreatureStats::ClearFeats()
{
    while (m_lstFeats.num != 0) {
        m_lstFeats.DelIndex(m_lstFeats.num - 1);
    }
    while (m_lstBonusFeats.num != 0) {
        m_lstBonusFeats.DelIndex(0);
    }
    while (m_lstFeatUses.num != 0) {
        free(m_lstFeatUses.element[0]);
        m_lstFeatUses.DelIndex(0);
    }
}

// FUNCTION: SWKOTOR 0x006486E0
unsigned short CSWCCreatureStats::GetFeat(unsigned short nIndex)
{
    if (nIndex >= m_lstFeats.num) {
        return 0xffff;
    }
    return m_lstFeats.element[nIndex];
}

// FUNCTION: SWKOTOR 0x006487D0
unsigned short CSWCCreatureStats::GetNumberKnownSpells(unsigned char nClassIndex, unsigned char nLevel)
{
    if (nClassIndex >= 2) {
        return 0;
    }
    return m_ClassInfo[nClassIndex].m_pKnownSpellList[nLevel].num;
}

// FUNCTION: SWKOTOR 0x00648800
unsigned char CSWCCreatureStats::GetSpellsPerDayLeft(unsigned char nClassIndex, unsigned char nLevel)
{
    if (nClassIndex >= 2) {
        return 0;
    }
    return m_ClassInfo[nClassIndex].GetSpellsPerDayLeft(nLevel);
}

// FUNCTION: SWKOTOR 0x006488B0
void CSWCCreatureStats_ClassInfo::ClearKnownSpells()
{
    for (int i = m_pKnownSpellList[0].num - 1; i >= 0; i--) {
        m_pKnownSpellList[0].DelIndex(i);
    }
}

// FUNCTION: SWKOTOR 0x006492C0
int CSWCCreatureStats::HasSpell(unsigned char nLevel, unsigned long nSpell)
{
    for (unsigned char i = 0; i < m_nNumMultiClasses; i++) {
        CSWCCreatureStats_ClassInfo* pInfo = &m_ClassInfo[i];
        for (int j = 0; j < pInfo->GetNumberKnownSpells(nLevel); j++) {
            if (pInfo->m_pKnownSpellList[nLevel].element[j] == nSpell) {
                return 1;
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00649340
int CSWCCreatureStats::HasNormalFeat(unsigned short nFeat)
{
    for (int i = 0; i < m_lstFeats.num; i++) {
        if (m_lstFeats.element[i] == nFeat) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00649430
void CSWCCreatureStats::SetFeatRemainingUses(unsigned short nFeat, unsigned char nUses)
{
    int nCount = m_lstFeatUses.num;
    for (int i = 0; i < nCount; i++) {
        if (m_lstFeatUses.element[i]->m_nFeat == nFeat) {
            m_lstFeatUses.element[i]->m_nUsesLeft = nUses;
            return;
        }
    }
    CSWCFeatUses* pUses = new CSWCFeatUses;
    pUses->m_nFeat = nFeat;
    pUses->m_nUsesLeft = nUses;
    if (m_lstFeatUses.num == m_lstFeatUses.array_size) {
        int nSize = m_lstFeatUses.array_size ? m_lstFeatUses.array_size * 2 : 16;
        CSWCFeatUses** pOld = m_lstFeatUses.element;
        m_lstFeatUses.array_size = nSize;
        m_lstFeatUses.element = new CSWCFeatUses*[nSize];
        for (int j = 0; j < m_lstFeatUses.num; j++) {
            m_lstFeatUses.element[j] = pOld[j];
        }
        if (pOld) {
            free(pOld);
        }
    }
    m_lstFeatUses.element[m_lstFeatUses.num++] = pUses;
}

// FUNCTION: SWKOTOR 0x00649520
void CSWCCreatureStats::ClearSpells()
{
    for (unsigned char i = 0; i < m_nNumMultiClasses; i++) {
        CExoArrayList<unsigned long>* pList = m_ClassInfo[i].m_pKnownSpellList;
        for (int j = pList->num - 1; j >= 0; j--) {
            pList->DelIndex(j);
        }
    }
}

// FUNCTION: SWKOTOR 0x00649580
void CSWCCreatureStats::ClearKnownSpells(unsigned char nClassIndex)
{
    if (nClassIndex < 2) {
        m_ClassInfo[nClassIndex].ClearKnownSpells();
    }
}

// FUNCTION: SWKOTOR 0x006495A0
// NONMATCH (17 diff insns): the original computes &m_ClassInfo[n] before the
// level test and keeps two separate -1 returns (`mov eax,-1` / `or eax,-1`).
unsigned long CSWCCreatureStats::GetKnownSpell(unsigned char nClassIndex, unsigned char nLevel, unsigned char nIndex)
{
    if (nClassIndex < 2) {
        CSWCCreatureStats_ClassInfo* pInfo = &m_ClassInfo[nClassIndex];
        if (nLevel < 1) {
            CExoArrayList<unsigned long>* pList = &pInfo->m_pKnownSpellList[nLevel];
            if (nIndex >= pList->num) {
                return 0xffffffff;
            }
            return pList->element[nIndex];
        }
    }
    return 0xffffffff;
}

// FUNCTION: SWKOTOR 0x00649660
void CSWCCreatureStats_ClassInfo::AddKnownSpell(unsigned char nLevel, unsigned long nSpell)
{
    if (nLevel < 1) {
        if (m_pKnownSpellList[nLevel].IndexOf(nSpell) == -1) {
            m_pKnownSpellList[nLevel].Add(nSpell);
        }
    }
}

// FUNCTION: SWKOTOR 0x0064A050
CSWCCreatureStats_ClassInfo::CSWCCreatureStats_ClassInfo()
{
    m_nClass = 0;
    m_nLevel = 0;
    m_n19 = 0;
    m_n1a = 0xff;
    m_n1b = 0xff;
    memset(m_nSpellsPerDayLeft, 0, sizeof(m_nSpellsPerDayLeft));
}

#pragma auto_inline(on)
