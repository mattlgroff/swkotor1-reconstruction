// CSWCCreature::GetReticlePoint, worker C s13 (translation-unit-local layout).
// The creature override (vtable slot 0x13c) of CSWCObject::GetReticlePoint
// (0x0063D560): the reticle sits on the "head_g" node of the body or head
// model, else on "rootdummy".

struct Vector {
    float x, y, z;
    Vector() { x = 0.0f; y = 0.0f; z = 0.0f; }
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    Vector operator-() const { return Vector(-x, -y, -z); }
};

inline float operator*(const Vector& a, const Vector& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

class Quaternion {
public:
    float w, x, y, z;
    Quaternion() { z = 0.0f; y = 0.0f; x = 0.0f; w = 1.0f; }
    Vector ToDirection();                           // 0x0043E250
};

class CAurPart {
public:
    virtual void v00(); virtual void v04(); virtual void v08();
    virtual void GetWorldTransform(Vector& vPosition, Quaternion& qOrientation);  // +0x0C (placeholder name)
};

class CAurObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9C();
    virtual void vA0(); virtual void vA4(); virtual void vA8(); virtual void vAC();
    virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC();
    virtual void vC0(); virtual void vC4(); virtual void vC8(); virtual void vCC();
    virtual void vD0(); virtual void vD4(); virtual void vD8(); virtual void vDC();
    virtual void vE0(); virtual void vE4(); virtual void vE8(); virtual void vEC();
    virtual void vF0(); virtual void vF4(); virtual void vF8(); virtual void vFC();
    virtual void v100(); virtual void v104(); virtual void v108();
    virtual CAurPart* FindPart(const char* sNode);  // +0x10C (placeholder name)
};

class CSWCModuleCamera {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual Vector GetPosition();                   // +0x20 (placeholder name)
    virtual Quaternion GetOrientation();            // +0x24 (placeholder name)
    virtual Vector WorldToScreen(Vector v, int b);  // +0x28 (placeholder name)
};

class CSWGuiManager {
public:
    char pad_0x0[0x6c];
    short m_nScreenWidth;                           // 0x6c (placeholder name)
    short m_nScreenHeight;                          // 0x6e (placeholder name)
};

class CClientExoApp {
public:
    CSWCModuleCamera* GetModuleCamera();            // 0x005ED560
    CSWGuiManager* GetGuiManager();                 // 0x005EDA40
};

class CAppManager {
public:
    void* m_pUnknown00;
    CClientExoApp* m_pClientExoApp;                 // 0x04
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CSWCAnimBase {
public:
    virtual void v00(); virtual void v04();
    virtual CAurObject* GetModel(unsigned char n);  // +0x08
};

class CSWCCreature {
public:
    virtual unsigned char GetReticlePoint(Vector& v);   // 0x13c

    char pad_0x4[0x68 - 0x4];
    CSWCAnimBase* m_pAnimationBase;                 // 0x68
};

// FUNCTION: SWKOTOR 0x0060FD80
unsigned char CSWCCreature::GetReticlePoint(Vector& v)
{
    unsigned char nResult = 0;
    CAurObject* pModel = m_pAnimationBase->GetModel(0xff);
    if (pModel) {
        CAurPart* pPart = pModel->FindPart("head_g");
        if (!pPart) {
            CAurObject* pHead = m_pAnimationBase->GetModel(0xfe);
            if (pHead) {
                pPart = pHead->FindPart("head_g");
            }
            if (!pPart) {
                pPart = pModel->FindPart("rootdummy");
            }
        }
        if (pPart) {
            Quaternion qOrientation;
            pPart->GetWorldTransform(v, qOrientation);
            CSWCModuleCamera* pCamera = g_pAppManager->m_pClientExoApp->GetModuleCamera();
            Vector vBack = -pCamera->GetOrientation().ToDirection();
            if (vBack * v > vBack * pCamera->GetPosition()) {
                nResult = 1;
            } else {
                nResult = 2;
            }
            v = pCamera->WorldToScreen(v, 1);
            v.x *= g_pAppManager->m_pClientExoApp->GetGuiManager()->m_nScreenWidth;
            v.y *= g_pAppManager->m_pClientExoApp->GetGuiManager()->m_nScreenHeight;
        }
    }
    return nResult;
}
