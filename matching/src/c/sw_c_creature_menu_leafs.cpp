#include "c/client_exo_app.h"

class CSWSCreature {
public:
    int GetIsInjured();                             // 0x004EFF30
};

// FUNCTION: SWKOTOR 0x00614B60
int CSWCCreature::IsInjured()
{
    if (GetServerObject() != 0) {
        CSWSCreature* pServerCreature = GetServerObject()->AsSWSCreature();
        return pServerCreature->GetIsInjured();
    }
    CSWSCreature* pServerCreature = 0;
    return pServerCreature->GetIsInjured();
}

// FUNCTION: SWKOTOR 0x00616200
void CSWCCreature::ForceMenuHostileAction(unsigned long nAction,
                                           CSWCCreature* pCreature)
{
    if (pCreature != 0) {
        pCreature->SetAttackTarget(1);
    }
    ForceMenuAction(nAction, pCreature);
}
