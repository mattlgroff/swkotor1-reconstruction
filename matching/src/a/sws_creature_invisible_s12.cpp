// CSWSCreature::GetInvisible (nwscreature.cpp): whether this creature's
// invisibility (or stealth) effects hide it from the viewing object.
#include "a/sws_creature.h"

class CSWSObject : public CGameObject {
};

// NONMATCH (85): register allocation (original: this in ebp, viewer in edi, i and nRace
// spilled, rules byte loaded into bl before the second GetInteger(1)); flow matches.
// FUNCTION: SWKOTOR 0x00501950
int CSWSCreature::GetInvisible(CSWSObject* pObject)
{
    if (!pObject->AsSWSCreature()) {
        return 0;
    }
    CSWSCreature* pViewer = pObject->AsSWSCreature();
    int nRace = pViewer->m_pStats->m_nRace;
    int nType = pViewer->m_pStats->m_n17E;
    for (int i = m_pStats->m_n140; i < m_appliedEffects.num; i++) {
        CGameEffect* pEffect = m_appliedEffects.element[i];
        if (pEffect->m_nType > EFFECT_TYPE_0746778) {
            return 0;
        }
        if (pEffect->m_nType == EFFECT_TYPE_0746738) {
            if (nRace != pEffect->GetInteger(1) && g_pRules->m_nAA != pEffect->GetInteger(1)) {
                continue;
            }
            if (nType != pEffect->GetInteger(3) && pEffect->GetInteger(3) != 0) {
                continue;
            }
            {
                if (pEffect->GetInteger(0) == 1 || pEffect->GetInteger(0) == 4) {
                    if (pViewer->m_n8EC & 5) {
                        return 0;
                    }
                    return 1;
                }
                if (pEffect->GetInteger(0) == 2 && (pViewer->m_n8EC & 6)) {
                    return 0;
                }
                return 1;
            }
        } else if (pEffect->m_nType == EFFECT_TYPE_0746778) {
            if (pViewer->GetAIStateReputation(m_idSelf) == 2 &&
                (nRace == pEffect->GetInteger(1) || g_pRules->m_nAA == pEffect->GetInteger(1)) &&
                (nType == pEffect->GetInteger(3) || pEffect->GetInteger(3) == 0)) {
                CSWSCreatureListEntry* pEntry = pViewer->GetFromList920(m_idSelf);
                if (pEntry) {
                    if (pEntry->m_n2 == 1) {
                        return (unsigned char)~pViewer->m_n8EC >> 2 & 1;
                    }
                    if (pEntry->m_n2 == 2) {
                        return 0;
                    }
                    if (pEntry->m_n2 == 0) {
                        if (pViewer->SavingThrowRoll(3, pEffect->GetInteger(4), 0, 0x7f000000, 1, 0, 0)) {
                            pViewer->UpdateObjectId(m_idSelf, 0, 0, 2, 0);
                        } else {
                            PacifyCreature(pViewer->m_idSelf);
                            pViewer->UpdateObjectId(m_idSelf, 0, 0, 1, 0);
                            return 1;
                        }
                    }
                } else {
                    if (pViewer->SavingThrowRoll(3, pEffect->GetInteger(4), 0, 0x7f000000, 1, 0, 0)) {
                        pViewer->UpdateObjectId(m_idSelf, 0, 0, 2, 0);
                    } else {
                        PacifyCreature(pViewer->m_idSelf);
                        pViewer->UpdateObjectId(m_idSelf, 0, 0, 1, 0);
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}
