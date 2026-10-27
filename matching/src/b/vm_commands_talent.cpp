// CSWVirtualMachineCommands::ExecuteCommandGetCreatureTalent (round s8):
// GetCreatureTalentRandom and GetCreatureTalentBest.
#include "b/vm_commands.h"
#include "b/sws_creature.h"

class CScriptTalent {
public:
    CScriptTalent();                                        // 0x004CA9E0
    ~CScriptTalent();                                       // 0x004015A0 (shared `ret`)

    int m_nType;                                            // 0x00
    int m_nId;                                              // 0x04
    unsigned char m_nMultiClass;                            // 0x08
    unsigned long m_oidItem;                                // 0x0c
    int m_nItemPropertyIndex;                               // 0x10
    unsigned char m_nCasterLevel;                           // 0x14
    unsigned char m_nMetaType;                              // 0x15
};

// FUNCTION: SWKOTOR 0x00536FE0
int CSWVirtualMachineCommands::ExecuteCommandGetCreatureTalent(int nCommandId, int nParameters)
{
    int nInclusion = 0;
    int nExcludeType = -1;
    int nExcludeId = -1;
    unsigned long oidCreature = INVALID_OBJECT_ID;
    int nCategory;
    if (!g_pVirtualMachine->StackPopInteger(&nCategory)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int nCRMax;
    if (nCommandId == 0x134) {
        if (!g_pVirtualMachine->StackPopInteger(&nCRMax)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
        if (nParameters > 2 && !g_pVirtualMachine->StackPopObject(&oidCreature)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
        if (nParameters > 3 && !g_pVirtualMachine->StackPopInteger(&nInclusion)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
        if (nParameters > 4 && !g_pVirtualMachine->StackPopInteger(&nExcludeType)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
        if (nParameters > 5 && !g_pVirtualMachine->StackPopInteger(&nExcludeId)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
    } else if (nCommandId == 0x133) {
        if (nParameters > 1 && !g_pVirtualMachine->StackPopObject(&oidCreature)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
        if (nParameters > 2 && !g_pVirtualMachine->StackPopInteger(&nInclusion)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
    }
    CScriptTalent talent;
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    if (pCreature) {
        int nId;
        int nType;
        unsigned char nMultiClass;
        unsigned long oidItem;
        int nItemPropertyIndex;
        unsigned char nCasterLevel;
        if (nCommandId == 0x134) {
            pCreature->GetStats()->GetCreatureTalent(
                nCategory, nCRMax, &nType, &nId, &nMultiClass, &oidItem, &nItemPropertyIndex, &nCasterLevel,
                nInclusion, nExcludeType, nExcludeId);
        } else if (nCommandId == 0x133) {
            pCreature->GetStats()->GetCreatureTalent(
                nCategory, -1, &nType, &nId, &nMultiClass, &oidItem, &nItemPropertyIndex, &nCasterLevel,
                nInclusion, -1, -1);
        }
        talent.m_nId = nId;
        talent.m_nType = nType;
        talent.m_nMultiClass = nMultiClass;
        talent.m_oidItem = oidItem;
        talent.m_nItemPropertyIndex = nItemPropertyIndex;
        talent.m_nCasterLevel = nCasterLevel;
    } else {
        talent.m_nId = -1;
    }
    if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_TALENT, &talent)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}
