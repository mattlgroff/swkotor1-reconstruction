// CSWCLevelUpStats (b00647_creature_levelup_stats).

#include "k/sw_c_creature_stats.h"
#include <string.h>

#pragma auto_inline(off)

// FUNCTION: SWKOTOR 0x00647900
int CSWCLevelUpStats::GetSkillPointCostForSkillRank(unsigned short nSkill, int bIncrease)
{
    int nCost;
    int bUseable = 0;
    int bClassSkill = 0;
    nCost = 0;
    CSWSkill* pSkill = &g_pRules->m_lstSkills[nSkill];
    if (!pSkill) {
        return 0;
    }
    if (pSkill->m_nFlags & 8) {
        bUseable = 1;
    }
    for (int i = 0; i < m_nNumMultiClasses; i++) {
        CSWClass* pClass = GetClassObject(i);
        if (pClass && pClass->IsSkillUseable(nSkill)) {
            bUseable = 1;
            if (pClass->IsClassSkill(nSkill)) {
                bClassSkill = 1;
                break;
            }
        }
    }
    if (bUseable) {
        nCost = bClassSkill ? 1 : 2;
        if (!bIncrease) {
            nCost = -nCost;
        }
    }
    return nCost;
}

// FUNCTION: SWKOTOR 0x006479F0
int CSWCLevelUpStats::GetBaseAttackBonus()
{
    unsigned char nBAB = 0;
    for (unsigned char i = 0; i < m_nNumMultiClasses; i++) {
        nBAB += GetClassObject(i)->GetAttackBonus(GetClassLevel(i));
    }
    return nBAB;
}

// FUNCTION: SWKOTOR 0x006488E0
void CSWCLevelUpStats::SetSkillRankBase(unsigned short nSkill, char nRank)
{
    if (nSkill < g_pRules->m_nNumSkills) {
        m_pSkillRanks[nSkill] = nRank;
        if (!(g_pRules->m_lstSkills[nSkill].m_nFlags & 2) && GetSkillRank(nSkill) == 0) {
            SetModifiedSkillRank(nSkill, 0);
            return;
        }
        char nMod;
        switch (g_pRules->m_lstSkills[nSkill].m_nKeyAbility) {
        case 0:
            nMod = GetSTRMod();
            break;
        case 1:
            nMod = GetDEXMod();
            break;
        case 2:
            nMod = GetCONMod();
            break;
        case 3:
            nMod = GetINTMod();
            break;
        case 4:
            nMod = GetWISMod();
            break;
        case 5:
            nMod = GetCHAMod();
            break;
        }
        SetModifiedSkillRank(nSkill, GetSkillRank(nSkill) + nMod);
    }
}

// FUNCTION: SWKOTOR 0x00648A00
void CSWCLevelUpStats::AddSkillPointsPerLevel()
{
    CSWClass* pClass = GetClassObject(m_nNumMultiClasses - 1);
    if (pClass) {
        unsigned char nTotal = GetTotalLevel();
        unsigned char nBase = pClass->m_nSkillPointBase;
        if (nTotal == 1) {
            int nPoints;
            if ((char)((char)floor(m_nINTBase * 0.5) - 5) + nBase / 2 > 1) {
                nBase = pClass->m_nSkillPointBase;
                nPoints = GetINTBaseMod() + nBase / 2;
            } else {
                nPoints = 1;
            }
            m_nSkillPoints = nPoints * 4;
        } else {
            int nPoints;
            if (((char)((char)floor(m_nINTBase * 0.5) - 5) + nBase) / 2 > 1) {
                nBase = pClass->m_nSkillPointBase;
                nPoints = (GetINTBaseMod() + nBase) / 2;
            } else {
                nPoints = 1;
            }
            m_nSkillPoints += nPoints;
        }
    }
}

// FUNCTION: SWKOTOR 0x00648B30
void CSWCLevelUpStats::SetFortSavingThrow()
{
    unsigned char nSave = 0;
    for (unsigned char i = 0; i < m_nNumMultiClasses; i++) {
        CSWClass* pClass = GetClassObject(i);
        if (pClass) {
            nSave += pClass->GetFortSaveBonus(GetClassLevel(i));
        }
    }
    m_nFortSave = nSave + ((char)floor(m_nCON * 0.5) - 5);
}

