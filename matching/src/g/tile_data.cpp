// TileData constructor / destructor (0x004473b0 / 0x00444e20).
// Self-contained: the sized CExoArrayList constructor's dead `new` keeps
// the original's empty-state SEH frame.
typedef unsigned int size_t;

void* __cdecl operator new(size_t n);           // 0x006fa7e6
void __cdecl operator delete(void* p);          // 0x006fa390

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
};

class Vector {
public:
    float x, y, z;

    Vector() { x = 0; y = 0; z = 0; }
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
};

class CTileObject {
public:
    virtual ~CTileObject();
};

class TileData {
public:
    int m_a00[6];                           // +0x00
    CExoArrayList<int> m_l18;               // +0x18
    CTileObject* m_pObject;                 // +0x24
    Vector m_v28;                           // +0x28
    int m_n34;                              // +0x34
    Vector m_vMin;                          // +0x38
    Vector m_vMax;                          // +0x44
    float m_f50;                            // +0x50
    float m_f54;                            // +0x54
    int m_n58;                              // +0x58
    CExoArrayList<int> m_l5c;               // +0x5c
    CExoArrayList<int> m_l68;               // +0x68
    CExoArrayList<int> m_l74;               // +0x74
    CExoArrayList<int> m_l80;               // +0x80
    int m_n8c;                              // +0x8c
    unsigned char m_b90;                    // +0x90
    char pad91[0x94 - 0x91];
    float m_f94;                            // +0x94

    TileData();                             // 0x004473b0
    ~TileData();                            // 0x00444e20
};

// FUNCTION: SWKOTOR 0x004473b0
TileData::TileData()
{
    m_pObject = 0;
    m_a00[0] = 0;
    m_a00[1] = 0;
    m_a00[2] = 0;
    m_a00[3] = 0;
    m_a00[4] = 0;
    m_a00[5] = 0;
    m_n34 = 0;
    m_n58 = 0;
    m_f50 = 1.0f;
    m_b90 = 0;
    m_f54 = 1.0f;
    m_f94 = 1.0f;
    m_vMin = Vector(1000000.0f, 1000000.0f, 1000000.0f);
    m_n8c = 4;
    m_vMax = Vector(1000000.0f, 1000000.0f, 1000000.0f);
}

// FUNCTION: SWKOTOR 0x00444e20
TileData::~TileData()
{
    if (m_pObject)
        delete m_pObject;
}
