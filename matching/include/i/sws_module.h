// CPathfindInformation / CWalkHitInfo leaves (0x004BB340-0x004BB570)
// and the CSWSModule inter-area path / update / journal / limbo cluster with
// its CExoArrayList / CResIFO leaves (0x004C30C0-0x004C8940).
// Self-contained declarations; only the members these functions use are
// placed.
#ifndef I_SWS_MODULE_H
#define I_SWS_MODULE_H

#include <stddef.h>

struct Vector {
    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    float x, y, z;
};

// 0x48 bytes (+0x40 not touched by Initialize).
class CWalkHitInfo {
public:
    void Initialize(int nType, const Vector& v1, const Vector& v2, const Vector& v3,
                    const Vector& v4, int nIndex, const Vector& v5);    // 0x004BB570

    int m_nType;                                    // +0x00
    Vector m_v04;                                   // +0x04
    Vector m_v10;                                   // +0x10
    Vector m_v1c;                                   // +0x1c
    Vector m_v28;                                   // +0x28
    Vector m_v34;                                   // +0x34
    int m_n40;                                      // +0x40
    int m_n44;                                      // +0x44
};

class CPathfindInformation {
public:
    void FUN_004bb340(float* p);                    // 0x004BB340
    void FUN_004bb380(Vector* pA, Vector* pB);      // 0x004BB380
    void FUN_004bb3c0(const Vector* pA, const Vector* pB);  // 0x004BB3C0
    void FUN_004bb400(int n, float* p);             // 0x004BB400
    void FUN_004bb440(int n, float* p);             // 0x004BB440
    void FUN_004bb480(float* p);                    // 0x004BB480
    void FUN_004bb4d0(float* p);                    // 0x004BB4D0
    void FUN_004bb520(float* p);                    // 0x004BB520
    void FUN_005cec50();                            // 0x005CEC50 (flips the path direction)
    void FUN_005cec90();                            // 0x005CEC90
    void ResetGridSearchData();                     // 0x005CEFA0
    void ResetInterTileSearchData();                // 0x005D00A0
    void ResetInterAreaPathSearchData();            // 0x005D0E00
    void ResetInterAreaSearchData();                // 0x005D0E70

    char m_pad0[4];
    float m_f04;                                    // +0x04
    char m_pad08[0x2c - 8];
    unsigned long m_oidCreature;                    // +0x2c
    unsigned long m_oidTransition;                  // +0x30
    char m_pad34[0x3c - 0x34];
    int m_n3c;                                      // +0x3c
    char m_pad40[4];
    Vector m_v44;                                   // +0x44
    float m_f50;                                    // +0x50
    unsigned long m_oid54;                          // +0x54
    int m_nState;                                   // +0x58
    Vector m_vGoal;                                 // +0x5c
    float m_f68;                                    // +0x68
    char m_pad6c[4];
    unsigned long m_oidTargetArea;                  // +0x70
    Vector m_vTarget;                               // +0x74
    unsigned long m_oidStartArea;                   // +0x80
    int m_n84;                                      // +0x84
    int m_n88;                                      // +0x88
    char m_pad8c[0xa0 - 0x8c];
    int m_bInterAreaPathFound;                      // +0xa0
    int m_nA4;                                      // +0xa4
    int m_nAreas;                                   // +0xa8
    unsigned long* m_pAreas;                        // +0xac
    unsigned long* m_pTransitions;                  // +0xb0
    int m_nExcluded;                                // +0xb4
    unsigned long* m_pExcluded;                     // +0xb8
    char m_padbc[0x1ac - 0xbc];
    float* m_p1ac;                                  // +0x1ac
    char m_pad1b0[0x1e4 - 0x1b0];
    Vector m_v1e4;                                  // +0x1e4
    Vector m_v1f0;                                  // +0x1f0
    int m_n1fc;                                     // +0x1fc
    float* m_p200;                                  // +0x200
    int m_n204;                                     // +0x204
    float* m_p208;                                  // +0x208
    char m_pad20c[0x218 - 0x20c];
    int m_n218;                                     // +0x218
    float* m_p21c;                                  // +0x21c
    int m_n220;                                     // +0x220
    int m_n224;                                     // +0x224
    float* m_p228;                                  // +0x228
    float* m_p22c;                                  // +0x22c
};

// ---- module cluster ---------------------------------------------------------

extern "C" char* strncpy(char* d, const char* s, size_t n);
extern "C" int strcmp(const char* a, const char* b);
#pragma intrinsic(strcmp)

class CExoString {
public:
    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    CExoString(const CExoString& s);                // 0x005E5B00
    CExoString(int n);                              // 0x005E5BC0
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    CExoString& operator=(const char* s);           // 0x005E5140
    int operator==(const char* s) const;            // 0x005E5290
    int operator==(const CExoString& s) const;      // 0x005E5200
    CExoString operator+(const CExoString& s) const;    // 0x005E5D10
    char* CStr() const;                             // 0x005E5670
    int GetLength() const;                          // 0x005E5790
    int Find(char c, int nStart) const;             // 0x005E5F20
    CExoString LowerCase() const;                   // 0x005E6080
    CExoString Right(int nCount) const;             // 0x005E6170
    void Format(const char* szFormat, ...);         // 0x005E5680
    char* m_sString;
    unsigned long m_nBufferLength;
};

