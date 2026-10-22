// the character abilities screen CSWGuiInGameAbilities (vtable
// 0x00755E50) and its list entry CSWGuiInGameSkillEntry (vtable 0x00755ED0),
// 0x006AB8B0-0x006ACCE0.
#include "l/sw_gui_in_game_abilities.h"

// FUNCTION: SWKOTOR 0x006AB8B0
void CSWGuiInGameAbilities::OnPanelRemoved()
{
    m_lbSkills.ClearControls();
    m_lstAbilities.Reset();
    m_lstSecond.Reset();
}

// FUNCTION: SWKOTOR 0x006AB8E0
void CSWGuiInGameSkillEntry::SetExtent(const CSWGuiExtent& extent)
{
    if (extent.width > 42) {
        m_extent = extent;
        CSWGuiExtent cIcon;
        cIcon.left = m_extent.left;
        cIcon.top = m_extent.top;
        cIcon.width = 42;
        cIcon.height = 42;
        m_cIconBorder.SetExtent(cIcon);
        m_cIconHilight.SetExtent(cIcon);
        m_cIcon.SetExtent(cIcon);
        cIcon.left += 42;
        cIcon.width = m_extent.width - 42;
        GetBorder().SetExtent(cIcon);
        m_cHilight.SetExtent(cIcon);
        m_cTextBox.SetExtent(IntersectExtents(GetBorder().GetInnerExtent(), m_cHilight.GetInnerExtent()));
    }
}

// FUNCTION: SWKOTOR 0x006AB9F0
void CSWGuiInGameSkillEntry::Draw(float fDelta)
{
    if (m_bFlag0) {
        m_cHilight.Draw(fDelta);
        m_cIconHilight.Draw(fDelta);
    } else {
        GetBorder().Draw(fDelta);
        m_cIconBorder.Draw(fDelta);
    }
    m_cIcon.Draw(fDelta);
    m_cTextBox.Draw(fDelta);
}

// FUNCTION: SWKOTOR 0x006ABA50
int CSWGuiAbilityList::GetSelectedId()
{
    if (m_lstChains.num > 0) {
        return m_lstChains.element[m_nPage]->m_aNodes[m_nRow].m_nId;
    }
    return -1;
}

// FUNCTION: SWKOTOR 0x006ABA80 SYMBOL: ??1CSWGuiInGameSkillEntry@@UAE@XZ

// FUNCTION: SWKOTOR 0x006ABAF0
CSWGuiInGameAbilities::~CSWGuiInGameAbilities()
{
    m_lbDesc.ClearControls();
    m_lbSkills.ClearControls();
}

// FUNCTION: SWKOTOR 0x006ABCE0
void CSWGuiInGameAbilities::RefreshList()
{
    if (m_pCreature) {
        CSWSCreatureStats* pStats = m_pCreature->AsServerCreature()->m_pStats;
        m_lstAbilities.SetCreature(pStats, 0);
        m_lstAbilities.SetMode(6);
        m_lstAbilities.Refresh();
    }
}

// FUNCTION: SWKOTOR 0x006ABD20
void CSWGuiInGameAbilities::UpdateDescriptionListBox(int bHasText)
{
    m_lbDesc.ClearControls();
    CSWGuiExtent extent = m_lblDesc.m_extent;
    if (bHasText) {
        extent.height = m_lblDesc.m_cTextBox.GetHeight();
    } else {
        extent.height = m_lblDesc.m_cTextBox.GetLineHeight();
    }
    m_lblDesc.SetExtent(extent);
    CSWGuiControl* pLabel = &m_lblDesc;
    m_lbDesc.AddControls(&pLabel, 1, 0, 0, 0);
    m_lbDesc.SetActiveControl(&m_lblDesc, 0);
}

// FUNCTION: SWKOTOR 0x006ABDC0
void CSWGuiInGameAbilities::SetDescription(unsigned long nStrRef)
{
    STR_RES strRes;
    g_pTlkTable->Fetch(nStrRef, strRes, g_nGender);
    m_lblDesc.m_cText.SetText(strRes.sString);
    UpdateDescriptionListBox(strRes.sString.GetLength() > 0);
}

