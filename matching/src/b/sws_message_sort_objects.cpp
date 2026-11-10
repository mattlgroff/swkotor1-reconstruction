// CSWSMessage::SortObjectsForGameObjectUpdate (round s13): the objects of
// the observer's area that the player gets updates for, sorted by category
// and then by distance (insertion into a `new[]` array grown by 32).
#include "b/sws_message.h"
#include "b/sws_creature.h"
#include "b/sws_player.h"
#include "b/server_exo_app.h"
#include <math.h>

extern const unsigned char OBJECT_TYPE_AREA;                // 0x00746633 (4)
extern const unsigned char GAME_OBJECT_VALID;               // 0x0074666C

class CSWSPlayerLUOSortedObjectList {
public:
    CSWSPlayerLUOSortedObjectList();                        // 0x00566D80

    unsigned long m_oidObject;                              // 0x0
    float m_fDistance;                                      // 0x4
    unsigned long m_nCategory;                              // 0x8
};

// FUNCTION: SWKOTOR 0x00567850
CSWSPlayerLUOSortedObjectList* CSWSMessage::SortObjectsForGameObjectUpdate(
    CSWSPlayer* pPlayer, CSWSObject* pObserver, CGameObjectArray* pArray, int* pCount)
{
    int nCount = 0;
    int nCapacity = 0;
    CSWSPlayerLUOSortedObjectList* pList = NULL;
    CGameObject* pObject = NULL;
    CSWSArea* pArea = NULL;
    Vector vPosition;
    unsigned long oidObject;
    if (pObserver) {
        pArea = pObserver->GetArea();
        vPosition = pObserver->m_vPosition;
    }
    if (!pObserver || !pArea) {
        CSWSCreature* pCreature = pObserver->AsSWSCreature();
        if (!pCreature) {
            goto done;
        }
        CSWSModule* pModule = g_pAppManager->GetServerExoApp()->GetModule();
        if (!pModule) {
            goto done;
        }
        pArea = pModule->GetArea();
        vPosition = pCreature->m_vLastPosition;
        if (!pArea) {
            goto done;
        }
    }
    if (pArea->GetFirstObjectIdInArea(&oidObject) != 1) {
        goto done;
    }
    do {
        if (pArray->GetGameObject(oidObject, &pObject) != GAME_OBJECT_VALID || !pObject ||
            pObject->m_nObjectType <= OBJECT_TYPE_AREA) {
            continue;
        }
        float fDY = pObject->m_vPosition.y - vPosition.y;
        float fDX = pObject->m_vPosition.x - vPosition.x;
        float fDistanceSq = fDX * fDX + fDY * fDY;
        int bInclude = 0;
        float fRange = 250.0f;
        if (pObserver->AsSWSCreature()) {
            fRange = (pObserver->AsSWSCreature()->GetSpotCheckDistance() >=
                      pObserver->AsSWSCreature()->GetListenCheckDistance())
                         ? pObserver->AsSWSCreature()->GetSpotCheckDistance()
                         : pObserver->AsSWSCreature()->GetListenCheckDistance();
        }
        if (fDistanceSq < fRange * fRange ||
            pPlayer->GetLastUpdateObject(pObserver->GetId())) {
            bInclude = 1;
        }
        unsigned long nCategory = SelectCategoryForGameObject(pObject, pObserver);
        if (bInclude != 1 || nCategory == 0xff) {
            continue;
        }
        if (nCapacity == nCount) {
            CSWSPlayerLUOSortedObjectList* pNew =
                new CSWSPlayerLUOSortedObjectList[nCapacity + 32];
            if (nCount > 0) {
                for (int i = 0; i < nCount; i++) {
                    pNew[i].m_fDistance = pList[i].m_fDistance;
                    pNew[i].m_nCategory = pList[i].m_nCategory;
                    pNew[i].m_oidObject = pList[i].m_oidObject;
                }
                delete[] pList;
            }
            pList = pNew;
            nCapacity += 32;
        }
        int j = nCount - 1;
        while (j >= 0) {
            if (nCategory < pList[j].m_nCategory ||
                (nCategory == pList[j].m_nCategory &&
                 fDistanceSq < pList[j].m_fDistance * pList[j].m_fDistance)) {
                pList[j + 1].m_fDistance = pList[j].m_fDistance;
                pList[j + 1].m_nCategory = pList[j].m_nCategory;
                pList[j + 1].m_oidObject = pList[j].m_oidObject;
                j--;
            }
            else {
                break;
            }
        }
        j++;
        pList[j].m_nCategory = nCategory;
        pList[j].m_fDistance = (float)sqrt(fDistanceSq);
        pList[j].m_oidObject = oidObject;
        nCount++;
    } while (pArea->GetNextObjectIdInArea(&oidObject) == 1);
done:
    *pCount = nCount;
    return pList;
}
