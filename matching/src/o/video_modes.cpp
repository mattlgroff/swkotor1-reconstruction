// Display-mode search helpers of the renderer, 0x0070D0A0-0x0070D2B9
// (descriptive names as in bucket c).
extern "C" void* __cdecl memset(void* p, int c, unsigned int n);
extern "C" void* __cdecl memcpy(void* d, const void* s, unsigned int n);
extern "C" int __cdecl abs(int n);
#pragma intrinsic(memset, memcpy, abs)

struct DEVMODEA {
    char dmDeviceName[32];                          // 0x00
    unsigned short dmSpecVersion;                   // 0x20
    unsigned short dmDriverVersion;                 // 0x22
    unsigned short dmSize;                          // 0x24
    unsigned short dmDriverExtra;                   // 0x26
    unsigned long dmFields;                         // 0x28
    char pad_0x2c[0x68 - 0x2c];
    unsigned long dmBitsPerPel;                     // 0x68
    unsigned long dmPelsWidth;                      // 0x6c
    unsigned long dmPelsHeight;                     // 0x70
    unsigned long dmDisplayFlags;                   // 0x74
    unsigned long dmDisplayFrequency;               // 0x78
    char pad_0x7c[0x9c - 0x7c];
};

extern "C" __declspec(dllimport) int __stdcall EnumDisplaySettingsA(const char* sDevice, unsigned long nMode,
                                                                    void* pDevMode);   // IAT 0x0073D3E4

// NONMATCH: the original keeps nBest in ESI (reassigned right after the
// rep movsd that borrows it); ours spills it and holds the refresh rate in EDI.
// FUNCTION: SWKOTOR 0x0070D0A0
unsigned long FindNearestVideoMode(unsigned long nWidth, unsigned long nHeight, unsigned char nDepth,
                                   unsigned long nRefresh)
{
    unsigned long nBest = 0xFFFFFFFF;
    unsigned long nMode = 0;
    DEVMODEA dmBest;
    DEVMODEA dm;
    memset(&dmBest, 0, sizeof(dmBest));
    memset(&dm, 0, sizeof(dm));
    dm.dmSize = sizeof(dm);
    while (EnumDisplaySettingsA(0, nMode, &dm)) {
        if (dm.dmPelsWidth >= 640 && dm.dmPelsHeight >= 480 && dm.dmBitsPerPel >= 16 &&
            (dm.dmDisplayFrequency >= 60 || dm.dmDisplayFrequency == 0 || dm.dmDisplayFrequency == 1)) {
            int nDiff = abs((int)(dm.dmBitsPerPel - nDepth));
            int nBestDiff = abs((int)(dmBest.dmBitsPerPel - nDepth));
            bool bCloser = false;
            if (nDiff < nBestDiff) {
                bCloser = true;
            } else if (nDiff == nBestDiff) {
                nDiff = abs((int)(dm.dmPelsWidth - nWidth));
                nBestDiff = abs((int)(dmBest.dmPelsWidth - nWidth));
                if (nDiff < nBestDiff) {
                    bCloser = true;
                } else if (nDiff == nBestDiff) {
                    nDiff = abs((int)(dm.dmPelsHeight - nHeight));
                    nBestDiff = abs((int)(dmBest.dmPelsHeight - nHeight));
                    if (nDiff < nBestDiff) {
                        bCloser = true;
                    } else if (nDiff == nBestDiff) {
                        nBestDiff = abs((int)(dmBest.dmDisplayFrequency - nRefresh));
                        nDiff = abs((int)(dm.dmDisplayFrequency - nRefresh));
                        if (nDiff < nBestDiff) {
                            bCloser = true;
                        }
                    }
                }
            }
            if (bCloser) {
                memcpy(&dmBest, &dm, sizeof(dm));
                nBest = nMode;
            }
        }
        memset(&dm, 0, sizeof(dm));
        dm.dmSize = sizeof(dm);
        nMode++;
    }
    return nBest;
}

// FUNCTION: SWKOTOR 0x0070D220
unsigned long FindVideoMode(unsigned long nWidth, unsigned long nHeight, unsigned char nDepth,
                            unsigned long nRefresh)
{
    DEVMODEA dm;
    unsigned long nMode = 0;
    memset(&dm, 0, sizeof(dm));
    dm.dmSize = sizeof(dm);
    while (EnumDisplaySettingsA(0, nMode, &dm)) {
        if (dm.dmPelsWidth == nWidth && dm.dmPelsHeight == nHeight && dm.dmBitsPerPel == nDepth) {
            unsigned long nFrequency = dm.dmDisplayFrequency;
            if (nFrequency == nRefresh || nFrequency == 0 || nFrequency == 1) {
                return nMode;
            }
        }
        memset(&dm, 0, sizeof(dm));
        dm.dmSize = sizeof(dm);
        nMode++;
    }
    return 0xFFFFFFFF;
}
