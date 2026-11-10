// Worker A round s10: CSWSCreature::EquipItem. functions.csv lists 0x004FEB90 as one 3904-byte
// row; the function ends at 0x004FECDF and the vtable-slot-0x78 handler starts at 0x004FECE0.
#include "a/sws_creature.h"

static inline void AddEvent(unsigned long oidCaller, unsigned long oidObject, unsigned long nEvent,
                            void* pData)
{
    GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(0, 0, oidCaller, oidObject, nEvent, pData);
}

// FUNCTION: SWKOTOR 0x004FEB90
int CSWSCreature::EquipItem(unsigned long nSlot, CSWSItem* pItem, int bApply, int a4, int a5)
{
    if (m_pInventory->GetItemInInventory(pItem)) {
        UnequipItem(pItem);
    }
    if (bApply) {
        pItem->ApplyItemProperties(this, nSlot, a4);
    }
    m_pInventory->PutItemInSlot(nSlot, pItem);
    ComputeArmourClass(pItem, 1, 0);
    m_pStats->UpdateForceResistance();
    if (!GetClientExoApp()->GetInGameGui()->m_pB4 && !a5 && !a4 && (nSlot == 0x10 || nSlot == 0x20)) {
        SetCombatState(1, 2);
    }
    int nWeight = m_nTotalWeight;
    m_nTotalWeight = nWeight + pItem->GetWeight();
    CScriptEvent* pEvent = new CScriptEvent;
    CSWSModule* pModule = GetServerExoApp()->GetModule6B0();
    pEvent->m_nType = 0x26;
    pEvent->SetObjectID(0, pItem->m_n14);
    AddEvent(m_idSelf, pModule->m_idSelf, EVENT_0x7441DC, pEvent);
    return 1;
}
