// CSWSCombatRound state helpers, action builders and lifecycle
// (behavioural references: src/batches/b004d2_combat_round_states.cpp and
// src/batches/b004d42_combat_round_lifecycle.cpp).
#include "a/sws_combat_round.h"

// FUNCTION: SWKOTOR 0x004D28B0
void CSWSCombatRound::SetRoundSpell(int nSpellParam, int nSpellId)
{
    m_nSpellId = nSpellId;
    m_bSpellCastRound = 1;
    m_pBaseCreature->m_nCombatState = 4;
    m_pBaseCreature->m_nCombatStateParam = nSpellParam;
}

// FUNCTION: SWKOTOR 0x004D28F0
void CSWSCombatRound::SetRoundPaused(int bPaused, unsigned long oidPausedBy)
{
    m_bRoundPaused = bPaused;
    m_oidRoundPausedBy = oidPausedBy;
    if (m_bInfinitePause && !bPaused) {
        m_bInfinitePause = 0;
    }
}

// FUNCTION: SWKOTOR 0x004D2920
void CSWSCombatRound::SetPauseTimer(int nTime, int bInfinite)
{
    m_bInfinitePause = bInfinite;
    if (bInfinite) {
        m_nPauseTimer = nTime;
        return;
    }
    if (nTime == 0) {
        m_nPauseTimer = nTime;
    }
    if (m_nPauseTimer > 0) {
        m_nPauseTimer += nTime;
    }
    else {
        m_nPauseTimer = nTime;
    }
}

// FUNCTION: SWKOTOR 0x004D2970
int CSWSCombatRound::GetActionValid(unsigned long oidActor, int nTime, int nDuration)
{
    if (m_bRoundPaused == 1 && m_oidRoundPausedBy != oidActor &&
        m_nTimer + nDuration < m_nPauseTimer + m_nTimer) {
        return 0;
    }
    if (m_bRoundStarted == 1) {
        return m_nRoundLength - m_nOverlapAmount - m_nTimer - nDuration + 1000 >= nTime;
    }
    return 1;
}

// Defined ahead of CalculateOn/OffHandAttacks: the original calls them.
// FUNCTION: SWKOTOR 0x004D5F70
void CSWSCombatRound::StartCombatRound(unsigned long oidTarget, int bSynchronized, int bMaster,
                                       unsigned short nAction, int nParam940)
{
    m_bSynchronized = bSynchronized;
    m_bMaster = bMaster;
    if (bSynchronized && !bMaster) {
        EndCombatRound(m_bRoundStarted);
    }
    if (m_bMaster) {
        m_oidMaster = m_pBaseCreature->m_idSelf;
        if (m_bSynchronized) {
            m_oidSlave = oidTarget;
            CSWSCreature* pSlave =
                g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidTarget);
            if (pSlave) {
                pSlave->m_pcCombatRound->m_oidMaster = m_oidMaster;
                pSlave->m_pcCombatRound->m_oidSlave = m_oidSlave;
                pSlave->m_pcCombatRound->m_nTimer = 0;
            }
        }
    }
    else {
        m_oidMaster = oidTarget;
    }
    m_bRoundStarted = 1;
    m_nTimer = 0;
    m_bRoundPaused = 0;
    m_nPauseTimer = 0;
    m_oidRoundPausedBy = OBJECT_INVALID;
    m_bSpellCastRound = 0;
    m_nSpellId = 0;
    m_nField940 = nParam940;
    m_nRoundLength = 3000;
    for (int i = 0; i < 5; i++) {
        GetAttack(i)->ClearAttackData();
    }
    CalculateOnHandAttacks(nAction);
    CalculateOffHandAttacks();
    m_nOffHandAttacksTaken = 0;
    m_nExtraAttacksTaken = 0;
    if (m_pBaseCreature->m_idSelf != oidTarget) {
        m_oidField9cc = oidTarget;
    }
    else {
        m_oidField9cc = OBJECT_INVALID;
    }
    m_oidNewAttackTarget = oidTarget;
    m_nField978 = 1;
    m_nField97c = 0;
    m_pBaseCreature->SetCombatTarget(oidTarget, 0);
    m_pBaseCreature->m_nCombatState = 3;
}

// FUNCTION: SWKOTOR 0x004D29F0
void CSWSCombatRound::SetOffHandAttack(int bOffHand)
{
    m_bOffHandAttack = bOffHand;
}

// FUNCTION: SWKOTOR 0x004D2A00
int CSWSCombatRound::GetOffHandAttack()
{
    return m_bOffHandAttack;
}

