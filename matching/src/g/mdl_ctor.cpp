// MdlNode family constructors.
#include "g/mdl_ctor.h"

// FUNCTION: SWKOTOR 0x0044a300
MdlNodeEmitter::MdlNodeEmitter(char* szName) : MdlNode(szName)
{
    m_nType = 5;
    m_n104 = 0;
    memset(m_szChunkName, 0, sizeof(m_szChunkName));
    memset(m_sz10b, 0, 32);
    m_n6c = 0;
    m_n50 = 0;
    m_n100 = 0;
    m_n54 = 0;
    m_n58 = 0;
    m_n10a = 0;
    m_n5c = 0;
    m_n12c = 0;
    m_n108 = 0;
    m_n60 = 0;
    m_n64 = 0;
    m_n68 = 0;
    m_szUpdate[0] = 0;
    m_szRender[0] = 0;
    m_szBlend[0] = 0;
    m_szTexture[0] = 0;
    m_szChunkName[0] = 0;
}

// FUNCTION: SWKOTOR 0x0044a3f0
MdlNodeLight::MdlNodeLight(char* szName) : MdlNode(szName)
{
    m_n98 = 1;
    m_n9c = 1;
    m_na0 = 1;
    m_na8 = 1;
    m_n94 = 0;
    m_na4 = 0;
    m_n50 = 0;
    m_nType = 3;
    m_n90 = 5;
}

// FUNCTION: SWKOTOR 0x0044a5c0
MdlNodeReference::MdlNodeReference(char* szName) : MdlNode(szName)
{
    m_nType = 0x11;
}

// FUNCTION: SWKOTOR 0x0044a8b0
MdlNodeAABB::MdlNodeAABB(char* szName) : MdlNodeTriMesh(szName)
{
    m_pTree = 0;
    m_bShadow = 0;
    m_bRender = 0;
    m_nType = 0x221;
}

void __cdecl MdlNodeTriMesh_Fn50(MdlNode* pNode);   // 0x0043e480
void __cdecl MdlNodeTriMesh_Fn54(MdlNode* pNode);   // 0x0043e490
extern Vector g_vDefaultDiffuse;                    // 0x0078e33c
extern Vector g_vDefaultAmbient;                    // 0x0078e348

// FUNCTION: SWKOTOR 0x0044a640
MdlNodeTriMesh::MdlNodeTriMesh(char* szName) : MdlNode(szName)
{
    m_pfn50 = MdlNodeTriMesh_Fn50;
    m_pfn54 = MdlNodeTriMesh_Fn54;
    m_n14c = -1;
    m_n150 = 0;
    m_a154[0] = -1;
    m_a154[1] = -1;
    m_a154[2] = -1;
    m_a154[3] = -1;
    m_a154[4] = -1;
    m_a154[5] = -1;
    m_a154[6] = -1;
    m_a154[7] = -1;
    m_a154[8] = -1;
    m_a154[9] = -1;
    m_a154[10] = -1;
    m_n18c = 0;
    m_n190 = 0;
    m_na4 = 0;
    m_b188 = 0;
    m_bRender = 1;
    m_bShadow = 1;
    m_b184 = 0;
    m_b186 = 0;
    m_b185 = 0;
    m_vDiffuse = g_vDefaultDiffuse;
    m_vAmbient = g_vDefaultAmbient;
    m_nTexture = -1;
    m_szTexture0[0] = 0;
    m_szTexture1[0] = 0;
    m_n180 = 0;
    m_n182 = 0;
    m_fRadius = 0;
    m_n12c = 0;
    m_pPostProcess = 0;
    m_n138 = 0;
    m_n194 = 0;
    m_p198 = 0;
    m_nType = 0x21;
}

void __cdecl MdlNodeAnimMesh_Fn50(MdlNode* pNode);  // 0x0043e420
void __cdecl MdlNodeAnimMesh_Fn54(MdlNode* pNode);  // 0x0043e430
void __cdecl MdlNodeSkin_Fn50(MdlNode* pNode);      // 0x0043e440
void __cdecl MdlNodeSkin_Fn54(MdlNode* pNode);      // 0x0043e450
void __cdecl MdlNodeDangly_Fn54(MdlNode* pNode);    // 0x0043e460
void __cdecl MdlNodeDangly_Fn50(MdlNode* pNode);    // 0x0043e470
int AurCreateVertexBuffer(int a, int nSize, int c, int nFormat,
                          int* pHandle);            // 0x00423ee0

// FUNCTION: SWKOTOR 0x0044abc0
MdlNodeAnimMesh::MdlNodeAnimMesh(char* szName) : MdlNodeTriMesh(szName)
{
    m_fSampleRate = 0;
    m_szTexture0[0] = 0;
    m_szTexture1[0] = 0;
    m_n1cc = 0;
    m_n1d0 = 0;
    m_p1c4 = 0;
    m_p1c8 = 0;
    m_pfn50 = MdlNodeAnimMesh_Fn50;
    m_pfn54 = MdlNodeAnimMesh_Fn54;
    m_nType = 0xa1;
}

// FUNCTION: SWKOTOR 0x0044ac70
MdlNodeSkin::MdlNodeSkin(char* szName) : MdlNodeTriMesh(szName)
{
    m_p1b0 = 0;
    m_n1a8 = -1;
    m_n1ac = -1;
    m_nType = 0x61;
    m_pfn50 = MdlNodeSkin_Fn50;
    m_pfn54 = MdlNodeSkin_Fn54;
}

// FUNCTION: SWKOTOR 0x0044ad80
MdlNodeLightsaber::MdlNodeLightsaber(char* szName) : MdlNodeTriMesh(szName)
{
    m_nType = 0x821;
    AurCreateVertexBuffer(0, 0x100, 2, 0x1012, &m_nVertexBuffer);
    AurCreateVertexBuffer(0, 0x640, 2, 0x1010, &m_nIndexBuffer);
}

// FUNCTION: SWKOTOR 0x0044ae00
MdlNodeDanglyMesh::MdlNodeDanglyMesh(char* szName) : MdlNodeTriMesh(szName)
{
    m_nType = 0x121;
    m_pfn54 = MdlNodeDangly_Fn54;
    m_pfn50 = MdlNodeDangly_Fn50;
}
