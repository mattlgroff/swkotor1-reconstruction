// Creature debug info: tag, faction, template, hit points, armour class,
// area and the tags of the equipped and carried items.
#define B_ITEM_REPOSITORY_OPAQUE_GET_ITEM_AT
#include "b/sws_message.h"
#include "b/sws_creature.h"
#include "b/sws_player.h"
#include "b/sws_item.h"

// FUNCTION: SWKOTOR 0x00570230
int CSWSMessage::SendServerToPlayerDebugInfo_Creature(CSWSPlayer* pPlayer, unsigned long oidCreature)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CSWSCreature* pCreature =
        g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    if (!pCreature || !pCreature->GetFaction() || !pCreature->m_pInventory ||
        !pCreature->GetItemRepository(1) || !pCreature->m_pStats) {
        return 0;
    }
    CreateWriteMessage(0x100, 0xffffffff, 1);
    WriteOBJECTIDServer(oidCreature);
    WriteCExoString(pCreature->m_sTag, 0x20);
    CSWSFaction* pFaction = pCreature->GetFaction();
    WriteCExoString(pFaction->m_sName, 0x20);
    WriteCResRef(pCreature->GetTemplateResRef(), 0x10);
    WriteWORD(pCreature->GetCurrentHitPoints(1), 0x10);
    WriteWORD(pCreature->GetCreatureSize(1), 0x10);
    WriteWORD(pCreature->GetArmorClass(), 0x10);
    WriteINT(pCreature->GetAILevel(), 0x20);
    float fChallengeRating = pCreature->GetStats()->m_fChallengeRating;
    WriteFLOAT(fChallengeRating, 1.0f, 0x20);
    if (pCreature->GetArea()) {
        CSWSArea* pArea = pCreature->GetArea();
        WriteCExoString(pArea->m_sTag, 0x20);
    } else {
        CExoString sEmpty("");
        WriteCExoString(sEmpty, 0x20);
    }
    unsigned long nEquipped = 0;
    unsigned long i;
    for (unsigned char c = 0; c < 18; c++) {
        if (pCreature->GetInventory()->GetItemInSlot(1 << c)) {
            nEquipped |= 1 << c;
        }
    }
    WriteDWORD(nEquipped, 0x20);
    for (unsigned char c = 0; c < 18; c++) {
        CSWSItem* pItem = pCreature->GetInventory()->GetItemInSlot(1 << c);
        if (pItem) {
            WriteCExoString(pItem->m_sTag, 0x20);
        }
    }
    WriteDWORD(pCreature->GetItemRepository(1)->GetNumItems(), 0x20);
    for (i = 0; i < pCreature->GetItemRepository(1)->GetNumItems(); i++) {
        CSWSItem* pItem = pCreature->GetItemRepository(1)->GetItemAt(i);
        if (pItem) {
            WriteCExoString(pItem->m_sTag, 0x20);
        }
    }
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    unsigned long nPlayerId = pPlayer->m_nPlayerID;
    return SendServerToPlayerMessage(nPlayerId, 0x21, 1, pMessage, nSize);
}
