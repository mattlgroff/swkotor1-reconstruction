// Worker A round s7: CSWSCreature::GetIsInUseRange (nwscreature.cpp).
#include "a/sws_creature.h"

class CSWSObject {
public:
    CSWSArea* GetArea();                            // 0x004CB120
    char pad_0x0[0x8c];
    unsigned long m_oidArea;                        // 0x8c
};

// FUNCTION: SWKOTOR 0x004F6000
int CSWSCreature::GetIsInUseRange(unsigned long oidTarget, float fExtra, int nFlag)
{
    CGameObject* pObject = GetServerExoApp()->GetGameObject(oidTarget);
    Vector vPosition(0.0f, 0.0f, 0.0f);
    float fRange = 0.0f;
    if (!pObject || !pObject->AsSWSObject() || !((CSWSObject*)pObject->AsSWSObject())->GetArea()) {
        return 0;
    }
    unsigned long oidArea = m_oidArea;
    if (oidArea != ((CSWSObject*)pObject->AsSWSObject())->m_oidArea) {
        return 0;
    }
    fExtra = fExtra + 0.1f;
    if (pObject->AsSWSTrigger() && pObject->AsSWSTrigger()->m_bHasGeometry) {
        return pObject->AsSWSTrigger()->InTrigger(&m_vPosition);
    }
    GetUseRange(oidTarget, vPosition, fRange, 0);
    Vector vHit(0.0f, 0.0f, 0.0f);
    Vector vFrom = m_vPosition;
    unsigned long oidSelf = m_idSelf;
    vFrom.z = vFrom.z + 1.5f;
    Vector vTo = vPosition;
    vTo.z = vPosition.z + 1.5f;
    unsigned long oidHit;
    if (!((CSWSObject*)pObject->AsSWSObject())->GetArea()->ClearLineOfSight(&vFrom, &vTo, &vHit, &oidHit, oidSelf,
                                                                            oidTarget, nFlag, 0)) {
        return 0;
    }
    float fDX = vTo.x - vFrom.x;
    float fDY = vTo.y - vFrom.y;
    float fReach = fRange + fExtra;
    return fDY * fDY + fDX * fDX <= fReach * fReach;
}
