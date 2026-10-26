// Worker L matching headers: the client animation bases of placeables
// (CSWCAnimBasePlaceable, vtable 0x007588B0), doors (CSWCAnimBaseDoor,
// vtable 0x00758A08) and triggers (CSWCAnimBaseTrigger, vtable 0x00758C70),
// 0x006E4E50-0x006E6200. The base class layout follows worker K's
// k/sw_c_anim_base.h; only the slots these classes use are named.
#ifndef L_SW_C_ANIM_BASE_OBJECTS_H
#define L_SW_C_ANIM_BASE_OBJECTS_H

extern "C" int __cdecl sprintf(char* buffer, const char* format, ...);

class CExoString {
public:
    char* m_sString;
    int m_nBufferLength;
    CExoString();                                   // 0x005B3190
    CExoString(const char* sString);                // 0x005E5A90
    CExoString(const CExoString& sString);          // 0x005E5B00
    void Format(const char* szFormat, ...);         // 0x005E5680
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    CExoString& operator=(const char* s);           // 0x005E5140
    CExoString operator+(const CExoString& s) const;    // 0x005E5D10
    int GetLength() const;                          // 0x005E5790
    char* CStr() const;                             // 0x005E5670
};

class CResRef {
public:
    CResRef operator=(const CResRef& r);            // 0x00406120
    void CopyToString(char* pStr) const;            // 0x00405FB0
    char m_resRef[16];
};

struct Vector {
    float x, y, z;
};

struct Quaternion {
    float w, x, y, z;
};

struct CSWGuiExtent;

// Aurora model object; only PlayAnimation (slot 0x18) is used here.
class CAurObject {
public:
    virtual ~CAurObject();
    virtual void v04();
    virtual void v08();
    virtual void v0C();
    virtual void v10();
    virtual void v14();
    virtual int PlayAnimation(const char* szAnim, float fSpeed, int bLoop, int nTransition);  // 0x18
    virtual void v1C(); virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9C();
    virtual void vA0(); virtual void vA4(); virtual void vA8(); virtual void vAC();
    virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC();
    virtual void vC0(); virtual void vC4(); virtual void vC8(); virtual void vCC();
    virtual void vD0(); virtual void vD4(); virtual void vD8(); virtual void vDC();
    virtual void vE0();
    virtual void SetDefaultAnimation();             // 0xe4 (descriptive)
};

CAurObject* NewCAurObject(const char* szModel, const char* szName, int a, int b);  // 0x00449CC0

class C2DA {
public:
    int GetCExoStringEntry(int nRow, int nColumn, CExoString* pValue);  // 0x00413190
};

class C2DArrays {
public:
    char pad_0x0[0x3c];
    C2DA* m_pAnimations;                            // 0x3c
    C2DA* GetAnimations() { return m_pAnimations; }
};

class CSWRules {
public:
    char pad_0x0[0xb8];
    C2DArrays* m_p2DArrays;                         // 0xb8
    C2DArrays* Get2DArrays() { return m_p2DArrays; }
};

extern CSWRules* g_pRules;                          // 0x007A3A28
extern int g_nAnimNameColumn;                       // 0x007A2174

// Common animation interface base (vtable 0x00754AE4, one slot).
class CAnimBase {
public:
    CAnimBase();                                    // 0x00698E00
    virtual ~CAnimBase();                           // 0x00698E20
    int m_n4;                                       // 0x04
    int m_n8;                                       // 0x08
    int m_nC;                                       // 0x0c
    int m_n10;                                      // 0x10
};

// Client animation base (vtable 0x00754F60), 0xF0 bytes.
class CSWCAnimBase : public CAnimBase {
public:
    CSWCAnimBase();                                 // 0x0069DFB0
    virtual ~CSWCAnimBase();                        // 0x00 (0x0069E140)
    virtual void SetVisible(int bVisible);          // 0x04
    virtual CAurObject* GetModel(unsigned char nPart);  // 0x08
    virtual int LoadModel(const CResRef& cResRef, unsigned char nPart);  // 0x0c
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual CExoString GetAnimationName(unsigned short nAnim);  // 0x40
    virtual void v44(); virtual void v48(); virtual void v4C(); virtual void v50();
    virtual int PlayAnimation(unsigned short nAnim, float fSpeed, int a, int nTransition);  // 0x54
    virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74();
    virtual void UnloadModel();                     // 0x78 (0x0069D740)
    virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9C();
    virtual void vA0(); virtual void vA4(); virtual void vA8(); virtual void vAC();
    virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC();
    virtual void vC0(); virtual void vC4(); virtual void vC8(); virtual void vCC();
    virtual void vD0(); virtual void vD4(); virtual void vD8(); virtual void vDC();
    virtual unsigned short ServerToClientAnimation(unsigned short nAnim);  // 0xe0
    virtual void vE4(); virtual void vE8(); virtual void vEC();
    virtual int PlayAnimOnModel(CAurObject* pModel, unsigned short nAnim, float fSpeed, int a,
                                int nTransition);   // 0xf0

