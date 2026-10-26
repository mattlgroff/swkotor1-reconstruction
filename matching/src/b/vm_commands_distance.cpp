// CSWVirtualMachineCommands: distances between objects and locations
// (GetDistanceBetween*, GetDistanceToObject*), 0x00537370-0x00537960.
#include "b/vm_commands.h"
#include "b/sws_creature.h"
#include "b/global_variable_table.h"

extern const unsigned char OBJECT_TYPE_AREA;                // 0x00746633

float Magnitude(const Vector& v);                           // 0x004AB100

// The original inlines vector subtraction here (0x00423BC0 is its
// out-of-line copy).
inline Vector VectorSubtract(const Vector& a, const Vector& b)
{
    return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
}

// FUNCTION: SWKOTOR 0x00537370
int CSWVirtualMachineCommands::ExecuteCommandGetDistanceBetween(int nCommandId, int nParameters)
{
    float fDistance = 0.0f;
    CGameObjectArray* pObjectArray = g_pAppManager->GetServerExoApp()->GetObjectArray();
    unsigned long oidObjectA;
    unsigned long oidObjectB;
    if (!g_pVirtualMachine->StackPopObject(&oidObjectA) ||
        !g_pVirtualMachine->StackPopObject(&oidObjectB)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObjectA;
    CGameObject* pObjectB;
    if (pObjectArray->GetGameObject(oidObjectA, &pObjectA) == GAME_OBJECT_VALID &&
        pObjectArray->GetGameObject(oidObjectB, &pObjectB) == GAME_OBJECT_VALID) {
        Vector vDelta = VectorSubtract(pObjectA->m_vPosition, pObjectB->m_vPosition);
        fDistance = Magnitude(vDelta);
    }
    return g_pVirtualMachine->StackPushFloat(fDistance) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00537470
int CSWVirtualMachineCommands::ExecuteCommandGetDistanceBetween2D(int nCommandId, int nParameters)
{
    float fDistance = 0.0f;
    CGameObjectArray* pObjectArray = g_pAppManager->GetServerExoApp()->GetObjectArray();
    unsigned long oidObjectA;
    unsigned long oidObjectB;
    if (!g_pVirtualMachine->StackPopObject(&oidObjectA) ||
        !g_pVirtualMachine->StackPopObject(&oidObjectB)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObjectA;
    CGameObject* pObjectB;
    if (pObjectArray->GetGameObject(oidObjectA, &pObjectA) == GAME_OBJECT_VALID &&
        pObjectArray->GetGameObject(oidObjectB, &pObjectB) == GAME_OBJECT_VALID) {
        Vector vA = pObjectA->m_vPosition;
        Vector vB = pObjectB->m_vPosition;
        Vector vDelta;
        vDelta.x = vA.x - vB.x;
        vDelta.y = vA.y - vB.y;
        vDelta.z = 0.0f;
        fDistance = Magnitude(vDelta);
    }
    return g_pVirtualMachine->StackPushFloat(fDistance) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00537590
int CSWVirtualMachineCommands::ExecuteCommandGetDistanceBetweenLocations(int nCommandId, int nParameters)
{
    CScriptLocation* pLocationA;
    CScriptLocation* pLocationB;
    if (!g_pVirtualMachine->StackPopEngineStructure(ENGINE_STRUCTURE_LOCATION, (void**)&pLocationA)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopEngineStructure(ENGINE_STRUCTURE_LOCATION, (void**)&pLocationB)) {
        if (pLocationA) {
            delete pLocationA;
        }
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    Vector vDelta = VectorSubtract(*(Vector*)pLocationA->m_vPosition, *(Vector*)pLocationB->m_vPosition);
    float fDistance = Magnitude(vDelta);
    if (!g_pVirtualMachine->StackPushFloat(fDistance)) {
        if (pLocationA) {
            delete pLocationA;
            pLocationA = NULL;
        }
        if (pLocationB) {
            delete pLocationB;
            pLocationB = NULL;
        }
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    if (pLocationA) {
        delete pLocationA;
        pLocationA = NULL;
    }
    if (pLocationB) {
        delete pLocationB;
        pLocationB = NULL;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005376C0
int CSWVirtualMachineCommands::ExecuteCommandGetDistanceBetweenLocations2D(int nCommandId, int nParameters)
{
    CScriptLocation* pLocationA;
    CScriptLocation* pLocationB;
    if (!g_pVirtualMachine->StackPopEngineStructure(ENGINE_STRUCTURE_LOCATION, (void**)&pLocationA)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopEngineStructure(ENGINE_STRUCTURE_LOCATION, (void**)&pLocationB)) {
        if (pLocationA) {
            delete pLocationA;
        }
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    Vector vA = *(Vector*)pLocationA->m_vPosition;
    Vector vB = *(Vector*)pLocationB->m_vPosition;
    Vector vDelta;
    vDelta.x = vA.x - vB.x;
    vDelta.y = vA.y - vB.y;
    vDelta.z = 0.0f;
    float fDistance = Magnitude(vDelta);
    if (!g_pVirtualMachine->StackPushFloat(fDistance)) {
        if (pLocationA) {
            delete pLocationA;
            pLocationA = NULL;
        }
        if (pLocationB) {
            delete pLocationB;
            pLocationB = NULL;
        }
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    if (pLocationA) {
        delete pLocationA;
        pLocationA = NULL;
    }
    if (pLocationB) {
        delete pLocationB;
        pLocationB = NULL;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00537820
int CSWVirtualMachineCommands::ExecuteCommandGetDistanceToObject(int nCommandId, int nParameters)
{
    float fDistance = -1.0f;
    unsigned long oidObject;
    if (!g_pVirtualMachine->StackPopObject(&oidObject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pSelf;
    if (m_bValidObjectRunScript == 1 &&
        g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pSelf) == GAME_OBJECT_VALID) {
        Vector vSelf = pSelf->m_vPosition;
        CSWSArea* pArea = ((CSWSObject*)pSelf)->GetArea();
        CGameObject* pObject;
        if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidObject, &pObject) == GAME_OBJECT_VALID &&
            pObject && pObject->m_nObjectType > OBJECT_TYPE_AREA) {
            Vector vObject = pObject->m_vPosition;
            if (((CSWSObject*)pObject)->GetArea() == pArea) {
                Vector vDelta = VectorSubtract(vObject, vSelf);
                fDistance = Magnitude(vDelta);
            }
        }
    }
    return g_pVirtualMachine->StackPushFloat(fDistance) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00537960
int CSWVirtualMachineCommands::ExecuteCommandGetDistanceToObject2D(int nCommandId, int nParameters)
{
    float fDistance = -1.0f;
    unsigned long oidObject;
    if (!g_pVirtualMachine->StackPopObject(&oidObject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pSelf;
    if (m_bValidObjectRunScript == 1 &&
        g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pSelf) == GAME_OBJECT_VALID) {
        Vector vSelf = pSelf->m_vPosition;
        CSWSArea* pArea = ((CSWSObject*)pSelf)->GetArea();
        CGameObject* pObject;
        if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidObject, &pObject) == GAME_OBJECT_VALID &&
            pObject && pObject->m_nObjectType > OBJECT_TYPE_AREA) {
            Vector vObject = pObject->m_vPosition;
            if (((CSWSObject*)pObject)->GetArea() == pArea) {
                Vector vDelta;
                vDelta.x = vObject.x - vSelf.x;
                vDelta.y = vObject.y - vSelf.y;
                vDelta.z = 0.0f;
                fDistance = Magnitude(vDelta);
            }
        }
    }
    return g_pVirtualMachine->StackPushFloat(fDistance) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

#include <math.h>

extern int g_bExoDebugDisabled;                             // 0x0083282C (.data)

// Stripped debug output: the arguments are still evaluated.
inline void ExoDebugLog(const char* sText) {}

// FUNCTION: SWKOTOR 0x00537FE0
int CSWVirtualMachineCommands::ExecuteCommandGetFacing(int nCommandId, int nParameters)
{
    float fFacing = -1.0f;
    unsigned long oidObject;
    Vector vOrientation(0.0f, 0.0f, 0.0f);
    if (!g_pVirtualMachine->StackPopObject(&oidObject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidObject, &pObject) == GAME_OBJECT_VALID &&
        pObject && pObject->m_nObjectType > OBJECT_TYPE_AREA) {
        vOrientation = pObject->m_vOrientation;
        vOrientation.z = 0.0f;
        Vector vNormal = Normalize(vOrientation);
        fFacing = (float)atan2(vOrientation.y, vOrientation.x) * 57.29578f;
        if (vNormal.y < 0.0f) {
            fFacing = fFacing + 360.0f;
        }
    }
    return g_pVirtualMachine->StackPushFloat(fFacing) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// AngleToVector and VectorToAngle share this handler.
// FUNCTION: SWKOTOR 0x005451C0
int CSWVirtualMachineCommands::ExecuteCommandAngleToVector(int nCommandId, int nParameters)
{
    if (nCommandId == 0x91) {
        Vector vVector(0.0f, 0.0f, 0.0f);
        if (!g_pVirtualMachine->StackPopVector(&vVector)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
        vVector = Normalize(vVector);
        float fAngle = (float)acos(vVector.x);
        if (vVector.y < 0.0f) {
            fAngle = 360.0f - fAngle;
        }
        if (!g_pVirtualMachine->StackPushFloat(fAngle)) {
            return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
        }
    } else if (nCommandId == 0x90) {
        float fAngle;
        Vector vVector(0.0f, 0.0f, 0.0f);
        if (!g_pVirtualMachine->StackPopFloat(&fAngle)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
        float fRadians = fAngle * 0.017453292f;
        vVector.x = (float)cos(fRadians);
        vVector.y = (float)sin(fRadians);
        vVector.z = 0.0f;
        if (!g_pVirtualMachine->StackPushVector(vVector)) {
            return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005414D0
int CSWVirtualMachineCommands::ExecuteCommandPrintFloat(int nCommandId, int nParameters)
{
    int nWidth;
    int nDecimals;
    int bCenter;
    float fFloat;
    if (!g_pVirtualMachine->StackPopFloat(&fFloat)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters >= 2) {
        if (!g_pVirtualMachine->StackPopInteger(&nWidth)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
        if (nWidth >= 0x12) {
            nWidth = 0x12;
        } else if (nWidth < 0) {
            nWidth = 0;
        }
    } else {
        nWidth = 0x12;
    }
    if (nParameters >= 3) {
        if (!g_pVirtualMachine->StackPopInteger(&nDecimals)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
        if (nDecimals >= 10) {
            nDecimals = 9;
        } else if (nDecimals < 0) {
            nDecimals = 0;
        }
    } else {
        nDecimals = 9;
    }
    if (nParameters >= 4) {
        if (!g_pVirtualMachine->StackPopInteger(&bCenter)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
    } else {
        bCenter = 0;
    }
    CExoString sFormat;
    if (bCenter == 0) {
        if (nWidth >= 10) {
            sFormat.Format("%%%2d.%02df\n", nWidth, nDecimals);
        } else {
            sFormat.Format("%%%1d.%02df\n", nWidth, nDecimals);
        }
    } else {
        if (nWidth >= 10) {
            sFormat.Format("PRINTFLOAT2: %%%2d.%02df\n", nWidth, nDecimals);
        } else {
            sFormat.Format("PRINTFLOAT2: %%%1d.%02df\n", nWidth, nDecimals);
        }
    }
    if (!g_bExoDebugDisabled) {
        ExoDebugLog(sFormat.CStr());
    }
    return 0;
}

// NONMATCH (17): x87 frame/register allocation; the format values were
// checked against the original executable.
// FUNCTION: SWKOTOR 0x00536000
int CSWVirtualMachineCommands::ExecuteCommandFloatToString(int nCommandId, int nParameters)
{
    float fValue;
    int nWidth;
    int nDecimals;
    if (!g_pVirtualMachine->StackPopFloat(&fValue)) return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    if (!g_pVirtualMachine->StackPopInteger(&nWidth)) return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    if (nWidth >= 18) nWidth = 18;
    else if (nWidth < 0) nWidth = 0;
    if (!g_pVirtualMachine->StackPopInteger(&nDecimals)) return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    if (nDecimals >= 10) nDecimals = 9;
    else if (nDecimals < 0) nDecimals = 0;
    CExoString sFormat;
    if (nWidth >= 10) sFormat.Format("%%%2d.%02df\n", nWidth, nDecimals);
    else sFormat.Format("%%%1d.%02df\n", nWidth, nDecimals);
    CExoString sResult;
    sResult.Format(sFormat.CStr(), fValue);
    CExoString sPush(sResult.CStr());
    if (!g_pVirtualMachine->StackPushString(sPush)) return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    return 0;
}

// FUNCTION: SWKOTOR 0x00547760
int CSWVirtualMachineCommands::ExecuteCommandFaceObjectAwayFromObject(int nCommandId, int nParameters)
{
    CSWSObject* pFacer = NULL;
    CSWSObject* pAwayFrom = NULL;
    unsigned long oidFacer;
    unsigned long oidAwayFrom;
    if (!g_pVirtualMachine->StackPopObject(&oidFacer)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopObject(&oidAwayFrom)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidFacer);
    if (pObject) {
        pFacer = pObject->AsSWSObject();
    }
    pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidAwayFrom);
    if (pObject) {
        pAwayFrom = pObject->AsSWSObject();
    }
    if (pFacer && pAwayFrom && pFacer->m_oidArea == pAwayFrom->m_oidArea) {
        Vector vDirection(0.0f, 0.0f, 0.0f);
        vDirection = pFacer->m_vPosition - pAwayFrom->m_vPosition;
        if (vDirection.y * vDirection.y + vDirection.x * vDirection.x + vDirection.z * vDirection.z > 0.0001f) {
            vDirection = Normalize(vDirection);
            pFacer->SetOrientation(vDirection);
        }
    }
    return 0;
}
