// CGuiInGame dialog reply display (guiingame.o).

#include "n/gui_in_game_dialog.h"

// FUNCTION: SWKOTOR 0x006340E0
// NONMATCH: 12 diff insns, register choice only: the original keeps the
// player's server id in eax and loads the client app before pushing it.
void CGuiInGame::ShowDialogReplies(unsigned long nArg1, unsigned long nArg2)
{
    if (m_nReplies > 1 || (m_nReplies > 0 && *m_pReplies != ""))
        g_pAppManager->m_pClientExoApp->m_pInternal->ClearInputState(0xffff);
    if (!m_pDialog)
        return;
    unsigned long oidPlayer = g_pAppManager->m_pClientExoApp->ServerToClientObjectId(
        g_pAppManager->m_pServerExoApp->GetClientObjectByObjectId(
            g_pAppManager->m_pServerExoApp->GetPlayerCreatureId())->m_oidPC);
    if (m_nReplies == 1 && *m_pReplies == "") {
        ((CSWGuiDialogPanel*)m_pDialog)->SetReplies(m_nReplies, m_pReplies, m_pRepliesData, nArg1, 0);
        ((CSWGuiDialogPanel*)m_pDialog)->m_n64 = nArg2;
        int bHasText = 1;
        ((CSWGuiDialogPanel*)m_pDialog)->SetEntryText(m_pReplies);
        if (m_pReplies->IsEmpty())
            bHasText = 0;
        m_nC8 = 0;
        m_bHasEntryText = bHasText;
        m_nD4 = 0;
        m_nD0 = 1;
        UpdateDialog();
        ((CSWGuiDialogPanel*)m_pDialog)->SetActive(0, 0);
        return;
    }
    ((CSWGuiDialogPanel*)m_pDialog)->SetReplies(m_nReplies, m_pReplies, m_pRepliesData, nArg1, 1);
    ((CSWGuiDialogPanel*)m_pDialog)->m_n64 = nArg2;
    UnlockDialogParticipantOrientations();
    int bSolo = 0;
    CSWCCreature* pCreature = g_pAppManager->m_pClientExoApp->GetCreatureByGameObjectID(oidPlayer);
    if (pCreature && pCreature->m_bSoloMode)
        bSolo = 1;
    m_oid17C = m_oidDialogSpeaker;
    m_oidDialogSpeaker = m_oidDialogOwner;
    CSWCCreature* pLeader = g_pAppManager->m_pClientExoApp->GetParty()->GetCharacter(0);
    if (bSolo && !m_bBD8)
        SetDialogOwner(oidPlayer);
    else if (pLeader)
        SetDialogOwner(pLeader->m_idSelf);
    else
        SetDialogOwner(oidPlayer);
    if (m_pDialog && m_pDialog == m_pDialogNormal) {
        if (!m_bShotCameraInitialized)
            InitializeShotCamera();
        UpdateShotCamera(*m_pShot, *m_pShotCameraId, *m_pShot12C, *m_pShot130, *m_pShot134, *m_pShotVideoEffect);
    }
}

// Descriptive name; the reply count is nReplies + nExtraReplies.
// FUNCTION: SWKOTOR 0x00634340
// NONMATCH: 29 diff insns, loop induction-register tie: the original keeps
// the pArg24 offset in ebx (spilled around the token parse, which then uses
// ebx for the talk table) and the Vector pointer in ebp; ours swaps them.
int CGuiInGame::HandleDialogReplies(unsigned long nArg1, unsigned long nArg2, unsigned long nArg3,
                                    CExoString* pTexts, int* pArg5, int* pArg6, int* pArg7, int* pArg8,
                                    float* pArg9, float* pArg10, int* pArg11, unsigned short* pArg12,
                                    float* pArg13, int* pArg14, unsigned char* pArg15, Vector* pArg16,
                                    float* pArg17, float* pArg18, unsigned long nReplies,
                                    unsigned long nExtraReplies, unsigned long nArg21, int* pArg22,
                                    unsigned long** pArg23, unsigned short** pArg24, unsigned long nArg25)
{
    unsigned long nCount = nReplies + nExtraReplies;
    if (nCount > 0) {
        CExoString sText;
        InitializeRepliesData(nCount);
        for (unsigned long i = 0; i < nCount; i++) {
            sText = pTexts[i];
            if (!sText.IsEmpty()) {
                CGameObject* pObject = g_pAppManager->m_pClientExoApp->GetGameObject(
                    g_pAppManager->m_pClientExoApp->ServerToClientObjectId(
                        g_pAppManager->m_pServerExoApp->GetClientObjectByObjectId(
                            g_pAppManager->m_pServerExoApp->GetPlayerCreatureId())->m_oidPC));
                CTlkTable* pTlk = g_pTlkTableAlt;
                pTlk->m_pTokenObject = pObject;
                pTlk->m_nGender = pTlk->GetGender();
                pTlk->m_nFlags &= ~1;
                pTlk->ParseTokens(&sText);
            }
            SetReplyData(i, sText, pArg5[i], pArg6[i], pArg7[i], pArg8[i], pArg9[i], pArg10[i],
                         pArg11[i], pArg12[i], pArg13[i], pArg14[i], pArg15[i], pArg16[i],
                         pArg17[i], pArg18[i], pArg22[i], pArg23[i], pArg24[i]);
        }
    }
    CleanUpDialogNode();
    ShowDialogReplies(nExtraReplies, nArg21);
    return 1;
}

