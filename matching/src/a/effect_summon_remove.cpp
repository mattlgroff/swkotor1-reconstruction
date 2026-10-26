#include "a/effect_list_handler.h"

// FUNCTION: SWKOTOR 0x004D8A10
int CSWSEffectListHandler::OnRemoveSummonCreature(CSWSObject*, CGameEffect* pEffect)
{
    CSWSCreature* pCreature = GetServerExoApp()->GetCreatureByGameObjectID(pEffect->GetObjectID(1));
    if (pCreature)
        pCreature->UnsummonMyself();
    return 1;
}
