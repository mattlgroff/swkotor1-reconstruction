// MdlNode family teardown: the per-type InternalDestructor bodies
// (released resources, children torn down through the type dispatcher).
#include "g/mdl_ctor.h"

// FUNCTION: SWKOTOR 0x0043fdd0
void MdlNode::InternalDestructor()
{
    int i = m_lChildren.num;
    while (i) {
        MdlNode* pChild = m_lChildren.element[i - 1];
        i--;
        pChild->InternalDestructorDispatch();
    }
    g_nMdlNodesAlive--;
}

// NONMATCH (14): `this` lands in edi and the -1 constant in esi; the
// original has them swapped (same instructions otherwise).
// FUNCTION: SWKOTOR 0x0043fd10
void MdlNodeLightsaber::InternalDestructor()
{
    PoolRelease(m_nVertexBuffer);
    m_nVertexBuffer = -1;
    PoolRelease(m_nIndexBuffer);
    m_nIndexBuffer = -1;
    MdlNode::InternalDestructor();
}

// NONMATCH (23): ebx/edi swap for `this` vs the inlined Remove's count
// (same instructions otherwise); the --i fetch form did not help.
// FUNCTION: SWKOTOR 0x00448000
void MdlNodeLight::InternalDestructor()
{
    int i = m_lFlares.num;
    while (i) {
        i--;
        CAurTexture* pFlare = m_lFlares.element[i];
        m_lFlares.element[i] = 0;
        AurTextureRelease(pFlare);
        m_lFlares.num--;
    }
    MdlNode::InternalDestructor();
}

// FUNCTION: SWKOTOR 0x0044a500
void MdlNodeLight::BinaryInternalDestructor()
{
    int i = m_lFlares.num;
    while (i) {
        i--;
        CAurTexture* pFlare = m_lFlares.element[i];
        m_lFlares.element[i] = 0;
        AurTextureRelease(pFlare);
        m_lFlares.num--;
    }
    m_lFlares.SetSize(0);
}

// NONMATCH (75): the original re-reads the child type for both the class
// test and the As* null test (`mov cl,[esi]; and cl,3; cmp; mov dl,[esi];
// and dl,cl`), compares word masks as 16-bit, and shares one ~MdlNode
// call block; MSVC CSEs the type load and folds the As* test here in
// every shape tried (As* calls, explicit masks, out-of-line As*).
// A volatile read reproduces the instruction pattern but not the layout.
// FUNCTION: SWKOTOR 0x0044a040
MdlNode::~MdlNode()
{
    if (m_nType)
        InternalDestructorDispatch();
    int i = m_lChildren.num;
    m_nType = 0;
    while (i)
        MdlNodeDelete(m_lChildren.element[--i]);
    g_nMdlNodesTotal--;
}

// FUNCTION: SWKOTOR 0x00448da0
MdlNodeAnimMesh::~MdlNodeAnimMesh()
{
    if (m_nType)
        InternalDestructorDispatch();
    m_nType = 0;
    InternalDestructorDispatch();
    delete m_p1c4;
    delete m_p1c8;
}

// FUNCTION: SWKOTOR 0x00449090
MdlNodeLightsaber::~MdlNodeLightsaber()
{
    if (m_nType)
        InternalDestructorDispatch();
    m_nType = 0;
}

// FUNCTION: SWKOTOR 0x004490f0
MdlNodeDanglyMesh::~MdlNodeDanglyMesh()
{
    if (m_nType)
        InternalDestructorDispatch();
    m_nType = 0;
    delete m_p1b4;
}

// FUNCTION: SWKOTOR 0x00449170
MdlNodeAABB::~MdlNodeAABB()
{
    if (m_nType)
        InternalDestructorDispatch();
    AABB* pTree = m_pTree;
    m_nType = 0;
    if (pTree)
        delete pTree;
}

// FUNCTION: SWKOTOR 0x00448e60
MdlNodeSkin::~MdlNodeSkin()
{
    if (m_nType)
        InternalDestructorDispatch();
    m_nType = 0;
    if (m_p1b0) {
        delete m_p1b0;
        m_p1b0 = 0;
    }
    m_lQBones.SetSize(0);
    m_lBoneMap.SetSize(0);
}

// FUNCTION: SWKOTOR 0x00448f80
MdlNodeLight::~MdlNodeLight()
{
    if (m_nType)
        InternalDestructorDispatch();
    int i = m_lFlareTextures.num;
    m_nType = 0;
    while (i) {
        delete m_lFlareTextures.element[--i];
        m_lFlareTextures.num--;
    }
}

// NONMATCH (8): the two counted loops pick eax/ecx/edx differently
// (value in eax, base in ecx / edx in the original).
// FUNCTION: SWKOTOR 0x00448bf0
MdlNodeTriMesh::~MdlNodeTriMesh()
{
    if (m_nType)
        InternalDestructorDispatch();
    MdlNodeTriMesh_PostProcess* pPostProcess = m_pPostProcess;
    m_nType = 0;
    if (pPostProcess)
        delete pPostProcess;
    if (m_nTexture != -1) {
        PoolRelease(m_nTexture);
        m_nTexture = -1;
    }
    for (int i = 0; i < m_lTextures.num; i++) {
        PoolRelease(m_lTextures[i]);
        m_lTextures[i] = -1;
    }
    for (int j = 0; j < m_lVerts.num; j++)
        delete m_lVerts[j];
    m_lVerts.num = 0;
    delete m_p198;
}
