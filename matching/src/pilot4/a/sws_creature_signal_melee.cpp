// Pilot 4 arm A: CSWSCreature::SignalMeleeDamage.
// NONMATCH (40 diff insns, 1 byte longer): control flow, stack and calls match;
// differences are register choice for argument loads (target/self, ms update)
// and the loop-entry count load being hoisted above the previous list clear.
#include "pilot4/a/p4a_signal_melee.h"

// FUNCTION: SWKOTOR 0x005B75D0
void CSWSCreature::SignalMeleeDamage(unsigned long oidTarget, int nAttacks)
{
    CServerAIMaster* pAI = g_pAppManager->GetServerExoApp()->GetServerAIMaster();
    CSWSCombatRound* pRound = m_pCombatRound;
    unsigned long nTime = 0;
    for (int i = 0; i < nAttacks; i++) {
        CSWSCombatAttackData* pAttack = pRound->GetAttack(i);
        pAI->AddEventDeltaTime(0, i, m_idSelf, oidTarget, EVENT_ATTACK, pAttack);
        m_pCombatRound->GetAttack(i);
        int nResult = pAttack->m_nAttackResult;
        if ((nResult == -1 && !m_b558) || (nResult != -1 && nResult > 0 && nResult <= 3)) {
            CGameEffect* pEffect = new CGameEffect(1);
            pEffect->m_nType = EFFECT_TYPE_DAMAGE;
            pEffect->SetDurationType(DURATION_TYPE_INSTANT);
            pEffect->SetCreator(m_idSelf);
            pEffect->SetNumIntegers(0x16);
            for (int k = 0; k < 15; k++) {
                pEffect->SetInteger(k, pAttack->m_nDamage[k]);
            }
            pEffect->SetInteger(15, 1);
            pEffect->SetInteger(16, pAttack->m_nTime >> 1);
            pEffect->SetInteger(0x13, 1);
            pEffect->SetInteger(0x15, 1);
            pAI->AddEventDeltaTime(0, nTime, m_idSelf, oidTarget, EVENT_APPLY_EFFECT, pEffect);
            for (int nEvent = 0; nEvent < pAttack->m_alstOnHitEvents.num; nEvent++) {
                pAI->AddEventDeltaTime(0, nTime, m_idSelf, oidTarget, EVENT_ON_HIT,
                                       pAttack->m_alstOnHitEvents.element[nEvent]);
            }
            pAttack->m_alstOnHitEvents.num = 0;
            for (int nEffect = 0; nEffect < pAttack->m_alstOnHitEffects.num; nEffect++) {
                CSWSCreature* pTarget = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidTarget);
                if (pTarget) {
                    pTarget->ApplyEffect(pAttack->m_alstOnHitEffects.element[nEffect], 0, 0);
                } else {
                    pAI->AddEventDeltaTime(0, nTime, m_idSelf, oidTarget, EVENT_APPLY_EFFECT,
                                           pAttack->m_alstOnHitEffects.element[nEffect]);
                }
                pAttack->m_alstOnHitEffects.element[nEffect] = 0;
            }
            pAttack->m_alstOnHitEffects.num = 0;
            for (int nSelf = 0; nSelf < pAttack->m_alstSelfEvents.num; nSelf++) {
                pAI->AddEventDeltaTime(0, nTime, m_idSelf, m_idSelf, EVENT_SELF,
                                       pAttack->m_alstSelfEvents.element[nSelf]);
            }
            pAttack->m_alstSelfEvents.num = 0;
        } else {
            CClientExoApp* pClient = g_pAppManager->GetClientExoApp();
            if (pClient->ServerToClientObjectId(m_idSelf) == pClient->GetPlayerCreatureId()) {
                pClient->AddFloatyText(pClient->ServerToClientObjectId(oidTarget), 2, 0);
            }
        }
        nTime += pAttack->m_nAnimationTime >> 1;
    }
}