// 8 bytes; Compare is 0x005EA020 (non-zero when equal).
class CExoLocString {
public:
    CExoLocString();                                // 0x005E9E10
    ~CExoLocString();                               // 0x005EA000
    int Compare(const CExoLocString& o) const;      // 0x005EA020
    int operator==(const CExoLocString& o);         // 0x005EA020
    CExoLocString& operator=(const CExoLocString& o);   // 0x005E9E70
    void* m_pInternal;
    unsigned long m_n4;
};

struct CResRef {
    char* GetResRefStr() const;                     // 0x00405FE0
    CResRef();                                      // 0x00405ED0
    CResRef(const char* s);                         // 0x00406D80
    CResRef(const CExoString& s);                   // 0x00406D60
    CResRef operator=(const CExoString& s);         // 0x004061F0
    CResRef operator=(const CResRef& o);            // 0x00406120
    int operator!=(const char* s) const;            // 0x00406060
    char m_resRef[16];
};

struct CResStruct {
    unsigned long m_nStructIndex;
};

struct CResList {
    unsigned long m_nStructIndex;
    char m_pLabel[16];
};

class CRes {
public:
    CRes();                                         // 0x00406460
    virtual ~CRes();
    void CancelRequest();                           // 0x00409B00
    void* Demand();                                 // 0x00409B10
    int Release();                                  // 0x00409B80
    char m_pad4[0x28 - 4];
};

class CExoStringList {
public:
    ~CExoStringList();                              // 0x005E9860
    CExoString* GetString(int n);                   // 0x005E95E0
    int GetCount();                                 // 0x00675DD0
};

class CExoResMan {
public:
    CRes* GetResObject(const CResRef& cResRef, unsigned short nType);       // 0x004074D0
    void SetResObject(const CResRef& cResRef, unsigned short nType, CRes* pRes); // 0x00407680
    CExoStringList* GetResOfType(unsigned short nType, CRes* pRes);         // 0x00408C60
    int ReleaseResObject(CRes* pRes);               // 0x00409CF0
    void AddResourceImageFilesAsync(const CExoString& sName, unsigned long nArg,
                                    unsigned long bSaveGame);              // 0x004064F0
    int AddEncapsulatedResourceFile(const CExoString& sName, unsigned long nPriority); // 0x004087A0
    int AddResourceDirectory(const CExoString& sName);                     // 0x00408800
    int RemoveResourceDirectory(const CExoString& sName);                  // 0x004088D0
    int RemoveEncapsulatedResourceFile(const CExoString& sName);           // 0x00408820
    int RemoveResourceImageFile(const CExoString& sName);                  // 0x00408830
    int Exists(const CResRef& cResRef, unsigned short nType, unsigned long* pSize); // 0x00408BC0
    int FUN_0063e7c0(const CExoString& sModule);    // 0x0063E7C0 (shared stub)
    char m_pad0[0x44];
    unsigned char m_bAsyncLoading;                  // +0x44 (placeholder name)
    char m_pad45[0x5c - 0x45];
    unsigned char m_bAsyncResult;                   // +0x5c (placeholder name)
};
extern CExoResMan* g_pExoResMan;                    // 0x007A39E8

class CResGFF : public CRes {
public:
    CResGFF();                                      // 0x004105A0
    virtual ~CResGFF();
    int AddList(CResList* pList, CResStruct* pStruct, char* szLabel);               // 0x00412450
    int AddListElement(CResStruct* pStruct, CResList* pList, unsigned long nType);  // 0x004124E0
    int WriteFieldDWORD(CResStruct* pStruct, unsigned long nValue, char* szLabel);  // 0x00412760
    int GetTopLevelStruct(CResStruct* pStruct);                                     // 0x00411240
    int WriteFieldVOID(CResStruct* pStruct, void* pData, unsigned long nSize,
                       char* szLabel);                                              // 0x00412C10
    int WriteFieldINT(CResStruct* pStruct, int nValue, char* szLabel);              // 0x004127B0
    int WriteFieldWORD(CResStruct* pStruct, unsigned short nValue, char* szLabel);  // 0x004126C0
    int WriteFieldDWORD64(CResStruct* pStruct, unsigned __int64 nValue, char* szLabel); // 0x00412800
    int WriteFieldFLOAT(CResStruct* pStruct, float fValue, char* szLabel);          // 0x00412870
    int WriteFieldCResRef(CResStruct* pStruct, const CResRef& cValue, char* szLabel); // 0x004128C0
    int WriteFieldCExoLocString(CResStruct* pStruct, CExoLocString& sValue,
                                char* szLabel);                                     // 0x00412A10
    int GetList(CResList* pList, CResStruct* pStruct, char* szLabel);               // 0x004118C0
    unsigned long GetListCount(CResList* pList);                                    // 0x00411940
    int GetListElement(CResStruct* pStruct, CResList* pList, unsigned long nIndex); // 0x00411990
    unsigned long GetStructID(CResStruct* pStruct);                                 // 0x004111C0
    CExoString ReadFieldCExoString(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                   const CExoString& sDefault);                     // 0x00411EC0
    unsigned long ReadFieldDWORD(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                 unsigned long nDefault);                           // 0x00411C20
    float ReadFieldFLOAT(CResStruct* pStruct, char* szLabel, int& bSuccess,
                         float fDefault);                                           // 0x00411D00
    int WriteFieldBYTE(CResStruct* pStruct, unsigned char nValue, char* szLabel);   // 0x00412620
    int WriteFieldCExoString(CResStruct* pStruct, const CExoString& sValue,
                             char* szLabel);                                        // 0x00412970
    int CreateGFFFile(CResStruct* pStruct, const CExoString& sType,
                      const CExoString& sVersion);                                  // 0x00411260
    int WriteGFFFile(CExoString& sFileName, unsigned short nType);                  // 0x00413030
    char m_pad28[0x8d - 0x28];
    char m_pFileType[4];                            // +0x8d
    char m_pad91[0xa0 - 0x91];
};

