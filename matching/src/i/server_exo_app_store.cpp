// CServerExoAppInternal::StorePlayerCharacters: writes the player creatures
// to TEMP:pifo before a module change.
#include "i/server_exo_app.h"

struct CResStruct {
    unsigned long m_nStructIndex;
};

struct CResList {
    unsigned long m_nStructIndex;
    char m_pLabel[16];
};

class CRes {
public:
    CRes();                                                         // 0x00406460
    virtual ~CRes();
    char m_pad4[0x28 - 4];
};

class CResGFF : public CRes {
public:
    CResGFF();                                                      // 0x004105A0
    virtual ~CResGFF();                                             // 0x00411570
    int CreateGFFFile(CResStruct* pStruct, const CExoString& sType,
                      const CExoString& sVersion);                  // 0x00411260
    int AddList(CResList* pList, CResStruct* pStruct, char* szLabel);               // 0x00412450
    int AddListElement(CResStruct* pStruct, CResList* pList, unsigned long nType);  // 0x004124E0
    int WriteGFFFile(const CExoString& sFileName, unsigned short nType);  // 0x00413030
    char m_pad28[0xa0 - 0x28];
};

class CExoFile {
public:
    CExoFile(const CExoString& sFile, unsigned short nType, const CExoString& sMode);  // 0x005E68D0
    ~CExoFile();                                                    // 0x005E69E0
    int FileOpened();                                               // 0x005E6A10
    unsigned long GetSize();                                        // 0x005E6950
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

// FUNCTION: SWKOTOR 0x004B2BA0
int CServerExoAppInternal::StorePlayerCharacters()
{
    CExoLinkedList<CSWSPlayer>* pPlayerList = m_pPlayerList;
    CResGFF cGFF;
    CResList cList;
    CResStruct cTopStruct;
    CResStruct cElement;
    int nStored = 0;

    bool bFailed = !cGFF.CreateGFFFile(&cTopStruct, "IFO ", "V2.0");
    if (bFailed) {
        return 0;
    }
    cGFF.AddList(&cList, &cTopStruct, "Mod_PlayerList");

    CExoLinkedListPosition pos = pPlayerList->GetHeadPos();
    while (pos) {
        CSWSPlayer* pPlayer = pPlayerList->GetAtPos(pos)->AsSWSPlayer();
        if (pPlayer) {
            pPlayer->m_nCharacterIndex = -1;
            CSWSCreature* pCreature =
                g_pAppManager->m_pServerExoApp->GetCreatureByGameObjectID(pPlayer->m_oidPCObject);
            if (pCreature) {
                cGFF.AddListElement(&cElement, &cList, 0xbead);
                pCreature->ClearAllActions(1);
                pCreature->SaveCreature(&cGFF, &cElement);
                pPlayer->m_nCharacterIndex = nStored;
                nStored++;
            }
        }
        pPlayerList->GetNext(pos);
    }

    if (nStored > 0) {
        CExoString sName;
        CExoString sFile;
        sName = "pifo";
        sFile = "TEMP:";
        sFile = sFile + sName;
        cGFF.WriteGFFFile(sFile.CStr(), 0x7de);
    }

    CSWPartyTable* pPartyTable = GetPartyTable();
    pPartyTable->UpdateMembers(1);
    pPartyTable->UpdateInventory(1);
    return nStored > 0;
}

// Sizes of the GAMEINPROGRESS: files plus a margin, times 10/9.
// FUNCTION: SWKOTOR 0x004B5F90
void CServerExoAppInternal::EstimateSaveGameSize(const CExoString& sModule, unsigned short nType)
{
    CExoArrayList<CExoString> lFiles;
    CExoString sUnused;
    unsigned long nTotal = 0;
    g_pExoBase->GetDirectoryList(&lFiles, "GAMEINPROGRESS:", 0xffff, 0, 0);
    int nCount = lFiles.num;
    if (nCount == 0) {
        m_nEstimatedSaveSize = 0x393870;
    } else {
        CExoString sFile;
        CExoString sPath;
        CExoString sExtension;
        for (int i = 0; i < nCount; i++) {
            sFile = lFiles.element[i];
            sPath = CExoString("GAMEINPROGRESS:") + sFile.Left(sFile.GetLength() - 4);
            sExtension = sFile.Right(3);
            CExoFile cFile(sPath, g_pExoBase->GetResTypeFromExtension(sExtension), "rb");
            if (!cFile.FileOpened()) {
                nTotal += 2500000;
            } else {
                nTotal += cFile.GetSize();
            }
        }
        if (nType != 0xbc1 && nType != 0x809) {
            nTotal += 2500000;
        }
        m_nEstimatedSaveSize = nTotal * 10 / 9;
    }
}
