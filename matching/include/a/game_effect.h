// CGameEffect (0x00503590-0x00504830) and the action-target record
// saved next to it (0x005034D0/0x005034F0). Self-contained declarations;
// only the members these functions use are placed.
#ifndef A_GAME_EFFECT_H
#define A_GAME_EFFECT_H

#include <stddef.h>

class CSWSObject;

class CExoString {
public:
    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    CExoString(const CExoString& s);                // 0x005E5B00
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    CExoString& operator=(const char* s);           // 0x005E5140

    char* m_sString;
    unsigned long m_nBufferLength;
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
    unsigned short ReadFieldWORD(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                 unsigned short nDefault);              // 0x00411B40
    unsigned long ReadFieldDWORD(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                 unsigned long nDefault);               // 0x00411C20
    int ReadFieldINT(CResStruct* pStruct, char* szLabel, int& bSuccess,
                     int nDefault);                                     // 0x00411C90
    float ReadFieldFLOAT(CResStruct* pStruct, char* szLabel, int& bSuccess,
                         float fDefault);                               // 0x00411D00
    unsigned __int64 ReadFieldDWORD64(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                      unsigned __int64 nDefault);       // 0x00411D70
    CExoString ReadFieldCExoString(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                   const CExoString& sDefault);         // 0x00411EC0

    int AddList(CResList* pList, CResStruct* pStruct, char* szLabel);               // 0x00412450
    int AddListElement(CResStruct* pOut, CResList* pList, unsigned long nType);     // 0x004124E0
    int WriteFieldBYTE(CResStruct* pStruct, unsigned char nValue, char* szLabel);   // 0x00412620
    int WriteFieldWORD(CResStruct* pStruct, unsigned short nValue, char* szLabel);  // 0x004126C0
    int WriteFieldDWORD(CResStruct* pStruct, unsigned long nValue, char* szLabel);  // 0x00412760
    int WriteFieldINT(CResStruct* pStruct, int nValue, char* szLabel);              // 0x004127B0
    int WriteFieldDWORD64(CResStruct* pStruct, unsigned __int64 nValue, char* szLabel); // 0x00412800
    int WriteFieldFLOAT(CResStruct* pStruct, float fValue, char* szLabel);          // 0x00412870
    int WriteFieldCExoString(CResStruct* pStruct, const CExoString& sValue, char* szLabel); // 0x00412970
};

class CGameObject {
public:
    virtual ~CGameObject() {}                       // 0x00
    virtual void v04(); virtual void v08(); virtual void v0c();
    virtual CSWSObject* AsSWSObject();              // 0x10
    virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual class CSWSAreaOfEffectObject* AsSWSAreaOfEffectObject();  // 0x50 (placeholder name)
};

// vtable 0x00745F18 (only the slot used here)
class CSWSObject : public CGameObject {
public:
    virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7c();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8c();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9c();
    virtual void va0(); virtual void va4(); virtual void va8(); virtual void vac();
    virtual void vb0(); virtual void vb4();
    virtual unsigned long GetLastSpellId();         // 0xb8 (placeholder name)
};

class CServerExoApp {
public:
    CGameObject* GetGameObject(unsigned long oid);  // 0x004AE750
};

class CAppManager {
public:
    char pad_0x0[0x8];
    CServerExoApp* m_pServerExoApp;                 // 0x8
};
extern CAppManager* g_pAppManager;                  // 0x007A39FC
inline CServerExoApp* GetServerExoApp() { return g_pAppManager->m_pServerExoApp; }

class CSWSAreaOfEffectObject {
public:
    char pad_0x0[0x248];
    unsigned long m_oidCreator;                     // 0x248
};

extern const int g_nEffectTrueType746708;          // 0x00746708
extern const int g_nEffectTrueType746714;          // 0x00746714

extern unsigned __int64 g_nNextEffectID;            // 0x007A1B40 (placeholder name)

// Action target record, 0x1c bytes (placeholder name; saved as ActionId,
// GroupId, Target_Area, Target_Object, Target_LocX/Y/Z).
class CSWSActionTarget {
public:
    CSWSActionTarget();                             // 0x005034D0
    int SaveData(CResGFF* pRes, CResStruct* pStruct);   // 0x005034F0