// Not auto-inlined into StartCombatRound in the original.
#pragma auto_inline(off)
// NONMATCH: sum lands in ecx / action id in ax (original: lea eax / cx).
// FUNCTION: SWKOTOR 0x004D2A10
int CSWSCombatRound::CalculateOnHandAttacks(unsigned short nAction)
{
    m_nOnHandAttacks = m_pBaseCreature->m_pStats->GetNumberOfAttacks() + m_nAdditionalAttacks;
    unsigned short nCheck = nAction;
    if (nCheck == 0xb || nCheck == 0x5b || nCheck == 0x35 || nCheck == 0x1e ||
        nCheck == 0x5c || nCheck == 0x1a) {
        m_nOnHandAttacks++;
    }
    return m_nOnHandAttacks;
}

#pragma auto_inline(on)

// FUNCTION: SWKOTOR 0x004D2A70
int CSWSCombatRound::CalculateOffHandAttacks()
{
    int bOffHand = 0;
    CSWSItem* pItem = m_pBaseCreature->m_pInventory->GetItemInSlot(0x10);
    if (pItem) {
        CSWBaseItem* pBaseItem = g_pRules->GetBaseItem(pItem->m_nBaseItem);
        if (pBaseItem->m_nMinimumSize - m_pBaseCreature->m_nCreatureSize <= 0) {
            pItem = m_pBaseCreature->m_pInventory->GetItemInSlot(0x20);
            if (pItem && g_pRules->GetBaseItem(pItem->m_nBaseItem)->m_bOffHandable &&
                g_pRules->GetBaseItem(pItem->m_nBaseItem)->m_nWeaponWield) {
                bOffHand = 1;
            }
        }
        else if (pItem->GetBaseItem()->m_nWeaponWield == 3) {
            bOffHand = 1;
        }
    }
    m_nOffHandAttacks = bOffHand;
    return bOffHand;
}

// FUNCTION: SWKOTOR 0x004D2B30
unsigned char CSWSCombatRound::GetCurrentAction()
{
    return m_nCurrentAction;
}

// FUNCTION: SWKOTOR 0x004D2B40
void CSWSCombatRound::SetCurrentAction(unsigned char nAction)
{
    m_nCurrentAction = nAction;
}

// Out-of-line copies of the inline GetAttack / GetTotalAttacks (address taken
// here so the COMDATs are emitted).
// FUNCTION: SWKOTOR 0x004D2B50 SYMBOL: ?GetAttack@CSWSCombatRound@@QAEPAVCSWSCombatAttackData@@H@Z
CSWSCombatAttackData* (CSWSCombatRound::*g_pfnGetAttack)(int) = &CSWSCombatRound::GetAttack;
// FUNCTION: SWKOTOR 0x004D2EB0 SYMBOL: ?GetTotalAttacks@CSWSCombatRound@@QAEHXZ
int (CSWSCombatRound::*g_pfnGetTotalAttacks)() = &CSWSCombatRound::GetTotalAttacks;

// FUNCTION: SWKOTOR 0x004D2B70
int CSWSCombatRound::ResolveChoreographedState(unsigned long oidTarget)
{
    int bResult = 0;
    CSWSItem* pWeapon = m_pBaseCreature->m_pInventory->GetItemInSlot(0x10);
    if (pWeapon) {
        unsigned char nWield = pWeapon->GetBaseItem()->m_nWeaponWield;
        if (pWeapon->GetBaseItem()->m_bRangedWeapon <= 0 && nWield != 1 && nWield != 8) {
            CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidTarget);
            if (pObject && pObject->AsSWSCreature()) {
                CSWInventory* pInventory = pObject->AsSWSCreature()->m_pInventory;
                pWeapon = pInventory->GetItemInSlot(0x10);
                if (pWeapon) {
                    nWield = pWeapon->GetBaseItem()->m_nWeaponWield;
                    if (pWeapon->GetBaseItem()->m_bRangedWeapon <= 0 && nWield != 1 &&
                        nWield != 8) {
                        bResult = 1;
                    }
                }
            }
        }
    }
    return bResult;
}

