// CSWCCreature::GetMenuPoint, worker C s8 (translation-unit-local layout).

#include "c/exo_base.h"

struct Vector {
    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    Vector operator-() const { return Vector(-x, -y, -z); }

    float x, y, z;
};

inline float DotProduct(const Vector& a, const Vector& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

class Quaternion {
public:
    float w, x, y, z;

    // Rotated +Z axis (inline).
    Vector GetZAxis() const
    {
        return Vector(2.0f * (y * w + x * z),
                      2.0f * (y * z - w * x),
                      1.0f - 2.0f * (y * y + x * x));
    }
};

class CAurCamera {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual Vector* GetPosition(Vector* pOut);      // 0x20 (descriptive)
    virtual Quaternion* GetOrientation(Quaternion* pOut);   // 0x24 (descriptive)
    virtual Vector* MapToScreen(Vector* pOut, Vector vPoint, int b);    // 0x28 (descriptive)
};

class CSWGuiManager {
public:
    char pad_0x0[0x6c];
    short m_nScreenWidth;                           // 0x6c
    short m_nScreenHeight;                          // 0x6e
};

class CClientExoApp {
public:
    CAurCamera* GetModuleCamera();                  // 0x005ED560
    CSWGuiManager* GetGuiManager();                 // 0x005EDA40
};

class CAppManager {
public:
    void* m_pUnknown00;
    CClientExoApp* m_pClientExoApp;                 // 0x04
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CSWSCreature {
public:
    char pad_0x0[0x18];
    CExoString m_sTag;                              // 0x18
};

class CSWSObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual CSWSCreature* AsSWSCreature();          // 0x30
};

class CSWCCreature {
public:
    unsigned char GetMenuPoint(Vector& vPoint);

    CSWSObject* GetServerObject();                  // 0x0063D4B0
    CSWSCreature* GetServerCreature()
    {
        return GetServerObject() ? GetServerObject()->AsSWSCreature() : 0;
    }

    void* m_pVtable;                                // 0x00
    char pad_0x4[0x24 - 0x4];
    Vector m_vPosition;                             // 0x24
};

// Projects the creature's radial-menu anchor (position plus a height offset,
// higher for Zaalbar and HK-47) to screen pixels. Returns 1 when the point
// lies in front of the camera, 2 when behind.
// NONMATCH (6 diff insns): x87 operand order of two commutative products
// (w*x in the Z axis, the x/y terms of the first dot product); the frame,
// temporaries and control flow are identical. Source operand order has no
// effect (16 permutations tried).
// FUNCTION: SWKOTOR 0x0060FF30
unsigned char CSWCCreature::GetMenuPoint(Vector& vPoint)
{
    CAurCamera* pCamera = g_pAppManager->m_pClientExoApp->GetModuleCamera();
    Vector vForward;
    {
        Quaternion qTemp;
        vForward = -pCamera->GetOrientation(&qTemp)->GetZAxis();
    }
    vPoint = m_vPosition;
    CSWSCreature* pServer = GetServerCreature();
    if (pServer && (pServer->m_sTag == "zaalbar" || pServer->m_sTag == "hk47")) {
        vPoint.z += 2.0f;
    } else {
        vPoint.z += 1.75f;
    }
    unsigned char nResult;
    {
        Vector vTemp;
        Vector* pCamPos = pCamera->GetPosition(&vTemp);
        nResult = 1;
        if (!(DotProduct(vForward, vPoint) > DotProduct(*pCamPos, vForward))) {
            nResult = 2;
        }
    }
    {
        Vector vTemp;
        vPoint = *pCamera->MapToScreen(&vTemp, vPoint, 1);
    }
    vPoint.x *= g_pAppManager->m_pClientExoApp->GetGuiManager()->m_nScreenWidth;
    vPoint.y *= g_pAppManager->m_pClientExoApp->GetGuiManager()->m_nScreenHeight;
    return nResult;
}
