// CSWSCreature disguise helpers (0x004EE010-0x004EE1FF).
#include "a/sws_creature.h"

// FUNCTION: SWKOTOR 0x004EE010
// NONMATCH (7): eax/ecx/edx naming in the SetPosition argument and the dead
// vSafe = m_vPosition copy.
int CSWSCreature::Disguise(int nAppearance, CGameEffect* pEffect, int bLoadingGame)
{
    CExoString sUnused;
    if (m_bDisguised) {
        return 0;
    }
    m_nDisguiseAppearance = m_pStats->m_nAppearanceType;
    m_pStats->m_nAppearanceType = (unsigned short)nAppearance;
    m_nAppearanceType = (unsigned short)nAppearance;
    UpdatePersonalSpace();
    UpdateAppearanceDependantInfo(1);
    m_bDisguised = 1;
    if (!bLoadingGame) {
        CSWSArea* pArea = GetArea();
        if (pArea) {
            Vector vSafe;
            vSafe.x = 0.0f;
            vSafe.y = 0.0f;
            vSafe.z = 0.0f;
            if (pArea->ComputeSafeLocation(m_vPosition, 20.0f, m_pFollowInfo, 1, &vSafe)) {
                SetPosition(vSafe, 1, 1, 0);
            } else {
                vSafe = m_vPosition;
            }
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004EE150
int CSWSCreature::UnDisguise(CGameEffect* pEffect)
{
    if (!m_bDisguised) {
        return 1;
    }
    m_pStats->m_nAppearanceType = m_nDisguiseAppearance;
    m_nAppearanceType = m_nDisguiseAppearance;
    UpdatePersonalSpace();
    UpdateAppearanceDependantInfo(1);
    m_bDisguised = 0;
    CSWSArea* pArea = GetArea();
    if (pArea) {
        Vector vSafe(0.0f, 0.0f, 0.0f);
        CSWSCreatureFollowInfo* pFollow = m_pFollowInfo;
        if (pArea->ComputeSafeLocation(m_vPosition, 20.0f, pFollow, 1, &vSafe)) {
            SetPosition(vSafe, 1, 1, 0);
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x004F28D0
void CSWSCreature::UpdateAmbientAnimationState()
{
    if (!GetDead() && !GetIsPossessed() && !GetDeadTemp()) {
        unsigned char nState = m_nSetState;
        if (nState == 6) {
            m_nAmbientAnimState = 0xb;
        } else if (nState == 4) {
            m_nAmbientAnimState = 2;
        } else if (nState == 7) {
            m_nAmbientAnimState = 5;
        } else if (nState == 9) {
            m_nAmbientAnimState = 9;
        } else if (nState == 0xc) {
            m_nAmbientAnimState = 0xa;
        } else if (nState == 0xd) {
            m_nAmbientAnimState = 0xd;
        } else if (nState == 0xa) {
            m_nAmbientAnimState = 6;
        } else if (nState == 8) {
            m_nAmbientAnimState = 7;
        } else if (nState == 3) {
            m_nAmbientAnimState = 8;
        } else {
            m_nAmbientAnimState = (nState == 5) ? 0xc : 0;
        }
    } else if (m_nAmbientAnimState != 4 && m_nAmbientAnimState != 3 &&
               m_nAmbientAnimState != 0xe) {
        unsigned char nNew = 4;
        if (m_nD4 == 0x279b || m_nD4 == 0x2785 || m_nD4 == 0x28a0) {
            nNew = 0xe;
        }
        m_nAmbientAnimState = nNew;
    }
    SetAnimation(10000);
}

// FUNCTION: SWKOTOR 0x004F6D70
// (name kept from earlier callers: it refreshes the stealth-mode effects)
// NONMATCH (35): the original loads the type word twice per iteration and
// keeps the count from the loop entry test in eax.
void CSWSCreature::UpdateAppearance()
{
    int bStealth = 0;
    if (m_nStealthMode == 1) {
        bStealth = 1;
    }
    int i = m_pStats->m_nStealthEffectStart;
    while (i < m_appliedEffects.num) {
        CGameEffect* pEffect = m_appliedEffects.element[i];
        if (pEffect->m_nType > EFFECT_TYPE_0746768) {
            break;
        }
        if (pEffect->m_nType == EFFECT_TYPE_0746768 &&
            (pEffect->m_nSubType & 7) == g_nDurationEquipped && pEffect->GetInteger(0) == 1) {
            RemoveEffectById(pEffect->GetID());
            i = m_pStats->m_nStealthEffectStart;
        } else {
            i++;
        }
    }
    int j = m_pStats->m_nSpeedEffectStart;
    while (j < m_appliedEffects.num) {
        CGameEffect* pEffect = m_appliedEffects.element[j];
        if (pEffect->m_nType > EFFECT_TYPE_07466F0) {
            break;
        }
        if (pEffect->m_nType == EFFECT_TYPE_07466F0 &&
            (pEffect->m_nSubType & 7) == g_nDurationEquipped && pEffect->m_oidCreator == m_idSelf) {
            RemoveEffectById(pEffect->GetID());
            j = m_pStats->m_nSpeedEffectStart;
        } else {
            j++;
        }
    }
    if (bStealth <= 0) {
        return;
    }
    CGameEffect* pNew = new CGameEffect(1);
    pNew->m_nType = (unsigned short)EFFECT_TYPE_0746768;
    pNew->m_nSubType = (pNew->m_nSubType & 0xfff8) | g_nDurationEquipped;
    pNew->m_bExpose = 0;
    pNew->SetCreator(m_idSelf);
    pNew->SetInteger(0, 1);
    ApplyEffect(pNew, 0, 0);
}
