// Worker A round s10: CSWSCreature vtable slot 0x74 (0x007470C8 + 0x74). functions.csv lists the
// start as 0x004FAB0D; the function really starts at 0x004FAB00. The name is descriptive.
#include "a/sws_creature.h"

// Batch-local views: the placeable shares the CSWSObject animation slot and fields that
// a/sws_creature.h only declares on the creature; the client party keeps three 0x88-byte member
// records at +0x24.
class CSWSPlaceableAnimView : public CGameObject {
public:
    virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78();
    virtual void SetAnimation(int nAnimation);      // 0x7c

    char pad_0x18[0xd4 - 0x18];
    int m_nAnimation;                               // 0xd4
    float m_fAnimationSpeed;                        // 0xd8
    char pad_0xdc[0x450 - 0xdc];
    int m_n450;                                     // 0x450 (placeholder name)
};

struct CSWPartyMemberView {
    char pad_0x0[0x24];
    CSWPartyMemberData m_aMembers[3];               // 0x24
    CSWPartyMemberData* GetMemberData(int nIndex) { return &m_aMembers[nIndex]; }
};

// CServerExoApp::GetClientObjectByObjectId (0x004AE910) expanded inline here.
static inline CSWSPlayer* GetClientObjectByObjectId(CServerExoApp* pServer, unsigned long oid)
{
    CServerExoAppInternal* pInternal = pServer->m_pInternal;
    return pInternal->GetClientObjectByObjectID(oid);
}

// Global-accessor wrappers: the original evaluates the argument before the app-manager chain,
// which is the shape of an inlined helper taking the id by value.
static inline unsigned long ServerToClientObjectId(unsigned long oid)
{
    return GetClientExoApp()->ServerToClientObjectId(oid);
}

static inline int GetPartyIndex(unsigned long oid)
{
    return GetClientExoApp()->GetSWParty()->IsPartyMember(oid);
}

static inline CSWSPlaceable* GetPlaceableByGameObjectID(unsigned long oid)
{
    return GetServerExoApp()->GetPlaceableByGameObjectID(oid);
}

// NONMATCH (34): (1) the four inventory-cancel sends load the player id in another order/register
// (original: Equip and Unequip push the node parameters before loading it); (2) cl cross-jumps the
// action 0xf tail into action 0xc here, because 0xc loads the vtable after the two target stores
// (original: before them, in edx, so the tails differ). Everything else is identical.
// FUNCTION: SWKOTOR 0x004FAB00
int CSWSCreature::ClearAction(CSWSObjectActionNode* pNode, int bArg)
{
    if (!pNode->m_n70) {
        return 0;
    }
    CSWSMessage* pMessage = GetServerExoApp()->GetSWSMessage();
    CSWSPlayer* pPlayer = GetClientObjectByObjectId(GetServerExoApp(), m_idSelf);
    if (pPlayer && pMessage) {
        if (pNode->m_nActionId == 8) {
            pMessage->SendServerToPlayerInventory_EquipCancel(pPlayer->m_nPlayerID, pNode->m_n38,
                                                              pNode->m_n3C);
        } else if (pNode->m_nActionId == 7) {
            pMessage->SendServerToPlayerInventory_PickupCancel(pPlayer->m_nPlayerID, pNode->m_n38);
        } else if (pNode->m_nActionId == 9) {
            pMessage->SendServerToPlayerInventory_DropCancel(pPlayer->m_nPlayerID, pNode->m_n38);
        } else if (pNode->m_nActionId == 0xb) {
            pMessage->SendServerToPlayerInventory_UnequipCancel(pPlayer->m_nPlayerID, pNode->m_n38);
        }
    }
    unsigned long nAction = pNode->m_nActionId;
    if (nAction == 0x2a) {
        Fn4F3690(0x14);
    } else if (nAction == 0xc) {
        m_oidAttackTarget = m_oidAttemptedTarget = 0x7f000000;
        SetAnimation(10000);
        ProcessPendingCombatActions();
    } else if (nAction == 1) {
        m_pFollowInfo->m_oid30 = 0x7f000000;
        m_pFollowInfo->m_oid38 = 0x7f000000;
        m_nAIState = 1;
    } else if (nAction == 0xf) {
        m_oid524 = 0x7f000000;
        m_oid528 = 0x7f000000;
        m_pCombatRound->RemoveSpellAction();
        m_b960 = 0;
        SetAnimation(10000);
        ProcessPendingCombatActions();
    } else if (nAction == 0x2e) {
        m_oid524 = 0x7f000000;
        m_oid528 = 0x7f000000;
        m_pCombatRound->RemoveSpellAction();
        SetAnimation(10000);
        m_n96C = 0;
        ProcessPendingCombatActions();
    } else if (nAction == 0x32) {
        SetCombatMode(0, 1);
    } else if (nAction == 9) {
        SetAnimation(10000);
        m_n988 = 0;
    } else if (nAction == 7) {
        SetAnimation(10000);
        m_n98C = 0;
    } else if (nAction == 0x1d || nAction == 0x19 || nAction == 0x1a || nAction == 0x1c ||
               nAction == 0x1b) {
        SetAnimation(10000);
        StopGuiTimingBar();
        m_n97C = 0;
    } else if (nAction == 0x36) {
        SetAnimation(10000);
        m_n970 = 0;
        m_n974 = 0;
    } else if (nAction == 0x38) {
        SetAnimation(10000);
        m_n978 = 0;
    } else if (nAction == 0x26) {
        SetAnimation(10000);
        m_n980 = 0;
        StopGuiTimingBar();
    } else if (nAction == 0x27) {
        SetAnimation(10000);
        m_n984 = 0;
        StopGuiTimingBar();
    } else if (nAction == 0x14) {
        SetAnimation(10000);
        m_n1F0 = 0;
    } else if (nAction == 0x3d) {
        if (m_bA88) {
            int nIndex = GetPartyIndex(ServerToClientObjectId(m_idSelf));
            if (nIndex >= 0 && nIndex <= 2) {
                ((CSWPartyMemberView*)GetClientExoApp()->GetSWParty())->GetMemberData(nIndex)->Reset();
            }
        }
    } else if (nAction == 0x18) {
        GetClientExoApp()->GetInGameGui()->SetGlobalDialogState(0);
    } else if (nAction == 0x20) {
        m_n50 = 0;
    } else if (nAction == 0x28) {
        CSWSPlaceableAnimView* pPlaceable =
            (CSWSPlaceableAnimView*)GetPlaceableByGameObjectID(pNode->m_n38);
        if (pPlaceable && pPlaceable->m_n450) {
            pPlaceable->m_n450 = 0;
            if (pPlaceable->m_nAnimation == 0x275b) {
                pPlaceable->SetAnimation(0x275c);
            }
            pPlaceable->m_fAnimationSpeed = 1.0f;
        }
    }
    if (pMessage && pNode->m_nActionId == 1 && bArg) {
        SetAnimation(10000);
    }
    if (GetLockOrientationToObject() != OBJECT_INVALID) {
        GetLockOrientationToObject();
        SetFacingTarget4F34A0(OBJECT_INVALID, 0);
    }
    if (bArg == 1) {
        m_nActionStartDay = 0;
        m_nActionStartTime = 0;
    }
    return 1;
}