class CResIFO : public CResGFF {
public:
    CResIFO();                                      // 0x004C30C0
    virtual ~CResIFO();
};

template <class T, int nType>
class CResHelper {
public:
    CResHelper() {}
    virtual ~CResHelper();                          // <CResIFO, 2014> 0x004C4C60
    int m_bAutoRequest;
    T* m_pRes;
    CResRef m_cResRef;
};

template <class T>
class CExoArrayList {
public:
    CExoArrayList(int nSize = 0)
    {
        num = 0;
        array_size = nSize;
        element = NULL;
        if (array_size)
            element = new T[array_size];
    }
    void Allocate(int n)
    {
        T* pOld = element;
        array_size = n;
        element = new T[n];
        for (int i = 0; i < num; i++) {
            element[i] = pOld[i];
        }
        if (pOld) {
            delete[] pOld;
        }
    }
    void Add(T t)
    {
        if (num == array_size) {
            Allocate(array_size ? array_size * 2 : 16);
        }
        element[num] = t;
        num++;
    }
    void AddInline(T t)
    {
        if (num == array_size) {
            Allocate(array_size ? array_size * 2 : 16);
        }
        element[num++] = t;
    }
    void Insert(T t, int nIndex)
    {
        int i = num;
        Add(t);
        for (; i > nIndex; i--)
            element[i] = element[i - 1];
        element[nIndex] = t;
    }
    void DelIndex(int nIndex)
    {
        num--;
        for (; nIndex < num; nIndex++) {
            element[nIndex] = element[nIndex + 1];
        }
    }
    void Remove(T t)
    {
        for (int i = num - 1; i >= 0; i--) {
            if (t == element[i]) {
                DelIndex(i);
                return;
            }
        }
    }
    int Contains(T t)
    {
        for (int i = num - 1; i >= 0; i--) {
            if (t == element[i]) {
                return 1;
            }
        }
        return 0;
    }
    int contains(T t);
    void Clear()
    {
        if (element) {
            delete[] element;
            element = NULL;
            array_size = 0;
        }
        num = 0;
    }
    ~CExoArrayList()
    {
        delete[] element;
        element = NULL;
    }
    T& operator[](int i) { return element[i]; }

    T* element;
    int num;
    int array_size;
};

// 0x28 bytes, sorted by tag.
struct CSWSTagNode {
    char m_szTag[0x20];
    unsigned long m_n20;
    unsigned long m_n24;
};

// 8-byte element of the list grown by 0x004C4F50 (layout unknown).
struct CSWSModulePair {
    unsigned long m_n0;
    unsigned long m_n4;
};

struct NWPlayerCharacterList_st {
    NWPlayerCharacterList_st();                     // 0x004C6830
    int operator==(const NWPlayerCharacterList_st& o) const
    {
        return m_sFirstName.Compare(o.m_sFirstName) && m_sLastName.Compare(o.m_sLastName);
    }
    ~NWPlayerCharacterList_st()                     // 0x004C5DF0
    {
        delete m_pData;
        m_pData = NULL;
    }
    CExoLocString m_sFirstName;                     // +0x00
    CExoLocString m_sLastName;                      // +0x08
    unsigned char m_nType;                          // +0x10 (placeholder name)
    CResRef m_cResRef;                              // +0x11
    char m_pad21[0x34 - 0x21];
    unsigned char* m_pData;                         // +0x34
    char m_pad38[0x40 - 0x38];
};

// 0x10 bytes; ctor 0x0059EA90 and dtor 0x0059EA30 are out of line.
class CSWSModuleStringEntry {
public:
    CSWSModuleStringEntry();                        // 0x0059EA90
    ~CSWSModuleStringEntry();                       // 0x0059EA30
    CExoString m_sName;                             // +0x00
    unsigned long m_n8;                             // +0x08
    unsigned long m_nC;                             // +0x0c
};

// 0x24 bytes.
class CWorldJournalEntry {
public:
    CWorldJournalEntry() {}                         // 0x004C3FE0
    CExoString m_sName;                             // +0x00
    CExoString m_sText;                             // +0x08
    unsigned long m_n10;
    unsigned long m_n14;
    unsigned long m_n18;
    unsigned long m_nID;                            // +0x1c (id or strref)
    unsigned long m_bStrref : 1;                    // +0x20
};

class CGameObject;
class CSWSCreature;
class CSWSArea;
class CSWSPlayer;
class CSWSWaypoint;

