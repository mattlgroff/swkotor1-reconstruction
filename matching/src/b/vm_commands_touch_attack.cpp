// CSWVirtualMachineCommands::ExecuteCommandTouchAttackMelee (round s9):
// TouchAttackMelee (0x92) and TouchAttackRanged (0x93).
#include "b/vm_commands.h"
#include "b/sws_creature.h"
#include "b/sws_message.h"
#include "b/sws_player.h"
#include "b/sw_rules.h"

extern const unsigned char GAME_OBJECT_VALID;               // 0x0074666C
extern const unsigned char OBJECT_TYPE_PLACEABLE;           // 0x00746638 (9)
extern const unsigned char OBJECT_TYPE_DOOR;                // 0x00746639 (10)

class CSWSCombatAttackData {
public:
    char pad_0x0[0x3c];
    unsigned char m_nDeflectB;                              // 0x3c (name guessed)
    char pad_0x3d[0x5c - 0x3d];
    unsigned char m_nDeflectA;                              // 0x5c (name guessed)
};

class CSWCCMessageData {
public:
    CSWCCMessageData();                                     // 0x004D69F0
    ~CSWCCMessageData();                                    // 0x004D6A40
    void SetInteger(int nIndex, int nValue);                // 0x004D76B0
    void SetObjectID(int nIndex, unsigned long oid);        // 0x004D7780

    char pad_0x0[0x34];
};

// Result: 0 miss, 1 hit, 2 critical hit.
// NONMATCH (67): register assignment (this/result/bonus/roll in ebx/edi/ebx/ebp
// in the original); the control flow and frame match.
// FUNCTION: SWKOTOR 0x00548CA0
int CSWVirtualMachineCommands::ExecuteCommandTouchAttackMelee(int nCommandId, int nParameters)
{
    int nResult = 0;
    unsigned long oidTarget;
    int bDisplayFeedback = 1;
    unsigned char nDeflectA = 0;
    unsigned char nDeflectB = 0;
    if (!g_pVirtualMachine->StackPopObject(&oidTarget)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (nParameters >= 2 && !g_pVirtualMachine->StackPopInteger(&bDisplayFeedback)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObjectArray* pObjectArray = g_pAppManager->GetServerExoApp()->GetObjectArray();
    CGameObject* pSelf;
    CGameObject* pTarget;
    if (pObjectArray->GetGameObject(oidTarget, &pTarget) == GAME_OBJECT_VALID &&
        pObjectArray->GetGameObject(m_oidObjectRunScript, &pSelf) == GAME_OBJECT_VALID) {
        int nAttackBonus;
        int nRoll;
        if (pSelf->AsSWSCreature()) {
            if (pTarget->AsSWSCreature()) {
                if (nCommandId == 0x92) {
                    CSWSCreatureStats* pStats = pSelf->AsSWSCreature()->m_pStats;
                    nAttackBonus = pStats->GetMeleeAttackBonus(0, 1, 1, 0);
                } else {
                    CSWSCreatureStats* pStats = pSelf->AsSWSCreature()->m_pStats;
                    nAttackBonus = pStats->GetRangedAttackBonus(0, 1, 1);
                }
                CSWSCreatureStats* pTargetStats = pTarget->AsSWSCreature()->m_pStats;
                int nAC = pTargetStats->GetArmorClassVersus(pSelf->AsSWSCreature(), 1);
                nRoll = g_pRules->RollDice(1, 0x14);
                if (pSelf->AsSWSCreature()->ResolveDefensiveEffects(pTarget->AsSWSObject(), 1)) {
                    nResult = 0;
                    int nAttack = pSelf->AsSWSCreature()->m_pCombatRound->m_nCurrentAttack;
                    CSWSCombatRound* pRound = pSelf->AsSWSCreature()->m_pCombatRound;
                    CSWSCombatAttackData* pAttack = pRound->GetAttack(nAttack);
                    nDeflectA = pAttack->m_nDeflectA;
                    nDeflectB = pAttack->m_nDeflectB;
                } else if (nRoll != 1 && (nAttackBonus + nRoll >= nAC || nRoll == 0x14)) {
                    if (nRoll == 0x14) {
                        nResult = (g_pRules->RollDice(1, 0x14) + nAttackBonus >= nAC) ? 2 : 1;
                    } else {
                        nResult = 1;
                    }
                } else {
                    nResult = 0;
                }
            } else {
                if (pTarget->m_nObjectType == OBJECT_TYPE_DOOR || pTarget->m_nObjectType == OBJECT_TYPE_PLACEABLE) {
                    nResult = 1;
                }
                nAttackBonus = nCommandId;
                nRoll = nCommandId;
            }
        } else {
            nAttackBonus = nCommandId;
            nRoll = nCommandId;
        }
        if (bDisplayFeedback == 1) {
            CSWCCMessageData* pData = new CSWCCMessageData;
            pData->SetObjectID(0, m_oidObjectRunScript);
            pData->SetObjectID(1, pTarget->GetId());
            pData->SetInteger(0, nAttackBonus);
            pData->SetInteger(1, nRoll);
            if (nResult == 0) {
                if (nDeflectA == 0) {
                    pData->SetInteger(2, 4);
                } else {
                    pData->SetInteger(2, nDeflectA);
                    pData->SetInteger(4, nDeflectB);
                }
            } else if (nResult == 1 || nResult == 2) {
                pData->SetInteger(2, nResult);
            }
            if (nCommandId == 0x93) {
                pData->SetInteger(3, 1);
            }
            CSWSPlayer* pPlayer = (CSWSPlayer*)g_pAppManager->GetServerExoApp()->GetClientObjectByObjectId(m_oidObjectRunScript);
            if (pPlayer) {
                g_pAppManager->GetServerExoApp()->GetSWSMessage()->SendServerToPlayerCCMessage(pPlayer->m_nPlayerID, 0xd, pData, 0);
            }
            pPlayer = (CSWSPlayer*)g_pAppManager->GetServerExoApp()->GetClientObjectByObjectId(pTarget->GetId());
            if (pPlayer) {
                g_pAppManager->GetServerExoApp()->GetSWSMessage()->SendServerToPlayerCCMessage(pPlayer->m_nPlayerID, 0xd, pData, 0);
            }
            if (pData) {
                delete pData;
            }
        }
    } else {
        bDisplayFeedback = 0;
    }
    if (!g_pVirtualMachine->StackPushInteger(nResult)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}
