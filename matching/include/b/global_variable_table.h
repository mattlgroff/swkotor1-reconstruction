// CSWGlobalVariableTable: script global variables (GLOBALVARS.res).
#ifndef B_GLOBAL_VARIABLE_TABLE_H
#define B_GLOBAL_VARIABLE_TABLE_H

#include "b/exo_string.h"
#include "b/res_gff.h"

class CScriptLocation {
public:
    CScriptLocation();      // 0x004CA7A0
    ~CScriptLocation();     // 0x004015A0 (shared empty function)
    int Equals(CScriptLocation* pOther);    // 0x0052BF80

    float m_vPosition[3];
    float m_vOrientation[3];
};

struct CSWGlobalVariableIdentifier {
    char m_sName[0x16];
    unsigned short m_nType;     // (kind << 14) ^ (index & 0x3fff)
};

#define GVT_NUM_IDENTIFIERS 0x6ef

class CSWGlobalVariableTable {
public:
    CSWGlobalVariableTable();
    ~CSWGlobalVariableTable();                                  // 0x00528DE0
    unsigned long Hash(const CExoString& sName);
    int AddIdentifier(const CExoString& sName, int nKind, int nIndex);
    int FindIdentifierIndex(const CExoString& sName);
    int AddBoolean(const CExoString& sName);
    int AddNumber(const CExoString& sName);
    int AddLocation(const CExoString& sName);
    int AddString(const CExoString& sName);
    void SetBit(unsigned char* pBits, int nIndex, int bValue);  // 0x005290C0

    void GetValueBoolean(const CExoString& sName, int* pValue);
    void GetValueNumber(const CExoString& sName, unsigned char* pValue);
    void GetValueLocation(const CExoString& sName, CScriptLocation* pValue);
    void GetValueString(const CExoString& sName, CExoString* pValue);
    void SetValueBoolean(const CExoString& sName, int bValue);
    void SetValueNumber(const CExoString& sName, int nValue);
    void SetValueLocation(const CExoString& sName, const CScriptLocation* pValue);
    void SetValueString(const CExoString& sName, const CExoString* pValue);

    void WriteTable(CExoString& sFileName, int bCatalogue);
    void ReadTableWithoutCatalogue(CResGFF* pRes, CResStruct* pStruct);
    void ReadTableWithCatalogue(CResGFF* pRes, CResStruct* pStruct);
    int ReadTable(CExoString& sFileName);
    void Save(CExoString& sDirectory);
    int Load();
    void ReadCatalogue(const CResRef& cResRef);

    CSWGlobalVariableIdentifier m_aIdentifiers[GVT_NUM_IDENTIFIERS];  // 0x0000
    unsigned char m_aBooleans[0x71];        // 0xa668
    unsigned char m_aNumbers[500];          // 0xa6d9
    CScriptLocation m_aLocations[100];      // 0xa8d0
    CExoString m_aStrings[5];               // 0xb230
    int m_nNumBooleans;                     // 0xb258
    int m_nNumNumbers;                      // 0xb25c
    int m_nNumLocations;                    // 0xb260
    int m_nNumStrings;                      // 0xb264
    unsigned long m_aCRCTable[256];         // 0xb268
    int m_nCollisions;                      // 0xb668
    int m_nTotalProbeLength;                // 0xb66c
    int m_nLongestProbe;                    // 0xb670
};

#endif
