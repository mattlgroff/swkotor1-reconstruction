// CSWSArea visual-effect broadcast (0x00507E20) and the object-list
// lookup that follows it in nwsarea.o (0x00508010, missing from
// functions.csv). project symbols.csv carries a call-derived
// PlaySoundAtLocation name for 0x00507E20; the body sends
// SendServerToPlayerArea_VisualEffect, so the earlier worker-notes name
// PlayVisualEffect is kept.
#include "a/sws_area.h"

extern const float g_fVisualEffectBroadcastRange;   // 0x00747440 (250.0f)

// FUNCTION: SWKOTOR 0x00507E20
void CSWSArea::PlayVisualEffect(unsigned short nEffect, Vector* pPosition)
{
    C2DA* pVisualEffects = g_pRules->m_p2DArrays->m_pVisualEffects;
    float fRadius;
    {
        CExoString sColumn("RADIUS");
        pVisualEffects->GetFLOATEntry(nEffect, sColumn, &fRadius);
    }
    fRadius = g_fVisualEffectBroadcastRange + fRadius;
    float fMinX = pPosition->x - fRadius;
    float fMaxX = fRadius + pPosition->x;
    CGameObjectArray* pArray = GetServerExoApp()->GetObjectArray();
    int nIndex;
    if (GetFirstObjectIndiceByX(&nIndex, fMinX) == 1) {
        for (; nIndex < m_aGameObjects.num; nIndex++) {
            CGameObject* pObject;
            if (pArray->GetGameObject(m_aGameObjects.element[nIndex], &pObject) == g_nGameObjectFound &&
                pObject && pObject->m_nObjectType > g_nObjectTypeArea) {
                Vector vObject = ((CSWSObject*)pObject)->m_vPosition;
                if (vObject.x > fMaxX) {
                    return;
                }
                CSWSPlayer* pPlayer = GetServerExoApp()->GetClientObjectByObjectID(pObject->m_idSelf);
                if (pPlayer && (vObject - *pPosition).MagnitudeSquared() <= fRadius * fRadius) {
                    GetServerExoApp()->GetSWSMessage()->SendServerToPlayerArea_VisualEffect(
                        pPlayer, nEffect, *pPosition);
                }
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x00508010
int CSWSArea::ListContainsObject(unsigned long oidObject, CExoArrayList<CGameObject*>* pList)
{
    if (pList && pList->num > 0) {
        for (int i = 0; i < pList->num; i++) {
            if (pList->element[i]->m_idSelf == oidObject) {
                return 1;
            }
        }
    }
    return 0;
}
