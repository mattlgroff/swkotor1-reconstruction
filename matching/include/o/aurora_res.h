// Worker O matching headers: the CRes subclasses of the Aurora texture/model
// resources (0x0070C690-0x00713170) and the CResHelper<T, nType> template
// that the CAurora* wrappers and CLIP derive from.
#ifndef O_AURORA_RES_H
#define O_AURORA_RES_H

class CExoString {
public:
    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    CExoString(const CExoString& s);                // 0x005E5B00
    CExoString(int n);                              // 0x005E5BC0
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    CExoString& operator=(const char* s);           // 0x005E5140
    int operator==(const CExoString& s) const;      // 0x005E5200
    int operator>(const CExoString& s) const;       // 0x005E5490
    int GetLength() const;                          // 0x005E5790
    CExoString LowerCase() const;                   // 0x005E6080

    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04
};

class CResRef {
public:
    CResRef();                                      // 0x00405ED0
    CResRef(const char* szResRef);                  // 0x00406D80
    CResRef(const CExoString& sResRef);             // 0x00406D60
    CResRef(const CResRef& cResRef);                // 0x00406DA0
    void CopyToString(CExoString& sString) const;   // 0x00405F70
    int operator==(const CResRef& cResRef) const;   // 0x004060B0
    int operator==(const char* szResRef) const;     // 0x004060D0
    int operator!=(const char* szResRef) const;     // 0x00406060
    CResRef operator=(const char* szResRef);        // 0x00406290
    CResRef operator=(const CResRef& cResRef);      // 0x00406120
    char m_resRef[16];
};

// vtable 0x0073D86C
class CRes {
public:
    CRes();                                         // 0x00406460
    virtual ~CRes();                                // 0x00409D40
    virtual int GetFixedResourceSize();             // 0x00406490
    virtual int GetFixedResourceDataOffset();       // 0x0063E7F0
    virtual int OnResourceFreed();                  // +0x0c
    virtual int OnResourceServiced();               // +0x10

    int GetDemands();                               // 0x004064B0
    int Request();                                  // 0x00408620
    int CancelRequest();                            // 0x00409B00
    void* Demand();                                 // 0x00409B10
    int ReadRaw(int nSize, char* pBuffer);          // 0x00409B20
    int Release();                                  // 0x00409B80

    unsigned short m_nDemands;                      // 0x04
    unsigned short m_nRequests;                     // 0x06
    unsigned long m_nID;                            // 0x08
    unsigned long m_nStatus;                        // 0x0c
    void* m_pResource;                              // 0x10
    class CExoKeyEntry* m_pKeyEntry;                // 0x14
    long m_nSize;                                   // 0x18
    unsigned long m_nRefs;                          // 0x1c
    int m_bHeaderPrefix;                            // 0x20
    int m_bLargePrefix;                             // 0x24
};

class CExoResMan {
public:
    void SetResObject(const CResRef& cResRef, unsigned short nType, CRes* pRes); // 0x00407680
    CRes* GetResObject(const CResRef& cResRef, unsigned short nType); // 0x004074D0
    unsigned long ReleaseResObject(CRes* pRes);     // 0x00409CF0
    int Exists(const CResRef& cResRef, unsigned short nType, unsigned long* pnTableEntry); // 0x00408BC0
    int AddEncapsulatedResourceFile(const CExoString& sName, unsigned long nPriority); // 0x004087A0
    int RemoveEncapsulatedResourceFile(const CExoString& sName); // 0x00408820
};

extern CExoResMan* g_pExoResMan;                    // 0x007A39E8

// Plain resources: loaded flag, size and data pointer (as CResNCS).
// CResMDX::OnResourceFreed (0x00710E60) and the inherited
// OnResourceServiced (0x005D4C60) are shared by every one of them.

// vtable 0x0075FB20
class CResLIP : public CRes {
public:
    CResLIP();
    virtual ~CResLIP();
    virtual int OnResourceFreed();
    virtual int OnResourceServiced();               // 0x005D4C60
    char* GetLIPDataPtr();                          // 0x00404DD0

    int m_bLoaded;                                  // 0x28
    int m_nLIPSize;                                 // 0x2c
    char* m_pLIP;                                   // 0x30
};

// vtable 0x0075FBA8
class CResVIS : public CRes {
public:
    CResVIS();
    virtual ~CResVIS();
    virtual int OnResourceFreed();
    virtual int OnResourceServiced();               // 0x005D4C60
    char* GetVISDataPtr();                          // 0x00404DD0

    int m_bLoaded;                                  // 0x28
    int m_nVISSize;                                 // 0x2c
    char* m_pVIS;                                   // 0x30
};

// vtable 0x0075FBBC
class CResTXI : public CRes {
public:
    CResTXI();
    virtual ~CResTXI();
    virtual int OnResourceFreed();
    virtual int OnResourceServiced();               // 0x005D4C60
    char* GetTXIDataPtr();                          // 0x00404DD0

