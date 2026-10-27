// Worker K matching headers: client creature appearance
// (CSWCCreatureAppearance, nwccreatureappearance.o, 0x00696430-0x00698DFF).
// Size 0x6C; allocated by CSWCCreature::CSWCCreature. The first 0x3C bytes
// are the CCAppearanceInfo data block that the Create*/Request* builders
// take as their argument.
#ifndef K_SW_C_CREATURE_APPEARANCE_H
#define K_SW_C_CREATURE_APPEARANCE_H

#include <string.h>

struct Vector {
    float x, y, z;
    Vector() { x = 0.0f; y = 0.0f; z = 0.0f; }
};

struct Quaternion {
    float w, x, y, z;
    Quaternion() { z = 0.0f; y = 0.0f; x = 0.0f; w = 1.0f; }
};

class CExoString {
public:
    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04

    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    CExoString(int n);                              // 0x005E5BC0
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    CExoString& operator=(const char* s);           // 0x005E5140
    CExoString operator+(const CExoString& s) const;  // 0x005E5D10
    char operator[](int n) const;                   // 0x005E5600
    int Find(const CExoString& s, int nStart) const;  // 0x005E5EB0
    int Find(char c, int nStart) const;             // 0x005E5F20
    char* CStr();                                   // 0x005E5670
    int GetLength() const;                          // 0x005E5790
    int IsEmpty() const;                            // 0x005E58A0
    void Format(char* szFormat, ...);               // 0x005E5680
};

class CResRef {
public:
    char m_resRef[16];
    CResRef();                                      // 0x00405ED0
    CResRef(const CExoString& s);                   // 0x00406D60
    CResRef(const char* s);                         // 0x00406D80
    CResRef operator=(const CResRef& r);            // 0x00406120
    CResRef operator=(const CExoString& s);         // 0x004061F0
    CResRef operator=(const char* s);               // 0x00406290
    int operator!=(const char* s) const;            // 0x00406060
    char* GetResRefStr() const;                     // 0x00405FE0
};

class CExoResMan {
public:
    int Exists(const CResRef& r, unsigned short nType, unsigned long* pSize);  // 0x00408BC0
};

extern CExoResMan* g_pExoResMan;                    // 0x007A39E8

class C2DA {
public:
    int GetCExoStringEntry(int nRow, int nColumn, CExoString* pValue);  // 0x00413190
    int GetCExoStringEntry(int nRow, const CExoString& sColumn, CExoString* pValue);  // 0x00413270
    int GetFLOATEntry(int nRow, int nColumn, float* pValue);  // 0x00413350
    int GetINTEntry(int nRow, int nColumn, int* pValue);  // 0x00413510
};

class C2DArrays {
public:
    char pad_0x0[0xc];
    C2DA* m_pAppearanceTable;                       // 0x0c
    C2DA* m_pGenderTable;                           // 0x10
    char pad_0x14[0x44 - 0x14];
    C2DA* m_pHeadTable;                             // 0x44
    C2DA* GetAppearanceTable() { return m_pAppearanceTable; }
    C2DA* GetGenderTable() { return m_pGenderTable; }
    C2DA* GetHeadTable() { return m_pHeadTable; }
};

class CSWRules {
public:
    char pad_0x0[0xb8];
    C2DArrays* m_p2DArrays;                         // 0xb8
    C2DArrays* Get2DArrays() { return m_p2DArrays; }
};

extern CSWRules* g_pRules;                          // 0x007A3A28

// Cached appearance.2da column indices (placeholder names).
extern int g_nAppearanceColModelType;               // 0x007A2110
extern int g_nAppearanceColCameraOffset;            // 0x007A20C8
extern int g_nAppearanceColRunRate;                 // 0x007A2120
extern int g_nAppearanceColHitRadius;               // 0x007A2118
extern int g_nAppearanceColHeight;                  // 0x007A2104
extern int g_nAppearanceColUnknown20D8;             // 0x007A20D8
extern int g_nAppearanceColUnknown20DC;             // 0x007A20DC
extern int g_nAppearanceColUnknown20E0;             // 0x007A20E0
extern int g_nAppearanceColUnknown20E4;             // 0x007A20E4
extern int g_nAppearanceColUnknown20F0;             // 0x007A20F0
extern int g_nAppearanceColUnknown20C4;             // 0x007A20C4
extern int g_nAppearanceColUnknown2114;             // 0x007A2114
extern int g_nAppearanceColEnvMap;                  // 0x007A20E8
extern int g_nAppearanceColRace;                    // 0x007A211C
extern int g_nGenderColGender;                      // 0x007A2148

extern const unsigned long g_oidInvalid;            // 0x00746644 (OBJECT_INVALID)

class CAurObject;

// Result of CAurObject::FindPart (placeholder names).
class CAurPart {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14();
    virtual CAurObject* GetObject();                // 0x18 (placeholder name)
};

class CAurObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14();
    virtual int PlayAnimation(const char* szAnim, float fSpeed, int a, int b);  // 0x18
    virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void SetUnknown40(int n);               // 0x40 (placeholder name)
    virtual void v44(); virtual void v48();
    virtual void SetVisible(int bVisible);          // 0x4c
    virtual void AttachToObject(CAurObject* pTo, const char* szNode, int n);  // 0x50
    virtual void v54();
    virtual void SetUnknown58(int n);               // 0x58 (placeholder name)
    virtual Vector SetPosition(Vector v);           // 0x5c
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94();
    virtual void GetNodePosition(const char* szNode, Vector* pv, Quaternion* pq);  // 0x98
    virtual void v9C();
    virtual int GetNodeTransform(const char* szNode, Vector* pv, Quaternion* pq);  // 0xa0
    virtual void vA4(); virtual void vA8(); virtual void vAC();
    virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC();
    virtual void vC0(); virtual void vC4(); virtual void vC8();
    virtual void SetAlpha(float fAlpha, int b);     // 0xcc
    virtual void vD0(); virtual void vD4(); virtual void vD8();
    virtual void BeginExternalAnimation();          // 0xdc (placeholder name)
    virtual void EndExternalAnimation();            // 0xe0 (placeholder name)
    virtual void vE4(); virtual void vE8(); virtual void vEC();
    virtual void vF0(); virtual void vF4(); virtual void vF8(); virtual void vFC();
    virtual void v100(); virtual void v104(); virtual void v108();
    virtual CAurPart* FindPart(const char* szNode); // 0x10c (placeholder name)

    void AnimateExternal(float fTime);              // 0x0043E150
};

class CSWCObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74();
    virtual int LoadModel(const CResRef& cModel, unsigned char nPart, unsigned char nAnimType);  // 0x78
    virtual void UnloadModel(unsigned char nPart);  // 0x7c
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90();
    virtual void AttachToScene(void* pScene);       // 0x94
    virtual CAurObject* GetModel(unsigned char nPart, int b);  // 0x98
    virtual void v9C();
    virtual void vA0(); virtual void vA4(); virtual void vA8();
    virtual int SetEnvMapPart(unsigned char nPart, int bEnable, int b, const CResRef& cEnvMap);  // 0xac
    virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC();
    virtual void vC0(); virtual void vC4(); virtual void vC8(); virtual void vCC();
    virtual void vD0(); virtual void vD4(); virtual void vD8(); virtual void vDC();
    virtual void vE0(); virtual void vE4(); virtual void vE8(); virtual void vEC();
    virtual void vF0(); virtual void vF4(); virtual void vF8(); virtual void vFC();
    virtual void v100(); virtual void v104(); virtual void v108(); virtual void v10C();
    virtual void v110(); virtual void v114();
    virtual void Unknown118();                      // 0x118
    virtual int Unknown11C();                       // 0x11c
    virtual void v120(); virtual void v124(); virtual void v128(); virtual void v12C();
    virtual void v130(); virtual void v134(); virtual void v138(); virtual void v13C();
    virtual void v140(); virtual void v144(); virtual void v148(); virtual void v14C();
    virtual void v150(); virtual void v154();
    virtual int RequestModel(const CResRef& cModel, const CResRef& cTexture,
                             unsigned char nAnimType);  // 0x158 (CSWCCreature 0x0060C1E0)

    char pad_0x4[0x20 - 0x4];
    void* m_pArea;                                  // 0x20
    Vector m_vPosition;                             // 0x24

    int ReplaceTexture(unsigned char nPart, const CResRef& cOld, const CResRef& cNew, int b);  // 0x0063C0A0
    int RestoreTexturePart(unsigned char nPart);    // 0x0063CE40
    void ReplaceTextures(const CResRef& cOld, const CResRef& cNew, int b);  // 0x0063C0C0
};

class CSWSCreature {
public:
    float GetMovementRateFactor();                  // 0x004EC370
};

class CSWCAnimBase {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void SetCheckItems(int b);              // 0x90
    virtual void v94(); virtual void v98(); virtual void v9C();
    virtual void vA0(); virtual void vA4(); virtual void vA8(); virtual void vAC();
    virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC();
    virtual void vC0(); virtual void vC4(); virtual void vC8(); virtual void vCC();
    virtual void vD0(); virtual void vD4(); virtual void vD8(); virtual void vDC();
    virtual void vE0(); virtual void vE4(); virtual void vE8();
    virtual void SetHeadItem(unsigned long oidItem);  // 0xec
};

