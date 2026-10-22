// Conversation GUI: CSWGuiDialog (vtable 0x007559E0 family), its computer /
// cinematic subclasses, the letterbox bars and CSWGuiDialogTop
// (0x006A6D70-0x006A8110). Behavioural reference:
// src/unknown/sub_006a6d70_gui_dialog_helpers.cpp,
// src/batches/b00698_anim_texture_vfx_gui.cpp, b006a5_vfx_equip_abilities_aoe.

#include "l/sw_gui_dialog.h"

// FUNCTION: SWKOTOR 0x006A6D70
void CSWGuiDialog::SetForced()
{
    m_bForced = 1;
}

// FUNCTION: SWKOTOR 0x006A6D80
int CSWGuiDialog::GetReplyIndex(int nReply)
{
    if (nReply < m_nReplies && nReply >= 0) {
        return m_aReplyIndex[nReply];
    }
    return -1;
}

// FUNCTION: SWKOTOR 0x006A6DA0
int CSWGuiLetterbox::IsFadeDone()
{
    if (m_fTime == 0.0f) {
        return 1;
    }
    return m_fTime > m_fStart + m_fDuration;
}

// FUNCTION: SWKOTOR 0x006A6DE0
void CSWGuiDialogComputer::OnPanelAdded()
{
    m_pSound->Play();
    CSWGuiPanel::OnPanelAdded();
}

// FUNCTION: SWKOTOR 0x006A6E00
void CSWGuiDialogComputer::OnPanelRemoved()
{
    m_pSound->Stop();
    CSWGuiPanel::OnPanelRemoved();
}

// FUNCTION: SWKOTOR 0x006A6E20
CExoString CSWGuiDialogComputer::GetFullScreenBG()
{
    CExoString sName = CSWGuiManager::GetScreenResolutionString(g_nScreenWidth, g_nScreenHeight);
    sName = sName + g_sComputerBGPrefix + CExoString(m_nResolution);
    return sName;
}

// FUNCTION: SWKOTOR 0x006A6F30 SYMBOL: ??1CSWGuiDialog@@UAE@XZ
void DestroyDialog(CSWGuiDialog* pDialog)
{
    pDialog->CSWGuiDialog::~CSWGuiDialog();
}

// FUNCTION: SWKOTOR 0x006A6F20 SYMBOL: ??_9@$BIE@AE
void (CSWGuiDialog::*GetSelectCurrentReply())(int)
{
    return &CSWGuiDialog::SelectCurrentReply;
}

// FUNCTION: SWKOTOR 0x006A6FC0
void __stdcall CSWGuiDialog::ResetReplyColor(CSWGuiControl* pControl)
{
    CSWGuiLabel* pLabel = pControl->GetLabel();
    pLabel->m_cText.SetColor(g_vDefaultReplyColor);
}

// FUNCTION: SWKOTOR 0x006A6FE0
void CSWGuiDialog::SetReplyColor(CSWGuiControl* pControl)
{
    CSWGuiLabel* pLabel = pControl->GetLabel();
    pLabel->m_cText.SetColor(m_vReplyColor);
}

// FUNCTION: SWKOTOR 0x006A7010
void CSWGuiDialog::SetDialogMessage(const CExoString& sMessage)
{
    HideControl(&m_lbReplies);
    HideControl(&m_lblMessage);
    m_sMessage = sMessage;
    if (m_bForced && g_pAppManager->GetClientExoApp()->GetInGameGui()->IsDialogForced()) {
        m_bForced = 0;
        m_bMessageHidden = 1;
    } else {
        ShowMessage();
    }
    m_bMessageShown = 1;
}

// FUNCTION: SWKOTOR 0x006A70A0
void CSWGuiDialog::ShowMessage()
{
    m_bMessageHidden = 0;
    if ((g_pAppManager->GetClientExoApp()->GetOptions()->m_nFlags & 4)
        || g_pAppManager->GetClientExoApp()->GetInGameGui()->m_bMessagePending) {
        m_lblMessage.m_cText.SetText(m_sMessage);
        g_pAppManager->GetClientExoApp()->GetInGameGui()->m_bMessagePending = 0;
    }
    m_lblMessage.m_bVisible = 1;
}