extern const unsigned char GAME_OBJECT_VALID;       // 0x0074666C
extern const unsigned char OBJECT_TYPE_TRIGGER;     // 0x00746636
extern const unsigned char OBJECT_TYPE_DOOR;        // 0x00746639
extern const unsigned char OBJECT_TYPE_WAYPOINT;    // 0x0074663B
extern const int EFFECT_TYPE_TIMESTOP;              // 0x0074677C
extern const int PLOT_PATH_OK;                      // 0x00745DBC
extern const int PLOT_PATH_DONE;                    // 0x00745DC0
extern const int PLOT_PATH_FAIL;                    // 0x00745DC4
extern int g_bWalkDebug;                            // 0x00832838

class CGameObjectArray {
public:
    unsigned char GetGameObject(unsigned long oid, CGameObject** ppObject); // 0x004D8230
    unsigned long GetNextObjectArrayID(int i) { return m_nNextObjectArrayID[i]; }
    unsigned long GetNextCharArrayID(int i) { return m_nNextCharArrayID[i]; }
    char m_pad0[4];
    unsigned long m_nNextObjectArrayID[2];          // +0x04
    unsigned long m_nNextCharArrayID[2];            // +0x0c
};

typedef void* CExoLinkedListPosition;

struct CExoLinkedListNode {
    CExoLinkedListNode* pPrev;
    CExoLinkedListNode* pNext;
    void* pObject;
};

class CExoLinkedListInternal {
public:
    void* GetAtPos(CExoLinkedListPosition pos);     // 0x005E9B60
    void* GetNext(CExoLinkedListPosition& pos);     // 0x005E9B90
    CExoLinkedListNode* m_pHead;
    CExoLinkedListNode* m_pTail;
    unsigned long m_nCount;
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedListPosition GetHeadPos() { return m_pList->m_pHead; }
    T* GetAtPos(CExoLinkedListPosition pos) { return (T*)m_pList->GetAtPos(pos); }
    T* GetNext(CExoLinkedListPosition& pos) { return (T*)m_pList->GetNext(pos); }
    CExoLinkedListInternal* m_pList;
};

class CSWSObject {
public:
    virtual ~CSWSObject();
    CSWSArea* GetArea();                            // 0x004CB120
    void SetOrientation(const Vector& vOrientation);    // 0x004CD170
    unsigned long m_idSelf;                         // +0x04
};

class CSWSTrigger : public CSWSObject {
public:
    unsigned long GetTargetArea();                  // 0x0058C830
    int FUN_0058d8c0(Vector* pIn, Vector* pOut);    // 0x0058D8C0
    char m_pad8[0x2b4 - 8];
    int m_bAreaTransition;                          // +0x2b4
};

class CSWSDoor : public CSWSObject {
public:
    int FUN_005890d0();                             // 0x005890D0
    void FUN_00589240(Vector* pOut, const Vector& vIn, int n);    // 0x00589240
    CSWSObject* GetLinkedObject();                  // 0x00589580
};

class CGameEffect {
public:
    char m_pad0[8];
    unsigned short m_nType;                         // +0x08
};

class CSWSCreatureStats {
public:
    char m_pad0[0x34];
    CExoLocString m_lsFirstName;                    // +0x34
    CExoLocString m_lsLastName;                     // +0x3c
};

class CSWSCreature : public CSWSObject {
public:
    CSWSCreature(unsigned long oid, int bArg);      // 0x004F7A10
    float FUN_005111c0();                           // 0x005111C0
    int SaveCreature(CResGFF* pRes, CResStruct* pStruct);  // 0x00500610 (placeholder name)
    int LoadCreature(CResGFF* pRes, CResStruct* pStruct, int bLoadFromSave);  // 0x00500350
    CSWSCreatureStats* GetCreatureStats() { return m_pStats; }
    void DumpToLog();                               // 0x004EF640
    void RemoveFromArea(int bArg);                  // 0x004F0FD0
    char m_pad8[0x124 - 8];
    CExoArrayList<CGameEffect*> m_appliedEffects;   // +0x124
    char m_pad130[0x34c - 0x130];
    int m_bLoadedFromSave;                          // +0x34c (placeholder name)
    char m_pad350[0xa74 - 0x350];
    CSWSCreatureStats* m_pStats;                    // +0xa74
    char m_padA78[0xac8 - 0xa78];
};

class CGameObject {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual CSWSDoor* AsSWSDoor();                  // +0x18
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual CSWSArea* AsSWSArea();                  // +0x28
    virtual void v2c();
    virtual CSWSCreature* AsSWSCreature();          // +0x30
    virtual void v34();
    virtual void v38();
    virtual void v3c();
    virtual CSWSTrigger* AsSWSTrigger();            // +0x40

    unsigned long m_idSelf;                         // +0x04
    unsigned char m_nObjectType;                    // +0x08
};

class CSWSArea {
public:
    CResRef GetResRef() { return m_cResRef; }
    int CountAreaTransitionTriggers(unsigned long* pList, int nCount,
                                    unsigned long* pExclude);       // 0x004BC400
    void FUN_00508a30();                            // 0x00508A30
    int LoadAreaFromData(void* pData, CExoString& sGitName,
                         unsigned long nSize);      // 0x0050BA00 (placeholder name)
    CSWSArea(const CResRef& cResRef, int bArg, unsigned long oid);  // 0x0050CF80
    virtual ~CSWSArea();
    int LoadArea(int bLoadFromSave);                // 0x0050E190
    char m_pad4[0x10c - 4];
    CResRef m_cResRef;                              // +0x10c
    char m_pad11c[4];
    unsigned long m_oid120;                         // +0x120
    char m_pad124[0x1e0 - 0x124];
    int m_n1e0;                                     // +0x1e0
    char m_pad1e4[0x2d4 - 0x1e4];
};

