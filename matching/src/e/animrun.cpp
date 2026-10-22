// AnimRun / GobEventHandler ctors and small Gob animation helpers
// (0x00481890 .. 0x00484CE0). Kept out of anim.cpp so the callers there
// keep their out-of-line calls.
#include <string.h>
#include "e/mdlanim.h"

class Animation;

class Model {
public:
    char pad_0x0[0x50];
    unsigned char m_bFlag0 : 1;                 // +0x50
    unsigned char m_bFlag1 : 1;
    unsigned char m_bFlag2 : 1;
    unsigned char m_bFlag3 : 1;
    unsigned char m_bFlag4 : 1;
    char pad_0x51[7];
    CExoArrayList<Animation*> m_lAnimations;    // +0x58
    Model* m_pSuperModel;                       // +0x64
    char pad_0x68[0x1c];
    float m_fAnimationScale;                    // +0x84
};

class AnimRun {
public:
    Animation* m_pAnimation;    // +0x00
    float m_fSpeed;             // +0x04
    int m_nFlags;               // +0x08
    float m_f0c;                // +0x0c
    float m_fTime;              // +0x10
    float m_f14;                // +0x14
    float m_fTransition;        // +0x18
    float m_fWeight;            // +0x1c
    float m_f20;                // +0x20
    AnimRun(Animation* pAnimation);
};

// FUNCTION: SWKOTOR 0x00481890
AnimRun::AnimRun(Animation* pAnimation)
{
    m_pAnimation = pAnimation;
    m_fSpeed = 1.0f;
    m_nFlags = 0;
    m_fTime = 0.0f;
    m_f0c = 0.0f;
    m_f14 = 1.0f;
    m_fTransition = 0.0f;
    m_f20 = 0.0f;
    m_fWeight = 1.0f;
}

// FUNCTION: SWKOTOR 0x00481A50
GobEventHandler::GobEventHandler(GobCallback pfn, char* szName, void* pData, float fTime,
                                 unsigned char bAlways)
{
    pfnCallback = pfn;
    if (!szName)
        szName = "";
    strcpy(szEvent, szName);
    pDefaultData = pData;
    this->fTime = fTime;
    this->bAlways = bAlways;
}

// FUNCTION: SWKOTOR 0x00481D60
unsigned char Gob::GetModelFlag2()
{
    if (m_pModel)
        return m_pModel->m_bFlag2;
    return 0;
}

// FUNCTION: SWKOTOR 0x00481D80
unsigned char Gob::GetModelFlag4()
{
    if (m_pModel)
        return m_pModel->m_bFlag4;
    return 0;
}

// FUNCTION: SWKOTOR 0x00484CA0
AnimRun* GetCurrentRun(Gob* pGob)
{
    for (int i = 0; i < pGob->m_lRuns.num; i++) {
        if (!(pGob->m_lRuns.element[i]->m_nFlags & 0x20) && !(pGob->m_lRuns.element[i]->m_nFlags & 0x8))
            return pGob->m_lRuns.element[i];
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00484CE0
float GetAnimationWeight(Model* pModel, Animation* pAnim)
{
    float fWeight = 1.0f;
    for (;;) {
        int nCount = 0;
        for (int i = 0; i < pModel->m_lAnimations.num; i++) {
            if (pModel->m_lAnimations.element[i] == pAnim)
                nCount++;
        }
        if (nCount)
            return fWeight;
        fWeight *= pModel->m_fAnimationScale;
        pModel = pModel->m_pSuperModel;
    }
}
