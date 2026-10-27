// CExoArrayList<T>: growable array (element, num, array_size).
#ifndef B_EXO_ARRAY_LIST_H
#define B_EXO_ARRAY_LIST_H

#include <stddef.h>

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
        element = NULL;
        if (array_size) {
            element = new T[array_size];
        }
    }

    CExoArrayList(const CExoArrayList<T>& lst);     // out of line (<CExoString*>: 0x004FDD30)

    ~CExoArrayList()
    {
        delete[] element;
        element = NULL;
    }

    // Out-of-line COMDAT for <int> at 0x005E03C0.
    void Allocate(int nSize)
    {
        T* pOld = element;
        array_size = nSize;
        element = new T[nSize];
        for (int i = 0; i < num; i++) {
            element[i] = pOld[i];
        }
        if (pOld) {
            delete[] pOld;
        }
    }

    T& operator[](int i) { return element[i]; }

    int count(T t);                 // out of line (<unsigned long>: 0x004AF240)

    void InsertAt(T t, int nPos);   // out of line (<unsigned long>: 0x00616940)

    void AddUnique(T t);            // out of line (<unsigned long>: 0x006449D0)

    void Add(T t)
    {
        if (num == array_size) {
            Allocate(array_size ? array_size * 2 : 16);
        }
        element[num++] = t;
    }

    void DelIndex(int i)
    {
        num--;
        for (; i < num; i++) {
            element[i] = element[i + 1];
        }
    }

    void SetSize(int nSize)
    {
        if (nSize == 0) {
            if (element) {
                delete[] element;
                element = NULL;
                array_size = 0;
            }
        }
        else {
            Allocate(nSize);
        }
        num = nSize;
    }
};

#endif
