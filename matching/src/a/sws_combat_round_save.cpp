// Worker A round s7: CSWSCombatRound::SaveCombatRound (nwscombatround.cpp).
#include "a/sws_combat_round.h"

// FUNCTION: SWKOTOR 0x004D3EC0
int CSWSCombatRound::SaveCombatRound(CResGFF* pRes, CResStruct* pStruct)
{
    pRes->WriteFieldBYTE(pStruct, (unsigned char)m_bRoundStarted, "RoundStarted");
    pRes->WriteFieldBYTE(pStruct, (unsigned char)m_bSpellCastRound, "SpellCastRound");
    pRes->WriteFieldBYTE(pStruct, (unsigned char)m_nField978, "DeflectArrow");
    pRes->WriteFieldBYTE(pStruct, (unsigned char)m_nField97c, "WeaponSucks");
    pRes->WriteFieldDWORD(pStruct, m_oidNewAttackTarget, "DodgeTarget");
    pRes->WriteFieldDWORD(pStruct, m_oidField98c, "NewAttackTarget");
    pRes->WriteFieldINT(pStruct, m_bSynchronized, "Engaged");
    pRes->WriteFieldINT(pStruct, m_bMaster, "Master");
    pRes->WriteFieldDWORD(pStruct, m_oidMaster, "MasterID");
    pRes->WriteFieldBYTE(pStruct, (unsigned char)m_bRoundPaused, "RoundPaused");
    pRes->WriteFieldDWORD(pStruct, m_oidRoundPausedBy, "RoundPausedBy");
    pRes->WriteFieldBYTE(pStruct, (unsigned char)m_bInfinitePause, "InfinitePause");
    pRes->WriteFieldINT(pStruct, m_nPauseTimer, "PauseTimer");
    pRes->WriteFieldINT(pStruct, m_nTimer, "Timer");
    pRes->WriteFieldINT(pStruct, m_nRoundLength, "RoundLength");
    pRes->WriteFieldINT(pStruct, m_nOverlapAmount, "OverlapAmount");
    pRes->WriteFieldINT(pStruct, m_nBleedTimer, "BleedTimer");
    pRes->WriteFieldBYTE(pStruct, m_nCurrentAttack, "CurrentAttack");
    pRes->WriteFieldWORD(pStruct, m_nAttackID, "AttackID");
    pRes->WriteFieldBYTE(pStruct, m_nField974, "AttackGroup");
    pRes->WriteFieldINT(pStruct, m_nField980, "ParryIndex");
    pRes->WriteFieldINT(pStruct, m_nField984, "NumAOOs");
    pRes->WriteFieldINT(pStruct, m_nField988, "NumCleaves");
    pRes->WriteFieldINT(pStruct, m_nOnHandAttacks, "OnHandAttacks");
    pRes->WriteFieldINT(pStruct, m_nOffHandAttacks, "OffHandAttacks");
    pRes->WriteFieldINT(pStruct, m_nEffectAttacks, "AdditAttacks");
    pRes->WriteFieldINT(pStruct, m_nAdditionalAttacks, "EffectAttacks");
    pRes->WriteFieldBYTE(pStruct, m_nField9a8, "ParryActions");
    pRes->WriteFieldINT(pStruct, m_nOffHandAttacksTaken, "OffHandTaken");
    pRes->WriteFieldINT(pStruct, m_nExtraAttacksTaken, "ExtraTaken");
    CResList cList;
    CResStruct cElement;
    pRes->AddList(&cList, pStruct, "AttackList");
    int i;
    for (i = 0; i < 5; i++) {
        pRes->AddListElement(&cElement, &cList, 0xaaee);
        m_pcAttackList[i].SaveData(pRes, &cElement);
    }
    pRes->AddList(&cList, pStruct, "SpecAttackList");
    for (i = 0; i < m_alstSpecialAttacks.num; i++) {
        pRes->AddListElement(&cElement, &cList, 0xdaee);
        pRes->WriteFieldWORD(&cElement, m_alstSpecialAttacks.element[i], "SpecialAttack");
    }
    pRes->AddList(&cList, pStruct, "SpecAttackIdList");
    for (i = 0; i < m_alstSpecialAttackIds.num; i++) {
        pRes->AddListElement(&cElement, &cList, 0xdaae);
        pRes->WriteFieldWORD(&cElement, m_alstSpecialAttackIds.element[i], "SpecialAttackId");
    }
    pRes->AddList(&cList, pStruct, "SchedActionList");
    CExoLinkedListPosition pos = m_pScheduledActions->GetHeadPos();
    while (pos) {
        CSWSCombatRoundAction* pAction = m_pScheduledActions->GetAtPos(pos);
        if (pAction) {
            pRes->AddListElement(&cElement, &cList, 0xdaea);
            pAction->SaveData(pRes, &cElement);
        }
        m_pScheduledActions->GetNext(pos);
    }
    return 1;
}
