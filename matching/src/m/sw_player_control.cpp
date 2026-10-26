// CSWPlayerControl / CSWPlayerControlCamRelative (swplayercontrol.o).
// Virtual slot names other than Control are descriptive; the base class
// slots are shared do-nothing stubs.

class CExoString {
public:
    CExoString(const char* s);                      // 0x005E5A90
    ~CExoString();                                  // 0x005E5C20
    char* m_sString;
    unsigned long m_nBufferLength;
};

extern "C" double __cdecl fabs(double);
#pragma intrinsic(fabs)

struct Vector {
    float x, y, z;
    Vector() { x = 0.0f; y = 0.0f; z = 0.0f; }
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
};

class C2DA {
public:
    int GetFLOATEntry(int nRow, const CExoString& sColumn, float* pValue);  // 0x00413430
};

class C2DAs {
public:
    char pad_0x0[0x74];
    C2DA* m_pCameraStyles;                          // 0x74
};

class CSWRules {
public:
    C2DAs* Get2DArrays() { return m_p2DArrays; }
    char pad_0x0[0xb8];
    C2DAs* m_p2DArrays;                             // 0xb8
};

extern CSWRules* g_pRules;                          // 0x007A3A28

class CSWCArea {
public:
    char pad_0x0[0xb8];
    int m_nCameraStyle;                             // 0xb8
};

class CSWCModule {
public:
    CSWCArea* GetArea() { return m_pArea; }
    char pad_0x0[0x48];
    CSWCArea* m_pArea;                              // 0x48
};

struct CSWCCreatureStatsMovement {
    char pad_0x0[0x5c];
    float m_fWalkRate;                              // 0x5c
    float m_fRunRate;                               // 0x60
};

class CSWCCreature {
public:
    void SetCombatMode(int bCombatMode);            // 0x0060F090
    char pad_0x0[0x21c];
    CSWCCreatureStatsMovement* m_pMovement;         // 0x21c
    char pad_0x220[0x2d4 - 0x220];
    unsigned char m_nMovementFlags;                 // 0x2d4
};

class CClientOptions {
public:
    float GetFarClip();                             // 0x0061D5E0
    float GetFarClip2();                            // 0x0061D610
    float GetFarClip3();                            // 0x0061D640
    char pad_0x0[0x88];
    int m_bAlwaysRun;                               // 0x88
};

class CSWParty;

class CClientExoApp {
public:
    CSWCModule* GetModule();                        // 0x005ED530
    CSWCCreature* GetCreatureByGameObjectID(unsigned long oid);     // 0x005ED5B0
    CClientOptions* GetClientOptions();             // 0x005ED700
    CSWParty* GetParty();                           // 0x005ED8B0
};

class CAppManager {
public:
    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
    char pad_0x0[0x4];
    CClientExoApp* m_pClientExoApp;                 // 0x04
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC
extern const int COMBAT_MODE_OFF;                   // 0x0074F220 (0)
extern const int COMBAT_MODE_ON;                    // 0x0074F224 (1)
extern int g_bPlayerControlFastMove;                // 0x00833BAC
extern float g_fPlayerControlFastRate;              // 0x007A25F8 (15.0)

#define OBJECT_INVALID 0x7f000000

class CAurCamera;

class CSWCameraClipHelper {
public:
    CSWCameraClipHelper();                          // 0x006D10B0
    char pad_0x0[0x40];
};

class CSWPlayerControl {
public:
    CSWPlayerControl(unsigned long oidCreature, CAurCamera* pCamera)
    {
        m_oidCreature = OBJECT_INVALID;
        m_pCamera = 0;
        m_bCombatMode = 0;
        SetCreature(oidCreature);
        m_pCamera = pCamera;
    }
    virtual void SetTurnRate(float f) {}
    virtual float GetTurnRate() { return 0.0f; }
    virtual void SetMoveRate(float f) {}
    virtual float GetMoveRate() { return 0.0f; }
    virtual void SetMoveMode(int n) {}
    virtual int GetMoveMode() { return 0; }
    virtual void Update() {}
    virtual float GetMaxInput() { return 0.0f; }
    virtual void v20(int n) {}
    virtual void v24(int n) {}
    virtual int Control(float fDeltaTime) { return 0; }
    virtual void ResetMovement() {}

