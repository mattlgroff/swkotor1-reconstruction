// CSWGlobalVariableTable (0x00528E50-0x0052B270).

#include "b/global_variable_table.h"

#include <string.h>
#include <ctype.h>

// FUNCTION: SWKOTOR 0x00528E50
unsigned long CSWGlobalVariableTable::Hash(const CExoString& sName)
{
    unsigned long nCRC = 0;
    for (int i = 0; i < sName.GetLength(); i++) {
        nCRC = (nCRC >> 8) ^ m_aCRCTable[(tolower(sName[i]) ^ nCRC) & 0xff];
    }
    return nCRC % GVT_NUM_IDENTIFIERS;
}

// FUNCTION: SWKOTOR 0x00528EC0
int CSWGlobalVariableTable::AddIdentifier(const CExoString& sName, int nKind, int nIndex)
{
    int nSlot = Hash(sName);
    if (m_aIdentifiers[nSlot].m_sName[0] != 0) {
        int nProbe = (nSlot + 1) % GVT_NUM_IDENTIFIERS;
        m_nCollisions++;
        int nLength = 1;
        while (nSlot != nProbe && m_aIdentifiers[nProbe].m_sName[0] != 0) {
            nProbe = (nProbe + 1) % GVT_NUM_IDENTIFIERS;
            nLength++;
        }
        if (nSlot != nProbe) {
            nSlot = nProbe;
        }
        m_nTotalProbeLength += nLength;
        if (nLength > m_nLongestProbe) {
            m_nLongestProbe = nLength;
        }
    }
    strcpy(m_aIdentifiers[nSlot].m_sName, sName.CStr());
    m_aIdentifiers[nSlot].m_nType = (unsigned short)((nKind << 14) ^ (nIndex & 0x3fff));
    return nSlot;
}

// FUNCTION: SWKOTOR 0x00528F70
int CSWGlobalVariableTable::AddBoolean(const CExoString& sName)
{
    int nSlot = -1;
    if (m_nNumBooleans < 900) {
        nSlot = AddIdentifier(sName, 0, m_nNumBooleans);
        m_nNumBooleans++;
    }
    return nSlot;
}

// FUNCTION: SWKOTOR 0x00528FA0
int CSWGlobalVariableTable::AddNumber(const CExoString& sName)
{
    int nSlot = -1;
    if (m_nNumNumbers < 500) {
        nSlot = AddIdentifier(sName, 1, m_nNumNumbers);
        m_nNumNumbers++;
    }
    return nSlot;
}

// FUNCTION: SWKOTOR 0x00528FD0
int CSWGlobalVariableTable::AddLocation(const CExoString& sName)
{
    int nSlot = -1;
    if (m_nNumLocations < 100) {
        nSlot = AddIdentifier(sName, 2, m_nNumLocations);
        m_nNumLocations++;
    }
    return nSlot;
}

// FUNCTION: SWKOTOR 0x00529000
int CSWGlobalVariableTable::AddString(const CExoString& sName)
{
    int nSlot = -1;
    if (m_nNumStrings < 5) {
        nSlot = AddIdentifier(sName, 3, m_nNumStrings);
        m_nNumStrings++;
    }
    return nSlot;
}

// FUNCTION: SWKOTOR 0x00529030
int CSWGlobalVariableTable::FindIdentifierIndex(const CExoString& sName)
{
    int nSlot = Hash(sName);
    if (m_aIdentifiers[nSlot].m_sName[0] != 0 && !sName.CompareNoCase(m_aIdentifiers[nSlot].m_sName)) {
        int nProbe = (nSlot + 1) % GVT_NUM_IDENTIFIERS;
        while (nSlot != nProbe) {
            if (m_aIdentifiers[nProbe].m_sName[0] == 0 || sName.CompareNoCase(m_aIdentifiers[nProbe].m_sName)) {
                break;
            }
            nProbe = (nProbe + 1) % GVT_NUM_IDENTIFIERS;
        }
        nSlot = nProbe;
    }
    if (!sName.CompareNoCase(m_aIdentifiers[nSlot].m_sName)) {
        return -1;
    }
    return nSlot;
}

