// CClientExoAppInternal members, worker C s9
// (clientexoappinternal.o; translation-unit-local layout).

class CGuiInGame;

class CSWCItem {
public:
    void SetPowered(int a, int b, int c, int d);    // 0x00647130
};

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30();
    virtual CSWCItem* AsSWCItem();                  // 0x34
};

class CSWCAnimBase {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40();
    virtual void SetDefaultAnimation(CSWCAnimBase& anim);  // 0x44 (descriptive)
    virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9C();
    virtual void vA0(); virtual void vA4(); virtual void vA8(); virtual void vAC();
    virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC();
    virtual void vC0(); virtual void vC4(); virtual void vC8(); virtual void vCC();
    virtual void vD0(); virtual void vD4(); virtual void vD8(); virtual void vDC();
    virtual CSWCAnimBase* GetAnimation(unsigned short nAnim, int b);   // 0xe0 (descriptive)

    char pad_0x4[0xc5 - 0x4];
    unsigned char m_nWeaponState;                   // 0xc5 (descriptive)
};

class CSWCCreature;

class CSWSCreature {
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
    virtual void v90();
    virtual int GetDead();                          // 0x94 (descriptive)

    int GetIsDying();                               // 0x004EF890
    CSWCCreature* GetClientCreature();              // 0x004EFB20
    void SetCombatState(int b, unsigned char n);    // 0x004F2610
};

class CSWCCreature {
public:
    unsigned long GetEquippedItemID(unsigned long nSlot);   // 0x0060CA80
    CSWSCreature* GetServerCreature();              // 0x0060FB20
    int ComputeWeaponOverlays(int a, int b);        // 0x00611700
    CSWCAnimBase* GetAnimationBase();               // 0x0063C070
    void AddFireForgetAnimation(unsigned short nAnim, unsigned long nTime, float fSpeed,
                                void* pCallback, unsigned long nData);  // 0x0063C760
};

class CClientExoApp {
public:
    CGameObject* GetGameObject(unsigned long oid);  // 0x005ED580
    CGuiInGame* GetInGameGui();                     // 0x005ED690
    CSWCCreature* GetCreatureByGameObjectID(unsigned long oid);   // 0x005ED5B0
};

class CAppManager {
public:
    void* m_pUnknown00;
    CClientExoApp* m_pClientExoApp;                 // 0x04

    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CSWPlayerControlCamRelative {
public:
    virtual void v00();
    virtual float GetMoveY();                       // 0x04 (descriptive)
    virtual void v08();
    virtual float GetMoveX();                       // 0x0c (descriptive)
};

struct Vector {
    Vector() {}
    Vector(float a, float b, float c) : x(a), y(b), z(c) {}
    Vector operator-(const Vector& v) const { return Vector(x - v.x, y - v.y, z - v.z); }
    Vector& operator+=(const Vector& v);            // 0x004A9740
    Vector& operator-=(const Vector& v);            // 0x004A9770
    float x, y, z;
};

Vector operator*(const Vector& v, float f);         // 0x00423BF0

struct Quaternion {
    Vector ToDirection() const;                     // 0x0043E250
    Vector GetYAxis() const;                        // 0x0044F420
    float w, x, y, z;
};

class CExoSound {
public:
    void Render();                                  // 0x005D5DD0
    void SetListenerOrientation(const Vector& vFront, const Vector& vUp);   // 0x005D5DE0
    void SetListenerPosition(const Vector& vPosition);  // 0x005D5DF0
};

extern CExoSound* g_pExoSound;                      // 0x007A39EC
extern float g_fListenerOffsetBlend;                // 0x007A22A0 (descriptive)

class CSWCameraOnAStick {
public:
    Vector GetTargetHeadPosition();                 // 0x005F5180
};

class CSWCCamera {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual Vector GetPosition();                   // 0x20
    virtual Quaternion GetOrientation();            // 0x24
    virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void* GetCameraOfType(unsigned long nType);  // 0x80 (descriptive)
};

class CSWTrackFollower {
public:
    Vector GetHookPosition();                       // 0x0066D5D0
};

class CSWCMiniGame {
public:
    char pad_0x0[0x24];
    CSWTrackFollower* m_pPlayer;                    // 0x24
};

class CSWCModule {
public:
    CSWCMiniGame* GetMiniGame();                    // 0x00641980

