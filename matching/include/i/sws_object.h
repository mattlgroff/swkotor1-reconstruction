// CSWSObject leaves, CSWSObjectActionNode, CScriptLocation,
// CScriptTalent and CLoopingVisualEffect (0x004CA7A0-0x004CFB80).
// Self-contained declarations; only the members these functions use are
// placed.
#ifndef I_SWS_OBJECT_H
#define I_SWS_OBJECT_H

#include <stddef.h>

class CSWSObject;
class CSWSCreature;
class CSWSArea;

class CExoString {
public:
    CExoString();                                   // 0x005B3190
    CExoString(const CExoString& s);                // 0x005E5B00
    ~CExoString();                                  // 0x005E5C20
    char* m_sString;
    unsigned long m_nBufferLength;
};

struct Vector {
    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    float x, y, z;
};

int operator==(const Vector& a, const Vector& b);  // 0x004AA980 (cdecl)

struct CResStruct {
    unsigned long m_nStructIndex;
};

struct CResList {
    unsigned long m_nStructIndex;
    char m_pLabel[16];
};

class CResGFF {
public:
    unsigned char ReadFieldBYTE(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                unsigned char nDefault);                // 0x00411A60
    unsigned long ReadFieldDWORD(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                 unsigned long nDefault);               // 0x00411C20
    int ReadFieldINT(CResStruct* pStruct, char* szLabel, int& bSuccess,
                     int nDefault);                                     // 0x00411C90
    float ReadFieldFLOAT(CResStruct* pStruct, char* szLabel, int& bSuccess,
                         float fDefault);                               // 0x00411D00
    int AddList(CResList* pList, CResStruct* pStruct, char* szLabel);   // 0x00412450
    int AddListElement(CResStruct* pNew, CResList* pList, unsigned long nType);  // 0x004124E0
    int WriteFieldCExoString(CResStruct* pStruct, const CExoString& sValue,
                             char* szLabel);                        // 0x00412970
    int WriteFieldBYTE(CResStruct* pStruct, unsigned char nValue, char* szLabel);   // 0x00412620
    int WriteFieldDWORD(CResStruct* pStruct, unsigned long nValue, char* szLabel);  // 0x00412760
    int WriteFieldINT(CResStruct* pStruct, int nValue, char* szLabel);              // 0x004127B0
    int WriteFieldFLOAT(CResStruct* pStruct, float fValue, char* szLabel);          // 0x00412870
};

// 0x18 bytes: position then orientation.
class CScriptLocation {
public:
    CScriptLocation();                              // 0x004CA7A0
    CScriptLocation(const Vector& vPosition, const Vector& vOrientation)
    {
        m_vPosition = vPosition;
        m_vOrientation = vOrientation;
    }
    void CopyScriptLocation(CScriptLocation* pOther);   // 0x004CA7C0 (FUN_004ca7c0)
    int LoadLocation(CResGFF* pRes, CResStruct* pStruct);   // 0x004CA800
    int SaveLocation(CResGFF* pRes, CResStruct* pStruct);   // 0x004CA8E0

    Vector m_vPosition;                             // 0x00
    Vector m_vOrientation;                          // 0x0c
};

// 0x16 bytes.
class CScriptTalent {
public:
    CScriptTalent();                                // 0x004CA9E0
    void CopyScriptTalent(CScriptTalent* pOther);   // 0x004CAA10 (FUN_004caa10)
    int SaveTalent(CResGFF* pRes, CResStruct* pStruct);     // 0x004CAA50
    int LoadTalent(CResGFF* pRes, CResStruct* pStruct);     // 0x004CAAF0
    int Equals(CScriptTalent* pOther);              // 0x004CABB0 (FUN_004cabb0)

    int m_nType;                                    // 0x00
    int m_nId;                                      // 0x04
    unsigned char m_nMultiClass;                    // 0x08
    unsigned long m_oidItem;                        // 0x0c
    int m_nItemPropertyIndex;                       // 0x10
    unsigned char m_nCasterLevel;                   // 0x14
    unsigned char m_nMetaType;                      // 0x15
};