    void Reset();
    void SetCombatModeFlag(int bCombatMode);
    void SetCreature(unsigned long oidCreature);
    void SetCamera(CAurCamera* pCamera);

    unsigned long m_oidCreature;                    // 0x04
    CAurCamera* m_pCamera;                          // 0x08
    int m_bCombatMode;                              // 0x0c
};

class CSWPlayerControlCamRelative : public CSWPlayerControl {
public:
    CSWPlayerControlCamRelative(unsigned long oidCreature, CAurCamera* pCamera);
    virtual void SetTurnRate(float f);
    virtual float GetTurnRate();
    virtual void SetMoveRate(float f);
    virtual float GetMoveRate();
    virtual void SetMoveMode(int n);
    virtual int GetMoveMode();
    virtual float GetMaxInput();
    virtual int Control(float fDeltaTime);
    virtual void ResetMovement()
    {
        m_v68 = Vector(0.0f, 0.0f, 0.0f);
        m_vInput = Vector(0.0f, 0.0f, 0.0f);
        m_v50 = Vector(0.0f, 0.0f, 0.0f);
        m_v44 = Vector(0.0f, 0.0f, 0.0f);
        m_v74 = Vector(0.0f, 0.0f, 0.0f);
        m_vVelocity = Vector(0.0f, 0.0f, 0.0f);
    }

    void Init();
    float GetMovementRate();