// FUNCTION: SWKOTOR 0x006339C0
// NONMATCH: 33 diff insns, register/stack-slot tie in the tail: the original
// keeps the server owner object in ebp and spills the animation count; ours
// does the opposite (and orders the reply/ended/player slots differently).
void CGuiInGame::UpdateDialog()
{
    if (!m_nGlobalDialogState)
        return;
    if (m_bFovPending == 1) {
        CSWCModule* pModule = g_pAppManager->m_pClientExoApp->GetModule();
        if (pModule->m_pCamera) {
            pModule->m_pCamera->SetFieldOfView(m_fPendingFov);
            m_bFovPending = 0;
        }
    }
    if (!m_nD0)
        return;
    unsigned long oidSpeaker = m_oidDialogOwner;
    int nNode = m_nC8;
    if (!m_pReplySpeakers)
        return;
    if (m_pReplySpeakers[nNode] != 0x7f000000)
        oidSpeaker = m_pReplySpeakers[nNode];
    unsigned long oidListener = m_oid184;
    CGameObject* pObject = g_pAppManager->GetClientExoApp()->GetGameObject(oidSpeaker);
    if (!pObject || !pObject->AsSWCObject())
        return;
    pObject = g_pAppManager->m_pClientExoApp->GetGameObject(oidListener);
    if (!pObject || !pObject->AsSWCObject())
        return;
    for (int i = 0; i < m_pParticipantCounts[nNode]; i++) {
        pObject = g_pAppManager->m_pClientExoApp->GetGameObject(m_ppParticipantIds[nNode][i]);
        if (!pObject || !pObject->AsSWCObject())
            return;
    }
    AppendToDialogBuffer(m_pReplies[m_nC8], oidListener);
    if (m_nD4) {
    int nReply;
    if (m_nC8 >= 0)
        nReply = ((CSWGuiDialogPanel*)m_pDialog)->GetReplyIndex(m_nC8);
    else
        nReply = -1;
    int bEnded = m_nC8 == -3;
    unsigned long oidPlayer = g_pAppManager->m_pServerExoApp->GetPlayerCreatureId();
    CSWSPlayer* pPlayer = g_pAppManager->GetServerExoApp()->GetClientObjectByObjectId(oidPlayer);
    unsigned long oidOwner = g_pAppManager->m_pServerExoApp->ClientToServerObjectID(((CSWGuiDialogPanel*)m_pDialog)->m_oidOwner);
    CGameObject* pOwner = g_pAppManager->GetServerExoApp()->GetGameObject(oidOwner);
    if (!m_bNoDialogAnimations) {
        int nCount = m_pParticipantCounts[nNode];
        InitializeDialogAnimationData(nCount);
        for (int j = 0; j < nCount; j++)
            SetDialogAnimation(j, m_ppParticipantIds[nNode][j], m_ppParticipantAnims[nNode][j]);
        SetDialogAnimations();
    }
    if (pOwner && pOwner->AsSWSObject())
        pOwner->AsSWSObject()->ReplyDialog(pPlayer->m_nPlayerId, nReply, bEnded,
                                           ((CSWGuiDialogPanel*)m_pDialog)->m_n64, 1);
    m_nD0 = 0;
    }
    else
        HandleDialogSelection(m_nC8, m_bHasEntryText);
}

