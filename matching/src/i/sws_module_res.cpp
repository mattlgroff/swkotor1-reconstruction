// CSWSModule resource mounting (0x004C4150-0x004C44D0).
#include "i/sws_module.h"

// FUNCTION: SWKOTOR 0x004C4150
void CSWSModule::AddModuleResources(const CExoString& sModule)
{
    CExoString sName;
    int nCount = 0;
    int nProgress = GetClientExoApp()->GetTutorialFlag(0);
    int nEnd = nProgress + GetClientExoApp()->GetTutorialFlag(1);
    g_pExoResMan->FUN_0063e7c0(sModule);
    int nStep = (nEnd - nProgress) / 7;
    {
        CExoString sDir("HD0:players");
        g_pExoResMan->AddEncapsulatedResourceFile(sDir, 2);
    }
    nProgress += nStep;
    GetClientExoApp()->SetLoadBarProgress(nProgress, 1);
    RunFrame(1.0f / 30.0f, 0, 0);
    int nColon = sModule.Find(':', 0);
    if (nColon == -1) {
        sName = sName + sModule;
    } else {
        sName = sName + sModule.Right(sModule.GetLength() - nColon - 1);
    }
    {
        CExoString sDir("CURRENTGAME:");
        g_pExoResMan->AddResourceDirectory(sDir);
    }
    {
        CResRef cResRef(sName);
        if (g_pExoResMan->Exists(cResRef, 0x809, NULL)) {
            m_bIsSaveGame = 1;
        }
    }
    {
        CExoString sDir("CURRENTGAME:");
        g_pExoResMan->RemoveResourceDirectory(sDir);
    }
    CExoString sLips;
    sLips.Format("LIPS:%s_loc", sName.CStr());
    g_pExoResMan->AddEncapsulatedResourceFile(sLips, 2);
    {
        CExoString sDir("LIPS:localization");
        g_pExoResMan->AddEncapsulatedResourceFile(sDir, 2);
    }
    g_pExoResMan->AddResourceImageFilesAsync(sName, 1, GetIsSaveGame());
    while (g_pExoResMan->m_bAsyncLoading == 1) {
        Sleep(10);
        if (nProgress < nEnd && ++nCount > 100) {
            nProgress += nStep;
            if (nProgress >= nEnd) {
                nProgress = nEnd;
            }
            GetClientExoApp()->SetLoadBarProgress(nProgress, 1);
            RunFrame(1.0f / 30.0f, 0, 0);
            nCount = 0;
        }
    }
    unsigned char bResult = g_pExoResMan->m_bAsyncResult;
    GetClientExoApp()->SetModuleResourcesLoadedFlag(bResult);
    GetClientExoApp()->SetLoadBarProgress(nEnd, 1);
    RunFrame(1.0f / 30.0f, 0, 0);
}

// FUNCTION: SWKOTOR 0x004C44D0
void CSWSModule::RemoveModuleResources(const CExoString& sModule)
{
    g_pExoResMan->RemoveEncapsulatedResourceFile(sModule);
    CExoString sName;
    int nColon = sModule.Find(':', 0);
    if (nColon == -1) {
        sName = sModule;
    } else {
        sName = sModule.Right(sModule.GetLength() - nColon - 1);
    }
    int nLive = g_pExoResMan->FUN_0063e7c0(sName);
    {
        CExoString sDir("CURRENTGAME:");
        g_pExoResMan->AddResourceDirectory(sDir);
    }
    g_pExoResMan->RemoveResourceImageFile(CExoString("CURRENTGAME:") + sName);
    g_pExoResMan->RemoveEncapsulatedResourceFile(CExoString("CURRENTGAME:") + sName);
    {
        CExoString sDir("CURRENTGAME:");
        g_pExoResMan->RemoveResourceDirectory(sDir);
    }
    {
        CExoString sDir("MODULES:");
        g_pExoResMan->AddResourceDirectory(sDir);
    }
    if (g_pExoResMan->Exists(sName, 0x7db, NULL)) {
        g_pExoResMan->RemoveEncapsulatedResourceFile(CExoString("MODULES:") + sName);
    } else if (g_pExoResMan->Exists(sName + CExoString("_s"), 0xbba, NULL)) {
        if (nLive == 0) {
            g_pExoResMan->RemoveResourceImageFile(CExoString("MODULES:") + sName + CExoString("_s"));
        } else {
            CExoString sDir;
            sDir.Format("LIVE%d:MODULES\\", nLive);
            g_pExoResMan->RemoveResourceImageFile(sDir + sName);
        }
    }
    {
        CExoString sDir("MODULES:");
        g_pExoResMan->RemoveResourceDirectory(sDir);
    }
    CExoString sLips;
    sLips.Format("LIPS:%s_loc", sName.CStr());
    g_pExoResMan->RemoveEncapsulatedResourceFile(sLips);
    {
        CExoString sDir("LIPS:localization");
        g_pExoResMan->RemoveEncapsulatedResourceFile(sDir);
    }
    if (g_pExoResMan->Exists(sName + CExoString("_a"), 0xbba, NULL)) {
        g_pExoResMan->RemoveResourceImageFile(CExoString("RIMS:") + sName + CExoString("_a"));
    }
    if (g_pExoResMan->Exists(sName + CExoString("_adx"), 0xbba, NULL)) {
        g_pExoResMan->RemoveResourceImageFile(CExoString("RIMS:") + sName + CExoString("_adx"));
    }
    {
        CExoString sDir("HD0:players");
        g_pExoResMan->RemoveEncapsulatedResourceFile(sDir);
    }
    GetClientExoApp()->ResetModuleResourcesLoaded();
}
