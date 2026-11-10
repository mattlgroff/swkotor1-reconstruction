// Worker A round s11: CSWSCreature::UseItem (behavioural reference:
// src/units/1719/ae9f07f3ce-claude_vm_actions_movement.cpp).
#include "a/sws_creature.h"

// FUNCTION: SWKOTOR 0x004FC210
// NONMATCH (24): nResult lands in edx (original: eax, property in edx) and the shared
// return-0 block is placed at the end instead of after the m_bE8 test.
int CSWSCreature::UseItem(unsigned long oidItem, unsigned char nProperty, unsigned char nArg,
                          unsigned long oidTarget, const Vector& vTarget, unsigned long oidArea, int bArg)
{
    int bNoTarget = 0;
    CSWSItem* pItem = GetServerExoApp()->GetItemByGameObjectID(oidItem);
    CGameObject* pTarget = GetServerExoApp()->GetGameObject(oidTarget);
    if (pItem && CanUseItem(pItem, 0)) {
        CSWItemProperty* pProperty = pItem->GetActiveProperty(nProperty);
        int nResult = 0;
        if (pProperty) {
            if ((pProperty->m_nActivation == 0xff ||
                 (pItem->m_nPropertyActivationMask & (1 << pProperty->m_nActivation))) &&
                pProperty->m_nPropertyName == 0xa) {
                if (pProperty->m_nUsesLeft) {
                    if (pTarget && pTarget->AsSWSCreature()) {
                        CSWSCreatureListEntry* pEntry = GetFromList920(oidTarget);
                        if (pEntry && pEntry->m_n2 == 1) {
                            return AI_ACTION_RESULT_005140E0;
                        }
                    }
                    g_pRules->GetBaseItem(pItem->m_nBaseItem);
                    if (oidTarget == 0x7f000000) {
                        bNoTarget = 1;
                    }
                    int bQueued = m_b4E0 && GetNodeById(0xc) ? 1 : 0;
                    return AddItemCastSpellActions(oidItem, nProperty, nArg, vTarget, oidTarget, bArg,
                                                   bNoTarget, bQueued, 0);
                }
            }
            else if (pProperty->m_nPropertyName == 0x25) {
                return UseSkill(6, 0, oidTarget, vTarget, oidArea, oidItem, nProperty);
            }
            else if (pProperty->m_nPropertyName == 0x2e) {
                unsigned long oidMineTarget = oidTarget;
                unsigned long oidMine = pItem->m_n14;
                if (!m_bE8) {
                    return 0;
                }
                if (GetArea() && !GetArea()->GetPlayerCanSetMines()) {
                    return 0;
                }
                if (GetServerExoApp()->GetClientObjectByObjectId(m_idSelf)) {
                    Fn4F1460();
                    if (m_n9F2 < 10) {
                        ClearAllActions(1);
                    }
                }
                if (GetServerExoApp()->GetGameObject(oidMine) == NULL || !m_pStats->GetCanUseSkill(1)) {
                    return 0;
                }
                float fX = vTarget.x;
                float fY = vTarget.y;
                float fZ = vTarget.z;
                AddAction(0x1d, 0xffff, 3, &oidMine, 3, &oidMineTarget, 2, &fX, 2, &fY, 2, &fZ);
                nResult = 1;
            }
        }
        return nResult;
    }
    return 0;
}
