// CSWSObject::DoDamageImmunity (hidden after LoadActionQueue).
#include "i/sws_object.h"

// FUNCTION: SWKOTOR 0x004CF160
int CSWSObject::DoDamageImmunity(CSWSCreature* pAttacker, int nDamage, unsigned short nFlags,
                                 int bSimulate, int bFeedback)
{
    int nImmunity = GetTotalDamageImmunity(nFlags);
    int nImmune;
    if (nImmunity > 0) {
        nImmune = nImmunity * nDamage / 100;
        if (nImmune < 1) {
            nImmune = 1;
        }
    }
    else {
        nImmune = nImmunity * nDamage / 100;
    }
    int nResult = nDamage - nImmune;
    if (!bSimulate && nImmunity > 0) {
        if (bFeedback == 1 && pAttacker) {
            CSWCCMessageData* pData = new CSWCCMessageData();
            pData->SetInteger(0, 0x3e);
            pData->SetInteger(1, nDamage - nResult);
            pData->SetInteger(2, nFlags);
            pData->SetObjectID(0, m_idSelf);
            CSWSCombatRound* pRound = pAttacker->m_pcCombatRound;
            CSWSCombatAttackData* pAttack = pRound->GetAttack(pRound->m_nCurrentAttack);
            pAttack->m_alstPendingFeedback.AddOutOfLine(pData);
        }
        else {
            CSWCCMessageData* pData = new CSWCCMessageData();
            CSWCCMessageData* pOther = new CSWCCMessageData();
            pData->SetInteger(0, 0x3e);
            pData->SetInteger(1, nDamage - nResult);
            pData->SetInteger(2, nFlags);
            pData->SetObjectID(0, m_idSelf);
            pData->CopyTo(pOther);
            if (AsSWSCreature()) {
                AsSWSCreature()->SendFeedbackMessage(0x3e, pData);
            }
            else {
                delete pData;
            }
            if (pAttacker) {
                pAttacker->SendFeedbackMessage(0x3e, pOther);
            }
            else {
                delete pOther;
            }
        }
    }
    return nResult;
}
