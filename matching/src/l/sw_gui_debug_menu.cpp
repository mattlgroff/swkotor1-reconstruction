// the powers/feats/skills debug menu
// CSWGuiPowersFeatsSkillsDebugMenu (layout "debug", vtable 0x00757A78): a
// list of "Add Power" / "Add Skill" / "Add Feat" buttons whose sub-lists
// grant the player creature a power (with its prerequisites), a feat (with
// its prerequisites) or three skill ranks.
#include "l/sw_gui_debug_menu.h"

// FUNCTION: SWKOTOR 0x006CEA90
// NONMATCH: the original spills `this` and keeps &m_cBorderParams in ebx
// (ours keeps `this` in ebx; 8 insns).
CSWGuiButton* CSWGuiPowersFeatsSkillsDebugMenu::CreateButton(const CExoString& sText,
                                                             PFN_GUI_CALLBACK pfnCallback,
                                                             CSWGuiButton* pProto)
{
    CSWGuiZeroExtent extent;
    CSWGuiButton* pButton = new CSWGuiButton;
    pButton->CopyStyle(extent, pProto);
    pButton->m_cTextParams.SetText(sText);
    CSWGuiBorderParams* pCorner = pButton->m_cHilightParams.GetCornerSize() > pButton->m_cBorderParams.GetCornerSize()
                                      ? &pButton->m_cHilightParams
                                      : &pButton->m_cBorderParams;
    int nCorner = pCorner->GetCornerSize();
    CSWGuiLabelText* pText = &pButton->m_cTextBox;
    extent.height = pText->GetHeight() + nCorner * 2;
    pButton->SetExtent(extent);
    pButton->AddEvent(0x27, this, pfnCallback);
    pButton->AddEvent(0x2d, this, pfnCallback);
    return pButton;
}

// FUNCTION: SWKOTOR 0x006CEBA0
// NONMATCH: pCreature / prerequisite pointer get swapped registers (edi/ebx).
void CSWGuiPowersFeatsSkillsDebugMenu::AddSpell(int nSpell, CSWSCreature* pCreature)
{
    CSWSpell* pSpell = g_pRules->m_pSpellArray->GetSpellI(nSpell);
    int nClass = pCreature->m_pStats->m_nNumClasses - 1;
    if (pSpell) {
        for (int i = 0; pSpell->m_anPrereqs[i] != -1; i++) {
            int nPrereq = pSpell->m_anPrereqs[i];
            if (!pCreature->m_pStats->HasSpell(0, nPrereq, 1)) {
                AddSpell(nPrereq, pCreature);
            }
        }
        pCreature->m_pStats->AddKnownSpell(nClass, nSpell);
    }
}

// FUNCTION: SWKOTOR 0x006CEC40
void CSWGuiPowersFeatsSkillsDebugMenu::AddFeat(unsigned short nFeat, CSWSCreature* pCreature)
{
    CSWFeat* pFeat = g_pRules->GetFeat(nFeat);
    if (pFeat && pCreature) {
        int nPrereq2 = pFeat->m_nPrereq2;
        int nPrereq1 = pFeat->m_nPrereq1;
        if (nPrereq1 != 0xffff && !pCreature->m_pStats->HasFeat(nPrereq1)) {
            AddFeat(nPrereq1, pCreature);
        }
        if (nPrereq2 != 0xffff && !pCreature->m_pStats->HasFeat(nPrereq2)) {
            AddFeat(nPrereq2, pCreature);
        }
        pCreature->m_pStats->AddFeat(nFeat);
    }
}

// FUNCTION: SWKOTOR 0x006CECD0
void CSWGuiPowersFeatsSkillsDebugMenu::OnSkillSelected(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        int nSkill = GetOptionIndex(pControl);
        CSWSCreature* pCreature = GetPlayerCreature();
        if (pCreature) {
            CSWSCreatureStats* pStats = pCreature->m_pStats;
            pStats->SetSkillRank(nSkill, pStats->GetSkillRankBase(0) + 3);
        }
        ShowMain();
        HideMenu();
    }
}

// FUNCTION: SWKOTOR 0x006CED60
// NONMATCH: the list box vptr load is scheduled after the m_nMode store
// (the original loads it first; 5 insns).
void CSWGuiPowersFeatsSkillsDebugMenu::ShowSkills()
{
    m_nSelected = 0;
    m_lbOptions.ClearControls();
    CExoArrayListL<CSWGuiControl*> lstButtons;
    CSWGuiButton* pProto = (CSWGuiButton*)m_lbOptions.m_pProtoItem;
    int nSkills = g_pRules->GetNumSkills();
    lstButtons.SetSize(nSkills);
    for (int i = 0; i < nSkills; i++) {
        CExoString sText("Add ");
        sText = sText + g_pRules->m_pSkills[(unsigned short)i].GetNameText();
        lstButtons[i] = CreateButton(sText, (PFN_GUI_CALLBACK)&CSWGuiPowersFeatsSkillsDebugMenu::OnSkillSelected, pProto);
    }
    FinishList(lstButtons, 2);
}