int FUN_004ca960(unsigned short nVisualEffect);     // 0x004CA960 (cdecl)

class CLoopingVisualEffect {
public:
    int GetIsBeam();                                // 0x004CA9D0

    unsigned short m_nVisualEffect;                 // 0x00
    unsigned long m_oidTarget;                      // 0x04 (placeholder name)
    unsigned char m_nBodyPart;                      // 0x08 (placeholder name)
};

typedef void* CExoLinkedListPosition;

struct CExoLinkedListNode {
    CExoLinkedListNode* pPrev;
    CExoLinkedListNode* pNext;
    void* pObject;
};

class CExoLinkedListInternal {
public:
    CExoLinkedListInternal();                       // 0x005E9AA0
    void* GetAtPos(CExoLinkedListPosition pos);     // 0x005E9B60
    void* GetNext(CExoLinkedListPosition& pos);     // 0x005E9B90
    void* Remove(CExoLinkedListPosition pos);       // 0x005E9C10
    void* RemoveHead();                             // 0x005E9C80
    CExoLinkedListPosition AddTail(void* p);        // 0x005E9DC0
    void* GetHeadObject() { return m_pHead ? m_pHead->pObject : NULL; }
    CExoLinkedListNode* m_pHead;
    CExoLinkedListNode* m_pTail;
    unsigned long m_nCount;
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedList();
    CExoLinkedListPosition GetHeadPos() { return m_pList->m_pHead; }
    T* GetHead() { return (T*)m_pList->GetHeadObject(); }
    T* GetAtPosInline(CExoLinkedListPosition pos)
    {
        CExoLinkedListNode* p = (CExoLinkedListNode*)pos;
        return p ? (T*)p->pObject : NULL;
    }
    T* GetAtPos(CExoLinkedListPosition pos) { return (T*)m_pList->GetAtPos(pos); }
    T* GetNext(CExoLinkedListPosition& pos) { return (T*)m_pList->GetNext(pos); }
    T* Remove(CExoLinkedListPosition pos) { return (T*)m_pList->Remove(pos); }
    T* RemoveHead() { return (T*)m_pList->RemoveHead(); }
    CExoLinkedListPosition AddTail(T* p) { return m_pList->AddTail(p); }
    int IsEmpty() { return m_pList->m_pHead == NULL; }
    CExoLinkedListInternal* m_pList;
};

template <class T>
CExoLinkedList<T>::CExoLinkedList()
{
    m_pList = new CExoLinkedListInternal();
}

template <class T> class CExoArrayList {
public:
    T& operator[](int i) { return element[i]; }
    void Allocate(int nSize);                       // 0x005E03C0
    void Remove(T t);                               // <unsigned long>: 0x0066DCC0
    int Count(T t)
    {
        int n = 0;
        for (int i = 0; i < num; i++) {
            if (element[i] == t) {
                n++;
            }
        }
        return n;
    }
    void Add(T t)
    {
        if (num == array_size) {
            Allocate(array_size ? array_size * 2 : 16);
        }
        element[num] = t;
        num++;
    }
    void DelIndex(int nIndex)
    {
        num--;
        for (; nIndex < num; nIndex++) {
            element[nIndex] = element[nIndex + 1];
        }
    }
    void SetSize(int nSize)
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

    T* element;
    int num;
    int array_size;
};

class CVirtualMachineScript;

class CVirtualMachine {
public:
    void DeleteScriptSituation(CVirtualMachineScript* pScript);   // 0x005D1110
};
extern CVirtualMachine* g_pVirtualMachine;          // 0x007A3A00

// 0x74 bytes.
class CSWSObjectActionNode {
public:
    CSWSObjectActionNode();                         // 0x004CAC00 (FUN_004cac00)
    ~CSWSObjectActionNode();                        // 0x004CC4D0

    unsigned long m_nActionId;                      // 0x00
    unsigned long m_nParamType[13];                 // 0x04
    void* m_pParam[13];                             // 0x38
    unsigned short m_nGroupActionId;                // 0x6c
    unsigned short m_nNumParams;                    // 0x6e
    int m_bInterruptable;                           // 0x70
};

