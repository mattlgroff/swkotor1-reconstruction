// CSWGuiSaveLoad::PopulateGameList: one entry per SAVES: directory, plus
// the "new save game" entry at the top when saving.
#include "l/sw_gui_save_load.h"

// FUNCTION: SWKOTOR 0x006CC160
void CSWGuiSaveLoad::PopulateGameList()
{
    CExoArrayList<CExoString> lstDirectories;
    CExoArrayList<CSWGuiControl*> lstControls;
    CSWGuiZeroExtent extent;
    extent.height = 0x2a;
    CSWGuiControl* pProto = m_lbGames.m_pProtoItem;
    CExoString sUnused;
    g_pExoBase->GetDirectoryList(&lstDirectories, "SAVES:", 0xffff, 1, 1);
    if (!g_pExoResMan->CreateDirectory2("TEMP:")) {
        g_pExoResMan->CleanDirectory("TEMP:", 0, 0);
    }
    for (int i = 0; i < lstDirectories.m_nCount; i++) {
        CSWGuiSaveLoadEntry* pEntry = new CSWGuiSaveLoadEntry;
        pEntry->CopyStyle(extent, (CSWGuiButton*)pProto);
        pEntry->LoadData(lstDirectories.m_pArray[i]);
        pEntry->SetName(lstDirectories.m_pArray[i]);
        if (m_bSave && (pEntry->m_nGameNumber == 1 || pEntry->m_nGameNumber == 0)) {
            delete pEntry;
        } else {
            if (m_bSave) {
                pEntry->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiSaveLoad::HandleSaveButton);
            } else {
                pEntry->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiSaveLoad::VerifyLoadGame);
            }
            pEntry->AddEvent(0x29, this, (PFN_GUI_CALLBACK)&CSWGuiSaveLoad::PromptDeleteGame);
            pEntry->AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiSaveLoad::ShowGame);
            lstControls.AddInline(pEntry);
            if (i == 0) {
                ShowGame(pEntry);
            }
        }
    }
    unsigned long nNext = 2;
    unsigned long nMax = 0;
    if (m_bSave) {
        for (int j = 0; j < lstControls.m_nCount; j++) {
            unsigned long nNumber = ((CSWGuiSaveLoadEntry*)lstControls.m_pArray[j])->m_nGameNumber;
            if (nNumber == nNext) {
                nNext++;
            }
            if (nMax > nNumber) {
                nNumber = nMax;
            }
            nMax = nNumber;
        }
        CSWGuiSaveLoadEntry* pNew = new CSWGuiSaveLoadEntry;
        pNew->CopyStyle(extent, (CSWGuiButton*)pProto);
        pNew->m_cTextParams.SetStrRef(0x636);
        pNew->m_nGameNumber = nMax + 1 >= 2 ? nMax + 1 : 2;
        pNew->m_bFlag2 = 1;
        pNew->m_bFlag6 = 1;
        pNew->AddEvent(0x27, this, (PFN_GUI_CALLBACK)&CSWGuiSaveLoad::HandleSaveButton);
        pNew->AddEvent(0, this, (PFN_GUI_CALLBACK)&CSWGuiSaveLoad::ShowGame);
        lstControls.InsertInline(pNew, 0);
    } else if (lstControls.m_nCount == 0) {
        m_nFlags = (m_nFlags & 0xFFFFFCFFul) | 0x400ul;
        if (m_bFromMainMenu) {
            g_pAppManager->GetClientExoApp()->DisplayMainMenu(0);
        } else {
            m_pManager->PopModalPanel();
        }
        CSWGuiMessageBox* pBox = g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pMessageBox;
        pBox->SetAllowCancel(0);
        pBox->SetCloseCallback(0, 0, 0);
        pBox->SetMessageStrRef(0xa5fb);
        GetManager()->AddPanel(pBox, 1, 1);
        return;
    }
    m_lbGames.AddControls(lstControls, 1, 0, 0);
    m_nNextGameNumber = nNext;
}
