// CClientExoAppInternal::AddDownloadedResources, worker C s8
// (clientexoappinternal.o; translation-unit-local layout).

#include "c/exo_base.h"

class CResRef {
public:
    CResRef(const CExoString& s);                   // 0x00406D60
    char m_resRef[16];
};

class CExoResMan {
public:
    int AddEncapsulatedResourceFile(const CExoString& sName, int nPriority);   // 0x004087A0
    int AddResourceImageFile(const CExoString& sName);  // 0x004087C0
    int AddFixedKeyTableFile(const CExoString& sName);  // 0x004087E0
    int Exists(const CResRef& cRes, unsigned short nType, int b);   // 0x00408BC0
};

class CTlkTable {
public:
    int AddFile(const CExoString& sName, int nIndex);   // 0x0041D920 (descriptive)
};

class CExoAliasList {
public:
    CExoString* GetAliasPath(const CExoString& sAlias, int b);  // 0x005E6890
};

class CExoBase {
public:
    char pad_0x0[0xc];
    CExoAliasList* m_pcExoAliasList;                // 0x0c
};

extern CExoBase* g_pExoBase;                        // 0x007A39E0
extern CExoResMan* g_pExoResMan;                    // 0x007A39E8
extern CTlkTable* g_pTlkTable;                      // 0x007A3A08
extern unsigned char g_nNumLiveDirectories;         // 0x0073ECD2

class CClientExoAppInternal {
public:
    void AddDownloadedResources();
};

// FUNCTION: SWKOTOR 0x005F4180
void CClientExoAppInternal::AddDownloadedResources()
{
    CExoString sName;
    CExoString sPath;
    for (int i = 1; i < g_nNumLiveDirectories; i++) {
        sName.Format("live%d", i);
        sPath.Format("LIVE%d", i);
        if (g_pExoBase->m_pcExoAliasList->GetAliasPath(sPath, 0)->GetLength()) {
            if (g_pExoResMan->Exists(sName, 0x7e2, 0)) {
                sPath.Format("LIVE%d:live%d", i, i);
                g_pTlkTable->AddFile(sPath, i);
            }
            if (g_pExoResMan->Exists(sName, 0x270f, 0)) {
                sPath.Format("LIVE%d:live%d", i, i);
                g_pExoResMan->AddFixedKeyTableFile(sPath);
            }
            if (g_pExoResMan->Exists(sName, 0xbba, 0)) {
                sPath.Format("LIVE%d:RIMSXBOX\\live%d", i, i);
                g_pExoResMan->AddResourceImageFile(sPath);
            }
            if (g_pExoResMan->Exists(sName, 0x7db, 0)) {
                sPath.Format("LIVE%d:live%d", i, i);
                g_pExoResMan->AddEncapsulatedResourceFile(sPath, 2);
            }
            CExoString sNameDX = sName + "DX";
            if (g_pExoResMan->Exists(sName, 0xbba, 0)) {
                sPath.Format("LIVE%d:RIMSXBOX\\live%ddx", i, i);
                g_pExoResMan->AddResourceImageFile(sPath);
            }
            sPath.Format("LIVE%d:OVERRIDE\\textures", i);
            g_pExoResMan->AddEncapsulatedResourceFile(sPath, 1);
        }
    }
}
