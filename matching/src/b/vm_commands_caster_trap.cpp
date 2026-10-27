// CSWVirtualMachineCommands: GetCasterLevel and GetNearestTrapToObject (round s8).
#include "b/vm_commands.h"
#include "b/sws_creature.h"
#include "b/sw_rules.h"
#include "b/sws_door.h"
#include "b/sws_item.h"
#include "b/server_exo_app.h"

// Spell fields shared by CSWSObject-derived objects.
struct CSWSObjectCasterView {
    char pad_0x0[0x1c0];
    int m_nLastSpellId;                                     // 0x1c0
    char pad_0x1c4[0x1d4 - 0x1c4];
    unsigned char m_nLastSpellCastClass;                    // 0x1d4
};

// Slot 0x50 object (bucket b calls the slot AsSWSTrigger; buckets a and i
// call it AsSWSAreaOfEffectObject).
class CSWSTrigger : public CSWSObject {
public:
    char pad_0xf4[0x248 - 0xf4];
    unsigned long m_oidCreator;                             // 0x248
};

// NONMATCH (14): register choice in the two innate-spell lookups (the
// original reloads the creature's spell id into esi).
// FUNCTION: SWKOTOR 0x00536AD0
int CSWVirtualMachineCommands::ExecuteCommandGetCasterLevel(int nCommandId, int nParameters)
{
    int nLevel = 0;
    unsigned long oidObject;
    if (!g_pVirtualMachine->StackPopObject(&oidObject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidObject, &pObject) == GAME_OBJECT_VALID) {
        CSWSCreature* pCreature = pObject->AsSWSCreature();
        void* pPlaceable = pObject->AsSWSPlaceable();
        unsigned long nSpellId;
        if (pCreature) {
            nSpellId = ((CSWSObjectCasterView*)pCreature)->m_nLastSpellId;
        } else if (!pPlaceable) {
            if (!pObject->AsSWSTrigger()) {
                goto done;
            }
            nSpellId = pObject->AsSWSTrigger()->GetLastSpellId();
            unsigned long oidCreator = pObject->AsSWSTrigger()->m_oidCreator;
            pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreator);
            pPlaceable = g_pAppManager->GetServerExoApp()->GetPlaceableByGameObjectID(oidCreator);
        }
        if (pCreature) {
            if (pCreature->m_n964) {
                nLevel = pCreature->m_n968;
            } else {
                int nClass = ((CSWSObjectCasterView*)pCreature)->m_nLastSpellCastClass;
                if (nClass == 0xff) {
                    CSWSpell* pSpell = g_pRules->m_pSpellArray->GetSpell(((CSWSObjectCasterView*)pCreature)->m_nLastSpellId);
                    if (pSpell) {
                        nLevel = pSpell->m_nInnateLevel * 2 - 1;
                        if (nLevel < 10) {
                            nLevel = 10;
                        }
                    }
                } else if (nClass == 0xfe) {
                    nLevel = pCreature->m_pStats->GetMemorizedSpellClass(nSpellId);
                } else {
                    nLevel = pCreature->m_pStats->GetClassLevel((unsigned char)nClass, 1);
                }
            }
        } else if (pPlaceable) {
            CSWSpell* pSpell = g_pRules->m_pSpellArray->GetSpell(((CSWSObjectCasterView*)pPlaceable)->m_nLastSpellId);
            if (pSpell) {
                nLevel = pSpell->m_nInnateLevel * 2 - 1;
                if (nLevel < 10) {
                    nLevel = 10;
                }
            }
        }
    }
done:
    return g_pVirtualMachine->StackPushInteger(nLevel) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}

// Slot 0x40 object (bucket b calls the slot AsSWSAreaOfEffectObject; it is
// the trigger): ids of the creatures that detected its trap.
struct CSWSTriggerDetectView {
    char pad_0x0[0x2a8];
    CExoArrayList<unsigned long> m_aTrapDetectedBy;         // 0x2a8 (name guessed)
};

// NONMATCH (20): the second pop's failure block and the detected-by count
// calls (the original evaluates the object before pushing the id).
// FUNCTION: SWKOTOR 0x0054A090
int CSWVirtualMachineCommands::ExecuteCommandGetNearestTrapToObject(int nCommandId, int nParameters)
{
    unsigned long oidTarget;
    CExoArrayList<unsigned long>* pTraps = NULL;
    float fBestDistance = 1.0e8f;
    int bDetectedOnly;
    if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&bDetectedOnly)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CSWSObject* pTarget;
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidTarget);
    if (pObject && pObject->AsSWSObject()) {
        pTarget = pObject->AsSWSObject();
        if (pTarget->GetArea()) {
            pTraps = &pObject->AsSWSObject()->GetArea()->m_aTrapList;
        }
    }
    unsigned long oidResult = INVALID_OBJECT_ID;
    if (pTraps) {
        for (int i = 0; i < pTraps->num; i++) {
            CGameObject* pTrap = g_pAppManager->GetServerExoApp()->GetGameObject(pTraps->element[i]);
            if (!pTrap) {
                continue;
            }
            if (bDetectedOnly == 1) {
                CExoArrayList<unsigned long>* pDetectedBy;
                if (pTrap->AsSWSAreaOfEffectObject()) {
                    pDetectedBy = &((CSWSTriggerDetectView*)pTrap->AsSWSAreaOfEffectObject())->m_aTrapDetectedBy;
                } else if (pTrap->AsSWSDoor()) {
                    pDetectedBy = &pTrap->AsSWSDoor()->m_aTrapDetectedBy;
                } else if (pTrap->AsSWSPlaceable()) {
                    pDetectedBy = &pTrap->AsSWSPlaceable()->m_aTrapDetectedBy;
                } else {
                    continue;
                }
                if (!pDetectedBy->count(oidTarget)) {
                    continue;
                }
            }
            Vector& vTrap = pTrap->AsSWSObject()->m_vPosition;
            Vector vDelta;
            vDelta.x = pTarget->m_vPosition.x - vTrap.x;
            vDelta.y = pTarget->m_vPosition.y - vTrap.y;
            vDelta.z = pTarget->m_vPosition.z - vTrap.z;
            float fDistance = vDelta.y * vDelta.y + vDelta.x * vDelta.x + vDelta.z * vDelta.z;
            if (fDistance < fBestDistance) {
                fBestDistance = fDistance;
                oidResult = pTraps->element[i];
            }
        }
    }
    return g_pVirtualMachine->StackPushObject(oidResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}
