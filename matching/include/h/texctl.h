// TextureController hierarchy (0x004608A0-0x004634D0) and the
// model text-field parse helpers (0x00463510-0x004638F0).
#ifndef MATCHING_H_TEXCTL_H
#define MATCHING_H_TEXCTL_H

#include <string.h>

extern "C" int __cdecl _stricmp(const char*, const char*);
extern "C" int __cdecl sscanf(const char*, const char*, ...);

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList(int nSize = 0)
    {
        num = 0;
        array_size = nSize;
        element = 0;
        if (array_size)
            element = new T[array_size];
    }
    ~CExoArrayList() { if (element) delete[] element; }
    void Allocate(int n);
    void AllocateOutOfLine(int n);      // 0x00437D60 (CExoArrayList<float>)
    T& operator[](int i) { return element[i]; }
    void DelIndex(int i);
    void Insert(T value, int nIndex);   // 0x00445020 (pointer lists)
    void SetSizeCall(int n);            // out-of-line SetSize (0x00444F10 MaxFace)
    void AddCall(T value);              // out-of-line Add (0x004647F0 MaxFace)
    __forceinline void AddInline(T value)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 8);
        element[num++] = value;
    }
    void SetSize(int n)
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
#ifndef H_ADD_OUT_OF_LINE
    __forceinline
#endif
    void Add(T value)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 8);
        element[num++] = value;
    }
    CExoArrayList<T>& operator=(const CExoArrayList<T>& o)
    {
        num = 0;
        for (int i = 0; i < o.num; i++)
            Add(o.element[i]);
        return *this;
    }
    void AddOutOfLine(T value)
    {
        if (num == array_size)
            AllocateOutOfLine(array_size ? array_size * 2 : 16);
        element[num++] = value;
    }
    // Allocate / SetSize expanded in place (TextureController::ParseField).
    __forceinline void AllocateInline(int n)
    {
        T* old = element;
        array_size = n;
        element = new T[n];
        for (int i = 0; i < num; i++)
            element[i] = old[i];
        if (old)
            delete[] old;
    }
    __forceinline void SetSizeInline(int n)
    {
        if (n == 0) {
            if (element) {
                delete[] element;
                element = 0;
            }
            array_size = 0;
        } else {
            AllocateInline(n);
        }
        num = n;
    }
    // Same, for the instances whose Allocate stays out of line.
    void SetSizeOutOfLine(int n)
    {
        if (n == 0) {
            if (element) {
                delete[] element;
                element = 0;
            }
            array_size = 0;
        } else {
            AllocateOutOfLine(n);
        }
        num = n;
    }
};

// Inline everywhere except keys.cpp, which defines H_ALLOCATE_OUT_OF_LINE to
// emit the out-of-line instances.
template <class T>
#ifndef H_ALLOCATE_OUT_OF_LINE
inline
#endif
void CExoArrayList<T>::Allocate(int n)
{
    T* old = element;
    array_size = n;
    element = new T[n];
    for (int i = 0; i < num; i++)
        element[i] = old[i];
    if (old)
        delete[] old;
}

class Vector {
public:
    float x, y, z;

    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    Vector& operator/=(float f);                // 0x004A97D0
};

class Quaternion {
public:
    float w, x, y, z;

    Quaternion() {}
    Quaternion(float fW, float fX, float fY, float fZ) { w = fW; x = fX; y = fY; z = fZ; }
    Quaternion(Vector vAxis, float fAngle);     // 0x004AC960
};

// Generic controller key (0x2c bytes): time plus up to ten values.
struct AurGenericKey {
    float fValues[10];      // +0x00
    int nNumValues;         // +0x28
};

struct AurKeyString {
    float fTime;            // +0x00
    char szValue[32];       // +0x04

    AurKeyString() { fTime = 0; }
};

// Field parsers (cdecl).
char* SkipWhitespace(char* s);                      // 0x00463510
char* GetWord(char* s);                             // 0x00463530
void ParseVector(char* s, float* pOut);             // 0x00463570
void ParseString(char* s, char* pOut);              // 0x00463590
void ParseFloat(char* s, float* pOut);              // 0x004635C0
void ParseInt(char* s, int* pOut);                  // 0x004635E0
void ParseShort(char* s, short* pOut);              // 0x00463650
void ParseBool(char* s, unsigned char* pOut);       // 0x00463680
void ParseFloatList(char* s, CExoArrayList<float>* pOut);   // 0x004628C0

// Texture whose pixels a controller animates (+0x04 of the controller).
class AurTexture {
public:
    virtual void t00(); virtual void t04(); virtual void t08(); virtual void t0c();
    virtual void t10(); virtual void t14(); virtual void t18(); virtual void t1c();
    virtual void t20(); virtual void t24(); virtual void t28(); virtual void t2c();
    virtual void t30(); virtual void t34(); virtual void t38();
    virtual unsigned char* GetData();   // +0x3c
    virtual void t40(); virtual void t44();
    virtual int GetWidth();             // +0x48
    virtual int GetHeight();            // +0x4c
    virtual int GetBytesPerPixel();     // +0x50
    virtual void t54();
    virtual int GetNumFramesX();        // +0x58
    virtual int GetNumFramesY();        // +0x5c
    virtual void t60(); virtual void t64(); virtual void t68(); virtual void t6c();
    virtual void t70(); virtual void t74(); virtual void t78(); virtual void t7c();
    virtual void t80(); virtual void t84(); virtual void t88(); virtual void t8c();
    virtual void t90(); virtual void t94(); virtual void t98(); virtual void t9c();
    virtual void ta0(); virtual void ta4();
    virtual void SetFrame(int nFrame);  // +0xa8
    virtual void UpdateImage(int nFlags, unsigned char* pData);    // +0xac, descriptive
    virtual void FreeImage();           // +0xb0, descriptive
};

