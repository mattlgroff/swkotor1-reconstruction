// Exo facade forwarders (`mov ecx, [ecx + k]; jmp Internal::X`) left over from
// b005e3/b005e4/b005e7: CExoBase, CExoIni, CExoAliasList, CExoFile,
// CExoTimers, CExoCriticalSection, CExoLocString, CExoRand, CClientExoApp.
// The internal targets are only declared, so each forwarder is a tail jmp;
// argument counts follow the targets' RET n.

class CExoString;
class CExoLocString;

class CExoBaseInternal {
public:
    int GetDirectoryList(void* a, void* b, void* c, int d, int e);  // 0x005E8CF0 RET 0x14
    int GetDirectorySize(void* a);                  // 0x005E7820 RET 4
    int GetResourceExtension(int a);                // 0x005E7A00 RET 4
    int GetResTypeFromExtension(void* a);           // 0x005E7A40 RET 4
    int LoadAliases(void* a);                       // 0x005E7A90 RET 4
    int WcsToMbs(void* a, void* b, int c);          // 0x005E8B20 RET 0xc
};

class CExoBase {
public:
    void* m_pcExoIni;                               // 0x00
    void* m_pcExoTimers;                            // 0x04
    void* m_pcExoDebug;                             // 0x08
    void* m_pcExoAliasList;                         // 0x0c
    void* m_pcExoRand;                              // 0x10
    CExoBaseInternal* m_pcExoBaseInternal;          // 0x14

    int GetDirectoryList(void* a, void* b, void* c, int d, int e);
    int GetDirectorySize(void* a);
    int GetResourceExtension(int a);
    int GetResTypeFromExtension(void* a);
    int LoadAliases(void* a);
    int WcsToMbs(void* a, void* b, int c);
};

// FUNCTION: SWKOTOR 0x005E6640
int CExoBase::GetDirectoryList(void* a, void* b, void* c, int d, int e)
{
    return m_pcExoBaseInternal->GetDirectoryList(a, b, c, d, e);
}

// FUNCTION: SWKOTOR 0x005E6650
int CExoBase::GetDirectorySize(void* a)
{
    return m_pcExoBaseInternal->GetDirectorySize(a);
}

// FUNCTION: SWKOTOR 0x005E6660
int CExoBase::GetResourceExtension(int a)
{
    return m_pcExoBaseInternal->GetResourceExtension(a);
}

// FUNCTION: SWKOTOR 0x005E6670
int CExoBase::GetResTypeFromExtension(void* a)
{
    return m_pcExoBaseInternal->GetResTypeFromExtension(a);
}

// FUNCTION: SWKOTOR 0x005E6680
int CExoBase::LoadAliases(void* a)
{
    return m_pcExoBaseInternal->LoadAliases(a);
}

// FUNCTION: SWKOTOR 0x005E6690
int CExoBase::WcsToMbs(void* a, void* b, int c)
{
    return m_pcExoBaseInternal->WcsToMbs(a, b, c);
}

class CExoIniInternal {
public:
    int ReadIniEntry(void* a, void* b, void* c, void* d);     // 0x005EA6C0 RET 0x10
    int WriteIniEntry(void* a, void* b, void* c, void* d);    // 0x005EAB00 RET 0x10
};

class CExoIni {
public:
    CExoIniInternal* m_pcExoIniInternal;            // 0x00

    int ReadIniEntry(void* a, void* b, void* c, void* d);
    int WriteIniEntry(void* a, void* b, void* c, void* d);
};

// FUNCTION: SWKOTOR 0x005E67C0
int CExoIni::ReadIniEntry(void* a, void* b, void* c, void* d)
{
    return m_pcExoIniInternal->ReadIniEntry(a, b, c, d);
}

// FUNCTION: SWKOTOR 0x005E67D0
int CExoIni::WriteIniEntry(void* a, void* b, void* c, void* d)
{
    return m_pcExoIniInternal->WriteIniEntry(a, b, c, d);
}

class CExoAliasListInternal {
public:
    int Add(void* a, void* b);                      // 0x005EBF30 RET 8
    int GetAliasPath(void* a, int b);               // 0x005EB650 RET 8
};

class CExoAliasList {
public:
    CExoAliasListInternal* m_pcExoAliasListInternal;    // 0x00

    int Add(void* a, void* b);
    int GetAliasPath(void* a, int b);
};

// FUNCTION: SWKOTOR 0x005E6880
int CExoAliasList::Add(void* a, void* b)
{
    return m_pcExoAliasListInternal->Add(a, b);
}

// FUNCTION: SWKOTOR 0x005E6890
int CExoAliasList::GetAliasPath(void* a, int b)
{
    return m_pcExoAliasListInternal->GetAliasPath(a, b);
}

class CExoFileInternal {
public:
    unsigned long GetOffset();                      // 0x005EB970
    unsigned long GetSize();                        // 0x005EB990
    unsigned long Read(void* p, unsigned long n, unsigned long c);   // 0x005EBA40 RET 0xc
    unsigned long Read(CExoString& s, unsigned long n);  // 0x005EB9F0 RET 8
    int ReadAsync(void* p, unsigned long n, unsigned long c);   // 0x005EBBF0 RET 0xc
    int ReadAsyncComplete();                        // 0x005EBC50
    unsigned long Write(const void* p, unsigned long n, unsigned long c);   // 0x005EBC60 RET 0xc
};

class CExoFile {
public:
    CExoFileInternal* m_pcExoFileInternal;          // 0x00

