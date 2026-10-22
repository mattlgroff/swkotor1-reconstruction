// small MdlNode members: node name lookup through the owning
// model's name table, and the skin post-process hook.
#include "g/mdl_ctor.h"

// FUNCTION: SWKOTOR 0x00438a00
char* MdlNode::GetName()
{
    if (m_pTree->AsModel())
        return m_pTree->AsModel()->m_lStrings[m_n04];
    return m_pTree->AsAnimation()->m_pModel->m_lStrings[m_n04];
}

// Scratch lists shared by the skin post-process.
extern CExoArrayList<int> g_lSkinScratch0;      // 0x007a8a70
extern CExoArrayList<int> g_lSkinScratch1;      // 0x007a8a7c
extern CExoArrayList<int> g_lSkinScratch2;      // 0x007a8a88

// FUNCTION: SWKOTOR 0x0043b350
void MdlNodeSkin::InternalPostProcess()
{
    if (m_n180 > 0)
        return;
    BuildBoneData();
    g_lSkinScratch2.num = 0;
    g_lSkinScratch1.num = 0;
    g_lSkinScratch0.num = 0;
}
