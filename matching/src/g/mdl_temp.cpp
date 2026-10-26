// MdlNode*::InternalRemoveTemporaryArrays (0x0043c090-0x0043c320): free
// the post-process scratch data once a mesh is built, and the per-vertex
// animation record those scratch lists hold for animated meshes.
void __cdecl operator delete[](void* p) throw();
#include "g/mdl_ctor.h"

// Scratch lists shared by the post-process (see mdl_misc.cpp).
extern CExoArrayList<int> g_lSkinScratch0;      // 0x007a8a70
extern CExoArrayList<int> g_lSkinScratch1;      // 0x007a8a7c
extern CExoArrayList<int> g_lSkinScratch2;      // 0x007a8a88

// FUNCTION: SWKOTOR 0x0043c090
void MdlNodeTriMesh::InternalRemoveTemporaryArrays()
{
    if (m_pPostProcess) {
        for (int i = 0; i < m_pPostProcess->m_lTemporary.num; i++)
            delete m_pPostProcess->m_lTemporary[i];
        m_pPostProcess->m_lTemporary.SetSize(0);
    }
    g_lSkinScratch2.num = 0;
    g_lSkinScratch1.num = 0;
    g_lSkinScratch0.num = 0;
    if (m_pPostProcess)
        delete m_pPostProcess;
    m_pPostProcess = 0;
}

// FUNCTION: SWKOTOR 0x0043c130
void MdlNodeDanglyMesh::InternalRemoveTemporaryArrays()
{
    for (int i = 0; i < m_pPostProcess->m_lTemporary.num; i++)
        delete m_pPostProcess->m_lTemporary[i];
    m_pPostProcess->m_lTemporary.SetSize(0);
    MdlNodeTriMesh::InternalRemoveTemporaryArrays();
}

// FUNCTION: SWKOTOR 0x0043c1a0
void MdlNodeSkin::InternalRemoveTemporaryArrays()
{
    MdlNodeTriMesh::InternalRemoveTemporaryArrays();
}

// Per-vertex animation scratch record of an animated mesh.
class NodeVertexAnim {
public:
    char pad00[0x58];
    CExoArrayList<Vector> m_lVerts;         // +0x58
    CExoArrayList<Vector> m_lTVerts;        // +0x64
    CExoArrayList<Vector> m_lNormals;       // +0x70

    ~NodeVertexAnim();
};

// FUNCTION: SWKOTOR 0x0043c1b0
NodeVertexAnim::~NodeVertexAnim()
{
    m_lVerts.SetSize(0);
    m_lTVerts.SetSize(0);
    m_lNormals.SetSize(0);
}

// FUNCTION: SWKOTOR 0x0043c320
void MdlNodeAnimMesh::InternalRemoveTemporaryArrays()
{
    for (int i = 0; i < m_pPostProcess->m_lTemporary.num; i++) {
        NodeVertexAnim* p = (NodeVertexAnim*)m_pPostProcess->m_lTemporary[i];
        if (p)
            delete p;
    }
    m_pPostProcess->m_lTemporary.SetSize(0);
    m_lAnimVerts.SetSize(0);
    m_lAnimTVerts.SetSize(0);
    m_lAnimNormals.SetSize(0);
    MdlNodeTriMesh::InternalRemoveTemporaryArrays();
}
