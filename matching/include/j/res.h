// Worker J matching headers: CRes subclasses (CResWave, CResLYT) and CLYT.
#ifndef J_RES_H
#define J_RES_H

#include "j/exo_string.h"

class CResRef {
public:
    CResRef();                                      // 0x00405ED0
    CResRef(const char* szResRef);                  // 0x00406D80
    CResRef(const CExoString& sResRef);             // 0x00406D60
    void CopyToString(CExoString& s) const;         // 0x00405F70
    char* GetResRefStr() const;                     // 0x00405FE0
    int operator==(const CResRef& cResRef) const;   // 0x004060B0
    int operator==(const char* szResRef) const;     // 0x004060D0
    int operator!=(const char* szResRef) const;     // 0x00406060
    int operator!=(const CResRef& cResRef) const;   // 0x00406DC0
    CResRef operator=(const char* szResRef);        // 0x00406290
    CResRef operator=(const CResRef& cResRef);      // 0x00406120
    char m_resRef[16];
};

class CRes {
public:
    CRes();                                         // 0x00406460
    virtual ~CRes();                                // 0x00409D40
    virtual int OnResourceFreed();

    int IsLoaded();                                 // 0x004064B0
    int Request();                                  // 0x00408620
    void* Demand();                                 // 0x00409B10
    int Release();                                  // 0x00409B80
    int CancelRequest();                            // 0x00409B00

    char pad_0x4[0xc];
    void* m_pResource;                              // 0x10
    char pad_0x14[0x14];
};

class CExoResMan {
public:
    void SetResObject(const CResRef& cResRef, unsigned short nType, CRes* pRes); // 0x00407680
    CRes* GetResObject(const CResRef& cResRef, unsigned short nType); // 0x004074D0
    unsigned long ReleaseResObject(CRes* pRes);     // 0x00409CF0
};

extern CExoResMan* g_pExoResMan;                    // 0x007A39E8

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

class CResWave : public CRes {
public:
    CResWave();
    virtual ~CResWave();
    virtual int OnResourceFreed();
    virtual void VirtualSlot08();
    virtual void VirtualSlot0C();
    virtual void VirtualSlot10();
    virtual void* GetWaveData();                    // vftable +0x14

    int m_n28;                                      // 0x28
    int m_n2C;                                      // 0x2c
    int m_n30;                                      // 0x30
    int m_n34;                                      // 0x34
    int m_n38;                                      // 0x38
};

class CResNCS : public CRes {
public:
    CResNCS();                                      // 0x005D4C30
    virtual ~CResNCS();                             // 0x005D4C50
    char* GetNCSDataPtr();                          // 0x00404DD0
    int GetNCSSize();                               // 0x00710E50

    int m_bLoaded;                                  // 0x28
    int m_nNCSSize;                                 // 0x2c
    char* m_pNCS;                                   // 0x30
};

class CResNSS : public CRes {
public:
    CResNSS();
    virtual ~CResNSS();

    int m_n28;                                      // 0x28
    int m_n2C;                                      // 0x2c
    int m_n30;                                      // 0x30
};

class CResLYT : public CRes {
public:
    CResLYT();
    virtual ~CResLYT();

    int m_n28;                                      // 0x28
    int m_n2C;                                      // 0x2c
    int m_n30;                                      // 0x30
};

// j/lyt.h has the full CLYT (CResHelper base) for lyt.cpp.
#ifndef J_LYT_H
class CLYT {
public:
    CLYT() {}
    virtual ~CLYT();                                // 0x005DE7E0
    int LoadLayout(CExoString& sName);              // 0x005DE900
    int LoadLayout(const CResRef& cResRef);         // 0x005DF140

    char pad_0x4[0x4];
    CRes* m_pRes;                                   // 0x08
    char pad_0xc[0x14];
    int m_bLoaded;                                  // 0x20
};
#endif

#endif
