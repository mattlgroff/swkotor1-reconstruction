// CSWCPlaceable light model: placeables.2da LightColor / LightNode /
// LightOffset columns and lightcolor.2da RED / GREEN / BLUE.
#include "k/sw_c_placeable_door.h"

CAurObject* __cdecl NewCAurObject(const char*, const char*, int, int);  // 0x00449CC0

// Called once from the placeable's model setup; descriptive name.
// FUNCTION: SWKOTOR 0x006826E0
int CSWCPlaceable::LoadLight()
{
    Vector vColor;
    CExoString sLightNode;
    CExoString sUnused;
    int nLightColor;
    if (!g_pRules->Get2DArrays()->GetPlaceables()->GetINTEntry(m_nAppearance, "LightColor", &nLightColor))
        return 1;
    CSWCArea* pArea = 0;
    if (g_pAppManager->GetClientExoApp()->GetModule()) {
        pArea = g_pAppManager->GetClientExoApp()->GetModule()->GetArea();
        if (pArea) {
            unsigned long oid = m_oid;
            m_pLight = (CAurObject*)pArea->GetPlaceableObjectLight(oid);
        }
    }
    if (!g_pRules->Get2DArrays()->GetPlaceables()->GetCExoStringEntry(m_nAppearance, "LightNode", &sLightNode))
        sLightNode = "impact";
    CAurObject* pModel = GetModel(0xff, 1);
    if (!pModel)
        return 0;
    if (!m_pLight) {
        Quaternion qOrientation;
        Vector vPosition;
        Vector vOffset;
        m_pLight = NewCAurObject("fx_placeable01", "placeablelight", 0, 0);
        if (!m_pLight)
            return 0;
        g_pRules->Get2DArrays()->GetLightColors()->GetFLOATEntry(nLightColor, "RED", &vColor.x);
        g_pRules->Get2DArrays()->GetLightColors()->GetFLOATEntry(nLightColor, "GREEN", &vColor.y);
        g_pRules->Get2DArrays()->GetLightColors()->GetFLOATEntry(nLightColor, "BLUE", &vColor.z);
        if (!g_pRules->Get2DArrays()->GetPlaceables()->GetFLOATEntry(m_nAppearance, "LightOffsetX", &vOffset.x))
            vOffset.x = 0.0f;
        if (!g_pRules->Get2DArrays()->GetPlaceables()->GetFLOATEntry(m_nAppearance, "LightOffsetY", &vOffset.y))
            vOffset.y = 0.0f;
        if (!g_pRules->Get2DArrays()->GetPlaceables()->GetFLOATEntry(m_nAppearance, "LightOffsetZ", &vOffset.z))
            vOffset.z = 3.0f;
        m_pLight->GetNodeTransform("root", &vPosition, &qOrientation);
        m_pLight->SetNodeTransform("root", vPosition + vOffset, qOrientation);
    }
    m_pLight->AttachToObject(pModel, "root", 0);
    if (pArea) {
        unsigned long oid = m_oid;
        pArea->RemovePlaceableObjectLight(oid, 0);
    }
    return 1;
}
