// Small Scene virtuals (vtable 0x741708) that functions.csv does not list:
// 0x0044f7e0, 0x0044f8b0, 0x0044fcc0, 0x0044fcd0, 0x0044fcf0, 0x0044fd10.
// Behavioural reference: src/units/0728/344241bbf2-m46_scene_c1.cpp,
// src/units/1470/5ac096f97b-scene3d.cpp, src/units/1381/0e2b88478d-w3_client_aurora.cpp.
class Vector {
public:
    float x, y, z;
};

class Gob {
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
    virtual void v0c0(); virtual void v0c4(); virtual void v0c8();
    virtual void SetObjectScale(float fScale, int bRecurse);   // +0xcc
};

extern int g_nGeomFadeMode;                     // 0x0078e3a0

class Scene {
public:
    virtual void v000(); virtual void v004(); virtual void v008(); virtual void v00c();
    virtual void SetObjectScale(Gob* pGob, float fScale);      // +0x10
    virtual void v014(); virtual void v018(); virtual void v01c();
    virtual void v020(); virtual void v024(); virtual void v028(); virtual void v02c();
    virtual void v030(); virtual void v034(); virtual void v038();
    virtual void ComputeStaticLighting();                      // +0x3c
    virtual void v040(); virtual void v044(); virtual void v048();
    virtual unsigned char SetGeomFadeProperties(int nMode);    // +0x4c
    virtual void v050(); virtual void v054(); virtual void v058();
    virtual int GetNumRooms();                                 // +0x5c
    virtual void v060(); virtual void v064(); virtual void v068(); virtual void v06c();
    virtual void v070(); virtual void v074(); virtual void v078(); virtual void v07c();
    virtual void v080(); virtual void v084();
    virtual int EnableVisibilityGraph();                       // +0x88
    virtual void v08c();
    virtual void v090(); virtual void v094(); virtual void v098(); virtual void v09c();
    virtual void v0a0(); virtual void v0a4(); virtual void v0a8(); virtual void v0ac();
    virtual void v0b0(); virtual void v0b4(); virtual void v0b8(); virtual void v0bc();
    virtual void v0c0(); virtual void v0c4(); virtual void v0c8(); virtual void v0cc();
    virtual void v0d0();
    virtual void UpdateStaticLighting();                       // +0xd4

    void SetAmbient(Vector vColor);

    char pad04[0x88 - 0x04];
    Vector m_vAmbient;                          // +0x88
    char pad94[0xcc - 0x94];
    int m_nRooms;                               // +0xcc
    char padd0[0xd4 - 0xd0];
    int m_nCurrentRoom;                         // +0xd4
    short m_bIgnoreVisibilityGraph;             // +0xd8
};

// FUNCTION: SWKOTOR 0x0044f7e0
int Scene::EnableVisibilityGraph()
{
    m_bIgnoreVisibilityGraph = 0;
    return m_nCurrentRoom != 0;
}

// FUNCTION: SWKOTOR 0x0044f8b0
int Scene::GetNumRooms()
{
    return m_nRooms;
}

// FUNCTION: SWKOTOR 0x0044fcc0
void Scene::ComputeStaticLighting()
{
    if (this)
        UpdateStaticLighting();
}

// FUNCTION: SWKOTOR 0x0044fcd0
void Scene::SetAmbient(Vector vColor)
{
    m_vAmbient = vColor;
}

// FUNCTION: SWKOTOR 0x0044fcf0
void Scene::SetObjectScale(Gob* pGob, float fScale)
{
    pGob->SetObjectScale(fScale, 1);
}

// FUNCTION: SWKOTOR 0x0044fd10
unsigned char Scene::SetGeomFadeProperties(int nMode)
{
    if (nMode == 1 || nMode == 2 || nMode == 4) {
        g_nGeomFadeMode = nMode;
        return 1;
    }
    return 0;
}
