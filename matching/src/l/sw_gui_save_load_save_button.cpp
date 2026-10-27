// CSWGuiSaveLoad::HandleSaveButton: checks the free space on the SAVES:
// drive (16 KB blocks) before asking for a save name or an overwrite.
#include "l/sw_gui_save_load.h"

// FUNCTION: SWKOTOR 0x006CBB60
void CSWGuiSaveLoad::HandleSaveButton(CSWGuiControl* pControl)
{
    int nExtra = 0;
    if (!pControl->m_n4c) {
        return;
    }
    g_pExoResMan->AddResourceDirectory("OPTIONS:");
    {
        CExoFile cFile("OPTIONS:OPT", 0, "r");
        if (!cFile.FileOpened() || !cFile.GetSize()) {
            nExtra = 2;
        }
    }
    CSWGuiSaveLoadEntry* pEntry = (CSWGuiSaveLoadEntry*)m_lbGames.GetSelectedControl();
    if (!pEntry) {
        return;
    }
    if (pEntry->m_bSavedGame) {
        unsigned __int64 nFree;
        g_pExoResMan->GetFreeDiskSpace("SAVES:", &nFree);
        __int64 nAvailable = (__int64)(nFree >> 14) + (nExtra + 0x640);
        if (nAvailable < nExtra + 0x640) {
            CSWGuiMessageBox* pBox = g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pMessageBox;
            int nShort = nExtra + 0x640 - (int)nAvailable;
            pEntry->m_nBlocksNeeded = nShort;
            float fMegabytes = (float)(nShort << 14) * 0.000001f;
            CExoString sMessage;
            CExoString sSize;
            sSize.Format("%2.3f", fMegabytes);
            g_pTlkTable->SetCustomToken(0, sSize);
            sMessage = g_pTlkTable->GetSimpleString(0xbb75);
            g_pTlkTable->ParseTokens(&sMessage);
            pBox->SetAllowCancel(0);
            pBox->SetMessage(sMessage);
            pBox->SetCloseCallback(this, 0, 0);
            GetManager()->AddPanel(pBox, 1, 1);
            return;
        }
        CSWGuiMessageBox* pBox = g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pMessageBox;
        pBox->SetAllowCancel(1);
        pBox->SetMessageStrRef(0x637);
        pBox->SetCloseCallback(this, (PFN_GUI_CALLBACK)&CSWGuiSaveLoad::PromptForSaveName, (int)pEntry);
        m_pManager->AddPanel(pBox, 1, 1);
        return;
    }
    unsigned __int64 nFree;
    g_pExoResMan->GetFreeDiskSpace("SAVES:", &nFree);
    __int64 nAvailable = (__int64)(nFree >> 14);
    if (nAvailable < nExtra + 0x640) {
        CSWGuiMessageBox* pBox = g_pAppManager->GetClientExoApp()->GetInGameGui()->m_pMessageBox;
        int nShort = nExtra + 0x640 - (int)nAvailable;
        pEntry->m_nBlocksNeeded = nShort;
        float fMegabytes = (float)(nShort << 14) * 0.000001f;
        CExoString sMessage;
        CExoString sSize;
        sSize.Format("%2.3f", fMegabytes);
        g_pTlkTable->SetCustomToken(0, sSize);
        sMessage = g_pTlkTable->GetSimpleString(0xbb75);
        g_pTlkTable->ParseTokens(&sMessage);
        pBox->SetAllowCancel(0);
        pBox->SetMessage(sMessage);
        pBox->SetCloseCallback(this, 0, 0);
        GetManager()->AddPanel(pBox, 1, 1);
        return;
    }
    PromptForSaveName(pEntry);
}
