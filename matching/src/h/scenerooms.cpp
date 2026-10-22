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
    unsigned char m_nFlags50;           // +0x50 (bit 2: use the position)
    unsigned char m_nFlags51;           // +0x51 (bit 0: all rooms)

    unsigned char UsePosition() { return (unsigned char)((m_nFlags50 >> 2) & 1); }
    unsigned char InAllRooms() { return (unsigned char)(m_nFlags51 & 1); }
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

// NONMATCH 17: the original tests the +0x50 / +0x51 flag bits with
// mov al / shr / test al,1 (ours: test byte ptr) and passes the division
// result pointer straight to the rotation.
// FUNCTION: SWKOTOR 0x0046CE70
void AddObjectToRooms(Scene* pScene, SceneObject* pObject)
{
    int nAdded = 0;
    Vector vPos;
    if (pObject->m_pFlags && pObject->m_pFlags->UsePosition())
        vPos = pObject->m_vPosition;
    else {
        Vector vCenter = (pObject->m_vBoxMax + pObject->m_vBoxMin) / 2.0f;
        vPos = pObject->m_qOrientation * vCenter + pObject->m_vPosition;
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