// Matched at 0x00628710 (gui_ingame_control_family.cpp); defined here
// unannotated so cl inlines it into HandleDialogSelection.
void CGuiInGame::SetDialogAnimation(unsigned long nIndex, unsigned long oid, unsigned short nAnimation)
{
    if (nIndex < m_nDialogAnimations && m_pDialogObjects && m_pDialogAnimations) {
        m_pDialogObjects[nIndex] = oid;
        m_pDialogAnimations[nIndex] = nAnimation;
    }
}

// FUNCTION: SWKOTOR 0x00633660
// NONMATCH: register-allocation tie (most shown diffs are a 1-byte shift):
// the original keeps bEnded in ebp, the server owner in ebx and reloads
// bAnimate from its slot; ours caches bAnimate in ebx and spills bEnded.
void CGuiInGame::HandleDialogSelection(int nSelection, int bAnimate)
{
    m_nD0 = 0;
    if (nSelection != -2) {
    int nReply;
    if (nSelection >= 0)
        nReply = ((CSWGuiDialogPanel*)m_pDialog)->GetReplyIndex(nSelection);
    else
        nReply = -1;
    int bEnded = nSelection == -3;
    CSWCMessage cMessage;
    unsigned long oidPlayer = g_pAppManager->m_pServerExoApp->GetPlayerCreatureId();
    CSWSPlayer* pPlayer = g_pAppManager->GetServerExoApp()->GetClientObjectByObjectId(oidPlayer);
    unsigned long oidOwner = g_pAppManager->m_pServerExoApp->ClientToServerObjectID(((CSWGuiDialogPanel*)m_pDialog)->m_oidOwner);
    CGameObject* pOwner = g_pAppManager->GetServerExoApp()->GetGameObject(oidOwner);
    ((CSWGuiDialogPanel*)m_pDialog)->SetEntryText(&m_pReplies[nSelection]);
    if (bAnimate)
        InitializeDialogAnimationData(m_pParticipantCounts[nSelection]);
    for (int i = 0; i < m_pParticipantCounts[nSelection]; i++)
        SetDialogAnimation(i, m_ppParticipantIds[nSelection][i], m_ppParticipantAnims[nSelection][i]);
    unsigned char nFade = m_pReply144[nSelection];
    if (nFade) {
        int bOn = 1;
        if (nFade == 4 || nFade == 1)
            bOn = 0;
        Vector* pColor = &m_pReply148[nSelection];
        float fLength = m_pReply150[nSelection];
        float fDelay = m_pReply14C[nSelection];
        if (m_pLetterbox)
            m_pLetterbox->StartFade(bOn, fDelay, fLength, *pColor);
    }
    if (bAnimate) {
        UnlockDialogParticipantOrientations();
        unsigned long oidListener = m_pReplySpeakers[nSelection];
        if (oidListener == 0x7f000000) {
            m_oid17C = m_oidDialogSpeaker;
            oidListener = m_oidDialogOwner;
        }
        else
            m_oid17C = m_oidDialogSpeaker;
        m_oid178 = m_oidDialogOwner;
        m_oidDialogSpeaker = oidListener;
        m_oidDialogOwner = m_oid184;
        SetDialogAnimations();
        if (m_pDialog && m_pDialog == m_pDialogNormal) {
            if (!SetAnimatedCamera(m_pReply138[nSelection], m_pReply13C[nSelection])) {
                if (!m_bShotCameraInitialized)
                    InitializeShotCamera();
                UpdateShotCamera(m_pShot[nSelection], m_pShotCameraId[nSelection], m_pShot12C[nSelection],
                                 m_pShot130[nSelection], m_pShot134[nSelection], m_pShotVideoEffect[nSelection]);
            }
        }
        else if (m_pShot[nSelection] == 6) {
            if (!m_bShotCameraInitialized)
                InitializeShotCamera();
            SetComputerCameraShot(m_pShotCameraId[nSelection], m_pShot134[nSelection], m_pShotVideoEffect[nSelection]);
        }
    }
    if (bEnded)
        CloseDialog();
    if (pOwner && pOwner->AsSWSObject())
        pOwner->AsSWSObject()->ReplyDialog(pPlayer->m_nPlayerId, nReply, bEnded,
                                           ((CSWGuiDialogPanel*)m_pDialog)->m_n64, 0);
    }
    else
        CloseDialog();
}
