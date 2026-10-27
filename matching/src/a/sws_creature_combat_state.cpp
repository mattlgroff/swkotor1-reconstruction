// CSWSCreature::SetCombatState (nwscreature.cpp).
#include <stdlib.h>
#include "a/sws_creature.h"

class CSWSFaction {
public:
    char pad_0x0[0xc];
    int m_nFactionId;                               // 0x0c
};

class CSWSObject {
public:
    char pad_0x0[0x18];
    CExoString m_sTag;                              // 0x18
};

// NONMATCH: 21 diff: debug-format locals load in a different order/registers; faction id lands in ecx instead of eax.
// FUNCTION: SWKOTOR 0x004F2610
void CSWSCreature::SetCombatState(int bInCombat, unsigned char nFlag)
{
    if (GetFaction()->m_nFactionId == 5) bInCombat = 0;
    if (m_b4E0 != bInCombat && bInCombat == 1 && nFlag == 1 && !m_bA88 && !m_b9D4 && rand() % 10 < 3)
        BroadcastBattleCry();
    m_b4E0 = bInCombat;
    if (bInCombat) {
        m_n4E4 = 8000;
        if (m_nAC0 != 1) m_nAC0 = nFlag;
    } else {
        m_n4E4 = 0;
        m_nAC0 = 0;
        m_n4DC = 1;
        m_n540 = -1;
        m_n544 = -1;
        m_oid53C = 0x7f000000;
        SetLastHostileActor(0x7f000000, 1);
        m_n548 = -1;
        m_n54C = -1;
        m_n550 = 0;
        m_n554 = 0;
        m_pCombatRound->ClearAllAttacks();
        m_pCombatRound->RemoveAllActions();
        m_oidAttackTarget = 0x7f000000;
        m_oidAttemptedTarget = 0x7f000000;
        m_oid524 = 0x7f000000;
        m_oid528 = 0x7f000000;
        m_pCombatRound->m_oid9CC = 0x7f000000;
        CSWSCombatRound* pRound = m_pCombatRound;
        if (pRound->m_bRoundStarted) {
            CExoString sText;
            CExoString sTarget;
            CGameObject* pTarget = GetServerExoApp()->GetGameObject(pRound->m_oid9C0);
            if (pTarget && pTarget->AsSWSObject()) {
                sTarget = reinterpret_cast<CSWSObject*>(pTarget->AsSWSObject())->m_sTag;
                if (sTarget.IsEmpty()) sTarget = "NULL";
            }
            int n9BC = pRound->m_n9BC;
            int n9B8 = pRound->m_n9B8;
            int n94C = pRound->m_n94C;
            unsigned long oid9C0 = pRound->m_oid9C0;
            int n944 = pRound->m_n944;
            unsigned long oidSelf = m_idSelf;
            sText.Format("WARNING:  %x %s %d %d %d %d %x %s %x", n9BC, m_sTag.CStr(), n944, oidSelf, n94C, n9B8,
                         oid9C0, sTarget.CStr(), pTarget);
            m_pCombatRound->EndCombatRound(0);
        }
    }
    if (GetClientCreature() && nFlag == 1) GetClientCreature()->SetCombatState(bInCombat);
}

class CSWSFactionMembers {
public:
    int GetMemberList(unsigned long** ppList);      // 0x005BE780
    char pad_0x0[0x4];
    int m_nMembers;                                 // 0x04
};

// FUNCTION: SWKOTOR 0x004FBBE0
void CSWSCreature::BroadcastCombatStateToParty(unsigned long oidHostile)
{
    CSWSCreature* pHostile = GetServerExoApp()->GetCreatureByGameObjectID(oidHostile);
    unsigned long oidTarget = 0x7f000000;
    unsigned long oidTarget2 = 0x7f000000;
    if (pHostile) {
        oidTarget = pHostile->m_oidAttackTarget;
        oidTarget2 = pHostile->m_oid524;
    }
    if (GetReputation(oidHostile, 0) <= 10 || oidHostile == m_idSelf) {
        if (oidTarget == m_idSelf || oidTarget2 == m_idSelf) SetCombatState(1, 1);
        else SetCombatState(1, 2);
    }
    if (m_bF8) return;
    unsigned long* pMembers;
    reinterpret_cast<CSWSFactionMembers*>(GetFaction())->GetMemberList(&pMembers);
    for (int i = 0; i < reinterpret_cast<CSWSFactionMembers*>(GetFaction())->m_nMembers; i++) {
        CGameObject* pObject = GetServerExoApp()->GetGameObject(pMembers[i]);
        if (!pObject || !pObject->AsSWSObject()) continue;
        CSWSCreature* pOther = reinterpret_cast<CSWSCreature*>(pObject->AsSWSObject());
        Vector vDelta = m_vPosition - pOther->m_vPosition;
        float fRange = 0.0f;
        if (m_bA88) {
            fRange = 30.0f;
        } else if (pOther->AsSWSCreature()) {
            fRange = pOther->AsSWSCreature()->GetSpotCheckDistance();
        } else {
            CExoString sColumn("PrimaryRange");
            g_pRules->Get2DArrays()->GetRanges()->GetFLOATEntry(11, sColumn, &fRange);
        }
        if (vDelta.MagnitudeSquared() <= fRange * fRange &&
            pOther->AsSWSCreature()->GetReputation(oidHostile, 0) <= 10) {
            if (oidTarget == pOther->m_idSelf || oidTarget2 == pOther->m_idSelf)
                pOther->AsSWSCreature()->SetCombatState(1, 1);
            else
                pOther->AsSWSCreature()->SetCombatState(1, 2);
            pOther->AsSWSCreature()->SetExcitedState(1);
        }
    }
}
