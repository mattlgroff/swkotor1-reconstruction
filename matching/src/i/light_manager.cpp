// LightManager members (aurlightmanager.o). No Mac names for these
// slots; names follow the playable build's descriptions.
#include "i/light_manager.h"

// FUNCTION: SWKOTOR 0x004A36B0
void LightManager::SetupLightsForThisPositionAndRadius(Vector* pPos, float fRadius)
{
    m_lRenderLights.num = 0;
    for (int i = 0; i < m_lLights.num; i++) {
        Light* pLight = m_lLights.element[i];
        if (pLight->m_fRadius <= 0.0f) {
            continue;
        }
        if (!pLight->m_bAffectDynamic && !pLight->m_bAffectStatic) {
            continue;
        }
        float fDX = pLight->m_vPosition.x - pPos->x;
        if (fDX < 0.0f) {
            fDX = -fDX;
        }
        float fGapX = fDX - pLight->m_fRadius;
        float fDY = pLight->m_vPosition.y - pPos->y;
        if (fDY < 0.0f) {
            fDY = -fDY;
        }
        if (fGapX < fRadius && fDY - pLight->m_fRadius < fRadius) {
            AddRenderLight(pLight);
        }
    }
}

// FUNCTION: SWKOTOR 0x004A4F10
int LightManager::DisableThisLight(Light* pLight)
{
    CExoArrayList<Light*> lights;
    if (m_bUseList74) {
        lights = m_lEnabled74;
    } else {
        lights = m_lEnabled44;
    }
    if (!lights.Contains(pLight)) {
        return 0;
    }
    glDisable(lights.IndexOf(pLight) + 0x4001);
    return 1;
}

// FUNCTION: SWKOTOR 0x004A4FF0
int LightManager::EnableThisLight(Light* pLight)
{
    CExoArrayList<Light*> lights;
    if (m_bUseList74) {
        lights = m_lEnabled74;
    } else {
        lights = m_lEnabled44;
    }
    if (!lights.Contains(pLight)) {
        return 0;
    }
    glEnable(lights.IndexOf(pLight) + 0x4001);
    return 1;
}

// FUNCTION: SWKOTOR 0x004A5680
void LightManager::DisableBumpMapLights()
{
    CExoArrayList<Light*> lights(m_lBumpMapLights);
    int nCount = lights.num;
    Light** pLights = lights.element;
    for (int i = 0; i < nCount; i++) {
        DisableThisLight(pLights[i]);
        m_bBumpMapLightsDisabled = 1;
        g_bVertexProgramRefresh = 1;
    }
}

// FUNCTION: SWKOTOR 0x004A5710
void LightManager::EnableBumpMapLights()
{
    CExoArrayList<Light*> lights(m_lBumpMapLights);
    int nCount = lights.num;
    Light** pLights = lights.element;
    for (int i = 0; i < nCount; i++) {
        EnableThisLight(pLights[i]);
        g_bVertexProgramRefresh = 1;
    }
    m_bBumpMapLightsDisabled = 0;
}

// NONMATCH (5 insns): x87 scheduling; the original issues the call's
// `add esp, 4` after `fadd [ebp]`.
// FUNCTION: SWKOTOR 0x004A3770
int LightManager::GetNearestLights(Scene* pScene, Vector vPos, float fRadius, int bDynamicOnly)
{
    if (!g_bDynamicLights) {
        return 0;
    }
    int nEnabled = 0;
    for (int i = 0; i < m_lLights.num && nEnabled < 3; i++) {
        Light* pLight = m_lLights.element[i];
        if (bDynamicOnly && !pLight->m_bAffectStatic) {
            continue;
        }
        if (!pLight->m_bAffectStatic && !pLight->m_bAffectDynamic) {
            continue;
        }
        Vector& vLightPos = pLight->m_vPosition;
        Vector vDelta;
        vDelta.x = vLightPos.x - vPos.x;
        vDelta.y = vLightPos.y - vPos.y;
        vDelta.z = vLightPos.z - vPos.z;
        if (FUN_004ab100(&vDelta) > fRadius + pLight->m_fRadius) {
            continue;
        }
        if (pLight->m_fRadius <= 0.0f) {
            continue;
        }
        glEnable(nEnabled + 0x4001);
        SetLightGL(nEnabled, vLightPos, pLight->m_vColor * pLight->m_fMultiplier, pLight->m_fRadius,
                   pLight->m_bAmbientOnly);
        nEnabled++;
    }
    for (int j = nEnabled; j < 3; j++) {
        glDisable(j + 0x4001);
    }
    return nEnabled;
}
