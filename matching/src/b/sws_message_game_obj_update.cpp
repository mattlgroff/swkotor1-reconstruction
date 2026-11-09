// Per-player game object update message: the update stages run until the
// message reaches 2000 bytes; the stage reached is written as work remaining.
#include "b/sws_message.h"
#include "b/sws_creature.h"
#include "b/sws_player.h"

class CExoTimers {
public:
    unsigned __int64 GetHighResolutionTimer();              // 0x005E9260
};

class CExoBase {
public:
    void* m_p0;                                             // 0x0
    CExoTimers* m_pcExoTimers;                              // 0x4
};

extern CExoBase* g_pExoBase;                                // 0x007A39E0

// FUNCTION: SWKOTOR 0x00578200
int CSWSMessage::SendServerToPlayerGameObjUpdate(CSWSPlayer* pPlayer, unsigned long oidArg)
{
    unsigned char* pMessage;
    unsigned long nSize;
    int nCount;
    g_pExoBase->m_pcExoTimers->GetHighResolutionTimer();
    CSWSObject* pObserver = (CSWSObject*)pPlayer->GetGameObject();
    CSWSArea* pArea = NULL;
    if (pObserver) {
        pArea = pObserver->GetArea();
    }
    CGameObjectArray* pArray = g_pAppManager->GetServerExoApp()->GetObjectArray();
    if (!pArray) {
        return 0;
    }
    CreateWriteMessage(0x400, pPlayer->m_nPlayerID, 1);
    int nStage = 0;
    int bFull = 0;
    DeleteLastUpdateObjectsInOtherAreas(pPlayer);
    g_pExoBase->m_pcExoTimers->GetHighResolutionTimer();
    if (PeekAtWriteMessageSize() >= 2000) {
        bFull = 1;
    } else {
        nStage = 1;
        if (pArea && pObserver) {
            ComputeGameObjectUpdateForObject(pPlayer, pObserver, pArray, pObserver->m_idSelf);
        }
        if (PeekAtWriteMessageSize() >= 2000) {
            bFull = 1;
        } else {
            nStage = 2;
            if (pArea && pObserver) {
                WriteGameObjUpdate_MajorGUIPanels(pPlayer);
            }
            if (PeekAtWriteMessageSize() >= 2000) {
                bFull = 1;
            }
        }
    }
    CSWSPlayerLUOSortedObjectList* pSorted =
        SortObjectsForGameObjectUpdate(pPlayer, pObserver, pArray, &nCount);
    if (!bFull) {
        nStage = 3;
        if (!ComputeGameObjectUpdateForCategory(1, 2000, pPlayer, pObserver, pArray, pSorted, nCount)) {
            nStage = 4;
            WriteGameObjUpdate_MinorGUIPanels(pPlayer);
            if (PeekAtWriteMessageSize() < 2000) {
                nStage = 6;
                if (!ComputeGameObjectUpdateForCategory(3, 2000, pPlayer, pObserver, pArray, pSorted, nCount)) {
                    nStage = 7;
                    if (!ComputeGameObjectUpdateForCategory(4, 2000, pPlayer, pObserver, pArray, pSorted, nCount)) {
                        nStage = 8;
                        WriteGameObjUpdate_PartyAIState(pPlayer);
                        if (PeekAtWriteMessageSize() < 2000) {
                            nStage = 9;
                            if (!ComputeGameObjectUpdateForCategory(5, 2000, pPlayer, pObserver, pArray, pSorted, nCount)) {
                                nStage = 10;
                                if (!ComputeGameObjectUpdateForCategory(6, 2000, pPlayer, pObserver, pArray, pSorted, nCount)) {
                                    nStage = 11;
                                    if (!ComputeGameObjectUpdateForCategory(7, 2000, pPlayer, pObserver, pArray, pSorted, nCount)) {
                                        nStage = 12;
                                        if (!ComputeGameObjectUpdateForCategory(8, 2000, pPlayer, pObserver, pArray, pSorted, nCount)) {
                                            nStage = 13;
                                            if (!ComputeGameObjectUpdateForCategory(9, 2000, pPlayer, pObserver, pArray, pSorted, nCount)) {
                                                nStage = 14;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    WriteGameObjUpdate_WorkRemaining(pObserver, (int)pArea, nStage, 14);
    if (pSorted) {
        delete pSorted;
    }
    if (GetWriteMessage(&pMessage, &nSize)) {
        if (nSize != 0) {
            unsigned long nPlayerId = pPlayer->m_nPlayerID;
            return SendServerToPlayerMessage(nPlayerId, 5, 1, pMessage, nSize);
        }
        return 1;
    }
    return 0;
}