// FUNCTION: SWKOTOR 0x00648BE0
void CSWCLevelUpStats::SetWillSavingThrow()
{
    unsigned char nSave = 0;
    for (unsigned char i = 0; i < m_nNumMultiClasses; i++) {
        CSWClass* pClass = GetClassObject(i);
        if (pClass) {
            nSave += pClass->GetWillSaveBonus(GetClassLevel(i));
        }
    }
    m_nWillSave = nSave + ((char)floor(m_nWIS * 0.5) - 5);
}

// FUNCTION: SWKOTOR 0x00648C90
void CSWCLevelUpStats::SetReflexSavingThrow()
{
    unsigned char nSave = 0;
    for (unsigned char i = 0; i < m_nNumMultiClasses; i++) {
        CSWClass* pClass = GetClassObject(i);
        if (pClass) {
            nSave += pClass->GetRefSaveBonus(GetClassLevel(i));
        }
    }
    m_nReflexSave = nSave + ((char)((char)floor(m_nDEX * 0.5) - 5) + m_nArmorDexBonus);
}

// FUNCTION: SWKOTOR 0x00648D50
int CSWCLevelUpStats::IncreaseSkillRank(unsigned short nSkill)
{
    int bUseable = 0;
    int bClassSkill = 0;
    CSWSkill* pSkill = &g_pRules->m_lstSkills[nSkill];
    if (!pSkill) {
        return 0;
    }
    if (pSkill->m_nFlags & 8) {
        bUseable = 1;
    }
    for (int i = 0; i < m_nNumMultiClasses; i++) {
        CSWClass* pClass = GetClassObject(i);
        if (pClass && pClass->IsSkillUseable(nSkill)) {
            bUseable = 1;
            if (pClass->IsClassSkill(nSkill)) {
                bClassSkill = 1;
                break;
            }
        }
    }
    if (!bClassSkill) {
        if (!bUseable || m_nSkillPoints < 2) {
            return 0;
        }
    } else if (m_nSkillPoints < 1) {
        return 0;
    }
    unsigned char nMax = GetTotalLevel() + 3;
    if (!bClassSkill) {
        nMax = nMax / 2;
    }
    if (GetSkillRank(nSkill) >= nMax) {
        return 0;
    }
    SetSkillRankBase(nSkill, GetSkillRank(nSkill) + 1);
    if (bClassSkill) {
        m_nSkillPoints--;
    } else {
        m_nSkillPoints -= 2;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00648EB0
void CSWCLevelUpStats::DecreaseSkillRank(unsigned short nSkill, unsigned char nMinRank)
{
    int bUseable = 0;
    int bClassSkill = 0;
    CSWSkill* pSkill = &g_pRules->m_lstSkills[nSkill];
    if (!pSkill) {
        return;
    }
    if (pSkill->m_nFlags & 8) {
        bUseable = 1;
    }
    for (int i = 0; i < m_nNumMultiClasses; i++) {
        CSWClass* pClass = GetClassObject(i);
        if (pClass && pClass->IsSkillUseable(nSkill)) {
            bUseable = 1;
            if (pClass->IsClassSkill(nSkill)) {
                bClassSkill = 1;
                break;
            }
        }
    }
    if (!bUseable) {
        return;
    }
    if (GetSkillRank(nSkill) == nMinRank) {
        return;
    }
    SetSkillRankBase(nSkill, GetSkillRank(nSkill) - 1);
    if (bClassSkill) {
        m_nSkillPoints++;
    } else {
        m_nSkillPoints += 2;
    }
}

// FUNCTION: SWKOTOR 0x00648FE0
void CSWCLevelUpStats::GetPackageFeatsLeft(unsigned char* pnNormal, unsigned char* pnBonus)
{
    unsigned char n = m_nNumMultiClasses - 1;
    CSWClass* pClass = GetClassObject(n);
    unsigned char nLevel = GetClassLevel(n);
    *pnNormal = pClass->GetNormalFeatsPerLevel(nLevel);
    *pnBonus = pClass->GetBonusFeatsPerLevel(nLevel);
}

// FUNCTION: SWKOTOR 0x00649070
// NONMATCH (8 diff insns): the merged `return 0` block sits after the null
// test in the original (later failures jump back); ours puts it last.
unsigned char CSWCLevelUpStats::ComputeNumberKnownSpellsLeft()
{
    unsigned char n = m_nNumMultiClasses - 1;
    unsigned char nClass = GetClass(n);
    CSWClass* pClass = &g_pRules->m_lstClasses[nClass];
    if (!pClass) {
        return 0;
    }
    if (nClass != CLASS_TYPE_JEDI_CONSULAR && nClass != CLASS_TYPE_JEDI_GUARDIAN &&
        nClass != CLASS_TYPE_JEDI_SENTINEL) {
        return 0;
    }
    {
    unsigned char nLeft = pClass->GetSpellsKnownPerLevel(GetClassLevel(n), 0);
    return nLeft != 0xff ? nLeft : (unsigned char)0;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00649110
void CSWCLevelUpStats::SetNormalBonusFlags(unsigned short nFeat, int* pbNormal, int* pbBonus)
{
    *pbNormal = 0;
    *pbBonus = 0;
    if (g_pRules->GetFeat(nFeat)) {
        CSWClass* pClass = GetClassObject(m_nNumMultiClasses - 1);
        if (pClass) {
            if (pClass->IsNormalFeat(nFeat)) {
                *pbNormal = 1;
            }
            if (pClass->IsBonusFeat(nFeat)) {
                *pbBonus = 1;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x006491A0
void CSWCLevelUpStats::AddDefaultPackageSkills(unsigned char nClass, int bAll)
{
    CSWClass* pClass = &g_pRules->m_lstClasses[nClass];
    int nPoints = m_nSkillPoints;
    int nNumSkills = g_pRules->m_nNumSkills;
    int i = 1;
    while (nPoints > 0 && i <= nNumSkills) {
        int nSkill = pClass->GetPackageSkill(i);
        if (nSkill != -1) {
            CSWSkill* pSkill;
            if (bAll ||
                (((pSkill = &g_pRules->m_lstSkills[(unsigned short)nSkill])->m_nFlags & 0x10) &&
                 (m_nRace != RACIAL_TYPE_DROID || (pSkill->m_nFlags & 0x20)))) {
                while (IncreaseSkillRank(nSkill)) {
                }
                nPoints = m_nSkillPoints;
            }
        }
        i++;
    }
}

// FUNCTION: SWKOTOR 0x00649260
int CSWCLevelUpStats::HasFeat(unsigned short nFeat, CExoArrayList<unsigned short>* pList)
{
    if (nFeat == 0xffff) {
        return 0;
    }
    if (CSWCCreatureStats::HasFeat(nFeat)) {
        return 1;
    }
    if (pList) {
        for (unsigned short i = 0; i < pList->num; i++) {
            if (nFeat == pList->element[i]) {
                return 1;
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x006496D0
void CSWCLevelUpStats::SetCONBase(unsigned char nCON)
{
    m_nCONBase = nCON;
    m_nCON = g_pRules->m_lstRaces[m_nRace].m_nCONAdjust + nCON;
    if (GetTotalLevel() == 1 && !m_bIsPC) {
        CSWClass* pClass = GetClassObject(m_nNumMultiClasses - 1);
        if (pClass) {
            short nBase = m_nBaseHitPoints;
            short nHP = (short)GetCONMod() + nBase;
            if (nHP < 1) {
                nHP = 1;
            }
            m_nMaxHitPoints = nHP;
            m_nCurrentHitPoints = nHP;
        }
    }
    SetFortSavingThrow();
}

// FUNCTION: SWKOTOR 0x00649770
void CSWCLevelUpStats::SetWISBase(unsigned char nWIS)
{
    m_nWISBase = nWIS;
    m_nWIS = g_pRules->m_lstRaces[m_nRace].m_nWISAdjust + nWIS;
    SetWillSavingThrow();
}

// FUNCTION: SWKOTOR 0x006497A0
int CSWCLevelUpStats::HasSpellPrereq(CSWSpell* pSpell, CExoArrayList<unsigned long>* pList)
{
    int nId;
    int i;
    for (i = 0; (nId = pSpell->m_lstPrereqSpells[i]) != -1; i++) {
        if (!HasSpell(0, nId)) {
            if (pList->IndexOf(nId) == -1) {
                return 0;
            }
        }
    }
    for (i = 0; (nId = pSpell->m_lstPrereqSpells2[i]) != -1; i++) {
        if (!HasSpell(0, nId)) {
            if (pList->IndexOf(nId) == -1) {
                return 0;
            }
        }
    }
    return pSpell->GetSpellLevel(GetClass(m_nNumMultiClasses - 1)) <= GetTotalLevel();
}

// FUNCTION: SWKOTOR 0x006498A0
int CSWCLevelUpStats::HasFeatPrereq(CSWFeat* pFeat, CExoArrayList<unsigned short>* pList)
{
    if (pFeat->m_nRequiredFeat1 != 0xffff && !HasFeat(pFeat->m_nRequiredFeat1, pList)) {
        return 0;
    }
    if (pFeat->m_nRequiredFeat2 != 0xffff && !HasFeat(pFeat->m_nRequiredFeat2, pList)) {
        return 0;
    }
    int bHasOrReq = 0;
    int bFound = 0;
    for (int i = 0; !bFound && i < 5; i++) {
        unsigned short nReq = (unsigned char)i < 5 ? pFeat->m_lstOrReqFeat[(unsigned char)i] : 0xffff;
        if (nReq != 0xffff) {
            bHasOrReq = 1;
            if (HasFeat(nReq, pList)) {
                bFound = 1;
            }
        }
    }
    if (bHasOrReq && !bFound) {
        return 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00649950
void CSWCLevelUpStats::AddGrantedFeats(CSWCLevelUpFeatList* pList)
{
    unsigned char nLevel;
    CSWClass* pClass;
    CSWRace* pRace = &g_pRules->m_lstRaces[m_nRace];
    for (int i = 0; i < g_pRules->m_nNumFeats; i++) {
        if (g_pRules->GetFeat(i)) {
            int nTotal = 0;
            for (int j = 0; j < m_nNumMultiClasses; j++) {
                nTotal += GetClassLevel(j);
            }
            if ((unsigned char)nTotal == 1 && !m_bIsPC && pRace->IsFirstLevelGrantedFeat(i)) {
                m_lstFeats.AddUnique(i);
                if (pList) {
                    pList->AddFeat(i);
                }
            } else {
                pClass = GetClassObject(m_nNumMultiClasses - 1);
                if (pClass && pClass->IsGrantedFeat(i, nLevel) &&
                    nLevel == GetClassLevel(m_nNumMultiClasses - 1)) {
                    m_lstFeats.AddUnique(i);
                    if (pList) {
                        pList->AddFeat(i);
                    }
                }
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x00649A70
// NONMATCH (3 diff insns): the original calls AddGrantedFeats without
// reloading ECX after ClearSpells (cross-function register use), skip.
void CSWCLevelUpStats::ClearPackage()
{
    m_nSkillPoints = 0;
    memset(m_pModifiedSkillRanks, 0, g_pRules->m_nNumSkills);
    memset(m_pSkillRanks, 0, g_pRules->m_nNumSkills);
    UpdateAbilityModifiedSkillRanks();
    ClearFeats();
    ClearSpells();
    AddGrantedFeats(0);
    while (m_lstResRefs.num != 0) {
        m_lstResRefs.DelIndex(m_lstResRefs.num - 1);
    }
}

// FUNCTION: SWKOTOR 0x00649B50
// NONMATCH (10 diff insns): code identical except the merged `return 0`
// block placement (first in the original, last here), as ComputeNumberKnownSpellsLeft.
int CSWCLevelUpStats::CanChooseSpell(unsigned long nSpell, unsigned char nKnownLeft, CExoArrayList<unsigned long>* pList)
{
    unsigned char n = m_nNumMultiClasses - 1;
    CSWSpell* pSpell = g_pRules->m_pSpellArray->GetSpell(nSpell);
    if (!pSpell) {
        return 0;
    }
    if (pSpell->m_nUserType != 1 && pSpell->m_nUserType != 5) {
        return 0;
    }
    if (!nKnownLeft) {
        return 0;
    }
    for (unsigned short i = 0; i < GetNumberKnownSpellsInline(n, 0); i++) {
        if (nSpell == GetKnownSpellInline(n, 0, (unsigned char)i)) {
            return 0;
        }
    }
    for (unsigned short j = 0; j < pList->num; j++) {
        if (nSpell == pList->element[j]) {
            return 0;
        }
    }
    if (HasSpellPrereq(pSpell, pList)) {
        return nKnownLeft - pList->num > 0;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00649C30
// NONMATCH (39 diff insns): this/pClass register swap, lazy `push ebp`, and
// the original keeps the alignment value (mov al,3 / setge) instead of folding it.
unsigned char CSWCLevelUpStats::AddDefaultPackageSpells(CExoArrayList<unsigned long>* pList)
{
    unsigned char nLeft = ComputeNumberKnownSpellsLeft();
    unsigned char nClass = GetClass(m_nNumMultiClasses - 1);
    int nNumSpells = g_pRules->m_pSpellArray->m_nNumSpells;
    CSWClass* pClass = &g_pRules->m_lstClasses[nClass];
    int nAdded = 0;
    int i = 1;
    if (pClass->m_bSpellCaster) {
        int bAlign = 1;
        if (GetSimpleAlignment() == 3) {
            bAlign = 0;
        }
        int nMax = nLeft;
        while (nAdded < nMax && i <= nNumSpells) {
            unsigned short nSpell = pClass->GetPackageSpell(bAlign, i);
            if (nSpell != -1) {
                if (CanChooseSpell(nSpell, nLeft, pList)) {
                    if (pList->num == pList->array_size) {
                        pList->Allocate(pList->array_size ? pList->array_size * 2 : 16);
                    }
                    pList->element[pList->num] = nSpell;
                    pList->num++;
                    nAdded++;
                }
                i++;
            }
        }
    }
    return nLeft;
}

// FUNCTION: SWKOTOR 0x0064A0D0
void CSWCLevelUpStats::SetSTRBase(unsigned char nSTR)
{
    m_nSTRBase = nSTR;
    m_nSTR = g_pRules->m_lstRaces[m_nRace].m_nSTRAdjust + nSTR;
    InitializeCombatInformation();
}

// FUNCTION: SWKOTOR 0x0064A100
void CSWCLevelUpStats::SetDEXBase(unsigned char nDEX, int bApply)
{
    int nAC = m_nACNaturalBase - (char)((char)floor(m_nDEX * 0.5) + m_nArmorDexBonus - 5);
    m_nDEXBase = nDEX;
    m_nDEX = g_pRules->m_lstRaces[m_nRace].m_nDEXAdjust + nDEX;
    SetReflexSavingThrow();
    if (bApply) {
        m_nACNaturalBase = (short)(char)((char)floor(m_nDEX * 0.5) + m_nArmorDexBonus - 5) + nAC;
    } else {
        m_nACNaturalBase = (short)(char)((char)floor(m_nDEX * 0.5) + m_nArmorDexBonus - 5) + 10;
    }
    InitializeCombatInformation();
}

// FUNCTION: SWKOTOR 0x0064A1F0
void CSWCLevelUpStats::SetClass(unsigned char nIndex, unsigned char nClass)
{
    if (nIndex < 2) {
        m_ClassInfo[nIndex].m_nClass = nClass;
        SetReflexSavingThrow();
        SetWillSavingThrow();
        SetFortSavingThrow();
        if (m_nBaseHitPoints == 0) {
            short nBase = g_pRules->m_lstClasses[nClass].m_nHitDie;
            m_nBaseHitPoints = nBase;
            short nHP = (short)(char)((char)floor(m_nCON * 0.5) - 5) + nBase;
            if (nHP < 1) {
                nHP = 1;
            }
            m_nMaxHitPoints = nHP;
            m_nCurrentHitPoints = nHP;
        }
        InitializeCombatInformation();
    }
}

// FUNCTION: SWKOTOR 0x0064A2A0
void CSWCLevelUpStats::SetClassLevel(unsigned char nIndex, unsigned char nLevel)
{
    if (nIndex < 2) {
        m_ClassInfo[nIndex].m_nLevel = nLevel;
        SetReflexSavingThrow();
        SetWillSavingThrow();
        SetFortSavingThrow();
        InitializeCombatInformation();
    }
}

// FUNCTION: SWKOTOR 0x0064A2E0
int CSWCLevelUpStats::FeatRequirementsMet(unsigned short nFeat, CExoArrayList<unsigned short>* pList)
{
    CSWFeat* pFeat = g_pRules->GetFeat(nFeat);
    if (!pFeat) {
        return 0;
    }
    CSWClass* pLastClass = GetClassObject(m_nNumMultiClasses - 1);
    unsigned char nGainLevel = pLastClass->GetFeatGainLevel(nFeat);
    if (nGainLevel != 0xff && nGainLevel > GetTotalLevel()) {
        return 0;
    }
    if (pFeat->m_nMinSpellLevel != 0) {
        int bMet = 0;
        for (unsigned char i = 0; !bMet && i < m_nNumMultiClasses; i++) {
            unsigned char nClass = GetClass(i);
            CSWClass* pClass = &g_pRules->m_lstClasses[nClass];
            if (pClass->m_bSpellCaster) {
                unsigned char nCHA = m_nCHABase;
                unsigned char nSpellLevel = pFeat->m_nMinSpellLevel;
                unsigned short nRace = m_nRace;
                if (pClass->GetSpellLevelRequirement(GetClassLevel(i), nSpellLevel, nClass, nRace, nCHA)) {
                    bMet = 1;
                }
            }
        }
        if (!bMet) {
            return 0;
        }
    }
    if (pFeat->m_nMinLevel > GetTotalLevel()) {
        return 0;
    }
    unsigned char nMinAttackBonus = pFeat->m_nMinAttackBonus;
    if (nMinAttackBonus > GetBaseAttackBonus()) {
        return 0;
    }
    if (pFeat->m_nMinSTR > m_nSTRBase) {
        return 0;
    }
    if (pFeat->m_nMinDEX > m_nDEXBase) {
        return 0;
    }
    if (pFeat->m_nMinINT > m_nINTBase) {
        return 0;
    }
    if (pFeat->m_nMinWIS > m_nWISBase) {
        return 0;
    }
    if (!HasFeatPrereq(pFeat, pList)) {
        return 0;
    }
    if (pFeat->m_nRequiredSkill != 0xffff && !GetCanUseSkill(pFeat->m_nRequiredSkill)) {
        return 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0064A770
// NONMATCH (1 diff insn): i/nNumFeats land in the nClass/bNormal arg slots
// instead of bNormal/bBonus.
unsigned char CSWCLevelUpStats::AddDefaultPackageFeats(unsigned char nClass, CExoArrayList<unsigned short>* pList,
                                                       int bNormal, int bBonus)
{
    unsigned char nNormal = 0;
    unsigned char nBonus = 0;
    GetPackageFeatsLeft(&nNormal, &nBonus);
    if (!bNormal) {
        nNormal = 0;
    }
    if (!bBonus) {
        nBonus = 0;
    }
    unsigned char nLeft = nNormal + nBonus;
    CSWClass* pClass = &g_pRules->m_lstClasses[nClass];
    int i = 1;
    int nNumFeats = g_pRules->m_nNumFeats;
    while (nLeft > 0 && i <= nNumFeats) {
        unsigned short nFeat = pClass->GetPackageFeat(i);
        if (nFeat != -1) {
            if (CanChooseFeat(nFeat, nNormal, nBonus, pList)) {
                pList->Add(nFeat);
                nLeft--;
            }
        }
        i++;
    }
    return nLeft;
}

#pragma auto_inline(on)
