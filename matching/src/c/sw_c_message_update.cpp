// CSWCMessage update/dispatch handlers (b00646_cswc_message_handlers).
// Kept apart from sw_c_message.cpp so the handlers it calls stay calls.

#define OBJECT_INVALID 0x7f000000

extern const unsigned char OBJECT_TYPE_CREATURE;    // 0x00746634
extern const unsigned char OBJECT_TYPE_ITEM;        // 0x00746635
extern const unsigned char OBJECT_TYPE_TRIGGER;     // 0x00746636
extern const unsigned char OBJECT_TYPE_PLACEABLE;   // 0x00746638
extern const unsigned char OBJECT_TYPE_DOOR;        // 0x00746639
extern const unsigned char OBJECT_TYPE_AREA_OF_EFFECT;  // 0x0074663A
extern const unsigned char GAME_OBJECT_VALID;       // 0x0074666C

extern int g_bCameraFollowFlag;                     // 0x007A2484
extern int g_bModuleLoadEnabled;                    // 0x008328FC

struct Vector {
    float x, y, z;
};

class CSWCItem;
class CSWCPlaceable;

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28();
    virtual void JoinParty();                       // 0x2c
    virtual void v30();
    virtual CSWCItem* AsSWCItem();                  // 0x34
    virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48();
    virtual CSWCPlaceable* AsSWCPlaceable();        // 0x4c

    char pad_0x4[0x4];
    unsigned char m_nObjectType;                    // 0x08
};

class CGameObjectArray {
public:
    unsigned char Delete(unsigned long id, CGameObject** ppObject);         // 0x004D81A0
    unsigned char GetGameObject(unsigned long id, CGameObject** ppObject);  // 0x004D8230
};

class CSWCObject {
public:
    virtual ~CSWCObject();
    virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9C();
    virtual void vA0(); virtual void vA4(); virtual void vA8(); virtual void vAC();
    virtual void vB0();
    virtual void ApplyVisualEffect(unsigned short nEffect, int a, unsigned long oidTarget,
                                   unsigned long oidSource, unsigned char nNode, int b,
                                   const Vector& vOffset);  // 0xb4
    virtual void RemoveVisualEffect(unsigned short nEffect);  // 0xb8

    void SetSolid(int b);                           // 0x0060E760

    unsigned long m_idSelf;                         // 0x04
    char pad_0x8[0x8];
};

class CSWItem {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80();
    virtual void SetPossessor(int b);               // 0x84
    virtual void v88(); virtual void v8C(); virtual void v90();
    virtual void SetEquipped(int b);                // 0x94
};

struct CSWItemProperty {
    char pad_0x0[0x6];
    unsigned short m_nUsesType;                     // 0x06
};

class CSWCItem : public CSWCObject, public CSWItem {
public:
    CSWItemProperty* GetActiveProperty(int nProperty);  // 0x006466A0
    void SetStackSize(int nSize);                   // 0x00646810
    void SetItemPropertyUsesLeft(int nProperty, unsigned short nUses);  // 0x006469B0
    unsigned long GetEquippedBy();                  // 0x00646630

    unsigned long m_idItem;                         // 0x14
    char pad_0x18[0x13c];
    unsigned long m_nInventorySlot;                 // 0x154
    long m_nInventoryIndex;                         // 0x158
    char pad_0x15c[0x24];
    unsigned long m_nRepositoryValue;               // 0x180
};

class CSWCPlaceable : public CSWCObject {
public:
    void SetFadeState(int n);                       // 0x0063E220
    void Fade(int a, int b, unsigned long nDelay, unsigned long nTime); // 0x0063CE60

    char pad_0x10[0xd4];
    int m_bFading;                                  // 0xe4
    char pad_0xe8[0x28];
    int m_bState110;                                // 0x110
    char pad_0x114[0x4];
    int m_bState118;                                // 0x118
    int m_bOpen;                                    // 0x11c
    char pad_0x120[0x4];
    int m_bState124;                                // 0x124
    char pad_0x128[0x14];
    int m_bSolid;                                   // 0x13c
};

class CSWCDoor : public CSWCObject {
public:
    char pad_0x10[0xf4];
    int m_bState104;                                // 0x104
    int m_bState108;                                // 0x108
    char pad_0x10c[0x4];
    int m_bOpen;                                    // 0x110
    int m_bState114;                                // 0x114
    char pad_0x118[0x8];
    int m_bSolid;                                   // 0x120
};

class CSWCCreature;

class CSWCCreatureVisuals {
public:
    void ResetVisuals();                            // 0x00647A80
};

