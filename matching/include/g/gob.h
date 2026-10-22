// Gob (vtable 0x741268, 101 slots) with the members its small
// virtual accessors touch. Self-contained (own Vector / Quaternion).
#ifndef G_GOB_H
#define G_GOB_H

#include "g/exo_list.h"

class Vector {
public:
    float x, y, z;

    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
};

class Quaternion {
public:
    float w, x, y, z;

    Quaternion() {}
    Quaternion(float fW, float fX, float fY, float fZ) { w = fW; x = fX; y = fY; z = fZ; }
};

class AurColor {
public:
    float r, g, b;
};

class Part;
class Scene;
class Model;
class MdlNode;

// Attachment record behind Gob +0x188.
class GobAttachment {
public:
    virtual ~GobAttachment();
    virtual void v04();
    virtual void Render(int nFlags);                    // +0x08
    virtual void SetLight(int n);                       // +0x0c
    virtual int GetId();                                // +0x10
};

// Part / scene virtuals called from the Gob accessors.
class Part {
public:
    virtual void v00(); virtual void v04(); virtual void v08();
    virtual void GetWorldTransform(Vector* pPos, Quaternion* pOrient);   // +0x0c
    virtual void v10(); virtual void v14(); virtual void v18();
    virtual int Intersect(Vector vStart, Vector vEnd, Vector* pHit, void* pInfo,
                          float fScale);               // +0x1c
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68();
    virtual void Finalize(Part* pRoot);                 // +0x6c
    virtual void v70();
    virtual void GetBoundingBox(Vector* pMin, Vector* pMax, float fScale);  // +0x74
    virtual void GetBoundingSphere(Vector* pCenter, float* pRadius);       // +0x78

    char pad04[0x08 - 0x04];
    Vector m_vPosition;                                 // +0x08
    Quaternion m_qOrientation;                          // +0x14
    char pad24[0x30 - 0x24];
    CExoArrayList<Part*> m_lChildren;                   // +0x30
    Part* m_pParent;                                    // +0x3c
    char pad40[0x48 - 0x40];
    int m_bDirty;                                       // +0x48
};

class Gob;

class Scene {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7c();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8c();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9c();
    virtual void va0(); virtual void va4(); virtual void va8(); virtual void vac();
    virtual void AddGob(Gob* pGob);                     // +0xb0
    virtual void RemoveGob(Gob* pGob);                  // +0xb4
};

class Model {
public:
    char pad00[0x08];
    char m_szName[0x20];                                // +0x08
    MdlNode* m_pRoot;                                   // +0x28
    char pad2c[0x50 - 0x2c];
    unsigned char m_nFlags;                             // +0x50
    char pad51[0x54 - 0x51];
    int m_nRefCount;                                    // +0x54
};