// Texture size helpers. They take the texture pointer by reference, so the
// callers store it to a stack slot that is never read back (the original's
// leading `push ecx`).
static __forceinline int GetNumFrames(AurTexture* const& pTexture)
{
    return pTexture->GetNumFramesY() * pTexture->GetNumFramesX();
}

static __forceinline int GetPixelCount(AurTexture* const& pTexture)
{
    return pTexture->GetHeight() * pTexture->GetWidth();
}

static __forceinline int GetImageSize(AurTexture* const& pTexture)
{
    return pTexture->GetBytesPerPixel() * pTexture->GetHeight() * pTexture->GetWidth();
}

// vtable 0x00741C08, size 0x38.
class TextureController {
public:
    TextureController(AurTexture* pTexture);        // 0x00462A70
    virtual ~TextureController();                   // 0x00460CF0
    virtual void Update() = 0;                      // +0x04
    virtual void Init() = 0;                        // +0x08
    virtual void ParseField(char* szField);         // +0x0c 0x00462B00
    virtual void v10();                             // 0x0041F250
    virtual unsigned char* DistortImage(unsigned char* pHeight);   // +0x14 0x00460D70
    virtual void Distort();                         // +0x18 0x0045FD70

    AurTexture* m_pTexture;                 // +0x04
    int m_nFrame;                           // +0x08
    int m_bDistort;                         // +0x0c
    int m_nDistortAngle;                    // +0x10
    float m_fDistortionAmplitude;           // +0x14
    float m_fSpeed;                         // +0x18
    CExoArrayList<float> m_lChannelScale;       // +0x1c
    CExoArrayList<float> m_lChannelTranslate;   // +0x28
    unsigned char* m_pDistortData;          // +0x34

    unsigned char* Height2Channel(unsigned char* pHeight, unsigned char* pOut); // 0x00461060
};

class RandomTextureController : public TextureController {
public:
    virtual void Update();                          // 0x00462800
    virtual void Init();                            // 0x004609F0
    unsigned char* m_p38;
    unsigned char* m_p3c;
};

// vtable 0x0073ED54, size 0x3c.
class LifeTextureController : public TextureController {
public:
    virtual void Update();                          // 0x00460250
    virtual void Init();                            // 0x00460120
    unsigned char* m_pCells;                // +0x38
};

class RingDistortTextureController : public TextureController {
public:
    virtual void Update();                          // 0x00460A90
    virtual void Init();                            // 0x00460A50
    unsigned char* m_p38;
    union {
        int m_n3c;
        float m_fPhase;                     // +0x3c
    };
};

// vtable 0x00741D2C
class WaterTextureController : public TextureController {
public:
    WaterTextureController(AurTexture* pTexture);   // 0x004630B0
    virtual ~WaterTextureController();              // 0x00463120
    virtual void Update();                          // 0x00461820
    virtual void Init();                            // 0x0045FF40
    virtual void ParseField(char* szField);         // 0x004631A0

    int m_nWaterWidth;                      // +0x38
    int m_nWaterHeight;                     // +0x3c
    float* m_p40;                           // +0x40 height grid
    float* m_p44;                           // +0x44 next height grid
    float* m_p48;                           // +0x48 velocity grid
    float m_fForcePhase;                    // +0x4c
    float m_fAnglePhase;                    // +0x50
    float m_fForceCycleSpeed;               // +0x54
    float m_fAngleCycleSpeed;               // +0x58
    unsigned char* m_p5c;                   // +0x5c image
    unsigned char* m_p60;                   // +0x60 second image
    int m_nCounter;                         // +0x64
    int m_n68;                              // +0x68
};

// vtable 0x00741D80
class WaveTextureController : public TextureController {
public:
    WaveTextureController(AurTexture* pTexture);    // 0x00463260
    virtual ~WaveTextureController();               // 0x004632B0
    virtual void Update();                          // 0x00462030
    virtual void Init();                            // 0x00460000
    virtual void ParseField(char* szField);         // 0x00463310

    float* m_p38;
    float* m_p3c;
};

// vtable 0x00741D9C
class ArturoTextureController : public TextureController {
public:
    ArturoTextureController(AurTexture* pTexture);  // 0x00463320
    virtual ~ArturoTextureController();             // 0x00463380
    virtual void Update();                          // 0x00462520
    virtual void Init();                            // 0x00460560
    virtual void ParseField(char* szField);         // 0x00463400
    virtual void BuildArturoImage();                // +0x1c 0x00460690

    int m_nArturoWidth;                     // +0x38
    int m_nArturoHeight;                    // +0x3c
    float* m_p40;
    float* m_p44;
    float* m_p48;
    float m_fTime;                          // +0x4c, descriptive
    float* m_p50;
};

// vtable 0x00741DD8
class CycleTIDTextureController : public TextureController {
public:
    CycleTIDTextureController(AurTexture* pTexture);    // 0x00463470
    virtual ~CycleTIDTextureController();               // 0x004634C0
    virtual void Update();                              // 0x00460970
    virtual void Init();                                // 0x004015A0
    virtual void ParseField(char* szField);             // 0x004634D0

    float m_fFps;                           // +0x38
    int m_nFrameIndex;                      // +0x3c
    float m_fTime;                          // +0x40
};

#endif
