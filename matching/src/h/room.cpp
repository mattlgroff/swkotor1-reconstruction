// CAurRoom and room visibility links (0x00452A60 .. 0x00454940).

#include <string.h>
#include "h/scene.h"

// FUNCTION: SWKOTOR 0x00452A60
CAurRoom::~CAurRoom()
{
    if (m_pModel)
        delete m_pModel;
    for (int i = 0; i < m_lObjects.num; i++) {
        if (m_lObjects[i])
            delete m_lObjects[i];
    }
}

// FUNCTION: SWKOTOR 0x004548A0
CAurRoom::CAurRoom(char* szName)
    : BSPNodeBase(VectorD(0.0f, 0.0f, 0.0f), 0)
{
    strcpy(m_szName, szName);
    m_pModel = 0;
}

void SceneToggleTilePart(Scene* pScene, ScenePart* pPart);     // 0x0046CA30

// FUNCTION: SWKOTOR 0x00455370
void Scene::SetSingleTileFlame(FlameGob* pFlame, FlameGob** ppCurrent, FlameData* pData)
{
    CExoArrayList<ScenePart*> lParts;
    if (*ppCurrent == pFlame)
        return;
    if (*ppCurrent) {
        (*ppCurrent)->GetParts(&lParts);
        for (int i = 0; i < lParts.num; i++)
            SceneToggleTilePart(this, lParts[i]);
        if (*ppCurrent) {
            delete *ppCurrent;
            *ppCurrent = 0;
        }
    }
    if (pFlame && pData) {
        pFlame->m_vPosition = pData->m_vPosition;
        pFlame->m_qOrientation = pData->m_qOrientation;
        pFlame->AttachToScene(this);
        pFlame->GetParts(&lParts);
        for (int i = 0; i < lParts.num; i++)
            SceneToggleTilePart(this, lParts[i]);
        *ppCurrent = pFlame;
    }
}

class TileRoom;

class TileMesh {
public:
    virtual void V00();
    virtual void V04();
    virtual void V08();
    virtual void V0C();
    virtual void V10();
    virtual void V14();
    virtual void V18();
    virtual void V1C();
    virtual void V20();
    virtual void V24();
    virtual void V28();
    virtual void V2C();
    virtual int HasBBox();                      // +0x30
    virtual void V34();
    virtual void V38();
    virtual void V3C();
    virtual void V40();
    virtual void V44();
    virtual int IsDiscardable();                // +0x48
    char pad_0x4[0x7c];
    TileRoom* m_pRoom;                          // +0x80
};

class TileLight {
public:
    char pad_0x0[0x1ec];
    TileRoom* m_pRoom;                          // +0x1ec
};

class TilePart {
public:
    virtual ~TilePart();
    virtual void V04();
    virtual void V08();
    virtual void V0C();
    virtual void V10();
    virtual void V14();
    virtual void V18();
    virtual void V1C();
    virtual void V20();
    virtual int IsKept();                       // +0x24
    virtual void V28();
    virtual TileMesh* GetMesh();                // +0x2c
    virtual void V30();
    virtual TileLight* GetLight();              // +0x34
};

class TileRoom {
public:
    char pad_0x0[0x18];
    CExoArrayList<TilePart*> m_lParts;          // +0x18
};

bool ProcessTilePart(TileRoom* pRoom, TilePart* pPart, int n);  // 0x004528B0
void ComputePartBBox(TileMesh* pMesh);                          // 0x00451DC0

// FUNCTION: SWKOTOR 0x00454B80
void ProcessStaticParts(CExoArrayList<TilePart*>* pList, int nUnused, TileRoom* pRoom,
    int n, int bDiscard)
{
    for (int i = 0; i < pList->num; i++) {
        TilePart* pPart = (*pList)[i];
        TileMesh* pMesh = pPart->GetMesh();
        bool bTile = ProcessTilePart(pRoom, pPart, n);
        if (pMesh) {
            pMesh->m_pRoom = pRoom;
            if (!pMesh->HasBBox())
                ComputePartBBox(pMesh);
            if (!bDiscard || !pMesh->IsDiscardable())
                goto keep;
        }
        else {
            TileLight* pLight = pPart->GetLight();
            if (!bDiscard) {
                if (bTile || pLight) {
                    if (pLight)
                        pLight->m_pRoom = pRoom;
                    goto keep;
                }
                if (pPart->IsKept())
                    goto keep;
            }
        }
        delete pPart;
        continue;
    keep:
        pRoom->m_lParts.Add(pPart);
    }
}

// FUNCTION: SWKOTOR 0x00454940
void Scene::SetVisibility(CAurRoom* pRoom, CAurRoom* pOther, int bVisible)
{
    if (bVisible) {
        if (!pRoom->m_lVisible.Contains(pOther)) {
            pRoom->m_lVisible.Add(pOther);
            pOther->m_lVisible.Add(pRoom);
        }
    }
    else if (pRoom->m_lVisible.Contains(pOther)) {
        pRoom->m_lVisible.Remove(pOther);
        pOther->m_lVisible.Remove(pRoom);
    }
}
