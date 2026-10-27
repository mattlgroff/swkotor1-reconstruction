// CSWGuiScriptSelect (SWGuiScriptSelect.o): the party member AI script
// picker (layout "ScriptSelect", rows from aiscripts.2da).
#include "l/sw_gui_script_select.h"

// FUNCTION: SWKOTOR 0x006E9AC0
CSWGuiScriptSelect::~CSWGuiScriptSelect()
{
    m_lbDesc.ClearControls();
}

// FUNCTION: SWKOTOR 0x006E9BC0
void CSWGuiScriptSelect::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x27:
        case 0x2D: {
            unsigned long oidCreature = m_oidCreature;
            CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
            if (pCreature) {
                pCreature->m_pStats->m_nAIState =
                    (unsigned short)m_lstScripts.element[m_lbStates.GetSelectedControl()->m_n50].m_nAIState;
            }
            m_pManager->PlayGuiSound(0);
            m_pManager->PopModalPanel();
            SetToDestroy(0);
            break;
        }
        case 0x28:
        case 0x2E:
            m_pManager->PlayGuiSound(0);
            m_pManager->PopModalPanel();
            SetToDestroy(0);
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x006E9CA0
void CSWGuiScriptSelect::SetCreatureID(unsigned long oidCreature)
{
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    if (pCreature) {
        m_oidCreature = oidCreature;
        int nState = pCreature->m_pStats->m_nAIState;
        for (int i = 0; i < m_lstScripts.num; i++) {
            if (nState == m_lstScripts.element[i].m_nAIState) {
                m_lbStates.SetSelectedControl(i, 0);
                m_lbStates.GetControl(i)->GetButtonToggle()->SetToggleState(1);
            } else {
                m_lbStates.GetControl(i)->GetButtonToggle()->SetToggleState(0);
            }
        }
    } else {
        m_oidCreature = 0x7F000000;
    }
}

// FUNCTION: SWKOTOR 0x006E9D50
void CSWGuiScriptSelect::SetDescription(unsigned long nStrRef)
{
    STR_RES strRes;
    g_pTlkTable->Fetch(nStrRef, strRes, g_nGender);
    m_lblDesc.m_cText.SetText(strRes.sString);
    m_lbDesc.ClearControls();
    CSWGuiExtent extent = m_lblDesc.m_extent;
    if (strRes.sString.GetLength() > 0) {
        extent.height = m_lblDesc.m_cTextBox.GetHeight();
    } else {
        extent.height = m_lblDesc.m_cTextBox.GetLineHeight();
    }
    m_lblDesc.SetExtent(extent);
    CSWGuiControl* pLabel = &m_lblDesc;
    m_lbDesc.AddControls(&pLabel, 1, 0, 0, 0);
    m_lbDesc.SetActiveControl(&m_lblDesc, 0);
}

// FUNCTION: SWKOTOR 0x006E9E70
void CSWGuiScriptSelect::OnScriptSelected(CSWGuiControl* pControl)
{
    unsigned long oidCreature = m_oidCreature;
    CSWSCreature* pCreature = g_pAppManager->GetServerExoApp()->GetCreatureByGameObjectID(oidCreature);
    if (pCreature) {
        pCreature->m_pStats->m_nAIState =
            (unsigned short)m_lstScripts.element[m_lbStates.GetSelectedControl()->m_n50].m_nAIState;
    }
    m_pManager->PopModalPanel();
    m_nFlags &= ~0x100;
    if ((m_nFlags & 0x600) != 0x400) {
        m_nFlags = (m_nFlags & ~0x400) | 0x200;
    }
}

// FUNCTION: SWKOTOR 0x006E9EF0 SYMBOL: ??_GCSWGuiScriptSelect@@UAEPAXI@Z
// Emitted with the vtable in this object.

// FUNCTION: SWKOTOR 0x006E9F10
CSWGuiControl* CSWGuiScriptSelect::CreateOption(unsigned long nStrRef, PFN_GUI_CALLBACK pfnCallback,
                                                CSWGuiControl* pProto)
{
    CSWGuiZeroExtent extent;
    CSWGuiButtonToggle* pOption = new CSWGuiButtonToggle();
    pOption->CopyToggleStyle(pProto->m_extent, pProto);
    pOption->m_cTextParams.SetStrRef(nStrRef);
    extent.height = pProto->GetHeight();
    pOption->SetExtent(extent);
    pOption->AddEvent(0, this, pfnCallback);
    pOption->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiScriptSelect::OnScriptSelected);
    return pOption;
}

// FUNCTION: SWKOTOR 0x006E9FE0
void CSWGuiScriptSelect::OnEnterOption(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        SetDescription(m_lstScripts.element[pControl->m_n50].m_nDescription);
    }
}

// FUNCTION: SWKOTOR 0x006EA000
CSWGuiScriptSelect::CSWGuiScriptSelect(CSWGuiManager* pManager)
    : CSWGuiPanel(pManager)
{
    m_nC38 = 0;
    StartLoadFromLayout("ScriptSelect");
    InitControl(&m_lbStates, "LST_AIState", 1);
    InitControl(&m_lbDesc, "LB_DESC", 1);
    m_lbDesc.m_n2c1 = 1;
    InitControl(&m_lblTitle, "LBL_TITLE", 1);
    InitControl(&m_btnBack, "BTN_Back", 1);
    InitControl(&m_btnAccept, "BTN_Accept", 1);
    UnloadResource();
    m_lblDesc.CopyStyle(m_lbDesc.m_pProtoItem->m_extent, m_lbDesc.m_pProtoItem, 1.0f);
    CExoArrayListL<CSWGuiControl*> lstOptions;
    CSWGuiControl* pProto = m_lbStates.m_pProtoItem;
    C2DA* p2DA = new C2DA(CResRef("aiscripts"), 0);
    p2DA->Load2DArray();
    lstOptions.SetSizeFree(p2DA->m_nNumRows);
    m_lstScripts.SetSizeFree(p2DA->m_nNumRows);
    for (int nRow = 0; nRow < p2DA->m_nNumRows; nRow++) {
        int nName;
        int nDescription;
        int nState;
        p2DA->GetINTEntry(nRow, "NAME_STRREF", &nName);
        p2DA->GetINTEntry(nRow, "DESCRIPTION_STRREF", &nDescription);
        p2DA->GetINTEntry(nRow, "AISTATE", &nState);
        lstOptions[nRow] = CreateOption(nName, (PFN_GUI_CALLBACK)&CSWGuiScriptSelect::OnEnterOption, pProto);
        m_lstScripts.element[nRow].m_nDescription = nDescription;
        m_lstScripts.element[nRow].m_nAIState = nState;
    }
    delete p2DA;
    m_lbStates.AddControls(lstOptions, 1, 0, 0);
    CSWGuiButton* pBack = &m_btnBack;
    pBack->m_bNavigable = 0;
    pBack->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::HandleInputEvent28);
    CSWGuiButton* pAccept = &m_btnAccept;
    pAccept->m_bNavigable = 0;
    pAccept->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiPanel::v50);
    m_lbStates.SetListFlag0(1);
    SetActiveControl(&m_lbStates, 0);
}