    float m_fTurnRate;                              // 0x10
    float m_fMoveRate;                              // 0x14
    int m_nMoveMode;                                // 0x18
    Vector m_vVelocity;                             // 0x1c
    Vector m_v28;                                   // 0x28
    float m_fCameraDistance;                        // 0x34
    float m_fMinTurnRate;                           // 0x38
    float m_fMaxTurnRate;                           // 0x3c
    char pad_0x40[0x4];
    Vector m_v44;                                   // 0x44
    Vector m_v50;                                   // 0x50
    Vector m_vInput;                                // 0x5c
    Vector m_v68;                                   // 0x68
    Vector m_v74;                                   // 0x74
    CSWCameraClipHelper m_cClipHelper;              // 0x80
    float m_fFarClip;                               // 0xc0
    float m_fFarClip2;                              // 0xc4
    float m_fFarClip3;                              // 0xc8
};

// FUNCTION: SWKOTOR 0x006792D0
void CSWPlayerControl::Reset()
{
    m_oidCreature = OBJECT_INVALID;
    m_pCamera = 0;
    m_bCombatMode = 0;
}

// Descriptive name.
// FUNCTION: SWKOTOR 0x006792E0
void CSWPlayerControl::SetCombatModeFlag(int bCombatMode)
{
    m_bCombatMode = bCombatMode;
    if (m_oidCreature != OBJECT_INVALID) {
        CSWCCreature* pCreature = g_pAppManager->m_pClientExoApp->GetCreatureByGameObjectID(m_oidCreature);
        if (pCreature) {
            if (m_bCombatMode) {
                pCreature->SetCombatMode(COMBAT_MODE_ON);
            } else {
                pCreature->SetCombatMode(COMBAT_MODE_OFF);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x00679340
void CSWPlayerControl::SetCreature(unsigned long oidCreature)
{
    if (m_oidCreature != OBJECT_INVALID) {
        CSWCCreature* pCreature = g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(m_oidCreature);
        if (pCreature) {
            pCreature->SetCombatMode(COMBAT_MODE_OFF);
        }
    }
    m_oidCreature = oidCreature;
    if (oidCreature != OBJECT_INVALID && m_bCombatMode) {
        CSWCCreature* pCreature = g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(oidCreature);
        if (pCreature) {
            pCreature->SetCombatMode(COMBAT_MODE_ON);
        }
    }
}

// FUNCTION: SWKOTOR 0x006793B0
void CSWPlayerControl::SetCamera(CAurCamera* pCamera)
{
    m_pCamera = pCamera;
}

// FUNCTION: SWKOTOR 0x006793C0
void CSWPlayerControlCamRelative::Init()
{
    m_fCameraDistance = 480.0f;
    C2DA* pCameraStyles;
    int nCameraStyle = g_pAppManager->m_pClientExoApp->GetModule()->GetArea()->m_nCameraStyle;
    pCameraStyles = g_pRules->Get2DArrays()->m_pCameraStyles;
    pCameraStyles->GetFLOATEntry(nCameraStyle, "MaxTurnRate", &m_fMaxTurnRate);
    pCameraStyles->GetFLOATEntry(nCameraStyle, "MinTurnRate", &m_fMinTurnRate);
    m_vVelocity = Vector(0.0f, 0.0f, 0.0f);
    m_fFarClip = g_pAppManager->m_pClientExoApp->GetClientOptions()->GetFarClip();
    m_fFarClip2 = g_pAppManager->m_pClientExoApp->GetClientOptions()->GetFarClip2();
    m_fFarClip3 = g_pAppManager->m_pClientExoApp->GetClientOptions()->GetFarClip3();
}

// Descriptive name: walk/run rate of the controlled creature.
// FUNCTION: SWKOTOR 0x00679510
float CSWPlayerControlCamRelative::GetMovementRate()
{
    if (g_bPlayerControlFastMove == 0 && g_pAppManager->GetClientExoApp()->GetClientOptions()->m_bAlwaysRun == 0) {
        if (g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(m_oidCreature)) {
            g_pAppManager->GetClientExoApp()->GetParty();
            if (g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(m_oidCreature)->m_nMovementFlags & 1) {
                return g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(m_oidCreature)->m_pMovement->m_fRunRate;
            }
            return g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(m_oidCreature)->m_pMovement->m_fWalkRate;
        }
        return 6.0f;
    }
    return g_fPlayerControlFastRate;
}

// FUNCTION: SWKOTOR 0x006795C0 SYMBOL: ?ResetMovement@CSWPlayerControlCamRelative@@UAEXXZ

// FUNCTION: SWKOTOR 0x006796F0 SYMBOL: ?GetTurnRate@CSWPlayerControl@@UAEMXZ
// FUNCTION: SWKOTOR 0x00679700
void CSWPlayerControlCamRelative::SetTurnRate(float f)
{
    m_fTurnRate = f;
}

// FUNCTION: SWKOTOR 0x00679710
float CSWPlayerControlCamRelative::GetTurnRate()
{
    return m_fTurnRate;
}

// FUNCTION: SWKOTOR 0x00679720
void CSWPlayerControlCamRelative::SetMoveRate(float f)
{
    m_fMoveRate = f;
}

// FUNCTION: SWKOTOR 0x00679730
float CSWPlayerControlCamRelative::GetMoveRate()
{
    return m_fMoveRate;
}

// FUNCTION: SWKOTOR 0x00679740
void CSWPlayerControlCamRelative::SetMoveMode(int n)
{
    m_nMoveMode = n;
}

int CSWPlayerControlCamRelative::GetMoveMode()
{
    return m_nMoveMode;
}

// FUNCTION: SWKOTOR 0x00679750
float CSWPlayerControlCamRelative::GetMaxInput()
{
    float fX = (float)fabs(m_vInput.x);
    float fY = (float)fabs(m_vInput.y);
    return fX > fY ? fX : fY;
}

int CSWPlayerControlCamRelative::Control(float fDeltaTime)
{
    return 0;
}

// FUNCTION: SWKOTOR 0x00679780
CSWPlayerControlCamRelative::CSWPlayerControlCamRelative(unsigned long oidCreature, CAurCamera* pCamera)
    : CSWPlayerControl(oidCreature, pCamera)
{
    ResetMovement();
    Init();
}
