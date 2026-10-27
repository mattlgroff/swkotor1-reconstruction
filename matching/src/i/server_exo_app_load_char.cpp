// CServerExoAppInternal::LoadCharacterStart.
#include "i/server_exo_app.h"

class CExoFile {
public:
    CExoFile(const CExoString& sFile, unsigned short nType, const CExoString& sMode);  // 0x005E68D0
    ~CExoFile();                                                    // 0x005E69E0
    int FileOpened();                                               // 0x005E6A10
    unsigned long Write(const void* pData, unsigned long nSize, unsigned long nCount);  // 0x005E69A0
    void* m_pInternal;
};

class CAppManager {
public:
    void* m_p0;
    void* m_pClientExoApp;                                          // +0x04
    CServerExoApp* m_pServerExoApp;                                 // +0x08
};
extern CAppManager* g_pAppManager;                                  // 0x007A39FC
inline CServerExoApp* GetServerExoApp() { return g_pAppManager->m_pServerExoApp; }

// NONMATCH: the original keeps the inlined lFiles destructor and return-1
// epilogue inside the 0xF branch (the other exits jump into it) and calls
// the out-of-line ~CExoArrayList<CExoString> (0x00405920) on the return-0
// path; cl places and inlines these differently here.
// FUNCTION: SWKOTOR 0x004B7470
int CServerExoAppInternal::LoadCharacterStart(unsigned char nType, CSWSPlayer* pPlayer,
                                              CResRef cResRef, void* pData, unsigned long nSize)
{
    CSWSModule* pModule = GetServerExoApp()->GetModule();
    CExoString sPlayerName;
    CExoArrayList<CExoString> lFiles;
    CExoString sVaultPath;

    if (nType == 2) {
        if (!pPlayer->GetGameObject()) {
            pPlayer->LoadServerCharacter(cResRef);
        }
    } else if (nType == 0xe) {
        if (!pPlayer->GetGameObject()) {
            CExoString sIndex;
            cResRef.CopyToString(sIndex);
            pPlayer->LoadCharacterIFO(sIndex.AsINT(), 0, 1);
        }
    } else if (nType == 0x11) {
        if (!pPlayer->GetGameObject()) {
            sPlayerName = pPlayer->GetPlayerName();
            g_pExoBase->GetDirectoryList(&lFiles, "SERVERVAULT:", 0xffff, 1, 0);
            if (lFiles.count(sPlayerName)) {
                sVaultPath.Format("SERVERVAULT:%s", sPlayerName.CStr());
                g_pExoResMan->AddResourceDirectory(sVaultPath);
                pPlayer->LoadServerCharacter(cResRef);
                pPlayer->m_nCharacterType = 4;
                g_pExoResMan->RemoveResourceDirectory(sVaultPath);
            }
        }
    } else if (nType == 0xf) {
        pPlayer->LoadCharacterIFO(-1, 1, 1);
        m_c1b770.CreateParty();
        LoadCharacterFinish(pPlayer, 0);
        return 1;
    } else {
        if (!pPlayer->GetGameObject()) {
            CExoFile* pFile = new CExoFile("TEMP:temp_char", 0x7df, "wb");
            if (pFile == NULL) {
                return 0;
            }
            if (!pFile->FileOpened()) {
                delete pFile;
                return 0;
            }
            pFile->Write(pData, 1, nSize);
            delete pFile;
            CExoString sTemp("TEMP:");
            g_pExoResMan->AddResourceDirectory(sTemp);
            if (nType == 1 || nType == 0x13) {
                pPlayer->LoadLocalCharacter();
            }
            g_pExoResMan->RemoveResourceDirectory(sTemp);
            g_pExoResMan->Leaf9460(sTemp, 0, 0);
            m_c1b770.CreateParty();
        }
    }

    if (pModule->m_bIsSaveGame && nType != 0xe) {
        int nIndex = pModule->GetPlayerIndexInPlayerList(pPlayer);
        if (nIndex != -1) {
            pPlayer->m_nPrimaryPlayerIndex = nIndex;
            SendCharacterQuery(pPlayer);
            return 1;
        }
    }
    LoadCharacterFinish(pPlayer, 0);
    return 1;
}
