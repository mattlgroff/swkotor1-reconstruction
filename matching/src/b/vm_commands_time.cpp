// CSWVirtualMachineCommands: GetTag and the small integer getters after it
// (world time, effect counts, per-object values; 0x0053DF00-0x0053E2B0).
#include "b/vm_commands.h"
#include "b/sws_creature.h"
#include "b/party_table.h"

extern const unsigned char OBJECT_TYPE_MODULE;              // 0x00746632 (3)
extern const unsigned char OBJECT_TYPE_AREA;                // 0x00746633 (4)

// Per-object value read by 0x0053E220 (+0x150 on plain objects, +0x314 on
// the vtable 0x5c kind).
class CGameObjectValueView : public CGameObject {
public:
    char pad_0xf4[0x150 - 0xf4];
    int m_n150;                                             // 0x150 (name guessed)
};

struct CGameObject5CView {
    char pad_0x0[0x314];
    int m_n314;                                             // 0x314 (name guessed)
};

// FUNCTION: SWKOTOR 0x0053DF00
int CSWVirtualMachineCommands::ExecuteCommandGetTag(int nCommandId, int nParameters)
{
    CExoString sTag;
    unsigned long oidObject;
    if (!g_pVirtualMachine->StackPopObject(&oidObject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidObject, &pObject) == GAME_OBJECT_VALID) {
        if (pObject->m_nObjectType == OBJECT_TYPE_MODULE) {
            sTag = pObject->AsSWSModule()->m_sTag;
        } else if (pObject->m_nObjectType == OBJECT_TYPE_AREA) {
            sTag = pObject->AsSWSArea()->m_sTag;
        } else {
            sTag = pObject->m_sTag;
        }
    }
    if (!g_pVirtualMachine->StackPushString(sTag)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0053E020
int CSWVirtualMachineCommands::ExecuteCommandGetTimeHour(int nCommandId, int nParameters)
{
    return g_pVirtualMachine->StackPushInteger(g_pAppManager->GetServerExoApp()->GetWorldTimer()->GetWorldTimeHour()) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053E060
int CSWVirtualMachineCommands::ExecuteCommandGetTimeMillisecond(int nCommandId, int nParameters)
{
    return g_pVirtualMachine->StackPushInteger(g_pAppManager->GetServerExoApp()->GetWorldTimer()->GetWorldTimeMillisecond()) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053E0A0
int CSWVirtualMachineCommands::ExecuteCommandGetTimeMinute(int nCommandId, int nParameters)
{
    return g_pVirtualMachine->StackPushInteger(g_pAppManager->GetServerExoApp()->GetWorldTimer()->GetWorldTimeMinute()) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053E0E0
int CSWVirtualMachineCommands::ExecuteCommandGetTimeSecond(int nCommandId, int nParameters)
{
    return g_pVirtualMachine->StackPushInteger(g_pAppManager->GetServerExoApp()->GetWorldTimer()->GetWorldTimeSecond()) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053E120
int CSWVirtualMachineCommands::ExecuteCommandGetEffectCount(int nCommandId, int nParameters)
{
    int nCount = 0;
    CSWSObject* pObject;
    CGameObject* pGameObject = g_pAppManager->GetServerExoApp()->GetGameObject(m_oidObjectRunScript);
    if (pGameObject && pGameObject->AsSWSObject()) {
        pObject = pGameObject->AsSWSObject();
    }
    if (pObject) {
        for (int i = 0; i < 15; i++) {
            unsigned long nMask = 1 << i;
            if (pObject->GetEffectCount(nMask) > 0) {
                nCount += pObject->GetEffectCount(nMask);
            }
        }
    }
    return g_pVirtualMachine->StackPushInteger(nCount) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053E1B0
int CSWVirtualMachineCommands::ExecuteCommandGetCreatureValue(int nCommandId, int nParameters)
{
    int nResult = 0;
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID &&
        pObject->AsSWSCreature()) {
        nResult = pObject->AsSWSCreature()->GetCreatureValue518B30();
    }
    return g_pVirtualMachine->StackPushInteger(nResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053E220
int CSWVirtualMachineCommands::ExecuteCommandGetObjectValue(int nCommandId, int nParameters)
{
    int nResult = -1;
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID) {
        switch (pObject->m_nObjectType) {
        case 3:
            nResult = pObject->AsSWSModule()->m_n148;
            break;
        case 4:
            nResult = pObject->AsSWSArea()->m_n1DC;
            break;
        case 0xd:
            nResult = ((CGameObject5CView*)pObject->AsSWS5C())->m_n314;
            break;
        default:
            nResult = ((CGameObjectValueView*)pObject)->m_n150;
            break;
        }
    }
    return g_pVirtualMachine->StackPushInteger(nResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053ECB0
int CSWVirtualMachineCommands::ExecuteCommandGetIsBusy(int nCommandId, int nParameters)
{
    unsigned long oidObject;
    int bResult = 1;
    if (!g_pVirtualMachine->StackPopObject(&oidObject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidObject);
    if (pObject) {
        if (pObject->AsSWSCreature()) {
            if (pObject->AsSWSCreature()->GetObjectState94() ||
                pObject->AsSWSCreature()->GetIsPaused() ||
                pObject->AsSWSCreature()->GetDeadTemp()) {
                bResult = 1;
            } else {
                bResult = 0;
            }
        } else {
            bResult = pObject->GetObjectState94();
        }
    }
    return g_pVirtualMachine->StackPushInteger(bResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053ED70
int CSWVirtualMachineCommands::ExecuteCommandGetNPCValue(int nCommandId, int nParameters)
{
    int nNPC;
    int nResult = 0;
    if (!g_pVirtualMachine->StackPopInteger(&nNPC)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    switch (nCommandId) {
    case 0x2b8:
        nResult = g_pAppManager->GetServerExoApp()->GetPartyTable()->GetNPCAvailable(nNPC);
        break;
    case 0x2c5:
        nResult = g_pAppManager->GetServerExoApp()->GetPartyTable()->GetNPCSelectable(nNPC);
        break;
    }
    return g_pVirtualMachine->StackPushInteger(nResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053EEC0
int CSWVirtualMachineCommands::ExecuteCommandGetSameObject54(int nCommandId, int nParameters)
{
    unsigned long oidObject;
    if (!g_pVirtualMachine->StackPopObject(&oidObject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int bResult;
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidObject);
    if (pObject && pObject->AsSWSObject()) {
        unsigned long oid54 = pObject->AsSWSObject()->m_oid54;
        pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oid54);
        if (pObject && pObject->AsSWSObject()) {
            bResult = pObject->AsSWSObject()->m_oid54 == oid54;
        } else {
            bResult = 0;
        }
    } else {
        bResult = 0;
    }
    return g_pVirtualMachine->StackPushInteger(bResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

class CSWGuiInGame {
public:
    char pad_0x0[0xb4];
    int m_nB4;                                              // 0xb4 (name guessed)
};

// FUNCTION: SWKOTOR 0x0053EF70
int CSWVirtualMachineCommands::ExecuteCommandGetGuiValueB4(int nCommandId, int nParameters)
{
    int nValue = g_pAppManager->GetClientExoApp()->GetInGameGui()->m_nB4;
    return g_pVirtualMachine->StackPushInteger(nValue) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053EFA0
int CSWVirtualMachineCommands::ExecuteCommandGetCreatureStat194Or1(int nCommandId, int nParameters)
{
    unsigned long oidCreature;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int nResult = 1;
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    if (pCreature) {
        nResult = pCreature->m_pStats->m_n194;
    }
    return g_pVirtualMachine->StackPushInteger(nResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053EBC0
int CSWVirtualMachineCommands::ExecuteCommandIntToString(int nCommandId, int nParameters)
{
    CExoString sResult;
    int nValue;
    if (!g_pVirtualMachine->StackPopInteger(&nValue)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    {
        CExoString sValue(nValue);
        sResult = sValue;
    }
    if (!g_pVirtualMachine->StackPushString(sResult)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0053F010
int CSWVirtualMachineCommands::ExecuteCommandGetLastConversation(int nCommandId, int nParameters)
{
    // As shipped: a successful push returns -2000, a failed one 0.
    int bPushed = g_pVirtualMachine->StackPushString(g_pAppManager->GetServerExoApp()->GetLastConversation());
    return bPushed ? VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW : 0;
}
