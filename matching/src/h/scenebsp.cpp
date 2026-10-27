// SceneBSP room / node helpers (0x0046A610-0x0046E290).

#include "h/scenebsp.h"

#define H_V4(a) virtual void a##0(); virtual void a##1(); \
    virtual void a##2(); virtual void a##3();
#define H_V16(a) H_V4(a##0) H_V4(a##1) H_V4(a##2) H_V4(a##3)

class Scene {
public:
    char pad_0x0[0xa0];
    void* m_pTiles;                     // +0xa0
    char pad_0xa4[0x24];
    CExoArrayList<BSPNode*> m_lRooms;   // +0xc8
    BSPNode* m_pCurrentRoom;            // +0xd4
    short m_nRoomMode;                  // +0xd8
};

class PartBox {
public:
    char pad_0x0[0x4c];
    Vector m_vMin;          // +0x4c
    Vector m_vMax;          // +0x58
};

class LightBox {
public:
    char pad_0x0[0x54];
    Vector m_vMin;          // +0x54
    Vector m_vMax;          // +0x60
};

class EmitterBox {
public:
    char pad_0x0[0x5c];
    Vector m_vMin;          // +0x5c
    Vector m_vMax;          // +0x68
};

class CAurObject {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void GetPosition(Vector* pPos, Quaternion* pOrient);   // +0x0c
    char pad_0x4[0xcc];
    float m_fRadius;        // +0xd0
};

class Bin {
public:
    char pad_0x0[8];
    float m_fRadius;        // +0x08
    char pad_0xc[0x20];
    Vector m_vCenter;       // +0x2c

    void Activate();               // 0x004A9630
    void Deactivate();              // 0x004A8460
};

class GobPlacement {
public:
    char pad_0x0[0x80];
    int m_nTileKey;                 // +0x80
};

class Gob {
public:
    H_V16(va) H_V16(vb) H_V16(vc) H_V16(vd) H_V16(ve) H_V16(vf)
    virtual bool IsAlwaysVisible(); // +0x180
    char pad_0x4[0x54];
    GobPlacement* m_pPlacement;     // +0x58
    char pad_0x5c[0x1c];
    Vector m_vPosition;             // +0x78
    Quaternion m_qOrientation;      // +0x84
    char pad_0x94[0xa4];
    float m_fRadius;                // +0x138
    Vector m_vCenterOffset;         // +0x13c
};

class AurMesh {
public:
    char pad_0x0[0x186];
    unsigned char m_bTransparent;   // +0x186
};

class MdlNode {
public:
    AurMesh* AsMesh();              // 0x0043E400
};

class Material {
public:
    int GetBucket();                // 0x0047AF00
};

class Shader {
public:
    H_V16(va) H_V4(vb) H_V4(vc)
    virtual void v60();
    virtual void v64();
    virtual void SetFrame(int n);   // +0x68
};

struct TexHolder {
    void* m_pName;                  // +0x00
};

class TexInfo {
public:
    char pad_0x0[0x44];
    TexHolder* m_pHolder;           // +0x44
};

class Part {
public:
    H_V4(va) H_V4(vb)
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual TexInfo* GetTexInfo();  // +0x2c
    H_V4(vc)
    virtual void v40();
    virtual void v44();
    virtual void v48();
    virtual Shader* GetShader();    // +0x4c
    virtual void v50();
    virtual EmitterBox* GetBounds();    // +0x54
    MdlNode* m_pNode;              // +0x04
    char pad_0x8[0x3c];
    Material* m_pMaterial;          // +0x44
};

extern CExoArrayList<Bin*> g_lActiveBins;       // 0x007FBF9C
extern CExoArrayList<Bin*> g_lPrevActiveBins;   // 0x007FBFFC
extern int g_nPartPlane;                        // 0x00827F4C
extern int g_nLightPlane;                       // 0x00827F50
extern int g_nEmitterPlane;                     // 0x00827F54

class TileLink {
public:
    char pad_0x0[0x6c];
    Part* m_pPart;                      // +0x6c
};

