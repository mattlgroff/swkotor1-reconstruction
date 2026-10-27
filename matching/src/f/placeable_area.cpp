// CSWSPlaceable::AddToArea (round s8, 0x00587170).

class CExoString {
public:
    CExoString();
    CExoString(const char *s);
    ~CExoString();
    char *m_sString;
    unsigned long m_nBufferLength;
};

class CResRef {
public:
    CResRef(const CExoString &s);
    CResRef operator=(const CResRef &r);
    char m_resRef[16];
};

class Vector {
public:
    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    float x, y, z;
};

class Quaternion {
public:
    Quaternion(Vector vAxis, float fAngle);
    float w, x, y, z;
};

float GetFacing(const Vector &vOrientation);

class C2DA {
public:
    int GetINTEntry(int nRow, const CExoString &sColumn, int *pValue);
    int GetCExoStringEntry(int nRow, const CExoString &sColumn, CExoString *pValue);
};

class C2DArrays {
public:
    char pad_0x0[0x2c];
    C2DA *m_pPlaceableTable;         // +0x2c
    C2DA *GetPlaceableTable() { return m_pPlaceableTable; }
};

class CSWRules {
public:
    char pad_0x0[0xb8];
    C2DArrays *m_p2DArrays;          // +0xb8
    C2DArrays *Get2DArrays() { return m_p2DArrays; }
};

extern CSWRules *g_pRules;
extern int g_bShowPlaceableBoxes;

class CSWSArea {
public:
    int AddStaticBoundingBox(unsigned long oid, Vector *pMin, Vector *pMax);
    void AddObjectToArea(unsigned long oid, int bRunScripts);
};

class CGameObject {
public:
    virtual ~CGameObject();
    unsigned long m_idSelf;          // +0x4
    unsigned char m_nObjectType;     // +0x8
    char pad_0x9[0x3];
    unsigned long GetID() { return m_idSelf; }
};

class CSWSObject : public CGameObject {
public:
    virtual ~CSWSObject();
    CSWSArea *GetArea();
    void SetArea(CSWSArea *pArea);
    void SetPosition(const Vector &v, int a, int b, int c);

    char pad_0xc[0x84];
    Vector m_vPosition;              // +0x90
    Vector m_vOrientation;           // +0x9c
    char pad_0xa8[0x180];
};

class CSWCollisionMesh {
public:
    virtual ~CSWCollisionMesh();
    int LoadMesh(int bTextOnly);
    int GetMeshBoundingBox(Vector &vMin, Vector &vMax);
    void RenderDebug(int bFill, unsigned long nColor1, unsigned long nColor2);

    char pad_0x4[0x8];
    CResRef m_cResRef;               // +0x0c
    char pad_0x1c[0x4];
    Vector m_vPosition;              // +0x20
    Vector m_vOffset;                // +0x2c
    Quaternion m_qOrientation;       // +0x38
    char pad_0x48[0x40];
};

class CSWPlaceableSurfaceMesh : public CSWCollisionMesh {
public:
    virtual ~CSWPlaceableSurfaceMesh();
};

class CSWSPlaceable : public CSWSObject {
public:
    virtual ~CSWSPlaceable();
    void RemoveFromArea();
    void AddToArea(CSWSArea *pArea, const Vector &vPosition, int bRunScripts);
    unsigned char GetAppearance() { return m_nAppearance; }

    char pad_0x228[0x8];
    unsigned char m_nAppearance;     // +0x230
    char pad_0x231[0x10b];
    int m_bPreciseUse;               // +0x33c
    int m_bHostile;                  // +0x340
    int m_nBoundingBoxIndex;         // +0x344
    char pad_0x348[0x58];
    CSWPlaceableSurfaceMesh m_cWalkMesh; // +0x3a0
};

// NONMATCH (21): the Hostile / ModelName lookups load g_pRules before the
// appearance byte and the sete result uses ecx instead of eax.
// FUNCTION: SWKOTOR 0x00587170
void CSWSPlaceable::AddToArea(CSWSArea *pArea, const Vector &vPosition, int bRunScripts)
{
    RemoveFromArea();
    if (!pArea)
        return;
    SetArea(pArea);
    SetPosition(vPosition, 0, 1, 0);
    CExoString sModelName;
    {
        int nPreciseUse;
        g_pRules->Get2DArrays()->GetPlaceableTable()->GetINTEntry(GetAppearance(), "PreciseUse", &nPreciseUse);
        m_bPreciseUse = nPreciseUse == 1;
    }
    int nHostile;
    g_pRules->Get2DArrays()->GetPlaceableTable()->GetINTEntry(GetAppearance(), "Hostile", &nHostile);
    m_bHostile = nHostile == 1;
    g_pRules->Get2DArrays()->GetPlaceableTable()->GetCExoStringEntry(GetAppearance(), "ModelName", &sModelName);
    CResRef cModel(sModelName);
    m_cWalkMesh.m_cResRef = cModel;
    if (m_cWalkMesh.LoadMesh(0)) {
        m_cWalkMesh.m_vPosition = m_vPosition;
        Vector vMin(0.0f, 0.0f, 0.0f);
        Vector vMax(0.0f, 0.0f, 0.0f);
        Vector vAxis(0.0f, 0.0f, 1.0f);
        m_cWalkMesh.m_qOrientation = Quaternion(vAxis, GetFacing(m_vOrientation) * 0.017453292f);
        m_cWalkMesh.GetMeshBoundingBox(vMin, vMax);
        if (g_bShowPlaceableBoxes)
            m_cWalkMesh.RenderDebug(0, 0xffff0000, 0xff00ff00);
        m_nBoundingBoxIndex = pArea->AddStaticBoundingBox(GetID(), &vMin, &vMax);
    }
    GetArea()->AddObjectToArea(m_idSelf, 1);
}
