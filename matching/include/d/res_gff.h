// CResGFF (generic file format resource) and C2DA.
// Layouts from docs/batches/b0041_gff_*.md and b0041_c2da_lookup.md.
// Names follow BioWare's Aurora conventions: CResStruct / CResList are the
// caller-side handles, CResGFFStruct / CResGFFField / CResGFFLabel the
// on-disk records.
#ifndef D_RES_GFF_H
#define D_RES_GFF_H

#include "d/exo_key_table.h"

extern "C" char* __cdecl strncpy(char* dst, const char* src, unsigned int n);
extern "C" void* __cdecl memcpy(void* dst, const void* src, unsigned int n);
extern "C" void* __cdecl memset(void* dst, int c, unsigned int n);
#pragma intrinsic(memcpy, memset)

extern char* g_sGFFVersion;                 // 0x0078d3cc -> "V3.2"

class Vector {
public:
    float x, y, z;
};

class Quaternion {
public:
    float x, y, z, w;
};

struct CResGFFHeader {
    unsigned long m_nFileType;              // 0x00
    unsigned long m_nFileVersion;           // 0x04
    unsigned long m_nStructOffset;          // 0x08
    unsigned long m_nStructCount;           // 0x0c
    unsigned long m_nFieldOffset;           // 0x10
    unsigned long m_nFieldCount;            // 0x14
    unsigned long m_nLabelOffset;           // 0x18
    unsigned long m_nLabelCount;            // 0x1c
    unsigned long m_nFieldDataOffset;       // 0x20
    unsigned long m_nFieldDataCount;        // 0x24
    unsigned long m_nFieldIndicesOffset;    // 0x28
    unsigned long m_nFieldIndicesCount;     // 0x2c
    unsigned long m_nListIndicesOffset;     // 0x30
    unsigned long m_nListIndicesCount;      // 0x34
};

struct CResGFFStruct {
    unsigned long m_nType;                  // 0x00
    unsigned long m_nDataOrDataOffset;      // 0x04
    unsigned long m_nFieldCount;            // 0x08
};

struct CResGFFField {
    unsigned long m_nType;                  // 0x00
    unsigned long m_nLabelIndex;            // 0x04
    unsigned long m_nDataOrDataOffset;      // 0x08
};

struct CResGFFLabel {
    char m_pLabel[16];
};

struct CResStruct {
    unsigned long m_nStructIndex;
};

struct CResList : public CResStruct {
    char m_pLabel[16];
};

// vtable 0x0073e2c4
class CResGFF : public CRes {
public:
    CResGFF();
    virtual ~CResGFF();
    virtual int OnResourceFreed();
    virtual int OnResourceServiced();

    unsigned long m_nStructGrowth;          // 0x28
    unsigned long m_nFieldGrowth;           // 0x2c
    unsigned long m_nLabelGrowth;           // 0x30
    unsigned long m_nFieldDataGrowth;       // 0x34
    unsigned long m_nFieldIndicesGrowth;    // 0x38
    unsigned long m_nListIndicesGrowth;     // 0x3c
    CResGFFHeader* m_pFileHeader;           // 0x40
    CResGFFStruct* m_pStructBuffer;         // 0x44
    unsigned long m_nStructBufferSize;      // 0x48
    CResGFFField* m_pFieldBuffer;           // 0x4c
    unsigned long m_nFieldBufferSize;       // 0x50
    CResGFFLabel* m_pLabelBuffer;           // 0x54
    unsigned long m_nLabelBufferSize;       // 0x58
    unsigned char* m_pFieldDataBuffer;      // 0x5c
    unsigned long m_nFieldDataBufferSize;   // 0x60
    unsigned char* m_pFieldIndicesArray;    // 0x64
    unsigned long m_nFieldIndicesArraySize; // 0x68
    unsigned long m_nFieldIndicesWasted;    // 0x6c
    unsigned char* m_pListIndicesArray;     // 0x70
    unsigned long m_nListIndicesArraySize;  // 0x74
    unsigned long m_nListIndicesWasted;     // 0x78
    char pad_0x7c[0x10];
    unsigned char m_bLoaded;                // 0x8c
    char m_pFileType[4];                    // 0x8d
    char pad_0x91[3];
    int m_bValidationFailed;                // 0x94
    int m_bResourceLoaded;                  // 0x98
    int m_bOwnsData;                        // 0x9c

