// CSWSMessage: server-side message writer (CNWMessage -> CSWMessage ->
// CSWSMessage). Only what the worker B send helpers use.
#ifndef B_SWS_MESSAGE_H
#define B_SWS_MESSAGE_H

#include "b/exo_string.h"
#include "b/exo_loc_string.h"
#include "b/vector.h"
#include "b/server_exo_app.h"
#include "b/res_gff.h"

class CSWSPlayer;
class CSWSItem;
class CSWSCreature;
class CSWSObject;

class CResRef;
class CGameObject;
class CSWSDoor;
class CSWSPlaceable;
class CLastUpdateObject;
class CSWSPlayerLastUpdateObject;
class CSWSPlayerLUOSortedObjectList;
class CLoopingVisualEffect;
class CSWSTrigger;
template <class T> class CExoArrayList;

class CNetLayer {
public:
    int SendMessageToPlayer(unsigned long nPlayerId, unsigned char* pData,
                            unsigned long nSize, unsigned long nFlags);  // 0x005D4CB0
};

// .rdata player id used by broadcasts (0x0074C52C).
extern const unsigned long PLAYER_ID_ALL_PLAYERS;

// vtable 0x007465A4 (CNWMessage)
class CNWMessage {
public:
    CNWMessage();                                                       // 0x004D60F0
    virtual ~CNWMessage();                                              // 0x004D6130

    void ExtendWriteBuffer(unsigned long nSize);                        // 0x004D6260
    int MessageReadOverflow(int bFlag);                                 // 0x004D6230
    int MessageReadUnderflow(int bFlag);                                // 0x004D6240
    int ReadBOOL();                                                     // 0x004D66F0
    unsigned char ReadBYTE(int nBits);                                  // 0x004D6720
    unsigned short ReadWORD(int nBits);                                 // 0x004D6750
    unsigned long ReadDWORD(int nBits);                                 // 0x004D6780
    float ReadFLOAT(float fScale, int nBits);                           // 0x004D67B0
    CExoString ReadCExoString(int nBits);                               // 0x004D67E0

    void WriteBOOL(int bValue);                                         // 0x004D68E0
    void WriteBYTE(unsigned char nValue, int nBits);                    // 0x004D62C0
    void WriteWORD(unsigned short nValue, int nBits);                   // 0x004D6300
    void WriteINT(int nValue, int nBits);                               // 0x004D6340
    void WriteDWORD(unsigned long nValue, int nBits);                   // 0x004D6340
    void WriteFLOAT(float fValue, float fScale, int nBits);             // 0x004D6920
    void WriteVector(const Vector& vValue);                            // 0x004D71B0
    void WriteCExoString(const CExoString& sValue, int nBits);          // 0x004D6960
    void WriteCResRef(CResRef cResRef, int nBits);                      // 0x004D6380
    int GetWriteMessage(unsigned char** ppMessage, unsigned long* pnSize); // 0x004D6440

    unsigned char* m_pnWriteBuffer;                                     // 0x04
    unsigned long m_nWriteBufferSize;                                   // 0x08
    unsigned long m_nWriteBufferPtr;                                    // 0x0c
    char pad_0x10[0x1c - 0x10];
    unsigned long m_nWriteBytesTotal;                                   // 0x1c
    char pad_0x20[0x28 - 0x20];
    unsigned char* m_pnReadBuffer;                                      // 0x28
    unsigned long m_nReadBufferSize;                                    // 0x2c
    unsigned long m_nReadBufferPtr;                                     // 0x30
    char pad_0x34[0x4c - 0x34];
    unsigned long m_nPlayerId;                                          // 0x4c
};

class CSWMessage : public CNWMessage {
public:
    unsigned long PeekAtWriteMessageSize();                             // 0x00405040
    void CreateWriteMessage(unsigned long nSize, unsigned long nPlayerId,
                            int bArg);                                  // 0x004D68A0
    CResRef ReadCResRef(int nBits);                                     // 0x004D6180
};