// FUNCTION: SWKOTOR 0x006CEF00
void CSWGuiPowersFeatsSkillsDebugMenu::OnPowerSelected(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        int nSpell = pControl->m_n58;
        CSWSpell* pSpell = g_pRules->m_pSpellArray->GetSpellI(nSpell);
        CSWSCreature* pCreature = GetPlayerCreature();
        if (pSpell && pCreature) {
            AddSpell(nSpell, pCreature);
        }
        ShowMain();
        HideMenu();
    }
}

// FUNCTION: SWKOTOR 0x006CEF80
void CSWGuiPowersFeatsSkillsDebugMenu::OnFeatSelected(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        int nIndex = GetOptionIndex(pControl);
        int nFeat = g_pRules->GetFeatId(nIndex);
        CSWFeat* pFeat = g_pRules->GetFeat(nFeat);
        CSWSCreature* pCreature = GetPlayerCreature();
        if (pFeat && pCreature) {
            AddFeat(nFeat, pCreature);
        }
        ShowMain();
        HideMenu();
    }
}

// FUNCTION: SWKOTOR 0x006CF020
void CSWGuiPowersFeatsSkillsDebugMenu::OnShowSkills(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        ShowSkills();
    }
}

// FUNCTION: SWKOTOR 0x006CF040
// NONMATCH: the constant 1 is kept in ebx and pProto spilled (the original
// keeps pProto in ebx).
void CSWGuiPowersFeatsSkillsDebugMenu::ShowPowers()
{
    m_nSelected = 0;
    m_lbOptions.ClearControls();
    CExoArrayListL<CSWGuiControl*> lstButtons;
    int nSpells = g_pRules->m_pSpellArray->m_nNumSpells;
    CSWGuiButton* pProto = (CSWGuiButton*)m_lbOptions.m_pProtoItem;
    for (int i = 0; i < nSpells; i++) {
        CSWSpell* pSpell = g_pRules->m_pSpellArray->GetSpellI(i);
        if (pSpell->m_bUsable == 1 && pSpell->m_nNameStrRef) {
            CExoString sText("Add ");
            sText = sText + g_pRules->m_pSpellArray->GetSpellI(i)->GetSpellNameText();
            CSWGuiButton* pButton = CreateButton(sText, (PFN_GUI_CALLBACK)&CSWGuiPowersFeatsSkillsDebugMenu::OnPowerSelected, pProto);
            pButton->m_n58 = i;
            lstButtons.AddAlloc(pButton);
        }
    }
    FinishList(lstButtons, 1);
}

// FUNCTION: SWKOTOR 0x006CF220
// NONMATCH: same m_nMode store / vptr load scheduling as ShowSkills.
void CSWGuiPowersFeatsSkillsDebugMenu::ShowFeats()
{
    m_nSelected = 0;
    m_lbOptions.ClearControls();
    CExoArrayListL<CSWGuiControl*> lstButtons;
    CSWGuiButton* pProto = (CSWGuiButton*)m_lbOptions.m_pProtoItem;
    int nFeats = g_pRules->m_nNumFeatIds;
    lstButtons.SetSize(nFeats);
    for (int i = 0; i < nFeats; i++) {
        CExoString sText("Add ");
        sText = sText + g_pRules->GetFeat(g_pRules->GetFeatId(i))->GetNameText();
        lstButtons[i] = CreateButton(sText, (PFN_GUI_CALLBACK)&CSWGuiPowersFeatsSkillsDebugMenu::OnFeatSelected, pProto);
    }
    FinishList(lstButtons, 3);
}

// FUNCTION: SWKOTOR 0x006CF3D0
void CSWGuiPowersFeatsSkillsDebugMenu::OnShowPowers(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        ShowPowers();
    }
}

// FUNCTION: SWKOTOR 0x006CF3F0
void CSWGuiPowersFeatsSkillsDebugMenu::OnShowFeats(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        ShowFeats();
    }
}