class CSWSExpression {
public:
    CSWSExpression();                               // 0x0059D990
    void SetExpression(CExoString sExpression);     // 0x0059DAB0 (placeholder name)
    int TestExpression(CExoString& sText, int bFlag);   // 0x0059E170 (placeholder name)
    int m_nId;                                      // 0x00
    char pad_0x4[0x18 - 0x4];
    CExoString m_sExpression;                       // 0x18
};

class CSWSDialog {
public:
    virtual ~CSWSDialog();
    int FUN_005a03c0(unsigned long nReply, CSWSObject* pObject, int a, int b, int c,
                     int d);                        // 0x005A03C0
    void RunEndConversationScript(CSWSObject* pObject, int bAborted);  // 0x005A0A40
    int RemovePlayer(unsigned long oid);            // 0x0059FB60
    int SendDialogReplies(CSWSObject* pObject, int nIndex);    // 0x005A3820 (placeholder name)
    int SendDialogEntry(CSWSObject* pObject, int nIndex, int nEntry,
                        int* pStatus);              // 0x005A4010 (placeholder name)
    void FUN_004cc490(void* p);                     // 0x004CC490
    unsigned long FUN_004cc4b0();                   // 0x004CC4B0
    unsigned long GetOwnerInline()
    {
        if (m_lstPlayers.m_pList->m_pHead) {
            return m_oidOwner;
        }
        return 0x7f000000;
    }

    char pad_0x4[0x64 - 0x4];
    CExoLinkedList<void> m_lstPlayers;              // 0x64 (placeholder name)
    char pad_0x68[0x7c - 0x68];
    unsigned long m_oidOwner;                       // 0x7c (placeholder name)
};

class CSWCObject {
public:
    void FUN_0063d520(CSWSObject* pObject);         // 0x0063D520
};

class CGameObject {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual CSWCObject* AsSWCObject();              // +0x0c
    virtual CSWSObject* AsSWSObject();              // +0x10
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual CSWSArea* AsSWSArea();                  // +0x28
    virtual void v2c();
    virtual CSWSCreature* AsSWSCreature();          // +0x30

    unsigned long m_idSelf;                         // +0x04
    unsigned char m_nObjectType;                    // +0x08
};

class CGameObjectArray {
public:
    unsigned char GetGameObject(unsigned long oid, CGameObject** ppObject); // 0x004D8230
};

struct CSWCCreatureEntry {
    char pad_0x0[0x2c];
    unsigned long m_oidObject;                      // 0x2c
    char pad_0x30[0x148 - 0x30];
    int m_bDeleteNoFade;                            // 0x148 (placeholder name)
    int m_bKeepCorpse;                              // 0x14c (placeholder name)
    int m_nFadeOutTime;                             // 0x150 (placeholder name)
};

class CSWSClient {
public:
    char pad_0x0[0xc];
    CExoLinkedList<CSWCCreatureEntry>* m_pEntries;  // 0x0c (placeholder name)
};

class CServerExoApp {
public:
    CGameObjectArray* GetObjectArray();             // 0x004AED70
    CGameObject* GetGameObject(unsigned long oid);  // 0x004AE750
    CSWSClient* GetClientObjectByObjectID(unsigned long oid);   // 0x004AE910
    unsigned long GetPlayerCreatureID();            // 0x004AEA40
};

class CGameObject;

class CClientExoApp {
public:
    CGameObject* GetGameObject(unsigned long oid);  // 0x005ED580
    unsigned long ServerToClientObjectId(unsigned long oid);    // 0x005EDA50
    void StopSoundPlayingInDialog();                // 0x005EDBD0
};

class CAppManager {
public:
    char pad_0x0[0x4];
    CClientExoApp* m_pClientExoApp;                 // 0x4
    CServerExoApp* m_pServerExoApp;                 // 0x8
};
extern CAppManager* g_pAppManager;                  // 0x007A39FC
inline CServerExoApp* GetServerExoApp() { return g_pAppManager->m_pServerExoApp; }

class CExoTimers {
public:
    unsigned __int64 GetHighResolutionTimer();      // 0x005E9260
};