    char pad_0x0[0x40];
    CSWCCamera* m_pCamera;                          // 0x40
};

class CSWPartyCharacter {
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
    virtual void vB0(); virtual void vB4(); virtual void vB8(); virtual void vBC();
    virtual void vC0(); virtual void vC4(); virtual void vC8(); virtual void vCC();
    virtual void vD0(); virtual void vD4();
    virtual float GetHeadHeight();                  // 0xd8 (descriptive)

    char pad_0x4[0x24 - 0x4];
    Vector m_vPosition;                             // 0x24
};

class CSWParty {
public:
    CSWPartyCharacter* GetCharacter(int n);         // 0x006346C0
};

class CGuiInGame {
public:
    char pad_0x0[0xb4];
    int m_bUnknownB4;                               // 0xb4
    char pad_0xb8[0xe4 - 0xb8];
    int m_bUnknownE4;                               // 0xe4
    char pad_0xe8[0xf4 - 0xe8];
    float m_fListenerDistance;                      // 0xf4 (descriptive)

    float GetListenerDistance() { return m_fListenerDistance; }
};

class CClientExoAppInternal {
public:
    CSWCCreature* GetPlayerCreature();              // 0x005F1420
    void PlayerFlourishWeapons();
    void UpdateSoundEngine();

    __forceinline CSWCCreature* GetCachedPlayerCreature()
    {
        if (m_oidCachedPlayer == m_oidPlayer) {
            return m_pCachedPlayerCreature;
        }
        CSWCCreature* pCreature = g_pAppManager->GetClientExoApp()->GetCreatureByGameObjectID(m_oidPlayer);
        m_pCachedPlayerCreature = pCreature;
        m_oidCachedPlayer = m_oidPlayer;
        return pCreature;
    }

