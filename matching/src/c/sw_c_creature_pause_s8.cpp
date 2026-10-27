// CSWCCreature::CGPauseCycle, worker C s8 (nwccreature.o;
// translation-unit-local layout).

#include <stdlib.h>
#include "c/exo_base.h"

// Model of a creature part; only the slots used here.
class CGob {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14();
    virtual void PlayAnimation(const char* sName, float fSpeed, int nFlags, float fStart);   // 0x18 (descriptive)
    virtual void GetAnimationLength(const char* sName, float* pStart, float* pLength);    // 0x1c (descriptive)
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9C();
    virtual void vA0(); virtual void vA4(); virtual void vA8(); virtual void vAC();
    virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC();
    virtual void vC0(); virtual void vC4(); virtual void vC8(); virtual void vCC();
    virtual void vD0(); virtual void vD4(); virtual void vD8(); virtual void vDC();
    virtual void vE0(); virtual void vE4(); virtual void vE8(); virtual void vEC();
    virtual void vF0(); virtual void vF4(); virtual void vF8(); virtual void vFC();
    virtual void v100(); virtual void v104(); virtual void v108(); virtual void v10C();
    virtual void v110(); virtual void v114(); virtual void v118(); virtual void v11C();
    virtual void v120(); virtual void v124();
    virtual void SetAnimationBlend(int b);          // 0x128 (descriptive)
};

class CSWCAnimBase {
public:
    virtual void v00(); virtual void v04();
    virtual CGob* GetPartModel(int nPart);          // 0x08 (descriptive)

    char pad_0x4[0xc6 - 0x4];
    unsigned char m_nIdleState;                     // 0xc6 (descriptive, 1 = idle)
};

class CSWCCreature {
public:
    void CGPauseCycle(float fDelta, int bReset);

    char pad_0x0[0x68];
    CSWCAnimBase* m_pAnimBase;                      // 0x68
    char pad_0x6c[0x390 - 0x6c];
    unsigned long m_nPauseTimer;                    // 0x390 (descriptive)
    unsigned long m_nPauseDelay;                    // 0x394 (descriptive)
};

// Idle fidget timer: on reset, starts a random pause animation at a random
// point; otherwise, once the delay ran out, plays a random fidget followed
// by a pause animation on head and body and picks a new 10-30 s delay.
// NONMATCH (34 diff insns): MSVC keeps 1 in bl for the idle-state compare
// and the EH state store (the original uses immediates); everything after is
// the resulting one-byte shift. (An int out-parameter instead of the float
// fStart also made it keep 0 in esi.)
// FUNCTION: SWKOTOR 0x0060F7C0
void CSWCCreature::CGPauseCycle(float fDelta, int bReset)
{
    if (!m_pAnimBase || m_pAnimBase->m_nIdleState != 1) {
        return;
    }
    CExoString sPause;
    CExoString sFidget;
    float fStart = 0.0f;
    float fLength = 0.0f;
    if (bReset) {
        switch (rand() % 2) {
        case 0:
            sPause = "pause1";
            break;
        case 1:
            sPause = "pause2";
            break;
        }
        m_nPauseDelay = (rand() % 201 + 100) * 100;
        int nPercent = rand() % 100;
        CGob* pPart = m_pAnimBase->GetPartModel(0xff);
        if (pPart) {
            pPart->GetAnimationLength(sPause.CStr(), &fStart, &fLength);
            pPart->PlayAnimation(sPause.CStr(), 1.0f, 2, nPercent * fLength * 0.01f);
        }
        pPart = m_pAnimBase->GetPartModel(0xfe);
        if (pPart) {
            pPart->PlayAnimation(sPause.CStr(), 1.0f, 2, nPercent * fLength * 0.01f);
        }
    } else {
        m_nPauseTimer += (int)(fDelta * 1000.0f);
        if (m_nPauseTimer >= m_nPauseDelay) {
            switch (rand() % 2) {
            case 0:
                sPause = "pause1";
                break;
            case 1:
                sPause = "pause2";
                break;
            }
            switch (rand() % 6) {
            case 0:
                sFidget = "greeting";
                break;
            case 1:
                sFidget = "hturnl";
                break;
            case 2:
                sFidget = "hturnr";
                break;
            case 3:
                sFidget = "pausebrd";
                break;
            case 4:
                sFidget = "pausesh";
                break;
            case 5:
                sFidget = "pause3";
                break;
            }
            CGob* pPart = m_pAnimBase->GetPartModel(0xff);
            if (pPart) {
                pPart->GetAnimationLength(sFidget.CStr(), &fStart, &fLength);
                pPart->SetAnimationBlend(1);
                pPart->PlayAnimation(sFidget.CStr(), 1.0f, 0x21, 0.0f);
                m_nPauseDelay += (int)(fLength * 1000.0f);
                pPart->PlayAnimation(sPause.CStr(), 1.0f, 0x60, 0.0f);
            }
            pPart = m_pAnimBase->GetPartModel(0xfe);
            if (pPart) {
                pPart->SetAnimationBlend(1);
                pPart->PlayAnimation(sFidget.CStr(), 1.0f, 0x21, 0.0f);
                pPart->PlayAnimation(sPause.CStr(), 1.0f, 0x60, 0.0f);
            }
            m_nPauseTimer = 0;
            m_nPauseDelay = (rand() % 201 + 100) * 100;
        }
    }
}