class CExoBase {
public:
    char pad_0x0[0x4];
    CExoTimers* m_pcExoTimers;                      // 0x4
};
extern CExoBase* g_pExoBase;                        // 0x007A39E0

struct CSWFeat {
    char pad_0x0[0x4];
    unsigned short m_nSpellId;                      // 0x04 (placeholder name)
};

class CSWRules {
public:
    CSWFeat* GetFeat(unsigned short nFeat);         // 0x00550C00
};
extern CSWRules* g_pRules;                          // 0x007A3A28

class CGameEffect {
public:
    void SaveGameEffect(CResGFF* pRes, CResStruct* pStruct);   // 0x00503790
    char pad_0x0[0x18];
    unsigned long m_oidCreator;                     // 0x18
    int m_nSpellId;                                 // 0x1c
};

class CSWSArea;

class CSWSArea {
public:
    void FUN_00506fe0(CSWSObject* pObject);         // 0x00506FE0
    char pad_0x0[0x120];
    unsigned long m_idSelf;                         // 0x120
};

class CSWSScriptVarTable {
public:
    void SaveVarTable(CResGFF* pRes, CResStruct* pStruct);  // 0x0059ADB0
    char pad_0x0[0x10];
};

class CSWVarTable {
public:
    void SaveVarTable(CResGFF* pRes, CResStruct* pStruct);  // 0x0059B250
    char pad_0x0[0x14];
};

extern const unsigned long OBJECT_INVALID_ID;       // 0x00746644
extern const unsigned char GAME_OBJECT_VALID;       // 0x0074666C

// vtable 0x00745F18
class CSWSObject : public CGameObject {
public:
    virtual void v34();
    virtual void v38();
    virtual void v3c();
    virtual void v40();
    virtual void v44();
    virtual void v48();
    virtual void v4c();
    virtual void v50();
    virtual void v54();
    virtual void v58();
    virtual void v5c();
    virtual void v60();
    virtual void v64();
    virtual void v68();
    virtual void v6c();
    virtual void v70();
    virtual int RunActionNode(CSWSObjectActionNode* pNode, int bFlag);  // 0x74 (placeholder name)
    virtual void v78();
    virtual void v7c();
    virtual void v80();
    virtual void v84();
    virtual void v88();
    virtual void v8c();
    virtual void v90();
    virtual void v94();
    virtual void v98();
    virtual short GetCurrentHitPoints(int bExcludeTemporary);   // 0x9c

