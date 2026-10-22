// SceneBSPRemovePart and the room-data teardown that follows it.

#include "h/scenebsp.h"

class RmModel {
public:
    void DelRef(RmModel* p);            // 0x0043E210
};

class RmTag {
public:
    RmModel* GetModel();                // 0x0043E1C0
};

class RmNode {
public:
    char pad_0x0[8];
    RmTag* m_pTag;                      // +0x08
};

class RmEmitter; class RmStatic; class RmDynamic; class RmLightHolder;
class RmLightA; class RmLightB; class RmWalkmesh; class RmEBox;

class RmPart {
public:
    virtual ~RmPart();                  // +0x00
    virtual void v04();
    virtual void v08();
    virtual void GetPosition(Vector* pPos, Quaternion* pOrient);   // +0x0c
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20();
    virtual RmEmitter* AsEmitter();        // +0x24
    virtual void v28();
    virtual RmStatic* AsStatic();         // +0x2c
    virtual void v30();
    virtual RmDynamic* AsDynamic();        // +0x34
    virtual void v38();
    virtual RmLightHolder* AsLightHolder();    // +0x3c
    virtual RmLightA* GetLight40();       // +0x40
    virtual RmLightB* GetLight44();       // +0x44
    virtual RmWalkmesh* AsWalkmesh();       // +0x48
    virtual void v4c(); virtual void v50();
    virtual RmEBox* AsEmitterBox();     // +0x54
    RmNode* m_pNode;                    // +0x04
    char pad_0x8[0x38];
    int m_bShared;                      // +0x40
};

class RmEmitter : public RmPart {};
class RmStatic : public RmPart {};
class RmDynamic : public RmPart {};
class RmLightHolder : public RmPart {};
class RmLightA : public RmPart {};
class RmLightB : public RmPart {};
class RmWalkmesh : public RmPart {};
class RmEBox : public RmPart {};

class RmBin {
public:
    ~RmBin();                           // 0x004A8350
};

class RmData {
public:
    CExoArrayList<RmEmitter*> m_lParts;  // +0x00
    CExoArrayList<RmPart*> m_lGobs;     // +0x0c
    CExoArrayList<RmStatic*> m_lStatic;  // +0x18
    CExoArrayList<RmDynamic*> m_lDynamic;  // +0x24
    CExoArrayList<RmLightA*> m_lLights30; // +0x30
    CExoArrayList<RmLightB*> m_lLights3C; // +0x3c
    CExoArrayList<RmPart*> m_lParts48;  // +0x48
    CExoArrayList<RmEBox*> m_lEmitterBoxes; // +0x54
    CExoArrayList<RmBin*> m_lBins;      // +0x60
    RmWalkmesh* m_pWalkmesh;             // +0x6c

    ~RmData();                          // 0x0046B1E0
};

class RmTile {
public:
    char pad_0x0[0x58];
    RmData* m_pData;                    // +0x58
};

class RmLightManager {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void RemoveEmitter(RmPart* p, int n);   // +0x10
};

class RmScene {
public:
    char pad_0x0[0x80];
    RmLightManager* m_pLightManager;    // +0x80
    char pad_0x84[0x1c];
    void* m_pTiles;                     // +0xa0
};

typedef int (*RmPredicate)(RmTile*, void*);
RmTile* FindTileIf(void* pTiles, Vector vPos, RmPredicate pfn, void* pKey);   // 0x004AD1A0

int NodeHasPart(RmTile* pNode, void* pPart);            // 0x0046B340
int NodeHasDynamic(RmTile* pNode, void* pPart);         // 0x0046B370
int NodeHasLight(RmTile* pNode, void* pPart);           // 0x0046B3A0
int NodeIsTile(RmTile* pNode, void* pPart);             // 0x0046A660
int NodeHasStatic(RmTile* pNode, void* pPart);          // 0x0046B400
int NodeHasEmitterBox(RmTile* pNode, void* pPart);      // 0x0046B430
int NodeHasPart48(RmTile* pNode, void* pPart);          // 0x0046B460

template <class T>
__forceinline void RemoveFrom(CExoArrayList<T>* pList, T p)
{
    if (pList->ContainsOutOfLine(p))
        pList->Remove(p);
}

