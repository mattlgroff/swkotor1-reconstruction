// CAurTexture / CAurTextureBasic, SafePointer and the Aur texture
// globals (0x00420440-0x004235f0; behavioural reference:
// src/batches/b0041_aur_texture.cpp, include/kotor/b0041_aur_texture.h).
#ifndef G_AUR_TEXTURE_H
#define G_AUR_TEXTURE_H

extern "C" {
__declspec(dllimport) void __stdcall glEnable(unsigned int cap);
__declspec(dllimport) void __stdcall glDisable(unsigned int cap);
__declspec(dllimport) void __stdcall glBindTexture(unsigned int target, unsigned int tex);
__declspec(dllimport) void __stdcall glTexParameteri(unsigned int target, unsigned int pname, int param);
__declspec(dllimport) void __stdcall glDeleteTextures(int n, const unsigned int* tex);
}

#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84fe

// --- CExoArrayList (element, num, array_size) -------------------------------
template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList() { num = 0; array_size = 0; element = 0; }
    CExoArrayList(int n);
    ~CExoArrayList() { if (element) delete[] element; }
    void Allocate(int n);                       // 0x005e03c0 for pointer lists
    void SetSize(int n);
    void Add(T t)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 8);
        element[num++] = t;
    }
    void Insert(T t, int nIndex);              // 0x00445020 (g/ptr_list.cpp)
    void Remove(T t);                          // 0x00445080
    void AddUnique(T t);                        // 0x0046bd10
    CExoArrayList<T>& operator=(const CExoArrayList<T>& o);    // 0x0046bc80
};

template <class T>
CExoArrayList<T>::CExoArrayList(int n)
{
    num = 0;
    array_size = 0;
    element = 0;
    if (n)
        Allocate(n);
}

template <class T>
void CExoArrayList<T>::SetSize(int n)
{
    if (n == 0) {
        if (element) {
            delete[] element;
            element = 0;
        }
        array_size = 0;
    } else {
        Allocate(n);
    }
    num = n;
}


#ifdef G_EXO_ARRAY_LIST_REMOVE
template <class T>
void CExoArrayList<T>::Remove(T t)
{
    int i;
    for (i = num - 1; i >= 0; i--) {
        if (element[i] == t)
            break;
    }
    num--;
    for (; i < num; i++)
        element[i] = element[i + 1];
}
#endif

class Vector {
public:
    float x, y, z;

    Vector() { x = 0; y = 0; z = 0; }
};

// --- SafePointer: a back-referenced pointer; the target keeps a list of the
// SafePointers that point at it (+0x04) and clears them when it dies. ------
template <class T>
class SafePointer {
public:
    T* m_p;

    SafePointer() { m_p = 0; }
    ~SafePointer()
    {
        if (m_p)
            m_p->m_lRefs.Remove(this);
    }
    SafePointer<T>& operator=(T* p)
    {
        if (m_p)
            m_p->m_lRefs.Remove(this);
        m_p = p;
        if (p)
            p->m_lRefs.Add(this);
        return *this;
    }
    T* operator->() { return m_p; }
    operator T*() { return m_p; }
};

template <class T>
class SafePointerList : public CExoArrayList<SafePointer<T>*> {
public:
    ~SafePointerList();
};

#ifndef G_SAFE_POINTER_LIST_DECL_ONLY
template <class T>
SafePointerList<T>::~SafePointerList()
{
    for (int i = num - 1; i >= 0; i--) {
        SafePointer<T>* sp = element[i];
        T* p = sp->m_p;
        if (p) {
            int j;
            for (j = p->m_lRefs.num - 1; j >= 0; j--) {
                if (p->m_lRefs.element[j] == sp)
                    break;
            }
            p->m_lRefs.num--;
            for (; j < p->m_lRefs.num; j++)
                p->m_lRefs.element[j] = p->m_lRefs.element[j + 1];
        }
        sp->m_p = 0;
    }
    SetSize(0);
}
#endif

// Base of every SafePointer target: the list of SafePointers aimed at it.
template <class T>
class SafePointerTarget {
public:
    SafePointerList<T> m_lRefs;
};

class CAurTextureBasic;
class CAurFontInfo;

// vtable 0x0073f0a8, 0x18 bytes: a texture reference that may forward to a
// target texture (+0x10).
class CAurTexture : public SafePointerTarget<CAurTexture> {
public:
    CAurTexture();
    virtual ~CAurTexture();                        // 0x00
    virtual CAurTextureBasic* AsBasic();           // 0x04
    virtual CAurTexture* GetTarget();              // 0x08
    virtual unsigned char IsCube();                // 0x0c
    virtual unsigned char IsSkipOrdering();        // 0x10
    virtual void v14(); virtual void v18(); virtual unsigned char IsLightmap();   // 0x1c
   
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual unsigned char KeepImage();             // 0x30
    virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7c();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8c();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9c();
    virtual void va0(); virtual void va4(); virtual void va8();
    virtual int SetImage(int a, void* pImage);     // 0xac 0x00421430
    virtual void FreeImage();                      // 0xb0
    virtual void Bind();                           // 0xb4 0x004214b0

    unsigned char IsNull();
    void SetTarget(CAurTexture* pTarget);
    void SetIsEnvironmentMapped(unsigned char b);

    SafePointer<CAurTexture> m_pTarget;            // +0x10
    int m_n14;                                     // +0x14
};

