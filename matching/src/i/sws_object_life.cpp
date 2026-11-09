// CSWSObject constructor (nwsobject.o, 0x004CFCB0). Self-contained
// declarations with every constructed member typed, so that the member
// constructors and the inlined SetPosition / SetOrientation are visible.
#include <stddef.h>

class CSWSObject;
class CSWSCreature;
class CSWSArea;
class CSWSDialog;
class CSWCObject;
class CGameEffect;
class CLoopingVisualEffect;
class CSWSExpression;
class CSWSObjectActionNode;

class CExoString {
public:
    CExoString();                                   // 0x005B3190
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const char* s);           // 0x005E5140
    char* m_sString;
    unsigned long m_nBufferLength;
};

class CExoLocString {
public:
    CExoLocString();                                // 0x005E9E10
    ~CExoLocString();                               // 0x005EA000
    void* m_pInternal;
    unsigned long m_n4;
};

class CResRef {
public:
    CResRef();                                      // 0x00405ED0
    char m_resRef[16];
};

class Vector {
public:
    Vector()
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }
    Vector(float fX, float fY, float fZ)
    {
        x = fX;
        y = fY;
        z = fZ;
    }
    float x, y, z;
};

int operator==(const Vector& a, const Vector& b);  // 0x004AA980

class CGameObject {
public:
    CGameObject(unsigned char nType, unsigned long oid);    // 0x004D7D60
    virtual ~CGameObject();                         // 0x004C3030
    virtual void SetId(unsigned long oid);          // +0x04 (placeholder name)
    virtual void ResetUpdateTimes(unsigned long nDay, unsigned long nTime);    // +0x08 (placeholder name)
    virtual void* As0c();
    virtual CSWSObject* AsSWSObject();              // +0x10
    virtual void* As14();
    virtual void* As18();
    virtual void* As1c();
    virtual void* As20();
    virtual void* As24();
    virtual CSWSArea* AsSWSArea();                  // +0x28
    virtual void* As2c();
    virtual CSWSCreature* AsSWSCreature();          // +0x30
    virtual void* As34();
    virtual void* As38();
    virtual void* As3c();
    virtual void* As40();
    virtual void* As44();
    virtual void* As48();
    virtual void* As4c();
    virtual void* As50();
    virtual void* As54();
    virtual void* As58();
    virtual void* As5c();
    virtual void* As60();
    virtual void* As64();
    virtual void* As68();
    virtual void* As6c();

    unsigned long m_idSelf;                         // +0x04
    unsigned char m_nObjectType;                    // +0x08
};

class CExoLinkedListInternal;

template <class T>
class CExoLinkedList {
public:
    CExoLinkedList();                               // 0x004CC3D0
    ~CExoLinkedList();
    CExoLinkedListInternal* m_pList;
};

template <class T>
class CExoArrayList {
public:
    CExoArrayList()
    {
        num = 0;
        array_size = 0;
        element = NULL;
    }
    ~CExoArrayList();
    T* element;
    int num;
    int array_size;
};

class CSWSScriptVarTable {
public:
    CSWSScriptVarTable()
    {
        m_nC = 0;
    }
    ~CSWSScriptVarTable();                          // 0x0059A980
    CExoArrayList<void*> m_aVars;                   // +0x00
    int m_nC;                                       // +0x0c
};

class CSWVarTable {
public:
    CSWVarTable();                                  // 0x0059AFE0
    char m_pad0[0x14];
};

class CSWSArea {
public:
    void FUN_00506fe0(CSWSObject* pObject);         // 0x00506FE0
};

class CGameObjectArray {
public:
    unsigned char GetGameObject(unsigned long oid, CGameObject** ppObject);    // 0x004D8230
    unsigned char AddInternalObject(unsigned long* pOid, CGameObject* pObject, int bCharacter);  // 0x004D80E0
    unsigned char AddExternalObject(unsigned long oid, CGameObject* pObject);  // 0x004D7E90 (placeholder name)
};

class CWorldTimer {
public:
    void GetWorldTime(unsigned long* pDay, unsigned long* pTime);   // 0x004ADE40
};

