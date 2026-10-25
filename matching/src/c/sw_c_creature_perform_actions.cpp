#include "c/client_exo_app.h"

class CSWCMessage {
public:
    int SendPlayerToServerInput_Attack(unsigned long oid, unsigned short nFeat); // 0x00677A90
};

// FUNCTION: SWKOTOR 0x006167D0
void CSWCCreature::PerformFeat(int nFeat, CSWCCreature* pTarget)
{
    if (pTarget != 0) pTarget->SetAttackTarget(1);
    PartyMenuUseItem((unsigned long)nFeat, pTarget);
}

// FUNCTION: SWKOTOR 0x00616900
void CSWCCreature::PerformSkill(unsigned short nSkill, CSWCCreature* pTarget)
{
    CSWCMessage* pMessage = g_pAppManager->GetClientExoApp()->GetSWCMessage();
    if (pMessage != 0) pMessage->SendPlayerToServerInput_Attack(m_idSelf, nSkill);
    if (pTarget != 0) pTarget->SetAttackTarget(1);
}
