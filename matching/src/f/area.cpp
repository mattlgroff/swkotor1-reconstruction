// CSWArea lifetime (b0058_f_lifetime_leaves).

class CSWArea {
public:
    virtual ~CSWArea();

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
