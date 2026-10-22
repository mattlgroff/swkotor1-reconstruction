// CExoKeyTable, CExoResMan key lookups and the resource-file
// classes (CExoBaseResFile / CExoResFile / CExoEncapsulatedFile /
// CExoResourceImageFile). Layouts from include/kotor/b0040_res_files.h.
#ifndef D_EXO_KEY_TABLE_H
#define D_EXO_KEY_TABLE_H

extern "C" int __cdecl tolower(int c);

class CExoString {
public:
    char* m_sString;
    int m_nBufferLength;
    CExoString();                                   // 0x005b3190
    CExoString(const char* sString);
    CExoString(const char* sString, int nLength);   // 0x005e5b70
    CExoString(int nValue);                         // 0x005e5bc0
    CExoString(const CExoString& sString);
    ~CExoString();
    CExoString& operator=(const CExoString& sString);
    CExoString& operator=(const char* sString);     // 0x005e5140
    int operator==(const char* sString) const;      // 0x005e5290
    int operator==(const CExoString& sString) const; // 0x005e5200
    int Find(char c, int nStart) const;             // 0x005e5f20
    void Format(const char* sFormat, ...);          // 0x005e5680
    CExoString SubString(int nStart, int nCount) const; // 0x005e6270
    int operator!=(const char* sString) const;      // 0x005e5390
    char* CStr() const;                             // 0x005e5670
    int GetLength() const;                          // 0x005e5790
    int IsEmpty() const;                            // 0x005e58a0
    CExoString operator+(const CExoString& sString) const; // 0x005e5d10
    CExoString LowerCase() const;                   // 0x005e6080
    char operator[](int nIndex) const;              // 0x005e5600
    int AsINT() const;                              // 0x005e5630
    float AsFLOAT() const;                          // 0x005e5650
    int CompareNoCase(const CExoString& sString) const; // 0x005e6450
};

class CResRef {
public:
    char m_resRef[16];
    CResRef();                                      // 0x00405ed0
    CResRef(const CResRef& cResRef);
    CResRef(const char* szResRef);
    CResRef(const char* pResRef, unsigned char nLength); // 0x00405ef0
    int operator==(const CResRef& cResRef) const;   // 0x004060b0
    int operator==(const char* szResRef) const;     // 0x004060d0
    int operator!=(const CResRef& cResRef) const;   // 0x00406dc0
    int IsValid() const;                            // 0x00406030
    CResRef operator=(const char* szResRef);        // 0x00406290
    CResRef operator=(const CResRef& cResRef);      // 0x00406120
    CResRef operator=(const char (&pResRef)[16]);   // 0x00406170
    CResRef operator=(const CExoString& sResRef);   // 0x004061f0
    CResRef operator+=(const CExoString& sString);  // 0x00406300
    CResRef(const CExoString& sResRef);             // 0x00406d60
    int operator!=(const CExoString& sResRef) const; // 0x00406de0
    int operator!=(const char* szResRef) const;     // 0x00406060
    void CopyToString(char* pStr) const;            // 0x00405fb0
    void CopyToString(CExoString& sStr) const;      // 0x00405f70
    char* GetResRefStr() const;                     // 0x00405fe0
    unsigned char GetLength() const;                // 0x00406040
    char* GetResRef() const;                        // 0x00641db0
};

typedef void* CExoLinkedListPosition;

struct CExoLinkedListNode {
    CExoLinkedListNode* pPrev;
    CExoLinkedListNode* pNext;
    void* pObject;
};

class CExoLinkedListInternal {
public:
    CExoLinkedListNode* pHead;
    CExoLinkedListNode* pTail;
    unsigned long m_nCount;