class CSWSMessage : public CSWMessage {
public:
    CSWSMessage();                                                      // 0x00520980
    int SendServerToPlayerCCMessage(unsigned long nPlayerId, unsigned char nMinor,
                                    class CSWCCMessageData* pData,
                                    class CSWSCombatAttackData* pAttackData); // 0x00568570
    int SendServerToServerAdminMessage(unsigned long nPlayerId, const CExoString& sMessage); // 0x00579890
    int SendServerToAllPlayersCreatureUpdate_StripEquippedItems(unsigned long oidCreature); // 0x00570180 (name inferred)
    int SendServerToPlayerSafeProjectileProjectile(CSWSPlayer* pPlayer,
        unsigned long oidSource, unsigned long oidTarget, Vector vSource, Vector vTarget,
        unsigned long nProjectile, unsigned char nType, int nTravelTime,
        unsigned char nFlags, unsigned char nAttackType, int nExtra); // 0x0056E5F0
    int SendServerToPlayerSafeProjectileSpell(CSWSPlayer* pPlayer,
        unsigned long oidSource, unsigned long oidTarget, Vector vSource, Vector vTarget,
        unsigned long nProjectile, unsigned char nType, int nExtra,
        unsigned char nItem, unsigned char nFlags, unsigned char nAttackType,
        int nExtra1, int nExtra2); // 0x0056E6E0 (name inferred)

    // Player-to-server handlers.
    int HandlePlayerToServerQuickChatMessage(CSWSPlayer* pPlayer, unsigned char nMinor);
    int HandlePlayerToServerGuiInventoryMessage(CSWSPlayer* pPlayer, unsigned char nMinor);
    int HandlePlayerToServerInputCancelGuiTimingEvent(CSWSPlayer* pPlayer);
    int HandlePlayerToServerServerStatusMessage(CSWSPlayer* pPlayer, unsigned char nMinor);
    int HandlePlayerToServerCharacterDownload(CSWSPlayer* pPlayer, unsigned char nMinor);
    int HandlePlayerToServerShutDownServer(CSWSPlayer* pPlayer, unsigned char nMinor);
    int HandlePlayerToServerPlayModuleCharacterList_Start(CSWSPlayer* pPlayer);
    int HandlePlayerToServerPlayModuleCharacterList_Stop(CSWSPlayer* pPlayer);
    int HandlePlayerToServerPlayModuleCharacterList(CSWSPlayer* pPlayer, unsigned char nMinor);
    int HandlePlayerToServerInputAbortDriveControl(CSWSPlayer* pPlayer);
    int HandlePlayerToServerMapPinSetMapPinAt(CSWSPlayer* pPlayer);
    int HandlePlayerToServerMapPinDestroyMapPin(CSWSPlayer* pPlayer);
    int HandlePlayerToServerMapPinChangePin(CSWSPlayer* pPlayer);
    int HandlePlayerToServerMapPinMessage(CSWSPlayer* pPlayer, unsigned char nMinor);
    int QuickChatMessage(unsigned long oidSpeaker, unsigned short nQuickChat); // 0x00571EC0
    int HandlePlayerToServerAreaMessage(CSWSPlayer* pPlayer, unsigned char nMinor);
    int HandlePlayerToServerPlayerDeath(CSWSPlayer* pPlayer, unsigned char nMinor);
    int HandlePlayerToServerDialogMessage(CSWSPlayer* pPlayer, unsigned char nMinor);
    int HandlePlayerToServerGuiContainerMessage(CSWSPlayer* pPlayer, unsigned char nMinor);
    int HandlePlayerToServerCharacterSheetMessage(CSWSPlayer* pPlayer, unsigned char nMinor);
    int HandlePlayerToServerGoldMessage(CSWSPlayer* pPlayer, unsigned char nMinor);
    int HandlePlayerToServerInputDriveControl(CSWSPlayer* pPlayer);
    int HandlePlayerToServerCharListMessage(CSWSPlayer* pPlayer, unsigned char nMinor);
    int ParseGetString(char* pText, unsigned long nLength, CExoString* pResult,
                       unsigned long nMaxLength);              // 0x00528090
    int ParseToken(char* szToken, char** ppText, unsigned long* pnLength,
                   int bSeparator);                            // 0x00528130
    int SendServerToPlayerCharList(CSWSPlayer* pPlayer);                // 0x00576360 (V0, name guessed)
    int HandleCharListCharacter(CSWSPlayer* pPlayer, unsigned char nIndex,
                                CResRef cResRef);                       // 0x00570C60 (V0, name guessed)
    int SendServerToPlayerPlayerList_Add(unsigned long nPlayerId, CSWSPlayer* pPlayer); // 0x00568030
    int SendServerToPlayerPlayerList_All(CSWSPlayer* pPlayer);         // 0x0056FE40
    int SendServerToPlayerModule_SetPauseState(unsigned char nState, int bPaused); // 0x0056C9F0
    int SendServerToPlayerPlayModuleCharacterListResponse(unsigned long nPlayerId,
                                                          unsigned long oidCharacter,
                                                          int bAdd); // 0x0056B400
    virtual ~CSWSMessage();                                             // 0x005209A0
    unsigned long ReadOBJECTIDServer();                                 // 0x00578540
    void SendGameObjectUpdate(unsigned long nPlayerId);                 // 0x00578520 (name guessed)
    int SendServerToPlayerGameObjUpdate(unsigned long nPlayerId,
                                        unsigned long oidArg);          // 0x00578200
    void WriteOBJECTIDServer(unsigned long oid);                        // 0x00578570
    void WriteCExoLocStringServer(const CExoLocString& sString,
                                  unsigned char nGender);               // 0x005785C0