// FUNCTION: SWKOTOR 0x006CF410
// NONMATCH: same m_nMode store / vptr load scheduling as ShowSkills.
void CSWGuiPowersFeatsSkillsDebugMenu::ShowMain()
{
    m_nSelected = 0;
    m_lbOptions.ClearControls();
    CExoArrayListL<CSWGuiControl*> lstButtons;
    CSWGuiButton* pProto = (CSWGuiButton*)m_lbOptions.m_pProtoItem;
    lstButtons.SetSize(3);
    lstButtons[0] = CreateButton("Add Power", (PFN_GUI_CALLBACK)&CSWGuiPowersFeatsSkillsDebugMenu::OnShowPowers, pProto);
    lstButtons[1] = CreateButton("Add Skill", (PFN_GUI_CALLBACK)&CSWGuiPowersFeatsSkillsDebugMenu::OnShowSkills, pProto);
    lstButtons[2] = CreateButton("Add Feat", (PFN_GUI_CALLBACK)&CSWGuiPowersFeatsSkillsDebugMenu::OnShowFeats, pProto);
    FinishList(lstButtons, 0);
}

// FUNCTION: SWKOTOR 0x006CF5A0
CSWGuiPowersFeatsSkillsDebugMenu::CSWGuiPowersFeatsSkillsDebugMenu(CSWGuiManager* pManager)
    : CSWGuiPanel(pManager)
{
    StartLoadFromLayout("debug");
    InitControl(&m_lbOptions, "LB_OPTIONS", 1);
    InitControl(&m_lblBuild, "LBL_BUILD", 1);
    UnloadResource();
    CExoString sBuild = CExoString("Build: ") + g_sBuildNumber;
    m_lblBuild.m_cText.SetText(sBuild);
    ShowMain();
    CenterPanel();
}

// FUNCTION: SWKOTOR 0x006CF6F0 SYMBOL: ??_GCSWGuiPowersFeatsSkillsDebugMenu@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x006CF710 SYMBOL: ??1CSWGuiPowersFeatsSkillsDebugMenu@@UAE@XZ

// FUNCTION: SWKOTOR 0x006CF770
void CSWGuiPowersFeatsSkillsDebugMenu::OnBack()
{
    if (m_nMode == 0) {
        HideMenu();
        ((CGuiInGameDebug*)g_pAppManager->m_pClientExoApp->GetInGameGui())->RefreshDebug();
    } else {
        ShowMain();
    }
}

// FUNCTION: SWKOTOR 0x006CF7B0
void CSWGuiPowersFeatsSkillsDebugMenu::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x31:
        case 0x3d:
            if (m_nSelected > 0) {
                m_nSelected--;
            } else if (m_nSelected == 0) {
                m_nSelected = m_nOptions - 1;
            }
            if (m_pManager) {
                m_pManager->PlayGuiSound(1);
            }
            break;
        case 0x32:
        case 0x3e:
            if (m_nSelected < m_nOptions) {
                m_nSelected++;
            } else if (m_nSelected == m_nOptions - 1) {
                m_nSelected = 0;
            }
            if (m_pManager) {
                m_pManager->PlayGuiSound(1);
            }
            break;
        case 0x28:
        case 0x2e:
            OnBack();
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// ---------------------------------------------------------------------------
// CSWGuiLoadModuleDebugMenu (vtable 0x00757AE8)

// FUNCTION: SWKOTOR 0x006CF890
// NONMATCH: same `this` spill / register choice as the powers menu CreateButton.
CSWGuiButton* CSWGuiLoadModuleDebugMenu::CreateButton(const CExoString& sText,
                                                      PFN_GUI_CALLBACK pfnCallback,
                                                      CSWGuiButton* pProto)
{
    CSWGuiZeroExtent extent;
    CSWGuiButton* pButton = new CSWGuiButton;
    pButton->CopyStyle(extent, pProto);
    pButton->m_cTextParams.SetText(sText);
    CSWGuiBorderParams* pCorner = pButton->m_cHilightParams.GetCornerSize() > pButton->m_cBorderParams.GetCornerSize()
                                      ? &pButton->m_cHilightParams
                                      : &pButton->m_cBorderParams;
    int nCorner = pCorner->GetCornerSize();
    CSWGuiLabelText* pText = &pButton->m_cTextBox;
    extent.height = pText->GetHeight() + nCorner * 2;
    pButton->SetExtent(extent);
    pButton->AddEvent(0x2d, this, pfnCallback);
    pButton->AddEvent(0x27, this, pfnCallback);
    return pButton;
}

// FUNCTION: SWKOTOR 0x006CF9A0
void CSWGuiLoadModuleDebugMenu::Close()
{
    GetDebugGui()->HideLoadModuleDebugMenu();
    GetDebugGui()->RefreshDebug();
}

