// Game-object update control; local minimal layouts preserve the server ABI.
#include "b/sws_message.h"

extern const unsigned char GAME_OBJECT_VALID; // 0x0074666C

class CSWSArea;

class CGameObject {
public:
    virtual ~CGameObject();
    virtual void v01(); virtual void v02(); virtual void v03();
    virtual CSWSObject* AsSWSObject();                         // 0x10
    virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CSWSCreature* AsSWSCreature();                      // 0x30
    unsigned long m_idSelf;                                   // 0x04
};

class CSWSObject : public CGameObject {
public:
    CSWSArea* GetArea();                                      // 0x004CB120
    char pad_0x8[0x1f8 - 0x8];
    int m_bVisible;                                           // 0x1f8
};

class CSWSModule {
public:
    CSWSArea* GetArea();                                      // 0x004C30F0
};

class CSWSPlayerLUOSortedObjectList {
public:
    unsigned long m_oidObject;
    unsigned long m_nUnused;
    unsigned long m_nCategory;
};

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
