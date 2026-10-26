// CResRef, CResGFF (generic file format) and C2DA, shared by worker B sources.
#ifndef B_RES_GFF_H
#define B_RES_GFF_H

#include "b/exo_string.h"

class CExoLocString;

class CResRef {
public:
    CResRef();                                          // 0x00405ED0
    CResRef(const CExoString& sName);                   // 0x00406D60
    CResRef(const char* szName);                        // 0x00406D80
    CResRef operator=(const char* szName);              // 0x00406290
    CResRef operator=(const CResRef& cResRef);          // 0x00406120
    void CopyToString(CExoString& sString) const;       // 0x00405F70

    char m_resRef[16];
};

class CRes {
public:
    virtual ~CRes();
    int Demand();                                                       // 0x00409B10
    int Dump(int bForce);                                               // 0x00409B40
    int Release();                                                      // 0x00409B80
    int GetLoaded();                                                    // 0x004064A0
};

class CExoResMan {
public:
    CRes* GetResObject(const CResRef& cResRef, unsigned short nType);  // 0x004074D0
    int ReleaseResObject(CRes* pRes);                                   // 0x00409CF0
    int RemoveFile(const CExoString& sFileName, unsigned short nType); // 0x00406B20
    int SetResObject(const CResRef& cResRef, unsigned short nType, CRes* pRes); // 0x00407680
    int CreateSignatureFile(const CExoString& sFileName, int a2, int a3, int a4); // 0x00406C60
    int AddResourceDirectory(const CExoString& sDirectory);            // 0x00408800
    int RemoveResourceDirectory(const CExoString& sDirectory);         // 0x004088D0
};

extern CExoResMan* g_pExoResMan;    // 0x007A39E8

struct CResStruct {
    unsigned long m_nStructIndex;
};

struct CResList {
    unsigned long m_nStructIndex;
    char m_pLabel[16];
};

class CResGFF : public CRes {
public:
    CResGFF();                                                          // 0x004105A0
    CResGFF(unsigned short nType, char* pFileType, const CResRef& cResRef); // 0x00410630
    virtual ~CResGFF();

    int CreateGFFFile(CResStruct* pTopLevelStruct, const CExoString& sFileType,
                      const CExoString& sVersion);                      // 0x00411260
    void GetTopLevelStruct(CResStruct* pStruct);                        // 0x00411240
    int GetStructFromStruct(CResStruct* pDest, CResStruct* pSource, char* szLabel); // 0x00411A10
    int AddStructToStruct(CResStruct* pDest, CResStruct* pSource, char* szLabel,
                          unsigned long nType);                 // 0x004125B0
    int GetList(CResList* pList, CResStruct* pStruct, char* szLabel);   // 0x004118C0
    unsigned long GetListCount(CResList* pList);                        // 0x00411940
    int GetListElement(CResStruct* pStruct, CResList* pList, unsigned long nIndex); // 0x00411990
    unsigned char ReadFieldBYTE(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                unsigned char nDefault);                // 0x00411A60
    short ReadFieldSHORT(CResStruct* pStruct, char* szLabel, int& bSuccess,
                         short nDefault);                               // 0x00411BB0
    int ReadFieldINT(CResStruct* pStruct, char* szLabel, int& bSuccess,
                     int nDefault);                                     // 0x00411C90
    float ReadFieldFLOAT(CResStruct* pStruct, char* szLabel, int& bSuccess,
                         float fDefault);                               // 0x00411D00
    unsigned short ReadFieldWORD(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                 unsigned short nDefault);              // 0x00411B40
    unsigned long ReadFieldDWORD(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                 unsigned long nDefault);               // 0x00411C20
    CResRef ReadFieldCResRef(CResStruct* pStruct, char* szLabel, int& bSuccess,
                             const CResRef& cDefault);          // 0x00411E10
    CExoString ReadFieldCExoString(CResStruct* pStruct, char* szLabel, int& bSuccess,
                                   const CExoString& sDefault);         // 0x00411EC0
    int ReadFieldVOID(CResStruct* pStruct, void* pData, unsigned long nSize, char* szLabel,
                      int& bSuccess, const unsigned char& nDefault);    // 0x00412380
    int AddList(CResList* pList, CResStruct* pStruct, char* szLabel);   // 0x00412450
    int AddListElement(CResStruct* pStruct, CResList* pList, unsigned long nType); // 0x004124E0
    int WriteFieldBYTE(CResStruct* pStruct, unsigned char nValue, char* szLabel); // 0x00412620
    int WriteFieldWORD(CResStruct* pStruct, unsigned short nValue, char* szLabel); // 0x004126C0
    int WriteFieldFLOAT(CResStruct* pStruct, float fValue, char* szLabel); // 0x00412870
    int WriteFieldCExoLocString(CResStruct* pStruct, CExoLocString& lsValue, char* szLabel); // 0x00412A10
    int WriteFieldINT(CResStruct* pStruct, int nValue, char* szLabel);  // 0x004127B0
    int WriteFieldDWORD(CResStruct* pStruct, unsigned long nValue, char* szLabel); // 0x00412760
    int WriteFieldCExoString(CResStruct* pStruct, const CExoString& sValue, char* szLabel); // 0x00412970
    int WriteFieldVOID(CResStruct* pStruct, void* pData, unsigned long nSize, char* szLabel); // 0x00412C10
    int WriteGFFFile(CExoString& sFileName, unsigned short nType);      // 0x00413030

    char pad_0x4[0x89];
    char m_pFileType[4];                                                // 0x8d
    char pad_0x91[0x3];
    int m_bLoaded;                                                      // 0x94
    char pad_0x98[0x8];
};

class C2DA {
public:
    C2DA(CResRef cResRef, int bPrivate);                                // 0x00413CC0
    virtual ~C2DA();                                                    // 0x00413D80
    int Load2DArray();                                                  // 0x004143B0
    int GetCExoStringEntry(int nRow, const CExoString& sColumn, CExoString* pValue); // 0x00413270
    int GetINTEntry(int nRow, int nColumn, int* pValue);                // 0x00413510
    int GetFLOATEntry(int nRow, const CExoString& sColumn, float* pValue); // 0x00413430
    int GetINTEntry(int nRow, const CExoString& sColumn, int* pValue);  // 0x00413660
    int GetINTEntry(const CExoString& sRow, const CExoString& sColumn, int* pValue); // 0x00414110

    char pad_0x4[0x24 - 0x4];
    int m_nNumRows;                                                     // 0x24
    int m_nNumColumns;                                                  // 0x28
    char pad_0x2c[0x28];
};

#endif