    CExoLinkedListInternal();                       // 0x005e9aa0
    void* GetHead()
    {
        void* pObject = pHead;
        if (pHead) {
            pObject = pHead->pObject;
        }
        return pObject;
    }
    ~CExoLinkedListInternal();
    void* GetAtPos(CExoLinkedListPosition pos);     // 0x005E9B60
    void* GetNext(CExoLinkedListPosition& pos);     // 0x005E9B90
    CExoLinkedListPosition AddTail(void* pObject);  // 0x005e9dc0
    CExoLinkedListPosition AddHead(void* pObject);
    void* Remove(CExoLinkedListPosition pos);       // 0x005e9c10
    void* RemoveHead();
    CExoLinkedListPosition GetTailPos();
    void RemoveAll();
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedListInternal* m_pcExoLinkedListInternal;

    CExoLinkedList();
    ~CExoLinkedList() { delete m_pcExoLinkedListInternal; }
    CExoLinkedListPosition GetHeadPos() { return m_pcExoLinkedListInternal->pHead; }
    T* GetHead() { return (T*)m_pcExoLinkedListInternal->GetHead(); }
    CExoLinkedListPosition AddTail(T* pObject) { return m_pcExoLinkedListInternal->AddTail(pObject); }
    CExoLinkedListPosition AddHead(T* pObject) { return m_pcExoLinkedListInternal->AddHead(pObject); }
    T* RemoveHead() { return (T*)m_pcExoLinkedListInternal->RemoveHead(); }
    T* Remove(CExoLinkedListPosition pos) { return (T*)m_pcExoLinkedListInternal->Remove(pos); }
    unsigned long Count() { return m_pcExoLinkedListInternal->m_nCount; }
    T* GetAtPos(CExoLinkedListPosition pos) { return (T*)m_pcExoLinkedListInternal->GetAtPos(pos); }
    T* GetNext(CExoLinkedListPosition& pos) { return (T*)m_pcExoLinkedListInternal->GetNext(pos); }
};

class CExoFile {
public:
    CExoFile(const CExoString& sFileName, unsigned short nResType, const CExoString& sMode); // 0x005e68d0
    ~CExoFile();                                                    // 0x005e69e0
    unsigned long GetSize();                                        // 0x005e6950
    unsigned long Read(void* pBuffer, unsigned long nSize, unsigned long nCount);       // 0x005e6960
    unsigned long ReadAsync(void* pBuffer, unsigned long nSize, unsigned long nCount);  // 0x005e6980
    int IsOpen();                                                   // 0x005e6a10
    int ReadAsyncComplete();                                        // 0x005e6990
    unsigned long Write(const void* pBuffer, unsigned long nSize, unsigned long nCount); // 0x005e69a0
    int Flush();                                                    // 0x005e6a20
    int Seek(long nOffset, int nFrom);                              // 0x005e6aa0
    int SeekBeginning();                                            // 0x005e6ad0

    void* m_pFileInternal;
};

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList()
    {
        num = 0;
        array_size = 0;
        element = 0;
    }
    ~CExoArrayList()
    {
        if (element) {
            delete[] element;
        }
        element = 0;
    }
    T& operator[](int i) { return element[i]; }

    // Out of line in d/exo_array_list.cpp.
    CExoArrayList(int n);
    void Allocate(int n);
    void SetSize(int n);
    void Add(T t);
    void Clear(int bFree);
    CExoArrayList<T>& operator=(const CExoArrayList<T>& o);
};

class CExoAliasList {
public:
    CExoString ResolveFileName(const CExoString& sName, unsigned short nResType); // 0x005e68a0
};

class CExoBase {
public:
    char pad_0x0[0xc];
    CExoAliasList* m_pcExoAliasList;    // 0x0c

    void GetDirectoryList(CExoArrayList<CExoString>* pList, const CExoString& sDirectory, unsigned short nResType,
                          int bSort, int bFullPath);    // 0x005e6640
    unsigned short GetResTypeFromExtension(const CExoString& sExtension); // 0x005e6670
    CExoString& GetResourceExtension(unsigned short nType);  // 0x005e6660
};

extern CExoBase* g_pExoBase;                // 0x007a39e0

