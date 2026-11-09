#include <stdlib.h>
#include <string.h>

// CSWCCombatAttackData initialization and memberwise copying. The copy helper
// name is descriptive: unlike a constructor, its EAX is not the destination.
struct Vector {
    float x, y, z;
    Vector() { x = 0.0f; y = 0.0f; z = 0.0f; }
    Vector(float fx, float fy, float fz) { x = fx; y = fy; z = fz; }
};
class CSWCCombatAttackData {
public:
    unsigned char m_nAttackType, m_nResult;
    char pad_0x2[2];
    unsigned long m_oidAttacker;
    unsigned short m_nAnimation, m_nReaction;
    unsigned char m_nFlags;
    char pad_0xd[3];
    int m_nDamage;
    unsigned long m_oidTarget;
    unsigned short m_n18, m_n1a, m_n1c, m_n1e;
    int m_n20, m_n24;
    unsigned char m_nHand;
    char pad_0x29[3];
    Vector m_vPosition;
    int m_n38, m_n3c, m_n40, m_n44;
    CSWCCombatAttackData();
    void CopyFrom(const CSWCCombatAttackData& source);
};

// FUNCTION: SWKOTOR 0x0060B630
CSWCCombatAttackData::CSWCCombatAttackData()
{
    m_oidAttacker = 0x7f000000;
    m_oidTarget = 0x7f000000;
    m_nAttackType = 0;
    m_nResult = 0;
    m_nAnimation = 0;
    m_nReaction = 0;
    m_nFlags = 0;
    m_n1e = 0;
    m_nDamage = 0;
    m_n18 = 0;
    m_n1a = 0;
    m_n1c = 0;
    m_vPosition = Vector(0.0f, 0.0f, 0.0f);
    m_n38 = -1;
    m_n3c = 0;
    m_n44 = 0;
    m_n20 = 0;
    m_n24 = 0;
    m_nHand = 0;
    m_n40 = 0;
}

// FUNCTION: SWKOTOR 0x0060B6B0
void CSWCCombatAttackData::CopyFrom(const CSWCCombatAttackData& source)
{
    m_nAttackType = source.m_nAttackType;
    m_nResult = source.m_nResult;
    m_oidAttacker = source.m_oidAttacker;
    m_oidTarget = source.m_oidTarget;
    m_nAnimation = source.m_nAnimation;
    m_nReaction = source.m_nReaction;
    m_nFlags = source.m_nFlags;
    m_nDamage = source.m_nDamage;
    m_n1e = source.m_n1e;
    m_n18 = source.m_n18;
    m_n1a = source.m_n1a;
    m_n1c = source.m_n1c;
    m_vPosition = source.m_vPosition;
    m_n38 = source.m_n38;
    m_n3c = source.m_n3c;
    m_n44 = source.m_n44;
    m_n20 = source.m_n20;
    m_n24 = source.m_n24;
    m_nHand = source.m_nHand;
    m_n40 = source.m_n40;
}

// CSWCCreature constructor (worker C s10). It lives in this file because the
// original knows that the attack-data constructor above cannot throw: the two
// attack-data allocations have no exception state. The layout is local to this
// translation unit; member names without a project record are descriptive.

void* __cdecl operator new(unsigned int n);         // 0x006FA7E6

class CResRef {
public:
    CResRef();                                      // 0x00405ED0
    char m_resRef[16];
};

template <class T>
class CExoArrayList {
public:
    T* element;                                     // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08

    CExoArrayList() { num = 0; array_size = 0; element = 0; }
    ~CExoArrayList() { delete[] element; element = 0; }
};

class CSWCLevelUpStats {
public:
    CSWCLevelUpStats();                             // 0x0064B2D0
    char pad_0x0[0x134];
};

class CSWCSpellCastVisual {
public:
    CSWCSpellCastVisual();                          // 0x00695CE0
    char pad_0x0[0x30];
};

class CSWCCreature;

class CSWCCreatureAppearance {
public:
    CSWCCreatureAppearance(CSWCCreature* pCreature);    // 0x00696430
    char pad_0x0[0x6c];
};

class CWorldTimer {
public:
    void GetWorldTime(unsigned long* pDay, unsigned long* pTime);   // 0x004ADE40
};

