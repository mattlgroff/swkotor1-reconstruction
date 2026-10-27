// LightManager light prioritisation slots +0x4c..+0x54
// (0x004A3CF0-0x004A3F6F).
#include "i/light_manager.h"


// FUNCTION: SWKOTOR 0x004A3CF0
void LightManager::PrioritizeIllumination(unsigned char bForce)
{
    m_lEnabled44.num = 0;
    Vector vZero(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < m_lRenderLights.num; i++) {
        if (!bForce && m_lRenderLights.element[i]->m_fPriority > 1.0f) {
            continue;
        }
        Light* pLight = m_lRenderLights.element[i];
        if (pLight->m_fRadius == 0.0f) {
            continue;
        }
        if (pLight->m_vColor.IsEqual(vZero)) {
            continue;
        }
        m_lEnabled44.Add(m_lRenderLights.element[i]);
    }
    qsort(m_lEnabled44.element, m_lEnabled44.num, sizeof(Light*), LightCompare);
    m_lBumpMapLights.num = 0;
    for (int j = 0; j < m_lEnabled44.num && j < 3; j++) {
        Light* pLight = m_lEnabled44.element[j];
        if (!pLight->m_bAmbientOnly && pLight->m_bAffectStatic) {
            m_lBumpMapLights.Add(pLight);
            return;
        }
    }
}

// FUNCTION: SWKOTOR 0x004A3E30
void LightManager::PrioritizeShadow()
{
    m_lShadowLights.num = 0;
    for (int i = 0; i < m_lRenderLights.num; i++) {
        Light* pLight = m_lRenderLights.element[i];
        if (!pLight->m_bCastShadow || pLight->m_bAmbientOnly) {
            continue;
        }
        if (pLight->m_vColor.x > 0.1f || pLight->m_vColor.y > 0.1f || pLight->m_vColor.z > 0.1f) {
            m_lShadowLights.Add(pLight);
        }
    }
    qsort(m_lShadowLights.element, m_lShadowLights.num, sizeof(Light*), LightCompare);
}

// FUNCTION: SWKOTOR 0x004A3EE0
void LightManager::PrioritizeBump()
{
    m_lBumpMapLights.num = 0;
    for (int i = 0; i < m_lRenderLights.num; i++) {
        Light* pLight = m_lRenderLights.element[i];
        if (!pLight->m_bAmbientOnly && pLight->m_bAffectStatic) {
            m_lBumpMapLights.Add(pLight);
        }
    }
    qsort(m_lBumpMapLights.element, m_lBumpMapLights.num, sizeof(Light*), LightCompare);
    if (m_lBumpMapLights.num > 1) {
        m_lBumpMapLights.num = 1;
    }
}