// FUNCTION: SWKOTOR 0x00529110
void CSWGlobalVariableTable::GetValueBoolean(const CExoString& sName, int* pValue)
{
    int nSlot = FindIdentifierIndex(sName);
    *pValue = 0;
    if (nSlot < 0) {
        CExoString sError;
        sError.Format("Script var %s not in catalogue!", sName.CStr());
        return;
    }
    if ((m_aIdentifiers[nSlot].m_nType & 0xc000) != 0) {
        CExoString sError;
        sError.Format("Script var %s is not a BOOLEAN!", sName.CStr());
        return;
    }
    int nIndex = m_aIdentifiers[nSlot].m_nType & 0x3fff;
    int nByte = nIndex / 8;
    *pValue = (m_aBooleans[nByte] >> (7 - (nIndex - nByte * 8))) & 1;
}

// FUNCTION: SWKOTOR 0x005290C0
void CSWGlobalVariableTable::SetBit(unsigned char* pBits, int nIndex, int bValue)
{
    int nByte = nIndex / 8;
    int nBit = nIndex - nByte * 8;
    unsigned char nMask = (unsigned char)(1 << (7 - nBit));
    if (bValue) {
        pBits[nByte] |= nMask;
    }
    else {
        pBits[nByte] &= ~nMask;
    }
}

// FUNCTION: SWKOTOR 0x00529240
void CSWGlobalVariableTable::GetValueNumber(const CExoString& sName, unsigned char* pValue)
{
    int nSlot = FindIdentifierIndex(sName);
    *pValue = 0;
    if (nSlot < 0) {
        CExoString sError;
        sError.Format("Script var %s not in catalogue!", sName.CStr());
        return;
    }
    if ((m_aIdentifiers[nSlot].m_nType & 0xc000) != 0x4000) {
        CExoString sError;
        sError.Format("Script var %s is not a NUMBER!", sName.CStr());
        return;
    }
    *pValue = m_aNumbers[m_aIdentifiers[nSlot].m_nType & 0x3fff];
}

// FUNCTION: SWKOTOR 0x00529350
void CSWGlobalVariableTable::GetValueLocation(const CExoString& sName, CScriptLocation* pValue)
{
    int nSlot = FindIdentifierIndex(sName);
    if (nSlot < 0) {
        CExoString sError;
        sError.Format("Script var %s not in catalogue!", sName.CStr());
        return;
    }
    if ((m_aIdentifiers[nSlot].m_nType & 0xc000) != 0x8000) {
        CExoString sError;
        sError.Format("Script var %s is not a LOCATION!", sName.CStr());
        return;
    }
    *pValue = m_aLocations[m_aIdentifiers[nSlot].m_nType & 0x3fff];
}

// FUNCTION: SWKOTOR 0x00529460
void CSWGlobalVariableTable::GetValueString(const CExoString& sName, CExoString* pValue)
{
    int nSlot = FindIdentifierIndex(sName);
    if (nSlot < 0) {
        CExoString sError;
        sError.Format("Script var %s not in catalogue!", sName.CStr());
        return;
    }
    if ((m_aIdentifiers[nSlot].m_nType & 0xc000) != 0xc000) {
        CExoString sError;
        sError.Format("Script var %s is not a STRING!", sName.CStr());
        return;
    }
    *pValue = m_aStrings[m_aIdentifiers[nSlot].m_nType & 0x3fff];
}

// FUNCTION: SWKOTOR 0x00529570
void CSWGlobalVariableTable::SetValueBoolean(const CExoString& sName, int bValue)
{
    int nSlot = FindIdentifierIndex(sName);
    if (nSlot < 0) {
        CExoString sError;
        sError.Format("Script var %s not in catalogue!", sName.CStr());
        return;
    }
    if ((m_aIdentifiers[nSlot].m_nType & 0xc000) != 0) {
        CExoString sError;
        sError.Format("Script var %s not a BOOLEAN!", sName.CStr());
        return;
    }
    SetBit(m_aBooleans, m_aIdentifiers[nSlot].m_nType & 0x3fff, bValue);
}

// FUNCTION: SWKOTOR 0x00529680
void CSWGlobalVariableTable::SetValueNumber(const CExoString& sName, int nValue)
{
    int nSlot = FindIdentifierIndex(sName);
    if (nSlot < 0) {
        CExoString sError;
        sError.Format("Script var %s not in catalogue!", sName.CStr());
        return;
    }
    if ((m_aIdentifiers[nSlot].m_nType & 0xc000) != 0x4000) {
        CExoString sError;
        sError.Format("Script var %s is not a NUMBER!", sName.CStr());
        return;
    }
    m_aNumbers[m_aIdentifiers[nSlot].m_nType & 0x3fff] = (unsigned char)nValue;
}