// NONMATCH: 26 diffs, all in the emitter branch: the original keeps the
// list in ecx (count eax, cursor edx, hits esi) around the inline Contains;
// we pick eax / edx / esi / ecx and need a `mov ecx, eax` before Remove.
// FUNCTION: SWKOTOR 0x0046C510
void SceneBSPRemovePart(RmScene* pScene, RmPart* pPart)
{
    if (!pScene->m_pTiles)
        return;
    Vector vPos(0.0f, 0.0f, 0.0f);
    Quaternion qOrient(1.0f, 0.0f, 0.0f, 0.0f);
    pPart->GetPosition(&vPos, &qOrient);
    RmEmitter* pEmitter;
    RmDynamic* pDynamic;
    RmLightHolder* pHolder;
    RmWalkmesh* pWalkmesh;
    RmStatic* pStatic;
    RmEBox* pBox;
    if ((pEmitter = pPart->AsEmitter()) != 0) {
        RmData* pData = FindTileIf(pScene->m_pTiles, vPos, NodeHasPart, pEmitter)->m_pData;
        pData->m_lParts.RemoveIfPresent(pEmitter);
        pScene->m_pLightManager->RemoveEmitter(pEmitter, 0);
        return;
    }
    if ((pDynamic = pPart->AsDynamic()) != 0) {
        RmData* pData = FindTileIf(pScene->m_pTiles, vPos, NodeHasDynamic, pDynamic)->m_pData;
        RemoveFrom(&pData->m_lDynamic, pDynamic);
        return;
    }
    if ((pHolder = pPart->AsLightHolder()) != 0) {
        RmData* pData = FindTileIf(pScene->m_pTiles, vPos, NodeHasLight, pHolder)->m_pData;
        RmLightA* pLightA;
        RmLightB* pLightB;
        if ((pLightA = pHolder->GetLight40()) != 0)
            RemoveFrom(&pData->m_lLights30, pLightA);
        else if ((pLightB = pHolder->GetLight44()) != 0)
            RemoveFrom(&pData->m_lLights3C, pLightB);
        return;
    }
    if ((pWalkmesh = pPart->AsWalkmesh()) != 0) {
        RmData* pData = FindTileIf(pScene->m_pTiles, vPos, NodeIsTile, pWalkmesh)->m_pData;
        if (pData->m_pWalkmesh == pWalkmesh)
            pData->m_pWalkmesh = 0;
        return;
    }
    if ((pStatic = pPart->AsStatic()) != 0) {
        RmData* pData = FindTileIf(pScene->m_pTiles, vPos, NodeHasStatic, pStatic)->m_pData;
        RemoveFrom(&pData->m_lStatic, pStatic);
        return;
    }
    if ((pBox = pPart->AsEmitterBox()) != 0) {
        RmData* pData = FindTileIf(pScene->m_pTiles, vPos, NodeHasEmitterBox, pBox)->m_pData;
        RemoveFrom(&pData->m_lEmitterBoxes, pBox);
        return;
    }
    RmData* pData = FindTileIf(pScene->m_pTiles, vPos, NodeHasPart48, pPart)->m_pData;
    RemoveFrom(&pData->m_lParts48, pPart);
}

template <class T>
__forceinline void DeleteLights(CExoArrayList<T>& l)
{
    int i = l.num;
    while (i--) {
        RmPart* p = l[i];
        if (!p->m_bShared)
            delete l[i];
        l.num--;
    }
}

template <class T>
__forceinline void DeleteParts(CExoArrayList<T>& l)
{
    int i = l.num;
    while (i--) {
        RmPart* p = l[i];
        if (!p->m_bShared) {
            RmModel* pModel = p->m_pNode->m_pTag->GetModel();
            if (pModel)
                pModel->DelRef(pModel);
            delete l[i];
        }
        l.num--;
    }
}

// FUNCTION: SWKOTOR 0x0046C850
void SceneBSPDeleteRoomData(RmTile* pTile)
{
    RmData* pData = pTile->m_pData;
    DeleteLights(pData->m_lLights30);
    DeleteLights(pData->m_lLights3C);
    DeleteParts(pData->m_lParts);
    DeleteParts(pData->m_lStatic);
    DeleteParts(pData->m_lDynamic);
    DeleteParts(pData->m_lParts48);
    int i = pData->m_lBins.num;
    while (i--) {
        RmBin* pBin = pData->m_lBins[i];
        delete pBin;
        pData->m_lBins.num--;
    }
    pData->~RmData();
    operator delete(pData);
}
