// CSWSMessage player-to-server handlers (0x005209B0-0x00527890).
#include "b/sws_message.h"
#include "b/sws_player.h"
#include "b/sws_creature.h"
#include "b/sws_item.h"

extern const unsigned char GAME_OBJECT_VALID;       // 0x0074666C

// FUNCTION: SWKOTOR 0x005209B0
int CSWSMessage::HandlePlayerToServerQuickChatMessage(CSWSPlayer* pPlayer, unsigned char nMinor)
{
    if (MessageReadOverflow(1)) {
        return 0;
    }
    unsigned short nQuickChat = ReadWORD(16);
    if (!MessageReadOverflow(1) && !MessageReadUnderflow(1)) {
        CGameObject* pObject = pPlayer->GetGameObject();
        if (pObject) {
            return QuickChatMessage(pObject->m_idSelf, nQuickChat);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00523240
int CSWSMessage::HandlePlayerToServerGuiInventoryMessage(CSWSPlayer* pPlayer, unsigned char nMinor)
{
    unsigned char bOpen = ReadBYTE(8);
    if (!MessageReadOverflow(1) && !MessageReadUnderflow(1)) {
        if (nMinor == 1) {
            pPlayer->m_pStoreGUI->SetOpen(bOpen != 0);
        }
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00523570
int CSWSMessage::HandlePlayerToServerInputCancelGuiTimingEvent(CSWSPlayer* pPlayer)
{
    CGameObject* pObject = pPlayer->GetGameObject();
    if (!pObject) {
        return 0;
    }
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    if (pCreature && pCreature->m_bCommandable == 1) {
        pCreature->ClearAllActions(1);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00524C40
int CSWSMessage::HandlePlayerToServerServerStatusMessage(CSWSPlayer* pPlayer, unsigned char nMinor)
{
    if (!MessageReadOverflow(1) && nMinor == 0) {
        return SendServerToPlayerServerStatus_Status(pPlayer->GetPlayerId());
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00525350
int CSWSMessage::HandlePlayerToServerCharacterDownload(CSWSPlayer* pPlayer, unsigned char nMinor)
{
    if (MessageReadOverflow(1)) {
        return 0;
    }
    switch (nMinor) {
    case 1:
        if (pPlayer->m_n64 != 1) {
            return SendServerToPlayerCharacterDownloadFail(pPlayer);
        }
        return SendServerToPlayerCharacterDownloadReply(pPlayer);
    default:
        return 0;
    }
}

// FUNCTION: SWKOTOR 0x00525390
int CSWSMessage::HandlePlayerToServerShutDownServer(CSWSPlayer* pPlayer, unsigned char nMinor)
{
    if (MessageReadOverflow(1)) {
        return 0;
    }
    int bValue = ReadBOOL();
    if (!MessageReadOverflow(1) && !MessageReadUnderflow(1)) {
        g_pAppManager->GetServerExoApp()->Shutdown(!bValue, 1);
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005253F0
int CSWSMessage::HandlePlayerToServerPlayModuleCharacterList_Stop(CSWSPlayer* pPlayer)
{
    if (!MessageReadOverflow(1) && !MessageReadUnderflow(1)) {
        pPlayer->m_n48 = 0;
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00525450
int CSWSMessage::HandlePlayerToServerInputAbortDriveControl(CSWSPlayer* pPlayer)
{
    if (!MessageReadOverflow(1) && !MessageReadUnderflow(1)) {
        CGameObject* pObject = pPlayer->GetGameObject();
        if (pObject) {
            CSWSCreature* pCreature = pObject->AsSWSCreature();
            if (pCreature && pCreature->m_bCommandable == 1) {
                CExoLinkedList<CSWSObjectActionNode>* pActions = &pCreature->m_lQueuedActions;
                if (pActions) {
                    CSWSObjectActionNode* pNode = pActions->GetHead();
                    if (pNode && pNode->m_nActionId == 0x33) {
                        pNode->m_pParameter[5] = 0;
                    }
                }
            }
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00527380
int CSWSMessage::HandlePlayerToServerMapPinMessage(CSWSPlayer* pPlayer, unsigned char nMinor)
{
    switch (nMinor) {
    case 2:
        return HandlePlayerToServerMapPinSetMapPinAt(pPlayer);
    case 3:
        return HandlePlayerToServerMapPinDestroyMapPin(pPlayer);
    case 5:
        return HandlePlayerToServerMapPinChangePin(pPlayer);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00527890
int CSWSMessage::HandlePlayerToServerPlayModuleCharacterList(CSWSPlayer* pPlayer, unsigned char nMinor)
{
    if (MessageReadOverflow(1)) {
        return 0;
    }
    switch (nMinor) {
    case 1:
        return HandlePlayerToServerPlayModuleCharacterList_Start(pPlayer);
    case 2:
        return HandlePlayerToServerPlayModuleCharacterList_Stop(pPlayer);
    default:
        return 0;
    }
}

// FUNCTION: SWKOTOR 0x00524B80
int CSWSMessage::HandlePlayerToServerAreaMessage(CSWSPlayer* pPlayer, unsigned char nMinor)
{
    if (MessageReadOverflow(1)) {
        return 0;
    }
    int bResult = 1;
    if (nMinor == 3) {
        if (pPlayer->m_n24 == 1) {
            pPlayer->m_n24 = 2;
        }
        if (g_pAppManager->GetServerExoApp()->GetServerMode() == 2) {
            g_pAppManager->GetServerExoApp()->MovePlayerToArea(pPlayer);
            bResult &= SendServerToPlayerPlayerList_Add(PLAYER_ID_ALL_PLAYERS, pPlayer);
            bResult &= SendServerToPlayerPlayerList_All(pPlayer);
            if (g_pAppManager->GetServerExoApp()->GetPauseState(1) == 1) {
                SendServerToPlayerModule_SetPauseState(1, 1);
            }
            if (g_pAppManager->GetServerExoApp()->GetPauseState(2) == 1) {
                SendServerToPlayerModule_SetPauseState(2, 1);
            }
        }
        return bResult;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005273D0
int CSWSMessage::HandlePlayerToServerPlayModuleCharacterList_Start(CSWSPlayer* pPlayer)
{
    if (!MessageReadOverflow(1) && !MessageReadUnderflow(1)) {
        CExoLinkedList<CSWSClient>* pPlayers = g_pAppManager->GetServerExoApp()->GetPlayerList();
        void* pPos = pPlayers->GetHeadPos();
        while (pPos) {
            CSWSPlayer* pOther = (CSWSPlayer*)pPlayers->GetAtPos(pPos);
            if (pOther && pOther->GetGameObject()) {
                unsigned long oidCharacter = pOther->GetGameObject()->m_idSelf;
                if (oidCharacter != OBJECT_INVALID_ID) {
                    CSWSMessage* pMessage = g_pAppManager->GetServerExoApp()->GetSWSMessage();
                    if (pMessage) {
                        pMessage->SendServerToPlayerPlayModuleCharacterListResponse(
                            pPlayer->m_nPlayerID, oidCharacter, 1);
                    }
                }
            }
            pPlayers->GetNext(pPos);
        }
        pPlayer->m_n48 = 1;
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005252A0
int CSWSMessage::HandlePlayerToServerPlayerDeath(CSWSPlayer* pPlayer, unsigned char nMinor)
{
    switch (nMinor) {
    case 1: {
        unsigned long oidModule = g_pAppManager->GetServerExoApp()->GetModule()->GetId();
        CScriptEvent* pEvent = new CScriptEvent;
        pEvent->m_nType = 0x21;
        unsigned long oidPlayer = pPlayer->GetGameObject()->m_idSelf;
        g_pAppManager->GetServerExoApp()->GetServerAIMaster()->AddEventDeltaTime(
            0, 0, oidPlayer, oidModule, EVENT_DELAY_INVENTORY, pEvent);
        break;
    }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00522810
int CSWSMessage::HandlePlayerToServerDialogMessage(CSWSPlayer* pPlayer, unsigned char nMinor)
{
    if (!MessageReadOverflow(1) && nMinor == 3) {
        unsigned long oidObject = ReadOBJECTIDServer();
        unsigned long nReply = ReadDWORD(32);
        int bArg = ReadBYTE(8) != 0;
        unsigned long nArg = ReadDWORD(32);
        if (!MessageReadOverflow(1) && !MessageReadUnderflow(1)) {
            CGameObject* pObject;
            if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(
                    oidObject, &pObject) == GAME_OBJECT_VALID) {
                return ((CSWSObject*)pObject)->ReplyDialog(pPlayer->m_nPlayerID, nReply, bArg,
                                                           nArg, 0);
            }
        }
    }
    return 0;
}

extern const unsigned char OBJECT_TYPE_ITEM;        // 0x00746635
extern const unsigned char OBJECT_TYPE_PLACEABLE;   // 0x00746638

// FUNCTION: SWKOTOR 0x005232A0
int CSWSMessage::HandlePlayerToServerGuiContainerMessage(CSWSPlayer* pPlayer, unsigned char nMinor)
{
    if (nMinor == 2) {
        unsigned long oidContainer = ReadOBJECTIDServer();
        int bValue = ReadBOOL();
        if (MessageReadOverflow(1) || MessageReadUnderflow(1)) {
            return 0;
        }
        CGameObject* pObject;
        if (g_pAppManager->GetServerExoApp()->GetObjectArray()->GetGameObject(
                oidContainer, &pObject) == GAME_OBJECT_VALID) {
            unsigned char nType = pObject->m_nObjectType;
            if (nType == OBJECT_TYPE_PLACEABLE) {
                unsigned long oidPlayer = pPlayer->GetGameObject()->m_idSelf;
                pObject->AsSWSPlaceable()->CloseInventory(oidPlayer, bValue);
            }
            else if (nType == OBJECT_TYPE_ITEM) {
                unsigned long oidPlayer = pPlayer->GetGameObject()->m_idSelf;
                pObject->AsSWSItem()->CloseInventory(oidPlayer, bValue);
            }
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00523380
int CSWSMessage::HandlePlayerToServerCharacterSheetMessage(CSWSPlayer* pPlayer, unsigned char nMinor)
{
    unsigned char nMode = ReadBYTE(8);
    unsigned long oidCreature = ReadOBJECTIDServer();
    if (MessageReadOverflow(1) || MessageReadUnderflow(1)) {
        return 0;
    }
    if (nMinor == 1) {
        if (!pPlayer->PermittedToDisplayCharacterSheet(oidCreature)) {
            nMode = 0xff;
            SendServerToPlayerGUICharacterSheet_NotPermitted(pPlayer->m_nPlayerID, oidCreature);
            oidCreature = 0x7f000000;
        }
        unsigned long oidOld = pPlayer->m_pInventoryGUI->m_oidCreature;
        CSWSCreature* pOld = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidOld);
        if (pOld) {
            pOld->m_nCharSheetViewers--;
        }
        CSWSCreature* pNew = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
        if (pNew) {
            pNew->m_nCharSheetViewers++;
        }
        pPlayer->m_pInventoryGUI->SetCreatureDisplayed(oidCreature);
        pPlayer->m_pInventoryGUI->m_nDisplayMode = nMode;
        if (nMode == 0xff) {
            pPlayer->m_pInventoryGUI->m_pItems->ClearEffectIcons();
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00523B20
int CSWSMessage::HandlePlayerToServerGoldMessage(CSWSPlayer* pPlayer, unsigned char nMinor)
{
    Vector vPosition;
    vPosition.x = 0.0f;
    vPosition.y = 0.0f;
    vPosition.z = 0.0f;
    CSWSCreature* pCreature;
    CGameObject* pObject = pPlayer->GetGameObject();
    if (pObject && pObject->AsSWSCreature()) {
        pCreature = pObject->AsSWSCreature();
    }
    if (!MessageReadOverflow(1)) {
        unsigned long oidTarget;
        int nAmount;
        switch (nMinor) {
        case 1:
            oidTarget = ReadOBJECTIDServer();
            nAmount = ReadDWORD(32);
            if (MessageReadOverflow(1)) {
                break;
            }
            if (oidTarget == 0x7f000000) {
                vPosition.x = ReadFLOAT(1.0f, 32);
                vPosition.y = ReadFLOAT(1.0f, 32);
                vPosition.z = ReadFLOAT(1.0f, 32);
            }
            if (MessageReadOverflow(1)) {
                break;
            }
            if (MessageReadOverflow(1)) {
                break;
            }
            pCreature->TransferGold(oidTarget, &vPosition, nAmount);
            break;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00523450
int CSWSMessage::HandlePlayerToServerInputDriveControl(CSWSPlayer* pPlayer)
{
    float fX = ReadFLOAT(1.0f, 32);
    float fY = ReadFLOAT(1.0f, 32);
    unsigned short nAngle = ReadWORD(12);
    unsigned char nA = ReadBYTE(8);
    unsigned char nB = ReadBYTE(8);
    if (MessageReadOverflow(1) || MessageReadUnderflow(1)) {
        return 0;
    }
    CGameObject* pObject = pPlayer->GetGameObject();
    if (!pObject) {
        return 0;
    }
    Vector vPosition(fX, fY, 0.0f);
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    if (pCreature) {
        if (pCreature->m_n9DC) {
            return 0;
        }
        if (pCreature->m_bCommandable == 1) {
            pCreature->SetCombatState(0, 1);
            pCreature->m_pCombatRound->ClearAllSpecialAttacks();
            pCreature->ClearAllActions(1);
            pCreature->ClearActivityFlag(8);
            pCreature->AddDriveAction(0xffff, &vPosition, nAngle, nA, nB, 2);
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x005246F0
int CSWSMessage::HandlePlayerToServerCharListMessage(CSWSPlayer* pPlayer, unsigned char nMinor)
{
    CResRef cResRef;
    if (MessageReadOverflow(1)) {
        return 0;
    }
    switch (nMinor) {
    case 1:
        if (MessageReadUnderflow(1)) {
            return 0;
        }
        if (g_pAppManager->GetServerExoApp()->GetServerMode() != 1 &&
            g_pAppManager->GetServerExoApp()->GetServerMode() != 2) {
            g_pAppManager->GetServerExoApp()->AddCharListRequest(pPlayer->m_nPlayerID);
            return 1;
        }
        return SendServerToPlayerCharList(pPlayer);
    case 3: {
        unsigned char nIndex = ReadBYTE(8);
        cResRef = ReadCResRef(16);
        if (MessageReadUnderflow(1)) {
            return 0;
        }
        return HandleCharListCharacter(pPlayer, nIndex, cResRef);
    }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00524C70
int CSWSMessage::HandlePlayerToServerMapPinSetMapPinAt(CSWSPlayer* pPlayer)
{
    Vector vPosition;
    vPosition.x = 0.0f;
    vPosition.y = 0.0f;
    vPosition.z = 0.0f;
    CExoString sNote;
    CExoString sVarName;
    if (MessageReadOverflow(1)) {
        return 0;
    }
    vPosition.x = ReadFLOAT(1.0f, 32);
    vPosition.y = ReadFLOAT(1.0f, 32);
    vPosition.z = ReadFLOAT(1.0f, 32);
    sNote = ReadCExoString(32);
    if (MessageReadUnderflow(1)) {
        return 0;
    }
    CGameObject* pObject = pPlayer->GetGameObject();
    if (!pObject) {
        return 0;
    }
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    if (!pCreature) {
        return 0;
    }
    CSWSScriptVarTable* pVars = &pCreature->m_ScriptVars;
    if (!pVars) {
        return 0;
    }
    CExoString sTotal("NW_TOTAL_MAP_PINS");
    int nPin = pVars->GetInt(sTotal) + 1;
    pVars->SetInt(sTotal, nPin, 0);
    sVarName.Format("NW_MAP_PIN_NTRY_%i", nPin);
    pVars->SetString(sVarName, sNote);
    sVarName.Format("NW_MAP_PIN_XPOS_%i", nPin);
    pVars->SetFloat(sVarName, vPosition.x);
    sVarName.Format("NW_MAP_PIN_YPOS_%i", nPin);
    pVars->SetFloat(sVarName, vPosition.y);
    sVarName.Format("NW_MAP_PIN_ZPOS_%i", nPin);
    pVars->SetFloat(sVarName, vPosition.z);
    sVarName.Format("NW_MAP_PIN_AREA_%i", nPin);
    pVars->SetObject(sVarName, pCreature->GetArea()->GetId());
    SendServerToPlayerMapPinAdded(pPlayer, vPosition, sNote, nPin);
    return 1;
}

// FUNCTION: SWKOTOR 0x00524F10
int CSWSMessage::HandlePlayerToServerMapPinDestroyMapPin(CSWSPlayer* pPlayer)
{
    CExoString sVarName;
    if (MessageReadOverflow(1)) {
        return 0;
    }
    int nPin = ReadDWORD(32);
    if (MessageReadUnderflow(1)) {
        return 0;
    }
    CGameObject* pObject = pPlayer->GetGameObject();
    if (!pObject) {
        return 0;
    }
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    if (!pCreature) {
        return 0;
    }
    CSWSScriptVarTable* pVars = &pCreature->m_ScriptVars;
    if (!pVars) {
        return 0;
    }
    sVarName.Format("NW_MAP_PIN_NTRY_%i", nPin);
    pVars->DestroyString(sVarName);
    sVarName.Format("NW_MAP_PIN_XPOS_%i", nPin);
    pVars->DestroyFloat(sVarName);
    sVarName.Format("NW_MAP_PIN_YPOS_%i", nPin);
    pVars->DestroyFloat(sVarName);
    sVarName.Format("NW_MAP_PIN_ZPOS_%i", nPin);
    pVars->DestroyFloat(sVarName);
    sVarName.Format("NW_MAP_PIN_AREA_%i", nPin);
    pVars->DestroyObject(sVarName);
    return 1;
}

// FUNCTION: SWKOTOR 0x00525080
int CSWSMessage::HandlePlayerToServerMapPinChangePin(CSWSPlayer* pPlayer)
{
    Vector vPosition;
    CExoString sNote;
    CExoString sVarName;
    if (MessageReadOverflow(1)) {
        return 0;
    }
    vPosition.x = ReadFLOAT(1.0f, 32);
    vPosition.y = ReadFLOAT(1.0f, 32);
    vPosition.z = ReadFLOAT(1.0f, 32);
    sNote = ReadCExoString(32);
    int nPin = ReadDWORD(32);
    if (MessageReadUnderflow(1)) {
        return 0;
    }
    CGameObject* pObject = pPlayer->GetGameObject();
    if (!pObject) {
        return 0;
    }
    CSWSCreature* pCreature = pObject->AsSWSCreature();
    if (!pCreature) {
        return 0;
    }
    CSWSScriptVarTable* pVars = &pCreature->m_ScriptVars;
    if (!pVars) {
        return 0;
    }
    sVarName.Format("NW_MAP_PIN_NTRY_%i", nPin);
    pVars->SetString(sVarName, sNote);
    sVarName.Format("NW_MAP_PIN_XPOS_%i", nPin);
    pVars->SetFloat(sVarName, vPosition.x);
    sVarName.Format("NW_MAP_PIN_YPOS_%i", nPin);
    pVars->SetFloat(sVarName, vPosition.y);
    sVarName.Format("NW_MAP_PIN_ZPOS_%i", nPin);
    pVars->SetFloat(sVarName, vPosition.z);
    sVarName.Format("NW_MAP_PIN_AREA_%i", nPin);
    pVars->SetObject(sVarName, pCreature->GetArea()->GetId());
    return 1;
}