// Procedure object (+0x34).
class CAurTextureProcedure {
public:
    virtual ~CAurTextureProcedure();
    virtual void Update();                         // 0x04
    virtual void Init();                           // 0x08
    virtual void v0c();
    virtual void Prepare();                        // 0x10
};

// vtable 0x0073f160, 0xec bytes.
class CAurTextureBasic : public CAurTexture {
public:
    CAurTextureBasic(char* szName, char* szFile);  // 0x00423150
    virtual ~CAurTextureBasic();                   // 0x00422f40
    void Reset();                                  // 0x00420670
    unsigned int glImage(int a);                   // 0x00420970
    void ParseField(char* szField);                // 0x00422390
    void Init(char* szText);                       // 0x00422af0
    void AddToOrderedLists();                      // 0x00421560
    void RemoveFromOrderedLists();                 // 0x00421650
    void ConstructImage();                         // 0x0041fa30
    void GetMemoryUsage(int* pOut);                // 0x00420710
    void AsNormalizeVectorCubeMap();               // 0x00420c20
    void BuildAndStoreTexture();                   // 0x004216a0
    void SetScheme(int* pInts, int n);             // 0x0041fd00
    void StoreMemoryUsage(int* pOut);
    int IsScheme(char* szFile, int* pInts, int nCount);

    char pad18[0x20 - 0x18];
    float m_fAlphaMean;                            // +0x20 (-1 = unknown)
    char pad24[0x34 - 0x24];
    CAurTextureProcedure* m_pProcedure;            // +0x34
    CAurFontInfo* m_pTxi;                          // +0x38
    int* m_pScheme;                                // +0x3c
    unsigned int* m_pGLNames;                      // +0x40
    void* m_pImage;                                // +0x44
    void* m_p48;                                   // +0x48
    char pad4c[0x58 - 0x4c];
    unsigned int m_nGLName;                        // +0x58
    int m_n5c;                                     // +0x5c
    int m_n60;                                     // +0x60
    int m_n64;                                     // +0x64
    int m_nWidth;                                  // +0x68
    int m_nHeight;                                 // +0x6c
    int m_nBytesPerPixel;                          // +0x70
    int m_nFormat;                                 // +0x74
    char m_szName[32];                             // +0x78
    char m_szFileName[32];                         // +0x98
    short m_nDownsampleMax;                        // +0xb8
    short m_nDownsampleMin;                        // +0xba
    short m_nDownsample;                           // +0xbc
    short m_nAppliedDownsample;                    // +0xbe
    short m_nClamp;                                // +0xc0
    short m_nNumX;                                 // +0xc2
    short m_nNumY;                                 // +0xc4
    char padc6[0xcc - 0xc6];
    short m_nSchemeCount;                          // +0xcc
    short m_nce;                                   // +0xce
    unsigned short m_nHash;                        // +0xd0
    unsigned char m_bTemporary;                    // +0xd2
    unsigned char m_bOrderedA;                     // +0xd3
    char padd4[0xda - 0xd4];
    unsigned char m_bEnvironmentMapped;            // +0xda
    unsigned char m_bCube;                         // +0xdb
    unsigned char m_bMipmap;                       // +0xdc
    unsigned char m_bFilter;                       // +0xdd
    char padde[0xe0 - 0xde];
    unsigned char m_bBuilt;                        // +0xe0
    unsigned char m_bSkipOrdering;                 // +0xe1
    unsigned char m_be2;                           // +0xe2
    unsigned char m_be3;                           // +0xe3
    unsigned char m_bMemoryDirty;                  // +0xe4
    unsigned char m_be5;                           // +0xe5
    unsigned char m_be6;                           // +0xe6
    unsigned char m_bNeedsUpdate;                  // +0xe7
    int m_nMemoryUsage;                            // +0xe8
};

// Font info (+0x18 / +0x24 lists).
class CAurFontInfo {
public:
    ~CAurFontInfo();

    char pad00[0x18];
    CExoArrayList<int> m_l18;                      // +0x18
    CExoArrayList<int> m_l24;                      // +0x24
};

// --- globals ---------------------------------------------------------------
extern unsigned int g_aGLTextureTargets[];      // 0x0078d448
extern unsigned int g_nGLTexture2D;             // 0x0078d458
extern unsigned int g_nGLTextureCubeMap;        // 0x0078d478
extern int g_nAnisotropyRequested;              // 0x007a685c
extern int g_nActiveTextureUnit;                // 0x007a6898
extern unsigned int g_aBoundTextures[];         // 0x007a683c
extern int g_nBindLimit;                        // 0x007a470c
extern int g_nBindCount;                        // 0x007a4710
extern unsigned char g_bTextureModeDynamic;     // 0x0078d400
extern unsigned char g_bBumpMaps;               // 0x007a46d4
extern unsigned char g_b7a46d5;                 // 0x007a46d5
extern SafePointer<CAurTexture> g_pNullTexture; // 0x007a477c
extern CExoArrayList<CAurTextureBasic*> g_lTextures;        // 0x007a4770
extern CExoArrayList<CAurTextureBasic*> g_lOrderedA;    // 0x007a4780
extern CExoArrayList<CAurTextureBasic*> g_lHashed;      // 0x007a4798
extern CExoArrayList<CAurTextureBasic*> g_lOrderedB;    // 0x007a47a4

int AurHasCubeMaps();                           // 0x0045f8b0

#endif
