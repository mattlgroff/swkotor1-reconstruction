// CSWGuiLevelUpCharGen: the level-up summary screen (layout "MAINCG").
#include "l/sw_gui_levelup_chargen.h"

namespace {
inline short ClampAlignment(short nGoodEvil)
{
    if (nGoodEvil >= 0) {
        int nValue = nGoodEvil;
        if (nValue > 100) {
            nValue = 100;
        }
        return (short)nValue;
    }
    return 0;
}
}

// FUNCTION: SWKOTOR 0x006E7B40
void CSWGuiLevelUpCharGen::OnPanelAdded()
{
    m_pLevelUpPanel->SetCanCancel(0);
    m_pManager->AddPanel(m_pLevelUpPanel, 1, 1);
    CSWGuiPanel::OnPanelAdded();
}

// FUNCTION: SWKOTOR 0x006E7DD0
void CSWGuiLevelUpCharGen::DetermineClass()
{
    m_sClassName = GetSnapshotStats()->GetClassDescription('\n');
    m_sClassName = m_sClassName.Right(m_sClassName.GetLength() - m_sClassName.Find('\n', 0) - 1);
    m_sClassName = m_sClassName.Left(m_sClassName.Find('(', 0));
}

// FUNCTION: SWKOTOR 0x006E7EC0
void CSWGuiLevelUpCharGen::SetNameString(const CExoString& sName)
{
    m_sName = sName;
    CExoString sLevel(m_pSnapshot->m_pStats->GetTotalLevel() + 1);
    m_lblName.m_cText.SetText(m_sName);
    m_lblClass.m_cText.SetText(m_sClassName);
    m_lblLevelVal.m_cText.SetText(sLevel);
}

// FUNCTION: SWKOTOR 0x006E8500
void CSWGuiLevelUpCharGen::NewAbilityScores(unsigned int nStr, unsigned int nCha, unsigned int nInt,
                                            unsigned int nWis, unsigned int nCon, unsigned int nDex,
                                            unsigned int nPoints)
{
    CSWCCreature* pCreature = GetLiveCreature();
    if (pCreature == 0) {
        return;
    }
    ((CSWCLevelUpStats*)pCreature->m_pStats)->SetSTRBase((unsigned char)nStr);
    ((CSWCLevelUpStats*)pCreature->m_pStats)->SetCHABase((unsigned char)nCha);
    ((CSWCLevelUpStats*)pCreature->m_pStats)->SetINTBase((unsigned char)nInt);
    ((CSWCLevelUpStats*)pCreature->m_pStats)->SetWISBase((unsigned char)nWis);
    ((CSWCLevelUpStats*)pCreature->m_pStats)->SetCONBase((unsigned char)nCon);
    ((CSWCLevelUpStats*)pCreature->m_pStats)->SetDEXBase((unsigned char)nDex, 1);
    pCreature->m_pStats->m_nAbilityPoints = (unsigned char)nPoints;
}

// FUNCTION: SWKOTOR 0x006E85A0
void CSWGuiLevelUpCharGen::ClearAbilities()
{
    CSWCCreature* pCreature = GetLiveCreature();
    if (pCreature == 0) {
        return;
    }
    ((CSWCLevelUpStats*)pCreature->m_pStats)->SetSTRBase(m_pSnapshot->m_pStats->m_nSTRBase);
    ((CSWCLevelUpStats*)pCreature->m_pStats)->SetCHABase(m_pSnapshot->m_pStats->m_nCHABase);
    ((CSWCLevelUpStats*)pCreature->m_pStats)->SetINTBase(m_pSnapshot->m_pStats->m_nINTBase);
    ((CSWCLevelUpStats*)pCreature->m_pStats)->SetWISBase(m_pSnapshot->m_pStats->m_nWISBase);
    ((CSWCLevelUpStats*)pCreature->m_pStats)->SetCONBase(m_pSnapshot->m_pStats->m_nCONBase);
    ((CSWCLevelUpStats*)pCreature->m_pStats)->SetDEXBase(m_pSnapshot->m_pStats->m_nDEXBase, 1);
    ((CSWCLevelUpStats*)pCreature->m_pStats)->SetFortSavingThrow();
    ((CSWCLevelUpStats*)pCreature->m_pStats)->SetWillSavingThrow();
    ((CSWCLevelUpStats*)pCreature->m_pStats)->SetReflexSavingThrow();
    pCreature->m_pStats->m_nBaseHitPoints = m_pSnapshot->m_pStats->m_nBaseHitPoints;
    {
        CSWCCreatureStats* pTarget = pCreature->m_pStats;
        pTarget->m_nMaxHitPoints = m_pSnapshot->m_pStats->GetMaxHitPoints();
    }
    pCreature->m_pStats->m_nCurrentHitPoints = m_pSnapshot->m_pStats->m_nCurrentHitPoints;
    pCreature->m_pStats->m_nBaseForcePoints = m_pSnapshot->m_pStats->m_nBaseForcePoints;
    pCreature->m_pStats->m_nCurrentForcePoints = m_pSnapshot->m_pStats->m_nCurrentForcePoints;
    pCreature->m_pStats->m_nMaxForcePoints =
        m_pSnapshot->m_pStats->m_nForcePointsBonus + m_pSnapshot->m_pStats->m_nMaxForcePoints;
}

