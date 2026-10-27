// Worker A round s7: CSWSCreature::RemoveFromArea (nwscreature.cpp).
#include "a/sws_creature.h"

extern const unsigned long EVENT_0x7441DC;      // 0x007441DC (placeholder name)

// FUNCTION: SWKOTOR 0x004F0FD0
void CSWSCreature::RemoveFromArea(int bArg)
{
    CGameObject* pObject;
    if (GetArea()) {
        GetArea()->RemoveObjectFromArea(m_idSelf);
    }
    if (!bArg) {
        ClearVisibleList();
        if (GetArea()) {
            CSWSArea* pArea = GetArea();
            CServerAIMaster* pAIMaster = GetServerExoApp()->GetServerAIMaster();
            for (int i = 0; i < pArea->m_lPlayers.num; i++) {
                CSWSCreature* pViewer = GetServerExoApp()->GetCreatureByGameObjectID(pArea->m_lPlayers.element[i]);
                if (!pViewer) {
                    continue;
                }
                CSWSCreatureListEntry* pEntry = pViewer->GetFromList920(m_idSelf);
                if (!pEntry) {
                    continue;
                }
                if (pEntry->m_b1) {
                    CScriptEvent* pEvent = new CScriptEvent;
                    pEvent->m_nType = 1;
                    pEvent->SetInteger(0, 1);
                    pEvent->SetObjectID(0, m_idSelf);
                    pAIMaster->AddEventDeltaTime(0, 0, m_idSelf, pViewer->m_idSelf, EVENT_0x7441DC, pEvent);
                }
                if (pEntry->m_b0) {
                    CScriptEvent* pEvent = new CScriptEvent;
                    pEvent->m_nType = 1;
                    pEvent->SetInteger(0, 3);
                    pEvent->SetObjectID(0, m_idSelf);
                    pAIMaster->AddEventDeltaTime(0, 0, m_idSelf, pViewer->m_idSelf, EVENT_0x7441DC, pEvent);
                }
                pViewer->OnIdChanged(m_idSelf);
            }
        }
    }
    CGameObjectArray* pArray = GetServerExoApp()->GetObjectArray();
    for (int i = 0; i < m_lAreaOfEffects.num; i++) {
        if (pArray->GetGameObject(m_lAreaOfEffects.element[i], &pObject) != GAME_OBJECT_VALID) {
            continue;
        }
        if (!pObject->AsSWSAreaOfEffectObject()) {
            continue;
        }
        pObject->AsSWSAreaOfEffectObject()->RemoveFromArea();
    }
    SetArea(NULL);
    SetPosition(Vector(0.0f, 0.0f, 0.0f), 1, 1, 0);
}
