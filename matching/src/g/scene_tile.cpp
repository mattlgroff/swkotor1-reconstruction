// Scene tile slots (vtable 0x741708 +0x28 / +0x2c): per-tile light
// positions and flame objects. Behavioural reference:
// src/units/0728/344241bbf2-m46_scene_c1.cpp, src/units/0694/aa8460a1b8-tile_scene.cpp.
class Vector {
public:
    float x, y, z;
};

int operator==(const Vector& a, const Vector& b);   // 0x004aa980

class CAurObject;

// A static light placed on a tile.
class TileLight {
public:
    char pad00[0x4c];
    Vector m_vPosition;                 // +0x4c
    char pad58[0x5c - 0x58];
    int m_bDirty;                       // +0x5c
    float m_fRadius;                    // +0x60
    char pad64[0x6c - 0x64];
    int m_bActive;                      // +0x6c
    char pad70[0x74 - 0x70];
    int m_nType;                        // +0x74
};

class TileData {
public:
    TileLight* m_pLight0;               // +0x00
    TileLight* m_pLight1;               // +0x04
    int m_nFlame0;                      // +0x08
    int m_nFlame1;                      // +0x0c
    int m_aFlame0[1];                   // +0x10
    int m_aFlame1[1];                   // +0x14
};

class CAurObject {
public:
    char pad00[0x170];
    TileData* m_pTile;                  // +0x170
};

class Scene {
public:
    virtual void v000(); virtual void v004(); virtual void v008(); virtual void v00c();
    virtual void v010(); virtual void v014(); virtual void v018(); virtual void v01c();
    virtual void v020(); virtual void v024(); virtual void v028(); virtual void v02c();
    virtual void v030(); virtual void v034(); virtual void v038(); virtual void v03c();
    virtual void v040(); virtual void v044(); virtual void v048(); virtual void v04c();
    virtual void v050(); virtual void v054(); virtual void v058(); virtual void v05c();
    virtual void v060(); virtual void v064(); virtual void v068(); virtual void v06c();
    virtual void v070(); virtual void v074(); virtual void v078(); virtual void v07c();
    virtual void v080(); virtual void v084(); virtual void v088(); virtual void v08c();
    virtual void v090(); virtual void v094(); virtual void v098(); virtual void v09c();
    virtual void v0a0(); virtual void v0a4(); virtual void v0a8(); virtual void v0ac();
    virtual void v0b0(); virtual void v0b4(); virtual void v0b8(); virtual void v0bc();
    virtual void v0c0(); virtual void v0c4(); virtual void v0c8(); virtual void v0cc();
    virtual void v0d0(); virtual void v0d4(); virtual void v0d8(); virtual void v0dc();
    virtual void v0e0(); virtual void v0e4(); virtual void v0e8(); virtual void v0ec();
    virtual void v0f0(); virtual void v0f4(); virtual void v0f8(); virtual void v0fc();
    virtual void v100(); virtual void v104(); virtual void v108(); virtual void v10c();
    virtual void v110(); virtual void v114(); virtual void v118();
    virtual int SetSingleTileFlame(CAurObject* pObject, int* pFlame, int n);   // +0x11c

    void SetTileLights(Vector* pTile, Vector* pLight0, Vector* pLight1);    // +0x28
    int SetTileFlames(Vector* pTile, CAurObject* p0, CAurObject* p1);       // +0x2c
};

TileData* SceneBSPGetTileData(Scene* pScene, Vector* pPosition);    // 0x0046a680
void UpdateStaticLighting(Scene* pScene, TileLight* pLight);         // 0x0046ca30

// FUNCTION: SWKOTOR 0x0044fb40
void Scene::SetTileLights(Vector* pTile, Vector* pLight0, Vector* pLight1)
{
    TileData* t = SceneBSPGetTileData(this, pTile);
    if (t->m_pLight0 && !(t->m_pLight0->m_vPosition == *pLight0)) {
        t->m_pLight0->m_vPosition = *pLight0;
        UpdateStaticLighting(this, t->m_pLight0);
        t->m_pLight0->m_bDirty = 1;
        t->m_pLight0->m_fRadius = 12.0f;
        t->m_pLight0->m_bActive = 1;
        t->m_pLight0->m_nType = 5;
    }
    if (t->m_pLight1 && !(t->m_pLight1->m_vPosition == *pLight1)) {
        t->m_pLight1->m_vPosition = *pLight1;
        UpdateStaticLighting(this, t->m_pLight1);
        t->m_pLight1->m_bDirty = 1;
        t->m_pLight1->m_fRadius = 8.0f;
        t->m_pLight1->m_bActive = 1;
        t->m_pLight1->m_nType = 5;
    }
}

// FUNCTION: SWKOTOR 0x0044fc20
int Scene::SetTileFlames(Vector* pTile, CAurObject* p0, CAurObject* p1)
{
    TileData* t = SceneBSPGetTileData(this, pTile);
    SetSingleTileFlame(p0, t->m_aFlame0, t->m_nFlame0);
    if (p0)
        p0->m_pTile = t;
    int nResult = SetSingleTileFlame(p1, t->m_aFlame1, t->m_nFlame1);
    if (p1)
        p1->m_pTile = t;
    return nResult;
}
