// Worker A round s9: CSWSCreature AI-state recomputation over the action queue.
#include "a/sws_creature.h"

// The name is the placeholder already recorded in symbols.csv (reference: ComputeAIState).
// FUNCTION: SWKOTOR 0x004F1460
void CSWSCreature::Fn4F1460()
{
    m_n9F2 = 0;
    m_oid9F4 = 0x7f000000;
    void* pos = m_pActions->m_pHead;
    while (pos) {
        CSWSObjectActionNode* pNode = (CSWSObjectActionNode*)m_pActions->GetAtPos(pos);
        unsigned long oidTarget = 0x7f000000;
        if (pNode->m_nActionId == 0xc) {
            oidTarget = pNode->m_n38;
        }
        ComputeAIStateOnAction(pNode->m_nActionId, oidTarget);
        m_pActions->GetNext(pos);
    }
    if (m_nCurrentAction != -1) {
        ComputeAIStateOnAction(m_nCurrentAction, m_oidCurrentActionTarget);
    }
}

extern const int EFFECT_TYPE_0746830;                // 0x00746830 (list-end sentinel, placeholder name)
extern const int EFFECT_TYPE_07466A4;                // 0x007466A4 (placeholder name)
extern const int EFFECT_TYPE_07466AC;                // 0x007466AC (placeholder name)
extern const int EFFECT_TYPE_0746684;                // 0x00746684 (placeholder name)
extern const int EFFECT_TYPE_07466B0;                // 0x007466B0 (placeholder name)
extern const int EFFECT_TYPE_07466D8;                // 0x007466D8 (placeholder name)
extern const int EFFECT_TYPE_0746788;                // 0x00746788 (placeholder name)
extern const int EFFECT_TYPE_074670C;                // 0x0074670C (placeholder name)
extern const int EFFECT_TYPE_0746700;                // 0x00746700 (placeholder name)
extern const int EFFECT_TYPE_0746794;                // 0x00746794 (placeholder name)
extern const int EFFECT_TYPE_0746798;                // 0x00746798 (placeholder name)
extern const int EFFECT_TYPE_07467A8;                // 0x007467A8 (placeholder name)
extern const int EFFECT_TYPE_07467AC;                // 0x007467AC (placeholder name)
extern const int EFFECT_TYPE_07467A0;                // 0x007467A0 (placeholder name)
extern const int EFFECT_TYPE_0746764;                // 0x00746764 (placeholder name)
extern const int EFFECT_TYPE_0746780;                // 0x00746780 (placeholder name)
extern const int EFFECT_TYPE_0746744;                // 0x00746744 (placeholder name)
extern const int EFFECT_TYPE_0746770;                // 0x00746770 (placeholder name)
extern const int EFFECT_TYPE_0746758;                // 0x00746758 (placeholder name)
extern const int EFFECT_TYPE_07466E4;                // 0x007466E4 (placeholder name)
extern const int EFFECT_TYPE_0746680;                // 0x00746680 (placeholder name)
extern const int EFFECT_TYPE_07466D4;                // 0x007466D4 (placeholder name)

// Virtual slot 0xd8; no functions.csv row (the 0x004F1460 row spans it).
// FUNCTION: SWKOTOR 0x004F14F0
void CSWSCreature::UpdateEffectPtrs()
{
    int nLastType = EFFECT_TYPE_0746830;
    for (int i = 0; i < m_appliedEffects.num; i++) {
        int nType = m_appliedEffects.element[i]->m_nType;
        if (nType != nLastType) {
            if (nType == EFFECT_TYPE_AC_INCREASE) {
                m_pStats->m_nEffectStart12E = i;
            } else if (nType == EFFECT_TYPE_07466A4) {
                m_pStats->m_nEffectStart130 = i;
            } else if (nType == EFFECT_TYPE_07466AC) {
                m_pStats->m_nEffectStart132 = i;
            } else if (nType == EFFECT_TYPE_0746684) {
                m_pStats->m_nEffectStart134 = i;
            } else if (nType == EFFECT_TYPE_07466B0) {
                m_pStats->m_nEffectStart136 = i;
            } else if (nType == EFFECT_TYPE_07466D8) {
                m_pStats->m_nEffectStart138 = i;
            } else if (nType == EFFECT_TYPE_0746788) {
                m_pStats->m_nEffectStart13C = i;
            } else if (nType == EFFECT_TYPE_0746738) {
                m_pStats->m_n140 = i;
            } else if (nType == EFFECT_TYPE_0746778) {
                m_pStats->m_nEffectStart13E = i;
            } else if (nType == EFFECT_TYPE_074670C) {
                m_pStats->m_nEffectStart144 = i;
            } else if (nType == EFFECT_TYPE_0746700) {
                m_pStats->m_nEffectStart142 = i;
            } else if (nType == EFFECT_TYPE_0746794) {
                m_pStats->m_nEffectStart146 = i;
            } else if (nType == EFFECT_TYPE_0746798) {
                m_pStats->m_nEffectStart148 = i;
            } else if (nType == EFFECT_TYPE_07467A8) {
                m_pStats->m_nEffectStart14A = i;
            } else if (nType == EFFECT_TYPE_07467AC) {
                m_pStats->m_nEffectStart14C = i;
            } else if (nType == EFFECT_TYPE_07467A0) {
                m_pStats->m_nEffectStart14E = i;
            } else if (nType == EFFECT_TYPE_0746764) {
                m_pStats->m_nEffectStart150 = i;
            } else if (nType == EFFECT_TYPE_0746780) {
                m_pStats->m_nEffectStart152 = i;
            } else if (nType == EFFECT_TYPE_0746744) {
                m_pStats->m_nEffectStart154 = i;
            } else if (nType == EFFECT_TYPE_0746770) {
                m_pStats->m_nEffectStart156 = i;
            } else if (nType == EFFECT_TYPE_0746758) {
                m_pStats->m_nEffectStart158 = i;
            } else if (nType == EFFECT_TYPE_07466E4) {
                m_pStats->m_nEffectStart15A = i;
            } else if (nType == EFFECT_TYPE_0746680) {
                m_pStats->m_nEffectStart15C = i;
            } else if (nType == EFFECT_TYPE_07466D4) {
                m_pStats->m_nEffectStart15E = i;
            } else if (nType == EFFECT_TYPE_0746768) {
                m_pStats->m_nStealthEffectStart = i;
            } else if (nType == EFFECT_TYPE_07466F0) {
                m_pStats->m_nSpeedEffectStart = i;
            }
        }
        nLastType = nType;
    }
}
