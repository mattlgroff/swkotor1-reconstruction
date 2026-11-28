// devmodea.h -- DEVMODEA
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/c/client_exo_app_internal_s5.cpp); the other copies' members fall on its members.
#ifndef SHARED_DEVMODEA_H
#define SHARED_DEVMODEA_H


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

#endif
