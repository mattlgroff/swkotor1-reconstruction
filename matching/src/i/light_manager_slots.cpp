// LightManager virtual slots +0x00..+0x3c (0x004A2DB0-0x004A30DF).
#include "i/light_manager.h"

// FUNCTION: SWKOTOR 0x004A2DB0
void LightManager::SetupLightsForThisGob(Vector* pPos, float fRadius)
{
    SetupLightsForThisPositionAndRadiusIn(pPos, fRadius, &m_l38);
    m_bUseList74 = 0;
}

// FUNCTION: SWKOTOR 0x004A2DE0
void LightManager::SetupLightsForThisRoom(Vector* pPos, float fRadius)
{
    SetupLightsForThisPositionAndRadiusIn(pPos, fRadius, &m_l38);
}

// FUNCTION: SWKOTOR 0x004A2E00
void LightManager::Reset()
{
    m_lRenderLights.num = 0;
}

// FUNCTION: SWKOTOR 0x004A2E10
void LightManager::UpdateLights()
{
    Quaternion qOrient;
    for (int i = 0; i < m_lLights.num; i++) {
        Light* pLight = m_lLights.element[i];
        pLight->GetWorldTransform(&pLight->m_vPosition, &qOrient);
    }
}

// NONMATCH (3 insns): the original pops ebp before the trailing loop's
// `cmp esi, 3`; loop shapes tried: for, while, do-while.
// FUNCTION: SWKOTOR 0x004A2E70
void LightManager::EnableHighestPriorityLights(int n)
{
    if (m_lEnabled44.num == 0) {
        return;
    }
    if (n >= m_lEnabled44.num) {
        n = m_lEnabled44.num;
    }
    int i;
    for (i = 0; i < n; i++) {
        glEnable(0x4001 + i);
        Light* pLight = m_lEnabled44.element[i];
        float fScale = pLight->m_fMultiplier;
        Vector vColor(fScale * pLight->m_vColor.x, fScale * pLight->m_vColor.y,
                      fScale * pLight->m_vColor.z);
        SetLightGL(i, m_lEnabled44.element[i]->m_vPosition, vColor,
                   m_lEnabled44.element[i]->m_fRadius, m_lEnabled44.element[i]->m_bAmbientOnly);
    }
    for (; i < 3; i++) {
        glDisable(0x4001 + i);
    }
}

// FUNCTION: SWKOTOR 0x004A2F30
void LightManager::PrioritizeLights(Vector* pPos, int bFlag)
{
    Vector vZero(0.0f, 0.0f, 0.0f);
    V58();
    for (int i = 0; i < m_lRenderLights.num; i++) {
        Light* pLight = m_lRenderLights.element[i];
        Vector vDelta(pLight->m_vPosition.x - pPos->x, pLight->m_vPosition.y - pPos->y, 0.0f);
        if (!VectorEqual(pLight->m_vColor, vZero) && pLight->m_fRadius > 0.0f) {
            pLight->m_fPriority = FUN_004ab100(&vDelta) / pLight->m_fRadius;
        }
        else {
            pLight->m_fPriority = 100000.0f;
        }
    }
    PrioritizeIllumination(bFlag);
}

// FUNCTION: SWKOTOR 0x004A3080
void LightManager::EnableLights()
{
    if (g_bDynamicLights) {
        return;
    }
    if (m_lEnabled44.num > 0) {
        g_nActiveLights = m_lEnabled44.num;
    }
    ControlLightsFade(&m_l68, &m_l80, &m_lEnabled44, 0);
    if (g_nHeadLight == 1) {
        EnableHeadLight();
    }
    else {
        AurDisableHeadLight();
    }
    m_bUseList74 = 0;
}

// FUNCTION: SWKOTOR 0x004A2FF0
void LightManager::EnableHeadLight()
{
    float fAlpha = 1.0f;
    glLightfv(0x4000, g_aGLLightParams[0], AurMakeColor(g_vHeadLightAmbient, fAlpha));
    fAlpha = 1.0f;
    glLightfv(0x4000, g_aGLLightParams[1], AurMakeColor(g_vHeadLightDiffuse, fAlpha));
    fAlpha = 1.0f;
    glLightfv(0x4000, g_aGLLightParams[3], AurMakeColor(g_pRenderCamera->m_vPosition, fAlpha));
    AurEnableHeadLight();
}

// FUNCTION: SWKOTOR 0x004A4480
void LightManager::EnableLightsDynamicOnly()
{
    if (g_bDynamicLights) {
        return;
    }
    CExoArrayList<Light*> lDynamic;
    for (int i = 0; i < m_lEnabled44.num; i++) {
        Light* pLight = m_lEnabled44.element[i];
        if (pLight->m_bAffectStatic == 1) {
            lDynamic.Add(pLight);
        }
    }
    g_nDynamicActiveLights = lDynamic.num;
    ControlLightsFade(&m_lEnabled74, &m_l8c, &lDynamic, 1);
    if (g_nHeadLight) {
        EnableHeadLight();
    }
    else {
        AurDisableHeadLight();
    }
    m_bUseList74 = 1;
}
