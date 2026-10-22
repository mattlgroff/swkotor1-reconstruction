// SceneBSPAddPart 0x0046E290 (and the SafePointer helper layout it
// uses). Own TU: the part kinds are reached through one vtable here.

#include "h/scenebsp.h"

class DynLight;
class StaticLight;

class Model {
public:
    void Release();                     // 0x0043E240
};

class PartGob {
public:
    char pad_0x0[0x58];
    int m_nTileKey;                     // +0x58
};

class PartStatic {
public:
    char pad_0x0[0x4c];
    Vector m_vMin;                      // +0x4c
    Vector m_vMax;                      // +0x58
};

class PartLight {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual DynLight* GetDynamicLight();    // +0x40
    virtual StaticLight* GetStaticLight();  // +0x44
    char pad_0x4[0x50];
    Vector m_vMin;                      // +0x54
    Vector m_vMax;                      // +0x60
};

class AurAnimMesh {
public:
    bool IsAnimated();                  // 0x0048D780
};

class PartMdlNode {
public:
    AurAnimMesh* AsAnimMesh();          // 0x0043E3C0
};

class Wind;

class PartDynamic {
public:
    void* vtable;
    PartMdlNode* m_pNode;               // +0x04
    char pad_0x8[0xc8];
    int m_nTileKey;                     // +0xd0
    char pad_0xd4[0xf0];
    SafePointer<Wind> m_pWind;          // +0x1c4
};

class PartEmitter {
public:
    char pad_0x0[0x5c];
    Vector m_vMin;                      // +0x5c
    Vector m_vMax;                      // +0x68
};

class PartObserver {
public:
    virtual void v00(); virtual void v04(); virtual void v08();
    virtual void OnAdd(PartGob* p);     // +0x0c
};

class ScenePart {
public:
    virtual void v00(); virtual void v04(); virtual void v08();
    virtual void GetPosition(Vector* pPos, Quaternion* pOrient);   // +0x0c
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20();
    virtual PartGob* AsGob();           // +0x24
    virtual void v28();
    virtual PartStatic* AsStatic();     // +0x2c
    virtual void v30();
    virtual PartDynamic* AsDynamic();   // +0x34
    virtual void v38();
    virtual PartLight* AsLight();       // +0x3c
    virtual void v40(); virtual void v44();
    virtual void* AsTileSlot();         // +0x48
    virtual void v4c(); virtual void v50();
    virtual PartEmitter* AsEmitter();   // +0x54
};

class Scene {
public:
    char pad_0x0[0x80];
    PartObserver* m_pObserver;          // +0x80
    Wind* m_pWind;                      // +0x84
    char pad_0x88[0x18];
    void* m_pTiles;                     // +0xa0
};

// The room lists hold the concrete part types; each Add is its own instance.
template <class T, class U>
inline CExoArrayList<T*>* ListOf(CExoArrayList<U*>* pList, T*)
{
    return (CExoArrayList<T*>*)pList;
}

// NONMATCH 12: the first branch keeps the list in eax, the original in ecx.
// FUNCTION: SWKOTOR 0x0046E290
int SceneBSPAddPart(Scene* pScene, ScenePart* pPart, Model* pModel)
{
    if (!pScene->m_pTiles)
        return 0;
    Vector vPos(0.0f, 0.0f, 0.0f);
    Quaternion qOrient(1.0f, 0.0f, 0.0f, 0.0f);
    pPart->GetPosition(&vPos, &qOrient);
    if (PartGob* pGob = pPart->AsGob()) {
        CExoArrayList<PartGob*>* pList = ListOf(
            &FindTile(pScene->m_pTiles, &vPos, pGob->m_nTileKey)->m_pData->m_lParts, pGob);
        int nFound = 0;
        for (int i = 0; i < pList->num; i++)
            if (pList->element[i] == pGob)
                nFound++;
        if (!nFound)
            pList->AddOutOfLine(pGob);
        pScene->m_pObserver->OnAdd(pGob);
    } else if (void* pSlot = pPart->AsTileSlot()) {
        FindTile(pScene->m_pTiles, &vPos, 0)->m_pData->m_p6C = pSlot;
    } else if (PartStatic* pStatic = pPart->AsStatic()) {
        Vector vMin = pStatic->m_vMin;
        Vector vMax = pStatic->m_vMax;
        ListOf(&FindTileBox(pScene->m_pTiles, &vMin, &vMax)->m_pData->m_lStatic, pStatic)
            ->AddOutOfLine(pStatic);
    } else if (PartLight* pLight = pPart->AsLight()) {
        Vector vMin = pLight->m_vMin;
        Vector vMax = pLight->m_vMax;
        nodedata* pData = FindTileBox(pScene->m_pTiles, &vMin, &vMax)->m_pData;
        if (pLight->GetDynamicLight())
            ListOf(&pData->m_lLights30, (DynLight*)0)->AddOutOfLine(pLight->GetDynamicLight());
        else
            ListOf(&pData->m_lLights3C, (StaticLight*)0)->AddOutOfLine(pLight->GetStaticLight());
    } else if (PartDynamic* pDynamic = pPart->AsDynamic()) {
        nodedata* pData = FindTile(pScene->m_pTiles, &vPos, pDynamic->m_nTileKey)->m_pData;
        if (!ListOf(&pData->m_lDynamic, pDynamic)->ContainsOutOfLine(pDynamic))
            ListOf(&pData->m_lDynamic, pDynamic)->AddOutOfLine(pDynamic);
        if (pDynamic->m_pNode->AsAnimMesh()->IsAnimated() && pScene->m_pWind) {
            pDynamic->m_pWind = pScene->m_pWind;
            pScene->m_pWind->AddPart(pDynamic);
        }
    } else if (PartEmitter* pEmitter = pPart->AsEmitter()) {
        Vector vMin = pEmitter->m_vMin;
        Vector vMax = pEmitter->m_vMax;
        nodedata* pData = FindTileBox(pScene->m_pTiles, &vMin, &vMax)->m_pData;
        if (!ListOf(&pData->m_lEmitterBoxes, pEmitter)->ContainsOutOfLine(pEmitter))
            ListOf(&pData->m_lEmitterBoxes, pEmitter)->AddOutOfLine(pEmitter);
    } else {
        nodedata* pData = FindTile(pScene->m_pTiles, &vPos, 0)->m_pData;
        if (!ListOf(&pData->m_lParts48, pPart)->ContainsOutOfLine(pPart))
            ListOf(&pData->m_lParts48, pPart)->AddOutOfLine(pPart);
    }
    if (pModel)
        pModel->Release();
    return 1;
}
