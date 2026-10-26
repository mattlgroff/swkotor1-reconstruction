// CSoundSet / CSoundSetList (soundset.o) and their
// CResHelper<CResSSF, 2060> base.

class CExoString {
public:
    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    ~CExoString();                                  // 0x005E5C20
    char* m_sString;
    unsigned long m_nBufferLength;
};

class CResRef {
public:
    CResRef();                                      // 0x00405ED0
    CResRef(const char* szResRef);                  // 0x00406D80
    int operator==(const CResRef& cResRef) const;   // 0x004060B0
    int operator==(const char* szResRef) const;     // 0x004060D0
    int operator!=(const char* szResRef) const;     // 0x00406060
    CResRef operator=(const char* szResRef);        // 0x00406290
    CResRef operator=(const CResRef& cResRef);      // 0x00406120
    CResRef operator=(const CExoString& sResRef);   // 0x004061F0
    char m_resRef[16];
};

class CRes {
public:
    CRes();                                         // 0x00406460
    virtual ~CRes();                                // 0x00409D40

    void* GetData();                                // 0x004064A0
    int IsLoaded();                                 // 0x004064B0
    int Request();                                  // 0x00408620
    int Release();                                  // 0x00409B80
    int CancelRequest();                            // 0x00409B00

    char pad_0x4[0x24];
};

// Sound set resource (restype 2060, .ssf). Descriptive name.
class CResSSF : public CRes {
public:
    CResSSF();                                      // 0x006DB650
    char pad_0x28[0x4];
};

class CExoResMan {
public:
    void SetResObject(const CResRef& cResRef, unsigned short nType, CRes* pRes); // 0x00407680
    CRes* GetResObject(const CResRef& cResRef, unsigned short nType); // 0x004074D0
    unsigned long ReleaseResObject(CRes* pRes);     // 0x00409CF0
};

extern CExoResMan* g_pExoResMan;                    // 0x007A39E8

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

template <class T, int nType>
CResHelper<T, nType>::~CResHelper()
{
    if (m_pRes && m_cResRef != "") {
        if (m_bAutoRequest) {
            m_pRes->CancelRequest();
            m_bAutoRequest = 0;
        }
        if (!g_pExoResMan->ReleaseResObject(m_pRes)) {
            if (m_pRes) {
                delete m_pRes;
            }
            m_pRes = 0;
        }
    }
}

template <class T, int nType>
void CResHelper<T, nType>::SetResRef(const CResRef& cResRef, int bAutoRequest)
{
    if (cResRef == m_cResRef) {
        return;
    }
    if (m_pRes && m_cResRef != "") {
        if (m_bAutoRequest) {
            m_pRes->CancelRequest();
        }
        if (!g_pExoResMan->ReleaseResObject(m_pRes)) {
            if (m_pRes) {
                delete m_pRes;
            }
            m_pRes = 0;
        }
    }
    if (cResRef == "") {
        m_pRes = 0;
        m_cResRef = "";
        return;
    }
    T* pRes = (T*)g_pExoResMan->GetResObject(cResRef, nType);
    if (!pRes) {
        pRes = new T();
        g_pExoResMan->SetResObject(cResRef, nType, pRes);
        if (!pRes) {
            return;
        }
    }
    m_pRes = pRes;
    if (bAutoRequest) {
        m_bAutoRequest = 1;
        m_pRes->Request();
    }
    m_cResRef = cResRef;
}

struct STR_RES {
    CExoString sString;                             // 0x00
    CResRef cSoundResRef;                           // 0x08
    float fSoundLength;                             // 0x18
};

class CTlkTable {
public:
    int Fetch(unsigned long nStrRef, STR_RES& strRes, unsigned char nGender); // 0x0041E550
};

extern CTlkTable* g_pTlkTable;                      // 0x007A3A08
extern unsigned char g_nGender;                     // 0x0073ECD0

struct CSSFHeader {
    char m_sFileType[4];
    char m_sVersion[4];
    unsigned long m_nTableOffset;                   // 0x08
};

class CSoundSet : public CResHelper<CResSSF, 2060> {
public:
    CSoundSet(const CResRef& cResRef);
    ~CSoundSet();
    int GetStrres(unsigned short nIndex, STR_RES& strRes);
};

class C2DA {
public:
    C2DA(CResRef cResRef, int bPrivate);            // 0x00413CC0
    virtual ~C2DA();
    int Load2DArray();                              // 0x004143B0
    int GetNumRows() { return m_nNumRows; }
    int GetINTEntry(int nRow, const CExoString& sColumn, int* pValue);      // 0x00413660
    int GetCExoStringEntry(int nRow, const CExoString& sColumn,
                           CExoString* pValue);     // 0x00413270

