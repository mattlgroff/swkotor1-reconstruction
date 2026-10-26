// DisplayFeedBackText: look up a talk-table row for a client object.
#include "b/vm_commands.h"

class CSWCObject {
public:
    void SetNameInfo(const CExoString& sName);              // 0x0063D2D0
};
class ClientGameObjectView {
public:
    virtual void v00(); virtual void v04(); virtual void v08();
    virtual CSWCObject* AsSWCObject();                       // slot 0x0c
};
class CTlkTable {
public:
    CExoString GetSimpleString(unsigned long nStrRef);       // 0x0041E8F0
};
struct FeedbackTablesView {
    char pad_0x0[0x108];
    C2DA* m_pFeedbackText;
};
extern CTlkTable* g_pTlkTable;                              // 0x007A3A08

// NONMATCH (18): table and return-value register ordering.
// FUNCTION: SWKOTOR 0x00530170
int CSWVirtualMachineCommands::ExecuteCommandDisplayFeedBackText(int nCommandId, int nParameters)
{
    unsigned long oidCreature;
    int nRow;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    oidCreature = g_pAppManager->GetClientExoApp()->ServerToClientObjectId(oidCreature);
    if (!g_pVirtualMachine->StackPopInteger(&nRow)) return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    ClientGameObjectView* pObject = (ClientGameObjectView*)g_pAppManager->GetClientExoApp()->GetGameObject(oidCreature);
    int nStrRef = 0;
    if (pObject && pObject->AsSWCObject()) {
        {
            CExoString sColumn("StrRef");
            ((FeedbackTablesView*)g_pRules->m_p2DArrays)->m_pFeedbackText->GetINTEntry(nRow, sColumn, &nStrRef);
        }
        if (nStrRef != 0) {
            CExoString sText = g_pTlkTable->GetSimpleString(nStrRef);
            pObject->AsSWCObject()->SetNameInfo(sText);
        } else {
            CExoString sText("BAD STRREF");
            pObject->AsSWCObject()->SetNameInfo(sText);
        }
    }
    return 0;
}
