// CClientExoAppInternal::InitializeClientOptions, worker C s8
// (clientexoappinternal.o; translation-unit-local layout).

#include "c/exo_base.h"

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

class CClientOptions {
public:
    void SetMouseLook(int bEnabled);                // 0x0061D670 (hardware mouse)
};

class CClientExoAppInternal {
public:
    void InitializeClientOptions();

    void* m_pUnknown00;                             // 0x00
    CClientOptions* m_pClientOptions;               // 0x04
};

// FUNCTION: SWKOTOR 0x005F1C20
void CClientExoAppInternal::InitializeClientOptions()
{
    CExoIni cIni;
    CExoString sValue;
    if (cIni.ReadIniEntry(sValue, ".\\swkotor.ini", "Game Options", "TooltipDelay Sec")) {
        if (sValue.AsINT() <= 0) {
            sValue.Format("1");
            cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Game Options", "TooltipDelay Sec");
        }
    } else {
        cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Game Options", "TooltipDelay Sec");
    }
    if (cIni.ReadIniEntry(sValue, ".\\swkotor.ini", "Graphics Options", "EnableHardwareMouse")) {
        CClientOptions* pOptions = m_pClientOptions;
        pOptions->SetMouseLook(sValue.AsINT() != 0);
    } else {
        m_pClientOptions->SetMouseLook(1);
        sValue.Format("%d", 1);
        cIni.WriteIniEntry(sValue, ".\\swkotor.ini", "Graphics Options", "EnableHardwareMouse");
    }
}
