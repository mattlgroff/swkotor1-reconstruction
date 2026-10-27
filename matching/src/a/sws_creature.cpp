// CSWSCreature medium leaves (0x004EAE30-0x005140E5; behavioural
// references: src/batches/b004ea_item_property_party_actions.cpp,
// src/batches/b004e5_enhancement_creature_ai.cpp, src/batches/b004ed_creature_misc.cpp).
#include <stdlib.h>
#include "a/sws_creature.h"

// FUNCTION: SWKOTOR 0x004EAE30
CSWGuiInterfaceAction::CSWGuiInterfaceAction()
{
    Reset();
}

// FUNCTION: SWKOTOR 0x004EB510
int CSWSCreature::AddDriveAction(unsigned short nGroupId, char* pVector, unsigned long b,
                                 unsigned long c, unsigned long d, unsigned long e)
{
    if (GetArea() == NULL) {
        return 0;
    }
    unsigned char bTrue = 1;
    if (m_b8E8) {
        d &= 0xfffd;
    }
    AddAction(0x33, nGroupId, 2, pVector, 2, pVector + 4, 1, &b, 1, &c, 1, &d, 1, &e,
              1, &bTrue);
    return 1;
}

// FUNCTION: SWKOTOR 0x004EB5A0
int CSWSCreature::AddPathfindingWaitActionToFront(unsigned short nGroupId)
{
    float fWait = 0.3f;
    if (GetServerExoApp()->GetClientObjectByObjectId(m_idSelf)) {
        fWait = 0.1f;
    }
    AddActionToFront(0x1e, nGroupId, 2, &fWait);
    return 1;
}

// FUNCTION: SWKOTOR 0x004ECE80
void CSWSCreature::ComputeArmourClass(CSWSItem* pItem, int a, int b)
{
    CSWSItem* pArmor = m_pInventory->GetItemInSlot(2);
    unsigned char nType = 0;
    if (pArmor) {
        CSWBaseItem* pBase = pArmor->GetBaseItem();
        if (pBase) {
            nType = pBase->m_nArmorType;
        }
    }
    m_pStats->m_nArmorType = nType;
}

// FUNCTION: SWKOTOR 0x004ECFF0
int CSWSCreature::AddAppearActions()
{
    if (m_bE8 == 0) {
        return 0;
    }
    AddAction(0x34, 0xffff, 0, NULL);
    SetGroupInterruptable(m_nAIGroupId, 0);
    return 1;
}

// FUNCTION: SWKOTOR 0x004ED060
// NONMATCH (7): original tests bDestroy first and pushes it in both arms of the event-type branch.
int CSWSCreature::AddDisappearActions(int bDestroy)
{
    if (m_bE8 == 0) {
        return 0;
    }
    AddAction(0x35, 0xffff, 0, NULL);
    SetGroupInterruptable(m_nAIGroupId, 0);
    unsigned long oidSelf = m_idSelf;
    int nEvent;
    if (bDestroy) {
        nEvent = EVENT_AI_SOON;
    }
    else {
        nEvent = EVENT_AI_LATER;
    }
    GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
        0, 2000, oidSelf, oidSelf, nEvent, (void*)bDestroy);
    return 1;
}
// FUNCTION: SWKOTOR 0x004ED110
// NONMATCH (4): vcall on the area sound object loads it into eax then mov ecx (original: straight into ecx).
void CSWSCreature::UpdateExcitedStateTimer()
{
    if (m_nExcitedTimer) {
        if (m_nExcitedTimer <= m_nTimeDelta) {
            m_nExcitedTimer = 0;
            CSWSPlayer* pPlayer = g_pAppManager->m_pServerExoApp->GetClientObjectByObjectId(m_idSelf);
            if (GetArea() && pPlayer && GetArea()->m_pSoundObject) {
                GetArea()->m_pSoundObject->Update(0);
            }
        }
        else {
            m_nExcitedTimer -= m_nTimeDelta;
        }
    }
}
// FUNCTION: SWKOTOR 0x004ED5E0
unsigned long CSWSCreature::GetDamageFlags()
{
    int nType = ((CSWSAttackData*)m_pCombatRound->GetAttack(m_pCombatRound->m_nCurrentAttack))->m_nWeaponAttackType;
    CSWSItem* pWeapon = m_pCombatRound->GetCurrentAttackWeapon(nType);
    if (pWeapon) {
        return pWeapon->GetDamageFlags();
    }
    return DAMAGE_FLAG_00;
}
// FUNCTION: SWKOTOR 0x004EDB20
int CSWSCreature::GetIsWeaponEffective(unsigned long oidTarget, unsigned long nFlags)
{
    CSWSCreature* pTarget = NULL;
    CGameObject* pObject = GetServerExoApp()->GetGameObject(oidTarget);
    if (pObject && pObject->AsSWSObject()) {
        pTarget = (CSWSCreature*)pObject->AsSWSObject();
        if (pTarget && ((CSWSObjectF8*)pTarget)->m_nF8 == 1) {
            return 0;
        }
    }
    int nBonus = m_pStats->GetSaveVsDamage(pTarget, nFlags, 0, 0, 1, 0);
    return GetWeaponDamageBonus(pTarget, nFlags) + nBonus > 0;
}

