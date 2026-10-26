// CSWVirtualMachineCommands: waypoint, transition, weapon and XP queries
// (0x0053E2B0-0x0053E685).
#include "b/vm_commands.h"
#include "b/sws_creature.h"
#include "b/sws_item.h"
#include "b/sws_door.h"
#include "b/global_variable_table.h"
#include <math.h>
#include <stdlib.h>

// Slot 0x40 object (bucket b calls the slot AsSWSAreaOfEffectObject; it is
// the trigger): area transition target.
class CSWSAreaOfEffectObject {
public:
    char pad_0x0[0x230];
    CExoString m_sLinkedTo;                                 // 0x230
    char pad_0x238[0x240 - 0x238];
    unsigned char m_nLinkedToFlags;                         // 0x240 (2: waypoint)
};

// FUNCTION: SWKOTOR 0x0053E2B0
int CSWVirtualMachineCommands::ExecuteCommandGetWaypointByTag(int nCommandId, int nParameters)
{
    CExoString sTag;
    unsigned long oidWaypoint = INVALID_OBJECT_ID;
    if (!g_pVirtualMachine->StackPopString(&sTag)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (g_pAppManager && g_pAppManager->GetServerExoApp()) {
        CSWSModule* pModule = g_pAppManager->GetServerExoApp()->GetModule();
        if (pModule) {
            oidWaypoint = pModule->GetWaypoint(sTag.LowerCase());
        }
    }
    if (!g_pVirtualMachine->StackPushObject(oidWaypoint)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0053E3C0
int CSWVirtualMachineCommands::ExecuteCommandGetTransitionTarget(int nCommandId, int nParameters)
{
    unsigned long oidTarget = INVALID_OBJECT_ID;
    unsigned long oidTransition;
    if (!g_pVirtualMachine->StackPopObject(&oidTransition)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidTransition, &pObject) == GAME_OBJECT_VALID) {
        if (pObject->AsSWSAreaOfEffectObject()) {
            CSWSAreaOfEffectObject* pTrigger = pObject->AsSWSAreaOfEffectObject();
            if (pObject->AsSWSAreaOfEffectObject()->m_nLinkedToFlags == 2) {
                oidTarget = g_pAppManager->GetServerExoApp()->GetModule()->GetWaypoint(pTrigger->m_sLinkedTo);
            } else {
                oidTarget = g_pAppManager->GetServerExoApp()->GetModule()->GetObjectByTag(pTrigger->m_sLinkedTo, 0);
            }
        } else if (pObject->AsSWSDoor()) {
            CSWSObject* pLinked = pObject->AsSWSDoor()->GetLinkedObject();
            if (pLinked) {
                oidTarget = pLinked->m_idSelf;
            }
        }
    }
    return g_pVirtualMachine->StackPushObject(oidTarget) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053E4C0
int CSWVirtualMachineCommands::ExecuteCommandGetWeaponRanged(int nCommandId, int nParameters)
{
    unsigned long oidItem;
    int bRanged = 0;
    if (!g_pVirtualMachine->StackPopObject(&oidItem)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSItem* pItem = g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(oidItem);
    if (pItem) {
        bRanged = pItem->GetBaseItemFromRules()->IsRangedWeapon();
    }
    return g_pVirtualMachine->StackPushInteger(bRanged) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053E600
int CSWVirtualMachineCommands::ExecuteCommandGetXP(int nCommandId, int nParameters)
{
    int nXP = 0;
    unsigned long oidCreature;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidCreature, &pObject) == GAME_OBJECT_VALID &&
        pObject) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        if (pCreature) {
            nXP = pCreature->m_pStats->m_nExperience;
        }
    }
    return g_pVirtualMachine->StackPushInteger(nXP) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

class CSWGuiInGame {
public:
    void UpdateStatus(int nType, int nValue);               // 0x0062EEB0
};

class CSWPartyTable {
public:
    int GetIsMember(int nNPC);                              // 0x00563710
    void DistributeExperience(int nXP, int bSignal);        // 0x005653A0
};

struct CSWSObjectListeningView {
    char pad_0x0[0x19c];
    int m_bListening;                                       // 0x19c
};

// FUNCTION: SWKOTOR 0x0053E690
int CSWVirtualMachineCommands::ExecuteCommandGiveGoldToCreature(int nCommandId, int nParameters)
{
    unsigned long oidCreature;
    int nGold;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nGold)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (nGold > 0 &&
        g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidCreature, &pObject) == GAME_OBJECT_VALID &&
        pObject) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        if (pCreature) {
            pCreature->AddGold(nGold, 1);
            if (pCreature->m_bInParty) {
                g_pAppManager->GetClientExoApp()->GetInGameGui()->UpdateStatus(1, nGold);
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0053E750
int CSWVirtualMachineCommands::ExecuteCommandGiveXPToCreature(int nCommandId, int nParameters)
{
    unsigned long oidCreature;
    int nXP;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nXP)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nXP > 0) {
        int nAmount = nXP;
        CGameObject* pObject;
        if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidCreature, &pObject) == GAME_OBJECT_VALID &&
            pObject) {
            CSWSCreature* pCreature = pObject->AsSWSCreature();
            if (pCreature) {
                if (pCreature->m_bInParty) {
                    g_pAppManager->GetClientExoApp()->GetInGameGui()->UpdateStatus(2, nAmount);
                }
                if (g_pAppManager->GetServerExoApp()->GetPartyTable()) {
                    g_pAppManager->GetServerExoApp()->GetPartyTable()->DistributeExperience(nXP, 1);
                }
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0053F080
int CSWVirtualMachineCommands::ExecuteCommandGetIsListening(int nCommandId, int nParameters)
{
    int bListening = 0;
    unsigned long oidObject;
    if (!g_pVirtualMachine->StackPopObject(&oidObject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidObject, &pObject) == GAME_OBJECT_VALID &&
        pObject) {
        bListening = ((CSWSObjectListeningView*)pObject)->m_bListening;
    }
    return g_pVirtualMachine->StackPushInteger(bListening) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053F100
int CSWVirtualMachineCommands::ExecuteCommandIsNPCPartyMember(int nCommandId, int nParameters)
{
    int nNPC;
    if (!g_pVirtualMachine->StackPopInteger(&nNPC)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    return g_pVirtualMachine->StackPushInteger(g_pAppManager->GetServerExoApp()->GetPartyTable()->GetIsMember(nNPC))
               ? 0
               : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053F160
int CSWVirtualMachineCommands::ExecuteCommandIsObjectPartyMember(int nCommandId, int nParameters)
{
    unsigned long oidCreature;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int bMember = 0;
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    if (pCreature) {
        bMember = pCreature->m_bInParty;
    }
    return g_pVirtualMachine->StackPushInteger(bMember) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053F1C0
int CSWVirtualMachineCommands::ExecuteCommandGetIsPC(int nCommandId, int nParameters)
{
    unsigned long oidCreature;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    return g_pVirtualMachine->StackPushInteger(g_pAppManager->GetServerExoApp()->GetClientObjectByObjectId(oidCreature) != NULL)
               ? 0
               : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053F3B0
int CSWVirtualMachineCommands::ExecuteCommandLocation(int nCommandId, int nParameters)
{
    Vector vPosition(0.0f, 0.0f, 0.0f);
    float fFacing;
    CScriptLocation* pLocation = new CScriptLocation;
    if (!g_pVirtualMachine->StackPopVector(&vPosition) || !g_pVirtualMachine->StackPopFloat(&fFacing)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    *(Vector*)pLocation->m_vPosition = vPosition;
    double fRadians = fFacing * 0.017453292519943295;
    *(Vector*)pLocation->m_vOrientation = Normalize(Vector((float)cos(fRadians), (float)sin(fRadians), 0.0f));
    if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_LOCATION, pLocation)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    delete pLocation;
    return 0;
}

// Also GetPositionFromLocation.
// FUNCTION: SWKOTOR 0x0053F500
int CSWVirtualMachineCommands::ExecuteCommandGetFacingFromLocation(int nCommandId, int nParameters)
{
    CScriptLocation* pLocation;
    if (!g_pVirtualMachine->StackPopEngineStructure(ENGINE_STRUCTURE_LOCATION, (void**)&pLocation)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int bResult = 1;
    switch (nCommandId) {
    case 0xdf:
        bResult = g_pVirtualMachine->StackPushVector(*(Vector*)pLocation->m_vPosition);
        break;
    case 0xe1: {
        float fFacing = (float)atan2(pLocation->m_vOrientation[1], pLocation->m_vOrientation[0]) * 57.29578f;
        bResult = g_pVirtualMachine->StackPushFloat(fFacing);
        break;
    }
    }
    if (pLocation) {
        delete pLocation;
    }
    return bResult ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// Also cos, sin, tan, acos, asin, atan, log, pow, sqrt and abs.
// FUNCTION: SWKOTOR 0x0053F5C0
int CSWVirtualMachineCommands::ExecuteCommandFabs(int nCommandId, int nParameters)
{
    float fValue;
    float fExponent;
    if (nCommandId == 0x4d) {
        int nValue;
        if (!g_pVirtualMachine->StackPopInteger(&nValue)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
        if (!g_pVirtualMachine->StackPushInteger(abs(nValue))) {
            return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
        }
        return 0;
    }
    float fResult = 0.0f;
    if (!g_pVirtualMachine->StackPopFloat(&fValue)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    switch (nCommandId) {
    case 0x43:
        fResult = (float)fabs(fValue);
        break;
    case 0x44:
        fResult = (float)cos(fValue * 0.017453292519943295);
        break;
    case 0x45:
        fResult = (float)sin(fValue * 0.017453292519943295);
        break;
    case 0x46:
        fResult = (float)tan(fValue * 0.017453292519943295);
        break;
    case 0x47:
        if (fabs(fValue) <= 1.0) {
            fResult = (float)acos(fValue) * 57.29578f;
        } else {
            fResult = 0.0f;
        }
        break;
    case 0x48:
        if (fabs(fValue) <= 1.0) {
            fResult = (float)asin(fValue) * 57.29578f;
        } else {
            fResult = 0.0f;
        }
        break;
    case 0x49:
        fResult = (float)atan(fValue) * 57.29578f;
        break;
    case 0x4a:
        if (fValue > 0.0f) {
            fResult = (float)log(fValue);
        } else {
            fResult = 0.0f;
        }
        break;
    case 0x4b:
        if (!g_pVirtualMachine->StackPopFloat(&fExponent)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
        if (fValue == 0.0 && fExponent < 0.0f) {
            fResult = 0.0f;
        } else {
            fResult = (float)pow(fValue, fExponent);
        }
        break;
    case 0x4c:
        if (fValue > 0.0f) {
            fResult = (float)sqrt(fValue);
        } else {
            fResult = 0.0f;
        }
        break;
    }
    if (!g_pVirtualMachine->StackPushFloat(fResult)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}
