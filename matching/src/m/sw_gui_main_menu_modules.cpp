// CSWGuiMainMenu::PopulateModules (0x0067BC40): fills the warp list box with
// one button per module found under MODULES: and the live%d aliases. The
// new button's constructor is inlined here, so this translation unit sees an
// inline CSWGuiButton constructor.

#define M_GUI_BUTTON_INLINE_CTOR
#include "m/sw_gui_main_menu.h"

typedef void (CSWGuiPanel::*PFN_GUI_CALLBACK)(CSWGuiControl*);

// FUNCTION: SWKOTOR 0x0067BC40
void CSWGuiMainMenu::PopulateModules()
{
    CExoString sName;
    CExoString sRim;
    CExoString sMod;
    CExoArrayList<CExoString> lstRims;
    CExoArrayList<CExoString> lstMods;
    CExoArrayList<CExoString> lstUnused1;
    CExoArrayList<CExoString> lstUnused2;
    CExoArrayList<CSWGuiControl*> lstControls;
    int nRim = 0;
    int nMod = 0;
    CSWGuiExtent cExtent = m_lbModules.m_cItemExtent;
    CExoString sUnused;
    g_pExoBase->GetDirectoryList(&lstMods, "MODULES:", 0x7db, 0, 1);
    g_pExoBase->GetDirectoryList(&lstRims, "MODULES:", 0xbba, 0, 1);
    CExoString sAlias;
    for (int nLive = 1; nLive < g_nLiveFolderCount; nLive++) {
        sAlias.Format("live%d", nLive);
        if (g_pExoBase->m_pAliasList->GetAliasPath(sAlias, 0)->GetLength() > 0) {
            sAlias = sAlias + ":modules";
            g_pExoBase->GetDirectoryList(&lstMods, sAlias, 0x7db, 0, 1);
            g_pExoBase->GetDirectoryList(&lstRims, sAlias, 0xbba, 0, 1);
        }
    }
    CSWGuiButton* pProto = m_lbModules.m_pProtoItem;
    int nRims = lstRims.num;
    int nMods = lstMods.num;
    while (nRim < nRims || nMod < nMods) {
        if (nRim >= nRims) {
            sRim = CExoString("zzzzzzzz.zzz");
        }
        else {
            sRim = lstRims.element[nRim];
            int nPos = sRim.Find(CExoString("_s.rim"), 0);
            if (nPos != -1) {
                nRim++;
                continue;
            }
        }
        if (nMod >= nMods) {
            sMod = CExoString("zzzzzzzz.zzz");
        }
        else {
            sMod = lstMods.element[nMod];
            int nPos = sMod.Find(CExoString("_s.rim"), 0);
            if (nPos != -1) {
                nMod++;
                continue;
            }
        }
        sRim = sRim.SubString(0, sRim.GetLength() - 4);
        sMod = sMod.SubString(0, sMod.GetLength() - 4);
        if (sRim.LowerCase() < sMod.LowerCase()) {
            sName = sRim;
            nRim++;
            if (sRim == sMod) {
                nMod++;
            }
        }
        else {
            sName = sMod;
            nMod++;
        }
        CSWGuiButton* pButton = new CSWGuiButton;
        pButton->CopyStyle(cExtent, pProto);
        pButton->m_cText.m_cParams.SetText(sName);
        cExtent.height = pButton->m_cBorder.m_cParams.GetCornerSize() * 2 + pButton->m_cText.GetHeight();
        pButton->SetExtent(cExtent);
        pButton->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiMainMenu::OnModuleSelected);
        pButton->AddEvent(0x2d, this, (PFN_GUI_CALLBACK)&CSWGuiMainMenu::OnModuleSelected);
        lstControls.AddInline(pButton);
        m_lstModuleNames.Add(sName);
    }
    m_lbModules.AddControls(lstControls, 1, 0, 0);
}