void SceneBSPGetObjectTextures(Part* pPart, CExoArrayList<void*>* pList);  // 0x0046C060

class TileData {
public:
    char pad_0x0[0x18];
    CExoArrayList<Part*> m_lObjects;    // +0x18
    TileLink* m_pNext;                  // +0x24

    ~TileData();                        // 0x00444E20
};

static inline TileData* GetTileData(Scene* pScene, void* pKey)
{
    if (!pScene->m_pTiles)
        return 0;
    return FindTile(pScene->m_pTiles, pKey, 0)->m_pData->m_pTileData;
}

BSPNode* FindTileIf(void* pTiles, Vector vPos, int (*pfn)(BSPNode*, Gob*), Gob* pKey);  // 0x004AD1A0
extern int g_bNoGobCulling;                     // 0x007FBF20
extern int g_nShaderFrame;                      // 0x0078E574
extern int g_bUseVisibility;                    // 0x0078E5C8
extern CExoArrayList<Part*> g_lTransparentParts;    // 0x00827F40
extern CExoArrayList<Part*> g_aMeshBuckets[];       // 0x008194E0
extern CExoArrayList<Part*> g_aBucketsA[];          // 0x007FC008
extern CExoArrayList<Part*> g_aBucketsC[];          // 0x0080AA80
extern CExoArrayList<Part*> g_lAllMeshParts;        // 0x007FBFB4
extern CExoArrayList<Part*> g_l7FBFCC, g_l7FBFC0, g_l7FBFF0, g_l7FBF90, g_l7FBF78;
extern CExoArrayList<Part*> g_l7FBF84, g_l7FBF6C, g_l80AA74, g_l7FBFE4, g_l80AA68,
    g_l7FBFA8, g_l7FBFD8;
extern int g_n7FBF28, g_n7FBF2C, g_n7FBF30;
extern CExoArrayList<void*> g_lModels;              // 0x007A8A64
int GetNumBuckets();                                // 0x0041FEB0

// FUNCTION: SWKOTOR 0x0046A610
void AurClearActiveBins()
{
    g_lActiveBins.num = 0;
    g_lPrevActiveBins.num = 0;
}

// FUNCTION: SWKOTOR 0x0046A660
int NodeIsTile(BSPNode* pNode, void* pTile)
{
    if (!pNode)
        return 0;
    return pNode->m_pData->m_p6C == pTile;
}

// FUNCTION: SWKOTOR 0x0046A680
TileData* SceneBSPGetTileData(Scene* pScene, void* pKey)
{
    return GetTileData(pScene, pKey);
}