class CSWCCreature : public CSWCObject {
public:
    unsigned long GetEquippedItemID(unsigned long nSlot);   // 0x0060CA80
    void EquipItem(unsigned long nSlot, CSWCItem* pItem);   // 0x00613BA0

    char pad_0x10[0x2e8];
    CSWCCreatureVisuals* m_pVisuals;                // 0x2f8
    char pad_0x2fc[0x18];
    int m_bCameraFollowA;                           // 0x314
    int m_bCameraFollowB;                           // 0x318
};

class CSWBaseItem {
public:
    char pad_0x0[0x18];
    unsigned char m_nModelType;                     // 0x18
};

class CSWBaseItemArray {
public:
    CSWBaseItem* GetBaseItem(int nBaseItem);        // 0x005B31A0
};

class CSWRules {
public:
    char pad_0x0[0x34];
    CSWBaseItemArray* m_pBaseItems;                 // 0x34

    CSWBaseItemArray* GetBaseItemArray() { return m_pBaseItems; }
};

extern CSWRules* g_pRules;                          // 0x007A3A28

int GetIsBeam(unsigned short nEffect);              // 0x004CA960

class CSWCPlayer {
public:
    char pad_0x0[0x28];
    unsigned long m_oidControlled;                  // 0x28
};

struct CModuleInfo {
    char pad_0x0[0x4];
    unsigned char m_nFlags;                         // 0x04
    char pad_0x5[0xff];
    unsigned long m_oidDebug;                       // 0x104
};

class CSWCModule {
public:
    void SetCameraTarget(unsigned long oid, int b); // 0x0063F9D0
    void SetTimeOfDayState(unsigned char nState, unsigned long nTime);  // 0x00640640
    void Reset();                                   // 0x004015A0
    int LoadModule();                               // 0x0063F660

    char pad_0x0[0x40];
    void* m_pArea;                                  // 0x40
    char pad_0x44[0x4];
    CModuleInfo* m_pInfo;                           // 0x48
    char pad_0x4c[0x58];
    unsigned long m_nTimeA4;                        // 0xa4
    unsigned long m_nTimeA8;                        // 0xa8
    unsigned long m_nTimeAC;                        // 0xac
    unsigned long m_nTimeB0;                        // 0xb0
};

class CGuiInGame {
public:
    int GetPartyAccessPanelUp(int b);               // 0x0062B4D0
    int GetEquipNPC();                              // 0x0062FEA0
    void Reset();                                   // 0x004015A0
};

class CClientExoAppInternal {
public:
    char pad_0x0[0x14];
    CGameObjectArray* m_pGameObjectArray;           // 0x14
};

class CClientExoApp {
public:
    void* m_pVtable;                                // 0x00
    CClientExoAppInternal* m_pcInternal;            // 0x04

    CSWCItem* GetItemByGameObjectID(unsigned long oid);             // 0x005ED5A0

    void UnloadModule();                            // 0x005ED430
    CSWCModule* CreateModule();                     // 0x005ED440
    CSWCModule* GetModule();                        // 0x005ED530
    void InitializeInGameGUI();                     // 0x005ED730
    CSWCCreature* GetPlayerCreature();              // 0x005ED540
    unsigned long GetPlayerCreatureId();            // 0x005ED550
    CGuiInGame* GetGuiInGame();                     // 0x005ED690
    int DontUseThisSetPlayerCreature(unsigned long oid);    // 0x005EE2E0
    CGameObjectArray* GetGameObjectArray();         // 0x005ED570
    CGameObject* GetGameObject(unsigned long oid);  // 0x005ED580
    CSWCPlayer* GetPlayerByGameObjectID(unsigned long oid);         // 0x005ED590
    CSWCCreature* GetCreatureByGameObjectID(unsigned long oid);     // 0x005ED5B0
    CSWCPlaceable* GetPlaceableByGameObjectID(unsigned long oid);   // 0x005ED5F0
    CSWCDoor* GetDoorByGameObjectID(unsigned long oid);             // 0x005ED600
    void AttachPlayerGameObjectToArea();            // 0x005ED720
    void SetPlayerCreature(unsigned long oid);      // 0x005ED780
    void SetCharacterChangeInProgress(int b);       // 0x005EE1A0
};

class CSWPartyTable {
public:
    unsigned long GetNPCObject(int nNPC, int a, int b);     // 0x00564700
};

struct CServerInfo {
    char pad_0x0[0x104];
    unsigned char m_nFlags;                         // 0x104
};

class CServerExoApp {
public:
    CServerInfo* GetServerInfo();                   // 0x004AEC90
    CSWPartyTable* GetPartyTable();                 // 0x004AEE70
};