// NONMATCH: this/bResult in edi/ebx (original ebx/edi), otherwise identical.
// FUNCTION: SWKOTOR 0x004D2C30
int CSWSCombatRound::ResolveEngagedState(unsigned long oidTarget)
{
    int bResult = 0;
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidTarget);
    if (pObject && pObject->AsSWSCreature()) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        int bStarted = pCreature->m_pcCombatRound->m_bRoundStarted;
        if (!bStarted && pCreature->m_oidAttackTarget == OBJECT_INVALID) {
            bResult = 1;
        }
        unsigned long oidSelf = m_pBaseCreature->m_idSelf;
        unsigned long oidAttackTarget = pCreature->m_oidAttackTarget;
        if (oidAttackTarget == oidSelf || pCreature->m_oidAttemptedSpellTarget == oidSelf) {
            bResult = 1;
        }
        if (bStarted && oidAttackTarget == OBJECT_INVALID) {
            bResult = 1;
        }
        CSWCPartyMember* pMember =
            g_pAppManager->GetClientExoApp()->GetParty()->GetPartyMember(0);
        if (pMember && oidTarget == pMember->GetCreature()->m_idServer) {
            if (pCreature->m_oidAttackTarget == OBJECT_INVALID &&
                pCreature->m_oidAttemptedSpellTarget == pCreature->m_oidAttackTarget) {
                bResult = 0;
            }
            if ((pCreature->m_oidAttackTarget != OBJECT_INVALID &&
                 pCreature->m_oidAttackTarget != m_pBaseCreature->m_idSelf) ||
                (pCreature->m_oidAttemptedSpellTarget != OBJECT_INVALID &&
                 pCreature->m_oidAttemptedSpellTarget != m_pBaseCreature->m_idSelf) ||
                (pCreature->m_oidAttemptedAttackTarget != OBJECT_INVALID &&
                 pCreature->m_oidAttemptedAttackTarget != m_pBaseCreature->m_idSelf)) {
                bResult = 0;
            }
        }
        if (pCreature->IsDebilitated()) {
            return 0;
        }
    }
    return bResult;
}