// FUNCTION: SWKOTOR 0x0046A6B0 SYMBOL: ?PartOutside@@YAHPAVPartBox@@PAV?$CExoArrayList@UPlane@@@@@Z
int PartOutside(PartBox* pPart, CExoArrayList<Plane>* pPlanes)
{
    for (int i = 0; i < pPlanes->num; i++) {
        if (BoxPlaneSide(&pPart->m_vMin, &pPart->m_vMax,
                         &pPlanes->element[(g_nPartPlane + i) % pPlanes->num]) > 0.0f) {
            g_nPartPlane = (g_nPartPlane + i) % pPlanes->num;
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0046A730 SYMBOL: ?PartOutside@@YAHPAVCAurObject@@PAV?$CExoArrayList@UPlane@@@@@Z
int PartOutside(CAurObject* pObject, CExoArrayList<Plane>* pPlanes)
{
    Vector vPos(0.0f, 0.0f, 0.0f);
    Quaternion qOrient(1.0f, 0.0f, 0.0f, 0.0f);
    pObject->GetPosition(&vPos, &qOrient);
    for (int i = 0; i < pPlanes->num; i++) {
        if (SpherePlaneSide(&vPos, pObject->m_fRadius, &pPlanes->element[i]) > 0.0f)
            return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0046A7E0 SYMBOL: ?PartOutside@@YAHPAVLightBox@@PAV?$CExoArrayList@UPlane@@@@@Z
int PartOutside(LightBox* pPart, CExoArrayList<Plane>* pPlanes)
{
    for (int i = 0; i < pPlanes->num; i++) {
        if (BoxPlaneSide(&pPart->m_vMin, &pPart->m_vMax,
                         &pPlanes->element[(g_nLightPlane + i) % pPlanes->num]) > 0.0f) {
            g_nLightPlane = i;
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0046A850 SYMBOL: ?PartOutside@@YAHPAVEmitterBox@@PAV?$CExoArrayList@UPlane@@@@@Z
int PartOutside(EmitterBox* pPart, CExoArrayList<Plane>* pPlanes)
{
    for (int i = 0; i < pPlanes->num; i++) {
        if (BoxPlaneSide(&pPart->m_vMin, &pPart->m_vMax,
                         &pPlanes->element[(g_nEmitterPlane + i) % pPlanes->num]) > 0.0f) {
            g_nEmitterPlane = i;
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0046A980
int BinOutside(Bin* pBin, CExoArrayList<Plane>* pPlanes)
{
    for (int i = 0; i < pPlanes->num; i++) {
        if (SpherePlaneSide(&pBin->m_vCenter, pBin->m_fRadius, &pPlanes->element[i]) > 0.0f)
            return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0046B340
int NodeHasPart(BSPNode* pNode, Part* pPart)
{
    if (!pNode)
        return 0;
    return pNode->m_pData->m_lParts.Contains(pPart);
}

// FUNCTION: SWKOTOR 0x0046B370
int NodeHasDynamic(BSPNode* pNode, Part* pPart)
{
    if (!pNode)
        return 0;
    return pNode->m_pData->m_lDynamic.Contains(pPart);
}

// FUNCTION: SWKOTOR 0x0046B3A0
int NodeHasLight(BSPNode* pNode, Part* pPart)
{
    if (!pNode)
        return 0;
    nodedata* pData = pNode->m_pData;
    if (pData->m_lLights30.Contains(pPart) || pData->m_lLights3C.Contains(pPart))
        return 1;
    return 0;
}

// FUNCTION: SWKOTOR 0x0046B400
int NodeHasStatic(BSPNode* pNode, Part* pPart)
{
    if (!pNode)
        return 0;
    return pNode->m_pData->m_lStatic.Contains(pPart);
}

// FUNCTION: SWKOTOR 0x0046B430
int NodeHasEmitterBox(BSPNode* pNode, EmitterBox* pBox)
{
    if (!pNode)
        return 0;
    return pNode->m_pData->m_lEmitterBoxes.Contains(pBox);
}

// FUNCTION: SWKOTOR 0x0046B460
int NodeHasPart48(BSPNode* pNode, Part* pPart)
{
    if (!pNode)
        return 0;
    return pNode->m_pData->m_lParts48.Contains(pPart);
}

// FUNCTION: SWKOTOR 0x0046B490
int NodeHasGob(BSPNode* pNode, Gob* pGob)
{
    if (!pNode)
        return 0;
    return pNode->m_pData->m_lGobs.Contains(pGob);
}

// FUNCTION: SWKOTOR 0x0046BD10 SYMBOL: ?AddUnique@?$CExoArrayList@PAVPart@@@@QAEXPAVPart@@@Z
template <class T>
void CExoArrayList<T>::AddUnique(T value)
{
    if (Contains(value))
        return;
    Add(value);
}

template void CExoArrayList<Part*>::AddUnique(Part* value);

// FUNCTION: SWKOTOR 0x0046BD70
void nodedata::AddObject(void* p)
{
    m_lObjects.Add(p);
}

// FUNCTION: SWKOTOR 0x0046BDB0
void nodedata::AddEmitter(void* p)
{
    m_lEmitters.Add(p);
}

// FUNCTION: SWKOTOR 0x0046A8C0
int GobOutside(Gob* pGob, CExoArrayList<Plane>* pPlanes)
{
    if (pGob->IsAlwaysVisible())
        return 1;
    for (int i = 0; i < pPlanes->num; i++) {
        if (!g_bNoGobCulling) {
            Vector vCenter = pGob->m_qOrientation * pGob->m_vCenterOffset + pGob->m_vPosition;
            if (SpherePlaneSide(&vCenter, pGob->m_fRadius, &pPlanes->element[i]) > 0.0f)
                return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0046B1E0
nodedata::~nodedata()
{
    delete m_pTileData;
}

static inline int ListHas(CExoArrayList<Bin*>& l, Bin* p)
{
    for (int i = 0; i < l.num; i++)
        if (l.element[i] == p)
            return 1;
    return 0;
}

// FUNCTION: SWKOTOR 0x0046BBC0
void AurProcessActiveBins()
{
    int i;
    for (i = 0; i < g_lActiveBins.num; i++)
        g_lActiveBins[i]->Activate();
    for (i = 0; i < g_lPrevActiveBins.num; i++) {
        Bin* pBin = g_lPrevActiveBins[i];
        if (!ListHas(g_lActiveBins, pBin))
            pBin->Deactivate();
    }
}

// FUNCTION: SWKOTOR 0x0046BC40
void ClearRoomObjectLists(Scene* pScene)
{
    for (int i = 0; i < pScene->m_lRooms.num; i++) {
        nodedata* pData = pScene->m_lRooms[i]->m_pData;
        pData->m_lList80.num = 0;
        pData->m_lObjects.num = 0;
        pData->m_lEmitters.num = 0;
    }
}

// FUNCTION: SWKOTOR 0x0046BC80 SYMBOL: ??4?$CExoArrayList@PAVBin@@@@QAEAAV0@ABV0@@Z
template CExoArrayList<Bin*>& CExoArrayList<Bin*>::operator=(const CExoArrayList<Bin*>& o);

// FUNCTION: SWKOTOR 0x0046BDF0
void AddPartToMeshBuckets(Part* pPart)
{
    Shader* pShader = pPart->GetShader();
    if (pShader)
        pShader->SetFrame(g_nShaderFrame);
    if (pPart->m_pNode->AsMesh()->m_bTransparent) {
        g_lTransparentParts.Add(pPart);
        return;
    }
    g_aMeshBuckets[pPart->m_pMaterial->GetBucket()].Add(pPart);
    g_lAllMeshParts.Add(pPart);
}

// FUNCTION: SWKOTOR 0x0046BEF0
void ClearBuckets()
{
    int n = GetNumBuckets();
    g_l7FBFCC.num = 0;
    g_l7FBFC0.num = 0;
    g_l7FBFF0.num = 0;
    g_l7FBF90.num = 0;
    g_l7FBF78.num = 0;
    int i;
    for (i = 0; i <= n; i++)
        g_aBucketsA[i].num = 0;
    for (i = 0; i <= n; i++) {
        g_aMeshBuckets[i].num = 0;
        g_aBucketsC[i].num = 0;
    }
    g_lTransparentParts.num = 0;
    g_l7FBF84.num = 0;
    g_l7FBF6C.num = 0;
    g_lAllMeshParts.num = 0;
    g_l80AA74.num = 0;
    g_l7FBFE4.num = 0;
    g_l80AA68.num = 0;
    g_l7FBFA8.num = 0;
    g_l7FBFD8.num = 0;
    g_lPrevActiveBins = g_lActiveBins;
    g_lActiveBins.num = 0;
    g_n7FBF28 = 0;
    g_n7FBF2C = 0;
    g_n7FBF30 = g_lModels.num;
}

// The lists use the sized constructor so the SEH frame is kept.
// FUNCTION: SWKOTOR 0x0046BFB0
nodedata::nodedata()
    : m_lParts(0), m_lGobs(0), m_lStatic(0), m_lDynamic(0), m_lLights30(0),
      m_lLights3C(0), m_lParts48(0), m_lEmitterBoxes(0), m_lParts60(0),
      m_lObjects(0), m_lList80(0), m_lEmitters(0)
{
    m_p6C = 0;
    m_pTileData = 0;
}

// FUNCTION: SWKOTOR 0x0046C380
void SceneBSPGetTileTextures(Scene* pScene, void* pKey, CExoArrayList<void*>* pList)
{
    TileData* pTile = GetTileData(pScene, pKey);
    pList->num = 0;
    if (!pTile)
        return;
    for (int i = 0; i < pTile->m_lObjects.num; i++) {
        TexInfo* pInfo = pTile->m_lObjects[i]->GetTexInfo();
        if (pInfo)
            pList->Add(pInfo->m_pHolder->m_pName);
    }
    TileLink* pNext = pTile->m_pNext;
    if (pNext)
        SceneBSPGetObjectTextures(pNext->m_pPart, pList);
}

// FUNCTION: SWKOTOR 0x0046C420
void SceneBSPAddGob(Scene* pScene, Gob* pGob)
{
    if (!pScene->m_pTiles)
        return;
    nodedata* pData = FindTile(pScene->m_pTiles, &pGob->m_vPosition,
                               pGob->m_pPlacement->m_nTileKey)->m_pData;
    if (pData->m_lGobs.Contains(pGob))
        return;
    pData->m_lGobs.Add(pGob);
}

// FUNCTION: SWKOTOR 0x0046C4A0
void SceneBSPRemoveGob(Scene* pScene, Gob* pGob)
{
    if (!pScene->m_pTiles)
        return;
    BSPNode* pNode = FindTileIf(pScene->m_pTiles, pGob->m_vPosition, NodeHasGob, pGob);
    if (!pNode)
        return;
    nodedata* pData = pNode->m_pData;
    if (pData->m_lGobs.Contains(pGob))
        pData->m_lGobs.Remove(pGob);
}

// FUNCTION: SWKOTOR 0x0046D070
void CollectActiveRooms(Scene* pScene, CExoArrayList<BSPNode*>* pList)
{
    if (!pScene->m_pCurrentRoom) {
        *pList = pScene->m_lRooms;
        return;
    }
    pList->num = 0;
    if (pScene->m_nRoomMode == 0 && g_bUseVisibility) {
        pList->AddOutOfLine(pScene->m_pCurrentRoom);
        for (int i = 0; i < pScene->m_pCurrentRoom->m_lVisible.num; i++)
            pList->Add(pScene->m_pCurrentRoom->m_lVisible[i]);
        return;
    }
    *pList = pScene->m_lRooms;
}

// FUNCTION: SWKOTOR 0x0046D130
void SceneAddPartToRooms(Scene* pScene, Part* pPart)
{
    EmitterBox* pBox = pPart->GetBounds();
    for (int i = 0; i < pScene->m_lRooms.num; i++) {
        BSPNode* pRoom = pScene->m_lRooms[i];
        if (pBox) {
            Vector vCenter = (pBox->m_vMax + pBox->m_vMin) * 0.5f;
            if (vCenter.x >= pRoom->m_vMin.x && vCenter.y >= pRoom->m_vMin.y
                && vCenter.x <= pRoom->m_vMax.x && vCenter.y <= pRoom->m_vMax.y)
                pRoom->m_pData->m_lEmitterBoxes.AddOutOfLine(pBox);
        }
    }
}

// FUNCTION: SWKOTOR 0x0046D1E0
void SceneRemovePartFromRooms(Scene* pScene, Part* pPart)
{
    EmitterBox* pBox = pPart->GetBounds();
    for (int i = 0; i < pScene->m_lRooms.num; i++) {
        nodedata* pData = pScene->m_lRooms[i]->m_pData;
        if (pBox && pData->m_lEmitterBoxes.Contains(pBox))
            pData->m_lEmitterBoxes.Remove(pBox);
    }
}

// FUNCTION: SWKOTOR 0x0046D250 SYMBOL: ??0?$CExoArrayList@H@@QAE@ABV0@@Z
template CExoArrayList<int>::CExoArrayList(const CExoArrayList<int>& o);

template <class T>
SafePointer<T>& SafePointer<T>::operator=(T* p)
{
    if (m_p)
        m_p->Remove(this);
    m_p = p;
    if (p)
        p->Add(this);
    return *this;
}

// FUNCTION: SWKOTOR 0x0046E240 SYMBOL: ??4?$SafePointer@VWind@@@@QAEAAV0@PAVWind@@@Z
template SafePointer<Wind>& SafePointer<Wind>::operator=(Wind* p);

class Light {
public:
    char pad_0x0[0x94];
    Vector m_vPosition;             // +0x94
};

static inline int RoomContains(BSPNode* pRoom, const Vector& v)
{
    return v.x >= pRoom->m_vMin.x && v.y >= pRoom->m_vMin.y
        && v.x <= pRoom->m_vMax.x && v.y <= pRoom->m_vMax.y;
}

// FUNCTION: SWKOTOR 0x0046CC00
void AddLightToRooms(Scene* pScene, Light* pLight)
{
    int nAdded = 0;
    int i;
    for (i = 0; i < pScene->m_lRooms.num; i++) {
        BSPNode* pRoom = pScene->m_lRooms[i];
        if (RoomContains(pRoom, pLight->m_vPosition)) {
            pRoom->m_pData->m_lList80.Add(pLight);
            nAdded++;
        }
    }
    if (nAdded)
        return;
    for (i = 0; i < pScene->m_lRooms.num; i++)
        pScene->m_lRooms[i]->m_pData->m_lList80.AddCopy(pLight);
}

class LightNode {
public:
    char pad_0x0[0x68];
    Scene* m_pScene;                // +0x68
};

void UpdateStaticLighting(Scene* pScene, Part* pPart);     // 0x0046CA30

// FUNCTION: SWKOTOR 0x0046DDF0
void UpdateStaticLightingCallback(LightNode* pNode, void* pUnused, Part* pPart)
{
    UpdateStaticLighting(pNode->m_pScene, pPart);
}

class Model;

class ModelHolder {
public:
    char pad_0x0[8];
    MdlNode* m_pModelNode;          // +0x08
};

class MdlNodeModel {
public:
    Model* AsModel();               // 0x0043E1C0
};

class TilePart {
public:
    void* vtable;
    ModelHolder* m_pHolder;         // +0x04
};

class TileParts {
public:
    char pad_0x0[0x18];
    CExoArrayList<TilePart*> m_lParts;  // +0x18
};

int SceneBSPAddPart(Scene* pScene, TilePart* pPart, Model* pModel);    // 0x0046E290

// FUNCTION: SWKOTOR 0x0046ED00
void SceneBSPAddTileData(Scene* pScene, TileParts* pTileData, int nTile)
{
    if (!pScene->m_pTiles)
        return;
    nodedata* pData = FindTile(pScene->m_pTiles, &nTile, 0)->m_pData;
    pData->m_pTileData = (TileData*)pTileData;
    for (int i = 0; i < pTileData->m_lParts.num; i++) {
        TilePart* pPart = ((TileParts*)pData->m_pTileData)->m_lParts[i];
        SceneBSPAddPart(pScene, pPart,
                        ((MdlNodeModel*)pPart->m_pHolder->m_pModelNode)->AsModel());
    }
}

class EnvMapTarget {
public:
    char pad_0x0[0x94];
    int m_n94;                      // +0x94

    void SetValue94(int n);
};

// FUNCTION: SWKOTOR 0x0046EE30
void EnvMapTarget::SetValue94(int n)
{
    m_n94 = n;
}

class PartMesh {
public:
    char pad_0x0[0xa4];
    void* m_pA4;                    // +0xa4
    char pad_0xa8[0xd8];
    unsigned short m_nFaces;        // +0x180
    char pad_0x182[7];
    unsigned char m_bShadow;        // +0x189
};

class PartRender {
public:
    void* vtable;
    PartMesh* m_pMesh;              // +0x04
    char pad_0x8[0x7c];
    float m_fAlpha;                 // +0x84
};

extern int g_bShadowsOnly;          // 0x0078E60C
extern int g_bSkipA4;               // 0x00827FCC

// NONMATCH 6: the last test folds to setne here; the original branches.
// FUNCTION: SWKOTOR 0x0046EE40
int PartIsRenderable(PartRender* pPart)
{
    PartMesh* pMesh = pPart->m_pMesh;
    if (pPart->m_fAlpha <= 0.001f)
        return 0;
    if (g_bShadowsOnly && !pMesh->m_bShadow)
        return 0;
    if (g_bSkipA4 && pMesh->m_pA4)
        return 0;
    if (pMesh->m_nFaces == 0)
        return 0;
    return 1;
}

// FUNCTION: SWKOTOR 0x0046EEA0
Vector VectorLerp(const Vector& a, const Vector& b, float t)
{
    return a * (1.0f - t) + b * t;
}

class LightRef {
public:
    void Update();                  // 0x004015A0
};

class HitTarget {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18();
    virtual int HitTest(Vector vStart, Vector vEnd, Vector* pHit, int nMask, float fScale);   // +0x1c
};

int BoxLineIntersect(Vector* pStart, Vector* pEnd, Vector* pMin, Vector* pMax);   // 0x004AA610
Vector* LinePlaneIntersect(Vector* pOut, Vector* pNormal, float fDist,
                           Vector* pStart, Vector* pEnd);                       // 0x004AB250

extern int g_nHitTests;             // 0x007FBF58
extern int g_nHitMask;              // 0x007FBF48
extern int g_bHitStatic;            // 0x007FBF4C
extern int g_bHitWalkmesh;          // 0x0078E5A8
extern int g_bHitGround;            // 0x007FBF50
extern int g_bHitGroundPlane;       // 0x007FBF54

// NONMATCH: 24 diffs. The original keeps bHit's zero and the null test in
// ebp, the loop counter (`cmp [edi+0x58], ebp`); we use edx for the zero.
// FUNCTION: SWKOTOR 0x0046B4C0
int HitPartOnBSPNode(BSPNode* pNode, Vector vStart, Vector vEnd, Vector* pHit)
{
    int i;
    g_nHitTests++;
    nodedata* pData = pNode->m_pData;
    i = 0;
    int bHit = 0;
    if (!pData)
        return 0;
    for (; i < pData->m_lEmitterBoxes.num; i++) {
        EmitterBox* pBox = pData->m_lEmitterBoxes[i];
        if (BoxLineIntersect(&vStart, &vEnd, &pBox->m_vMin, &pBox->m_vMax)
            && ((HitTarget*)pBox)->HitTest(vStart, vEnd, &vEnd, g_nHitMask, 1.0f)) {
            bHit = 1;
            if (pHit)
                *pHit = vEnd;
        }
    }
    if (g_bHitStatic) {
        for (i = 0; i < pData->m_lStatic.num; i++) {
            if (BoxLineIntersect(&vStart, &vEnd, &((PartBox*)pData->m_lStatic[i])->m_vMin,
                                 &((PartBox*)pData->m_lStatic[i])->m_vMax)
                && ((HitTarget*)pData->m_lStatic[i])->HitTest(vStart, vEnd, &vEnd, g_nHitMask, 1.0f)) {
                bHit = 1;
                if (pHit)
                    *pHit = vEnd;
            }
        }
    }
    g_bHitGround = 0;
    if (!g_bHitWalkmesh)
        return bHit;
    if (pData->m_p6C)
        return (((HitTarget*)pData->m_p6C)->HitTest(vStart, vEnd, pHit, g_nHitMask, 1.0f) != 0) | bHit;
    if (!g_bHitGroundPlane || pNode->m_n14 || pNode->m_n10)
        return bHit;
    if (vEnd.z >= 0.0f || vStart.z <= 0.0f)
        return bHit;
    if (pHit) {
        Vector vUp(0.0f, 0.0f, 1.0f);
        Vector vResult;
        *pHit = *LinePlaneIntersect(&vResult, &vUp, 0.0f, &vStart, &vEnd);
    }
    g_bHitGround = 1;
    return 1;
}

// FUNCTION: SWKOTOR 0x0046B790
void UpdateRoomLights(BSPNode* pNode)
{
    nodedata* pData = pNode->m_pData;
    for (int i = 0; i < pData->m_lParts60.num; i++)
        ((LightRef*)pData->m_lParts60[i])->Update();
}
