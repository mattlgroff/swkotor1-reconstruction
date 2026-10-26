// Sound obstruction factor between two points (0x0050CCF0): walks the
// segment through every blocking object in the current module area,
// adding 0.4 per solid hit and 0.55 per door (capped at 1.0, ~100 ms).
#include "a/sws_area.h"

class CSWSModuleArea {
public:
    CSWSArea* GetArea();                            // 0x004C30F0
};

int operator==(const Vector& a, const Vector& b);   // 0x004AA980
Vector Normalize(const Vector& v);                  // 0x004AB180

class CExoTimers {
public:
    unsigned __int64 GetHighResolutionTimer();      // 0x005E9260
};

class CExoBase {
public:
    char pad_0x0[0x4];
    CExoTimers* m_pcExoTimers;                      // 0x4
};
extern CExoBase* g_pExoBase;                        // 0x007A39E0

// FUNCTION: SWKOTOR 0x0050CCF0
float __stdcall GetObstructionFactor(Vector vFrom, Vector vTo)
{
    unsigned long nStart = (unsigned long)(g_pExoBase->m_pcExoTimers->GetHighResolutionTimer() / 1000);
    float fFactor = 0.0f;
    if (vTo == Vector(0.0f, 0.0f, 0.0f)) {
        return 0.0f;
    }
    CServerExoApp* pServer = GetServerExoApp();
    if (pServer == NULL) {
        return 0.0f;
    }
    CSWSModule* pModule = pServer->GetModule();
    CSWSArea* pArea = NULL;
    if (pModule) {
        pArea = ((CSWSModuleArea*)pModule)->GetArea();
    }
    if (pArea == NULL) {
        return 0.0f;
    }
    unsigned long oidBlocker = 0x7f000000;
    Vector vHit(0.0f, 0.0f, 0.0f);
    unsigned long nElapsed = 0;
    int bClear = pArea->ClearLineOfSight(&vFrom, &vTo, &vHit, &oidBlocker, 0x7f000000, 0x7f000000,
                                         0, 0);
    while (!bClear && nElapsed < 100) {
        if (oidBlocker == 0x7f000000) {
            fFactor = fFactor + 0.4f;
        } else {
            CGameObject* pObject = GetServerExoApp()->GetGameObject(oidBlocker);
            if (pObject) {
                if (pObject->AsSWSPlaceable()) {
                    fFactor = fFactor + 0.4f;
                }
                if (pObject->AsSWSDoor()) {
                    fFactor = fFactor + 0.55f;
                }
            }
        }
        if (fFactor > 1.0f) {
            return 1.0f;
        }
        Vector vDirection = vTo - vHit;
        vFrom = vHit;
        Vector vStep = Normalize(vDirection) * 0.1f;
        unsigned long oidPrevious = oidBlocker;
        oidBlocker = 0x7f000000;
        vFrom = Vector(vStep.x + vFrom.x, vStep.y + vFrom.y, vStep.z + vFrom.z);
        nElapsed = (unsigned long)(g_pExoBase->m_pcExoTimers->GetHighResolutionTimer() / 1000) - nStart;
        bClear = pArea->ClearLineOfSight(&vFrom, &vTo, &vHit, &oidBlocker, oidPrevious, 0x7f000000,
                                         0, 0);
    }
    return fFactor;
}