class CServerExoApp {
public:
    CGameObjectArray* GetObjectArray();             // 0x004AED70
    CWorldTimer* GetWorldTimer();                   // 0x004AEDE0
};

class CAppManager {
public:
    void* m_p0;
    void* m_pClientExoApp;                          // +0x04
    CServerExoApp* m_pServerExoApp;                 // +0x08
};
extern CAppManager* g_pAppManager;                  // 0x007A39FC
inline CServerExoApp* GetServerExoApp() { return g_pAppManager->m_pServerExoApp; }

extern const unsigned long OBJECT_INVALID;          // 0x00746644

// vtable 0x00745F18
class CSWSObject : public CGameObject {
public:
    CSWSObject(unsigned char nType, unsigned long oid, int bCharacter);     // 0x004CFCB0
    virtual ~CSWSObject();                          // 0x004D0220

    CSWSArea* GetArea()
    {
        CGameObject* pObject = NULL;
        CGameObjectArray* pArray = g_pAppManager->m_pServerExoApp->GetObjectArray();
        pArray->GetGameObject(m_oidArea, &pObject);
        if (pObject) {
            return pObject->AsSWSArea();
        }
        return NULL;
    }                                               // 0x004CB120
    void SetPosition(const Vector& vPosition, int bUpdateArea, int bUnused, int bNoUpdateFlag)
    {
        if (m_vPosition == vPosition) {
            return;
        }
        if (bNoUpdateFlag == 0) {
            m_nUpdateFlags[0] |= 2;
        }
        CSWSArea* pArea = GetArea();
        m_vPosition = vPosition;
        if (bUpdateArea) {
            if (AsSWSCreature()) {
                AsSWSCreature();
            }
            if (pArea) {
                pArea->FUN_00506fe0(this);
            }
        }
    }                                               // 0x004CD1A0
    void SetUpdateFlag(unsigned long nBit)
    {
        unsigned long nIndex = nBit / 32;
        m_nUpdateFlags[nIndex] |= 1 << (nBit - nIndex * 32);
    }                                               // 0x004CC1F0
    void SetOrientation(const Vector& v)
    {
        m_vOrientation = v;
        SetUpdateFlag(2);
    }                                               // 0x004CD170

