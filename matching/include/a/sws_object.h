// CSWSObject leaves, destructor and effect list management
// (vtable 0x00745F18, 0x004D0170-0x004D1D47). Self-contained declarations;
// only the members these functions use are placed.
#ifndef A_SWS_OBJECT_H
#define A_SWS_OBJECT_H

#include <stddef.h>

class CSWSObject;
class CSWSCreature;

class CExoString {
public:
    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50

    char* m_sString;
    unsigned long m_nBufferLength;
};

class CExoLocString {
public:
    ~CExoLocString();                               // 0x005EA000

    char pad_0x0[0xc];
};

class CResRef {
public:
    CResRef(const char* szName);                    // 0x00406D80
    CResRef operator=(const CResRef& cResRef);      // 0x00406120

    char m_resRef[16];
};

struct CResStruct {
    unsigned long m_nStructIndex;
};

struct CResList {
    unsigned long m_nStructIndex;
    char m_pLabel[16];
};

class CResGFF {
public:
    int GetList(CResList* pList, CResStruct* pStruct, char* szLabel);   // 0x004118C0
    unsigned long GetListCount(CResList* pList);                        // 0x00411940
    int GetListElement(CResStruct* pStruct, CResList* pList, unsigned long nIndex); // 0x00411990
    unsigned char ReadFieldBYTE(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                unsigned char nDefault);                // 0x00411A60
    int ReadFieldINT(CResStruct* pStruct, char* szLabel, int& bSuccess,
                     int nDefault);                                     // 0x00411C90
    CExoString ReadFieldCExoString(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                   const CExoString& sDefault);         // 0x00411EC0
};

template <class T> class CExoArrayList {
public:
    ~CExoArrayList() { delete[] element; element = NULL; }
    T& operator[](int i) { return element[i]; }
    int IndexOf(T t)
    {
        for (int i = 0; i < num; i++) {
            if (element[i] == t) {
                return i;
            }
        }
        return -1;
    }
    void DelIndex(int nIndex)
    {
        num--;
        for (; nIndex < num; nIndex++) {
            element[nIndex] = element[nIndex + 1];
        }
    }
    void Remove(T t) { DelIndex(IndexOf(t)); }
    void SetSize(int nSize);
    void Insert(T t, int nPos);                     // <CGameEffect*>: 0x00616940
    void Add(T t);                                  // <unsigned long>: 0x006449D0

    T* element;
    int num;
    int array_size;
};

template <class T> void CExoArrayList<T>::SetSize(int nSize)
{
    if (nSize == 0) {
        if (element) {
            delete[] element;
            element = NULL;
            array_size = 0;
        }
    }
    num = nSize;
}

class CExoLinkedListInternal {
public:
    ~CExoLinkedListInternal();                      // 0x005E9AB0
    void* RemoveHead();                             // 0x005E9C80

    void* pHead;
    void* pTail;
    unsigned long m_nCount;
};

template <class T> class CExoLinkedList {
public:
    ~CExoLinkedList() { delete m_pcExoLinkedListInternal; }
    int IsEmpty() { return m_pcExoLinkedListInternal->pHead == NULL; }
    T* RemoveHead() { return (T*)m_pcExoLinkedListInternal->RemoveHead(); }

    CExoLinkedListInternal* m_pcExoLinkedListInternal;
};

#pragma pack(push, 4)
class CGameEffect {
public:
    CGameEffect(int bCreateNewID);                  // 0x00503E40
    ~CGameEffect();                                 // 0x00503590
    int GetInteger(int nIndex);                     // 0x00503690
    void SetExpiryTime(unsigned long nDay, unsigned long nTime);  // 0x00503750
    int LoadGameEffect(CResGFF* pRes, CResStruct* pStruct);       // 0x005043A0
    unsigned short GetDurationType() { return m_nSubType & 7; }

    unsigned __int64 m_nID;                         // 0x00
    unsigned short m_nType;                         // 0x08
    unsigned short m_nSubType;                      // 0x0a
    float m_fDuration;                              // 0x0c
    char pad_0x10[0x18 - 0x10];
    unsigned long m_oidCreator;                     // 0x18
    unsigned long m_nSpellId;                       // 0x1c
    int m_bExpose;                                  // 0x20 (placeholder name)
    char pad_0x24[0x88 - 0x24];
    int m_bShowIcon;                                // 0x88 (placeholder name)
};
#pragma pack(pop)

class CSWSExpression {
public:
    ~CSWSExpression();                              // 0x0059D9E0
};