    unsigned long m_nActionId;                      // 0x00
    unsigned short m_nGroupId;                      // 0x04
    unsigned long m_oidArea;                        // 0x08
    float m_fX;                                     // 0x0c
    float m_fY;                                     // 0x10
    float m_fZ;                                     // 0x14
    unsigned long m_oidTarget;                      // 0x18
};

#pragma pack(push, 4)
class CGameEffect {
public:
    CGameEffect(int bCreateNewID);                  // 0x00503E40
    CGameEffect(CGameEffect* pCopy);                // 0x00503F70
    ~CGameEffect();                                 // 0x00503590
    void SetNumIntegers(int nNum);                  // 0x00503630
    int GetInteger(int nIndex);                     // 0x00503690
    void SetInteger(int nIndex, int nValue);        // 0x005036A0
    float GetFloat(int nIndex);                     // 0x005036C0
    void SetFloat(int nIndex, float fValue);        // 0x005036D0
    unsigned long GetObjectID(int nIndex);          // 0x005036E0 (pilot2)
    void SetObjectID(int nIndex, unsigned long oid);    // 0x005036F0
    CExoString GetString(int nIndex);               // 0x00503700
    void SetString(int nIndex, const CExoString& s);    // 0x00503730
    void SetExpiryTime(unsigned long nDay, unsigned long nTime);    // 0x00503750
    void GetExpiryTime(unsigned long* pDay, unsigned long* pTime);  // 0x00503770
    void SaveGameEffect(CResGFF* pRes, CResStruct* pStruct);        // 0x00503790
    void SetCreator(unsigned long oid);             // 0x00503A00
    int GetScriptEffectType();                      // 0x00503A70
    void CopyEffect(CGameEffect* pFrom, int bSkipIntegers);         // 0x00504090
    void UpdateLinked();                            // 0x00504240
    void LoadGameEffect(CResGFF* pRes, CResStruct* pStruct);        // 0x005043A0
    void SetLinked(CGameEffect* pLeft, CGameEffect* pRight);        // 0x00504810

    unsigned short GetDurationType() { return m_nSubType & 7; }
    float GetDuration() { return m_fDuration; }
    void SetType(int nType) { m_nType = (unsigned short)nType; }
    void SetSkipOnLoad(int b) { m_bSkipOnLoad = b; }
    unsigned __int64 GetID() { return m_nID; }
    unsigned short GetType() { return m_nType; }
    unsigned short GetSubType() { return m_nSubType; }
    int GetSkipOnLoad() { return m_bSkipOnLoad; }
    unsigned long GetCreator() { return m_oidCreator; }
    unsigned long GetSpellId() { return m_nSpellId; }
    int GetExpose() { return m_bExpose; }
    int GetNumIntegers() { return m_nNumIntegers; }
    void SetDuration(float fDuration) { m_fDuration = fDuration; }

    unsigned __int64 m_nID;                         // 0x00
    unsigned short m_nType;                         // 0x08
    unsigned short m_nSubType;                      // 0x0a
    float m_fDuration;                              // 0x0c
    unsigned long m_nExpiryCalendarDay;             // 0x10
    unsigned long m_nExpiryTimeOfDay;               // 0x14
    unsigned long m_oidCreator;                     // 0x18
    unsigned long m_nSpellId;                       // 0x1c
    int m_bExpose;                                  // 0x20
    int m_n24;                                      // 0x24 (placeholder name)
    CGameEffect* m_pLinkLeft;                       // 0x28
    CGameEffect* m_pLinkRight;                      // 0x2c
    int m_nNumIntegers;                             // 0x30
    int* m_nParamInteger;                           // 0x34
    float m_nParamFloat[4];                         // 0x38
    CExoString m_sParamString[6];                   // 0x48
    unsigned long m_oidParamObjectID[4];            // 0x78
    int m_bSkipOnLoad;                              // 0x88
};
#pragma pack(pop)

#endif