    CExoLocString m_sLocName;                       // 0x0c
    unsigned short m_nNextGroupActionId;            // 0x14 (placeholder name)
    unsigned short m_nLastGroupActionId;            // 0x16 (placeholder name)
    CExoString m_sTag;                              // 0x18
    CResRef m_cTemplateResRef;                      // 0x20
    unsigned short m_nPortraitId;                   // 0x30
    char pad_0x32[0x34 - 0x32];
    CSWSDialog* m_pDialog;                          // 0x34
    int m_n38;                                      // 0x38
    int m_n3c;                                      // 0x3c
    unsigned long m_n40;                            // 0x40
    unsigned long m_n44;                            // 0x44
    int m_n48;                                      // 0x48
    int m_n4c;                                      // 0x4c
    int m_n50;                                      // 0x50
    unsigned long m_oidDialogOwner;                 // 0x54
    int m_n58;                                      // 0x58
    unsigned long m_nDialogDelayFlags;              // 0x5c (placeholder name)
    int m_n60;                                      // 0x60
    int m_n64;                                      // 0x64
    unsigned long m_nCameraAnimationTime;           // 0x68
    char pad_0x6c[0x70 - 0x6c];
    int m_n70;                                      // 0x70
    int m_n74;                                      // 0x74
    int m_n78;                                      // 0x78
    char pad_0x7c[0x88 - 0x7c];
    int m_n88;                                      // 0x88
    unsigned long m_oidArea;                        // 0x8c
    Vector m_vPosition;                             // 0x90
    Vector m_vOrientation;                          // 0x9c
    char pad_0xa8[0xb8 - 0xa8];
    float m_fb8;                                    // 0xb8
    int m_nbc;                                      // 0xbc
    int m_nc0;                                      // 0xc0
    unsigned long m_nLastUpdateDay;                 // 0xc4 (placeholder name)
    unsigned long m_nLastUpdateTime;                // 0xc8 (placeholder name)
    char pad_0xcc[0xd4 - 0xcc];
    int m_nAnimation;                               // 0xd4
    float m_fd8;                                    // 0xd8
    int m_nCurrentHitPoints;                        // 0xdc
    int m_ne0;                                      // 0xe0
    int m_nTemporaryHitPoints;                      // 0xe4 (placeholder name)
    int m_bCommandable;                             // 0xe8
    int m_nec;                                      // 0xec
    int m_nf0;                                      // 0xf0
    int m_nf4;                                      // 0xf4
    int m_bf8;                                      // 0xf8
    CExoLinkedList<CSWSObjectActionNode> m_lQueuedActions;  // 0xfc
    CSWSScriptVarTable m_ScriptVars;                // 0x100
    CSWVarTable m_VarTable;                         // 0x110
    CExoArrayList<CGameEffect*> m_appliedEffects;   // 0x124
    CExoArrayList<CLoopingVisualEffect*> m_lstLoopingVisualEffects; // 0x130
    CExoArrayList<unsigned long> m_lstEffectTargets;    // 0x13c
    int m_n148;                                     // 0x148
    int m_n14c;                                     // 0x14c
    char pad_0x150[0x154 - 0x150];
    unsigned long m_oid154;                         // 0x154
    unsigned long m_oidLastHostileActor;            // 0x158
    unsigned long m_oid15c;                         // 0x15c
    unsigned long m_oid160;                         // 0x160
    unsigned long m_oid164;                         // 0x164
    int* m_p168;                                    // 0x168
    unsigned short m_n16c;                          // 0x16c
    unsigned char m_n16e;                           // 0x16e
    char pad_0x16f;
    unsigned long m_oid170;                         // 0x170
    unsigned long m_oid174;                         // 0x174
    int m_n178;                                     // 0x178
    unsigned long m_oid17c;                         // 0x17c
    char pad_0x180[0x184 - 0x180];
    CExoString m_s184;                              // 0x184
    CExoArrayList<CSWSExpression*> m_lstListenExpressions;  // 0x18c
    char pad_0x198[0x19c - 0x198];
    int m_bListening;                               // 0x19c (placeholder name)
    CExoArrayList<CExoString*> m_lstMatchedExpressionStrings;   // 0x1a0
    char* m_pDamageImmunity;                        // 0x1ac (placeholder name)
    Vector m_vLastSpellTarget;                      // 0x1b0 (placeholder name)
    unsigned long m_oidLastSpellTarget;             // 0x1bc (placeholder name)
    int m_nLastSpellId;                             // 0x1c0 (placeholder name)
    unsigned char m_n1c4;                           // 0x1c4
    char pad_0x1c5[0x1cc - 0x1c5];
    int m_bSpellImpactQueued;                       // 0x1cc (placeholder name)
    int m_n1d0;                                     // 0x1d0
    unsigned char m_nLastSpellCastClass;            // 0x1d4
    unsigned char m_n1d5;                           // 0x1d5
    char pad_0x1d6[0x1d8 - 0x1d6];
    unsigned long m_oid1d8;                         // 0x1d8
    int m_n1dc;                                     // 0x1dc
    int m_n1e0;                                     // 0x1e0
    int m_n1e4;                                     // 0x1e4
    unsigned short m_n1e8;                          // 0x1e8
    char pad_0x1ea[0x1ec - 0x1ea];
    int m_b1ec;                                     // 0x1ec
    int m_n1f0;                                     // 0x1f0
    int m_n1f4;                                     // 0x1f4
    int m_n1f8;                                     // 0x1f8
    unsigned long m_nUpdateFlags[1];                 // 0x1fc
    int m_bPlotObject;                              // 0x200 (placeholder name)
    int m_bDeleteNoFade;                            // 0x204
    int m_bKeepCorpse;                              // 0x208
    int m_nFadeOutTime;                             // 0x20c
    char pad_0x210[0x214 - 0x210];
    int m_n214;                                     // 0x214
    int m_n218;                                     // 0x218
    int m_n21c;                                     // 0x21c
    int m_bStopSoundPlaying;                        // 0x220 (placeholder name)
    CSWCObject* m_pClientObject;                    // 0x224
};

