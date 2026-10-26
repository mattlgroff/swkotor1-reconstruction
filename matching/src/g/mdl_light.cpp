// MdlNodeLight::PostProcess (0x0043dfc0): resolve the lens-flare texture
// names into the SafePointer texture list, with the back-reference
// bookkeeping of the SafePointer assignment expanded in place.
typedef unsigned int size_t;

void* __cdecl operator new[](size_t n);
void __cdecl operator delete[](void* p);

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    T& operator[](int i) { return element[i]; }
    void SetSize(int n);
    void Allocate(int n)
    {
        T* pOld = element;
        array_size = n;
        element = new T[n];
        for (int i = 0; i < num; i++)
            element[i] = pOld[i];
        if (pOld)
            delete[] pOld;
    }
    void Add(T t)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 16);
        element[num++] = t;
    }
    void Remove(T t)
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
};

class SafePointerTexture;

class CAurTexture {
public:
    virtual ~CAurTexture();

    CExoArrayList<SafePointerTexture*> m_lRefs;     // +0x04
};

// SafePointer<CAurTexture>.
class SafePointerTexture {
public:
    CAurTexture* m_p;

    ~SafePointerTexture();                          // 0x00422380
    SafePointerTexture& operator=(CAurTexture* p)
    {
        if (m_p)
            m_p->m_lRefs.Remove(this);
        m_p = p;
        if (p)
            p->m_lRefs.Add(this);
        return *this;
    }
};

CAurTexture* AurTextureGetReference(char* szName, int n);   // 0x00423a60

class MdlNodeLight {
public:
    char pad00[0x54];
    CExoArrayList<SafePointerTexture> m_lFlareTextures;  // +0x54
    char pad60[0x84 - 0x60];
    CExoArrayList<char*> m_lFlareTextureNames;           // +0x84

    void PostProcess();
};

// FUNCTION: SWKOTOR 0x0043dfc0
void MdlNodeLight::PostProcess()
{
    m_lFlareTextures.SetSize(m_lFlareTextureNames.num);
    int i = m_lFlareTextureNames.num;
    while (i) {
        i--;
        m_lFlareTextures[i] = AurTextureGetReference(m_lFlareTextureNames[i], 0);
    }
}