class CExoStringList {
public:
    CExoStringList(int bSorted, int bCaseSensitive, int nGrowSize);  // 0x005e9550
    ~CExoStringList();                                              // 0x005e9860
    void Add(CExoString* pString);                                  // 0x005e98b0
    CExoStringList* Merge(CExoStringList* pList, int a, int b);     // 0x005e9980
    int GetCount();                                                 // 0x00675dd0

    char pad[0x14];
};

// vtable 0x0073d86c
class CRes {
public:
    CRes();                                         // 0x00406460
    virtual ~CRes();
    virtual int GetFixedResourceSize();             // 0x04
    virtual int GetFixedResourceDataOffset();       // 0x08
    virtual int OnResourceFreed();                  // 0x0c
    virtual int OnResourceServiced();               // 0x10
    unsigned short m_nDemands;      // 0x04
    unsigned short m_nRequests;     // 0x06
    unsigned long m_nID;            // 0x08
    unsigned long m_nStatus;        // 0x0c
    void* m_pResource;              // 0x10
    class CExoKeyEntry* m_pKeyEntry; // 0x14
    long m_nSize;                   // 0x18
    unsigned long m_nRefs;          // 0x1c
    int m_bHeaderPrefix;            // 0x20
    int m_bLargePrefix;             // 0x24

    unsigned long GetStatus();                      // 0x00405040
    void* GetData();                                // 0x004064a0
    void GetResRef(CResRef& cResRef, unsigned short& nType); // 0x004064c0
    int Request();                                  // 0x00408620
    int CancelRequest();                            // 0x00409b00
    void* Demand();                                 // 0x00409b10
    int Release();                                  // 0x00409b80
    void Dump(int bRemove);         // 0x00409b40
    int GetDemands();               // 0x004064b0
    int ReadRaw(int nSize, char* pBuffer); // 0x00409b20
};

class CExoKeyEntry {
public:
    CExoKeyEntry()
    {
        m_pRes = 0;
        m_nID = 0;
    }

    CResRef m_cResRef;              // 0x00
    CRes* m_pRes;                   // 0x10
    unsigned long m_nID;            // 0x14
    unsigned short m_nRefs;         // 0x18
    unsigned short m_nType;         // 0x1a
};

class CExoLocString {
public:
    CExoLocString();                                                // 0x005e9e10
    CExoLocString(const CExoLocString& lsString);                   // 0x005e9f80
    ~CExoLocString();
    void AddString(int nLanguage, const CExoString& sString, unsigned char nGender); // 0x005ea340
    int GetString(int nIndex, int* nLanguage, CExoString* sString, unsigned char* nGender); // 0x005e9eb0
    unsigned long GetStringCount() const;                           // 0x005e9ef0
    unsigned long GetStringLength(int nIndex);                      // 0x005e9f00
    CExoLocString& operator=(const CExoLocString& lsString);        // 0x005e9f80
    void ClearLocString();                                          // 0x005e9ea0

    void* m_pList;
    unsigned long m_nStrRef;
};

// vtable 0x0073e128
class CExoBaseResFile {
public:
    CExoBaseResFile()
    {
        m_bOpen = 0;
        m_bLoaded = 0;
    }
    virtual ~CExoBaseResFile() {}                                   // 0x0040d560
    virtual void AddRefs() = 0;                                     // 0x04
    virtual void AddAsyncRefs() = 0;                                // 0x08
    virtual int CloseFile() = 0;                                    // 0x0c
    virtual int CloseAsyncFile() = 0;                               // 0x10
    virtual void DelRefs() = 0;                                     // 0x14
    virtual void DelAsyncRefs() = 0;                                // 0x18
    virtual CExoFile* GetFile();                                    // 0x1c 0x0040d570
    virtual CExoFile* GetAsyncFile();                               // 0x20 0x00644760
    virtual unsigned long GetResourceSize(unsigned long nID) = 0;   // 0x24
    virtual int Initialize() = 0;                                   // 0x28
    virtual int OpenFile() = 0;                                     // 0x2c
    virtual int OpenAsyncFile() = 0;                                // 0x30
    virtual unsigned long ReadResource(unsigned long nID, void* pBuffer, unsigned long nSize, unsigned long nOffset) = 0; // 0x34
    virtual void ReadResourceAsync(unsigned long nID, void* pBuffer, unsigned long nSize, unsigned long nOffset) = 0;    // 0x38
    virtual int LoadHeader(unsigned char nType) = 0;                // 0x3c
    virtual int UnloadHeader() = 0;                                 // 0x40