    // 0x004CA000-0x004CFFFF
    int ReplyDialog(unsigned long nReply, int a, int b, int c, int d);  // 0x004CB480
    int StopDialog();                               // 0x004CB4F0
    void StopSoundPlayingInDialog();                // 0x004CB5A0
    void SetDialogOwner(unsigned long oid);         // 0x004CB7A0
    CScriptLocation GetScriptLocation();            // 0x004CB7B0 (FUN_004cb7b0)
    int GetDead();                                  // 0x004CB810 (FUN_004cb810)
    unsigned long FUN_004cafe0(unsigned long nMask);    // 0x004CAFE0
    CSWSArea* GetArea();                            // 0x004CB120
    void SetArea(CSWSArea* pArea);                  // 0x004CB160 (FUN_004cb160)
    int FUN_004cc1c0(unsigned short nId);           // 0x004CC1C0
    void SetUpdateFlag(unsigned long nBit);         // 0x004CC1F0 (FUN_004cc1f0)
    int GetUpdateFlag(unsigned long nBit);          // 0x004CC220 (FUN_004cc220)
    void ClearUpdateFlag(unsigned long nBit);       // 0x004CC250 (FUN_004cc250)
    void SetCurrentHitPoints(int nHP);              // 0x004CC280 (FUN_004cc280)
    CSWCObject* GetClientObject();                  // 0x004CC2B0
    void AttachmentFromClientObject(CSWCObject* p); // 0x004CC310
    void DetachFromClientObject();                  // 0x004CC320
    void ClearDialog();                             // 0x004CC330 (FUN_004cc330)
    void TimeStampCameraAnimation();                // 0x004CC360
    void SetLookAtTarget(unsigned long oid);        // 0x004CC440 (FUN_004cc440)
    unsigned short GetActionByGroupId(unsigned short nGroupId,
                                      CSWSObjectActionNode** ppNode);   // 0x004CC530
    CExoLinkedListPosition GetPositionByGroupIndex(int nIndex);         // 0x004CC5C0
    CSWSObjectActionNode* GetNodeById(unsigned short nId);              // 0x004CC630
    int GetNumActionGroups();                       // 0x004CC690
    void RemoveGroup(unsigned short nGroupId);      // 0x004CC6F0
    void SetGroupInterruptable(unsigned short nGroupId, int bInterruptable);  // 0x004CC790
    void DeleteCurrentAIAction();                   // 0x004CCD30
    void SetAnimation(int nAnimation);              // 0x004CCF60
    void DamageHitPoints(int nDamage);              // 0x004CCF80 (FUN_004ccf80)
    void SetOrientation(const Vector& v);           // 0x004CD170 (FUN_004cd170)
    void RemoveLoopingVisualEffect(unsigned short nVisualEffect);       // 0x004CD240
    int RemoveObjectFromDialog(unsigned long oid);  // 0x004CD660
    CSWSExpression* GetListenExpressionById(int nId);   // 0x004CD6C0 (FUN_004cd6c0)
    int TestListenExpression(CExoString& sText);    // 0x004CD700
    int GetHasSpellEffectApplied(int nSpellId);     // 0x004CDAC0 (FUN_004cdac0)
    int GetHasFeatEffectApplied(unsigned short nFeat);  // 0x004CE680
    void SetFadeOutTime(int nTime);                 // 0x004CE8A0
    void SetDeleteNoFade(int bValue);               // 0x004CE920
    void SetKeepCorpse(int bValue);                 // 0x004CE9A0
    void SaveObjectState(CResGFF* pRes, CResStruct* pStruct);   // 0x004CEC50
    void AddLoopingVisualEffect(unsigned short nVisualEffect, unsigned long oidTarget,
                                unsigned char nBodyPart);   // 0x004CF390
    void ClearMatchedExpressionStrings();           // 0x004CFA50
    int AddOpenDoorAction(unsigned long oidDoor, int bFlag);    // 0x004CFAC0
    int AddCloseDoorAction(unsigned long oidDoor, int bFlag);   // 0x004CFB20

    void SetPosition(const Vector& vPosition, int bUpdateArea, int bUnused,
                     int bNoUpdateFlag);            // 0x004CD1A0
    int IsDialogDelay();                            // 0x004CD2D0
    int SendDialogEntry(int nEntry);                // 0x004CD460
    int SendDialogReplies();                        // 0x004CD500
    void SaveEffectList(CResGFF* pRes, CResStruct* pStruct);    // 0x004CC9D0
    void SaveListenData(CResGFF* pRes, CResStruct* pStruct);    // 0x004CCA50
    void RemoveEffectTarget(unsigned long oid);     // 0x004CF3F0
    void SetListenExpression(const CExoString& sExpression, int nId);  // 0x004CF8F0
    void AddMatchedExpressionString(const CExoString& sString);     // 0x004CF9B0
    int AddUnlockObjectAction(unsigned long oidObject, unsigned long oidKey,
                              int bFlag);           // 0x004CFB80
    int AddLockObjectAction(unsigned long oidObject);   // 0x004CFC20

    int FUN_0057a240(unsigned long nActionId);      // 0x0057A240
    void SaveActionQueue(CResGFF* pRes, CResStruct* pStruct);   // 0x004CC7E0
    void AddAction(int nActionId, unsigned short nGroupId,
                   int nType1, void* p1, int nType2 = 0, void* p2 = NULL,
                   int nType3 = 0, void* p3 = NULL, int nType4 = 0, void* p4 = NULL,
                   int nType5 = 0, void* p5 = NULL, int nType6 = 0, void* p6 = NULL,
                   int nType7 = 0, void* p7 = NULL, int nType8 = 0, void* p8 = NULL,
                   int nType9 = 0, void* p9 = NULL, int nType10 = 0, void* p10 = NULL,
                   int nType11 = 0, void* p11 = NULL, int nType12 = 0, void* p12 = NULL,
                   int nType13 = 0, void* p13 = NULL);  // 0x004CEA20