class CAppManager {
public:
    void* m_pUnknown00;                             // 0x00
    CClientExoApp* m_pClientExoApp;                 // 0x04
    CServerExoApp* m_pServerExoApp;                 // 0x08

    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
    CServerExoApp* GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CSWCMessage {
public:
    int MessageReadOverflow(int bFlag);             // 0x004D6230
    int MessageReadUnderflow(int bFlag);            // 0x004D6240
    int ReadBOOL();                                 // 0x004D66F0
    unsigned char ReadBYTE(int nBits);              // 0x004D6720
    unsigned short ReadWORD(int nBits);             // 0x004D6750
    unsigned long ReadDWORD(int nBits);             // 0x004D6780
    unsigned long ReadOBJECTIDClient();             // 0x00692630

    void SendPlayerToServerInput(int a, int b, int c, int d);   // 0x00677410
    int SendPlayerToServerModuleLoaded();           // 0x00677490
    void SkipMessage();                             // 0x004D6170

    int HandleServerToPlayerModule_Type10();        // 0x006634A0
    int HandleServerToPlayerModule_Type11();        // 0x00652840
    int HandleServerToPlayerModule_Type12();        // 0x00652860
    int HandleServerToPlayerModule_Type13();        // 0x006529F0
    int HandleServerToPlayerModule_Type14();        // 0x00652AD0
    int HandleServerToPlayerUpdate_GuiContainer();  // 0x0064FEF0
    int HandleServerToPlayerUpdate_GuiElement53();  // 0x00667CC0

    int HandleServerToPlayerCreatureUpdate_Add();   // 0x0064C0A0
    int HandleServerToPlayerDoorUpdate_Add();       // 0x0064CF30
    int HandleServerToPlayerPlaceableUpdate_Add();  // 0x0064D500
    int HandleServerToPlayerItemUpdate_Add(int bFlag, CSWCItem** ppItem);  // 0x0064E7B0
    int HandleServerToPlayerTriggerUpdate_Add();    // 0x0064F4B0
    int HandleServerToPlayerAreaOfEffectUpdate_Add();   // 0x00651680

    int HandleServerToPlayerJournal_Type1();        // 0x006520A0
    int HandleServerToPlayerJournal_Type2();        // 0x00652210
    int HandleServerToPlayerJournal_Type3();        // 0x00652290
    int HandleServerToPlayerJournal_Type4();        // 0x006522E0
    int HandleServerToPlayerJournal_Type5();        // 0x00652330
    int HandleServerToPlayerJournal_Type6();        // 0x006632A0
    int HandleServerToPlayerJournal_Type7();        // 0x00651ED0
    int HandleServerToPlayerJournal_Type8();        // 0x00651FB0
    int HandleServerToPlayerJournal_Type9();        // 0x00652380
    int HandleServerToPlayerJournal_Type12();       // 0x006526E0

    int HandleServerToPlayerDebugInfo_Creature(unsigned long oid);  // 0x00663840
    int HandleServerToPlayerDebugInfo_Item(unsigned long oid);      // 0x00652F20
    int HandleServerToPlayerDebugInfo_Placeable(unsigned long oid); // 0x00653850

    int HandleServerToPlayerCreatureUpdate_ControlObject(unsigned long oid);
    int HandleServerToPlayerPlaceableUpdate_Delete();
    int HandleServerToPlayerItemUpdate_Delete();
    int HandleServerToPlayerUpdate_Add();
    int HandleServerToPlayerJournalMessage(unsigned char nType);
    int HandleServerToPlayerDebugInfo(int nUnused);
    int HandleServerToPlayerDoorUpdate_Update(unsigned long oid, unsigned long nFlags);
    int HandleServerToPlayerPlaceableUpdate_Update(unsigned long oid, unsigned long nFlags);
    int HandleServerToPlayerUpdateVisualEffects(CSWCObject* pObject);
    int HandleServerToPlayerParty(unsigned char nType);
    int HandleServerToPlayerCreatureUpdate_Update(unsigned long oid, unsigned long nFlags);    // 0x006655E0
    int HandleServerToPlayerGenericObjectUpdate_Update(unsigned long oid, unsigned long nFlags);   // 0x00651880
    int HandleServerToPlayerItemUpdate_Update(unsigned long oid, unsigned long nFlags);        // 0x0064EB90
    int HandleServerToPlayerTriggerUpdate_Update(unsigned long oid, unsigned long nFlags);     // 0x0064F430
    int HandleServerToPlayerAreaOfEffectUpdate_Update(unsigned long oid, unsigned long nFlags);    // 0x006517F0
    int HandleServerToPlayerUpdate_Update();
    int HandleServerToPlayerModule(unsigned char nType);
    int HandleServerToPlayerUpdate_GuiElements();
    int HandleServerToPlayerUpdate_GuiInventory();
    int HandleServerToPlayerModuleUpdate_Time();
    int HandleServerToPlayerUpdate_GuiQuickbarItemUseCount();
    int HandleServerToPlayerUpdate_GuiRepository(int nUnused);
    int HandleServerToPlayerItemAppearance(int* pBaseItem, unsigned char* pA,
                                           unsigned char* pB, unsigned char* pC);
};

// FUNCTION: SWKOTOR 0x0064CAF0
// NONMATCH: eax/ecx/edx swapped in four arg temporaries (11 insns).
int CSWCMessage::HandleServerToPlayerCreatureUpdate_ControlObject(unsigned long oid)
{
    CClientExoApp* pApp = g_pAppManager->GetClientExoApp();
    pApp->SetCharacterChangeInProgress(0);
    CSWCCreature* pPrevious = pApp->GetPlayerCreature();
    pApp->SetPlayerCreature(oid);
    if (pPrevious) {
        CSWCPlayer* pPlayer = pApp->GetPlayerByGameObjectID(pPrevious->m_idSelf);
        if (pPlayer) {
            pPlayer->m_oidControlled = oid;
        }
    }
    CSWCCreature* pCreature = pApp->GetCreatureByGameObjectID(oid);
    if (!pCreature) {
        return 0;
    }
    CSWCModule* pModule = pApp->GetModule();
    if (!pModule) {
        return 0;
    }
    if (!pModule->m_pArea) {
        return 0;
    }
    if (pPrevious != pCreature) {
        pApp->GetPlayerByGameObjectID(oid);
        pCreature->m_pVisuals->ResetVisuals();
        pApp->AttachPlayerGameObjectToArea();
        if (g_bCameraFollowFlag) {
            pCreature->m_bCameraFollowA = 1;
            pCreature->m_bCameraFollowB = 1;
        } else {
            pModule->SetCameraTarget(pCreature->m_idSelf, 1);
        }
        if (pApp->GetModule()->m_pInfo && (pApp->GetModule()->m_pInfo->m_nFlags & 8)) {
            Vector vOffset;
            vOffset.x = 0.0f;
            vOffset.y = 0.0f;
            vOffset.z = 0.0f;
            pCreature->ApplyVisualEffect(0x138b, 0, OBJECT_INVALID, OBJECT_INVALID, 0, 0, vOffset);
            if (pPrevious) {
                pPrevious->RemoveVisualEffect(0x138b);
            }
        }
        SendPlayerToServerInput(0xe, 2, 0, 0);
        return 1;
    }
    if (g_bCameraFollowFlag) {
        pCreature->m_bCameraFollowA = 1;
        pCreature->m_bCameraFollowB = 1;
    } else {
        pModule->SetCameraTarget(pCreature->m_idSelf, 1);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0064DA20
int CSWCMessage::HandleServerToPlayerPlaceableUpdate_Delete()
{
    unsigned long oid = ReadOBJECTIDClient();
    int bInstant = ReadBOOL();
    int bResetFade = ReadBOOL();
    unsigned long nTime = ReadDWORD(0x20);
    if (MessageReadOverflow(1)) {
        return 0;
    }
    CGameObject* pObject = g_pAppManager->GetClientExoApp()->GetGameObject(oid);
    if (!pObject) {
        return 0;
    }
    unsigned long nDelay = bInstant ? 0 : 0xafc8;
    if (pObject->AsSWCPlaceable()) {
        CSWCPlaceable* pPlaceable = pObject->AsSWCPlaceable();
        if (bResetFade) {
            pPlaceable->SetFadeState(0);
            pPlaceable->Fade(0, 1, nDelay, 0);
        } else {
            pPlaceable->Fade(0, 1, nDelay, nTime);
        }
        pPlaceable->m_bFading = 1;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0064EAC0
int CSWCMessage::HandleServerToPlayerItemUpdate_Delete()
{
    unsigned long oid = ReadOBJECTIDClient();
    int bDestroy = ReadBOOL();
    if (MessageReadOverflow(1)) {
        return 0;
    }
    CGameObjectArray* pArray = g_pAppManager->GetClientExoApp()->GetGameObjectArray();
    CGameObject* pObject;
    if (bDestroy == 1) {
        if (pArray->Delete(oid, &pObject) != GAME_OBJECT_VALID || !pObject) {
            return 0;
        }
        if (pObject->AsSWCItem()) {
            CSWCItem* pItem = pObject->AsSWCItem();
            if (pItem) {
                delete pItem;
            }
        }
        return 1;
    }
    if (pArray->GetGameObject(oid, &pObject) != GAME_OBJECT_VALID) {
        return 0;
    }
    CSWCItem* pItem = pObject->AsSWCItem();
    if (!pItem) {
        return 0;
    }
    pItem->SetPossessor(0);
    if (!pItem->GetEquippedBy()) {
        pItem->SetEquipped(0);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00656BD0
int CSWCMessage::HandleServerToPlayerUpdate_Add()
{
    unsigned char nType = ReadBYTE(8);
    if (MessageReadOverflow(1)) {
        return 0;
    }
    if (nType == OBJECT_TYPE_CREATURE) {
        return HandleServerToPlayerCreatureUpdate_Add();
    }
    if (nType == OBJECT_TYPE_ITEM) {
        return HandleServerToPlayerItemUpdate_Add(0, 0);
    }
    if (nType == OBJECT_TYPE_TRIGGER) {
        return HandleServerToPlayerTriggerUpdate_Add();
    }
    if (nType == OBJECT_TYPE_DOOR) {
        return HandleServerToPlayerDoorUpdate_Add();
    }
    if (nType == OBJECT_TYPE_AREA_OF_EFFECT) {
        return HandleServerToPlayerAreaOfEffectUpdate_Add();
    }
    if (nType == OBJECT_TYPE_PLACEABLE) {
        return HandleServerToPlayerPlaceableUpdate_Add();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0066A370
int CSWCMessage::HandleServerToPlayerJournalMessage(unsigned char nType)
{
    switch (nType) {
    case 6:
        return HandleServerToPlayerJournal_Type6();
    case 7:
        return HandleServerToPlayerJournal_Type7();
    case 8:
        return HandleServerToPlayerJournal_Type8();
    case 1:
        return HandleServerToPlayerJournal_Type1();
    case 2:
        return HandleServerToPlayerJournal_Type2();
    case 3:
        return HandleServerToPlayerJournal_Type3();
    case 4:
        return HandleServerToPlayerJournal_Type4();
    case 5:
        return HandleServerToPlayerJournal_Type5();
    case 9:
        return HandleServerToPlayerJournal_Type9();
    case 12:
        return HandleServerToPlayerJournal_Type12();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0066A410
int CSWCMessage::HandleServerToPlayerDebugInfo(int nUnused)
{
    unsigned long oid = ReadOBJECTIDClient();
    if (!MessageReadOverflow(1)) {
        CGameObject* pObject = g_pAppManager->GetClientExoApp()->GetGameObject(oid);
        if (pObject) {
            switch (pObject->m_nObjectType) {
            case 5:
                return HandleServerToPlayerDebugInfo_Creature(oid);
            case 6:
                return HandleServerToPlayerDebugInfo_Item(oid);
            case 9:
                return HandleServerToPlayerDebugInfo_Placeable(oid);
            }
        } else if (g_pAppManager->GetClientExoApp()->GetModule()->m_pInfo->m_oidDebug == oid) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0064D420
int CSWCMessage::HandleServerToPlayerDoorUpdate_Update(unsigned long oid, unsigned long nFlags)
{
    CSWCDoor* pDoor = g_pAppManager->GetClientExoApp()->GetDoorByGameObjectID(oid);
    if (!pDoor) {
        return 0;
    }
    if (nFlags & 0x10) {
        int b1 = ReadBOOL();
        int bOpen = ReadBOOL();
        int b3 = ReadBOOL();
        int b4 = ReadBOOL();
        if (MessageReadOverflow(1)) {
            return 0;
        }
        pDoor->m_bState114 = b1;
        pDoor->m_bState108 = b3;
        pDoor->m_bState104 = b4;
        if (bOpen) {
            if (!pDoor->m_bSolid) {
                pDoor->SetSolid(1);
            }
            pDoor->m_bOpen = 1;
        } else {
            if (pDoor->m_bSolid) {
                pDoor->SetSolid(0);
            }
            pDoor->m_bOpen = 0;
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0064DAE0
int CSWCMessage::HandleServerToPlayerPlaceableUpdate_Update(unsigned long oid, unsigned long nFlags)
{
    CSWCPlaceable* pPlaceable = g_pAppManager->GetClientExoApp()->GetPlaceableByGameObjectID(oid);
    if (nFlags & 0x10) {
        int b1 = ReadBOOL();
        int bOpen = ReadBOOL();
        int b3 = ReadBOOL();
        int b4 = ReadBOOL();
        if (MessageReadOverflow(1)) {
            return 0;
        }
        if (pPlaceable) {
            pPlaceable->m_bState124 = b1;
            pPlaceable->m_bState118 = b3;
            pPlaceable->m_bState110 = b4;
            if (bOpen) {
                if (!pPlaceable->m_bSolid) {
                    pPlaceable->SetSolid(1);
                }
                pPlaceable->m_bOpen = 1;
            } else {
                if (pPlaceable->m_bSolid) {
                    pPlaceable->SetSolid(0);
                }
                pPlaceable->m_bOpen = 0;
            }
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0064DBC0
int CSWCMessage::HandleServerToPlayerUpdateVisualEffects(CSWCObject* pObject)
{
    unsigned short nCount = ReadWORD(0x10);
    if (MessageReadOverflow(1)) {
        return 0;
    }
    for (; nCount > 0; nCount--) {
        unsigned char nOp = ReadBYTE(8);
        unsigned short nEffect = ReadWORD(0x10);
        if (MessageReadOverflow(1)) {
            return 0;
        }
        unsigned long oidTarget;
        unsigned char nNode;
        if (GetIsBeam(nEffect)) {
            oidTarget = ReadOBJECTIDClient();
            nNode = ReadBYTE(8);
        } else {
            oidTarget = OBJECT_INVALID;
            nNode = 0;
        }
        if (MessageReadOverflow(1)) {
            return 0;
        }
        if (nOp == 0x41) {
            if (pObject) {
                Vector vOffset;
                vOffset.x = 0.0f;
                vOffset.y = 0.0f;
                vOffset.z = 0.0f;
                pObject->ApplyVisualEffect(nEffect, 0, oidTarget, OBJECT_INVALID, nNode, 0, vOffset);
            }
        } else if (nOp == 0x44) {
            if (pObject) {
                pObject->RemoveVisualEffect(nEffect);
            }
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0064E730
// NONMATCH: [edi] load scheduled after the g_pRules load (1 insn).
int CSWCMessage::HandleServerToPlayerItemAppearance(int* pBaseItem, unsigned char* pA,
                                                    unsigned char* pB, unsigned char* pC)
{
    *pBaseItem = ReadDWORD(0x20);
    if (MessageReadOverflow(1)) {
        return 0;
    }
    *pA = ReadBYTE(8);
    CSWBaseItem* pBase = g_pRules->GetBaseItemArray()->GetBaseItem(*pBaseItem);
    if (pBase->m_nModelType == 1) {
        *pB = ReadBYTE(8);
        *pC = ReadBYTE(8);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0064F1A0
int CSWCMessage::HandleServerToPlayerParty(unsigned char nType)
{
    if (MessageReadOverflow(1)) {
        return 0;
    }
    switch (nType) {
    case 9:
        ReadOBJECTIDClient();
        return 1;
    case 1:
    case 3: {
        CGameObjectArray* pArray = g_pAppManager->GetClientExoApp()->m_pcInternal->m_pGameObjectArray;
        int nCount = ReadDWORD(0x20);
        for (int i = 0; i < nCount; i++) {
            unsigned long oid = ReadOBJECTIDClient();
            CGameObject* pObject;
            if (pArray->GetGameObject(oid, &pObject) == GAME_OBJECT_VALID) {
                pObject->JoinParty();
            }
        }
        break;
    }
    case 4: {
        int nCount = ReadDWORD(0x20);
        for (int i = 0; i < nCount; i++) {
            ReadOBJECTIDClient();
        }
        break;
    }
    default:
        return 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0064FB20
int CSWCMessage::HandleServerToPlayerUpdate_GuiQuickbarItemUseCount()
{
    unsigned char nCount = ReadBYTE(8);
    if (MessageReadOverflow(1)) {
        return 0;
    }
    for (int i = 0; i < nCount; i++) {
        ReadBYTE(8);
        ReadBYTE(8);
        unsigned long oid = ReadOBJECTIDClient();
        unsigned char nProperty = ReadBYTE(8);
        unsigned short nUses = ReadWORD(0x10);
        if (MessageReadOverflow(1)) {
            return 0;
        }
        CSWCItem* pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(oid);
        if (pItem) {
            CSWItemProperty* pProperty = pItem->GetActiveProperty(nProperty);
            if (pProperty && pProperty->m_nUsesType != 1) {
                pItem->SetItemPropertyUsesLeft(nProperty, nUses);
            } else {
                pItem->SetStackSize(nUses);
            }
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0064FDF0
int CSWCMessage::HandleServerToPlayerUpdate_GuiRepository(int nUnused)
{
    unsigned char nOp = ReadBYTE(8);
    if (MessageReadOverflow(1)) {
        return 0;
    }
    if (nOp == 0x41) {
        if (MessageReadOverflow(1)) {
            return 0;
        }
        CSWCItem* pItem;
        HandleServerToPlayerItemUpdate_Add(1, &pItem);
        return 1;
    }
    if (nOp == 0x55) {
        unsigned long oid = ReadOBJECTIDClient();
        unsigned long nStackSize = ReadDWORD(0x20);
        unsigned long nValue = ReadDWORD(0x20);
        if (MessageReadOverflow(1)) {
            return 0;
        }
        CSWCItem* pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(oid);
        if (pItem) {
            pItem->SetStackSize(nStackSize);
            pItem->m_nRepositoryValue = nValue;
        }
        return 1;
    }
    if (nOp == 0x44) {
        unsigned long oid = ReadOBJECTIDClient();
        if (MessageReadOverflow(1)) {
            return 0;
        }
        g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(oid);
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0064FC10
// NONMATCH: original keeps bSwapped memory-homed at [esp+0x10] (ebp a cached copy).
int CSWCMessage::HandleServerToPlayerUpdate_GuiInventory()
{
    unsigned char nOp = ReadBYTE(8);
    if (MessageReadOverflow(1)) {
        return 0;
    }
    int bSwapped = 0;
    unsigned long oidPlayer = g_pAppManager->GetClientExoApp()->GetPlayerCreatureId();
    if (g_pAppManager->GetServerExoApp()->GetServerInfo()->m_nFlags & 1) {
        if (g_pAppManager->GetClientExoApp()->GetGuiInGame()->GetPartyAccessPanelUp(1)) {
            int nNPC = g_pAppManager->GetClientExoApp()->GetGuiInGame()->GetEquipNPC();
            unsigned long oidNPC = g_pAppManager->GetServerExoApp()->GetPartyTable()->GetNPCObject(nNPC, 0, 1);
            if (g_pAppManager->GetClientExoApp()->DontUseThisSetPlayerCreature(oidNPC)) {
                bSwapped = 1;
            }
        }
    }
    if (nOp == 0x41) {
        unsigned long nSlot = ReadDWORD(0x20);
        if (MessageReadOverflow(1)) {
            if (bSwapped) {
                g_pAppManager->GetClientExoApp()->DontUseThisSetPlayerCreature(oidPlayer);
            }
            return 0;
        }
        CSWCItem* pItem;
        if (!HandleServerToPlayerItemUpdate_Add(1, &pItem)) {
            if (bSwapped) {
                g_pAppManager->GetClientExoApp()->DontUseThisSetPlayerCreature(oidPlayer);
            }
            return 0;
        }
        pItem->m_nInventorySlot = nSlot;
        pItem->m_nInventoryIndex = -1;
        CSWCCreature* pCreature = g_pAppManager->GetClientExoApp()->GetPlayerCreature();
        if (pCreature) {
            if (pCreature->GetEquippedItemID(nSlot) != pItem->m_idItem) {
                pCreature->EquipItem(nSlot, pItem);
            }
        }
        if (bSwapped) {
            g_pAppManager->GetClientExoApp()->DontUseThisSetPlayerCreature(oidPlayer);
        }
        return 1;
    }
    if (nOp == 0x44) {
        ReadDWORD(0x20);
        if (MessageReadOverflow(1)) {
            if (bSwapped) {
                g_pAppManager->GetClientExoApp()->DontUseThisSetPlayerCreature(oidPlayer);
            }
            return 0;
        }
        if (bSwapped) {
            g_pAppManager->GetClientExoApp()->DontUseThisSetPlayerCreature(oidPlayer);
        }
        return 1;
    }
    if (bSwapped) {
        g_pAppManager->GetClientExoApp()->DontUseThisSetPlayerCreature(oidPlayer);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x006528B0
int CSWCMessage::HandleServerToPlayerModuleUpdate_Time()
{
    unsigned char nMask = ReadBYTE(8);
    unsigned char bState = nMask & 1;
    unsigned char nState;
    unsigned long nTime;
    if (bState) {
        nState = ReadBYTE(8);
        if (nState != 3 && nState != 4) {
            nTime = 0;
        } else {
            nTime = ReadDWORD(0x20);
        }
    }
    unsigned char bB0 = nMask & 2;
    unsigned long nB0;
    if (bB0) {
        nB0 = ReadBYTE(8);
    }
    unsigned char bAC = nMask & 4;
    unsigned long nAC;
    if (bAC) {
        nAC = ReadBYTE(8);
    }
    unsigned char bA8 = nMask & 8;
    unsigned long nA8;
    if (bA8) {
        nA8 = ReadBYTE(8);
    }
    unsigned char bA4 = nMask & 0x10;
    unsigned long nA4;
    if (bA4) {
        nA4 = ReadDWORD(0x20);
    }
    if (MessageReadOverflow(1)) {
        return 0;
    }
    CSWCModule* pModule = g_pAppManager->GetClientExoApp()->GetModule();
    if (pModule) {
        if (bState) {
            pModule->SetTimeOfDayState(nState, nTime);
        }
        if (bB0) {
            pModule->m_nTimeB0 = nB0;
        }
        if (bAC) {
            pModule->m_nTimeAC = nAC;
        }
        if (bA8) {
            pModule->m_nTimeA8 = nA8;
        }
        if (bA4) {
            pModule->m_nTimeA4 = nA4;
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x006684F0
int CSWCMessage::HandleServerToPlayerModule(unsigned char nType)
{
    if (MessageReadOverflow(1)) {
        return 0;
    }
    switch (nType) {
    case 1: {
        CSWCModule* pModule = g_pAppManager->GetClientExoApp()->GetModule();
        if (!g_bModuleLoadEnabled) {
            SkipMessage();
            return 1;
        }
        if (!pModule) {
            pModule = g_pAppManager->GetClientExoApp()->CreateModule();
        }
        pModule->Reset();
        if (!pModule->LoadModule()) {
            return 0;
        }
        g_pAppManager->GetClientExoApp()->InitializeInGameGUI();
        g_pAppManager->GetClientExoApp()->GetGuiInGame()->Reset();
        return SendPlayerToServerModuleLoaded();
    }
    case 3:
        return HandleServerToPlayerModuleUpdate_Time();
    case 11:
        return HandleServerToPlayerModule_Type11();
    case 12:
        return HandleServerToPlayerModule_Type12();
    case 10:
        return HandleServerToPlayerModule_Type10();
    case 8:
        g_pAppManager->GetClientExoApp()->UnloadModule();
        return 1;
    case 6:
        return 1;
    case 13:
        return HandleServerToPlayerModule_Type13();
    case 14:
        return HandleServerToPlayerModule_Type14();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0066A1F0
int CSWCMessage::HandleServerToPlayerUpdate_GuiElements()
{
    unsigned char nElement = ReadBYTE(8);
    if (MessageReadOverflow(1)) {
        return 0;
    }
    switch (nElement) {
    case 0x49:
        return HandleServerToPlayerUpdate_GuiInventory();
    case 0x53:
        return HandleServerToPlayerUpdate_GuiElement53();
    case 0x43:
        return HandleServerToPlayerUpdate_GuiContainer();
    case 0x42:
        return HandleServerToPlayerUpdate_GuiRepository(1);
    case 0x41:
    case 0x52:
        return HandleServerToPlayerUpdate_GuiRepository(0);
    case 0x51:
        return HandleServerToPlayerUpdate_GuiQuickbarItemUseCount();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0066A2A0
int CSWCMessage::HandleServerToPlayerUpdate_Update()
{
    unsigned char nType = ReadBYTE(8);
    unsigned long oid = ReadOBJECTIDClient();
    unsigned long nFlags = ReadDWORD(0x20);
    if (MessageReadOverflow(1)) {
        return 0;
    }
    if (nType == OBJECT_TYPE_CREATURE) {
        return HandleServerToPlayerCreatureUpdate_Update(oid, nFlags);
    }
    if (!HandleServerToPlayerGenericObjectUpdate_Update(oid, nFlags)) {
        return 0;
    }
    if (nType == OBJECT_TYPE_ITEM) {
        return HandleServerToPlayerItemUpdate_Update(oid, nFlags);
    }
    if (nType == OBJECT_TYPE_TRIGGER) {
        return HandleServerToPlayerTriggerUpdate_Update(oid, nFlags);
    }
    if (nType == OBJECT_TYPE_DOOR) {
        return HandleServerToPlayerDoorUpdate_Update(oid, nFlags);
    }
    if (nType == OBJECT_TYPE_AREA_OF_EFFECT) {
        return HandleServerToPlayerAreaOfEffectUpdate_Update(oid, nFlags);
    }
    if (nType == OBJECT_TYPE_PLACEABLE) {
        return HandleServerToPlayerPlaceableUpdate_Update(oid, nFlags);
    }
    return 0;
}
