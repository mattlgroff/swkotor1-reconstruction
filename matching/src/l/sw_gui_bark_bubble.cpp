// Bark bubble panel CSWGuiBarkBubble (vtable 0x00755C60,
// 0x006A9770-0x006AA020): one label (LBL_BARKTEXT from "barkbubble"), an
// optional positional streaming voice-over and a display timer.

#include "l/sw_gui_bark_bubble.h"

// FUNCTION: SWKOTOR 0x006A9770
CSWGuiBarkBubble::CSWGuiBarkBubble(CSWGuiManager* pManager)
    : CSWGuiPanel(pManager)
{
    StartLoadFromLayout("barkbubble");
    InitControl(&m_lblText, "LBL_BARKTEXT", 1);
    UnloadResource();
    (CSWGuiExtent&)m_baseExtent = m_extent;
    m_fTime = 0.0f;
    m_pSound = 0;
    m_bPlaying = 0;
    m_bIgnoreRange = 0;
    m_nStartTime = 0;
}

// FUNCTION: SWKOTOR 0x006A9890
CSWGuiBarkBubble::~CSWGuiBarkBubble()
{
    StopSound();
}

// FUNCTION: SWKOTOR 0x006A9920
void CSWGuiBarkBubble::SetBark(unsigned long oidSpeaker, const CExoString& sText,
                               const CResRef& cSound, int bIgnoreRange)
{
    unsigned long oidPlayer = g_pAppManager->GetServerExoApp()->GetPlayerCreatureID();
    unsigned long oidServer = g_pAppManager->GetServerExoApp()->GetClientObjectByObjectID(oidPlayer)->m_oidServer;
    unsigned long oidClient = g_pAppManager->GetClientExoApp()->ServerToClientObjectId(oidServer);
    CGameObject* pPlayer = g_pAppManager->GetClientExoApp()->GetGameObject(oidClient);
    CExoString sBark(sText);
    CTlkTable* pTlk = g_pTlkTableAlt;
    pTlk->m_pTokenObject = pPlayer;
    pTlk->m_nGender = pTlk->GetGender();
    pTlk->m_bTokenFlag = 0;
    pTlk->ParseTokens(&sBark);
    m_lblText.m_cText.SetText(sBark.CStr());
    int nBorder = m_lblText.m_cBorder.GetCornerSize();
    int nHeight = m_lblText.m_cTextBox.GetHeight() + nBorder * 2 + 10;
    CSWGuiExtent extent = m_extent;
    extent.height = nHeight;
    SetExtent(extent);
    m_nFlags &= ~0x700;
    m_oidSpeaker = oidSpeaker;
    m_bIgnoreRange = bIgnoreRange;
    m_fTime = sBark.GetLength() * 0.11f + 1.0f;
    StopSound();
    m_pSound = new CExoStreamingSoundSource;
    if (oidSpeaker != 0x7f000000) {
        Vector vPosition = g_pAppManager->GetClientExoApp()->GetGameObject(oidSpeaker)
            ->AsSWCObject()->m_vPosition;
        m_pSound->SetProperty0x58(1);
        m_pSound->SetReverbEnabled(0);
        m_pSound->SetProperty0x5C(vPosition);
    }
    m_pSound->SetPriorityGroup(0x1a);
    m_pSound->SetVolume(g_nBarkVolume);
    m_bPlaying = m_pSound->Play(cSound, 8, 1, 0, 0);
}

// FUNCTION: SWKOTOR 0x006A9BB0
void CSWGuiBarkBubble::Pause()
{
    if (m_fTime > 0.0f || m_pSound) {
        if (m_pSound) {
            m_pSound->Pause();
        }
        HidePanel();
    }
}

// FUNCTION: SWKOTOR 0x006A9C10
void CSWGuiBarkBubble::Resume()
{
    if (m_fTime > 0.0f || m_pSound) {
        if (m_pSound) {
            m_pSound->Resume();
        }
        g_pAppManager->GetClientExoApp()->GetGuiManager()->AddPanel(this, 4, 1);
    }
}

// FUNCTION: SWKOTOR 0x006A9C60
void CSWGuiBarkBubble::Hide(int bStopSound)
{
    if (bStopSound) {
        StopSound();
    }
    m_fTime = 0.0f;
    HidePanel();
}

// FUNCTION: SWKOTOR 0x006A9CE0
void CSWGuiBarkBubble::Draw(float fDelta)
{
    CSWGuiExtent extent = m_baseExtent;
    extent.height = m_extent.height;
    if (!g_pAppManager->GetClientExoApp()->GetInGameGui()->IsMinimapHidden()) {
        int nMarginX, nMarginY;
        m_pManager->GetSafeMargins(&nMarginX, &nMarginY);
        extent.width = m_pManager->m_nWidth - nMarginX * 2;
    }
    if (g_pAppManager->GetClientExoApp()->GetCombatMode()) {
        extent.top += g_pAppManager->GetClientExoApp()->GetInGameGui()->GetCombatBarHeight();
    }
    SetExtent(extent);
    int bShow = 1;
    if (m_oidSpeaker != 0x7f000000) {
        CSWCObject* pLeader = g_pAppManager->GetClientExoApp()->GetParty()->GetMember(0);
        if (pLeader) {
            Vector vLeader = pLeader->m_vPosition;
            CGameObject* pSpeaker = g_pAppManager->GetClientExoApp()->GetGameObject(m_oidSpeaker);
            if (!pSpeaker || !pSpeaker->AsSWCObject()) {
                bShow = 0;
            } else if (!m_bIgnoreRange) {
                Vector vDiff = vLeader - pSpeaker->AsSWCObject()->m_vPosition;
                if (!(vDiff.x * vDiff.x + vDiff.y * vDiff.y + vDiff.z * vDiff.z < 36.0f)) {
                    bShow = 0;
                }
            }
        }
    }
    if (m_pSound && m_pSound->IsPlaying()) {
        if (bShow) {
            CSWGuiPanel::Draw(fDelta);
            if (m_pManager->m_fFrameTime < 1.5f) {
                m_fTime -= m_pManager->m_fFrameTime;
            }
        } else {
            SetToDestroy(0);
        }
    } else if (m_pSound && (m_pSound->GetPlayFailed() || !m_bPlaying)) {
        if (m_fTime > 0.0f && bShow) {
            CSWGuiPanel::Draw(fDelta);
            if (m_pManager->m_fFrameTime < 1.5f) {
                m_fTime -= m_pManager->m_fFrameTime;
            }
        } else {
            HidePanel();
        }
    } else {
        StopSound();
        HidePanel();
    }
    if (g_pAppManager->GetClientExoApp()->GetCombatMode()) {
        m_extent.top -= g_pAppManager->GetClientExoApp()->GetInGameGui()->GetCombatBarHeight();
    }
}

// FUNCTION: SWKOTOR 0x006A9F90
void CSWGuiBarkBubble::OnPanelAdded()
{
    m_nStartTime = (int)(g_pAppManager->GetClientExoApp()->GetWorldTimer()->GetSnapshotTime() / 1000);
    CSWGuiPanel::OnPanelAdded();
}

// FUNCTION: SWKOTOR 0x006A9FD0
void CSWGuiBarkBubble::SetExtent(const CSWGuiExtent& extent)
{
    CSWGuiExtent label;
    label.left = 5;
    label.top = 5;
    label.width = extent.width - 10;
    label.height = extent.height - 10;
    m_lblText.SetExtent(label);
    CSWGuiPanel::SetExtent(extent);
}

// FUNCTION: SWKOTOR 0x006AA020 SYMBOL: ??_GCSWGuiBarkBubble@@UAEPAXI@Z
