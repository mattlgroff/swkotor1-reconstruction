// Static tile objects (room lights and flames) hooked into their scene.
// Behavioural reference: src/units/0731/6e1385c7b5-m46_tile.cpp.
#include "g/gob.h"

// A static tile object (room light/flame) placed into its scene.
class TileData {
public:
    char pad00[0x24];
    Gob* m_pGob;                        // +0x24
};

void UpdateStaticLightingCallback(Gob* pGob, void* pData, Part* pPart);   // 0x0046ddf0

// FUNCTION: SWKOTOR 0x0044fac0
void ProcessStaticObject(Gob* pGob, Scene* pScene, TileData* pTile, Vector* pPos,
                         Quaternion* pOrient)
{
    pTile->m_pGob = pGob;
    pGob->m_vPosition = *pPos;
    pGob->m_qOrientation = *pOrient;
    pGob->m_pTileData = pTile;
    pGob->SetScene(pScene);
    pGob->AddCallback(UpdateStaticLightingCallback, "AnimateStaticLight", 0, 9999.0f, 0);
}