// FUNCTION: SWKOTOR 0x00529790
void CSWGlobalVariableTable::SetValueLocation(const CExoString& sName, const CScriptLocation* pValue)
{
    int nSlot = FindIdentifierIndex(sName);
    if (nSlot < 0) {
        CExoString sError;
        sError.Format("Script var %s not in catalogue!", sName.CStr());
        return;
    }
    if ((m_aIdentifiers[nSlot].m_nType & 0xc000) != 0x8000) {
        CExoString sError;
        sError.Format("Script var %s is not a LOCATION!", sName.CStr());
        return;
    }
    m_aLocations[m_aIdentifiers[nSlot].m_nType & 0x3fff] = *pValue;
}

// FUNCTION: SWKOTOR 0x005298A0
void CSWGlobalVariableTable::SetValueString(const CExoString& sName, const CExoString* pValue)
{
    int nSlot = FindIdentifierIndex(sName);
    if (nSlot < 0) {
        CExoString sError;
        sError.Format("Script var %s not in catalogue!", sName.CStr());
        return;
    }
    if ((m_aIdentifiers[nSlot].m_nType & 0xc000) != 0xc000) {
        CExoString sError;
        sError.Format("Script var %s is not a STRING!", sName.CStr());
        return;
    }
    m_aStrings[m_aIdentifiers[nSlot].m_nType & 0x3fff] = *pValue;
}

// FUNCTION: SWKOTOR 0x0052B170
CSWGlobalVariableTable::CSWGlobalVariableTable()
{
    for (unsigned long n = 0; n <= 0xff; n++) {
        unsigned long nCRC = n;
        for (int k = 8; k > 0; k--) {
            if (nCRC & 1) {
                nCRC = (nCRC >> 1) ^ 0xedb88320;
            }
            else {
                nCRC = nCRC >> 1;
            }
        }
        m_aCRCTable[n] = nCRC;
    }
    memset(m_aIdentifiers, 0, sizeof(m_aIdentifiers));
    m_nNumBooleans = 0;
    m_nNumNumbers = 0;
    m_nNumLocations = 0;
    m_nNumStrings = 0;
    m_nCollisions = 0;
    m_nTotalProbeLength = 0;
    m_nLongestProbe = 0;
    memset(m_aBooleans, 0, sizeof(m_aBooleans));
    memset(m_aNumbers, 0, sizeof(m_aNumbers));
    memset(m_aLocations, 0, sizeof(m_aLocations));
    memset(m_aStrings, 0, sizeof(m_aStrings));
}