    CResGFF(unsigned short nType, char* pFileType, const CResRef& cResRef);
    void ReleaseResource();
    void InitializeForWriting();            // 0x00410aa0
    int CreateGFFFile(CResStruct* pStruct, const CExoString& sFileType, const CExoString& sVersion);
    int WriteGFFData(CExoFile& file, unsigned long& nSize);
    unsigned long GetTotalSize();
    void Pack(unsigned char nPercent, unsigned long nMinBytes);
    int WriteGFFFile(const CExoString& sFileName, unsigned short nResType);
    CExoLocString ReadFieldCExoLocString(CResStruct* pStruct, char* szLabel, int& bSuccess, const CExoLocString& lsDefault);
    int WriteFieldCExoLocString(CResStruct* pStruct, CExoLocString& lsValue, char* szLabel);
    CResGFFStruct* GetStruct(CResStruct* pStruct)
    {
        if (pStruct && m_pFileHeader && m_pStructBuffer && pStruct->m_nStructIndex < m_pFileHeader->m_nStructCount) {
            return &m_pStructBuffer[pStruct->m_nStructIndex];
        }
        return 0;
    }
    CResGFFField* GetField(CResGFFStruct* pStruct, unsigned long nIndex);
    CResGFFField* GetField(CResStruct* pStruct, unsigned long nIndex);
    void* GetDataField(CResGFFField* pField, unsigned long& nSize);
    void* GetDataLayoutList(CResGFFField* pField, unsigned long& nSize);
    unsigned long AddStruct(unsigned long nType);
    unsigned long AddLabel(char* szLabel);
    void* AddDataField(unsigned long nOffset, unsigned long nOldSize, unsigned long nNewSize);
    void* AddDataLayoutField(unsigned long nOffset, unsigned long nOldSize, unsigned long nNewSize);
    void* AddDataLayoutList(unsigned long nOffset, unsigned long nOldSize, unsigned long nNewSize);
    unsigned long GetFieldByLabel(CResStruct* pStruct, char* szLabel);
    CResGFFField* AddField(CResStruct* pStruct, char* szLabel, unsigned long nType);
    unsigned long GetFieldType(CResStruct* pStruct, char* szLabel, unsigned long nIndex);
    int GetList(CResList* pList, CResStruct* pStruct, char* szLabel);
    unsigned long GetListCount(CResList* pList);
    int GetListElement(CResStruct* pStruct, CResList* pList, unsigned long nIndex);
    int GetStructFromStruct(CResStruct* pOut, CResStruct* pStruct, char* szLabel);
    unsigned long GetStructID(CResStruct* pStruct);         // 0x004111c0
    unsigned long GetElementCount(CResStruct* pStruct);     // 0x00411200
    int GetTopLevelStruct(CResStruct* pStruct);             // 0x00411240

    unsigned char ReadFieldBYTE(CResStruct* pStruct, char* szLabel, int& bSuccess, unsigned char nDefault);
    char ReadFieldCHAR(CResStruct* pStruct, char* szLabel, int& bSuccess, char nDefault);
    unsigned short ReadFieldWORD(CResStruct* pStruct, char* szLabel, int& bSuccess, unsigned short nDefault);
    short ReadFieldSHORT(CResStruct* pStruct, char* szLabel, int& bSuccess, short nDefault);
    unsigned long ReadFieldDWORD(CResStruct* pStruct, char* szLabel, int& bSuccess, unsigned long nDefault);
    int ReadFieldINT(CResStruct* pStruct, char* szLabel, int& bSuccess, int nDefault);
    float ReadFieldFLOAT(CResStruct* pStruct, char* szLabel, int& bSuccess, float fDefault);
    unsigned __int64 ReadFieldDWORD64(CResStruct* pStruct, char* szLabel, int& bSuccess, unsigned __int64 nDefault);
    CResRef ReadFieldCResRef(CResStruct* pStruct, char* szLabel, int& bSuccess, const CResRef& cDefault);
    CExoString ReadFieldCExoString(CResStruct* pStruct, char* szLabel, int& bSuccess, const CExoString& sDefault);
    Quaternion ReadFieldQuaternion(CResStruct* pStruct, char* szLabel, int& bSuccess, const Quaternion& qDefault);
    Vector ReadFieldVector(CResStruct* pStruct, char* szLabel, int& bSuccess, const Vector& vDefault);
    void* ReadFieldVOID(CResStruct* pStruct, void* pBuffer, unsigned long nSize, char* szLabel, int& bSuccess, void* pDefault);

    int AddList(CResList* pList, CResStruct* pStruct, char* szLabel);
    int AddListElement(CResStruct* pOut, CResList* pList, unsigned long nType);
    int AddStructToStruct(CResStruct* pOut, CResStruct* pStruct, char* szLabel, unsigned long nType);
    int WriteFieldBYTE(CResStruct* pStruct, unsigned char nValue, char* szLabel);
    int WriteFieldCHAR(CResStruct* pStruct, char nValue, char* szLabel);
    int WriteFieldWORD(CResStruct* pStruct, unsigned short nValue, char* szLabel);
    int WriteFieldSHORT(CResStruct* pStruct, short nValue, char* szLabel);
    int WriteFieldDWORD(CResStruct* pStruct, unsigned long nValue, char* szLabel);
    int WriteFieldINT(CResStruct* pStruct, int nValue, char* szLabel);
    int WriteFieldDWORD64(CResStruct* pStruct, unsigned __int64 nValue, char* szLabel);
    int WriteFieldFLOAT(CResStruct* pStruct, float fValue, char* szLabel);
    int WriteFieldCResRef(CResStruct* pStruct, const CResRef& cValue, char* szLabel);
    int WriteFieldCExoString(CResStruct* pStruct, const CExoString& sValue, char* szLabel);
    int WriteFieldVOID(CResStruct* pStruct, void* pData, unsigned long nSize, char* szLabel);
    int WriteFieldQuaternion(CResStruct* pStruct, const Quaternion& qValue, char* szLabel);
    int WriteFieldVector(CResStruct* pStruct, const Vector& vValue, char* szLabel);
};

#endif
