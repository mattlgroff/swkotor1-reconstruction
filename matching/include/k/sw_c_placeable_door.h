#ifndef K_SW_C_PLACEABLE_DOOR_H
#define K_SW_C_PLACEABLE_DOOR_H
#include <math.h>
#include <string.h>
class CExoString {
public:
 char* m_sString; unsigned long m_nBufferLength;
 CExoString(); CExoString(const char*); CExoString(const CExoString&); ~CExoString();
 CExoString& operator=(const char*);
 CExoString& operator=(const CExoString&);
 CExoString operator+(const CExoString&) const;
 CExoString SubString(int,int) const; char* CStr() const;
};
class CSWCObject;
class CResRef;
class CSWCDoor;
void __cdecl DoorAnimationEvent(void*,const char*,CSWCDoor*);
struct Vector { float x,y,z; Vector() { x=0; y=0; z=0; } };
struct Quaternion { float w,x,y,z; Quaternion() { z=0; y=0; x=0; w=1; } };
class CAurObject {
public:
 virtual ~CAurObject();
 virtual void v04();
 virtual void v08();
 virtual void v0C();
 virtual void v10();
 virtual void v14();
 virtual void v18();
 virtual void v1C();
 virtual void v20();
 virtual void v24();
 virtual void SetAnimationEvent(void (__cdecl *)(void*,const char*,CSWCDoor*),const char*,CSWCObject*,float,int);
 virtual void v2C();
 virtual void v30();
 virtual void v34();
 virtual void v38();
 virtual void v3C();
 virtual void v40();
 virtual void v44();
 virtual void v48();
 virtual void SetVisible(int);
 virtual void AttachToObject(CAurObject*, const char*, int);
 virtual void v54();
 virtual void v58();
 virtual void v5C();
 virtual void v60();
 virtual void v64();
 virtual void v68();
 virtual void v6C();
 virtual void v70();
 virtual void SetClientObject(CSWCObject*);
 virtual void v78();
 virtual void SetObjectType(int);
 virtual void v80();
 virtual void v84();
 virtual void v88();
 virtual void v8C();
 virtual void SetIgnoreHitCheck(unsigned char);
 virtual void v94();
 virtual void v98();
 virtual void v9C();
 virtual int GetNodeTransform(const char*,Vector*,Quaternion*);
 virtual void vA4();
 virtual void vA8();
 virtual void vAC();
 virtual void vB0();
 virtual void vB4();
 virtual void vB8();
 virtual void vBC();
};
class CAnimBase { public: virtual ~CAnimBase(); int m_n4,m_n8,m_nC,m_n10; };
class CSWCAnimBase : public CAnimBase {
public:
 virtual ~CSWCAnimBase();
 virtual void v04();
 virtual CAurObject* GetModel(unsigned char);
 virtual int LoadModel(const CResRef&,unsigned char);
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
 virtual int PlayAnim(int,float,int,int);
 virtual void v58();
 virtual void v5C();
 virtual void v60();
 virtual void v64();
 virtual void v68();
 virtual void v6C();
 virtual void v70();
 virtual void v74();
 virtual void UnloadModel();
 virtual void v7C();
 virtual void v80();
 virtual void v84();
 virtual void v88();
 virtual void v8C();
 virtual void v90();
 virtual void v94();
 virtual void v98();
 virtual void v9C();
 virtual void vA0();
 virtual int CreateShadow(const CExoString&);
 virtual void vA8();
 virtual void vAC();
 virtual void vB0();
 virtual void vB4();
 virtual void vB8();
 virtual void ModelVBC();
 char pad14[0xd0-0x14]; int m_nD0; char padd4[0xf0-0xd4];
};
class CSWSPlaceable { public: char pad0[0x230]; unsigned char m_nAppearance; char pad231[0x340-0x231]; int m_bHostile; char pad344[0x44c-0x344]; void* m_pCreature; };
class CSWSDoor { public: void MoveToNextOpenState(); };
class CSWSObject {
public:
 virtual void v00();
 virtual void v04();
 virtual void v08();
 virtual void v0C();
 virtual void v10();
 virtual void v14();
 virtual CSWSDoor* AsSWCDoor();
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
 virtual CSWSPlaceable* AsSWCPlaceable();
};
class CSWCObjectServerLink { public: virtual ~CSWCObjectServerLink(); CSWSObject* GetServerObject(); };
class CSWCObject : public CSWCObjectServerLink {
public:
 CSWCObject(unsigned char); virtual ~CSWCObject();
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
 virtual void UnloadModel();
 virtual void v7C();
 virtual void v80();
 virtual void v84();
 virtual void v88();
 virtual void v8C();
 virtual void v90();
 virtual void v94();
 virtual CAurObject* GetModel(unsigned char, int);
 virtual void v9C();
 virtual void vA0();
 virtual void vA4();
 virtual void vA8();
 virtual void vAC();
 virtual void vB0();
 virtual void vB4();
 virtual void vB8();
 virtual void vBC();
 virtual void vC0();
 virtual void UpdateVisualEffectsList();
 virtual void vC8();
 virtual void vCC();
 virtual void vD0();
 virtual void vD4();
 virtual void vD8();
 virtual void vDC();
 virtual void vE0();
 virtual void vE4();
 virtual void UpdatePopUpBubble();
 virtual void vEC();
 virtual void vF0();
 virtual void vF4();
 virtual void vF8();
 virtual void vFC();
 virtual void v100();
 virtual void v104();
 virtual void RefreshHilite();
 unsigned long m_oid; char pad08[0x3c-8];
 unsigned long m_nDay, m_nTick, m_nElapsed;
 unsigned short m_nAnimation; char pad4a[2]; float m_fSpeed;
 char pad50[0x68-0x50]; CSWCAnimBase* m_pModel;
 char pad6c[0xa4-0x6c]; float m_fCanSeeHeight; char pada8[0xd4-0xa8]; int m_bShadow; char padd8[0xf0-0xd8]; unsigned long m_nCountdown; char padf4[8];
 float GetAnimationLength(unsigned long);
 void PlayAnimationTimed(unsigned long, int, float, int, int); // descriptive name, 0063C760
 int IsAnimationLooping(unsigned short); // descriptive name, 0063C300
 void SetLoopingAnimation(unsigned short); // descriptive name, 0063D830
 void ClearAllActions();
 int PlayAnimation(unsigned short, float, int); // 0063E930
 int FadeObject(); // 0063DF60; advances fade and reports deferred deletion
 void EmptyUpdate(); // descriptive name for shared no-op at 004015A0
};
class CSWCCreature : public CSWCObject { public: void PerformAttack(unsigned long); };
class CSWCPlaceable : public CSWCObject {
public:
 CSWCPlaceable(); virtual ~CSWCPlaceable();
 CExoString m_sName; CExoString m_sModel;
 unsigned char m_nAppearance; char pad10d[3];
 int m_n110,m_n114,m_n118,m_n11c,m_n120,m_n124,m_n128,m_n12c,m_n130,m_n134,m_n138,m_n13c,m_n140;
 CExoString m_sLight; CAurObject* m_pLight;
 void UnloadModel(); void PlayAnimationAction(unsigned long,int,int);
 void ActionMenuUse(int,CSWCCreature*); void ActionMenuOpen(int,CSWCCreature*);
 int LoadModel(const CResRef&,unsigned char,int);
 CExoString GetName(); void ActionMenuLock(int,CSWCCreature*);
 void UnloadLight(); int GetIsHostile(); int PlayAnimation(unsigned short,float,int);
 CSWSPlaceable* GetServerPlaceable();
 CSWSPlaceable* ServerInline() { if(GetServerObject()) return GetServerObject()->AsSWCPlaceable(); return 0; }
 void UpdateAI(); void UpdateAnimation();
 int SetShadow(); void SetAppearance(unsigned char); float GetCanSeeHeight();
};
class CSWCDoor : public CSWCObject {
public:
 CSWCDoor(); virtual ~CSWCDoor();
 CExoString m_sName;
 int m_n104,m_n108,m_n10c,m_n110,m_n114,m_n118;
 unsigned char m_nOpenState; char pad11d[3]; int m_n120;
 unsigned char m_n124,m_n125; char pad126[2]; int m_n128,m_bTransition;
 CExoString m_sTransition; int m_n138; CExoString m_sDescription;
 void SetAnimation(int); void SetOpenState(unsigned char);
 void ActionMenuUse(int,CSWCCreature*); void ActionMenuLock(int,CSWCCreature*);
 void PlayAnimationAction(unsigned long,int,int); void ActionMenuOpen(int,CSWCCreature*);
 void UpdateInteraction(); // 00684660, descriptive name
 int LoadModel(const CResRef&,unsigned char,int);
 CSWSDoor* GetServerDoor();
 void UpdateAI(); void UpdateAnimation();
};
class CSWCMessage { public:
 int SendPlayerToServer_06_0B(unsigned long);
 int SendPlayerToServer_06_03(unsigned long,unsigned short);
 int SendPlayerToServer_06_0C(unsigned long,unsigned long);
};
class CGuiInGame { public: int CanPerformAction(); /* 0062F9A0, elapsed-action timer gate */ };
class CSWCArea { public: void RemovePlaceableObjectLight(unsigned long,int); };
class CSWCModule { public: char pad0[0x48]; CSWCArea* m_pArea; CSWCArea* GetArea() { return m_pArea; } };
class CClientExoApp { public:
 CSWCModule* GetModule(); CSWCMessage* GetMessage(); CGuiInGame* GetInGameGui();
 int ShowTutorialWindow(int,unsigned long,unsigned long,int);
 void DelayNextEnemyAutoPause();
 class CWorldTimer* GetWorldTimer();
 class CGameObjectArray* GetGameObjectArray(); class CClientAIMaster* GetClientAIMaster();
};
class CAppManager { public: void* vptr; CClientExoApp* m_pClientExoApp; CClientExoApp* GetClientExoApp() { return m_pClientExoApp; } };
extern CAppManager* g_pAppManager;
class CWorldTimer { public: void GetWorldTime(unsigned long*,unsigned long*);
 int SubtractWorldTimes(unsigned long,unsigned long,unsigned long,unsigned long,unsigned long*,unsigned long*); };