class CClientExoApp {
public:
    CWorldTimer* GetWorldTimer();                   // 0x005ED4F0
};

class CAppManager {
public:
    void* m_pUnknown00;
    CClientExoApp* m_pClientExoApp;                 // 0x04

    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC
extern const unsigned long OBJECT_INVALID;          // 0x00746644
extern const unsigned char OBJECT_TYPE_CREATURE;    // 0x00746634

struct CPerceptionRecord;

class CSWCObject {
public:
    CSWCObject(unsigned char nType);                // 0x0063E530
    virtual ~CSWCObject();                          // 0x0063E800

    char pad_0x4[0x30 - 0x4];
    Vector m_vOrientation;                          // 0x30
    unsigned long m_nWorldDay;                      // 0x3c
    unsigned long m_nWorldTime;                     // 0x40
    char pad_0x44[0x48 - 0x44];
    unsigned short m_nAnimation;                    // 0x48
    char pad_0x4a[0x58 - 0x4a];
    unsigned short m_nLoopingAnimation;             // 0x58
    char pad_0x5a[0xfc - 0x5a];
};

class CSWCCreature : public CSWCObject {
public:
    CSWCCreature();
    virtual ~CSWCCreature();                        // 0x006170D0

    void* m_apSoundSources[12];                     // 0xfc
    void* m_p12C;                                   // 0x12c
    void* m_p130;                                   // 0x130
    void* m_p134;                                   // 0x134
    int m_n138;                                     // 0x138
    int m_n13C;                                     // 0x13c
    int m_n140;                                     // 0x140
    int m_n144;                                     // 0x144
    int m_n148;                                     // 0x148
    int m_n14C;                                     // 0x14c
    int m_n150;                                     // 0x150
    int m_n154;                                     // 0x154
    int m_n158;                                     // 0x158
    void* m_p15C;                                   // 0x15c
    void* m_apPathRequests[4];                      // 0x160
    void* m_pPathRequest;                           // 0x170
    int m_n174;                                     // 0x174
    int m_n178;                                     // 0x178
    void* m_pLoopingSound;                          // 0x17c
    int m_n180;                                     // 0x180
    unsigned short m_n184;                          // 0x184
    char pad_0x186[0x2];
    int m_n188;                                     // 0x188
    int m_n18C;                                     // 0x18c
    int m_n190;                                     // 0x190
    int m_n194;                                     // 0x194
    int m_n198;                                     // 0x198
    int m_n19C;                                     // 0x19c
    int m_n1A0;                                     // 0x1a0
    int m_n1A4;                                     // 0x1a4
    int m_n1A8;                                     // 0x1a8
    int m_n1AC;                                     // 0x1ac
    int m_n1B0;                                     // 0x1b0
    int m_n1B4;                                     // 0x1b4
    int m_n1B8;                                     // 0x1b8
    Vector* m_pWayPoints;                           // 0x1bc
    int m_n1C0;                                     // 0x1c0
    Vector m_v1C4;                                  // 0x1c4
    Vector m_v1D0;                                  // 0x1d0
    Vector m_v1DC;                                  // 0x1dc
    int m_n1E8;                                     // 0x1e8
    unsigned short m_n1EC;                          // 0x1ec
    unsigned char m_n1EE;                           // 0x1ee
    unsigned char m_n1EF;                           // 0x1ef
    unsigned char m_n1F0;                           // 0x1f0
    char pad_0x1f1[0x3];
    int m_n1F4;                                     // 0x1f4
    char pad_0x1f8[0x4];
    float m_f1FC;                                   // 0x1fc
    Vector m_v200;                                  // 0x200
    char pad_0x20c[0x214 - 0x20c];
    int m_n214;                                     // 0x214
    char pad_0x218[0x4];
    CSWCCreatureAppearance* m_pAppearance;          // 0x21c
    unsigned long m_aEquippedItems[18];             // 0x220
    unsigned long m_oidShownRightWeapon;            // 0x268
    unsigned long m_oidShownLeftWeapon;             // 0x26c
    int m_aLastInstant[18];                         // 0x270
    CSWCCombatAttackData* m_pAttackData;            // 0x2b8
    CSWCCombatAttackData* m_pDefendData;            // 0x2bc
    int m_n2C0;                                     // 0x2c0
    char pad_0x2c4[0x4];
    unsigned long m_oid2C8;                         // 0x2c8
    int m_bBlurWeapon;                              // 0x2cc
    unsigned short m_n2D0;                          // 0x2d0
    char pad_0x2d2[0x2dc - 0x2d2];
    unsigned char m_n2DC;                           // 0x2dc
    CResRef m_cResRef2DD;                           // 0x2dd
    char pad_0x2ed[0x3];
    CSWCSpellCastVisual* m_pCastVisual;             // 0x2f0
    unsigned long m_oid2F4;                         // 0x2f4
    CSWCLevelUpStats* m_pStats;                     // 0x2f8
    unsigned short m_n2FC;                          // 0x2fc
    char pad_0x2fe[0x2];
    int m_n300;                                     // 0x300
    int m_n304;                                     // 0x304
    int m_n308;                                     // 0x308
    void* m_pNameRecord;                            // 0x30c
    int m_n310;                                     // 0x310
    int m_n314;                                     // 0x314
    int m_n318;                                     // 0x318
    char pad_0x31c[0x358 - 0x31c];
    int m_n358;                                     // 0x358
    int m_n35C;                                     // 0x35c
    CExoArrayList<CPerceptionRecord*> m_lstPerception;  // 0x360
    void* m_pSoundSet;                              // 0x36c
    int m_n370;                                     // 0x370
    int m_n374;                                     // 0x374
    int m_n378;                                     // 0x378
    unsigned long m_oid37C;                         // 0x37c
    unsigned long m_oid380;                         // 0x380
    unsigned long m_oid384;                         // 0x384
    unsigned long m_oid388;                         // 0x388
    char pad_0x38c[0x4];
    int m_n390;                                     // 0x390
    int m_n394;                                     // 0x394
    int m_n398;                                     // 0x398
    unsigned long m_oid39C;                         // 0x39c
    void* m_pVisibleList;                           // 0x3a0
    int m_n3A4;                                     // 0x3a4
    int m_n3A8;                                     // 0x3a8
    int m_n3AC;                                     // 0x3ac
    int m_n3B0;                                     // 0x3b0
    int m_n3B4;                                     // 0x3b4
    int m_n3B8;                                     // 0x3b8
    int m_n3BC;                                     // 0x3bc
    int m_n3C0;                                     // 0x3c0
    int m_n3C4;                                     // 0x3c4
    unsigned long m_oidProjectile;                  // 0x3c8
    int m_n3CC;                                     // 0x3cc
    int m_n3D0;                                     // 0x3d0
    char pad_0x3d4[0x4];
    int m_bOverrideLightsaber;                      // 0x3d8
    unsigned long m_oid3DC;                         // 0x3dc
    unsigned char m_n3E0;                           // 0x3e0
    char pad_0x3e1[0x3];
    unsigned long m_oid3E4;                         // 0x3e4
    int m_n3E8;                                     // 0x3e8
    int m_a3EC[17];                                 // 0x3ec
    void* m_p430;                                   // 0x430
    int m_n434;                                     // 0x434
    int m_n438;                                     // 0x438
    int m_n43C;                                     // 0x43c
    unsigned int m_b440_0 : 1;                      // 0x440
    unsigned int m_b440_1 : 1;
    unsigned short m_n444;                          // 0x444
    char pad_0x446[0x2];
    int m_n448;                                     // 0x448
};

// NONMATCH (3 insns): ours schedules the m_f1FC constant store ahead of the
// imul of the random idle delay; the original stores the delay first.
// FUNCTION: SWKOTOR 0x00616A20
CSWCCreature::CSWCCreature()
    : CSWCObject(OBJECT_TYPE_CREATURE)
{
    m_b440_0 = 0;
    m_b440_1 = 1;
    m_n3A4 = 0;
    m_pSoundSet = 0;
    m_n2FC = 0xffff;
    m_pStats = new CSWCLevelUpStats;
    m_pWayPoints = 0;
    m_n1C0 = 0;
    m_n1EC = 0;
    m_nAnimation = 10000;
    m_n2C0 = 0;
    m_n14C = 0;
    m_n150 = 0;
    m_n1B8 = 0;
    m_bOverrideLightsaber = 0;
    m_oidShownRightWeapon = 0x7f000000;
    m_oidShownLeftWeapon = 0x7f000000;
    m_n1E8 = 0;
    m_pAttackData = new CSWCCombatAttackData;
    m_pDefendData = new CSWCCombatAttackData;
    int i;
    for (i = 0; i < 18; i++) {
        m_aEquippedItems[i] = OBJECT_INVALID;
        m_aLastInstant[i] = 0;
    }
    unsigned long nDay, nTime;
    g_pAppManager->GetClientExoApp()->GetWorldTimer()->GetWorldTime(&nDay, &nTime);
    m_nWorldDay = nDay;
    m_nWorldTime = nTime;
    m_pAppearance = new CSWCCreatureAppearance(this);
    if (m_pAppearance) {
        m_n1EE = 0;
        m_n1EF = 0;
        m_n1F0 = 0;
        m_n1F4 = 1;
        m_n214 = 0;
        m_vOrientation = Vector(1.0f, 0.0f, 0.0f);
        m_v1C4 = Vector(1.0f, 0.0f, 0.0f);
        m_v1D0 = Vector(1.0f, 0.0f, 0.0f);
        m_nLoopingAnimation = 10000;
        m_bBlurWeapon = 0;
        m_pLoopingSound = 0;
        m_n180 = 0;
        m_n184 = 0;
        m_n188 = 0;
        m_n18C = 1;
        m_pCastVisual = new CSWCSpellCastVisual;
        m_oid2F4 = OBJECT_INVALID;
        m_n2D0 = 0xffff;
        m_n138 = 0;
        m_p15C = 0;
        m_n190 = 0;
        m_n194 = 0;
        m_n198 = 0;
        m_n19C = 0;
        m_n1A0 = 0;
        m_n1A4 = 0;
        m_n1A8 = 0;
        m_n1AC = 0;
        m_n1B0 = 0;
        for (i = 0; i < 12; i++) {
            m_apSoundSources[i] = 0;
        }
        m_p12C = 0;
        m_p130 = 0;
        m_p134 = 0;
        m_n148 = 0;
        m_n300 = 0;
        m_n304 = 0;
        for (i = 0; i < 4; i++) {
            m_apPathRequests[i] = 0;
        }
        m_pPathRequest = 0;
        m_n174 = 0;
        m_n178 = 0;
        m_n2DC = 2;
        m_n1B4 = 0;
        m_n308 = 200000;
        m_pNameRecord = 0;
        m_n144 = 0;
        m_oid2C8 = OBJECT_INVALID;
        m_n158 = 2000;
        m_n154 = 6000;
        m_n358 = 0;
        m_n35C = 0;
        m_n310 = 0;
        m_n314 = 0;
        m_n318 = 0;
        m_n370 = 0;
        m_n374 = 0;
        m_n378 = 0;
        m_oid37C = OBJECT_INVALID;
        m_oid380 = OBJECT_INVALID;
        m_oid384 = OBJECT_INVALID;
        m_oid388 = OBJECT_INVALID;
        m_n390 = 0;
        m_n398 = 0;
        m_n394 = (rand() % 4 + 2) * 30000;
        m_f1FC = 1.0f;
        m_oid39C = OBJECT_INVALID;
        m_pVisibleList = 0;
        m_n3A8 = 0;
        m_n3AC = 0;
        m_n3B0 = 0;
        m_n3B4 = 0;
        m_n3B8 = 0;
        m_n3BC = 0x2772;
        m_n3E8 = 0;
        m_n13C = 0;
        m_n140 = 0;
        m_oidProjectile = 0x7f000000;
        m_n3CC = 0;
        m_n3C0 = -1;
        m_n3C4 = -1;
        m_n3D0 = 0;
        m_oid3DC = 0x7f000000;
        m_n3E0 = 0;
        m_oid3E4 = 0x7f000000;
        for (i = 0; i < 17; i++) {
            m_a3EC[i] = 0;
        }
        m_p430 = 0;
        m_n434 = 0;
        m_n438 = 0;
        m_n43C = 0;
        m_n444 = 0xffff;
        m_n448 = 0;
    }
}
