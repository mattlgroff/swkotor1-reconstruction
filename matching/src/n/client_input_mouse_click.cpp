// CClientExoAppInternal::HandleMouseClickInWorld (clientinput.o) and the
// party-member combat-message setter. Standalone layouts.
class CSWCCreature;
class CSWParty { public: CSWCCreature* GetPlayerCharacter(); /* 0x00635460 */ };
class CGameObjectBase { public: int m_nDummy; };
class CGameObjectBase2 { public: int m_nDummy2; };
class CDefaultActionTarget : public CGameObjectBase, public CGameObjectBase2 {};
class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08();
    virtual CDefaultActionTarget* AsDefaultActionTarget(); // +0x0C
};
#pragma pack(push,4)
struct CDefaultAction {
    char pad_0x0[8];
    unsigned long m_nArgument;
    void (CDefaultActionTarget::*m_pfnAction)(unsigned long nArgument,CSWCCreature* pPlayer);
    char pad_0x14[8];
    unsigned long m_oidTarget;
};
#pragma pack(pop)
class CPlayerControl { public: void ShowAction(); /* 0x00689D80 */ };
class CGuiInGame { public: char pad_0x0[0x90]; CPlayerControl* m_pPlayerControl; char pad_0x94[0x20]; int m_bB4; void SetCombatMessage(unsigned long message); /* 0x0062B110 */ };
class CClientExoApp {
public:
    unsigned long GetPlayerCreatureId(); // 0x005ED550
    CGameObject* GetGameObject(unsigned long oid); // 0x005ED580
    CGuiInGame* GetInGameGui(); // 0x005ED690
    CSWParty* GetSWParty(); // 0x005ED8B0
};
class CAppManager { public: void* m_p0; CClientExoApp* m_pClientExoApp; CClientExoApp* GetClientExoApp() { return m_pClientExoApp; } };
extern CAppManager* g_pAppManager; // 0x007A39FC
class CClientOptions { public: char pad_0x0[0x6d]; unsigned char m_nCameraMode; };
class CSWGuiInGameInput { public: char pad_0x0[0x30]; int m_bInputActive; };
class CSWGuiManager { public: void PlayGuiSound(char nSound); /* 0x0040A140 */ };
class CClientExoAppInternal {
public:
    void* m_p0; CClientOptions* m_pClientOptions;
    char pad_0x8[0x38]; CSWGuiInGameInput* m_pGuiInGame;
    char pad_0x44[0x230]; CSWGuiManager* m_pGuiManager;
    char pad_0x278[0x3c]; unsigned long m_oidMouseTarget; unsigned long m_oidMouseDownTarget;
    char pad_0x2bc[0x1e8]; unsigned long m_oidHilite;
    char pad_0x4a8[0x20]; CDefaultAction* m_pDefaultActions; int m_nDefaultActions;
    void SetHilite(unsigned long oid,int b); // 0x005F4A20
    void HandleMouseClickInWorld();
};

// FUNCTION: SWKOTOR 0x00620350
void CClientExoAppInternal::HandleMouseClickInWorld()
{
    if (g_pAppManager->GetClientExoApp()->GetInGameGui()->m_bB4) return;
    if (m_pGuiInGame->m_bInputActive) return;
    if (m_pClientOptions->m_nCameraMode == 5) return;
    if (m_oidMouseDownTarget != 0x7f000000 && m_oidMouseTarget == m_oidHilite && m_oidMouseDownTarget == m_oidMouseTarget) {
        int nActions = m_nDefaultActions;
        m_oidMouseDownTarget = 0x7f000000;
        if (nActions > 0) {
            CDefaultAction* pAction = m_pDefaultActions;
            if (pAction) {
                CSWCCreature* pPlayer = g_pAppManager->GetClientExoApp()->GetSWParty()->GetPlayerCharacter();
                CGameObject* pObject = g_pAppManager->GetClientExoApp()->GetGameObject(pAction->m_oidTarget);
                if (pObject) {
                    CDefaultActionTarget* pTarget = pObject->AsDefaultActionTarget();
                    if (pTarget) {
                        m_pGuiManager->PlayGuiSound(6);
                        (pTarget->*pAction->m_pfnAction)(pAction->m_nArgument,pPlayer);
                    }
                }
            }
        }
        return;
    }
    if (m_oidHilite != 0x7f000000) SetHilite(m_oidHilite,0);
    if (g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pPlayerControl)
        g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pPlayerControl->ShowAction();
}

class CSWPartyMemberData {
public:
    unsigned long m_oidMember; char pad_0x4[0x80]; unsigned long m_nCombatMessage;
    void SetCombatMessage(unsigned long message);
};

// FUNCTION: SWKOTOR 0x006345E0
void CSWPartyMemberData::SetCombatMessage(unsigned long message)
{
    m_nCombatMessage = message;
    if (m_oidMember == g_pAppManager->GetClientExoApp()->GetPlayerCreatureId())
        g_pAppManager->GetClientExoApp()->GetInGameGui()->SetCombatMessage(m_nCombatMessage);
}