    char pad_0x4[0x20];
    int m_nNumRows;                                 // 0x24
    char pad_0x28[0x2c];
};

class CSoundSetList {
public:
    CSoundSetList();
    ~CSoundSetList();
    CResRef GetSoundSetResRef(unsigned short nId);

    unsigned short m_nNumSoundSets;                 // 0x00
    CResRef* m_pResRefs;                            // 0x04
    unsigned char* m_pGenders;                      // 0x08
    unsigned long* m_pStrRefs;                      // 0x0c
};

// FUNCTION: SWKOTOR 0x00678940 SYMBOL: ??1?$CResHelper@VCResSSF@@$0IAM@@@UAE@XZ
// FUNCTION: SWKOTOR 0x006789A0 SYMBOL: ?SetResRef@?$CResHelper@VCResSSF@@$0IAM@@@QAEXABVCResRef@@H@Z
// FUNCTION: SWKOTOR 0x00678B00 SYMBOL: ??_G?$CResHelper@VCResSSF@@$0IAM@@@UAEPAXI@Z
template class CResHelper<CResSSF, 2060>;

CResHelper<CResSSF, 2060>* NewResHelperSSF() { return new CResHelper<CResSSF, 2060>; }

// FUNCTION: SWKOTOR 0x00678820
int CSoundSet::GetStrres(unsigned short nIndex, STR_RES& strRes)
{
    CSSFHeader* pHeader = (CSSFHeader*)m_pRes->GetData();
    if (nIndex > 0 && nIndex < 29) {
        unsigned long nStrRef = ((unsigned long*)((char*)pHeader + pHeader->m_nTableOffset))[nIndex - 1];
        if (nStrRef != 0xffffffff) {
            g_pTlkTable->Fetch(nStrRef, strRes, g_nGender);
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00678880
CSoundSetList::~CSoundSetList()
{
    if (m_pResRefs) {
        delete[] m_pResRefs;
        m_pResRefs = 0;
    }
    if (m_pGenders) {
        delete[] m_pGenders;
        m_pGenders = 0;
    }
    if (m_pStrRefs) {
        delete[] m_pStrRefs;
        m_pStrRefs = 0;
    }
}

// FUNCTION: SWKOTOR 0x006788D0
CResRef CSoundSetList::GetSoundSetResRef(unsigned short nId)
{
    if (nId >= m_nNumSoundSets) {
        CResRef cEmpty;
        return cEmpty;
    }
    return m_pResRefs[nId];
}

// FUNCTION: SWKOTOR 0x00678B20
CSoundSet::CSoundSet(const CResRef& cResRef)
{
    SetResRef(cResRef, 0);
}

// FUNCTION: SWKOTOR 0x00678BA0
CSoundSet::~CSoundSet()
{
    if (m_pRes && m_pRes->IsLoaded()) {
        m_pRes->Release();
    }
}

// FUNCTION: SWKOTOR 0x00678EA0 SYMBOL: ??_GCSoundSet@@UAEPAXI@Z
CSoundSet* NewSoundSet(const CResRef& cResRef) { return new CSoundSet(cResRef); }

// FUNCTION: SWKOTOR 0x00678C10
CSoundSetList::CSoundSetList()
{
    m_pResRefs = 0;
    m_pGenders = 0;
    m_pStrRefs = 0;
    m_nNumSoundSets = 0;
    C2DA* pSoundSets = new C2DA("SoundSet", 0);
    if (pSoundSets == 0 || !pSoundSets->Load2DArray()) {
        return;
    }
    C2DA* pTypes = new C2DA("SoundSetType", 0);
    if (pTypes == 0) {
        delete pSoundSets;
        return;
    }
    if (!pTypes->Load2DArray()) {
        delete pSoundSets;
        return;
    }
    m_nNumSoundSets = pSoundSets->GetNumRows();
    m_pResRefs = new CResRef[m_nNumSoundSets];
    m_pGenders = new unsigned char[m_nNumSoundSets];
    m_pStrRefs = new unsigned long[m_nNumSoundSets];
    for (unsigned short i = 0; i < m_nNumSoundSets; i++) {
        CExoString sResRef;
        int nValue;
        pSoundSets->GetCExoStringEntry(i, "RESREF", &sResRef);
        m_pResRefs[i] = sResRef;
        pSoundSets->GetINTEntry(i, "STRREF", &nValue);
        m_pStrRefs[i] = nValue;
        pSoundSets->GetINTEntry(i, "GENDER", &nValue);
        m_pGenders[i] = (unsigned char)nValue;
    }
    delete pSoundSets;
    delete pTypes;
}
