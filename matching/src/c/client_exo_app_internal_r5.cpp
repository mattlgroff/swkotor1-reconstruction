// CClientExoAppInternal members, worker C r5 (V3 queue: object lookups,
// pause/exclusion helpers, small setters).

#include <stdlib.h>
#include "c/client_exo_app.h"

extern "C" __declspec(dllimport) void __stdcall PostQuitMessage(int nExitCode);

extern const unsigned char GAME_OBJECT_VALID;       // 0x0074666C
extern const unsigned char OBJECT_TYPE_AREA;        // 0x00746633
extern const unsigned char OBJECT_TYPE_CREATURE;    // 0x00746634
extern const unsigned char OBJECT_TYPE_ITEM;        // 0x00746635
extern const unsigned char OBJECT_TYPE_TRIGGER;     // 0x00746636
extern const unsigned char OBJECT_TYPE_PROJECTILE;  // 0x00746637
extern const unsigned char OBJECT_TYPE_PLACEABLE;   // 0x00746638
extern const unsigned char OBJECT_TYPE_DOOR;        // 0x00746639
extern const unsigned char OBJECT_TYPE_AREAOFEFFECT;    // 0x0074663A
extern const unsigned char OBJECT_TYPE_SOUND;       // 0x0074663E
extern const int KEY_DISPLAY_FIRST;                 // 0x0074D3D8
extern const int KEY_DISPLAY_LAST;                  // 0x0074D3DC

struct Vector {
    float x, y, z;
    Vector() {}
    Vector(float fx, float fy, float fz) { x = fx; y = fy; z = fz; }
};

class CClientPlayer {
public:
    char pad_0x0[0x4];
    unsigned long m_nPlayerId;                      // 0x04
    char pad_0x8[0x20];
    unsigned long m_oidPlayer;                      // 0x28
};

class CExoStreamingSoundSource {
public:
    ~CExoStreamingSoundSource();                    // 0x005D6200
    void Stop();                                    // 0x005D5B80
};

class CSWGuiHitTester {
public:
    unsigned char HitTest(void* pControl);          // 0x00684ED0
};

class CSWGuiToolTip {
public:
    void SetInstantMode();                          // 0x006856E0
    void ClearInstantMode();                        // 0x00685710

    char pad_0x0[0xbc];
    CSWGuiHitTester m_cHitTester;                   // 0xbc
};

class CSWGuiManagerInput {
public:
    char pad_0x0[0x8];
    void* m_pActiveControl;                         // 0x08
    char pad_0xc[0x10];
    unsigned long m_bUnknown1c_0 : 1;               // 0x1c
    unsigned long m_bUnknown1c_1 : 1;
    unsigned long m_bSoftwareCursor : 1;
    char pad_0x20[0x18];
    int m_nCursor;                                  // 0x38

    void SetSoftwareMouseCursor(int nCursor, int bSoftware, int bDown);   // 0x0040A270
    void HandleLMouseDown(int b);                   // 0x0040C570
};

struct COptionsFlags {
    char pad_0x0[0x8];
    unsigned char m_nFlags;                         // 0x08
};

extern int g_bMouseButtonDown;                      // 0x008338F4
extern int g_bSwapMouseButtons;                     // 0x008338F0

class CSWGuiModalWarningBox {
public:
    void OnPanelAdded();                            // 0x00627260
};

class CGuiInGame {
public:
    char pad_0x0[0x90];
    CSWGuiToolTip* m_pToolTip;                      // 0x90
    char pad_0x94[0x4];
    CSWGuiModalWarningBox* m_pDeathPanel;           // 0x98

    void Initialize();                              // 0x00632860
    void IndicateObject(unsigned long oid);         // 0x0062B000
    void StartGlobalFade(int a, int b, float fTime, Vector* pColor);   // 0x0062ABF0
    void StartGlobalFade(int nType, float fTime, float fAlpha, const Vector& vColor);   // 0x0062ABF0
    int CanShowTutorialWindow(int n);               // 0x0062F420
    int TestTutorialWindow(int n);                  // 0x0062B0D0
    void AddGoldMessage(int b, int nDelta);         // 0x0062EEB0
};

class CResRef {
public:
    char m_resRef[16];
    CResRef(const char* s);                         // 0x00406D80
};

class CSWGuiDisableable {
public:
    void SetDisabled(int b);                        // 0x00417FB0
};

class CSWGuiPanelR5 {
public:
    char pad_0x0[0x64];
    CSWGuiDisableable m_cControl;                   // 0x64

    void Destroy();                                 // 0x0067AA80
    void SetBackground(const CResRef& cRes);        // 0x0040A870
};

