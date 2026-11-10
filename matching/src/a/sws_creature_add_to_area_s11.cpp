// Worker A round s11: CSWSCreature::AddToArea (behavioural reference:
// src/units/1227/a1f5403078-creature_placement_native_ai.cpp).
#include "a/sws_creature.h"

// FUNCTION: SWKOTOR 0x004FA100
void CSWSCreature::AddToArea(CSWSArea* pArea, float fX, float fY, float fZ, int bArg5, int bArg6)
{
    CSWSPlayer* pPlayer = GetServerExoApp()->GetClientObjectByObjectId(m_idSelf);
    if (GetArea() != pArea || bArg5 == 1) {
        RemoveFromArea(0);
    }
    CServerAIMaster* pAIMaster = GetServerExoApp()->GetServerAIMaster();
    CGameObjectArray* pArray = GetServerExoApp()->GetObjectArray();
    CGameObject* pObject;
    for (int i = 0; i < m_lAreaOfEffects.num; i++) {
        if (pArray->GetGameObject(m_lAreaOfEffects.element[i], &pObject) == GAME_OBJECT_VALID &&
            pObject->AsSWSAreaOfEffectObject()) {
            pObject->AsSWSAreaOfEffectObject()->AddToArea(pArea, fX, fY, fZ, 1);
        }
    }
    if (pArea == NULL) {
        return;
    }
    if (GetArea() != pArea && pArea->m_idSelf != m_oidLastArea && pPlayer) {
        CSWSMessage* pMessage = GetServerExoApp()->GetSWSMessage();
        if (pMessage) {
            m_oidLastArea = pArea->m_idSelf;
            m_v314 = Vector(fX, fY, fZ);
            m_n320 = 0;
            pMessage->SendServerToPlayerArea_ClientArea(pPlayer, pArea, fX, fY, fZ, m_vOrientation, 0);
            SetArea(NULL);
            SetPosition(Vector(fX, fY, fZ), 0, 1, 0);
            return;
        }
    }
    SetPosition(Vector(fX, fY, fZ), 1, 1, 0);
    if (GetArea() != pArea || bArg5 == 1) {
        pArea->AddObjectToArea(m_idSelf, bArg5 == 0);
    }
    SetArea(pArea);
    if (m_nAILevel == 0 && pArea->m_nPlayersInArea > 0 && pAIMaster) {
        pAIMaster->SetAILevel((CSWSObject*)this, 1);
    }
    if (m_nAILevel == 1 && pArea->m_nPlayersInArea == 0 && pAIMaster) {
        pAIMaster->SetAILevel((CSWSObject*)this, 0);
    }
    if (bArg6) {
        unsigned long oidArea = pArea->m_idSelf;
        UpdateSubareasOnJumpPosition(m_vPosition, oidArea, !GetServerExoApp()->GetLoadFromSaveGame());
    }
}
