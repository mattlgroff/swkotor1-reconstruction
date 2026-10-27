// CSWSObject::BroadcastDialog.
#include "i/sws_object.h"

class CScriptEvent {
public:
    CScriptEvent();                                 // 0x004D7540
    void SetInteger(int nIndex, int nValue);        // 0x004D76B0
    void SetString(int nIndex, const CExoString& sValue);   // 0x004D6AE0
    unsigned short m_nType;                         // 0x00
    char pad_0x2[0x34 - 0x2];
};

class CServerAIMaster {
public:
    int AddEventDeltaTime(unsigned long nDays, unsigned long nTime, unsigned long oidCaller,
                          unsigned long oidObject, unsigned long nEventId,
                          void* pEventData);        // 0x004B08D0
};

struct CSWSCreatureVisibility {
    unsigned long m_oidObject;                      // 0x00
    unsigned char m_nFlags;                         // 0x04
};

class CSWSCreatureListen : public CSWSCreature {
public:
    CSWSCreatureVisibility* GetFromList920(unsigned long oid);  // 0x00517A20
    float GetListenCheckDistance();                 // 0x004EFD20
};

Vector VectorSubtract(const Vector& a, const Vector& b);    // 0x00423BC0

extern unsigned long EVENT_SIGNAL_EVENT;            // 0x007441DC

// FUNCTION: SWKOTOR 0x004CD760
void CSWSObject::BroadcastDialog(CExoString& sText, float fRange, int bFlag)
{
    int nIndex = -1;
    CGameObject* pGameObject = NULL;
    CGameObject* pAreaObject = NULL;
    GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidArea, &pAreaObject);
    CSWSArea* pArea = pAreaObject ? pAreaObject->AsSWSArea() : NULL;
    CGameObjectArray* pObjects = GetServerExoApp()->GetObjectArray();
    if (pArea == NULL) {
        return;
    }
    pArea->GetFirstObjectIndiceByX(&nIndex, m_vPosition.x - fRange);
    while (nIndex >= 0 && nIndex < pArea->m_aCreatures.num) {
        unsigned long oid = pArea->m_aCreatures.element[nIndex++];
        if (pObjects->GetGameObject(oid, &pGameObject) != GAME_OBJECT_VALID) {
            return;
        }
        CSWSObject* pObject = (CSWSObject*)pGameObject;
        if (pObject == NULL || oid == m_idSelf) {
            continue;
        }
        Vector vPosition = pObject->m_vPosition;
        if (fRange + m_vPosition.x < vPosition.x) {
            return;
        }
        Vector vDiff = m_vPosition - vPosition;
        float fDistance = vDiff * vDiff;
        if (fDistance <= fRange * fRange) {
            if (!pObject->m_bListening) {
                continue;
            }
            if (pObject->AsSWSCreature() && AsSWSCreature()) {
                CSWSCreatureVisibility* pVis =
                    ((CSWSCreatureListen*)pObject->AsSWSCreature())->GetFromList920(m_idSelf);
                if (pVis == NULL || !(pVis->m_nFlags & 2)) {
                    continue;
                }
                if (bFlag && !(pVis->m_nFlags & 1) && fDistance >= 100.0f) {
                    continue;
                }
            }
            else if (pObject->AsSWSCreature() && !AsSWSCreature()) {
                Vector vDelta = VectorSubtract(m_vPosition, vPosition);
                float fListen = ((CSWSCreatureListen*)pObject->AsSWSCreature())->GetListenCheckDistance();
                if (vDelta.x * vDelta.x + vDelta.y * vDelta.y + vDelta.z * vDelta.z >
                    fListen * fListen) {
                    continue;
                }
            }
            int nMatch = pObject->TestListenExpression(sText);
            if (nMatch >= 0) {
                CScriptEvent* pEvent = new CScriptEvent;
                pEvent->m_nType = 7;
                pEvent->SetInteger(0, pObject->GetListenExpressionById(nMatch)->m_lstMatches.num);
                pEvent->SetInteger(1, nMatch);
                for (int i = 1; i < pObject->GetListenExpressionById(nMatch)->m_lstMatches.num; i++) {
                    pEvent->SetString(i, *pObject->GetListenExpressionById(nMatch)->m_lstMatches.element[i]);
                }
                GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
                    0, 0, m_idSelf, oid, EVENT_SIGNAL_EVENT, pEvent);
            }
        }
    }
}