// FUNCTION: SWKOTOR 0x006A7110
int CSWGuiDialog::SelectReply(int nReply)
{
    if (nReply < m_nReplies) {
        g_pAppManager->GetClientExoApp()->GetInGameGui()->SelectReply(nReply, m_nReplies, 1);
        g_pAppManager->GetClientExoApp()->GetInGameGui()->UpdateDialog();
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x006A7160
void CSWGuiDialog::ClearReplies()
{
    HideControl(&m_lbReplies);
    HideControl(&m_lblMessage);
    m_nSelected = -1;
    m_nReplies = 0;
    m_sMessage = "";
    for (int i = 0; i < 20; i++) {
        m_aReplies[i].m_cText.SetColor(m_vReplyColor);
        m_aReplies[i].m_cText.SetText("");
    }
}

// FUNCTION: SWKOTOR 0x006A7230
// Code and jump tables are identical; matchcmp reports "length differs"
// because functions.csv has no entry for 0x006A7390 (SelectCurrentReply),
// which directly follows the tables without padding.
void CSWGuiDialog::HandleInputEvent(int nEvent, int bDown)
{
    if (!bDown) {
        return;
    }
    switch (nEvent) {
    case 0x27:
    case 0x2d:
        if (m_bMessageShown) {
            if (!m_bMessageHidden) {
                unsigned long oidSpeaker = m_oidSpeaker;
                unsigned long oid = g_pAppManager->GetServerExoApp()->ClientToServerObjectID(oidSpeaker);
                g_pAppManager->GetServerExoApp()->SkipPastDialog(oid);
            }
        } else {
            m_pManager->PlayGuiSound(0);
            SelectReply(m_lbReplies.m_nSelected);
        }
        break;
    case 0x31:
    case 0x3d:
        if (m_nSelected > 0) {
            m_nSelected--;
        }
        break;
    case 0x32:
    case 0x3e:
        if (m_nSelected < m_nReplies - 1) {
            m_nSelected++;
        }
        break;
    case 0xfe:
    case 0xff:
    case 0x100:
    case 0x101:
    case 0x102:
    case 0x103:
    case 0x104:
    case 0x105:
    case 0x106: {
        int nReply = nEvent - 0xfe;
        if (nReply < m_nReplies) {
            m_pManager->PlayGuiSound(0);
            SelectReply(nReply);
        }
        break;
    }
    case 0x1f9:
        if (m_bMessageShown && !m_bMessageHidden) {
            unsigned long oidSpeaker = m_oidSpeaker;
            unsigned long oid = g_pAppManager->GetServerExoApp()->ClientToServerObjectID(oidSpeaker);
            g_pAppManager->GetServerExoApp()->SkipPastDialog(oid);
        }
        break;
    }
    if (nEvent == 0x27 || nEvent == 0x2d) {
        return;
    }
    if (!m_bMessageShown || nEvent == 0 || nEvent == 1) {
        CSWGuiPanel::HandleInputEvent(nEvent, bDown);
    }
}

// FUNCTION: SWKOTOR 0x006A7390
void CSWGuiDialog::SelectCurrentReply(int nUnused)
{
    m_pManager->PlayGuiSound(0);
    SelectReply(m_lbReplies.m_nSelected);
}

// FUNCTION: SWKOTOR 0x006A73B0
void CSWGuiLetterbox::SetBarExtent(CSWGuiExtent extent)
{
    switch (m_nMode) {
    case 1:
        extent.top = 0;
        break;
    case 2:
        extent.height = m_pManager->m_nHeight - extent.top;
        break;
    }
    if (m_bDone) {
        static_cast<CSWGuiExtent&>(m_barExtent) = extent;
        m_extent = extent;
        return;
    }
    switch (m_nMode) {
    case 1:
        if (extent.height > m_barExtent.height) {
            m_barExtent.height = extent.height;
        }
        break;
    case 2:
        if (extent.top < m_barExtent.top) {
            int nDelta = m_barExtent.top - extent.top;
            m_barExtent.top -= nDelta;
            m_barExtent.height += nDelta;
        }
        break;
    }
}

// FUNCTION: SWKOTOR 0x006A7440
void CSWGuiLetterbox::SetExtent(const CSWGuiExtent& extent)
{
    CSWGuiExtent cExtent = extent;
    switch (m_nMode) {
    case 1:
        cExtent.top = 0;
        break;
    case 2:
        cExtent.height = m_pManager->m_nHeight - cExtent.top;
        break;
    }
    CSWGuiPanel::SetExtent(cExtent);
}

// FUNCTION: SWKOTOR 0x006A74B0
void CSWGuiLetterbox::ShowTop()
{
    if (!m_pManager) {
        m_pManager = g_pAppManager->GetClientExoApp()->GetGuiManager();
    }
    CSWGuiManager* pManager = m_pManager;
    int nBar = (int)(pManager->m_nWidth * 0.42857143f);
    int nHeight = (int)((pManager->m_nHeight - nBar) * 0.5f);
    m_targetExtent.height = 0;
    m_barExtent.top = 0;
    m_barExtent.left = 0;
    m_barExtent.width = pManager->m_nWidth;
    m_extent.top = 0;
    m_extent.left = 0;
    m_barExtent.height = nHeight;
    m_extent.width = pManager->m_nWidth;
    m_extent.height = 0;
    m_nMode = 1;
}

// FUNCTION: SWKOTOR 0x006A7540
void CSWGuiLetterbox::ShowBottom()
{
    if (!m_pManager) {
        m_pManager = g_pAppManager->GetClientExoApp()->GetGuiManager();
    }
    CSWGuiManager* pManager = m_pManager;
    int nScreenHeight = pManager->m_nHeight;
    int nBar = (int)(pManager->m_nWidth * 0.42857143f);
    int nHeight = (int)((nScreenHeight - nBar) * 0.5f);
    m_targetExtent.top = nScreenHeight;
    m_barExtent.top = pManager->m_nHeight - nHeight;
    m_barExtent.left = 0;
    m_barExtent.width = pManager->m_nWidth;
    m_barExtent.height = nHeight;
    m_extent.top = pManager->m_nHeight;
    m_extent.left = 0;
    m_extent.width = pManager->m_nWidth;
    m_extent.height = nHeight;
    m_nMode = 2;
}

// FUNCTION: SWKOTOR 0x006A75E0
void CSWGuiLetterbox::ResetFade()
{
    if (m_nMode == 3) {
        SetAlpha(0.0f);
    } else {
        SetAlpha(1.0f);
    }
    m_fStart = 0;
    m_fDuration = 0;
    m_fTime = 0;
    m_bFadeIn = 0;
}

// FUNCTION: SWKOTOR 0x006A7620
void CSWGuiLetterbox::StartFade(int bFadeIn, float fStart, float fDuration, const Vector& vColor)
{
    m_vColor = vColor;
    if (bFadeIn) {
        SetAlpha(1.0f);
    } else {
        SetAlpha(0.0f);
    }
    m_bFadeIn = bFadeIn;
    m_fStart = fStart;
    m_fDuration = fDuration;
    m_fTime = 0.1f;
}

// FUNCTION: SWKOTOR 0x006A7AE0
// NONMATCH (6): the original loads nMarginY before the cdq and emits
// `add eax, -5`; every spelling of `/ 2 - nMarginY - 5` tried gives `sub eax, 5`.
void CSWGuiDialogTop::SetReply(const CExoString& sReply)
{
    if (g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pLetterbox->m_bDone) {
        int nMarginX;
        int nMarginY;
        m_pManager->GetSafeMargins(&nMarginX, &nMarginY);
        m_lblReply.m_cText.SetText(sReply);
        int nHeight = m_lblReply.m_cTextBox.GetHeight();
        CSWGuiExtent cExtent = m_lblReply.m_extent;
        if (nHeight <= m_extent.height) {
            CSWGuiManager* pManager = m_pManager;
            nHeight = (pManager->m_nHeight - (int)(pManager->m_nWidth * 0.42857143f)) / 2 - nMarginY - 5;
        }
        m_extent.height = nHeight;
        cExtent.height = nHeight;
        m_lblReply.SetExtent(cExtent);
        cExtent = m_extent;
        cExtent.left = 0;
        cExtent.top = 0;
        cExtent.width = m_pManager->m_nWidth;
        cExtent.height += nMarginY;
        CGuiInGame* pGui = g_pAppManager->GetClientExoApp()->GetInGameGui();
        pGui->m_pLetterbox->SetBarExtent(cExtent);
    }
}

// FUNCTION: SWKOTOR 0x006A7C20
CSWGuiDialogCinematic::~CSWGuiDialogCinematic()
{
    if (m_pTop) {
        delete m_pTop;
        m_pTop = 0;
    }
}

// FUNCTION: SWKOTOR 0x006A7C90
void CSWGuiDialog::SetSpeaker(unsigned long oid)
{
    m_oidSpeaker = oid;
}

// FUNCTION: SWKOTOR 0x006A8050
CSWGuiReplyEntry::CSWGuiReplyEntry()
{
}

// FUNCTION: SWKOTOR 0x006A80B0
CSWGuiReplyEntry::~CSWGuiReplyEntry()
{
}

// FUNCTION: SWKOTOR 0x006A81E0
void CSWGuiDialogComputer::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x39:
            m_lbEntries.HandleInputEvent(0x31, 1);
            break;
        case 0x3a:
            m_lbEntries.HandleInputEvent(0x32, 1);
            break;
        }
    }
    CSWGuiDialog::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x006A8110