class Gob {
public:
    virtual ~Gob();                                     // +0x000
    virtual void v004();
    virtual char* GetName();                            // +0x008
    virtual char* GetModelName();                       // +0x00c
    virtual void v010(); virtual void v014(); virtual void v018(); virtual void v01c();
    virtual void v020(); virtual void v024(); virtual void v028(); virtual void v02c();
    virtual void v030(); virtual void v034(); virtual void v038(); virtual void v03c();
    virtual int SetCanDownSample(unsigned char b);       // +0x040
    virtual void v044(); virtual void v048();
    virtual int SetSceneAndReport(Scene* pScene);       // +0x04c
    virtual void v050(); virtual void v054();
    virtual void SetAttachment(GobAttachment* p);       // +0x058
    virtual Vector SetPosition(Vector v);               // +0x05c
    virtual Quaternion SetOrientation(Quaternion q);    // +0x060
    virtual Vector GetPosition();                       // +0x064
    virtual Quaternion GetOrientation();                // +0x068
    virtual Vector GetVelocity();                       // +0x06c
    virtual int GetUserData();                          // +0x070
    virtual void SetUserData(int n);                    // +0x074
    virtual int GetUserData2();                         // +0x078
    virtual void SetUserData2(int n);                   // +0x07c
    virtual void CopyUserData(Gob* pFrom);              // +0x080
    virtual Gob* IntersectSegment(Vector vStart, Vector vEnd, Vector* pHit);  // +0x084
    virtual void v088();
    virtual Gob* IntersectLine(Vector vStart, Vector vEnd, Vector* pHit, void* pInfo);             // +0x08c
    virtual void SetNoClip(unsigned char b);            // +0x090
    virtual int LookAtPart(char* szName, Vector v1, Vector v2, int n);  // +0x094
    virtual int GetPartWorldTransform(char* szName, Vector* pPos, Quaternion* pOrient);  // +0x098
    virtual int SetPartTransform(char* szName, Vector v, Quaternion q);  // +0x09c
    virtual int GetPartTransform(char* szName, Vector* pPos, Quaternion* pOrient);  // +0x0a0
    virtual void v0a4();
    virtual void SetAttachmentLight(int n);             // +0x0a8
    virtual int AttachModel(char* szModel, char* szPart);  // +0x0ac
    virtual void v0b0();
    virtual void SetColorShift(AurColor c, int nMode, int bPropagate);  // +0x0b4
    virtual void SetIllumination(AurColor c, int bPropagate);           // +0x0b8
    virtual void ClearFadeFlag();                       // +0x0bc
    virtual void SetFadeFlag();                         // +0x0c0
    virtual void v0c4(); virtual void v0c8();
    virtual unsigned char SetScale(float f, unsigned char bBase);       // +0x0cc
    virtual void SetSelfIllum(const Vector& v);              // +0x0d0
    virtual void ClearSelfIllum();                      // +0x0d4
    virtual void SetFog(unsigned char b, int a, int b2, int c);         // +0x0d8
    virtual void Show();                                // +0x0dc
    virtual void Hide();                                // +0x0e0
    virtual void FogOff();                              // +0x0e4
    virtual void FogOn();                               // +0x0e8
    virtual void GetCachedBoundingBox(Vector* pMin, Vector* pMax);      // +0x0ec
    virtual void GetBoundingBox(Vector* pMin, Vector* pMax);            // +0x0f0
    virtual void GetBoundingSphere(Vector* pCenter, float* pRadius);    // +0x0f4
    virtual void SetHidden();                           // +0x0f8
    virtual void ClearHidden();                         // +0x0fc
    virtual void SetModelFlag4();                       // +0x100
    virtual void v104();
    virtual GobAttachment* GetAttachment(int nId);      // +0x108
    virtual Part* FindPart(char* szName);               // +0x10c
    virtual int EnumParts(int a, int b);                // +0x110
    virtual void SetFlag154(unsigned char b);           // +0x114
    virtual void v118(); virtual void v11c(); virtual void v120(); virtual void v124();
    virtual void v128(); virtual void v12c();
    virtual void SetFlag12();                           // +0x130
    virtual void ClearFlag12();                         // +0x134
    virtual void SetFlag14();                           // +0x138
    virtual void ClearFlag14();                         // +0x13c
    virtual Vector GetHeadPosition();                   // +0x140
    virtual void SetFlag155(unsigned char b);           // +0x144
    virtual unsigned char GetFlag155();                 // +0x148
    virtual void v14c(); virtual void v150(); virtual void v154();
    virtual void SetFloat180(float f);                  // +0x158
    virtual float GetFloat180();                        // +0x15c
    virtual void v160(); virtual void v164(); virtual void v168();
    virtual void DoEvent(char* szEvent, void* pData);  // +0x16c
    virtual unsigned char GetFlag10();                  // +0x170
    virtual unsigned char GetFlag17e();                 // +0x174
    virtual void SetFlag11();                           // +0x178
    virtual void ClearFlag11();                         // +0x17c
    virtual unsigned char GetFlag11();                  // +0x180
    virtual void UpdateBoundingBox();                   // +0x184
    virtual void UpdateBoundingSphere();                // +0x188
    virtual void SetImposter(int a, int b);             // +0x18c
    virtual unsigned char GetFlag12();                  // +0x190

