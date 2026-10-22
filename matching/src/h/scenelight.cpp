// static light gathering for tiles and rooms (LightNodeTile,
// LightManager::CollectLightsForGivenRoom).

#include "h/scenebsp.h"

class LtModel {
public:
    char pad_0x0[0x7c];
    float m_fRadius;                    // +0x7c
    Vector m_vOffset;                   // +0x80
};

class LtNode {
public:
    LtModel* AsMesh();                  // 0x0043E400
};

class LtLight {
public:
    char pad_0x0[4];
    LtNode* m_pNode;                    // +0x04
    Vector m_vPosition;                 // +0x08
    Quaternion m_qOrientation;          // +0x14
};

class LtObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28();
    virtual LtLight* AsLight();         // +0x2c
};

class LtRoom {
public:
    char pad_0x0[0x4c];
    Vector m_vExtent;                   // +0x4c
    float m_fRadius;                    // +0x58
    char pad_0x5c[0x14];
    int m_bStatic;                      // +0x70
    char pad_0x74[0x20];
    Vector m_vCenter;                   // +0x94
};

class LtTileData {
public:
    char pad_0x0[0x18];
    CExoArrayList<LtObject*> m_lObjects;    // +0x18
};

class LtNodeData {
public:
    char pad_0x0[0x70];
    LtTileData* m_pTileData;            // +0x70
};

class LtTile {
public:
    char pad_0x0[0x58];
    LtNodeData* m_pData;                // +0x58
};

class LtRoomSet {
public:
    char pad_0x0[0x20];
    CExoArrayList<LtRoom*> m_lRooms;    // +0x20
};

class LtPart {
public:
    char pad_0x0[0x80];
    LtRoomSet* m_pRooms;                // +0x80
};

int VectorEquals(Vector* a, Vector* b);                 // 0x004AA980
float VectorMagnitude(Vector* pVector);                 // 0x004AB100
void LightPartTriMesh(LtLight* pLight, CExoArrayList<LtRoom*> lRooms, LtPart* pPart);  // 0x0046A9E0

// NONMATCH: 53 diffs, register allocation only: the original keeps the
// zero / room index in ebx, the in-reach count in ebp (Add with the size in a
// register) and spills pModel; we keep pModel in ebx and re-read the count.
// FUNCTION: SWKOTOR 0x0046DB60
void LightNodeTile(LtTile* pTile, LtPart* pPart)
{
    int i, j;
    LtNodeData* pData = pTile->m_pData;
    if (!pData->m_pTileData)
        return;
    CExoArrayList<LtRoom*> lInReach;
    CExoArrayList<LtRoom*> lRooms;
    lRooms = pPart->m_pRooms->m_lRooms;
    for (i = 0; i < pData->m_pTileData->m_lObjects.num; i++) {
        LtLight* pLight = pData->m_pTileData->m_lObjects[i]->AsLight();
        if (!pLight)
            continue;
        lInReach.num = 0;
        LtModel* pModel = pLight->m_pNode->AsMesh();
        Vector vPos = pLight->m_qOrientation * pModel->m_vOffset + pLight->m_vPosition;
        for (j = 0; j < lRooms.num; j++) {
            LtRoom* pRoom = lRooms[j];
            if (pRoom->m_bStatic)
                continue;
            Vector vZero(0.0f, 0.0f, 0.0f);
            if (VectorEquals(&pRoom->m_vExtent, &vZero))
                continue;
            Vector d = pRoom->m_vCenter - vPos;
            float fDist = VectorMagnitude(&d);
            if (fDist < pModel->m_fRadius + pRoom->m_fRadius)
                lInReach.AddB(pRoom);
        }
        LightPartTriMesh(pLight, lInReach, pPart);
    }
}

class LmLightInfo {
public:
    char pad_0x0[0x7c];
    int m_nStaticCount;                 // +0x7c
};

class LmLight {
public:
    char pad_0x0[4];
    LmLightInfo* m_pInfo;               // +0x04
    char pad_0x8[0x44];
    Vector m_vColor;                    // +0x4c
};

class LmRoomData {
public:
    CExoArrayList<LmLight*> m_lLights;  // +0x00
    char pad_0xc[0x74];
    CExoArrayList<LmLight*> m_lStatic;  // +0x80
};

class LmRoom {
public:
    char pad_0x0[0x58];
    LmRoomData* m_pData;                // +0x58
    CExoArrayList<LmRoom*> m_lVisible;  // +0x5c
};

class LmScene {
public:
    char pad_0x0[0xc8];
    CExoArrayList<LmRoom*> m_lRooms;    // +0xc8
};

extern LmScene* g_pScene;               // 0x007B92A0

class LightManager {
public:
    char pad_0x0[0x38];
    CExoArrayList<LmLight*> m_lColored;     // +0x38
    char pad_0x44[0x80];
    CExoArrayList<LmLight*> m_lStatic;      // +0xc4

    void CollectLightsForGivenRoom(LmRoom* pRoom);
};

// NONMATCH: 73 diffs, register allocation: the original keeps the zero in
// ebp (ours ebx), keeps lRooms' count in a register through the inline Add,
// and keeps the Contains count (edx, copied to ecx) for the AddCopy size test.
// FUNCTION: SWKOTOR 0x0046DE10
void LightManager::CollectLightsForGivenRoom(LmRoom* pRoom)
{
    m_lColored.num = 0;
    m_lStatic.num = 0;
    CExoArrayList<LmRoom*> lRooms;
    if (pRoom) {
        lRooms = pRoom->m_lVisible;
        lRooms.AddB(pRoom);
    } else {
        lRooms = g_pScene->m_lRooms;
    }
    for (int i = 0; i < lRooms.num; i++) {
        LmRoomData* pData = lRooms[i]->m_pData;
        for (int j = 0; j < pData->m_lLights.num; j++) {
            LmLight* pLight = pData->m_lLights[j];
            if (pLight->m_pInfo->m_nStaticCount > 0) {
                if (!m_lStatic.Contains(pLight))
                    m_lStatic.AddCopy(pLight);
            }
            if (pLight->m_vColor.x != 0.0f || pLight->m_vColor.y != 0.0f
                || pLight->m_vColor.z != 0.0f)
                m_lColored.AddCopy(pLight);
        }
        CExoArrayList<LmLight*> lStatic(pData->m_lStatic);
        for (int k = 0; k < lStatic.num; k++) {
            LmLight* pLight = lStatic[k];
            if (pLight->m_pInfo->m_nStaticCount > 0) {
                if (!m_lStatic.Contains(pLight))
                    m_lStatic.AddCopy(pLight);
            }
            if ((pLight->m_vColor.x != 0.0f || pLight->m_vColor.y != 0.0f
                 || pLight->m_vColor.z != 0.0f)
                && !m_lColored.Contains(pLight))
                m_lColored.AddCopy(pLight);
        }
    }
}
