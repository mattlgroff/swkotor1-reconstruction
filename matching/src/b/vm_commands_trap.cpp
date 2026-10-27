// CSWVirtualMachineCommands::ExecuteCommandGetTrapDisarmable (round s10):
// one handler for GetTrapDisarmable, GetTrapDetectable, GetTrapDetectedBy,
// GetTrapFlagged, GetTrapBaseType, GetTrapOneShot, GetTrapCreator,
// GetTrapKeyTag, GetTrapDisarmDC, GetTrapDetectDC, SetTrapDetectedBy and
// SetTrapDisabled. Triggers (vtable slot 0x40), placeables and doors keep
// their trap data at different offsets (names guessed).
#include "b/vm_commands.h"
#include "b/sws_creature.h"
#include "b/sws_item.h"
#include "b/exo_array_list.h"

extern const unsigned char OBJECT_TYPE_TRIGGER;             // 0x00746636 (7)
extern const unsigned char OBJECT_TYPE_PLACEABLE;           // 0x00746638 (9)
extern const unsigned char OBJECT_TYPE_DOOR;                // 0x00746639 (10)
extern const unsigned long EVENT_SIGNAL_EVENT;              // 0x007441DC (name guessed)

struct CSWSTriggerTrapView {
    int GetTrapActive() { return m_bTrapActive; }
    int GetDetectedBy(unsigned long oid) { return m_aDetectedBy.count(oid); }
    void SetDetectedBy(unsigned long oid) { m_aDetectedBy.AddUnique(oid); }

    char pad_0x0[0x27c];
    CExoString m_sKeyTag;                                   // 0x27c
    char pad_0x284[0x2a8 - 0x284];
    CExoArrayList<unsigned long> m_aDetectedBy;             // 0x2a8
    char pad_0x2b4[0x2bc - 0x2b4];
    int m_bTrapActive;                                      // 0x2bc
    int m_bDetectable;                                      // 0x2c0
    int m_bDisarmable;                                      // 0x2c4
    int m_bFlagged;                                         // 0x2c8
    char pad_0x2cc[0x2d0 - 0x2cc];
    int m_bOneShot;                                         // 0x2d0
    unsigned char m_nTrapType;                              // 0x2d4
    char pad_0x2d5[0x2d8 - 0x2d5];
    int m_nDetectDC;                                        // 0x2d8
    int m_nDisarmDC;                                        // 0x2dc
    char pad_0x2e0[0x2e4 - 0x2e0];
    unsigned long m_oidCreator;                             // 0x2e4
};

struct CSWSPlaceableTrapView {
    int GetDetectedBy(unsigned long oid) { return m_aDetectedBy.count(oid); }
    void SetDetectedBy(unsigned long oid) { m_aDetectedBy.AddUnique(oid); }

    char pad_0x0[0x264];
    CExoString m_sKeyTag;                                   // 0x264
    char pad_0x26c[0x276 - 0x26c];
    unsigned char m_nDetectDC;                              // 0x276
    char pad_0x277[0x278 - 0x277];
    int m_bTrapped;                                         // 0x278
    unsigned char m_nDisarmDC;                              // 0x27c
    char pad_0x27d[0x280 - 0x27d];
    int m_bDisarmable;                                      // 0x280
    int m_bDetectable;                                      // 0x284
    int m_bOneShot;                                         // 0x288
    unsigned char m_bFlagged;                               // 0x28c
    char pad_0x28d[0x290 - 0x28d];
    unsigned char m_nTrapType;                              // 0x290
    char pad_0x291[0x318 - 0x291];
    CExoArrayList<unsigned long> m_aDetectedBy;             // 0x318
};

struct CSWSDoorTrapView {
    int GetDetectedBy(unsigned long oid) { return m_aDetectedBy.count(oid); }
    void SetDetectedBy(unsigned long oid) { m_aDetectedBy.AddUnique(oid); }