CSWGuiDialogComputer::~CSWGuiDialogComputer()
{
    if (m_pSound) {
        delete m_pSound;
        m_pSound = 0;
    }
    m_lbEntries.ClearControls();
}

// FUNCTION: SWKOTOR 0x006A8520
CSWGuiDialogComputerCamera::~CSWGuiDialogComputerCamera()
{
    if (m_pCamera) {
        delete m_pCamera;
        m_pCamera = 0;
    }
}

// FUNCTION: SWKOTOR 0x006A8680 SYMBOL: ??_GCSWGuiDialog@@UAEPAXI@Z
CSWGuiDialog* NewDialog()
{
    return new CSWGuiDialog;
}

// FUNCTION: SWKOTOR 0x006A8930
CSWGuiLetterbox::CSWGuiLetterbox(CSWGuiManager* pManager)
    : CSWGuiPanel(pManager)
{
    m_nMode = 0;
    m_bDone = 0;
    m_vColor = g_vBlack;
    SetAlpha(1.0f);
    m_fStart = 0;
    m_fDuration = 0;
    m_fTime = 0;
    m_bFadeIn = 0;
}

// FUNCTION: SWKOTOR 0x006A89F0 SYMBOL: ??_GCSWGuiLetterbox@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x006A8A10 SYMBOL: ??1CSWGuiLetterbox@@UAE@XZ

