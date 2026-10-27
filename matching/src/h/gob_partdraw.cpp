// Gob part-tree draw (0x00473EA0). The original inlines two recursion
// levels; only the outermost expands the _glScalef wrapper.

extern "C" {
__declspec(dllimport) void __stdcall glPushMatrix();
__declspec(dllimport) void __stdcall glPopMatrix();
__declspec(dllimport) void __stdcall glScalef(float x, float y, float z);
}

class Vector {
public:
    float x, y, z;
};

class Quaternion {
public:
    float w, x, y, z;
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

extern int g_bPartTransform;                // 0x0078E5F0
extern int g_nPartDrawDepth;                // 0x0078E5E0
extern Vector g_vPartDraw;                  // 0x0078E8AC

class MdlNodeTriMesh {
public:
    char pad_0x0[0x189];
    unsigned char m_bRender;                // +0x189
};

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
    virtual void V5C();
    virtual void Mat();                     // +0x60
    virtual void Draw(int nFlag);           // +0x64
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
    void PartDraw(Part* pPart, int nFlag);
};

#pragma inline_recursion(on)
#pragma inline_depth(2)

// FUNCTION: SWKOTOR 0x00473EA0
void Gob::PartDraw(Part* pPart, int nFlag)
{
    if (!pPart->m_bVisible)
        return;
    Vector vSaved = g_vPartDraw;
    glPushMatrix();
    if (g_bPartTransform)
        GLRender::Transform(&pPart->m_vPosition, &pPart->m_qOrientation, 0);
    float fScale = pPart->m_fScale;
    _glScalef(fScale, fScale, fScale);
    if (!pPart->AsTriMesh() || !pPart->m_pNode || pPart->m_pNode->m_bRender) {
        pPart->Mat();
        pPart->Draw(nFlag);
    }
    if (g_nPartDrawDepth) {
        g_nPartDrawDepth--;
        for (int i = 0; i < pPart->m_lChildren.num; i++)
            PartDraw(pPart->m_lChildren.element[i], nFlag);
        g_nPartDrawDepth++;
    }
    glPopMatrix();
    g_vPartDraw = vSaved;
}
