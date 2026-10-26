// CSWCObject animation/table and server-link family; local complete layout.
#ifndef N_OBJECT_CONTROL_FAMILY_H
#define N_OBJECT_CONTROL_FAMILY_H
struct Vector { float x,y,z; Vector& operator=(const Vector& v) { x=v.x; y=v.y; z=v.z; return *this; } };
Vector Normalize(const Vector&); // 0x004AB180
class CExoString;
class CResRef { public: char data[16]; CResRef(const CExoString&); CResRef operator=(const CResRef&); int operator==(const char*) const; void CopyToString(CExoString&) const; };
class CExoString { public: char* data; unsigned long length; CExoString(); ~CExoString(); int GetLength() const; char* CStr() const; CExoString& operator=(const CExoString&); };
// Descriptive name for the recovered 28-byte queue record; padding is untouched.
struct FireForgetAnimation {
    int active; unsigned short animation; unsigned short pad;
    unsigned long day,tick,duration; float speed; void* data;
    FireForgetAnimation();
};
class CSWCObject; class CSWCCreature; class CSWCArea; class CAurObject;
class CSWCAnimBase {
public:
    virtual ~CSWCAnimBase();
    virtual void v04();
    virtual CAurObject* GetModel(unsigned char);
    virtual void v0C();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual int ReplaceTexture(unsigned char,const CResRef&,const CResRef&,int);
    virtual int RestoreTexturePart(unsigned char);
    virtual void v24();
    virtual void v28();
    virtual void v2C();
    virtual void v30();
    virtual void v34();
    virtual void v38();
    virtual void v3C();
    virtual CExoString GetAnimationName(int);
    virtual void v44();
    virtual void v48();
    virtual void v4C();
    virtual void v50();
    virtual void v54();
    virtual void v58();
    virtual void v5C();
    virtual void v60();
    virtual int SetFacing(const Vector&);
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
    virtual int SetEnvMapPart(unsigned char,int,int,const CResRef&);
    virtual int SetEnvMap(int,int,const CResRef&);
    virtual void vA0();
    virtual void vA4();
    virtual void DestroyShadow();
    virtual void vAC();
    virtual void vB0();
    virtual void vB4();
    virtual void ModelVC0();
    virtual void vBC();
    virtual void vC0();
    virtual void vC4();
    virtual void vC8();
    virtual void vCC();
    virtual void vD0();
    virtual void vD4();
    virtual void vD8();
    virtual void vDC();
    virtual unsigned short ServerToClientAnimation(unsigned short);
};
class CAurObject {
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
    virtual void SetClientObject(CSWCObject*);
    virtual void v78();
    virtual void SetObjectType(unsigned char);
    virtual void v80();
    virtual void v84();
    virtual void v88();
    virtual void v8C();
    virtual void v90();
    virtual void v94();
    virtual void v98();
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
    virtual void vC4();
    virtual void vC8();
    virtual void vCC();
    virtual void vD0();
    virtual void vD4();
    virtual void SetWind(int,float,int,int);
};
class CExoSoundSource {
public: CExoSoundSource(const CResRef&); virtual ~CExoSoundSource(); void* m_pInternal;
};
class CSWPortrait {
public: char data[16]; CSWPortrait(); ~CSWPortrait(); int SetPortraitId(unsigned short); CResRef GetPortrait(unsigned char);
};
class C2DA { public: int GetINTEntry(int,int,int*); };
class CTwoDimArrays { public: char pad[0x3c]; C2DA* m_pAnimations; };
class CSWRules { public: char pad[0xb8]; CTwoDimArrays* m_p2DArrays; };
extern CSWRules* g_pRules; // 0x007A3A28
extern int g_nAnimStationaryColumn;
extern int g_nAnimPauseColumn;
extern int g_nAnimWalkingColumn;
extern int g_nAnimRunningColumn;
extern int g_nAnimLoopingColumn;
extern int g_nAnimFireForgetColumn;
extern int g_nAnimOverlayColumn;
extern int g_nAnimParryColumn;
extern int g_nAnimAttackColumn;
extern int g_nAnimHideEquippedItemsColumn;
class CSWCModule;
class CGuiInGame { public: char pad[0xb4]; int m_bItemsHidden; int IsLoopingDialogAnimation(unsigned short); int IsFireForgetDialogAnimation(unsigned short); };
class CWorldTimer {
public:
    void GetWorldTime(unsigned long*,unsigned long*);
    unsigned long GetTimeDifferenceFromWorldTime(unsigned long,unsigned long);
    int CompareWorldTimes(unsigned long,unsigned long,unsigned long,unsigned long);
    int AddWorldTimes(unsigned long,unsigned long,unsigned long,unsigned long,unsigned long*,unsigned long*);
};
class CClientExoApp { public: CSWCModule* GetModule(); CWorldTimer* GetWorldTimer(); CGuiInGame* GetInGameGui(); unsigned char GetActivePauseState(); };
class CSWSObject { public: void AttachmentFromClientObject(CSWCObject*); };
class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual CSWSObject* AsSWSObject();
};
class CServerExoApp { public: unsigned long ClientToServerObjectID(unsigned long); CGameObject* GetGameObject(unsigned long); };
class CAppManager { public: void* pad; CClientExoApp* m_pClientExoApp; CServerExoApp* m_pServerExoApp; };
extern CAppManager* g_pAppManager;
class CSWCObject {
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
    virtual CSWCCreature* AsSWCCreature();
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
    virtual void UpdateOrientation();
    virtual void v94();
    virtual CAurObject* GetModel(unsigned char,int);
    virtual void SetAnimation(unsigned short,int,int);
    virtual void AddAnimation(unsigned short,int); // +A0
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
    virtual void SetPortrait(const CResRef&); // +EC
    virtual void SetPortraitResource(const CResRef&,int); // +F0, descriptive
    unsigned long m_oidSelf; // +04
    unsigned char m_nType; // +08
    char pad09[0x20-9];
    CSWCArea* m_pArea; // +20
    Vector m_vPosition; // +24
    Vector m_vOrientation; // +30
    char pad3c[0x48-0x3c];
    unsigned short m_nCurrentAnimation;
    unsigned short m_nPendingAnimation;
    int m_nAnimationEvent;
    int m_n50;
    char pad54[4];
    unsigned short m_nLoopingAnimation; // +58
    unsigned short m_nAnimationCapacity; // +5A
    FireForgetAnimation* m_pAnimationQueue; // +5C
    unsigned char m_nAnimationCount; // +60
    char pad61[3];
    int m_n64;
    CSWCAnimBase* m_pAnimationBase; // +68
    char pad6c[0xa8-0x6c];
    CResRef m_cPortrait; // +A8
    unsigned short m_nPortraitId; // +B8
    char padba[0xc8-0xba];
    CExoSoundSource* m_pVocalization; // +C8
    int m_nCC;
    int m_nVocalizationType; // +D0
    int m_bShadowBlob; // +D4
    Vector m_vShadowOrientation; // +D8
    int m_bNoServerLink; // +E4
    CExoString m_sNameInfo; // +E8
    int m_nNameInfoTime; // +F0
    int m_nF4;
    CSWSObject* m_pServerObject; // +F8
    unsigned short GetLoopingAnimation();
    void SetOrientation(const Vector&);
    int AnimationStationary(unsigned short);
    int AnimationPause(unsigned short);
    int AnimationWalk(unsigned short);
    int AnimationRun(unsigned short);
    int AnimationLooping(unsigned short);
    int AnimationFireAndForget(unsigned short);
    int AnimationOverlay(unsigned short);
    int AnimationPlayOutOfPlace(unsigned short);
    int AnimationDialog(unsigned short);
    int AnimationParry(unsigned short);
    int AnimationAttack(unsigned short);
    int AnimationHideEquippedItems(unsigned short);
    int GetTopFireForgetAnimation(unsigned short*,float*,unsigned long*,void**);
    void AddFireForgetAnimation(unsigned short,unsigned long,float,void*,unsigned long);
    void RemoveTopFireForgetAnimation();
    void ClearFireForgetAnimations();
    void AbortFireForgetAnimation(int);
    void SetLoopingAnimation(unsigned short);
    int ReplaceTexture(unsigned char,const CResRef&,const CResRef&,int);
    int RestoreTexturePart(unsigned char);
    CSWSObject* GetServerObject();
    void SetNameInfo(const CExoString&);
    void GetNameInfo(CExoString*,int*);
    int SetEnvMap(int,int,const CResRef&);
    int SetEnvMapPart(unsigned char,int,int,const CResRef&);
    void OnAnimationChanged(unsigned long,unsigned short); // descriptive; 0x0063D850
    int HasModel();
    void SetPortraitRef(const CResRef&); // descriptive; 0x0063CFD0
    void OnAnimationEvent(const char*,const char*); // descriptive; 0x0063D330
    void SetWind(int,float);
    void NewVocalization(const CResRef&,int);
    void SetPortraitId(unsigned short);
    void RemoveShadowBlob();
    void AcquireAnimationBase(CSWCObject*);
    void SetArea(CSWCArea*);
    void UpdateAreaFade(CSWCArea*); // descriptive; 0x0063E350
    void SetAnimationFromEvent(unsigned long,int); // descriptive; 0x0063D3F0

};
class CSWCCreature : public CSWCObject {
public: char padfc[0x3cc-0xfc]; void* m_pEquippedItem; void ShowEquippedItems(); void UnregisterAnimationCallbacks(); // 0x00610550
};
template<class T> class CExoArrayList { public: T* element; int count,capacity; void AddUnique(T); void Remove(T); };
class CSWCArea { public: char pad[0xbc]; CResRef m_cEnvMap; char padcc[0x20c-0xcc]; CExoArrayList<unsigned long> m_lObjects; };
class CSWCModule { public: char pad[0x48]; CSWCArea* m_pArea; };
#endif
