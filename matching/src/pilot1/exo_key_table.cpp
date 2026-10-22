// Pilot group 1: CExoKeyTable. Needs /GX (EH frame for new).

class CExoString {
public:
    char* m_sString;
    int m_nBufferLength;
    CExoString();
    ~CExoString();
    CExoString& operator=(const CExoString& sString);
};

class CResRef {
public:
    char m_resRef[16];
    CResRef();
    CResRef(const CResRef& cResRef);
    CResRef operator=(const char* szResRef);
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

class CExoBaseResFile {
public:
    CExoBaseResFile()
    {
        m_nRefs = 0;
    }
    virtual ~CExoBaseResFile();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void v09(); virtual void v10();
    virtual int OpenFile();     // 0x2c

    CExoString m_sFileName;     // 0x4
    unsigned short m_nFlags;    // 0xc
    char pad_0xe[0x2];
    unsigned long m_n10;        // 0x10
    unsigned long m_n14;        // 0x14
    unsigned long m_n18;        // 0x18
    unsigned long m_n1c;        // 0x1c
    unsigned long m_n20;        // 0x20
    unsigned long m_n24;        // 0x24
    unsigned long m_n28;        // 0x28
    unsigned long m_nRefs;      // 0x2c
};

class CExoResourceImageFile : public CExoBaseResFile {
public:
    CExoResourceImageFile()
    {
        m_n14 = 0;
        m_nFlags = 0;
        m_n10 = 0;
        m_n1c = 0;
        m_n24 = 0;
        m_pHeader = 0;
    }
    virtual ~CExoResourceImageFile();

    RIM_ENTRY* GetEntry(unsigned long nIndex)
    {
        return (RIM_ENTRY*)((char*)m_pHeader + m_pHeader->nOffsetToEntries) + nIndex;
    }

    RIM_HEADER* m_pHeader;      // 0x30
};

struct CExoKeyEntry;

class CExoKeyTable {
public:
    unsigned long m_nTableType;         // 0x0
    char pad_0x4[0x8];
    unsigned long m_nTableSize;         // 0xc
    CExoKeyEntry* m_pTable;             // 0x10
    char pad_0x14[0xc];
    CExoString m_sName;                 // 0x20
    unsigned long m_nTableID;           // 0x28
    int m_bResourceImage;               // 0x2c
    CExoResourceImageFile** m_ppResFile; // 0x30

    CExoKeyEntry* AllocateTable(unsigned long nSize);
    int AddKey(CResRef* pResRef, unsigned long nType, unsigned long nID, int nFlags);
    int AddResourceImageContents(int nFlags);
};

// FUNCTION: SWKOTOR 0x0040F990
int CExoKeyTable::AddResourceImageContents(int nFlags)
{
    CResRef cResRef;
    RIM_ENTRY entry;

    m_bResourceImage = 1;
    m_ppResFile = new CExoResourceImageFile*;
    if (m_ppResFile == 0) {
        return 0;
    }
    *m_ppResFile = new CExoResourceImageFile;
    if (*m_ppResFile == 0) {
        delete m_ppResFile;
        m_ppResFile = 0;
        return 0;
    }
    (*m_ppResFile)->m_sFileName = m_sName;
    if (!(*m_ppResFile)->OpenFile()) {
        return 0;
    }
    RIM_HEADER* pHeader = (*m_ppResFile)->m_pHeader;
    if (pHeader->nEntries <= 0) {
        m_nTableType = 0;
        if (*m_ppResFile) {
            delete *m_ppResFile;
        }
        delete m_ppResFile;
        m_ppResFile = 0;
        return 0;
    }
    m_nTableSize = (unsigned long)(pHeader->nEntries * 1.2);
    m_pTable = AllocateTable(m_nTableSize);
    if (m_pTable == 0) {
        m_nTableType = 0;
        if (*m_ppResFile) {
            delete *m_ppResFile;
        }
        delete m_ppResFile;
        m_ppResFile = 0;
        return 0;
    }
    for (unsigned long i = 0; i < pHeader->nEntries; i++) {
        entry = *(*m_ppResFile)->GetEntry(i);
        cResRef = entry.resRef;
        entry.nID = (((m_nTableID & 0x3ff) | 0x400) << 20) | (entry.nID & 0xfffff);
        AddKey(&cResRef, entry.nType, entry.nID, nFlags);
    }
    return 1;
}