extern unsigned char g_nObjectTypePlaceable; // 00746638
extern unsigned char g_nObjectTypeDoor; // 00746639
extern const char g_sEmpty[]; // 0073D71C
class CClientObject;
class CGameObject;
class CClientAIList { public: int RemoveObject(CClientObject*); };
class CClientAIMaster { public: void* vptr; CClientAIList m_List; CClientAIList* GetAIList() { return &m_List; } };
class CGameObjectArray { public: unsigned char Delete(unsigned long,CGameObject**); };
extern const unsigned char g_nObjectArraySuccess; // 0074666C
class C2DA { public: int GetINTEntry(int,const CExoString&,int*); int GetFLOATEntry(int,const CExoString&,float*); };
class C2DAs { public: char pad0[0x2c]; C2DA* m_pPlaceables; C2DA* GetPlaceables() { return m_pPlaceables; } };
class CSWRules { public: char pad0[0xb8]; C2DAs* m_p2DArrays; };
extern CSWRules* g_pRules;
class CResRef { public: char m_resRef[16]; void CopyToString(CExoString&) const; };
class CSWCAnimBasePlaceable : public CSWCAnimBase { public: CSWCAnimBasePlaceable(); };
class CSWCAnimBaseDoor : public CSWCAnimBase { public: CSWCAnimBaseDoor(); };
// C++98 layout checks; none of the existing shared headers are extended.
typedef char KObjectSize[(sizeof(CSWCObject)==0xfc)?1:-1];
typedef char KPlaceableSize[(sizeof(CSWCPlaceable)==0x150)?1:-1];
typedef char KDoorSize[(sizeof(CSWCDoor)==0x144)?1:-1];
typedef char KAnimBaseSize[(sizeof(CAnimBase)==0x14)?1:-1];
typedef char KClientAnimSize[(sizeof(CSWCAnimBase)==0xf0)?1:-1];
#endif
