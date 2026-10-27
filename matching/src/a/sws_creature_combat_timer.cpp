// Worker A round s7: CSWSCreature::UpdateCombatRoundTimer (nwscreature.cpp).
// GetAttack is an out-of-line call from this translation unit.
#define SWS_COMBAT_ROUND_OUTLINE_GETATTACK
#include "a/sws_combat_round.h"

// FUNCTION: SWKOTOR 0x004FAF20
void CSWSCreature::UpdateCombatRoundTimer()
{
    if (m_pcCombatRound && !g_pAppManager->GetServerExoApp()->GetPauseState(2)) {
        if (m_pcCombatRound->m_bRoundPaused) {
            if (!m_pcCombatRound->m_bInfinitePause) {
                CSWSCombatAttackData* pAttack = m_pcCombatRound->GetAttack(m_pcCombatRound->m_nCurrentAttack);
                int nRemaining = m_pcCombatRound->m_nPauseTimer - m_nUpdateDelta;
                if (nRemaining > 0) {
                    m_pcCombatRound->DecrementPauseTimer(m_nUpdateDelta);
                    if (pAttack->m_alstSubAttacks.num > 0 &&
                        pAttack->IsSubAttackReady(m_pcCombatRound->m_nPauseTimer)) {
                        pAttack->ResolveTopSubAttackToAttack();
                        SignalDamage(pAttack->m_nField40, pAttack->m_oidReactObject);
                        pAttack->RemoveTopSubAttack();
                    }
                }
                else {
                    while (pAttack->m_alstSubAttacks.num > 0) {
                        pAttack->ResolveTopSubAttackToAttack();
                        SignalDamage(pAttack->m_nField40, pAttack->m_oidReactObject);
                        pAttack->RemoveTopSubAttack();
                    }
                    ResetCombatAnimations(-nRemaining);
                    if (m_pcCombatRound->m_bMaster) {
                        unsigned long oidSlave = m_pcCombatRound->m_oidSlave;
                        CSWSCreature* pSlave = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidSlave);
                        if (pSlave) {
                            pSlave->ResetCombatAnimations(-nRemaining);
                        }
                    }
                    unsigned long oidTarget = m_pcCombatRound->m_oidField9cc;
                    CGameObject* pTarget = g_pAppManager->GetServerExoApp()->GetGameObject(oidTarget);
                    if (pTarget && pTarget->AsSWSObject()) {
                        CSWSObject* pObject = pTarget->AsSWSObject();
                        if (pObject->GetIsInConversation() ||
                            (pObject->AsSWSCreature() && pObject->AsSWSCreature()->GetDeadTemp())) {
                            m_pcCombatRound->EndCombatRound(1);
                        }
                    }
                }
            }
            else {
                m_pcCombatRound->DecrementPauseTimer(m_nUpdateDelta);
            }
        }
        else {
            m_pcCombatRound->IncrementTimer(m_nUpdateDelta);
        }
    }
}
