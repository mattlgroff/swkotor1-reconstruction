// Game-object update control; local minimal layouts preserve the server ABI.
#include "b/sws_message.h"

#include "shared/globals.h"

class CSWSArea;

#include "shared/game_object.h"

class CSWSObject : public CGameObject {
public:
    CSWSArea* GetArea();                                      // 0x004CB120
    char pad_0xc[0x1ec];
    int m_bVisible;                                           // 0x1f8
};

class CSWSModule {
public:
    CSWSArea* GetArea();                                      // 0x004C30F0
};

#include "shared/sws_player_luo_sorted_object_list.h"

// FUNCTION: SWKOTOR 0x00578060
// NONMATCH: stack-slot reuse and flag-register allocation differ (14 instructions).
void CSWSMessage::ComputeGameObjectUpdateForObject(CSWSPlayer* pPlayer,
    CSWSObject* pObserver, CGameObjectArray* pArray, unsigned long oidObject)
{
    CSWSObject* pObject;
    {
        CGameObject* pGameObject;
        if (pArray->GetGameObject(oidObject, &pGameObject) != GAME_OBJECT_VALID) {
            return;
        }
        if (pGameObject == NULL) {
            return;
        }
        pObject = pGameObject->AsSWSObject();
    }
    int bVisible = TestObjectVisible(pObject, pObserver);
    pObject->m_bVisible = bVisible;
    if (bVisible == 1) {
        CLastUpdateObject* pLast = NULL;
        unsigned long nFlags;
        unsigned long nAppearanceFlags;
        TestObjectUpdateDifferences(pPlayer, pObject, &pLast, &nFlags, &nAppearanceFlags);
        if (nAppearanceFlags || nFlags) {
            WriteGameObjUpdate_UpdateObject(pPlayer, pObject, pLast, nFlags, nAppearanceFlags);
        }
        StoreValuesInLastUpdateObject(pPlayer, pLast, pObject, nFlags, nAppearanceFlags);
    } else {
        DeleteLastUpdateObjectsForObject(pPlayer, pObject->m_idSelf);
    }
}

// FUNCTION: SWKOTOR 0x00578130
// NONMATCH: two area-pointer null checks use TEST rather than CMP against the zero register.
int CSWSMessage::ComputeGameObjectUpdateForCategory(unsigned long nCategory,
    unsigned long nLimit, CSWSPlayer* pPlayer, CSWSObject* pObserver,
    CGameObjectArray* pArray, CSWSPlayerLUOSortedObjectList* pObjects, int nCount)
{
    CGameObject* pObject = NULL;
    if (pObserver == NULL || pObserver->GetArea() == NULL) {
        if (pObserver->AsSWSCreature() == NULL) {
            return 0;
        }
        CSWSModule* pModule = g_pAppManager->GetServerExoApp()->GetModule();
        if (pModule == NULL || pModule->GetArea() == NULL) {
            return 0;
        }
    }
    for (int i = 0; i < nCount; i++) {
        if (pObjects[i].m_nCategory == nCategory) {
            unsigned long oidObject = pObjects[i].m_oidObject;
            if (pArray->GetGameObject(oidObject, &pObject) == GAME_OBJECT_VALID) {
                ComputeGameObjectUpdateForObject(pPlayer, pObserver, pArray, oidObject);
                if (PeekAtWriteMessageSize() >= nLimit) {
                    return 1;
                }
            }
        }
    }
    return 0;
}
