// Server creature AI action and perception-list family. Ordinary C++98
// layouts extend only padding in the existing partial creature declaration.
#include "a/sws_creature.h"

extern const int AI_ACTION_IN_PROGRESS;              // 0x00745DBC

// FUNCTION: SWKOTOR 0x00513F60
int CSWSCreature::AIActionWaitForEndOfRound(CSWSObjectActionNode*)
{
    if (GetDead() == 1 || GetIsPossessed() == 1 || GetDeadTemp() == 1)
        return AI_ACTION_RESULT_005140D0;
    if (m_pCombatRound->m_bRoundStarted == 1 && !m_pCombatRound->GetSpellActionPending())
        return AI_ACTION_IN_PROGRESS;
    return AI_ACTION_RESULT_005140E0;
}

// FUNCTION: SWKOTOR 0x00511130
int CSWSCreature::AIActionPartyFollowLeader(CSWSObjectActionNode*)
{
    if (!m_bA88 || !m_pPartyFollowInfo || GetDead() == 1 ||
        GetIsPossessed() == 1 || GetDeadTemp() == 1 || !m_pFollowInfo ||
        !(m_nPartyFollowFlags & 2))
        return AI_ACTION_RESULT_005140D0;
    if (GetArea()) {
        unsigned long oidSelf = m_idSelf;
        if (GetServerExoApp()->GetPlayerCreatureID() == oidSelf)
            return AI_ACTION_RESULT_005140D0;
        m_nAIState = 3;
    }
    return AI_ACTION_IN_PROGRESS;
}