    char pad_0x0[0x18];
    CSWCModule* m_pModule;                          // 0x18
    char pad_0x1c[0x20 - 0x1c];
    unsigned long m_oidPlayer;                      // 0x20
    char pad_0x24[0x270 - 0x24];
    CSWParty* m_pParty;                             // 0x270
    char pad_0x274[0x2a0 - 0x274];
    CSWPlayerControlCamRelative* m_pPlayerControl;  // 0x2a0
    char pad_0x2a4[0x3bc - 0x2a4];
    unsigned long m_oidCachedPlayer;                // 0x3bc
    CSWCCreature* m_pCachedPlayerCreature;          // 0x3c0
};

// NONMATCH (2 instructions): the original sets ecx for the outer +0x44 call
// before pushing the GetAnimation result; nested, named-temp and reference
// forms all push first.
// FUNCTION: SWKOTOR 0x005F4460
void CClientExoAppInternal::PlayerFlourishWeapons()
{
    GetCachedPlayerCreature()->ComputeWeaponOverlays(0, 1);
    CSWSCreature* pServer = GetCachedPlayerCreature()->GetServerCreature();
    if (pServer && !pServer->GetDead() && !pServer->GetIsDying()) {
        unsigned char nState = pServer->GetClientCreature()->GetAnimationBase()->m_nWeaponState;
        if (nState < 2 || nState > 4) {
            return;
        }
        unsigned long oidItem = GetPlayerCreature()->GetEquippedItemID(0x10);
        if (oidItem != 0x7f000000) {
            CGameObject* pItem = g_pAppManager->GetClientExoApp()->GetGameObject(oidItem);
            if (pItem && pItem->AsSWCItem()) {
                pItem->AsSWCItem()->SetPowered(1, 0, 1, 0);
            }
        }
        oidItem = GetPlayerCreature()->GetEquippedItemID(0x20);
        if (oidItem != 0x7f000000) {
            CGameObject* pItem = g_pAppManager->GetClientExoApp()->GetGameObject(oidItem);
            if (pItem && pItem->AsSWCItem()) {
                pItem->AsSWCItem()->SetPowered(1, 0, 1, 0);
            }
        }
        pServer->SetCombatState(1, 2);
    }
    if (m_pPlayerControl && (m_pPlayerControl->GetMoveX() != 0.0f || m_pPlayerControl->GetMoveY() != 0.0f)) {
        GetCachedPlayerCreature()->GetAnimationBase()->SetDefaultAnimation(*
            GetCachedPlayerCreature()->GetAnimationBase()->GetAnimation(0x27ad, 1));
    } else {
        GetCachedPlayerCreature()->AddFireForgetAnimation(0x27ae, 0x5dc, 1.0f, 0, 0);
    }
}

// NONMATCH (~49 insns): the camera-on-a-stick branch. The original copies the
// head position into the offset slot and multiplies (diff * blend) with the
// global as a memory operand; ours loads the global first. Making operator*
// inline matches that branch but then inlines the 0x1070 branch as well (the
// original calls the out-of-line 0x00423BF0 copy there: inline budget).
// FUNCTION: SWKOTOR 0x005F5370
void CClientExoAppInternal::UpdateSoundEngine()
{
    if (m_pModule) {
        CSWCCamera* pCamera = m_pModule->m_pCamera;
        CSWCCreature* pPlayer = GetCachedPlayerCreature();
        if (pCamera) {
            CSWCMiniGame* pMiniGame = m_pModule->GetMiniGame();
            CSWTrackFollower* pFollower = pMiniGame ? pMiniGame->m_pPlayer : 0;
            if (pFollower) {
                pFollower->GetHookPosition();
            }
            if (pFollower || pPlayer) {
                Vector vPosition(0.0f, 0.0f, 0.0f);
                Vector vFront(0.0f, 0.0f, 0.0f);
                vPosition = pCamera->GetPosition();
                Quaternion qOrientation;
                qOrientation = pCamera->GetOrientation();
                if (g_fListenerOffsetBlend < 0.0f) {
                    g_fListenerOffsetBlend = 0.0f;
                } else if (g_fListenerOffsetBlend > 1.0f) {
                    g_fListenerOffsetBlend = 1.0f;
                }
                CSWCameraOnAStick* pStick = (CSWCameraOnAStick*)pCamera->GetCameraOfType(0x106a);
                if (pStick) {
                    Vector vOffset;
                    vOffset = pStick->GetTargetHeadPosition();
                    Vector vDiff = vOffset - vPosition;
                    vOffset.x = vDiff.x * g_fListenerOffsetBlend;
                    vOffset.y = vDiff.y * g_fListenerOffsetBlend;
                    vOffset.z = vDiff.z * g_fListenerOffsetBlend;
                    vPosition += vOffset;
                } else if (pCamera->GetCameraOfType(0x1070)) {
                    CSWPartyCharacter* pCharacter = m_pParty->GetCharacter(0);
                    Vector vTarget = pCharacter->m_vPosition;
                    float fTop = pCharacter->GetHeadHeight() + vTarget.z;
                    vPosition += Vector(vTarget.x - vPosition.x, vTarget.y - vPosition.y, fTop - vPosition.z) *
                                 g_fListenerOffsetBlend;
                } else if (g_pAppManager->GetClientExoApp()->GetInGameGui()->m_bUnknownB4 &&
                           g_pAppManager->GetClientExoApp()->GetInGameGui()->m_bUnknownE4) {
                    vPosition -= qOrientation.ToDirection() *
                                 g_pAppManager->GetClientExoApp()->GetInGameGui()->GetListenerDistance();
                }
                g_pExoSound->SetListenerPosition(vPosition);
                vFront = qOrientation.ToDirection();
                vFront.x *= -1.0f;
                vFront.y *= -1.0f;
                vFront.z *= -1.0f;
                g_pExoSound->SetListenerOrientation(vFront, qOrientation.GetYAxis());
            }
        }
    }
    g_pExoSound->Render();
}