class CSWSObjectActionNode {
public:
    ~CSWSObjectActionNode();                        // 0x004CC4D0
};

class CSWSScriptVarTable {
public:
    ~CSWSScriptVarTable();                          // 0x0059A980
    void LoadVarTable(CResGFF* pRes, CResStruct* pStruct);  // 0x0059AA80
    char pad_0x0[0x10];
};

class CSWVarTable {
public:
    void LoadVarTable(CResGFF* pRes, CResStruct* pStruct);  // 0x0059B0F0
    char pad_0x0[0x14];
};

class CSWSObject34 {
public:
    virtual ~CSWSObject34();
};

class CSWSObject224 {
public:
    void Destroy();                                 // 0x0063D530 (placeholder name)
};

class CSWSpell {
public:
    void SetSpellLevel(int nLevel);                 // 0x0059B650 (placeholder name)
    char pad_0x0[0x140];
    unsigned char m_n140;                           // 0x140
};

class CSWSpellArray {
public:
    CSWSpell* GetSpell(int nSpell);                 // 0x0059B6D0
};

class CSWRules {
public:
    char pad_0x0[0x8c];
    CSWSpellArray* m_pSpellArray;                   // 0x8c
};
extern CSWRules* g_pRules;                          // 0x007A3A28

class CSWSCreatureStats {
public:
    unsigned char GetSpellLevel(unsigned char nClass);  // 0x005A4E90 (placeholder name)
    char pad_0x0[0x152];
    unsigned short m_nSpellLevelAbsorptionStart;    // 0x152 (placeholder name)
};

class CGameObject {
public:
    virtual ~CGameObject() {}                       // 0x00
    virtual void v04(); virtual void v08(); virtual void v0c();
    virtual CSWSObject* AsSWSObject();              // 0x10
    virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual CSWSCreature* AsSWSCreature();          // 0x30
};

class CGameObjectArray {
public:
    int Delete(unsigned long oid);                  // 0x004D8290
};

class CWorldTimer {
public:
    void GetWorldTime(unsigned long* pDay, unsigned long* pTime);           // 0x004ADE40
    unsigned long GetCalendarDayFromSeconds(float fSeconds);                // 0x004AE1B0
    unsigned long GetTimeOfDayFromSeconds(float fSeconds);                  // 0x004AE200
    void AddWorldTimes(unsigned long nDay1, unsigned long nTime1, unsigned long nDay2,
                       unsigned long nTime2, unsigned long* pDay, unsigned long* pTime); // 0x004ADF50
};

class CServerAIMaster {
public:
    int OnEffectApplied(CSWSObject* pObject, CGameEffect* pEffect, int bLoadingGame);  // 0x004AF0F0
    int OnEffectRemoved(CSWSObject* pObject, CGameEffect* pEffect);                    // 0x004AF100
};

class CServerExoApp {
public:
    CGameObject* GetGameObject(unsigned long oid);  // 0x004AE750
    CGameObjectArray* GetObjectArray();             // 0x004AED70
    CServerAIMaster* GetServerAIMaster();           // 0x004AED80
    CWorldTimer* GetWorldTimer();                   // 0x004AEDE0
};

class CAppManager {
public:
    char pad_0x0[0x8];
    CServerExoApp* m_pServerExoApp;                 // 0x8
};
extern CAppManager* g_pAppManager;                  // 0x007A39FC
inline CServerExoApp* GetServerExoApp() { return g_pAppManager->m_pServerExoApp; }

extern const int g_nTRUE;                           // 0x00746678
extern const unsigned char g_nObjectTypeCreature;   // 0x00746634 (placeholder name)
extern const unsigned short g_nDurationTemporary;   // 0x007473C0
extern const unsigned short g_nDurationInnate;      // 0x007473BC (placeholder name)
extern const unsigned short g_nDurationEquipped;    // 0x007473C8 (placeholder name)
extern const int g_nEffectType7466A0;               // 0x007466A0
extern const int g_nEffectType7466F4;               // 0x007466F4
extern const int g_nEffectType74671C;               // 0x0074671C
extern const int g_nEffectType746780;               // 0x00746780
extern const int g_nEffectType746788;               // 0x00746788

