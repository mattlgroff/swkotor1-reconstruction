// CSWSCreature party broadcasts and small leaves (0x004EAD70-0x004ECE73;
// behavioural reference: src/batches/b004e5_enhancement_creature_ai.cpp).
#include <stdlib.h>
#include "a/sws_creature_broadcast.h"

// FUNCTION: SWKOTOR 0x004EAD70
void CSWSCreature::SetFlag288Bit3(int bValue)
{
    m_b288_3 = bValue;
}

// FUNCTION: SWKOTOR 0x004EADA0
int CSWSCreature::GetHasDamageFlag(unsigned char nFlag)
{
    if (nFlag == 0xff || (m_nDamageFlags & (1 << nFlag))) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004EB190
int CSWSCreature::GetHasFlags9FC(unsigned long nMask)
{
    return (m_n9FC & nMask) != 0;
}

// FUNCTION: SWKOTOR 0x004EC370
float CSWSCreature::GetMovementRateFactor()
{
    float fFactor = m_fMovementRateFactor;
    if (fFactor < 0.125f) {
        return 0.125f;
    }
    if (fFactor > 1.5f) {
        return 1.5f;
    }
    return fFactor;
}

// FUNCTION: SWKOTOR 0x004EC3B0
void CSWSCreature::SetMovementRateFactor(float fFactor)
{
    if (fFactor < 0.125f) {
        m_fMovementRateFactor = 0.125f;
    }
    else if (fFactor > 1.5f) {
        m_fMovementRateFactor = 1.5f;
    }
    else {
        m_fMovementRateFactor = fFactor;
    }
}

// FUNCTION: SWKOTOR 0x004EC400
void CSWSCreature::BroadcastDamageDataToParty(CSWCCMessageData* pData, int bHeal)
{
    unsigned long* pMembers;
    GetFaction()->GetPlayerList(&pMembers);
    for (int i = 0; i < GetFaction()->m_aMembers.num; i++) {
        CSWSPlayer* pPlayer = GetServerExoApp()->GetClientObjectByObjectId(pMembers[i]);
        if (pPlayer) {
            unsigned long nPlayerID;
            CGameObject* pObject = pPlayer->GetGameObject();
            if (pObject && m_oidArea == pObject->m_oidArea) {
                Vector v = pObject->m_vPosition - m_vPosition;
                if (v.x * v.x + v.y * v.y + v.z * v.z <= 900.0f) {
                    nPlayerID = pPlayer->m_nPlayerID;
                    if (bHeal == 0) {
                        GetServerExoApp()->GetSWSMessage()->SendServerToPlayerCCMessage(
                            nPlayerID, 3, pData, 0);
                    }
                    else {
                        GetServerExoApp()->GetSWSMessage()->SendServerToPlayerCCMessage(
                            nPlayerID, 2, pData, 0);
                    }
                }
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x004EC500
void CSWSCreature::BroadcastDeathDataToParty(CSWCCMessageData* pData)
{
    unsigned long* pMembers;
    if (GetFaction() == NULL) {
        return;
    }
    GetFaction()->GetPlayerList(&pMembers);
    for (int i = 0; i < GetFaction()->m_aMembers.num; i++) {
        CSWSPlayer* pPlayer = GetServerExoApp()->GetClientObjectByObjectId(pMembers[i]);
        if (pPlayer) {
            CGameObject* pObject = pPlayer->GetGameObject();
            if (pObject && m_oidArea == pObject->m_oidArea) {
                Vector v = pPlayer->GetGameObject()->m_vPosition - m_vPosition;
                if (v.x * v.x + v.y * v.y + v.z * v.z <= 900.0f) {
                    unsigned long nPlayerID = pPlayer->m_nPlayerID;
                    GetServerExoApp()->GetSWSMessage()->SendServerToPlayerCCMessage(
                        nPlayerID, 4, pData, 0);
                }
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x004EC610
void CSWSCreature::BroadcastData0B(CSWCCMessageData* pData)
{
    unsigned long* pMembers;
    GetFaction()->GetPlayerList(&pMembers);
    for (int i = 0; i < GetFaction()->m_aMembers.num; i++) {
        CSWSPlayer* pPlayer = GetServerExoApp()->GetClientObjectByObjectId(pMembers[i]);
        if (pPlayer) {
            CGameObject* pObject = pPlayer->GetGameObject();
            if (pObject && m_oidArea == pObject->m_oidArea) {
                Vector v = pPlayer->GetGameObject()->m_vPosition - m_vPosition;
                if (v.x * v.x + v.y * v.y + v.z * v.z <= 900.0f) {
                    unsigned long nPlayerID = pPlayer->m_nPlayerID;
                    GetServerExoApp()->GetSWSMessage()->SendServerToPlayerCCMessage(
                        nPlayerID, 0xb, pData, 0);
                }
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x004EC700
void CSWSCreature::BroadcastSkillData(CSWCCMessageData* pData)
{
    unsigned long oidSelf = m_idSelf;
    CSWSPlayer* pPlayer = GetServerExoApp()->GetClientObjectByObjectId(oidSelf);
    if (pPlayer) {
        unsigned long nPlayerID = pPlayer->m_nPlayerID;
        GetServerExoApp()->GetSWSMessage()->SendServerToPlayerCCMessage(
            nPlayerID, 9, pData, 0);
    }
    if (pData) {
        delete pData;
    }
}

// FUNCTION: SWKOTOR 0x004EC760
void CSWSCreature::BroadcastSavingThrowData(CSWCCMessageData* pData)
{
    unsigned long oidSelf = m_idSelf;
    CSWSPlayer* pPlayer = GetServerExoApp()->GetClientObjectByObjectId(oidSelf);
    if (pPlayer) {
        GetServerExoApp()->GetSWSMessage()->SendServerToPlayerCCMessage(
            pPlayer->m_nPlayerID, 7, pData, 0);
    }
}

// FUNCTION: SWKOTOR 0x004EC7A0
void CSWSCreature::BroadcastStealthDetectionData(CSWCCMessageData* pData)
{
    unsigned long* pMembers;
    GetFaction()->GetPlayerList(&pMembers);
    for (int i = 0; i < GetFaction()->m_aMembers.num; i++) {
        CSWSPlayer* pPlayer = GetServerExoApp()->GetClientObjectByObjectId(pMembers[i]);
        if (pPlayer) {
            CGameObject* pObject = pPlayer->GetGameObject();
            if (pObject && m_oidArea == pObject->m_oidArea) {
                unsigned long nPlayerID = pPlayer->m_nPlayerID;
                GetServerExoApp()->GetSWSMessage()->SendServerToPlayerCCMessage(
                    nPlayerID, 0x10, pData, 0);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x004EC840
void CSWSCreature::BroadcastMineDetectionData(CSWCCMessageData* pData)
{
    unsigned long* pMembers;
    GetFaction()->GetPlayerList(&pMembers);
    for (int i = 0; i < GetFaction()->m_aMembers.num; i++) {
        CSWSPlayer* pPlayer = GetServerExoApp()->GetClientObjectByObjectId(pMembers[i]);
        if (pPlayer) {
            CGameObject* pObject = pPlayer->GetGameObject();
            if (pObject && m_oidArea == pObject->m_oidArea) {
                unsigned long nPlayerID = pPlayer->m_nPlayerID;
                GetServerExoApp()->GetSWSMessage()->SendServerToPlayerCCMessage(
                    nPlayerID, 0x13, pData, 0);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x004EC8E0
void CSWSCreature::BroadcastBattleCry()
{
    if (GetArea() == NULL) {
        return;
    }
    unsigned long oidPC = GetServerExoApp()->GetPlayerCharacterID();
    CSWSPlayer* pPlayer = GetServerExoApp()->GetClientObjectByObjectId(oidPC);
    if (pPlayer) {
        CGameObject* pObject = pPlayer->GetGameObject();
        if (pObject && m_oidArea == pObject->m_oidArea) {
            Vector v = pObject->m_vPosition - m_vPosition;
            if (v.x * v.x + v.y * v.y + v.z * v.z <= 900.0f) {
                int nRandom = rand() % 5;
                int nBattleCry;
                if (nRandom == 0) {
                    nBattleCry = 1;
                }
                else if (nRandom == 1) {
                    nBattleCry = 2;
                }
                else if (nRandom == 2) {
                    nBattleCry = 3;
                }
                else if (nRandom == 3) {
                    nBattleCry = 4;
                }
                else {
                    nBattleCry = 5;
                }
                unsigned long oidSelf = m_idSelf;
                GetServerExoApp()->GetSWSMessage()->SendServerToPlayerBattleCry(
                    pPlayer, oidSelf, nBattleCry);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x004EC9E0
void CSWSCreature::BroadcastAttackSummaryToParty(CSWCCMessageData* pData)
{
    CSWSFaction* pFaction = GetFaction();
    if (pFaction == NULL) {
        return;
    }
    unsigned long* pMembers = NULL;
    int nMembers = pFaction->GetPlayerList(&pMembers);
    for (int i = 0; i < nMembers; i++) {
        CSWSPlayer* pPlayer = GetServerExoApp()->GetClientObjectByObjectId(pMembers[i]);
        if (pPlayer) {
            CGameObject* pObject = pPlayer->GetGameObject();
            if (pObject && m_oidArea == pObject->m_oidArea) {
                Vector v = pObject->m_vPosition - m_vPosition;
                if (v.x * v.x + v.y * v.y + v.z * v.z <= 900.0f) {
                    unsigned long nPlayerID = pPlayer->m_nPlayerID;
                    GetServerExoApp()->GetSWSMessage()->SendServerToPlayerCCMessage(
                        nPlayerID, 0x12, pData, 0);
                }
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x004ECAE0
void CSWSCreature::BroadcastBreakdownMessage(int nType, CSWCCMessageData* pData)
{
    CSWSFaction* pFaction = GetFaction();
    if (pFaction == NULL) {
        return;
    }
    unsigned long* pMembers = NULL;
    int nMembers = pFaction->GetPlayerList(&pMembers);
    for (int i = 0; i < nMembers; i++) {
        CSWSPlayer* pPlayer = GetServerExoApp()->GetClientObjectByObjectId(pMembers[i]);
        if (pPlayer) {
            CGameObject* pObject = pPlayer->GetGameObject();
            if (pObject && m_oidArea == pObject->m_oidArea) {
                Vector v = pObject->m_vPosition - m_vPosition;
                if (v.x * v.x + v.y * v.y + v.z * v.z <= 900.0f) {
                    unsigned long nPlayerID = pPlayer->m_nPlayerID;
                    GetServerExoApp()->GetSWSMessage()->SendServerToPlayerCCMessage(
                        nPlayerID, nType, pData, 0);
                }
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x004ECBE0
void CSWSCreature::BroadcastAttackBreakdown(unsigned long oidTarget, CSWCCMessageData* pData)
{
    BroadcastBreakdownMessage(0x14, pData);
    CSWSCreature* pTarget = GetServerExoApp()->GetCreatureByGameObjectID(oidTarget);
    if (pTarget) {
        pTarget->BroadcastBreakdownMessage(0x14, pData);
    }
}

// FUNCTION: SWKOTOR 0x004ECC20
void CSWSCreature::BroadcastThreatBreakdown(unsigned long oidTarget, CSWCCMessageData* pData)
{
    BroadcastBreakdownMessage(0x15, pData);
    CSWSCreature* pTarget = GetServerExoApp()->GetCreatureByGameObjectID(oidTarget);
    if (pTarget) {
        pTarget->BroadcastBreakdownMessage(0x15, pData);
    }
}

// FUNCTION: SWKOTOR 0x004ECC60
void CSWSCreature::BroadcastDefenseBreakdown(unsigned long oidTarget, CSWCCMessageData* pData)
{
    BroadcastBreakdownMessage(0x16, pData);
    CSWSCreature* pTarget = GetServerExoApp()->GetCreatureByGameObjectID(oidTarget);
    if (pTarget) {
        pTarget->BroadcastBreakdownMessage(0x16, pData);
    }
}

// FUNCTION: SWKOTOR 0x004ECCA0
void CSWSCreature::BroadcastDamageBreakdown(unsigned long oidTarget, CSWCCMessageData* pData)
{
    BroadcastBreakdownMessage(0x17, pData);
    CSWSCreature* pTarget = GetServerExoApp()->GetCreatureByGameObjectID(oidTarget);
    if (pTarget) {
        pTarget->BroadcastBreakdownMessage(0x17, pData);
    }
}

// FUNCTION: SWKOTOR 0x004ECCE0
void CSWSCreature::BroadcastEffectApplicationBreakdown(unsigned long oidTarget,
                                                       CSWCCMessageData* pData)
{
    BroadcastBreakdownMessage(0x19, pData);
    CSWSCreature* pTarget = GetServerExoApp()->GetCreatureByGameObjectID(oidTarget);
    if (pTarget) {
        pTarget->BroadcastBreakdownMessage(0x19, pData);
    }
}

// FUNCTION: SWKOTOR 0x004ECD20
void CSWSCreature::BroadcastDeflectBreakdown(unsigned long oidTarget, CSWCCMessageData* pData)
{
    BroadcastBreakdownMessage(0x1a, pData);
    CSWSCreature* pTarget = GetServerExoApp()->GetCreatureByGameObjectID(oidTarget);
    if (pTarget) {
        pTarget->BroadcastBreakdownMessage(0x1a, pData);
    }
}

// FUNCTION: SWKOTOR 0x004ECD60
void CSWSCreature::BroadcastPoisoned(CSWCCMessageData* pData)
{
    CSWSFaction* pFaction = GetFaction();
    if (pFaction == NULL) {
        return;
    }
    unsigned long* pMembers = NULL;
    int nMembers = pFaction->GetPlayerList(&pMembers);
    for (int i = 0; i < nMembers; i++) {
        CSWSPlayer* pPlayer = GetServerExoApp()->GetClientObjectByObjectId(pMembers[i]);
        if (pPlayer) {
            CGameObject* pObject = pPlayer->GetGameObject();
            if (pObject && m_oidArea == pObject->m_oidArea) {
                Vector v = pObject->m_vPosition - m_vPosition;
                if (v.x * v.x + v.y * v.y + v.z * v.z <= 900.0f) {
                    unsigned long nPlayerID = pPlayer->m_nPlayerID;
                    GetServerExoApp()->GetSWSMessage()->SendServerToPlayerCCMessage(
                        nPlayerID, 0x1b, pData, 0);
                }
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x004ECE60
void CSWSCreature::UpdateFlag4DC()
{
    m_b4DC = (m_pA88 == NULL);
}

// FUNCTION: SWKOTOR 0x004EB160
void CSWSCreature::SetId(unsigned long nId)
{
    unsigned long nOldId = m_idSelf;
    UpdateObjectId(nId, 1, 1, 0, 0);
    m_idSelf = nId;
    OnIdChanged(nOldId);
}

// FUNCTION: SWKOTOR 0x004EBB30
int CSWSCreature::RangeWeaponEquipped()
{
    CSWSItem* pItem = m_pInventory->GetItemInSlot(0x10);
    if (pItem) {
        int nBaseItem = pItem->m_nBaseItem;
        return g_pRules->m_pBaseItemArray->GetBaseItem(nBaseItem)->GetIsRangedWeapon() > 0;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004ED190
int CSWSCreature::GetRelativeWeaponSize(CSWSItem* pItem)
{
    int nBaseItem = pItem->m_nBaseItem;
    int nCreatureSize = m_nCreatureSize;
    int nSize = g_pRules->m_pBaseItemArray->GetBaseItem(nBaseItem)->m_nWeaponSize - nCreatureSize;
    if (nSize < -2 || nSize > 1) {
        return -10;
    }
    return nSize;
}

// FUNCTION: SWKOTOR 0x004EDAE0
unsigned char CSWSCreature::GetGenderForDialog()
{
    if (m_nAppearanceType == 10000 && m_nGender != 1) {
        return 2;
    }
    return m_nGender;
}

// FUNCTION: SWKOTOR 0x004EDB00
CSWSJournal* CSWSCreature::GetJournal()
{
    return GetServerExoApp()->GetPartyTable()->GetJournal();
}

// FUNCTION: SWKOTOR 0x004EDD60
int CSWSCreature::GetGold()
{
    if (m_pA88) {
        return GetServerExoApp()->GetPartyTable()->m_nGold;
    }
    return m_nGold;
}

// FUNCTION: SWKOTOR 0x004EDD90
void CSWSCreature::SetGold(int nGold)
{
    if (nGold > 999999999) {
        nGold = 999999999;
    }
    if (m_pA88) {
        GetServerExoApp()->GetPartyTable()->m_nGold = nGold;
    }
    else {
        m_nGold = nGold;
    }
}

// FUNCTION: SWKOTOR 0x004EE210
int CSWSCreature::GetFlags8ECState()
{
    if ((m_n8EC & 0x10) || ((m_n8EC & 8) && !(m_n8EC & 2))) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004EF450
void CSWSCreature::RemoveSpellActionFromRound()
{
    if (m_pCombatRound->GetSpellActionQueued() == 1) {
        m_pCombatRound->RemoveSpellAction();
    }
}

// FUNCTION: SWKOTOR 0x004EF480
void CSWSCreature::StartGuiTimingBar(int nDuration, int nEvent)
{
    if (m_bPlayerCharacter == 1) {
        unsigned long oidSelf = m_idSelf;
        CSWSPlayer* pPlayer = GetServerExoApp()->GetClientObjectByObjectId(oidSelf);
        CSWSMessage* pMessage = GetServerExoApp()->GetSWSMessage();
        if (pPlayer && pMessage) {
            pMessage->SendServerToPlayerGuiTimingEvent(pPlayer, 1, nEvent, nDuration);
        }
    }
}

// FUNCTION: SWKOTOR 0x004EF4D0
void CSWSCreature::StopGuiTimingBar()
{
    if (m_bPlayerCharacter == 1) {
        unsigned long oidSelf = m_idSelf;
        CSWSPlayer* pPlayer = GetServerExoApp()->GetClientObjectByObjectId(oidSelf);
        CSWSMessage* pMessage = GetServerExoApp()->GetSWSMessage();
        if (pPlayer && pMessage) {
            pMessage->SendServerToPlayerGuiTimingEvent(pPlayer, 0, 0, 0);
        }
    }
}

// FUNCTION: SWKOTOR 0x004EF520
void CSWSCreature::SetInvitedToParty(int bInvited)
{
    m_bInvitedToParty = bInvited;
    GetServerExoApp()->GetWorldTimer()->GetWorldTime(&m_nInvitedDay, &m_nInvitedTime);
}

// FUNCTION: SWKOTOR 0x004EFA80
unsigned long CSWSCreature::GetForceItemMask()
{
    unsigned long nMask = 0;
    for (int i = 0; i < 0x12; i++) {
        CSWSItem* pItem = m_pInventory->GetItemInSlot(i);
        if (pItem) {
            switch (pItem->GetBaseItem()->m_nForceItemType) {
            case 0x1f:
                nMask |= 1;
                break;
            case 0x20:
                nMask |= 2;
                break;
            case 0x21:
                nMask |= 4;
                break;
            case 0x22:
                nMask |= 8;
                break;
            case 0x23:
                nMask |= 0x10;
                break;
            case 0x24:
                nMask |= 0x20;
                break;
            case 0x27:
            case 0x28:
            case 0x29:
                nMask |= 0x40;
                break;
            }
        }
    }
    return nMask;
}

// FUNCTION: SWKOTOR 0x004EFB20
CSWCCreature* CSWSCreature::GetClientCreature()
{
    if (GetClientObject()) {
        return GetClientObject()->AsSWCCreature();
    }
    return NULL;
}

// FUNCTION: SWKOTOR 0x004EFDD0
unsigned long CSWSCreature::GetAAxValue(int nType)
{
    switch (nType) {
    case 0x10:
        return m_nAAC;
    case 0x20:
        return m_nAA8;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004EFE00
void CSWSCreature::SetAAxValue(int nType, unsigned long nValue)
{
    switch (nType) {
    case 0x10:
        m_nAAC = nValue;
        break;
    case 0x20:
        m_nAA8 = nValue;
        break;
    }
}

// FUNCTION: SWKOTOR 0x004EFEF0
void CSWSCreature::SetByte4C4(unsigned char nValue)
{
    if ((nValue == 4 || nValue == 3) &&
        (m_nAppearanceType == 0x279b || m_nAppearanceType == 0x2785 || m_nAppearanceType == 0x28a0)) {
        nValue = 0xe;
    }
    m_n4C4 = nValue;
}
