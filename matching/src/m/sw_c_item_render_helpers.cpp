// Item sound/model helpers used by inventory lifecycle messages.
class CExoString { public: char* CStr() const; };
class CResRef { public: char m_ref[16]; CResRef(); CResRef operator=(const char*); CResRef operator=(const CResRef&); };
class CAurObject { public: virtual ~CAurObject();
 virtual void v04();
 virtual void v08();
 virtual void v0C();
 virtual void v10();
 virtual void v14();
 virtual int PlayAnimation(const char*,float,int,int);
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
 virtual void AttachToScene(void*);
 virtual void AttachModel(CAurObject*,const char*,int);
 virtual void v54();
 virtual void v58();
 virtual void v5C();
 virtual void v60();
};
CAurObject* NewCAurObject(const char*,const char*,int,int);
class CAurAnimationBase { public: char pad_000[0xdc]; unsigned long m_nItemFlags; };
class CSWCCreature { public: unsigned long GetEquippedItemID(unsigned long); CAurAnimationBase* GetAnimationBase(); };
struct CSWBaseItemData { char pad_000[0x78]; unsigned char m_nDropSound;
 char pad_079[0xac-0x79]; unsigned char m_nItemClass;
};
class CSWBaseItem : public CSWBaseItemData {};
class CSWBaseItemArray { public: CSWBaseItem* GetBaseItem(int); };
class CSWRules { public: char pad_000[0x34]; CSWBaseItemArray* m_pBaseItemArray;
 CSWBaseItemArray* GetBaseItemArray() { return m_pBaseItemArray; }
};
extern CSWRules* g_pRules;
class CClientAIMaster;
class CClientExoApp { public: CResRef GetItemDropSound(unsigned char);
 CSWCCreature* GetCreatureByGameObjectID(unsigned long); CClientAIMaster* GetClientAIMaster();
};
class CAppManager { public: void* m_pUnknown00; CClientExoApp* m_pClientExoApp; };
extern CAppManager* g_pAppManager;
class CExoSound { public: int PlayOneShotSound(const CResRef&,unsigned char,unsigned long,unsigned char,float,float); };
extern CExoSound* g_pExoSound;
class CSWCObject { public: virtual ~CSWCObject(); char pad_004[8]; int m_nBaseItem; };
class CSWItem { public:
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
 virtual void v84();
 virtual void v88();
 virtual void v8C();
 virtual void v90();
 virtual void v94();
 virtual CAurObject* GetModel(unsigned char,int);
};
class CClientAIMaster { public: int AddObject(CSWItem*); };
class CSWCItem : public CSWCObject,public CSWItem { public:
 unsigned long m_idItem;
 char pad_018[0x78-0x18]; CAurObject* m_pModel;
 char pad_07c[0x110-0x7c]; unsigned char m_nArmorType;
 char pad_111[0x160-0x111]; unsigned long m_oidEquippedBy;
 char pad_164[0x194-0x164]; CAurObject* m_pSpecialAttack;
 char pad_198[4]; int m_bModelOverride;
 CSWBaseItemData* GetBaseItem(); // established alias, 0x005B4790
 int PlayDropSound();
 void AttachAuxiliaryModel(); // descriptive 0x00646830
 void SetSpecialAttackModel(const CExoString&,const CExoString&); // descriptive 0x00646880
 int GetModelOverride() { return m_bModelOverride; }
 void SetEquippedModelFlags(int); // descriptive 0x00646BA0
};
// FUNCTION: SWKOTOR 0x006466D0
int CSWCItem::PlayDropSound()
{
 CResRef sound;
 if (GetBaseItem()->m_nItemClass>=0x1f && GetBaseItem()->m_nItemClass<=0x26) {
  switch(m_nArmorType) {
  case 0: sound="it_armorleather"; break;
  case 1: sound="it_armorchain"; break;
  case 2: sound="it_armorplate"; break;
  case 3: sound="it_materialcloth"; break;
  }
 } else {
  int base=m_nBaseItem;
  unsigned char index=g_pRules->GetBaseItemArray()->GetBaseItem(base)->m_nDropSound;
  CClientExoApp* client=g_pAppManager->m_pClientExoApp;
  sound=client->GetItemDropSound(index);
 }
 CExoSound* engine=g_pExoSound;
 return engine->PlayOneShotSound(sound,0xb,0,0,0.0f,0.0f);
}
// FUNCTION: SWKOTOR 0x00646830
void CSWCItem::AttachAuxiliaryModel()
{
 if (m_pModel) {
  CAurObject* auxiliary=GetModel(0xfc,1);
  if (auxiliary) {
   CAurObject* model=GetModel(0xff,1);
   if (model) auxiliary->AttachModel(model,"root",0);
  }
 }
}
// FUNCTION: SWKOTOR 0x00646880
void CSWCItem::SetSpecialAttackModel(const CExoString& model,const CExoString& node)
{
 if (m_pSpecialAttack) { delete m_pSpecialAttack; m_pSpecialAttack=0; }
 m_pSpecialAttack=NewCAurObject(model.CStr(),"specialattack",0,0);
 if (m_pSpecialAttack) {
  m_pSpecialAttack->AttachModel(GetModel(255,1),node.CStr(),0);
  m_pSpecialAttack->PlayAnimation("impact",1.0f,1,0);
 }
 g_pAppManager->m_pClientExoApp->GetClientAIMaster()->AddObject(this);
}
// FUNCTION: SWKOTOR 0x00646BA0
void CSWCItem::SetEquippedModelFlags(int enabled)
{
 CClientExoApp* client=g_pAppManager->m_pClientExoApp;
 CSWCCreature* owner=client->GetCreatureByGameObjectID(m_oidEquippedBy);
 if (!owner) return;
 unsigned char flags=0;
 if (m_idItem==owner->GetEquippedItemID(0x10)) flags=1;
 else if (m_idItem==owner->GetEquippedItemID(0x20)) flags=2;
 if (!owner->GetAnimationBase()) return;
 if (enabled) {
  if (!GetModelOverride()) flags|=(unsigned char)owner->GetAnimationBase()->m_nItemFlags;
 } else {
  if (GetModelOverride()) flags=(unsigned char)owner->GetAnimationBase()->m_nItemFlags & ~flags;
  else flags &= 0xfc;
 }
 if (GetBaseItem()->m_nItemClass!=0x27 && GetBaseItem()->m_nItemClass!=0x28 && GetBaseItem()->m_nItemClass!=0x29) flags|=4;
 else flags&=0xfb;
 owner->GetAnimationBase()->m_nItemFlags=flags;
}
