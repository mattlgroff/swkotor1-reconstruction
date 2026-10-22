// the model code's CExoArrayList instantiations that grow by
// doubling from 16 and allocate with new[] / delete[] (0x006fc0d2 /
// 0x006fc0cd): operator[], Allocate, Add, operator= and the nested-list
// destructor (0x00437c40-0x00438ba5). Self-contained.
#ifndef G_AUR_ARRAY_H
#define G_AUR_ARRAY_H

typedef unsigned int size_t;

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    ~CExoArrayList()
    {
        if (element)
            delete[] element;
    }
    T& operator[](int i) { return element[i]; }
    void Allocate(int n);
    void SetSize(int n);
    void Add(T t)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 16);
        element[num++] = t;
    }
    CExoArrayList<T>& operator=(const CExoArrayList<T>& o)
    {
        num = 0;
        for (int i = 0; i < o.num; i++)
            Add(o.element[i]);
        return *this;
    }
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

class Quaternion {
public:
    float w, x, y, z;

    Quaternion() { z = 0; y = 0; x = 0; w = 1.0f; }
};

// 16-byte plain element (four floats).
struct Vector4 {
    float x, y, z, w;
};

class Part;

// Back-referenced texture pointer; its destructor is out of line
// (0x00422380).
class CAurTexture;
class SafePointerTexture {
public:
    CAurTexture* m_p;

    ~SafePointerTexture();
};

#endif
