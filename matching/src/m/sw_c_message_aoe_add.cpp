class CSWCArea { public: char pad_000[0x184]; void* m_pScene; };
class CSWCModule { public: char pad_000[0x48]; CSWCArea* m_pArea; };
class CGameObject { public:
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
 virtual void v78();
 virtual void v7C();
 virtual void v80();
 virtual void SetArea(CSWCArea*);
};
class CSWCObject : public CGameObject { public: char pad_004[0x100-4]; };
class CSWCAreaOfEffectObject : public CSWCObject { public:
 char pad_100[0x144-0x100]; void* m_pScene;
 char pad_148[0x178-0x148]; unsigned long m_oidCreator;
 CSWCAreaOfEffectObject(); // descriptive ctor, 0x006D8310
 int LoadEffect(unsigned long); // descriptive, 0x006D8490
};
class CGameObjectArray { public: unsigned char AddExternalObject(unsigned long*,CGameObject*,int); };
class CClientAIMaster { public: int AddObject(CSWCObject*); };
class CClientExoApp { public: CSWCModule* GetModule(); CGameObjectArray* GetGameObjectArray(); CClientAIMaster* GetClientAIMaster(); };
class CAppManager { public: void* m_pUnknown00; CClientExoApp* m_pClientExoApp; };
extern CAppManager* g_pAppManager;
extern const unsigned char GAME_OBJECT_VALID;
class CSWCMessage { public:
 unsigned long ReadOBJECTIDClient(); unsigned long ReadDWORD(int); int MessageReadOverflow(int);
 int HandleServerToPlayerAreaOfEffectUpdate_Add();
};
// FUNCTION: SWKOTOR 0x00651680
int CSWCMessage::HandleServerToPlayerAreaOfEffectUpdate_Add()
{
 unsigned long id=ReadOBJECTIDClient();
 unsigned long effect=ReadDWORD(32);
 unsigned long creator=ReadOBJECTIDClient();
 if (MessageReadOverflow(1)) return 0;
 CSWCAreaOfEffectObject* object = new CSWCAreaOfEffectObject;
 if (!object) return 0;
 if (!object->LoadEffect(effect)) return 0;
 CSWCArea* area=g_pAppManager->m_pClientExoApp->GetModule()->m_pArea;
 object->SetObjectID(id);
 object->SetArea(area);
 object->m_oidCreator=creator;
 object->m_pScene=area->m_pScene;
 CGameObjectArray* array=g_pAppManager->m_pClientExoApp->GetGameObjectArray();
 if (array->AddExternalObject(&id,object,1)!=GAME_OBJECT_VALID) return 0;
 CClientAIMaster* ai=g_pAppManager->m_pClientExoApp->GetClientAIMaster();
 ai->AddObject(object);
 return 1;
}