// FUNCTION: SWKOTOR 0x0052ABE0
int CSWGlobalVariableTable::ReadTable(CExoString& sFileName)
{
    CResStruct cTopLevel;
    CResList cList;
    CResGFF* pRes = new CResGFF(0, "GVT ", sFileName);
    if (pRes != NULL && pRes->m_bLoaded) {
        pRes->GetTopLevelStruct(&cTopLevel);
        if (pRes->GetList(&cList, &cTopLevel, "CatBoolean")) {
            ReadTableWithCatalogue(pRes, &cTopLevel);
        }
        else {
            ReadTableWithoutCatalogue(pRes, &cTopLevel);
        }
        delete pRes;
        return 1;
    }
    ExoDebugNoOp(0xbc4e);
    if (pRes != NULL) {
        delete pRes;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0052AD10
void CSWGlobalVariableTable::Save(CExoString& sDirectory)
{
    CExoString sFileName = sDirectory + "\\" + "GLOBALVARS";
    WriteTable(sFileName, 1);
}

// FUNCTION: SWKOTOR 0x0052ADE0
int CSWGlobalVariableTable::Load()
{
    CExoString sFileName("GLOBALVARS");
    return ReadTable(sFileName);
}

// FUNCTION: SWKOTOR 0x0052AE50
void CSWGlobalVariableTable::ReadCatalogue(const CResRef& cResRef)
{
    C2DA c2DA(cResRef, 1);
    CExoString sName;
    CExoString sType;
    if (c2DA.Load2DArray()) {
        for (int nRow = 0; nRow < c2DA.m_nNumRows; nRow++) {
            if (c2DA.GetCExoStringEntry(nRow, "Name", &sName)) {
                if (sName.GetLength() < 0x16) {
                    if (c2DA.GetCExoStringEntry(nRow, "Type", &sType)) {
                        if (FindIdentifierIndex(sName) < 0) {
                            int nResult;
                            if (sType.CompareNoCase("BOOLEAN")) {
                                nResult = AddBoolean(sName);
                            }
                            else if (sType.CompareNoCase("NUMBER")) {
                                nResult = AddNumber(sName);
                            }
                            else if (sType.CompareNoCase("LOCATION")) {
                                nResult = AddLocation(sName);
                            }
                            else if (sType.CompareNoCase("STRING")) {
                                nResult = AddString(sName);
                            }
                            else {
                                CExoString sError;
                                sError.Format("Global variable %s has an invalid type!", sName.CStr());
                                nResult = 0;
                            }
                            if (nResult < 0) {
                                CExoString sError;
                                sError.Format("Too many %s globals, %s won't fit!", sType.CStr(), sName.CStr());
                            }
                        }
                        else {
                            CExoString sError;
                            sError.Format("Global variable %s is duplicated!", sName.CStr());
                        }
                    }
                }
                else {
                    CExoString sError;
                    sError.Format("Global variable name %s is longer than %d characters!", sName.CStr(), 0x15);
                }
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x00529EF0
// NONMATCH: the original strength-reduces the slot loop on &m_nType (tests the
// name via [p-0x16]) and stores the ReadFieldVOID default byte after the
// first call's pushes; ours bases the loop on the name. Otherwise identical.
void CSWGlobalVariableTable::ReadTableWithoutCatalogue(CResGFF* pRes, CResStruct* pTopLevel)
{
    CScriptLocation aLocations[100];
    CExoString aStrings[5];
    unsigned char aBooleans[0x71];
    unsigned char aNumbers[500];
    int bSuccess;
    unsigned char nDefault;
    CResList cList;

    memset(aBooleans, 0, sizeof(aBooleans));
    memset(aNumbers, 0, sizeof(aNumbers));
    nDefault = 0;
    pRes->ReadFieldVOID(pTopLevel, aBooleans, 0x71, "ValBoolean", bSuccess, nDefault);
    pRes->ReadFieldVOID(pTopLevel, aNumbers, 500, "ValNumber", bSuccess, nDefault);
    pRes->ReadFieldVOID(pTopLevel, aLocations, 0x960, "ValLocation", bSuccess, nDefault);

    int nStrings = 0;
    if (pRes->GetList(&cList, pTopLevel, "ValString")) {
        nStrings = pRes->GetListCount(&cList) > 5 ? 5 : pRes->GetListCount(&cList);
        for (int i = 0; i < nStrings; i++) {
            CResStruct cElement;
            pRes->GetListElement(&cElement, &cList, i);
            aStrings[i] = pRes->ReadFieldCExoString(&cElement, "String", bSuccess, "");
        }
    }

    int nBoolean = 0;
    unsigned char* pNumber = aNumbers;
    CScriptLocation* pLocation = aLocations;
    CExoString* pString = &aStrings[nStrings];
    for (int nSlot = 0; nSlot < GVT_NUM_IDENTIFIERS; nSlot++) {
        if (m_aIdentifiers[nSlot].m_sName[0] != 0) {
            unsigned short nType = m_aIdentifiers[nSlot].m_nType;
            switch (nType >> 14) {
            case 0: {
                int nByte = nBoolean / 8;
                int nBit = nBoolean - nByte * 8;
                SetBit(m_aBooleans, nType & 0x3fff, (aBooleans[nByte] >> (7 - nBit)) & 1);
                nBoolean++;
                break;
            }
            case 1:
                m_aNumbers[nType & 0x3fff] = *pNumber;
                pNumber++;
                break;
            case 2:
                m_aLocations[nType & 0x3fff] = *pLocation;
                pLocation++;
                break;
            case 3:
                m_aStrings[nType & 0x3fff] = *pString;
                pString++;
                break;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x005299B0
// NONMATCH: same slot-loop base as ReadTableWithoutCatalogue, and the
// original frame is 8 bytes larger (local slot order differs).
void CSWGlobalVariableTable::WriteTable(CExoString& sFileName, int bCatalogue)
{
    CScriptLocation aLocations[100];
    CExoString aStrings[5];
    unsigned char aBooleans[0x71];
    unsigned char aNumbers[500];
    CResStruct cTopLevel;
    CResStruct cElement;
    CResList cCatBoolean;
    CResList cCatNumber;
    CResList cCatLocation;
    CResList cCatString;
    CResList cValString;

    CExoString sPrefix;
    int nColon = sFileName.Find(":", 0);
    if (nColon != -1) {
        sPrefix = sFileName.Left(nColon + 1);
    }

    CResGFF* pRes = new CResGFF();
    pRes->CreateGFFFile(&cTopLevel, "GVT ", "V2.0");
    if (bCatalogue) {
        pRes->AddList(&cCatBoolean, &cTopLevel, "CatBoolean");
        pRes->AddList(&cCatNumber, &cTopLevel, "CatNumber");
        pRes->AddList(&cCatLocation, &cTopLevel, "CatLocation");
        pRes->AddList(&cCatString, &cTopLevel, "CatString");
    }

    memset(aBooleans, 0, sizeof(aBooleans));
    memset(aNumbers, 0, sizeof(aNumbers));
    int nBoolean = 0;
    int nStrings = 0;
    unsigned char* pNumber = aNumbers;
    CScriptLocation* pLocation = aLocations;
    CExoString* pString = aStrings;
    for (int nSlot = 0; nSlot < GVT_NUM_IDENTIFIERS; nSlot++) {
        if (m_aIdentifiers[nSlot].m_sName[0] != 0) {
            unsigned short nType = m_aIdentifiers[nSlot].m_nType;
            CResList* pList;
            switch (nType >> 14) {
            case 0: {
                int nIndex = nType & 0x3fff;
                int nByte = nIndex / 8;
                int nBit = nIndex - nByte * 8;
                SetBit(aBooleans, nBoolean, (m_aBooleans[nByte] >> (7 - nBit)) & 1);
                nBoolean++;
                pList = &cCatBoolean;
                break;
            }
            case 1:
                *pNumber = m_aNumbers[nType & 0x3fff];
                pNumber++;
                pList = &cCatNumber;
                break;
            case 2:
                *pLocation = m_aLocations[nType & 0x3fff];
                pLocation++;
                pList = &cCatLocation;
                break;
            case 3:
                *pString = m_aStrings[nType & 0x3fff];
                nStrings++;
                pString++;
                pList = &cCatString;
                break;
            }
            if (bCatalogue) {
                pRes->AddListElement(&cElement, pList, 0);
                pRes->WriteFieldCExoString(&cElement, m_aIdentifiers[nSlot].m_sName, "Name");
            }
        }
    }

    pRes->WriteFieldVOID(&cTopLevel, aBooleans, m_nNumBooleans / 8 + 1, "ValBoolean");
    pRes->WriteFieldVOID(&cTopLevel, aNumbers, m_nNumNumbers, "ValNumber");
    pRes->WriteFieldVOID(&cTopLevel, aLocations, 0x960, "ValLocation");
    pRes->AddList(&cValString, &cTopLevel, "ValString");
    for (int i = 0; i < nStrings; i++) {
        CResStruct cString;
        pRes->AddListElement(&cString, &cValString, 0);
        pRes->WriteFieldCExoString(&cString, aStrings[i], "String");
    }
    pRes->WriteGFFFile(sFileName, 0);
    if (pRes != NULL) {
        delete pRes;
    }
    if (sPrefix == "REBOOTDATA:") {
        g_pExoResMan->CreateSignatureFile(sFileName, 0, 0, 0);
    }
}

// FUNCTION: SWKOTOR 0x0052A280
// NONMATCH: control flow and calls match; the original frame is 8 bytes
// larger (local slot order), keeps the slot index in esi and stores the
// ReadFieldVOID default byte late.
void CSWGlobalVariableTable::ReadTableWithCatalogue(CResGFF* pRes, CResStruct* pTopLevel)
{
    CScriptLocation aLocations[100];
    CExoString aStrings[5];
    unsigned char aBooleans[0x71];
    unsigned char aNumbers[500];
    CExoString sName;
    CResList cList;
    CResList cValues;
    CResStruct cElement;
    int bSuccess;
    unsigned char nDefault;

    memset(m_aBooleans, 0, sizeof(m_aBooleans));
    memset(m_aNumbers, 0, sizeof(m_aNumbers));
    memset(m_aLocations, 0, sizeof(m_aLocations));
    memset(m_aStrings, 0, sizeof(m_aStrings));
    memset(aBooleans, 0, sizeof(aBooleans));
    memset(aNumbers, 0, sizeof(aNumbers));
    nDefault = 0;

    pRes->ReadFieldVOID(pTopLevel, aBooleans, 0x71, "ValBoolean", bSuccess, nDefault);
    if (bSuccess) {
        pRes->GetList(&cList, pTopLevel, "CatBoolean");
        for (int i = 0; i < (int)pRes->GetListCount(&cList); i++) {
            pRes->GetListElement(&cElement, &cList, i);
            sName = pRes->ReadFieldCExoString(&cElement, "Name", bSuccess, "");
            if (bSuccess && sName != "") {
                int nSlot = FindIdentifierIndex(sName);
                if (nSlot < 0) {
                    nSlot = AddBoolean(sName);
                    if (nSlot < 0) {
                        CExoString sError;
                        sError.Format("Too many BOOLEAN globals, %s won't fit!", sName.CStr());
                    }
                }
                int nByte = i / 8;
                int nBit = i - nByte * 8;
                SetBit(m_aBooleans, m_aIdentifiers[nSlot].m_nType & 0x3fff, (aBooleans[nByte] >> (7 - nBit)) & 1);
            }
        }
    }

    pRes->ReadFieldVOID(pTopLevel, aNumbers, 500, "ValNumber", bSuccess, nDefault);
    if (bSuccess) {
        pRes->GetList(&cList, pTopLevel, "CatNumber");
        for (int i = 0; i < (int)pRes->GetListCount(&cList); i++) {
            pRes->GetListElement(&cElement, &cList, i);
            sName = pRes->ReadFieldCExoString(&cElement, "Name", bSuccess, "");
            if (bSuccess && sName != "") {
                int nSlot = FindIdentifierIndex(sName);
                if (nSlot < 0) {
                    nSlot = AddNumber(sName);
                    if (nSlot < 0) {
                        CExoString sError;
                        sError.Format("Too many NUMBER globals, %s won't fit!", sName.CStr());
                    }
                }
                m_aNumbers[m_aIdentifiers[nSlot].m_nType & 0x3fff] = aNumbers[i];
            }
        }
    }

    pRes->ReadFieldVOID(pTopLevel, aLocations, 0x960, "ValLocation", bSuccess, nDefault);
    if (bSuccess) {
        pRes->GetList(&cList, pTopLevel, "CatLocation");
        CScriptLocation* pLocation = aLocations;
        for (int i = 0; i < (int)pRes->GetListCount(&cList); i++) {
            pRes->GetListElement(&cElement, &cList, i);
            sName = pRes->ReadFieldCExoString(&cElement, "Name", bSuccess, "");
            if (bSuccess && sName != "") {
                int nSlot = FindIdentifierIndex(sName);
                if (nSlot < 0) {
                    nSlot = AddLocation(sName);
                    if (nSlot < 0) {
                        CExoString sError;
                        sError.Format("Too many LOCATION globals, %s won't fit!", sName.CStr());
                    }
                }
                m_aLocations[m_aIdentifiers[nSlot].m_nType & 0x3fff] = *pLocation;
            }
            pLocation++;
        }
    }

    if (pRes->GetList(&cValues, pTopLevel, "ValString")) {
        int nStrings = pRes->GetListCount(&cValues) > 5 ? 5 : pRes->GetListCount(&cValues);
        for (int i = 0; i < nStrings; i++) {
            CResStruct cString;
            pRes->GetListElement(&cString, &cValues, i);
            aStrings[i] = pRes->ReadFieldCExoString(&cString, "String", bSuccess, "");
        }
        pRes->GetList(&cList, pTopLevel, "CatString");
        CExoString* pString = aStrings;
        for (int i = 0; i < (int)pRes->GetListCount(&cList); i++) {
            pRes->GetListElement(&cElement, &cList, i);
            sName = pRes->ReadFieldCExoString(&cElement, "Name", bSuccess, "");
            if (bSuccess && sName != "") {
                int nSlot = FindIdentifierIndex(sName);
                if (nSlot < 0) {
                    nSlot = AddString(sName);
                    if (nSlot < 0) {
                        CExoString sError;
                        sError.Format("Too many STRING globals, %s won't fit!", sName.CStr());
                    }
                }
                m_aStrings[m_aIdentifiers[nSlot].m_nType & 0x3fff] = *pString;
            }
            pString++;
        }
    }
}
