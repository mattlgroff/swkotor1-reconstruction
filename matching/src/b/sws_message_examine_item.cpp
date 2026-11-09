// Item examine data.
#define B_SWS_ITEM_OPAQUE_PROPERTY_GETTERS
#include "b/sws_message.h"
#include "b/sws_creature.h"
#include "b/sws_player.h"
#include "b/sws_item.h"

// NONMATCH (85): same code shape; the original keeps the player in edi and
// the item in ebp (swapped here), which also swaps the loop counter and the
// passive count. Accessors, declaration order and count forms did not move it.
// FUNCTION: SWKOTOR 0x0056EE40
int CSWSMessage::SendServerToPlayerExamineGui_ItemData(CSWSPlayer* pPlayer, unsigned long oid)
{
    unsigned char* pMessage;
    unsigned long nSize;
    unsigned long nPlayerId = pPlayer->m_nPlayerID;
    CreateWriteMessage(0x80, nPlayerId, 1);
    CSWSItem* pItem = g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(oid);
    if (pItem) {
        CSWSCreature* pCreature = pPlayer->GetGameObject()->AsSWSCreature();
        WriteOBJECTIDServer(pItem->m_idSelf);
        WriteBOOL(1);
        if (pCreature) {
            WriteBOOL(pCreature->CanUseItem(pItem, 0));
        } else {
            WriteBOOL(0);
        }
        CExoLocString sDescription(pItem->m_lsDescIdentified);
        if (HasValidLocalizedString(&sDescription, 0)) {
            WriteCExoLocStringServer(sDescription, 0);
        } else {
            sDescription = pItem->m_lsDescription;
            WriteCExoLocStringServer(sDescription, 0);
        }
        int bLevelOk = 0;
        if (pCreature) {
            if (pCreature->GetStats()->GetLevel(0) >= pItem->GetMinEquipLevel()) {
                bLevelOk = 1;
            }
        }
        WriteBOOL(bLevelOk);
        if (!bLevelOk) {
            WriteBYTE(pItem->GetMinEquipLevel(), 8);
        }
        WriteINT(pItem->m_nCharges, 0x20);
        WriteINT(pItem->GetWeight(), 0x20);
        int nPassive = pItem->m_nPassiveProperties;
        int nActive = pItem->m_nActiveProperties;
        int i;
        for (i = 0; i < nPassive; i++) {
            pItem->GetPassiveProperty(i);
        }
        WriteWORD(nActive + nPassive, 0x10);
        for (i = 0; i < nActive; i++) {
            CSWItemProperty* pProperty = pItem->GetActiveProperty(i);
            WriteWORD(pProperty->m_nPropertyName, 0x10);
            WriteWORD(pProperty->m_nSubType, 0x10);
            WriteWORD(pProperty->m_nCostValue, 0x10);
            unsigned char nParam = pProperty->m_nParam1Value;
            WriteBYTE(nParam, 8);
        }
        for (i = 0; i < nPassive; i++) {
            pItem->GetPassiveProperty(i);
        }
    } else {
        WriteOBJECTIDServer(OBJECT_INVALID_ID);
    }
    if (GetWriteMessage(&pMessage, &nSize)) {
        return SendServerToPlayerMessage(nPlayerId, 0x1b, 2, pMessage, nSize);
    }
    return 0;
}
