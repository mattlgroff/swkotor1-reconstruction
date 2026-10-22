// CSWMiniObstacle (vtable 0x0075287C): constructor, destructor and the
// PlayAnimation slot. No auto_inline(off): the CResRef array constructor
// loop is only inlined without it.

#include "m/sw_mini_game_object.h"

// FUNCTION: SWKOTOR 0x00670640
CSWMiniObstacle::CSWMiniObstacle(CSWCMiniGame* pGame, CAurObject* pModel)
    : CSWMiniGameObject(3)
{
    m_pModel = pModel;
}

// FUNCTION: SWKOTOR 0x006706C0
void CSWMiniObstacle::PlayAnimation(const char* szAnim, float fSpeed, int nFlags)
{
    if (m_pModel) {
        m_pModel->PlayAnimation(szAnim, fSpeed, nFlags, 0);
    }
}

// FUNCTION: SWKOTOR 0x006706E0
// NONMATCH (3 diff insns): in the inlined CAurObjectRef destructor the
// original computes the list `this` (`lea ecx, [eax+4]`) before `push esi`,
// the same split as ~CSWMiniGameObject 0x0066C610. CExoArrayList::Remove is
// declared throw() so the destructor keeps the original's EH states.
CSWMiniObstacle::~CSWMiniObstacle()
{
    if (m_pModel) {
        delete (CAurObject*)m_pModel;
    }
}
