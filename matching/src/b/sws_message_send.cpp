// CSWSMessage small SendServerToPlayer* writers (0x00567E60-0x0056B610).
#include "b/sws_message.h"
#include "b/sws_player.h"
#include "b/sws_creature.h"
#include "b/sws_item.h"
#include "b/faction_manager.h"

class CSWSDoor : public CSWSObject {
public:
    char pad_0xf4[0x3a4 - 0xf4];
    CExoLocString m_sDescription;                   // 0x3a4
};

class CGameObjectArray;
// CGameObjectArray lookup result (.rdata 0x0074666C, 0 = success).
extern const unsigned char GAME_OBJECT_ARRAY_SUCCESS;

// Panel ids compared from .rdata (0x00747E8C, 0x00747E90).
extern const int GUI_PANEL_POPUP_A;
extern const int GUI_PANEL_POPUP_B;

// Visual effect ids that carry a beam source/origin (0x004CA960, cdecl).
int IsBeamVisualEffect(unsigned short nEffect);

// Custom dialog token (<CUSTOMn>), 0xC bytes.
struct CTlkTableTokenCustom {
    int m_nNumber;                                  // 0x0
    CExoString m_sValue;                            // 0x4
};

class CTlkTable {
public:
    char pad_0x0[0x48];
    CTlkTableTokenCustom* m_pTokensCustom;          // 0x48
    unsigned long m_nTokensCustom;                  // 0x4c
};

extern CTlkTable* g_pTlkTable;                      // 0x007A3A08
// Tokens up to this number are engine tokens, not sent (.rdata 0x0073ECCC).
extern const int TLK_CUSTOM_TOKEN_FIRST;

