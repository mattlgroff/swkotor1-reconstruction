// skin vertex programs, MdlNodeSkin helpers and the CExoArrayList
// instances used by the skin / part code (0x00487310 .. 0x004885E0).

#define E_INLINE_ALLOCATE
#include "e/mdlanim.h"

class VertexProgram {
public:
    VertexProgram(char* szProgram, int nType);     // 0x004A27D0
    char pad_0x0[0x201c];
};

extern int g_bUseHardwareSkin;              // 0x0078E5EC
extern char g_szSkinVPHw[];                 // 0x00792658
extern char g_szSkinVPSw[];                 // 0x0079B5A8
extern char g_szSkinSphereMapVP[];          // 0x0079E958
extern char g_szSkinCubeMapVPHw[];          // 0x007938C8
extern char g_szSkinCubeMapVPSw[];          // 0x0079F128
extern int g_nSkinVPType;                   // 0x00830578
extern int g_nSkinSphereMapVPType;          // 0x0083058C
extern int g_nSkinCubeMapVPType;            // 0x00830590

// FUNCTION: SWKOTOR 0x00487310
VertexProgram* GetSkinVP()
{
    static VertexProgram* s_pVP = new VertexProgram(
        g_bUseHardwareSkin ? g_szSkinVPHw : g_szSkinVPSw, g_nSkinVPType);
    return s_pVP;
}

// FUNCTION: SWKOTOR 0x004873C0
VertexProgram* GetSkinSphereMapVP()
{
    static VertexProgram* s_pVP = new VertexProgram(g_szSkinSphereMapVP,
                                                    g_nSkinSphereMapVPType);
    return s_pVP;
}

// FUNCTION: SWKOTOR 0x00487460
VertexProgram* GetSkinCubeMapVP()
{
    static VertexProgram* s_pVP = new VertexProgram(
        g_bUseHardwareSkin ? g_szSkinCubeMapVPHw : g_szSkinCubeMapVPSw,
        g_nSkinCubeMapVPType);
    return s_pVP;
}

class MdlNodeInstance {
public:
    MdlNodeInstance(MdlNode* pNode);        // 0x00445840
    virtual ~MdlNodeInstance();
    char pad_0x4[0x94];
};

class MdlNodeSkinInstance : public MdlNodeInstance {
public:
    MdlNodeSkinInstance(MdlNodeSkin* pNode) : MdlNodeInstance(pNode) {}
    virtual ~MdlNodeSkinInstance();
};

// FUNCTION: SWKOTOR 0x00487510
MdlNodeInstance* MdlNodeSkin::InternalCreateInstance(Part* pPart)
{
    return new MdlNodeSkinInstance(this);
}

// The original calls Allocate (0x0043F6B0) out of line here but inlines it
// in InternalGenVertices; the call target is a relocation, so the
// out-of-line name only has to keep it from being inlined.
// FUNCTION: SWKOTOR 0x004880D0
template <> CExoArrayList<class SkinVertexWeight>& CExoArrayList<class SkinVertexWeight>::operator=(const CExoArrayList<class SkinVertexWeight>& o)
{
    num = 0;
    for (int i = 0; i < o.num; i++) {
        SkinVertexWeight value = o.element[i];
        if (num == array_size)
            AllocateOutOfLine(array_size ? array_size * 2 : 8);
        element[num++] = value;
    }
    return *this;
}

// FUNCTION: SWKOTOR 0x00488160
template <> void CExoArrayList<class Matrix>::AllocateOutOfLine(int n)
{
    Matrix* old = element;
    array_size = n;
    element = new Matrix[n];
    for (int i = 0; i < num; i++)
        element[i] = old[i];
    if (old)
        delete[] old;
}

// FUNCTION: SWKOTOR 0x004885E0
void MdlNodeSkin::InternalGenVertices()
{
    CExoArrayList<SkinVertexWeight> lWeights;

    for (int f = 0; f < m_lFaces.num; f++) {
        for (int k = 0; k < 3; k++) {
            MdlNodeTriMeshData* d = m_pData;
            MdlFaceIndices* r = &d->m_lFaceIndices.element[f];
            Vector* pPos = &d->m_lVerts.element[d->m_lFaceIndices.element[f].pPos[k]];
            Vector* pTV0 = d->m_lTVerts0.num ? &d->m_lTVerts0.element[r->pTex[k]] : 0;
            Vector* pTV1 = d->m_lTVerts1.num ? &d->m_lTVerts1.element[r->pTex[3 + k]] : 0;
            Vector* pTV2 = d->m_lTVerts2.num ? &d->m_lTVerts2.element[r->pTex[6 + k]] : 0;
            Vector* pTV3 = d->m_lTVerts3.num ? &d->m_lTVerts3.element[r->pTex[9 + k]] : 0;
            Vector* pNormal = &d->m_lNormals.element[r->pNormal[k]];
            NodeVertex* pVertex = new NodeVertex(pPos, pTV0, pTV1, pTV2, pTV3, pNormal, 0);

            int i;
            for (i = 0; i < m_pData->m_lNodeVerts.num; i++) {
                if (*m_pData->m_lNodeVerts.element[i] == *pVertex) {
                    delete pVertex;
                    m_lFaces.element[f].verts[k] = (short)i;
                    goto next;
                }
            }
            m_lFaces.element[f].verts[k] = (short)m_pData->m_lNodeVerts.num;
            m_pData->m_lNodeVerts.Add(pVertex);
            lWeights.Add(m_lWeights.element[m_pData->m_lFaceIndices.element[f].pPos[k]]);
        next:;
        }
    }
    m_lWeights = lWeights;
}
