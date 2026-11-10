// CSWArea lifetime (b0058_f_lifetime_leaves).

class Vector {
public:
    float x, y, z;
};

class CSWArea {
public:
    virtual ~CSWArea();
    virtual void v04();
    virtual int SetFog(float fFogNear, float fFogFar, Vector vFogColor);   // +0x08
    virtual int SetIsNight(int bIsNight);                                  // +0x0c
    virtual int SetWind(unsigned char nWind);                              // +0x10

    char pad_0x4[0x14];
    void *m_p18;   // +0x18
    void *m_p1C;   // +0x1c
    void *m_p20;   // +0x20
    void *m_p24;   // +0x24
    void *m_p28;   // +0x28
    char pad_0x2c[0x8];
    void *m_p34;   // +0x34
    char pad_0x38[0x8];
    void *m_p40;   // +0x40
    char pad_0x44[0x8];
    void *m_p4C;   // +0x4c
    void *m_p50;   // +0x50
    Vector m_vFogColor;              // +0x54
    float m_fFogNear;                // +0x60
    float m_fFogFar;                 // +0x64
    char pad_0x68[0x3c];
    int m_bIsNight;                  // +0xa4
    char pad_0xa8[0x3];
    unsigned char m_nWind;           // +0xab
};

// FUNCTION: SWKOTOR 0x0058F7A0
CSWArea::~CSWArea()
{
    delete m_p1C;
    delete m_p20;
    delete m_p24;
    delete m_p28;
    delete m_p18;
    delete m_p34;
    delete m_p40;
    delete m_p4C;
    delete m_p50;
}

// FUNCTION: SWKOTOR 0x0058FF60 SYMBOL: ??_GCSWArea@@UAEPAXI@Z

// Fog / night / wind setters (vtable +0x08..+0x10; descriptive names, the
// client area overrides SetWind). Their starts are missing from functions.csv.
// FUNCTION: SWKOTOR 0x0058FCE0
int CSWArea::SetFog(float fFogNear, float fFogFar, Vector vFogColor)
{
    m_vFogColor = vFogColor;
    m_fFogNear = fFogNear;
    m_fFogFar = fFogFar;
    return 1;
}

// FUNCTION: SWKOTOR 0x0058FD10
int CSWArea::SetIsNight(int bIsNight)
{
    m_bIsNight = bIsNight;
    return 1;
}

// FUNCTION: SWKOTOR 0x0058FD30
int CSWArea::SetWind(unsigned char nWind)
{
    m_nWind = nWind;
    return 1;
}

// ---------------------------------------------------------------------------
// CSWPlaceableSurfaceMesh (client walkmesh of a placeable).

class CSWCollisionMesh {
public:
    virtual ~CSWCollisionMesh();
};

class CSWPlaceableSurfaceMesh : public CSWCollisionMesh {
public:
    virtual ~CSWPlaceableSurfaceMesh();
};

// FUNCTION: SWKOTOR 0x005CDAC0
CSWPlaceableSurfaceMesh::~CSWPlaceableSurfaceMesh()
{
}
