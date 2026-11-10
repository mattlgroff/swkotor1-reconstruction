// LightManager::ControlLightsFade (vtable +0x5c, 0x004A4580): a function
// start that functions.csv merges into the preceding entry. The frame time
// is read as a volatile (fld [global]; fsubr [fade]), the only form found
// that loads it before the fade value.
#include <stdlib.h>
#include "i/light_manager.h"

extern volatile float g_fFrameTime;                    // 0x0078E574 (placeholder name)
extern int g_bLightsFadeLocked;                         // 0x008305F4 (placeholder name)
extern int g_bBlackFog;                                 // 0x008305E0
extern int g_nMaxLights;                                // 0x007A1934 (placeholder name)
extern int g_nEnabledLights;                            // 0x008305E4 (placeholder name)
extern int g_bNegativeLight;                            // 0x008305F0 (placeholder name)

extern float g_fLightFadeRate;                          // 0x007A1940 (placeholder name)
extern float g_fShadowFadeRate;                         // 0x007A1944 (placeholder name)

// NONMATCH (142 insns): register allocation. cl keeps s_lWanted.num in a
// register across the first loop (the original reloads it), and the fade
// list stays in memory where the original keeps it in ebp.
// FUNCTION: SWKOTOR 0x004A3F70
void LightManager::EnableHighestPriorityLightsFade(int n, CAurObjectLightContainer* pContainer)
{
    CExoArrayList<Light*>* pLights = &pContainer->m_lLights;
    static CExoArrayList<Light*> s_lWanted;
    s_lWanted.num = 0;
    for (int i = 0; i < n && i < m_lEnabled44.num; i++) {
        s_lWanted.Add(m_lEnabled44.element[i]);
    }

    for (int j = pLights->num; j != 0;) {
        Light* pLight = pLights->element[j - 1];
        j--;
        LightNode* pNode = pLight->m_pNode->AsMdlNodeLight();
        if (s_lWanted.Contains(pLight)) {
            if (pNode->m_bNoFade) {
                float fFade = g_fLightFadeRate * g_fFrameTime + pContainer->m_lFades.element[j];
                pContainer->m_lFades.element[j] = __min(1.0f, fFade);
            }
            else {
                pContainer->m_lFades.element[j] = 1.0f;
            }
        }
        else {
            if (pNode->m_bNoFade) {
                float fFade = pContainer->m_lFades.element[j] - g_fLightFadeRate * g_fFrameTime;
                pContainer->m_lFades.element[j] = __max(0.0f, fFade);
            }
            else {
                pContainer->m_lFades.element[j] = 0.0f;
            }
            if (pContainer->m_lFades.element[j] <= 0.0f) {
                pLights->DelIndex(j);
                pContainer->m_lFades.DelIndex(j);
            }
        }
    }

    if (pLights->num < n && pLights->num < s_lWanted.num) {
        for (int k = 0; k < s_lWanted.num; k++) {
            if (pLights->num >= n) {
                break;
            }
            Light* pLight = s_lWanted.element[k];
            if (!pLights->Contains(pLight)) {
                pLights->Add(pLight);
                pContainer->m_lFades.AddOutOfLine(0.0f);
            }
        }
    }

    Light* pShadow;
    for (int m = 0; m < m_lEnabled44.num; m++) {
        pShadow = m_lEnabled44.element[m];
        if (pShadow->m_bCastShadow) {
            goto found;
        }
    }
    if (pContainer->m_pShadowLight) {
        float fFade = pContainer->m_fShadowFade - g_fShadowFadeRate * g_fFrameTime;
        pContainer->m_fShadowFade = __max(0.0f, fFade);
        if (pContainer->m_fShadowFade <= 0.0f) {
            pContainer->m_pShadowLight = NULL;
        }
    }
    goto compare;
found:
    if (pContainer->m_pShadowLight == NULL) {
        pContainer->m_pShadowLight = pShadow;
        float fFade = g_fShadowFadeRate * g_fFrameTime + pContainer->m_fShadowFade;
        pContainer->m_fShadowFade = __min(1.0f, fFade);
    }
    else if (pContainer->m_pShadowLight == pShadow) {
        float fFade = g_fShadowFadeRate * g_fFrameTime + pContainer->m_fShadowFade;
        pContainer->m_fShadowFade = __min(1.0f, fFade);
    }
    else {
        float fFade = pContainer->m_fShadowFade - g_fShadowFadeRate * g_fFrameTime;
        pContainer->m_fShadowFade = __max(0.0f, fFade);
        if (pContainer->m_fShadowFade <= 0.0f) {
            pContainer->m_pShadowLight = pShadow;
            fFade = g_fShadowFadeRate * g_fFrameTime + pContainer->m_fShadowFade;
            pContainer->m_fShadowFade = __min(1.0f, fFade);
        }
    }
compare:
    int bChanged = 1;
    if (pLights->num == m_lDC.num) {
        bChanged = 0;
        for (int c = 0; c < pLights->num; c++) {
            if (pLights->element[c] != m_lDC.element[c] || pContainer->m_lFades.element[c] != m_lE8.element[c]) {
                bChanged = 1;
                break;
            }
        }
    }
    if (bChanged) {
        m_lDC = *pLights;
        m_lE8 = pContainer->m_lFades;
        g_bVertexProgramRefresh = 1;
        int i;
        for (i = 0; i < pLights->num; i++) {
            Light* pLight = pLights->element[i];
            pLight->m_fFade = pContainer->m_lFades.element[i];
            glEnable(0x4001 + i);
            Vector vColor = pLight->m_vColor * pLight->m_fFade;
            SetLightGL(i, pLight->m_vPosition, vColor, pLight->m_fRadius, pLight->m_bAmbientOnly);
        }
        for (i = pLights->num; i < 3; i++) {
            glDisable(0x4001 + i);
        }
    }
    m_lEnabled44 = *pLights;
}