    CExoString m_sFileName;         // 0x04
    unsigned short m_nResType;      // 0x0c
    unsigned long m_nSize;          // 0x10
    CExoFile* m_pFile;              // 0x14
    CExoFile* m_pAsyncFile;         // 0x18
    long m_nRefs;                   // 0x1c
    long m_nAsyncRefs;              // 0x20
    int m_bOpen;                    // 0x24
    int m_bAsyncOpen;               // 0x28
    int m_bLoaded;                  // 0x2c
};

struct BIF_HEADER {
    unsigned long nFileType;        // 0x00
    unsigned long nVersion;         // 0x04
    unsigned long nVarResCount;     // 0x08
    unsigned long nFixedResCount;   // 0x0c
    unsigned long nVarTableOffset;  // 0x10
};

struct BIF_VAR_ENTRY {
    unsigned long nID;              // 0x00
    unsigned long nOffset;          // 0x04
    unsigned long nSize;            // 0x08
    unsigned long nType;            // 0x0c
};

// vtable 0x0073e1b0
class CExoResFile : public CExoBaseResFile {
public:
    CExoResFile();
    virtual ~CExoResFile();
    virtual void AddRefs();
    virtual void AddAsyncRefs();
    virtual int CloseFile();
    virtual int CloseAsyncFile();
    virtual void DelRefs();
    virtual void DelAsyncRefs();
    virtual unsigned long GetResourceSize(unsigned long nID);
    virtual int Initialize()                                        // 0x0040d6b0
    {
        m_sFileName.CStr();
        m_pFile = 0;
        m_pAsyncFile = 0;
        m_nResType = 0;
        m_nSize = 0;
        m_nRefs = 0;
        m_nAsyncRefs = 0;
        m_bOpen = 0;
        m_bAsyncOpen = 0;
        m_pHeader = 0;
        m_pVarTable = 0;
        return 1;
    }
    virtual int OpenFile();
    virtual int OpenAsyncFile();
    virtual unsigned long ReadResource(unsigned long nID, void* pBuffer, unsigned long nSize, unsigned long nOffset);
    virtual void ReadResourceAsync(unsigned long nID, void* pBuffer, unsigned long nSize, unsigned long nOffset);
    virtual int LoadHeader(unsigned char nType);
    virtual int UnloadHeader();

    BIF_HEADER* m_pHeader;          // 0x30
    BIF_VAR_ENTRY* m_pVarTable;     // 0x34
};

struct ERF_HEADER {
    unsigned long nFileType;        // 0x00
    unsigned long nVersion;         // 0x04
    unsigned long nLanguageCount;   // 0x08
    unsigned long nLocalizedStringSize; // 0x0c
    unsigned long nEntryCount;      // 0x10
    unsigned long nOffsetToLocalizedString; // 0x14
    unsigned long nOffsetToKeyList; // 0x18
    unsigned long nOffsetToResourceList; // 0x1c
    char pad_0x20[0x80];
};

struct ERF_RES_ENTRY {
    unsigned long nOffset;          // 0x00
    unsigned long nSize;            // 0x04
};