    unsigned short m_aTextureIndices[10];           // 0x14
    unsigned char m_nTextureCount;                  // 0x28
    unsigned char m_nTextureStart;                  // 0x29
    char pad_0x2a[2];
    CResRef m_cModelResRef;                         // 0x2c
    int m_bReplaceTextureIndexed;                   // 0x3c
    CResRef m_cEnvMap;                              // 0x40
    int m_bEnvMap;                                  // 0x50
    Vector m_aVectors[5];                           // 0x54
    int m_a90[5];                                   // 0x90
    int m_aA4[5];                                   // 0xa4
    CAurObject* m_pModel;                           // 0xb8
    CAurObject* m_pShadow;                          // 0xbc
    float m_fC0;                                    // 0xc0
    unsigned char m_nAnimType;                      // 0xc4
    unsigned char m_nC5;                            // 0xc5
    unsigned char m_nC6;                            // 0xc6
    unsigned char m_nC7;                            // 0xc7
    unsigned char m_nC8;                            // 0xc8
    char pad_0xc9[3];
    int m_nCC;                                      // 0xcc
    int m_nD0;                                      // 0xd0
    int m_bShadowAttached;                          // 0xd4
    float m_fTiltSpeed;                             // 0xd8
    int m_nDC;                                      // 0xdc
    Quaternion m_qE0;                               // 0xe0
};

class CSWCAnimBasePlaceable : public CSWCAnimBase {
public:
    CSWCAnimBasePlaceable();                        // 0x006E4E50
    virtual ~CSWCAnimBasePlaceable();               // 0x006E4E70
    virtual int LoadModel(const CResRef& cResRef, unsigned char nPart);  // 0x0c (0x006E4EC0)
    virtual CExoString GetAnimationName(unsigned short nAnim);  // 0x40 (0x006E51A0)
    virtual int PlayAnimation(unsigned short nAnim, float fSpeed, int a, int nTransition);  // 0x54 (0x006E4F30)
    virtual unsigned short ServerToClientAnimation(unsigned short nAnim);  // 0xe0 (0x006E53F0)
};

class CSWCAnimBaseDoor : public CSWCAnimBase {
public:
    CSWCAnimBaseDoor();                             // 0x006E56A0
    virtual ~CSWCAnimBaseDoor();                    // 0x006E56C0
    virtual int LoadModel(const CResRef& cResRef, unsigned char nPart);  // 0x0c (0x006E5710)
    virtual CExoString GetAnimationName(unsigned short nAnim);  // 0x40 (0x006E59C0)
    virtual unsigned short ServerToClientAnimation(unsigned short nAnim);  // 0xe0 (0x006E5AC0)
    virtual int PlayAnimOnModel(CAurObject* pModel, unsigned short nAnim, float fSpeed, int a,
                                int nTransition);   // 0xf0 (0x006E57B0)
};

// Visual-effect model animation base (vtable 0x00758B68).
class CSWCAnimBaseVisualEffect : public CSWCAnimBase {
public:
    CSWCAnimBaseVisualEffect();                     // 0x006E5CE0
    virtual ~CSWCAnimBaseVisualEffect();            // 0x006E5D00
    virtual int LoadModel(const CResRef& cResRef, unsigned char nPart);  // 0x0c (0x006E5D50)
    virtual CExoString GetAnimationName(unsigned short nAnim);  // 0x40 (0x006E5DC0)
    virtual int PlayAnimOnModel(CAurObject* pModel, unsigned short nAnim, float fSpeed, int a,
                                int nTransition);   // 0xf0 (0x006E5E60)
};

class CSWCAnimBaseTrigger : public CSWCAnimBase {
public:
    CSWCAnimBaseTrigger();                          // 0x006E5F40
    virtual ~CSWCAnimBaseTrigger();                 // 0x006E5F60
    virtual int LoadModel(const CResRef& cResRef, unsigned char nPart);  // 0x0c (0x006E5FB0)
    virtual CExoString GetAnimationName(unsigned short nAnim);  // 0x40 (0x006E6040)
    virtual unsigned short ServerToClientAnimation(unsigned short nAnim);  // 0xe0 (0x006E6100)
};

#endif
