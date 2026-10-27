// CSWSWaypoint::LoadWaypoint / LoadFromTemplate (b005c_f2_journal_store):
// GFF and UTW template loading of a waypoint.

class CExoString {
public:
    CExoString();
    CExoString(const char *s);
    ~CExoString();
    void Format(const char *szFormat, ...);
    char *CStr();
    char *m_sString;
    unsigned long m_nBufferLength;
};

class CExoLocString {
public:
    CExoLocString();
    ~CExoLocString();
    CExoLocString &operator=(const CExoLocString &s);
    void *m_pList;
    unsigned long m_nStrRef;
};

class Vector {
public:
    Vector() : x(0.0f), y(0.0f), z(0.0f) {}
    void Normalize();
    float x, y, z;
};

class CResRef {
public:
    void CopyToString(CExoString &s) const;
    char m_resRef[16];
};

struct CResStruct {
    unsigned long m_nStructIndex;
};

class CResGFF {
public:
    CResGFF(unsigned short nType, char *szFileType, const CResRef &cResRef);
    virtual ~CResGFF();
    void GetTopLevelStruct(CResStruct *pStruct);
    CExoString ReadFieldCExoString(CResStruct *pStruct, char *szLabel, int &bSuccess,
                                   const CExoString &sDefault);
    CExoLocString ReadFieldCExoLocString(CResStruct *pStruct, char *szLabel, int &bSuccess,
                                         const CExoLocString &sDefault);
    float ReadFieldFLOAT(CResStruct *pStruct, char *szLabel, int &bSuccess, float fDefault);
    unsigned char ReadFieldBYTE(CResStruct *pStruct, char *szLabel, int &bSuccess,
                                unsigned char nDefault);
    char pad_0x4[0x90];
    int m_bLoaded;                   // +0x94 (placeholder name)
    char pad_0x98[0x8];
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
};

extern CAppManager *g_pAppManager;

class CSWSWaypoint {
public:
    void SetTag(CExoString &sTag);
    void SetPosition(const Vector &vPosition, int a, int b, int c);
    void SetOrientation(const Vector &vOrientation);

    char pad_0x0[0x4];
    unsigned long m_idSelf;          // +0x4
    char pad_0x8[0x10];
    CExoString m_sTag;               // +0x18
    char pad_0x20[0x208];
    int m_bHasMapNote;               // +0x228
    int m_bMapNoteEnabled;           // +0x22c
    CExoLocString m_sMapNote;        // +0x230
    CExoLocString m_sLocalizedName;  // +0x238

    int LoadWaypoint(CResGFF *pGFF, CResStruct *pStruct);
    int LoadFromTemplate(const CResRef &cResRef);
};

// FUNCTION: SWKOTOR 0x005C83B0
int CSWSWaypoint::LoadFromTemplate(const CResRef &cResRef)
{
    CResGFF *pGFF = new CResGFF(0x80a, "UTW ", cResRef);
    if (!pGFF->m_bLoaded) {
        delete pGFF;
        CExoString sError;
        CExoString sResRef;
        cResRef.CopyToString(sResRef);
        sError.Format("Waypoint template %s doesn't exist.\n", sResRef.CStr());
        return 0;
    }
    CResStruct cTop;
    pGFF->GetTopLevelStruct(&cTop);
    LoadWaypoint(pGFF, &cTop);
    delete pGFF;
    return 1;
}

// NONMATCH (15): code is identical but the original frame is 8 bytes larger
// (locals shifted by 4/8); hoisting the map-note flags did not change it.
// FUNCTION: SWKOTOR 0x005C7F30
int CSWSWaypoint::LoadWaypoint(CResGFF *pGFF, CResStruct *pStruct)
{
    int bSuccess;
    int bHasMapNote;
    int bMapNoteEnabled;
    Vector vPosition;
    Vector vOrientation;
    CExoLocString sMapNote;
    SetTag(pGFF->ReadFieldCExoString(pStruct, "Tag", bSuccess, ""));
    m_sLocalizedName =
        pGFF->ReadFieldCExoLocString(pStruct, "LocalizedName", bSuccess, CExoLocString());
    vPosition.x = pGFF->ReadFieldFLOAT(pStruct, "XPosition", bSuccess, 0.0f);
    vPosition.y = pGFF->ReadFieldFLOAT(pStruct, "YPosition", bSuccess, 0.0f);
    vPosition.z = pGFF->ReadFieldFLOAT(pStruct, "ZPosition", bSuccess, 0.0f);
    vOrientation.x = pGFF->ReadFieldFLOAT(pStruct, "XOrientation", bSuccess, 0.0f);
    vOrientation.y = pGFF->ReadFieldFLOAT(pStruct, "YOrientation", bSuccess, 0.0f);
    vOrientation.z = pGFF->ReadFieldFLOAT(pStruct, "ZOrientation", bSuccess, 0.0f);
    if (vOrientation.x * vOrientation.x + vOrientation.y * vOrientation.y +
            vOrientation.z * vOrientation.z != 0.0f)
        vOrientation.Normalize();
    SetPosition(vPosition, 1, 1, 0);
    SetOrientation(vOrientation);
    bHasMapNote = pGFF->ReadFieldBYTE(pStruct, "HasMapNote", bSuccess, 0);
    if (bSuccess && bHasMapNote) {
        bMapNoteEnabled = pGFF->ReadFieldBYTE(pStruct, "MapNoteEnabled", bSuccess, 0);
        sMapNote = pGFF->ReadFieldCExoLocString(pStruct, "MapNote", bSuccess, CExoLocString());
        if (bSuccess) {
            m_bMapNoteEnabled = bMapNoteEnabled;
            m_bHasMapNote = bHasMapNote;
            m_sMapNote = sMapNote;
        }
    }
    g_pAppManager->m_pServerExoApp->GetModule()->AddObjectToLookupTable(m_sTag, m_idSelf);
    return 1;
}