void PlayDelayedSound(float fTime, int a, int b);   // 0x00401C10

struct CRoomEnvironment {
    char pad_0x0[0x30];
    int m_nEnvironment;                             // 0x30
    float m_fEnvironmentScale;                      // 0x34
    float GetEnvironmentScale() { return m_fEnvironmentScale; }
    char pad_0x38[0xc];
    int m_nRoomId;                                  // 0x44
};

class CRoomModel {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78();
    virtual void SetRoom(int nRoom);                // 0x7c
    virtual void v80();
    virtual int IsBusy();                           // 0x84
};

class CSWCAreaRooms {
public:
    char pad_0x0[0x184];
    CRoomModel* m_pRoomModel;                       // 0x184

    int GetRoomIndex(int nRoom, int b);             // 0x00605610
    CRoomEnvironment* GetRoom(int nIndex);          // 0x006054F0
};

class CExoSoundEnv : public CExoSound {
public:
    void SetEnvironment(int n);                     // 0x005D6470
    void SetEnvironmentScale(float f);              // 0x005D6480
};

struct CCreatureAnimBase {
    char pad_0x0[0xc5];
    unsigned char m_nWeaponStyle;                   // 0xc5
};

class CSWCCreatureAnim : public CSWCCreature {
public:
    CCreatureAnimBase* GetAnimBase();               // 0x0063C070
    int GetRangedAnimation(int nAnim, unsigned char nStyle);   // 0x00614110
    int InCreatureCombat(CSWCCreature* pTarget);    // 0x0060D3E0
    int ResolveAttackIndex(int nAttack, int nCombat, int n);  // 0x0060D0E0
    int GetMeleeAnimation(int nAttack, int nAnim, int nCombat, int nStyle);  // 0x00613DA0

    char pad_0x3e0[0x8];
    int m_nClientAnimation;                         // 0x3e8
};

struct CSWSCombatData {
    char pad_0x0[0x34];
    unsigned long m_oidTarget;                      // 0x34
};

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
    virtual int GetDead();                          // 0x94

    char pad_0x4[0x33c];
    CSWSCombatData* m_pCombatData;                  // 0x340

    int GetGold();                                  // 0x004EDD60
    void SetGold(int n);                            // 0x004EDD90
    void ClearActions(int n);                       // 0x004F76C0
    int GetDeadTemp();                              // 0x004EF890
};

class CSWCPartyCreature : public CSWCCreature {
public:
    CSWSCreature* GetServerCreature();              // 0x0060FB20
};

struct CPartyCreatureData {
    char pad_0x0[0x24];
    Vector m_vPosition;                             // 0x24
    Vector m_vOrientation;                          // 0x30
};

class CSWPartyR5 {
public:
    CSWCPartyCreature* GetPC();                     // 0x00635460
    void SetPC(int nIndex);                         // 0x00635480
    void JumpTo(Vector* pPos, float fFacing, int b);    // 0x00637890
};

float GetFacing(Vector* pOrientation);              // 0x004AA0F0

class CVirtualMachine {
public:
    int RunScript(CExoString* pScript, unsigned long oid, int b);  // 0x005D0FC0
};

class CRender {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20();
    virtual void SetGamma(float f);                 // 0x24
};

extern CVirtualMachine* g_pVirtualMachine;          // 0x007A3A00
extern CRender* g_pRender;                          // 0x007A39F8
extern const int MUSIC_STATE_NORMAL;                // 0x0074C5EC

class CSWGuiManagerMouse {
public:
    void ShowMouse();                               // 0x0040B060
    void HideMouse();                               // 0x0040A230
};

class CExoInput {
public:
    void SetMouseMode(unsigned char nMode);         // 0x005DF5A0
    CExoString GetDisplayCharacterForKey(int nKey); // 0x005DF640
    void SetActive(int b);                          // 0x005DF540
    void RefreshMouseCursor(int b);                 // 0x005DF5B0

};

class CSWGuiRefreshable {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28();
    virtual void Refresh();                         // 0x2c
};

class CTlkTable {
public:
    CExoString GetSimpleString(unsigned long nStrRef);  // 0x0041E8F0
};

struct CToolTipGlobals {
    char pad_0x0[0x48];
    float m_fDelay;                                 // 0x48

    int IsMouseOverGui();                           // 0x0040A170
};

class CSWCCreatureSound : public CSWCCreature {
public:
    void PlaySoundSet(int nSound, int b);           // 0x00611470
};

class CSWPartyClient {
public:
    void SetPlayerCreature(unsigned long oid);      // 0x006364C0
};

extern CExoInput* g_pExoInput;                      // 0x007A39E4
extern CTlkTable* g_pTlkTable;                      // 0x007A3A08
extern CToolTipGlobals* g_pToolTipGlobals;          // 0x007A39F4