// FUNCTION: SWKOTOR 0x005188E0
int CSWSCreature::HasAction(unsigned long nType)
{
    if (m_pActions->m_pHead) {
        void* pos = m_pActions->m_pHead;
        while (pos) {
            CSWSObjectActionNode* pNode = (CSWSObjectActionNode*)m_pActions->GetAtPos(pos);
            if (pNode->m_nActionId == nType) return 1;
            m_pActions->GetNext(pos);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00518940
CSWSObjectActionNode* CSWSCreature::GetAction(unsigned long nType)
{
    if (m_pActions->m_pHead) {
        void* pos = m_pActions->m_pHead;
        while (pos) {
            CSWSObjectActionNode* pNode = (CSWSObjectActionNode*)m_pActions->GetAtPos(pos);
            if (pNode->m_nActionId == nType) return pNode;
            m_pActions->GetNext(pos);
        }
    }
    return 0;
}

// NONMATCH: MSVC elides the observed post-dereference null test (4 instructions).
// FUNCTION: SWKOTOR 0x005178F0
void CSWSCreature::ClearVisibleList()
{
    for (int i = m_l920.num - 1; i >= 0; i--) {
        CSWSCreatureListEntry* pEntry = m_l920.element[i];
        if (pEntry->m_oid != m_idSelf) {
            m_l920.DelIndex(i);
            delete pEntry;
        }
    }
}

// NONMATCH: same post-dereference null-test elimination (2 instructions).
// FUNCTION: SWKOTOR 0x00517960
void CSWSCreature::OnIdChanged(unsigned long oid)
{
    if (m_l920.num > 0) {
        for (int i = m_l920.num - 1; i >= 0; i--) {
            CSWSCreatureListEntry* pEntry = m_l920.element[i];
            if (pEntry->m_oid == oid) {
                m_l920.DelIndex(i);
                delete pEntry;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x00510FD0
int CSWSCreature::AIActionEquipItem(CSWSObjectActionNode* pNode)
{
    ActivityManager(2);
    if (pNode->m_nParameterCount < 3) return AI_ACTION_RESULT_005140D0;
    unsigned long oidItem = pNode->m_n38;
    unsigned long nSlot = pNode->m_n3C;
    int nValue = pNode->m_n40;
    if (!GetDead() && !GetIsPossessed() && !GetDeadTemp() && RunEquip(oidItem, nSlot, nValue) == 1)
        return AI_ACTION_RESULT_005140E0;
    unsigned long oidSelf = m_idSelf;
    CSWSPlayer* pPlayer = GetServerExoApp()->GetClientObjectByObjectId(oidSelf);
    CSWSMessage* pMessage = GetServerExoApp()->GetSWSMessage();
    if (pMessage && pPlayer)
        pMessage->SendServerToPlayerInventory_EquipCancel(pPlayer->m_nPlayerID, oidItem, nSlot);
    return AI_ACTION_RESULT_005140D0;
}

// FUNCTION: SWKOTOR 0x00513EC0
int CSWSCreature::AIActionUnequipItem(CSWSObjectActionNode* pNode)
{
    ActivityManager(2);
    unsigned long oidItem = pNode->m_n38;
    unsigned long oidRepository = pNode->m_n3C;
    int nValue = pNode->m_n40;
    if (!GetDead() && !GetIsPossessed() && !GetDeadTemp() &&
        RunUnequip(oidItem, oidRepository, 1, 0, nValue) == 1)
        return AI_ACTION_RESULT_005140E0;
    unsigned long oidSelf = m_idSelf;
    CSWSPlayer* pPlayer = GetServerExoApp()->GetClientObjectByObjectId(oidSelf);
    CSWSMessage* pMessage = GetServerExoApp()->GetSWSMessage();
    if (pPlayer && pMessage)
        pMessage->SendServerToPlayerInventory_UnequipCancel(pPlayer->m_nPlayerID, oidItem);
    return AI_ACTION_RESULT_005140D0;
}

// NONMATCH: 5 diff: EAX/EDX app-manager register tie after the player query.
// FUNCTION: SWKOTOR 0x005137C0
int CSWSCreature::AIActionOrientCamera(CSWSObjectActionNode* pNode)
{
    // Parameters are stored as dwords in action nodes; a float view gives the
    // exact by-value camera ABI without changing the action record layout.
    union CameraValue { unsigned long n; float f; } x, y, z;
    unsigned long oidSelf = m_idSelf;
    unsigned long nFlags = pNode->m_n38;
    x.n = pNode->m_n3C;
    y.n = pNode->m_n40;
    z.n = pNode->m_n44;
    unsigned long nMode = pNode->m_n48;
    CSWSPlayer* pPlayer = GetServerExoApp()->GetClientObjectByObjectId(oidSelf);
    if (pPlayer) {
        CSWSMessage* pMessage = GetServerExoApp()->GetSWSMessage();
        if (pMessage) pMessage->SendServerToPlayerCamera_ChangeLocation(pPlayer, nFlags, x.f, y.f, z.f, nMode);
    }
    return AI_ACTION_RESULT_005140E0;
}

// FUNCTION: SWKOTOR 0x005140F0
int CSWSCreature::AIActionAppear(CSWSObjectActionNode*)
{
    unsigned long nDay, nTime, nDeltaDay, nDeltaTime;
    GetServerExoApp()->GetWorldTimer()->GetWorldTime(&nDay, &nTime);
    GetServerExoApp()->GetWorldTimer()->SubtractWorldTimes(nDay, nTime,
        m_nActionStartDay, m_nActionStartTime, &nDeltaDay, &nDeltaTime);
    if (nDeltaTime < 2000) {
        m_fAnimationSpeed = 1.0f;
        SetAnimation(0x274f);
        return AI_ACTION_IN_PROGRESS;
    }
    if (AsSWSCreature()) {
        m_fAnimationSpeed = 1.0f;
        SetAnimation(0x2711);
    }
    return AI_ACTION_RESULT_005140E0;
}

// FUNCTION: SWKOTOR 0x005141B0
int CSWSCreature::AIActionDisappear(CSWSObjectActionNode*)
{
    unsigned long nDay, nTime, nDeltaDay, nDeltaTime;
    GetServerExoApp()->GetWorldTimer()->GetWorldTime(&nDay, &nTime);
    GetServerExoApp()->GetWorldTimer()->SubtractWorldTimes(nDay, nTime,
        m_nActionStartDay, m_nActionStartTime, &nDeltaDay, &nDeltaTime);
    if (nDeltaTime < 2000) {
        m_fAnimationSpeed = 1.0f;
        SetAnimation(0x274e);
        return AI_ACTION_IN_PROGRESS;
    }
    if (AsSWSCreature()) {
        m_fAnimationSpeed = 1.0f;
        SetAnimation(0x2711);
    }
    return AI_ACTION_RESULT_005140E0;
}

// FUNCTION: SWKOTOR 0x00510C20
int CSWSCreature::AIActionEncounterCreatureDestroySelf()
{
    if (GetDead() == 1 || GetIsPossessed() == 1 || GetDeadTemp() == 1)
        return AI_ACTION_RESULT_005140D0;
    CSWSEncounter* pEncounter = GetServerExoApp()->GetEncounterByGameObjectID(m_oidEncounter);
    if (pEncounter) {
        pEncounter->RemoveFromActiveCreatureCount(m_pStats->GetChallengeRating(), 1);
        m_bEncounterDestroyed = 1;
    }
    CServerExoApp* pServer = GetServerExoApp();
    unsigned long nEvent = EVENT_AI_LATER;
    unsigned long oidSelf = m_idSelf;
    pServer->GetServerAIMaster()->AddEventDeltaTime(0, 5000, oidSelf, oidSelf, nEvent, 0);
    m_bE8 = 0;
    return AI_ACTION_RESULT_005140E0;
}

// FUNCTION: SWKOTOR 0x00515860
unsigned char CSWSCreature::CheckWeaponSlot(CSWSItem* pItem, unsigned long* pSlot, int bFeedback, int bUpdate)
{
    if (!CheckProficiencies(pItem, *pSlot)) {
        if (bFeedback == 1 && bUpdate == 1) SendFeedbackMessage(0x77, 0);
        return 0;
    }
    if (m_pInventory->GetItemInSlot(*pSlot)) {
        if (bFeedback == 1 && bUpdate == 1) SendFeedbackMessage(0x7b, 0);
        return 2;
    }
    return 1;
}

extern const float g_fFacingEpsilon;                 // 0x00747504

// NONMATCH: 4 diff: the Y-input spill and X fsub are scheduled in opposite order.
// FUNCTION: SWKOTOR 0x0050FC90
int CSWSCreature::AIActionChangeFacingPoint(CSWSObjectActionNode* pNode)
{
    if (GetDead() == 1 || GetIsPossessed() == 1 || GetDeadTemp() == 1)
        return AI_ACTION_RESULT_005140D0;
    union PositionValue { unsigned long n; float f; } x, y;
    x.n = pNode->m_n38;
    y.n = pNode->m_n3C;
    Vector vDirection;
    vDirection.x = x.f - m_vPosition.x;
    vDirection.y = y.f - m_vPosition.y;
    vDirection.z = 0.0f;
    if (vDirection.x * vDirection.x + vDirection.y * vDirection.y >= g_fFacingEpsilon)
        SetOrientation4CD170(NormalizeVector(vDirection));
    return AI_ACTION_RESULT_005140E0;
}

// Partial CSWSObject layout used by the object's AsSWSObject virtual return.
class CSWSObject {
public:
    CSWSArea* GetArea();                            // 0x004CB120
    char pad_0[0x90];
    Vector m_vPosition;
};

// NONMATCH: 8 diff: vector temporary stack reuse/copy scheduling differs.
// FUNCTION: SWKOTOR 0x0050FB60
int CSWSCreature::AIActionChangeFacingObject(CSWSObjectActionNode* pNode)
{
    if (GetDead() == 1 || GetIsPossessed() == 1 || GetDeadTemp() == 1)
        return AI_ACTION_RESULT_005140D0;
    CGameObject* pObject;
    unsigned long oid = pNode->m_n38;
    if (GetServerExoApp()->GetObjectArray()->GetGameObject(oid, &pObject) != GAME_OBJECT_VALID ||
        pObject->m_nObjectType <= OBJECT_TYPE_0746633)
        return AI_ACTION_RESULT_005140D0;
    CSWSArea* pArea = ((CSWSObject*)pObject)->GetArea();
    if (pArea != GetArea()) return AI_ACTION_RESULT_005140D0;
    Vector vTarget = ((CSWSObject*)pObject->AsSWSObject())->m_vPosition;
    Vector vDirection = VectorSubtract(vTarget, m_vPosition);
    vDirection.z = 0.0f;
    if (vDirection.x * vDirection.x + vDirection.y * vDirection.y >= g_fFacingEpsilon)
        SetOrientation4CD170(NormalizeVector(vDirection));
    return AI_ACTION_RESULT_005140E0;
}
