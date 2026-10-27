// CSWVirtualMachineCommands: GetIs*/GetItem* queries (0x00539940-0x0053A87F).
#define B_SWS_ITEM_OPAQUE_PROPERTY_GETTERS
#include "b/vm_commands.h"
#include "b/sws_creature.h"
#include "b/sws_item.h"
#include "b/sws_door.h"
#include "b/global_variable_table.h"
#include <math.h>

extern const unsigned long OBJECT_INVALID_ID;               // 0x00746644
extern const unsigned char OBJECT_TYPE_AREA;                // 0x00746633
extern const unsigned char OBJECT_TYPE_TRIGGER;             // 0x00746636
extern const unsigned char OBJECT_TYPE_PLACEABLE;           // 0x00746638
extern const unsigned char OBJECT_TYPE_DOOR;                // 0x00746639

// Trapped flag of triggers (TU-local view).
struct CSWSTriggerTrapView {
    char pad_0x0[0x2bc];
    int m_bTrapped;                                         // 0x2bc
};

// Matched-substring and listen state of CSWSObject (TU-local view).
struct CSWSObjectListenView {
    char pad_0x0[0x198];
    int m_nListenPatternNumber;                             // 0x198
    char pad_0x19c[0x1a4 - 0x19c];
    int m_nMatchedSubstringsCount;                          // 0x1a4
    char pad_0x1a8[0x1d5 - 0x1a8];
    unsigned char m_nMetaMagicFeat;                         // 0x1d5
};

