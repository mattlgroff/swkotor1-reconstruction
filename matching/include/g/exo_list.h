// CExoArrayList as used by the model / part units (element, num,
// array_size). Separate from the copy in g/aur_texture.h, which carries the
// texture unit's own extras.
#ifndef G_EXO_LIST_H
#define G_EXO_LIST_H

typedef unsigned int size_t;

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList() { num = 0; array_size = 0; element = 0; }
    ~CExoArrayList() { if (element) delete[] element; }
    void Allocate(int n);
    void SetSize(int n);
    void DelIndex(int i);                       // 0x006857a0
    void Remove(T t);                           // 0x00445080
#ifdef G_EXO_LIST_CONTAINS
    int Contains(T t)
    {
        int n = 0;
        for (int i = 0; i < num; i++)
            if (element[i] == t)
                n++;
        return n;
    }
#endif
    void Add(T t)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 8);
        element[num++] = t;
    }
};

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

#endif