// vtable 0x0073e1f8
class CExoEncapsulatedFile : public CExoBaseResFile {
public:
    CExoEncapsulatedFile();
    virtual ~CExoEncapsulatedFile();
    virtual void AddRefs();
    virtual void AddAsyncRefs();
    virtual int CloseFile();
    virtual int CloseAsyncFile();
    virtual void DelRefs();
    virtual void DelAsyncRefs();
    virtual unsigned long GetResourceSize(unsigned long nID);
    virtual int Initialize()                                        // 0x0040dc00
    {
        m_pFile = 0;
        m_pAsyncFile = 0;
        m_nResType = 0;
        m_nSize = 0;
        m_nRefs = 0;
        m_nAsyncRefs = 0;
        m_bOpen = 0;
        m_bAsyncOpen = 0;
        m_pHeader = 0;
        m_pResList = 0;
        m_nType = 0;
        return 1;
    }
    virtual int OpenFile();
    virtual int OpenAsyncFile();
    virtual unsigned long ReadResource(unsigned long nID, void* pBuffer, unsigned long nSize, unsigned long nOffset);
    virtual void ReadResourceAsync(unsigned long nID, void* pBuffer, unsigned long nSize, unsigned long nOffset);
    virtual int LoadHeader(unsigned char nType);
    virtual int UnloadHeader();

    CExoLocString m_lsDescription;  // 0x30
    ERF_HEADER* m_pHeader;          // 0x38
    ERF_RES_ENTRY* m_pResList;      // 0x3c
    unsigned char m_nType;          // 0x40
};

struct RIM_HEADER {
    char pad_0x0[0xc];
    unsigned long nEntries;         // 0xc
    unsigned long nOffsetToEntries; // 0x10
};

struct RIM_ENTRY {
    char resRef[16];        // 0x0
    unsigned long nType;    // 0x10
    unsigned long nID;      // 0x14
    unsigned long nOffset;  // 0x18
    unsigned long nSize;    // 0x1c
};

// vtable 0x0073e240
class CExoResourceImageFile : public CExoBaseResFile {
public:
    CExoResourceImageFile()
    {
        m_pFile = 0;
        m_nResType = 0;
        m_nSize = 0;
        m_nRefs = 0;
        m_bOpen = 0;
        m_pHeader = 0;
    }
    virtual ~CExoResourceImageFile();
    virtual void AddRefs();
    virtual void AddAsyncRefs();
    virtual int CloseFile();
    virtual int CloseAsyncFile();
    virtual void DelRefs();
    virtual void DelAsyncRefs();
    virtual unsigned long GetResourceSize(unsigned long nID);
    virtual int Initialize();
    virtual int OpenFile();
    virtual int OpenAsyncFile();
    virtual unsigned long ReadResource(unsigned long nID, void* pBuffer, unsigned long nSize, unsigned long nOffset);
    virtual void ReadResourceAsync(unsigned long nID, void* pBuffer, unsigned long nSize, unsigned long nOffset);
    virtual int LoadHeader(unsigned char nType);
    virtual int UnloadHeader();
    virtual void* GetResource(unsigned long nID);                   // 0x44

    RIM_ENTRY* GetEntry(unsigned long nIndex)
    {
        return (RIM_ENTRY*)((char*)m_pHeader + m_pHeader->nOffsetToEntries) + nIndex;
    }

    RIM_HEADER* m_pHeader;          // 0x30
};

class CExoKeyTable {
public:
    unsigned long m_nTableType;             // 0x00
    int m_bRebuilding;                      // 0x04
    unsigned long m_n08;                    // 0x08
    unsigned long m_nTableSize;             // 0x0c
    CExoKeyEntry* m_pTable;                 // 0x10
    unsigned long m_nKeys;                  // 0x14
    unsigned long m_n18;                    // 0x18
    unsigned long m_nKind;                  // 0x1c
    CExoString m_sName;                     // 0x20
    unsigned long m_nTableID;               // 0x28
    unsigned long m_nFiles;                 // 0x2c
    CExoBaseResFile** m_ppResFile;          // 0x30

