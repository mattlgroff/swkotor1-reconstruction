// model-side list helpers: the out-of-line CExoArrayList
// instantiations for faces / parts and the MaxTree free lists
// (0x00444ee0-0x00445310).
#include "g/mdl_node.h"

class Part;

// FUNCTION: SWKOTOR 0x00444ee0 SYMBOL: ??1?$CExoArrayList@VMaxFace_PostProcess@@@@QAE@XZ
// FUNCTION: SWKOTOR 0x00444f10 SYMBOL: ?SetSize@?$CExoArrayList@VMaxFace_PostProcess@@@@QAEXH@Z
template class CExoArrayList<MaxFace_PostProcess>;

// FUNCTION: SWKOTOR 0x00444f70 SYMBOL: ??1?$CExoArrayList@VTriFace@@@@QAE@XZ
template CExoArrayList<TriFace>::~CExoArrayList();

// FUNCTION: SWKOTOR 0x00444fa0 SYMBOL: ?SetSize@?$CExoArrayList@PAVPart@@@@QAEXH@Z
template void CExoArrayList<Part*>::SetSize(int n);

// FUNCTION: SWKOTOR 0x00445190
void MaxTree::AddToFreeTokenList(char* p)
{
    m_lFreeTokenList.Add(p);
}

// FUNCTION: SWKOTOR 0x004451d0
void MaxTree::ResetFreeLists()
{
    m_lFreeList.SetSize(0);
    m_lFreeTokenList.SetSize(0);
}

// FUNCTION: SWKOTOR 0x004452d0
void MdlNode::AddToFreeList(void* p)
{
    m_pTree->m_lFreeList.Add(p);
}

// FUNCTION: SWKOTOR 0x00445310
void MdlNode::AddToFreeTokenList(char* p)
{
    m_pTree->m_lFreeTokenList.Add(p);
}
