// LightManager::GetShadowLights (0x004A4B10): picks the shadow casting
// lights into +0x98 and fades them in and out.
#include <stddef.h>
#include "i/light_manager.h"

// NONMATCH (27 insns, ~4 real): the original shares one `lea eax, [esi+98h]`
// return block between the mode-2 exit and the end, and keeps separate
// `lea/push` pairs in both final assignment branches; cl hoists &m_l98
// into edi and merges them.
// FUNCTION: SWKOTOR 0x004A4B10
CExoArrayList<Light*>* LightManager::GetShadowLights(int nMode)
{
    m_l98.num = 0;
    if (nMode == 2) {
        int nBest = 6;
        Light* pBest = NULL;
        for (int i = 0; i < m_lLights.num; i++) {
            Light* pLight = m_lLights.element[i];
            if (!pLight->m_bCastShadow || pLight->m_bAmbientOnly) {
                continue;
            }
            if (pLight->m_fRadius < 1000.0f && pLight->m_bAffectStatic) {
                continue;
            }
            if (pLight->m_pNode->m_nShadowPriority < nBest) {
                nBest = pLight->m_pNode->m_nShadowPriority;
                pBest = pLight;
            }
        }
        if (pBest) {
            m_l98.Add(pBest);
            m_pShadowLight = pBest;
        }
        return &m_l98;
    }
    for (int i = 0; m_l98.num < g_nMaxShadowLights && i < m_lShadowLights.num; i++) {
        Light* pLight = m_lShadowLights.element[i];
        if (nMode == 1 && !pLight->m_bAffectStatic) {
            continue;
        }
        if (pLight->m_fPriority > 1.0f) {
            continue;
        }
        m_l98.Add(pLight);
        if (nMode == 1) {
            break;
        }
    }
    CExoArrayList<Light*> lPrevious;
    if (nMode == 1) {
        lPrevious = m_lB8;
    }
    else {
        lPrevious = m_lAC;
    }
    for (int j = 0; j < m_l98.num; j++) {
        Light* pLight = m_l98.element[j];
        if (!lPrevious.Contains(pLight)) {
            pLight->m_fShadowFade = 0.0f;
        }
    }
    for (int k = 0; k < g_nMaxShadowLights && k < lPrevious.num; k++) {
        Light* pLight = lPrevious.element[k];
        if (m_l98.Contains(pLight)) {
            continue;
        }
        float fFade = pLight->m_fShadowFade - g_fFrameTime / g_fShadowLightFadeTime * 2.0f;
        if (0.0f > fFade) {
            fFade = 0.0f;
        }
        pLight->m_fShadowFade = fFade;
        if (fFade > 0.0f) {
            int n = m_l98.num;
            if (n == m_l98.array_size) {
                int nSize = m_l98.array_size ? m_l98.array_size * 2 : 8;
                Light** pOld = m_l98.element;
                m_l98.array_size = nSize;
                m_l98.element = new Light*[nSize];
                for (int m = 0; m < m_l98.num; m++) {
                    m_l98.element[m] = pOld[m];
                }
                if (pOld) {
                    delete[] pOld;
                }
            }
            m_l98.element[m_l98.num++] = pLight;
            for (; n > 0; n--) {
                m_l98.element[n] = m_l98.element[n - 1];
            }
            m_l98.element[0] = pLight;
        }
    }
    if (m_l98.num > g_nMaxShadowLights) {
        m_l98.num = g_nMaxShadowLights;
    }
    for (int c = 0; c < m_l98.num; c++) {
        float fFade = g_fFrameTime / g_fShadowLightFadeTime + m_l98.element[c]->m_fShadowFade;
        if (1.0f < fFade) {
            fFade = 1.0f;
        }
        m_l98.element[c]->m_fShadowFade = fFade;
    }
    if (nMode == 1) {
        m_lB8 = m_l98;
    }
    else {
        m_lAC = m_l98;
    }
    return &m_l98;
}
