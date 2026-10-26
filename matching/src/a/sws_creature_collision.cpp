// Creature collision flag query (descriptive name; native 0x004F62A0).
#include "a/sws_creature.h"

struct CCollisionActionListNode {
    void* m_pPrev;
    void* m_pNext;
    CSWSObjectActionNode* m_pAction;
};
extern int g_bCollisionDisabled;                    // 0x007A1B34 (descriptive)

// NONMATCH (15): return-block placement and register allocation differ.
// FUNCTION: SWKOTOR 0x004F62A0
int CSWSCreature::GetCollisionFlag(CSWSCreature* pOther)
{
    if (g_bCollisionDisabled)
        return 0;
    int result = 0;
    if (GetReputation(pOther->m_idSelf, 0) < 10 || pOther->m_bA88)
        return result;
    CServerExoApp* pServer = GetServerExoApp();
    CExoLinkedListInternal** ppActions = &pOther->m_pActions;
    CSWSCreature* pSelected = pServer->GetCreatureByGameObjectID(pServer->GetPlayerCreatureID());
    if (pSelected != this) {
        result = 0;
        if (pSelected != pOther && ppActions && !(*ppActions)->m_pHead &&
            !pOther->m_b4E0 && !pOther->m_pCombatRound->m_n958)
            result = 1;
    } else if (pSelected != pOther) {
        CCollisionActionListNode* pNode = (CCollisionActionListNode*)(*ppActions)->m_pHead;
        result = pOther->m_bA88;
        if (pNode && pNode->m_pAction) {
            unsigned long nAction = pNode->m_pAction->m_nActionId;
            if (nAction == 12 || nAction == 63 || nAction == 15 || nAction == 46)
                result = 0;
        }
    }
    return result;
}
