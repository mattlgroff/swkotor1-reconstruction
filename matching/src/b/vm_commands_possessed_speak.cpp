// CSWVirtualMachineCommands: GetItemPossessedBy, ActionSpeakString and
// SetJournalQuestEntryPicture (round s8).
#include "b/vm_commands.h"
#include "b/sws_creature.h"
#include "b/sws_message.h"
#include "b/server_exo_app.h"
#include "b/sws_item.h"
#include "b/sws_player.h"
// NONMATCH (2 instructions): lea ecx/eax register choice for the tag compare.
// FUNCTION: SWKOTOR 0x0053A390
int CSWVirtualMachineCommands::ExecuteCommandGetItemPossessedBy(int nCommandId, int nParameters)
{
    unsigned long oidCreature;
    unsigned long oidItem = INVALID_OBJECT_ID;
    CExoString sItemTag;
    int bFound;
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopString(&sItemTag)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CExoString sTag = sItemTag.LowerCase();
    CGameObject* pObject;
    if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(oidCreature, &pObject) == GAME_OBJECT_VALID) {
        if (pObject->AsSWSCreature()) {
            CSWSCreature* pCreature = pObject->AsSWSCreature();
            oidItem = pCreature->GetItemRepository(1)->FindItemWithTag(sTag);
            bFound = oidItem != INVALID_OBJECT_ID;
            CSWInventory* pInventory = pCreature->m_pInventory;
            for (int nSlot = 0; nSlot < 18 && !bFound; nSlot++) {
                unsigned long nSlotFlag = 1 << nSlot;
                CSWSItem* pItem = pInventory->GetItemInSlot(nSlotFlag);
                if (pItem && pItem->m_sTag == sTag) {
                    oidItem = pItem->m_idSelf;
                    bFound = 1;
                }
            }
        } else if (pObject->AsSWSPlaceable()) {
            if (pObject->AsSWSPlaceable()->m_bHasInventory && pObject->AsSWSPlaceable()->m_pItemRepository) {
                CItemRepository* pRepository = pObject->AsSWSPlaceable()->m_pItemRepository;
                oidItem = pRepository->FindItemWithTag(sTag);
            }
        } else if (pObject->AsSWSItem()) {
            if (pObject->AsSWSItem()->m_pRepository) {
                CItemRepository* pRepository = pObject->AsSWSItem()->m_pRepository;
                oidItem = pRepository->FindItemWithTag(sTag);
            }
        }
    }
    if (!g_pVirtualMachine->StackPushObject(oidItem)) {
        return VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
    }
    return 0;
}
// FUNCTION: SWKOTOR 0x00544090
int CSWVirtualMachineCommands::ExecuteCommandActionSpeakString(int nCommandId, int nParameters)
{
    CExoString sString;
    int nTalkVolume;
    if (!g_pVirtualMachine->StackPopString(&sString)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nTalkVolume)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (m_bValidObjectRunScript == 1) {
        CGameObject* pObject;
        if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(m_oidObjectRunScript, &pObject) == GAME_OBJECT_VALID) {
            int nVolume = 1;
            if (nTalkVolume == 1) {
                nVolume = 3;
            } else if (nTalkVolume == 2) {
                nVolume = 2;
            } else if (nTalkVolume == 3) {
                nVolume = 0xd;
            } else if (nTalkVolume == 4) {
                nVolume = 0xe;
            }
            if (pObject) {
                if (nCommandId == 0xdd) {
                    CExoString sName("");
                    g_pAppManager->GetServerExoApp()->GetSWSMessage()->ChatMessage(nVolume, m_oidObjectRunScript, sString,
                                                                                   0xffffffff, sName);
                } else if (pObject->m_bCommandable) {
                    ((CSWSObject*)pObject)->AddAction(0xe, 0xffff, 4, &sString, 1, &nVolume);
                }
            }
        }
    }
    return 0;
}
// FUNCTION: SWKOTOR 0x0054B1E0
int CSWVirtualMachineCommands::ExecuteCommandSetJournalQuestEntryPicture(int nCommandId, int nParameters)
{
    CExoString sPlotID;
    CExoString sVarName;
    void* pPos;
    unsigned long oidCreature;
    int nPicture;
    int bAllPartyMembers;
    int bAllPlayers;
    if (!g_pVirtualMachine->StackPopString(&sPlotID)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopObject(&oidCreature)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&nPicture)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&bAllPartyMembers)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    if (!g_pVirtualMachine->StackPopInteger(&bAllPlayers)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    sVarName.Format("NW_JOURNAL_PICTURE%s", sPlotID);
    if (!bAllPlayers) {
        CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
        if (pCreature) {
            CSWSScriptVarTable* pVars = &pCreature->m_ScriptVars;
            if (pVars) {
                pVars->SetInt(sVarName, nPicture, 0);
            }
            if (bAllPartyMembers) {
                CSWSFaction* pFaction = pCreature->GetFaction();
                if (pFaction) {
                    unsigned long* pMembers = pFaction->m_aMembers.element;
                    int nMembers = pFaction->m_aMembers.num;
                    if (pMembers) {
                        for (int i = 0; i < nMembers; i++) {
                            CSWSCreature* pMember = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(pMembers[i]);
                            if (pMember && pMember != pCreature) {
                                CSWSScriptVarTable* pMemberVars = &pMember->m_ScriptVars;
                                if (pMemberVars) {
                                    pMemberVars->SetInt(sVarName, nPicture, 0);
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        CExoLinkedList<CSWSClient>* pPlayers = g_pAppManager->GetServerExoApp()->GetPlayerList();
        if (!pPlayers) {
            return 1;
        }
        pPos = pPlayers->GetHeadPos();
        while (pPos) {
            CSWSPlayer* pPlayer = (CSWSPlayer*)pPlayers->GetAtPos(pPos);
            if (pPlayer && pPlayer->GetGameObject()) {
                CSWSCreature* pPlayerCreature = pPlayer->GetGameObject()->AsSWSCreature();
                if (pPlayerCreature) {
                    CSWSScriptVarTable* pVars = &pPlayerCreature->m_ScriptVars;
                    if (pVars) {
                        pVars->SetInt(sVarName, nPicture, 0);
                    }
                }
            }
            pPlayers->GetNext(pPos);
        }
    }
    return 0;
}
