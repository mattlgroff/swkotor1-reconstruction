// Object-update message handlers. Partial interfaces preserve original base offsets.
#define OBJECT_INVALID 0x7f000000
extern const unsigned char GAME_OBJECT_VALID;
extern const unsigned char OBJECT_TYPE_CREATURE;
extern const unsigned char OBJECT_TYPE_ITEM;
extern const unsigned char OBJECT_TYPE_TRIGGER;
extern const unsigned char OBJECT_TYPE_DOOR;
extern const unsigned char OBJECT_TYPE_AREA_OF_EFFECT;
extern const unsigned char OBJECT_TYPE_PLACEABLE;
class CGameObject;
class CSWCDoor { public: virtual ~CSWCDoor(); };
class CGameObject {
public:
    virtual ~CGameObject();
    virtual void v04(); virtual void v08(); virtual void v0C(); virtual void v10();
    virtual CSWCDoor* AsSWCDoor();
};
class CGameObjectArray {
public:
    unsigned char Delete(unsigned long, CGameObject**);
};
class CResRef { public: CResRef(); char m_ref[16]; };
class CSWCArea { public: char pad_000[0x184]; int m_nAreaState; };
class CSWCModule { public: char pad_000[0x48]; CSWCArea* m_pArea; };
class CSWItem {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0C();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1C();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2C();
    virtual void v30();
    virtual void v34();
    virtual void v38();
    virtual void v3C();
    virtual void v40();
    virtual void v44();
    virtual void v48();
    virtual void v4C();
    virtual void v50();
    virtual void v54();
    virtual void v58();
    virtual void v5C();
    virtual void v60();
    virtual void v64();
    virtual void v68();
    virtual void v6C();
    virtual void v70();
    virtual void v74();
    virtual void v78();
    virtual void v7C();
    virtual void v80();
    virtual void SetArea(CSWCArea*);
    virtual void v88();
    virtual void v8C();
    virtual void v90();
    virtual void SetEquipped(int);
};
class CSWCObject { public: virtual ~CSWCObject(); char pad_004[0x10-4]; };
class CSWCItem : public CSWCObject, public CSWItem {};
class CSWCTrigger { public: char pad_000[0x108]; int m_bHighlightable; void UpdateTriggerColor(int); };
class CClientAIMaster;
class CClientExoApp {
public:
    CSWCTrigger* GetTriggerByGameObjectID(unsigned long);
    CSWCItem* GetItemByGameObjectID(unsigned long);
    CSWCModule* GetModule();
    CGameObjectArray* GetGameObjectArray();
    CClientAIMaster* GetClientAIMaster();
};
class CAppManager { public: void* m_pUnknown00; CClientExoApp* m_pClientExoApp; };
extern CAppManager* g_pAppManager;
class CSWCMessage {
public:
    int ReadBOOL();
    unsigned char ReadBYTE(int);
    unsigned long ReadOBJECTIDClient();
    int MessageReadOverflow(int);
    int HandleServerToPlayerItemUpdate_Update(unsigned long, unsigned long);
    int HandleServerToPlayerTriggerUpdate_Update(unsigned long, unsigned long);
    int HandleServerToPlayerDoorUpdate_Delete();
    int HandleServerToPlayerUpdate_Delete();
    int HandleServerToPlayerCreatureUpdate_Delete();
    int HandleServerToPlayerItemUpdate_Delete();
    int HandleServerToPlayerTriggerUpdate_Delete();
    int HandleServerToPlayerAreaOfEffectUpdate_Delete();
    int HandleServerToPlayerPlaceableUpdate_Delete();
};

// FUNCTION: SWKOTOR 0x0064EB90
int CSWCMessage::HandleServerToPlayerItemUpdate_Update(unsigned long id, unsigned long mask)
{
    CResRef unused;
    CClientExoApp* client = g_pAppManager->m_pClientExoApp;
    CSWCItem* item = client->GetItemByGameObjectID(id);
    if (!item) return 0;
    CSWCArea* area = g_pAppManager->m_pClientExoApp->GetModule()->m_pArea;
    item->SetEquipped(area->m_nAreaState);
    item->SetArea(area);
    return 1;
}

// FUNCTION: SWKOTOR 0x0064F430
int CSWCMessage::HandleServerToPlayerTriggerUpdate_Update(unsigned long id, unsigned long mask)
{
    CClientExoApp* client = g_pAppManager->m_pClientExoApp;
    CSWCTrigger* trigger = client->GetTriggerByGameObjectID(id);
    if (!trigger) return 0;
    if ((mask & 0x10) && trigger->m_bHighlightable) {
        int highlighted = ReadBOOL();
        if (MessageReadOverflow(1)) return 0;
        if (highlighted) trigger->UpdateTriggerColor(0);
        else trigger->UpdateTriggerColor(1);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0064D3A0
int CSWCMessage::HandleServerToPlayerDoorUpdate_Delete()
{
    unsigned long id = ReadOBJECTIDClient();
    if (MessageReadOverflow(1)) return 0;
    CGameObject* object;
    CGameObjectArray* array = g_pAppManager->m_pClientExoApp->GetGameObjectArray();
    if (array->Delete(id, &object) != GAME_OBJECT_VALID) return 0;
    if (!object) return 0;
    if (object->AsSWCDoor()) {
        g_pAppManager->m_pClientExoApp->GetClientAIMaster();
        CSWCDoor* door = object->AsSWCDoor();
        if (door) delete door;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00656C90
int CSWCMessage::HandleServerToPlayerUpdate_Delete()
{
    unsigned char type = ReadBYTE(8);
    if (MessageReadOverflow(1)) return 0;
    if (type == OBJECT_TYPE_CREATURE) return HandleServerToPlayerCreatureUpdate_Delete();
    if (type == OBJECT_TYPE_ITEM) return HandleServerToPlayerItemUpdate_Delete();
    if (type == OBJECT_TYPE_TRIGGER) return HandleServerToPlayerTriggerUpdate_Delete();
    if (type == OBJECT_TYPE_DOOR) return HandleServerToPlayerDoorUpdate_Delete();
    if (type == OBJECT_TYPE_AREA_OF_EFFECT) return HandleServerToPlayerAreaOfEffectUpdate_Delete();
    if (type == OBJECT_TYPE_PLACEABLE) return HandleServerToPlayerPlaceableUpdate_Delete();
    return 0;
}
