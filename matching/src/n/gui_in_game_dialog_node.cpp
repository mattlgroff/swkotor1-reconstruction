// CGuiInGame dialog node clean-up, reply text, dialog sound and participant
// animation reset (guiingame.o).
#include "n/gui_in_game_dialog.h"

// Descriptive name: end-of-node clean-up without clearing the replies.
// FUNCTION: SWKOTOR 0x0062D8C0
void CGuiInGame::CleanUpDialogNode()
{
    m_vE8 = Vector(0.0f, 0.0f, 0.0f);
    m_nE4 = 0;
    m_nF4 = 0;
    m_pLetterbox->ResetFade();
    if (m_pDialog == 0 || m_pDialog != m_pDialogNormal) {
        if (m_pGuiManager->PanelExists(m_pDialogCamera)) {
            m_pGuiManager->RemovePanel(m_pDialogCamera);
            m_pGuiManager->AddPanel(m_pDialog, 2, 1);
            g_pAppManager->m_pClientExoApp->DisableVideoEffect();
        }
    }
    m_fAnimatedCameraTime = 0.0f;
    g_pExoInput->CoolDownEvent(0x43, 750);
    g_pExoInput->CoolDownEvent(0xB5, 750);
}

// Descriptive name: the same clean-up, also clearing the reply data.
// FUNCTION: SWKOTOR 0x0062D990
void CGuiInGame::CleanUpDialogReplies()
{
    m_vE8 = Vector(0.0f, 0.0f, 0.0f);
    m_nE4 = 0;
    m_nF4 = 0;
    m_pLetterbox->ResetFade();
    ClearRepliesData();
    if (m_pDialog == 0 || m_pDialog != m_pDialogNormal) {
        if (m_pGuiManager->PanelExists(m_pDialogCamera)) {
            m_pGuiManager->RemovePanel(m_pDialogCamera);
            m_pGuiManager->AddPanel(m_pDialog, 2, 1);
            g_pAppManager->m_pClientExoApp->DisableVideoEffect();
        }
    }
    m_fAnimatedCameraTime = 0.0f;
    g_pExoInput->CoolDownEvent(0x43, 750);
    g_pExoInput->CoolDownEvent(0xB5, 750);
}

// The parsed copy is discarded, as in the original.
// FUNCTION: SWKOTOR 0x0062E100
int CGuiInGame::HandleDialogReplyChosen(unsigned long nArg1, unsigned long nArg2, unsigned long nArg3,
                                        const CExoString& sText, unsigned long nArg5,
                                        unsigned char nArg6, int nArg7)
{
    CExoString sParsed(sText);
    if (!sText.IsEmpty()) {
        CGameObject* pObject = g_pAppManager->m_pClientExoApp->GetGameObject(
            g_pAppManager->m_pClientExoApp->ServerToClientObjectId(
                g_pAppManager->m_pServerExoApp->GetClientObjectByObjectId(
                    g_pAppManager->m_pServerExoApp->GetPlayerCreatureId())->m_oidPC));
        CTlkTable* pTlk = g_pTlkTableAlt;
        pTlk->m_pTokenObject = pObject;
        pTlk->m_nGender = pTlk->GetGender();
        pTlk->m_nFlags &= ~1;
        pTlk->ParseTokens(&sParsed);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0062E1D0
int CGuiInGame::HandleDialogSound(unsigned long oid, const CResRef& rSound)
{
    CGameObject* pObject = g_pAppManager->m_pClientExoApp->GetGameObject(m_oidDialogOwner);
    if (pObject == 0 || pObject->AsSWCObject() == 0)
        return 0;
    CSWCObject* pSpeaker = pObject->AsSWCObject();
    if (rSound.IsValid() && pSpeaker) {
        if (m_pDialogSound) {
            m_pDialogSound->Stop();
            delete m_pDialogSound;
        }
        m_pDialogSound = new CExoStreamingSoundSource;
        m_pDialogSound->SetPriorityGroup(9);
        if (!m_pDialogSound->Play(rSound, 8, 1, 0, 0) && !m_pDialogSound->Play(rSound, 4, 1, 0, 0)) {
            pSpeaker->PlayDialogSound(rSound);
            return 0;
        }
        SetLipSyncData(oid, rSound);
    }
    m_oidLastSpeaker = pSpeaker->m_idSelf;
    return 1;
}

// FUNCTION: SWKOTOR 0x00631B70
void CGuiInGame::ResetDialogAnimations()
{
    for (unsigned long i = 0; i < m_nLastDialogParticipants; ++i) {
        CGameObject* pObject = g_pAppManager->m_pClientExoApp->GetGameObject(m_pLastDialogParticipants[i]);
        if (pObject) {
            if (pObject->AsSWCCreature()) {
                CSWCCreature* pCreature = pObject->AsSWCCreature();
                if (CanPlayDialogAnimations(pCreature->m_idSelf)) {
                    pCreature->m_n54 = 0;
                    if (pCreature->GetCurrentAnimation() != 10000)
                        pCreature->SetAnimation(10000, 0, 1);
                    pCreature->m_pAnimationBase->SetFlag138(0);
                }
            }
            if (pObject->AsSWCPlaceable()) {
                CSWCPlaceable* pPlaceable = pObject->AsSWCPlaceable();
                CSWSPlaceable* pServer = pPlaceable->GetServerPlaceable();
                if (pPlaceable) {
                    pPlaceable->m_n54 = 0;
                    pPlaceable->SetAnimation(pServer->m_nAnimation, 0, 0);
                    pPlaceable->m_pAnimationBase->SetFlag138(0);
                }
            }
        }
    }
}

// Descriptive name: resets the two talking participants (+0x178/+0x17C).
// FUNCTION: SWKOTOR 0x00631C60
void CGuiInGame::ResetTalkAnimations()
{
    CGameObject* pObject = g_pAppManager->m_pClientExoApp->GetGameObject(m_oid178);
    if (pObject && pObject->AsSWCObject()) {
        if (!pObject->AsSWCCreature() || CanPlayDialogAnimations(pObject->AsSWCCreature()->m_idSelf)) {
            if (pObject->AsSWCObject()->GetCurrentAnimation() != 10000)
                pObject->AsSWCObject()->SetAnimation(10000, 0, 1);
            pObject->AsSWCObject()->m_pAnimationBase->SetFlag138(0);
        }
    }
    pObject = g_pAppManager->m_pClientExoApp->GetGameObject(m_oid17C);
    if (pObject && pObject->AsSWCObject()) {
        if (!pObject->AsSWCCreature() || CanPlayDialogAnimations(pObject->AsSWCCreature()->m_idSelf)) {
            if (pObject->AsSWCObject()->GetCurrentAnimation() != 10000)
                pObject->AsSWCObject()->SetAnimation(10000, 0, 1);
            pObject->AsSWCObject()->m_pAnimationBase->SetFlag138(0);
        }
    }
}
