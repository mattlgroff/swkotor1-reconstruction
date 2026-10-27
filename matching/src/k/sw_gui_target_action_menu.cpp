// CSWGuiMainInterfaceAction (one icon column of the target menu) and the
// main interface's target action menu (CSWGuiMainInterface +0xbc),
// 0x00684DF0-0x00686750.
#include "k/sw_gui_main_interface.h"

// FUNCTION: SWKOTOR 0x00684DF0
// NONMATCH: register allocation only (the original keeps nY in ebx, nX in
// ebp and the hit control in edi; direct returns and a pointer local tried).
CSWGuiControl* CSWGuiMainInterfaceAction::HitCheckMouse(int nX, int nY)
{
    CSWGuiControl* pControl = &m_btnUp;
    if (pControl->IsMouseOver(nX, nY)) {
        return pControl;
    }
    pControl = &m_btnDown;
    if (pControl->IsMouseOver(nX, nY)) {
        return pControl;
    }
    pControl = &m_btnIcon;
    if (pControl->IsMouseOver(nX, nY)) {
        return pControl;
    }
    if (m_btnFrame.IsMouseOver(nX, nY)) {
        return &m_btnFrame;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00685840
int CSWGuiMainInterfaceAction::IsActive()
{
    if (m_btnIcon.m_bActive || m_btnUp.m_bActive || m_btnDown.m_bActive ||
        m_btnFrame.m_bActive) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00685870
void CSWGuiMainInterfaceAction::Draw(float fDelta)
{
    if (m_btnFrame.m_bVisible) {
        m_btnFrame.Draw(fDelta);
    }
    if (m_btnUp.m_bVisible) {
        m_btnUp.Draw(fDelta);
    }
    if (m_btnDown.m_bVisible) {
        m_btnDown.Draw(fDelta);
    }
    if (m_btnIcon.m_bVisible) {
        m_btnIcon.Draw(fDelta);
    }
}

// FUNCTION: SWKOTOR 0x00685960
void CSWGuiMainInterfaceAction::SetIcon(const CResRef& cIcon, const CExoString& sName, int bEnabled, int a4)
{
    if (cIcon != m_btnIcon.m_cBorder.m_cFillImage) {
        m_btnIcon.m_cBorder.SetFillImage(cIcon, 0);
        m_btnIcon.m_cHilite.SetFillImage(cIcon, 0);
        m_nFlash = 0;
    }
    m_btnIcon.m_cBorder.m_fAlpha = bEnabled ? 1.0f : 0.25f;
    if (bEnabled) {
        m_btnIcon.m_cHilite.m_fAlpha = 1.0f;
    } else {
        m_btnIcon.m_cHilite.m_fAlpha = 0.25f;
    }
}

// FUNCTION: SWKOTOR 0x00684E70
void CSWGuiTargetActionMenu::UpdateTimers(float fDelta)
{
    if (m_fNameTimer != -1.0f) {
        m_fNameTimer -= fDelta;
        if (m_fNameTimer <= 0.0f) {
            m_fNameTimer = -1.0f;
        }
    }
    if (m_fHiliteTimer != -1.0f) {
        m_fHiliteTimer -= fDelta;
        if (m_fHiliteTimer <= 0.0f) {
            m_fHiliteTimer = -1.0f;
        }
    }
}

// FUNCTION: SWKOTOR 0x00684ED0
char CSWGuiTargetActionMenu::GetControlType(CSWGuiControl* pControl)
{
    if (pControl == &m_lblName || pControl == &m_pbHealth || pControl == &m_lblNameBG ||
        pControl == &m_lblBarBG) {
        return 1;
    }
    for (int i = 0; i < 3; i++) {
        if (pControl == &m_aActions[i].m_btnUp || pControl == &m_aActions[i].m_btnDown ||
            pControl == &m_aActions[i].m_btnIcon || pControl == &m_aActions[i].m_btnFrame) {
            return 2;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x006859E0
void CSWGuiTargetActionMenu::SetBars(int bFriend, int bPoison)
{
    CExoString sScroll;
    if (bFriend) {
        if (bPoison) {
            CResRef cBar("POISON_BAR");
            m_pbHealth.m_cProgress.SetFillImage(cBar, 0);
        } else {
            CResRef cBar("FRIEND_BAR");
            m_pbHealth.m_cProgress.SetFillImage(cBar, 0);
        }
        sScroll = "lbl_miscroll_f";
    } else {
        if (bPoison) {
            CResRef cBar("POISON_BAR");
            m_pbHealth.m_cProgress.SetFillImage(cBar, 0);
        } else {
            CResRef cBar("ENEMY_BAR");
            m_pbHealth.m_cProgress.SetFillImage(cBar, 0);
        }
        sScroll = "lbl_miscroll_h";
    }
    for (int i = 0; i < 3; i++) {
        CResRef cScroll(sScroll);
        m_aActions[i].m_btnFrame.m_cBorder.SetFillImage(cScroll, 0);
    }
}

// FUNCTION: SWKOTOR 0x00685AF0
void CSWGuiTargetActionMenu::SetNameLabel(const CExoString& sName)
{
    m_lblName.m_cText.SetText(sName);
    CSWGuiExtent ext = m_lblName.m_extent;
    ext.height = m_lblName.m_cTextBox.GetHeight();
    m_lblName.SetExtent(ext);
    m_lblNameBG.SetExtent(ext);
    int nBottom = ext.top + ext.height;
    ext = m_pbHealth.m_extent;
    ext.top = m_nBarGap + nBottom;
    m_pbHealth.SetExtent(ext);
    m_lblBarBG.SetExtent(ext);
    int nDelta = m_nActionGap - m_aActions[0].m_btnFrame.m_extent.top + nBottom;
    for (int i = 0; i < 3; i++) {
        m_aActions[i].m_btnIcon.MoveTop(nDelta);
        m_aActions[i].m_btnFrame.MoveTop(nDelta);
        m_aActions[i].m_btnUp.MoveTop(nDelta);
        m_aActions[i].m_btnDown.MoveTop(nDelta);
    }
}

// FUNCTION: SWKOTOR 0x00685ED0
void CSWGuiTargetActionMenu::Draw(float fDelta)
{
    if (m_bShown) {
        AurGUIStartLayer();
        if (AurGUISetupViewport(m_extent.left, m_extent.top, m_extent.width, m_extent.height,
                                &g_vGuiViewportColor, 0, 1.0f)) {
            m_lblBarBG.Draw(fDelta);
            m_lblNameBG.Draw(fDelta);
            m_lblName.Draw(fDelta);
            m_pbHealth.Draw(fDelta);
            for (int i = 0; i < 3; i++) {
                m_aActions[i].Draw(fDelta);
            }
            AurGUICloseViewport();
        }
        AurGUIStopLayer();
    }
}

// FUNCTION: SWKOTOR 0x00685F90
CSWGuiControl* CSWGuiTargetActionMenu::HitCheckMouse(int nX, int nY)
{
    CSWGuiControl* pHit = 0;
    if (m_bShown) {
        nX -= m_extent.left;
        nY -= m_extent.top;
        if (m_lblName.IsMouseOver(nX, nY)) {
            pHit = &m_lblName;
        } else if (m_pbHealth.IsMouseOver(nX, nY)) {
            pHit = &m_pbHealth;
        } else {
            int bAny = 0;
            int i;
            for (i = 0; i < 3 && !bAny; i++) {
                if (m_aLists[i].m_nCount) {
                    bAny = 1;
                }
            }
            if (bAny) {
                for (i = 0; i < 3 && !pHit; i++) {
                    pHit = m_aActions[i].HitCheckMouse(nX, nY);
                }
            }
        }
    }
    if (m_bMouseOver && !pHit) {
        m_fHiliteTimer = 0.5f;
    }
    m_bMouseOver = pHit != 0;
    return pHit;
}

// FUNCTION: SWKOTOR 0x006865B0
// NONMATCH: register allocation (list / entry / selected-slot registers
// rotated; pointer, reference and direct-index forms tried).
void CSWGuiTargetActionMenu::SelectNextAction(int nMenu)
{
    if (nMenu >= 0 && nMenu < 3) {
        CSWGuiTargetActionList* pList = &m_aLists[nMenu];
        if (pList->m_nCount) {
            g_pAppManager->m_pClientExoApp->GetParty()->GetPlayerCharacter();
            int& nSelected = m_anSelected[m_nSelectedRow][nMenu];
            CSWGuiTargetAction* pNext = 0;
            if (nSelected != -1) {
                for (int i = 0; i < pList->m_nCount; i++) {
                    if (pList->m_pData[i].m_nId == nSelected) {
                        if (i + 1 < pList->m_nCount) {
                            pNext = &pList->m_pData[i + 1];
                        } else {
                            pNext = pList->m_pData;
                        }
                        break;
                    }
                }
            }
            if (pList->m_nCount > 1) {
                if (!pNext) {
                    pNext = &pList->m_pData[1];
                }
                nSelected = pNext->m_nId;
                g_pAppManager->m_pClientExoApp->GetGuiManager()->PlayGuiSound(7);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x00686680
// NONMATCH: register allocation (list / entry / selected-slot registers
// rotated; pointer, reference and direct-index forms tried).
void CSWGuiTargetActionMenu::SelectPrevAction(int nMenu)
{
    if (nMenu >= 0 && nMenu < 3) {
        CSWGuiTargetActionList* pList = &m_aLists[nMenu];
        if (pList->m_nCount) {
            g_pAppManager->m_pClientExoApp->GetParty()->GetPlayerCharacter();
            int& nSelected = m_anSelected[m_nSelectedRow][nMenu];
            CSWGuiTargetAction* pPrev = 0;
            if (nSelected != -1) {
                for (int i = 0; i < pList->m_nCount; i++) {
                    if (pList->m_pData[i].m_nId == nSelected) {
                        if (i > 0) {
                            pPrev = &pList->m_pData[i - 1];
                        } else {
                            pPrev = &pList->m_pData[pList->m_nCount - 1];
                        }
                        break;
                    }
                }
            }
            if (pList->m_nCount > 1) {
                if (!pPrev) {
                    pPrev = &pList->m_pData[pList->m_nCount - 1];
                }
                nSelected = pPrev->m_nId;
                g_pAppManager->m_pClientExoApp->GetGuiManager()->PlayGuiSound(7);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x00689200
CSWGuiMainInterfaceAction::CSWGuiMainInterfaceAction()
{
    m_nFlash = 0;
    m_n718 = 0;
}

// FUNCTION: SWKOTOR 0x00689280
void CSWGuiMainInterfaceAction::SetFrame(int bShow, int bArrows)
{
    if (bShow) {
        CExoString sEmpty("");
        CResRef cEmpty("");
        if (cEmpty != m_btnIcon.m_cBorder.m_cFillImage) {
            m_btnIcon.m_cBorder.SetFillImage(cEmpty, 0);
            m_btnIcon.m_cHilite.SetFillImage(cEmpty, 0);
            m_nFlash = 0;
        }
        m_btnIcon.m_cBorder.m_fAlpha = 1.0f;
        m_btnIcon.m_cHilite.m_fAlpha = 1.0f;
    }
    m_btnFrame.m_cBorder.m_fAlpha = bShow ? 0.5f : 1.0f;
    m_btnFrame.m_cHilite.m_fAlpha = bShow ? 0.5f : 1.0f;
    m_btnUp.SetVisible(bArrows);
    m_btnDown.SetVisible(bArrows);
}

// FUNCTION: SWKOTOR 0x006893A0
CSWGuiMainInterfaceAction::~CSWGuiMainInterfaceAction()
{
}

// FUNCTION: SWKOTOR 0x0068B0C0
CSWGuiTargetActionMenu::~CSWGuiTargetActionMenu()
{
}

// FUNCTION: SWKOTOR 0x00689410
// NONMATCH: control flow and the inlined SetIcon agree; the list pointer
// lives in ebx (ebp holds this, then bEnabled) in the original.
void CSWGuiTargetActionMenu::PopulateMenus(CSWCCreature* pPC, int bDisabled, CSWCObject* pTarget)
{
    m_nSelectedRow = pPC->GetTargetInterfaceTargetType(pTarget);
    int i;
    for (i = 0; i < 3; i++) {
        for (int j = 0; j < m_aLists[i].m_nCount; j++) {
            m_aLists[i].m_pData[j].Reset();
        }
        m_aLists[i].m_nCount = 0;
        pPC->GetTargetActions(pTarget, i, &m_aLists[i]);
    }
    for (i = 0; i < 3; i++) {
        int bFound = 0;
        for (int j = 0; j < m_aLists[i].m_nCount; j++) {
            if (m_aLists[i].m_pData[j].m_nId == m_anSelected[m_nSelectedRow][i]) {
                m_aActions[i].SetFrame(0, m_aLists[i].m_nCount > 1);
                CSWGuiTargetAction* pAction = &m_aLists[i].m_pData[j];
                m_aActions[i].SetIcon(pAction->m_cIcon, pAction->m_sName, (pAction->m_nFlags & 1) && !bDisabled, 0);
                bFound = 1;
                break;
            }
        }
        if (!bFound) {
            if (m_aLists[i].m_nCount > 0) {
                m_aActions[i].SetFrame(0, m_aLists[i].m_nCount > 1);
                CSWGuiTargetAction* pAction = m_aLists[i].m_pData;
                m_aActions[i].SetIcon(pAction->m_cIcon, pAction->m_sName, (pAction->m_nFlags & 1) && !bDisabled, 0);
            } else {
                m_aActions[i].SetFrame(1, 0);
            }
        }
    }
    SetTarget(pTarget);
    UpdateActions(pPC, pTarget);
}

// FUNCTION: SWKOTOR 0x00685CB0
// NONMATCH: frame slots and register choice differ (pTarget in esi, the
// object in ebx), and the original moves the GetDisplayName arm after the
// epilogue; behaviour agrees.
void CSWGuiTargetActionMenu::SetTarget(CSWCObject* pTarget)
{
    CExoString sName;
    if (pTarget) {
        CSWCObject* pObject = pTarget->GetClientObject();
        if (pObject) {
            CExoString sInfo;
            int bSkip;
            pTarget->GetNameInfo(&sInfo, &bSkip);
            if (!bSkip) {
                if (m_fNameTimer > 0.0f) {
                    sName = g_pAppManager->GetClientExoApp()->GetGUIString(m_nNameStrRef);
                } else {
                    int nMenu = m_nSelectedMenu;
                    if (m_aActions[nMenu].IsActive() && nMenu >= 0 && m_aLists[nMenu].m_nCount > 0) {
                        CSWGuiTargetActionList* pList = &m_aLists[nMenu];
                        int nSelected = m_anSelected[m_nSelectedRow][nMenu];
                        CSWGuiTargetAction* pAction = 0;
                        if (nSelected != -1) {
                            for (int i = 0; i < pList->m_nCount; i++) {
                                if (pList->m_pData[i].m_nId == nSelected) {
                                    pAction = &pList->m_pData[i];
                                }
                            }
                        }
                        if (!pAction) {
                            pAction = pList->m_pData;
                        }
                        sName = pAction->m_sName;
                        int nCount = pAction->m_nCount;
                        if (nCount > 1) {
                            sName.Format("%s (%d)", sName.CStr(), nCount);
                        }
                    } else {
                        sName = pTarget->GetDisplayName();
                    }
                }
            }
            m_pbHealth.SetMaxValue(pObject->GetMaxHitPoints(1));
            m_pbHealth.SetCurValue(pObject->GetCurrentHitPoints(0));
        }
    }
    SetNameLabel(sName);
    m_lblName.m_cText.m_fAlpha = 1.0f;
}

// NONMATCH (7): load scheduling only; the original reads the action frame's
// top byte before storing the width. Inline, hoisted-local and reordered
// statement shapes do not reproduce it.
// FUNCTION: SWKOTOR 0x0068BF50
void CSWGuiTargetActionMenu::Initialize(CSWGuiPanel* pPanel)
{
    m_pPanel = pPanel;
    for (int i = 0; i < 3; i++) {
        m_aActions[i].Initialize(pPanel, i, 1);
    }
    pPanel->InitControl(&m_lblName, "LBL_NAME", 0);
    pPanel->InitControl(&m_lblNameBG, "LBL_NAMEBG", 0);
    pPanel->InitControl(&m_lblBarBG, "LBL_HEALTHBG", 0);
    pPanel->InitControl(&m_pbHealth, "PB_HEALTH", 0);
    m_extent.width = m_lblName.m_extent.width;
    unsigned char nBottom = m_lblName.m_extent.height + m_lblName.m_extent.top;
    m_nBarGap = m_pbHealth.m_extent.top - nBottom;
    m_nActionGap = m_aActions[0].m_btnFrame.m_extent.top - nBottom;
    SetNameLabel("");
}

// FUNCTION: SWKOTOR 0x00689D80
// NONMATCH: same family of ties as PopulateMenus: this / pPC frame slots
// swapped, bDisabled kept in ebx and the list walked from +0x78 instead of
// +0x74. Control flow, inlined SetIcon and calls agree; a list pointer
// local made the register choice worse.
void CSWGuiMainInterface::UpdateActions()
{
    CSWCCreature* pPC = g_pAppManager->GetClientExoApp()->GetParty()->GetPlayerCharacter();
    CSWSCreature* pServer = pPC ? pPC->GetServerCreature() : 0;
    CGameObject* pObject = g_pAppManager->GetClientExoApp()->GetGameObject(m_nTargetId);
    CSWCObject* pTarget = 0;
    int bDisabled = 1;
    if (!pPC) {
        return;
    }
    if (pServer && !pServer->GetDead() && !pServer->GetDeadTemp()) {
        bDisabled = 0;
    }
    if (pPC->m_idSelf != m_n68) {
        m_n68 = pPC->m_idSelf;
    }
    if (pObject) {
        // Slot 0x0c returns the client object (AsSWCObject).
        pTarget = (CSWCObject*)pObject->AsActionTarget();
    }
    int i;
    for (i = 0; i < 6; i++) {
        for (int j = 0; j < m_aPersonal[i].m_nCount; j++) {
            m_aPersonal[i].m_pData[j].Reset();
        }
        m_aPersonal[i].m_nCount = 0;
        pPC->GetPersonalActions(i, m_aPersonal[i]);
    }
    m_cTargetMenu.PopulateMenus(pPC, bDisabled, pTarget);
    for (i = 0; i < 4; i++) {
        int bFound = 0;
        for (int j = 0; j < m_aPersonal[i].m_nCount; j++) {
            if (m_aPersonal[i].m_pData[j].m_nId == m_anPersonalSelected[i]) {
                m_aPersonalActions[i].SetFrame(0, m_aPersonal[i].m_nCount > 1);
                CSWGuiTargetAction* pAction = &m_aPersonal[i].m_pData[j];
                m_aPersonalActions[i].SetIcon(pAction->m_cIcon, pAction->m_sName,
                                              pAction->m_bEnabled && !bDisabled, 0);
                bFound = 1;
                break;
            }
        }
        if (!bFound) {
            if (m_aPersonal[i].m_nCount > 0) {
                m_aPersonalActions[i].SetFrame(0, m_aPersonal[i].m_nCount > 1);
                CSWGuiTargetAction* pAction = m_aPersonal[i].m_pData;
                m_aPersonalActions[i].SetIcon(pAction->m_cIcon, pAction->m_sName,
                                              pAction->m_bEnabled && !bDisabled, 0);
            } else {
                m_aPersonalActions[i].SetFrame(1, 0);
            }
        }
    }
    UpdateMessage();
}

// FUNCTION: SWKOTOR 0x00689610
// NONMATCH: register allocation only: the original keeps the list pointer
// on the stack and walks the search with the index in esi (later the
// action pointer); ours holds the list in esi. Member, pointer and inline
// search shapes tried (an inline search helper is not inlined).
void CSWGuiTargetActionMenu::UseAction(int nSlot, int a2)
{
    CSWGuiTargetActionList* pList = &m_aLists[nSlot];
    if (pList->m_nCount) {
        CClientExoApp* pApp = g_pAppManager->m_pClientExoApp;
        CSWCCreature* pPC = pApp->GetParty()->GetPlayerCharacter();
        CSWGuiTargetAction* pAction = 0;
        int nSelected = m_anSelected[m_nSelectedRow][nSlot];
        if (nSelected != -1) {
            for (int i = 0; i < pList->m_nCount; i++) {
                if (pList->m_pData[i].m_nId == nSelected) {
                    pAction = &pList->m_pData[i];
                    break;
                }
            }
        }
        if (!pAction) {
            pAction = pList->m_pData;
        }
        if (pPC && pAction) {
            CGameObject* pObject = g_pAppManager->GetClientExoApp()->GetGameObject(pAction->m_oidTarget);
            if (pObject) {
                CSWCActionTarget* pTarget = pObject->AsActionTarget();
                if (pTarget) {
                    if (pAction->m_bEnabled && pAction->m_pfnAction) {
                        int bAttack = 0;
                        if (!(pPC->m_nCombatFlags & 1)) {
                            if (!pApp->GetCombatMode() && m_nSelectedRow == 3) {
                                bAttack = 1;
                            }
                            pPC->StartPersonalAction();
                            if ((m_nSelectedRow == 0 && pAction->m_nId == 0x3f5) || m_nSelectedRow == 3) {
                                CSWGuiMainInterface* pMain = (CSWGuiMainInterface*)m_pPanel;
                                pMain->m_fQueueTime = -1.0f;
                                ((CSWGuiMainInterface*)m_pPanel)->SetCombatMessage(0xbc50);
                            }
                        } else if (pApp->GetPauseOnPersonalAction()) {
                            pApp->GetInGameGui()->SetPaused(1, 10);
                        }
                        g_pAppManager->m_pClientExoApp->GetGuiManager()->PlayGuiSound(6);
                        (pTarget->*pAction->m_pfnAction)(pAction->m_nId, pPC);
                        m_aActions[nSlot].StartFlash();
                        if (!(pPC->m_nCombatFlags & 1)) {
                            if (bAttack) {
                                pPC->SetAttackTarget(1);
                            }
                            if (pPC->GetServerCreature()) {
                                pPC->GetServerCreature()->ClearAllActions(0);
                            }
                        }
                    } else {
                        if (pAction->m_nDisabledReason) {
                            switch (pAction->m_nDisabledReason) {
                            case 1: m_nNameStrRef = 0x96d5; break;
                            case 2: m_nNameStrRef = 0x96d6; break;
                            case 3: m_nNameStrRef = 0x96d7; break;
                            case 4: m_nNameStrRef = 0xa5b6; break;
                            case 5: m_nNameStrRef = 0xa602; break;
                            case 6: m_nNameStrRef = 0xbb40; break;
                            }
                            m_fNameTimer = 5.0f;
                        }
                        g_pAppManager->m_pClientExoApp->GetGuiManager()->PlayGuiSound(2);
                    }
                }
            }
        }
    }
}
