// CSWVirtualMachineCommands: Action* commands that queue actions on the
// script owner (0x0052C6A0-0x0052F4F0).
#include "b/vm_commands.h"
#include "b/sws_creature.h"
#include "b/global_variable_table.h"

class CScriptTalent {
public:
    ~CScriptTalent() {}

    int m_nType;                                            // 0x00
    int m_nId;                                              // 0x04
    unsigned char m_nMultiClass;                            // 0x08
    unsigned long m_oidItem;                                // 0x0c
    int m_nItemPropertyIndex;                               // 0x10
    unsigned char m_nCasterLevel;                           // 0x14
    unsigned char m_nMetaType;                              // 0x15
};

class CSWGuiInGame {
public:
    char pad_0x0[0xb4];
    int m_nB4;                                              // 0xb4 (name guessed)
};

// FUNCTION: SWKOTOR 0x0052C6A0
int CSWVirtualMachineCommands::ExecuteCommandActionBarkString(int nCommandId, int nParameters)
{
    int nStrRef;
    if (!g_pVirtualMachine->StackPopInteger(&nStrRef)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pAppManager->GetClientExoApp()->GetInGameGui()->m_nB4) {
        CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(m_oidObjectRunScript);
        if (pCreature) {
            pCreature->AddAction(0x3e, 0xffff, 1, &nStrRef);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0052C740
int CSWVirtualMachineCommands::ExecuteCommandActionDoCommand(int nCommandId, int nParameters)
{
    CVirtualMachineScript* pScript = NULL;
    if (!g_pVirtualMachine->StackPopCommand(&pScript)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID) {
        ((CSWSObject*)pObject)->AddDoCommandAction(pScript);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0052C7B0
int CSWVirtualMachineCommands::ExecuteCommandActionEquipMostDamagingMelee(int nCommandId, int nParameters)
{
    unsigned long oidVersus;
    int bOffHand;
    if (!g_pVirtualMachine->StackPopObject(&oidVersus)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&bOffHand)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(m_oidObjectRunScript);
    if (pCreature) {
        pCreature->EquipMostDamagingMeleeWeapon(oidVersus, bOffHand, 0);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0052C820
int CSWVirtualMachineCommands::ExecuteCommandActionEquipMostDamagingRanged(int nCommandId, int nParameters)
{
    unsigned long oidVersus;
    if (!g_pVirtualMachine->StackPopObject(&oidVersus)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(m_oidObjectRunScript);
    if (pCreature) {
        if (!pCreature->EquipMostDamagingRangedWeapon(oidVersus)) {
            pCreature->EquipMostDamagingMeleeWeapon(oidVersus, 0, 0);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0052C880
int CSWVirtualMachineCommands::ExecuteCommandActionEquipMostEffectiveArmor(int nCommandId, int nParameters)
{
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(m_oidObjectRunScript);
    if (pCreature) {
        pCreature->EquipMostEffectiveArmor();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0052C8B0
int CSWVirtualMachineCommands::ExecuteCommandActionGiveItem(int nCommandId, int nParameters)
{
    unsigned long oidItem;
    unsigned long oidTarget;
    if (!g_pVirtualMachine->StackPopObject(&oidItem)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(m_oidObjectRunScript);
    if (pObject && g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(oidItem)) {
        if (nCommandId == 0x87) {
            pObject->AsSWSObject()->AddGiveItemActions(oidItem, oidTarget, -1, 1);
        } else {
            pObject->AsSWSObject()->AddTakeItemActions(oidItem, oidTarget, 1);
        }
    }
    return 0;
}

extern int g_bDebugActions;                                 // 0x0083281C (name guessed)
extern const unsigned char OBJECT_TYPE_AREA;                // 0x00746633 (4)

// FUNCTION: SWKOTOR 0x0052CBE0
int CSWVirtualMachineCommands::ExecuteCommandActionFollowLeader(int nCommandId, int nParameters)
{
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(m_oidObjectRunScript);
    if (pCreature && pCreature->m_bInParty && pCreature->m_bCommandable == 1) {
        if (g_bDebugActions) {
            pCreature->m_sTag.CStr();
        }
        pCreature->AddAction(0x3d, 0xffff, 0, NULL);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0052CC70
int CSWVirtualMachineCommands::ExecuteCommandActionInteractObject(int nCommandId, int nParameters)
{
    unsigned long oidPlaceable;
    if (!g_pVirtualMachine->StackPopObject(&oidPlaceable)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pPlaceable = (CGameObject*)g_pAppManager->GetServerExoApp()->GetPlaceableByGameObjectID(oidPlaceable);
    if (pPlaceable) {
        CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(m_oidObjectRunScript);
        if (pCreature) {
            pCreature->AddUseObjectAction(pPlaceable->m_idSelf);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0052CCE0
int CSWVirtualMachineCommands::ExecuteCommandActionJumpToObject(int nCommandId, int nParameters)
{
    int bWalkStraightLineToPoint = 1;
    unsigned long oidJumpTo;
    if (!g_pVirtualMachine->StackPopObject(&oidJumpTo)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters > 1 && !g_pVirtualMachine->StackPopInteger(&bWalkStraightLineToPoint)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(m_oidObjectRunScript);
    if (pCreature && pCreature->m_bCommandable) {
        if (nCommandId == 0xc4) {
            pCreature->AddAction(0x30, 0xffff, 3, &oidJumpTo, 1, &bWalkStraightLineToPoint);
        } else {
            pCreature->AddActionToFront(0x30, 0xffff, 3, &oidJumpTo, 1, &bWalkStraightLineToPoint);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0052CDC0
int CSWVirtualMachineCommands::ExecuteCommandActionJumpToLocation(int nCommandId, int nParameters)
{
    Vector vPosition(-1.0f, -1.0f, -1.0f);
    Vector vOrientation(0.0f, 0.0f, 0.0f);
    CScriptLocation* pLocation;
    if (!g_pVirtualMachine->StackPopEngineStructure(ENGINE_STRUCTURE_LOCATION, (void**)&pLocation)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    vPosition = *(Vector*)pLocation->m_vPosition;
    vOrientation = *(Vector*)pLocation->m_vOrientation;
    delete pLocation;
    pLocation = NULL;
    unsigned long oidArea;
    int bFlag;
    if (nParameters < 2) {
        bFlag = 1;
    } else if (!g_pVirtualMachine->StackPopInteger(&bFlag)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (m_bValidObjectRunScript == 1 &&
        g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID &&
        pObject->AsSWSCreature() && pObject->AsSWSCreature()->m_bCommandable) {
        CGameObject* pArea = (CGameObject*)(CSWSAreaGameObject*)pObject->AsSWSCreature()->GetArea();
        oidArea = pArea->m_idSelf;
        if (pArea->m_nObjectType == OBJECT_TYPE_AREA) {
            float fRange = 20.0f;
            if (nCommandId == 0xd6) {
                ((CSWSObject*)pObject)->AddAction(5, 0xffff, 2, &vPosition.x, 2, &vPosition.y, 2, &vPosition.z, 3, &oidArea,
                                                  1, &bFlag, 2, &fRange, 2, &vOrientation.x, 2, &vOrientation.y);
            } else {
                ((CSWSObject*)pObject)->AddActionToFront(5, 0xffff, 2, &vPosition.x, 2, &vPosition.y, 2, &vPosition.z, 3, &oidArea,
                                                         1, &bFlag, 2, &fRange, 2, &vOrientation.x, 2, &vOrientation.y);
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0052CFF0
int CSWVirtualMachineCommands::ExecuteCommandActionLockObject(int nCommandId, int nParameters)
{
    unsigned long oidTarget;
    if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(m_oidObjectRunScript);
    if (pObject && pObject->AsSWSObject()) {
        if (nCommandId == 0x1e3) {
            pObject->AsSWSObject()->AddUnlockObjectAction(oidTarget, INVALID_OBJECT_ID, 0);
        } else if (nCommandId == 0x1e4) {
            pObject->AsSWSObject()->AddLockObjectAction(oidTarget);
        }
    }
    return 0;
}

extern const unsigned long OBJECT_INVALID;                  // 0x00746644

// FUNCTION: SWKOTOR 0x0052D330
int CSWVirtualMachineCommands::ExecuteCommandActionPauseConversation(int nCommandId, int nParameters)
{
    CGameObject* pObject;
    if (m_bValidObjectRunScript == 1 &&
        g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID &&
        pObject->AsSWSObject() && pObject->AsSWSObject()->m_bCommandable) {
        int bDead = 0;
        if (pObject->AsSWSCreature()) {
            bDead = pObject->AsSWSCreature()->GetDeadTemp();
        }
        if (pObject->AsSWSObject()->GetObjectState94() == 1 || pObject->AsSWSObject()->GetIsPaused() == 1 || bDead == 1) {
            pObject->AsSWSObject()->SetDialogOwner(OBJECT_INVALID);
        } else {
            pObject->AsSWSObject()->m_bDialogPaused = 1;
            if (pObject->AsSWSCreature()) {
                pObject->AsSWSCreature()->SetAIStateFlag(4, 1);
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0052D440
int CSWVirtualMachineCommands::ExecuteCommandActionRandomWalk(int nCommandId, int nParameters)
{
    CGameObject* pObject;
    if (m_bValidObjectRunScript == 1 &&
        g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID) {
        Vector vPosition = pObject->m_vPosition;
        CSWSArea* pArea = ((CSWSObject*)pObject)->GetArea();
        CResRef cResRef;
        if (pArea && pObject->m_bCommandable) {
            unsigned long oidArea = pArea->m_idSelf;
            ((CSWSObject*)pObject)->AddAction(0x2d, 0xffff, 2, &vPosition.x, 2, &vPosition.y, 2, &vPosition.z, 3, &oidArea);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0052D520
int CSWVirtualMachineCommands::ExecuteCommandActionResumeConversation(int nCommandId, int nParameters)
{
    CGameObject* pObject;
    if (m_bValidObjectRunScript == 1 &&
        g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID &&
        pObject->m_bCommandable) {
        ((CSWSObject*)pObject)->AddAction(0x20, 0xffff, 0, NULL);
    }
    return 0;
}

extern const float MOVE_AWAY_DEFAULT_RANGE;                 // 0x00746B10 (40.0f, name guessed)

// NONMATCH (27): the original shares vDiff's slot with vFace (0x44 frame) and
// keeps pObject in edi; ours keeps the constant 1 in edi. A non-inlined
// MagnitudeSquared() temp fixed the frame but adds a call.
// FUNCTION: SWKOTOR 0x0052D090
int CSWVirtualMachineCommands::ExecuteCommandActionMoveAwayFromLocation(int nCommandId, int nParameters)
{
    int bRun = 0;
    float fMoveAwayRange = MOVE_AWAY_DEFAULT_RANGE;
    CScriptLocation* pLocation;
    if (!g_pVirtualMachine->StackPopEngineStructure(ENGINE_STRUCTURE_LOCATION, (void**)&pLocation)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters >= 2 && !g_pVirtualMachine->StackPopInteger(&bRun)) {
        if (pLocation) {
            delete pLocation;
        }
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters >= 3 && !g_pVirtualMachine->StackPopFloat(&fMoveAwayRange)) {
        if (pLocation) {
            delete pLocation;
        }
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (m_bValidObjectRunScript == 1 &&
        g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID &&
        pObject->AsSWSCreature() && pObject->AsSWSCreature()->m_bCommandable == 1) {
        Vector vAwayFrom = *(Vector*)pLocation->m_vPosition;
        CSWSArea* pArea = pObject->AsSWSCreature()->GetArea();
        CResRef cResRef;
        if (pArea) {
            Vector vDiff = pObject->m_vPosition - vAwayFrom;
            if (vDiff.z * vDiff.z + vDiff.y * vDiff.y + vDiff.x * vDiff.x > fMoveAwayRange * fMoveAwayRange) {
                return 0;
            }
            Vector vNewPosition = pArea->ComputeAwayVector(pObject->m_vPosition, vAwayFrom, fMoveAwayRange,
                                                           pObject->AsSWSCreature()->m_n340);
            Vector vFace(0.0f, 0.0f, 0.0f);
            int nTen = 10;
            pObject->AsSWSCreature()->AddMoveToPointAction(0xffff, vNewPosition, pArea->m_idSelf, OBJECT_INVALID, bRun,
                                                           0.0f, 0, 0, 0, 0, 0, 1, 0, vFace, 0, 0, 0);
            ((CSWSObject*)pObject)->AddAction(0x2c, 0xfffe, 2, &pLocation->m_vPosition[0], 2, &pLocation->m_vPosition[1],
                                              2, &pLocation->m_vPosition[2], 1, &bRun, 2, &fMoveAwayRange, 1, &nTen);
            return 0;
        }
    }
    if (pLocation) {
        delete pLocation;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0052D8F0
int CSWVirtualMachineCommands::ExecuteCommandActionUseFeat(int nCommandId, int nParameters)
{
    int nFeat;
    unsigned long oidTarget;
    if (!g_pVirtualMachine->StackPopInteger(&nFeat)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID &&
        pObject->AsSWSCreature() && pObject->AsSWSCreature()->GetArea()) {
        pObject->AsSWSCreature()->UseFeat((unsigned short)nFeat, 0, oidTarget, pObject->AsSWSCreature()->GetArea()->GetId());
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0052D9B0
int CSWVirtualMachineCommands::ExecuteCommandActionUseSkill(int nCommandId, int nParameters)
{
    int nSkill;
    unsigned long oidTarget;
    if (!g_pVirtualMachine->StackPopInteger(&nSkill)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int nSubSkill = 0;
    if (nParameters >= 3 && !g_pVirtualMachine->StackPopInteger(&nSubSkill)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    unsigned long oidItemUsed = INVALID_OBJECT_ID;
    if (nParameters >= 4 && !g_pVirtualMachine->StackPopObject(&oidItemUsed)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID &&
        pObject->AsSWSCreature() && pObject->AsSWSCreature()->GetArea()) {
        pObject->AsSWSCreature()->UseSkill((unsigned char)nSkill, (unsigned char)nSubSkill, oidTarget, Vector(0.0f, 0.0f, 0.0f),
                                           pObject->AsSWSCreature()->GetArea()->GetId(), oidItemUsed, 0);
    }
    return 0;
}

// NONMATCH (41): register permutation only (the original holds pTalent in
// ecx and the byte fields in dl/al/cl).
// FUNCTION: SWKOTOR 0x0052DAF0
int CSWVirtualMachineCommands::ExecuteCommandActionUseTalent(int nCommandId, int nParameters)
{
    CScriptTalent* pTalent;
    if (!g_pVirtualMachine->StackPopEngineStructure(ENGINE_STRUCTURE_TALENT, (void**)&pTalent)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int nType = pTalent->m_nType;
    if (nType > -1) {
        int nId = pTalent->m_nId;
        unsigned char nMultiClass = pTalent->m_nMultiClass;
        unsigned long oidItem = pTalent->m_oidItem;
        int nItemPropertyIndex = pTalent->m_nItemPropertyIndex;
        unsigned char nCasterLevel = pTalent->m_nCasterLevel;
        unsigned char nMetaType = pTalent->m_nMetaType;
        CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(m_oidObjectRunScript);
        if (pCreature) {
            if (nCommandId == 0x135) {
                unsigned long oidTarget = INVALID_OBJECT_ID;
                if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
                    return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
                }
                pCreature->UseTalentOnObject(nType, nId, oidTarget, nMultiClass, oidItem, nItemPropertyIndex,
                                             nCasterLevel, nMetaType);
            } else if (nCommandId == 0x136) {
                CScriptLocation* pLocation;
                if (!g_pVirtualMachine->StackPopEngineStructure(ENGINE_STRUCTURE_LOCATION, (void**)&pLocation)) {
                    return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
                }
                pCreature->UseTalentAtLocation(nType, nId, pLocation, nMultiClass, oidItem, nItemPropertyIndex,
                                               nCasterLevel, nMetaType);
                if (pLocation) {
                    delete pLocation;
                }
            }
        }
    }
    if (pTalent) {
        delete pTalent;
    }
    return 0;
}

class CServerAIMaster {
public:
    int AddEventDeltaTime(unsigned long nDays, unsigned long nTime, unsigned long nCaller,
                          unsigned long nObject, unsigned long nEventId, void* pEventData); // 0x004B08D0
};

extern const unsigned long EVENT_TIMED_EVENT;               // 0x007441B8 (1)

// FUNCTION: SWKOTOR 0x0052E720
int CSWVirtualMachineCommands::ExecuteCommandAssignCommand(int nCommandId, int nParameters)
{
    unsigned long oidActionSubject;
    CVirtualMachineScript* pScript;
    if (!g_pVirtualMachine->StackPopObject(&oidActionSubject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    pScript = NULL;
    if (!g_pVirtualMachine->StackPopCommand(&pScript)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidActionSubject, &pObject) == GAME_OBJECT_VALID) {
        unsigned long oidCaller = OBJECT_INVALID;
        if (m_bValidObjectRunScript == 1) {
            oidCaller = m_oidObjectRunScript;
        }
        g_pAppManager->GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(0, 0, oidCaller, oidActionSubject, EVENT_TIMED_EVENT, pScript);
    } else if (pScript) {
        delete (void*)pScript;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0052E7F0
int CSWVirtualMachineCommands::ExecuteCommandActionAttack(int nCommandId, int nParameters)
{
    int bPassive = 0;
    unsigned long oidAttackee;
    if (!g_pVirtualMachine->StackPopObject(&oidAttackee)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters >= 2 && !g_pVirtualMachine->StackPopInteger(&bPassive)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(m_oidObjectRunScript);
    if (pCreature) {
        pCreature->AddAttackActions(oidAttackee, 0, 0, bPassive, 0, 0, 0, 1, 0x2719, 0x5dc, 1, 0, 0, 4, 0);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0052E890
int CSWVirtualMachineCommands::ExecuteCommandCutsceneAttack(int nCommandId, int nParameters)
{
    unsigned long oidTarget = INVALID_OBJECT_ID;
    int nAnimation = 0;
    int nAttackResult = 0;
    int nDamage = 0;
    if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nAnimation)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nAttackResult)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nDamage)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(m_oidObjectRunScript);
    if (pCreature) {
        pCreature->AddCutsceneAttackActions(oidTarget, nAnimation, nAttackResult, nDamage);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0052E950
int CSWVirtualMachineCommands::ExecuteCommandCutsceneMove(int nCommandId, int nParameters)
{
    unsigned long oidTarget;
    Vector vPosition(0.0f, 0.0f, 0.0f);
    int bRun;
    if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopVector(&vPosition)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&bRun)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(m_oidObjectRunScript);
    if (pCreature) {
        pCreature->AddCutsceneMoveActions(oidTarget, vPosition, bRun);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0052F4A0
int CSWVirtualMachineCommands::ExecuteCommandClearAllActions(int nCommandId, int nParameters)
{
    CGameObject* pObject = NULL;
    if (m_bValidObjectRunScript &&
        g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID) {
        ((CSWSObject*)pObject)->ClearAllActions(1);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0052F4F0
int CSWVirtualMachineCommands::ExecuteCommandActionCloseDoor(int nCommandId, int nParameters)
{
    unsigned long oidDoor;
    if (!g_pVirtualMachine->StackPopObject(&oidDoor)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (m_bValidObjectRunScript == 1 &&
        g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID &&
        ((CSWSObject*)pObject)->GetArea()) {
        pObject->AsSWSObject()->AddCloseDoorAction(oidDoor, 0);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005302F0
int CSWVirtualMachineCommands::ExecuteCommandFeetToMeters(int nCommandId, int nParameters)
{
    float fValue;
    float fResult;
    if (!g_pVirtualMachine->StackPopFloat(&fValue)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    switch (nCommandId) {
    case 0xda:
        fResult = fValue * 0.3048f;
        break;
    case 0xdb:
        fResult = fValue * 0.9144f;
        break;
    }
    return g_pVirtualMachine->StackPushFloat(fResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00530660
int CSWVirtualMachineCommands::ExecuteCommandDoSinglePlayerAutoSave(int nCommandId, int nParameters)
{
    g_pAppManager->GetServerExoApp()->SetAutoSavePending(1);
    return 0;
}

// FUNCTION: SWKOTOR 0x0052FE30
int CSWVirtualMachineCommands::ExecuteCommandDelayCommand(int nCommandId, int nParameters)
{
    CVirtualMachineScript* pScript = NULL;
    float fSeconds;
    if (!g_pVirtualMachine->StackPopFloat(&fSeconds)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopCommand(&pScript)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (m_bValidObjectRunScript == 1 &&
        g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID) {
        unsigned long oidCaller = OBJECT_INVALID;
        if (m_bValidObjectRunScript == 1) {
            oidCaller = m_oidObjectRunScript;
        }
        g_pAppManager->GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
            0, (unsigned long)(fSeconds * 1000.0f), oidCaller, m_oidObjectRunScript, EVENT_TIMED_EVENT, pScript);
    } else if (pScript) {
        delete (void*)pScript;
    }
    return 0;
}

// NONMATCH: register swap only (the original keeps `this` in esi and
// nParameters in edi).
// FUNCTION: SWKOTOR 0x0053F990
int CSWVirtualMachineCommands::ExecuteCommandActionMoveAwayFromObject(int nCommandId, int nParameters)
{
    int bRun = 0;
    float fMoveAwayRange = MOVE_AWAY_DEFAULT_RANGE;
    unsigned long oidFleeFrom;
    if (!g_pVirtualMachine->StackPopObject(&oidFleeFrom)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters >= 2 && !g_pVirtualMachine->StackPopInteger(&bRun)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters >= 3 && !g_pVirtualMachine->StackPopFloat(&fMoveAwayRange)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (m_bValidObjectRunScript == 1 &&
        g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID &&
        pObject->AsSWSCreature() && pObject->AsSWSCreature()->m_bCommandable) {
        CGameObject* pFleeFrom;
        if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidFleeFrom, &pFleeFrom) == GAME_OBJECT_VALID) {
            int nTen = 10;
            ((CSWSObject*)pObject)->AddAction(3, 0xfffe, 3, &oidFleeFrom, 1, &bRun, 2, &fMoveAwayRange, 1, &nTen);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00545800
int CSWVirtualMachineCommands::ExecuteCommandActionWait(int nCommandId, int nParameters)
{
    float fSeconds;
    if (!g_pVirtualMachine->StackPopFloat(&fSeconds)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (m_bValidObjectRunScript == 1 &&
        g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID &&
        pObject->AsSWSObject() && pObject->AsSWSObject()->m_bCommandable) {
        pObject->AsSWSObject()->AddAction(0x1e, 0xffff, 2, &fSeconds);
    }
    return 0;
}

// NONMATCH (27): stack-slot allocation and shared branch placement.
// FUNCTION: SWKOTOR 0x0053FE00
int CSWVirtualMachineCommands::ExecuteCommandActionMoveToLocation(int nCommandId, int nParameters)
{
    int bRun = 0;
    float fTimeout = 30.0f;
    CScriptLocation* pLocation;
    if (!g_pVirtualMachine->StackPopEngineStructure(ENGINE_STRUCTURE_LOCATION, (void**)&pLocation)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters > 1) {
        if (!g_pVirtualMachine->StackPopInteger(&bRun)) return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        if (nCommandId == 0x17e && nParameters > 2 && !g_pVirtualMachine->StackPopFloat(&fTimeout))
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (m_bValidObjectRunScript == 1 &&
        g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID &&
        pObject->AsSWSCreature()) {
        CSWSArea* pArea = pObject->AsSWSCreature()->GetArea();
        if (pArea) {
            if (nCommandId == 0x15) fTimeout = 0.0f;
            else if (nCommandId != 0x17e) return 0;
            Vector vFacing(0.0f, 0.0f, 0.0f);
            pObject->AsSWSCreature()->AddMoveToPointAction(0xffff, *(Vector*)pLocation->m_vPosition,
                pArea->m_idSelf, OBJECT_INVALID, bRun, 0.0f, fTimeout, 0, 0, 0, 0, 0, 0, vFacing, 0, 0, 0);
            return 0;
        }
    }
    if (pLocation) delete pLocation;
    return 0;
}
