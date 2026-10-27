// CSWVirtualMachineCommands::ExecuteCommandStartNewModule (round s8).
#include "b/vm_commands.h"
#include "b/server_exo_app.h"
#include "b/vector.h"

class CSWGuiInGame {
public:
    void StartGlobalFade(int nType, float fArg1, float fArg2, Vector* pColor); // 0x0062ABF0

    char pad_0x0[0xb4];
    int m_nB4;                                              // 0xb4 (name guessed)
    char pad_0xb8[0xc04 - 0xb8];
    int m_nC04;                                             // 0xc04 (name guessed)
};

// FUNCTION: SWKOTOR 0x00544390
int CSWVirtualMachineCommands::ExecuteCommandStartNewModule(int nCommandId, int nParameters)
{
    CExoString sModuleName;
    if (!g_pVirtualMachine->StackPopString(&sModuleName)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CExoString sWayPoint;
    if (!g_pVirtualMachine->StackPopString(&sWayPoint)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CExoString sMovie1;
    if (!g_pVirtualMachine->StackPopString(&sMovie1)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CExoString sMovie2;
    if (!g_pVirtualMachine->StackPopString(&sMovie2)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CExoString sMovie3;
    CExoString sMovie4;
    CExoString sMovie5;
    CExoString sMovie6;
    if (nParameters > 4) {
        if (!g_pVirtualMachine->StackPopString(&sMovie3) || !g_pVirtualMachine->StackPopString(&sMovie4) ||
            !g_pVirtualMachine->StackPopString(&sMovie5) || !g_pVirtualMachine->StackPopString(&sMovie6)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
    }
    CServerExoApp* pServer = g_pAppManager->m_pServerExoApp;
    CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
    pServer->SetMoveToModulePending(1);
    pServer->SetMoveToModuleString(sModuleName);
    pServer->SetMoveToModuleStartWaypoint(sWayPoint);
    pClient->AddMoveToModuleMovie(sMovie1);
    pClient->AddMoveToModuleMovie(sMovie2);
    pClient->AddMoveToModuleMovie(sMovie3);
    pClient->AddMoveToModuleMovie(sMovie4);
    pClient->AddMoveToModuleMovie(sMovie5);
    pClient->AddMoveToModuleMovie(sMovie6);
    CSWGuiInGame* pGui = g_pAppManager->GetClientExoApp()->GetInGameGui();
    if (pGui && (pGui->m_nB4 || pGui->m_nC04)) {
        Vector vColor(0.0f, 0.0f, 0.0f);
        g_pAppManager->GetClientExoApp()->GetInGameGui()->StartGlobalFade(0, 0.0f, 0.0f, &vColor);
    }
    return 0;
}