// FUNCTION: SWKOTOR 0x006A8A60
void CSWGuiLetterbox::SetFullScreen()
{
    if (!m_pManager) {
        m_pManager = g_pAppManager->GetClientExoApp()->GetGuiManager();
    }
    m_nMode = 3;
    SetAlpha(0.0f);
    m_fStart = 0;
    m_fDuration = 0;
    m_fTime = 0;
    m_bFadeIn = 0;
    m_extent.top = 0;
    m_extent.left = 0;
    m_extent.width = m_pManager->m_nWidth;
    m_extent.height = m_pManager->m_nHeight;
}

// FUNCTION: SWKOTOR 0x006A8AD0 SYMBOL: ??_GCSWGuiDialogTop@@UAEPAXI@Z
CSWGuiDialogTop* NewDialogTop()
{
    return new CSWGuiDialogTop;
}

// FUNCTION: SWKOTOR 0x006A8AF0 SYMBOL: ??1CSWGuiDialogTop@@UAE@XZ

// FUNCTION: SWKOTOR 0x006A8DC0 SYMBOL: ??_GCSWGuiDialogCinematic@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x006A94C0 SYMBOL: ??_GCSWGuiDialogComputer@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x006A9750 SYMBOL: ??_GCSWGuiDialogComputerCamera@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x006A7CA0
void CSWGuiDialogCinematic::SetExtent(const CSWGuiExtent& extent)
{
    CSWGuiPanel::SetExtent(extent);
    CSWGuiExtent cExtent = m_lbReplies.m_extent;
    cExtent.width = m_extent.width;
    m_lbReplies.SetExtent(cExtent);
    cExtent = m_lblMessage.m_extent;
    cExtent.width = m_extent.width;
    m_lblMessage.SetExtent(cExtent);
}

