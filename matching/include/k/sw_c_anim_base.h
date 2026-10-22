// Worker K matching headers: client animation base (CSWCAnimBase, vtable
// 0x00754F60) and CSWCAnimBaseTW (vtable 0x00754E58), 0x0069CD10-0x0069DE70.
#ifndef K_SW_C_ANIM_BASE_H
#define K_SW_C_ANIM_BASE_H

#include <string.h>

struct Vector {
    float x, y, z;
    Vector() { x = 0.0f; y = 0.0f; z = 0.0f; }
};

struct Quaternion {
    float w, x, y, z;
    Quaternion() { z = 0.0f; y = 0.0f; x = 0.0f; w = 1.0f; }
    Quaternion(Vector vAxis, float fAngle);         // 0x004AC960
};

extern Vector g_vZAxis;                             // 0x007A19B0 (0, 0, 1)

extern float g_fGroundTiltScale;                    // 0x007A2680

float GetYaw(const Vector& v);                      // 0x004AA0F0
float GetPitch(const Quaternion& q);                // 0x004A9F40
float GetRoll(const Quaternion& q);                 // 0x004AA020
Quaternion EulerToQuaternion(float fYaw, float fPitch, float fRoll);   // 0x004ACAC0

class CExoString {
public:
    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04

    CExoString();                                   // 0x005B3190
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    CExoString& operator=(const char* s);           // 0x005E5140
    char* CStr() const;                             // 0x005E5670
    void Format(const char* szFormat, ...);         // 0x005E5680
};

class CResRef {
public:
    char m_resRef[16];
    CResRef();                                      // 0x00405ED0
    CResRef(const char* s);                         // 0x00406D80
    CResRef(const CResRef& r);
    CResRef operator=(const CResRef& r);            // 0x00406120
    CResRef operator=(const char* s);               // 0x00406290
    void CopyToString(char* pStr) const;            // 0x00405FB0
    void CopyToString(CExoString& s) const;         // 0x00405F70
    int operator!=(const CResRef& r) const;         // 0x00406DC0
    char* GetResRef() const;                        // 0x00641DB0
};

// Aurora model object (NewCAurObject 0x00449CC0).
class CAurObject {
public:
    virtual ~CAurObject();  // 0x00
    virtual void v04();
    virtual void v08();
    virtual void v0C();
    virtual void v10();
    virtual void v14();
    virtual int PlayAnimation(const char* szAnim, float fSpeed, int a, int b);  // 0x18
    virtual int GetEventTime(const char* szAnim, float* pfA, float* pfB);  // 0x1c
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2C();
    virtual void ReplaceTextureIndexed(char* szOld, char* szNew, int nCount, unsigned short* pIndices, int nStart);  // 0x30
    virtual void ReplaceTexture(char* szOld, char* szNew);  // 0x34
    virtual void v38();
    virtual void RestoreTextures();  // 0x3c
    virtual void v40();
    virtual void v44();
    virtual void SetEnvMap(char* szEnvMap);  // 0x48
    virtual void SetVisible(int bVisible);  // 0x4c
    virtual void AttachToObject(CAurObject* pTo, const char* szNode, int n);  // 0x50
    virtual void v54();
    virtual void v58();
    virtual Vector SetPosition(Vector v);  // 0x5c
    virtual Quaternion SetOrientation(Quaternion q);  // 0x60
    virtual Vector GetPosition(int n);  // 0x64
    virtual Quaternion GetOrientation();  // 0x68
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
    virtual void SetNodePosition(const char* szNode, Vector v, Quaternion q);  // 0x94
    virtual void GetNodePosition(const char* szNode, Vector* pv, Quaternion* pq);  // 0x98
    virtual void v9C();
    virtual int GetNodeTransform(const char* szNode, Vector* pv, Quaternion* pq);  // 0xa0
    virtual void vA4();
    virtual void vA8();
    virtual void vAC();
    virtual void vB0();
    virtual void vB4();
    virtual void vB8();
    virtual void vBC();  // 0xbc
    virtual void vC0();  // 0xc0
    virtual void vC4();
    virtual void vC8();
    virtual void SetAlpha(float fAlpha, int b);  // 0xcc
    virtual void vD0();
    virtual void vD4();
    virtual void vD8();
    virtual void vDC();
    virtual void vE0();
    virtual void vE4();
    virtual void vE8();
    virtual void vEC();
    virtual void vF0();
    virtual void vF4();
    virtual void vF8();  // 0xf8
    virtual void vFC();  // 0xfc
    virtual void v100();
    virtual void v104();
    virtual void v108();
    virtual void v10C();
    virtual void v110();
    virtual void v114();
    virtual void v118();
    virtual void v11C();
    virtual void SetLight(const char* szNode, int a, int b, int c, int d);  // 0x120
    virtual void ClearLight(const char* szNode);  // 0x124
    virtual void v128(int n);  // 0x128
    virtual void v12C(int n);  // 0x12c
    virtual void v130();  // 0x130
    virtual void v134();  // 0x134
    virtual void v138();  // 0x138
    virtual void v13C();  // 0x13c
    virtual void v140();
    virtual void v144(unsigned char b);  // 0x144
    virtual void v148();
    virtual void v14C();
    virtual void v150(char* sz, int n);  // 0x150
    virtual void v154();  // 0x154
};