    char pad_0x0[0x2d0];
    CExoString m_sKeyTag;                                   // 0x2d0
    char pad_0x2d8[0x2dc - 0x2d8];
    CExoArrayList<unsigned long> m_aDetectedBy;             // 0x2dc
    int m_bTrapped;                                         // 0x2e8
    unsigned char m_nDisarmDC;                              // 0x2ec
    unsigned char m_nDetectDC;                              // 0x2ed
    char pad_0x2ee[0x2f4 - 0x2ee];
    unsigned char m_bFlagged;                               // 0x2f4
    char pad_0x2f5[0x2f8 - 0x2f5];
    unsigned char m_nTrapType;                              // 0x2f8
    char pad_0x2f9[0x2fc - 0x2f9];
    int m_bDisarmable;                                      // 0x2fc
    int m_bDetectable;                                      // 0x300
    int m_bOneShot;                                         // 0x304
};

#define TRIGGER_TRAP(p) ((CSWSTriggerTrapView*)(p)->AsSWSAreaOfEffectObject())
#define PLACEABLE_TRAP(p) ((CSWSPlaceableTrapView*)(p)->AsSWSPlaceable())
#define DOOR_TRAP(p) ((CSWSDoorTrapView*)(p)->AsSWSDoor())

// Matches with local view pointers for the detected-by lists, the scoped
// CExoString("") fallback with a bool result, and GetId() for the event ids.
// FUNCTION: SWKOTOR 0x0054C0E0
int CSWVirtualMachineCommands::ExecuteCommandGetTrapDisarmable(int nCommandId, int nParameters)
{
    unsigned long oidCreature;
    unsigned long oidTrap;
    if (!g_pVirtualMachine->StackPopObject(&oidTrap)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidTrap);
    if (nCommandId == 0x20f) {
        int bValue = 0;
        if (pObject) {
            if (pObject->m_nObjectType == OBJECT_TYPE_TRIGGER) {
                bValue = TRIGGER_TRAP(pObject)->m_bDisarmable;
            } else if (pObject->m_nObjectType == OBJECT_TYPE_PLACEABLE) {
                bValue = PLACEABLE_TRAP(pObject)->m_bDisarmable;
            } else if (pObject->m_nObjectType == OBJECT_TYPE_DOOR) {
                bValue = DOOR_TRAP(pObject)->m_bDisarmable;
            }
        }
        if (!g_pVirtualMachine->StackPushInteger(bValue)) {
            return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
        }
    } else if (nCommandId == 0x210) {
        int bValue = 0;
        if (pObject) {
            if (pObject->m_nObjectType == OBJECT_TYPE_TRIGGER) {
                bValue = TRIGGER_TRAP(pObject)->m_bDetectable;
            } else if (pObject->m_nObjectType == OBJECT_TYPE_PLACEABLE) {
                bValue = PLACEABLE_TRAP(pObject)->m_bDetectable;
            } else if (pObject->m_nObjectType == OBJECT_TYPE_DOOR) {
                bValue = DOOR_TRAP(pObject)->m_bDetectable;
            }
        }
        if (!g_pVirtualMachine->StackPushInteger(bValue)) {
            return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
        }
    } else if (nCommandId == 0x211) {
        int bDetected = 0;
        if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
        if (pObject) {
            if (pObject->m_nObjectType == OBJECT_TYPE_TRIGGER) {
                CSWSTriggerTrapView* pTrigger = TRIGGER_TRAP(pObject);
                bDetected = pTrigger->m_aDetectedBy.count(oidCreature);
            } else if (pObject->m_nObjectType == OBJECT_TYPE_PLACEABLE) {
                CSWSPlaceableTrapView* pPlaceable = PLACEABLE_TRAP(pObject);
                bDetected = pPlaceable->m_aDetectedBy.count(oidCreature);
            } else if (pObject->m_nObjectType == OBJECT_TYPE_DOOR) {
                CSWSDoorTrapView* pDoor = DOOR_TRAP(pObject);
                bDetected = pDoor->m_aDetectedBy.count(oidCreature);
            }
        }
        if (!g_pVirtualMachine->StackPushInteger(bDetected)) {
            return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
        }
    } else if (nCommandId == 0x212) {
        int bValue = 0;
        if (pObject) {
            if (pObject->m_nObjectType == OBJECT_TYPE_TRIGGER) {
                bValue = TRIGGER_TRAP(pObject)->m_bFlagged;
            } else if (pObject->m_nObjectType == OBJECT_TYPE_PLACEABLE) {
                bValue = PLACEABLE_TRAP(pObject)->m_bFlagged;
            } else if (pObject->m_nObjectType == OBJECT_TYPE_DOOR) {
                bValue = DOOR_TRAP(pObject)->m_bFlagged;
            }
        }
        if (!g_pVirtualMachine->StackPushInteger(bValue)) {
            return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
        }
    } else if (nCommandId == 0x213) {
        int nType = -1;
        if (pObject) {
            if (pObject->m_nObjectType == OBJECT_TYPE_TRIGGER) {
                nType = TRIGGER_TRAP(pObject)->m_nTrapType;
            } else if (pObject->m_nObjectType == OBJECT_TYPE_PLACEABLE) {
                nType = PLACEABLE_TRAP(pObject)->m_nTrapType;
            } else if (pObject->m_nObjectType == OBJECT_TYPE_DOOR) {
                nType = DOOR_TRAP(pObject)->m_nTrapType;
            }
        }
        if (!g_pVirtualMachine->StackPushInteger(nType)) {
            return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
        }
    } else if (nCommandId == 0x214) {
        int bValue = 0;
        if (pObject) {
            if (pObject->m_nObjectType == OBJECT_TYPE_TRIGGER) {
                bValue = TRIGGER_TRAP(pObject)->m_bOneShot;
            } else if (pObject->m_nObjectType == OBJECT_TYPE_PLACEABLE) {
                bValue = PLACEABLE_TRAP(pObject)->m_bOneShot;
            } else if (pObject->m_nObjectType == OBJECT_TYPE_DOOR) {
                bValue = DOOR_TRAP(pObject)->m_bOneShot;
            }
        }
        if (!g_pVirtualMachine->StackPushInteger(bValue)) {
            return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
        }
    } else if (nCommandId == 0x215) {
        unsigned long oidCreator = INVALID_OBJECT_ID;
        if (pObject && pObject->m_nObjectType == OBJECT_TYPE_TRIGGER) {
            oidCreator = TRIGGER_TRAP(pObject)->m_oidCreator;
        }
        if (!g_pVirtualMachine->StackPushObject(oidCreator)) {
            return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
        }
    } else if (nCommandId == 0x216) {
        CExoString* pKeyTag = NULL;
        if (pObject) {
            if (pObject->m_nObjectType == OBJECT_TYPE_TRIGGER) {
                pKeyTag = &TRIGGER_TRAP(pObject)->m_sKeyTag;
            } else if (pObject->m_nObjectType == OBJECT_TYPE_PLACEABLE) {
                pKeyTag = &PLACEABLE_TRAP(pObject)->m_sKeyTag;
            } else if (pObject->m_nObjectType == OBJECT_TYPE_DOOR) {
                pKeyTag = &DOOR_TRAP(pObject)->m_sKeyTag;
            }
        }
        if (pKeyTag == NULL) {
            bool bFailed;
            {
                CExoString sEmpty("");
                bFailed = !g_pVirtualMachine->StackPushString(sEmpty);
            }
            if (bFailed) {
                return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
            }
        } else if (!g_pVirtualMachine->StackPushString(*pKeyTag)) {
            return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
        }
    } else if (nCommandId == 0x217) {
        int nDC = 0;
        if (pObject) {
            if (pObject->m_nObjectType == OBJECT_TYPE_TRIGGER) {
                nDC = TRIGGER_TRAP(pObject)->m_nDisarmDC;
            } else if (pObject->m_nObjectType == OBJECT_TYPE_PLACEABLE) {
                nDC = PLACEABLE_TRAP(pObject)->m_nDisarmDC;
            } else if (pObject->m_nObjectType == OBJECT_TYPE_DOOR) {
                nDC = DOOR_TRAP(pObject)->m_nDisarmDC;
            }
        }
        if (!g_pVirtualMachine->StackPushInteger(nDC)) {
            return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
        }
    } else if (nCommandId == 0x218) {
        int nDC = 0;
        if (pObject) {
            if (pObject->m_nObjectType == OBJECT_TYPE_TRIGGER) {
                nDC = TRIGGER_TRAP(pObject)->m_nDetectDC;
            } else if (pObject->m_nObjectType == OBJECT_TYPE_PLACEABLE) {
                nDC = PLACEABLE_TRAP(pObject)->m_nDetectDC;
            } else if (pObject->m_nObjectType == OBJECT_TYPE_DOOR) {
                nDC = DOOR_TRAP(pObject)->m_nDetectDC;
            }
        }
        if (!g_pVirtualMachine->StackPushInteger(nDC)) {
            return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
        }
    } else if (nCommandId == 0x226) {
        unsigned long oidSetter;
        int bDone = 0;
        if (!g_pVirtualMachine->StackPopObject(&oidSetter)) {
            return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
        }
        if (g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidSetter) && pObject) {
            if (pObject->m_nObjectType == OBJECT_TYPE_TRIGGER) {
                CSWSTriggerTrapView* pTrigger = TRIGGER_TRAP(pObject);
                pTrigger->m_aDetectedBy.AddUnique(oidSetter);
                bDone = 1;
            } else if (pObject->m_nObjectType == OBJECT_TYPE_PLACEABLE) {
                CSWSPlaceableTrapView* pPlaceable = PLACEABLE_TRAP(pObject);
                pPlaceable->m_aDetectedBy.AddUnique(oidSetter);
                bDone = 1;
            } else if (pObject->m_nObjectType == OBJECT_TYPE_DOOR) {
                CSWSDoorTrapView* pDoor = DOOR_TRAP(pObject);
                pDoor->m_aDetectedBy.AddUnique(oidSetter);
                bDone = 1;
            }
        }
        if (!g_pVirtualMachine->StackPushInteger(bDone)) {
            return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
        }
    } else if (nCommandId == 0x22b) {
        if (pObject) {
            if (pObject->m_nObjectType == OBJECT_TYPE_TRIGGER) {
                if (TRIGGER_TRAP(pObject)->GetTrapActive() == 1) {
                    CScriptEvent* pEvent = new CScriptEvent;
                    pEvent->m_nType = 0x18;
                    g_pAppManager->GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
                        0, 0, m_oidObjectRunScript, pObject->GetId(), EVENT_SIGNAL_EVENT, pEvent);
                    g_pAppManager->GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
                        0, 1, m_oidObjectRunScript, pObject->GetId(), EVENT_DESTROY_OBJECT, NULL);
                }
            } else if (pObject->m_nObjectType == OBJECT_TYPE_PLACEABLE) {
                if (PLACEABLE_TRAP(pObject)->m_bTrapped == 1) {
                    CScriptEvent* pEvent = new CScriptEvent;
                    pEvent->m_nType = 0x18;
                    g_pAppManager->GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
                        0, 0, m_oidObjectRunScript, pObject->GetId(), EVENT_SIGNAL_EVENT, pEvent);
                }
            } else if (pObject->m_nObjectType == OBJECT_TYPE_DOOR) {
                if (DOOR_TRAP(pObject)->m_bTrapped == 1) {
                    CScriptEvent* pEvent = new CScriptEvent;
                    pEvent->m_nType = 0x18;
                    g_pAppManager->GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
                        0, 0, m_oidObjectRunScript, pObject->GetId(), EVENT_SIGNAL_EVENT, pEvent);
                }
            }
        }
    }
    return 0;
}