// FUNCTION: SWKOTOR 0x006A7D30
void CSWGuiDialogCinematic::ShowMessage()
{
    CSWGuiDialog::ShowMessage();
    CSWGuiExtent cExtent = m_lblMessage.m_extent;
    int nGrow = m_lblMessage.m_cTextBox.GetHeight() - cExtent.height;
    if (nGrow > 0) {
        m_extent.top -= nGrow;
        m_extent.height += nGrow;
        cExtent.height += nGrow;
    }
    m_lblMessage.SetExtent(cExtent);
    cExtent = m_extent;
    CSWGuiManager* pManager = m_pManager;
    cExtent.left = 0;
    cExtent.width = pManager->m_nWidth;
    cExtent.height = pManager->m_nHeight - cExtent.top;
    CGuiInGame* pGui = g_pAppManager->GetClientExoApp()->GetInGameGui();
    pGui->m_pLetterboxBottom->SetBarExtent(cExtent);
}

// FUNCTION: SWKOTOR 0x006A7E20
int CSWGuiDialogCinematic::SelectReply(int nReply)
{
    if (nReply < m_nReplies) {
        g_pAppManager->GetClientExoApp()->GetInGameGui()->SelectReply(nReply, m_nReplies, 1);
        g_pAppManager->GetClientExoApp()->GetInGameGui()->UpdateDialog();
        m_pTop->SetReply("");
        if (m_pManager->PanelExists(m_pTop)) {
            m_pManager->RemovePanel(m_pTop);
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x006A8DE0
void CSWGuiDialogCinematic::AddReply(unsigned long a1, int a2, int a3, int a4, int bLast)
{
    if (bLast) {
        m_pTop->SetReply(m_sMessage);
    }
    CSWGuiDialog::AddReply(a1, a2, a3, a4, bLast);
    if (m_lbReplies.m_bVisible) {
        CSWGuiExtent cExtent = m_lbReplies.m_extent;
        int nLine = m_aReplies[0].m_cTextBox.GetLineHeight();
        cExtent.height -= cExtent.height % nLine;
        m_lbReplies.SetExtent(cExtent);
    }
    if (bLast && !m_pManager->PanelExists(m_pTop)) {
        m_pManager->AddPanel(m_pTop, 0, 1);
    }
}

// FUNCTION: SWKOTOR 0x006A94E0
void CSWGuiDialogComputer::AddReply(unsigned long a1, int a2, int a3, int a4, int bLast)
{
    CSWGuiDialog::AddReply(a1, a2, a3, a4, bLast);
    if (bLast) {
        m_lblMessage.m_bVisible = 1;
    }
    LayoutEntries();
}

// FUNCTION: SWKOTOR 0x006A7EF0
void CSWGuiDialogCinematic::ClearReplies()
{
    CSWGuiZeroExtent cExtent;
    int nMarginX;
    int nMarginY;
    m_pManager->GetSafeMargins(&nMarginX, &nMarginY);
    CSWGuiManager* pManager = m_pManager;
    cExtent.left = nMarginX;
    int nScreenHeight = pManager->m_nHeight;
    cExtent.top = nScreenHeight - (nScreenHeight - (int)(pManager->m_nWidth * 0.42857143f)) / 2;
    cExtent.width = pManager->m_nWidth - 2 * nMarginX;
    cExtent.height = 100;
    int nShrink = m_extent.height - cExtent.height;
    SetExtent(cExtent);
    cExtent.left = 0;
    cExtent.width = m_pManager->m_nWidth;
    cExtent.height = m_pManager->m_nHeight - cExtent.top;
    CGuiInGame* pGui = g_pAppManager->GetClientExoApp()->GetInGameGui();
    pGui->m_pLetterboxBottom->SetExtent(cExtent);
    static_cast<CSWGuiExtent&>(cExtent) = m_lblMessage.m_extent;
    cExtent.height -= nShrink;
    m_lblMessage.SetExtent(cExtent);
    m_lblMessage.m_cText.SetText("");
    CSWGuiDialog::ClearReplies();
}

// FUNCTION: SWKOTOR 0x006A9520
void CSWGuiDialogComputer::SetDialogMessage(const CExoString& sMessage)
{
    LayoutEntries();
    CSWGuiDialog::SetDialogMessage(sMessage);
    CSWGuiListBox* pList = &m_lbEntries;
    pList->m_bVisible = m_lblMessage.m_bVisible;
    if (m_lbEntries.m_bVisible) {
        pList->ClearControls();
        CSWGuiExtent cExtent = m_lblMessage.m_extent;
        int nHeight;
        if (sMessage.GetLength() > 0) {
            nHeight = m_lblMessage.m_cTextBox.GetHeight();
        } else {
            nHeight = m_lblMessage.m_cTextBox.GetLineHeight();
        }
        cExtent.height = nHeight;
        m_lblMessage.SetExtent(cExtent);
        CSWGuiControl* pControl = &m_lblMessage;
        pList->AddControls(&pControl, 1, 0, 0, 0);
        pList->SetActiveControl(&m_lblMessage, 0);
    }
}

static inline float MaxF(float a, float b) { return a > b ? a : b; }
static inline float MinF(float a, float b) { return a < b ? a : b; }

// FUNCTION: SWKOTOR 0x006A7680
void CSWGuiLetterbox::Draw(float fDelta)
{
    if (m_nMode == 3 && m_fTime > 0.0f) {
        if (fDelta < 5.0f) {
            m_fTime += fDelta;
        }
        if (m_fTime > m_fStart) {
            float fAlpha = (m_fTime - m_fStart) / m_fDuration;
            SetAlpha(m_bFadeIn ? MaxF(0.0f, 1.0f - fAlpha) : MinF(1.0f, fAlpha));
        }
    }
    if (!m_bDone) {
        if (fDelta > 0.5f) {
            fDelta = 0.5f;
        }
        if (m_nMode == 1) {
            CSWGuiExtent cExtent = m_barExtent;
            if (m_extent.height < m_barExtent.height) {
                cExtent.height = (int)(fDelta * ((m_barExtent.height - m_targetExtent.height) * 2.0f)) + m_extent.height;
            } else {
                m_bDone = 1;
            }
            SetExtent(cExtent);
        } else if (m_nMode == 2) {
            CSWGuiExtent cExtent = m_barExtent;
            if (m_extent.top > m_barExtent.top) {
                int nStep = (int)(fDelta * ((m_barExtent.top - m_targetExtent.top) * 2.0f));
                cExtent.top = m_extent.top + nStep;
                cExtent.height = m_extent.height - nStep;
            } else {
                m_bDone = 1;
                g_pAppManager->GetClientExoApp()->GetInGameGui()->HideLetterbox();
                cExtent = m_barExtent;
            }
            SetExtent(cExtent);
        }
    }
    CSWGuiPanel::Draw(fDelta);
}
