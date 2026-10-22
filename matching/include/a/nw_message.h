// CNWMessage / CSWMessage stream helpers and CSWCCMessageData
// (0x004D60F0-0x004D7780). Self-contained declarations.
#ifndef A_NW_MESSAGE_H
#define A_NW_MESSAGE_H

#include <stddef.h>
#include <string.h>

class CExoString {
public:
    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    CExoString(const CExoString& s);                // 0x005E5B00
    CExoString(const char* s, int nLength);         // 0x005E5B70
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    char* CStr() const;                             // 0x005E5670
    int GetLength() const;                          // 0x005E5790
    char* m_sString;
    unsigned long m_nBufferLength;
};

class CResRef {
public:
    CResRef(const char* s);                         // 0x00406D80
    CResRef(const CResRef& r);                      // 0x00406DA0
    char* GetResRef();                              // 0x00641DB0 (returns this)
    char m_resRef[16];
};

struct Vector {
    float x, y, z;
};

struct CResStruct {
    unsigned long m_nStructIndex;
};

struct CResList {
    unsigned long m_nStructIndex;
    char m_pLabel[16];
};

class CResGFF {
public:
    int AddList(CResList* pList, CResStruct* pStruct, char* szLabel);               // 0x00412450
    int AddListElement(CResStruct* pStruct, CResList* pList, unsigned long nType);  // 0x004124E0
    int WriteFieldBYTE(CResStruct* pStruct, unsigned char nValue, char* szLabel);   // 0x00412620
    int WriteFieldDWORD(CResStruct* pStruct, unsigned long nValue, char* szLabel);  // 0x00412760
    int WriteFieldINT(CResStruct* pStruct, int nValue, char* szLabel);              // 0x004127B0
    int WriteFieldFLOAT(CResStruct* pStruct, float fValue, char* szLabel);          // 0x00412870
    int WriteFieldCExoString(CResStruct* pStruct, const CExoString& sValue, char* szLabel); // 0x00412970
};

template <class T> class CExoArrayList {
public:
    CExoArrayList(int nSize = 0)
    {
        num = 0;
        array_size = nSize;
        element = NULL;
        if (array_size) {
            element = new T[array_size];
        }
    }
    ~CExoArrayList() { delete[] element; element = 0; }
    void Allocate(int n);                           // <unsigned long> 0x005E03C0
    void SetSize(int nSize)                         // <CExoString> 0x00405950
    {
        if (nSize == 0) {
            if (element) {
                delete[] element;
                element = NULL;
                array_size = 0;
            }
        }
        else {
            Allocate(nSize);
        }
        num = nSize;
    }
    T* element;
    int num;
    int array_size;
};

extern const float g_fMessageZero;                  // 0x0073D700 (0.0f)

// vtable 0x007465A4
class CNWMessage {
public:
    CNWMessage();
    virtual ~CNWMessage();

    void SetReadMessage(unsigned char* pBuffer, unsigned long nSize,
                        unsigned long nPlayerId, unsigned long nFlags);
    unsigned char* ReadVOIDPtr(unsigned long nSize);
    int MessageReadOverflow(int bFlag);
    int MessageReadUnderflow(int bFlag);
    int MoreDataToRead();
    void SetReadPositionToEnd();
    int ReadBOOL();
    unsigned char ReadBYTE(int nBits);
    unsigned short ReadWORD(int nBits);
    unsigned long ReadDWORD(int nBits);
    float ReadFLOAT(float fScale, int nBits);
    CExoString ReadCExoString(int nBits);
    Vector ReadVector();

    void ExtendWriteBuffer(unsigned long nSize);
    int GetWriteMessage(unsigned char** ppBuffer, unsigned long* pSize);
    void WriteBOOL(int bValue);
    void WriteBYTE(unsigned char nValue, int nBits);
    void WriteWORD(unsigned short nValue, int nBits);
    void WriteDWORD(unsigned long nValue, int nBits);
    void WriteFLOAT(float fValue, float fScale, int nBits);
    void WriteCExoString(const CExoString& sValue, int nBits);
    void WriteCResRef(CResRef cResRef, int nSize);
    void WriteVOIDPtr(void* pData, unsigned long nSize);
    void WriteVector(const Vector& v);

    unsigned char* m_pnWriteBuffer;         // 0x04
    unsigned long m_nWriteBufferSize;       // 0x08
    unsigned long m_nWriteBufferPtr;        // 0x0c
    unsigned char* m_pnWriteFragments;      // 0x10
    unsigned long m_n14;                    // 0x14
    unsigned long m_n18;                    // 0x18
    unsigned long m_nWriteBytesTotal;       // 0x1c
    unsigned long m_n20;                    // 0x20
    int m_bWriting;                         // 0x24
    unsigned char* m_pnReadBuffer;          // 0x28
    unsigned long m_nReadBufferSize;        // 0x2c
    unsigned long m_nReadBufferPtr;         // 0x30
    unsigned long m_n34;                    // 0x34
    unsigned long m_n38;                    // 0x38
    unsigned long m_n3c;                    // 0x3c
    unsigned long m_n40;                    // 0x40
    unsigned char m_n44;                    // 0x44
    unsigned char m_n45;                    // 0x45
    unsigned char pad_0x46[2];
    unsigned long m_nFlags;                 // 0x48
    unsigned long m_nPlayerId;              // 0x4c
};

class CSWMessage : public CNWMessage {
public:
    CResRef ReadCResRef(int nSize);
    void CreateWriteMessage(unsigned long nSize, unsigned long nPlayerId, unsigned long nFlags);
};

class CSWCCMessageData {
public:
    CSWCCMessageData();
    ~CSWCCMessageData();
    int GetInteger(int nIndex);
    float GetFloat(int nIndex);
    unsigned long GetObjectID(int nIndex);
    CExoString GetString(int nIndex);
    void SetInteger(int nIndex, int nValue);
    void SetFloat(int nIndex, float fValue);
    void SetObjectID(int nIndex, unsigned long oidValue);
    void SetString(int nIndex, const CExoString& sValue);
    int SaveData(CResGFF* pRes, CResStruct* pStruct);

    unsigned char m_nType;                      // 0x00
    CExoArrayList<int> m_lstIntegers;           // 0x04
    CExoArrayList<float> m_lstFloats;           // 0x10
    CExoArrayList<CExoString> m_lstStrings;     // 0x1c
    CExoArrayList<unsigned long> m_lstObjects;  // 0x28
};

#endif
