// CSWSCreature::GetMaxForcePoints (nwscreature.cpp).
#include "a/sws_creature.h"

extern const unsigned char CLASS_TYPE_JEDI_CONSULAR;    // 0x0074B31D

// NONMATCH: 43 diff: eax/ecx/edx naming in the level loop and stat-reload scheduling in the NPC branch.
// FUNCTION: SWKOTOR 0x004FD490
short CSWSCreature::GetMaxForcePoints()
{
    if (m_pStats->m_nRace == RACE_TYPE_0074BF10) return 0;
    if (m_pStats->m_bIsPC && GetServerExoApp()->GetPartyTable()->m_nF0 == -1) {
        if (CSWClass::IsForceUserClass(m_pStats->GetClass((unsigned char)(m_pStats->m_nNumClasses - 1)))) {
            short nTotal = 0;
            for (int i = 0; i < m_pStats->GetLevel(0); i++) {
                int nDie = m_pStats->GetLevelStats(i)->m_nForceDie;
                if (nDie) {
                    int nGain = m_pStats->m_nF2 + nDie + m_pStats->m_nF4;
                    if (nGain < 1) nGain = 1;
                    nTotal += nGain;
                }
            }
            if (m_pStats->HasFeat(0x74)) nTotal += 40;
            for (unsigned char n = 0; n < m_pStats->m_nNumClasses; n++) {
                if (m_pStats->GetClass(n) == CLASS_TYPE_JEDI_CONSULAR && GetHasEffectOfType(EFFECT_TYPE_074682C)) {
                    nTotal += 50;
                    break;
                }
            }
            return nTotal;
        }
    } else {
        short nTotal = m_pStats->m_nBaseForcePoints;
        char nMod = m_pStats->m_nF2;
        nTotal += m_pStats->GetLevel(0) * nMod;
        nMod = m_pStats->m_nF4;
        nTotal += m_pStats->GetLevel(0) * nMod;
        if (m_pStats->HasFeat(0x74)) nTotal += 40;
        for (unsigned char n = 0; n < m_pStats->m_nNumClasses; n++) {
            if (m_pStats->GetClass(n) == CLASS_TYPE_JEDI_CONSULAR && GetHasEffectOfType(EFFECT_TYPE_074682C)) {
                nTotal += 50;
                break;
            }
        }
        if (m_pStats->GetIsJedi()) {
            if (m_pStats->GetLevel(0) > nTotal) return m_pStats->GetLevel(0);
            return nTotal;
        }
    }
    return 0;
}