    int m_bLoaded;                                  // 0x28
    int m_nTXISize;                                 // 0x2c
    char* m_pTXI;                                   // 0x30
};

// vtable 0x0075FBD0
class CResMDX : public CRes {
public:
    CResMDX();
    virtual ~CResMDX();
    virtual int OnResourceFreed();
    virtual int OnResourceServiced();               // 0x005D4C60
    char* GetMDXDataPtr();                          // 0x00404DD0
    int GetMDXSize();

    int m_bLoaded;                                  // 0x28
    int m_nMDXSize;                                 // 0x2c
    char* m_pMDX;                                   // 0x30
};

// vtable 0x0074C404 (ctor 0x005CEA50 is bucket f)
class CResMDL : public CRes {
public:
    CResMDL();                                      // 0x005CEA50
    virtual ~CResMDL();
    virtual int OnResourceFreed();
    virtual int OnResourceServiced();
    char* GetMDLDataPtr();                          // 0x00404DD0
    int GetMDLSize();                               // 0x00710E50

    int m_bLoaded;                                  // 0x28
    int m_nMDLSize;                                 // 0x2c
    char* m_pMDL;                                   // 0x30
};

struct PLTHeader {
    unsigned long m_nFileType;                      // 0x00 'PLT '
    unsigned long m_nVersion;                       // 0x04
    unsigned long m_nLayers;                        // 0x08
    unsigned long m_bRLE;                           // 0x0c
    unsigned long m_nWidth;                         // 0x10
    unsigned long m_nHeight;                        // 0x14
};

// vtable 0x0075FB94
class CResPLT : public CRes {
public:
    CResPLT();
    virtual ~CResPLT();
    virtual int OnResourceFreed();
    virtual int OnResourceServiced();
    int GetPLTAttrib(unsigned long* pWidth, unsigned long* pHeight, unsigned long* pLayers);
    int ReadRLEPLT();
    unsigned char* GetPLTDataPtr();                 // 0x00712F90 (shared with CResTPC)

    int m_bAllocated;                               // 0x28
    int m_bRLE;                                     // 0x2c
    int m_bLoaded;                                  // 0x30
    unsigned long m_nPLTSize;                       // 0x34
    unsigned long m_nRLESize;                       // 0x38
    unsigned char* m_pPLT;                          // 0x3c
    PLTHeader* m_pHeader;                           // 0x40
};

struct DDSHeader {
    unsigned long m_nWidth;                         // 0x00
    unsigned long m_nHeight;                        // 0x04
    unsigned long m_nBytesPerPixel;                 // 0x08
    unsigned long m_nDataSize;                      // 0x0c
    float m_fAlphaMean;                             // 0x10
};

// vtable 0x0075FBE4
class CResDDS : public CRes {
public:
    CResDDS();
    virtual ~CResDDS();
    virtual int OnResourceFreed();                  // 0x00711010
    virtual int OnResourceServiced();
    unsigned char* GetDDSDataPtr();                 // 0x00404DD0
    int GetDDSAttrib(unsigned long* pWidth, unsigned long* pHeight, unsigned char* pBytesPerPixel,
                     unsigned long* pDataSize, float* pAlphaMean);

    int m_bLoaded;                                  // 0x28
    int m_n2C;                                      // 0x2c
    unsigned char* m_pDDS;                          // 0x30
    DDSHeader* m_pHeader;                           // 0x34
};

struct FourPCHeader {
    unsigned long m_nWidth;                         // 0x00
    unsigned long m_nHeight;                        // 0x04
    float m_fAlphaMean;                             // 0x08
    int m_nDataSize;                                // 0x0c
};

// vtable 0x0075FBF8
class CRes4PC : public CRes {
public:
    CRes4PC();
    virtual ~CRes4PC();
    virtual int OnResourceFreed();
    virtual int OnResourceServiced();
    unsigned char* Get4PCDataPtr();                 // 0x00404DD0
    int Get4PCAttrib(unsigned long* pWidth, unsigned long* pHeight, float* pAlphaMean, int* pDataSize);

    int m_bLoaded;                                  // 0x28
    int m_n2C;                                      // 0x2c
    unsigned char* m_p4PC;                          // 0x30
    FourPCHeader* m_pHeader;                        // 0x34
};

#pragma pack(push, 1)
struct TGAHeader {
    unsigned char m_nIDLength;                      // 0x00
    unsigned char m_nColorMapType;                  // 0x01
    unsigned char m_nImageType;                     // 0x02
    unsigned short m_nColorMapOrigin;               // 0x03
    unsigned short m_nColorMapLength;               // 0x05
    unsigned char m_nColorMapEntrySize;             // 0x07
    unsigned short m_nXOrigin;                      // 0x08
    unsigned short m_nYOrigin;                      // 0x0a
    unsigned short m_nWidth;                        // 0x0c
    unsigned short m_nHeight;                       // 0x0e
    unsigned char m_nBitsPerPixel;                  // 0x10
    unsigned char m_nImageDescriptor;               // 0x11
};
#pragma pack(pop)

