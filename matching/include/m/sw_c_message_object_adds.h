#ifndef M_SW_C_MESSAGE_OBJECT_ADDS_H
#define M_SW_C_MESSAGE_OBJECT_ADDS_H
struct Vector { float x,y,z; Vector() { x=0; y=0; z=0; } };
class CExoString { public: char* m_sString; unsigned long m_nBufferLength;
 CExoString(); CExoString(const char*); ~CExoString();
 CExoString& operator=(const CExoString&); CExoString& operator=(const char*);
};
class CResRef { public: char m_ref[16]; CResRef(); CResRef(const char*); CResRef(const CExoString&); };
class C2DA { public: int GetCExoStringEntry(int,const CExoString&,CExoString*); int GetINTEntry(int,const CExoString&,int*); };
class CTwoDimArrays { public:
 char pad_000[0x24]; C2DA *m_pGenericDoors,*m_pDoors,*m_pPlaceables;
 char pad_030[0x8c-0x30]; C2DA* m_pTraps;
 C2DA* GetGenericDoors() { return m_pGenericDoors; }
 C2DA* GetDoors() { return m_pDoors; }
 C2DA* GetPlaceables() { return m_pPlaceables; }
 C2DA* GetTraps() { return m_pTraps; }
};
class CSWRules { public: char pad_000[0xb8]; CTwoDimArrays* m_p2DArrays;
 CTwoDimArrays* Get2DArrays() { return m_p2DArrays; }
};
extern CSWRules* g_pRules;
class CSWCArea { public: char pad_000[0x184]; void* m_pScene; };
class CSWCModule { public: char pad_000[0x48]; CSWCArea* m_pArea; };
class CGameObject { public: virtual ~CGameObject();
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
 virtual void AttachToScene(void*);
 unsigned long m_idSelf;
};
class CSWCObject : public CGameObject { public: char pad_008[0xfc-8];
 void SetVisibilityOnSetArea(CSWCArea*); // descriptive 0x0063E350
};
class CSWCDoor : public CSWCObject { public:
 CExoString m_sName;
 int m_n104,m_n108,m_n10C,m_n110,m_n114;
 char pad_118[0x124-0x118]; unsigned char m_nGenericType,m_nDoorType;
 char pad_126[0x138-0x126]; int m_bIgnoreHitCheck;
 CExoString m_sDescription;
 CSWCDoor();
 void SetState(int); // descriptive 0x00683D40
 void SetAnimationWithoutTrigger(unsigned short); // descriptive 0x00683D00
 void SetIsAreaTransition(int);
};
class CSWCTrigger : public CSWCObject { public:
 char pad_0fc[4]; CExoString m_sName;
 int m_bAreaTransition,m_bHostile,m_n110,m_n114;
 float m_fHighlightWidth;
 char pad_11c[0x130-0x11c]; int m_nPointCapacity; Vector* m_pPoints;
 char pad_138[8]; CExoString m_sText;
 unsigned char m_nTrapType; char pad_149[3];
 CSWCTrigger();
 void SetCursor(unsigned char); // descriptive 0x006914D0
 void AddToRenderList(int); // descriptive 0x00691CC0
 void DisplayTrigger(int);
 void SpecialDisplay(int); // descriptive 0x00691E00
};
class CGameObjectArray { public: unsigned char AddExternalObject(unsigned long*,CGameObject*,int); };
class CClientAIMaster { public: int AddObject(CSWCObject*); };
class CClientExoApp { public:
 CSWCModule* GetModule(); CGameObjectArray* GetGameObjectArray();
 CClientAIMaster* GetClientAIMaster(); CSWCDoor* GetDoorByGameObjectID(unsigned long);
 void AddObjectToHitcheckIgnoreList(unsigned long);
};
class CAppManager { public: void* m_pUnknown00; CClientExoApp* m_pClientExoApp; };
extern CAppManager* g_pAppManager;
extern const unsigned char GAME_OBJECT_VALID;
extern int g_bShowAllTriggers; // 0x0083285C
class CSWCMessage { public:
 unsigned long ReadOBJECTIDClient(); unsigned char ReadBYTE(int); unsigned short ReadWORD(int);
 int ReadBOOL(); float ReadFLOAT(float,int); int MessageReadOverflow(int);
 CExoString ReadCExoLocStringClient(CResRef&,unsigned long*);
 int HandleServerToPlayerDoorUpdate_Add(); int HandleServerToPlayerTriggerUpdate_Add();
};
#endif
