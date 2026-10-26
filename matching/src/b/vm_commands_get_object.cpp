// CSWVirtualMachineCommands: object, party and PC queries
// (0x0053C390-0x0053CC2F).
#include "b/vm_commands.h"
#include "b/sws_creature.h"
#include "b/sws_item.h"
#include "b/sws_player.h"

extern const unsigned long OBJECT_INVALID_ID;               // 0x00746644
extern const unsigned char OBJECT_TYPE_AREA;                // 0x00746633

class CSWParty {
public:
    CSWCCreature* GetCharacter(int nIndex);                 // 0x006346C0

    int m_nCount;                                           // 0x00
};

// Client creature id (CSWCCreature keeps its object id at +0x04).
struct CSWCCreatureIdView {
    char pad_0x0[0x4];
    unsigned long m_idSelf;                                 // 0x04
};

struct CSWSObjectPlotView {
    char pad_0x0[0xf8];
    int m_bPlotObject;                                      // 0xf8
};

// PC levelling-up object of the module.
struct CSWSModuleLevelUpView {
    char pad_0x0[0x194];
    unsigned long m_oidPCLevellingUp;                       // 0x194
};

// Also GetNextObjectInArea.
// FUNCTION: SWKOTOR 0x0053C390
int CSWVirtualMachineCommands::ExecuteCommandGetFirstObjectInArea(int nCommandId, int nParameters)
{
    CSWSArea* pArea = NULL;
    CExoString sUnused;
    CGameObjectArray* pObjectArray = g_pAppManager->GetServerExoApp()->GetObjectArray();
    unsigned long oidResult = OBJECT_INVALID_ID;
    unsigned long oidArea;
    int nObjectFilter;
    CGameObject* pObject;
    if (nParameters >= 1) {
        if (!g_pVirtualMachine->StackPopObject(&oidArea)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
        if (pObjectArray->GetGameObject(oidArea, &pObject) == GAME_OBJECT_VALID && pObject) {
            pArea = pObject->AsSWSArea();
        }
    }
    if (nParameters >= 2) {
        if (!g_pVirtualMachine->StackPopInteger(&nObjectFilter)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
    } else {
        nObjectFilter = 0x7fff;
    }
    if (!pArea) {
        if (pObjectArray->GetGameObject(m_oidObjectRunScript, &pObject) != GAME_OBJECT_VALID || !pObject) {
            goto done;
        }
        if (pObject->m_nObjectType > OBJECT_TYPE_AREA) {
            pArea = ((CSWSObject*)pObject)->GetArea();
        } else if (pObject->m_nObjectType == OBJECT_TYPE_AREA) {
            pArea = pObject->AsSWSArea();
        } else {
            goto done;
        }
        if (!pArea) {
            goto done;
        }
    }
    if (nCommandId == 0x5d) {
        if (!pArea->GetFirstObjectIdInArea(&oidResult)) {
            oidResult = OBJECT_INVALID_ID;
        }
    } else {
        if (!pArea->GetNextObjectIdInArea(&oidResult)) {
            oidResult = OBJECT_INVALID_ID;
        }
    }
    {
    CGameObject* pFound;
    while (pObjectArray->GetGameObject(oidResult, &pFound) == GAME_OBJECT_VALID) {
        int nType = pFound->m_nObjectType;
        if (!nType) {
            break;
        }
        if ((nObjectFilter & 0x1) && nType == 5) break;
        if ((nObjectFilter & 0x2) && nType == 6) break;
        if ((nObjectFilter & 0x4) && nType == 7) break;
        if ((nObjectFilter & 0x8) && nType == 10) break;
        if ((nObjectFilter & 0x10) && nType == 11) break;
        if ((nObjectFilter & 0x20) && nType == 12) break;
        if ((nObjectFilter & 0x40) && nType == 9) break;
        if ((nObjectFilter & 0x80) && nType == 14) break;
        if ((nObjectFilter & 0x100) && nType == 13) break;
        if ((nObjectFilter & 0x200) && nType == 16) break;
        if (!pArea->GetNextObjectIdInArea(&oidResult)) {
            oidResult = OBJECT_INVALID_ID;
        }
    }
    }
done:
    if (!g_pVirtualMachine->StackPushObject(oidResult)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0053C610
int CSWVirtualMachineCommands::ExecuteCommandGetObjectType(int nCommandId, int nParameters)
{
    int nType = 0;
    unsigned long oidObject;
    if (!g_pVirtualMachine->StackPopObject(&oidObject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidObject, &pObject) == GAME_OBJECT_VALID &&
        pObject->m_nObjectType > OBJECT_TYPE_AREA) {
        switch (pObject->m_nObjectType) {
        case 5: nType = 1; break;
        case 6: nType = 2; break;
        case 7: nType = 4; break;
        case 10: nType = 8; break;
        case 11: nType = 0x10; break;
        case 12: nType = 0x20; break;
        case 9: nType = 0x40; break;
        case 14: nType = 0x80; break;
        case 13: nType = 0x100; break;
        case 16: nType = 0x200; break;
        default: nType = 0x7fff; break;
        }
    }
    return g_pVirtualMachine->StackPushInteger(nType) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053C720
int CSWVirtualMachineCommands::ExecuteCommandGetIsObjectValid(int nCommandId, int nParameters)
{
    int bValid = 0;
    unsigned long oidObject;
    if (!g_pVirtualMachine->StackPopObject(&oidObject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (g_pAppManager->GetServerExoApp()->GetGameObject(oidObject)) {
        bValid = 1;
    }
    return g_pVirtualMachine->StackPushInteger(bValid) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// Also GetObjectHeard.
// FUNCTION: SWKOTOR 0x0053C780
int CSWVirtualMachineCommands::ExecuteCommandGetObjectSeen(int nCommandId, int nParameters)
{
    int bResult = 0;
    unsigned long oidTarget;
    unsigned long oidSource;
    if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopObject(&oidSource)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidSource, &pObject) == GAME_OBJECT_VALID &&
        pObject->AsSWSCreature()) {
        if (nCommandId == 0x121) {
            CSWSCreatureVisibility* pEntry = pObject->AsSWSCreature()->GetFromList920(oidTarget);
            if (pEntry) {
                bResult = pEntry->m_bSeen;
            }
        } else if (nCommandId == 0x122) {
            CSWSCreatureVisibility* pEntry = pObject->AsSWSCreature()->GetFromList920(oidTarget);
            if (pEntry) {
                bResult = pEntry->m_bHeard;
            }
        }
    }
    if (!g_pVirtualMachine->StackPushInteger(bResult)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0053C870
int CSWVirtualMachineCommands::ExecuteCommandGetPartyMemberByIndex(int nCommandId, int nParameters)
{
    int nIndex;
    if (!g_pVirtualMachine->StackPopInteger(&nIndex)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    unsigned long oidMember = INVALID_OBJECT_ID;
    CSWCCreature* pCreature = g_pAppManager->GetClientExoApp()->GetSWParty()->GetCharacter(nIndex);
    if (pCreature) {
        unsigned long oidClient = ((CSWCCreatureIdView*)pCreature)->m_idSelf;
        oidMember = g_pAppManager->GetServerExoApp()->ClientToServerObjectID(oidClient);
    }
    return g_pVirtualMachine->StackPushObject(oidMember) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053C8F0
int CSWVirtualMachineCommands::ExecuteCommandGetPartyMemberCount(int nCommandId, int nParameters)
{
    int nCount = g_pAppManager->GetClientExoApp()->GetSWParty()->m_nCount;
    return g_pVirtualMachine->StackPushInteger(nCount) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// Also GetNextPC.
// FUNCTION: SWKOTOR 0x0053C920
int CSWVirtualMachineCommands::ExecuteCommandGetFirstPC(int nCommandId, int nParameters)
{
    unsigned long oidPC = OBJECT_INVALID_ID;
    if (nCommandId == 0x224) {
        oidPC = g_pAppManager->GetServerExoApp()->GetFirstPCObject();
    } else if (nCommandId == 0x225) {
        oidPC = g_pAppManager->GetServerExoApp()->GetNextPCObject();
    }
    return g_pVirtualMachine->StackPushObject(oidPC) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053C980
int CSWVirtualMachineCommands::ExecuteCommandGetPCLevellingUp(int nCommandId, int nParameters)
{
    unsigned long oidPC = INVALID_OBJECT_ID;
    CSWSModule* pModule = g_pAppManager->GetServerExoApp()->GetModule();
    if (pModule) {
        oidPC = ((CSWSModuleLevelUpView*)pModule)->m_oidPCLevellingUp;
    }
    return g_pVirtualMachine->StackPushObject(oidPC) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053C9C0
int CSWVirtualMachineCommands::ExecuteCommandGetPCSpeaker(int nCommandId, int nParameters)
{
    unsigned long oidPlayer = g_pAppManager->GetServerExoApp()->GetPlayerCreatureId();
    CSWSPlayer* pPlayer = (CSWSPlayer*)g_pAppManager->GetServerExoApp()->GetClientObjectByObjectId(oidPlayer);
    unsigned long oidSpeaker = pPlayer->m_oid38;
    return g_pVirtualMachine->StackPushObject(oidSpeaker) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053CA00
int CSWVirtualMachineCommands::ExecuteCommandGetPlaceableIllumination(int nCommandId, int nParameters)
{
    unsigned long oidPlaceable;
    if (!g_pVirtualMachine->StackPopObject(&oidPlaceable)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int bIlluminated = 0;
    CSWSPlaceable* pPlaceable = g_pAppManager->GetServerExoApp()->GetPlaceableByGameObjectID(oidPlaceable);
    if (pPlaceable) {
        bIlluminated = pPlaceable->GetLightIsOn();
    }
    return g_pVirtualMachine->StackPushInteger(bIlluminated) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053CA60
int CSWVirtualMachineCommands::ExecuteCommandGetPlotFlag(int nCommandId, int nParameters)
{
    unsigned long oidTarget;
    int bPlot = 0;
    if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidTarget);
    if (pObject && pObject->AsSWSObject() && ((CSWSObjectPlotView*)pObject->AsSWSObject())->m_bPlotObject) {
        bPlot = 1;
    }
    return g_pVirtualMachine->StackPushInteger(bPlot) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053CAE0
int CSWVirtualMachineCommands::ExecuteCommandGetPosition(int nCommandId, int nParameters)
{
    unsigned long oidTarget;
    Vector vPosition(0.0f, 0.0f, 0.0f);
    if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidTarget, &pObject) == GAME_OBJECT_VALID &&
        pObject && pObject->m_nObjectType > OBJECT_TYPE_AREA) {
        vPosition = pObject->m_vPosition;
    }
    return g_pVirtualMachine->StackPushVector(vPosition) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053CBA0
int CSWVirtualMachineCommands::ExecuteCommandGetRacialType(int nCommandId, int nParameters)
{
    int nRace = 0;
    unsigned long oidCreature;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidCreature, &pObject) == GAME_OBJECT_VALID) {
        if (pObject->m_nObjectType == 5) {
            nRace = pObject->AsSWSCreature()->m_pStats->m_nRace;
        } else {
            nRace = 0;
        }
    }
    return g_pVirtualMachine->StackPushInteger(nRace) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

extern int g_nScriptVar;                                    // 0x00832828 (.data)

// Last-spell state shared by creatures, placeables and doors (CSWSObject).
struct CSWSObjectSpellView {
    char pad_0x0[0x1bc];
    unsigned long m_oidSpellTarget;                         // 0x1bc
    int m_nSpellId;                                         // 0x1c0
    char pad_0x1c4[0x1d8 - 0x1c4];
    unsigned long m_oidLastSpellCaster;                     // 0x1d8
    int m_nLastSpellId;                                     // 0x1dc
    int m_bLastSpellHarmful;                                // 0x1e0
};

struct CSWSCreatureSpellItemView {
    char pad_0x0[0x95c];
    unsigned long m_oidSpellCastItem;                       // 0x95c
};

struct CSWSAreaRestrictView {
    char pad_0x0[0x2b0];
    int m_nPlayerRestrictMode;                              // 0x2b0
};

struct CSWSCreatureStatsGenderView {
    char pad_0x0[0x64];
    unsigned char m_nGender;                                // 0x64
};

// FUNCTION: SWKOTOR 0x0053CED0
int CSWVirtualMachineCommands::ExecuteCommandGetReputation(int nCommandId, int nParameters)
{
    int nReputation = -1;
    unsigned long oidSource;
    unsigned long oidTarget;
    if (!g_pVirtualMachine->StackPopObject(&oidSource)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidTarget, &pObject) == GAME_OBJECT_VALID) {
        nReputation = pObject->AsSWSObject()->GetReputation(oidSource, 0);
    }
    if (!g_pVirtualMachine->StackPushInteger(nReputation)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0053CF70
int CSWVirtualMachineCommands::ExecuteCommandGetPlayerRestrictMode(int nCommandId, int nParameters)
{
    int nMode = 0;
    unsigned long oidCreature = INVALID_OBJECT_ID;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    if (pCreature && pCreature->m_bInParty && g_pAppManager->GetServerExoApp()->GetModule() &&
        g_pAppManager->GetServerExoApp()->GetModule()->GetArea()) {
        nMode = ((CSWSAreaRestrictView*)g_pAppManager->GetServerExoApp()->GetModule()->GetArea())->m_nPlayerRestrictMode;
    }
    return g_pVirtualMachine->StackPushInteger(nMode) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053D020
int CSWVirtualMachineCommands::ExecuteCommandGetRunScriptVar(int nCommandId, int nParameters)
{
    return g_pVirtualMachine->StackPushInteger(g_nScriptVar) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053D050
int CSWVirtualMachineCommands::ExecuteCommandGetSkillRank(int nCommandId, int nParameters)
{
    int nSkill;
    unsigned long oidTarget;
    if (!g_pVirtualMachine->StackPopInteger(&nSkill)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int nRank = -1;
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidTarget, &pObject) == GAME_OBJECT_VALID &&
        pObject->AsSWSCreature()) {
        CSWSCreatureStats* pStats = pObject->AsSWSCreature()->m_pStats;
        nRank = pStats->GetSkillRank(nSkill, NULL, 0);
    }
    if (!g_pVirtualMachine->StackPushInteger(nRank)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// Also GetLastSpell and GetLastSpellHarmful.
// FUNCTION: SWKOTOR 0x0053D110
int CSWVirtualMachineCommands::ExecuteCommandGetLastSpellCaster(int nCommandId, int nParameters)
{
    CGameObject* pObject;
    int bValid = g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) ==
                 GAME_OBJECT_VALID;
    if (nCommandId == 0xf5) {
        unsigned long oidCaster = INVALID_OBJECT_ID;
        if (bValid) {
            switch (pObject->m_nObjectType) {
            case 5:
                oidCaster = ((CSWSObjectSpellView*)pObject->AsSWSCreature())->m_oidLastSpellCaster;
                break;
            case 9:
                oidCaster = ((CSWSObjectSpellView*)pObject->AsSWSPlaceable())->m_oidLastSpellCaster;
                break;
            case 10:
                oidCaster = ((CSWSObjectSpellView*)pObject->AsSWSDoor())->m_oidLastSpellCaster;
                break;
            }
        }
        if (!g_pVirtualMachine->StackPushObject(oidCaster)) {
            return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
        }
    } else if (nCommandId == 0xf6) {
        int nSpellId = -1;
        if (bValid) {
            switch (pObject->m_nObjectType) {
            case 5:
                nSpellId = ((CSWSObjectSpellView*)pObject->AsSWSCreature())->m_nLastSpellId;
                break;
            case 9:
                nSpellId = ((CSWSObjectSpellView*)pObject->AsSWSPlaceable())->m_nLastSpellId;
                break;
            case 10:
                nSpellId = ((CSWSObjectSpellView*)pObject->AsSWSDoor())->m_nLastSpellId;
                break;
            }
        }
        if (!g_pVirtualMachine->StackPushInteger(nSpellId)) {
            return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
        }
    } else if (nCommandId == 0x1a7) {
        int bHarmful = 0;
        if (bValid) {
            switch (pObject->m_nObjectType) {
            case 5:
                bHarmful = ((CSWSObjectSpellView*)pObject->AsSWSCreature())->m_bLastSpellHarmful;
                break;
            case 9:
                bHarmful = ((CSWSObjectSpellView*)pObject->AsSWSPlaceable())->m_bLastSpellHarmful;
                break;
            case 10:
                bHarmful = ((CSWSObjectSpellView*)pObject->AsSWSDoor())->m_bLastSpellHarmful;
                break;
            }
        }
        if (!g_pVirtualMachine->StackPushInteger(bHarmful)) {
            return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0053D250
int CSWVirtualMachineCommands::ExecuteCommandGetSpellCastItem(int nCommandId, int nParameters)
{
    unsigned long oidItem = INVALID_OBJECT_ID;
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(m_oidObjectRunScript);
    if (pObject && pObject->AsSWSCreature()) {
        oidItem = ((CSWSCreatureSpellItemView*)pObject->AsSWSCreature())->m_oidSpellCastItem;
    }
    return g_pVirtualMachine->StackPushObject(oidItem) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053D2B0
int CSWVirtualMachineCommands::ExecuteCommandGetSpellId(int nCommandId, int nParameters)
{
    int nSpellId = -1;
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID &&
        pObject->AsSWSCreature()) {
        nSpellId = ((CSWSObjectSpellView*)pObject->AsSWSCreature())->m_nSpellId;
    }
    return g_pVirtualMachine->StackPushInteger(nSpellId) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053D510
int CSWVirtualMachineCommands::ExecuteCommandGetSpellTargetObject(int nCommandId, int nParameters)
{
    unsigned long oidTarget = OBJECT_INVALID_ID;
    CGameObject* pObject;
    if (m_bValidObjectRunScript == 1 &&
        g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID) {
        CSWSObject* pSWSObject = pObject->AsSWSObject();
        if (pSWSObject && (pSWSObject->AsSWSCreature() || pSWSObject->AsSWSPlaceable())) {
            oidTarget = ((CSWSObjectSpellView*)pSWSObject)->m_oidSpellTarget;
        }
    }
    return g_pVirtualMachine->StackPushObject(oidTarget) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053D5A0
int CSWVirtualMachineCommands::ExecuteCommandGetGender(int nCommandId, int nParameters)
{
    unsigned long oidCreature;
    int nGender = -1;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    if (pCreature && nCommandId == 0x166) {
        nGender = ((CSWSCreatureStatsGenderView*)pCreature->m_pStats)->m_nGender;
    }
    return g_pVirtualMachine->StackPushInteger(nGender) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// Slot 0x50 object (bucket b calls the slot AsSWSTrigger; buckets a and i
// call it AsSWSAreaOfEffectObject): creator and save DC.
class CSWSTrigger : public CSWSObject {
public:
    char pad_0xf4[0x248 - 0xf4];
    unsigned long m_oidCreator;                             // 0x248
    char pad_0x24c[0x258 - 0x24c];
    int m_nSpellSaveDC;                                     // 0x258
};

// FUNCTION: SWKOTOR 0x0053D320
int CSWVirtualMachineCommands::ExecuteCommandGetSpellSaveDC(int nCommandId, int nParameters)
{
    int nDC = 14;
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID) {
        CSWSCreature* pCreature;
        int nSpellId;
        if (pObject->AsSWSCreature()) {
            pCreature = pObject->AsSWSCreature();
            if (!pCreature) {
                goto done;
            }
            nSpellId = ((CSWSObjectSpellView*)pCreature)->m_nSpellId;
        } else if (pObject->AsSWSTrigger()) {
            unsigned long oidCreator = pObject->AsSWSTrigger()->m_oidCreator;
            pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreator);
            if (!pCreature) {
                nDC = pObject->AsSWSTrigger()->m_nSpellSaveDC;
            }
            nSpellId = pObject->AsSWSTrigger()->GetLastSpellId();
        } else {
            goto done;
        }
        if (pCreature) {
            nDC = pCreature->CalculateSpellSaveDC(nSpellId);
        }
    }
done:
    return g_pVirtualMachine->StackPushInteger(nDC) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

struct STR_RES {
    CExoString sString;                                     // 0x00
    CResRef cSoundResRef;                                   // 0x08
    float fSoundLength;                                     // 0x18
};

class CTlkTable {
public:
    int Fetch(unsigned long nStrRef, STR_RES& strRes, unsigned char nGender); // 0x0041E550
};

extern CTlkTable* g_pTlkTable;                              // 0x007A3A08
extern const unsigned char GENDER_MALE;                     // 0x0073ECD0

// FUNCTION: SWKOTOR 0x0053D610
int CSWVirtualMachineCommands::ExecuteCommandGetStringByStrRef(int nCommandId, int nParameters)
{
    int nStrRef;
    if (!g_pVirtualMachine->StackPopInteger(&nStrRef)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    STR_RES strRes;
    g_pTlkTable->Fetch(nStrRef, strRes, GENDER_MALE);
    CExoString sString(strRes.sString);
    if (!g_pVirtualMachine->StackPushString(sString)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}