// FUNCTION: SWKOTOR 0x006E8730
void CSWGuiLevelUpCharGen::ClearSkills()
{
    CSWCCreature* pCreature = GetLiveCreature();
    if (pCreature == 0) {
        return;
    }
    pCreature->m_pStats->m_nSkillPoints = m_pSnapshot->m_pStats->m_nSkillPoints;
    for (unsigned char nSkill = 0; nSkill < g_pRules->m_nNumSkills; ++nSkill) {
        ((CSWCLevelUpStats*)pCreature->m_pStats)->SetSkillRankBase(
            nSkill, m_pSnapshot->m_pStats->GetSkillRank(nSkill));
    }
}

// FUNCTION: SWKOTOR 0x006E9A50
void CSWGuiLevelUpCharGen::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown && (nEvent == 0x28 || nEvent == 0x2E)) {
        m_pManager->PlayGuiSound(0);
        m_pManager->PopModalPanel();
        m_pLevelUpPanel->m_nFlags = (m_pLevelUpPanel->m_nFlags & 0xFFFFFCFFul) | 0x400ul;
        m_nFlags = (m_nFlags & 0xFFFFFCFFul) | 0x400ul;
        m_pCharacter->AttachModel();
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x006E7B70
CSWGuiLevelUpCharGen::~CSWGuiLevelUpCharGen()
{
    if (m_pSnapshot) {
        delete (CSWCCreatureDeletable*)m_pSnapshot;
    }
    m_pSnapshot = 0;
}

// FUNCTION: SWKOTOR 0x006E88A0
void CSWGuiLevelUpCharGen::BeginPlayGame()
{
    m_pManager->PopModalPanel();
    m_pLevelUpPanel->m_nFlags = (m_pLevelUpPanel->m_nFlags & 0xFFFFFCFFul) | 0x400ul;
    CSWCCreature* pCreature = GetLiveCreature();
    pCreature->m_pStats->GetLastClass();
    CSWCMessage* pMessage = g_pAppManager->GetClientExoApp()->GetMessage();
    pMessage->SendPlayerToServerLevelUp(m_pSnapshot->m_pStats, pCreature->m_pStats);
    pCreature->m_n440 |= 2;
    m_pManager->PopModalPanel();
    m_nFlags = (m_nFlags & 0xFFFFFCFFul) | 0x400ul;
    m_pCharacter->AttachModel();
    if (m_nInputClass == 0 && g_pAppManager->GetClientExoApp()->GetPausedByCombat()) {
        g_pAppManager->GetClientExoApp()->SetPausedByCombat(0, 0, 0);
    }
    if (g_bGuiFlag == 0 && m_nInputClass != 2) {
        m_pCharacter->m_nFlags = (m_pCharacter->m_nFlags & 0xFFFFFCFFul) | 0x400ul;
        g_pAppManager->GetClientExoApp()->GetInGameGui()->m_bCharacterScreenOpen = 0;
        g_pExoSound->SetSoundMode(g_nSoundModeZero);
    }
    g_pAppManager->GetClientExoApp()->SetInputClass(m_nInputClass, 1);
}

// FUNCTION: SWKOTOR 0x006E89F0
int CSWGuiLevelUpCharGen::CopyCreatureStats(CSWCCreature* pFrom, CSWCCreature* pTo)
{
    CSWCLevelUpStats* pDst = (CSWCLevelUpStats*)pTo->m_pStats;
    CSWCLevelUpStats* pSrc = (CSWCLevelUpStats*)pFrom->m_pStats;
    pDst->m_sFirstName = pSrc->m_sFirstName;
    pDst->m_sLastName = pSrc->m_sLastName;
    pDst->m_cPortrait = pSrc->GetPortrait(0);
    pDst->m_nExperience = pSrc->m_nExperience;
    pDst->m_n33 = pSrc->m_n33;
    pDst->m_nGender = pSrc->m_nGender;
    pDst->m_nGoodEvil = ClampAlignment(pSrc->m_nGoodEvil);
    pDst->SetSTRBase(pSrc->m_nSTRBase);
    pDst->SetDEXBase(pSrc->m_nDEXBase, 1);
    pDst->SetCONBase(pSrc->m_nCONBase);
    pDst->SetINTBase(pSrc->m_nINTBase);
    pDst->SetWISBase(pSrc->m_nWISBase);
    pDst->SetCHABase(pSrc->m_nCHABase);
    pDst->m_nBaseHitPoints = pSrc->m_nBaseHitPoints;
    pDst->m_nCurrentHitPoints = pSrc->m_nCurrentHitPoints;
    pDst->m_nMaxHitPoints = pSrc->GetMaxHitPoints();
    pDst->m_nBaseForcePoints = pSrc->m_nBaseForcePoints;
    pDst->m_nMaxForcePoints = pSrc->m_nForcePointsBonus + pSrc->m_nMaxForcePoints;
    pDst->m_nCurrentForcePoints = pSrc->m_nCurrentForcePoints;
    for (unsigned char nClass = 0; nClass < pSrc->m_nNumClasses; nClass++) {
        pDst->m_nNumClasses = nClass + 1;
        pDst->SetClass(nClass, pSrc->GetClass(nClass));
        pDst->SetClassLevel(nClass, pSrc->GetClassLevel(nClass));
        pDst->ClearKnownSpells(nClass);
        for (unsigned char nSpell = 0; nSpell < pSrc->GetNumberKnownSpells(nClass, 0); nSpell++) {
            pDst->AddKnownSpell(nClass, pSrc->GetKnownSpell(nClass, 0, nSpell));
        }
    }
    pDst->m_nRace = pSrc->m_nRace;
    pDst->m_sSubRace = pSrc->m_sSubRace;
    pDst->m_n30 = pSrc->m_n30;
    pDst->SetFortSavingThrow();
    pDst->SetWillSavingThrow();
    pDst->SetReflexSavingThrow();
    for (int nAttack = 0; nAttack < 4; nAttack++) {
        pDst->m_nBaseAttackBonus = (char)pSrc->GetBaseAttackBonus();
    }
    pDst->m_nGoodEvil = ClampAlignment(pSrc->m_nGoodEvil);
    pDst->ClearSkills();
    pDst->m_nSkillPoints = pSrc->m_nSkillPoints;
    for (unsigned char nSkill = 0; nSkill < g_pRules->m_nNumSkills; nSkill++) {
        pDst->SetSkillRankBase(nSkill, pSrc->GetSkillRank(nSkill));
    }
    pDst->ClearFeats();
    CExoArrayListL<unsigned short>* pFeats = (CExoArrayListL<unsigned short>*)&pDst->m_pFeats;
    for (unsigned long nIndex = 0; (unsigned short)nIndex < pSrc->GetNumFeats(); nIndex++) {
        unsigned short nFeat = pSrc->GetFeat((unsigned short)nIndex);
        int nFound = 0;
        for (int i = 0; i < pFeats->num; i++) {
            if (pFeats->element[i] == nFeat) {
                nFound++;
            }
        }
        if (!nFound) {
            pFeats->Add(nFeat);
        }
    }
    pDst->m_nAbilityPoints = pSrc->m_nAbilityPoints;
    pDst->m_nACNaturalBase = pSrc->GetArmorClass(0);
    pDst->m_nACNaturalBase = pSrc->GetArmorClass(0);
    return 1;
}

// FUNCTION: SWKOTOR 0x006E7F70
void CSWGuiLevelUpCharGen::AbilitiesChanged()
{
    CSWCCreature* pCreature = GetLiveCreature();
    if (pCreature == 0) {
        return;
    }
    ((CSWCLevelUpStats*)pCreature->GetClientStats())->SetFortSavingThrow();
    ((CSWCLevelUpStats*)pCreature->GetClientStats())->SetWillSavingThrow();
    ((CSWCLevelUpStats*)pCreature->GetClientStats())->SetReflexSavingThrow();
    {
        CExoString sValue(pCreature->GetClientStats()->m_nCHABase);
        m_lblChaAb.m_cText.SetText(sValue);
    }
    {
        CExoString sValue(pCreature->GetClientStats()->m_nSTRBase);
        m_lblStrAb.m_cText.SetText(sValue);
    }
    {
        CExoString sValue(pCreature->GetClientStats()->m_nINTBase);
        m_lblIntAb.m_cText.SetText(sValue);
    }
    {
        CExoString sValue(pCreature->GetClientStats()->m_nWISBase);
        m_lblWisAb.m_cText.SetText(sValue);
    }
    {
        CExoString sValue(pCreature->GetClientStats()->m_nCONBase);
        m_lblConAb.m_cText.SetText(sValue);
    }
    {
        CExoString sValue(pCreature->GetClientStats()->m_nDEXBase);
        m_lblDexAb.m_cText.SetText(sValue);
    }

    unsigned char nClass = pCreature->GetClientStats()->GetLastClass();
    CSWCCreatureStats* pStats = pCreature->GetClientStats();
    unsigned char nScore = pStats->m_nCON;
    char nServerMod = pCreature->AsServerCreature()->m_pStats->m_nConstitutionModifier;
    int nConDelta = pStats->GetAbilityMod(nScore) - nServerMod;
    pStats = pCreature->GetClientStats();
    nScore = pStats->m_nWIS;
    nServerMod = pCreature->AsServerCreature()->m_pStats->m_nWisdomModifier;
    int nWisDelta = pStats->GetAbilityMod(nScore) - nServerMod;
    pStats = pCreature->GetClientStats();
    nScore = pStats->m_nCHA;
    nServerMod = pCreature->AsServerCreature()->m_pStats->m_nCharismaModifier;
    int nChaDelta = pStats->GetAbilityMod(nScore) - nServerMod;

    char nHitDie = g_pRules->m_pClasses[nClass].m_nHitDie;
    char nVitality = GetSnapshotStats()->GetTotalLevel() * nConDelta +
                     pCreature->GetClientStats()->GetAbilityMod(pCreature->GetClientStats()->m_nCON) + nHitDie;
    if (nVitality < 1) {
        nVitality = 1;
    }
    pCreature->GetClientStats()->m_nBaseHitPoints = nHitDie + GetSnapshotStats()->m_nBaseHitPoints;
    {
        CSWCCreatureStats* pTarget = pCreature->GetClientStats();
        pTarget->m_nMaxHitPoints = GetSnapshotStats()->GetMaxHitPoints() + nVitality;
    }
    pCreature->GetClientStats()->m_nCurrentHitPoints = nVitality + GetSnapshotStats()->m_nCurrentHitPoints;

    if (IsJediClass(nClass)) {
        char nForceDie = g_pRules->m_pClasses[nClass].m_nForceDie;
        pStats = pCreature->GetClientStats();
        unsigned char nWis = pStats->m_nWIS;
        char nChaMod = pStats->GetAbilityMod(pStats->m_nCHA);
        char nForce = GetSnapshotStats()->GetTotalLevel() * (char)(nWisDelta + nChaDelta) +
                      pStats->GetAbilityMod(nWis) + nChaMod + nForceDie;
        if (nForce < 1) {
            nForce = 1;
        }
        pCreature->GetClientStats()->m_nBaseForcePoints = nForceDie + GetSnapshotStats()->m_nBaseForcePoints;
        pCreature->GetClientStats()->m_nCurrentForcePoints = GetSnapshotStats()->m_nCurrentForcePoints + nForce;
        pCreature->GetClientStats()->m_nMaxForcePoints =
            nForce + (unsigned short)(GetSnapshotStats()->m_nForcePointsBonus + GetSnapshotStats()->m_nMaxForcePoints);
    }

    {
        CExoString sValue(pCreature->GetClientStats()->GetArmorClass(0));
        m_lblDef.m_cText.SetText(sValue);
    }
    {
        CExoString sValue(pCreature->GetClientStats()->m_nFortSave);
        m_lblNewFort.m_cText.SetText(sValue);
    }
    {
        CExoString sValue(pCreature->GetClientStats()->m_nReflexSave);
        m_lblNewRefl.m_cText.SetText(sValue);
    }
    {
        CExoString sValue(pCreature->GetClientStats()->GetMaxHitPoints());
        m_lblVit.m_cText.SetText(sValue);
    }
    {
        CExoString sValue(pCreature->GetClientStats()->m_nWillSave);
        m_lblNewWill.m_cText.SetText(sValue);
    }
}
