// Extended interfaces local to message lifecycle/update handlers.
#ifndef M_SW_C_MESSAGE_UPDATES_H
#define M_SW_C_MESSAGE_UPDATES_H
struct Vector { float x,y,z; };
class CExoString { public: char* m_sString; unsigned long m_nBufferLength;
 CExoString(); CExoString(const char*); ~CExoString(); };
class CResRef { public: char m_ref[16]; CResRef(const CExoString&); CResRef(const char*); };
class C2DA { public:
 int GetINTEntry(int, const CExoString&, int*);
 int GetCExoStringEntry(int, const CExoString&, CExoString*);
};
class CTwoDimArrays { public: char pad_000[0xc]; C2DA* m_pAppearance;
 char pad_010[0x8c-0x10]; C2DA* m_pTraps;
 char pad_090[0xd4-0x90]; C2DA* m_pBodyBag;
 C2DA* GetAppearance() { return m_pAppearance; }
 C2DA* GetTraps() { return m_pTraps; }
 C2DA* GetBodyBag() { return m_pBodyBag; } };
class CSWBaseItem { public: char pad_000[0x18]; unsigned char m_nModelType; char pad_019[0x48-0x19]; void* m_pModelHook; };
class CSWBaseItemArray { public: CSWBaseItem* GetBaseItem(int); };
class CSWRules { public: char pad_000[0x34]; CSWBaseItemArray* m_pBaseItemArray; char pad_038[0xb8-0x38]; CTwoDimArrays* m_p2DArrays;
 CSWBaseItemArray* GetBaseItemArray() { return m_pBaseItemArray; }
 CTwoDimArrays* Get2DArrays() { return m_p2DArrays; } };
extern CSWRules* g_pRules;
class CSWCArea { public: char pad_000[0x184]; int m_nAreaState; void AddCorpse(unsigned long); };
class CSWCModule { public: char pad_000[0x48]; CSWCArea* m_pArea; };
class CSWSCreature;
class CSWCItem;
class CSWCCreatureAppearance { public: char pad_000[0x18]; unsigned short m_nAppearanceType; char GetGender(); }; // descriptive 0x00696CA0
class CSWCCreature {
public:
 virtual ~CSWCCreature();
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
    virtual int LoadModel(const CResRef&,unsigned char,int);
    virtual void v7C();
    virtual void v80();
    virtual void SetArea(CSWCArea*);
    virtual void v88();
    virtual void v8C();
    virtual void v90();
    virtual void SetEquipped(int);
    virtual void v98();
    virtual void v9C();
    virtual void vA0();
    virtual void vA4();
    virtual void vA8();
    virtual void vAC();
    virtual int AttachHook(unsigned char,int,const CResRef&);
    virtual void vB4();
    virtual void vB8();
    virtual void ClearActions();
 unsigned long m_idSelf;
 char pad_008[0x20-8]; CSWCArea* m_pArea;
 char pad_024[0xe4-0x24]; int m_bFading;
 char pad_0e8[0x1b0-0xe8]; int m_nState1B0;
 char pad_1b4[0x21c-0x1b4];
 CSWCCreatureAppearance* m_pAppearance;
 CSWCCreatureAppearance* GetAppearance() { return m_pAppearance; }
 unsigned long GetEquippedItemID(unsigned long);
 void EquipItem(unsigned long,CSWCItem*);
 void SetLastInstantEquip(unsigned long,int); // descriptive, 0x0060CD50
 CSWSCreature* GetServerCreature();
 void SetFadeState(int);
 void SetDesiredFadeState(int,int,int,int);
 void SetCombatState(int); // descriptive name, 0x006182A0
};
class CSWCTrigger {
public: virtual ~CSWCTrigger();
 char pad_004[0x24-4]; Vector m_vPosition;
 char pad_030[0x148-0x30]; unsigned char m_nTrapType;
 void RemoveFromRenderList(); // descriptive, 0x00691D80
 void PlayAnimation(unsigned short,int,int); // descriptive, 0x006911B0
};
class CSWCObject { public: virtual ~CSWCObject(); char pad_004[8]; int m_nBaseItem; };
class CSWItem { public:
 virtual void v00();
 virtual void SetObjectID(unsigned long);
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
 virtual int LoadModel(const CResRef&,unsigned char,int);
 virtual void v7C();
 virtual void v80();
 virtual void SetArea(CSWCArea*);
 virtual void v88();
 virtual void v8C();
 virtual void v90();
 virtual void SetEquipped(int);
 virtual void v98();
 virtual void v9C();
 virtual void vA0();
 virtual void vA4();
 virtual void vA8();
 virtual void vAC();
 virtual int AttachHook(unsigned char,int,const CResRef&);
 void SetVisibilityOnSetArea(CSWCArea*); // descriptive, 0x0063E350
};
class CSWCItem : public CSWCObject, public CSWItem { public:
 unsigned long m_idItem;
 char pad_018[0x128-0x18]; CResRef m_resModel;
 char pad_138[0x148-0x138]; int m_nInventoryState; unsigned long m_nPendingSlot;
 unsigned long m_nState150; unsigned long m_nInventorySlot;
 char pad_158[0x164-0x158]; unsigned long m_oidPossessor; int m_nState168;
 int PlayDropSound();
 CSWCCreature* GetEquippedBy();
 void SetEquippedBy(CSWCCreature*);
};
class CClientAIMaster { public: int AddObject(CSWItem*); };
class CGameObject {
public: virtual ~CGameObject();
 virtual void v04(); virtual void v08(); virtual void v0C();
 virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
 virtual void v20(); virtual void v24(); virtual void v28(); virtual CSWCCreature* AsSWCCreature();
 virtual void v30(); virtual CSWCItem* AsSWCItem(); virtual void v38();
 virtual CSWCTrigger* AsSWCTrigger();
};
class CGameObjectArray { public: unsigned char Delete(unsigned long,CGameObject**);
 unsigned char GetGameObject(unsigned long,CGameObject**);
 unsigned char AddExternalObject(unsigned long*,CSWItem*,int); };
