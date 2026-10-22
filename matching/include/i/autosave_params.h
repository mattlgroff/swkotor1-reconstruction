// KOTOR_AUTOSAVE_PARAMS, CStatusSummary, CServerInfo dtor and the
// small leaves around them (0x004B1330-0x004B28E0). Self-contained.
#ifndef I_AUTOSAVE_PARAMS_H
#define I_AUTOSAVE_PARAMS_H

class CExoString {
public:
    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    CExoString(const CExoString& s);                // 0x005E5B00
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const char* s);           // 0x005E5140
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    int operator==(const CExoString& s) const;      // 0x005E5200
    char* CStr() const;                             // 0x005E5670
    char* m_sString;
    unsigned long m_nBufferLength;
};

struct CResStruct {
    unsigned long m_nStructIndex;
};

class CResGFF {
public:
    int AddStructToStruct(CResStruct* pOut, CResStruct* pStruct, char* szLabel, unsigned long nType); // 0x004125B0
    int WriteFieldBYTE(CResStruct* pStruct, unsigned char nValue, char* szLabel);   // 0x00412620
    int WriteFieldWORD(CResStruct* pStruct, unsigned short nValue, char* szLabel);  // 0x004126C0
    int WriteFieldDWORD(CResStruct* pStruct, unsigned long nValue, char* szLabel);  // 0x00412760
    int WriteFieldINT(CResStruct* pStruct, int nValue, char* szLabel);              // 0x004127B0
    int WriteFieldCExoString(CResStruct* pStruct, const CExoString& sValue, char* szLabel); // 0x00412970
};

class CExoLinkedListInternal {
public:
    CExoLinkedListInternal();                       // 0x005E9AA0
    void* m_pHead;
    void* m_pTail;
    unsigned long m_nCount;
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedList() { m_pList = new CExoLinkedListInternal; }
    CExoLinkedListInternal* m_pList;
};

class CSWSClient;

template <class T>
class CExoArrayList {
public:
    int count(T t);
    T* m_pData;
    int m_nSize;
    int m_nAllocated;
};

class CServerInfo {
public:
    ~CServerInfo();                                 // 0x004B1360
    char m_pad0[8];
    CExoString m_s08;                               // +0x08
    CExoString m_s10;                               // +0x10
    CExoString m_s18;                               // +0x18
    CExoString m_s20;                               // +0x20
    char m_pad28[0xfc - 0x28];
    CExoString m_sModuleName;                       // +0xfc
};

struct CStatusSummary {
    int SaveToGFF(CResGFF* pGFF, CResStruct* pStruct);  // 0x004B26C0
    unsigned long m_nFlags;                         // +0x00
    unsigned char m_nLightShift;                    // +0x04
    unsigned char m_nDarkShift;                     // +0x05
    unsigned char m_pad6[2];
    int m_nCredits;                                 // +0x08
    int m_nXP;                                      // +0x0c
    int m_nStealthXP;                               // +0x10
    int m_nSuppressed;                              // +0x14
};

struct KOTOR_AUTOSAVE_PARAMS {
    KOTOR_AUTOSAVE_PARAMS();                        // 0x004B2840
    ~KOTOR_AUTOSAVE_PARAMS();                       // 0x004B2230
    int Reset();                                    // 0x004B1400
    int SaveToGFF(CResGFF* pGFF, CResStruct* pStruct);  // 0x004B28E0

    CExoString m_sLoadMusic;                        // +0x00
    CExoString m_sStartWaypoint;                    // +0x08
    CExoString m_sMovie1;                          // +0x10
    CExoString m_sMovie2;                          // +0x18
    CExoString m_sMovie3;                          // +0x20
    CExoString m_sMovie4;                          // +0x28
    CExoString m_sMovie5;                          // +0x30
    CExoString m_sMovie6;                          // +0x38
    unsigned long m_nYear;                          // +0x40
    unsigned char m_nMonth;                         // +0x44
    unsigned char m_nDay;                           // +0x45
    unsigned char m_nHour;                          // +0x46
    unsigned char m_pad47;
    unsigned short m_nMinute;                       // +0x48
    unsigned short m_nSecond;                       // +0x4a
    unsigned short m_nMillisecond;                  // +0x4c
    unsigned short m_pad4e;
    unsigned long m_nPauseDay;                      // +0x50
    unsigned long m_nPauseTime;                     // +0x54
    CStatusSummary m_cStatusSummary;                // +0x58
};

class CVirtualMachineCmdImplementer {
public:
    virtual ~CVirtualMachineCmdImplementer();       // 0x004B1330
};

class CSWVirtualMachineCommands : public CVirtualMachineCmdImplementer {
public:
    virtual ~CSWVirtualMachineCommands();           // 0x0052C070
};

// Unnamed 0x3c-byte record; FUN_004b13e0 seeds dwords 0, 1, 4 and 14.
struct CUnknown004b13e0 {
    void FUN_004b13e0(unsigned long nValue);        // 0x004B13E0
    unsigned long m_n00;
    unsigned long m_n04;
    unsigned long m_pad08[2];
    unsigned long m_n10;
    unsigned long m_pad14[9];
    unsigned long m_n38;
};

// Unnamed 0x48-byte record built before CSWSArea::TestDirectLine in the
// area path helpers (0x004BCxxx); the ctor zeroes it and sets +0x40 to
// OBJECT_INVALID.
struct CUnknown004b14b0 {
    CUnknown004b14b0();                             // 0x004B14B0
    unsigned long m_n00;
    unsigned long m_n04;
    unsigned long m_n08;
    unsigned long m_n0c;
    unsigned long m_n10;
    unsigned long m_n14;
    unsigned long m_n18;
    unsigned long m_n1c;
    unsigned long m_n20;
    unsigned long m_n24;
    unsigned long m_n28;
    unsigned long m_n2c;
    unsigned long m_n30;
    unsigned long m_n34;
    unsigned long m_n38;
    unsigned long m_n3c;
    unsigned long m_oid40;                          // +0x40
    unsigned long m_n44;                            // +0x44
};

#endif
