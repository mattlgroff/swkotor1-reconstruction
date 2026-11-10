// Worker A round s10: CSWSCreature::AddAttackActions. functions.csv lists 0x004FDE40 as one
// 3408-byte row; the function ends at 0x004FE20E and the slot-0x70 virtual starts at 0x004FE210.
#include "a/sws_creature.h"

extern "C" int rand();                              // 0x006FC447

static inline void AddEvent(unsigned long oidCaller, unsigned long oidObject, unsigned long nEvent,
                            void* pData)
{
    GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(0, 0, oidCaller, oidObject, nEvent, pData);
}

static inline int GetIsSelectedNPCObject(unsigned long oid)
{
    return GetServerExoApp()->GetPartyTable()->GetIsSelectedNPCObject(oid);
}

// NONMATCH (52): one cause. The `nOne = 1` store makes cl keep the constant 1 in ebx for the whole
// function (push ebx / cmp x,ebx); the original keeps immediates and stores `mov [slot],1` directly.
// With any other stored value the output is byte-identical. Tried: bClear = 1 (the original reuses
// that argument slot), unsigned/array/top-level local, const-reference temporary, single return.
// FUNCTION: SWKOTOR 0x004FDE40
int CSWSCreature::AddAttackActions(unsigned long oidTarget, int a2, int a3, int a4, int bClear, int bFront,
                                   int bImmediate, int a8, int a9, int a10, int a11, int a12, int a13,
                                   int a14, int a15)
{
    if (!bImmediate) {
        if (a3 && bClear) {
            ClearAllActions(1);
            m_pCombatRound->RemoveAllActions();
            m_pCombatRound->ClearAllAttacks();
        }
        m_pCombatRound->AddAttackAction(oidTarget, a2, a3, 1500, 1);
        if (!m_b4E0 && GetServerExoApp()->GetClientObjectByObjectId(m_idSelf)) {
            Fn4F1460();
            ClearAllActions(1);
        }
        if (!GetNodeById(0x3f)) {
            int nOne = 1;
            AddAction(0x3f, 0xffff, 1, &nOne);
        }
    } else {
    Fn4F1460();
    if (!m_bE8) {
        return 0;
    }
    CSWSCreatureListEntry* pEntry = GetFromList920(oidTarget);
    if (pEntry && pEntry->m_n2 == 1) {
        return 0;
    }
    if (oidTarget != 0x7f000000) {
        CGameObject* pObject = GetServerExoApp()->GetGameObject(oidTarget);
        if (pObject && pObject->AsSWSCreature() && GetAIStateReputation(oidTarget) == 1) {
            SendFeedbackMessage(0xbb, 0);
            return 0;
        }
    }
    if (bClear) {
        ClearAllActions(1);
    }
    if (oidTarget != 0x7f000000) {
        if (bFront) {
            AddActionToFront(0xc, 0xffff, 1, &a12, 3, &oidTarget, 1, &a8, 1, &a9, 1, &a10, 1, &a11, 1, &a2, 1,
                             &a13, 1, &a14, 1, &a15);
        } else {
            AddAction(0xc, 0xffff, 1, &a12, 3, &oidTarget, 1, &a8, 1, &a9, 1, &a10, 1, &a11, 1, &a2, 1, &a13,
                      1, &a14, 1, &a15);
        }
        CSWSCreature* pTarget = GetServerExoApp()->GetCreatureByGameObjectID(oidTarget);
        if (pTarget &&
            (pTarget->m_b9D4 == 1 || GetIsSelectedNPCObject(pTarget->m_idSelf) == 1)) {
            pTarget->m_oid520 = m_idSelf;
        }
        BroadcastCombatStateToParty(m_idSelf);
        if (pTarget) {
            m_n4DC = m_bA88 == 0;
        }
        if (!m_b4E0 && m_b9D4 == 1) {
            if (rand() % 4 < 3) {
                BroadcastBattleCry();
            }
        } else if (m_bA88 == 1) {
            if (rand() % 10 == 0) {
                BroadcastBattleCry();
            }
        }
        if (m_oidAttackTarget == 0x7f000000) {
            m_oidAttackTarget = oidTarget;
        }
        m_n4E8 = a4;
    }
    CSWSDoor* pDoor = GetServerExoApp()->GetDoorByGameObjectID(oidTarget);
    if (pDoor && pDoor->m_bF8 == 1) {
        CScriptEvent* pEvent = new CScriptEvent;
        pEvent->m_nType = 0x22;
        AddEvent(m_idSelf, oidTarget, EVENT_0x7441DC, pEvent);
    }
    }
    return 1;
}
