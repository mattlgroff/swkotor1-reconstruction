struct Vector { float x,y,z; };
class CResRef { public: char m_ref[16]; CResRef(); CResRef operator=(const CResRef&); };
class CSWCDoor { public: void SetState(int); }; // descriptive 0x00683D40
class CSWCObject { public:
 virtual void v00();
 virtual void v04();
 virtual void v08();
 virtual CSWCObject* AsSWCObject();
 virtual void v10();
 virtual CSWCDoor* AsSWCDoor();
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
 virtual void v84();
 virtual void SetOrientation(const Vector&);
 virtual void SetPosition(const Vector&);
 virtual void v90();
 virtual void v94();
 virtual void v98();
 virtual int PlayAnimation(unsigned short,int,int);
 virtual void vA0();
 virtual void vA4();
 virtual void vA8();
 virtual void vAC();
 virtual void vB0();
 virtual void vB4();
 virtual void vB8();
 virtual void vBC();
 virtual void vC0();
 virtual void vC4();
 virtual void vC8();
 virtual void vCC();
 virtual void vD0();
 virtual void vD4();
 virtual void vD8();
 virtual void vDC();
 virtual void vE0();
 virtual void vE4();
 virtual void vE8();
 virtual void SetPortrait(const CResRef&);
 virtual void vF0();
 virtual void SetPortraitId(unsigned short);
 char pad_004[4]; unsigned char m_nObjectType;
 char pad_009[0x4c-9]; float m_fAnimationSpeed;
};
class CGameObject { public: virtual void v00(); virtual void v04(); virtual void v08(); virtual CSWCObject* AsSWCObject(); };
class CClientExoApp { public: CGameObject* GetGameObject(unsigned long); };
class CAppManager { public: void* m_pUnknown00; CClientExoApp* m_pClientExoApp; };
extern CAppManager* g_pAppManager;
extern const unsigned char OBJECT_TYPE_DOOR;
class CSWCMessage { public:
 float ReadFLOAT(float,int); unsigned short ReadWORD(int); CResRef ReadCResRef(int);
 int MessageReadOverflow(int); int HandleServerToPlayerUpdateVisualEffects(CSWCObject*);
 int HandleServerToPlayerGenericObjectUpdate_Update(unsigned long,unsigned long);
};
// FUNCTION: SWKOTOR 0x00651880
int CSWCMessage::HandleServerToPlayerGenericObjectUpdate_Update(unsigned long id,unsigned long mask)
{
 CResRef portraitRef;
 Vector position = {0,0,0}, orientation = {0,0,0};
 CSWCObject* target = 0;
 CClientExoApp* client = g_pAppManager->m_pClientExoApp;
 CGameObject* object = client->GetGameObject(id);
 if (mask & 1) { position.x=ReadFLOAT(1.0f,32); position.y=ReadFLOAT(1.0f,32); position.z=ReadFLOAT(1.0f,32); }
 if (mask & 2) { orientation.x=ReadFLOAT(1.0f,32); orientation.y=ReadFLOAT(1.0f,32); orientation.z=ReadFLOAT(1.0f,32); }
 unsigned short portrait;
 if (mask & 0x20) { portrait=ReadWORD(16); if (portrait>=0xfffe) portraitRef=ReadCResRef(16); }
 float speed;
 unsigned short animation;
 if (mask & 4) speed=ReadFLOAT(1.0f,32);
 if (mask & 4) animation=ReadWORD(16);
 if (MessageReadOverflow(1)) return 0;
 if (object && object->AsSWCObject()) {
  target=object->AsSWCObject();
  if (mask & 1) target->SetPosition(position);
  if (mask & 2) target->SetOrientation(orientation);
  if (mask & 0x20) { target->SetPortraitId(portrait); if (portrait>=0xfffe) target->SetPortrait(portraitRef); }
  if (mask & 4) {
   target->m_fAnimationSpeed=speed;
   if (target->m_nObjectType == OBJECT_TYPE_DOOR) {
    if (animation==0x2726) target->AsSWCDoor()->SetState(0);
    else if (animation==0x2742) target->AsSWCDoor()->SetState(1);
    else if (animation==0x2743) target->AsSWCDoor()->SetState(2);
   }
   target->PlayAnimation(animation,0,0);
  }
 }
 if ((mask & 8) && !HandleServerToPlayerUpdateVisualEffects(target)) return 0;
 return 1;
}
