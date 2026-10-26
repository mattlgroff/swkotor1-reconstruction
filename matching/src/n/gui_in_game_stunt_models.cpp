// CGuiInGame::UnloadStuntModelsForDialog (guiingame.o).

#include "n/gui_in_game_dialog.h"

// FUNCTION: SWKOTOR 0x0062C6F0
void CGuiInGame::UnloadStuntModelsForDialog()
{
    unsigned long nAnimation;
    for (unsigned long i = 0; i < m_nStuntObjects; i++) {
        CGameObject* pGameObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_pStuntObjects[i]);
        if (!pGameObject)
            continue;
        CSWCObject* pObject = pGameObject->AsSWCObject();
        if (!pObject)
            continue;
        CDialogStuntModel* pStunt1 = pObject->GetModel(0xff, 0);
        if (pStunt1) {
            pStunt1->PlayAnimation(0, 1.0f, 0, 0);
            pStunt1->ResetAnimation();
        }
        CDialogStuntModel* pStunt2 = pObject->GetModel(0xfe, 0);
        if (pStunt2) {
            pStunt2->PlayAnimation(0, 1.0f, 0, 0);
            pStunt2->ResetAnimation();
        }
        CExoString sName;
        if (pObject->AsSWCCreature()) {
            sName = pObject->GetAnimationBase()->GetAnimationName(10000);
            nAnimation = 10000;
        }
        if (pObject->AsSWCPlaceable()) {
            CSWSPlaceable* pServer = pObject->AsSWCPlaceable()->GetServerPlaceable();
            if (pServer) {
                nAnimation = pServer->m_nAnimationDword;
                sName = pObject->AsSWCPlaceable()->GetAnimationBase()->GetAnimationName(nAnimation);
            }
            else {
                sName = pObject->AsSWCPlaceable()->GetAnimationBase()->GetAnimationName(10000);
                nAnimation = 10000;
            }
        }
        if (pStunt1)
            pStunt1->PlayAnimation(sName.CStr(), 1.0f, 2, 0);
        if (pStunt2)
            pStunt2->PlayAnimation(sName.CStr(), 1.0f, 2, 0);
        if (pObject->AsSWCCreature())
            pObject->SetAnimation(nAnimation, 0, 0);
        if (pObject->AsSWCPlaceable())
            pObject->AsSWCPlaceable()->SetAnimation(nAnimation, 0, 0);
    }
    if (m_pStuntObjects) {
        delete[] m_pStuntObjects;
        m_pStuntObjects = 0;
    }
    if (m_pStunt194) {
        delete[] (char*)m_pStunt194;
        m_pStunt194 = 0;
    }
    m_nStuntObjects = 0;
}

// FUNCTION: SWKOTOR 0x006332B0
// NONMATCH: 34 diff insns, register choice/scheduling only (argument vs
// app-pointer load order around GetGameObject/ClientToServerObjectID and the
// owner/speaker store order); the control flow and calls match.
int CGuiInGame::CloseDialog()
{
    if (m_bInitialized) {
        g_pAppManager->m_pClientExoApp->DisableVideoEffect();
        ClearDialogNonDependantList();
        m_bBD8 = 0;
        if (m_oidPartyLeaderAtStart != 0x7f000000) {
            if (!m_pGuiManager->PanelExists(m_pPartySelection)) {
                for (int i = 0; i < g_pAppManager->m_pClientExoApp->GetParty()->m_nCount; i++) {
                    CSWCCreature* pLeader = g_pAppManager->m_pClientExoApp->GetParty()->GetCharacter(0);
                    if (pLeader && g_pAppManager->GetServerExoApp()->ClientToServerObjectID(pLeader->m_idSelf) == m_oidPartyLeaderAtStart)
                        break;
                    g_pAppManager->m_pClientExoApp->ChangeCharacter(-2, 0);
                }
            }
            m_oidPartyLeaderAtStart = 0x7f000000;
        }
        CGameObject* pObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidDialogOwner);
        if (pObject && pObject->AsSWCCreature())
            pObject->AsSWCCreature()->LookAt(0x7f000000, 0.0f);
        pObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidDialogSpeaker);
        if (pObject && pObject->AsSWCCreature())
            pObject->AsSWCCreature()->LookAt(0x7f000000, 0.0f);
        m_oid178 = m_oidDialogOwner;
        m_oidDialogOwner = 0x7f000000;
        m_oid17C = m_oidDialogSpeaker;
        m_oidDialogSpeaker = 0x7f000000;
        ResetDialogAnimations();
        ResetTalkAnimations();
        m_nB0 = 0;
        SetGlobalDialogState(0);
        if (m_pDialog == m_pDialogComputer) {
            CSWGuiDialogPanel* pComputer = (CSWGuiDialogPanel*)m_pDialog;
            if (!g_bGuiFlag) {
                pComputer->m_nFlags = (pComputer->m_nFlags & ~0x200) | 0x500;
            }
            else {
                pComputer->m_nFlags |= 0x100;
                if ((pComputer->m_nFlags & 0x600) != 0x400)
                    pComputer->m_nFlags = (pComputer->m_nFlags & ~0x400) | 0x200;
            }
            m_pDialogComputer = 0;
        }
        else
            m_pGuiManager->RemovePanel(m_pDialog);
        m_pGuiManager->RemovePanel(m_pDialogCamera);
        m_nBB4 = 0;
        if (m_pDialog == m_pDialogNormal) {
            if (!g_bLetterbox) {
                m_pGuiManager->RemovePanel(m_pLetterboxTop);
                m_pGuiManager->RemovePanel(m_pLetterboxBottom);
                m_pGuiManager->RemovePanel(m_pLetterbox);
            }
            m_pLetterboxTop->StopFade();
            m_pLetterboxBottom->StopFade();
            m_pLetterbox->StopFade();
        }
        ((CSWGuiDialogPanel*)m_pDialog)->SetForced();
        m_pDialog = m_pDialogNormal;
        EndDialogCamera();
        UnloadStuntModelsForDialog();
        ClearRepliesData();
        m_nDialogAnimations = 0;
        if (m_pDialogObjects) {
            delete[] m_pDialogObjects;
            m_pDialogObjects = 0;
        }
        if (m_pDialogAnimations) {
            delete[] m_pDialogAnimations;
            m_pDialogAnimations = 0;
        }
        m_nLastDialogParticipants = 0;
        if (m_pLastDialogParticipants) {
            delete[] m_pLastDialogParticipants;
            m_pLastDialogParticipants = 0;
        }
        if (m_pLastDialogAnimations) {
            delete[] m_pLastDialogAnimations;
            m_pLastDialogAnimations = 0;
        }
        m_nB50 = 0;
        m_nB54 = 0;
        if (g_pAppManager->m_pClientExoApp->GetInputClass() == 3) {
            g_pAppManager->m_pClientExoApp->SetInputClass(0, 1);
            g_pExoSound->SetSoundMode(SOUND_MODE_NORMAL);
        }
        g_pAppManager->m_pClientExoApp->ResetDriveAcceleration();
        if (m_bPendingBark) {
            CResRef cSound;
            if (m_bInitialized) {
                m_pBarkBubble->SetBark(0x7f000000, m_sPendingBark, cSound, 0);
                m_pGuiManager->AddPanel(m_pBarkBubble, 4, 1);
            }
            m_bPendingBark = 0;
            m_sPendingBark = "";
        }
        g_pAppManager->m_pClientExoApp->SetLastClickedOnTarget(0x7f000000);
        return m_nB0;
    }
    return 0;
}
