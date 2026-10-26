struct Vector { float x,y,z; };
class CResRef { public: char m_ref[16]; CResRef(const char*); };
class CSWCArea { public: char pad_000[0x184]; int m_nAreaState; };
class CSWCModule { public: char pad_000[0x48]; CSWCArea* m_pArea; };
class CSWCObject;
class CGameObject { public:
 virtual ~CGameObject();

 virtual void SetObjectID(unsigned long);
 virtual void v08();
 virtual CSWCObject* AsSWCObject();
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
 virtual void SetPosition(const Vector&);
 virtual void v90();
 virtual void SetEquipped(int);
 virtual void v98();
 virtual void v9C();
 virtual void vA0();
 virtual void vA4();
 virtual void vA8();
 virtual void vAC();
 virtual void vB0();
 virtual void ApplyVisualEffect(unsigned short,int,unsigned long,unsigned long,unsigned char,unsigned char,const Vector&);

 unsigned long m_idSelf; unsigned char m_nObjectType;
};
class CSWCObject : public CGameObject { public:
 char pad_00c[0xe4-0xc]; int m_bFading;
 char pad_0e8[0x100-0xe8];
};
class CSWCCreatureAppearance { public: char pad_000[0x18]; unsigned short m_nAppearanceType; };
class CSWCCreature : public CSWCObject { public:
 char pad_100[0x1dc-0x100]; Vector m_vDesiredPosition;
 char pad_1e8[0x21c-0x1e8]; CSWCCreatureAppearance* m_pAppearance;
 char pad_220[0x310-0x220]; int m_nState310;
 char pad_314[0x44c-0x314];
 CSWCCreature();
 void SetDesiredFadeState(int,int,int,int);
};
class CGameObjectArray { public: unsigned char AddInternalObject(unsigned long*,CGameObject*,int); };
class CClientAIMaster { public: int AddObject(CSWCObject*); };
class CClientExoApp { public: CSWCModule* GetModule(); CGameObjectArray* GetGameObjectArray(); CClientAIMaster* GetClientAIMaster(); CGameObject* GetGameObject(unsigned long); };
class CAppManager { public: void* m_pUnknown00; CClientExoApp* m_pClientExoApp; };
extern CAppManager* g_pAppManager;
extern const unsigned char GAME_OBJECT_VALID;
extern const unsigned char OBJECT_TYPE_CREATURE, OBJECT_TYPE_PLACEABLE, OBJECT_TYPE_DOOR;
#define OBJECT_INVALID 0x7f000000
int GetIsBeam(unsigned short);
class CSWCMessage { public:
 unsigned long ReadOBJECTIDClient(); unsigned short ReadWORD(int); unsigned char ReadBYTE(int); float ReadFLOAT(float,int);
 int MessageReadOverflow(int); int MessageReadUnderflow(int);
 int HandleServerToPlayerCreatureUpdate_VisEffect();
};
// FUNCTION: SWKOTOR 0x0064C3C0
int CSWCMessage::HandleServerToPlayerCreatureUpdate_VisEffect()
{
 Vector sourcePosition={0,0,0};
 Vector position={0,0,0};
 float duration=0.0f;
 unsigned long targetID=ReadOBJECTIDClient();
 unsigned short effect=ReadWORD(16);
 if (MessageReadOverflow(1)) return 0;
 position.x=ReadFLOAT(1.0f,32); position.y=ReadFLOAT(1.0f,32); position.z=ReadFLOAT(1.0f,32);
 if (GetIsBeam(effect)) {
  unsigned long sourceID=ReadOBJECTIDClient();
  unsigned long beamTarget=ReadOBJECTIDClient();
  unsigned char a=ReadBYTE(8);
  unsigned char b=ReadBYTE(8);
  sourcePosition.x=ReadFLOAT(1.0f,32); sourcePosition.y=ReadFLOAT(1.0f,32); sourcePosition.z=ReadFLOAT(1.0f,32);
  if (targetID==OBJECT_INVALID) duration=ReadFLOAT(1.0f,32);
  if (MessageReadOverflow(1)) return 0;
  CGameObject* source=g_pAppManager->m_pClientExoApp->GetGameObject(sourceID);
  if (!source || !(source->m_nObjectType==OBJECT_TYPE_CREATURE || source->m_nObjectType==OBJECT_TYPE_PLACEABLE || source->m_nObjectType==OBJECT_TYPE_DOOR)) {

   CSWCCreature* dummy=new CSWCCreature;
   if (!dummy) return 0;
   CResRef model("c_dummy");
   dummy->m_nState310=0;
   if (dummy->LoadModel(model,255,0)) {
    CSWCArea* area=g_pAppManager->m_pClientExoApp->GetModule()->m_pArea;
    if (area) {
     unsigned long dummyID;
     CGameObjectArray* array=g_pAppManager->m_pClientExoApp->GetGameObjectArray();
     // Preserve the first source-dummy's native inverted status test.
     if (!array->AddInternalObject(&dummyID,dummy,0)) return 0;
     dummy->SetObjectID(dummyID);
     dummy->SetArea(area);
     CClientAIMaster* ai=g_pAppManager->m_pClientExoApp->GetClientAIMaster();
     ai->AddObject(dummy);
     dummy->SetEquipped(area->m_nAreaState);
     dummy->SetPosition(sourcePosition);
     dummy->m_vDesiredPosition=sourcePosition;
     dummy->m_pAppearance->m_nAppearanceType=0xc9;
    }
   }
   dummy->SetDesiredFadeState(1,1,duration==0.0f ? 6000 : (int)(duration*1000.0f),0);
   dummy->m_bFading=1;

  }
  CGameObject* existing=g_pAppManager->m_pClientExoApp->GetGameObject(targetID);
  CSWCObject* target;
  if (existing && (existing->m_nObjectType==OBJECT_TYPE_CREATURE || existing->m_nObjectType==OBJECT_TYPE_PLACEABLE || existing->m_nObjectType==OBJECT_TYPE_DOOR)) target=existing->AsSWCObject();
  else {

   CSWCCreature* dummy=new CSWCCreature;
   if (!dummy) return 0;
   CResRef model("c_dummy");
   dummy->m_nState310=0;
   if (dummy->LoadModel(model,255,0)) {
    CSWCArea* area=g_pAppManager->m_pClientExoApp->GetModule()->m_pArea;
    if (area) {
     unsigned long dummyID;
     CGameObjectArray* array=g_pAppManager->m_pClientExoApp->GetGameObjectArray();
     // Preserve the first source-dummy's native inverted status test.
     if (array->AddInternalObject(&dummyID,dummy,0)!=GAME_OBJECT_VALID) return 0;
     dummy->SetObjectID(dummyID);
     dummy->SetArea(area);
     CClientAIMaster* ai=g_pAppManager->m_pClientExoApp->GetClientAIMaster();
     ai->AddObject(dummy);
     dummy->SetEquipped(area->m_nAreaState);
     dummy->SetPosition(position);
     dummy->m_vDesiredPosition=position;
     dummy->m_pAppearance->m_nAppearanceType=0xc9;
    }
   }
   dummy->SetDesiredFadeState(1,1,duration==0.0f ? 6000 : (int)(duration*1000.0f),0);
   dummy->m_bFading=1;

   target=dummy;
  }
  int instant=duration==0.0f;
  Vector zero={0,0,0};
  if (targetID==OBJECT_INVALID) {
   target->ApplyVisualEffect(effect,instant,sourceID,beamTarget,a,b,position);
   return 1;
  } else {
   target->ApplyVisualEffect(effect,instant,sourceID,beamTarget,a,b,zero);
   return 1;
  }
 }
 if (MessageReadOverflow(1) || MessageReadUnderflow(1)) return 0;
 CSWCObject* target;
 if (targetID==OBJECT_INVALID) {

   CSWCCreature* dummy=new CSWCCreature;
   if (!dummy) return 0;
   CResRef model("c_dummy");
   dummy->m_nState310=0;
   if (dummy->LoadModel(model,255,0)) {
    CSWCArea* area=g_pAppManager->m_pClientExoApp->GetModule()->m_pArea;
    if (area) {
     unsigned long dummyID;
     CGameObjectArray* array=g_pAppManager->m_pClientExoApp->GetGameObjectArray();
     // Preserve the first source-dummy's native inverted status test.
     if (array->AddInternalObject(&dummyID,dummy,0)!=GAME_OBJECT_VALID) return 0;
     dummy->SetObjectID(dummyID);
     dummy->SetArea(area);
     CClientAIMaster* ai=g_pAppManager->m_pClientExoApp->GetClientAIMaster();
     ai->AddObject(dummy);
     dummy->SetEquipped(area->m_nAreaState);
     dummy->SetPosition(position);
     dummy->m_vDesiredPosition=position;
     dummy->m_pAppearance->m_nAppearanceType=0xc9;
    }
   }
   dummy->SetDesiredFadeState(1,1,10000,0);
   dummy->m_bFading=1;

  target=dummy;
 } else {
  target=static_cast<CSWCObject*>(g_pAppManager->m_pClientExoApp->GetGameObject(targetID));
  if (!target) return 0;
 }
 target->ApplyVisualEffect(effect,1,OBJECT_INVALID,OBJECT_INVALID,0,0,position);
 return 1;
}
