#include "c/client_exo_app.h"

class CExoIni {
public:
    CExoIni();                                      // 0x005E6750
    ~CExoIni();                                     // 0x005E67E0
    int ReadIniEntry(CExoString& sValue, const CExoString& sFile, const CExoString& sSection,
                     const CExoString& sKey);       // 0x005E67C0

    void* m_pcExoIniInternal;                       // 0x00
};

extern unsigned long g_nScreenRefreshRate;          // 0x0078D1DC

// FUNCTION: SWKOTOR 0x005F0C60
int CClientExoAppInternal::IsValidResolution(int width, int height)
{
    if (width == 0x320) {
        if (height == 0x258) return 1;
    }
    if (width == 0x400) {
        if (height == 0x300) return 1;
    }
    if (width == 0x500) {
        if (height == 0x3c0 || height == 0x400) return 1;
    }
    if (width == 0x640) {
        if (height == 0x4b0) return 1;
    }
    return 0;
}

// Kept in IsValidResolution's translation unit: the original relies on that
// callee leaving ecx intact.
// FUNCTION: SWKOTOR 0x005F0CE0
int CClientExoAppInternal::ReadVideoModeSettings(unsigned long* pWidth, unsigned long* pHeight,
                                                 unsigned char* pDepth, unsigned long* pRefresh)
{
    CExoString sValue;
    CExoIni cIni;
    int bFound = cIni.ReadIniEntry(sValue, ".\\swkotor.ini", "Graphics Options", "Width");
    *pWidth = sValue.AsINT();
    cIni.ReadIniEntry(sValue, ".\\swkotor.ini", "Graphics Options", "Height");
    *pHeight = sValue.AsINT();
    *pDepth = 32;
    if (cIni.ReadIniEntry(sValue, ".\\swkotor.ini", "Graphics Options", "AllowHighMonitorFrequency")) {
        m_bAllowHighMonitorFrequency = sValue.AsINT() != 0;
    } else {
        m_bAllowHighMonitorFrequency = 0;
    }
    if (cIni.ReadIniEntry(sValue, ".\\swkotor.ini", "Graphics Options", "RefreshRate")) {
        g_nScreenRefreshRate = sValue.AsINT();
        *pRefresh = g_nScreenRefreshRate;
    } else {
        *pRefresh = g_nScreenRefreshRate;
    }
    if (!IsValidResolution(*pWidth, *pHeight)) {
        *pWidth = 800;
        *pHeight = 600;
    }
    if (*pDepth != 32) {
        *pDepth = 32;
    }
    if (*pRefresh < 60) {
        *pRefresh = 60;
    }
    if (!m_bAllowHighMonitorFrequency && *pRefresh > 85) {
        *pRefresh = 60;
    }
    return bFound;
}