CAurObject* NewCAurObject(const char* szModel, const char* szName, int a, int b);  // 0x00449CC0

// Screen extent (same layout as worker D's CSWGuiExtent).
struct CSWGuiExtent {
    int left;
    int top;
    int width;
    int height;
};

CSWGuiExtent UnionExtents(const CSWGuiExtent& a, const CSWGuiExtent& b);   // 0x0040A520

// Common animation interface base (vtable 0x00754AE4, one slot).
class CAnimBase {
public:
    CAnimBase();                                    // 0x00698E00
    virtual ~CAnimBase();                           // 0x00698E20
    int m_n4;                                       // 0x04
    int m_n8;                                       // 0x08
    int m_nC;                                       // 0x0c
    int m_n10;                                      // 0x10

    int SetValue(int nIndex, int nValue);           // 0x00698E30
};

class CSWCAnimBase : public CAnimBase {
public:
    CSWCAnimBase();                                 // 0x0069DFB0
    virtual ~CSWCAnimBase();  // 0x00
    virtual void SetVisible(int bVisible);  // 0x04
    virtual CAurObject* GetModel(unsigned char nPart);  // 0x08
    virtual int LoadModel(const CResRef& cResRef, unsigned char nPart);  // 0x0c
    virtual int ReplaceTexture(unsigned char nPart, const CResRef& cOld, const CResRef& cNew, int bStore);  // 0x1c
    virtual int ReplaceTexture(const CResRef& cOld, const CResRef& cNew, int bStore);  // 0x18
    virtual int ReplaceTexture(unsigned char nPart, const CResRef& cOld, const CResRef& cNew, int nCount, unsigned short* pIndices, int bStore, int nStart);  // 0x14
    virtual int ReplaceTexture(const CResRef& cOld, const CResRef& cNew, int nCount, unsigned short* pIndices, int bStore, int nStart);  // 0x10
    virtual int RestoreTexturePart(unsigned char nPart);  // 0x20
    virtual int RestoreTexture();  // 0x24
    virtual int SetSkin(const CResRef& cSkin, int n);  // 0x28
    virtual int ClearSkin();  // 0x2c
    virtual int SetFlag144(int b);  // 0x30
    virtual int HasModel(int n);  // 0x34
    virtual float GetAnimationEventTime(int nAnim);  // 0x38
    virtual float GetAnimationEventTime2(int nAnim);  // 0x3c
    virtual CExoString GetAnimationName(int nAnim);  // 0x40
    virtual int PlayAnimSet(int a, int b);  // 0x44
    virtual int HasAnimation(int nAnim);  // 0x48
    virtual int HasAnimationLength(int nAnim);  // 0x4c
    virtual int HasAnimationLength2(int nAnim);  // 0x50
    virtual int PlayAnim(int nAnim, float fSpeed, int a, int b);  // 0x54
    virtual int PlayAnimPart(unsigned char nPart, int nAnim, float fSpeed, int a, int b);  // 0x58
    virtual int SetTalk(int a, int b, int c, int d);  // 0x5c
    virtual int SetOrientation(const Quaternion& q);  // 0x60
    virtual int SetFacing(const Vector& v);  // 0x64
    virtual int SetPosition(const Vector& v);  // 0x68
    virtual void SetFlag130(int b);  // 0x6c
    virtual void SetFlag138(int b);  // 0x70
    virtual void UnloadModelPart(unsigned char nPart);  // 0x74
    virtual void UnloadModel();  // 0x78
    virtual int SetWieldedItem(unsigned long oid, const CExoString& sNode, unsigned char nHand, float fAlpha);  // 0x7c
    virtual void SetWieldedModels(const CResRef& cRight, const CResRef& cLeft);  // 0x80
    virtual void ShowWieldedItems();  // 0x84
    virtual void DetachHeadItem();  // 0x88
    virtual void AttachHeadItem();  // 0x8c
    virtual void SetCheckItems(int b);  // 0x90
    virtual void v94();  // 0x94
    virtual int SetEnvMapPart(unsigned char nPart, int bEnable, int bStore, const CResRef& cEnvMap);  // 0x98
    virtual int SetEnvMap(int bEnable, int bStore, const CResRef& cEnvMap);  // 0x9c
    virtual int RestoreEnvMap();  // 0xa0
    virtual int CreateShadow(const CExoString& sModel);  // 0xa4
    virtual void DestroyShadow();  // 0xa8
    virtual void AttachShadow(int bForce);  // 0xac
    virtual void DetachShadow();  // 0xb0
    virtual void SetShadowOrientation(const Quaternion& q);  // 0xb4
    virtual void ModelVC0();  // 0xb8
    virtual void ModelVBC();  // 0xbc
    virtual Quaternion GetOrientation();  // 0xc0
    virtual void UpdateSound(int bOn);  // 0xc4
    virtual int SetModelV128Self(int n);  // 0xc8
    virtual int SetModelV12CSelf(int n);  // 0xcc
    virtual void SetGroundTiltAmount(float f);  // 0xd0
    virtual void UpdateTilt();  // 0xd4
    virtual int GetBoundingExtent(CSWGuiExtent* pExtent);  // 0xd8
    virtual void vDC();  // 0xdc
    virtual void vE0();  // 0xe0
    virtual void ModelVF8();  // 0xe4
    virtual void ModelVFC();  // 0xe8
    virtual void SetHeadItem(unsigned long oid);  // 0xec
    virtual int PlayAnimOnModel(CAurObject* pModel, int nAnim, float fSpeed, int a, int b);  // 0xf0
    virtual int PlayAnimOnPart(CAurObject* pModel, unsigned char nPart, int nAnim, float fSpeed, int a, int b);  // 0xf4

