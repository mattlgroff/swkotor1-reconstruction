// CSWSObject door and lock AI actions (round s10).
#include "b/sws_creature.h"
#include "b/sws_item.h"
#include "b/server_exo_app.h"
#include "b/vector.h"
#include "b/sws_door.h"

// .rdata action results.
extern const unsigned long ACTION_COMPLETE;         // 0x00745DC0 (2)
extern const unsigned long ACTION_FAILED;           // 0x00745DC4 (3)
extern const unsigned char GAME_OBJECT_VALID;       // 0x0074666C
extern const unsigned long EVENT_CLOSE_OBJECT;      // 0x007441CC (name guessed)
extern const unsigned long EVENT_OPEN_OBJECT;       // 0x007441D0 (name guessed)
extern const unsigned long EVENT_SIGNAL_EVENT;      // 0x007441DC (name guessed)

// FUNCTION: SWKOTOR 0x0057BBF0
unsigned long CSWSObject::AIActionCloseDoor(CSWSObjectActionNode* pNode)
{
    int bRun;
    int bDead = 0;
    if (AsSWSCreature()) {
        bDead = AsSWSCreature()->GetDeadTemp();
    }
    if (GetObjectState94() == 1 || GetIsPaused() == 1 || bDead == 1) {
            return ACTION_FAILED;
    }
    if (AsSWSCreature()) {
        AsSWSCreature()->ActivityManager(2);
    }
    bRun = 0;
    unsigned long oidDoor = (unsigned long)pNode->m_pParameter[0];
    if (pNode->m_nNumParams > 1 && pNode->m_pParameter[1]) {
        bRun = (int)pNode->m_pParameter[1];
    }
    CGameObject* pObject;
    float fRange;
    Vector vPosition(0.0f, 0.0f, 0.0f);
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidDoor, &pObject) != GAME_OBJECT_VALID) {
            return ACTION_FAILED;
    }
    if (AsSWSCreature()) {
        if (!pObject->AsSWSDoor() && !pObject->AsSWSPlaceable()) {
            return ACTION_FAILED;
        }
        ((CSWSObject*)pObject)->GetArea();
        AsSWSCreature()->GetUseRange(oidDoor, vPosition, fRange, 0);
        if (GetArea() != pObject->AsSWSObject()->GetArea() ||
            !AsSWSCreature()->GetIsInUseRange(oidDoor, 0.0f, 0)) {
            AsSWSCreature()->AddActionToFront(0x15, 0xffff, 3, &oidDoor, 1, &bRun);
            Vector vFace(0.0f, 0.0f, 0.0f);
            AsSWSCreature()->AddMoveToPointActionToFront(pNode->m_n6C, vPosition,
                                                         pObject->AsSWSObject()->m_oidArea,
                                                         INVALID_OBJECT_ID, bRun, fRange, 0.0f,
                                                         0, 0, 0, 0, 0, 0, vFace, 0, 0, 0);
            return ACTION_COMPLETE;
        }
    }
    SetDialogOwner(oidDoor);
    g_pAppManager->GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(0, 0, m_idSelf, oidDoor,
                                                                            EVENT_CLOSE_OBJECT, NULL);
    float fDelay = 0.5f;
    AddActionToFront(0x1e, pNode->m_n6C, 2, &fDelay);
    return ACTION_COMPLETE;
}

// CSWSObject +0x1f0: an open-door action is already queued (name guessed).
struct CSWSObjectOpenDoorView {
    char pad_0x0[0x1f0];
    int m_bOpenDoorPending;                                 // 0x1f0
};

