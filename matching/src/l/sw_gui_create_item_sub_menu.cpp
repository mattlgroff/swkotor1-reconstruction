// The debug item-creation sub menu CSWGuiCreateItemSubMenu (layout
// "debug", vtable 0x00756B30): a "Back" button followed by one button per
// UTI template of the chosen base item; picking one gives the item to the
// player creature.
#include "l/sw_gui_create_item_sub_menu.h"

inline CGuiInGameDebug* GetSubMenuGui()
{
    return (CGuiInGameDebug*)g_pAppManager->m_pClientExoApp->GetInGameGui();
}

// FUNCTION: SWKOTOR 0x006BD980
void CSWGuiCreateItemSubMenu::HandleInputEvent(int nEvent, int bDown)
{
    if (bDown) {
        switch (nEvent) {
        case 0x31:
        case 0x3d:
            if (m_nSelected == -0x63) {
                m_nSelected = -1;
            }
            if (m_nSelected > 0) {
                m_nSelected--;
            } else if (m_nSelected == 0) {
                m_nSelected = m_lstButtons.num - 1;
            }
            if (m_pManager) {
                m_pManager->PlayGuiSoundI(1);
            }
            break;
        case 0x32:
        case 0x3e:
            if (m_nSelected == -0x63) {
                m_nSelected = -1;
            }
            if (m_lstButtons.num > m_nSelected) {
                m_nSelected++;
            } else if (m_nSelected == m_lstButtons.num) {
                m_nSelected = 0;
            }
            if (m_pManager) {
                m_pManager->PlayGuiSoundI(1);
            }
            break;
        case 0x28:
        case 0x2e:
            GetSubMenuGui()->HideSubItemCreateMenu();
            break;
        }
    }
    CSWGuiPanel::HandleInputEvent(nEvent, bDown);
}

// FUNCTION: SWKOTOR 0x006BDA90
// NONMATCH: same `this` spill / register choice as the other debug menu
// CreateButton copies (8 insns).
CSWGuiButton* CSWGuiCreateItemSubMenu::CreateButton(const CExoString& sText,
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

// FUNCTION: SWKOTOR 0x006BDBA0
void CSWGuiCreateItemSubMenu::OnBack(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        GetSubMenuGui()->HideSubItemCreateMenu();
        GetSubMenuGui()->ShowItemCreateMenu();
    }
}

// FUNCTION: SWKOTOR 0x006BDBE0
void CSWGuiCreateItemSubMenu::OnItemSelected(CSWGuiControl* pControl)
{
    if (pControl->m_n4c) {
        CResRef cTemplate;
        CExoString* pName = m_lstResRefs.GetString(pControl->GetUserData() - 1);
        cTemplate = pName->CStr();
        CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
        pClient->CreatureAcquireItem(pClient->GetPlayerCreatureId(), cTemplate);
        GetSubMenuGui()->HideSubItemCreateMenu();
    }
}

// FUNCTION: SWKOTOR 0x006BDC60
CSWGuiCreateItemSubMenu::CSWGuiCreateItemSubMenu(CSWGuiManager* pManager)
    : CSWGuiPanel(pManager)
{
    m_nSelected = 0;
    StartLoadFromLayout("debug");
    InitControl(&m_lbOptions, "LB_OPTIONS", 1);
    InitControl(&m_lblBuild, "LBL_BUILD", 1);
    UnloadResource();
    CExoString sBuild = CExoString("Build: ") + g_sBuildNumber;
    m_lblBuild.m_cText.SetText(sBuild);
    CenterPanel();
}

// FUNCTION: SWKOTOR 0x006BDDE0 SYMBOL: ??_GCSWGuiCreateItemSubMenu@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x006BDE00 SYMBOL: ??1CSWGuiCreateItemSubMenu@@UAE@XZ

// FUNCTION: SWKOTOR 0x006BDEA0
void CSWGuiCreateItemSubMenu::LoadItems(int nBaseItem)
{
    CSWGuiButton* pProto = (CSWGuiButton*)m_lbOptions.m_pProtoItem;
    int bOwned = 0;
    m_lstNames.m_bSorted = 1;
    m_lstResRefs.m_bSorted = 1;
    CExoStringList* pFiles = g_pExoResMan->GetResOfType(0x7e9, 0);
    if (pFiles) {
        int nFiles = pFiles->GetCount();
        for (int i = 0; i < nFiles; i++) {
            CExoString* pFile = pFiles->GetString(i);
            CResGFF* pGFF = (CResGFF*)g_pExoResMan->GetResObject(pFile->CStr(), 0x7e9);
            if (!pGFF) {
                pGFF = new CResGFF;
                strncpy(pGFF->m_pFileType, "UTI ", 4);
                g_pExoResMan->SetResObject(pFile->CStr(), 0x7e9, pGFF);
                bOwned = 1;
            }
            if (pGFF && pGFF->Demand()) {
                CResRef cTemplate;
                CResStruct cTop;
                int bSuccess;
                pGFF->GetTopLevelStruct(&cTop);
                cTemplate = pGFF->ReadFieldCResRef(&cTop, "TemplateResRef", bSuccess, "BAD_BLUEPRINT");
                int nItem = pGFF->ReadFieldINT(&cTop, "BaseItem", bSuccess, 0);
                if (bSuccess && nItem == nBaseItem) {
                    CExoString sTemplate;
                    cTemplate.CopyToString(sTemplate);
                    m_lstNames.Add(new CExoString(sTemplate));
                    m_lstResRefs.Add(new CExoString(*pFile));
                }
                pGFF->Release();
                g_pExoResMan->ReleaseResObject(pGFF);
                if (bOwned) {
                    bOwned = 0;
                    if (pGFF) {
                        delete pGFF;
                    }
                }
            }
        }
        delete pFiles;
    }
    m_lstButtons.SetSizeFree(m_lstNames.GetCount() + 1);
    m_lstButtons[0] = CreateButton("Back", (PFN_GUI_CALLBACK)&CSWGuiCreateItemSubMenu::OnBack, pProto);
    for (int j = 0; j < m_lstNames.GetCount(); j++) {
        CExoString sName = *m_lstNames.GetString(j);
        m_lstButtons[j + 1] = CreateButton(sName, (PFN_GUI_CALLBACK)&CSWGuiCreateItemSubMenu::OnItemSelected, pProto);
    }
    m_lbOptions.AddControls(m_lstButtons, 1, 0, 0);
    CSWGuiListBox* pList = &m_lbOptions;
    pList->m_nListFlags |= 0x40;
    pList->SetSelectedControl(0, 0);
    m_nSelected = 0;
    SetActiveControl(pList, 0);
}

// FUNCTION: SWKOTOR 0x006BE2B0
void CSWGuiCreateItemSubMenu::ClearItems()
{
    m_lbOptions.ClearControls();
    m_lstButtons.SetSizeFree(0);
    m_lstResRefs.Clear();
    m_lstNames.Clear();
    m_nSelected = -0x63;
}
