// CExoArrayList<SafePointer<CAurTexture> >::Allocate (0x0043de00): the
// texture SafePointer list of the model texture tables, whose element
// assignment expands the back-reference bookkeeping in place.
typedef unsigned int size_t;

void* __cdecl operator new[](size_t n);
void __cdecl operator delete[](void* p);

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    void Allocate(int n);
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
    void Add(T t)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 16);
        element[num++] = t;
    }
};

class CAurTexture;

template <class T>
class SafePointer {
public:
    T* m_p;

    SafePointer();                                      // 0x004204e0
    ~SafePointer();                                     // 0x00422380
    SafePointer<T>& operator=(const SafePointer<T>& p)
    {
        T* pTarget = p.m_p;
        if (m_p)
            m_p->m_lRefs.Remove(this);
        m_p = pTarget;
        if (pTarget)
            pTarget->m_lRefs.Add(this);
        return *this;
    }
};

class CAurTexture {
public:
    virtual ~CAurTexture();

    CExoArrayList<SafePointer<CAurTexture>*> m_lRefs;   // +0x04
};

template <class T>
void CExoArrayList<T>::Allocate(int n)
{
    T* pOld = element;
    array_size = n;
    element = new T[n];
    for (int i = 0; i < num; i++)
        element[i] = pOld[i];
    if (pOld)
        delete[] pOld;
}

// FUNCTION: SWKOTOR 0x0043de00 SYMBOL: ?Allocate@?$CExoArrayList@V?$SafePointer@VCAurTexture@@@@@@QAEXH@Z
template void CExoArrayList<SafePointer<CAurTexture> >::Allocate(int n);
