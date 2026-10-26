#ifndef I_TRIANGLE_BIN_H
#define I_TRIANGLE_BIN_H

#include "i/triangle_list.h"

class CAurTexture;

class SafePointerHost : public CExoArrayList<void*> {
public:
    void Remove(void* p);                               // 0x00445080
    void RemoveNoThrow(void* p) throw();                // 0x00445080 (same body)
};

class CAurTexture {
public:
    virtual ~CAurTexture();

    SafePointerHost m_lSafePointers;                    // +0x04
};

template <class T>
class SafePointer {
public:
    SafePointer() { m_p = 0; }
    ~SafePointer()
    {
        if (m_p) {
            SafePointerHost* pHost = &m_p->m_lSafePointers;
            pHost->RemoveNoThrow(this);
        }
    }
    void Reset()
    {
        if (m_p) {
            SafePointerHost* pHost = &m_p->m_lSafePointers;
            pHost->RemoveNoThrow(this);
        }
        m_p = 0;
    }

    T* m_p;
};

class Vector3 {
public:
    Vector3()
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }

    float x;
    float y;
    float z;
};

void AurTextureRelease(CAurTexture* pTexture);          // 0x00420D70

class CAurTriangleBin {
public:
    CAurTriangleBin();                                  // 0x004A82B0
    ~CAurTriangleBin();                                 // 0x004A8350
    void DestroyGrassPolys();                           // 0x004A8460
    void BuildGrassPolys();
    __declspec(noinline) void CreateArrays(); // 0x004A87C0

    unsigned short vertex_format;
    unsigned char flags;
    unsigned char pad_03;
    unsigned int pad_04;
    unsigned int pad_08;
    int polygon_count;                                  // +0x0c
    int m_n10;                                          // +0x10
    Vector3 m_v14;                                      // +0x14
    Vector3 m_v20;                                      // +0x20
    Vector3 m_v2C;                                      // +0x2c
    unsigned char* m_pVertices;                         // +0x38
    unsigned char* m_pIndices;                          // +0x3c
    int m_n40;                                          // +0x40
    int m_n44;                                          // +0x44
    CExoArrayList<CAurTriangleListElem> m_lTriangles;   // +0x48
    SafePointer<CAurTexture> m_pTexture;                // +0x54
    CExoArrayList<int> m_lGrass58;                      // +0x58
    CExoArrayList<SafePointer<CAurTexture> > m_lGrassTextures;  // +0x64
};

int GrassSortTris(const void* a, const void* b);        // 0x004A7690

#endif