    char pad_0xc[0x34 - 0xc];
    CSWSDialog* m_pDialog;                          // 0x34
    int m_n38;                                      // 0x38
    int m_n3c;                                      // 0x3c
    char pad_0x40[0x48 - 0x40];
    int m_n48;                                      // 0x48
    int m_n4c;                                      // 0x4c
    int m_n50;                                      // 0x50
    unsigned long m_oidDialogOwner;                 // 0x54
    char pad_0x58[0x68 - 0x58];
    unsigned long m_nCameraAnimationTime;           // 0x68
    char pad_0x6c[0x70 - 0x6c];
    int m_n70;                                      // 0x70
    char pad_0x74[0x8c - 0x74];
    unsigned long m_oidArea;                        // 0x8c
    Vector m_vPosition;                             // 0x90
    Vector m_vOrientation;                          // 0x9c
    char pad_0xa8[0xd4 - 0xa8];
    int m_nAnimation;                               // 0xd4
    char pad_0xd8[0xdc - 0xd8];
    int m_nCurrentHitPoints;                        // 0xdc
    char pad_0xe0[0xe8 - 0xe0];
    int m_bCommandable;                             // 0xe8
    char pad_0xec[0xfc - 0xec];
    CExoLinkedList<CSWSObjectActionNode> m_lQueuedActions;  // 0xfc
    CSWSScriptVarTable m_ScriptVars;                // 0x100
    CSWVarTable m_VarTable;                         // 0x110
    CExoArrayList<CGameEffect*> m_appliedEffects;   // 0x124
    CExoArrayList<CLoopingVisualEffect*> m_lstLoopingVisualEffects; // 0x130
    CExoArrayList<unsigned long> m_lstEffectTargets;    // 0x13c
    char pad_0x148[0x168 - 0x148];
    unsigned long* m_p168;                          // 0x168
    char pad_0x16c[0x18c - 0x16c];
    CExoArrayList<CSWSExpression*> m_lstListenExpressions;  // 0x18c
    char pad_0x198[0x1a0 - 0x198];
    CExoArrayList<CExoString*> m_lstMatchedExpressionStrings;   // 0x1a0
    char pad_0x1ac[0x1f4 - 0x1ac];
    int m_n1f4;                                     // 0x1f4
    char pad_0x1f8[0x1fc - 0x1f8];
    unsigned long m_nUpdateFlags[1];                // 0x1fc
    int m_bPlotObject;                              // 0x200 (placeholder name)
    int m_bDeleteNoFade;                            // 0x204
    int m_bKeepCorpse;                              // 0x208
    int m_nFadeOutTime;                             // 0x20c
    char pad_0x210[0x214 - 0x210];
    int m_n214;                                     // 0x214
    char pad_0x218[0x220 - 0x218];
    int m_bStopSoundPlaying;                        // 0x220 (placeholder name)
    CSWCObject* m_pClientObject;                    // 0x224
    char pad_0x228[0x254 - 0x228];
    unsigned long m_oidLookAtTarget;                // 0x254 (placeholder name)
    Vector m_vLookAt;                               // 0x258 (placeholder name)
};

class CSWSCreatureStats {
public:
    int GetCanUseSkill(unsigned char nSkill);       // 0x005AF880
};

class CSWSCreature : public CSWSObject {
public:
    void FUN_004f34a0(unsigned long oid, int n);    // 0x004F34A0
    void SendFeedbackMessage(unsigned short nMessage, void* pData);    // 0x004EDE10
    CSWSCreatureStats* GetCreatureStats() { return m_pStats; }
    char pad_0x264[0x9d4 - 0x264];
    int m_b9d4;                                     // 0x9d4
    char pad_0x9d8[0xa74 - 0x9d8];
    CSWSCreatureStats* m_pStats;                    // 0xa74
};

#endif
