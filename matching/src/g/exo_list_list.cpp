// CExoArrayList<int> assignment and the list-of-lists reallocation the
// triangle-strip builder uses (0x00438bb0, 0x0043c260).
typedef unsigned int size_t;

void* __cdecl operator new[](size_t n);
void __cdecl operator delete[](void* p);

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList();
    ~CExoArrayList();

    CExoArrayList<T>& operator=(const CExoArrayList<T>& l);

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
};

template <class T>
CExoArrayList<T>& CExoArrayList<T>::operator=(const CExoArrayList<T>& l)
{
    num = 0;
    for (int i = 0; i < l.num; i++)
        Add(l.element[i]);
    return *this;
}

// FUNCTION: SWKOTOR 0x00438bb0 SYMBOL: ??4?$CExoArrayList@H@@QAEAAV0@ABV0@@Z
template CExoArrayList<int>& CExoArrayList<int>::operator=(const CExoArrayList<int>& l);

// FUNCTION: SWKOTOR 0x0043c260 SYMBOL: ?Allocate@?$CExoArrayList@V?$CExoArrayList@H@@@@QAEXH@Z
template void CExoArrayList<CExoArrayList<int> >::Allocate(int n);
