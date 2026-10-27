// CSWSObject::StartDialog (0x004CF490).
#include "i/sws_object.h"

class CGuiInGame {
public:
    void SetGlobalDialogState(int bState);          // 0x0062EC60
    int HideContainerGui();                         // 0x0062D510
    void StartGlobalFade(int nType, float fStart, float fEnd,
                         const Vector& vColor);     // 0x0062ABF0
    char pad_0x0[0x188];
    unsigned long m_oidDialogObject;                // 0x188 (placeholder name)
    char pad_0x18c[0xb98 - 0x18c];
    int m_bFadeOutPending;                          // 0xb98 (placeholder name)
};

class CSWCModule {
public:
    void ExitCombatMode();                          // 0x00641DC0
};

extern int g_bCameraResetPending;                   // 0x007A2288

// 0x0c bytes.
struct CSWSDialogPlayer {
    unsigned long m_nPlayerID;                      // 0x00
    unsigned long m_oidObject;                      // 0x04
    unsigned char m_nGender;                        // 0x08
};

// FUNCTION: SWKOTOR 0x004CF490
int CSWSObject::StartDialog(unsigned long oidWith, const CExoString& sDialog, int bFlag)
{
    CGameObject* pGameObject = NULL;
    CSWSClient* pPlayer = g_pAppManager->m_pServerExoApp->GetClientObjectByObjectID(
        g_pAppManager->m_pServerExoApp->GetPlayerCreatureID());
    g_pAppManager->m_pClientExoApp->SetDialogSent(0);
    g_pAppManager->m_pClientExoApp->SetDialogModelsDirty(0);

    if (m_pDialog == NULL) {
        m_n38 = 0;
        m_n3c = 0;
        m_n40 = 0;
        m_n44 = 0;
        m_n48 = 0;
        m_n50 = 0;
        m_n74 = 1;
        m_n60 = 0;
        m_n64 = 0;
        m_n70 = 0;
        m_nCameraAnimationTime = 0;

        CResRef cDialog;
        if (sDialog == "") {
            cDialog = GetDialogResref();
        } else {
            cDialog = sDialog;
        }
        if (cDialog == "") {
            return 0;
        }

        CResGFF* pGFF = new CResGFF(0x7ed, "DLG ", cDialog);
        if (!pGFF->m_bLoaded) {
            delete pGFF;
            return 0;
        }
        m_pDialog = new CSWSDialog();
        if (!m_pDialog->LoadDialog(pGFF, 1)) {
            if (m_pDialog) {
                delete m_pDialog;
            }
            m_pDialog = NULL;
            delete pGFF;
            return 0;
        }
        delete pGFF;

        m_pDialog->m_n90 = bFlag;
        g_pAppManager->m_pClientExoApp->GetInGameGui()->SetGlobalDialogState(1);
        {
            CExoString sName;
            cDialog.CopyToString(sName);
        }
        if (g_pAppManager->m_pClientExoApp->m_pInternal->m_pModuleState->m_nState == 5) {
            g_bCameraResetPending = 1;
            g_pAppManager->m_pClientExoApp->GetModule()->ExitCombatMode();
            g_pAppManager->m_pClientExoApp->SetInputClass(0, 1);
        }

        if (g_pAppManager->m_pServerExoApp->GetObjectArray()->GetGameObject(
                oidWith, &pGameObject) != GAME_OBJECT_VALID) {
            return 0;
        }

        int nGender = g_nDefaultGender;
        CGameObject* pPC = g_pAppManager->m_pServerExoApp->GetGameObject(pPlayer->m_oidPCObject);
        if (pPC && pPC->AsSWSCreature()) {
            nGender = pPC->AsSWSCreature()->GetGender();
        }
        CSWSObject* pWith = (CSWSObject*)pGameObject;
        CSWSDialogPlayer* pDialogPlayer = new CSWSDialogPlayer;
        pDialogPlayer->m_nPlayerID = pPlayer->m_nPlayerID;
        pDialogPlayer->m_oidObject = oidWith;
        pDialogPlayer->m_nGender = nGender;
        m_pDialog->FUN_004cc490(pDialogPlayer);
        m_pDialog->m_oidOwner = oidWith;
        m_pDialog->m_nOwnerGender = nGender;

        pWith->m_oidDialogOwner = m_idSelf;
        if (pWith->AsSWSCreature()) {
            pWith->AsSWSCreature()->ClearActionsForType4F8770(2);
            pWith->AsSWSCreature()->SetActivity(4, 1, 0);
        }
        g_pAppManager->m_pClientExoApp->GetInGameGui()->m_oidDialogObject = m_idSelf;
        m_oidDialogOwner = m_idSelf;
        m_nDialogDelayFlags = 0;
        if (AsSWSCreature()) {
            AsSWSCreature()->ClearActionsForType4F8770(2);
            AsSWSCreature()->SetActivity(4, 1, 0);
        }

        m_pDialog->SetOwnerAILevel(this);
        int bResult = SendDialogEntry(m_pDialog->GetStartEntry(this));
        g_pAppManager->m_pClientExoApp->GetInGameGui()->HideContainerGui();
        if (bResult) {
            g_pAppManager->m_pClientExoApp->ClearEnemySighted();
        } else {
            if (g_pAppManager->m_pClientExoApp->GetInGameGui()->m_bFadeOutPending) {
                g_pAppManager->m_pClientExoApp->GetInGameGui()->m_bFadeOutPending = 0;
                g_pAppManager->m_pClientExoApp->GetInGameGui()->StartGlobalFade(
                    1, 0.0f, 1.0f, Vector(0.0f, 0.0f, 0.0f));
            }
            g_pAppManager->m_pClientExoApp->GetInGameGui()->SetGlobalDialogState(0);
        }
        return bResult;
    }
    if (m_n214) {
        g_pAppManager->m_pClientExoApp->GetInGameGui()->SetGlobalDialogState(0);
    }
    return 1;
}
