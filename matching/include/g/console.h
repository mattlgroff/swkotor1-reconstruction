// Aurora console function registry (funcholder / ConsoleFunc) and
// the AurRes resource reader stack, 0x0044ba00-0x0044c030 (behavioural
// reference: src/unknown/sub_0044ba_strings.cpp, sub_0044bd70.cpp,
// sub_0044bdc0.cpp, src/batches/b0043_aurora_members.cpp,
// b0044_gob_lookup.cpp).
#ifndef G_CONSOLE_H
#define G_CONSOLE_H

struct _iobuf;
typedef struct _iobuf FILE;

extern "C" {
int __cdecl strcmp(const char* a, const char* b);
unsigned int __cdecl fread(void* p, unsigned int size, unsigned int n, FILE* f);
char* __cdecl fgets(char* s, int n, FILE* f);
int __cdecl sscanf(const char* s, const char* fmt, ...);
void __cdecl free(void* p);
}
#pragma intrinsic(strcmp)

void* __cdecl operator new(unsigned int n);     // 0x006fa7e6
void __cdecl operator delete(void* p);          // 0x006fa390

extern char g_szConsoleResult[0x500];           // 0x007b8ab8

// Console command callback holder (base vtable 0x00741428).
class funcholder {
public:
    virtual ~funcholder() {}
    virtual void Call(char* szArgs) {}
    virtual void Query(char* szArgs) {}
};

// Typed holders (vtables 0x00741434 / 0x00741440 / 0x0074144c); the
// linker folded their identical ??_G, destructors and Query bodies.
class funcholder_void : public funcholder {
public:
    void (__cdecl* m_pfn)();                    // +0x04

    funcholder_void(void (__cdecl* pfn)()) { m_pfn = pfn; }
    virtual ~funcholder_void();
    virtual void Call(char* szArgs);
    virtual void Query(char* szArgs);
};

class funcholder_str : public funcholder {
public:
    void (__cdecl* m_pfn)(char*);               // +0x04

    funcholder_str(void (__cdecl* pfn)(char*)) { m_pfn = pfn; }
    virtual void Call(char* szArgs);
};

class funcholder_int : public funcholder {
public:
    void (__cdecl* m_pfn)(int);                 // +0x04

    funcholder_int(void (__cdecl* pfn)(int)) { m_pfn = pfn; }
    virtual void Call(char* szArgs);
};

// A named console command (0x54 bytes; the name is the first member).
class ConsoleFunc {
public:
    char m_szName[0x50];                        // +0x00
    funcholder* m_pHolder;                      // +0x50

    ConsoleFunc(char* szName, void (__cdecl* pfn)());
    ConsoleFunc(char* szName, void (__cdecl* pfn)(char*));
    ConsoleFunc(char* szName, void (__cdecl* pfn)(int));
    ~ConsoleFunc();
};

extern ConsoleFunc* g_apConsoleFuncs[0x4000];   // 0x007a8ab8
extern int g_nConsoleFuncs;                     // 0x007b8fb8

void __cdecl ConsoleAddFunc(ConsoleFunc* pFunc);        // 0x0044ba80
void __cdecl ConsoleRemoveFunc(char* szName);           // 0x0044bb10
funcholder* __cdecl ConsoleFindFunc(char* szName);      // 0x0044bb90
char* __cdecl ConsoleExecute(char* szLine);             // 0x0044bc70

// One open resource on the reader stack (file or in-memory).
class AurRes {
public:
    FILE* m_pFile;          // +0x00 (0 = in-memory data)
    int m_nLine;            // +0x04
    char* m_pBuffer;        // +0x08
    char* m_pData;          // +0x0c
    int m_nRemaining;       // +0x10
    int m_nBufferSize;      // +0x14
    void* m_pHandle;        // +0x18 (loader handle of in-memory data)

    AurRes(int nBufferSize)
    {
        m_pFile = 0;
        m_nLine = -1;
        m_nBufferSize = nBufferSize;
        m_pBuffer = 0;
        m_pData = 0;
        m_nRemaining = 0;
        m_pHandle = 0;
    }
};

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    void Allocate(int n);                       // 0x005e03c0 (AurRes*)
    void Remove(T t);                           // 0x00445080
    void Add(T t)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 8);
        element[num++] = t;
    }
    int Contains(T t)
    {
        int n = 0;
        for (int i = 0; i < num; i++)
            if (element[i] == t)
                n++;
        return n;
    }
};

extern CExoArrayList<AurRes*> g_lAurResStack;   // 0x007b8fd8

char* __cdecl AurResGetDataBytes(int nSize, AurRes* pRes);  // 0x0044bd10
void __cdecl AurResFreeDataBytes(AurRes* pRes, char* pData); // 0x0044bd50
AurRes* __cdecl AurResFind(AurRes* pRes);                   // 0x0044bd70
void __cdecl AurResRemove(AurRes* pRes);                    // 0x0044bdc0
char* __cdecl AurResReadFile(int nSize);                    // 0x0044bee0
char* __cdecl AurResReadData(int nSize);                    // 0x0044bf30
char* __cdecl AurResGetData(int nSize);                     // 0x0044bfd0

#endif
