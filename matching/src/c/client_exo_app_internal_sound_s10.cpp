// CClientExoAppInternal::InitializeSoundOptions (clientexoappinternal.o, Mac
// name), worker C s10: reads the [Sound Options] ini entries, creates the
// sound system and writes the effective values back. Self-contained.

void* __cdecl operator new(unsigned int n);         // 0x006FA7E6

class CExoString {
public:
    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04

    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    ~CExoString();                                  // 0x005E5C20
    int AsINT() const;                              // 0x005E5630
    float AsFLOAT() const;                          // 0x005E5650
    void Format(const char* format, ...);           // 0x005E5680
};

class CExoIni {
public:
    CExoIni();                                      // 0x005E6750
    ~CExoIni();                                     // 0x005E67E0
    int ReadIniEntry(CExoString& sValue, const CExoString& sFile, const CExoString& sSection,
                     const CExoString& sKey);       // 0x005E67C0
    int WriteIniEntry(const CExoString& sValue, const CExoString& sFile, const CExoString& sSection,
                      const CExoString& sKey);      // 0x005E67D0

    void* m_pcExoIniInternal;                       // 0x00
};

// The first two constructor arguments are pushed as raw byte variables, so
// they are unsigned char here; bucket j's matched constructor declares four
// unsigned longs (call targets are masked either way).
class CExoSound {
public:
    CExoSound(unsigned char n2DVoices, unsigned char n3DVoices, int nEAX, int bHardware);   // 0x005D5BC0
    void SetProperty0x00(unsigned long n);          // 0x005D6430
    void SetProperty0x04(unsigned long n);          // 0x005D6440
    void SetScale60(float f);                       // 0x005D6320
    float GetScale60();                             // 0x005D6380
    unsigned char GetNumber3DVoices();              // 0x005D63A0
    unsigned char GetNumber2DVoices();              // 0x005D63B0
    unsigned long GetProperty0x100();               // 0x005D6450

    void* m_pcExoSoundInternal;                     // 0x00
};

extern CExoSound* g_pExoSound;                      // 0x007A39EC

// Sound-related members of CClientOptions (0x49 is written from the sound
// system's reported EAX level; client_options_r10.cpp calls it m_nSoundQuality).
class CClientOptions {
public:
    char pad_0x0[0x49];
    unsigned char m_nEAX;                           // 0x49
    char pad_0x4a[0x2];
    int m_bForceSoftware;                           // 0x4c
};

class CClientExoApp {
public:
    CClientOptions* GetClientOptions();        // 0x005ED700
    void SetEAXLevel(unsigned char n) { GetClientOptions()->m_nEAX = n; }   // inline, descriptive
};

class CAppManager {
public:
    void* m_pUnknown00;
    CClientExoApp* m_pClientExoApp;                 // 0x04

    CClientExoApp* GetClientExoApp() { return m_pClientExoApp; }
};

extern CAppManager* g_pAppManager;                  // 0x007A39FC

class CClientExoAppInternal {
public:
    void InitializeSoundOptions();
};

// FUNCTION: SWKOTOR 0x005F1F70
void CClientExoAppInternal::InitializeSoundOptions()
{
    CExoIni cIni;
    CExoString sUnused;
    CExoString sValue;
    int nEAX = 3;
    int nForceSoftware = 0;
    int nSoundInit = 0;

    if (cIni.ReadIniEntry(sValue, ".\\swkotor.ini", "Sound Options", "Sound Init")) {
        nSoundInit = sValue.AsINT();
    }
    sValue.Format("%d", 1);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Sound Options", "Sound Init");
    unsigned char n2DVoices = 24;
    unsigned char n3DVoices = 16;
    if (cIni.ReadIniEntry(sValue, ".\\swkotor.ini", "Sound Options", "Number 2D Voices")) {
        n2DVoices = sValue.AsINT();
    }
    if (cIni.ReadIniEntry(sValue, ".\\swkotor.ini", "Sound Options", "Number 3D Voices")) {
        n3DVoices = sValue.AsINT();
    }
    if (nSoundInit) {
        nEAX = 0;
        nForceSoftware = 1;
    } else {
        if (cIni.ReadIniEntry(sValue, ".\\swkotor.ini", "Sound Options", "EAX")) {
            nEAX = sValue.AsINT();
        }
        if (cIni.ReadIniEntry(sValue, ".\\swkotor.ini", "Sound Options", "Force Software")) {
            nForceSoftware = sValue.AsINT();
            g_pAppManager->m_pClientExoApp->GetClientOptions()->m_bForceSoftware = nForceSoftware;
        }
    }
    g_pExoSound = new CExoSound(n2DVoices, n3DVoices, nEAX, nForceSoftware == 0);
    if (cIni.ReadIniEntry(sValue, ".\\swkotor.ini", "Sound Options", "Environment Effects Nonstreaming")) {
        g_pExoSound->SetProperty0x04(sValue.AsINT());
    }
    if (cIni.ReadIniEntry(sValue, ".\\swkotor.ini", "Sound Options", "Environment Effects Streaming")) {
        g_pExoSound->SetProperty0x00(sValue.AsINT());
    }
    if (cIni.ReadIniEntry(sValue, ".\\swkotor.ini", "Sound Options", "2D3D Bias")) {
        g_pExoSound->SetScale60(sValue.AsFLOAT());
    }
    sValue.Format("%d", g_pExoSound->GetProperty0x100());
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Sound Options", "EAX");
    g_pAppManager->GetClientExoApp()->SetEAXLevel(g_pExoSound->GetProperty0x100());
    sValue.Format("%d", nForceSoftware);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Sound Options", "Force Software");
    sValue.Format("%.2f", g_pExoSound->GetScale60());
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Sound Options", "2D3D Bias");
    sValue.Format("%d", g_pExoSound->GetNumber3DVoices());
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Sound Options", "Number 2D Voices");
    sValue.Format("%d", g_pExoSound->GetNumber2DVoices());
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Sound Options", "Number 3D Voices");
    sValue.Format("%d", 0);
    cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Sound Options", "Sound Init");
}