class CSWSPlayer {
public:
    CSWSPlayer(unsigned long oid);                  // 0x00562C70
    virtual ~CSWSPlayer();
    CGameObject* GetGameObject();                   // 0x00561080
    void SaveServerCharacter();                     // 0x005624C0
    int LoadCharacterFromIFO(unsigned long nIndex, int bArg1, int bArg2);   // 0x00561E30
    CExoString GetPlayerName();                     // 0x005610B0
    int GetIsPrimaryPlayer() { return m_bIsPrimaryPlayer; }
    char m_pad4[0x38 - 4];
    unsigned long m_oidNWSObject;                   // +0x38
    int m_bIsPrimaryPlayer;                         // +0x3c
    char m_pad40[0x84 - 0x40];
};

class CERFFile {
public:
    CERFFile();                                     // 0x005DD9C0
    ~CERFFile();                                    // 0x005DE3A0
    int Create(CExoString& sFileName);              // 0x005DCFC0
    void SetVersion(const char* szTypeVersion);     // 0x005DCE30 (placeholder name; folded)
    int WriteHeader();                              // 0x005DD080
    int WriteStringTable();                         // 0x005DD5A0
    int SetNumEntries(unsigned long nEntries);      // 0x005DDA50
    char m_pad0[0xd0];
    int AddResource(const char* szName, unsigned short nType, void* pRes, int bA,
                    int bB);                        // 0x005DDF30
    int Finish();                                   // 0x005DD0E0
};

class CSWSMessage {
public:
    void WriteBYTE(unsigned char nValue, int nBits);                    // 0x004D62C0 (CNWMessage)
    void WriteCResRef(CResRef cResRef, int nBits);                      // 0x004D6380 (CNWMessage)
    void WriteCExoString(const CExoString& sValue, int nBits);          // 0x004D6960 (CNWMessage)
    void WriteCExoLocStringServer(const CExoLocString& sValue,
                                  unsigned char nGender);               // 0x005785C0
    int SendServerToPlayerJournalAddWorld(CSWSPlayer* p, unsigned long nID, const CExoString& sName,
                                          const CExoString& sText, unsigned long n10,
                                          unsigned long n14);                           // 0x0056A1A0
    int SendServerToPlayerJournalAddWorldStrref(CSWSPlayer* p, unsigned long nStrref, unsigned long n18,
                                                unsigned long n10, unsigned long n14);  // 0x0056A260
    void SendServerToPlayerLoadBar_UpdateStallEvent(unsigned long nStep, unsigned long nTotal);  // 0x0056C910
    int SendServerToPlayerJournalDeleteWorld(CSWSPlayer* p, unsigned long nID);         // 0x0056A300
    int SendServerToPlayerJournalDeleteWorldStrref(CSWSPlayer* p, unsigned long nID);   // 0x0056A370
    int SendServerToPlayerJournalDeleteWorldAll(CSWSPlayer* p);                         // 0x0056A3E0
    int SendServerToPlayerModuleUpdate_Time(CSWSPlayer* p, unsigned char nFlags,
                                            unsigned long nYear, unsigned long nMonth,
                                            unsigned long nDay, unsigned long nHour,
                                            unsigned char nPhase, unsigned long nTime); // 0x0056A5F0
};

class CWorldTimer {
public:
    void FUN_004add50(unsigned long nDay, unsigned long* pYear, unsigned long* pMonth,
                      unsigned long* pDay);         // 0x004ADD50
    void FUN_004add90(unsigned long nTime, unsigned long* pHour, unsigned long* pMinute,
                      unsigned long* pSecond, unsigned long* pMs);  // 0x004ADD90
    void GetWorldTime(unsigned long* pDay, unsigned long* pTime);   // 0x004ADE40
    char m_pad0[0x28];
    unsigned long m_nPauseDay;                      // +0x28
    unsigned long m_nPauseTime;                     // +0x2c
    void FUN_004ae460(unsigned long nDay1, unsigned long nTime1, unsigned long nDay2,
                      unsigned long nTime2, unsigned long* pDays,
                      unsigned long* pTime);        // 0x004AE460
};

class CServerAIMaster {
public:
    int SaveEventQueue(CResGFF* pRes, CResStruct* pStruct); // 0x004B0970
    int AddEventDeltaTime(unsigned long nDays, unsigned long nTime, unsigned long oidCaller,
                          unsigned long oidObject, unsigned long nEventId,
                          void* pEventData);            // 0x004B08D0
    char m_pad0[0x68];
    int m_n68;                                      // +0x68
};

class CFactionManager {
public:
    int SaveFactions(CResGFF* pRes, CResList* pList);       // 0x0052B790
    int SaveReputations(CResGFF* pRes, CResList* pList);    // 0x0052B830
};

class CServerExoAppInternal {
public:
    CFactionManager* GetFactionManager() { return m_pFactionManager; }
    char m_pad0[0x10054];
    CFactionManager* m_pFactionManager;             // +0x10054
};

