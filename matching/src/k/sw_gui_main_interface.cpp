// CSWGuiMainInterface (vtable 0x00753F50): message line, action icons and
// queue, status labels, personal action selection, 0x00685560-0x00688E10.
#include "k/sw_gui_main_interface.h"

// FUNCTION: SWKOTOR 0x00685560
void CSWGuiMainInterface::SetMessageText(const CExoString& sText)
{
    m_lblMessage.m_cText.SetText(sText);
    CSWGuiExtent ext = m_lblMessage.m_extent;
    ext.height = m_lblMessage.m_cTextBox.GetHeight();
    ext.top = m_nMessageBottom - ext.height;
    m_lblMessage.SetExtent(ext);
    m_lblMessageBG.SetExtent(ext);
}

// FUNCTION: SWKOTOR 0x006855F0
void CSWGuiMainInterface::SetTargetId(int nId, int a2)
{
    if (m_nTargetId != nId) {
        m_nTargetId = nId;
        m_fTargetMenuTime = -1.0f;
    }
}

// FUNCTION: SWKOTOR 0x00685610
void CSWGuiMainInterface::ShowActionIcon(int* pCount, const CResRef& cIcon, const CResRef& cAlt,
                                         int bEnabled, int bForce)
{
    if (bForce || cIcon != "") {
        if (cIcon.IsValid()) {
            m_aActionIcons[*pCount].m_cBorder.SetFillImage(cIcon, 0);
        } else {
            m_aActionIcons[*pCount].m_cBorder.SetFillImage(cAlt, 0);
        }
        if (!bEnabled && *pCount == 0) {
            m_aActionIcons[*pCount].m_cBorder.m_fAlpha = 0.25f;
        } else {
            m_aActionIcons[*pCount].m_cBorder.m_fAlpha = 1.0f;
        }
        m_aActionIcons[(*pCount)++].SetVisible(1);
    }
}

// FUNCTION: SWKOTOR 0x006856E0
void CSWGuiMainInterface::HideActionDescription()
{
    m_lblDescription.SetVisible(0);
    m_lblDescriptionAlt.SetVisible(1);
    m_lblDescriptionBG.SetVisible(1);
    m_bDescriptionHidden = 1;
}

// FUNCTION: SWKOTOR 0x00685710
void CSWGuiMainInterface::ShowActionDescription()
{
    m_lblDescriptionAlt.SetVisible(0);
    if (m_bQueueShown) {
        m_lblDescription.SetVisible(1);
    }
    m_lblDescriptionBG.m_bVisible = m_lblDescription.m_bVisible;
    m_bDescriptionHidden = 0;
}

// FUNCTION: SWKOTOR 0x00685760
void CSWGuiMainInterface::UpdateFonts()
{
    CSWGuiPanel::UpdateFonts();
    m_cTargetMenu.m_lblName.UpdateFonts();
    m_cTargetMenu.m_lblNameBG.UpdateFonts();
    m_cTargetMenu.m_lblBarBG.UpdateFonts();
}

// FUNCTION: SWKOTOR 0x00686E20
// NONMATCH: the original keeps 0 in edi (EH state, the slot / count tests and
// the null action) and saves one push; same shape as SetTarget 0x00685CB0.
void CSWGuiMainInterface::UpdateMessage()
{
    CExoString sText;
    float fAlpha = 1.0f;
    if (m_fTargetTime > 0.0f) {
        float fFade = m_fTargetTime * 0.2f;
        if (fFade < 0.5f) {
            fAlpha = fFade * 2.0f;
        }
        sText = g_pAppManager->GetClientExoApp()->GetGUIString(m_n70);
    } else {
        int nSlot = m_nLastPersonalSlot;
        CSWGuiMainInterfaceAction& action = m_aPersonalActions[nSlot];
        if ((action.m_btnIcon.m_bActive || action.m_btnUp.m_bActive || action.m_btnDown.m_bActive ||
             action.m_btnFrame.m_bActive) &&
            nSlot >= 0) {
            int nCount = m_aPersonal[nSlot].m_nCount;
            if (nCount > 0) {
                int nSelected = m_anPersonalSelected[nSlot];
                CSWGuiTargetAction* pAction = 0;
                if (nSelected != -1) {
                    for (int i = 0; i < nCount; i++) {
                        if (m_aPersonal[nSlot].m_pData[i].m_nId == nSelected) {
                            pAction = &m_aPersonal[nSlot].m_pData[i];
                        }
                    }
                }
                if (!pAction) {
                    pAction = m_aPersonal[nSlot].m_pData;
                }
                sText = pAction->m_sName;
                int nActions = pAction->m_nCount;
                if (nActions > 1) {
                    sText.Format("%s (%d)", sText.CStr(), nActions);
                }
            }
        }
    }
    SetMessageText(sText);
    m_lblMessage.m_cText.m_fAlpha = fAlpha;
}