// Also GetIsPlaceableObjectActionPossible.
// Matches with the inline GetStats() accessor (round s10).
// FUNCTION: SWKOTOR 0x00539940
int CSWVirtualMachineCommands::ExecuteCommandGetIsDoorActionPossible(int nCommandId, int nParameters)
{
    int bPossible = 0;
    CSWSDoor* pDoor = NULL;
    CSWSPlaceable* pPlaceable = NULL;
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(m_oidObjectRunScript);
    unsigned long oidTarget;
    int nAction;
    if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nAction)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nCommandId == 0x151) {
        pDoor = g_pAppManager->GetServerExoApp()->GetDoorByGameObjectID(oidTarget);
    } else {
        pPlaceable = g_pAppManager->GetServerExoApp()->GetPlaceableByGameObjectID(oidTarget);
    }
    if (pCreature && ((pDoor && !pDoor->m_n3C0) || (pPlaceable && !pPlaceable->m_n398))) {
        switch (nAction) {
        case 0:
            if (nCommandId == 0x151 && pDoor) {
                if (pDoor->m_nAnimationState == 0x2726 && !pDoor->m_bLocked) {
                    bPossible = 1;
                }
            } else if (pPlaceable && pPlaceable->m_nD4 == 0x275c && !pPlaceable->m_n260) {
                bPossible = 1;
            }
            break;
        case 1: {
            CSWSCreatureStats* pStats = pCreature->GetStats();
            char nRank = pStats->GetSkillRank(6, NULL, 0);
            if (nCommandId == 0x151 && pDoor) {
                if (pDoor->m_bLocked && !pDoor->m_bKeyRequired && nRank > 0 &&
                    nRank + 20 >= pDoor->m_nOpenLockDC) {
                    bPossible = 1;
                }
            } else if (pPlaceable && pPlaceable->m_n260 && !pPlaceable->m_bKeyRequired && nRank > 0 &&
                       nRank + 20 >= pPlaceable->m_nOpenLockDC) {
                bPossible = 1;
            }
            break;
        }
        case 2:
            if ((pDoor && !pDoor->m_bPlot) || (pPlaceable && !pPlaceable->m_nF8)) {
                bPossible = 1;
            }
            break;
        case 3:
            bPossible = 1;
            break;
        case 4:
            if ((pDoor && !pDoor->m_bPlot && !pDoor->m_bKeyRequired) ||
                (pPlaceable && !pPlaceable->m_nF8 && !pPlaceable->m_bKeyRequired)) {
                unsigned char nMultiClass = 0;
                do {
                    bPossible = pCreature->GetStats()->GetSpellUsesLeft(0x5d, nMultiClass, 0, 0) > 0;
                    nMultiClass++;
                } while (!bPossible && nMultiClass <= pCreature->GetStats()->m_nNumMultiClasses);
            }
            break;
        }
    }
    if (!g_pVirtualMachine->StackPushInteger(bPossible)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00539BD0
int CSWVirtualMachineCommands::ExecuteCommandGetIsDusk(int nCommandId, int nParameters)
{
    int bResult = 0;
    CSWSModule* pModule = g_pAppManager->GetServerExoApp()->GetModule();
    if (pModule && pModule->m_nTimeOfDay == 4) {
        bResult = 1;
    }
    return g_pVirtualMachine->StackPushInteger(bResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00539E20
int CSWVirtualMachineCommands::ExecuteCommandGetIsOpen(int nCommandId, int nParameters)
{
    unsigned long oidObject = INVALID_OBJECT_ID;
    int bOpen = 0;
    if (!g_pVirtualMachine->StackPopObject(&oidObject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidObject);
    if (pObject) {
        if (pObject->AsSWSDoor()) {
            bOpen = pObject->AsSWSDoor()->m_bOpen != 0;
        } else if (pObject->AsSWSPlaceable()) {
            bOpen = pObject->AsSWSPlaceable()->m_nD4 == 0x275b;
        }
    }
    return g_pVirtualMachine->StackPushInteger(bOpen) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x00539ED0
int CSWVirtualMachineCommands::ExecuteCommandGetIsPlayableRacialType(int nCommandId, int nParameters)
{
    int bPlayable = 0;
    unsigned long oidCreature;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    int nRace;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidCreature, &pObject) == GAME_OBJECT_VALID) {
        if (pObject->m_nObjectType == 5) {
            nRace = pObject->AsSWSCreature()->m_pStats->m_nRace;
        } else {
            nRace = g_pRules->m_nAA;
        }
    }
    if (g_pRules->m_pRaces[(unsigned short)nRace].m_bPlayerRace) {
        bPlayable = 1;
    }
    return g_pVirtualMachine->StackPushInteger(bPlayable) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

class CScriptTalent {
public:
    ~CScriptTalent();                                       // 0x004015A0 (shared empty function)

    int m_nType;                                            // 0x00
};

// FUNCTION: SWKOTOR 0x00539F90
int CSWVirtualMachineCommands::ExecuteCommandGetIsTalentValid(int nCommandId, int nParameters)
{
    CScriptTalent* pTalent;
    int bValid = 1;
    if (!g_pVirtualMachine->StackPopEngineStructure(ENGINE_STRUCTURE_TALENT, (void**)&pTalent)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (pTalent->m_nType == -1) {
        bValid = 0;
    }
    delete pTalent;
    pTalent = NULL;
    return g_pVirtualMachine->StackPushInteger(bValid) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053A000
int CSWVirtualMachineCommands::ExecuteCommandGetIsTrapped(int nCommandId, int nParameters)
{
    unsigned long oidObject;
    if (!g_pVirtualMachine->StackPopObject(&oidObject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int bTrapped = 0;
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidObject);
    if (pObject) {
        if (pObject->m_nObjectType == OBJECT_TYPE_TRIGGER) {
            bTrapped = ((CSWSTriggerTrapView*)pObject->AsSWSAreaOfEffectObject())->m_bTrapped;
        } else if (pObject->m_nObjectType == OBJECT_TYPE_PLACEABLE) {
            bTrapped = pObject->AsSWSPlaceable()->m_bTrapped;
        } else if (pObject->m_nObjectType == OBJECT_TYPE_DOOR) {
            bTrapped = pObject->AsSWSDoor()->m_bTrapped;
        }
    }
    return g_pVirtualMachine->StackPushInteger(bTrapped) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053A0A0
int CSWVirtualMachineCommands::ExecuteCommandGetIsWeaponEffective(int nCommandId, int nParameters)
{
    int bEffective = 0;
    unsigned long oidVersus;
    int bOffHand;
    if (!g_pVirtualMachine->StackPopObject(&oidVersus)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&bOffHand)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(m_oidObjectRunScript);
    if (pCreature) {
        bEffective = pCreature->GetIsWeaponEffective(oidVersus, bOffHand);
    }
    if (!g_pVirtualMachine->StackPushInteger(bEffective)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0053A130
int CSWVirtualMachineCommands::ExecuteCommandGetItemACValue(int nCommandId, int nParameters)
{
    unsigned long oidItem;
    if (!g_pVirtualMachine->StackPopObject(&oidItem)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int nAC = 0;
    CSWSItem* pItem = g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(oidItem);
    if (pItem) {
        nAC = pItem->m_n244;
        for (int i = 0; i < pItem->m_nPassiveProperties; i++) {
            CSWItemProperty* pProperty = pItem->GetPassiveProperty(i);
            if (pProperty && pProperty->m_nPropertyName == 1) {
                nAC += pProperty->m_nCostValue;
            }
        }
    }
    return g_pVirtualMachine->StackPushInteger(nAC) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053A1D0
int CSWVirtualMachineCommands::ExecuteCommandGetItemHasItemProperty(int nCommandId, int nParameters)
{
    unsigned long oidItem;
    int nProperty;
    if (!g_pVirtualMachine->StackPopObject(&oidItem)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nProperty)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    int bHas = 0;
    CSWSItem* pItem = g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(oidItem);
    if (pItem) {
        int i;
        for (i = 0; i < pItem->m_nActiveProperties; i++) {
            CSWItemProperty* pProperty = pItem->GetActiveProperty(i);
            if (pProperty &&
                (pProperty->m_nUpgradeSlot == 0xff || (pItem->m_nEquippedSlots & (1 << pProperty->m_nUpgradeSlot))) &&
                pProperty->m_nPropertyName == nProperty) {
                bHas = 1;
                goto done;
            }
        }
        for (i = 0; i < pItem->m_nPassiveProperties; i++) {
            CSWItemProperty* pProperty = pItem->GetPassiveProperty(i);
            if (pProperty && pProperty->m_nPropertyName == nProperty) {
                bHas = 1;
                break;
            }
        }
    }
done:
    if (!g_pVirtualMachine->StackPushInteger(bHas)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0053A2C0
int CSWVirtualMachineCommands::ExecuteCommandGetItemInSlot(int nCommandId, int nParameters)
{
    int nSlot;
    unsigned long oidCreature;
    if (!g_pVirtualMachine->StackPopInteger(&nSlot)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    nSlot = (int)pow(2.0, nSlot);
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    unsigned long oidItem = OBJECT_INVALID_ID;
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidCreature, &pObject) == GAME_OBJECT_VALID) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        if (pCreature && pCreature->m_pInventory) {
            CSWSItem* pItem = pCreature->m_pInventory->GetItemInSlot(nSlot);
            if (pItem) {
                oidItem = pItem->m_idSelf;
            }
        }
    }
    if (!g_pVirtualMachine->StackPushObject(oidItem)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0053A5C0
int CSWVirtualMachineCommands::ExecuteCommandGetItemPossessor(int nCommandId, int nParameters)
{
    unsigned long oidItem;
    if (!g_pVirtualMachine->StackPopObject(&oidItem)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    unsigned long oidPossessor = OBJECT_INVALID_ID;
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidItem, &pObject) == GAME_OBJECT_VALID &&
        pObject->AsSWSItem()) {
        oidPossessor = pObject->AsSWSItem()->m_oidPossessor;
    }
    CGameObject* pPossessor = g_pAppManager->GetServerExoApp()->GetGameObject(oidPossessor);
    if (pPossessor && pPossessor->AsSWSItem()) {
        oidPossessor = pPossessor->AsSWSItem()->m_oidPossessor;
    }
    return g_pVirtualMachine->StackPushObject(oidPossessor) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053B840
int CSWVirtualMachineCommands::ExecuteCommandGetLocation(int nCommandId, int nParameters)
{
    unsigned long oidObject;
    CScriptLocation cLocation;
    if (!g_pVirtualMachine->StackPopObject(&oidObject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidObject, &pObject) == GAME_OBJECT_VALID &&
        pObject && pObject->m_nObjectType > OBJECT_TYPE_AREA) {
        cLocation = ((CSWSObject*)pObject)->GetScriptLocation();
    }
    if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_LOCATION, &cLocation)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0053B960
int CSWVirtualMachineCommands::ExecuteCommandGetLocked(int nCommandId, int nParameters)
{
    int bLocked = 0;
    unsigned long oidObject;
    if (!g_pVirtualMachine->StackPopObject(&oidObject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidObject, &pObject) == GAME_OBJECT_VALID) {
        if (pObject->AsSWSDoor()) {
            bLocked = pObject->AsSWSDoor()->m_bLocked;
        } else if (pObject->AsSWSPlaceable()) {
            bLocked = pObject->AsSWSPlaceable()->m_n260;
        }
    }
    return g_pVirtualMachine->StackPushInteger(bLocked) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// Also GetLockKeyTag, GetLockLockable, GetLockUnlockDC and GetLockLockDC.
// FUNCTION: SWKOTOR 0x0053BA10
int CSWVirtualMachineCommands::ExecuteCommandGetLockKeyRequired(int nCommandId, int nParameters)
{
    unsigned long oidObject;
    if (!g_pVirtualMachine->StackPopObject(&oidObject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidObject);
    if (nCommandId == 0x219) {
        int bKeyRequired = 0;
        if (pObject) {
            if (pObject->m_nObjectType == OBJECT_TYPE_DOOR) {
                bKeyRequired = pObject->AsSWSDoor()->m_bKeyRequired;
            } else if (pObject->m_nObjectType == OBJECT_TYPE_PLACEABLE) {
                bKeyRequired = pObject->AsSWSPlaceable()->m_bKeyRequired;
            }
        }
        if (!g_pVirtualMachine->StackPushInteger(bKeyRequired)) {
            return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
        }
    } else if (nCommandId == 0x21a) {
        CExoString* pKeyName = NULL;
        if (pObject) {
            if (pObject->m_nObjectType == OBJECT_TYPE_DOOR) {
                pKeyName = &pObject->AsSWSDoor()->m_sKeyName;
            } else if (pObject->m_nObjectType == OBJECT_TYPE_PLACEABLE) {
                pKeyName = &pObject->AsSWSPlaceable()->m_sKeyName;
            }
        }
        if (!pKeyName) {
            if (!g_pVirtualMachine->StackPushString("")) {
                return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
            }
        } else {
            if (!g_pVirtualMachine->StackPushString(*pKeyName)) {
                return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
            }
        }
    } else if (nCommandId == 0x21b) {
        int bLockable = 0;
        if (pObject) {
            if (pObject->m_nObjectType == OBJECT_TYPE_DOOR) {
                bLockable = pObject->AsSWSDoor()->m_bLockable;
            } else if (pObject->m_nObjectType == OBJECT_TYPE_PLACEABLE) {
                bLockable = pObject->AsSWSPlaceable()->m_n330;
            }
        }
        if (!g_pVirtualMachine->StackPushInteger(bLockable)) {
            return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
        }
    } else if (nCommandId == 0x21c) {
        int nDC = 0;
        if (pObject) {
            if (pObject->m_nObjectType == OBJECT_TYPE_DOOR) {
                nDC = pObject->AsSWSDoor()->m_nOpenLockDC;
            } else if (pObject->m_nObjectType == OBJECT_TYPE_PLACEABLE) {
                nDC = pObject->AsSWSPlaceable()->m_nOpenLockDC;
            }
        }
        if (!g_pVirtualMachine->StackPushInteger(nDC)) {
            return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
        }
    } else if (nCommandId == 0x21d) {
        int nDC = 0;
        if (pObject) {
            if (pObject->m_nObjectType == OBJECT_TYPE_DOOR) {
                nDC = pObject->AsSWSDoor()->m_nCloseLockDC;
            } else if (pObject->m_nObjectType == OBJECT_TYPE_PLACEABLE) {
                nDC = pObject->AsSWSPlaceable()->m_nCloseLockDC;
            }
        }
        if (!g_pVirtualMachine->StackPushInteger(nDC)) {
            return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0053BCC0
int CSWVirtualMachineCommands::ExecuteCommandGetMatchedSubstringsCount(int nCommandId, int nParameters)
{
    int nCount = 0;
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID &&
        pObject) {
        nCount = ((CSWSObjectListenView*)pObject)->m_nMatchedSubstringsCount;
    }
    return g_pVirtualMachine->StackPushInteger(nCount) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053BD20
int CSWVirtualMachineCommands::ExecuteCommandGetListenPatternNumber(int nCommandId, int nParameters)
{
    int nNumber = 0;
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID &&
        pObject) {
        nNumber = ((CSWSObjectListenView*)pObject)->m_nListenPatternNumber;
    }
    return g_pVirtualMachine->StackPushInteger(nNumber) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053BD80
int CSWVirtualMachineCommands::ExecuteCommandGetMetaMagicFeat(int nCommandId, int nParameters)
{
    unsigned char nFeat = 0xff;
    CSWSObject* pSWSObject;
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(m_oidObjectRunScript);
    if (pObject && pObject->AsSWSObject()) {
        pSWSObject = pObject->AsSWSObject();
    }
    if (pSWSObject) {
        nFeat = ((CSWSObjectListenView*)pSWSObject)->m_nMetaMagicFeat;
    }
    return g_pVirtualMachine->StackPushInteger(nFeat) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053BDF0
int CSWVirtualMachineCommands::ExecuteCommandGetModule(int nCommandId, int nParameters)
{
    unsigned long oidModule = INVALID_OBJECT_ID;
    if (g_pAppManager && g_pAppManager->GetServerExoApp()) {
        CSWSModule* pModule = g_pAppManager->GetServerExoApp()->GetModule();
        if (pModule) {
            oidModule = pModule->GetId();
        }
    }
    return g_pVirtualMachine->StackPushObject(oidModule) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// Also GetModuleItemAcquiredFrom, GetModuleItemLost and GetModuleItemLostBy.
// FUNCTION: SWKOTOR 0x0053BE30
int CSWVirtualMachineCommands::ExecuteCommandGetModuleItemAcquired(int nCommandId, int nParameters)
{
    unsigned long oidItem = INVALID_OBJECT_ID;
    CSWSModule* pModule = g_pAppManager->GetServerExoApp()->GetModule();
    if (pModule) {
        switch (nCommandId) {
        case 0x11a:
            oidItem = pModule->m_oidItemAcquired;
            break;
        case 0x11b:
            oidItem = pModule->m_oidItemAcquiredFrom;
            break;
        case 0x124:
            oidItem = pModule->m_oidItemLost;
            break;
        case 0x125:
            oidItem = pModule->m_oidItemLostBy;
            break;
        }
    }
    return g_pVirtualMachine->StackPushObject(oidItem) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// FUNCTION: SWKOTOR 0x0053A680
int CSWVirtualMachineCommands::ExecuteCommandGetJournalQuestExperience(int nCommandId, int nParameters)
{
    CExoString sPlotID;
    CResStruct cTopStruct;
    CResStruct cElement;
    CResList cList;
    int nXP = 0;
    int bSuccess = 1;
    if (!g_pVirtualMachine->StackPopString(&sPlotID)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CResGFF* pGFF = new CResGFF(0x808, "JRL ", "Global");
    if (pGFF->m_bLoaded) {
        pGFF->GetTopLevelStruct(&cTopStruct);
        pGFF->GetList(&cList, &cTopStruct, "Categories");
        int nCount = pGFF->GetListCount(&cList);
        for (int i = 0; i < nCount; i++) {
            pGFF->GetListElement(&cElement, &cList, i);
            if (pGFF->ReadFieldCExoString(&cElement, "Tag", bSuccess, "") == sPlotID) {
                nXP = pGFF->ReadFieldDWORD(&cElement, "XP", bSuccess, 0);
                break;
            }
        }
        delete pGFF;
    }
    if (!g_pVirtualMachine->StackPushInteger(nXP)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}

// View of module entry location and the module's script-event values.
struct ModuleEntryLocationView {
    char pad_0x0[0x10];
    float m_fLocation[5];
};
struct ModuleScriptEventsView {
    char pad_0x0[0x58];
    ModuleEntryLocationView* m_pEntry;
    char pad_0x5c[0x16c - 0x5c];
    unsigned long m_oid16C;
    unsigned long m_oid170;
    unsigned long m_oid174;
    char pad_0x178[0x19c - 0x178];
    unsigned long m_oid19C;
};

// NONMATCH (32): branch layout and location copy register assignment.
// FUNCTION: SWKOTOR 0x0053F800
int CSWVirtualMachineCommands::ExecuteCommandGetLastItemEquipped(int nCommandId, int nParameters)
{
    CScriptLocation cLocation;
    unsigned long oidResult = 0x7f000000;
    CSWSModule* pModule = g_pAppManager->GetServerExoApp()->GetModule();
    int bPushObject = 0;
    int bPushLocation = 0;
    if (pModule) {
        ModuleScriptEventsView* pEvents = (ModuleScriptEventsView*)pModule;
        if (nCommandId == 0x123) { oidResult = pEvents->m_oid16C; bPushObject = 1; }
        else if (nCommandId == 0x19a) { oidResult = pEvents->m_oid170; bPushObject = 1; }
        else if (nCommandId == 0x19b) {
            cLocation.m_vPosition[0] = pEvents->m_pEntry->m_fLocation[0];
            cLocation.m_vPosition[1] = pEvents->m_pEntry->m_fLocation[1];
            cLocation.m_vPosition[2] = pEvents->m_pEntry->m_fLocation[2];
            cLocation.m_vOrientation[0] = pEvents->m_pEntry->m_fLocation[3];
            cLocation.m_vOrientation[1] = pEvents->m_pEntry->m_fLocation[4];
            bPushLocation = 1;
        } else if (nCommandId == 0x1a3) { oidResult = pEvents->m_oid174; bPushObject = 1; }
        else if (nCommandId == 0x34) { oidResult = pEvents->m_oid19C; bPushObject = 1; }
    } else if (nCommandId == 0x19b) { bPushLocation = 1; }
    else if (nCommandId == 0x123 || nCommandId == 0x19a || nCommandId == 0x1a3 || nCommandId == 0x34) { bPushObject = 1; }
    if (bPushObject) {
        if (!g_pVirtualMachine->StackPushObject(oidResult)) return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    } else if (bPushLocation) {
        if (!g_pVirtualMachine->StackPushEngineStructure(ENGINE_STRUCTURE_LOCATION, &cLocation)) return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}
