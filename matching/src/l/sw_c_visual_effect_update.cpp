// CSWCVisualEffectOnObject per-frame updates with x87 math (0x006A4A80-
// 0x006A4C00).

#include "l/sw_c_visual_effect.h"

// FUNCTION: SWKOTOR 0x006A4A80
// NONMATCH: x87 register/spill choice in the Vector lerp differs (23 insns).
void CSWCVisualEffectOnObject::UpdateSelfIlluminationPulse(unsigned long nDelta)
{
    CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[0]);
    if (pGameObject) {
        CSWCObject* pObject = pGameObject->AsSWCObject();
        if (pObject) {
            if (m_nBeamTimer[3] > 1000) {
                m_nBeamTimer[3] -= 1000;
                m_bC7 = !m_bC7;
            }
            if (!m_bC7) {
                float t = (float)m_nBeamTimer[3] * 0.001f;
                m_vOffset = m_vColor1 * (1.0f - t) + m_vColor2 * t;
            } else {
                float t = (float)m_nBeamTimer[3] * 0.001f;
                m_vOffset = m_vColor1 * t + m_vColor2 * (1.0f - t);
            }
            pObject->SetColorOverlay(m_vOffset, 3, 0, 1);
            m_nBeamTimer[3] += nDelta;
        }
    }
}

// FUNCTION: SWKOTOR 0x006A4C00
// NONMATCH: the `m_bC7 == 3` fix-up block is laid out after the 6000 ms branch.
void CSWCVisualEffectOnObject::UpdateAlpha(unsigned long nDelta)
{
    CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidTargets[0]);
    if (pGameObject) {
        CSWCObject* pObject = pGameObject->AsSWCObject();
        if (pObject) {
            unsigned short nType = m_nA2 - 400;
            if (nType != 1) {
                if (nType != 4) {
                    return;
                }
                if (m_nBeamTimer[3] >= 1000) {
                    m_bC7 += (unsigned char)(m_nBeamTimer[3] / 1000);
                    m_bC7 %= 4;
                    m_nBeamTimer[3] %= 1000;
                    if (m_bC7 == 1) {
                        m_bC7 = 2;
                    } else if (m_bC7 == 3) {
                        m_bC7 = 0;
                    }
                }
            } else {
                if (m_nBeamTimer[3] >= 6000) {
                    m_bC7 += (unsigned char)(m_nBeamTimer[3] / 6000);
                    m_bC7 %= 4;
                    m_nBeamTimer[3] %= 6000;
                }
            }
            switch (m_bC7) {
            case 0:
                if (nType == 1) {
                    m_fCC = 1.0f - (float)m_nBeamTimer[3] * (1.0f / 6000.0f);
                    if (m_fCC < 0.0f) {
                        m_fCC = 0.0f;
                    }
                } else {
                    m_fCC = 1.0f - (0.5f * ((float)m_nBeamTimer[3] * 0.001f) + 0.2f);
                    if (m_fCC < 0.3f) {
                        m_fCC = 0.3f;
                    }
                }
                break;
            case 1:
                if (nType == 1) {
                    m_fCC = 0.0f;
                } else {
                    m_fCC = 0.3f;
                }
                break;
            case 2:
                if (nType == 1) {
                    m_fCC = (float)m_nBeamTimer[3] * (1.0f / 6000.0f);
                    if (m_fCC > 1.0f) {
                        m_fCC = 1.0f;
                    }
                } else {
                    m_fCC = 0.5f * ((float)m_nBeamTimer[3] * 0.001f) + 0.3f;
                    if (m_fCC > 0.8f) {
                        m_fCC = 0.8f;
                    }
                }
                break;
            case 3:
                if (nType == 1) {
                    m_fCC = 1.0f;
                } else {
                    m_fCC = 0.8f;
                }
                break;
            }
            pObject->SetAlpha(m_fCC, 0xff);
            m_nBeamTimer[3] += nDelta;
        }
    }
}
