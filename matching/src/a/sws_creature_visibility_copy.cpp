#include <stdlib.h>
template <class T> class CExoArrayList {
public:
    CExoArrayList& operator=(const CExoArrayList& other);
    void Allocate(int nSize) {
        T* pOld = element;
        array_size = nSize;
        element = new T[nSize];
        for (int i = 0; i < num; i++) element[i] = pOld[i];
        if (pOld) delete[] pOld;
    }
    void Add(T t) {
        if (num == array_size) Allocate(array_size ? array_size*2 : 16);
        element[num++] = t;
    }
    T* element;
    int num, array_size;
};
template <class T> CExoArrayList<T>& CExoArrayList<T>::operator=(const CExoArrayList<T>& other)
{
    num = 0;
    for (int i = 0; i < other.num; i++) Add(other.element[i]);
    return *this;
}
// The ring/action visibility data includes a generic dword-array copy helper.
// FUNCTION: SWKOTOR 0x0051B720 SYMBOL: ??4?$CExoArrayList@K@@QAEAAV0@ABV0@@Z
template CExoArrayList<unsigned long>& CExoArrayList<unsigned long>::operator=(const CExoArrayList<unsigned long>& other);