// FUNCTION: SWKOTOR 0x004EDDD0
int CSWSCreature::DecrementSpellReadyCount(unsigned long nSpell, unsigned char nMultiClass,
                                           unsigned char nMeta, int a, int bInstant)
{
    if (m_b960 == 0) {
        if (nMultiClass == 0xfe) {
            m_pStats->DecrementSpellUses(nSpell, 0, (unsigned char)bInstant);
        }
        else {
            return m_pStats->UnReadySpell(nSpell);
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004EF640
void CSWSCreature::DumpToLog()
{
    m_pStats->GetFullName().CStr();
    m_pStats->Dump6190();
    m_pStats->Dump6310();
    m_pStats->Dump63D0();
    m_pStats->Dump6550();
    m_pStats->Dump6250();
    m_pStats->Dump6490();
}

// FUNCTION: SWKOTOR 0x004EF6E0
int CSWSCreature::CalculateSpellSaveDC(int nSpell)
{
    char nF4 = m_pStats->m_nF4;
    char nF2 = m_pStats->m_nF2;
    int nDC = m_pStats->GetLevel(1) + nF2 + nF4 + 5;
    if (m_pStats->HasFeat(0x5a)) {
        nDC += 4;
    }
    else if (m_pStats->HasFeat(0x59)) {
        nDC += 2;
    }
    else if (m_pStats->HasFeat(0x58)) {
        nDC++;
    }
    return nDC;
}
// FUNCTION: SWKOTOR 0x004EF770
CItemRepository* CSWSCreature::GetItemRepository(int bParty)
{
    if (bParty) {
        if (m_bA88) {
            return GetServerExoApp()->GetPartyTable()->GetItemRepository();
        }
        if (((CSWSModule*)GetServerExoApp()->GetModule())->m_nFlags104 & 1) {
            unsigned long oid = m_idSelf;
            if (GetServerExoApp()->GetPartyTable()->GetIsNPCObject(oid)) {
                if (((CGuiInGame*)GetClientExoApp()->GetInGameGui(0))->GetIsInMenu()) {
                    return GetServerExoApp()->GetPartyTable()->GetItemRepository();
                }
            }
        }
    }
    return m_pItemRepository;
}
// FUNCTION: SWKOTOR 0x004EF890
// NONMATCH (5): vtable for GetCurrentHitPoints loaded into edx instead of eax.
int CSWSCreature::GetDeadTemp()
{
    int nMembers = GetClientExoApp()->GetSWParty()->m_nMembers;
    for (int i = 0; i < nMembers; i++) {
        CSWPartyMember* pMember = GetClientExoApp()->GetSWParty()->GetMember(i);
        unsigned long oid = GetClientExoApp()->ServerToClientObjectId(m_idSelf);
        if (pMember && pMember->m_oid == oid) {
            return GetCurrentHitPoints(0) <= 0;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004EF820
int CSWSCreature::GetIsPartyMemberOr4CB810()
{
    int nMembers = GetClientExoApp()->GetSWParty()->m_nMembers;
    for (int i = 0; i < nMembers; i++) {
        CSWPartyMember* pMember = GetClientExoApp()->GetSWParty()->GetMember(i);
        unsigned long oid = GetClientExoApp()->ServerToClientObjectId(m_idSelf);
        if (pMember && pMember->m_oid == oid) {
            return 0;
        }
    }
    return Fn4CB810();
}

// FUNCTION: SWKOTOR 0x004EF910
void CSWSCreature::OnEndDialog(int bArg)
{
    g_pVirtualMachine->RunScript(&m_sOnEndDialog, m_idSelf, 1);
}

// FUNCTION: SWKOTOR 0x004EFC30
int CSWSCreature::RemovePazaakCard(CSWSItem* pItem)
{
    int nIndex = pItem->m_nPazaakCard - 1;
    CSWPartyTable* pTable = GetServerExoApp()->GetPartyTable();
    int nHave = pTable->m_nPazaakCards[nIndex];
    int nTake = nHave < pItem->m_nStackSize ? nHave : pItem->m_nStackSize;
    pTable->m_nPazaakCards[nIndex] = nHave - nTake;
    return nTake;
}
// FUNCTION: SWKOTOR 0x004EFF30
int CSWSCreature::GetIsInjured()
{
    if (m_bAC4 == 0) {
        float fCurrent = (float)GetCurrentHitPoints(0);
        if (fCurrent / GetMaxHitPoints(1) < 0.2f) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004F0F90
int CSWSCreature::GetPauseReadyAnimation()
{
    if (GetClientObject()) {
        CSWCCreature* pClient = ((CGameObjectClient*)GetClientObject())->AsSWCCreature();
        if (pClient) {
            return pClient->GetPauseReadyAnimation();
        }
    }
    return (m_b4E0 != 0) + 10000;
}

// FUNCTION: SWKOTOR 0x004F7750
int CSWSCreature::GetCurrentlyFollowingLeader()
{
    CExoLinkedListInternal* pList = m_pActions;
    void* pos = pList->m_pHead;
    if (pos) {
        CSWSAction* pAction = (CSWSAction*)pList->GetAtPos(pos);
        if (pAction && pAction->m_nType == 0x3d) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004F7780
int CSWSCreature::GetTotalACEffectBonus()
{
    CExoArrayList<CGameEffect*>* pEffects = &m_appliedEffects;
    if (pEffects == NULL) {
        return 0;
    }
    int nBonus = 0;
    if (pEffects->num > 0) {
        for (int i = 0; i < pEffects->num; i++) {
            CGameEffect* pEffect = pEffects->element[i];
            if (pEffect->m_nType == EFFECT_TYPE_AC_INCREASE) {
                nBonus += pEffect->GetInteger(1);
            }
            else if (pEffect->m_nType == EFFECT_TYPE_AC_DECREASE) {
                nBonus -= pEffect->GetInteger(1);
            }
        }
    }
    return nBonus;
}

// FUNCTION: SWKOTOR 0x0050EE30
void CSWSCreature::SetDetectMode(unsigned char nMode)
{
    if (nMode != m_nDetectMode) {
        m_nDetectMode = nMode;
        UpdateAppearance();
    }
}

// FUNCTION: SWKOTOR 0x0050EE50
void CSWSCreature::SetStealthMode(unsigned char nMode, unsigned long nArg)
{
    if (nMode != m_nStealthMode) {
        m_nStealthMode = nMode;
        m_nStealthArg = nArg;
        UpdateAppearance();
    }
}

// FUNCTION: SWKOTOR 0x00513FC0
CSWSFaction* CSWSCreature::GetFaction()
{
    CFactionManager* p = GetServerExoApp()->m_pInternal->m_pFactionManager;
    return p->GetFaction(m_pStats->m_nFactionId);
}
// FUNCTION: SWKOTOR 0x005140D0
int CSWSCreature::FUN_005140d0(int nArg)
{
    return AI_ACTION_RESULT_005140D0;
}

// FUNCTION: SWKOTOR 0x005140E0
int CSWSCreature::AIActionRest(int nArg)
{
    return AI_ACTION_RESULT_005140E0;
}

// FUNCTION: SWKOTOR 0x004EDF80
void CSWSCreature::ResetFollowInfo()
{
    CSWSCreatureFollowInfo* pInfo = m_pFollowInfo;
    unsigned char n = pInfo->m_n100;
    pInfo->m_nF8 = 1;
    pInfo->m_n100 = n + 1;
    m_pFollowInfo->m_nFC = 0;
    m_pFollowInfo->m_n104 = m_pFollowInfo->m_p8C - m_pFollowInfo->m_p9C;
    m_pFollowInfo->m_vPosition = m_vPosition;
    m_pFollowInfo->m_vOrientation = m_vOrientation;
}

// FUNCTION: SWKOTOR 0x004EDED0
void CSWSCreature::AutoCloseGUIPanels(int bArg)
{
    CSWSPlayer* pPlayer = g_pAppManager->m_pServerExoApp->GetClientObjectByObjectId(m_idSelf);
    if (pPlayer && *pPlayer->m_pControlled != OBJECT_INVALID) {
        CGameObject* pObject;
        CGameObjectArray* pArray = g_pAppManager->m_pServerExoApp->GetObjectArray();
        unsigned long oid = *pPlayer->m_pControlled;
        if (pArray->GetGameObject(oid, &pObject) == GAME_OBJECT_VALID) {
            if (pObject->m_nObjectType == OBJECT_TYPE_ITEM) {
                CSWSItem* pItem = pObject->AsSWSItem();
                if (pItem) {
                    pItem->CloseInventory(m_idSelf, 1);
                }
            }
            else if (pObject->m_nObjectType == OBJECT_TYPE_PLACEABLE) {
                CSWSPlaceable* pPlaceable = pObject->AsSWSPlaceable();
                if (pPlaceable) {
                    pPlaceable->CloseInventory(m_idSelf, 1);
                }
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x004EF560
void CSWSCreature::DisplayFloatyDataToSelf(int nValue)
{
    CSWSPlayer* pPlayer = g_pAppManager->m_pServerExoApp->GetClientObjectByObjectId(m_idSelf);
    if (pPlayer) {
        CSWCCMessageData* pData = new CSWCCMessageData();
        if (pData) {
            pData->SetObjectID(0, m_idSelf);
            pData->SetInteger(9, 0x5d);
            pData->SetInteger(0, nValue);
            unsigned long nPlayerID = pPlayer->m_nPlayerID;
            g_pAppManager->m_pServerExoApp->GetSWSMessage()->SendServerToPlayerCCMessage(
                nPlayerID, 0xb, pData, NULL);
            delete pData;
        }
    }
}

// FUNCTION: SWKOTOR 0x004EFB40
void CSWSCreature::AddPazaakCard(CSWSItem* pItem)
{
    int nIndex = (pItem->m_nPazaakCard + 11) % 18;
    CSWPartyTable* pTable = GetServerExoApp()->GetPartyTable();
    pTable->AddPazaakCards(nIndex, pItem->m_nStackSize);
    CItemRepository* pRepository = GetItemRepository(1);
    if (pRepository->FindItemWithBaseItemType(0x2b, 0) == 0x7f000000) {
        CSWSItem* pNew = new CSWSItem(0x7f000000);
        CResRef cResRef("g_I_PazSidebd001");
        pNew->LoadFromTemplate(cResRef);
        pRepository->AddItem(&pNew, 1, 0, 1);
    }
}

// FUNCTION: SWKOTOR 0x004EDE10
// NONMATCH (15): original duplicates the SetInteger push into both arms of the pData == NULL test.
void CSWSCreature::SendFeedbackMessage(unsigned short nMessage, CSWCCMessageData* pData)
{
    CSWSPlayer* pPlayer = GetServerExoApp()->GetClientObjectByObjectId(m_idSelf);
    if (pPlayer) {
        if (pData == NULL) {
            pData = new CSWCCMessageData();
        }
        pData->SetInteger(9, nMessage);
        CSWSMessage* pMessage = GetServerExoApp()->GetSWSMessage();
        pMessage->SendServerToPlayerCCMessage(pPlayer->m_nPlayerID, 0xb, pData, NULL);
    }
    delete pData;
}

// FUNCTION: SWKOTOR 0x004EFC70
float CSWSCreature::GetSpotCheckDistance()
{
    float fDistance = m_fSpotDistance;
    if (m_b4E0 && GetServerExoApp()->GetCreatureByGameObjectID(GetServerExoApp()->GetPlayerCreatureID()) != this) {
        CExoString sColumn("PrimaryRange");
        g_pRules->Get2DArrays()->GetRanges()->GetFLOATEntry(0x12, sColumn, &fDistance);
    }
    return fDistance;
}
// FUNCTION: SWKOTOR 0x004EFD20
float CSWSCreature::GetListenCheckDistance()
{
    float fDistance = m_fListenDistance;
    if (m_b4E0 && GetServerExoApp()->GetCreatureByGameObjectID(GetServerExoApp()->GetPlayerCreatureID()) != this) {
        CExoString sColumn("SecondaryRange");
        g_pRules->Get2DArrays()->GetRanges()->GetFLOATEntry(0x12, sColumn, &fDistance);
    }
    return fDistance;
}
// FUNCTION: SWKOTOR 0x004EFE30
void CSWSCreature::InstantUseItem(unsigned long oidItem)
{
    CSWSItem* pItem = GetServerExoApp()->GetItemByGameObjectID(oidItem);
    for (int i = 0; i < pItem->m_nActiveProperties; i++) {
        CSWItemProperty* pProperty = pItem->GetActiveProperty(i);
        if (pProperty->m_nPropertyName == 10) {
            Vector vTarget(0.0f, 0.0f, 0.0f);
            UseItemProperty(pProperty->m_nSubType, &vTarget, m_idSelf, 0, oidItem, 0, 0, 0, 0);
            pItem->DecrementUses(i, m_idSelf);
            if (m_b4E0) {
                m_nAB0 = 3000;
            }
            return;
        }
    }
}

// FUNCTION: SWKOTOR 0x004EB620
int CSWSCreature::AddPickUpItemActions(unsigned long oidItem, unsigned long oidArea, unsigned long nArg)
{
    if (GetServerExoApp()->GetItemByGameObjectID(oidItem) && GetArea() && m_bE8) {
        AddAction(7, 0xffff, 3, &oidItem, 3, &oidArea, 1, &nArg);
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004FDB20
void CSWSCreature::SetInParty(int bInParty, int bUpdate)
{
    m_bA88 = bInParty;
    if (bUpdate) {
        if (bInParty) {
            if (m_pPartyFollowInfo == NULL) {
                m_pPartyFollowInfo = new CSWSCreaturePartyFollowInfo();
            }
            if (m_nAILevel != 4) {
                GetServerExoApp()->GetServerAIMaster()->SetAILevel((CSWSObject*)this, 4);
            }
        }
        else {
            if (m_pPartyFollowInfo) {
                delete m_pPartyFollowInfo;
                m_pPartyFollowInfo = NULL;
            }
            if (m_nAILevel != 2) {
                g_pAppManager->m_pServerExoApp->GetServerAIMaster()->SetAILevel((CSWSObject*)this, 2);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x004EE230
int CSWSSpellScriptData::SaveData(CResGFF* pGFF, CResStruct* pStruct)
{
    pGFF->WriteFieldINT(pStruct, m_nSpellId, "SpellId");
    pGFF->WriteFieldDWORD(pStruct, m_oidCaster, "CasterId");
    pGFF->WriteFieldDWORD(pStruct, m_oidTarget, "TargetId");
    pGFF->WriteFieldDWORD(pStruct, m_oidArea, "AreaId");
    pGFF->WriteFieldDWORD(pStruct, m_oidItem, "ItemId");
    pGFF->WriteFieldCExoString(pStruct, m_sScript, "Script");
    pGFF->WriteFieldFLOAT(pStruct, m_vTargetPosition.x, "TargetPosX");
    pGFF->WriteFieldFLOAT(pStruct, m_vTargetPosition.y, "TargetPosY");
    pGFF->WriteFieldFLOAT(pStruct, m_vTargetPosition.z, "TargetPosZ");
    return 1;
}

// FUNCTION: SWKOTOR 0x004EE2E0
int CSWSSpellScriptData::LoadData(CResGFF* pGFF, CResStruct* pStruct)
{
    int bSuccess;
    m_nSpellId = pGFF->ReadFieldINT(pStruct, "SpellId", bSuccess, 0);
    m_oidCaster = pGFF->ReadFieldDWORD(pStruct, "CasterId", bSuccess, 0);
    m_oidTarget = pGFF->ReadFieldDWORD(pStruct, "TargetId", bSuccess, 0);
    m_oidArea = pGFF->ReadFieldDWORD(pStruct, "AreaId", bSuccess, 0);
    m_oidItem = pGFF->ReadFieldDWORD(pStruct, "ItemId", bSuccess, 0);
    {
        CExoString sDefault("");
        m_sScript = pGFF->ReadFieldCExoString(pStruct, "Script", bSuccess, sDefault);
    }
    m_vTargetPosition.x = pGFF->ReadFieldFLOAT(pStruct, "TargetPosX", bSuccess, 0.0f);
    m_vTargetPosition.y = pGFF->ReadFieldFLOAT(pStruct, "TargetPosY", bSuccess, 0.0f);
    m_vTargetPosition.z = pGFF->ReadFieldFLOAT(pStruct, "TargetPosZ", bSuccess, 0.0f);
    return 1;
}

// FUNCTION: SWKOTOR 0x004ED620
void CSWSCreature::UpdateAppearanceForEquippedItems()
{
    CSWSItem* pItem = m_pInventory->GetItemInSlot(0x20);
    unsigned long oid0 = pItem ? pItem->m_n14 : OBJECT_INVALID;
    pItem = m_pInventory->GetItemInSlot(0x10);
    unsigned long oid1 = pItem ? pItem->m_n14 : OBJECT_INVALID;
    pItem = m_pInventory->GetItemInSlot(2);
    unsigned long oid2 = pItem ? pItem->m_n14 : OBJECT_INVALID;
    pItem = m_pInventory->GetItemInSlot(1);
    unsigned long oid3 = pItem ? pItem->m_n14 : OBJECT_INVALID;
    if (oid0 != m_aEquippedAppearance[0] || oid1 != m_aEquippedAppearance[1] ||
        oid2 != m_aEquippedAppearance[2] || oid3 != m_aEquippedAppearance[3]) {
        m_aEquippedAppearance[0] = oid0;
        m_aEquippedAppearance[1] = oid1;
        m_aEquippedAppearance[2] = oid2;
        m_aEquippedAppearance[3] = oid3;
    }
}

// FUNCTION: SWKOTOR 0x004EB020
// NONMATCH (29): each read result stays in a register until after the next call's pushes.
void CSWSCreaturePartyFollowInfo::Load(CResGFF* pGFF, CResStruct* pStruct)
{
    int bSuccess;
    m_oidFollowObject = pGFF->ReadFieldDWORD(pStruct, "FollowObject", bSuccess, m_oidFollowObject);
    m_vFollowLocation = pGFF->ReadFieldVector(pStruct, "FollowLocation", bSuccess, m_vFollowLocation);
    m_vLastLeaderPos = pGFF->ReadFieldVector(pStruct, "LastLeaderPos", bSuccess, m_vLastLeaderPos);
    m_vLastFollowerPos = pGFF->ReadFieldVector(pStruct, "LastFollowerPos", bSuccess, m_vLastFollowerPos);
    float fMaxSpeed = m_fMaxSpeed;
    m_fMaxSpeed = pGFF->ReadFieldFLOAT(pStruct, "MaxSpeed", bSuccess, fMaxSpeed);
    m_bStickToPos = pGFF->ReadFieldBYTE(pStruct, "StickToPos", bSuccess, m_bStickToPos);
    m_nResult = pGFF->ReadFieldDWORD(pStruct, "Result", bSuccess, m_nResult);
    m_nTimeElapsed = pGFF->ReadFieldDWORD(pStruct, "TimeElapsed", bSuccess, m_nTimeElapsed);
    m_bInSafetyRange = pGFF->ReadFieldBYTE(pStruct, "InSafetyRange", bSuccess, m_bInSafetyRange);
}

// FUNCTION: SWKOTOR 0x004EAF70
void CSWSCreaturePartyFollowInfo::Save(CResGFF* pGFF, CResStruct* pStruct)
{
    pGFF->WriteFieldDWORD(pStruct, m_oidFollowObject, "FollowObject");
    pGFF->WriteFieldVector(pStruct, m_vFollowLocation, "FollowLocation");
    pGFF->WriteFieldVector(pStruct, m_vLastLeaderPos, "LastLeaderPos");
    pGFF->WriteFieldVector(pStruct, m_vLastFollowerPos, "LastFollowerPos");
    pGFF->WriteFieldFLOAT(pStruct, m_fMaxSpeed, "MaxSpeed");
    pGFF->WriteFieldBYTE(pStruct, (unsigned char)m_bStickToPos, "StickToPos");
    pGFF->WriteFieldDWORD(pStruct, m_nResult, "Result");
    pGFF->WriteFieldDWORD(pStruct, m_nTimeElapsed, "TimeElapsed");
    pGFF->WriteFieldBYTE(pStruct, (unsigned char)m_bInSafetyRange, "InSafetyRange");
}

// FUNCTION: SWKOTOR 0x004EAE80
void CSWPartyMemberData::Reset()
{
    m_n34 = 0;
    m_n44 = 0;
    m_n48 = 0;
    m_n68 = 0;
    m_vC = Vector(0.0f, 0.0f, 0.0f);
    m_v18 = Vector(0.0f, 0.0f, 0.0f);
    m_v28 = Vector(0.0f, 0.0f, 0.0f);
    m_n4 = -1;
    m_v4C = Vector(0.0f, 1.0f, 0.0f);
    m_f58 = 1.0f;
    m_oid80 = 0x7f000000;
    m_n84 = 0xbc50;
}

// FUNCTION: SWKOTOR 0x004EF2E0
// NONMATCH (16): only the return tail differs (original: two epilogues, mov eax,3).
unsigned char CSWSCreature::GetBodyBagAppearance()
{
    int nAppearance;
    bool bFailed;
    {
        CExoString sColumn("Appearance");
        bFailed = !g_pRules->Get2DArrays()->GetBodyBag()->GetINTEntry(m_nBodyBag, sColumn, &nAppearance);
    }
    if (bFailed) {
        int nBodyBag;
        {
            CExoString sColumn("Body_Bag");
            bFailed = !g_pRules->Get2DArrays()->GetAppearance()->GetINTEntry(m_nAppearanceType, sColumn, &nBodyBag);
        }
        if (bFailed) {
            nBodyBag = 0;
        }
        {
            CExoString sColumn("Appearance");
            bFailed = !g_pRules->Get2DArrays()->GetBodyBag()->GetINTEntry(nBodyBag, sColumn, &nAppearance);
        }
        if (!bFailed) {
            return (unsigned char)nAppearance;
        }
        return 3;
    }
    return (unsigned char)nAppearance;
}

// Reset (0x004F0020) lives in sws_creature2.cpp: with its body in the same
// file cl keeps ECX live across the call instead of saving this in ESI.
// FUNCTION: SWKOTOR 0x004F79E0
CSWSCreaturePartyFollowInfo::CSWSCreaturePartyFollowInfo() throw()
    : m_vFollowLocation(0.0f, 0.0f, 0.0f),
      m_vLastLeaderPos(0.0f, 0.0f, 0.0f),
      m_vLastFollowerPos(0.0f, 0.0f, 0.0f)
{
    Reset();
}
