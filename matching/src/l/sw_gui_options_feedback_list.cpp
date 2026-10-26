// Feedback options: builds the checkbox list from the list box prototype.
// The checkbox constructor is inline here, as in the original.
#define L_OPTIONS_CHECKBOX_INLINE_CTOR
#include "l/sw_gui_options_in_game.h"
#include "l/sw_gui_in_game_abilities.h"

// FUNCTION: SWKOTOR 0x006E1300
void CSWGuiOptionsFeedback::PopulateOptionsListbox()
{
    CSWGuiControl* pProto = m_control_64.m_pProtoItem;
    CExoArrayListL<CSWGuiControl*> lstControls;
    CClientOptions* pOptions = g_pAppManager->GetClientExoApp()->GetOptions();
    if (pOptions == 0) {
        return;
    }
    for (int i = 0; i < 9; i++) {
        CSWGuiOptionsCheckbox* pBox = new CSWGuiOptionsCheckbox;
        pBox->CopyToggleStyle(pProto->m_extent, pProto);
        pBox->AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsFeedback::OnEnterSlot);
        switch (i) {
        case 0:
            pBox->m_cTextParams.SetStrRef(0xa527);
            pBox->m_n58 = 0xa52e;
            pBox->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsFeedback::OnHide);
            pBox->AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsFeedback::OnHide);
            break;
        case 1:
            pBox->m_cTextParams.SetStrRef(0xa528);
            pBox->m_n58 = 0xa52f;
            pBox->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsFeedback::OnTutorial);
            pBox->AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsFeedback::OnTutorial);
            break;
        case 2:
            pBox->m_cTextParams.SetStrRef(0xa529);
            pBox->m_n58 = 0xa530;
            pBox->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsFeedback::OnSubtitles);
            pBox->AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsFeedback::OnSubtitles);
            break;
        case 3:
            pBox->m_cTextParams.SetStrRef(0xa52b);
            pBox->m_n58 = 0xa531;
            pBox->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsFeedback::OnMiniMap);
            pBox->AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsFeedback::OnMiniMap);
            break;
        case 4:
            pBox->m_cTextParams.SetStrRef(0xa52d);
            pBox->m_n58 = 0xa533;
            pBox->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsFeedback::OnFloaty);
            pBox->AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsFeedback::OnFloaty);
            break;
        case 5:
            pBox->m_cTextParams.SetStrRef(0xa5d2);
            pBox->m_n58 = 0xa5d3;
            pBox->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsFeedback::OnSummary);
            pBox->AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsFeedback::OnSummary);
            break;
        case 6:
            pBox->m_cTextParams.SetStrRef(0xbfa5);
            pBox->m_n58 = 0xbfa9;
            pBox->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsFeedback::OnHideInGameGUI);
            pBox->AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsFeedback::OnHideInGameGUI);
            break;
        case 7:
            pBox->m_cTextParams.SetStrRef(0xbfa6);
            pBox->m_n58 = 0xbfaa;
            pBox->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsFeedback::OnUseSmallFont);
            pBox->AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsFeedback::OnUseSmallFont);
            break;
        case 8:
            pBox->m_cTextParams.SetStrRef(0xbfdb);
            pBox->m_n58 = 0xbfda;
            pBox->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsFeedback::OnEnableToolTips);
            pBox->AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiOptionsFeedback::OnEnableToolTips);
            break;
        }
        lstControls.AddAlloc(pBox);
    }
    m_control_64.ClearControls();
    m_control_64.AddControls(lstControls, 1, 1, 0);
    m_control_64.ClearListFlag40();
}
