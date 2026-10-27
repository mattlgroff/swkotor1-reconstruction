// CSWVirtualMachineCommands::ExecuteCommandResistForce (round s9).
#include "b/vm_commands.h"
#include "b/sws_creature.h"
#include "b/sws_message.h"
#include "b/sws_player.h"
#include "b/sw_rules.h"

extern const unsigned char OBJECT_TYPE_CREATURE;            // 0x00746634 (5)

// Spell fields shared by CSWSObject-derived objects.
struct CSWSObjectSpellView {
    char pad_0x0[0x1c0];
    int m_nLastSpellId;                                     // 0x1c0
    char pad_0x1c4[0x1d4 - 0x1c4];
    unsigned char m_nLastSpellCastClass;                    // 0x1d4
};

// Slot 0x50 object (bucket b's AsSWSTrigger) carries its caster level.
class CSWSTrigger : public CSWSObject {
public:
    char pad_0xf4[0x25c - 0xf4];
    int m_nCasterLevel;                                     // 0x25c (name guessed)
};

class CSWCCMessageData {
public:
    CSWCCMessageData();                                     // 0x004D69F0
    ~CSWCCMessageData();                                    // 0x004D6A40
    void SetInteger(int nIndex, int nValue);                // 0x004D76B0
    void SetObjectID(int nIndex, unsigned long oid);        // 0x004D7780

    char pad_0x0[0x34];
};

// Returns 1 when the target resisted (force resistance roll or immunity),
// otherwise the absorption/immunity result or 0.
// NONMATCH (124): register allocation (the original keeps nResult on the stack
// and nCasterLevel in ebp; cl keeps nResult in ebx), which also changes the
// placement of the callee-saved pushes and the result-zero exits.
// FUNCTION: SWKOTOR 0x00541CC0
int CSWVirtualMachineCommands::ExecuteCommandResistForce(int nCommandId, int nParameters)
{
    int nResult;
    unsigned long oidTarget;
    unsigned long oidCaster;
    if (!g_pVirtualMachine->StackPopObject(&oidCaster)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pCaster = g_pAppManager->GetServerExoApp()->GetGameObject(oidCaster);
    CGameObject* pTarget = g_pAppManager->GetServerExoApp()->GetGameObject(oidTarget);
    if (pCaster && pTarget) {
        CSWSpell* pSpell = g_pRules->m_pSpellArray->GetSpell(((CSWSObjectSpellView*)pCaster->AsSWSObject())->m_nLastSpellId);
        if (pSpell) {
            int nCasterLevel;
            CGameObject* pSelf = g_pAppManager->GetServerExoApp()->GetGameObject(m_oidObjectRunScript);
            if (pSelf && pSelf->AsSWSTrigger()) {
                nCasterLevel = pSelf->AsSWSTrigger()->m_nCasterLevel;
            } else {
                if (pCaster->m_nObjectType == OBJECT_TYPE_CREATURE) {
                    CSWSCreature* pCreature = pCaster->AsSWSCreature();
                    if (pCreature->m_n964 == 1) {
                        nCasterLevel = pCreature->m_n968;
                        goto have_level;
                    }
                    if (((CSWSObjectSpellView*)pCreature)->m_nLastSpellCastClass != 0xff &&
                        ((CSWSObjectSpellView*)pCreature)->m_nLastSpellCastClass != 0xfe) {
                        nCasterLevel = pCreature->GetStats()->GetLevel(0);
                        goto have_level;
                    }
                }
                nCasterLevel = pSpell->m_nInnateLevel * 2 - 1;
            }
        have_level:
            if (pTarget->m_nObjectType == OBJECT_TYPE_CREATURE) {
                CSWSCreature* pTargetCreature = pTarget->AsSWSCreature();
                nResult = pTargetCreature->DoSpellLevelAbsorption(pCaster->AsSWSObject());
                if (nResult == -1) {
                    nResult = pTargetCreature->DoSpellImmunity(pCaster->AsSWSObject());
                }
                int nResistance = 0;
                int nRoll;
                if (nResult == -1) {
                    if (pSpell->m_bForceResistable != 1) {
                        goto done;
                    }
                    nResistance = pTargetCreature->GetStats()->GetForceResistance();
                    if (nResistance <= 0) {
                        nResult = 0;
                        goto done;
                    }
                    nRoll = g_pRules->RollDice(1, 0x14);
                    if (nRoll + nCasterLevel >= nResistance) {
                        nResult = 0;
                        goto done;
                    }
                    nResult = 1;
                } else if (nResult != 1) {
                    goto done;
                }
                CSWCCMessageData* pData = new CSWCCMessageData;
                pData->SetObjectID(0, oidTarget);
                pData->SetInteger(0, 1);
                CSWSPlayer* pPlayer = (CSWSPlayer*)g_pAppManager->GetServerExoApp()->GetClientObjectByObjectId(oidCaster);
                if (pPlayer) {
                    g_pAppManager->GetServerExoApp()->GetSWSMessage()->SendServerToPlayerCCMessage(pPlayer->m_nPlayerID, 0xa, pData, 0);
                }
                pPlayer = (CSWSPlayer*)g_pAppManager->GetServerExoApp()->GetClientObjectByObjectId(oidTarget);
                if (pPlayer) {
                    g_pAppManager->GetServerExoApp()->GetSWSMessage()->SendServerToPlayerCCMessage(pPlayer->m_nPlayerID, 0xa, pData, 0);
                }
                if (pData) {
                    delete pData;
                }
                pData = new CSWCCMessageData;
                pData->SetObjectID(0, oidTarget);
                pData->SetObjectID(1, oidCaster);
                pData->SetInteger(0, nRoll);
                pData->SetInteger(1, nCasterLevel);
                pData->SetInteger(2, nResistance);
                pPlayer = (CSWSPlayer*)g_pAppManager->GetServerExoApp()->GetClientObjectByObjectId(oidCaster);
                if (pPlayer) {
                    g_pAppManager->GetServerExoApp()->GetSWSMessage()->SendServerToPlayerCCMessage(pPlayer->m_nPlayerID, 0x11, pData, 0);
                }
                pPlayer = (CSWSPlayer*)g_pAppManager->GetServerExoApp()->GetClientObjectByObjectId(oidTarget);
                if (pPlayer) {
                    g_pAppManager->GetServerExoApp()->GetSWSMessage()->SendServerToPlayerCCMessage(pPlayer->m_nPlayerID, 0x11, pData, 0);
                }
                if (pData) {
                    delete pData;
                }
            } else {
                nResult = 0;
            }
        } else {
            nResult = 0;
        }
    } else {
        nResult = 0;
    }
done:
    if (!g_pVirtualMachine->StackPushInteger(nResult)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}