// FUNCTION: SWKOTOR 0x006ABE70
int CSWGuiInGameAbilities::CanShowPowers()
{
    CSWCCreatureStats* pStats = m_pCreature->m_pStats;
    unsigned char nClass = pStats->GetClass(pStats->m_nNumClasses - 1);
    if (IsJediClass(nClass) && m_lstAbilities.m_lstChains.num > 0) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x006ACB40
void CSWGuiInGameSkillEntry::SetSkill(unsigned short nId, unsigned char nType)
{
    CResRef cIcon("inone");
    int nStrRef = 0;
    switch (nType) {
    case 0: {
        CSWSkill* pSkill = &g_pRules->m_pSkills[nId];
        if (pSkill) {
            cIcon = pSkill->m_sIcon;
            nStrRef = pSkill->m_nNameStrRef;
        }
        break;
    }
    case 1: {
        CSWFeat* pFeat = g_pRules->GetFeat(nId);
        if (pFeat) {
            cIcon = pFeat->m_cIcon;
            nStrRef = pFeat->m_nNameStrRef;
        }
        break;
    }
    case 2: {
        CSWSpell* pSpell = g_pRules->m_pSpellArray->GetSpell(nId);
        if (pSpell) {
            cIcon = pSpell->m_cIcon;
            nStrRef = pSpell->m_nNameStrRef;
        }
        break;
    }
    }
    CSWGuiBorderParams* pParams = &m_cIcon.m_cParams;
    pParams->m_nStyle = 1;
    pParams->SetFillImage(cIcon, 0);
    m_cText.SetStrRef(nStrRef);
}

// FUNCTION: SWKOTOR 0x006ACC30 SYMBOL: ??_GCSWGuiInGameAbilities@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x006ACC50
CSWGuiInGameSkillEntry::CSWGuiInGameSkillEntry()
{
}

// FUNCTION: SWKOTOR 0x006ACCC0 SYMBOL: ??_GCSWGuiInGameSkillEntry@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x006AD410
void CSWGuiInGameAbilities::ShowFeat(unsigned short nFeat)
{
    CSWFeat* pFeat = g_pRules->GetFeat(nFeat);
    SetDescriptionText(pFeat->GetDescriptionText());
    m_lbl2910.m_cText.SetStrRef(pFeat->m_nNameStrRef);
}

// FUNCTION: SWKOTOR 0x006AD4B0
void CSWGuiInGameAbilities::OnAbilitySelectionChanged(CSWGuiControl* pControl)
{
    CSWGuiAbilityChain* pRow = (CSWGuiAbilityChain*)m_lbSkills.GetSelectedControl();
    if (g_pAppManager->GetClientExoApp()->GetInGameGui()->m_nAbilityMode == 1) {
        unsigned short nIndex = pRow->HitTest(m_lbSkills.m_ptLocalMouse);
        m_lstAbilities.SetSelected(nIndex);
        ShowSkill(nIndex);
        return;
    }
    if (g_pAppManager->GetClientExoApp()->GetInGameGui()->m_nAbilityMode == 2) {
        unsigned short nIndex = pRow->HitTest(m_lbSkills.m_ptLocalMouse);
        m_lstSecond.SetSelected(nIndex);
        ShowFeat(nIndex);
    }
}

// FUNCTION: SWKOTOR 0x006AD960
void CSWGuiInGameAbilities::CreateSkillList()
{
    int nSkills = g_pRules->m_nNumSkills;
    m_lstSkills.num = 0;
    CSWSCreature* pServer = m_pCreature->AsServerCreature();
    int nEntry = 0;
    for (int i = 0; i < nSkills; i++) {
        CSWSkill* pSkill = &g_pRules->m_pSkills[(unsigned short)i];
        if (pSkill) {
            char nBase = pServer->m_pStats->GetSkillRank(i, 0, 1);
            char nTotal = pServer->m_pStats->GetSkillRank(i, 0, 0);
            CSWSkillListEntry entry;
            entry.pSkill = pSkill;
            entry.nBase = nBase;
            entry.nBonus = nTotal - nBase;
            m_lstSkills.Add(entry);
            m_aEntries[nEntry].SetSkill(i, 0);
            nEntry++;
        }
    }
    m_n3F70 = 0;
}

// FUNCTION: SWKOTOR 0x006ADA70
void CSWGuiInGameAbilities::OnShowFeats(CSWGuiControl* pControl)
{
    g_pAppManager->GetClientExoApp()->GetInGameGui()->m_nAbilityMode = 2;
    UpdateList();
}

// FUNCTION: SWKOTOR 0x006ADAA0
void CSWGuiInGameAbilities::OnShowPowers(CSWGuiControl* pControl)
{
    g_pAppManager->GetClientExoApp()->GetInGameGui()->m_nAbilityMode = 1;
    UpdateList();
}

// FUNCTION: SWKOTOR 0x006ADAD0
void CSWGuiInGameAbilities::OnShowSkills(CSWGuiControl* pControl)
{
    g_pAppManager->GetClientExoApp()->GetInGameGui()->m_nAbilityMode = 0;
    UpdateList();
}

// FUNCTION: SWKOTOR 0x006ADB00
void CSWGuiInGameAbilities::SetCharacter(CSWCCreature* pCreature)
{
    m_pCreature = pCreature;
    if (pCreature) {
        SetPortrait(&m_lbl1F10, ((CSWCCreatureVirtuals*)pCreature)->GetPortrait(1));
        m_lbSkills.ClearControls();
        if (m_pCreature) {
            m_lstSecond.SetCreatureB(m_pCreature->AsServerCreature()->m_pStats);
            m_lstSecond.SetMode(6);
            m_lstSecond.Refresh();
        }
        CreateSkillList();
        if (IsJediClass(m_pCreature->m_pStats->GetLastClass())) {
            RefreshList();
            if (m_lstAbilities.m_lstChains.num > 0) {
                m_btn2D54.m_bVisible = 1;
            }
        } else {
            m_btn2D54.m_bVisible = 0;
            if (g_pAppManager->GetClientExoApp()->GetInGameGui()->m_nAbilityMode == 1) {
                g_pAppManager->GetClientExoApp()->GetInGameGui()->m_nAbilityMode = 0;
            }
        }
        UpdateList();
    }
}

// FUNCTION: SWKOTOR 0x006ADC20
void CSWGuiInGameAbilities::OnPanelAdded()
{
    m_nPartyMember = -1;
    m_btn3860.m_n58 = -2;
    m_btn3A24.m_n58 = -2;
    SetCharacter((CSWCCreature*)g_pAppManager->GetClientExoApp()->GetParty()->GetMember(0));
    UpdatePortraits();
    m_btn369C.HandleMouseOver(0);
    CSWGuiPanel::OnPanelAdded();
}

// FUNCTION: SWKOTOR 0x006ADC80
void CSWGuiInGameAbilities::OnChangeCharacter(CSWGuiControl* pControl)
{
    int nDirection = 1;
    if (pControl->m_n4c) {
        CSWCCreature* pCreature;
        if (g_pAppManager->GetServerExoApp()->GetServerInfo()->m_bPartyMode) {
            char nMember = (char)pControl->m_n58;
            if (nMember == -1) {
                pCreature = g_pAppManager->GetClientExoApp()->GetPlayerCreature();
            } else {
                CClientExoApp* pClient = g_pAppManager->GetClientExoApp();
                pCreature = pClient->GetCreatureByGameObjectID(pClient->ServerToClientObjectId(
                    g_pAppManager->m_pServerExoApp->GetPartyTable()->GetNPCObject(nMember, 0, 1)));
            }
            if (!pCreature) {
                goto update;
            }
            if (m_btn3DAC.m_bVisible) {
                if (pControl == &m_btn3860) {
                    m_btn3860.m_n58 = m_btn3A24.m_n58;
                }
            } else {
                m_btn3860.m_n58 = -2;
            }
            m_nPartyMember = nMember;
            SetCharacter(pCreature);
        } else {
            if (pControl == &m_btn3A24) {
                nDirection = 2;
            }
            if (!g_pAppManager->GetClientExoApp()->ChangeCharacterToNextLivingPartyMember(0, 0, nDirection)) {
                return;
            }
            m_pManager->PlayGuiSound(1);
            SetCharacter(g_pAppManager->GetClientExoApp()->GetParty()->GetPlayerCharacter());
        }
update:
        UpdatePortraits();
    }
}
