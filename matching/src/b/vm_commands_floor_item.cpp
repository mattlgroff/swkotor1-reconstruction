// Create an item at a script location in the active module.
#include "b/vm_commands.h"
#include "b/sws_message.h"
#include "b/sws_item.h"

class CScriptLocation {
public:
    ~CScriptLocation(); // 0x004015A0
    Vector m_vPosition;
    Vector m_vOrientation;
};
extern const unsigned long OBJECT_INVALID; // 0x00746644

// NONMATCH (30): frame-slot allocation and error-branch placement differ.
// FUNCTION: SWKOTOR 0x00547F60
int CSWVirtualMachineCommands::ExecuteCommandCreateItemOnFloor(int nCommandId, int nParameters)
{
    CSWSMessage* pMessage = g_pAppManager->GetServerExoApp()->GetSWSMessage();
    unsigned long oidResult = OBJECT_INVALID;
    CExoString sTemplate;
    CScriptLocation* pLocation = 0;
    int bAppear = 0;
    if (!g_pVirtualMachine->StackPopString(&sTemplate) ||
        !g_pVirtualMachine->StackPopEngineStructure(2, (void**)&pLocation) ||
        !g_pVirtualMachine->StackPopInteger(&bAppear)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSModule* pModule = g_pAppManager->GetServerExoApp()->GetModule();
    if (pModule) {
        CSWSArea* pArea = pModule->GetArea();
        if (pArea) {
            CResRef cTemplate(sTemplate.CStr());
            CSWSItem* pItem = new CSWSItem(INVALID_OBJECT_ID);
            pItem->LoadFromTemplate(cTemplate);
            pItem->SetPosition(pLocation->m_vPosition, 1, 1, 0);
            pItem->SetOrientation(pLocation->m_vOrientation);
            pItem->AddToArea(pArea, pLocation->m_vPosition.x, pLocation->m_vPosition.y, pLocation->m_vPosition.z, 1);
            oidResult = pItem->m_idSelf;
            if (pMessage) {
                Vector vPosition(pLocation->m_vPosition.x, pLocation->m_vPosition.y, pLocation->m_vPosition.z);
                pMessage->SendServerToPlayerItemScriptAdd(OBJECT_INVALID, pArea, pItem, vPosition);
            }
        }
    }
    int bPushed = g_pVirtualMachine->StackPushObject(oidResult);
    if (pLocation) { delete pLocation; pLocation = 0; }
    return bPushed ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}
