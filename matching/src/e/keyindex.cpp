// NewController::GetKeyIndex (0x00482B70) and Part child
// broadcast (0x00484260). Separate from newcontroller.cpp so its callers
// keep the out-of-line call.
#include "e/mdlanim.h"

// FUNCTION: SWKOTOR 0x00482B70
void NewController::GetKeyIndex(float fTime, MdlNode* pNode, int* pIndex, float* pFrac)
{
    *pIndex = 0;
    *pFrac = 1.0f;
    while (*pIndex < m_nNumKeys) {
        if (!(fTime > pNode->m_lControllerData.element[m_nTimeIndex + *pIndex]))
            break;
        (*pIndex)++;
    }
    if (*pIndex != 0 && *pIndex != m_nNumKeys) {
        float* pKey = &pNode->m_lControllerData.element[m_nTimeIndex + *pIndex];
        *pFrac = (fTime - pKey[-1]) / (pKey[0] - pKey[-1]);
    }
}

// Forwards the state to every child part.
// FUNCTION: SWKOTOR 0x00484260
void Part::SetChildState(int nState)
{
    for (int i = 0; i < m_lChildren.num; i++)
        m_lChildren.element[i]->SetChildState(nState);
}

// FUNCTION: SWKOTOR 0x00481AA0
// (returned by value: the hidden result pointer is the first argument)
Quaternion Interpolate(const Quaternion* pA, const Quaternion* pB, float t)
{
    return Slerp(*pA, *pB, t);
}