class CServerExoApp {
public:
    CServerExoAppInternal* GetInternal() { return m_pInternal; }
    CFactionManager* GetFactionManager() { return GetInternal()->m_pFactionManager; }
    void* m_pVtbl;
    CServerExoAppInternal* m_pInternal;             // +0x04

    CSWSCreature* GetCreatureByGameObjectID(unsigned long oid);         // 0x004AE770
    CSWSPlayer* GetClientObjectByObjectId(unsigned long oid);           // 0x004AE910
    unsigned long ClientToServerObjectId(unsigned long oid);            // 0x004AEA30
    CSWSArea* GetAreaByGameObjectID(unsigned long oid);                 // 0x004AE780
    void TogglePauseState(unsigned char nState);                        // 0x004AE960
    unsigned char GetActivePauseState();                                // 0x004AE990
    CExoLinkedList<unsigned long>* GetActiveExclusionList();            // 0x004AE9D0
    CGameObjectArray* GetObjectArray();                                 // 0x004AED70
    CServerAIMaster* GetServerAIMaster();                               // 0x004AED80
    CExoLinkedList<CSWSPlayer>* GetPlayerList();                        // 0x004AED90
    CSWSMessage* GetSWSMessage();                                       // 0x004AEDB0
    CWorldTimer* GetWorldTimer();                                       // 0x004AEDE0
};

class CSWCCreature {
public:
    char m_pad0[4];
    unsigned long m_idSelf;                         // +0x04
};

class CSWParty {
public:
    CSWCCreature* GetCharacter(int n);              // 0x006346C0
};

class CClientExoApp {
public:
    CSWParty* GetSWParty();                         // 0x005ED8B0
    void SetLoadBarProgress(int nProgress, int bRedraw);    // 0x005EDD40
    unsigned char GetTutorialFlag(int n);           // 0x005EDFF0 (load bar start / span)
    void SetModuleResourcesLoadedFlag(unsigned char n);     // 0x005EE0A0
    void ResetModuleResourcesLoaded();              // 0x005EE0C0
};

void RunFrame(float fDelta, int bArg1, int bArg2);  // 0x00401C10
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long dwMilliseconds);

struct CAppManager14 {
    char m_pad0[8];
    unsigned long m_n8;                             // +0x08
    unsigned long m_nC;                             // +0x0c
};

class CAppManager {
public:
    void* m_p0;
    CClientExoApp* m_pClientExoApp;                 // +0x04
    CServerExoApp* m_pServerExoApp;                 // +0x08
    char m_pad0c[0x14 - 0x0c];
    CAppManager14* m_p14;                           // +0x14
};
extern CAppManager* g_pAppManager;                  // 0x007A39FC
inline CServerExoApp* GetServerExoApp() { return g_pAppManager->m_pServerExoApp; }
inline CClientExoApp* GetClientExoApp() { return g_pAppManager->m_pClientExoApp; }

class CExoAliasList {
public:
    CExoString ResolveFileName(const CExoString& sName, unsigned short nResType); // 0x005E68A0
};

class CExoBase {
public:
    char m_pad0[0xc];
    CExoAliasList* m_pcExoAliasList;                // +0x0c
};
extern CExoBase* g_pExoBase;                        // 0x007A39E0

extern "C" __declspec(dllimport) int __stdcall DeleteFileA(const char* lpFileName);

class CScriptEvent {
public:
    CScriptEvent();                                 // 0x004D7540
    unsigned short m_nType;                         // +0x00
    char m_pad2[0x34 - 2];
};

extern unsigned long EVENT_SIGNAL_EVENT;            // 0x007441DC
const char* SetFramesPerSecond(const char* szFPS);  // 0x005EE4A0
extern int g_nModuleFrameLock;                      // 0x00832904 (placeholder name)
extern int g_bConsoleOverlay;                       // 0x0078E360
extern float g_fFrameLock;                          // 0x007A3C64 (placeholder name)

class CVirtualMachine {
public:
    int RunScript(CExoString* psScript, unsigned long oid, int bOid);  // 0x005D0FC0
};
extern CVirtualMachine* g_pVirtualMachine;          // 0x007A3A00

struct CSWSModuleExpansion {
    CExoLocString m_sName;                          // +0x00
    int m_nID;                                      // +0x08
};

struct CSWSModuleCutScene {
    CResRef m_cName;                                // +0x00
    unsigned long m_nID;                            // +0x10
};

struct CSWSModuleID {
    char m_aID[0x20];                               // +0x00
    int m_nCreatorID;                               // +0x20
    unsigned long m_nVersion;                       // +0x24
};

struct CSWSModuleEntryInfo {
    CResRef m_cArea;                                // +0x00
    float m_fX;                                     // +0x10
    float m_fY;                                     // +0x14
    float m_fZ;                                     // +0x18
    float m_fDirX;                                  // +0x1c
    float m_fDirY;                                  // +0x20
};

class CSWSScriptVarTable {
public:
    void SaveVarTable(CResGFF* pRes, CResStruct* pStruct);  // 0x0059ADB0
    char m_pad0[0x10];
};

class CSWVarTable {
public:
    void SaveVarTable(CResGFF* pRes, CResStruct* pStruct);  // 0x0059B250
    char m_pad0[0x14];
};