// FUNCTION: SWKOTOR 0x006CF9D0
void CSWGuiLoadModuleDebugMenu::OnModuleSelected(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        GetDebugGui()->HideLoadModuleDebugMenu();
        CExoString sModule = ((CSWGuiButton*)pControl)->m_cTextParams.m_sText;
        g_pAppManager->GetServerExoApp()->SetMoveToModuleString(sModule);
        g_pAppManager->m_pServerExoApp->SetMoveToModulePending(1);
    }
}

CSWGuiLoadModuleDebugMenu::CSWGuiLoadModuleDebugMenu(CSWGuiManager* pManager, int bUnused)
    : CSWGuiPanel(pManager)
{
}

// FUNCTION: SWKOTOR 0x006D0240 SYMBOL: ??_GCSWGuiLoadModuleDebugMenu@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x006D0260 SYMBOL: ??1CSWGuiLoadModuleDebugMenu@@UAE@XZ

// FUNCTION: SWKOTOR 0x006D02C0
void CSWGuiLoadModuleDebugMenu::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x31:
        case 0x3d:
            if (m_nSelected > 0) {
                m_nSelected--;
            } else if (m_nSelected == 0) {
                m_nSelected = m_nOptions - 1;
            }
            if (m_pManager) {
                m_pManager->PlayGuiSound(1);
            }
            break;
        case 0x32:
        case 0x3e:
            if (m_nSelected < m_nOptions) {
                m_nSelected++;
            } else if (m_nSelected == m_nOptions - 1) {
                m_nSelected = 0;
            }
            if (m_pManager) {
                m_pManager->PlayGuiSound(1);
            }
            break;
        case 0x28:
        case 0x2e:
            Close();
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// ---------------------------------------------------------------------------
// CSWGuiItemCreateMenu (vtable 0x00757B68)

// FUNCTION: SWKOTOR 0x006D03A0
void CSWGuiItemCreateMenu::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x31:
        case 0x3d:
            if (m_nSelected > 0) {
                m_nSelected--;
            } else if (m_nSelected == 0) {
                m_nSelected = g_pRules->m_pBaseItems->m_nNumBaseItems;
            }
            if (m_pManager) {
                m_pManager->PlayGuiSound(1);
            }
            break;
        case 0x32:
        case 0x3e:
            if (g_pRules->m_pBaseItems->m_nNumBaseItems > m_nSelected) {
                m_nSelected++;
            } else if (m_nSelected == g_pRules->m_pBaseItems->m_nNumBaseItems) {
                m_nSelected = 0;
            }
            if (m_pManager) {
                m_pManager->PlayGuiSound(1);
            }
            break;
        case 0x28:
        case 0x2e:
            GetDebugGui()->HideItemCreateMenu();
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x006D0490
// NONMATCH: same `this` spill / register choice as the powers menu CreateButton.
CSWGuiButton* CSWGuiItemCreateMenu::CreateButton(const CExoString& sText,
                                                 PFN_GUI_CALLBACK pfnCallback,
                                                 CSWGuiButton* pProto)
{
    CSWGuiZeroExtent extent;
    CSWGuiButton* pButton = new CSWGuiButton;
    pButton->CopyStyle(extent, pProto);
    pButton->m_cTextParams.SetText(sText);
    CSWGuiBorderParams* pCorner = pButton->m_cHilightParams.GetCornerSize() > pButton->m_cBorderParams.GetCornerSize()
                                      ? &pButton->m_cHilightParams
                                      : &pButton->m_cBorderParams;
    int nCorner = pCorner->GetCornerSize();
    CSWGuiLabelText* pText = &pButton->m_cTextBox;
    extent.height = pText->GetHeight() + nCorner * 2;
    pButton->SetExtent(extent);
    pButton->AddEvent(0x27, this, pfnCallback);
    pButton->AddEvent(0x2d, this, pfnCallback);
    return pButton;
}

// FUNCTION: SWKOTOR 0x006D05A0
void CSWGuiItemCreateMenu::OnBack(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        GetDebugGui()->HideItemCreateMenu();
        GetDebugGui()->RefreshDebug();
    }
}

// FUNCTION: SWKOTOR 0x006D05E0
void CSWGuiItemCreateMenu::OnItemSelected(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        GetDebugGui()->HideItemCreateMenu();
        ((CGuiInGameDebug*)g_pAppManager->GetClientExoApp()->GetInGameGui())->ShowSubItemCreateMenu(pControl->GetUserData() - 1);
    }
}

CSWGuiItemCreateMenu::CSWGuiItemCreateMenu(CSWGuiManager* pManager, int bUnused)
    : CSWGuiPanel(pManager)
{
}

// FUNCTION: SWKOTOR 0x006D09C0 SYMBOL: ??_GCSWGuiItemCreateMenu@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x006D09E0 SYMBOL: ??1CSWGuiItemCreateMenu@@UAE@XZ
