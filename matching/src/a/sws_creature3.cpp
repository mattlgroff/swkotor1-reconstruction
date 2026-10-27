// Worker A round s3: CSWSCreature cutscene/gold/detection/inventory helpers.
#include "a/sws_creature.h"

struct CResStruct {
    unsigned long m_nStructIndex;
};

// FUNCTION: SWKOTOR 0x004F9FB0
int CSWSCreature::AddCutsceneAttackActions(unsigned long oidTarget, int nAnimation, int nResult, int nDamage)
{
    m_pCombatRound->AddCutsceneAttackAction(oidTarget, nAnimation, nResult, nDamage);
    if (!m_b4E0 && GetServerExoApp()->GetClientObjectByObjectId(m_idSelf)) {
        Fn4F1460();
        ClearAllActions(1);
    }
    if (!GetNodeById(0x3f)) {
        nDamage = 1;
        AddAction(0x3f, 0xffff, 1, &nDamage);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004FA050
int CSWSCreature::AddCutsceneMoveActions(unsigned long oidTarget, const Vector& vPosition, int nSpeed)
{
    m_pCombatRound->AddCutsceneMoveActions(oidTarget, vPosition, nSpeed);
    if (!m_b4E0 && GetServerExoApp()->GetClientObjectByObjectId(m_idSelf)) {
        Fn4F1460();
        ClearAllActions(1);
    }
    if (!GetNodeById(0x3f)) {
        nSpeed = 1;
        AddAction(0x3f, 0xffff, 1, &nSpeed);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004F3DB0
void CSWSCreature::AddGold(int nGold, int bFeedback)
{
    int nNew = GetGold() + nGold;
    if (nNew > 999999999) {
        nNew = 999999999;
    }
    int nDelta = nNew - GetGold();
    SetGold(nNew);
    if (bFeedback == 1 && nDelta) {
        CSWCCMessageData* pData = new CSWCCMessageData();
        pData->SetInteger(0, nDelta);
        SendFeedbackMessage(0x94, pData);
    }
}

// FUNCTION: SWKOTOR 0x004F3ED0
void CSWSCreature::RemoveGold(int nGold, int bFeedback)
{
    int nNew = GetGold();
    nNew -= nGold;
    if (nNew < 0) {
        nNew = 0;
    }
    int nDelta = GetGold() - nNew;
    SetGold(nNew);
    if (bFeedback == 1 && nDelta) {
        CSWCCMessageData* pData = new CSWCCMessageData();
        pData->SetInteger(0, nDelta);
        SendFeedbackMessage(0x95, pData);
    }
}

// FUNCTION: SWKOTOR 0x004FB180
void CSWSCreature::LoadFollowInfo(CResGFF* pGFF, CResStruct* pStruct)
{
    CResStruct cStruct;
    if (!pGFF->GetStructFromStruct(&cStruct, pStruct, "FollowInfo")) {
        return;
    }
    if (m_pPartyFollowInfo) {
        return;
    }
    m_pPartyFollowInfo = new CSWSCreaturePartyFollowInfo();
    m_pPartyFollowInfo->Load(pGFF, &cStruct);
}

// FUNCTION: SWKOTOR 0x00500C00
int CSWSCreature::DoStealthDetection(CSWSCreature* pTarget, int bLOS, int* pSeen, int* pHeard, int bArg)
{
    int bSeen = 0;
    if (!pTarget) {
        *pSeen = 0;
        *pHeard = 0;
        return 0;
    }
    if (pTarget->GetArea() != GetArea()) {
        *pSeen = 0;
        *pHeard = 0;
        return 0;
    }
    int bHeard = DoListenDetection(pTarget, bArg);
    if (bLOS) {
        bSeen = DoSpotDetection(pTarget, bArg);
    }
    *pHeard = bHeard;
    *pSeen = bSeen;
    if (bSeen || bHeard) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0051B370
void CSWSCreature::UpdateObjectId(unsigned long oid, int a2, int a3, int a4, int a5)
{
    if (GetIsInList920(oid)) {
        return;
    }
    CSWSCreatureListEntry* pEntry = new CSWSCreatureListEntry;
    pEntry->m_oid = oid;
    pEntry->m_b0 = a2;
    pEntry->m_b1 = a3;
    pEntry->m_n2 = a4;
    pEntry->m_b4 = a5;
    m_l920.Add(pEntry);
}

// FUNCTION: SWKOTOR 0x004FAA70
int CSWSCreature::UnequipItem(CSWSItem* pItem)
{
    pItem->RemoveItemProperties(this, m_pInventory->GetSlotFromItem(pItem));
    m_pInventory->RemoveItem(pItem);
    ComputeArmourClass(pItem, 1, 0);
    int nWeight = m_nTotalWeight;
    m_b344 = 1;
    m_nTotalWeight = nWeight - pItem->GetWeight();
    if (pItem->GetBaseItem() && pItem->GetBaseItem()->m_nAC == 0x2c && m_nStealthMode == 1) {
        ToggleMode(1);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004F3580
// NONMATCH (8): the sound-object vcall loads the object into eax then ecx (original: straight
// into ecx) and m_idSelf is read after the timer store; same tie as UpdateExcitedStateTimer.
void CSWSCreature::SetExcitedState(unsigned char nState)
{
    int nDuration;
    bool bFail;
    {
        CExoString sColumn("DURATION");
        C2DA* pTable = g_pRules->Get2DArrays()->m_pExcitedDuration;
        bFail = !pTable->GetINTEntry(nState, sColumn, &nDuration);
    }
    if (bFail) {
        return;
    }
    if ((unsigned long)nDuration <= m_nExcitedTimer) {
        return;
    }
    m_nExcitedTimer = nDuration;
    CSWSPlayer* pPlayer = GetServerExoApp()->GetClientObjectByObjectId(m_idSelf);
    if (GetArea() && pPlayer) {
        if (GetArea()->m_pSoundObject) {
            if (GetNearestEnemy(30.0f, 0x7f000000, 0x7f000000) != 0x7f000000) {
                GetArea()->m_pSoundObject->Update(1);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x00510EF0
int CSWSCreature::RemoveItem(CSWSItem* pItem, int a2, int a3, CSWSItem** ppItem)
{
    if (!pItem) {
        return 0;
    }
    if (m_pInventory->Fn5A4950(pItem) == 1) {
        RunUnequip(pItem->m_n14, 0x7f000000, a2, ppItem, 0);
        if (ppItem) {
            pItem = *ppItem;
        }
    }
    if (pItem->m_oidPossessor == m_idSelf) {
        if (GetItemRepository(1)->GetItemInRepository(pItem, 0) == 1) {
            GetItemRepository(1)->RemoveItem(pItem);
        }
    } else {
        CGameObject* pObject = GetServerExoApp()->GetGameObject(pItem->m_oidPossessor);
        if (!pObject || pObject->m_nObjectType != OBJECT_TYPE_ITEM) {
            return 0;
        }
        CSWSItem* pContainer = pObject->AsSWSItem();
        if (pContainer->m_oidPossessor != m_idSelf) {
            return 0;
        }
        pContainer->m_pRepository->RemoveItem(pItem);
    }
    pItem->SetPossessor(0x7f000000, 1, a3, 0x7f000000);
    return 1;
}

// FUNCTION: SWKOTOR 0x004F8A50
int CSWSCreature::AddMoveToPointActionToFront(unsigned short nGroup, const Vector& vTarget, unsigned long oidArea,
    unsigned long oidTarget, int bRun, float fRange, float fTolerance, int a7, int a8, int a9, int a10,
    int a11, int a12, const Vector& vOffset, unsigned long a14, unsigned long a15, int a16)
{
    a7 = (a7 << 1) | bRun;
    if (fTolerance != g_fZero) {
        a7 |= 4;
    }
    a7 |= (((a9 & 7) << 1) | a11) << 3;
    if (a10) {
        a7 |= (a10 & 3) << 7;
    }
    a7 |= (((a16 & 1) << 1) | (a12 & 1)) << 9;
    CSWSCreatureFollowInfo* pFollowInfo = m_pFollowInfo;
    m_nAIState = 2;
    pFollowInfo->m_n26C = 0;
    pFollowInfo->m_n270 = 0;
    pFollowInfo->m_n274 = 0;
    AddActionToFront(1, nGroup, 2, (void*)&vTarget.x, 2, (void*)&vTarget.y, 2, (void*)&vTarget.z,
                     3, &oidArea, 3, &oidTarget, 1, &a7, 2, &fRange, 1, &a8, 2, &fTolerance,
                     2, (void*)&vOffset.x, 2, (void*)&vOffset.y, 1, &a14, 1, &a15);
    SetFacingTarget4F34A0(0x7f000000, 0);
    return 1;
}

// FUNCTION: SWKOTOR 0x004F8B60
int CSWSCreature::AddMoveToPointAction(unsigned short nGroup, const Vector& vTarget, unsigned long oidArea,
    unsigned long oidTarget, int bRun, float fRange, float fTolerance, int a7, int a8, int a9, int a10,
    int a11, int a12, const Vector& vOffset, unsigned long a14, unsigned long a15, int a16)
{
    a7 = (a7 << 1) | bRun;
    if (fTolerance != g_fZero) {
        a7 |= 4;
    }
    a7 |= (((a9 & 7) << 1) | a11) << 3;
    if (a10) {
        a7 |= (a10 & 3) << 7;
    }
    a7 |= (((a16 & 1) << 1) | (a12 & 1)) << 9;
    CSWSCreatureFollowInfo* pFollowInfo = m_pFollowInfo;
    m_nAIState = 2;
    pFollowInfo->m_n26C = 0;
    pFollowInfo->m_n270 = 0;
    pFollowInfo->m_n274 = 0;
    AddAction(1, nGroup, 2, (void*)&vTarget.x, 2, (void*)&vTarget.y, 2, (void*)&vTarget.z,
              3, &oidArea, 3, &oidTarget, 1, &a7, 2, &fRange, 1, &a8, 2, &fTolerance,
              2, (void*)&vOffset.x, 2, (void*)&vOffset.y, 1, &a14, 1, &a15);
    SetFacingTarget4F34A0(0x7f000000, 0);
    return 1;
}

// FUNCTION: SWKOTOR 0x004F0310
// NONMATCH (18): nSlot/oidItem EDI/EBP swap; success tails return through ECX in the original.
int CSWSCreature::ReplaceEquipItemActions(CSWSItem* pItem, unsigned long nSlot, int bSkipPending, int nArg)
{
    unsigned long oidItem = pItem->m_n14;
    int bResult;
    void* pos;
    if (!bSkipPending && m_pCombatRound->m_pPending) {
        pos = m_pCombatRound->m_pPending->m_pInternal->m_pHead;
        while (pos) {
            CSWSCombatRoundPending* pPending =
                (CSWSCombatRoundPending*)m_pCombatRound->m_pPending->GetAtPos(pos);
            if (pPending && pPending->m_nType == 6 && (pPending->m_n1C == nSlot || pPending->m_oid == oidItem)) {
                pPending->m_n1C = nSlot;
                pPending->m_oid = oidItem;
                bResult = 1;
                return bResult;
            }
            m_pCombatRound->m_pPending->GetNext(pos);
        }
    }
    pos = m_pActions->m_pHead;
    while (pos) {
        CSWSObjectActionNode* pNode = (CSWSObjectActionNode*)m_pActions->GetAtPos(pos);
        if (pNode && pNode->m_nActionId == 8 && (pNode->m_n3C == nSlot || pNode->m_n38 == oidItem)) {
            pNode->m_n3C = nSlot;
            pNode->m_n38 = oidItem;
            pNode->m_n40 = nArg;
            bResult = 1;
            return bResult;
        }
        m_pActions->GetNext(pos);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004F06D0
// NONMATCH (24): pItem/pPlayer/pMessage register rotation (EDI/EBP/EBX in the original).
int CSWSCreature::AddUnequipActions(CSWSItem* pItem, unsigned long oidRepository, int bToFront, int bSkipPending, int nArg)
{
    unsigned long oidItem = pItem->m_n14;
    CSWSPlayer* pPlayer = GetServerExoApp()->GetClientObjectByObjectId(m_idSelf);
    CSWSMessage* pMessage = GetServerExoApp()->GetSWSMessage();
    if (!m_bE8) {
        return 0;
    }
    if (m_b4E0 == 1 && m_nAC0 == 1) {
        CSWSItem* pWeapon = m_pInventory->GetItemInSlot(2);
        if (pWeapon && pWeapon->m_n14 == oidItem) {
            if (pPlayer && pMessage) {
                pMessage->SendServerToPlayerInventory_UnequipCancel(pPlayer->m_nPlayerID, oidItem);
            }
            SendFeedbackMessage(0xc2, NULL);
            return 0;
        }
    }
    if (UpdateTarget4F05D0(pItem, oidRepository, bSkipPending, nArg)) {
        return 1;
    }
    if (m_b4E0 == 1 && m_nAC0 == 1 && !bSkipPending) {
        m_pCombatRound->AddUnequipAction(oidItem, oidRepository, nArg);
        return 1;
    }
    if (bToFront) {
        AddActionToFront(0xb, 0xffff, 3, &oidItem, 3, &oidRepository, 1, &nArg);
    } else {
        AddAction(0xb, 0xffff, 3, &oidItem, 3, &oidRepository, 1, &nArg);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004F7530
void CSWSCreature::ProcessPendingCombatActions()
{
    if (!m_pCombatRound->GetActionPending()) {
        return;
    }
    CExoLinkedListWrap* pList = m_pCombatRound->m_pPending;
    if (!pList) {
        return;
    }
    void* pos = pList->m_pInternal->m_pTail;
    if (!pos) {
        return;
    }
    CSWSCombatRoundPending* pAction = (CSWSCombatRoundPending*)pList->m_pInternal->GetAtPos(pos);
    while (pos) {
        void* pRemovePos = NULL;
        CSWSCombatRoundPending* pRemove = NULL;
        if (pAction->m_nType == 6) {
            CSWSItem* pItem = GetServerExoApp()->GetItemByGameObjectID(pAction->m_oid);
            if (pItem) {
                AddEquipItemActions(pItem, pAction->m_n1C, 1, 1, pAction->m_n54, pAction->m_n84, 0);
                pRemovePos = pos;
                pRemove = pAction;
            }
        } else if (pAction->m_nType == 7) {
            CSWSItem* pItem = GetServerExoApp()->GetItemByGameObjectID(pAction->m_oid);
            if (pItem) {
                AddUnequipActions(pItem, pAction->m_n20, 1, 1, pAction->m_n54);
                pRemovePos = pos;
                pRemove = pAction;
            }
        }
        pAction = (CSWSCombatRoundPending*)pList->m_pInternal->GetPrev(pos);
        if (pRemovePos) {
            pList->m_pInternal->Remove(pRemovePos);
            if (pRemove) {
                delete pRemove;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x00518B30
int CSWSCreature::GetHasValidPendingAction()
{
    void* pos = m_pCombatRound->m_pPending->m_pInternal->m_pHead;
    if (!pos) {
        return 0;
    }
    CSWSCombatRoundPending* pAction =
        (CSWSCombatRoundPending*)m_pCombatRound->m_pPending->GetAtPos(pos);
    while (pAction) {
        CGameObject* pTarget = NULL;
        if (pAction->m_oid != 0x7f000000) {
            pTarget = GetServerExoApp()->GetGameObject(pAction->m_oid);
            if (pTarget && pTarget->AsSWSCreature() && pTarget->AsSWSCreature()->GetDead()) {
                pTarget = NULL;
            }
        }
        CGameObject* pOther = NULL;
        if (pAction->m_oid44 != 0x7f000000) {
            pOther = GetServerExoApp()->GetGameObject(pAction->m_oid44);
            if (pOther && pOther->AsSWSCreature() && pOther->AsSWSCreature()->GetDead()) {
                pTarget = NULL;
            }
        }
        if (pAction->m_n84 && pTarget && pOther) {
            return 1;
        }
        pAction = (CSWSCombatRoundPending*)m_pCombatRound->m_pPending->GetNext(pos);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0051B440
// NONMATCH (25): the original caches 0 in EBX and 1 in EDI (this in EBP); same blocks otherwise.
int CSWSCreature::CanUseItem(CSWSItem* pItem, int bArg)
{
    int bResult = 1;
    if (!pItem) {
        return 0;
    }
    if (!CheckItemAlignmentRestrictions(pItem) || !CheckItemClassRestrictions(pItem) ||
        !CheckItemRaceRestrictions(pItem) || !CheckItemFeatRestrictions(pItem)) {
        bResult = 0;
    }
    if (m_b9D4 == 1 && m_pStats->GetLevel(0) < pItem->GetMinEquipLevel() &&
        GetServerExoApp()->GetServerInfo()->m_nC8) {
        bResult = 0;
    }
    if (bResult == 1) {
        CSWBaseItem* pBase = g_pRules->GetBaseItem(pItem->m_nBaseItem);
        if (pBase && pBase->m_nWeaponType != 8 && pBase->m_nWeaponWield) {
            int nLevel = m_n4F8;
            if (g_pRules->GetBaseItem(pItem->m_nBaseItem)->m_n1B - nLevel > 1) {
                bResult = 0;
            }
        }
        unsigned long nSlots = pBase->m_nEquipableSlots;
        for (int i = 0; i < 0x12; i++) {
            if (nSlots & (1 << i)) {
                if (!CheckProficiencies(pItem, 1 << i)) {
                    bResult = 0;
                }
                break;
            }
        }
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x005182F0
int CSWSCreature::CheckItemRaceRestrictions(CSWSItem* pItem)
{
    int bAllow = 1;
    if (pItem->GetPropertyByTypeExists(0xa, 0)) {
        CSWItemProperty* pProperty = NULL;
        pItem->FindProperty(&pProperty, 0xa, 0x81);
        if (pProperty) {
            bAllow = 0;
        }
    }
    unsigned char nRestriction = pItem->GetBaseItem()->m_nRaceRestriction;
    if (nRestriction && bAllow) {
        switch (nRestriction) {
        case 1:
            if (m_pStats->m_nRace != RACE_TYPE_0074BF14) {
                return 0;
            }
            break;
        case 2:
            if (m_pStats->m_nRace != RACE_TYPE_0074BF10) {
                return 0;
            }
            break;
        default:
            return 0;
        }
    }
    unsigned char nSubRace = m_pStats->m_nSubRace;
    if (pItem->GetBaseItem()->m_nSubraceMask & (1 << nSubRace)) {
        return 0;
    }
    if (!pItem->GetPropertyByTypeExists(0x2d, 0)) {
        return 1;
    }
    for (int i = 0; i < pItem->m_nPassiveProperties; i++) {
        CSWItemProperty* pProperty = pItem->GetPassiveProperty(i);
        if (pProperty->m_nActivation == 0xff || ((1 << pProperty->m_nActivation) & pItem->m_nPropertyActivationMask)) {
            if (pItem->GetPassiveProperty(i)->m_nPropertyName == 0x2d) {
                pItem->GetPassiveProperty(i);
                unsigned short nRace = m_pStats->m_nRace;
                if (nRace == pItem->GetPassiveProperty(i)->m_nSubType) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00501C40
// NONMATCH (17): this/pRepository EDI/EBX swap only.
void CSWSCreature::ForceEquipClothing()
{
    if (m_bForceEquipClothing) {
        CItemRepository* pRepository = GetItemRepository(1);
        unsigned long oidItem;
        {
            CExoString sTag("g_a_clothes01");
            oidItem = pRepository->FindItemWithTag(sTag);
        }
        CSWSItem* pItem = GetServerExoApp()->GetItemByGameObjectID(oidItem);
        if (!pItem) {
            pItem = new CSWSItem(0x7f000000);
            if (!pItem) {
                goto done;
            }
            CResRef cTemplate("g_a_clothes01");
            if (!pItem->LoadFromTemplate(cTemplate)) {
                delete pItem;
                goto done;
            }
            pItem->m_nFlags288 |= 8;
            pItem->SetPossessor(m_idSelf, 0, 0, 0x7f000000);
        }
        if (pRepository->GetItemInRepository(pItem, 0)) {
            pRepository->RemoveItem(pItem);
        }
        Fn4FEB90(2, pItem, 1, 0, 0);
done:
        m_bForceEquipClothing = 0;
    }
}

// FUNCTION: SWKOTOR 0x00510CD0
// NONMATCH (72): base-item pointers and return-code blocks laid out differently; needs restructuring.
unsigned char CSWSCreature::Fn510CD0(CSWSItem* pItem, unsigned long* pSlot, int a3, int a4)
{
    CSWBaseItem* pRightBase = NULL;
    if (*pSlot != 0x10 && *pSlot != 0x20) {
        return 0;
    }
    CSWSItem* pRight = m_pInventory->GetItemInSlot(0x10);
    CSWSItem* pLeft = m_pInventory->GetItemInSlot(0x20);
    if (pRight) {
        pRightBase = g_pRules->GetBaseItem(pRight->m_nBaseItem);
    }
    if (pLeft) {
        g_pRules->GetBaseItem(pLeft->m_nBaseItem);
    }
    if (!pItem) {
        return 0;
    }
    CSWBaseItem* pBase = g_pRules->GetBaseItem(pItem->m_nBaseItem);
    if (!pRight) {
        if (pLeft) {
            return 0;
        }
        if (*pSlot == 0x20) {
            *pSlot = 0x10;
        }
        return 1;
    }
    int bRanged = pBase->GetIsRangedSlot();
    if (!pLeft) {
        if (bRanged != 1) {
            if ((pBase->m_n1A > 0) == (pRightBase->m_n1A > 0) && pRightBase->GetIsMeleeSlot() == 1) {
                return *pSlot == 0x20 ? 1 : 2;
            }
            *pSlot = 0x10;
            return 2;
        }
    } else if (bRanged != 1) {
        if ((pBase->m_n1A > 0) == (pRightBase->m_n1A > 0)) {
            *pSlot = 0x10;
            return 2;
        }
    }
    *pSlot = 0x10;
    return 3;
}

// FUNCTION: SWKOTOR 0x004F0420
int CSWSCreature::AddEquipItemActions(CSWSItem* pItem, unsigned long nSlot, int bToFront, int bSkipPending,
                                      int nArg5, int nArg6, int bNoReplace)
{
    if (m_pInventory && (nSlot == 2 || nSlot == 1 || nSlot == 0x10 || nSlot == 0x20) && pItem &&
        m_pInventory->Fn5A4950(pItem)) {
        return 0;
    }
    unsigned long oidSelf = m_idSelf;
    CSWSPlayer* pPlayer = GetServerExoApp()->GetClientObjectByObjectId(oidSelf);
    CSWSMessage* pMessage = GetServerExoApp()->GetSWSMessage();
    unsigned long oidItem = pItem->m_n14;
    if (!m_bE8) {
        if (pPlayer && pMessage) {
            pMessage->SendServerToPlayerInventory_EquipCancel(pPlayer->m_nPlayerID, oidItem, nSlot);
        }
        return 0;
    }
    if (m_b4E0 == 1 && m_nAC0 == 1 && nSlot == 2) {
        if (pPlayer && pMessage) {
            pMessage->SendServerToPlayerInventory_EquipCancel(pPlayer->m_nPlayerID, oidItem, nSlot);
        }
        SendFeedbackMessage(0xc1, NULL);
        return 0;
    }
    if (!bNoReplace && ReplaceEquipItemActions(pItem, nSlot, bSkipPending, nArg5)) {
        return 1;
    }
    if (m_b4E0 == 1 && m_nAC0 == 1 && !bSkipPending) {
        m_pCombatRound->AddEquipAction(oidItem, nSlot, nArg5, nArg6);
        return 1;
    }
    if (bToFront) {
        AddActionToFront(8, 0xffff, 3, &oidItem, 1, &nSlot, 1, &nArg5);
    } else {
        AddAction(8, 0xffff, 3, &oidItem, 1, &nSlot, 1, &nArg5);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004F3A60
int CSWSCreature::EquipMostEffectiveArmor()
{
    if (!m_pInventory || !m_pItemRepository) return 0;
    CSWSItem* pBest = 0;
    unsigned long nBestAC = 0;
    CSWSItem* pItem = m_pInventory->GetItemInSlot(2);
    CSWSItem* pCurrent = pItem;
    int i;
    if (pItem && g_pRules->GetBaseItem(pItem->m_nBaseItem)) {
        pBest = pItem;
        nBestAC = pItem->m_nBaseAC;
        for (i = 0; i < pItem->m_nPassiveProperties; i++) {
            CSWItemProperty* pProperty = pItem->GetPassiveProperty(i);
            if (pProperty &&
                (pProperty->m_nActivation == 0xff ||
                 ((1 << pProperty->m_nActivation) & pItem->m_nPropertyActivationMask)) &&
                pProperty->m_nPropertyName == 1)
                nBestAC += pProperty->m_nCostValue;
        }
    }
    for (unsigned long n = 0; n < m_pItemRepository->m_nItems; n++) {
        pItem = m_pItemRepository->ItemListGetItem(n);
        if (!pItem) continue;
        unsigned long nSlot = 2;
        if (pItem->GetBaseItem()->m_nAC < 0x1f || pItem->GetBaseItem()->m_nAC > 0x26) continue;
        if (!CanEquipItem(pItem, &nSlot, 0, 0, 1)) continue;
        if (!g_pRules->GetBaseItem(pItem->m_nBaseItem)) continue;
        unsigned long nAC = pItem->m_nBaseAC;
        for (i = 0; i < pItem->m_nPassiveProperties; i++) {
            CSWItemProperty* pProperty = pItem->GetPassiveProperty(i);
            if (pProperty &&
                (pProperty->m_nActivation == 0xff ||
                 ((1 << pProperty->m_nActivation) & pItem->m_nPropertyActivationMask)) &&
                pProperty->m_nPropertyName == 1)
                nAC += pProperty->m_nCostValue;
        }
        if (nAC > nBestAC) {
            pBest = pItem;
            nBestAC = nAC;
        }
    }
    if (!pBest) return 0;
    if (pCurrent != pBest) AddEquipItemActions(pBest, 2, 0, 0, 0, 0, 0);
    return 1;
}

// FUNCTION: SWKOTOR 0x004FD070
int CSWSCreature::AddUseTalentAtLocationActions(int nType, int nId, Vector* pTarget, unsigned char nMultiClass,
                                                unsigned long oidItem, int nProperty, int a7, unsigned char a8)
{
    unsigned long oidFound;
    int nFound;
    if (m_bE8 && GetArea() && GetArea()->GetId() != 0x7f000000) {
        if (nType == 0) {
            if (oidItem != 0x7f000000 && nProperty != -1) {
                ClearAllActions(1);
                UseItem(oidItem, nProperty, 0, 0x7f000000, *pTarget, GetArea()->GetId(), 0);
                return 1;
            }
            if (m_pStats->GetItemDetailsForPower(nId, &oidFound, &nFound) == 1) {
                ClearAllActions(1);
                UseItem(oidFound, nFound, 0, 0x7f000000, *pTarget, GetArea()->GetId(), 0);
                return 1;
            }
            AddCastSpellActions(nId, nMultiClass, 0, a8, 0, *pTarget, 0x7f000000, 0, 1, 0, 0, 0, 0, -1, a7, 0);
        }
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00500CA0
// NONMATCH (36): frame/out-parameter slot packing (0xc vs 0x10) and early-return block placement.
int CSWSCreature::AddUseTalentOnObjectActions(int nType, int nId, unsigned long oidTarget, unsigned char nMultiClass,
                                              unsigned long oidItem, int nProperty, int a7, unsigned char a8)
{
    unsigned long oidFound;
    CSWSCreature* pTarget = GetServerExoApp()->GetCreatureByGameObjectID(oidTarget);
    if (!pTarget || !m_bE8) {
        return 0;
    }
    Vector vTarget = pTarget->m_vPosition;
    unsigned long oidArea = GetArea()->GetId();
    switch (nType) {
    case 2:
        UseSkill(nId, 0, oidTarget, vTarget, oidArea, 0x7f000000, 0);
        return 1;
    case 1:
        AddAttackActions(oidTarget, nId, 0, 0, 1, 0, 0, 1, 0x2719, 0x5dc, 1, 0, 0, 4, 0);
        return 1;
    case 0:
        if (oidItem != 0x7f000000 && nProperty != -1) {
            UseItem(oidItem, nProperty, 0, oidTarget, vTarget, oidArea, 0);
            return 1;
        } else {
            int nFound;
            if (m_pStats->GetItemDetailsForPower(nId, &oidFound, &nFound) == 1) {
                UseItem(oidFound, nFound, 0, oidTarget, vTarget, oidArea, 0);
                return 1;
            }
            AddCastSpellActions(nId, nMultiClass, 0, a8, 0, vTarget, oidTarget, 0, 0, 0, 0, 0, 0, -1, a7, 0);
            return 1;
        }
    }
    return 1;
}
