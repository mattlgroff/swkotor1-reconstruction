// TextureController leaves in 0x0041f250-0x0041f4c5 (the base
// class and the other controllers live in worker H's 0x004608a0-0x004634d0;
// layout copied from matching/include/h/texctl.h).

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList() { num = 0; array_size = 0; element = 0; }
    ~CExoArrayList() { if (element) delete[] element; }
};

class AurTexture;

// vtable 0x00741c08, size 0x38.
class TextureController {
public:
    TextureController(AurTexture* pTexture);        // 0x00462a70
    virtual ~TextureController();                   // 0x00460cf0
    virtual void Update() = 0;                      // +0x04
    virtual void Init() = 0;                        // +0x08
    virtual void ParseField(char* szField);         // +0x0c 0x00462b00
    virtual bool IsLinear();                        // +0x10 0x0041f250
    virtual void v14();                             // +0x14 0x00460d70
    virtual void Distort();                         // +0x18 0x0045fd70

    AurTexture* m_pTexture;                 // +0x04
    int m_nFrame;                           // +0x08
    int m_bDistort;                         // +0x0c
    int m_nDistortAngle;                    // +0x10
    float m_fDistortionAmplitude;           // +0x14
    float m_fSpeed;                         // +0x18
    CExoArrayList<float> m_lChannelScale;       // +0x1c
    CExoArrayList<float> m_lChannelTranslate;   // +0x28
    unsigned char* m_pDistortData;          // +0x34
};

// vtable 0x0073ed54
class LifeTextureController : public TextureController {
public:
    virtual ~LifeTextureController();               // 0x0041f2b0
    virtual void Update();
    virtual void Init();

    unsigned char* m_pCells;                // +0x38
};

// vtable 0x0073ed70
class PerlinTextureController : public TextureController {
public:
    virtual ~PerlinTextureController();             // 0x0041f350
    virtual void Update();
    virtual void Init();

    unsigned char* m_pNoise;                // +0x38
    unsigned char* m_pGradient;             // +0x3c
};

// vtable 0x0073ed90
class RandomTextureController : public TextureController {
public:
    virtual ~RandomTextureController();             // 0x0041f3f0
    virtual void Update();
    virtual void Init();                            // 0x004609f0

    unsigned char* m_p38;                   // +0x38
    unsigned char* m_p3c;                   // +0x3c
};

// vtable 0x0073edac
class RingDistortTextureController : public TextureController {
public:
    virtual ~RingDistortTextureController();        // 0x0041f470
    virtual void Update();
    virtual void Init();                            // 0x00460a50

    unsigned char* m_p38;                   // +0x38
    int m_n3c;                              // +0x3c
};

// FUNCTION: SWKOTOR 0x0041f250
bool TextureController::IsLinear()
{
    return false;
}

// FUNCTION: SWKOTOR 0x0041f290 SYMBOL: ??_GLifeTextureController@@UAEPAXI@Z
// FUNCTION: SWKOTOR 0x0041f2b0
LifeTextureController::~LifeTextureController()
{
    delete[] m_pCells;
}

// FUNCTION: SWKOTOR 0x0041f330 SYMBOL: ??_GPerlinTextureController@@UAEPAXI@Z
// FUNCTION: SWKOTOR 0x0041f350
PerlinTextureController::~PerlinTextureController()
{
    delete[] m_pNoise;
    delete[] m_pGradient;
}

// FUNCTION: SWKOTOR 0x0041f3d0 SYMBOL: ??_GRandomTextureController@@UAEPAXI@Z
// FUNCTION: SWKOTOR 0x0041f3f0
RandomTextureController::~RandomTextureController()
{
    delete[] m_p38;
    delete[] m_p3c;
}

// FUNCTION: SWKOTOR 0x0041f450 SYMBOL: ??_GRingDistortTextureController@@UAEPAXI@Z
// FUNCTION: SWKOTOR 0x0041f470
RingDistortTextureController::~RingDistortTextureController()
{
    delete[] m_p38;
}
