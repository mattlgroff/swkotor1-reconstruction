// Tile teardown for the scene BSP (0x0046D2F0).

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;
};

class Vector;

class Model {
public:
    void DelRef(Model* p);                  // 0x0043E210
};

class MaxTree {
public:
    Model* AsModel();                       // 0x0043E1C0
};

class MdlNode {
public:
    char pad_0x0[8];
    MaxTree* m_pTree;                       // +0x08
};

class Part {
public:
    virtual ~Part();
    MdlNode* m_pNode;                       // +0x04
};

class TileData {
public:
    char pad_0x0[0x18];
    CExoArrayList<Part*> m_lParts;          // +0x18
    Part* m_pWalkmesh;                      // +0x24

    ~TileData();                            // 0x00444E20
};

class nodedata {
public:
    char pad_0x0[0x70];
    TileData* m_pTileData;                  // +0x70
};

class BSPNode {
public:
    char pad_0x0[0x58];
    nodedata* m_pData;                      // +0x58
};

class Scene {
public:
    char pad_0x0[0xa0];
    void* m_pBSP;                           // +0xa0
};

BSPNode* FindTile(void* pTiles, Vector* pPoint, int n);    // 0x004AD060
void SceneBSPRemovePart(Scene* pScene, Part* pPart);       // 0x0046C510

// FUNCTION: SWKOTOR 0x0046D2F0
void SceneBSPRemoveTiledata(Scene* pScene, Vector* pPoint)
{
    if (!pScene->m_pBSP)
        return;
    nodedata* pData = FindTile(pScene->m_pBSP, pPoint, 0)->m_pData;
    if (!pData->m_pTileData)
        return;
    int n = pData->m_pTileData->m_lParts.num;
    while (n) {
        Part* pPart = pData->m_pTileData->m_lParts.element[--n];
        SceneBSPRemovePart(pScene, pPart);
        Model* pModel = pPart->m_pNode->m_pTree->AsModel();
        if (pModel)
            pModel->DelRef(pModel);
        delete pPart;
        pData->m_pTileData->m_lParts.num--;
    }
    if (pData->m_pTileData->m_pWalkmesh) {
        delete pData->m_pTileData->m_pWalkmesh;
        pData->m_pTileData->m_pWalkmesh = 0;
    }
    delete pData->m_pTileData;
    pData->m_pTileData = 0;
}
