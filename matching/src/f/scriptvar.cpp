// CSWSScriptVarTable accessors and lifetime
// (b0059_f2_scriptvar_dialog).

class CExoString {
public:
    CExoString();
    CExoString(const char *s);
    CExoString(const CExoString &s);
    ~CExoString();
    CExoString &operator=(const CExoString &s);
    int operator==(const CExoString &s) const;
    int operator==(const char *s) const;
    char operator[](int n) const;
    int GetLength() const;
    CExoString Left(int n) const;
    CExoString SubString(int nStart, int nCount) const;

    char *m_sString;
    unsigned long m_nBufferLength;
};

class Vector {
public:
    float x, y, z;
};

struct CResStruct {
    unsigned long m_nIndex;
};

struct CResList {
    unsigned long m_nType;
    unsigned long m_nIndex;
    unsigned long m_nCount;
    char pad[0x8];
};

class CResGFF {
public:
    int GetStructFromStruct(CResStruct *pOut, CResStruct *pStruct, char *szLabel);
    int GetList(CResList *pList, CResStruct *pStruct, char *szLabel);
    unsigned long GetListCount(CResList *pList);
    int GetListElement(CResStruct *pOut, CResList *pList, unsigned long nIndex);
    int ReadFieldINT(CResStruct *pStruct, char *szLabel, int &bSuccess, int nDefault);
    unsigned long ReadFieldDWORD(CResStruct *pStruct, char *szLabel, int &bSuccess, unsigned long nDefault);
    float ReadFieldFLOAT(CResStruct *pStruct, char *szLabel, int &bSuccess, float fDefault);
    CExoString ReadFieldCExoString(CResStruct *pStruct, char *szLabel, int &bSuccess,
                                   const CExoString &sDefault);
    int AddStructToStruct(CResStruct *pOut, CResStruct *pStruct, char *szLabel, unsigned long nType);
    int AddList(CResList *pList, CResStruct *pStruct, char *szLabel);
    int AddListElement(CResStruct *pOut, CResList *pList, unsigned long nType);
    int WriteFieldINT(CResStruct *pStruct, int nValue, char *szLabel);
    int WriteFieldDWORD(CResStruct *pStruct, unsigned long nValue, char *szLabel);
    int WriteFieldFLOAT(CResStruct *pStruct, float fValue, char *szLabel);
    int WriteFieldCExoString(CResStruct *pStruct, const CExoString &sValue, char *szLabel);
};

class CScriptLocation {
public:
    CScriptLocation();
    ~CScriptLocation();
    void LoadLocation(CResGFF *pGFF, CResStruct *pStruct);
    void SaveLocation(CResGFF *pGFF, CResStruct *pStruct);
    Vector m_vPosition;
    Vector m_vOrientation;
};

class CSWSScriptVar {
public:
    CExoString m_sName;
    unsigned long m_nType;
    void *m_pValue;
};

template <class T>
class CExoArrayList {
public:
    T *element;
    int num;
    int array_size;

    ~CExoArrayList()
    {
        if (element)
            delete[] element;
        element = 0;
    }
    int Num() const { return num; }
    void Allocate(int s);
    void Add(T t)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 16);
        element[num++] = t;
    }
};

class CSWGlobalVariableTable {
public:
    void SetValueNumber(const CExoString &sName, int nValue, int bFlag);
    void SetValueBoolean(const CExoString &sName, int nValue, int bFlag);
    void SetValueString(const CExoString &sName, int nValue, int bFlag);
    void SetValueLocation(const CExoString &sName, int nValue, int bFlag);
};

class CSWSModule {
public:
    CSWGlobalVariableTable *GetGlobalVariables();
};

extern const unsigned long OBJECT_INVALID;   // 0x00746644

class CSWSScriptVarTable {
public:
    ~CSWSScriptVarTable();
    void internal_DeleteIndex(int nIndex);
    void DeleteIndex(CExoString &sName, unsigned long nType);
    int GetInt(CExoString &sName);
    float GetFloat(CExoString &sName);
    unsigned long GetObject(CExoString &sName);
    void SetFloat(CExoString &sName, float fValue);
    void SetString(CExoString &sName, CExoString &sValue);
    void SetObject(CExoString &sName, unsigned long oidValue);
    void SetLocation(CExoString &sName, CScriptLocation lValue);
    void DestroyFloat(CExoString &sName);
    void DestroyString(CExoString &sName);
    void DestroyObject(CExoString &sName);
    void LoadVarTable(CResGFF *pGFF, CResStruct *pStruct);
    void SaveVarTable(CResGFF *pGFF, CResStruct *pStruct);
    CSWSScriptVar *MatchIndex(CExoString &sName, unsigned long nType, int bCreate);
    CExoString GetString(CExoString &sName);
    CScriptLocation GetLocation(CExoString &sName);
    void SetInt(CExoString &sName, int nValue, int bFlag);

