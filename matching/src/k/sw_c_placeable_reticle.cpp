// CSWCPlaceable::GetReticlePoint: screen position of the "rootdummy" node
// for placeables whose server object carries a creature (0x44c); otherwise
// the CSWCObject version. Not in the function table: starts after
// 0x00682F20's padding.
#include "k/sw_c_placeable_door.h"

inline float operator*(const Vector& a, const Vector& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct KQuaternion : Quaternion {
    Vector ToDirection();                           // 0x0043E250
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
    virtual Vector GetPosition();                   // +0x20 (placeholder name)
    virtual KQuaternion GetOrientation();           // +0x24 (placeholder name)
    virtual Vector WorldToScreen(Vector v, int b);  // +0x28 (placeholder name)
};

class CSWGuiManager {
public:
    char pad_0x0[0x6c];
    short m_nScreenWidth;                           // +0x6C (placeholder name)
    short m_nScreenHeight;                          // +0x6E (placeholder name)
};

// FUNCTION: SWKOTOR 0x00682FF0
unsigned char CSWCPlaceable::GetReticlePoint(Vector& v)
{
    CSWSPlaceable* pServer = ServerInline();
    if (pServer && pServer->m_pCreature) {
        CAurObject* pModel = m_pModel->GetModel(0xff);
        if (pModel) {
            CAurPart* pPart = pModel->FindPart("rootdummy");
            if (pPart) {
                Quaternion qOrientation;
                pPart->GetWorldTransform(v, qOrientation);
                CSWCModuleCamera* pCamera = g_pAppManager->m_pClientExoApp->GetModuleCamera();
                Vector vBack = -pCamera->GetOrientation().ToDirection();
                unsigned char nResult;
                if (vBack * v > vBack * pCamera->GetPosition()) {
                    nResult = 1;
                }
                else {
                    nResult = 2;
                }
                v = pCamera->WorldToScreen(v, 1);
                v.x *= g_pAppManager->m_pClientExoApp->GetGuiManager()->m_nScreenWidth;
                v.y *= g_pAppManager->m_pClientExoApp->GetGuiManager()->m_nScreenHeight;
                return nResult;
            }
        }
    }
    return CSWCObject::GetReticlePoint(v);
}
