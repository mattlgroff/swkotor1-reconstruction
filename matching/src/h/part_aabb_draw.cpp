// PartAABB walkmesh debug draw (0x00471560).

#include "h/renderpaths.h"

extern "C" __declspec(dllimport) void __stdcall glFrontFace(unsigned int);
extern "C" __declspec(dllimport) void __stdcall glPolygonMode(unsigned int, unsigned int);

extern int g_nTransformMode;        // 0x0078D494
extern int g_nTransformCount;       // 0x00827FA8
extern float g_aTransformMatrix[16];    // 0x008280A0
extern float g_fRadToDeg;           // 0x00742EF8
extern int g_bTransformParts;       // 0x0078E5F0
extern int g_bShowWalkMesh;         // 0x007FBF5C, descriptive
extern int g_nFlatTriangles;        // 0x00827FB0
extern unsigned int g_eGLTexture2DB;    // 0x0078D458
extern unsigned int g_eGLLighting;      // 0x0078D45C
extern unsigned int g_eGLCW;        // 0x0073F2C0
extern unsigned int g_eGLCCW;       // 0x0073F2C4
extern unsigned int g_aPolygonModes[2]; // 0x0073F2E4
extern unsigned int g_eGLFront;     // 0x0073F2EC

unsigned long* GetWalkMeshColors();         // 0x0046F200
unsigned long* GetInverseWalkMeshColors();  // 0x0046F300

class TriFace;
template <class T> class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;
};
void DrawTriangles(CExoArrayList<TriFace>* pFaces, Vector* pVerts, unsigned long* pColors);    // 0x004318F0

// GLRender::Transform 0x0046F060, expanded in this TU.
inline void TransformInline(const Vector* pPos, const Quaternion* pOri, int bForce)
{
    if (!g_nTransformMode && !bForce)
        return;
    g_nTransformCount++;
    if (g_nTransformMode > 0) {
        BuildMatrix(g_aTransformMatrix, pPos, pOri);
        glMultMatrixf(g_aTransformMatrix);
        return;
    }
    static Vector s_vPos(0.0f, 0.0f, 0.0f);
    static Quaternion s_qOri(1.0f, 0.0f, 0.0f, 0.0f);
    s_vPos = *pPos;
    glTranslatef(s_vPos.x, s_vPos.y, s_vPos.z);
    s_qOri = *pOri;
    Vector vAxis = s_qOri.GetAxis();
    glRotatef((float)(acos(s_qOri.w) * 2.0) * g_fRadToDeg, vAxis.x, vAxis.y, vAxis.z);
}

// Transform 0x0046F6B0, expanded in this TU.
inline void TransformPartInline(const Vector* pPos, const Quaternion* pOri, int bForce)
{
    if (g_bTransformParts)
        TransformInline(pPos, pOri, bForce);
}

class MdlNodeAABB {
public:
    char pad_0x0[0x58];
    CExoArrayList<TriFace> m_lFaces;    // +0x58
    char pad_0x64[0x198 - 0x64];
    Vector* m_pVerts;                   // +0x198
};

class PartAABB {
public:
    virtual void V00();
    MdlNodeAABB* m_pNode;           // +0x04
    Vector m_vPosition;             // +0x08
    Quaternion m_qOrientation;      // +0x14
    char m_pad24[0x40 - 0x24];
    int m_bNoTransform;             // +0x40

    void TransformModelviewMatrix()
    {
        if (!m_bNoTransform) {
            glPushMatrix();
            TransformPartInline(&m_vPosition, &m_qOrientation, 0);
        }
    }
    void UntransformModelviewMatrix()
    {
        if (!m_bNoTransform)
            glPopMatrix();
    }
    void Draw(int n);
};

// FUNCTION: SWKOTOR 0x00471560
void PartAABB::Draw(int n)
{
    if (!g_bShowWalkMesh)
        return;
    static unsigned long* s_pColors = GetWalkMeshColors();
    static unsigned long* s_pInverseColors = GetInverseWalkMeshColors();
    MdlNodeAABB* pNode = m_pNode;
    glDisable(g_eGLTexture2DB);
    glDisable(g_eGLLighting);
    glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
    if (!pNode)
        return;
    TransformModelviewMatrix();
    g_nFlatTriangles += pNode->m_lFaces.num;
    DrawTriangles(&pNode->m_lFaces, pNode->m_pVerts, s_pColors);
    glFrontFace(g_eGLCW);
    DrawTriangles(&pNode->m_lFaces, pNode->m_pVerts, s_pInverseColors);
    glFrontFace(g_eGLCCW);
    glPolygonMode(g_eGLFront, g_aPolygonModes[0]);
    DrawTriangles(&pNode->m_lFaces, pNode->m_pVerts, s_pInverseColors);
    glFrontFace(g_eGLCW);
    DrawTriangles(&pNode->m_lFaces, pNode->m_pVerts, s_pColors);
    glFrontFace(g_eGLCCW);
    glPolygonMode(g_eGLFront, g_aPolygonModes[1]);
    UntransformModelviewMatrix();
    glEnable(g_eGLTexture2DB);
    glEnable(g_eGLLighting);
}
