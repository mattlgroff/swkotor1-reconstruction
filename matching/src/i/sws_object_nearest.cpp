// CSWSObject::GetNearestObjectByTag (0x004CB5C0).
#include <math.h>
#include "i/sws_object.h"

// FUNCTION: SWKOTOR 0x004CB5C0
unsigned long CSWSObject::GetNearestObjectByTag(const CExoString& sTag, float fMaxDistance)
{
    unsigned long oidNearest = 0x7f000000;
    int bCheckDistance = fMaxDistance != 0.0f;
    float fBest = fMaxDistance * fMaxDistance;
    CSWSArea* pArea = GetAreaInline();
    if (pArea && !(sTag == "")) {
        Vector vPosition = m_vPosition;
        unsigned long oidObject;
        if (pArea->GetFirstObjectIdInArea(&oidObject) == 1) {
            do {
                CGameObject* pGameObject;
                if (GetServerExoApp()->GetObjectArray()->GetGameObject(oidObject, &pGameObject) ==
                    GAME_OBJECT_VALID) {
                    CSWSObject* pObject = (CSWSObject*)pGameObject;
                    Vector vObject = pObject->m_vPosition;
                    if (pObject->GetAreaInline() == pArea && pObject->m_sTag.CompareNoCase(sTag)) {
                        Vector vDiff = vObject - vPosition;
                        float fDistance =
                            vDiff.x * vDiff.x + vDiff.y * vDiff.y + vDiff.z * vDiff.z;
                        if (!bCheckDistance || fDistance <= fBest) {
                            fBest = (float)sqrt(fDistance);
                            oidNearest = pObject->m_idSelf;
                            bCheckDistance = 1;
                        }
                    }
                }
            } while (pArea->GetNextObjectIdInArea(&oidObject) == 1);
        }
        return oidNearest;
    }
    return 0x7f000000;
}