// Matches only with the inline GetId() for the two own-id arguments and the
// door faction read into a local before the creature call.
// FUNCTION: SWKOTOR 0x0057D490
unsigned long CSWSObject::AIActionOpenDoor(CSWSObjectActionNode* pNode)
{
    int bDead = 0;
    if (AsSWSCreature()) {
        bDead = AsSWSCreature()->GetDeadTemp();
    }
    if (GetObjectState94() == 1 || GetIsPaused() == 1 || bDead == 1) {
        ((CSWSObjectOpenDoorView*)this)->m_bOpenDoorPending = 0;
            return ACTION_FAILED;
    }
    if (AsSWSCreature()) {
        AsSWSCreature()->ActivityManager(2);
    }
    int bRun = 1;
    unsigned long oidDoor = (unsigned long)pNode->m_pParameter[0];
    if (pNode->m_nNumParams > 1 && pNode->m_pParameter[1]) {
        bRun = (int)pNode->m_pParameter[1];
    }
    CGameObject* pObject;
    float fRange;
    Vector vPosition(0.0f, 0.0f, 0.0f);
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidDoor, &pObject) != GAME_OBJECT_VALID) {
        ((CSWSObjectOpenDoorView*)this)->m_bOpenDoorPending = 0;
        return ACTION_FAILED;
    }
    if (AsSWSCreature()) {
        CSWSDoor* pDoor = pObject->AsSWSDoor();
        if (!pDoor) {
            ((CSWSObjectOpenDoorView*)this)->m_bOpenDoorPending = 0;
            return ACTION_FAILED;
        }
        ((CSWSObject*)pObject)->GetArea();
        AsSWSCreature()->GetUseRange(oidDoor, vPosition, fRange, 0);
        if (pDoor->m_nAnimationState != 0x2726) {
            return ACTION_COMPLETE;
        }
        if (GetArea() == pObject->AsSWSObject()->GetArea() &&
            AsSWSCreature()->GetIsInUseRange(oidDoor, 0.0f, 0)) {
            if (pDoor->m_bTrapped && ((CSWSObject*)pDoor)->GetReputation(GetId(), 0) < 0x5a) {
                int nDoorFaction = pDoor->m_nFactionId;
                if (nDoorFaction != AsSWSCreature()->GetStats()->m_nFactionId) {
                    CScriptEvent* pEvent = new CScriptEvent;
                    pEvent->m_nType = 0x1a;
                    g_pAppManager->GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
                        0, 0, GetId(), oidDoor, EVENT_SIGNAL_EVENT, pEvent);
                    return ACTION_FAILED;
                }
            }
            if (!AsSWSCreature()->EvaluateLock(pObject->AsSWSObject(), 0)) {
                CScriptEvent* pEvent = new CScriptEvent;
                pEvent->m_nType = 0x22;
                g_pAppManager->GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
                    0, 0, GetId(), oidDoor, EVENT_SIGNAL_EVENT, pEvent);
                ((CSWSObjectOpenDoorView*)this)->m_bOpenDoorPending = 0;
                return ACTION_FAILED;
            }
            SetDialogOwner(oidDoor);
            if (!((CSWSObjectOpenDoorView*)this)->m_bOpenDoorPending) {
                AsSWSCreature()->AddActionToFront(0x14, 0xffff, 3, &oidDoor, 1, &bRun);
                ((CSWSObjectOpenDoorView*)this)->m_bOpenDoorPending = 1;
                return ACTION_COMPLETE;
            }
        } else {
            AsSWSCreature()->AddActionToFront(0x14, 0xffff, 3, &oidDoor, 1, &bRun);
            if (pDoor->m_bAutoRemoveKey && pDoor->m_bLocked) {
                float fDelay = 0.5f;
                AsSWSCreature()->AddActionToFront(0x1e, 0xfffe, 2, &fDelay);
                AsSWSCreature()->AddActionToFront(0x13, 0xfffe, 3, &oidDoor);
            }
            Vector vFace(0.0f, 0.0f, 0.0f);
            AsSWSCreature()->AddMoveToPointActionToFront(0xfffe, vPosition,
                                                         pObject->AsSWSObject()->m_oidArea,
                                                         INVALID_OBJECT_ID, bRun, fRange, 0.0f,
                                                         0, 0, 0, 0, 0, 0, vFace, 0, 0, 0);
            ((CSWSObjectOpenDoorView*)this)->m_bOpenDoorPending = 1;
            return ACTION_COMPLETE;
        }
    }
    ((CSWSObjectOpenDoorView*)this)->m_bOpenDoorPending = 0;
    g_pAppManager->GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(0, 0, m_idSelf, oidDoor,
                                                                            EVENT_OPEN_OBJECT, NULL);
    float fDelay = 0.5f;
    AddActionToFront(0x1e, pNode->m_n6C, 2, &fDelay);
    return ACTION_COMPLETE;
}