// vtable 0x00745F18
class CSWSObject : public CGameObject {
public:
    virtual ~CSWSObject();
    virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7c();
    virtual CResRef GetDialogResref();              // 0x80
    virtual void v84(); virtual void v88();
    virtual CExoLocString* GetFirstName();          // 0x8c
    virtual CExoLocString* GetLastName();           // 0x90 (folded with 0x8c)
    virtual void v94();
    virtual unsigned short GetGender(int nUnused);  // 0x98 (placeholder name)
    virtual void v9c(); virtual void va0(); virtual void va4(); virtual void va8();
    virtual void vac(); virtual void vb0(); virtual void vb4(); virtual void vb8();
    virtual void vbc(); virtual void vc0();
    virtual unsigned long GetLastTriggered();       // 0xc4 (placeholder name)
    virtual CResRef GetTemplateResRef();            // 0xc8
    virtual void SetTemplateResRef(const CResRef& cResRef);  // 0xcc
    virtual unsigned short Get30();                 // 0xd0 (placeholder name)
    virtual void vd4();
    virtual void UpdateEffectPtrs();                // 0xd8 (placeholder name)
    virtual void OnEffectListChanged(CGameEffect* pEffect);  // 0xdc (placeholder name)

    void RemoveEffectTarget(unsigned long oid);     // 0x004CF3F0
    void ClearMatchedExpressionStrings();           // 0x004CFA50
    void SetListenExpression(const CExoString& sExpr, int nID);  // 0x004CF8F0
    void LoadActionQueue(CResGFF* pRes, CResStruct* pStruct);    // 0x004CECB0 (placeholder name)

    void LoadListenData(CResGFF* pRes, CResStruct* pStruct);
    void RemoveEffect(CGameEffect* pEffect);
    int RemoveEffectById(unsigned __int64 nID);
    void RemoveEffectByCreator(unsigned long oidCreator);
    void RemoveSomeEffectsOfDurationType(unsigned short nType);
    void RemoveEffectByType(unsigned long nSpellId, int nType, int nValue);
    void ClearAllEffects(int bKeepOwn);
    int DoSpellLevelAbsorption(CSWSObject* pCaster);
    void ApplyEffect(CGameEffect* pEffect, int bLoadingGame, int bFlag);
    void LoadEffectList(CResGFF* pRes, CResStruct* pStruct);
    void LoadObjectState(CResGFF* pRes, CResStruct* pStruct);

    unsigned long m_idSelf;                         // 0x04
    unsigned char m_nObjectType;                    // 0x08
    char pad_0x9[0xc - 0x9];
    CExoLocString m_sLocName;                       // 0x0c
    CExoString m_sTag;                              // 0x18
    CResRef m_cTemplateResRef;                      // 0x20
    unsigned short m_n30;                           // 0x30
    char pad_0x32[0x34 - 0x32];
    CSWSObject34* m_p34;                            // 0x34
    char pad_0x38[0xe0 - 0x38];
    unsigned short m_nE0;                           // 0xe0
    char pad_0xe2[0xe8 - 0xe2];
    int m_bE8;                                      // 0xe8
    char pad_0xec[0xfc - 0xec];
    CExoLinkedList<CSWSObjectActionNode> m_lQueuedActions;  // 0xfc
    CSWSScriptVarTable m_ScriptVars;                // 0x100
    CSWVarTable m_VarTable;                         // 0x110
    CExoArrayList<CGameEffect*> m_appliedEffects;   // 0x124
    CExoArrayList<unsigned long> m_lst130;          // 0x130
    CExoArrayList<unsigned long> m_lstEffectTargets; // 0x13c
    int m_n148;                                     // 0x148
    char pad_0x14c[0x168 - 0x14c];
    unsigned char* m_p168;                          // 0x168
    char pad_0x16c[0x184 - 0x16c];
    CExoString m_s184;                              // 0x184
    CExoArrayList<CSWSExpression*> m_lstListenExpressions;  // 0x18c
    char pad_0x198[0x1a0 - 0x198];
    CExoArrayList<unsigned long> m_lst1A0;          // 0x1a0
    unsigned char* m_p1AC;                          // 0x1ac
    char pad_0x1b0[0x1c0 - 0x1b0];
    int m_nLastSpellId;                             // 0x1c0
    char pad_0x1c4[0x1d4 - 0x1c4];
    unsigned char m_nLastSpellCastClass;            // 0x1d4
    char pad_0x1d5[0x1ec - 0x1d5];
    int m_b1EC;                                     // 0x1ec
    char pad_0x1f0[0x224 - 0x1f0];
    CSWSObject224* m_p224;                          // 0x224
};

class CSWSCreature : public CSWSObject {
public:
    char pad_0x228[0x964 - 0x228];
    int m_n964;                                     // 0x964
    char pad_0x968[0xa74 - 0x968];
    CSWSCreatureStats* m_pStats;                    // 0xa74
};

#endif
