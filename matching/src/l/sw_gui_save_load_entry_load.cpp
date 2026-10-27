// CSWGuiSaveLoadEntry::LoadData: parses "NNNNNN - name", flags corrupt
// saves, and reads savenfo.res (area, module, time, hints, live content,
// party portraits).
#include "l/sw_gui_save_load.h"

extern unsigned char g_nLiveFolderCount;            // 0x0073ECD2 (descriptive)

// FUNCTION: SWKOTOR 0x006C8E50
void CSWGuiSaveLoadEntry::LoadData(const CExoString& sDirectory)
{
    CExoString sLabel;
    CExoString sAlias;
    CExoString sUnused;
    int nPos = sDirectory.Find(" - ", 0);
    m_nGameNumber = sDirectory.Left(nPos).AsINT();
    m_sGameName = sDirectory.Right(sDirectory.GetLength() - 3 - nPos);
    sAlias = CExoString("SAVES:") + sDirectory;
    g_pExoResMan->AddResourceDirectory(sAlias);
    CExoString sCorrupt = g_pExoBase->m_pcExoAliasList->ResolveFileName(sAlias + "\\" + "CORRUPT", 0);
    CExoFile cFile(sAlias + "\\" + "CORRUPT", 0, "rb");
    if (cFile.FileOpened()) {
        m_bSavedGame = 1;
        m_bCorrupt = 1;
        g_pExoResMan->RemoveResourceDirectory(sAlias);
        return;
    }
    CResGFF* pGFF = new CResGFF(0, "NFO ", "savenfo");
    CResStruct cTop;
    pGFF->GetTopLevelStruct(&cTop);
    int bSuccess;
    m_sAreaName = pGFF->ReadFieldCExoString(&cTop, "AREANAME", bSuccess, "");
    m_sLastModule = pGFF->ReadFieldCExoString(&cTop, "LASTMODULE", bSuccess, "");
    m_nTimePlayed = pGFF->ReadFieldDWORD(&cTop, "TIMEPLAYED", bSuccess, 0);
    m_sSaveName = pGFF->ReadFieldCExoString(&cTop, "SAVEGAMENAME", bSuccess, "");
    if (!bSuccess) {
        m_sSaveName = CExoString("Old Save Game");
    }
    m_bCheatUsed = pGFF->ReadFieldBYTE(&cTop, "CHEATUSED", bSuccess, 0);
    m_bRebootAutoSave = pGFF->ReadFieldBYTE(&cTop, "REBOOTAUTOSAVE", bSuccess, 0);
    m_bPCAutoSave = pGFF->ReadFieldBYTE(&cTop, "PCAUTOSAVE", bSuccess, 0);
    m_sScreenShot = pGFF->ReadFieldCExoString(&cTop, "SCREENSHOT", bSuccess, "");
    unsigned char nHint = m_nGameplayHint;
    m_nGameplayHint = pGFF->ReadFieldBYTE(&cTop, "GAMEPLAYHINT", bSuccess, nHint);
    nHint = m_nStoryHint;
    m_nStoryHint = pGFF->ReadFieldBYTE(&cTop, "STORYHINT", bSuccess, nHint);
    unsigned char nLiveContent = pGFF->ReadFieldBYTE(&cTop, "LIVECONTENT", bSuccess, 0);
    if (nLiveContent) {
        CExoString sLive;
        for (int i = 1; i < g_nLiveFolderCount; i++) {
            sLive.Format("LIVE%d", i);
            CExoString sContent = pGFF->ReadFieldCExoString(&cTop, sLive.CStr(), bSuccess, sLive);
            if (g_pExoBase->m_pcExoAliasList->GetAliasPath(sLive, 0).GetLength() == 0 &&
                (nLiveContent & (1 << (i - 1)))) {
                m_bSavedGame = 1;
                m_bMissingContent = 1;
                m_sMissingContent = sContent;
                delete pGFF;
                g_pExoResMan->RemoveResourceDirectory(sAlias);
                return;
            }
        }
    }
    for (int j = 0; j < 3; j++) {
        sLabel.Format("%s%d", "PORTRAIT", j);
        m_aPortraits[j] = pGFF->ReadFieldCResRef(&cTop, sLabel.CStr(), bSuccess, "");
    }
    delete pGFF;
    g_pExoResMan->RemoveResourceDirectory(sAlias);
    m_bCorrupt = 0;
    m_bMissingContent = 0;
    m_sMissingContent = "";
    m_bSavedGame = 1;
}
