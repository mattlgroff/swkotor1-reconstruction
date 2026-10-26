// CSWVirtualMachineCommands: the command implementer's own virtuals
// (vtable 0x007450E4): destructor, command dispatch, script-start hook and
// the engine-structure (effect / event / location / talent) helpers.
#include "b/vm_commands.h"
#include "b/vm_effect.h"
#include "b/global_variable_table.h"

class CServerAIMaster {
public:
    void AppendToScriptsRun(const CExoString& sScript);     // 0x0052BFC0
};

class CScriptTalent {
public:
    CScriptTalent();                                        // 0x004CA9E0
    void CopyScriptTalent(CScriptTalent* pOther);           // 0x004CAA10
    int SaveTalent(CResGFF* pRes, CResStruct* pStruct);     // 0x004CAA50
    int LoadTalent(CResGFF* pRes, CResStruct* pStruct);     // 0x004CAAF0
    int Equals(CScriptTalent* pOther);                      // 0x004CABB0

    int m_nType;                                            // 0x00
    int m_nId;                                              // 0x04
    unsigned char m_nMultiClass;                            // 0x08
    unsigned long m_oidItem;                                // 0x0c
    int m_nItemPropertyIndex;                               // 0x10
    unsigned char m_nCasterLevel;                           // 0x14
    unsigned char m_nMetaType;                              // 0x15
};

// FUNCTION: SWKOTOR 0x0052C070
CSWVirtualMachineCommands::~CSWVirtualMachineCommands()
{
    if (m_pCommands) {
        delete[] m_pCommands;
        m_pCommands = NULL;
    }
}

// FUNCTION: SWKOTOR 0x0052C0D0
int CSWVirtualMachineCommands::RunCommand(int nCommandId, int nParameters)
{
    if (nCommandId < 0x304 && m_pCommands[nCommandId]) {
        return (this->*m_pCommands[nCommandId])(nCommandId, nParameters);
    }
    return -2002;
}

// FUNCTION: SWKOTOR 0x0052C100
void CSWVirtualMachineCommands::RunScriptStarted(CExoString* sScriptFile)
{
    CServerAIMaster* pAIMaster = g_pAppManager->GetServerExoApp()->GetServerAIMaster();
    if (*sScriptFile != "") {
        pAIMaster->AppendToScriptsRun(*sScriptFile);
    }
}

// FUNCTION: SWKOTOR 0x0052C140
void* CSWVirtualMachineCommands::CopyGameDefinedStructure(int nType, void* pStructure)
{
    if (nType == 0) {
        CGameEffect* pEffect = new CGameEffect(0);
        pEffect->CopyEffect((CGameEffect*)pStructure, 0);
        return pEffect;
    }
    if (nType == 1) {
        CScriptEvent* pEvent = new CScriptEvent;
        pEvent->CopyScriptEvent((CScriptEvent*)pStructure);
        return pEvent;
    }
    if (nType == 2) {
        CScriptLocation* pLocation = new CScriptLocation;
        pLocation->CopyScriptLocation((CScriptLocation*)pStructure);
        return pLocation;
    }
    if (nType == 3) {
        CScriptTalent* pTalent = new CScriptTalent;
        pTalent->CopyScriptTalent((CScriptTalent*)pStructure);
        return pTalent;
    }
    return NULL;
}

// FUNCTION: SWKOTOR 0x0052C2D0
int CSWVirtualMachineCommands::SaveGameDefinedStructure(int nType, void* pStructure, CResGFF* pRes, CResStruct* pStruct)
{
    CResStruct cStruct;
    pRes->AddStructToStruct(&cStruct, pStruct, "GameDefinedStrct", nType);
    if (nType == 0) {
        ((CGameEffect*)pStructure)->SaveGameEffect(pRes, &cStruct);
        return 1;
    }
    if (nType == 1) {
        return ((CScriptEvent*)pStructure)->SaveEvent(pRes, &cStruct);
    }
    if (nType == 2) {
        return ((CScriptLocation*)pStructure)->SaveLocation(pRes, &cStruct);
    }
    if (nType == 3) {
        return ((CScriptTalent*)pStructure)->SaveTalent(pRes, &cStruct);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0052C360
int CSWVirtualMachineCommands::LoadGameDefinedStructure(int nType, void** ppStructure, CResGFF* pRes, CResStruct* pStruct)
{
    CResStruct cStruct;
    pRes->GetStructFromStruct(&cStruct, pStruct, "GameDefinedStrct");
    if (nType == 0) {
        CGameEffect* pEffect = new CGameEffect(1);
        *ppStructure = pEffect;
        pEffect->LoadGameEffect(pRes, &cStruct);
        return 1;
    }
    if (nType == 1) {
        CScriptEvent* pEvent = new CScriptEvent;
        *ppStructure = pEvent;
        return pEvent->LoadEvent(pRes, &cStruct);
    }
    if (nType == 2) {
        CScriptLocation* pLocation = new CScriptLocation;
        *ppStructure = pLocation;
        return pLocation->LoadLocation(pRes, &cStruct);
    }
    if (nType == 3) {
        CScriptTalent* pTalent = new CScriptTalent;
        *ppStructure = pTalent;
        return pTalent->LoadTalent(pRes, &cStruct);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0052C510
int CSWVirtualMachineCommands::GetEqualGameDefinedStructure(int nType, void* pStructure1, void* pStructure2)
{
    if (nType == 0) {
        if (((CGameEffect*)pStructure1)->m_nID == ((CGameEffect*)pStructure2)->m_nID) {
            return 1;
        }
    } else if (nType == 1) {
        return *(CScriptEvent*)pStructure1 == *(CScriptEvent*)pStructure2;
    } else if (nType == 2) {
        return ((CScriptLocation*)pStructure1)->Equals((CScriptLocation*)pStructure2);
    } else if (nType == 3) {
        return ((CScriptTalent*)pStructure1)->Equals((CScriptTalent*)pStructure2);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0052C580
void* CSWVirtualMachineCommands::CreateGameDefinedStructure(int nType)
{
    if (nType == 0) {
        return new CGameEffect(0);
    }
    if (nType == 1) {
        return new CScriptEvent;
    }
    if (nType == 2) {
        return new CScriptLocation;
    }
    if (nType == 3) {
        return new CScriptTalent;
    }
    return NULL;
}
