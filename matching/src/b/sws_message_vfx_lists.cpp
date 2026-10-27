// CSWSMessage::AssignVisualEffectLists (round s9): rebuild a player's last
// update copy of a looping visual effect list.
#include "b/sws_message.h"
#include "b/exo_array_list.h"

class CLoopingVisualEffect {
public:
    int GetIsBeam();                                        // 0x004CA9D0

    int operator==(const CLoopingVisualEffect& other) const
    {
        return m_nId == other.m_nId && m_oidTarget == other.m_oidTarget && m_nBodyPart == other.m_nBodyPart;
    }

    unsigned short m_nId;                                   // 0x0
    char pad_0x2[0x4 - 0x2];
    unsigned long m_oidTarget;                              // 0x4
    unsigned char m_nBodyPart;                              // 0x8 (name guessed)
};

static inline void AddUnique(CExoArrayList<CLoopingVisualEffect*>* pList, CLoopingVisualEffect* pEffect)
{
    int nCount = 0;
    for (int i = 0; i < pList->num; i++) {
        if (pList->element[i] == pEffect) {
            nCount++;
        }
    }
    if (!nCount) {
        pList->Add(pEffect);
    }
}

// FUNCTION: SWKOTOR 0x00570530
void CSWSMessage::AssignVisualEffectLists(CExoArrayList<CLoopingVisualEffect*>* pTarget,
                                          CExoArrayList<CLoopingVisualEffect*>* pSource)
{
    for (int i = 0; i < pTarget->num; i++) {
        delete pTarget->element[i];
        pTarget->element[i] = NULL;
    }
    pTarget->SetSize(0);
    for (int i = 0; i < pSource->num; i++) {
        int bFound = 0;
        for (int j = 0; j < pTarget->num; j++) {
            if (pSource->element[i]->m_nId == pTarget->element[j]->m_nId) {
                if (!pSource->element[i]->GetIsBeam() || *pSource->element[i] == *pTarget->element[j]) {
                    bFound = 1;
                }
            }
        }
        if (!bFound) {
            CLoopingVisualEffect* pEffect = new CLoopingVisualEffect;
            pEffect->m_nId = pSource->element[i]->m_nId;
            pEffect->m_oidTarget = pSource->element[i]->m_oidTarget;
            pEffect->m_nBodyPart = pSource->element[i]->m_nBodyPart;
            AddUnique(pTarget, pEffect);
        }
    }
}
