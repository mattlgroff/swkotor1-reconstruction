#include "n/object_control_family.h"

// FUNCTION: SWKOTOR 0x0063C170
int CSWCObject::AnimationStationary(unsigned short nAnimation)
{
    int nValue = 0;
    unsigned short nRow = m_pAnimationBase ? m_pAnimationBase->ServerToClientAnimation(nAnimation) : nAnimation;
    C2DA* pTable = g_pRules->m_p2DArrays->m_pAnimations;
    if (pTable->GetINTEntry(nRow,g_nAnimStationaryColumn,&nValue))
        return nValue != 0;
    return 1;
}

// FUNCTION: SWKOTOR 0x0063C1E0
int CSWCObject::AnimationPause(unsigned short nAnimation)
{
    int nValue = 0;
    unsigned short nRow = m_pAnimationBase ? m_pAnimationBase->ServerToClientAnimation(nAnimation) : nAnimation;
    C2DA* pTable = g_pRules->m_p2DArrays->m_pAnimations;
    if (pTable->GetINTEntry(nRow,g_nAnimPauseColumn,&nValue))
        return nValue != 0;
    return 0;
}

// FUNCTION: SWKOTOR 0x0063C240
int CSWCObject::AnimationWalk(unsigned short nAnimation)
{
    int nValue = 0;
    unsigned short nRow = m_pAnimationBase ? m_pAnimationBase->ServerToClientAnimation(nAnimation) : nAnimation;
    C2DA* pTable = g_pRules->m_p2DArrays->m_pAnimations;
    if (pTable->GetINTEntry(nRow,g_nAnimWalkingColumn,&nValue))
        return nValue != 0;
    return 0;
}

// FUNCTION: SWKOTOR 0x0063C2A0
int CSWCObject::AnimationRun(unsigned short nAnimation)
{
    int nValue = 0;
    unsigned short nRow = m_pAnimationBase ? m_pAnimationBase->ServerToClientAnimation(nAnimation) : nAnimation;
    C2DA* pTable = g_pRules->m_p2DArrays->m_pAnimations;
    if (pTable->GetINTEntry(nRow,g_nAnimRunningColumn,&nValue))
        return nValue != 0;
    return 0;
}

// FUNCTION: SWKOTOR 0x0063C410
int CSWCObject::AnimationOverlay(unsigned short nAnimation)
{
    int nValue = 0;
    unsigned short nRow = m_pAnimationBase ? m_pAnimationBase->ServerToClientAnimation(nAnimation) : nAnimation;
    C2DA* pTable = g_pRules->m_p2DArrays->m_pAnimations;
    if (pTable->GetINTEntry(nRow,g_nAnimOverlayColumn,&nValue))
        return nValue != 0;
    return 0;
}

// FUNCTION: SWKOTOR 0x0063C4F0
int CSWCObject::AnimationParry(unsigned short nAnimation)
{
    int nValue = 0;
    unsigned short nRow = m_pAnimationBase ? m_pAnimationBase->ServerToClientAnimation(nAnimation) : nAnimation;
    C2DA* pTable = g_pRules->m_p2DArrays->m_pAnimations;
    if (pTable->GetINTEntry(nRow,g_nAnimParryColumn,&nValue))
        return nValue != 0;
    return 0;
}

// FUNCTION: SWKOTOR 0x0063C550
int CSWCObject::AnimationAttack(unsigned short nAnimation)
{
    int nValue = 0;
    unsigned short nRow = m_pAnimationBase ? m_pAnimationBase->ServerToClientAnimation(nAnimation) : nAnimation;
    C2DA* pTable = g_pRules->m_p2DArrays->m_pAnimations;
    if (pTable->GetINTEntry(nRow,g_nAnimAttackColumn,&nValue))
        return nValue != 0;
    return 0;
}

// FUNCTION: SWKOTOR 0x0063C5B0
int CSWCObject::AnimationHideEquippedItems(unsigned short nAnimation)
{
    int nValue = 0;
    unsigned short nRow = m_pAnimationBase ? m_pAnimationBase->ServerToClientAnimation(nAnimation) : nAnimation;
    C2DA* pTable = g_pRules->m_p2DArrays->m_pAnimations;
    if (pTable->GetINTEntry(nRow,g_nAnimHideEquippedItemsColumn,&nValue))
        return nValue != 0;
    return 0;
}

// FUNCTION: SWKOTOR 0x0063C300
int CSWCObject::AnimationLooping(unsigned short nAnimation)
{
    if (m_nType == 5 && ((nAnimation >= 0x578 && nAnimation <= 0x5f7) ||
                        (nAnimation >= 0x640 && nAnimation <= 0x6bf)))
        return 1;
    int nValue = 0;
    unsigned short nRow = m_pAnimationBase ? m_pAnimationBase->ServerToClientAnimation(nAnimation) : nAnimation;
    C2DA* pTable = g_pRules->m_p2DArrays->m_pAnimations;
    if (pTable->GetINTEntry(nRow,g_nAnimLoopingColumn,&nValue))
        return nValue != 0;
    return 0;
}

// FUNCTION: SWKOTOR 0x0063C380
int CSWCObject::AnimationFireAndForget(unsigned short nAnimation)
{
    if ((m_nType == 5 || m_nType == 9) && ((nAnimation >= 0x3e8 && nAnimation <= 0x467) ||
                                        (nAnimation >= 0x4b0 && nAnimation <= 0x52f)))
        return 1;
    int nValue = 0;
    unsigned short nRow = m_pAnimationBase ? m_pAnimationBase->ServerToClientAnimation(nAnimation) : nAnimation;
    C2DA* pTable = g_pRules->m_p2DArrays->m_pAnimations;
    if (pTable->GetINTEntry(nRow,g_nAnimFireForgetColumn,&nValue))
        return nValue != 0;
    return 0;
}

// FUNCTION: SWKOTOR 0x0063C470
int CSWCObject::AnimationPlayOutOfPlace(unsigned short nAnimation)
{
    if ((nAnimation >= 0x4b0 && nAnimation <= 0x52f) ||
        (nAnimation >= 0x640 && nAnimation <= 0x6bf))
        return 1;
    return 0;
}

// FUNCTION: SWKOTOR 0x0063C4A0
int CSWCObject::AnimationDialog(unsigned short nAnimation)
{
    if (g_pAppManager->m_pClientExoApp->GetInGameGui()->IsFireForgetDialogAnimation(nAnimation) ||
        g_pAppManager->m_pClientExoApp->GetInGameGui()->IsLoopingDialogAnimation(nAnimation))
        return 1;
    return 0;
}
