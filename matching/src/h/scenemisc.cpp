// small SceneBSP helpers whose extents became reachable
// (own TU so the callers in scenebsp.cpp keep calling them).

#include "h/scenebsp.h"

class Light;

class LitPart {
public:
    char pad_0x0[0x58];
    float m_fRadius;                    // +0x58
    char pad_0x5c[0x14];
    int m_bStatic;                      // +0x70
    char pad_0x74[0x20];
    Vector m_vCenter;                   // +0x94
};

class LightScene {
public:
    char pad_0x0[0xa0];
    BSPNode* m_pTiles;                  // +0xa0
};

class LightBin {
public:
    char pad_0x0[8];
    float m_fRadius;                    // +0x08
    char pad_0xc[0x20];
    Vector m_vCenter;                   // +0x2c

    void SetDistance(float f);          // 0x00616FA0
};

class BinNode {
public:
    char pad_0x0[0x58];
    struct BinData* m_pData;            // +0x58
};

struct BinData {
    char pad_0x0[0x60];
    CExoArrayList<LightBin*> m_lBins;   // +0x60
};

void BSPSphereCheck(BSPNode* pNode, Vector* pCenter, float fRadius,
    CExoArrayList<BSPNode*>* pList);                 // 0x004AD560
float VectorMagnitude(Vector* pVector);             // 0x004AB100
void LightNodeTile(BSPNode* pTile, LightScene* pScene);     // 0x0046DB60
void BSPTraverse(BSPNode* pNode, void (*pfn)(BSPNode*, LightScene*),
    LightScene* pScene);                            // 0x004ACEA0
void UpdateRoomLights(BSPNode* pNode, LightScene* pScene);  // 0x0046B790

extern CExoArrayList<BSPNode*> g_lLightTiles;       // 0x007FBFE4
extern CExoArrayList<LightBin*> g_lActiveBins;      // 0x007FBF9C
extern int g_n7FBF2C;

// FUNCTION: SWKOTOR 0x0046A620
int Vector::IsZero()
{
    return x == 0.0f && y == 0.0f && z == 0.0f;
}

// FUNCTION: SWKOTOR 0x0046CA30
void UpdateStaticLighting(LightScene* pScene, LitPart* pPart)
{
    if (pPart->m_bStatic)
        return;
    CExoArrayList<BSPNode*> lLights;
    if (pScene->m_pTiles) {
        BSPSphereCheck(pScene->m_pTiles, &pPart->m_vCenter, pPart->m_fRadius, &lLights);
        for (int i = 0; i < lLights.num; i++)
            g_lLightTiles.AddUnique(lLights[i]);
    }
}

// FUNCTION: SWKOTOR 0x0046CAE0
void ActivateBinsInRange(BinNode* pNode, Vector* pPos, float fMaxSq)
{
    BinData* pData = pNode->m_pData;
    int n = pData->m_lBins.num;
    for (int i = 0; i < n; i++) {
        LightBin* pBin = pData->m_lBins[i];
        Vector d(0.0f, 0.0f, 0.0f);
        d = *pPos - pBin->m_vCenter;
        float f = VectorMagnitude(&d) - pBin->m_fRadius;
        if (f < 0.0f)
            f = 0.0f;
        float fSq = f * f;
        if (fSq < fMaxSq) {
            g_n7FBF2C++;
            g_lActiveBins.Add(pBin);
            pBin->SetDistance(fSq);
        }
    }
}

// FUNCTION: SWKOTOR 0x0046DDA0
void DoStaticLightBuckets(LightScene* pScene)
{
    int i;
    for (i = 0; i < g_lLightTiles.num; i++)
        LightNodeTile(g_lLightTiles[i], pScene);
    if (g_lLightTiles.num && pScene->m_pTiles)
        BSPTraverse(pScene->m_pTiles, UpdateRoomLights, pScene);
}

extern int g_n7A4710, g_n827FAC, g_n827FB0, g_n827FB4, g_n827FB8, g_n827FA8,
    g_n827FBC, g_n7B92A8;

// FUNCTION: SWKOTOR 0x0046EE00
void FUN_0046ee00()
{
    g_n7A4710 = 0;
    g_n827FAC = 0;
    g_n827FB0 = 0;
    g_n827FB4 = 0;
    g_n827FB8 = 0;
    g_n827FA8 = 0;
    g_n827FBC = 0;
    g_n7B92A8 = 0;
}
