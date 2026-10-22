// Gob::SetLookAt (0x00487190). Own TU: the AnimRun ctor is
// inlined here, while anim.cpp's PlayAnimation calls it out of line.

#include "e/mdlanim.h"

class Animation {
public:
    char pad_0x0[0x54];
    float m_fTransition;        // +0x54
};

// One playing animation in Gob::m_lRuns (0x24 bytes).
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

    AnimRun(Animation* pAnimation)
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
};

class LookAtAnimation {
public:
    virtual ~LookAtAnimation();
    unsigned char m_bActive;    // +0x04
    Animation* m_pAnimation;    // +0x08
    AnimRun* m_pRun;            // +0x0c
    char pad_0x10[0x2c];

    LookAtAnimation(Gob* pTarget, Part* pTargetPart, Gob* pGob, Part* pPart, float fYaw,
                    float fPitch, float fX, float fY);     // 0x00487020
};

Part* FindPart(Part* pRoot, char* szName);     // 0x0043FE00

// FUNCTION: SWKOTOR 0x00487190
void Gob::SetLookAt(Gob* pTarget, char* szTargetPart, char* szPart, float fYaw, float fPitch,
                    float fX, float fY)
{
    Part* pTargetPart = FindPart(pTarget->m_pRootPart, szTargetPart);
    Part* pPart = FindPart(m_pRootPart, szPart);
    if (pTargetPart && pPart) {
        if (m_pLookAt) {
            AnimRun* pOld = m_pLookAt->m_pRun;
            if (pOld) {
                if (m_lRuns.Contains(pOld))
                    m_lRuns.Remove(pOld);
                delete pOld;
                LookAtAnimation* pLook = m_pLookAt;
                pLook->m_pRun = 0;
            }
            if (m_pLookAt)
                delete m_pLookAt;
            m_pLookAt = 0;
        }
        m_pLookAt = new LookAtAnimation(pTarget, pTargetPart, this, pPart, fYaw, fPitch, fX, fY);
        Animation* pAnim = m_pLookAt->m_pAnimation;
        AnimRun* pRun = new AnimRun(pAnim);
        pRun->m_fTime = 0.0f;
        pRun->m_f14 = 0.0f;
        pRun->m_fTransition = pAnim->m_fTransition;
        pRun->m_nFlags = 8;
        m_lRuns.Insert(pRun, 0);
        m_pLookAt->m_pRun = pRun;
    }
    else {
        StartLookAt();
    }
}
