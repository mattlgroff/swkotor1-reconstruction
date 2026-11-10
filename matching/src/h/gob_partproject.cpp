// Gob part-tree shadow projection (0x00474710). Like Gob::PartDraw, the
// original inlines two recursion levels; only the outermost expands
// _glScalef, Vector::IsZero and the Vector subtraction. The method name is
// descriptive (the per-part virtual at +0x5c is Part::Project).

extern "C" {
__declspec(dllimport) void __stdcall glPushMatrix();
__declspec(dllimport) void __stdcall glPopMatrix();
__declspec(dllimport) void __stdcall glScalef(float x, float y, float z);
}

class Vector {
public:
    float x, y, z;

    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    // 0x0046A620
    inline int IsZero() { return x == 0.0f && y == 0.0f && z == 0.0f; }
};

// 0x00423BC0
inline Vector operator-(const Vector& a, const Vector& b)
{
    return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
}

class Quaternion {
public:
    float w, x, y, z;

    Quaternion() {}
};

Quaternion operator-(const Quaternion& q);                  // 0x004A9B50
Vector operator*(const Quaternion& q, const Vector& v);     // 0x004A9BB0

class Plane {
public:
    Vector n;
    float d;

    Plane() { n.x = 0.0f; n.y = 0.0f; n.z = 0.0f; d = 0.0f; }
    Plane(const Vector& vN, float fD) { n = vN; d = fD; }
    void Transform(const Vector& vTranslation, const Quaternion& qRotation);  // 0x004AB4C0
};

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;
};

// 0x00459150
inline void _glScalef(float x, float y, float z)
{
    glScalef(x, y, z);
}

class GLRender {
public:
    static void Transform(const Vector* pPos, const Quaternion* pOri, int bForce);  // 0x0046F060
};

void SetBeaming();                          // 0x00471450

extern int g_bPartTransform;                // 0x0078E5F0
extern int g_bProjectWorldSpace;            // 0x007B9374, descriptive
extern int g_nPartsProjected;               // 0x00827FA8, descriptive

class MdlNodeTriMesh {
public:
    char pad_0x0[0x188];
    unsigned char m_bBeaming;               // +0x188

};

class CAurShadowData;

class Part {
public:
    virtual void V00();
    virtual void V04();
    virtual void V08();
    virtual void V0C();
    virtual void V10();
    virtual void V14();
    virtual void V18();
    virtual void V1C();
    virtual void V20();
    virtual void V24();
    virtual void V28();
    virtual void* AsTriMesh();              // +0x2c
    virtual void V30();
    virtual void V34();
    virtual void V38();
    virtual void V3C();
    virtual void V40();
    virtual void V44();
    virtual void V48();
    virtual void V4C();
    virtual void V50();
    virtual void V54();
    virtual void V58();
    virtual void Project(const Vector& vLight, float fRange, const Plane& plane, CAurShadowData* pData);  // +0x5c
    MdlNodeTriMesh* m_pNode;                // +0x04
    Vector m_vPosition;                     // +0x08
    Quaternion m_qOrientation;              // +0x14
    float m_fScale;                         // +0x24
    char pad_0x28[4];
    unsigned char m_bVisible;               // +0x2c
    char pad_0x2d[3];
    CExoArrayList<Part*> m_lChildren;       // +0x30
};

class Gob {
public:
    char pad_0x0[0x6c];
    Part* m_pRootPart;                      // +0x6c
    char pad_0x70[8];
    Vector m_vPosition;                     // +0x78
    Quaternion m_qOrientation;              // +0x84
    char pad_0x94[0x1b4 - 0x94];
    float m_fScale;                         // +0x1b4

    void PartProject(Part* pPart, Vector vLight, float fRange, Plane plane);
};

#pragma inline_recursion(on)
#pragma inline_depth(2)

// NONMATCH: `this` and pPart swap ebx/ebp; the original also spills `this`
// to [esp+0x50] so the inlined level can keep the plane's d in ebp.
// FUNCTION: SWKOTOR 0x00474710
void Gob::PartProject(Part* pPart, Vector vLight, float fRange, Plane plane)
{
    if (!pPart->m_bVisible)
        return;
    glPushMatrix();
    if (pPart == m_pRootPart) {
        if (g_bPartTransform)
            GLRender::Transform(&m_vPosition, &m_qOrientation, 1);
        if (!g_bProjectWorldSpace) {
            vLight = -m_qOrientation * (vLight - m_vPosition);
            plane.Transform(m_vPosition, m_qOrientation);
        }
        if (*(int*)&m_fScale != 0x3f800000)
            _glScalef(m_fScale, m_fScale, m_fScale);
    }
    Plane local;
    if (!g_bProjectWorldSpace) {
        vLight = -pPart->m_qOrientation * (vLight - pPart->m_vPosition);
        local = Plane(plane.n, plane.d);
        local.Transform(pPart->m_vPosition, pPart->m_qOrientation);
    }
    if (g_bPartTransform)
        GLRender::Transform(&pPart->m_vPosition, &pPart->m_qOrientation, 0);
    g_nPartsProjected++;
    if (*(int*)&pPart->m_fScale != 0x3f800000)
        _glScalef(pPart->m_fScale, pPart->m_fScale, pPart->m_fScale);
    if (g_bProjectWorldSpace || !vLight.IsZero()) {
        if (pPart->AsTriMesh() ? pPart->m_pNode->m_bBeaming : 0)
            SetBeaming();
        pPart->Project(vLight, fRange, local, 0);
    }
    for (int i = 0; i < pPart->m_lChildren.num; i++)
        PartProject(pPart->m_lChildren.element[i], vLight, fRange, local);
    glPopMatrix();
}
