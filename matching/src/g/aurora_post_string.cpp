// CAurPostString (timed on-screen debug strings), 0x0044d010-0x0044d320.
extern "C" {
__declspec(dllimport) void __stdcall glColor4f(float r, float g, float b, float a);
char* __cdecl strcpy(char* d, const char* s);
unsigned int __cdecl strlen(const char* s);
}
#pragma intrinsic(strcpy, strlen)

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    T& operator[](int i) { return element[i]; }
    void Allocate(int nSize); // 0x005e03c0
    void Remove(T x);         // 0x00445080
    void Add(T x)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 8);
        element[num] = x;
        num++;
    }
};

struct AurColor {
    float r, g, b, a;
};

class CAurPostString {
public:
    CAurPostString(const char* sz, int nX, int nY, float fLife);
    void Render();

    char m_szText[0x400];  // +0x000
    int m_nLength;         // +0x400
    float m_fLife;         // +0x404
    float m_fTotalLife;    // +0x408
    AurColor m_cStart;     // +0x40c
    AurColor m_cEnd;       // +0x41c
    int m_nX;              // +0x42c
    int m_nY;              // +0x430
};

extern CExoArrayList<CAurPostString*> g_aPostStrings; // 0x007b90fc
extern AurColor g_cPostStart;                         // 0x0078e368
extern AurColor g_cPostEnd;                           // 0x0078e378
extern AurColor g_cCurrentColor;                      // 0x00798bb4
extern float g_fFrameTime;                            // 0x0078e574

void AurDrawString(CAurPostString* p, int nX, int nY); // 0x0044ce50

inline void DestroyPostString(CAurPostString* p)
{
    if (p) {
        g_aPostStrings.Remove(p);
        delete p;
    }
}

// NONMATCH (23 insns): the original spills (1 - t) with fstp and evaluates
// t * end before (1 - t) * start for r/b/a; ours keeps (1 - t) in st(0)
// and multiplies it first whatever the source operand order.
// FUNCTION: SWKOTOR 0x0044d010
void CAurPostString::Render()
{
    float t;
    if (m_fTotalLife > 0.0f)
        t = (m_fTotalLife - m_fLife) / m_fTotalLife;
    else
        t = 0.0f;
    glColor4f(t * m_cEnd.r + (1.0f - t) * m_cStart.r,
              (1.0f - t) * m_cStart.g + t * m_cEnd.g,
              t * m_cEnd.b + (1.0f - t) * m_cStart.b,
              t * m_cEnd.a + (1.0f - t) * m_cStart.a);
    AurColor cSaved = g_cCurrentColor;
    g_cCurrentColor.r = t * m_cEnd.r + (1.0f - t) * m_cStart.r;
    g_cCurrentColor.g = (1.0f - t) * m_cStart.g + t * m_cEnd.g;
    g_cCurrentColor.b = t * m_cEnd.b + (1.0f - t) * m_cStart.b;
    g_cCurrentColor.a = t * m_cEnd.a + (1.0f - t) * m_cStart.a;
    AurDrawString(this, m_nX, m_nY);
    g_cCurrentColor = cSaved;
    m_fLife -= g_fFrameTime;
    if (m_fTotalLife > 0.0f && m_fLife < 0.0f) {
        g_aPostStrings.Remove(this);
        delete this;
    }
}

// NONMATCH (19 insns): the original keeps the `test edi, edi` null check of
// p after the field compares; MSVC drops it here (nested if, && p, inline
// helper and a copied pointer all tried).
// FUNCTION: SWKOTOR 0x0044d320
CAurPostString::CAurPostString(const char* sz, int nX, int nY, float fLife)
{
    strcpy(m_szText, sz);
    m_fLife = m_fTotalLife = fLife;
    m_nX = nX;
    m_nY = nY;
    int i = g_aPostStrings.num;
    m_nLength = strlen(sz);
    while (i) {
        CAurPostString* p = g_aPostStrings[i - 1];
        i--;
        if (((p->m_nX <= m_nX && p->m_nLength + p->m_nX >= m_nX)
             || (m_nX <= p->m_nX && m_nLength + m_nX >= p->m_nX))
            && p->m_nY == m_nY)
            DestroyPostString(p);
    }
    g_aPostStrings.Add(this);
    m_cStart.r = g_cPostStart.r;
    m_cStart.g = g_cPostStart.g;
    m_cStart.b = g_cPostStart.b;
    m_cStart.a = g_cPostStart.a;
    m_cEnd.r = g_cPostEnd.r;
    m_cEnd.g = g_cPostEnd.g;
    m_cEnd.b = g_cPostEnd.b;
    m_cEnd.a = g_cPostEnd.a;
}
