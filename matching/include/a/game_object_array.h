// CGameObjectArray (0x004D7D80-0x004D82A2), NWN layout.
// Self-contained declarations.
#ifndef A_GAME_OBJECT_ARRAY_H
#define A_GAME_OBJECT_ARRAY_H

#include <stddef.h>

typedef unsigned long OBJECT_ID;

class CGameObject {
public:
    virtual ~CGameObject();                     // slot 0 (scalar deleting dtor)
};

class CGameObjectArrayNode {
public:
    CGameObjectArrayNode()
    {
        m_nObjectID = 0;
        m_objObject = NULL;
        m_pNext = NULL;
    }
    OBJECT_ID m_nObjectID;                      // 0x00
    CGameObject* m_objObject;                   // 0x04
    CGameObjectArrayNode* m_pNext;              // 0x08
};

class CGameObjectArray {
public:
    CGameObjectArray(int bClientMode);
    ~CGameObjectArray();
    unsigned char AddObjectAtPos(OBJECT_ID nObjectID, CGameObject* pObject);
    unsigned char AddExternalObject(OBJECT_ID* pObjectID, CGameObject* pObject, int bUnused);
    unsigned char AddInternalObject(OBJECT_ID* pObjectID, CGameObject* pObject, int bCharacter);
    unsigned char Delete(OBJECT_ID nObjectID, CGameObject** ppObject);
    unsigned char GetGameObject(OBJECT_ID nObjectID, CGameObject** ppObject);
    unsigned char Delete(OBJECT_ID nObjectID);

    CGameObjectArrayNode** m_pArray;            // 0x00
    OBJECT_ID m_nNextObjectArrayID[2];          // 0x04 internal, 0x08 external
    OBJECT_ID m_nNextCharArrayID[2];            // 0x0c internal, 0x10 external
    int m_bClientMode;                          // 0x14
};

#endif