// vtable 0x0075FC48
class CResTGA : public CRes {
public:
    CResTGA();
    virtual ~CResTGA();
    virtual int OnResourceFreed();
    virtual int OnResourceServiced();
    int GetTGAAttrib(unsigned long* pWidth, unsigned long* pHeight, unsigned char* pBitsPerPixel);
    int ReadColorMappedRLETGA();
    int ReadUnmappedRLETGA();
    unsigned char* GetTGADataPtr();                 // 0x00712FA0 (shared with CResTPC)

    int m_bAllocated;                               // 0x28
    int m_bColorMapped;                             // 0x2c
    int m_bRLE;                                     // 0x30
    int m_bLoaded;                                  // 0x34
    unsigned long m_nImageSize;                     // 0x38
    unsigned long m_nRLESize;                       // 0x3c
    unsigned char* m_pColorMap;                     // 0x40
    unsigned char* m_pImage;                        // 0x44
    TGAHeader* m_pHeader;                           // 0x48
};

struct TPCHeader {
    unsigned long m_nDataSize;                      // 0x00 (0 = uncompressed)
    float m_fAlphaTest;                             // 0x04
    unsigned short m_nWidth;                        // 0x08
    unsigned short m_nHeight;                       // 0x0a
    unsigned char m_nEncoding;                      // 0x0c
    unsigned char m_nMipMaps;                       // 0x0d
};

// vtable 0x0075FC5C
class CResTPC : public CRes {
public:
    CResTPC();
    virtual ~CResTPC();
    virtual int OnResourceFreed();
    virtual int OnResourceServiced();
    int GetTPCAttrib(int* pWidth, int* pHeight, char* pFormat, int* pCompressedSize, int* pDataSize,
                     int* pTXISize, float* pAlphaTest, char* pMipMaps);
    unsigned char* GetTPCDataPtr();                 // 0x00712F90
    unsigned char* GetTXIDataPtr();                 // 0x00712FA0
    int GetTPCDataSize();                           // 0x00712FB0
    int GetTXIDataSize();                           // 0x00712FC0

    int m_bCompressed;                              // 0x28
    int m_bLoaded;                                  // 0x2c
    int m_nDataSize;                                // 0x30
    int m_nCompressedSize;                          // 0x34
    int m_nTXISize;                                 // 0x38
    unsigned char* m_pTPC;                          // 0x3c
    TPCHeader* m_pHeader;                           // 0x40
    unsigned char* m_pTXI;                          // 0x44
};

struct LTRHeader {
    unsigned long m_nFileType;                      // 0x00 "LTR "
    unsigned long m_nVersion;                       // 0x04 "V1.0"
    unsigned char m_nLetters;                       // 0x08
};

// Letter probability tables for CSWNameGen: one start/middle/end set per
// letter count (single, after one letter, after two letters).
// vtable 0x0075FC34
class CResLTR : public CRes {
public:
    CResLTR();
    virtual ~CResLTR();
    virtual int OnResourceFreed();
    virtual int OnResourceServiced();

    float*** m_pTripleStart;                        // 0x28
    float*** m_pTripleMiddle;                       // 0x2c
    float*** m_pTripleEnd;                          // 0x30
    float** m_pDoubleStart;                         // 0x34
    float** m_pDoubleMiddle;                        // 0x38
    float** m_pDoubleEnd;                           // 0x3c
    float* m_pSingleStart;                          // 0x40
    float* m_pSingleMiddle;                         // 0x44
    float* m_pSingleEnd;                            // 0x48
    LTRHeader* m_pHeader;                           // 0x4c
    unsigned char* m_pLTR;                          // 0x50
    int m_nLTRSize;                                 // 0x54
    int m_bLoaded;                                  // 0x58
};

// Back pointer stored just before or after a resource's payload.
#pragma pack(push, 2)
struct CResHelperInfo {
    unsigned short m_nResType;                      // 0x00
    void* m_pResHelper;                             // 0x02
};
#pragma pack(pop)

// Resource handle base (vftable slot 0 = virtual dtor).
template <class T, int nType>
class CResHelper {
public:
    CResHelper()
    {
        m_pRes = 0;
        m_cResRef = "";
        m_bAutoRequest = 0;
    }
    virtual ~CResHelper();
    void SetResRef(const CResRef& cResRef, int bAutoRequest);

    int m_bAutoRequest;                             // 0x04
    T* m_pRes;                                      // 0x08
    CResRef m_cResRef;                              // 0x0c
};

#endif