    CExoArrayList<Gob**> m_lWatchers;                   // +0x04
    unsigned char m_b10;                                // +0x10
    unsigned char m_b11;                                // +0x11
    unsigned char m_b12;                                // +0x12
    unsigned char m_b13;                                // +0x13
    unsigned char m_b14;                                // +0x14
    char pad15[0x30 - 0x15];
    int m_n30;                                          // +0x30
    int m_n34;                                          // +0x34
    char m_szName[0x20];                                // +0x38
    Model* m_pModel;                                    // +0x58
    char pad5c[0x68 - 0x5c];
    Scene* m_pScene;                                    // +0x68
    Part* m_pRoot;                                      // +0x6c
    char pad70[0x78 - 0x70];
    Vector m_vPosition;                                 // +0x78
    Quaternion m_qOrientation;                          // +0x84
    Vector m_vVelocity;                                 // +0x94
    char padA0[0xc0 - 0xa0];
    unsigned char m_bCanDownSample;                     // +0xc0
    char padC1;
    short m_nFade;                                      // +0xc2
    short m_bColorShift;                                // +0xc4
    short m_bIllumination;                              // +0xc6
    short m_bSelfIllum;                                 // +0xc8
    char padCA[0xcc - 0xca];
    AurColor m_cColorShift;                             // +0xcc
    int m_nColorShiftMode;                              // +0xd8
    char padDC[0xe0 - 0xdc];
    AurColor m_cIllumination;                           // +0xe0
    CExoArrayList<Model*> m_lAttachedModels;            // +0xec
    CExoArrayList<Part*> m_lAttachedParts;              // +0xf8
    char pad104[0x120 - 0x104];
    Vector m_vBoxMin;                                   // +0x120
    Vector m_vBoxMax;                                   // +0x12c
    float m_fRadius;                                    // +0x138
    Vector m_vCenter;                                   // +0x13c
    Vector m_vSelfIllum;                                // +0x148
    unsigned char m_b154;                               // +0x154
    unsigned char m_b155;                               // +0x155
    unsigned char m_bFog;                               // +0x156
    char pad157;
    int m_nFog158;                                      // +0x158
    int m_nFog15c;                                      // +0x15c
    int m_nFog160;                                      // +0x160
    char pad164[0x167 - 0x164];
    unsigned char m_bDirty;                             // +0x167
    char pad168[0x17c - 0x168];
    unsigned char m_bHidden;                            // +0x17c
    unsigned char m_bNoClip;                            // +0x17d
    unsigned char m_b17e;                               // +0x17e
    char pad17f;
    float m_f180;                                       // +0x180
    char pad184[0x188 - 0x184];
    GobAttachment* m_pAttachment;                       // +0x188
    char pad18c[0x1ac - 0x18c];
    int m_nUserData;                                    // +0x1ac
    int m_nUserData2;                                   // +0x1b0
    float m_fScale;                                     // +0x1b4
    float m_fBaseScale;                                 // +0x1b8
    CExoArrayList<Gob*> m_lChildren;                    // +0x1bc

    void SetScene(Scene* pScene);                       // 0x0043f200
    int EnumParts(Part* pRoot, int a, int b);           // 0x004474d0
};

void __cdecl PartSetFog(Part* pRoot, int bOn);          // 0x00440550
Part* __cdecl FindPart(Part* pRoot, char* szName);      // 0x0043fe00
Model* __cdecl FindModel(char* szName);                 // 0x00464110
Part* __cdecl CreateInstanceTreeR(MdlNode* pNode, Gob* pGob);       // 0x00449200
void __cdecl PartInitMaterial(Gob* pGob, Part* pPart, char* szName); // 0x00442d70
void __cdecl PartAttachUpdate(Gob* pGob, Part* pPart);  // 0x004448c0
int __cdecl SetUpImposters(int a, int b);               // 0x00432160
int __cdecl PartSetCanDownSample(Part* pRoot, unsigned char b);   // 0x004434a0

#endif
