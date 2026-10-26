// CResHelper<AreaGFF, 2012> (the area's .ARE resource holder at
// CSWSArea +0x100): destructor and SetResRef. Self-contained declarations
// in the shape of worker J's j/res.h / j/res_helper.h.

class CResRef {
public:
    CResRef();                                      // 0x00405ED0
    int operator==(const CResRef& cResRef) const;   // 0x004060B0
    int operator==(const char* szResRef) const;     // 0x004060D0
    int operator!=(const char* szResRef) const;     // 0x00406060
    CResRef operator=(const char* szResRef);        // 0x00406290
    CResRef operator=(const CResRef& cResRef);      // 0x00406120
    char m_resRef[16];
};

class CRes {
public:
    virtual ~CRes();
    int Request();                                  // 0x00408620
    int CancelRequest();                            // 0x00409B00
};

class CExoResMan {
public:
    void SetResObject(const CResRef& cResRef, unsigned short nType, CRes* pRes); // 0x00407680
    CRes* GetResObject(const CResRef& cResRef, unsigned short nType); // 0x004074D0
    int ReleaseResObject(CRes* pRes);               // 0x00409CF0
};
extern CExoResMan* g_pExoResMan;                    // 0x007A39E8

class CResGFF : public CRes {
public:
    CResGFF();                                      // 0x004105A0
    virtual ~CResGFF();
    char m_pad4[0xa0 - 4];
};

class AreaGFF : public CResGFF {
public:
    AreaGFF();                                      // 0x00504830
};

template <class T, int nType>
class CResHelper {
public:
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

// FUNCTION: SWKOTOR 0x00506BD0 SYMBOL: ??1?$CResHelper@VAreaGFF@@$0HNM@@@UAE@XZ
// FUNCTION: SWKOTOR 0x00506C30 SYMBOL: ?SetResRef@?$CResHelper@VAreaGFF@@$0HNM@@@QAEXABVCResRef@@H@Z
template class CResHelper<AreaGFF, 2012>;

template <class T>
class CExoArrayList {
public:
    void Allocate(int n)
    {
        T* pOld = element;
        array_size = n;
        element = new T[n];
        for (int i = 0; i < num; i++) {
            element[i] = pOld[i];
        }
        if (pOld) {
            delete[] pOld;
        }
    }
    void Add(T t)
    {
        if (num == array_size) {
            Allocate(array_size ? array_size * 2 : 16);
        }
        element[num++] = t;
    }

    T* element;
    int num;
    int array_size;
};

// FUNCTION: SWKOTOR 0x00506E20 SYMBOL: ?Allocate@?$CExoArrayList@VCResRef@@@@QAEXH@Z
template void CExoArrayList<CResRef>::Allocate(int n);

// FUNCTION: SWKOTOR 0x005089E0 SYMBOL: ?Add@?$CExoArrayList@VCResRef@@@@QAEXVCResRef@@@Z
template void CExoArrayList<CResRef>::Add(CResRef t);
