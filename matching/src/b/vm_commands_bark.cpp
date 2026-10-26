// VM BarkString: fetch a talk-table entry and pass it to the client.
#include "b/vm_commands.h"
#include "b/exo_string.h"
#include "b/sws_item.h"

struct STR_RES {
    STR_RES();                                              // 0x00414DC0
    CExoString m_sText;
    CResRef m_cSound;
    float m_fSoundLength;
};
class CTlkTable {
public:
    int Fetch(unsigned long nStrRef, STR_RES& sResult, unsigned char nGender); // 0x0041E550
};
class CGuiInGame {
public:
    void ShowBarkBubble(unsigned long oidSpeaker, const CExoString& sText,
                        const CResRef& cSound, int bArg);     // 0x0062AB30
    char pad_0x0[0xb4];
    int m_nB4;
};
extern CTlkTable* g_pTlkTable;                              // 0x007A3A08
extern unsigned char g_nGender;                             // 0x0073ECD0

// NONMATCH (11): GUI state uses a different scratch register.
// FUNCTION: SWKOTOR 0x00548900
int CSWVirtualMachineCommands::ExecuteCommandBarkString(int nCommandId, int nParameters)
{
    int nStrRef;
    unsigned long oidSpeaker;
    if (!g_pVirtualMachine->StackPopObject(&oidSpeaker)) return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    if (!g_pVirtualMachine->StackPopInteger(&nStrRef)) return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    if (((CGuiInGame*)g_pAppManager->GetClientExoApp()->GetInGameGui())->m_nB4) return 0;
    {
        STR_RES sResult;
        g_pTlkTable->Fetch(nStrRef, sResult, g_nGender);
        CResRef cSound = sResult.m_cSound;
        CExoString sText = sResult.m_sText;
        if (oidSpeaker == 0x7f000000) {
            ((CGuiInGame*)g_pAppManager->GetClientExoApp()->GetInGameGui())->ShowBarkBubble(oidSpeaker, sText, cSound, 0);
        } else {
            oidSpeaker = g_pAppManager->GetClientExoApp()->ServerToClientObjectId(oidSpeaker);
            g_pAppManager->GetClientExoApp()->BarkString(oidSpeaker, sText, cSound, 0);
        }
    }
    return 0;
}
