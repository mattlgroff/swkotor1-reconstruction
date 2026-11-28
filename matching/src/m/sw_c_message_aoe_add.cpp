class CSWCArea { public: char pad_000[0x184]; void* m_pScene; };
class CSWCModule { public: char pad_000[0x48]; CSWCArea* m_pArea; };
#include "shared/game_object.h"
class CSWCObject : public CGameObject {
public:
    virtual void v70();
    virtual void v74();
    virtual void v78();
    virtual void v7C();
    virtual void v80();
    virtual void SetArea(CSWCArea*);
    char pad_0xc[0x100 - 0xc];
};
class CSWCAreaOfEffectObject : public CSWCObject { public:
 char pad_100[0x144-0x100]; void* m_pScene;
 char pad_148[0x178-0x148]; unsigned long m_oidCreator;
 CSWCAreaOfEffectObject(); // descriptive ctor, 0x006D8310
 int LoadEffect(unsigned long); // descriptive, 0x006D8490
};
class CGameObjectArray { public: unsigned char AddExternalObject(unsigned long*,CGameObject*,int); };
class CClientAIMaster { public: int AddObject(CSWCObject*); };
class CClientExoApp { public: CSWCModule* GetModule(); CGameObjectArray* GetGameObjectArray(); CClientAIMaster* GetClientAIMaster(); };
#include "shared/app_manager.h"
#include "shared/globals.h"
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
 object->SetId(id);
 object->SetArea(area);
 object->m_oidCreator=creator;
 object->m_pScene=area->m_pScene;
 CGameObjectArray* array=g_pAppManager->m_pClientExoApp->GetGameObjectArray();
 if (array->AddExternalObject(&id,object,1)!=GAME_OBJECT_VALID) return 0;
 CClientAIMaster* ai=g_pAppManager->m_pClientExoApp->GetClientAIMaster();
 ai->AddObject(object);
 return 1;
}