class CSWCCreature : public CSWCObject {
public:
    CSWSCreature* AsServerCreature();               // 0x0060FB20
    CSWCAnimBase* GetAnimBase();                    // 0x0063C070
    void SetLightingFlag(unsigned char nFlag);      // 0x0060C8C0
    void UpdateWeaponClass();                       // 0x00613710
    void UnloadAttachedModel();                     // 0x0060BAD0 (descriptive name; deletes +0x170)
    void ResetAppearanceState();                    // 0x004015A0 (descriptive name, folded empty)
    unsigned long GetEquippedItemID(unsigned long nSlot);  // 0x0060CA80
};

class CGameObject {
public:
    char pad_0x0[0x10];
    CSWCObject m_cClientObject;                     // 0x10 (placeholder)
};

class CClientExoApp {
public:
    CGameObject* GetItemByGameObjectID(unsigned long id);  // 0x005ED5A0
    CSWCCreature* GetPlayerCreature();              // 0x005ED540
    int IsPlayerCreature(CSWCCreature* pCreature);  // 0x005EDA30
    int GetPausedByCombat();                        // 0x005EDC10
};

class CAppManager {
public:
    char pad_0x0[0x4];
    CClientExoApp* m_pClientExoApp;                 // 0x04
    void* m_pServerExoApp;                          // 0x08
    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

void SwitchWeaponEvent(unsigned long nEvent, const char* szEvent, CSWCCreature* pCreature);  // 0x00610F40

// Appearance request / data block (size 0x3C).
struct CCAppearanceInfo {
    unsigned char m_nBodyVariation;                 // 0x00
    unsigned char m_nTextureVariation;              // 0x01
    unsigned short m_anUnknown02[6];                // 0x02
    char pad_0x0e[2];
    unsigned long m_oidHeadItem;                    // 0x10 (placeholder name)
    unsigned long m_nUnknown14;                     // 0x14 (placeholder name)
    unsigned short m_nAppearanceType;               // 0x18
    unsigned char m_nHead;                          // 0x1a
    unsigned char m_nGender;                        // 0x1b
    unsigned char m_nHeadModel;                     // 0x1c (placeholder name)
    unsigned char m_nUnknown1D;                     // 0x1d
    unsigned char m_nUnknown1E;                     // 0x1e
    unsigned char m_nUnknown1F;                     // 0x1f
    unsigned char m_nUnknown20;                     // 0x20
    unsigned char m_nTail;                          // 0x21
    unsigned char m_nWing;                          // 0x22
    char pad_0x23;
    float m_fUnknown24;                             // 0x24
    float m_fHeadHeight;                            // 0x28 (placeholder name)
    short m_nAlignment;                             // 0x2c
    char pad_0x2e[2];
    unsigned long m_nUnknown30;                     // 0x30
    unsigned char m_nUnknown34;                     // 0x34
    char pad_0x35[3];
    int m_nUnknown38;                               // 0x38
};

class CSWCCreatureAppearance : public CCAppearanceInfo {
public:
    CSWCCreatureAppearance(CSWCCreature* pCreature);
    int CreateHead(unsigned char nAnimType, CCAppearanceInfo* pInfo);
    char GetGenderChar();
    unsigned char GetAnimType(const CExoString& sModelType);
    int RequestSimple(CCAppearanceInfo* pInfo);
    int RequestHeadAppearance(CCAppearanceInfo* pInfo);
    float GetHeadNodeHeight();
    CAurObject* GetCameraFollowPart();
    int GetFollowingBody();
    float GetHeadHeightOffset();
    float GetCameraOffset();
    float GetRunRate();
    void GetBTypeModelAndTextureResRef(CCAppearanceInfo* pInfo, CResRef* pModel, CResRef* pTexture);
    void ForceAnimate();
    int CreateHeadAppearance(CCAppearanceInfo* pInfo);
    int CreateTailAppearance(CCAppearanceInfo* pInfo);
    int CreateWingAppearance(CCAppearanceInfo* pInfo);
    int RequestBType(CCAppearanceInfo* pInfo);
    int RequestBodyAppearance(CCAppearanceInfo* pInfo);
    int CreateBTypeBody(unsigned char nAnimType, CCAppearanceInfo* pInfo);
    int CreateSimple(unsigned char nAnimType, CCAppearanceInfo* pInfo);
    int CreateBodyAppearance(CCAppearanceInfo* pInfo);
    void SetAppearanceAlignment(short nAlignment);

    CSWCCreature* m_pCreature;                      // 0x3c
    unsigned long m_nUnknown40;                     // 0x40
    unsigned long m_nUnknown44;                     // 0x44
    unsigned long m_nUnknown48;                     // 0x48
    unsigned long m_nUnknown4C;                     // 0x4c
    unsigned long m_nUnknown50;                     // 0x50
    unsigned long m_nUnknown54;                     // 0x54
    float m_fUnknown58;                             // 0x58
    float m_fUnknown5C;                             // 0x5c
    float m_fUnknown60;                             // 0x60
    float m_fUnknown64;                             // 0x64
    int m_bForceAnimate;                            // 0x68 (placeholder name)
};

#endif