struct CTlkTableToken {
    unsigned long m_nNumber;                        // +0x00
    CExoString m_sValue;                            // +0x04
};

class CTlkTable {
public:
    char m_pad0[0x48];
    CTlkTableToken* m_pTokens;                      // +0x48
    unsigned long m_nTokens;                        // +0x4c
};
extern CTlkTable* g_pTlkTable;                      // 0x007A3A08
extern const int MIN_CUSTOM_TOKEN;                  // 0x0073ECCC
extern unsigned __int64 g_nNextEffectID;            // 0x007A1B40

// Player list entry walked by GetPrimaryPlayerIndex.
struct CSWSModulePlayerEntry {
    CExoString m_sPlayerName;                       // +0x00
    CExoLocString m_sFirstName;                     // +0x08
    CExoLocString m_sLastName;                      // +0x10
    int m_bPrimary;                                 // +0x18
};

class CSWSModule {
public:
    virtual ~CSWSModule();                          // 0x004C68A0

    CSWSArea* GetArea();                            // 0x004C30F0
    int GetIsSaveGame() { return m_bIsSaveGame; }
    void ClearAreaCache()
    {
        m_pCachedArea = NULL;
        m_oidCachedArea = 0x7f000000;
    }                                               // 0x004C3150
    void GetTime(unsigned long* pYear, unsigned long* pMonth, unsigned long* pDay,
                 unsigned long* pHour, unsigned char* pPhase,
                 unsigned long* pTime);             // 0x004C4100 (FUN_004c4100)
    int GetPrimaryPlayerIndex();                    // 0x004C5D90
    int FindTagNode(const char* szTag);             // 0x004C5E60 (FUN_004c5e60)
    void SaveLimboCreatures(CResGFF* pRes, CResStruct* pStruct);  // 0x004C5BB0
    unsigned long GetObjectByTagAndType(const CExoString& sTag, int nType,
                                        int nNth);  // 0x004C60B0 (placeholder name)
    void SetLowerCaseName(const CExoString& sName); // 0x004C3060 (placeholder name)
    void WriteModuleInfo(void* pUnused);            // 0x004C3170 (placeholder name)
    CExoString GetModuleShortName();                // 0x004C4B80 (placeholder name)
    int RemoveObjectFromLookupTable(const CExoString& sTag, unsigned long oid);  // 0x004C5EF0 (placeholder name)
    unsigned long GetObjectByTag(const CExoString& sTag, int nNth);   // 0x004C5FE0 (placeholder name)
    int AddObjectToLookupTable(const CExoString& sTag, unsigned long oid);  // 0x004C7DE0 (placeholder name)
    int LoadTemporaryArea();                        // 0x004C3B10 (placeholder name)
    void AddWorldJournalEntry(unsigned long nID, const CExoString& sName, const CExoString& sText,
                              unsigned long n10, unsigned long n14);    // 0x004C7ED0 (placeholder name)
    void AddWorldJournalEntryStrref(unsigned long nStrref, unsigned long n18, unsigned long n10,
                                    unsigned long n14);                 // 0x004C8030 (placeholder name)
    int GetNextWorldJournalEntryID();               // 0x004C61D0 (FUN_004c61d0)
    void UpdateTime(unsigned long nDay, unsigned long nTime, unsigned long nDelta); // 0x004C6210
    void RemoveObjectFromLimbo(unsigned long oid);  // 0x004C63F0 (FUN_004c63f0)
    int IsObjectInLimbo(unsigned long oid);         // 0x004C6450 (FUN_004c6450)
    void TimeStopSanityCheck();                     // 0x004C6480
    int AIUpdate();                                 // 0x004C6C90
    void DoUpdate();                                // 0x004C6DC0
    unsigned long GetWaypoint(const CExoString& sTag);  // 0x004C6E00
    void DeleteWorldJournalEntry(unsigned long nID);        // 0x004C8170
    void DeleteWorldJournalEntryStrref(unsigned long nStrref);  // 0x004C8230
    void DeleteWorldJournalAllEntries();            // 0x004C82F0
    void AddObjectToLimbo(unsigned long oid);       // 0x004C83B0
    int GetPlayerIndexInPlayerList(CSWSPlayer* pPlayer);                     // 0x004C5C70
    int SaveStatic(CERFFile* pERF, const CExoString& sType, unsigned short nResType,
                   int bFlag);                                              // 0x004C5980
    int SavePlayers(CResGFF* pRes, CResStruct* pStruct, const CExoString& sPrefix,
                    CExoArrayList<unsigned long>& aPlayers);                // 0x004C7870
    int SaveModuleIFOFinish(CResGFF* pRes, CResStruct* pStruct, CERFFile* pERF,
                            CExoString& sPrefix, CExoArrayList<unsigned long>& aPlayers); // 0x004C8B90
    int SaveModuleFinish(CExoString& sPrefix, CExoString& sPath);           // 0x004CA680
    int SaveModuleFAC();                                                    // 0x004C3960
    void PackPlayerCharacterListIntoMessage(CSWSPlayer* pPlayer,
                                            CExoArrayList<NWPlayerCharacterList_st*>& aList); // 0x004C8E90
    int LoadLimboCreatures(CResGFF* pRes, CResStruct* pStruct, int bLoadFromSave); // 0x004C8C70
    void SavePrimaryPlayerInfo(CSWSCreature* pCreature, const CExoString& sDirectory); // 0x004C3C70
    void RemoveModuleResources(const CExoString& sModule);                  // 0x004C44D0
    void AddModuleResources(const CExoString& sModule);                     // 0x004C4150
    int LoadModuleFinish();                                                 // 0x004C5880
    int LoadModuleInProgress(int nAreaIndex, int nArg);                     // 0x004C5720
    int SaveModuleIFOStart(CResGFF* pRes, CResStruct* pStruct);             // 0x004C7050
    void SaveModuleStart(CExoString& sPrefix, CExoString& sPath);           // 0x004C8960
    void SetIntraAreaGoal(CPathfindInformation* pPath);                     // 0x004C3EA0
    int PlotPathInArea(CPathfindInformation* pPath, int nArg);             // 0x004C3260
    int GenerateInterAreaDFSSuccessors(int nDepth, CPathfindInformation* pPath,
                                       unsigned long** ppList);             // 0x004C54A0
    int InterAreaDFS(int nDepth, int nMaxDepth, CPathfindInformation* pPath); // 0x004C5510
    int ComputeInterAreaPath(CPathfindInformation* pPath);                  // 0x004C6BD0
    int PlotInterAreaPath(CPathfindInformation* pPath, int nArg);          // 0x004C6E20
    int PlotPath(CPathfindInformation* pPath, int nArg);                   // 0x004C6F70