    CExoKeyTable();
    ~CExoKeyTable();
    int GetTableIndex(unsigned long* pIndex, const CResRef& cResRef, unsigned short nType);
    unsigned long Hash(const CResRef& cResRef, unsigned short nType);
    CExoKeyEntry* AllocateTable(unsigned long nSize);
    CExoKeyEntry* FindKey(const CResRef& cResRef, unsigned short nType);
    CExoKeyEntry* AddKey(CResRef* pResRef, unsigned short nType, unsigned long nID, int nFlags);
    void DestroyTable();
    int LocateBifFile(const CExoString& sName);
    CExoStringList* GetKeysOfType(unsigned short nType);
    int AddDirectoryContents(int nFlags);
    int AddEncapsulatedContents(int nFlags);
    int AddResourceImageContents(int nFlags);
    int AddKeyTableContents(int nFlags);
    int BuildNewTable(unsigned long nKind, const CExoString& sName, unsigned long nTableID, int nFlags);
    void RebuildTable();
    unsigned long GetEntryCount(int bAll);                          // 0x0040d1a0
};

class CExoCriticalSection {
public:
    CExoCriticalSection();                          // 0x005e9290
    ~CExoCriticalSection();                         // 0x005e9320
    void EnterCriticalSection();                    // 0x005e9300
    void LeaveCriticalSection();                    // 0x005e9310
    void* m_pInternal;
};

class CExoResMan {
public:
    unsigned long m_nTotalPhysicalMemory;                       // 0x00
    long m_nMemoryBudget;                                       // 0x04
    long m_nAvailableMemory;                                    // 0x08
    unsigned long m_n0c;                                        // 0x0c
    CExoLinkedList<CExoKeyTable> m_lstFixedKeyTables;           // 0x10
    CExoLinkedList<CExoKeyTable> m_lstOverrideKeyTables;        // 0x14
    CExoLinkedList<CExoKeyTable> m_lstEncapsulatedKeyTables;    // 0x18
    CExoLinkedList<CExoKeyTable> m_lstDirectoryKeyTables;       // 0x1c
    CExoLinkedList<CRes> m_lstToBeFreed;                        // 0x20
    CExoLinkedList<CRes> m_lstAsyncResQueue;                    // 0x24
    CRes* m_pCurrentAsyncRes;                                   // 0x28
    CExoFile* m_pAsyncFile;                                     // 0x2c
    int m_bAsyncSuspended;                                      // 0x30
    unsigned char m_b34;                                        // 0x34
    void* m_hThread;                                            // 0x38
    unsigned long m_nThreadID;                                  // 0x3c
    CExoCriticalSection m_csAsync;                              // 0x40
    unsigned char m_bImageAddBusy;                              // 0x44
    int m_bShutdown;                                            // 0x48
    CExoString m_sImageName;                                    // 0x4c
    unsigned long m_n54;                                        // 0x54
    unsigned long m_n58;                                        // 0x58
    unsigned char m_b5c;                                        // 0x5c

