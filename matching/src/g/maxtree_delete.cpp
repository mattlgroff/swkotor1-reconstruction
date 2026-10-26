// MaxTree::operator delete (0x0044aec0): frees a binary (relocated) tree
// image, or destroys an ASCII model / animation tree node by node.
#include "g/mdl_ctor.h"

class AurRes;

void __cdecl AurResClose(AurRes* pRes, int nSize);  // 0x0044c8f0

class SuperModelRef {
public:
    int DelRef(MaxTree* pUser);                     // 0x0043e210 (Model::DelRef)
};

class AnimationTree : public Animation {
public:
    ~AnimationTree();                               // 0x004850a0 (Animation::~Animation)
};

inline Model* MaxTree::AsModel()
{
    return (m_nType & 0x7f) == 2 ? (Model*)this : 0;
}

inline Animation* MaxTree::AsAnimation()
{
    return (m_nType & 0x7f) == 5 ? (Animation*)this : 0;
}

// FUNCTION: SWKOTOR 0x0044aec0
// NONMATCH (134): the binary-image half matches; the inlined node delete
// has the same double type read as ~MdlNode 0x0044a040 (see mdl_dtor.cpp),
// which MSVC CSEs here as well.
void MaxTree::operator delete(void* p)
{
    MaxTree* pTree = (MaxTree*)p;
    if (!pTree)
        return;
    if (pTree->m_nType & 0x80) {
        pTree->FreeTheFreeList();
        Model* pModel = (Model*)pTree;
        if (pModel->m_pSuperModel) {
            ((SuperModelRef*)pModel->m_pSuperModel)->DelRef(pTree);
            pModel->m_pSuperModel = 0;
        }
        pTree->m_pfnDestroy(pTree);
        if (pTree->AsModel()->m_pFont)
            AurResClose((AurRes*)pTree->AsModel()->m_pFont, 0);
        ::operator delete(pTree);
        return;
    }
    if (!pTree->m_nType)
        return;
    pTree->m_pfnDestroy(pTree);
    if (pTree->m_pRoot)
        MdlNodeDelete(pTree->m_pRoot);
    if (pTree->AsModel())
        delete pTree->AsModel();
    else if (pTree->AsAnimation())
        delete (AnimationTree*)pTree->AsAnimation();
}