// NONMATCH (26 insns): the original reloads nLimit before `nRoom = nLimit -
// num` (ours reuses eax); the 4-byte shift then moves two loop alignments.
// FUNCTION: SWKOTOR 0x004A4580
void LightManager::ControlLightsFade(CExoArrayList<Light*>* pActive, CExoArrayList<Light*>* pPrevious,
                                     CExoArrayList<Light*>* pDesired, int bDynamicOnly)
{
    if (g_bLightsFadeLocked) {
        return;
    }
    int nLimit = g_nMaxLights;
    if (g_bBlackFog) {
        int nLead = 0;
        for (int k = 0; k < pDesired->num; k++) {
            if (pDesired->element[k]->m_bDynamicType != g_bBlackFog) {
                break;
            }
            nLead++;
        }
        if (nLimit >= nLead) {
            nLimit = nLead;
        }
    }

    int j = pActive->num;
    while (j != 0) {
        Light* pLight = pActive->element[j - 1];
        j--;
        float* pFade = bDynamicOnly ? &pLight->m_fDynamicFade : &pLight->m_fFade;
        if (pDesired->Contains(pLight)) {
            if (pDesired->IndexOf(pLight) >= nLimit) {
                *pFade -= g_fFrameTime;
                if (*pFade < 0.0f) {
                    *pFade = 0.0f;
                    pActive->DelIndex(j);
                }
            }
            else if (*pFade < 1.0f) {
                float fFade = *pFade + g_fFrameTime;
                *pFade = __min(1.0f, fFade);
            }
        }
        else if (m_lLights.Contains(pLight)) {
            *pFade -= g_fFrameTime;
            if (*pFade < 0.0f) {
                *pFade = 0.0f;
                pActive->DelIndex(j);
            }
        }
        else {
            pActive->DelIndex(j);
        }
    }

    int nNew = 0;
    for (int n = 0; n < nLimit && n < pDesired->num; n++) {
        Light* pLight = pDesired->element[n];
        if (!pLight->m_pNode->m_bNoFade && !pActive->Contains(pLight)) {
            nNew++;
        }
    }

    int i;
    int nRoom = nLimit - pActive->num;
    while (nNew > nRoom) {
        int bRemoved = 0;
        for (i = pActive->num - 1; i >= 0; i--) {
            Light* pLight = pActive->element[i];
            int nIndex;
            if (pDesired->Contains(pLight)) {
                nIndex = pDesired->IndexOf(pLight);
            }
            else {
                nIndex = nLimit + 1;
            }
            if (nIndex > nLimit) {
                pActive->DelIndex(i);
                bRemoved = 1;
                break;
            }
        }
        if (!bRemoved) {
            break;
        }
        nRoom++;
    }

    for (i = 0; i < nLimit; i++) {
        if (i >= pDesired->num) {
            break;
        }
        if (pActive->num >= nLimit) {
            break;
        }
        Light* pLight = pDesired->element[i];
        float* pFade = bDynamicOnly ? &pLight->m_fDynamicFade : &pLight->m_fFade;
        if (!pActive->Contains(pLight)) {
            pActive->AddInline(pLight);
            if (!pLight->m_pNode->m_bNoFade) {
                *pFade = 1.0f;
            }
            else if (pPrevious->Contains(pLight)) {
                *pFade = 0.0f;
            }
            else {
                *pFade = 0.0f;
            }
        }
        float fFade = *pFade + g_fFrameTime;
        *pFade = __min(1.0f, fFade);
    }

    qsort(pActive->element, pActive->num, sizeof(Light*), LightCompare);
    i = 0;
    g_nEnabledLights = __min(nLimit, pActive->num);
    g_bNegativeLight = 0;
    int bDynamic = bDynamicOnly;
    for (; i < pActive->num; i++) {
        float* pFade = bDynamic ? &pActive->element[i]->m_fDynamicFade : &pActive->element[i]->m_fFade;
        Vector vColor = pActive->element[i]->m_vColor * *pFade * pActive->element[i]->m_fMultiplier;
        if (vColor.x < 0.0f) {
            g_bNegativeLight = 1;
        }
        glEnable(0x4001 + i);
        SetLightGL(i, pActive->element[i]->m_vPosition, vColor, pActive->element[i]->m_fRadius,
                   pActive->element[i]->m_bAmbientOnly);
    }
    for (int n = i; n < 3; n++) {
        glDisable(0x4001 + n);
    }
    *pPrevious = *pDesired;
}