    int m_bAutoRequest;                             // +0x04 (CResHelper<CResIFO, 2014>)
    CResIFO* m_pRes;                                // +0x08
    char m_pad0c[0x20 - 0x0c];
    unsigned long m_idSelf;                         // +0x20
    char m_pad24[4];
    CExoLinkedList<CSWSModuleExpansion> m_lstExpansions;    // +0x28
    CExoLinkedList<CSWSModuleCutScene> m_lstCutScenes;      // +0x2c
    CResRef m_cAreaResRef;                          // +0x30 (placeholder name)

    unsigned long m_oidArea;                        // +0x40
    int m_bAIUpdateToggle;                          // +0x44
    CExoLinkedList<CSWSModulePlayerEntry> m_lPlayers;   // +0x48
    CExoLocString m_cDescription;                   // +0x4c
    CSWSModuleID* m_pModuleID;                      // +0x54
    CSWSModuleEntryInfo* m_pEntryInfo;              // +0x58
    CExoString m_sModuleName;                       // +0x5c (placeholder name)
    CExoLocString m_cName;                          // +0x64 (placeholder name)
    CResRef m_cResRef6c;                            // +0x6c (placeholder name)
    CResRef m_cResRef7c;                            // +0x7c (placeholder name)
    CSWSScriptVarTable m_cScriptVarTable;           // +0x8c
    CSWVarTable m_cVarTable;                        // +0x9c
    CExoString m_sScripts[15];                      // +0xb0 (heartbeat first)
    unsigned long m_nLastHeartbeatDay;              // +0x128
    unsigned long m_nLastHeartbeatTime;             // +0x12c
    CExoArrayList<CSWSTagNode> m_aTagNodes;         // +0x130
    char m_pad13c[0x1a0 - 0x13c];
    unsigned char m_n1a0;                           // +0x1a0 (placeholder name)
    unsigned char m_nDawnHour;                      // +0x1a1
    unsigned char m_nDuskHour;                      // +0x1a2
    char m_pad1a3[3];
    unsigned char m_nXPScale;                       // +0x1a6
    char m_pad1a7;
    unsigned long m_nYear;                          // +0x1a8
    unsigned long m_nMonth;                         // +0x1ac
    unsigned long m_nDay;                           // +0x1b0
    unsigned long m_nHour;                          // +0x1b4
    unsigned long m_nTransitionTime;                // +0x1b8
    unsigned char m_nPhase;                         // +0x1bc
    char m_pad1bd[3];
    unsigned long m_nLastUpdateDay;                 // +0x1c0
    unsigned long m_nLastUpdateTime;                // +0x1c4
    int m_bIsSaveGame;                              // +0x1c8 (placeholder name)
    CExoArrayList<CWorldJournalEntry> m_aWorldJournal;  // +0x1cc
    int m_bLoaded;                                  // +0x1d8 (placeholder name)
    CExoArrayList<unsigned long> m_aLimbo;          // +0x1dc
    union {
        void* m_pTempAreaData;                      // +0x1e8 (placeholder name)
        CERFFile* m_pSaveERF;                       // +0x1e8 while saving
    };
    CResStruct* m_pSaveIFOStruct;                   // +0x1ec
    CResGFF* m_pSaveIFO;                            // +0x1f0
    union {
        unsigned long m_nTempAreaSize;              // +0x1f4 (placeholder name)
        CExoArrayList<unsigned long>* m_pSavePlayerIDs; // +0x1f4 while saving
    };
    CExoString m_sLowerCaseName;                    // +0x1f8 (placeholder name)
    int m_bIsNWM;                                   // +0x200 (placeholder name)
    CExoString m_sNWMResName;                       // +0x204
    unsigned long m_nSaveEntries;                   // +0x20c (placeholder name)
    char m_pad210[0x21c - 0x210];
    unsigned long m_oidCachedArea;                  // +0x21c
    CSWSArea* m_pCachedArea;                        // +0x220
};

#endif