    CExoResMan();
    ~CExoResMan();
    int Free(CRes* pRes);                                       // 0x00407020
    int FreeChunk();                                            // 0x004070a0
    void FreeResourceData(CRes* pRes);                          // 0x00406540
    void SetResObject(const CResRef& cResRef, unsigned short nType, CRes* pRes); // 0x00407680
    CRes* GetResObject(const CResRef& cResRef, unsigned short nType); // 0x004074d0
    int Malloc(CRes* pRes);                                     // 0x004077b0
    int CancelRequest(CRes* pRes);                              // 0x004088f0
    void* Demand(CRes* pRes);                                   // 0x004089f0
    void Update(int nUnused);                                   // 0x00408d40
    int RemoveKeyTable(const CExoString& sName, unsigned long nTableKind); // 0x00407830
    int AddKeyTable(const CExoString& sName, unsigned long nTableKind, unsigned long nFilter); // 0x00406e20
    int RemoveResourceImageFile(const CExoString& sName);       // 0x00408830
    void AddResourceImageFilesAsync(const CExoString& sName, unsigned long a, unsigned long b); // 0x004064f0
    int ServiceCurrentAsyncRes();                               // 0x00408530
    int GetFreeDiskSpace(const CExoString& sPath, unsigned __int64* pFreeBytes); // 0x004067b0
    int CreateDirectory2(const CExoString& sPath);              // 0x004068c0
    int RemoveFile(const CExoString& sName, unsigned short nResType); // 0x00406b20
    int FileCopy(const CExoString& sSource, const CExoString& sDest, unsigned short nResType,
                 unsigned short nDestResType);                  // 0x00406990
    int Release(CRes* pRes);                                    // 0x00408c90
    void Dump(CRes* pRes, int bRemove);                         // 0x00408b90
    unsigned long ReleaseResObject(CRes* pRes);                 // 0x00409cf0
    int Exists(const CResRef& cResRef, unsigned short nType, unsigned long nTableKind); // 0x00408c00
    int IsSpecialResType(unsigned short nType);                 // 0x00406720
    int CreateSignatureFile(const CExoString& sA, const CExoString& sB, unsigned long nA, unsigned long nB); // 0x00406c60
    int GetTableCount(CRes* pRes, int bAll);                    // 0x00407780
    int AddEncapsulatedResourceFile(const CExoString& sName, unsigned long nFilter); // 0x004087a0
    int AddResourceImageFile(const CExoString& sName);          // 0x004087c0
    int AddFixedKeyTableFile(const CExoString& sName);          // 0x004087e0
    int AddResourceDirectory(const CExoString& sName);          // 0x00408800
    int RemoveEncapsulatedResourceFile(const CExoString& sName); // 0x00408820
    int RemoveFixedKeyTableFile(const CExoString& sName);       // 0x004088c0
    int RemoveResourceDirectory(const CExoString& sName);       // 0x004088d0
    int UpdateKeyTable(const CExoString& sName, unsigned long nTableKind); // 0x00407fe0
    int UpdateResourceDirectory(const CExoString& sName);       // 0x004088e0
    CExoStringList* GetResOfType(unsigned short nType, CRes* pRes); // 0x00408c60
    CExoStringList* GetResOfType(unsigned short nType, int bOnlyBif); // 0x00407390
    int ReadRaw(CRes* pRes, int nSize, char* pBuffer);         // 0x00408e30
    int ServiceFromDirectoryRaw(CRes* pRes, int nSize, char* pBuffer);     // 0x00408080
    int ServiceFromEncapsulatedRaw(CRes* pRes, int nSize, char* pBuffer);  // 0x004082d0
    int ServiceFromImageRaw(CRes* pRes, int nSize, char* pBuffer);         // 0x00408390
    int ServiceFromResFileRaw(CRes* pRes, int nSize, char* pBuffer);       // 0x00408450
    int ServiceFromDirectory(CRes* pRes, int bAsync);           // 0x004078f0
    int ServiceFromEncapsulated(CRes* pRes, int bAsync);        // 0x00407bd0
    int ServiceFromImage(CRes* pRes, int bAsync);               // 0x00407d50
    int ServiceFromResFile(CRes* pRes, int bAsync);             // 0x00407e00

    int GetKeyEntryFromTable(CExoLinkedList<CExoKeyTable>* pList, const CResRef& cResRef, unsigned short nType,
                             CExoKeyTable** ppTable, CExoKeyEntry** ppEntry, unsigned long nFilter);
    int GetKeyEntry(const CResRef& cResRef, unsigned short nType, CExoKeyTable*& pTable, CExoKeyEntry*& pEntry);
    CExoKeyTable* GetTable(CRes* pRes);
    CExoStringList* GetResOfTypeFromTable(CExoLinkedList<CExoKeyTable>* pList, unsigned short nType);
    unsigned short GetResTypeFromFile(const CExoString& sFile);         // 0x00406650
    void GetResRefFromFile(CResRef& cResRef, const CExoString& sFile); // 0x004065a0
};

extern CExoResMan* g_pExoResMan;            // 0x007a39e8
extern unsigned long g_aCRCTable[256];      // 0x007a3d78

// Defined out of line (not inlined at /Ob1): one COMDAT for every T.
template <class T>
CExoLinkedList<T>::CExoLinkedList()
{
    m_pcExoLinkedListInternal = new CExoLinkedListInternal();
}

#endif
