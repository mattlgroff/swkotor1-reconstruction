// CSWSCreature gold transfer (nwscreature.cpp). Descriptive name: no symbol record.
#include "a/sws_creature.h"

class CSWSObject {
public:
    int AddGiveItemActions(unsigned long oidItem, unsigned long oidTarget, int a3, int a4); // 0x0057C870
};

// NONMATCH: 44 diff: amount/target registers swap (ebx/ebp) and the two give-item tails get cross-jumped.
// FUNCTION: SWKOTOR 0x004FD730
int CSWSCreature::TransferGold(unsigned long oidTarget, Vector* pPosition, unsigned long nAmount)
{
    if (nAmount <= 0) return 0;
    if (nAmount > (unsigned long)GetGold()) nAmount = GetGold();
    CSWSItem* pItem = new CSWSItem(0x7f000000);
    CResRef cTemplate("g_i_credits001");
    pItem->LoadFromTemplate(cTemplate);
    pItem->m_nStackSize = nAmount;
    pItem->SetPossessor(m_idSelf, 0, 0, 0x7f000000);
    if (pItem) {
        if (oidTarget != 0x7f000000) {
            CGameObject* pTarget = GetServerExoApp()->GetGameObject(oidTarget);
            if (pTarget) {
                if (pTarget->AsSWSCreature()) {
                    GetItemRepository(1)->AddItem(&pItem, 1, 0, 1);
                    RemoveGold(nAmount, 1);
                    return 1;
                }
                if (pTarget->AsSWSPlaceable()) {
                    if (!pTarget->AsSWSPlaceable()->m_bLocked) {
                        reinterpret_cast<CSWSObject*>(this)->AddGiveItemActions(pItem->m_n14, oidTarget, -1, 0);
                        return 1;
                    }
                } else if (pTarget->AsSWSItem()) {
                    if (g_pRules->GetBaseItem(pTarget->AsSWSItem()->m_nBaseItem)->m_bContainer) {
                        reinterpret_cast<CSWSObject*>(this)->AddGiveItemActions(pItem->m_n14, oidTarget, -1, 0);
                        return 1;
                    }
                }
            }
        } else {
            AddAction9(pItem->m_n14, pPosition, 1);
            return 1;
        }
        delete pItem;
    }
    return 0;
}