    CExoArrayList<CSWSScriptVar> m_lVars;  // +0x0
    CSWSModule *m_pModule;                 // +0xc
};

// FUNCTION: SWKOTOR 0x0059A2A0 SYMBOL: ?Add@?$CExoArrayList@VCSWSScriptVar@@@@QAEXVCSWSScriptVar@@@Z
template class CExoArrayList<CSWSScriptVar>;

// FUNCTION: SWKOTOR 0x0059A330
void CSWSScriptVarTable::internal_DeleteIndex(int nIndex)
{
    m_lVars.num--;
    for (; nIndex < m_lVars.num; nIndex++)
        m_lVars.element[nIndex] = m_lVars.element[nIndex + 1];
}

// FUNCTION: SWKOTOR 0x0059A390
CSWSScriptVar *CSWSScriptVarTable::MatchIndex(CExoString &sName, unsigned long nType, int bCreate)
{
    for (int i = 0; i < m_lVars.num; i++) {
        if (m_lVars.element[i].m_nType == nType && m_lVars.element[i].m_sName == sName)
            return &m_lVars.element[i];
    }
    if (bCreate) {
        CSWSScriptVar var;
        var.m_sName = sName;
        var.m_nType = nType;
        var.m_pValue = 0;
        if (nType == 3)
            var.m_pValue = new CExoString;
        else if (nType == 5)
            var.m_pValue = new CScriptLocation;
        m_lVars.Add(var);
        return &m_lVars.element[m_lVars.Num() - 1];
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0059A4E0
void CSWSScriptVarTable::DeleteIndex(CExoString &sName, unsigned long nType)
{
    for (int i = 0; i < m_lVars.num; i++) {
        if (m_lVars.element[i].m_nType == nType && m_lVars.element[i].m_sName == sName) {
            internal_DeleteIndex(i);
            return;
        }
    }
}

// FUNCTION: SWKOTOR 0x0059A530
int CSWSScriptVarTable::GetInt(CExoString &sName)
{
    int nValue = 0;
    CSWSScriptVar *pVar = MatchIndex(sName, 1, 0);
    if (pVar)
        nValue = (int)pVar->m_pValue;
    return nValue;
}

// FUNCTION: SWKOTOR 0x0059A560
float CSWSScriptVarTable::GetFloat(CExoString &sName)
{
    float fValue = 0.0f;
    CSWSScriptVar *pVar = MatchIndex(sName, 2, 0);
    if (pVar)
        fValue = *(float *)&pVar->m_pValue;
    return fValue;
}

// FUNCTION: SWKOTOR 0x0059A590
CExoString CSWSScriptVarTable::GetString(CExoString &sName)
{
    CExoString s;
    CSWSScriptVar *pVar = MatchIndex(sName, 3, 0);
    if (pVar)
        s = *(CExoString *)pVar->m_pValue;
    return s;
}

// FUNCTION: SWKOTOR 0x0059A620
unsigned long CSWSScriptVarTable::GetObject(CExoString &sName)
{
    unsigned long oidValue = OBJECT_INVALID;
    CSWSScriptVar *pVar = MatchIndex(sName, 4, 0);
    if (pVar)
        oidValue = (unsigned long)pVar->m_pValue;
    return oidValue;
}

// FUNCTION: SWKOTOR 0x0059A650
CScriptLocation CSWSScriptVarTable::GetLocation(CExoString &sName)
{
    CScriptLocation loc;
    CSWSScriptVar *pVar = MatchIndex(sName, 5, 0);
    if (pVar)
        loc = *(CScriptLocation *)pVar->m_pValue;
    return loc;
}

// FUNCTION: SWKOTOR 0x0059A6F0
void CSWSScriptVarTable::SetInt(CExoString &sName, int nValue, int bFlag)
{
    MatchIndex(sName, 1, 1)->m_pValue = (void *)nValue;
    if (!m_pModule)
        return;
    if (sName.GetLength() <= 10)
        return;
    if (!m_pModule->GetGlobalVariables())
        return;
    if (sName.Left(10) == "GLOBALVAR_") {
        if (sName[11] == 'E')
            m_pModule->GetGlobalVariables()->SetValueNumber(sName.SubString(16, 128), nValue, bFlag);
        else if (sName[11] == 'D')
            m_pModule->GetGlobalVariables()->SetValueBoolean(sName.SubString(15, 128), nValue, bFlag);
        else if (sName[11] == 'T')
            m_pModule->GetGlobalVariables()->SetValueString(sName.SubString(15, 128), nValue, bFlag);
        else if (sName[11] == 'P')
            m_pModule->GetGlobalVariables()->SetValueLocation(sName.SubString(18, 128), nValue, bFlag);
    }
}

// FUNCTION: SWKOTOR 0x0059A8C0
void CSWSScriptVarTable::SetFloat(CExoString &sName, float fValue)
{
    *(float *)&MatchIndex(sName, 2, 1)->m_pValue = fValue;
}

// FUNCTION: SWKOTOR 0x0059A8E0
void CSWSScriptVarTable::SetString(CExoString &sName, CExoString &sValue)
{
    CSWSScriptVar *pVar = MatchIndex(sName, 3, 1);
    *(CExoString *)pVar->m_pValue = sValue;
}

// FUNCTION: SWKOTOR 0x0059A900
void CSWSScriptVarTable::SetObject(CExoString &sName, unsigned long oidValue)
{
    MatchIndex(sName, 4, 1)->m_pValue = (void *)oidValue;
}

// FUNCTION: SWKOTOR 0x0059A920
void CSWSScriptVarTable::SetLocation(CExoString &sName, CScriptLocation lValue)
{
    *(CScriptLocation *)MatchIndex(sName, 5, 1)->m_pValue = lValue;
}

// FUNCTION: SWKOTOR 0x0059A980
CSWSScriptVarTable::~CSWSScriptVarTable()
{
    for (int i = 0; i < m_lVars.num; i++) {
        if (m_lVars.element[i].m_nType == 3) {
            delete (CExoString *)m_lVars.element[i].m_pValue;
        }
        if (m_lVars.element[i].m_nType == 5) {
            delete (CScriptLocation *)m_lVars.element[i].m_pValue;
        }
    }
}

// FUNCTION: SWKOTOR 0x0059AA50
void CSWSScriptVarTable::DestroyFloat(CExoString &sName)
{
    DeleteIndex(sName, 2);
}

// FUNCTION: SWKOTOR 0x0059AA60
void CSWSScriptVarTable::DestroyString(CExoString &sName)
{
    DeleteIndex(sName, 3);
}

// FUNCTION: SWKOTOR 0x0059AA70
void CSWSScriptVarTable::DestroyObject(CExoString &sName)
{
    DeleteIndex(sName, 4);
}

// FUNCTION: SWKOTOR 0x0059AA80
void CSWSScriptVarTable::LoadVarTable(CResGFF *pGFF, CResStruct *pStruct)
{
    CScriptLocation lValue;
    CExoString sName;
    CExoString sValue;
    CResList cList;
    CResStruct cElement;
    int bSuccess;

    if (pGFF->GetList(&cList, pStruct, "VarTable")) {
        for (unsigned long i = 0; i < pGFF->GetListCount(&cList); i++) {
            pGFF->GetListElement(&cElement, &cList, i);
            sName = pGFF->ReadFieldCExoString(&cElement, "Name", bSuccess, "");
            switch (pGFF->ReadFieldDWORD(&cElement, "Type", bSuccess, 0)) {
            case 2:
                SetFloat(sName, pGFF->ReadFieldFLOAT(&cElement, "Value", bSuccess, 0.0f));
                break;
            case 1:
            {
                int nValue = pGFF->ReadFieldINT(&cElement, "Value", bSuccess, 0);
                SetInt(sName, nValue, 1);
            }
                break;
            case 5: {
                CResStruct cLocation;
                pGFF->GetStructFromStruct(&cLocation, &cElement, "Value");
                lValue.LoadLocation(pGFF, &cLocation);
                SetLocation(sName, lValue);
                break;
            }
            case 4:
                SetObject(sName, pGFF->ReadFieldDWORD(&cElement, "Value", bSuccess, 0));
                break;
            case 3:
                sValue = pGFF->ReadFieldCExoString(&cElement, "Value", bSuccess, "");
                SetString(sName, sValue);
                break;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x0059ADB0
void CSWSScriptVarTable::SaveVarTable(CResGFF *pGFF, CResStruct *pStruct)
{
    CScriptLocation lValue;
    CResList cList;
    CResStruct cElement;

    pGFF->AddList(&cList, pStruct, "VarTable");
    for (int i = 0; i < m_lVars.num; i++) {
        pGFF->AddListElement(&cElement, &cList, 0);
        pGFF->WriteFieldCExoString(&cElement, m_lVars.element[i].m_sName, "Name");
        pGFF->WriteFieldDWORD(&cElement, m_lVars.element[i].m_nType, "Type");
        switch (m_lVars.element[i].m_nType) {
        case 2:
            pGFF->WriteFieldFLOAT(&cElement, GetFloat(m_lVars.element[i].m_sName), "Value");
            break;
        case 1:
            pGFF->WriteFieldINT(&cElement, GetInt(m_lVars.element[i].m_sName), "Value");
            break;
        case 5: {
            lValue = GetLocation(m_lVars.element[i].m_sName);
            CResStruct cLocation;
            pGFF->AddStructToStruct(&cLocation, &cElement, "Value", 1);
            lValue.SaveLocation(pGFF, &cLocation);
            break;
        }
        case 4:
            pGFF->WriteFieldDWORD(&cElement, GetObject(m_lVars.element[i].m_sName), "Value");
            break;
        case 3:
            pGFF->WriteFieldCExoString(&cElement, GetString(m_lVars.element[i].m_sName), "Value");
            break;
        }
    }
}