// FUNCTION: SWKOTOR 0x004CFCB0
CSWSObject::CSWSObject(unsigned char nType, unsigned long oid, int bCharacter)
    : CGameObject(nType, oid)
{
    SetPosition(Vector(), 1, 0, 0);
    SetOrientation(Vector(1.0f, 0.0f, 0.0f));
    m_nAnimation = 10000;
    m_nbc = 0;
    m_nc0 = 0;
    m_fb8 = 1.0f;
    m_n88 = 0;
    if (oid == 0x7f000000) {
        GetServerExoApp()->GetObjectArray()->AddInternalObject(&m_idSelf, this, bCharacter);
    } else {
        GetServerExoApp()->GetObjectArray()->AddExternalObject(m_idSelf, this);
    }
    m_nCurrentHitPoints = 1;
    m_ne0 = 1;
    m_nTemporaryHitPoints = 0;
    m_pDialog = NULL;
    m_oidDialogOwner = OBJECT_INVALID;
    m_oidArea = OBJECT_INVALID;
    m_n50 = 0;
    m_n58 = 10001;
    m_nDialogDelayFlags = 0;
    m_bCommandable = 1;
    m_bListening = 0;
    m_sTag = "";
    m_n1d5 = 0xff;
    m_nLastSpellCastClass = 0xff;
    m_nec = 1;
    m_nf0 = 0;
    m_nf4 = 0;
    m_fd8 = 1.0f;
    m_n78 = -1;
    m_oid17c = 0x7f000000;
    m_oidDialogOwner = 0x7f000000;
    m_oidLastHostileActor = 0x7f000000;
    m_oid160 = 0x7f000000;
    m_oid15c = 0x7f000000;
    m_oid154 = 0x7f000000;
    m_oid164 = 0x7f000000;
    m_n1d0 = 0;
    m_nLastSpellId = -1;
    m_n1c4 = 0;
    m_oidLastSpellTarget = 0x7f000000;
    m_vLastSpellTarget = Vector(0.0f, 0.0f, 0.0f);
    m_bSpellImpactQueued = 0;
    m_n1dc = -1;
    m_n1e0 = 0;
    m_oid1d8 = 0x7f000000;
    m_oid174 = 0x7f000000;
    m_n178 = 0;
    m_n14c = -1;
    m_pDamageImmunity = new char[15];
    int i;
    for (i = 0; i < 15; i++) {
        m_pDamageImmunity[i] = 0;
    }
    m_p168 = new int[15];
    for (i = 0; i < 15; i++) {
        m_p168[i] = -1;
    }
    m_n16c = 0;
    m_n16e = 0;
    m_oid170 = 0x7f000000;
    m_bf8 = 0;
    m_nNextGroupActionId = 0;
    m_nLastGroupActionId = 0;
    m_n1e4 = 0;
    m_n1e8 = 0;
    m_b1ec = 0;
    m_n148 = 0;
    m_nPortraitId = 0xffff;
    m_n1f0 = 0;
    m_n1f4 = 0;
    m_n1f8 = 0;
    m_nUpdateFlags[0] = 0;
    m_bPlotObject = 0;
    m_bDeleteNoFade = 0;
    m_bKeepCorpse = 0;
    m_nFadeOutTime = 0;
    m_n74 = 0;
    m_n214 = 0;
    m_n40 = 0;
    m_n44 = 0;
    m_n70 = 0;
    m_n60 = 0;
    m_n64 = 0;
    m_n218 = 0;
    m_n21c = 1;
    m_pClientObject = NULL;
    m_bStopSoundPlaying = 0;
    GetServerExoApp()->GetWorldTimer()->GetWorldTime(&m_nLastUpdateDay, &m_nLastUpdateTime);
}
