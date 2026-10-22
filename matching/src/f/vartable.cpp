// CSWVarTable (per-object boolean bit array and byte numbers),
// GFF load / save (0x0059AFE0-0x0059B250).

#define min(a, b) (((a) < (b)) ? (a) : (b))

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
    unsigned char ReadFieldBYTE(CResStruct *pStruct, char *szLabel, int &bSuccess, unsigned char nDefault);
    unsigned long ReadFieldDWORD(CResStruct *pStruct, char *szLabel, int &bSuccess, unsigned long nDefault);
    int AddStructToStruct(CResStruct *pOut, CResStruct *pStruct, char *szLabel, unsigned long nType);
    int AddList(CResList *pList, CResStruct *pStruct, char *szLabel);
    int AddListElement(CResStruct *pOut, CResList *pList, unsigned long nType);
    int WriteFieldBYTE(CResStruct *pStruct, unsigned char nValue, char *szLabel);
    int WriteFieldDWORD(CResStruct *pStruct, unsigned long nValue, char *szLabel);
};

class CSWBooleanArray {
public:
    CSWBooleanArray() { for (int i = 0; i < 3; i++) m_a[i] = 0; }
    unsigned long m_a[3];
};

class CSWVarTable {
public:
    CSWVarTable();
    int GetBoolean(int nIndex);
    void SetBoolean(int nIndex, int bValue);
    unsigned char GetNumber(int nIndex);
    void SetNumber(int nIndex, unsigned char nValue);
    void LoadVarTable(CResGFF *pGFF, CResStruct *pStruct);
    void SaveVarTable(CResGFF *pGFF, CResStruct *pStruct);

    CSWBooleanArray m_cBooleans;     // +0x0
    unsigned char m_aNumbers[8];     // +0xc
};

// FUNCTION: SWKOTOR 0x0059AFE0
CSWVarTable::CSWVarTable()
{
    *(unsigned long *)&m_aNumbers[0] = 0;
    *(unsigned long *)&m_aNumbers[4] = 0;
}

// FUNCTION: SWKOTOR 0x0059B000
int CSWVarTable::GetBoolean(int nIndex)
{
    if (nIndex < 0 || nIndex >= 96)
        return 0;
    return m_cBooleans.m_a[nIndex / 32] & (1 << (nIndex % 32));
}

// FUNCTION: SWKOTOR 0x0059B040
void CSWVarTable::SetBoolean(int nIndex, int bValue)
{
    if (nIndex < 0 || nIndex >= 96)
        return;
    if (bValue)
        m_cBooleans.m_a[nIndex / 32] |= (1 << (nIndex % 32));
    else
        m_cBooleans.m_a[nIndex / 32] &= ~(1 << (nIndex % 32));
}

// FUNCTION: SWKOTOR 0x0059B0B0
unsigned char CSWVarTable::GetNumber(int nIndex)
{
    if (nIndex < 0 || nIndex >= 8)
        return 0;
    return m_aNumbers[nIndex];
}

// FUNCTION: SWKOTOR 0x0059B0D0
void CSWVarTable::SetNumber(int nIndex, unsigned char nValue)
{
    if (nIndex < 0 || nIndex >= 8)
        return;
    m_aNumbers[nIndex] = nValue;
}

// FUNCTION: SWKOTOR 0x0059B0F0
void CSWVarTable::LoadVarTable(CResGFF *pGFF, CResStruct *pStruct)
{
    CResStruct cTable;
    CResStruct cElement;
    CResList cList;
    int bSuccess;
    int i;
    int nCount;

    if (!pGFF->GetStructFromStruct(&cTable, pStruct, "SWVarTable"))
        return;

    if (pGFF->GetList(&cList, &cTable, "BitArray")) {
        pGFF->GetListCount(&cList);
        nCount = min(pGFF->GetListCount(&cList), 3);
        for (i = 0; i < nCount; i++) {
            if (pGFF->GetListElement(&cElement, &cList, i))
                m_cBooleans.m_a[i] = pGFF->ReadFieldDWORD(&cElement, "Variable", bSuccess, 0);
        }
    }
    if (pGFF->GetList(&cList, &cTable, "ByteArray")) {
        pGFF->GetListCount(&cList);
        nCount = min(pGFF->GetListCount(&cList), 8);
        for (i = 0; i < nCount; i++) {
            if (pGFF->GetListElement(&cElement, &cList, i))
                m_aNumbers[i] = pGFF->ReadFieldBYTE(&cElement, "Variable", bSuccess, 0);
        }
    }
}

// FUNCTION: SWKOTOR 0x0059B250
void CSWVarTable::SaveVarTable(CResGFF *pGFF, CResStruct *pStruct)
{
    CResStruct cTable;
    CResStruct cElement;
    CResList cList;
    int i;

    pGFF->AddStructToStruct(&cTable, pStruct, "SWVarTable", 0);
    pGFF->AddList(&cList, &cTable, "BitArray");
    for (i = 0; i < 3; i++) {
        pGFF->AddListElement(&cElement, &cList, 0);
        pGFF->WriteFieldDWORD(&cElement, m_cBooleans.m_a[i], "Variable");
    }
    pGFF->AddList(&cList, &cTable, "ByteArray");
    for (i = 0; i < 8; i++) {
        pGFF->AddListElement(&cElement, &cList, 0);
        pGFF->WriteFieldBYTE(&cElement, m_aNumbers[i], "Variable");
    }
}