// FUNCTION: SWKOTOR 0x00567E60
int CSWSMessage::SendServerToPlayerMessage(unsigned long nPlayerId, unsigned char nMajor,
                                           unsigned char nMinor, unsigned char* pData,
                                           unsigned long nSize)
{
    unsigned char* pMessage;
    unsigned long nMessageSize;
    if (nSize) {
        pMessage = pData;
        nMessageSize = nSize;
    }
    else {
        nMessageSize = 3;
        pMessage = new unsigned char[3];
        m_nPlayerId = 0xffffffff;
    }
    if (!pMessage) {
        return 0;
    }
    pMessage[0] = 'P';
    pMessage[1] = nMajor;
    pMessage[2] = nMinor;
    int bResult = g_pAppManager->GetServerExoApp()->GetNetLayer()->SendMessageToPlayer(
        nPlayerId, pMessage, nMessageSize, 0);
    if (!nSize) {
        delete[] pMessage;
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x00569B80
int CSWSMessage::SendServerToPlayerCheatNasty(unsigned long nPlayerId, unsigned long oidTarget)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(4, 0xffffffff, 1);
    WriteOBJECTIDServer(oidTarget);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 0xf, 0xb, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x00569BE0
int CSWSMessage::SendServerToPlayerCheatDebugMode(int bDebugMode)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(4, 0xffffffff, 1);
    WriteBOOL(bDebugMode);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(PLAYER_ID_ALL_PLAYERS, 0xf, 0x15, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x00569EC0
int CSWSMessage::SendServerToPlayerVoiceChat_Play(CSWSPlayer* pPlayer, unsigned long oidSpeaker,
                                                  unsigned char nVoiceChat)
{
    unsigned char* pMessage;
    unsigned long nSize = 5;
    CreateWriteMessage(nSize, 0xffffffff, 1);
    WriteOBJECTIDServer(oidSpeaker);
    WriteBYTE(nVoiceChat, 8);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(pPlayer->GetPlayerId(), 0x1a, 1, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x00569F40
int CSWSMessage::SendServerToPlayerGuiContainerObject_Open(CSWSPlayer* pPlayer,
                                                           unsigned long oidContainer, int nArg)
{
    unsigned char* pMessage;
    unsigned long nSize = 8;
    CreateWriteMessage(nSize, 0xffffffff, 1);
    WriteOBJECTIDServer(oidContainer);
    WriteINT(nArg, 0x20);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(pPlayer->GetPlayerId(), 0x19, 1, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x00569FC0
int CSWSMessage::SendServerPlayerItemUpdate_DestroyItem(CSWSPlayer* pPlayer, unsigned long oidItem)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(4, 0xffffffff, 1);
    WriteOBJECTIDServer(oidItem);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(pPlayer->GetPlayerId(), 5, 7, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056A020
int CSWSMessage::SendServerToPlayerJournalAddQuest(CSWSPlayer* pPlayer, const CExoString& sTag,
                                                   int nState, int nPriority,
                                                   unsigned short nPlanetId, int bComplete,
                                                   int nCalendarDay, int nTimeOfDay,
                                                   const CExoLocString& sName,
                                                   const CExoLocString& sText, int nXP,
                                                   int nEntryId, int nPlotIndex)
{
    unsigned char* pMessage;
    unsigned long nSize = sTag.GetLength() + 0x16;
    CreateWriteMessage(nSize, 0xffffffff, 1);
    WriteCExoLocStringServer(sName, 0);
    WriteCExoLocStringServer(sText, 0);
    WriteINT(nCalendarDay, 0x20);
    WriteINT(nTimeOfDay, 0x20);
    WriteCExoString(sTag, 0x20);
    WriteINT(nState, 0x20);
    WriteINT(nPriority, 0x20);
    WriteWORD(nPlanetId, 0x10);
    WriteBOOL(bComplete);
    WriteINT(nXP, 0x20);
    WriteINT(nEntryId, 0x20);
    WriteINT(nPlotIndex, 0x20);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(pPlayer->GetPlayerId(), 0x1c, 6, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056A130
int CSWSMessage::SendServerToPlayerJournalRemoveQuest(CSWSPlayer* pPlayer, const CExoString& sTag)
{
    unsigned char* pMessage;
    unsigned long nSize = sTag.GetLength();
    CreateWriteMessage(nSize, 0xffffffff, 1);
    WriteCExoString(sTag, 0x20);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(pPlayer->GetPlayerId(), 0x1c, 7, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056A1A0
int CSWSMessage::SendServerToPlayerJournalAddWorld(CSWSPlayer* pPlayer, int nIndex,
                                                   const CExoString& sName,
                                                   const CExoString& sText,
                                                   unsigned long nCalendarDay,
                                                   unsigned long nTimeOfDay)
{
    unsigned char* pMessage;
    unsigned long nSize = sName.GetLength() + sText.GetLength() + 0xc;
    CreateWriteMessage(nSize, 0xffffffff, 1);
    WriteINT(nIndex, 0x20);
    WriteCExoString(sName, 0x20);
    WriteCExoString(sText, 0x20);
    WriteINT(nCalendarDay, 0x20);
    WriteINT(nTimeOfDay, 0x20);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(pPlayer->GetPlayerId(), 0x1c, 1, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056A260
int CSWSMessage::SendServerToPlayerJournalAddWorldStrref(CSWSPlayer* pPlayer, unsigned long nName,
                                                         unsigned long nText,
                                                         unsigned long nCalendarDay,
                                                         unsigned long nTimeOfDay)
{
    unsigned char* pMessage;
    unsigned long nSize = 0x10;
    CreateWriteMessage(nSize, 0xffffffff, 1);
    WriteINT(nName, 0x20);
    WriteINT(nText, 0x20);
    WriteINT(nCalendarDay, 0x20);
    WriteINT(nTimeOfDay, 0x20);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(pPlayer->GetPlayerId(), 0x1c, 2, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056A300
int CSWSMessage::SendServerToPlayerJournalDeleteWorld(CSWSPlayer* pPlayer, int nIndex)
{
    unsigned char* pMessage;
    unsigned long nSize = 4;
    CreateWriteMessage(nSize, 0xffffffff, 1);
    WriteINT(nIndex, 0x20);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(pPlayer->GetPlayerId(), 0x1c, 3, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056A370
int CSWSMessage::SendServerToPlayerJournalDeleteWorldStrref(CSWSPlayer* pPlayer,
                                                            unsigned long nStrRef)
{
    unsigned char* pMessage;
    unsigned long nSize = 4;
    CreateWriteMessage(nSize, 0xffffffff, 1);
    WriteINT(nStrRef, 0x20);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(pPlayer->GetPlayerId(), 0x1c, 4, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056A3E0
int CSWSMessage::SendServerToPlayerJournalDeleteWorldAll(CSWSPlayer* pPlayer)
{
    unsigned char* pMessage;
    unsigned long nSize = 1;
    CreateWriteMessage(nSize, 0xffffffff, 1);
    WriteBYTE(1, 8);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(pPlayer->GetPlayerId(), 0x1c, 5, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056A450
int CSWSMessage::SendServerToPlayerJournalFullUpdateNotNeeded(CSWSPlayer* pPlayer, int bNeeded)
{
    unsigned char* pMessage;
    unsigned long nSize;
    if (bNeeded) {
        CreateWriteMessage(0xc, 0xffffffff, 1);
        WriteBOOL(1);
        WriteBOOL(1);
        WriteINT(0, 0x20);
    }
    else {
        CreateWriteMessage(4, 0xffffffff, 1);
        WriteBOOL(0);
    }
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(pPlayer->GetPlayerId(), 0x1c, 9, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056A4E0
int CSWSMessage::SendServerToPlayerJournalUpdated(CSWSPlayer* pPlayer, int bQuest, int bCompleted,
                                                  const CExoLocString& sName)
{
    unsigned char* pMessage;
    unsigned long nSize = 8;
    CreateWriteMessage(nSize, 0xffffffff, 1);
    WriteCExoLocStringServer(sName, 0);
    WriteBOOL(bQuest);
    WriteBOOL(bCompleted);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(pPlayer->GetPlayerId(), 0x1c, 0xc, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056A560
int CSWSMessage::SendServerToPlayerLevelUp_Confirmation(unsigned long nPlayerId, int bSuccess)
{
    if (bSuccess) {
        return SendServerToPlayerMessage(nPlayerId, 0x1d, 1, NULL, 0);
    }
    return SendServerToPlayerMessage(nPlayerId, 0x1d, 2, NULL, 0);
}

// FUNCTION: SWKOTOR 0x0056A5F0
int CSWSMessage::SendServerToPlayerModuleUpdate_Time(CSWSPlayer* pPlayer, unsigned char nFlags,
                                                     unsigned long nMillisecond,
                                                     unsigned char nSecond, unsigned char nMinute,
                                                     unsigned char nHour,
                                                     unsigned char nTransitionType,
                                                     unsigned long nTransitionTime)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(0xd, 0xffffffff, 1);
    WriteBYTE(nFlags, 8);
    if (nFlags & 1) {
        WriteBYTE(nTransitionType, 8);
        if (nTransitionType == 3 || nTransitionType == 4) {
            WriteINT(nTransitionTime, 0x20);
        }
    }
    if (nFlags & 2) {
        WriteBYTE(nHour, 8);
    }
    if (nFlags & 4) {
        WriteBYTE(nMinute, 8);
    }
    if (nFlags & 8) {
        WriteBYTE(nSecond, 8);
    }
    if (nFlags & 0x10) {
        WriteINT(nMillisecond, 0x20);
    }
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(pPlayer->GetPlayerId(), 3, 3, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056A6D0
int CSWSMessage::SendServerToPlayerMapPinEnabled(CSWSPlayer* pPlayer, unsigned long oidPin,
                                                 int bEnabled)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(8, 0xffffffff, 1);
    WriteOBJECTIDServer(oidPin);
    WriteBOOL(bEnabled);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(pPlayer->GetPlayerId(), 0x20, 1, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056A740
int CSWSMessage::SendServerToPlayerMapPinAdded(CSWSPlayer* pPlayer, Vector vPosition,
                                               const CExoString& sNote, unsigned long nId)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(sNote.GetLength() + 0x10, 0xffffffff, 1);
    WriteFLOAT(vPosition.x, 1.0f, 0x20);
    WriteFLOAT(vPosition.y, 1.0f, 0x20);
    WriteFLOAT(vPosition.z, 1.0f, 0x20);
    WriteCExoString(sNote, 0x20);
    WriteINT(nId, 0x20);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(pPlayer->GetPlayerId(), 0x20, 4, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056AD60
int CSWSMessage::SendServerToPlayerAmbientMusicPlay(unsigned long nPlayerId, int bPlay)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(4, 0xffffffff, 1);
    WriteBOOL(bPlay);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 0x28, 1, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056ADC0
int CSWSMessage::SendServerToPlayerAmbientMusicSetDelay(unsigned long nPlayerId, int nDelay)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(4, 0xffffffff, 1);
    WriteINT(nDelay, 0x20);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 0x28, 2, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056AE20
int CSWSMessage::SendServerToPlayerAmbientMusicChangeTrack(unsigned long nPlayerId, int bDay,
                                                           int nTrack)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(8, 0xffffffff, 1);
    WriteBOOL(bDay);
    WriteINT(nTrack, 0x20);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 0x28, 3, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056AE90
int CSWSMessage::SendServerToPlayerAmbientBattleMusicPlay(unsigned long nPlayerId, int bPlay)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(4, 0xffffffff, 1);
    WriteBOOL(bPlay);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 0x28, 4, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056AEF0
int CSWSMessage::SendServerToPlayerAmbientBattleMusicChange(unsigned long nPlayerId, int nTrack)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(4, 0xffffffff, 1);
    WriteINT(nTrack, 0x20);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 0x28, 5, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056AF50
int CSWSMessage::SendServerToPlayerAmbientSoundLoopPlay(unsigned long nPlayerId, int bPlay)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(4, 0xffffffff, 1);
    WriteBOOL(bPlay);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 0x28, 6, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056AFB0
int CSWSMessage::SendServerToPlayerAmbientSoundLoopChange(unsigned long nPlayerId, int bDay,
                                                          int nSound)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(8, 0xffffffff, 1);
    WriteBOOL(bDay);
    WriteINT(nSound, 0x20);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 0x28, 7, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056B020
int CSWSMessage::SendServerToPlayerAmbientSoundVolumeChange(unsigned long nPlayerId, int bDay,
                                                            int nVolume)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(8, 0xffffffff, 1);
    WriteBOOL(bDay);
    WriteINT(nVolume, 0x20);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 0x28, 8, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056B240
int CSWSMessage::SendServerToPlayerInventory_LearnScroll(unsigned long nPlayerId,
                                                         unsigned long oidScroll,
                                                         unsigned char nMinor)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(4, 0xffffffff, 1);
    WriteOBJECTIDServer(oidScroll);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    SendServerToPlayerMessage(nPlayerId, 0xc, nMinor, pMessage, nSize);
    return 1;
}

// FUNCTION: SWKOTOR 0x0056B2B0
int CSWSMessage::SendServerToPlayerShutDownServer(unsigned long nPlayerId, int nReason)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(4, 0xffffffff, 1);
    WriteINT(nReason, 0x20);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    SendServerToPlayerMessage(nPlayerId, 0x2f, 0, pMessage, nSize);
    return 1;
}

// FUNCTION: SWKOTOR 0x0056B320
int CSWSMessage::SendServerToPlayerDestroyDeathGUI(unsigned long nPlayerId)
{
    SendServerToPlayerMessage(nPlayerId, 0x24, 2, NULL, 0);
    return 1;
}

// FUNCTION: SWKOTOR 0x0056B380
int CSWSMessage::SendServerToPlayerGuiTimingEvent(CSWSPlayer* pPlayer, int bStart,
                                                  unsigned char nEventId, int nDuration)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(8, 0xffffffff, 1);
    WriteBOOL(bStart);
    if (bStart) {
        WriteINT(nDuration, 0x20);
        WriteBYTE(nEventId, 8);
    }
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(pPlayer->GetPlayerId(), 0x30, 1, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056B610
int CSWSMessage::SendServerToPlayerGUICharacterSheet_NotPermitted(unsigned long nPlayerId,
                                                                  unsigned long oidCreature)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(0x80, 0xffffffff, 1);
    WriteOBJECTIDServer(oidCreature);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 0x15, 2, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056C610
int CSWSMessage::SendServerToPlayerCamera_ChangeLocation(CSWSPlayer* pPlayer, unsigned char nFlags,
                                                         float fDistance, float fPitch,
                                                         float fFacing, unsigned char nSpeed)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(0x100, 0xffffffff, 1);
    WriteBYTE(nFlags, 8);
    if (nFlags & 1) {
        WriteFLOAT(fDistance, 1.0f, 0x20);
    }
    if (nFlags & 2) {
        WriteFLOAT(fPitch, 1.0f, 0x20);
    }
    if (nFlags & 4) {
        WriteFLOAT(fFacing, 1.0f, 0x20);
    }
    if (nFlags & 8) {
        WriteBYTE(nSpeed, 8);
    }
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(pPlayer->GetPlayerId(), 0x10, 1, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056C6D0
int CSWSMessage::SendServerToPlayerCamera_SetMode(CSWSPlayer* pPlayer, unsigned char nMode)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(1, 0xffffffff, 1);
    WriteBYTE(nMode, 8);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(pPlayer->GetPlayerId(), 0x10, 2, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056C730
int CSWSMessage::SendServerToPlayerLogin_CharacterQuery(CSWSPlayer* pPlayer,
                                                        unsigned char* pClassCount, int* pClasses,
                                                        unsigned char* pLevels,
                                                        unsigned long* pExperience)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(0x80, 0xffffffff, 1);
    WriteBYTE(*pClassCount, 8);
    for (unsigned char i = 0; i < *pClassCount; i++) {
        WriteINT(pClasses[i], 0x20);
        WriteBYTE(pLevels[i], 8);
    }
    WriteINT(*pExperience, 0x20);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(pPlayer->GetPlayerId(), 2, 0xa, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056C7F0
int CSWSMessage::SendServerToPlayerLogin_NeedCharacter(unsigned long nPlayerId)
{
    return SendServerToPlayerMessage(nPlayerId, 2, 0x10, NULL, 0);
}

// FUNCTION: SWKOTOR 0x0056C850
int CSWSMessage::SendServerToPlayerModule_SaveGameStatus(unsigned char nStatus)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(1, 0xffffffff, 1);
    WriteBYTE(nStatus, 8);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(PLAYER_ID_ALL_PLAYERS, 3, 0xd, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056C8B0
int CSWSMessage::SendServerToPlayerLoadBar_StartStallEvent(unsigned long nEvent)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(4, 0xffffffff, 1);
    WriteINT(nEvent, 0x20);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(PLAYER_ID_ALL_PLAYERS, 0x2c, 1, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056C910
int CSWSMessage::SendServerToPlayerLoadBar_UpdateStallEvent(unsigned long nEvent,
                                                            unsigned long nProgress)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(4, 0xffffffff, 1);
    WriteINT(nEvent, 0x20);
    WriteINT(nProgress, 0x20);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(PLAYER_ID_ALL_PLAYERS, 0x2c, 2, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056C980
int CSWSMessage::SendServerToPlayerLoadBar_EndStallEvent(unsigned long nEvent,
                                                         unsigned long nError)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(4, 0xffffffff, 1);
    WriteINT(nEvent, 0x20);
    WriteINT(nError, 3);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(PLAYER_ID_ALL_PLAYERS, 0x2c, 3, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056CAC0
int CSWSMessage::SendServerToPlayerModule_ExportReply(CSWSPlayer* pPlayer)
{
    unsigned char* pMessage;
    unsigned long nSize;
    pPlayer->PackCreatureIntoMessage();
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(pPlayer->GetPlayerId(), 3, 0xa, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056CB10
int CSWSMessage::SendServerToPlayerModule_Loading(CSWSPlayer* pPlayer)
{
    return SendServerToPlayerMessage(pPlayer->GetPlayerId(), 3, 6, NULL, 0);
}

// FUNCTION: SWKOTOR 0x0056CB80
int CSWSMessage::SendServerToPlayerSaveLoad_Status(CSWSPlayer* pPlayer, unsigned char nStatus,
                                                   int nError)
{
    unsigned char* pMessage;
    unsigned long nSize;
    if (!pPlayer) {
        return 0;
    }
    CreateWriteMessage(4, 0xffffffff, 1);
    WriteBYTE(nStatus, 8);
    WriteINT(nError, 4);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(pPlayer->GetPlayerId(), 0x2d, 1, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056CBF0
int CSWSMessage::SendServerToPlayerModule_StartStartNewModule()
{
    return SendServerToPlayerMessage(PLAYER_ID_ALL_PLAYERS, 3, 0xb, NULL, 0);
}

// FUNCTION: SWKOTOR 0x0056CC50
int CSWSMessage::SendServerToPlayerModule_EndStartNewModule(int bSuccess)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(1, 0xffffffff, 1);
    WriteBOOL(bSuccess);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(PLAYER_ID_ALL_PLAYERS, 3, 0xc, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056CCB0
int CSWSMessage::SendServerToPlayerModule_DumpPlayer(CSWSPlayer* pPlayer)
{
    return SendServerToPlayerMessage(pPlayer->GetPlayerId(), 3, 8, NULL, 0);
}

// FUNCTION: SWKOTOR 0x0056D140
int CSWSMessage::SendServerToPlayerArea_VisualEffect(CSWSPlayer* pPlayer, unsigned short nEffect,
                                                     float fX, float fY, float fZ)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(0xe, 0xffffffff, 1);
    WriteWORD(nEffect, 0x10);
    WriteFLOAT(fX, 1.0f, 0x20);
    WriteFLOAT(fY, 1.0f, 0x20);
    WriteFLOAT(fZ, 1.0f, 0x20);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    unsigned long nPlayerId = pPlayer->m_nPlayerID;
    return SendServerToPlayerMessage(nPlayerId, 4, 2, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056D1E0
int CSWSMessage::SendServerToPlayerArea_Weather(CSWSPlayer* pPlayer, unsigned char nWeather,
                                                int bEnabled)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(5, 0xffffffff, 1);
    WriteBYTE(nWeather, 8);
    WriteBOOL(bEnabled);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(pPlayer->GetPlayerId(), 4, 4, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056D250
int CSWSMessage::SendServerToPlayerSetCustomToken(unsigned long nPlayerId, int nToken,
                                                  const CExoString& sValue)
{
    unsigned char* pMessage;
    unsigned long nSize = sValue.GetLength() + 8;
    CreateWriteMessage(nSize, nPlayerId, 1);
    WriteINT(nToken, 0x20);
    WriteCExoString(sValue, 0x20);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 0x32, 1, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056D2D0
int CSWSMessage::SendServerToPlayerSetCustomTokenList(unsigned long nPlayerId)
{
    unsigned char* pMessage;
    unsigned long nSize;
    unsigned long nTokens = g_pTlkTable->m_nTokensCustom;
    unsigned long nCount = nTokens;
    unsigned long i;
    for (i = 0; i < nTokens; i++) {
        if (g_pTlkTable->m_pTokensCustom[i].m_nNumber <= TLK_CUSTOM_TOKEN_FIRST) {
            nCount--;
        }
    }
    if (!nCount) {
        return 1;
    }
    nSize = nCount * 8 + 4;
    CreateWriteMessage(nSize, nPlayerId, 1);
    WriteINT(nCount, 0x20);
    for (i = 0; i < nTokens; i++) {
        if (g_pTlkTable->m_pTokensCustom[i].m_nNumber > TLK_CUSTOM_TOKEN_FIRST) {
            WriteINT(g_pTlkTable->m_pTokensCustom[i].m_nNumber, 0x20);
            WriteCExoString(g_pTlkTable->m_pTokensCustom[i].m_sValue, 0x20);
        }
    }
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 0x32, 2, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056D3C0
// NONMATCH: 18 diff insns. The original keeps nChatType in its argument slot
// (byte store + dword reload for the minor id) and gives nSize a real local;
// ours enregisters the new minor id and drops the push ecx frame.
int CSWSMessage::SendServerToPlayerChatMultiLang_Helper(
    unsigned long nPlayerId, unsigned char nChatType, unsigned long oidSpeaker,
    const CExoLocString& sMessage, unsigned long oidTarget, unsigned char nGender,
    int bPrivate, const CResRef& cSound, unsigned long oidSound, int nValue)
{
    unsigned char* pMessage;
    unsigned long nSize;
    if (nChatType != 8) {
        return 0;
    }
    nSize = 0x28;
    CreateWriteMessage(nSize, nPlayerId, 1);
    WriteOBJECTIDServer(oidSpeaker);
    WriteOBJECTIDServer(oidTarget);
    WriteCExoLocStringServer(sMessage, nGender);
    WriteCResRef(cSound, 0x10);
    WriteOBJECTIDServer(oidSound);
    WriteINT(nValue, 0x20);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    nChatType = (bPrivate != 0) + 0xb;
    return SendServerToPlayerMessage(nPlayerId, 9, nChatType, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056D4A0
int CSWSMessage::SendServerToPlayerAIActionPlaySound(unsigned long nPlayerId,
                                                     unsigned long oidSource,
                                                     const CExoString& sSound)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(sSound.GetLength() + 8, 0xffffffff, 1);
    WriteOBJECTIDServer(oidSource);
    WriteCExoString(sSound, 0x20);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 9, 7, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056D510
int CSWSMessage::SendServerToPlayerSoundObject_Play(CSWSPlayer* pPlayer, unsigned long oidSound)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(4, 0xffffffff, 1);
    WriteOBJECTIDServer(oidSound);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    unsigned long nPlayerId = pPlayer->m_nPlayerID;
    return SendServerToPlayerMessage(nPlayerId, 0x17, 2, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056D570
int CSWSMessage::SendServerToPlayerSoundObject_Stop(CSWSPlayer* pPlayer, unsigned long oidSound)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(4, 0xffffffff, 1);
    WriteOBJECTIDServer(oidSound);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    unsigned long nPlayerId = pPlayer->m_nPlayerID;
    return SendServerToPlayerMessage(nPlayerId, 0x17, 3, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056D5D0
int CSWSMessage::SendServerToPlayerSoundObject_ChangePosition(CSWSPlayer* pPlayer,
                                                              unsigned long oidSound,
                                                              float fX, float fY, float fZ)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(0x10, 0xffffffff, 1);
    WriteOBJECTIDServer(oidSound);
    WriteFLOAT(fX, 1.0f, 0x20);
    WriteFLOAT(fY, 1.0f, 0x20);
    WriteFLOAT(fZ, 1.0f, 0x20);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    unsigned long nPlayerId = pPlayer->m_nPlayerID;
    return SendServerToPlayerMessage(nPlayerId, 0x17, 5, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056D670
int CSWSMessage::SendServerToPlayerChat_StrRef(unsigned long nPlayerId, unsigned long oidSpeaker,
                                               unsigned char nChatType, unsigned long nStrRef)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(8, 0xffffffff, 1);
    WriteOBJECTIDServer(oidSpeaker);
    WriteINT(nStrRef, 0x20);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 9, nChatType, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056D6E0
int CSWSMessage::SendServerToPlayerChat_Party(unsigned long nPlayerId, unsigned long oidSpeaker,
                                              const CExoString& sMessage)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CSWSCreature* pCreature =
        g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidSpeaker);
    if (!pCreature) {
        return 0;
    }
    CreateWriteMessage(sMessage.GetLength() + 8, 0xffffffff, 1);
    WriteOBJECTIDServer(oidSpeaker);
    WriteCExoString(sMessage, 0x20);
    WriteFLOAT(pCreature->m_vPosition.x, 1.0f, 0x20);
    WriteFLOAT(pCreature->m_vPosition.y, 1.0f, 0x20);
    WriteFLOAT(pCreature->m_vPosition.z, 1.0f, 0x20);
    WriteCExoLocStringServer(*pCreature->GetFirstName(), 0);
    WriteCExoLocStringServer(*pCreature->GetLastName(), 0);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 9, 6, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056D7E0
int CSWSMessage::SendServerToPlayerChat_Talk(unsigned long nPlayerId, unsigned long oidSpeaker,
                                             const CExoString& sMessage)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(sMessage.GetLength() + 8, 0xffffffff, 1);
    WriteOBJECTIDServer(oidSpeaker);
    WriteCExoString(sMessage, 0x20);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 9, 1, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056D850
int CSWSMessage::SendServerToPlayerQuickChat(unsigned long nPlayerId, unsigned long oidSpeaker,
                                             unsigned short nQuickChat)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(0xa, 0xffffffff, 1);
    WriteOBJECTIDServer(oidSpeaker);
    WriteWORD(nQuickChat, 0x10);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 0x16, 0, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056D8C0
int CSWSMessage::SendServerToPlayerChat_Tell(unsigned long nPlayerId, unsigned long oidSpeaker,
                                             const CExoString& sMessage)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CSWSCreature* pCreature =
        g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidSpeaker);
    if (!pCreature) {
        return 0;
    }
    CreateWriteMessage(sMessage.GetLength() + 0x14, 0xffffffff, 1);
    WriteOBJECTIDServer(oidSpeaker);
    WriteCExoString(sMessage, 0x20);
    WriteFLOAT(pCreature->m_vPosition.x, 1.0f, 0x20);
    WriteFLOAT(pCreature->m_vPosition.y, 1.0f, 0x20);
    WriteFLOAT(pCreature->m_vPosition.z, 1.0f, 0x20);
    WriteCExoLocStringServer(*pCreature->GetFirstName(), 0);
    WriteCExoLocStringServer(*pCreature->GetLastName(), 0);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 9, 4, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056D9C0
int CSWSMessage::SendServerToPlayerChat_ServerTell(unsigned long nPlayerId,
                                                   const CExoString& sMessage)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(sMessage.GetLength() + 4, 0xffffffff, 1);
    WriteCExoString(sMessage, 0x20);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 9, 5, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056DA30
int CSWSMessage::SendServerToPlayerChat_Shout(unsigned long nPlayerId, unsigned long oidSpeaker,
                                              const CExoString& sMessage)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CSWSCreature* pCreature =
        g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidSpeaker);
    CreateWriteMessage(sMessage.GetLength() + 0x14, 0xffffffff, 1);
    if (pCreature) {
        WriteOBJECTIDServer(oidSpeaker);
        WriteCExoString(sMessage, 0x20);
        WriteFLOAT(pCreature->m_vPosition.x, 1.0f, 0x20);
        WriteFLOAT(pCreature->m_vPosition.y, 1.0f, 0x20);
        WriteFLOAT(pCreature->m_vPosition.z, 1.0f, 0x20);
        WriteCExoLocStringServer(*pCreature->GetFirstName(), 0);
        WriteCExoLocStringServer(*pCreature->GetLastName(), 0);
    }
    else {
        CExoLocString sFirstName;
        CExoLocString sLastName;
        sFirstName.m_nStrRef = 0x10782;
        WriteOBJECTIDServer(INVALID_OBJECT_ID);
        WriteCExoString(sMessage, 0x20);
        WriteFLOAT(0.0f, 1.0f, 0x20);
        WriteFLOAT(0.0f, 1.0f, 0x20);
        WriteFLOAT(0.0f, 1.0f, 0x20);
        WriteCExoLocStringServer(sFirstName, 0);
        WriteCExoLocStringServer(sLastName, 0);
    }
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 9, 2, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056DBF0
int CSWSMessage::SendServerToPlayerChat_Whisper(unsigned long nPlayerId, unsigned long oidSpeaker,
                                                const CExoString& sMessage)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(sMessage.GetLength() + 8, 0xffffffff, 1);
    WriteOBJECTIDServer(oidSpeaker);
    WriteCExoString(sMessage, 0x20);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 9, 3, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056FB10
int CSWSMessage::SendServerToPlayerInventory_Drop(unsigned long nPlayerId, unsigned long oidItem)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(4, 0xffffffff, 1);
    WriteOBJECTIDServer(oidItem);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 0xc, 3, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056FB70
int CSWSMessage::SendServerToPlayerInventory_DropCancel(unsigned long nPlayerId,
                                                        unsigned long oidItem)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(4, 0xffffffff, 1);
    WriteOBJECTIDServer(oidItem);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 0xc, 4, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056FBD0
int CSWSMessage::SendServerToPlayerInventory_EquipCancel(unsigned long nPlayerId,
                                                         unsigned long oidItem, int nSlot)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(8, 0xffffffff, 1);
    WriteOBJECTIDServer(oidItem);
    WriteINT(nSlot, 0x20);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    SendServerToPlayerMessage(nPlayerId, 0xc, 2, pMessage, nSize);
    g_pAppManager->GetServerExoApp()->GetClientObjectByPlayerID(nPlayerId, 0);
    return 1;
}

// FUNCTION: SWKOTOR 0x0056FC50
int CSWSMessage::SendServerToPlayerInventory_Pickup(unsigned long nPlayerId,
                                                    unsigned long oidItem)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(4, 0xffffffff, 1);
    WriteOBJECTIDServer(oidItem);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 0xc, 5, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056FCB0
int CSWSMessage::SendServerToPlayerInventory_PickupCancel(unsigned long nPlayerId,
                                                          unsigned long oidItem)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(4, 0xffffffff, 1);
    WriteOBJECTIDServer(oidItem);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 0xc, 6, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056FD10
int CSWSMessage::SendServerToPlayerInventory_Unequip(unsigned long nPlayerId,
                                                     unsigned long oidItem, int bUnequip)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(8, 0xffffffff, 1);
    WriteOBJECTIDServer(oidItem);
    WriteBOOL(bUnequip);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 0xc, 7, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056FD80
int CSWSMessage::SendServerToPlayerInventory_UnequipCancel(unsigned long nPlayerId,
                                                           unsigned long oidItem)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(4, 0xffffffff, 1);
    WriteOBJECTIDServer(oidItem);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 0xc, 8, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056FDE0
int CSWSMessage::SendServerToPlayerLogin_Confirm(unsigned long nPlayerId)
{
    return SendServerToPlayerMessage(nPlayerId, 2, 5, NULL, 0);
}

#pragma inline_depth(0)
// FUNCTION: SWKOTOR 0x0056A7F0
int CSWSMessage::SendServerToPlayerDebugInfo_Item(CSWSPlayer* pPlayer, unsigned long oidItem)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CSWSItem* pItem = g_pAppManager->m_pServerExoApp->GetItemByGameObjectID(oidItem);
    if (!pItem) {
        return 0;
    }
    CreateWriteMessage(8, 0xffffffff, 1);
    WriteOBJECTIDServer(oidItem);
    WriteCExoString(pItem->m_sTag, 0x20);
    WriteINT(pItem->m_nPassiveProperties, 0x20);
    for (int i = 0; i < pItem->m_nPassiveProperties; i++) {
        CSWItemProperty* pProperty = pItem->GetPassiveProperty(i);
        if (pProperty) {
            WriteWORD(pProperty->m_nPropertyName, 0x10);
        }
    }
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    unsigned long nPlayerId = pPlayer->m_nPlayerID;
    return SendServerToPlayerMessage(nPlayerId, 0x21, 2, pMessage, nSize);
}
#pragma inline_depth()

// FUNCTION: SWKOTOR 0x0056A8C0
// NONMATCH: 32 diff insns, register rotation only (eax/ecx/edx swap from the
// first virtual call on); tried chained null checks and a name local.
int CSWSMessage::SendServerToPlayerDebugInfo_Placeable(CSWSPlayer* pPlayer,
                                                       unsigned long oidPlaceable)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CSWSPlaceable* pPlaceable =
        g_pAppManager->GetServerExoApp()->GetPlaceableByGameObjectID(oidPlaceable);
    if (!pPlaceable) {
        return 0;
    }
    CServerExoAppInternal* pInternal = g_pAppManager->GetServerExoApp()->m_pInternal;
    if (!pInternal) {
        return 0;
    }
    CFactionManager* pFactionManager = pInternal->m_pFactionManager;
    if (!pFactionManager) {
        return 0;
    }
    CFaction* pFaction = pFactionManager->GetFaction(pPlaceable->m_nFactionId);
    if (!pFaction) {
        return 0;
    }
    CreateWriteMessage(8, 0xffffffff, 1);
    WriteOBJECTIDServer(oidPlaceable);
    WriteCExoLocStringServer(*pPlaceable->GetFirstName(), 0);
    WriteCExoString(pPlaceable->m_sTag, 0x20);
    WriteBYTE(pPlaceable->m_nAppearance, 8);
    WriteWORD(pPlaceable->GetCurrentHitPoints(1), 0x10);
    WriteBYTE(pPlaceable->m_nHardness, 8);
    WriteBYTE(pPlaceable->m_nWillSave, 8);
    WriteBYTE(pPlaceable->m_nFortSave, 8);
    WriteCResRef(pPlaceable->m_cOnUsed, 0x10);
    WriteCExoString(pFaction->m_sFactionName, 0x20);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    unsigned long nPlayerId = pPlayer->m_nPlayerID;
    return SendServerToPlayerMessage(nPlayerId, 0x21, 3, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056AA30
int CSWSMessage::SendServerToPlayerDebugInfo_Area(CSWSPlayer* pPlayer, unsigned long oidArea)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CSWSArea* pArea = g_pAppManager->GetServerExoApp()->GetAreaByGameObjectID(oidArea);
    if (!pArea) {
        return 0;
    }
    CreateWriteMessage(8, 0xffffffff, 1);
    WriteOBJECTIDServer(oidArea);
    WriteCExoString(pArea->m_sTag, 0x20);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    unsigned long nPlayerId = pPlayer->m_nPlayerID;
    return SendServerToPlayerMessage(nPlayerId, 0x21, 4, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056AAC0
int CSWSMessage::SendServerToPlayerPopUpGUIPanel(unsigned long oidPlayer, int nPanel, int bArg1,
                                                 int bArg2, int nArg, const CExoString& sText)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CSWSPlayer* pPlayer =
        (CSWSPlayer*)g_pAppManager->GetServerExoApp()->GetClientObjectByObjectId(oidPlayer);
    if (!pPlayer) {
        return 1;
    }
    if (nPanel == GUI_PANEL_POPUP_A) {
        CSWSCreature* pCreature =
            g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidPlayer);
        if (!pCreature || !pCreature->m_pStats->m_bIsPC) {
            return 1;
        }
        CreateWriteMessage(0x80, 0xffffffff, 1);
        WriteBOOL(bArg1);
        WriteBOOL(bArg2);
        WriteINT(nArg, 0x20);
        WriteCExoString(sText, 0x20);
        if (!GetWriteMessage(&pMessage, &nSize)) {
            return 0;
        }
        unsigned long nPlayerId = pPlayer->m_nPlayerID;
        return SendServerToPlayerMessage(nPlayerId, 0x24, 1, pMessage, nSize);
    }
    if (nPanel == GUI_PANEL_POPUP_B) {
        CSWSCreature* pCreature =
            g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidPlayer);
        if (!pCreature || !pCreature->m_pStats->m_bIsPC) {
            return 1;
        }
        CreateWriteMessage(8, 0xffffffff, 1);
        WriteCExoString(sText, 0x20);
        if (!GetWriteMessage(&pMessage, &nSize)) {
            return 0;
        }
        return SendServerToPlayerMessage(pPlayer->GetPlayerId(), 0x24, 3, pMessage, nSize);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0056B090
int CSWSMessage::SendServerToPlayerCharacterDownloadReply(CSWSPlayer* pPlayer)
{
    unsigned char* pMessage;
    unsigned long nSize;
    int bRestore = 0;
    if (pPlayer->GetGameObject()) {
        CSWSCreature* pCreature = pPlayer->GetGameObject()->AsSWSCreature();
        if (pCreature) {
            if (pCreature->m_n9DC) {
                pCreature->m_nF8 = 0;
                bRestore = 1;
            }
            if (!pPlayer->PackCreatureIntoMessage()) {
                if (bRestore) {
                    pCreature->m_nF8 = 1;
                }
                return 0;
            }
            if (bRestore) {
                pCreature->m_nF8 = 1;
            }
            if (GetWriteMessage(&pMessage, &nSize)) {
                return SendServerToPlayerMessage(pPlayer->GetPlayerId(), 0x2b, 2, pMessage, nSize);
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0056B140
int CSWSMessage::SendServerToPlayerCharacterDownloadFail(CSWSPlayer* pPlayer)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CExoString sName;
    if (!pPlayer->HasCharacterFile()) {
        return 0;
    }
    pPlayer->m_cResRef.CopyToString(sName);
    CreateWriteMessage(sName.GetLength(), 0xffffffff, 1);
    WriteCExoString(sName, 0x20);
    unsigned char nType = pPlayer->m_n64;
    WriteBOOL(nType == 4);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    unsigned long nPlayerId = pPlayer->m_nPlayerID;
    return SendServerToPlayerMessage(nPlayerId, 0x2b, 3, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056B670
int CSWSMessage::SendServerToPlayerItemScriptAdd(unsigned long nPlayerId, unsigned long oidArea,
                                                 CSWSItem* pItem, Vector vPosition)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(0x80, 0xffffffff, 1);
    WriteOBJECTIDServer(pItem->m_idSelf);
    AddItemAppearanceToMessage(pItem);
    AddActiveItemPropertiesToMessage(pItem, NULL);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 5, 8, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x00569E10
int CSWSMessage::SendServerToPlayerUpdateActiveItemProperties(CSWSPlayer* pPlayer,
                                                              unsigned long oidItem,
                                                              unsigned char nUseable,
                                                              unsigned char nChanged,
                                                              unsigned char* pUses)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(5, 0xffffffff, 1);
    WriteOBJECTIDServer(oidItem);
    WriteBYTE(nUseable, 8);
    WriteBYTE(nChanged, 8);
    for (unsigned char i = 0; i < 8; i++) {
        if (nChanged & (1 << i)) {
            WriteBYTE(pUses[i], 8);
        }
    }
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(pPlayer->GetPlayerId(), 0x18, 1, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x00567EF0
int CSWSMessage::SendServerToPlayerModule_Info(unsigned long nPlayerId)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CSWSModule* pModule = g_pAppManager->GetServerExoApp()->GetModule();
    if ((g_pAppManager->GetServerExoApp()->GetServerMode() == 1 ||
         g_pAppManager->GetServerExoApp()->GetServerMode() == 2) && pModule) {
        CreateWriteMessage(0x80, 0xffffffff, 1);
        pModule->PackModuleIntoMessage(nPlayerId);
        if (GetWriteMessage(&pMessage, &nSize)) {
            return SendServerToPlayerMessage(nPlayerId, 3, 1, pMessage, nSize);
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00567F90
int CSWSMessage::SendServerToPlayerParty_List(unsigned long nPlayerId, int nCount,
                                              unsigned long* pIds, unsigned char nMinor,
                                              unsigned long oidExclude)
{
    unsigned char* pMessage;
    unsigned long nSize;
    int nWrite = 0;
    int i;
    for (i = 0; i < nCount; i++) {
        if (pIds[i] != oidExclude) {
            nWrite++;
        }
    }
    CreateWriteMessage(0x80, 0xffffffff, 1);
    WriteINT(nWrite, 0x20);
    for (i = 0; i < nCount; i++) {
        if (pIds[i] != oidExclude) {
            WriteOBJECTIDServer(pIds[i]);
        }
    }
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 0xe, nMinor, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x00569C40
int CSWSMessage::SendServerToPlayerGameObjUpdateVisEffect(CSWSPlayer* pPlayer,
                                                          unsigned short nEffect,
                                                          unsigned long oidTarget,
                                                          unsigned long oidSource,
                                                          unsigned long oidOrigin,
                                                          unsigned char nArg5,
                                                          unsigned char nArg6, Vector vTarget,
                                                          float fArg10)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CSWSObject* pSourceObject = NULL;
    CSWSObject* pTargetObject = NULL;
    CGameObject* pSource = g_pAppManager->GetServerExoApp()->GetGameObject(oidSource);
    CGameObject* pTarget = g_pAppManager->GetServerExoApp()->GetGameObject(oidTarget);
    if (pSource) {
        pSourceObject = pSource->AsSWSObject();
    }
    if (pTarget) {
        pTargetObject = pTarget->AsSWSObject();
    }
    CreateWriteMessage(6, 0xffffffff, 1);
    WriteOBJECTIDServer(oidTarget);
    WriteWORD(nEffect, 0x10);
    if (!pTargetObject) {
        WriteFLOAT(vTarget.x, 1.0f, 0x20);
        WriteFLOAT(vTarget.y, 1.0f, 0x20);
        WriteFLOAT(vTarget.z, 1.0f, 0x20);
    }
    else {
        WriteFLOAT(pTargetObject->m_vPosition.x, 1.0f, 0x20);
        WriteFLOAT(pTargetObject->m_vPosition.y, 1.0f, 0x20);
        WriteFLOAT(pTargetObject->m_vPosition.z, 1.0f, 0x20);
    }
    if (IsBeamVisualEffect(nEffect)) {
        WriteOBJECTIDServer(oidSource);
        WriteOBJECTIDServer(oidOrigin);
        WriteBYTE(nArg5, 8);
        WriteBYTE(nArg6, 8);
        WriteFLOAT(pSourceObject->m_vPosition.x, 1.0f, 0x20);
        WriteFLOAT(pSourceObject->m_vPosition.y, 1.0f, 0x20);
        WriteFLOAT(pSourceObject->m_vPosition.z, 1.0f, 0x20);
        if (oidTarget == INVALID_OBJECT_ID) {
            WriteFLOAT(fArg10, 1.0f, 0x20);
        }
    }
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    unsigned long nPlayerId = pPlayer->m_nPlayerID;
    return SendServerToPlayerMessage(nPlayerId, 5, 3, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x005682B0
int CSWSMessage::SendServerToPlayerPlayerList_Delete(unsigned long nPlayerId, CSWSPlayer* pPlayer)
{
    unsigned char* pMessage;
    unsigned long nSize;
    unsigned long nDeleteId = pPlayer->m_nPlayerID;
    CreateWriteMessage(4, 0xffffffff, 1);
    WriteINT(nDeleteId, 0x20);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 0xa, 3, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x00568310
int CSWSMessage::SendServerToPlayerServerStatus_Status(unsigned long nPlayerId)
{
    switch (g_pAppManager->GetServerExoApp()->GetServerMode()) {
    case 0:
        return SendServerToPlayerMessage(nPlayerId, 1, 2, NULL, 0);
    case 1:
        return SendServerToPlayerMessage(nPlayerId, 1, 1, NULL, 0);
    case 2:
        return SendServerToPlayerMessage(nPlayerId, 1, 3, NULL, 0);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0056F430
int CSWSMessage::SendServerToPlayerGameObjUpdate_ObjControl(unsigned long nPlayerId,
                                                            unsigned long oid)
{
    unsigned char* pMessage;
    unsigned long nSize;
    CreateWriteMessage(4, 0xffffffff, 1);
    WriteOBJECTIDServer(oid);
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 5, 2, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056F0A0
int CSWSMessage::SendServerToPlayerExamineGui_PlaceableData(CSWSPlayer* pPlayer,
                                                            unsigned long oid)
{
    unsigned char* pMessage;
    unsigned long nSize;
    unsigned long nPlayerId = pPlayer->m_nPlayerID;
    CreateWriteMessage(0x80, nPlayerId, 1);
    CGameObject* pObject = NULL;
    CGameObjectArray* pArray = g_pAppManager->GetServerExoApp()->GetObjectArray();
    if (pArray->GetGameObject(oid, &pObject) == GAME_OBJECT_ARRAY_SUCCESS) {
        CSWSPlaceable* pPlaceable = pObject->AsSWSPlaceable();
        if (pPlaceable) {
            WriteOBJECTIDServer(pPlaceable->m_idSelf);
            WriteCExoLocStringServer(pPlaceable->m_sDescription, 0);
        }
    }
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 0x1b, 4, pMessage, nSize);
}

// FUNCTION: SWKOTOR 0x0056F370
int CSWSMessage::SendServerToPlayerExamineGui_DoorData(CSWSPlayer* pPlayer, unsigned long oid)
{
    unsigned char* pMessage;
    unsigned long nSize;
    unsigned long nPlayerId = pPlayer->m_nPlayerID;
    CreateWriteMessage(0x80, nPlayerId, 1);
    CGameObject* pObject = NULL;
    CGameObjectArray* pArray = g_pAppManager->GetServerExoApp()->GetObjectArray();
    if (pArray->GetGameObject(oid, &pObject) == GAME_OBJECT_ARRAY_SUCCESS) {
        CSWSDoor* pDoor = pObject->AsSWSDoor();
        if (pDoor) {
            WriteOBJECTIDServer(pDoor->m_idSelf);
            WriteCExoLocStringServer(pDoor->m_sDescription, 0);
        }
    }
    if (!GetWriteMessage(&pMessage, &nSize)) {
        return 0;
    }
    return SendServerToPlayerMessage(nPlayerId, 0x1b, 6, pMessage, nSize);
}
