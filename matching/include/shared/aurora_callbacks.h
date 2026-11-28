// aurora_callbacks.h -- the renderer's resource and raster callbacks (CODE_RULES.md, R2)
//
// The CAuroraInterface constructor (o/aurora_texture_packs.cpp) points each
// at its CAuroraInterface function; the renderer calls through them.
#ifndef SHARED_AURORA_CALLBACKS_H
#define SHARED_AURORA_CALLBACKS_H

extern unsigned char* (*g_pfnAurResGet)(const char*, const char*, int*); // 0x007B8FC4
extern void (*g_pfnAurResFree)(void*, int);                              // 0x007B8FC8
extern unsigned char* (*g_pfnGetRaster)(char*, unsigned long*, unsigned long*, unsigned long*); // 0x0078D404
extern int (*g_pfnReadRasterHeader)(char*, unsigned long*, unsigned long*, unsigned long*); // 0x0078D408
extern unsigned char* (*g_pfnGetCompressedRaster)(char*, unsigned long*, unsigned long*, unsigned long*,
                                                  unsigned long*, float*); // 0x0078D40C
extern int (*g_pfnReadCompressedRasterHeader)(char*, unsigned long*, unsigned long*, unsigned long*,
                                              unsigned long*, float*); // 0x0078D410
extern unsigned char* (*g_pfnGet16BitRaster)(char*, unsigned long*, unsigned long*, float*, int*); // 0x0078D414
extern int (*g_pfnGet16BitRasterHeader)(char*, unsigned long*, unsigned long*, float*, int*); // 0x0078D418
extern unsigned char* (*g_pfnGetLayeredRaster)(char*, unsigned long*, unsigned long*, unsigned long*); // 0x0078D41C
extern unsigned char* (*g_pfnGetProcessedRaster)(char*, int*, int*, char*, int*, int*, int*, float*,
                                                 char*); // 0x0078D420
extern int (*g_pfnReadProcessedRasterHeader)(char*, int*, int*, char*, int*, int*, float*, char*); // 0x0078D424
extern void (*g_pfnReleaseRaster)(void*, int, int, int, int, int, int, unsigned char); // 0x007A46F8
extern void (*g_pfnReleaseLayeredRaster)(void*, int, int);  // 0x007A46FC

#endif