class CSWCMessage;
class CClientExoApp { public:
 CSWCMessage* GetSWCMessage();
 CGameObject* GetGameObject(unsigned long);
 CSWCCreature* GetPlayerCreature();
 CSWCModule* GetModule();
 CClientAIMaster* GetClientAIMaster();
 CSWCCreature* GetCreatureByGameObjectID(unsigned long);
 CSWCTrigger* GetTriggerByGameObjectID(unsigned long);
 CGameObjectArray* GetGameObjectArray();
};
class CAppManager { public: void* m_pUnknown00; CClientExoApp* m_pClientExoApp; };
extern CAppManager* g_pAppManager;
extern const unsigned char GAME_OBJECT_VALID;
class CExoSound { public: int PlayOneShotSoundAt(const CResRef&,Vector,float,unsigned long,unsigned long,unsigned long,unsigned long,unsigned long); };
extern CExoSound* g_pExoSound;
void SwitchWeaponEvent(unsigned long,const char*,CSWCCreature*); // descriptive 0x00610F40
class CSWCMessage {
public: unsigned long ReadOBJECTIDClient(); int ReadBOOL(); unsigned long ReadDWORD(int);
 unsigned char ReadBYTE(int); int MessageReadOverflow(int); int MessageReadUnderflow(int);
 CSWCItem* CreateItem(unsigned long,unsigned char,char);
 int HandleServerToPlayerItemAppearance(int*,unsigned char*,unsigned char*,unsigned char*);
 int HandleServerToPlayerActiveItemProperties(unsigned long);
 int HandleServerToPlayerItemUpdate_Add(int,CSWCItem**);
 int HandleServerToPlayerInventory(unsigned char);
 int SendPlayerToServer_0C_01(unsigned long,unsigned long);
 int HandleServerToPlayerItemScript_Add();
 int HandleServerToPlayerCreatureUpdate_Delete();
 int HandleServerToPlayerTriggerUpdate_Delete();
};
#endif
