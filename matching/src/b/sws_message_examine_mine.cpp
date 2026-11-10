// Mine examine data: trap type and how hard the viewer finds the disarm.
#include "b/sws_message.h"
#include "b/sws_creature.h"
#include "b/sws_player.h"
#include "b/sws_item.h"
#include "b/sws_door.h"

extern const unsigned char OBJECT_TYPE_TRIGGER;             // 0x00746636 (7)
extern const unsigned char OBJECT_TYPE_PLACEABLE;           // 0x00746638 (9)
extern const unsigned char OBJECT_TYPE_DOOR;                // 0x00746639 (10)

// Trap trigger (vtable slot 0x40, AsSWSTrigger).
class CSWSTrigger : public CSWSObject {
public:
    char pad_0xf4[0x2c4 - 0xf4];
    int m_bTrapDisarmable;                                  // 0x2c4 (name guessed)
    char pad_0x2c8[0x2d4 - 0x2c8];
    unsigned char m_nTrapType;                              // 0x2d4 (name guessed)
    char pad_0x2d5[0x2dc - 0x2d5];
    int m_nTrapDisarmDC;                                    // 0x2dc (name guessed)
};

// FUNCTION: SWKOTOR 0x0056F160
int CSWSMessage::SendServerToPlayerExamineGui_MineData(CSWSPlayer* pPlayer, unsigned long oid,
                                                       CSWSCreature* pCreature, int bArg)
{
    unsigned char* pMessage;
    unsigned long nSize;
    unsigned long nPlayerId = pPlayer->m_nPlayerID;
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oid);
    if (!pObject) {
        return 0;
    }
    int nSkill = pCreature->m_pStats->GetSkillRank(1, NULL, 0) + 20;
    g_pAppManager->GetServerExoApp()->GetNetLayer()->GetPlayerInfo(nPlayerId);
    CreateWriteMessage(10, 0xffffffff, 1);
    WriteOBJECTIDServer(oid);
    WriteBOOL(bArg);
    int nDC;
    if (pObject->m_nObjectType == OBJECT_TYPE_TRIGGER) {
        nDC = pObject->AsSWSTrigger()->m_nTrapDisarmDC;
        CSWSTrigger* pTrigger = pObject->AsSWSTrigger();
        WriteBYTE(pTrigger->m_nTrapType, 8);
        if (!pObject->AsSWSTrigger()->m_bTrapDisarmable || nDC > 35) {
            WriteBYTE(4, 8);
        } else if (nDC <= nSkill - 15) {
            WriteBYTE(0, 8);
        } else if (nDC <= nSkill - 10) {
            WriteBYTE(1, 8);
        } else if (nDC <= nSkill - 5) {
            WriteBYTE(2, 8);
        } else if (nDC <= nSkill) {
            WriteBYTE(3, 8);
        } else {
            WriteBYTE(4, 8);
        }
    } else if (pObject->m_nObjectType == OBJECT_TYPE_DOOR) {
        nDC = pObject->AsSWSDoor()->m_nTrapDisarmDC;
        CSWSDoor* pDoor = pObject->AsSWSDoor();
        WriteBYTE(pDoor->m_nTrapType, 8);
        if (!pObject->AsSWSDoor()->m_bTrapDisarmable || nDC > 35) {
            WriteBYTE(4, 8);
        } else if (nDC <= nSkill - 15) {
            WriteBYTE(0, 8);
        } else if (nDC <= nSkill - 10) {
            WriteBYTE(1, 8);
        } else if (nDC <= nSkill - 5) {
            WriteBYTE(2, 8);
        } else if (nDC <= nSkill) {
            WriteBYTE(3, 8);
        } else {
            WriteBYTE(4, 8);
        }
    } else if (pObject->m_nObjectType == OBJECT_TYPE_PLACEABLE) {
        nDC = pObject->AsSWSPlaceable()->m_nTrapDisarmDC;
        CSWSPlaceable* pPlaceable = pObject->AsSWSPlaceable();
        WriteBYTE(pPlaceable->m_nTrapType, 8);
        if (!pObject->AsSWSPlaceable()->m_n280 || nDC > 35) {
            WriteBYTE(4, 8);
        } else if (nDC <= nSkill - 15) {
            WriteBYTE(0, 8);
        } else if (nDC <= nSkill - 10) {
            WriteBYTE(1, 8);
        } else if (nDC <= nSkill - 5) {
            WriteBYTE(2, 8);
        } else if (nDC <= nSkill) {
            WriteBYTE(3, 8);
        } else {
            WriteBYTE(4, 8);
        }
    }
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 0x1b, 5, pMessage, nSize);
}
