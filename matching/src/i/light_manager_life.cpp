// LightManager constructor and light list registration (slots
// +0x0c..+0x14, 0x004A38B0-0x004A3CEF).
#include "i/light_manager.h"

// FUNCTION: SWKOTOR 0x004A38B0
LightManager::LightManager()
{
    m_bUseList74 = 0;
    m_pShadowLight = 0;
    m_nA8 = 0;
    m_bBumpMapLightsDisabled = 0;
}

// FUNCTION: SWKOTOR 0x004A39F0
void LightManager::AddLight(Light* pLight)
{
    if (m_lLights.Contains(pLight)) {
        return;
    }
    Quaternion qOrient;
    pLight->GetWorldTransform(&pLight->m_vPosition, &qOrient);
    m_lLights.Add(pLight);
    if (!pLight->m_bAffectStatic) {
        m_lStaticLights.Add(pLight);
    }
    if (pLight->m_bDynamicType && (pLight->m_bAffectStatic || pLight->m_bAffectDynamic)) {
        m_lDynamicLights.Add(pLight);
    }
}

// FUNCTION: SWKOTOR 0x004A3AE0
void LightManager::RemoveLight(Light* pLight, unsigned char bAll)
{
    if (m_lLights.Contains(pLight)) {
        m_lLights.Remove(pLight);
        if (!pLight->m_bAffectStatic) {
            m_lStaticLights.Remove(pLight);
        }
        if (pLight->m_bDynamicType && (pLight->m_bAffectStatic || pLight->m_bAffectDynamic)) {
            m_lDynamicLights.Remove(pLight);
        }
    }
    if (m_lC4.Contains(pLight)) {
        m_lC4.Remove(pLight);
    }
    if (!bAll) {
        return;
    }
    if (m_lEnabled44.Contains(pLight)) {
        m_lEnabled44.Remove(pLight);
    }
    if (m_l68.Contains(pLight)) {
        m_l68.Remove(pLight);
    }
    if (m_lEnabled74.Contains(pLight)) {
        m_lEnabled74.Remove(pLight);
    }
    if (m_l80.Contains(pLight)) {
        m_l80.Remove(pLight);
    }
    if (m_l8c.Contains(pLight)) {
        m_l8c.Remove(pLight);
    }
    if (m_lAC.Contains(pLight)) {
        m_lAC.Remove(pLight);
    }
    if (m_lB8.Contains(pLight)) {
        m_lB8.Remove(pLight);
    }
}

// FUNCTION: SWKOTOR 0x004A3CA0
void LightManager::AddRenderLight(Light* pLight)
{
    if (m_lRenderLights.Contains(pLight)) {
        return;
    }
    m_lRenderLights.Add(pLight);
}

// FUNCTION: SWKOTOR 0x004A2BC0
LightManager::~LightManager()
{
}
