// CSWSObjectActionNode destructor 0x004CC4D0 (own file so /Ob2
// does not inline it into the action-queue members).
#include "i/sws_object.h"

// FUNCTION: SWKOTOR 0x004CC4D0
CSWSObjectActionNode::~CSWSObjectActionNode()
{
    for (int i = 0; i < m_nNumParams; i++) {
        if (m_pParam[i]) {
            if (m_nParamType[i] == 4) {
                delete (CExoString*)m_pParam[i];
                m_pParam[i] = NULL;
            } else if (m_nParamType[i] == 5 && g_pVirtualMachine) {
                g_pVirtualMachine->DeleteScriptSituation((CVirtualMachineScript*)m_pParam[i]);
                m_pParam[i] = NULL;
            }
        }
    }
}