    void DestroyModelInline()
    {
        if (m_pModel) {
            SetVisible(0);
            delete m_pModel;
            m_pModel = 0;
        }
    }

    int SetModelV128(CAurObject* pModel, int n);
    void RefreshHilite();                           // 0x0069EC00
    int PlayAnimSetOnModel(CAurObject* pModel, int a, int b);  // 0x0069E420
    int GetBoundingRect(CAurObject* pModel, CSWGuiExtent* pExtent);  // 0x0069EDC0
    int SetModelV12C(CAurObject* pModel, int n);

    unsigned short m_aTextureIndices[10];           // 0x14
    unsigned char m_nTextureCount;                  // 0x28
    unsigned char m_nTextureStart;                  // 0x29
    char pad_0x2a[2];
    CResRef m_cReplaceTexture;                      // 0x2c
    int m_bReplaceTextureIndexed;                   // 0x3c
    CResRef m_cEnvMap;                              // 0x40
    int m_bEnvMap;                                  // 0x50
    Vector m_aVectors[5];                           // 0x54
    int m_a90[5];                                   // 0x90
    int m_aA4[5];                                   // 0xa4
    CAurObject* m_pModel;                           // 0xb8
    CAurObject* m_pShadow;                          // 0xbc
    float m_fC0;                                    // 0xc0
    unsigned char m_nAnimType;                      // 0xc4 (1 head, 0xb tail/wings)
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

class CSWCAnimBaseTW : public CSWCAnimBase {
public:
    CSWCAnimBaseTW();                               // 0x0069CBD0
    virtual ~CSWCAnimBaseTW();
    virtual CAurObject* GetModel(unsigned char nPart);  // 0x08
    virtual int LoadModel(const CResRef& cResRef, unsigned char nPart);  // 0x0c
    virtual int ReplaceTexture(unsigned char nPart, const CResRef& cOld, const CResRef& cNew, int bStore);  // 0x1c
    virtual int ReplaceTexture(const CResRef& cOld, const CResRef& cNew, int bStore);  // 0x18
    virtual int ReplaceTexture(unsigned char nPart, const CResRef& cOld, const CResRef& cNew, int nCount, unsigned short* pIndices, int bStore, int nStart);  // 0x14
    virtual int ReplaceTexture(const CResRef& cOld, const CResRef& cNew, int nCount, unsigned short* pIndices, int bStore, int nStart);  // 0x10
    virtual int RestoreTexturePart(unsigned char nPart);  // 0x20
    virtual int RestoreTexture();                   // 0x24
    virtual void UnloadModelPart(unsigned char nPart);  // 0x74
    virtual void UnloadModel();                     // 0x78
    virtual int SetEnvMapPart(unsigned char nPart, int bEnable, int bStore, const CResRef& cEnvMap);  // 0x98
    virtual int SetEnvMap(int bEnable, int bStore, const CResRef& cEnvMap);  // 0x9c
    virtual int RestoreEnvMap();                    // 0xa0
    virtual int PlayAnimOnModel(CAurObject* pModel, int nAnim, float fSpeed, int a, int b);  // 0xf0
    virtual int PlayAnimOnPart(CAurObject* pModel, unsigned char nPart, int nAnim, float fSpeed, int a, int b);  // 0xf4

    unsigned short m_aTextureIndices2[10];          // 0xf0
    unsigned char m_nTextureCount2;                 // 0x104
    unsigned char m_nTextureStart2;                 // 0x105
    unsigned short m_aTextureIndices3[10];          // 0x106
    unsigned char m_nTextureCount3;                 // 0x11a
    unsigned char m_nTextureStart3;                 // 0x11b
    CResRef m_cReplaceTexture2;                     // 0x11c
    int m_bReplaceTextureIndexed2;                  // 0x12c
    CResRef m_cReplaceTexture3;                     // 0x130
    int m_bReplaceTextureIndexed3;                  // 0x140
    CResRef m_cEnvMap2;                             // 0x144
    int m_bEnvMap2;                                 // 0x154
    CResRef m_cEnvMap3;                             // 0x158
    int m_bEnvMap3;                                 // 0x168
    CAurObject* m_pModel2;                          // 0x16c
    CAurObject* m_pModel3;                          // 0x170
};

#endif