    unsigned long GetOffset();
    unsigned long GetSize();
    unsigned long Read(void* p, unsigned long n, unsigned long c);
    unsigned long Read(CExoString& s, unsigned long n);
    int ReadAsync(void* p, unsigned long n, unsigned long c);
    int ReadAsyncComplete();
    unsigned long Write(const void* p, unsigned long n, unsigned long c);
};

// FUNCTION: SWKOTOR 0x005E6940
unsigned long CExoFile::GetOffset()
{
    return m_pcExoFileInternal->GetOffset();
}

// FUNCTION: SWKOTOR 0x005E6950
unsigned long CExoFile::GetSize()
{
    return m_pcExoFileInternal->GetSize();
}

// FUNCTION: SWKOTOR 0x005E6960 SYMBOL: ?Read@CExoFile@@QAEKPAXKK@Z
unsigned long CExoFile::Read(void* p, unsigned long n, unsigned long c)
{
    return m_pcExoFileInternal->Read(p, n, c);
}

// FUNCTION: SWKOTOR 0x005E6970 SYMBOL: ?Read@CExoFile@@QAEKAAVCExoString@@K@Z
unsigned long CExoFile::Read(CExoString& s, unsigned long n)
{
    return m_pcExoFileInternal->Read(s, n);
}

// FUNCTION: SWKOTOR 0x005E6980
int CExoFile::ReadAsync(void* p, unsigned long n, unsigned long c)
{
    return m_pcExoFileInternal->ReadAsync(p, n, c);
}

// FUNCTION: SWKOTOR 0x005E6990
int CExoFile::ReadAsyncComplete()
{
    return m_pcExoFileInternal->ReadAsyncComplete();
}

// FUNCTION: SWKOTOR 0x005E69A0 SYMBOL: ?Write@CExoFile@@QAEKPBXKK@Z
unsigned long CExoFile::Write(const void* p, unsigned long n, unsigned long c)
{
    return m_pcExoFileInternal->Write(p, n, c);
}

class CExoTimersInternal {
public:
    unsigned long GetLowResolutionTimer();          // 0x005EC200
    unsigned long GetHighResolutionTimer();         // 0x005EC210
};

class CExoTimers {
public:
    CExoTimersInternal* m_pcExoTimersInternal;      // 0x00

    unsigned long GetLowResolutionTimer();
    unsigned long GetHighResolutionTimer();
};

// FUNCTION: SWKOTOR 0x005E9250
unsigned long CExoTimers::GetLowResolutionTimer()
{
    return m_pcExoTimersInternal->GetLowResolutionTimer();
}

// FUNCTION: SWKOTOR 0x005E9260
unsigned long CExoTimers::GetHighResolutionTimer()
{
    return m_pcExoTimersInternal->GetHighResolutionTimer();
}

class CExoCriticalSectionInternal {
public:
    void EnterCriticalSection();                    // 0x005EC270
    void LeaveCriticalSection();                    // 0x005EC280
};

class CExoCriticalSection {
public:
    CExoCriticalSectionInternal* m_pcExoCriticalSectionInternal;   // 0x00

    void EnterCriticalSection();
    void LeaveCriticalSection();
};

// FUNCTION: SWKOTOR 0x005E9300
void CExoCriticalSection::EnterCriticalSection()
{
    m_pcExoCriticalSectionInternal->EnterCriticalSection();
}

// FUNCTION: SWKOTOR 0x005E9310
void CExoCriticalSection::LeaveCriticalSection()
{
    m_pcExoCriticalSectionInternal->LeaveCriticalSection();
}

class CExoLocStringInternal {
public:
    void ClearLocString();                          // 0x005ECB10
    unsigned long GetStringLength(int nLanguage);   // 0x005ECA20 RET 4
};

class CExoLocString {
public:
    CExoLocStringInternal* m_pcExoLocStringInternal;   // 0x00
    unsigned long m_dwStringRef;                    // 0x04

    void ClearLocString();
    unsigned long GetStringLength(int nLanguage);
    int Compare(const CExoLocString& o);            // 0x005E9F10 RET 4
    int operator==(const CExoLocString& o);
    int operator!=(const CExoLocString& o);
};

// FUNCTION: SWKOTOR 0x005E9EA0
void CExoLocString::ClearLocString()
{
    m_pcExoLocStringInternal->ClearLocString();
}

// FUNCTION: SWKOTOR 0x005E9F00
unsigned long CExoLocString::GetStringLength(int nLanguage)
{
    return m_pcExoLocStringInternal->GetStringLength(nLanguage);
}

// FUNCTION: SWKOTOR 0x005EA020
int CExoLocString::operator==(const CExoLocString& o)
{
    return Compare(o);
}

// FUNCTION: SWKOTOR 0x005EA030
int CExoLocString::operator!=(const CExoLocString& o)
{
    return !Compare(o);
}

class CExoRandInternal {
public:
    unsigned long Rand();                           // 0x005ECCA0
};

class CExoRand {
public:
    CExoRandInternal* m_pcExoRandInternal;          // 0x00

    unsigned long Rand();
};

// FUNCTION: SWKOTOR 0x005EA490
unsigned long CExoRand::Rand()
{
    return m_pcExoRandInternal->Rand();
}

class CClientExoAppInternal {
public:
    int SetPausedByCombat(int a, int b, int c);     // 0x005F2E10 RET 0xc
};

class CClientExoApp {
public:
    void* vtable;                                   // 0x00
    CClientExoAppInternal* m_pcInternal;            // 0x04

    int SetPausedByCombat(int a, int b, int c);
};

// FUNCTION: SWKOTOR 0x005EDC20
int CClientExoApp::SetPausedByCombat(int a, int b, int c)
{
    return m_pcInternal->SetPausedByCombat(a, b, c);
}
