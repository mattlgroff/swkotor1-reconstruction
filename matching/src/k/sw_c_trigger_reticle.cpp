// CSWCTrigger::GetReticlePoint: screen position of the "lookathook" node,
// same body as CSWCObject::GetReticlePoint (0x0063D560, bucket n). Not in
// the function table: starts after 0x00691A30's padding.
#include "k/sw_c_trigger.h"

inline Vector operator-(const Vector& v)
{
    return Vector(-v.x, -v.y, -v.z);
}

inline float operator*(const Vector& a, const Vector& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

class Quaternion {
public:
    float w, x, y, z;
    Quaternion() { z = 0.0f; y = 0.0f; x = 0.0f; w = 1.0f; }
    Vector ToDirection();                                            // 0x0043E250
};

class CAurPart {
public:
    virtual void v00(); virtual void v04(); virtual void v08();
    virtual void GetWorldTransform(Vector& vPosition, Quaternion& qOrientation);  // +0x0C (placeholder name)
};

class CSWCModuleCamera {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual Vector GetPosition();                                    // +0x20 (placeholder name)
    virtual Quaternion GetOrientation();                             // +0x24 (placeholder name)
    virtual Vector WorldToScreen(Vector v, int b);                   // +0x28 (placeholder name)
};

class CSWGuiManager {
public:
    char pad_0x0[0x6c];
    short m_nScreenWidth;                                            // +0x6C (placeholder name)
    short m_nScreenHeight;                                           // +0x6E (placeholder name)
};

// FUNCTION: SWKOTOR 0x00691B00
unsigned char CSWCTrigger::GetReticlePoint(Vector& v)
{
    int nResult;
    int bFound = 0;
    CAurObject* pModel = m_pModel->GetModel(0xff);
    if (pModel) {
        Quaternion qOrientation;
        CAurPart* pPart = pModel->FindPart("lookathook");
        if (pPart) {
            pPart->GetWorldTransform(v, qOrientation);
            CSWCModuleCamera* pCamera = g_pAppManager->m_pClientExoApp->GetModuleCamera();
            Vector vBack = -pCamera->GetOrientation().ToDirection();
            if (vBack * v > vBack * pCamera->GetPosition()) {
                nResult = 1;
            }
            else {
                nResult = 2;
            }
            v = g_pAppManager->m_pClientExoApp->GetModuleCamera()->WorldToScreen(v, 1);
            v.x *= g_pAppManager->m_pClientExoApp->GetGuiManager()->m_nScreenWidth;
            v.y *= g_pAppManager->m_pClientExoApp->GetGuiManager()->m_nScreenHeight;
            bFound = 1;
        }
    }
    if (!bFound) {
        nResult = GetCenterReticlePoint(v);
    }
    return (unsigned char)nResult;
}