// FUNCTION: SWKOTOR 0x005EE8F0
int CClientExoAppInternal::GetPauseState(unsigned char nType)
{
    return m_nPauseFlags & nType;
}

// FUNCTION: SWKOTOR 0x005EEA10
void CClientExoAppInternal::ExitProgram()
{
    PostQuitMessage(0);
}

// FUNCTION: SWKOTOR 0x005EEAD0
void CClientExoAppInternal::ServerModeChange(unsigned char nMode)
{
    m_nServerMode = nMode;
}

// FUNCTION: SWKOTOR 0x005F10F0
CGameObject* CClientExoAppInternal::GetGameObject(unsigned long oid)
{
    CGameObject* pObject = 0;
    if (m_pGameObjectArray->GetGameObject(oid, &pObject) == GAME_OBJECT_VALID) {
        return pObject;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005F1130
CSWCItem* CClientExoAppInternal::GetItemByGameObjectID(unsigned long oid)
{
    CGameObject* pObject = 0;
    if (m_pGameObjectArray->GetGameObject(oid, &pObject) == GAME_OBJECT_VALID
        && pObject && pObject->m_nObjectType == OBJECT_TYPE_ITEM) {
        return (CSWCItem*)pObject->GetItemModel();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005F1180
CSWCCreature* CClientExoAppInternal::GetCreatureByGameObjectID(unsigned long oid)
{
    CGameObject* pObject = 0;
    if (m_pGameObjectArray->GetGameObject(oid, &pObject) == GAME_OBJECT_VALID
        && pObject && pObject->m_nObjectType == OBJECT_TYPE_CREATURE) {
        return pObject->AsSWCCreature();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005F11D0
CSWCArea* CClientExoAppInternal::GetAreaByGameObjectID(unsigned long oid)
{
    CGameObject* pObject = 0;
    if (m_pGameObjectArray->GetGameObject(oid, &pObject) == GAME_OBJECT_VALID
        && pObject && pObject->m_nObjectType == OBJECT_TYPE_AREA) {
        return pObject->AsSWCArea();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005F1220
CSWCTrigger* CClientExoAppInternal::GetTriggerByGameObjectID(unsigned long oid)
{
    CGameObject* pObject = 0;
    if (m_pGameObjectArray->GetGameObject(oid, &pObject) == GAME_OBJECT_VALID
        && pObject && pObject->m_nObjectType == OBJECT_TYPE_TRIGGER) {
        return ((CSWCObject*)pObject)->AsSWCTrigger();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005F1270
CSWCProjectile* CClientExoAppInternal::GetProjectileByGameObjectID(unsigned long oid)
{
    CGameObject* pObject = 0;
    if (m_pGameObjectArray->GetGameObject(oid, &pObject) == GAME_OBJECT_VALID
        && pObject && pObject->m_nObjectType == OBJECT_TYPE_PROJECTILE) {
        return ((CSWCObject*)pObject)->AsSWCProjectile();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005F12C0
CSWCPlaceable* CClientExoAppInternal::GetPlaceableByGameObjectID(unsigned long oid)
{
    CGameObject* pObject = 0;
    if (m_pGameObjectArray->GetGameObject(oid, &pObject) == GAME_OBJECT_VALID
        && pObject && pObject->m_nObjectType == OBJECT_TYPE_PLACEABLE) {
        return ((CSWCObject*)pObject)->AsSWCPlaceable();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005F1310
CSWCDoor* CClientExoAppInternal::GetDoorByGameObjectID(unsigned long oid)
{
    CGameObject* pObject = 0;
    if (m_pGameObjectArray->GetGameObject(oid, &pObject) == GAME_OBJECT_VALID
        && pObject && pObject->m_nObjectType == OBJECT_TYPE_DOOR) {
        return pObject->AsSWCDoor();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005F1360
CSWCAreaOfEffectObject* CClientExoAppInternal::GetAreaOfEffectByGameObjectID(unsigned long oid)
{
    CGameObject* pObject = 0;
    if (m_pGameObjectArray->GetGameObject(oid, &pObject) == GAME_OBJECT_VALID
        && pObject && pObject->m_nObjectType == OBJECT_TYPE_AREAOFEFFECT) {
        return ((CSWCObject*)pObject)->AsSWCAreaOfEffectObject();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005F13B0
CSWCSoundObject* CClientExoAppInternal::GetSoundObjectByGameObjectID(unsigned long oid)
{
    CGameObject* pObject = 0;
    if (m_pGameObjectArray->GetGameObject(oid, &pObject) == GAME_OBJECT_VALID
        && pObject && pObject->m_nObjectType == OBJECT_TYPE_SOUND) {
        return ((CSWCObject*)pObject)->AsSWCSoundObject();
    }
    return 0;
}


// FUNCTION: SWKOTOR 0x005F1410
void CClientExoAppInternal::InitializeInGameGUI()
{
    m_pGuiInGame->Initialize();
}

// FUNCTION: SWKOTOR 0x005F2B00
void CClientExoAppInternal::SetPlayerCharacterName(const CExoString& sName)
{
    m_sPlayerCharacterName = sName;
    m_nPlayerCharacterSet = 1;
}

// FUNCTION: SWKOTOR 0x005F2B50
int CClientExoAppInternal::IsPlayerCreature(CGameObject* pObject)
{
    if (!pObject) {
        return 0;
    }
    if (m_oidPlayer != OBJECT_INVALID) {
        return pObject->m_idSelf == m_oidPlayer;
    }
    unsigned long oidServer = g_pAppManager->m_pServerExoApp->GetPlayerCreatureID();
    return pObject->m_idSelf == g_pAppManager->GetClientExoApp()->ServerToClientObjectId(oidServer);
}

// FUNCTION: SWKOTOR 0x005F2BB0
void CClientExoAppInternal::SetPlayerCreature(unsigned long oid)
{
    m_oidPlayer = oid;
    if (m_pParty) {
        ((CSWPartyClient*)m_pParty)->SetPlayerCreature(oid);
    }
}

// FUNCTION: SWKOTOR 0x005F2C60
void CClientExoAppInternal::IndicateObject(CGameObject* pObject)
{
    m_pGuiInGame->IndicateObject(pObject ? pObject->m_idSelf : OBJECT_INVALID);
}

// FUNCTION: SWKOTOR 0x005F2F20
void CClientExoAppInternal::HurryDeathSequence()
{
    Vector vColor(0.0f, 0.0f, 0.0f);
    m_pGuiInGame->StartGlobalFade(0, 0, 0.5f, &vColor);
}

// FUNCTION: SWKOTOR 0x005F2F60
void CClientExoAppInternal::SetGameSpeed(float fSpeed)
{
    if (g_pAppManager) {
        if (g_pAppManager->m_pClientExoApp) {
            g_pAppManager->m_pClientExoApp->GetWorldTimer()->SetSnapshotScale(fSpeed);
            g_pAppManager->m_pClientExoApp->GetPauseTimer()->SetSnapshotScale(fSpeed);
            g_pAppManager->m_pClientExoApp->GetAnimationTimer()->SetSnapshotScale(fSpeed);
        }
        if (g_pAppManager->GetServerExoApp()) {
            g_pAppManager->GetServerExoApp()->GetWorldTimer()->SetSnapshotScale(fSpeed);
            g_pAppManager->GetServerExoApp()->GetPauseTimer()->SetSnapshotScale(fSpeed);
        }
    }
}

// FUNCTION: SWKOTOR 0x005F3030
void CClientExoAppInternal::SetCanSendDialog(int b)
{
    m_bCanSendDialog = b;
    m_nCanSendDialogTime = (int)(g_pAppManager->m_pClientExoApp->m_pcInternal->m_pPauseTimer->GetSnapshotTime() / 1000);
}

// FUNCTION: SWKOTOR 0x005F4120
int CClientExoAppInternal::ShowTutorialWindow(int nWindow, int a, int b, int c)
{
    if (!m_pGuiInGame->CanShowTutorialWindow(nWindow)) {
        return 0;
    }
    if (!m_pGuiInGame->TestTutorialWindow(nWindow)) {
        return 0;
    }
    m_nTutorialWindowA = a;
    m_nTutorialWindow = (unsigned char)nWindow;
    m_nTutorialWindowB = b;
    m_nTutorialWindowC = c;
    return 1;
}

// FUNCTION: SWKOTOR 0x005F49B0
void CClientExoAppInternal::SetMouseMode(unsigned char nMode)
{
    SetInputState(4);
    g_pExoInput->SetMouseMode(nMode);
    if (m_pGuiManager) {
        if (nMode == 1) {
            ((CSWGuiManagerMouse*)m_pGuiManager)->ShowMouse();
        } else {
            ((CSWGuiManagerMouse*)m_pGuiManager)->HideMouse();
        }
    }
    ClearInputState(4);
}

// FUNCTION: SWKOTOR 0x005F4BE0
void CClientExoAppInternal::SetToolTipInstantMode(int bInstant)
{
    if (m_pGuiInGame) {
        if (bInstant) {
            g_pToolTipGlobals->m_fDelay = 0.0f;
            if (m_pGuiInGame->m_pToolTip) {
                m_pGuiInGame->m_pToolTip->SetInstantMode();
            }
        } else {
            g_pToolTipGlobals->m_fDelay = g_pAppManager->m_pClientExoApp->GetUnknown700()->GetToolTipDelay();
            if (m_pGuiInGame->m_pToolTip) {
                m_pGuiInGame->m_pToolTip->ClearInstantMode();
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x005F4D60
void CClientExoAppInternal::PlayCharacterSelectSound(CSWCCreature* pCreature)
{
    int n = rand() % 3;
    if (n == 0) {
        ((CSWCCreatureSound*)pCreature)->PlaySoundSet(7, 1);
    } else if (n == 1) {
        ((CSWCCreatureSound*)pCreature)->PlaySoundSet(8, 1);
    } else {
        ((CSWCCreatureSound*)pCreature)->PlaySoundSet(9, 1);
    }
}

// FUNCTION: SWKOTOR 0x005F4DF0
CExoString CClientExoAppInternal::GetDisplayCharacterForKey(int nKey)
{
    if (nKey >= KEY_DISPLAY_FIRST && nKey < KEY_DISPLAY_LAST) {
        return g_pExoInput->GetDisplayCharacterForKey(nKey);
    }
    return g_pTlkTable->GetSimpleString(0xbc10);
}

// FUNCTION: SWKOTOR 0x005F4E50
void CClientExoAppInternal::SetEvilEndingHackInProgress(int b)
{
    m_bEvilEndingHackInProgress = b;
    if (!b && m_pEvilEndingSound) {
        m_pEvilEndingSound->Stop();
        delete m_pEvilEndingSound;
        m_pEvilEndingSound = 0;
    }
}

static inline unsigned long ServerToClientId(unsigned long oid)
{
    if (oid != OBJECT_INVALID) {
        return oid | 0x80000000;
    }
    return OBJECT_INVALID;
}

inline CSWCCreature* CClientExoAppInternal::LookUpCreature(unsigned long oid)
{
    CGameObject* pObject = 0;
    return (m_pGameObjectArray->GetGameObject(oid, &pObject) == GAME_OBJECT_VALID
        && pObject && pObject->m_nObjectType == OBJECT_TYPE_CREATURE) ? pObject->AsSWCCreature() : 0;
}

// FUNCTION: SWKOTOR 0x005F4EC0

int CClientExoAppInternal::DontUseThisSetPlayerCreature(unsigned long oid)
{
    // NONMATCH (7 insns): register allocation around the inlined creature lookup
    // (original loads the array before `lea edx, &pObject` and tests the type in al).
    CSWCCreature* pCreature = LookUpCreature(ServerToClientId(oid));
    if (pCreature) {
        m_oidCachedPlayer = oid;
        m_oidPlayer = oid;
        m_pCachedPlayerCreature = pCreature;
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005F56B0
int CClientExoAppInternal::IsOnExclusionList(unsigned long oid)
{
    // NONMATCH (4 insns): GetNext loads the list into edx and builds &pos in ecx.
    if (!m_pPauseExclusionList) {
        return 1;
    }
    CExoLinkedListPosition pos = m_pPauseExclusionList->GetHeadPos();
    while (pos) {
        if ((unsigned long)m_pPauseExclusionList->m_pcExoLinkedListInternal->GetAtPos(pos) == oid) {
            return 1;
        }
        m_pPauseExclusionList->m_pcExoLinkedListInternal->GetNext(pos);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005F5C70
CClientPlayer* CClientExoAppInternal::GetPlayerByPlayerID(unsigned long nPlayerId)
{
    // NONMATCH (jump target only): the original does not thread the empty-list
    // `je` past the second `test eax, eax`.
    CExoLinkedListPosition pos = m_lstPlayerList.GetHeadPos();
    CClientPlayer* pPlayer = m_lstPlayerList.GetHead();
    while (pPlayer) {
        if (pPlayer->m_nPlayerId == nPlayerId) {
            return pPlayer;
        }
        pPlayer = m_lstPlayerList.GetNext(pos);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005F5CB0
CClientPlayer* CClientExoAppInternal::GetPlayerByGameObjectID(unsigned long oid)
{
    // NONMATCH (jump target only), see GetPlayerByPlayerID.
    CExoLinkedListPosition pos = m_lstPlayerList.GetHeadPos();
    CClientPlayer* pPlayer = m_lstPlayerList.GetHead();
    while (pPlayer) {
        if (pPlayer->m_oidPlayer == oid) {
            return pPlayer;
        }
        pPlayer = m_lstPlayerList.GetNext(pos);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005F74B0
int CClientExoAppInternal::StopRumblePattern(int nPattern)
{
    int n = m_lstActiveRumbles.num;
    for (int i = 0; i < n; i++) {
        if (m_lstActiveRumbles[i].m_nPattern == nPattern) {
            m_lstActiveRumbles.RemoveIndex(i);
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x005F7DC0
CWorldTimer* CClientExoAppInternal::GetActiveTimer(unsigned long oid)
{
    if (oid != OBJECT_INVALID && ((m_nPauseFlags & 2) || (m_nPauseFlags & 1))) {
        // NONMATCH (4 insns): original tests bit 0 with `and al, 1; test al, al`
        // and places `return 0` after the pause-timer block.
        if (m_nPauseFlags & 2) {
            if (IsOnExclusionList(oid) == 1) {
                return m_pPauseTimer;
            }
        } else if (m_nPauseFlags & 1) {
            if (IsOnExclusionList(oid) == 1) {
                return m_pExclusionTimer;
            }
        } else {
            return 0;
        }
    }
    return m_pWorldTimer;

}

// FUNCTION: SWKOTOR 0x005FBF50
void CClientExoAppInternal::AddMovieToMovieQueue(int nMovie, int nFlags)
{
    m_lstQueuedMovieFlags.Add(nFlags);
    LoadMovieList(nMovie, &m_lstQueuedMovies);
}

// FUNCTION: SWKOTOR 0x00602CF0
int CClientExoAppInternal::PlayMovie(int nMovie, int b, int c, int d)
{
    if (m_bMovieQueueActive) {
        return 0;
    }
    if (!LoadMovieList(nMovie, &m_lstMoveToModuleMovies)) {
        return 0;
    }
    return PlayMovieList(&m_lstMoveToModuleMovies, b, 1, 0, 0);
}

// FUNCTION: SWKOTOR 0x005EE860
void CClientExoAppInternal::UpdateRoom(int nRoom)
{
    if (!m_pModule) {
        return;
    }
    CSWCAreaRooms* pArea = (CSWCAreaRooms*)m_pModule->m_pPlayer;
    if (!pArea) {
        return;
    }
    CRoomModel* pModel = pArea->m_pRoomModel;
    if (!pModel) {
        return;
    }
    if (pModel->IsBusy()) {
        return;
    }
    int nIndex = pArea->GetRoomIndex(nRoom, 0);
    CRoomEnvironment* pEnv = pArea->GetRoom(nIndex);
    if (nIndex < 0) {
        g_pAppManager->m_pClientExoApp->GetInGameGui();
        return;
    }
    pModel->SetRoom(pEnv->m_nRoomId);
    ((CExoSoundEnv*)g_pExoSound)->SetEnvironment(pEnv->m_nEnvironment);
    ((CExoSoundEnv*)g_pExoSound)->SetEnvironmentScale(pEnv->GetEnvironmentScale());
}

// FUNCTION: SWKOTOR 0x005F32E0
int CClientExoAppInternal::GetClientMeleeAnimation(unsigned long oidAttacker, unsigned long oidTarget,
                                                   int nAttack, int nAnim, int bSet, int nValue)
{
    // NONMATCH (18 insns): same lookup register allocation as DontUseThisSetPlayerCreature,
    // and the final call loads nAnim into ecx / the style into eax.
    CSWCCreatureAnim* pCreature = (CSWCCreatureAnim*)LookUpCreature(ServerToClientId(oidAttacker));
    CSWCCreature* pTarget = LookUpCreature(ServerToClientId(oidTarget));
    int nResult = 0;
    if (pCreature) {
        if (bSet) {
            return pCreature->m_nClientAnimation = nValue;
        }
        unsigned char nStyle = pCreature->GetAnimBase()->m_nWeaponStyle;
        int nCombat = pCreature->InCreatureCombat(pTarget);
        pCreature->ResolveAttackIndex(nAttack, nCombat, -1);
        nResult = pCreature->GetMeleeAnimation(nAttack, nAnim, nCombat, nStyle);
    }
    return nResult;
}

// FUNCTION: SWKOTOR 0x005F33F0
int CClientExoAppInternal::GetClientRangedAnimation(unsigned long oid, unsigned long nAnim, int bSet, int nValue)
{
    CSWCCreatureAnim* pCreature = (CSWCCreatureAnim*)LookUpCreature(ServerToClientId(oid));
    int nResult = 0;
    if (pCreature) {
        if (bSet) {
            return pCreature->m_nClientAnimation = nValue;
        }
        nResult = pCreature->GetRangedAnimation(nAnim, pCreature->GetAnimBase()->m_nWeaponStyle);
    }
    return nResult;
}

// FUNCTION: SWKOTOR 0x005F3950
void CClientExoAppInternal::EndPazaakGame(int bWon, int nWager, int bRun, int nExtra)
{
    // NONMATCH (2 insns): the original loads pPC->m_idSelf before g_pAppManager.
    m_bPazaakWon = bWon;
    m_nPazaakWager = nWager;
    SetInputClass(0, 1);
    m_bPazaakIsRunning = 0;
    CSWCPartyCreature* pPC = ((CSWPartyR5*)g_pAppManager->GetClientExoApp()->GetParty())->GetPC();
    CSWSCreature* pCreature = g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(
        g_pAppManager->GetServerExoApp()->ClientToServerObjectID(pPC->m_idSelf));
    if (pCreature && nWager > 0) {
        int nGold = pCreature->GetGold();
        if (!bWon) {
            int nLeft = nGold - nWager;
            nWager = nLeft <= 0 ? 0 : nLeft;
        } else {
            nWager += nGold;
        }
        if (nWager != nGold) {
            pCreature->SetGold(nWager);
            g_pAppManager->m_pClientExoApp->GetInGameGui()->AddGoldMessage(1, nWager - nGold);
        }

    }
    if (bRun) {
        StartPazaakGame(m_nPazaakDeck, (int)&m_sPazaakScript, 0, 1, nExtra);
        return;
    }
    if (g_pVirtualMachine && m_sPazaakScript.IsNotEmpty(0)) {
        g_pVirtualMachine->RunScript(&m_sPazaakScript, OBJECT_INVALID, 0);
    }
    m_pGuiManager->PlayGuiSound(5);
    g_pExoSound->SetMusicState(MUSIC_STATE_NORMAL);
}

// FUNCTION: SWKOTOR 0x005F4710
int CClientExoAppInternal::LoadOptions()
{
    if (!m_pClientOptions->IsLoaded()) {
        m_pClientOptions->SetDefaults();
    }
    g_pRender->SetGamma(g_pAppManager->GetClientExoApp()->GetUnknown700()->GetGamma(0));
    return 1;
}

// FUNCTION: SWKOTOR 0x005F6B60
void CClientExoAppInternal::ChangeCharacter(int nIndex, int bSound)
{
    CSWCPartyCreature* pOld = ((CSWPartyR5*)m_pParty)->GetPC();
    ((CSWPartyR5*)m_pParty)->SetPC(nIndex);
    if (pOld == ((CSWPartyR5*)m_pParty)->GetPC()) {

        return;
    }
    CSWCPartyCreature* pPC = ((CSWPartyR5*)m_pParty)->GetPC();
    CPartyCreatureData* pData = (CPartyCreatureData*)pPC;
    Vector vPos = pData->m_vPosition;
    ((CSWPartyR5*)m_pParty)->JumpTo(&vPos, GetFacing(&pData->m_vOrientation), 1);
    CSWSCreature* pServer = pPC->GetServerCreature();
    if (pServer) {
        pServer->m_pCombatData->m_oidTarget = OBJECT_INVALID;
        pServer->ClearActions(0x3d);
        if (pServer->GetDead() || pServer->GetDeadTemp()) {
            bSound = 0;
        }
    }
    m_pModule->SetMapCharacter(pPC->m_idSelf, 1);
    if (bSound) {
        PlayCharacterSelectSound(pPC);
    }
    if (m_pCharacterPanel) {
        ((CSWGuiRefreshable*)m_pCharacterPanel)->Refresh();
    }
    m_bCharacterChangeInProgress = 1;
}

// FUNCTION: SWKOTOR 0x005F6E20
void CClientExoAppInternal::HideLoadScreen(int bFade, int bNotify)
{
    ClearInputState(2);
    if (!m_pLoadScreen || !m_pGuiManager) {
        return;
    }
    if (bFade) {
        Vector vColor(0.0f, 0.0f, 0.0f);
        m_pGuiInGame->StartGlobalFade(0, 0, 0.0f, &vColor);
    }
    m_pGuiManager->PopModalPanel();
    if (m_pGuiManager->RemovePanel(m_pLoadScreen)) {
        ((CSWGuiPanelR5*)m_pLoadScreen)->Destroy();
    }
    ((CSWGuiPanelR5*)m_pLoadScreen)->m_cControl.SetDisabled(0);
    {
        CResRef cBackground("");
        ((CSWGuiPanelR5*)m_pLoadScreen)->SetBackground(cBackground);
    }
    if (g_pExoInput) {
        g_pExoInput->SetActive(1);
    }
    ClearInputState(0);
    StopLoadMusic();
    if (!bFade && bNotify) {
        PlayDelayedSound(0.033333335f, 0, 0);
    }
}

// FUNCTION: SWKOTOR 0x0061F880
void CClientExoAppInternal::PerformLButtonDownAction(int bDouble)
{
    // NONMATCH (19 insns): esi/edi swapped (original keeps this in edi, bSwap in esi).
    g_bMouseButtonDown = 1;
    if (g_pExoInput) {
        g_pExoInput->RefreshMouseCursor(1);
    }
    int bSwap;
    CSWGuiManagerInput* pGui = (CSWGuiManagerInput*)m_pGuiManager;
    if (pGui) {
        pGui->SetSoftwareMouseCursor(pGui->m_nCursor, pGui->m_bSoftwareCursor, 1);
    }
    bSwap = g_bSwapMouseButtons;
    if ((((COptionsFlags*)g_pAppManager->m_pClientExoApp->GetUnknown700())->m_nFlags & 2)
        && (int)m_pInputClass != 2 && (int)m_pInputClass != 3) {
        bSwap = !bSwap;
    }
    if ((!bSwap && (int)m_pInputClass != 4) || (int)m_pInputClass == 2 || (int)m_pInputClass == 3) {
        if (!bDouble) {
            m_oidLastClickedOnTarget = m_oidLastTarget;
        }
        ((CSWGuiManagerInput*)m_pGuiManager)->HandleLMouseDown(bDouble);
    }
}

// FUNCTION: SWKOTOR 0x00620530
void CClientExoAppInternal::PerformLButtonUpAction()
{
    // NONMATCH (21 insns): the original keeps two tail `jmp HandleMouseClickInWorld`
    // blocks (ours cross-jumps them) and loads the tooltip before pushing the control.
    g_bMouseButtonDown = 0;
    if (g_pExoInput) {
        g_pExoInput->RefreshMouseCursor(0);
    }
    CSWGuiManagerInput* pGui = (CSWGuiManagerInput*)m_pGuiManager;
    if (pGui) {
        pGui->SetSoftwareMouseCursor(pGui->m_nCursor, pGui->m_bSoftwareCursor, 0);
    }
    int bSwap = g_bSwapMouseButtons;
    if ((((COptionsFlags*)g_pAppManager->m_pClientExoApp->GetUnknown700())->m_nFlags & 2)
        && (int)m_pInputClass != 2 && (int)m_pInputClass != 3) {
        bSwap = !bSwap;
    }
    if ((!bSwap && (int)m_pInputClass != 4) || (int)m_pInputClass == 2 || (int)m_pInputClass == 3) {
        if (((CSWGuiManagerInput*)m_pGuiManager)->m_pActiveControl
            && m_pGuiInGame->m_pToolTip->m_cHitTester.HitTest(
                   ((CSWGuiManagerInput*)m_pGuiManager)->m_pActiveControl) == 1) {
            if (!g_pAppManager->m_pServerExoApp) {
                return;
            }
            HandleMouseClickInWorld();
            return;
        }
        if (g_pToolTipGlobals->IsMouseOverGui()) {
            return;
        }
        if (g_pAppManager->GetServerExoApp()) {
            HandleMouseClickInWorld();
        }
    }
}

// Worker C s1 additions.

class CSWCameraModeDeath {
public:
    CSWCameraModeDeath(void* pOwner, CGob* pTarget);    // 0x0063BBD0 (descriptive)
    char pad_0x0[0x40];
};

class CSWCModuleCamera {
public:
    virtual void v00();
    virtual void* GetOwner();                       // 0x04 (descriptive)
    virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78();
    virtual void SetCameraMode(CSWCameraModeDeath* pMode);  // 0x7c (descriptive)
};

// FUNCTION: SWKOTOR 0x005F7200
void CClientExoAppInternal::RunDeathSequence()
{
    // NONMATCH (12 insns): same inlined creature lookup tie as
    // DontUseThisSetPlayerCreature (array load before &pObject, type in al).
    m_bGameOver = 1;
    m_bGameOverSet = 1;
    m_pGuiManager->HideAllPanels();
    m_pGuiInGame->m_pDeathPanel->OnPanelAdded();
    CSWCCreature* pCreature = LookUpCreature(m_oidPortraitCreature);
    if (m_pModule && pCreature) {
        CSWCModuleCamera* pCamera = (CSWCModuleCamera*)m_pModule->m_pScene;
        if (pCamera) {
            pCamera->SetCameraMode(new CSWCameraModeDeath(pCamera->GetOwner(), pCreature->vD0()));
        }
    }
    Vector vColor(0.0f, 0.0f, 0.0f);
    m_pGuiInGame->StartGlobalFade(0, 12.0f, 1.0f, vColor);
}
