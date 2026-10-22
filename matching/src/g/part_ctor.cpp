// Part instance constructors.
#include "g/part_ctor.h"

// FUNCTION: SWKOTOR 0x00445720
MdlNodeTriMesh_PostProcess::MdlNodeTriMesh_PostProcess()
{
}

// FUNCTION: SWKOTOR 0x004457a0
Part::Part(MdlNode* pNode)
{
    m_fScale = 1.0f;
    m_pNode = pNode;
    m_n3c = 0;
    m_p40 = 0;
    m_n48 = 0;
    m_pMaterial = 0;
    g_nPartCount++;
    m_n28 = 0;
    m_b2c = 0;
}

// FUNCTION: SWKOTOR 0x00445a40
PartLight::PartLight(MdlNode* pNode) : Part(pNode)
{
    m_f78 = 1.0f;
    m_n5c = 0;
    m_f68 = 0;
    m_f6c = 0;
    m_f70 = 0;
    m_n58 = 0;
    m_n7c = 0;
    m_n80 = 0;
    m_n84 = 0;
    m_n88 = 0;
    m_n74 = 0;
    m_n90 = 0;
    m_n64 = 0;
    m_f8c = 1.0f;
    m_f60 = -1.0f;
}

// FUNCTION: SWKOTOR 0x004478d0
PartLightsaber::PartLightsaber(MdlNode* pNode) : PartTriMesh(pNode)
{
    m_n3c0 = 0;
    m_n3c4 = 0;
    m_n3c8 = 0;
    m_n3bc = 0;
    m_n3ba = 0;
    m_n3b8 = 0;
}

// FUNCTION: SWKOTOR 0x004453b0
MdlNodeTriMesh_PostProcess::~MdlNodeTriMesh_PostProcess()
{
    m_lVerts.SetSize(0);
    m_lColors.SetSize(0);
    m_lTVerts0.SetSize(0);
    m_lTVerts1.SetSize(0);
    m_lTVerts2.SetSize(0);
    m_lTVerts3.SetSize(0);
    m_lNormals.SetSize(0);
    m_lFaces.SetSize(0);
    for (int i = 0; i < m_lStrings.num; i++)
        delete m_lStrings[i];
    m_lStrings.SetSize(0);
}

// NONMATCH (8): the original reuses esi (&m_nConstraintBuffer) for the
// PoolLock argument and edx for the last memcpy count.
// FUNCTION: SWKOTOR 0x00447980
PartDanglyMesh::PartDanglyMesh(MdlNode* pNode) : PartTriMesh(pNode)
{
    m_pConstraints = 0;
    m_pConstraints = (Vector*)operator new(
        pNode->AsMdlNodeTriMesh()->m_nVerts * sizeof(Vector));
    memcpy(m_pConstraints, pNode->AsMdlNodeDanglyMesh()->m_pConstraints,
           pNode->AsMdlNodeTriMesh()->m_nVerts * sizeof(Vector));
    PoolRequest(0, pNode->AsMdlNodeTriMesh()->m_nVerts * sizeof(Vector), 2, 0,
                &m_nConstraintBuffer);
    void* pBuffer = PoolLock(m_nConstraintBuffer, 0, 0, 1);
    memcpy(pBuffer, m_pConstraints,
           pNode->AsMdlNodeTriMesh()->m_nVerts * sizeof(Vector));
    PoolUnlock(m_nConstraintBuffer);
}

// FUNCTION: SWKOTOR 0x00445840
PartTriMesh::PartTriMesh(MdlNode* pNode) : Part(pNode)
{
    m_n80 = 0;
    m_fAlpha = 1.0f;
    m_n94 = 0;
    m_n70 = 0;
    m_nTexture0 = -1;
    m_nTexture1 = -1;
    m_pMaterial = new Material;
    m_b7c = 0;
    m_bRenderEnvMap = 1;
    m_nId = g_nNextTriMeshId++;
    MaxTree* pModel = pNode->m_pTree->AsModel();
    if (pModel)
        m_bRenderEnvMap = pModel->m_bRenderEnvMap;
    g_nPartTriMeshCount++;
    MdlNodeTriMesh* pMesh = pNode->AsMdlNodeTriMesh();
    if (pMesh->m_n138) {
        m_pTVerts = (TriMeshTVert*)operator new(pMesh->m_nVerts * 8);
        char* pData = (char*)PoolLock(pMesh->m_pTree->AsModel()->m_nVertexBuffer,
                                      0, 0, 8);
        for (int i = 0; i < pMesh->m_nVerts; i++) {
            m_pTVerts[i].u = *(int*)(pData + (pMesh->m_nVertexStride * i +
                                              pMesh->m_nVertexOffset +
                                              pMesh->m_nTVertOffset));
            m_pTVerts[i].v = *(int*)(pData + (pMesh->m_nVertexStride * i +
                                              pMesh->m_nVertexOffset +
                                              pMesh->m_nTVertOffset) + 4);
        }
        PoolUnlock(pMesh->m_pTree->AsModel()->m_nVertexBuffer);
    } else {
        m_pTVerts = 0;
    }
}

// FUNCTION: SWKOTOR 0x00440320
PartLight::~PartLight()
{
    for (int i = 0; i < g_lGobs.num; i++) {
        CAurObjectLightContainer* pLights = g_lGobs[i]->m_pLights;
        if (pLights && pLights->m_lLights.Contains(this)) {
            int n = pLights->m_lLights.IndexOf(this);
            if (n >= 0) {
                pLights->m_lLights.DelIndex(n);
                pLights->m_lPriorities.DelIndex(n);
            }
        }
        if (pLights->m_pMainLight == this) {
            pLights->m_pMainLight = 0;
            pLights->m_nMainPriority = 0;
        }
    }
}

// FUNCTION: SWKOTOR 0x00445ac0 SYMBOL: ??_GPartLight@@UAEPAXI@Z
