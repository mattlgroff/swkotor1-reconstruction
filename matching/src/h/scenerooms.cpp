// room assignment for emitters and scene objects (0x0046CD70,
// 0x0046CE70). Own TU so nodedata::AddEmitter / AddObject stay calls.

#include "h/scenebsp.h"

class Scene {
public:
    char pad_0x0[0xc8];
    CExoArrayList<BSPNode*> m_lRooms;   // +0xc8
};

static inline int RoomContains(BSPNode* pRoom, const Vector& v)
{
    return v.x >= pRoom->m_vMin.x && v.y >= pRoom->m_vMin.y
        && v.x <= pRoom->m_vMax.x && v.y <= pRoom->m_vMax.y;
}

class EmitterNode {
public:
    char pad_0x0[0x20];
    CExoArrayList<nodedata*> m_lRooms;  // +0x20
    char pad_0x2c[0x4c];
    Vector m_vPosition;             // +0x78
    char pad_0x84[0x10];
    Vector m_vExtent;               // +0x94
};

class Emitter {
public:
    char pad_0x0[0x40];
    EmitterNode* m_pNode;           // +0x40
};

int EmitterIsLocal(Vector* pPos, Vector* pExtent);     // 0x004AA980

// NONMATCH 8: register choice only (eax/edx swaps).
// FUNCTION: SWKOTOR 0x0046CD70
void AddEmitterToRooms(Scene* pScene, Emitter* pEmitter)
{
    if (!pEmitter->m_pNode)
        return;
    if (EmitterIsLocal(&pEmitter->m_pNode->m_vPosition, &pEmitter->m_pNode->m_vExtent)
        && pEmitter->m_pNode->m_lRooms.num > 0) {
        pEmitter->m_pNode->m_lRooms[0]->AddEmitter(pEmitter);
        return;
    }
    Vector* pPos = &pEmitter->m_pNode->m_vPosition;
    int nAdded = 0;
    int i;
    for (i = 0; i < pScene->m_lRooms.num; i++) {
        BSPNode* pRoom = pScene->m_lRooms[i];
        if (RoomContains(pRoom, *pPos)) {
            pRoom->m_pData->AddEmitter(pEmitter);
            nAdded++;
        }
    }
    if (nAdded)
        return;
    for (i = 0; i < pScene->m_lRooms.num; i++)
        pScene->m_lRooms[i]->m_pData->AddEmitter(pEmitter);
}

struct ObjectFlags {
    char pad_0x0[0x50];
    unsigned short m_b0 : 1;            // +0x50
    unsigned short m_b1 : 1;
    unsigned short m_bUsePosition : 1;  // +0x50 bit 2
    unsigned short m_b3 : 5;
    unsigned short m_bAllRooms : 1;     // +0x51 bit 0

    unsigned char UsePosition() { return m_bUsePosition; }
    bool InAllRooms() { return m_bAllRooms; }
};

class SceneObject {
public:
    char pad_0x0[0x58];
    ObjectFlags* m_pFlags;          // +0x58
    char pad_0x5c[0x1c];
    Vector m_vPosition;             // +0x78
    Quaternion m_qOrientation;      // +0x84
    char pad_0x94[0x8c];
    Vector m_vBoxMin;               // +0x120
    Vector m_vBoxMax;               // +0x12c
};

Vector operator/(const Vector& v, float f);     // 0x00437A90
extern int g_nLastObjectRoom;                   // 0x007FBF68

// NONMATCH 6: frame is 0xC larger (the original builds the division result
// in vPos's slot) and the +0x51 bit test is `test byte ptr` here.
// FUNCTION: SWKOTOR 0x0046CE70
void AddObjectToRooms(Scene* pScene, SceneObject* pObject)
{
    int nAdded = 0;
    Vector vPos;
    if (pObject->m_pFlags && pObject->m_pFlags->UsePosition())
        vPos = pObject->m_vPosition;
    else {
        vPos = pObject->m_qOrientation * ((pObject->m_vBoxMax + pObject->m_vBoxMin) / 2.0f)
             + pObject->m_vPosition;
    }
    int i;
    if (!pObject->m_pFlags || !pObject->m_pFlags->InAllRooms()) {
        for (i = 0; i < pScene->m_lRooms.num; i++) {
            BSPNode* pRoom = pScene->m_lRooms[i];
            if (RoomContains(pRoom, vPos)) {
                pRoom->m_pData->AddObject(pObject);
                nAdded++;
                g_nLastObjectRoom = i;
            }
        }
        if (nAdded)
            return;
    }
    for (i = 0; i < pScene->m_lRooms.num; i++)
        pScene->m_lRooms[i]->m_pData->m_lObjects.AddCopy(pObject);
    g_nLastObjectRoom = 999;
}