    void AddActiveItemPropertiesToMessage(CSWSItem* pItem,
                                          CSWSCreature* pCreature);     // 0x00566F20
    void AddItemAppearanceToMessage(CSWSItem* pItem);                   // 0x00567090
    void AddDoorAppearanceToMessage(CSWSDoor* pDoor);                   // 0x00566DA0
    void AddPlaceableAppearanceToMessage(CSWSPlaceable* pPlaceable);    // 0x00566DF0
    unsigned long ComputeAppearanceUpdateRequired(CGameObject* pObject,
                                                  CLastUpdateObject* pLast); // 0x00567100
    int ComputeInventoryUpdateRequired(CSWSPlayer* pPlayer, unsigned long nSlot); // 0x00567240
    int CompareVisualEffectLists(CExoArrayList<CLoopingVisualEffect*>* pList1,
                                 CExoArrayList<CLoopingVisualEffect*>* pList2); // 0x005672F0
    unsigned short ComputeLastUpdate_GuiSkills(CSWSCreature* pCreature); // 0x00567430
    int ComputeLastUpdate_GuiKnownSpellUses(CSWSCreature* pCreature,
                                            CSWSPlayerLastUpdateObject* pLast); // 0x00567490
    void StoreLastUpdate_ActionQueue(CSWSCreature* pCreature,
                                     CSWSPlayerLastUpdateObject* pLast); // 0x00567520 (name guessed)
    int ComputeLastUpdate_ActionQueue(CSWSCreature* pCreature,
                                      CSWSPlayerLastUpdateObject* pLast); // 0x00567590
    unsigned short ComputeLastUpdate_PlayerState(CSWSCreature* pCreature); // 0x00567600
    unsigned char ComputeLastUpdate_AssociateState(CSWSCreature* pCreature); // 0x005676B0
    void WriteGameObjUpdate_WriteInventorySlotAdd(CSWSCreature* pCreature, CSWSItem* pItem,
                                                  unsigned long nSlot); // 0x00567710
    void UpdateLastUpdateInventory(CSWSPlayer* pPlayer, unsigned long nSlot); // 0x005683D0
    unsigned long SelectCategoryForGameObject(CGameObject* pObject,
                                              CGameObject* pPlayerObject); // 0x00567780
    void WriteGameObjUpdate_WorkRemaining(CGameObject* pObject, int bOnce, int nDone,
                                          int nTotal);                  // 0x00567BA0
    void AddTriggerGeometryToMessage(CSWSTrigger* pTrigger);            // 0x00567C20
    void UpdateLastUpdateObjectAppearance(CGameObject* pObject, CLastUpdateObject* pLast,
                                          unsigned long nFlags);        // 0x00568480
    void UpdateLastUpdateObject(CSWSPlayer* pPlayer, CSWSObject* pObject,
                                 CLastUpdateObject* pLast, unsigned long nFlags); // 0x00575C80
    void StoreValuesInLastUpdateObject(CSWSPlayer* pPlayer, CLastUpdateObject* pLast,
                                        CSWSObject* pObject, unsigned long nFlags,
                                        unsigned long nAppearanceFlags); // 0x00576FC0
    void WriteGameObjUpdate_CharacterSheet(CSWSPlayer* pPlayer, unsigned long nFlags); // 0x0056F490
    void WriteGameObjUpdate_MinorGUIPanels(CSWSPlayer* pPlayer); // 0x00572990
    unsigned long ComputeUpdateRequired(CSWSPlayer* pPlayer, CSWSObject* pObject,
                                         CLastUpdateObject* pLast, int bSelf); // 0x0056B950
    CLastUpdateObject* CreateNewLastUpdateObject(CSWSPlayer* pPlayer, CSWSObject* pObject,
                                                 unsigned long* pFlags,
                                                 unsigned long* pAppearanceFlags); // 0x00577000
    void TestObjectUpdateDifferences(CSWSPlayer* pPlayer, CSWSObject* pObject,
                                      CLastUpdateObject** ppLast, unsigned long* pFlags,
                                      unsigned long* pAppearanceFlags); // 0x00577FA0
    int TestObjectVisible(CSWSObject* pObject, CSWSObject* pObserver); // 0x0056DC60
    void DeleteLastUpdateObjectsForObject(CSWSPlayer* pPlayer, unsigned long oidObject); // 0x0056E190
    void WriteGameObjUpdate_UpdateObject(CSWSPlayer* pPlayer, CSWSObject* pObject,
        CLastUpdateObject* pLast, unsigned long nFlags, unsigned long nAppearanceFlags); // 0x00574A10
    void ComputeGameObjectUpdateForObject(CSWSPlayer* pPlayer, CSWSObject* pObserver,
        CGameObjectArray* pArray, unsigned long oidObject); // 0x00578060
    int ComputeGameObjectUpdateForCategory(unsigned long nCategory, unsigned long nLimit,
        CSWSPlayer* pPlayer, CSWSObject* pObserver, CGameObjectArray* pArray,
        CSWSPlayerLUOSortedObjectList* pObjects, int nCount); // 0x00578130
    int CompareObjectIdLists(CExoArrayList<unsigned long>* pList1,
                             CExoArrayList<unsigned long>* pList2); // 0x0056B830 (name guessed)
    int ComputeVisibilityLists(CSWSCreature* pCreature,
                               CSWSPlayerLastUpdateObject* pLast);      // 0x0056B880
    int ComputeLastUpdate_GuiFeats(CSWSCreature* pCreature,
                                   CSWSPlayerLastUpdateObject* pLast);  // 0x0056C310
    int ComputeLastUpdate_CompareSpellLikeAbility(CSWSCreature* pCreature,
                                                  CSWSPlayerLastUpdateObject* pLast); // 0x0056C430
    void ComputeLastUpdate_WriteSpellLikeAbility(CSWSCreature* pCreature,
                                                 CSWSPlayerLastUpdateObject* pLast); // 0x0056C4D0
    void AssignCreatureLists(CExoArrayList<unsigned long>* pSource,
                             CExoArrayList<unsigned long>* pTarget);    // 0x005706F0
    void ComputeLastUpdate_StoreUpdateSpellLikeAbility(CSWSCreature* pCreature,
                                                       CSWSPlayerLastUpdateObject* pLast); // 0x00570BA0
    static void GetLocStringServer(unsigned long nPlayerId, const CExoLocString& sFirst,
                                   const CExoLocString& sLast, CExoString& sOut,
                                   unsigned char nGender);              // 0x00578870

