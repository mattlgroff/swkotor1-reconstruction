// render buffer pools (RequestPoolManager and its system-memory
// and vertex-buffer-object implementations), 0x0042e240-0x00436470 plus the
// Sys helpers at 0x00423a90-0x00423b50 (g/glrender.cpp).
#ifndef G_REQUEST_POOL_H
#define G_REQUEST_POOL_H

void __cdecl operator delete[](void* p) throw();

#include "g/exo_list.h"

// GL_ARB_vertex_buffer_object entry points (function-pointer globals).
typedef void(__stdcall* PFNGLBINDBUFFERARB)(unsigned int target, unsigned int buffer);
typedef void(__stdcall* PFNGLDELETEBUFFERSARB)(int n, const unsigned int* buffers);
typedef void(__stdcall* PFNGLGENBUFFERSARB)(int n, unsigned int* buffers);
typedef void(__stdcall* PFNGLBUFFERDATAARB)(unsigned int target, int size, const void* data, unsigned int usage);
typedef void(__stdcall* PFNGLBUFFERSUBDATAARB)(unsigned int target, int offset, int size, const void* data);
typedef void*(__stdcall* PFNGLMAPBUFFERARB)(unsigned int target, unsigned int access);
typedef unsigned char(__stdcall* PFNGLUNMAPBUFFERARB)(unsigned int target);
typedef void(__stdcall* PFNGLGETBUFFERPARAMETERIVARB)(unsigned int target, unsigned int pname, int* params);
extern PFNGLBINDBUFFERARB glBindBufferARB;                       // 0x007bb7b4
extern PFNGLDELETEBUFFERSARB glDeleteBuffersARB;                 // 0x007bb7bc
extern PFNGLGENBUFFERSARB glGenBuffersARB;                       // 0x007bb5b4
extern PFNGLBUFFERDATAARB glBufferDataARB;                       // 0x007bb544
extern PFNGLBUFFERSUBDATAARB glBufferSubDataARB;                 // 0x007bb75c
extern PFNGLMAPBUFFERARB glMapBufferARB;                         // 0x007bb634
extern PFNGLUNMAPBUFFERARB glUnmapBufferARB;                     // 0x007bb6e0
extern PFNGLGETBUFFERPARAMETERIVARB glGetBufferParameterivARB;   // 0x007bb5a4

#define GL_ARRAY_BUFFER_ARB 0x8892
#define GL_ELEMENT_ARRAY_BUFFER_ARB 0x8893
#define GL_BUFFER_SIZE_ARB 0x8764
#define GL_READ_ONLY_ARB 0x88b8
#define GL_WRITE_ONLY_ARB 0x88b9
#define GL_READ_WRITE_ARB 0x88ba
#define GL_DYNAMIC_DRAW_ARB 0x88e8

extern "C" {
void* __cdecl memset(void* d, int c, unsigned int n);
void* __cdecl memcpy(void* d, const void* s, unsigned int n);
void __cdecl free(void* p);
}
#pragma intrinsic(memset, memcpy)

void* __cdecl operator new(unsigned int n);

// vtable 0x0073f488 (all slots but the destructor are _purecall).
class RequestPoolManager {
public:
    RequestPoolManager() {}
    virtual ~RequestPoolManager();                             // 0x00
    virtual int Request(int nType, int nSize, int nFlags) = 0; // 0x04
    virtual void* Lock(int nId, int nFlags) = 0;               // 0x08
    virtual void Unlock(int nId) = 0;                          // 0x0c
    virtual int GetBuffer(int nId) = 0;                        // 0x10
    virtual int GetIndices(int nId) = 0;                       // 0x14
    virtual void Release(int nId) = 0;                         // 0x18
    virtual void ResizePool(int nId, int nSize) = 0;           // 0x1c
    virtual int GetPoolSize(int nId) = 0;                      // 0x20
    virtual void Slot9(int a, int b) = 0;                      // 0x24
    virtual void LoseBuffers() = 0;                            // 0x28
    virtual void RestoreBuffers() = 0;                         // 0x2c

    CExoArrayList<int> m_aFree;      // +0x04
    CExoArrayList<int> m_aLocked;    // +0x10
    unsigned int m_nCount;           // +0x1c
};

// vtable 0x0073f4b8, 0x28 bytes.
class RequestPoolManagerSys : public RequestPoolManager {
public:
    RequestPoolManagerSys();
    virtual ~RequestPoolManagerSys();
    virtual int Request(int nType, int nSize, int nFlags);
    virtual void* Lock(int nId, int nFlags);
    virtual void Unlock(int nId);
    virtual int GetBuffer(int nId);
    virtual int GetIndices(int nId);
    virtual void Release(int nId);
    virtual void ResizePool(int nId, int nSize);
    virtual int GetPoolSize(int nId);
    virtual void Slot9(int a, int b);
    virtual void LoseBuffers();
    virtual void RestoreBuffers();

    void** m_ppPools;    // +0x20
    int* m_pSizes;       // +0x24
};

// vtable 0x0073f4fc, 0x5c bytes.
class RequestPoolManagerVBO : public RequestPoolManager {
public:
    RequestPoolManagerVBO();
    virtual ~RequestPoolManagerVBO();
    virtual int Request(int nType, int nSize, int nFlags);
    virtual void* Lock(int nId, int nFlags);
    virtual void Unlock(int nId);
    virtual int GetBuffer(int nId);
    virtual int GetIndices(int nId);
    virtual void Release(int nId);
    virtual void ResizePool(int nId, int nSize);
    virtual int GetPoolSize(int nId);
    virtual void Slot9(int a, int b);
    virtual void LoseBuffers();
    virtual void RestoreBuffers();

    CExoArrayList<int> m_aFlags;         // +0x20
    CExoArrayList<unsigned int> m_aNames; // +0x2c
    CExoArrayList<void*> m_aSaved;       // +0x38
    CExoArrayList<int> m_aSavedSizes;    // +0x44
    CExoArrayList<unsigned int> m_aTargets; // +0x50
};

extern RequestPoolManager* g_pRequestPoolManager;  // 0x007a68a4
extern int g_nPoolBytes;                           // 0x007a68a8
extern int g_nPoolPages;                           // 0x007a68ac
extern unsigned char g_bVBOWriteOnly;              // 0x0078e398

#endif
