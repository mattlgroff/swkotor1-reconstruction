// Worker A round 10: more CSWSCreature leaves (0x004EFFA0-0x005188C0), the
// CGameObject ctor and two CExoArrayList instantiations.
#include <stdlib.h>
#include "a/sws_creature.h"

inline float ClampRate(float f)
{
    if (f < 0.125f) {
        return 0.125f;
    }
    if (f > 1.5f) {
        return 1.5f;
    }
    return f;
}

// FUNCTION: SWKOTOR 0x004D7D60
CGameObject::CGameObject(unsigned char nObjectType, unsigned long oid)
{
    m_idSelf = oid;
    m_nObjectType = nObjectType;
}

// FUNCTION: SWKOTOR 0x004EFFA0
CSWGuiInterfaceAction& CSWGuiInterfaceAction::operator=(const CSWGuiInterfaceAction& o)
{
    m_sName = o.m_sName;
    m_n08 = o.m_n08;
    m_data = o.m_data;
    m_n1C = o.m_n1C;
    m_resIcon = o.m_resIcon;
    m_b30 = o.m_b30;
    m_n30 = o.m_n30;
    m_n34 = o.m_n34;
    return *this;
}

// FUNCTION: SWKOTOR 0x004F0020
void CSWSCreaturePartyFollowInfo::Reset()
{
    Vector vZero(0.0f, 0.0f, 0.0f);
    m_vFollowLocation = vZero;
    m_vLastLeaderPos = vZero;
    m_vLastFollowerPos = vZero;
    m_oidFollowObject = 0x7f000000;
    m_fMaxSpeed = 0.0f;
    m_bStickToPos = 0;
    m_nResult = AI_ACTION_RESULT_005140E0;
    m_nTimeElapsed = 0;
    m_bInSafetyRange = 1;
}

// FUNCTION: SWKOTOR 0x004F1BE0
float CSWSCreature::GetMovementRate4F1BE0()
{
    float fBase = m_pStats->m_f198;
    float fResult = fBase * ClampRate(m_fA08) * 1000.0f;
    if (1000.0f > fResult) {
        fResult = 1000.0f;
    }
    return fResult;
}