    int SendServerToPlayerMessage(unsigned long nPlayerId, unsigned char nMajor,
                                  unsigned char nMinor, unsigned char* pData,
                                  unsigned long nSize);

    int SendServerToPlayerCheatNasty(unsigned long nPlayerId, unsigned long oidTarget);
    int SendServerToPlayerCheatDebugMode(int bDebugMode);
    int SendServerToPlayerVoiceChat_Play(CSWSPlayer* pPlayer, unsigned long oidSpeaker,
                                         unsigned char nVoiceChat);
    int SendServerToPlayerGuiContainerObject_Open(CSWSPlayer* pPlayer,
                                                  unsigned long oidContainer, int nArg);
    int SendServerPlayerItemUpdate_DestroyItem(CSWSPlayer* pPlayer, unsigned long oidItem);
    int SendServerToPlayerJournalAddQuest(CSWSPlayer* pPlayer, const CExoString& sTag,
                                          int nState, int nPriority, unsigned short nPlanetId,
                                          int bComplete, int nCalendarDay, int nTimeOfDay,
                                          const CExoLocString& sName, const CExoLocString& sText,
                                          int nXP, int nEntryId, int nPlotIndex);
    int SendServerToPlayerModuleUpdate_Time(CSWSPlayer* pPlayer, unsigned char nFlags,
                                            unsigned long nMillisecond, unsigned char nSecond,
                                            unsigned char nMinute, unsigned char nHour,
                                            unsigned char nTransitionType,
                                            unsigned long nTransitionTime);
    int SendServerToPlayerJournalRemoveQuest(CSWSPlayer* pPlayer, const CExoString& sTag);
    int SendServerToPlayerJournalAddWorld(CSWSPlayer* pPlayer, int nIndex,
                                          const CExoString& sName, const CExoString& sText,
                                          unsigned long nCalendarDay, unsigned long nTimeOfDay);
    int SendServerToPlayerJournalAddWorldStrref(CSWSPlayer* pPlayer, unsigned long nName,
                                                unsigned long nText, unsigned long nCalendarDay,
                                                unsigned long nTimeOfDay);
    int SendServerToPlayerJournalDeleteWorld(CSWSPlayer* pPlayer, int nIndex);
    int SendServerToPlayerJournalDeleteWorldStrref(CSWSPlayer* pPlayer, unsigned long nStrRef);
    int SendServerToPlayerJournalDeleteWorldAll(CSWSPlayer* pPlayer);
    int SendServerToPlayerJournalFullUpdateNotNeeded(CSWSPlayer* pPlayer, int bNeeded);
    int SendServerToPlayerJournalUpdated(CSWSPlayer* pPlayer, int bQuest, int bCompleted,
                                         const CExoLocString& sName);
    int SendServerToPlayerLevelUp_Confirmation(unsigned long nPlayerId, int bSuccess);
    int SendServerToPlayerMapPinEnabled(CSWSPlayer* pPlayer, unsigned long oidPin,
                                        int bEnabled);
    int SendServerToPlayerMapPinAdded(CSWSPlayer* pPlayer, Vector vPosition,
                                      const CExoString& sNote, unsigned long nId);
    int SendServerToPlayerAmbientMusicPlay(unsigned long nPlayerId, int bPlay);
    int SendServerToPlayerAmbientMusicSetDelay(unsigned long nPlayerId, int nDelay);
    int SendServerToPlayerAmbientMusicChangeTrack(unsigned long nPlayerId, int bDay,
                                                  int nTrack);
    int SendServerToPlayerAmbientBattleMusicPlay(unsigned long nPlayerId, int bPlay);
    int SendServerToPlayerAmbientBattleMusicChange(unsigned long nPlayerId, int nTrack);
    int SendServerToPlayerAmbientSoundLoopPlay(unsigned long nPlayerId, int bPlay);
    int SendServerToPlayerAmbientSoundLoopChange(unsigned long nPlayerId, int bDay,
                                                 int nSound);
    int SendServerToPlayerAmbientSoundVolumeChange(unsigned long nPlayerId, int bDay,
                                                   int nVolume);
    int SendServerToPlayerInventory_LearnScroll(unsigned long nPlayerId,
                                                unsigned long oidScroll,
                                                unsigned char nMinor);
    int SendServerToPlayerShutDownServer(unsigned long nPlayerId, int nReason);
    int SendServerToPlayerDestroyDeathGUI(unsigned long nPlayerId);
    int SendServerToPlayerGuiTimingEvent(CSWSPlayer* pPlayer, int bStart,
                                         unsigned char nEventId, int nDuration);
    int SendServerToPlayerGUICharacterSheet_NotPermitted(unsigned long nPlayerId,
                                                         unsigned long oidCreature);
    int SendServerToPlayerCamera_ChangeLocation(CSWSPlayer* pPlayer, unsigned char nFlags,
                                                float fDistance, float fPitch, float fFacing,
                                                unsigned char nSpeed);
    int SendServerToPlayerCamera_SetMode(CSWSPlayer* pPlayer, unsigned char nMode);
    int SendServerToPlayerLogin_CharacterQuery(CSWSPlayer* pPlayer, unsigned char* pClassCount,
                                               int* pClasses, unsigned char* pLevels,
                                               unsigned long* pExperience);
    int SendServerToPlayerLogin_NeedCharacter(unsigned long nPlayerId);
    int SendServerToPlayerModule_SaveGameStatus(unsigned char nStatus);
    int SendServerToPlayerLoadBar_StartStallEvent(unsigned long nEvent);
    int SendServerToPlayerLoadBar_UpdateStallEvent(unsigned long nEvent, unsigned long nProgress);
    int SendServerToPlayerLoadBar_EndStallEvent(unsigned long nEvent, unsigned long nError);
    int SendServerToPlayerModule_ExportReply(CSWSPlayer* pPlayer);
    int SendServerToPlayerModule_Loading(CSWSPlayer* pPlayer);
    int SendServerToPlayerSaveLoad_Status(CSWSPlayer* pPlayer, unsigned char nStatus, int nError);
    int SendServerToPlayerModule_StartStartNewModule();
    int SendServerToPlayerModule_EndStartNewModule(int bSuccess);
    int SendServerToPlayerModule_DumpPlayer(CSWSPlayer* pPlayer);
    int SendServerToPlayerArea_VisualEffect(CSWSPlayer* pPlayer, unsigned short nEffect,
                                            float fX, float fY, float fZ);
    int SendServerToPlayerArea_Weather(CSWSPlayer* pPlayer, unsigned char nWeather,
                                       int bEnabled);
    int SendServerToPlayerSetCustomToken(unsigned long nPlayerId, int nToken,
                                         const CExoString& sValue);
    int SendServerToPlayerSetCustomTokenList(unsigned long nPlayerId);
    int ChatStrRefMessage(unsigned long nTalkVolume, unsigned long oidSpeaker,
                          unsigned long nStrRef);                       // 0x00571760
    int ChatMessage(unsigned long nTalkVolume, unsigned long oidSpeaker, CExoString sText,
                    unsigned long oidTarget, const CExoString& sName);  // 0x00571960 (name guessed)
    int SendServerToPlayerChatMultiLang_Helper(unsigned long nPlayerId, unsigned char nChatType,
                                               unsigned long oidSpeaker,
                                               const CExoLocString& sMessage,
                                               unsigned long oidTarget, unsigned char nGender,
                                               int bPrivate, const CResRef& cSound,
                                               unsigned long oidSound, int nValue);
    int SendServerToPlayerAIActionPlaySound(unsigned long nPlayerId, unsigned long oidSource,
                                            const CExoString& sSound);
    int SendServerToPlayerSoundObject_Play(CSWSPlayer* pPlayer, unsigned long oidSound);
    int SendServerToPlayerSoundObject_Stop(CSWSPlayer* pPlayer, unsigned long oidSound);
    int SendServerToPlayerSoundObject_ChangePosition(CSWSPlayer* pPlayer, unsigned long oidSound,
                                                     float fX, float fY, float fZ);
    int SendServerToPlayerChat_StrRef(unsigned long nPlayerId, unsigned long oidSpeaker,
                                      unsigned char nChatType, unsigned long nStrRef);
    int SendServerToPlayerChat_Party(unsigned long nPlayerId, unsigned long oidSpeaker,
                                     const CExoString& sMessage);
    int SendServerToPlayerChat_Talk(unsigned long nPlayerId, unsigned long oidSpeaker,
                                    const CExoString& sMessage);
    int SendServerToPlayerQuickChat(unsigned long nPlayerId, unsigned long oidSpeaker,
                                    unsigned short nQuickChat);
    int SendServerToPlayerChat_Tell(unsigned long nPlayerId, unsigned long oidSpeaker,
                                    const CExoString& sMessage);
    int SendServerToPlayerChat_ServerTell(unsigned long nPlayerId, const CExoString& sMessage);
    int SendServerToPlayerChat_Shout(unsigned long nPlayerId, unsigned long oidSpeaker,
                                     const CExoString& sMessage);
    int SendServerToPlayerChat_Whisper(unsigned long nPlayerId, unsigned long oidSpeaker,
                                       const CExoString& sMessage);
    int SendServerToPlayerInventory_Drop(unsigned long nPlayerId, unsigned long oidItem);
    int SendServerToPlayerInventory_DropCancel(unsigned long nPlayerId, unsigned long oidItem);
    int SendServerToPlayerInventory_EquipCancel(unsigned long nPlayerId, unsigned long oidItem,
                                                int nSlot);
    int SendServerToPlayerInventory_Pickup(unsigned long nPlayerId, unsigned long oidItem);
    int SendServerToPlayerInventory_PickupCancel(unsigned long nPlayerId, unsigned long oidItem);
    int SendServerToPlayerInventory_Unequip(unsigned long nPlayerId, unsigned long oidItem,
                                            int bUnequip);
    int SendServerToPlayerInventory_UnequipCancel(unsigned long nPlayerId,
                                                  unsigned long oidItem);
    int SendServerToPlayerLogin_Confirm(unsigned long nPlayerId);
    int SendServerToPlayerDebugInfo_Item(CSWSPlayer* pPlayer, unsigned long oidItem);
    int SendServerToPlayerDebugInfo_Placeable(CSWSPlayer* pPlayer, unsigned long oidPlaceable);
    int SendServerToPlayerDebugInfo_Area(CSWSPlayer* pPlayer, unsigned long oidArea);
    int SendServerToPlayerPopUpGUIPanel(unsigned long oidPlayer, int nPanel, int bArg1,
                                        int bArg2, int nArg, const CExoString& sText);
    int SendServerToPlayerCharacterDownloadReply(CSWSPlayer* pPlayer);
    int SendServerToPlayerCharacterDownloadFail(CSWSPlayer* pPlayer);
    int SendServerToPlayerItemScriptAdd(unsigned long nPlayerId, unsigned long oidArea,
                                        CSWSItem* pItem, Vector vPosition);
    int SendServerToPlayerUpdateActiveItemProperties(CSWSPlayer* pPlayer,
                                                     unsigned long oidItem,
                                                     unsigned char nUseable,
                                                     unsigned char nChanged,
                                                     unsigned char* pUses);
    int SendServerToPlayerModule_Info(unsigned long nPlayerId);
    int SendServerToPlayerParty_List(unsigned long nPlayerId, int nCount, unsigned long* pIds,
                                     unsigned char nMinor, unsigned long oidExclude);
    int SendServerToPlayerGameObjUpdateVisEffect(CSWSPlayer* pPlayer, unsigned short nEffect,
                                                 unsigned long oidTarget,
                                                 unsigned long oidSource,
                                                 unsigned long oidOrigin, unsigned char nArg5,
                                                 unsigned char nArg6, Vector vTarget,
                                                 float fArg10);
    int SendServerToPlayerPlayerList_Delete(unsigned long nPlayerId, CSWSPlayer* pPlayer);
    int SendServerToPlayerServerStatus_Status(unsigned long nPlayerId);
    int SendServerToPlayerGameObjUpdate_ObjControl(unsigned long nPlayerId, unsigned long oid);
    int SendServerToPlayerExamineGui_PlaceableData(CSWSPlayer* pPlayer, unsigned long oid);
    int SendServerToPlayerExamineGui_DoorData(CSWSPlayer* pPlayer, unsigned long oid);
};

#endif