// FUNCTION: SWKOTOR 0x006874C0
void CSWGuiMainInterface::ShowActionQueue(int bShow)
{
    if (bShow != m_bQueueShown) {
        int bMove = 1;
        m_bQueueShown = bShow;
        if (!bShow && !m_lblQueueBG.m_bVisible) {
            bMove = 0;
        }
        m_aQueueLabels[0].m_bVisible = bShow;
        m_aQueueLabels[1].m_bVisible = bShow;
        m_aQueueLabels[2].m_bVisible = bShow;
        m_lblQueueBG.m_bVisible = bShow;
        m_btnQueue0.m_bVisible = bShow;
        m_btnQueue1.m_bVisible = bShow;
        m_btnQueue2.m_bVisible = bShow;
        if (!m_bDescriptionHidden) {
            m_lblDescription.m_bVisible = bShow;
            m_lblDescriptionBG.m_bVisible = bShow;
        }
        m_aActionIcons[0].SetVisible(bShow);
        m_aActionIcons[1].SetVisible(bShow);
        m_aActionIcons[2].SetVisible(bShow);
        m_aActionIcons[3].SetVisible(bShow);
        int nDelta = m_nQueueShift;
        if (!bShow) {
            nDelta = -nDelta;
            m_fQueueTime = -1.0f;
        }
        if (bMove) {
            m_lblMap.MoveTop(nDelta);
            m_lblMapBorder.MoveTop(nDelta);
            m_btnMapArrow.MoveTop(nDelta);
            m_nMapTop += nDelta;
            for (int i = 0; i < 9; i++) {
                m_aStatus[i].MoveTop(nDelta);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x00687FD0
CSWParty* CSWGuiMainInterface::GetParty()
{
    return g_pAppManager->m_pClientExoApp->GetParty();
}

// FUNCTION: SWKOTOR 0x00687FE0
void CSWGuiMainInterface::ShowStatus(unsigned char nStatus)
{
    switch (nStatus) {
    case 4: m_aStatus[5].m_bVisible = 0; break;
    case 5: m_aStatus[4].m_bVisible = 0; break;
    case 3: m_aStatus[2].m_bVisible = 0; break;
    case 2: m_aStatus[3].m_bVisible = 0; break;
    }
    m_aStatus[nStatus].Flash();
}

// FUNCTION: SWKOTOR 0x00688060
void CSWGuiMainInterface::RemoveControl(CSWGuiControl* pControl)
{
    if (pControl) {
        CExoArrayList<CSWGuiControl*>* pList = &m_lstControls;
        int nIndex = -1;
        for (int i = 0; i < pList->num; i++) {
            if (pList->element[i] == pControl) {
                nIndex = i;
                break;
            }
        }
        if (nIndex != -1) {
            pList->DelIndex(nIndex);
        }
        delete pControl;
    }
}

// FUNCTION: SWKOTOR 0x006880C0
void CSWGuiMainInterface::CancelCombat()
{
    CSWSCreature* pCreature =
        g_pAppManager->m_pClientExoApp->GetParty()->GetPlayerCharacter()->GetServerCreature();
    if (pCreature && pCreature->m_pCombatRound && !pCreature->m_pCombatRound->RemoveLastAction()) {
        pCreature->ClearAllActions(1);
    }
}

// FUNCTION: SWKOTOR 0x00688790
void CSWGuiMainInterface::CheckCombatTutorial()
{
    if (m_btnQueue0.m_bVisible) {
        if (!g_pAppManager->m_pClientExoApp->ShowTutorialWindow(0x20, 0x7f000000, 0x7f000000, 0)) {
            CancelCombat();
        }
    }
}

// FUNCTION: SWKOTOR 0x006887D0
void CSWGuiMainInterface::RefreshTargetActions()
{
    CSWCCreature* pPC = g_pAppManager->m_pClientExoApp->GetParty()->GetPlayerCharacter();
    if (pPC) {
        pPC->SetAttackTarget(0);
        pPC->ClearTargetActions();
        m_pManager->PlayGuiSound(0);
        m_fTargetMenuTime = -1.0f;
        m_fTargetTime = -1.0f;
    }
}

// FUNCTION: SWKOTOR 0x00688820
// NONMATCH: ebx / ebp swapped (count pointer vs selected id); local and
// declaration-order variants tried.
void CSWGuiMainInterface::SelectNextPersonalAction(int nSlot)
{
    if (nSlot >= 0 && nSlot < 4) {
        if (m_aPersonal[nSlot].m_nCount) {
            g_pAppManager->m_pClientExoApp->GetParty()->GetPlayerCharacter();
            CSWGuiTargetAction* pNext = 0;
            if (m_anPersonalSelected[nSlot] != -1) {
                for (int i = 0; i < m_aPersonal[nSlot].m_nCount; i++) {
                    if (m_aPersonal[nSlot].m_pData[i].m_nId == m_anPersonalSelected[nSlot]) {
                        if (i + 1 < m_aPersonal[nSlot].m_nCount) {
                            pNext = &m_aPersonal[nSlot].m_pData[i + 1];
                        } else {
                            pNext = m_aPersonal[nSlot].m_pData;
                        }
                        break;
                    }
                }
            }
            if (m_aPersonal[nSlot].m_nCount > 1) {
                if (!pNext) {
                    pNext = &m_aPersonal[nSlot].m_pData[1];
                }
                m_anPersonalSelected[nSlot] = pNext->m_nId;
                m_pManager->PlayGuiSound(7);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x006888E0
// NONMATCH: ebx / ebp swapped (count pointer vs selected id); local and
// declaration-order variants tried.
void CSWGuiMainInterface::SelectPrevPersonalAction(int nSlot)
{
    if (nSlot >= 0 && nSlot < 4) {
        if (m_aPersonal[nSlot].m_nCount) {
            g_pAppManager->m_pClientExoApp->GetParty()->GetPlayerCharacter();
            CSWGuiTargetAction* pPrev = 0;
            if (m_anPersonalSelected[nSlot] != -1) {
                for (int i = 0; i < m_aPersonal[nSlot].m_nCount; i++) {
                    if (m_aPersonal[nSlot].m_pData[i].m_nId == m_anPersonalSelected[nSlot]) {
                        if (i > 0) {
                            pPrev = &m_aPersonal[nSlot].m_pData[i - 1];
                        } else {
                            pPrev = &m_aPersonal[nSlot].m_pData[m_aPersonal[nSlot].m_nCount - 1];
                        }
                        break;
                    }
                }
            }
            if (m_aPersonal[nSlot].m_nCount > 1) {
                if (!pPrev) {
                    pPrev = &m_aPersonal[nSlot].m_pData[m_aPersonal[nSlot].m_nCount - 1];
                }
                m_anPersonalSelected[nSlot] = pPrev->m_nId;
                m_pManager->PlayGuiSound(7);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x00688DB0
void CSWGuiMainInterface::GetMapExtent(CSWGuiExtent* pExtent)
{
    pExtent->top = 3;
    CClientOptions* pOptions = g_pAppManager->m_pClientExoApp->GetOptions();
    int bShow = pOptions ? pOptions->ShowMinimap() : 1;
    if (bShow) {
        pExtent->top += m_btnMinimap.m_extent.top + m_btnMinimap.m_extent.height - 1;
    }
    pExtent->left = m_pManager->m_nWidth - pExtent->width - 3;
}

// FUNCTION: SWKOTOR 0x0068AD60
void CSWGuiMainInterface::DoPersonalAction(int nSlot, int a2)
{
    if (nSlot >= 0 && nSlot < 6) {
        if (g_bClearTargetOnAction) {
            RefreshTargetActions();
        }
        m_nLastPersonalSlot = nSlot;
        if (m_aPersonal[nSlot].m_nCount) {
            CClientExoApp* pApp = g_pAppManager->m_pClientExoApp;
            CSWCCreature* pPC = pApp->GetParty()->GetPlayerCharacter();
            CSWGuiTargetAction* pAction = 0;
            if (m_anPersonalSelected[nSlot] != -1) {
                for (int i = 0; i < m_aPersonal[nSlot].m_nCount; i++) {
                    if (m_aPersonal[nSlot].m_pData[i].m_nId == m_anPersonalSelected[nSlot]) {
                        pAction = &m_aPersonal[nSlot].m_pData[i];
                        break;
                    }
                }
            }
            if (!pAction) {
                pAction = m_aPersonal[nSlot].m_pData;
            }
            if (pPC && pAction) {
                CGameObject* pObject = g_pAppManager->GetClientExoApp()->GetGameObject(pAction->m_oidTarget);
                if (pObject) {
                    CSWCActionTarget* pTarget = pObject->AsActionTarget();
                    if (pTarget) {
                        if (pAction->m_bEnabled && pAction->m_pfnAction) {
                            if (!(pPC->m_nCombatFlags & 1)) {
                                pPC->StartPersonalAction();
                                m_pManager->PlayGuiSound(6);
                            } else if (pApp->GetPauseOnPersonalAction()) {
                                pApp->GetInGameGui()->SetPaused(1, 10);
                            }
                            (pTarget->*pAction->m_pfnAction)(pAction->m_nId, pPC);
                            m_aPersonalActions[nSlot].StartFlash();
                            if (!(pPC->m_nCombatFlags & 1) && pPC->GetServerCreature()) {
                                pPC->GetServerCreature()->ClearAllActions(0);
                            }
                        } else {
                            if (pAction->m_nDisabledReason) {
                                switch (pAction->m_nDisabledReason) {
                                case 1: m_n70 = 0x96d5; break;
                                case 2: m_n70 = 0x96d6; break;
                                case 3: m_n70 = 0x96d7; break;
                                case 4: m_n70 = 0xa5b6; break;
                                case 5: m_n70 = 0xa602; break;
                                case 6: m_n70 = 0xbb40; break;
                                }
                                m_fTargetTime = 5.0f;
                            }
                            m_pManager->PlayGuiSound(2);
                        }
                    }
                }
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x0068AF70
void CSWGuiMainInterface::OnPersonalPrevClicked(CSWGuiControl* pControl)
{
    if (pControl->m_bMouseClick) {
        int nSlot = pControl->m_nId;
        if (g_pAppManager->m_pClientExoApp->GetOptions()->m_bTutorialPause) {
            g_pAppManager->m_pClientExoApp->SetTutorialPaused(1, 7);
        }
        g_pAppManager->m_pClientExoApp->ShowTutorialWindow(5, 0x7f000000, 0x7f000000, 0);
        SelectPrevPersonalAction(nSlot);
    }
}

// FUNCTION: SWKOTOR 0x0068AFE0
void CSWGuiMainInterface::OnPersonalNextClicked(CSWGuiControl* pControl)
{
    if (pControl->m_bMouseClick) {
        int nSlot = pControl->m_nId;
        if (g_pAppManager->m_pClientExoApp->GetOptions()->m_bTutorialPause) {
            g_pAppManager->m_pClientExoApp->SetTutorialPaused(1, 7);
        }
        g_pAppManager->m_pClientExoApp->ShowTutorialWindow(5, 0x7f000000, 0x7f000000, 0);
        SelectNextPersonalAction(nSlot);
    }
}

// FUNCTION: SWKOTOR 0x0068B050
void CSWGuiMainInterface::OnCancelCombatClicked(CSWGuiControl* pControl)
{
    if (pControl->m_bMouseClick && m_btnQueue0.m_bVisible) {
        if (!g_pAppManager->m_pClientExoApp->ShowTutorialWindow(0x20, 0x7f000000, 0x7f000000, 0)) {
            CancelCombat();
        }
    }
}

// FUNCTION: SWKOTOR 0x0068B0A0
void CSWGuiMainInterface::OnClearTargetClicked(CSWGuiControl* pControl)
{
    if (pControl->m_bMouseClick) {
        RefreshTargetActions();
    }
}

// FUNCTION: SWKOTOR 0x0068B170
CSWGuiMainInterface::~CSWGuiMainInterface()
{
    delete m_pExtra;
}

// FUNCTION: SWKOTOR 0x006858E0
void CSWGuiMainInterfaceAction::Update(float fDelta)
{
    if (m_nFlash) {
        m_fFlashTime -= fDelta;
        if (m_fFlashTime <= 0.0f) {
            m_nFlash--;
            m_fFlashTime = 0.1f;
        }
        float fAlpha = 1.0f;
        if (m_nFlash) {
            fAlpha = (0.1f - m_fFlashTime) * 10.0f;
            if (m_nFlash == 2) {
                fAlpha = 1.0f - fAlpha;
            }
        }
        m_btnIcon.m_cBorder.m_fAlpha = fAlpha;
        m_btnIcon.m_cHilite.m_fAlpha = fAlpha;
    }
}

// FUNCTION: SWKOTOR 0x0068B4A0
void CSWGuiMainInterface::Draw(float fDelta)
{
    if (!(m_nPanelFlags & 0x80)) {
        return;
    }
    CSWCCreature* pPC = g_pAppManager->m_pClientExoApp->GetParty()->GetPlayerCharacter();
    if (!pPC || pPC->m_idSelf == 0x7f000000) {
        return;
    }
    if (m_fTargetTime != -1.0f) {
        m_fTargetTime -= fDelta;
        if (m_fTargetTime <= 0.0f) {
            m_fTargetTime = -1.0f;
        }
    }
    UpdateTarget();
    UpdateParty(m_pManager->GetFrameTime());
    m_cTargetMenu.UpdateTimers(fDelta);
    UpdateActions();
    int i;
    for (i = 0; i < 9; i++) {
        m_aStatus[i].Update(m_pManager->GetFrameTime());
    }
    if (m_fSlideTime > 0.0f) {
        m_fSlideTime -= fDelta;
        if (m_fSlideTime > 0.0f) {
            for (i = 0; i < 3; i++) {
                m_aParty[i].SlideIn(m_fSlideTime);
            }
            m_cLeader.SlideIn(m_fSlideTime);
        } else {
            for (i = 0; i < 3; i++) {
                m_aParty[i].SlideIn(0.25f);
            }
            m_cLeader.SlideIn(0.25f);
            m_cLeader.Hide();
            m_fSlideTime = 0.0f;
        }
    }
    ShowActionQueue(g_pAppManager->m_pClientExoApp->GetCombatMode());
    for (i = 0; i < 6; i++) {
        m_aPersonalActions[i].Update(fDelta);
    }
    if (m_bQueueShown) {
        UpdateQueue();
    }
    if (g_bDrawGui) {
        DrawMap(fDelta);
        CSWGuiPanel::Draw(fDelta);
        m_cTargetMenu.Draw(fDelta);
        AurGUIStartLayer();
        if (AurGUISetupViewport(m_extent.left, m_extent.top, m_extent.width, m_extent.height,
                                &m_vColor, 0, m_fAlpha)) {
            for (i = m_lstControls.num - 1; i >= 0; i--) {
                CSWGuiControl* pControl = m_lstControls.element[i];
                if (pControl) {
                    pControl->Draw(fDelta);
                }
            }
            AurGUICloseViewport();
        }
        for (i = 0; i < 3; i++) {
            m_aParty[i].DrawStacks(fDelta, i == 0);
        }
        m_cLeader.DrawStacks(fDelta, 0);
        AurGUIStopLayer();
    }
}
