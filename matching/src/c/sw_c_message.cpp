// CSWCMessage (b0060_client_dtors_forwarders, b0062 message leaves,
// b0063 client gui panels, b0064 hide/message handlers, b0065 virtual leaves).

#define OBJECT_INVALID 0x7f000000

extern const unsigned char OBJECT_TYPE_CREATURE;    // 0x00746634
extern const unsigned char GAME_OBJECT_VALID;       // 0x0074666C

class CGameObject;

class CSWCAreaOfEffectObject {
public:
    void Destroy();                                 // 0x006D9350
};

class CSWCCreature {
public:
    void StripEquippedItems();                      // 0x0060E010
    void PlayVoiceChat(unsigned short nType, int b);    // 0x00611470
};

class CSWCItem {
public:
    virtual ~CSWCItem();

    char pad_0x4[0x10];
    unsigned long m_idSelf;                         // 0x14
};

class CGameObjectArray {
public:
    unsigned char Delete(unsigned long id, CGameObject** ppObject);     // 0x004D81A0
};

class CGuiInGame {
public:
    int ShowContainerGui(unsigned long oid, int b); // 0x0062D4B0
    void ShowLevelUp(int b);                        // 0x00628650

    char pad_0x0[0x108];
    int m_bItemCreateAvailable;                     // 0x108
};

struct CClientSettings {
    unsigned char m_nFlags;                         // 0x00
};

class CClientExoAppInternal {
public:
    void SaveLoadComplete();                        // 0x005F29F0
};

class CClientExoApp {
public:
    void* m_pVtable;                                // 0x00
    CClientExoAppInternal* m_pcInternal;            // 0x04

    CSWCAreaOfEffectObject* GetAreaOfEffectByGameObjectID(unsigned long oid);   // 0x005ED610
    CSWCCreature* GetCreatureByGameObjectID(unsigned long oid);     // 0x005ED5B0
    CSWCItem* GetItemByGameObjectID(unsigned long oid);             // 0x005ED5A0
    CGameObjectArray* GetGameObjectArray();         // 0x005ED570
    CGuiInGame* GetGuiInGame();                     // 0x005ED690
    CClientSettings* GetSettings();                 // 0x005ED700
    void SetDebugMode(int b);                       // 0x005ED850
};