// FUNCTION: SWKOTOR 0x004F7880
int CSWSCreature::GetHasEffectOfType(int nType)
{
    for (int i = 0; i < m_appliedEffects.num; i++) {
        if (m_appliedEffects.element[i]->m_nType == nType) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004F77E0
void CSWSCreature::RemoveFirstEffect108()
{
    for (int i = 0; i < m_appliedEffects.num; i++) {
        if (m_appliedEffects.element[i]->m_nType == EFFECT_TYPE_0746828) {
            RemoveEffectById(m_appliedEffects.element[i]->m_nID);
            return;
        }
    }
}

// FUNCTION: SWKOTOR 0x004F7830
void CSWSCreature::RemoveFirstEffect109()
{
    for (int i = 0; i < m_appliedEffects.num; i++) {
        if (m_appliedEffects.element[i]->m_nType == EFFECT_TYPE_074682C) {
            RemoveEffectById(m_appliedEffects.element[i]->m_nID);
            return;
        }
    }
}

// FUNCTION: SWKOTOR 0x0050E250
void CSWSCreature::SetVector248(const Vector& v)
{
    m_v248 = v;
}

// FUNCTION: SWKOTOR 0x005179E0
int CSWSCreature::GetIsInList920(unsigned long oid)
{
    for (int i = 0; i < m_l920.num; i++) {
        if (m_l920.element[i]->m_oid == oid) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00517A20
CSWSCreatureListEntry* CSWSCreature::GetFromList920(unsigned long oid)
{
    for (int i = 0; i < m_l920.num; i++) {
        if (m_l920.element[i]->m_oid == oid) {
            return m_l920.element[i];
        }
    }
    return NULL;
}

// FUNCTION: SWKOTOR 0x005188C0
unsigned long CSWSCreature::Get93CField4()
{
    if (m_p93C) {
        return m_p93C->m_n4;
    }
    return 0;
}

#pragma auto_inline(off)
template <class T> void CExoArrayList<T>::Allocate(int nSize)
{
    T* pOld = element;
    array_size = nSize;
    element = new T[nSize];
    for (int i = 0; i < num; i++) {
        element[i] = pOld[i];
    }
    if (pOld) {
        delete[] pOld;
    }
}
#pragma auto_inline(on)

template <class T> void CExoArrayList<T>::Insert(T t, int nPos)
{
    int i = num;
    Add(t);
    for (; i > nPos; i--) {
        element[i] = element[i - 1];
    }
    element[nPos] = t;
}

// FUNCTION: SWKOTOR 0x004F7920 SYMBOL: ?Allocate@?$CExoArrayList@VCSWGuiInterfaceAction@@@@QAEXH@Z
template void CExoArrayList<CSWGuiInterfaceAction>::Allocate(int nSize);

// FUNCTION: SWKOTOR 0x004F78C0 SYMBOL: ?Insert@?$CExoArrayList@M@@QAEXMH@Z
template void CExoArrayList<float>::Insert(float t, int nPos);

// FUNCTION: SWKOTOR 0x004F1B20
float CSWSCreature::GetMovementRate4F1B20()
{
    float fBase = m_pStats->m_f19C;
    float fResult = fBase * ClampRate(m_fA08) * 1000.0f;
    if (fResult <= 100.0f) {
        return 0.0f;
    }
    unsigned long oidSelf = m_idSelf;
    unsigned long oidClient = GetClientExoApp()->ServerToClientObjectId(oidSelf);
    CSWCCreature* pClient = GetClientExoApp()->GetCreatureByGameObjectID(oidClient);
    GetClientExoApp()->GetSWParty();
    if ((m_n9FC & 1) && pClient) {
        return pClient->m_p21C->m_f60 * 1000.0f;
    }
    return fResult;
}

// FUNCTION: SWKOTOR 0x004F7650
int CSWSCreature::GetHasActivePartyMember920()
{
    GetClientExoApp()->GetSWParty();
    for (int i = 0; i < m_l920.num; i++) {
        CSWSCreatureListEntry* pEntry = m_l920.element[i];
        if (pEntry->m_nFlags & 1) {
            unsigned long oid = pEntry->m_oid;
            CSWSCreature* pCreature = GetServerExoApp()->GetCreatureByGameObjectID(oid);
            if (pCreature && pCreature->m_bA88) {
                return 1;
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004F76C0
int CSWSCreature::RemoveFirstActionOfType(unsigned long nAction)
{
    int bResult = 0;
    void* pos = m_pActions->m_pHead;
    while (pos) {
        CSWSObjectActionNode* pNode = (CSWSObjectActionNode*)m_pActions->GetAtPos(pos);
        if (pNode->m_nActionId == nAction && RunAction(pNode, 1) == 1) {
            m_pActions->Remove(pos);
            delete pNode;
            bResult = 1;
            break;
        }
        m_pActions->GetNext(pos);
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x004F5F70
void CSWSCreature::RemoveEffects4F5F70()
{
    int i = m_pStats->m_n140;
    while (i < m_appliedEffects.num && m_appliedEffects.element[i]->m_nType <= EFFECT_TYPE_0746778) {
        CGameEffect* pEffect = m_appliedEffects.element[i];
        if (pEffect->m_nType == EFFECT_TYPE_0746738 && pEffect->GetInteger(0) == 1) {
            RemoveEffectById(pEffect->m_nID);
        }
        else if (pEffect->m_nType == EFFECT_TYPE_0746778) {
            RemoveEffectById(pEffect->m_nID);
        }
        else {
            i++;
        }
    }
}

// FUNCTION: SWKOTOR 0x004F3770
int CSWSCreature::CountItems4F3770(int bFlag8, int bSkipEquipped, int bNo108)
{
    int nCount = 0;
    if (!bSkipEquipped && m_pInventory) {
        for (unsigned long nSlot = 0; nSlot < 14; nSlot++) {
            CSWSInventory* pInventory = m_pInventory;
            CSWSItem* pItem = pInventory->GetItemInSlot(1 << nSlot);
            if (pItem && (!bFlag8 || (pItem->m_n288 & 8)) && (!bNo108 || !pItem->m_n108)) {
                nCount++;
            }
        }
    }
    CItemRepository* pRepository = GetItemRepository(1);
    if (pRepository) {
        for (unsigned long i = 0; i < pRepository->m_nItems; i++) {
            CSWSItem* pItem = pRepository->ItemListGetItem(i);
            if (pItem && (!bFlag8 || (pItem->m_n288 & 8)) && (!bNo108 || !pItem->m_n108)) {
                nCount++;
            }
        }
    }
    return nCount;
}

// FUNCTION: SWKOTOR 0x004F8770
void CSWSCreature::ClearActionsForType4F8770(int nType)
{
    if (GetServerExoApp()->GetClientObjectByObjectId(m_idSelf)) {
        Fn4F1460();
        if ((nType == 1 || nType == 2) && m_n9F2 < 10) {
            ClearAllActions(1);
        }
        if (nType == 8 || nType == 4) {
            ClearAllActions(1);
        }
    }
}

// FUNCTION: SWKOTOR 0x004F0900
int CSWSCreature::AddAction38(unsigned long a1, unsigned long a2, unsigned long a3)
{
    if (!m_pStats->GetCanUseSkill(7)) {
        SendFeedbackMessage(0, NULL);
        return 0;
    }
    if (!m_bE8) {
        return 0;
    }
    int bTrue = 1;
    AddAction(0x38, 0xffff, 3, &a1, 3, &a2, 1, &a3, 1, &bTrue);
    return 1;
}

// FUNCTION: SWKOTOR 0x004F0840
int CSWSCreature::AddAction9(unsigned long oidItem, Vector* pV, unsigned long a3)
{
    if (!GetServerExoApp()->GetItemByGameObjectID(oidItem)) {
        return 0;
    }
    if (!GetArea()) {
        return 0;
    }
    if (m_b4E0 == 1) {
        CSWSItem* pItem = m_pInventory->GetItemInSlot(2);
        if (pItem && pItem->m_n14 == oidItem) {
            SendFeedbackMessage(0xc2, NULL);
            return 0;
        }
    }
    if (!m_bE8) {
        return 0;
    }
    AddAction(9, 0xffff, 3, &oidItem, 2, &pV->x, 2, &pV->y, 2, &pV->z, 1, &a3);
    return 1;
}

// FUNCTION: SWKOTOR 0x004F61D0
// NONMATCH (16): ebx/ebp swap between the loop index and pTarget.
void CSWSCreature::RemoveTargetActions4F61D0(CGameObject* pTarget)
{
    int i;
    CExoLinkedListInternal** ppActions = &m_pActions;
    CSWSObjectActionNode* pAction;
    if (ppActions == NULL) {
        return;
    }
    int nGroups = GetNumActionGroups();
    for (i = 0; i < nGroups; i++) {
        void* pos = GetPositionByGroupIndex(i);
        if (pos == NULL) {
            continue;
        }
        CSWSObjectActionNode* pNode = (CSWSObjectActionNode*)m_pActions->GetAtPos(pos);
        if (pNode == NULL) {
            continue;
        }
        unsigned short nGroupId = pNode->m_nGroupId;
        if (GetActionByGroupId(nGroupId, &pAction) == 0xffff || pAction == NULL) {
            continue;
        }
        if (pAction->m_nActionId == 0xc) {
            if (pTarget && pAction->m_n38 != pTarget->m_idSelf) {
                continue;
            }
        }
        else if (pAction->m_nActionId == 0xf) {
            if (pTarget && pAction->m_n4C != pTarget->m_idSelf) {
                continue;
            }
        }
        else if (pAction->m_nActionId == 0x2e) {
            if (pTarget && pAction->m_n44 != pTarget->m_idSelf) {
                continue;
            }
        }
        else {
            continue;
        }
        RemoveGroup(nGroupId);
        i = 0;
    }
}

// FUNCTION: SWKOTOR 0x004F1250
// NONMATCH (2): ecx instead of eax for the combat round in the last test.
int CSWSCreature::ResetCombatState4F1250(int nTime)
{
    int bResult = 0;
    m_pCombatRound->Fn4D28F0(0, 0x7f000000);
    m_pCombatRound->Fn4D2920(0, 0);
    m_pCombatRound->IncrementTimer(nTime);
    m_pCombatRound->SetCurrentAction(0);
    m_n4DC = 0;
    if (!GetDead()) {
        if (!Fn63E7F0()) {
            if (!GetDeadTemp()) {
                if (Fn4CAE60(m_nD4)) {
                    if (!IsDebilitated()) {
                        if (m_pCombatRound->m_n940 == 0) {
                            SetAnimation(GetPauseReadyAnimation());
                            bResult = 1;
                        }
                    }
                }
            }
        }
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x004F2520
void CSWSCreature::SendVoiceChat4F2520(unsigned long nArg)
{
    if (GetArea() == NULL) {
        return;
    }
    CExoArrayList<unsigned long>* pPlayers = &GetArea()->m_lPlayers;
    for (int i = 0; i < pPlayers->num; i++) {
        CSWSPlayer* pPlayer = GetServerExoApp()->GetClientObjectByObjectId(pPlayers->element[i]);
        if (pPlayer == NULL) {
            continue;
        }
        CSWSCreature* pObject = pPlayer->GetGameObject();
        if (pObject && m_oidArea == pObject->m_oidArea) {
            Vector vDiff = pObject->m_vPosition - m_vPosition;
            if (vDiff.x * vDiff.x + vDiff.y * vDiff.y + vDiff.z * vDiff.z <= 900.0f) {
                unsigned long oidSelf = m_idSelf;
                GetServerExoApp()->GetSWSMessage()->SendServerToPlayerVoiceChat_Play(pPlayer, oidSelf, nArg);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x004F0A60
void CSWSCreature::SplitToRepository4F0A60(CSWSItem* pItem, int nCount)
{
    CItemRepository* pRepository = GetItemRepository(1);
    if (pItem->m_oidPossessor != m_idSelf) {
        CGameObject* pObject = GetServerExoApp()->GetGameObject(pItem->m_oidPossessor);
        if (pObject->AsSWSItem() == NULL) {
            return;
        }
        CSWSItem* pContainer = pObject->AsSWSItem();
        if (pContainer->m_oidPossessor == m_idSelf && pContainer->m_pRepository) {
            pRepository = pContainer->m_pRepository;
        }
    }
    CSWSItem* pNew = pItem->SplitItem(nCount);
    if (pNew) {
        if (!pRepository->AddItem(&pNew, 0, 0, 1)) {
            pNew->AddToArea(GetArea(), m_vPosition.x, m_vPosition.y, m_vPosition.z, 1);
        }
    }
}

// FUNCTION: SWKOTOR 0x004F0990
// NONMATCH (32): pItem/pOther in ebp/ebx instead of ebx/ebp, nothing else.
void CSWSCreature::MergeOrEquip4F0990(CSWSItem* pItem, CSWSItem* pOther)
{
    if (g_pRules->GetBaseItem(pItem->m_nBaseItem)->m_nStackSize <= 1) {
        return;
    }
    if (pItem->MergeItem(pOther)) {
        Fn510EF0(pOther, 0, 0, 0);
        if (pOther) {
            delete pOther;
        }
        return;
    }
    if (m_pInventory->Fn5A4950(pItem) == 1) {
        for (int i = 0; i < 0x12; i++) {
            CSWSItem* pSlot = m_pInventory->GetItemInSlot(1 << i);
            if (pSlot && pSlot->m_n14 == pItem->m_n14) {
                AddEquipItemActions(pOther, 1 << i, 0, 0, 0, 0, 0);
                return;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x004F34A0
void CSWSCreature::SetFacingTarget4F34A0(unsigned long oidTarget, int bForce)
{
    if (!bForce) {
        if (GetDead()) {
            return;
        }
        if (GetDeadTemp()) {
            return;
        }
    }
    if (oidTarget == OBJECT_INVALID && m_oid944 != OBJECT_INVALID && m_oid944 != m_idSelf) {
        CGameObject* pObject;
        if (GetServerExoApp()->GetObjectArray()->GetGameObject(m_oid944, &pObject) == GAME_OBJECT_VALID &&
            pObject->m_nObjectType > OBJECT_TYPE_0746633) {
            Vector vDir = VectorSubtract(((CSWSCreature*)pObject)->m_vPosition, m_vPosition);
            vDir = NormalizeVector(vDir);
            SetOrientation4CD170(vDir);
        }
    }
    m_oid944 = oidTarget;
}

// FUNCTION: SWKOTOR 0x004F3690
void CSWSCreature::Fn4F3690(unsigned short nMessage)
{
    SetAnimation(10000);
    SendFeedbackMessage(nMessage, NULL);
    if (!(m_nA00 & 8)) {
        m_n9FC &= ~8;
    }
    if (m_b9D4) {
        CSWSModule* pModule = GetServerExoApp()->GetModule6B0();
        CScriptEvent* pEvent = new CScriptEvent();
        pEvent->m_nType = 0x23;
        pEvent->SetInteger(0, 3);
        unsigned long oidModule = pModule->m_idSelf;
        unsigned long oidSelf = m_idSelf;
        GetServerExoApp()->GetServerAIMaster()->AddEventAbsoluteTime(0, 0, oidSelf, oidModule,
                                                                    EVENT_0x7441DC, pEvent);
        StopGuiTimingBar();
    }
}

// FUNCTION: SWKOTOR 0x004F05D0
// NONMATCH (9): GetAtPos receiver loaded after the push; success tails return via ecx.
int CSWSCreature::UpdateTarget4F05D0(CSWSItem* pItem, int a2, int bSkipPending, unsigned long a4)
{
    int bResult = 0;
    unsigned long oid = pItem->m_n14;
    void* pos;
    if (!bSkipPending && m_pCombatRound->m_pPending) {
        pos = m_pCombatRound->m_pPending->m_pInternal->m_pHead;
        while (pos) {
            CSWSCombatRoundPending* pPending =
                (CSWSCombatRoundPending*)m_pCombatRound->m_pPending->m_pInternal->GetAtPos(pos);
            if (pPending && pPending->m_nType == 7 && pPending->m_oid == oid) {
                pPending->m_oid = oid;
                bResult = 1;
                break;
            }
            m_pCombatRound->m_pPending->m_pInternal->GetNext(pos);
        }
    }
    if (!bResult) {
        pos = m_pActions->m_pHead;
        while (pos) {
            CSWSObjectActionNode* pNode = (CSWSObjectActionNode*)m_pActions->GetAtPos(pos);
            if (pNode && pNode->m_nActionId == 0xb && pNode->m_n38 == oid) {
                pNode->m_n38 = oid;
                pNode->m_n3C = a4;
                bResult = 1;
                break;
            }
            m_pActions->GetNext(pos);
        }
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x004ED310
// NONMATCH (37): the original reloads m_pStats in the level loop and loads con/base before GetLevel.
short CSWSCreature::GetMaxHitPoints(int bIncludeToughness)
{
    int nBonus = 0;
    if (m_pStats->m_bIsPC == 1 && GetServerExoApp()->GetPartyTable()->m_nF0 == -1) {
        for (int i = 0; i < m_pStats->GetLevel(0); i++) {
            int nHP = m_pStats->GetLevelStats((unsigned char)i)->m_nHitDie + m_pStats->m_nConMod;
            if (nHP < 1) {
                nHP = 1;
            }
            if (bIncludeToughness) {
                if (m_pStats->HasFeat(0x7c)) {
                    nHP += 2;
                }
                else if (m_pStats->HasFeat(0x54)) {
                    nHP++;
                }
            }
            nBonus += nHP;
        }
        return (short)nBonus;
    }
    if (bIncludeToughness) {
        if (m_pStats->HasFeat(0x7c)) {
            nBonus = 2;
        }
        else if (m_pStats->HasFeat(0x54)) {
            nBonus = 1;
        }
    }
    if (m_pStats->HasFeat(0x5f)) {
        nBonus += 2;
    }
    if (m_nE0 + m_pStats->m_nConMod * m_pStats->GetLevel(0) < m_pStats->GetLevel(0)) {
        return (short)(m_pStats->GetLevel(0) * nBonus + m_pStats->GetLevel(0));
    }
    return (short)((m_pStats->m_nConMod + nBonus) * m_pStats->GetLevel(0) + m_nE0);
}

// FUNCTION: SWKOTOR 0x004ED1D0
// NONMATCH (30): this/loop index in ebp/edi swapped; char sums use other byte registers.
short CSWSCreature::GetArmorClass()
{
    short nAC = 10;
    for (int i = 0; i < m_pStats->m_nNumClasses; i++) {
        CSWClass* pClass = &g_pRules->m_pClasses[m_pStats->GetClass(i)];
        nAC += pClass->GetArmorBonus(m_pStats->GetClassLevel(i, 0));
    }
    CSWSCreatureStats* pStats = m_pStats;
    nAC = (char)(pStats->m_nFC - pStats->m_nFD + pStats->m_nF7) + (nAC + (char)(pStats->m_nF8 - pStats->m_nF9 + pStats->m_nArmorType)) + (char)(pStats->m_nFA - pStats->m_nFB);
    int nCap = 10;
    int nMax = pStats->m_n100 - pStats->m_n101;
    if (nMax <= 10) {
        nCap = nMax;
    }
    nAC = (char)(pStats->GetACNaturalBase() + (pStats->m_nFE - pStats->m_nFF)) + nAC + (char)nCap;
    nAC += m_pStats->GetDEXMod(1);
    nAC += m_pStats->GetTotalACFeatMod();
    if (IsDebilitated()) {
        nAC -= 4;
    }
    return nAC;
}

// FUNCTION: SWKOTOR 0x004EB3A0
// NONMATCH (2 real, 19 counted): the target lookup loads g_pAppManager into ecx instead of eax.
float CSWSCreature::ComputeSpellRange(CExoString sRange, unsigned long oidTarget)
{
    float fRange = 0.0f;
    if (sRange == "P") {
        sRange = "T";
    }
    if (sRange == "P") {
        fRange = g_pRules->m_fRangePersonal;
    }
    else if (sRange == "T") {
        fRange = g_pRules->m_fRangeTouch;
    }
    else if (sRange == "S") {
        fRange = g_pRules->m_fRangeShort;
    }
    else if (sRange == "M") {
        fRange = g_pRules->m_fRangeMedium;
    }
    else if (sRange == "L") {
        fRange = g_pRules->m_fRangeLong;
    }
    else if (sRange == "W") {
        fRange = g_pRules->m_fRangeW;
    }
    fRange += m_pFollowInfo->m_fPersonalSpace - 0.1f;
    CSWSCreature* pTarget = GetServerExoApp()->GetCreatureByGameObjectID(oidTarget);
    if (pTarget) {
        fRange += pTarget->m_pFollowInfo->m_fPersonalSpace - 0.1f;
    }
    return fRange;
}

// FUNCTION: SWKOTOR 0x004ED6E0
// NONMATCH (17): the CreaturePersonalSpace fallback copies with mov instead of fld/fstp (eax/ecx swap).
void CSWSCreature::UpdatePersonalSpace()
{
    float fValue = 0.6f;
    int bOk = g_pRules->Get2DArrays()->GetAppearance()->GetFLOATEntry(m_nAppearanceType, g_nAppearanceColHitRadius, &fValue);
    CSWSCreatureFollowInfo* pInfo = m_pFollowInfo;
    if (bOk == 1) {
        pInfo->m_fHitRadius = fValue;
    }
    else {
        pInfo->m_fHitRadius = 0.6f;
    }
    pInfo->Fn5CEC90();
    if (g_pRules->Get2DArrays()->GetAppearance()->GetFLOATEntry(m_nAppearanceType, g_nAppearanceColPerSpace, &fValue) == 1) {
        m_pFollowInfo->m_fPersonalSpace = fValue;
    }
    else {
        m_pFollowInfo->m_fPersonalSpace = fValue;
    }
    bOk = g_pRules->Get2DArrays()->GetAppearance()->GetFLOATEntry(m_nAppearanceType, g_nAppearanceColCrePerSpace, &fValue);
    pInfo = m_pFollowInfo;
    if (bOk == 1) {
        pInfo->m_fCreaturePersonalSpace = fValue;
    }
    else {
        pInfo->m_fCreaturePersonalSpace = pInfo->m_fPersonalSpace;
    }
    if (g_pRules->Get2DArrays()->GetAppearance()->GetFLOATEntry(m_nAppearanceType, g_nAppearanceColWalkDist, &fValue) == 1) {
        m_pFollowInfo->m_fWalkDist = fValue;
    }
    else {
        m_pFollowInfo->m_fWalkDist = 0.5f;
    }
    if (g_pRules->Get2DArrays()->GetAppearance()->GetFLOATEntry(m_nAppearanceType, g_nAppearanceColHeight, &fValue) == 1) {
        m_pFollowInfo->m_fHeight = fValue;
    }
    else {
        m_pFollowInfo->m_fHeight = 1.0f;
    }
}

// FUNCTION: SWKOTOR 0x004ECEE0
// NONMATCH (28): the original runs both feat switches on one movzx copy (sub esi chain) and pushes it.
int CSWSCreature::UseFeat(unsigned short nFeat, unsigned long oidTarget, int a3, int a4)
{
    int bResult = 0;
    if (m_pStats->HasFeat(nFeat)) {
    int nNext = nFeat;
    int nUse;
    do {
        nUse = nNext;
        nNext = g_pRules->GetFeat((unsigned short)nUse)->m_nSuccessor;
    } while (nNext > 0 && m_pStats->HasFeat((unsigned short)nNext));
    nFeat = (unsigned short)nUse;
    switch (nFeat) {
    case 8:
    case 11:
    case 17:
    case 18:
    case 19:
    case 20:
    case 26:
    case 28:
    case 29:
    case 30:
    case 31:
    case 53:
    case 77:
    case 81:
    case 82:
    case 83:
    case 91:
    case 92:
        m_pCombatRound->AddAttackAction(oidTarget, 0, nFeat, 0x5dc, 1);
        bResult = 1;
        break;
    }
    if (nFeat == 2 || nFeat == 0x19 || nFeat == 0x36) {
        m_nModeFeat = m_nModeFeat ? 0 : nFeat;
    }
    }
    return bResult;
}
