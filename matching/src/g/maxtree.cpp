// MaxTree / Model construction and teardown.
#include "g/mdl_ctor.h"

void __cdecl MaxTreeDestroyRoot(MaxTree* pTree);    // 0x0044a8e0
void __cdecl MaxTreeFn43e190(MaxTree* pTree);       // 0x0043e190
void __cdecl ModelDestroy(MaxTree* pTree);          // 0x0044aa60
void __cdecl ModelFn43e1e0(MaxTree* pTree);         // 0x0043e1e0
void __cdecl ModelRegister(Model* pModel);          // 0x0046a5f0
int PoolRequest(int nType, int nSize, int nFlags, int nUnused,
                int* pId);                          // 0x00423ee0

// FUNCTION: SWKOTOR 0x0044a900
MaxTree::MaxTree(char* szName)
{
    strcpy(m_szName, szName);
    m_pRoot = 0;
    m_nType = 1;
    m_n48 = 0;
    m_n2c = 0;
    m_lTextures.SetSize(0);
    m_lFreeList.SetSize(0);
    m_pfnDestroy = MaxTreeDestroyRoot;
    m_pfn04 = MaxTreeFn43e190;
}

// FUNCTION: SWKOTOR 0x0043f730 SYMBOL: ??1MaxTree@@QAE@XZ

// FUNCTION: SWKOTOR 0x0044a9c0
void MaxTree::FreeTheFreeList()
{
    int i;
    for (i = 0; i < m_lFreeList.num; i++)
        m_lFreeList[i]->AsMdlNodeLight()->BinaryInternalDestructor();
    m_lFreeList.SetSize(0);
    for (i = 0; i < m_lTextures.num; i++) {
        PoolRelease(m_lTextures[i]);
        m_lTextures[i] = -1;
    }
    m_lTextures.SetSize(0);
}

// FUNCTION: SWKOTOR 0x0044aa70
Model::Model(char* szName) : MaxTree(szName)
{
    m_vBBoxMin = Vector(-5.0f, -5.0f, -1.0f);
    m_nType = 2;
    m_pSuperModel = 0;
    m_n50 = 0;
    m_b52 = 0;
    m_b53 = 1;
    m_fRadius = 7.0f;
    m_n54 = 0;
    m_fScale = 1.0f;
    m_vBBoxMax = Vector(5.0f, 5.0f, 10.0f);
    ModelRegister(this);
    m_b88 = 0;
    m_pfnDestroy = ModelDestroy;
    m_pfn04 = ModelFn43e1e0;
    m_nTexture = -1;
    m_nTexture = PoolRequest(0, 1, 2, 0, &m_nTexture);
    m_nb0 = 0;
    m_pFont = 0;
    m_lStrings.num = 0;
}

// NONMATCH (5): PoolRelease argument and the string-loop base use edx /
// eax instead of the original eax / edx.
// FUNCTION: SWKOTOR 0x0043f790
Model::~Model()
{
    if (m_nType) {
        int i = m_lAnimations.num;
        m_nType = 0;
        while (i) {
            MaxTree::operator delete(m_lAnimations[--i]);
            m_lAnimations.num = i;
        }
        PoolRelease(m_nTexture);
        m_nTexture = -1;
        for (i = 0; i < m_lStrings.num; i++)
            delete m_lStrings[i];
    }
}
