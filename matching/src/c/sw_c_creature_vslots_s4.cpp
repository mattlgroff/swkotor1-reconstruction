// CSWCCreature / CSWCObject virtual overrides and creature animation-event
// callbacks (worker c, round s4). Every address here is a real function start
// that functions.csv lacks; each is referenced from the CSWCCreature vtable
// (0x0074F5F8), the CSWCObject vtable (0x007516C8) or a callback table.
// Slot names follow matching/include/n/sw_c_object.h; creature-only slots
// past 0x148 have descriptive names.

class CResRef;
class CExoString;
class CGuiInGame;
class CSWCArea;

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
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9C();
    virtual void vA0(); virtual void vA4(); virtual void vA8(); virtual void vAC();
    virtual void vB0(); virtual void vB4();
    virtual void TurnOnShadows();                   // 0xb8
    virtual void TurnOffShadows();                  // 0xbc

    int Hilite(int a, int b, int c);                // 0x006A03E0
    int UnHilite(int a);                            // 0x006A0440
};

class CGuiInGame {
public:
    void ShowBarkBubble(unsigned long oid, const CExoString& text, const CResRef& sound, int b);   // 0x0062AB30
};

struct CSWCAreaX {
    char pad_0x0[0x104];
    unsigned long m_nId104;                         // 0x104
};

class CSWCModuleX {
public:
    char pad_0x0[0x48];
    CSWCAreaX* m_pArea;                             // 0x48
};

class CClientExoApp {
public:
    CSWCModuleX* GetModule();                       // 0x005ED530
    CGuiInGame* GetInGameGui();                     // 0x005ED690
};

class CAppManager {
public:
    void* m_pUnknown00;
    CClientExoApp* m_pClientExoApp;                 // 0x04

    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC
extern const unsigned long OBJECT_INVALID_ID;       // 0x00746644

struct Vector {
    float x, y, z;
    Vector() {}
    Vector(float fx, float fy, float fz) { x = fx; y = fy; z = fz; }
};

Vector Normalize(const Vector& v);                  // 0x004AB180

class CSWCObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80();
    virtual void SetArea(CSWCAreaX* pArea);         // 0x84
    virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94(); virtual void v98();
    virtual void SetAnimation(unsigned short nAnim, int a, int b);  // 0x9c
    virtual void AddFireForgetAnimation(unsigned short nAnim, void* pData);  // 0xa0
    virtual int Hilite(int a, int b, int c, int d); // 0xa4
    virtual int UnHilite(int a);                    // 0xa8
    virtual void vAC();
    virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC();
    virtual void vC0(); virtual void vC4(); virtual void vC8(); virtual void vCC();
    virtual void vD0(); virtual void vD4(); virtual void vD8(); virtual void vDC();
    virtual void vE0(); virtual void vE4();
    virtual void UnknownE8();                       // 0xe8
    virtual void vEC();
    virtual void vF0(); virtual void vF4(); virtual void vF8(); virtual void vFC();
    virtual void RemoveShadowBlob();                // 0x100
    virtual void v104(); virtual void v108(); virtual void v10C();
    virtual void TurnOnShadows();                   // 0x110
    virtual void TurnOffShadows();                  // 0x114
    virtual void v118(); virtual void v11C();
    virtual int BarkString(unsigned long oid, const CExoString& text, const CResRef& sound, int b);  // 0x120
    virtual int BarkBubble(unsigned long oid, const CExoString& text, const CResRef& sound, int b);  // 0x124
    virtual int Unknown128(const CResRef& sound);                  // 0x128 (CSWCCreature: lip sync)
    virtual void v12C();
    virtual void ClientServerSyncHandler_Orientation(const Vector& v);  // 0x130
    virtual void ClientServerSyncHandler_Animation(unsigned long nAnim, int nTime);  // 0x134

    unsigned short GetCurrentAnimation();           // 0x0063C060
    int AnimationFireAndForget(unsigned short nAnim);   // 0x0063C380
    int AnimationStationary(unsigned short nAnim);  // 0x0063C170

    unsigned long m_idSelf;                         // 0x04
    char pad_0x8[0x18];
    CSWCAreaX* m_pArea;                             // 0x20
    char pad_0x24[0x28];
    int m_nAnimationTime;                           // 0x4c
    char pad_0x50[0x18];
    CSWCAnimBase* m_pAnimBase;                      // 0x68
};

class CSWCCreature : public CSWCObject {
public:
    virtual void v138(); virtual void v13C();
    virtual void v140(); virtual void v144(); virtual void v148();
    virtual Vector* GetCurrentWayPoint();           // 0x14c
    virtual Vector* GetLastWayPoint();              // 0x150
    virtual Vector* GetFirstWayPoint();             // 0x154

    virtual void SetArea(CSWCAreaX* pArea);         // 0x84
    virtual int Hilite(int a, int b, int c, int d); // 0xa4
    virtual int UnHilite(int a);                    // 0xa8
    virtual void UnknownE8();                       // 0xe8
    virtual void RemoveShadowBlob();                // 0x100
    virtual int BarkString(unsigned long oid, const CExoString& text, const CResRef& sound, int b);  // 0x120
    virtual void ClientServerSyncHandler_Animation(unsigned long nAnim, int nTime);  // 0x134
    virtual void ClientServerSyncHandler_Orientation(const Vector& v);  // 0x130

