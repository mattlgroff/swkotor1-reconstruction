// CSWCArea::AddPlaceableObjectLight, worker C s9
// (translation-unit-local layout; nwcarea.o).

#include "c/exo_base.h"

struct Vector {
    Vector() {}
    Vector(float a, float b, float c) : x(a), y(b), z(c) {}
    Vector operator+(const Vector& v) const { return Vector(x + v.x, y + v.y, z + v.z); }
    float x, y, z;
};

struct Quaternion {
    Quaternion() {}
    Quaternion(float a, float b, float c, float d) : w(a), x(b), y(c), z(d) {}
    float w, x, y, z;
};

class C2DA {
public:
    int GetFLOATEntry(int nRow, const CExoString& sColumn, float* pValue);  // 0x00413430
    int GetINTEntry(int nRow, const CExoString& sColumn, int* pValue);      // 0x00413660
};

class CTwoDimArrays {
public:
    char pad_0x0[0x2c];
    C2DA* m_pPlaceables;                            // 0x2c
    char pad_0x30[0x48 - 0x30];
    C2DA* m_pLightColor;                            // 0x48
};

class CSWRules {
public:
    char pad_0x0[0xb8];
    CTwoDimArrays* m_p2DArrays;                     // 0xb8
};

extern CSWRules* g_pRules;                          // 0x007A3A28

class CAurObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48();
    virtual void AddToScene(void* pScene);          // 0x4c (descriptive)
    virtual void v50(); virtual void v54(); virtual void v58();
    virtual Vector SetPosition(Vector vPosition);   // 0x5c (descriptive)
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94(); virtual void v98();
    virtual int SetNodeTransform(const char* sNode, Vector vPosition, Quaternion qOrientation);  // 0x9c (descriptive)
    virtual int GetNodeTransform(const char* sNode, Vector* pPosition, Quaternion* pOrientation);  // 0xa0 (descriptive)
};

CAurObject* NewCAurObject(const char* sModel, const char* sType, int a, int b);   // 0x00449CC0

struct CPlaceableLight {
    unsigned long m_oidPlaceable;                   // 0x00
    CAurObject* m_pLight;                           // 0x04
};

class CSWCArea {
public:
    int AddPlaceableObjectLight(unsigned long oid, unsigned char nAppearance, Vector vPosition);

    char pad_0x0[0x184];
    void* m_pScene;                                 // 0x184
    char pad_0x188[0x218 - 0x188];
    CExoArrayList<CPlaceableLight*> m_aPlaceableLights;  // 0x218
};

// NONMATCH: the original has an 8-aligned ebp frame (`and esp, -8`), so its
// state constant lives in ebx instead of ebp and every local shifts. Neither
// POD Vector/Quaternion nor a dead double triggers that frame; the body
// (lookups, record search, node transform, inline Add) follows the original.
// FUNCTION: SWKOTOR 0x00606E30
int CSWCArea::AddPlaceableObjectLight(unsigned long oid, unsigned char nAppearance, Vector vPosition)
{
    Quaternion qOrientation(1.0f, 0.0f, 0.0f, 0.0f);
    Vector vColor(0.0f, 0.0f, 0.0f);
    Vector vOffset(0.0f, 0.0f, 0.0f);
    Vector vNode(0.0f, 0.0f, 0.0f);
    int nLightColor;
    if (!g_pRules->m_p2DArrays->m_pPlaceables->GetINTEntry(nAppearance, CExoString("LightColor"), &nLightColor)) {
        return 1;
    }
    int bFound = 0;
    CPlaceableLight* pLight;
    for (int i = 0; i < m_aPlaceableLights.num; i++) {
        if (m_aPlaceableLights.element[i] && m_aPlaceableLights.element[i]->m_oidPlaceable == oid) {
            pLight = m_aPlaceableLights.element[i];
            bFound = 1;
            break;
        }
    }
    if (!bFound) {
        pLight = new CPlaceableLight;
        if (!pLight) {
            return 0;
        }
    }
    pLight->m_pLight = NewCAurObject("fx_placeable01", "placeablelight", 0, 0);
    if (!pLight->m_pLight) {
        delete pLight;
        return 0;
    }
    pLight->m_oidPlaceable = oid;
    g_pRules->m_p2DArrays->m_pLightColor->GetFLOATEntry(nLightColor, CExoString("RED"), &vColor.x);
    g_pRules->m_p2DArrays->m_pLightColor->GetFLOATEntry(nLightColor, CExoString("GREEN"), &vColor.y);
    g_pRules->m_p2DArrays->m_pLightColor->GetFLOATEntry(nLightColor, CExoString("BLUE"), &vColor.z);
    if (!g_pRules->m_p2DArrays->m_pPlaceables->GetFLOATEntry(nAppearance, CExoString("LightOffsetX"), &vOffset.x)) {
        vOffset.x = 0.0f;
    }
    if (!g_pRules->m_p2DArrays->m_pPlaceables->GetFLOATEntry(nAppearance, CExoString("LightOffsetY"), &vOffset.y)) {
        vOffset.y = 0.0f;
    }
    if (!g_pRules->m_p2DArrays->m_pPlaceables->GetFLOATEntry(nAppearance, CExoString("LightOffsetZ"), &vOffset.z)) {
        vOffset.z = 3.0f;
    }
    pLight->m_pLight->GetNodeTransform("root", &vNode, &qOrientation);
    pLight->m_pLight->SetNodeTransform("root", vNode + vOffset, qOrientation);
    pLight->m_pLight->SetPosition(vPosition);
    pLight->m_pLight->AddToScene(m_pScene);
    if (!bFound) {
        m_aPlaceableLights.Add(pLight);
    }
    return 1;
}
