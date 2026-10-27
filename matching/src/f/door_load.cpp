// CSWSDoor instance loader (round s8, 0x0058C5F0): either the full field
// loader or a template plus the per-instance transition fields.

class CExoString {
public:
    CExoString();
    CExoString(const char *s);
    ~CExoString();
    CExoString &operator=(const CExoString &s);
    char *m_sString;
    unsigned long m_nBufferLength;
};

class CExoLocString {
public:
    ~CExoLocString();
    CExoLocString &operator=(const CExoLocString &s);
    void *m_pList;
    void *m_pPad;
};

class CResRef {
public:
    CResRef(const char *s);
    void CopyToString(CExoString &s) const;
    char m_resRef[16];
};

struct CResStruct {
    unsigned long m_nStructIndex;
};

class CResGFF {
public:
    CResRef ReadFieldCResRef(CResStruct *pStruct, char *szLabel, int &bSuccess,
                             const CResRef &cDefault = CResRef(""));
    CExoLocString ReadFieldCExoLocString(CResStruct *pStruct, char *szLabel, int &bSuccess,
                                         const CExoLocString &sDefault);
    CExoString ReadFieldCExoString(CResStruct *pStruct, char *szLabel, int &bSuccess,
                                   const CExoString &sDefault = CExoString(""));
    unsigned char ReadFieldBYTE(CResStruct *pStruct, char *szLabel, int &bSuccess, unsigned char nDefault);
};

class CSWSModule {
public:
    int AddObjectToLookupTable(const CExoString &sTag, unsigned long oid);
};

class CServerExoApp {
public:
    CSWSModule *GetModule();
};

class CAppManager {
public:
    char pad_0x0[0x8];
    CServerExoApp *m_pServerExoApp;  // +0x8
    CServerExoApp *GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager *g_pAppManager;

class CSWSDoor {
public:
    int LoadInstance(CResGFF *pRes, CResStruct *pStruct, int bUseTemplate);
    int LoadDoor(CResGFF *pRes, CResStruct *pStruct, int bFlag);
    int LoadFromTemplate(const CResRef &cTemplate);

    void *m_vtbl;
    unsigned long m_idSelf;          // +0x4
    char pad_0x8[0x10];
    CExoString m_sTag;               // +0x18
    char pad_0x20[0x364];
    unsigned char m_nLinkedToFlags;  // +0x384
    char pad_0x385[0x3];
    CExoString m_sLinkedTo;          // +0x388
    CExoString m_sLinkedToModule;    // +0x390
    char pad_0x398[0x30];
    CExoLocString m_sTransitionDestination; // +0x3c8
};

// FUNCTION: SWKOTOR 0x0058C5F0
int CSWSDoor::LoadInstance(CResGFF *pRes, CResStruct *pStruct, int bUseTemplate)
{
    int bResult = 1;
    if (bUseTemplate) {
        int bSuccess;
        CResRef cTemplate = pRes->ReadFieldCResRef(pStruct, "TemplateResRef", bSuccess, "");
        if (!bSuccess || !LoadFromTemplate(cTemplate))
            return 0;
        m_sTransitionDestination =
            pRes->ReadFieldCExoLocString(pStruct, "TransitionDestination", bSuccess, m_sTransitionDestination);
        m_sLinkedTo = pRes->ReadFieldCExoString(pStruct, "LinkedTo", bSuccess, "");
        m_nLinkedToFlags = pRes->ReadFieldBYTE(pStruct, "LinkedToFlags", bSuccess, 0);
        CExoString sModule;
        CResRef cModule = pRes->ReadFieldCResRef(pStruct, "LinkedToModule", bSuccess, "");
        cModule.CopyToString(sModule);
        m_sLinkedToModule = sModule;
    } else {
        bResult = LoadDoor(pRes, pStruct, 0);
    }
    if (bResult)
        g_pAppManager->GetServerExoApp()->GetModule()->AddObjectToLookupTable(m_sTag, m_idSelf);
    return bResult;
}
