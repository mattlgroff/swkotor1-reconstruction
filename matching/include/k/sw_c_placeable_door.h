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
struct Vector { float x,y,z; Vector() { x=0; y=0; z=0; } Vector(float fX,float fY,float fZ) { x=fX; y=fY; z=fZ; } Vector operator-() const { return Vector(-x,-y,-z); } };
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
 virtual void vC0(); virtual void vC4(); virtual void vC8(); virtual void vCC();
 virtual void vD0(); virtual void vD4(); virtual void vD8(); virtual void vDC();
 virtual void vE0(); virtual void vE4(); virtual void vE8(); virtual void vEC();
 virtual void vF0(); virtual void vF4(); virtual void vF8(); virtual void vFC();
 virtual void v100(); virtual void v104(); virtual void v108();
 virtual class CAurPart* FindPart(const char*); // +0x10C (placeholder name, as bucket n)
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
 virtual int SetOrientation(const Quaternion&); // +0x60
 virtual int SetFacing(const Vector&); // +0x64
 virtual int SetPosition(const Vector&); // +0x68
 virtual void SetFlag130(int); // +0x6C (name as k/sw_c_anim_base.h)
 virtual void SetFlag138(int); // +0x70
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
class CSWSPlaceable { public: char pad0[0x230]; unsigned char m_nAppearance; char pad231[0x340-0x231]; int m_bHostile; char pad344[0x440-0x344]; int m_n440; int m_n444; char pad448[4]; void* m_pCreature; unsigned char GetAppearance() { return m_nAppearance; } };
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
 virtual void SetAnimationSlot(unsigned short,int,int); // +0x9C SetAnimation (the door / placeable overrides are declared non-virtually below)
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
 unsigned long m_oid; char pad08[0x20-8]; class CSWCArea* m_pArea; Vector m_vPosition; Vector m_vOrientation;
 unsigned long m_nDay, m_nTick, m_nElapsed;
 unsigned short m_nAnimation; char pad4a[2]; float m_fSpeed;
 int m_bOutOfPlace; int m_n54; char pad58[0x64-0x58]; int m_nFireForgetTime; CSWCAnimBase* m_pModel;
 char pad6c[0xa4-0x6c]; float m_fCanSeeHeight; char pada8[0xd4-0xa8]; int m_bShadow; char padd8[0xf0-0xd8]; unsigned long m_nCountdown; char padf4[8];
 float GetAnimationLength(unsigned long);
 void PlayAnimationTimed(unsigned long, int, float, int, int); // descriptive name, 0063C760
 int IsAnimationLooping(unsigned short); // descriptive name, 0063C300
 void SetLoopingAnimation(unsigned short); // descriptive name, 0063D830
 void ClearAllActions();
 int PlayAnimation(unsigned short, float, int); // 0063E930
 int FadeObject(); // 0063DF60; advances fade and reports deferred deletion
 void EmptyUpdate(); // descriptive name for shared no-op at 004015A0
 unsigned short GetLoopingAnimation(); // 0063C100
 int AnimationFireAndForget(unsigned short); // 0063C380
 int GetTopFireForgetAnimation(unsigned short*,float*,unsigned long*,void**); // 0063C610
 void RemoveTopFireForgetAnimation(); // 0063C9C0
 int AnimationPlayOutOfPlace(unsigned short); // 0063C470
 void AbortFireForgetAnimation(int); // 0063CAD0
 unsigned char GetReticlePoint(Vector&); // 0063D560 (virtual in the original; called directly here)
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
 unsigned char GetReticlePoint(Vector&); // 00682FF0
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
class CSWCArea { public: void RemovePlaceableObjectLight(unsigned long,int); int ParentCorpseExists(unsigned long); /* 006057B0 */ };
class CSWCModule { public: char pad0[0x48]; CSWCArea* m_pArea; CSWCArea* GetArea() { return m_pArea; } };
class CClientExoApp { public:
 CSWCModule* GetModule(); CSWCMessage* GetMessage(); CGuiInGame* GetInGameGui();
 int ShowTutorialWindow(int,unsigned long,unsigned long,int);
 void DelayNextEnemyAutoPause();
 class CWorldTimer* GetWorldTimer();
 class CGameObjectArray* GetGameObjectArray(); class CClientAIMaster* GetClientAIMaster();
 class CSWCModuleCamera* GetModuleCamera(); class CSWGuiManager* GetGuiManager(); // 005ED560, 005EDA40
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
class C2DA { public: int GetINTEntry(int,const CExoString&,int*); int GetFLOATEntry(int,const CExoString&,float*);
 int GetINTEntry(int,int,int*); /* 00413510 */ int GetCExoStringEntry(int,int,CExoString*); /* 00413190 */
 int GetCExoStringEntry(int,const CExoString&,CExoString*); /* 00413270 */ };
class C2DAs { public: char pad0[0x24]; C2DA* m_pDoorTypes; C2DA* m_pGenericDoors; C2DA* m_pPlaceables; char pad30[0x70-0x30]; C2DA* m_pPlaceableSounds; C2DA* GetPlaceables() { return m_pPlaceables; } C2DA* GetPlaceableSounds() { return m_pPlaceableSounds; } };
class CSWRules { public: char pad0[0xb8]; C2DAs* m_p2DArrays; C2DAs* Get2DArrays() { return m_p2DArrays; } };
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