// NONMATCH: this/bResult register swap as in ResolveEngagedState.
// FUNCTION: SWKOTOR 0x004D2D60
int CSWSCombatRound::ResolveMasterState(unsigned long oidTarget, int* pbMaster)
{
    int bResult = 0;
    if (*pbMaster) {
        CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidTarget);
        if (pObject && pObject->AsSWSCreature()) {
            CSWSCreature* pCreature = pObject->AsSWSCreature();
            if (pCreature->m_oidAttackTarget == OBJECT_INVALID) {
                bResult = 1;
            }
            CSWSCreature* pSelf = m_pBaseCreature;
            if (pCreature->m_oidAttackTarget == pSelf->m_idSelf &&
                pCreature->m_pcCombatRound->m_bMaster == 0) {
                bResult = 1;
            }
            if (pSelf->m_bPlayerCharacter) {
                return 1;
            }
            CSWCPartyMember* pMember =
                g_pAppManager->GetClientExoApp()->GetParty()->GetPartyMember(0);
            if (pMember) {
                pMember->GetCreature();
            }
        }
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x004D2E20
int CSWSCombatRound::HasCreatureWeapons()
{
    if (m_pBaseCreature->m_pInventory->GetItemInSlot(0x10) == 0 &&
        m_pBaseCreature->m_pInventory->GetItemInSlot(0x20) == 0 &&
        (m_pBaseCreature->m_pInventory->GetItemInSlot(0x4000) ||
         m_pBaseCreature->m_pInventory->GetItemInSlot(0x8000) ||
         m_pBaseCreature->m_pInventory->GetItemInSlot(0x10000))) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004D3440
void CSWSCombatRound::DecrementRoundLength(int nTime, int bFlag)
{
    int nLength;
    if (m_bRoundStarted == 0) {
        return;
    }
    if (m_nRoundLength - nTime < 0 && 1000 - m_nOverlapAmount >= nTime - m_nRoundLength) {
        m_nRoundLength = nTime + 1;
        m_nOverlapAmount = m_nOverlapAmount - m_nRoundLength + nTime + 1;
    }
    else if (m_nRoundLength - nTime < 0 && bFlag) {
        m_nRoundLength = nTime;
        m_nBleedTimer = m_nBleedTimer - m_nRoundLength + nTime;
    }
    CExoLinkedListPosition pos = m_pScheduledActions->GetHeadPos();
    while (pos) {
        CSWSCombatRoundAction* pAction = m_pScheduledActions->GetAtPos(pos);
        nLength = m_nRoundLength;
        if (nLength > 0) {
            pAction->m_nActionTimer =
                (int)((float)pAction->m_nActionTimer / nLength * (nLength - nTime));
        }
        else {
            pAction->m_nActionTimer = 0;
        }
        m_pScheduledActions->GetNext(pos);
    }
    nLength = m_nRoundLength;
    float fRatio;
    if (nLength == 0) {
        fRatio = 1.0f;
    }
    else {
        fRatio = (float)m_nTimer / nLength;
    }
    nLength -= nTime;
    m_nTimer = (int)(nLength * fRatio);
    if (nLength < 0) {
        m_nRoundLength = nTime;
        m_nOverlapAmount = m_nOverlapAmount - m_nRoundLength + nTime;
    }
    m_nRoundLength -= nTime;
}

// FUNCTION: SWKOTOR 0x004D3580
int CSWSCombatRound::GetActionPending()
{
    if (m_pScheduledActions && m_pScheduledActions->GetHeadPos()) {
        if (m_nTimer >= m_pScheduledActions->GetAtPos(m_pScheduledActions->GetHeadPos())->m_nActionTimer) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004D35C0
int CSWSCombatRound::GetSpellActionPending()
{
    if (m_pScheduledActions) {
        CExoLinkedListPosition pos = m_pScheduledActions->GetHeadPos();
        if (pos) {
            CSWSCombatRoundAction* pAction = m_pScheduledActions->GetAtPos(pos);
            while (pos) {
                if (pAction && m_nTimer >= pAction->m_nActionTimer &&
                    pAction->m_nActionType == 2) {
                    return 1;
                }
                pAction = m_pScheduledActions->GetNext(pos);
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004D3630
CSWSCombatRoundAction* CSWSCombatRound::GetAction()
{
    if (m_pScheduledActions) {
        CSWSCombatRoundAction* pAction = m_pScheduledActions->RemoveHead();
        if (pAction && m_nTimer >= pAction->m_nActionTimer) {
            return pAction;
        }
    }
    return 0;
}

// NONMATCH: pos gets a fresh slot (push ecx); the original reuses nIndex's slot.
// FUNCTION: SWKOTOR 0x004D3660
void CSWSCombatRound::AddAction(CSWSCombatRoundAction* pAction, int nIndex)
{
    CExoLinkedListPosition pos;
    if (m_pScheduledActions && m_pScheduledActions->Count() < 4) {
        int nInsertAt = nIndex;
        pos = m_pScheduledActions->GetHeadPos();
        if (nInsertAt == -1) {
            while (pos) {
                if (pAction->m_nActionTimer < m_pScheduledActions->GetAtPos(pos)->m_nActionTimer) {
                    m_pScheduledActions->AddBefore(pAction, pos);
                    return;
                }
                m_pScheduledActions->GetNext(pos);
            }
            m_pScheduledActions->AddTail(pAction);
            return;
        }
        int i = 0;
        while (pos && i < nInsertAt) {
            m_pScheduledActions->GetNext(pos);
            i++;
        }
        if (pos) {
            m_pScheduledActions->AddBefore(pAction, pos);
        }
        else {
            m_pScheduledActions->AddTail(pAction);
        }
        return;
    }
    if (pAction) {
        delete pAction;
    }
}

// FUNCTION: SWKOTOR 0x004D3770
void CSWSCombatRound::RemoveAllActions()
{
    if (m_pScheduledActions) {
        while (!m_pScheduledActions->IsEmpty()) {
            CSWSCombatRoundAction* pAction = m_pScheduledActions->RemoveHead();
            if (pAction) {
                delete pAction;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x004D37B0
int CSWSCombatRound::RemoveLastAction()
{
    int bResult = 0;
    if (m_pScheduledActions && !m_pScheduledActions->IsEmpty()) {
        CSWSCombatRoundAction* pAction = m_pScheduledActions->RemoveTail();
        if (pAction) {
            delete pAction;
        }
        bResult = 1;
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x004D37E0
void CSWSCombatRound::ClearAllAttacks()
{
    for (int i = 0; i < 5; i++) {
        GetAttack(i)->ClearAttackData();
    }
}

// NONMATCH: original keeps the new action in ECX across Clear() (whole-program
// register knowledge, likely /GL); everything else identical. Same for the
// other inlined-ctor builders below and 0x004D2FF0.
// FUNCTION: SWKOTOR 0x004D3810
void CSWSCombatRound::AddCutsceneAttackAction(unsigned long oidTarget, int nParam78,
                                              int nParam7c, int nParam80)
{
    CSWSCombatRoundAction* pAction = new CSWSCombatRoundAction;
    pAction->m_nActionType = 1;
    pAction->m_oidTarget = oidTarget;
    pAction->m_nAnimation = 0x2719;
    pAction->m_nAnimationTime = (g_nCombatAnimationTime + 3000) / 2;
    pAction->m_nCutsceneParam78 = nParam78;
    pAction->m_bCutscene = 1;
    pAction->m_nCutsceneParam7c = nParam7c;
    pAction->m_nCutsceneParam80 = nParam80;
    AddAction(pAction, -1);
    if (m_pBaseCreature->m_oidAttackTarget == OBJECT_INVALID) {
        m_pBaseCreature->m_oidAttackTarget = oidTarget;
    }
}

// FUNCTION: SWKOTOR 0x004D38B0
void CSWSCombatRound::AddAttackAction(unsigned long oidTarget, unsigned short nFeat,
                                      int nParam84, int nUnused, int nNumAttacks)
{
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidTarget);
    if (pObject == 0 || pObject->AsSWSObject() == 0) {
        return;
    }
    if (pObject->AsSWSObject()->GetIsInConversation()) {
        return;
    }
    if (pObject->AsSWSDoor() && pObject->AsSWSDoor()->m_bOpen) {
        return;
    }
    CSWSCombatRoundAction* pAction = new CSWSCombatRoundAction;
    if (nFeat) {
        pAction->m_nActionType = 0xb;
        pAction->m_nParam5c = nFeat;
    }
    else {
        pAction->m_nActionType = 1;
    }
    pAction->m_nParam84 = nParam84;
    pAction->m_nAnimation = 0x2719;
    pAction->m_nAnimationTime = (g_nCombatAnimationTime + 3000) / 2;
    pAction->m_nNumAttacks = nNumAttacks;
    pAction->m_oidTarget = oidTarget;
    AddAction(pAction, -1);
    if (m_pBaseCreature->m_oidAttackTarget == OBJECT_INVALID) {
        m_pBaseCreature->m_oidAttackTarget = oidTarget;
    }
}

// NONMATCH: ECX kept across Clear() in the original (see 0x004D3810).
// FUNCTION: SWKOTOR 0x004D39B0
void CSWSCombatRound::AddSWSpellAction(int nSpellId, int nSpellClass, int nParam2c,
                                       int nParam30, int nParam34, Vector vTarget,
                                       unsigned long oidTarget, int nParam48, int nParam4c,
                                       unsigned char nParam50, int nParam54, int nParam5c,
                                       unsigned char nParam60, int nParam84)
{
    CSWSCombatRoundAction* pAction = new CSWSCombatRoundAction;
    pAction->m_nParam84 = nParam84;
    pAction->m_nSpellClass = nSpellClass;
    pAction->m_nSpellId = nSpellId;
    pAction->m_nSpellParam30 = nParam30;
    pAction->m_nSpellParam2c = nParam2c;
    pAction->m_nSpellParam34 = nParam34;
    pAction->m_vTargetLocation = vTarget;
    pAction->m_nSpellParam48 = nParam48;
    pAction->m_nSpellParam4c = nParam4c;
    pAction->m_nSpellParam50 = nParam50;
    pAction->m_nParam54 = nParam54;
    pAction->m_nParam5c = nParam5c;
    pAction->m_nActionType = 9;
    pAction->m_nAnimation = 0x2721;
    pAction->m_nAnimationTime = 500;
    pAction->m_oidSpellTarget = oidTarget;
    pAction->m_nSpellParam60 = nParam60;
    if (m_pBaseCreature->m_oidAttemptedSpellTarget == OBJECT_INVALID) {
        m_pBaseCreature->m_oidAttemptedSpellTarget = oidTarget;
    }
    AddAction(pAction, -1);
}

// NONMATCH: ECX kept across Clear() in the original (see 0x004D3810).
// FUNCTION: SWKOTOR 0x004D3A90
void CSWSCombatRound::AddSWItemSpellAction(unsigned long oidItemTarget, int nParam68,
                                           int nParam6c, Vector vTarget,
                                           unsigned long oidTarget, int nParam48,
                                           int nParam84)
{
    CSWSCombatRoundAction* pAction = new CSWSCombatRoundAction;
    pAction->m_nParam84 = nParam84;
    pAction->m_oidItemSpellTarget = oidItemTarget;
    pAction->m_nItemSpellParam68 = nParam68;
    pAction->m_nItemSpellParam6c = nParam6c;
    pAction->m_vTargetLocation = vTarget;
    pAction->m_nActionType = 0xa;
    pAction->m_oidSpellTarget = oidTarget;
    pAction->m_nSpellParam48 = nParam48;
    if (m_pBaseCreature->m_oidAttemptedSpellTarget == OBJECT_INVALID) {
        m_pBaseCreature->m_oidAttemptedSpellTarget = oidTarget;
    }
    AddAction(pAction, -1);
}

// NONMATCH: ECX kept across Clear() in the original (see 0x004D3810).
// FUNCTION: SWKOTOR 0x004D3B30
void CSWSCombatRound::AddCutsceneMoveActions(unsigned long oidTarget, Vector vTarget,
                                             int nParam54)
{
    CSWSCombatRoundAction* pAction = new CSWSCombatRoundAction;
    pAction->m_nParam54 = nParam54;
    pAction->m_oidTarget = oidTarget;
    pAction->m_vTargetLocation = vTarget;
    pAction->m_nActionType = 0xc;
    AddAction(pAction, -1);
}

// FUNCTION: SWKOTOR 0x004D3BA0
void CSWSCombatRound::RemoveSpellAction()
{
    if (m_pScheduledActions) {
        CExoLinkedListPosition pos = m_pScheduledActions->GetHeadPos();
        while (pos) {
            CSWSCombatRoundAction* pAction = m_pScheduledActions->GetAtPos(pos);
            if (pAction && pAction->m_nActionType == 2) {
                CExoLinkedListPosition posRemove = pos;
                m_pScheduledActions->GetNext(pos);
                m_pScheduledActions->Remove(posRemove);
                delete pAction;
            }
            else {
                m_pScheduledActions->GetNext(pos);
            }
        }
    }
}

// NONMATCH: ECX kept across Clear() in the original (see 0x004D3810).
// FUNCTION: SWKOTOR 0x004D3C30
void CSWSCombatRound::AddEquipAction(unsigned long oidItem, int nInventorySlot, int nParam54,
                                     int nParam84)
{
    if (g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(oidItem) == 0) {
        return;
    }
    DecrementRoundLength(1500, 1);
    CSWSCombatRoundAction* pAction = new CSWSCombatRoundAction;
    pAction->m_nParam84 = nParam84;
    pAction->m_nActionType = 6;
    pAction->m_nActionTimer = 0;
    pAction->m_nAnimationTime = 1500;
    pAction->m_oidTarget = oidItem;
    pAction->m_nInventorySlot = nInventorySlot;
    pAction->m_nParam54 = nParam54;
    m_pScheduledActions->AddHead(pAction);
}

// NONMATCH: ECX kept across Clear() in the original (see 0x004D3810).
// FUNCTION: SWKOTOR 0x004D3CE0
void CSWSCombatRound::AddUnequipAction(unsigned long oidItem, unsigned long oidRepository,
                                       int nParam54)
{
    CSWSItem* pItem = g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(oidItem);
    if (pItem && !m_pBaseCreature->CanUnequipWeapon(pItem)) {
        return;
    }
    DecrementRoundLength(1500, 1);
    CSWSCombatRoundAction* pAction = new CSWSCombatRoundAction;
    pAction->m_nActionType = 7;
    pAction->m_nActionTimer = 0;
    pAction->m_nAnimationTime = 1500;
    pAction->m_oidTarget = oidItem;
    pAction->m_oidItem = oidRepository;
    pAction->m_nParam54 = nParam54;
    m_pScheduledActions->AddHead(pAction);
}

// FUNCTION: SWKOTOR 0x004D3DA0
int CSWSCombatRound::GetWeaponAttackType()
{
    int bCreatureWeapons = HasCreatureWeapons();
    int bExtraAttack;
    int bOffHand = m_bOffHandAttack;
    if (m_nEffectAttacks || m_nAdditionalAttacks) {
        bExtraAttack = m_nCurrentAttack + 1 > m_nOnHandAttacks;
    }
    else {
        bExtraAttack = 0;
    }
    if (!bCreatureWeapons) {
        if (!bOffHand && !bExtraAttack) {
            return m_pBaseCreature->m_pInventory->GetItemInSlot(0x10) ? 1 : 7;
        }
        if (bOffHand) {
            return 2;
        }
        return m_pBaseCreature->m_pInventory->GetItemInSlot(0x10) ? 6 : 8;
    }
    return m_pBaseCreature->m_pInventory->GetItemInSlot(0x4000) ? 3 : 0;
}

// FUNCTION: SWKOTOR 0x004D3E50
void CSWSCombatRound::UpdateAttackTargetForAllAction(unsigned long oidTarget)
{
    if (m_pScheduledActions) {
        CExoLinkedListPosition pos = m_pScheduledActions->GetHeadPos();
        while (pos) {
            CSWSCombatRoundAction* pAction = m_pScheduledActions->GetAtPos(pos);
            if (pAction->m_nActionType == 1 && pAction->m_bRetargettable == 1) {
                pAction->m_oidTarget = oidTarget;
            }
            m_pScheduledActions->GetNext(pos);
        }
    }
}

// FUNCTION: SWKOTOR 0x004D42E0
void CSWSCombatRound::DEBUGDumpCombatRoundToScreen(int nUnused, CExoString& sMessage)
{
    sMessage.CStr();
    CExoLinkedListPosition pos = m_pScheduledActions->GetHeadPos();
    if (pos) {
        CSWSCombatRoundAction* pAction = m_pScheduledActions->GetAtPos(pos);
        while (pAction) {
            pAction = m_pScheduledActions->GetNext(pos);
        }
    }
}

// NONMATCH: original caches timer/started in edx/ecx, spills ebx around the
// m_bMaster test and reloads GetHead(); condition chain otherwise the same.
// FUNCTION: SWKOTOR 0x004D4C10
void CSWSCombatRound::IncrementTimer(int nTime)
{
    m_nTimer = m_nTimer + nTime;
    if (m_bRoundStarted && m_nTimer >= m_nRoundLength && m_bSpellCastRound) {
        CExoLinkedListNode* pNode =
            (CExoLinkedListNode*)m_pBaseCreature->m_lQueuedActions.GetHeadPos();
        if (pNode && pNode->pObject &&
            m_pBaseCreature->m_lQueuedActions.GetHead()->m_nActionId == 0xf) {
            return;
        }
        EndCombatRound(1);
        return;
    }
    if (m_bRoundStarted && m_nTimer >= m_nRoundLength &&
        !(m_bSynchronized && !m_bMaster) && !m_bSpellCastRound) {
        EndCombatRound(1);
        return;
    }
    if (m_bRoundStarted && m_nTimer >= m_nRoundLength && m_bSynchronized && !m_bMaster) {
        CExoString sMessage;
        unsigned long oidMaster = m_oidMaster;
        CGameObject* pMaster = g_pAppManager->GetServerExoApp()->GetGameObject(oidMaster);
        if (pMaster == 0) {
            CSWSCreature* pCreature = m_pBaseCreature;
            sMessage.Format("CSWSCombatRound::IncrementTimer - %s Master cannot be found and round has expired; Resetting",
                            pCreature->m_sTag.CStr());
        }
        else {
            sMessage.Format("CSWSCombatRound::IncrementTimer - %s Master IS found (%x) and round has expired (%d %d); Resetting",
                            m_pBaseCreature->m_sTag.CStr(), ((CSWCCreature*)pMaster)->m_idServer,
                            m_nTimer, m_nRoundLength);
        }
        m_oidMaster = OBJECT_INVALID;
        EndCombatRound(1);
        return;
    }
    if (m_nTimer < 0) {
        CExoString sMessage;
        CSWSCreature* pCreature = m_pBaseCreature;
        sMessage.Format("CSWSCombatRound::IncrementTimer - %s Timer is negative at %d; Ending combat round and resetting",
                        pCreature->m_sTag.CStr(), m_nTimer);
        EndCombatRound(1);
        return;
    }
    if (m_bSynchronized && !m_bMaster && m_oidMaster == m_pBaseCreature->m_idSelf) {
        m_bSynchronized = 0;
        m_oidMaster = OBJECT_INVALID;
    }
}

// FUNCTION: SWKOTOR 0x004D4E80
void CSWSCombatRound::DecrementPauseTimer(int nTime)
{
    CExoString sMessage;
    unsigned long oidMaster = m_oidMaster;
    if (g_pAppManager->GetServerExoApp()->GetGameObject(oidMaster) == 0) {
        CSWSCreature* pCreature = m_pBaseCreature;
        sMessage.Format("CSWSCombatRound::DecrementPauseTimer - %s Master cannot be found expire the round; Resetting",
                        pCreature->m_sTag.CStr());
        m_oidMaster = OBJECT_INVALID;
        EndCombatRound(1);
    }
    else {
        m_nPauseTimer -= nTime;
        if (m_nPauseTimer <= 0 && !m_bInfinitePause) {
            m_nPauseTimer = 0;
            m_bRoundPaused = 0;
            m_oidRoundPausedBy = OBJECT_INVALID;
        }
    }
}

// FUNCTION: SWKOTOR 0x004D4F60
void CSWSCombatRound::ClearAllSpecialAttacks()
{
    for (int i = m_nCurrentAttack; i < (unsigned char)GetTotalAttacks(); i++) {
        if (GetAttack(i)->m_nSpecialAttack != 0) {
            GetAttack(i)->m_nSpecialAttack = 0;
        }
    }
    if (m_alstSpecialAttacks.element) {
        delete[] m_alstSpecialAttacks.element;
        m_alstSpecialAttacks.element = 0;
        m_alstSpecialAttacks.array_size = 0;
    }
    m_alstSpecialAttacks.num = 0;
}

// NONMATCH: original tail-calls GetItemInSlot (pop esi/edi; mov [esp+4], slot;
// jmp) with saved registers; cl 13.00.9466 only tail-calls frameless functions.
// FUNCTION: SWKOTOR 0x004D4FF0
CSWSItem* CSWSCombatRound::GetCurrentAttackWeapon(int nType)
{
    if (nType == 0) {
        nType = GetWeaponAttackType();
    }
    if (nType == 1 || nType == 6) {
        return m_pBaseCreature->m_pInventory->GetItemInSlot(0x10);
    }
    if (nType == 2) {
        CSWSItem* pItem = m_pBaseCreature->m_pInventory->GetItemInSlot(0x10);
        if (pItem && g_pRules->m_pBaseItemArray->GetBaseItem(pItem->m_nBaseItem)->m_nWeaponWield == 3) {
            return pItem;
        }
        return m_pBaseCreature->m_pInventory->GetItemInSlot(0x20);
    }
    if (nType == 3) {
        return m_pBaseCreature->m_pInventory->GetItemInSlot(0x4000);
    }
    if (nType == 4) {
        return m_pBaseCreature->m_pInventory->GetItemInSlot(0x8000);
    }
    if (nType == 5) {
        return m_pBaseCreature->m_pInventory->GetItemInSlot(0x10000);
    }
    if (nType == 7 || nType == 8) {
        return m_pBaseCreature->m_pInventory->GetItemInSlot(8);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004D5CB0
CSWSCombatRound::CSWSCombatRound(CSWSCreature* pCreature)
{
    m_nField000 = 0;
    m_bRoundStarted = 0;
    m_bSpellCastRound = 0;
    m_nSpellId = 0;
    m_bRoundPaused = 0;
    m_nField940 = 0;
    m_oidRoundPausedBy = OBJECT_INVALID;
    m_nPauseTimer = 0;
    m_bInfinitePause = 0;
    m_pScheduledActions = new CExoLinkedList<CSWSCombatRoundAction>;
    m_nField984 = 1;
    m_nField988 = 1;
    m_pBaseCreature = pCreature;
    m_nField968 = 0;
    m_nCurrentAttack = 0;
    m_bOffHandAttack = 0;
    m_nOnHandAttacks = 0;
    m_nOffHandAttacks = 0;
    m_nOffHandAttacksTaken = 0;
    m_nExtraAttacksTaken = 0;
    m_nEffectAttacks = 0;
    m_nAdditionalAttacks = 0;
    m_nField980 = 0;
    m_nField978 = 0;
    m_nField97c = 0;
    m_nTimer = 0;
    m_nField948 = 0;
    m_nRoundLength = 0;
    m_nBleedTimer = 0;
    m_nOverlapAmount = 0;
    m_nField9a8 = 0;
    m_oidField98c = OBJECT_INVALID;
    m_nAttackID = 0;
    m_oidNewAttackTarget = OBJECT_INVALID;
    m_bSynchronized = 0;
    m_bMaster = 0;
    m_pField9c8 = 0;
    m_oidField9cc = OBJECT_INVALID;
    m_nField9d4 = 0;
    m_nCurrentAction = 0;
    m_nField974 = 0xff;
    memset(m_pcAttackList, 0, sizeof(m_pcAttackList));
}

// FUNCTION: SWKOTOR 0x004D5E60
CSWSCombatRound::~CSWSCombatRound()
{
    if (m_pScheduledActions) {
        while (!m_pScheduledActions->IsEmpty()) {
            CSWSCombatRoundAction* pAction = m_pScheduledActions->RemoveHead();
            if (pAction) {
                delete pAction;
            }
        }
        delete m_pScheduledActions;
        m_pScheduledActions = 0;
    }
    if (m_pField9c8) {
        delete m_pField9c8;
        m_pField9c8 = 0;
    }
}

// NONMATCH: original returns ECX after Clear() without saving it (see 0x004D3810).
// FUNCTION: SWKOTOR 0x004D2FF0
CSWSCombatRoundAction::CSWSCombatRoundAction() throw()
{
    Clear();
}

#pragma auto_inline(off)
// FUNCTION: SWKOTOR 0x004D2F30 SYMBOL: ??0?$CExoLinkedList@VCSWSCombatRoundAction@@@@QAE@XZ
template <> CExoLinkedList<CSWSCombatRoundAction>::CExoLinkedList()
{
    m_pcExoLinkedListInternal = new CExoLinkedListInternal;
}
#pragma auto_inline(on)