    void TurnToDirection(Vector* pDirection);       // 0x00611850 (descriptive)
    void AreaPopUpMatch();                          // 0x004015A0 (empty)
    void AreaPopUpMismatch();                       // 0x004015A0 (empty)

    char pad_0x6c[0x150];
    Vector* m_pWayPoints;                           // 0x1bc
    Vector* m_pCurWayPoint;                         // 0x1c0
    Vector m_vDesiredDirection;                     // 0x1c4
    Vector m_vDesiredOrientation;                   // 0x1d0
    char pad_0x1dc[0x10];
    unsigned short m_nWayPoints;                    // 0x1ec
    char pad_0x1ee[0x1ae];
    unsigned long m_nId39C;                         // 0x39c
};

// FUNCTION: SWKOTOR 0x0060B310
void CSWCObject::TurnOnShadows()
{
    if (m_pAnimBase) {
        m_pAnimBase->TurnOnShadows();
    }
}

// FUNCTION: SWKOTOR 0x0060B320
void CSWCObject::TurnOffShadows()
{
    if (m_pAnimBase) {
        m_pAnimBase->TurnOffShadows();
    }
}

// FUNCTION: SWKOTOR 0x0060C100
void CSWCCreature::ClientServerSyncHandler_Animation(unsigned long nAnim, int nTime)
{
    m_nAnimationTime = nTime;
    if (nAnim == 0x2719 || nAnim == 0x277d || nAnim == 0x271c || nAnim == 0x271b
        || nAnim == 0x271e || nAnim == 0x2716 || nAnim == 0x2718 || nAnim == 0x27ac) {
        return;
    }
    if (nAnim == GetCurrentAnimation()) {
        return;
    }
    if (AnimationFireAndForget((unsigned short)nAnim)) {
        AddFireForgetAnimation((unsigned short)nAnim, 0);
    } else {
        SetAnimation((unsigned short)nAnim, 0, 0);
    }
}

// FUNCTION: SWKOTOR 0x0060C190
Vector* CSWCCreature::GetCurrentWayPoint()
{
    return m_pCurWayPoint;
}

// FUNCTION: SWKOTOR 0x0060C1A0
Vector* CSWCCreature::GetLastWayPoint()
{
    if (m_nWayPoints == 0) {
        return 0;
    }
    return &m_pWayPoints[m_nWayPoints - 1];
}

// FUNCTION: SWKOTOR 0x0060C1C0
Vector* CSWCCreature::GetFirstWayPoint()
{
    if (m_nWayPoints == 0) {
        return 0;
    }
    return m_pWayPoints;
}

// FUNCTION: SWKOTOR 0x0060E480
void CSWCCreature::RemoveShadowBlob()
{
    CSWCObject::RemoveShadowBlob();
}

// FUNCTION: SWKOTOR 0x0060E770
int CSWCCreature::Hilite(int a, int b, int c, int d)
{
    if (m_pAnimBase) {
        return m_pAnimBase->Hilite(a, b, d);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0060E7A0
int CSWCCreature::UnHilite(int a)
{
    if (m_pAnimBase) {
        return m_pAnimBase->UnHilite(a);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0060EDC0
void CSWCCreature::UnknownE8()
{
    CSWCModuleX* pModule = g_pAppManager->GetClientExoApp()->GetModule();
    if (pModule == 0) {
        return;
    }
    CSWCAreaX* pArea = pModule->m_pArea;
    if (pArea == 0) {
        return;
    }
    if (m_nId39C == pArea->m_nId104) {
        AreaPopUpMatch();
    } else {
        AreaPopUpMismatch();
    }
}

// FUNCTION: SWKOTOR 0x0060F510
int CSWCCreature::BarkString(unsigned long oid, const CExoString& text, const CResRef& sound, int b)
{
    Unknown128(sound);
    return BarkBubble(oid, text, sound, b);
}

// FUNCTION: SWKOTOR 0x0060F560
int CSWCObject::BarkBubble(unsigned long oid, const CExoString& text, const CResRef& sound, int b)
{
    g_pAppManager->GetClientExoApp()->GetInGameGui()->ShowBarkBubble(oid, text, sound, b);
    return 1;
}

// FUNCTION: SWKOTOR 0x00614B20
void CSWCCreature::SetArea(CSWCAreaX* pArea)
{
    CSWCObject::SetArea(pArea);
    if (m_pArea) {
        m_nId39C = m_pArea->m_nId104;
    } else {
        m_nId39C = OBJECT_INVALID_ID;
    }
}

// FUNCTION: SWKOTOR 0x00611EC0
void CSWCCreature::ClientServerSyncHandler_Orientation(const Vector& v)
{
    m_vDesiredOrientation = Normalize(v);
    m_vDesiredDirection = Normalize(v);
    if (AnimationStationary(GetCurrentAnimation())) {
        TurnToDirection(&m_vDesiredOrientation);
    } else {
        TurnToDirection(&m_vDesiredDirection);
    }
}