class CAppManager {
public:
    void* m_pUnknown00;                             // 0x00
    CClientExoApp* m_pClientExoApp;                 // 0x04

    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CSWCMessage {
public:
    CSWCMessage();
    virtual ~CSWCMessage();

    // CSWMessage / CResGFF-style readers
    int MessageReadOverflow(int bFlag);             // 0x004D6230
    int MessageReadUnderflow(int bFlag);            // 0x004D6240
    int ReadBOOL();                                 // 0x004D66F0
    unsigned char ReadBYTE(int nBits);              // 0x004D6720
    unsigned long ReadDWORD(int nBits);             // 0x004D6780
    unsigned long ReadOBJECTIDClient();             // 0x00692630

    int HandleServerToPlayerAreaOfEffectUpdate_Update(unsigned long oid, int nUnused);
    int HandleServerToPlayerUpdate_Appearance();
    int HandleServerToPlayerUpdate_Appearance_Creature();   // 0x00655E40
    int HandleServerToPlayerSafeProjectile(unsigned char nKind);
    int HandleServerToPlayerSafeProjectile_Fire();          // 0x00650B90
    int HandleServerToPlayerSafeProjectile_Hit();           // 0x006501B0

    int HandleServerToPlayerAreaOfEffectUpdate_Delete();
    int HandleServerToPlayerCreatureUpdate_StripEquippedItems();
    int HandleServerToPlayerGuiTimingEvent(unsigned char nKind);
    int HandleServerToPlayerGuiTimingEvent_Info();
    int HandleServerToPlayerLevelUpMessage(unsigned char nKind);
    int HandleServerToPlayerGuiContainerObjectMessage(unsigned char nKind);
    int HandleServerToPlayerItemUpdate_DestroyItem();
    int HandleServerToPlayerVoiceChatMessage(unsigned char nKind);
    int HandleServerToPlayerSaveLoad(unsigned char nKind);
    int HandleServerToPlayerGUICharacterSheet(unsigned char nKind);
    int HandleServerToPlayerCheatMessage(unsigned char nKind);
    int HandleServerToPlayerCheat_Unknown();        // 0x00651C10
    int HandleServerToPlayerCheat_DebugMode()
    {
        int bDebug = ReadBOOL();
        if (MessageReadOverflow(1)) {
            return 0;
        }
        g_pAppManager->GetClientExoApp()->SetDebugMode(bDebug);
        return 1;
    }
};

// Defined only to emit the vtable and its scalar deleting destructor.
CSWCMessage::CSWCMessage()
{
}

// FUNCTION: SWKOTOR 0x00654CA0 SYMBOL: ??_GCSWCMessage@@UAEPAXI@Z
// CSWCMessage::`scalar deleting destructor' (compiler generated).

// FUNCTION: SWKOTOR 0x006517F0
int CSWCMessage::HandleServerToPlayerAreaOfEffectUpdate_Update(unsigned long oid, int nUnused)
{
    if (g_pAppManager->GetClientExoApp()->GetAreaOfEffectByGameObjectID(oid)) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00656C60
int CSWCMessage::HandleServerToPlayerUpdate_Appearance()
{
    unsigned char nType = ReadBYTE(8);
    if (MessageReadOverflow(1) || nType != OBJECT_TYPE_CREATURE) {
        return 0;
    }
    return HandleServerToPlayerUpdate_Appearance_Creature();
}

// FUNCTION: SWKOTOR 0x006631C0
int CSWCMessage::HandleServerToPlayerSafeProjectile(unsigned char nKind)
{
    int bResult = 0;
    if (nKind == 1) {
        bResult = HandleServerToPlayerSafeProjectile_Fire();
    } else if (nKind == 2) {
        bResult = HandleServerToPlayerSafeProjectile_Hit();
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x006517B0
int CSWCMessage::HandleServerToPlayerAreaOfEffectUpdate_Delete()
{
    unsigned long oid = ReadOBJECTIDClient();
    if (MessageReadOverflow(1)) {
        return 0;
    }
    CSWCAreaOfEffectObject* pAoE = g_pAppManager->GetClientExoApp()->GetAreaOfEffectByGameObjectID(oid);
    if (pAoE) {
        pAoE->Destroy();
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00651AF0
int CSWCMessage::HandleServerToPlayerCreatureUpdate_StripEquippedItems()
{
    unsigned long oid = ReadOBJECTIDClient();
    if (MessageReadOverflow(1)) {
        return 0;
    }
    CSWCCreature* pCreature = g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(oid);
    if (!pCreature) {
        return 0;
    }
    pCreature->StripEquippedItems();
    return 1;
}

// FUNCTION: SWKOTOR 0x00665590
int CSWCMessage::HandleServerToPlayerGuiTimingEvent(unsigned char nKind)
{
    if (MessageReadOverflow(1)) {
        return 0;
    }
    switch (nKind) {
    case 1:
        HandleServerToPlayerGuiTimingEvent_Info();
        break;
    }
    if (MessageReadOverflow(1) || MessageReadUnderflow(1)) {
        return 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x006527E0
int CSWCMessage::HandleServerToPlayerLevelUpMessage(unsigned char nKind)
{
    if (MessageReadOverflow(1)) {
        return 0;
    }
    switch (nKind) {
    case 1: {
        CGuiInGame* pGui = g_pAppManager->GetClientExoApp()->GetGuiInGame();
        if (pGui) {
            pGui->ShowLevelUp(0);
        }
        break;
    }
    }
    if (MessageReadOverflow(1) || MessageReadUnderflow(1)) {
        return 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00651810
int CSWCMessage::HandleServerToPlayerGuiContainerObjectMessage(unsigned char nKind)
{
    unsigned long oid = ReadOBJECTIDClient();
    unsigned long nValue = ReadDWORD(0x20);
    if (MessageReadOverflow(1) || MessageReadUnderflow(1)) {
        return 0;
    }
    if (nKind == 1 && oid != OBJECT_INVALID) {
        g_pAppManager->GetClientExoApp()->GetGuiInGame()->ShowContainerGui(oid, nValue);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00651B30
// NONMATCH: deleting-dtor vtable load uses edx instead of eax (5 insns).
int CSWCMessage::HandleServerToPlayerItemUpdate_DestroyItem()
{
    unsigned long oid = ReadOBJECTIDClient();
    if (MessageReadOverflow(1)) {
        return 0;
    }
    CSWCItem* pItem = g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(oid);
    if (!pItem) {
        return 0;
    }
    CGameObject* pObject;
    CGameObjectArray* pArray = g_pAppManager->GetClientExoApp()->GetGameObjectArray();
    if (pArray->Delete(pItem->m_idSelf, &pObject) != GAME_OBJECT_VALID) {
        return 0;
    }
    delete pItem;
    return 1;
}

// FUNCTION: SWKOTOR 0x00651BA0
int CSWCMessage::HandleServerToPlayerVoiceChatMessage(unsigned char nKind)
{
    if (nKind == 1) {
        unsigned long oid = ReadOBJECTIDClient();
        unsigned char nType = ReadBYTE(8);
        if (MessageReadOverflow(1) || MessageReadUnderflow(1)) {
            return 0;
        }
        CSWCCreature* pCreature = g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(oid);
        if (pCreature) {
            pCreature->PlayVoiceChat(nType, 1);
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00654900
int CSWCMessage::HandleServerToPlayerSaveLoad(unsigned char nKind)
{
    switch (nKind) {
    case 1: {
        unsigned char nType = ReadBYTE(8);
        unsigned long bSuccess = ReadDWORD(4);
        if (MessageReadOverflow(1) || MessageReadUnderflow(1)) {
            return 0;
        }
        if (bSuccess && nType != 2) {
            g_pAppManager->GetClientExoApp()->m_pcInternal->SaveLoadComplete();
        }
        break;
    }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00654A30
int CSWCMessage::HandleServerToPlayerGuiTimingEvent_Info()
{
    if (ReadBOOL()) {
        ReadDWORD(0x20);
        ReadBYTE(8);
    }
    if (MessageReadOverflow(1)) {
        return 0;
    }
    if (g_pAppManager->GetClientExoApp()->GetSettings()->m_nFlags & 0x10) {
        CGuiInGame* pGui = g_pAppManager->GetClientExoApp()->GetGuiInGame();
        if (!pGui || !pGui->m_bItemCreateAvailable) {
            return 0;
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00654AA0
int CSWCMessage::HandleServerToPlayerGUICharacterSheet(unsigned char nKind)
{
    if (MessageReadOverflow(1)) {
        return 0;
    }
    switch (nKind) {
    case 2:
        ReadOBJECTIDClient();
        if (MessageReadOverflow(1)) {
            return 0;
        }
        g_pAppManager->GetClientExoApp()->GetGuiInGame();
        break;
    }
    if (MessageReadOverflow(1) || MessageReadUnderflow(1)) {
        return 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00663220
int CSWCMessage::HandleServerToPlayerCheatMessage(unsigned char nKind)
{
    switch (nKind) {
    case 0x15:
        return HandleServerToPlayerCheat_DebugMode();
    case 0x0c:
        ReadOBJECTIDClient();
        ReadBOOL();
        return MessageReadOverflow(1) == 0;
    case 0x0b:
        return HandleServerToPlayerCheat_Unknown();
    }
    return 0;
}
