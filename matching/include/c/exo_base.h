// Worker C matching headers: shared Exo foundation types.
#ifndef C_EXO_BASE_H
#define C_EXO_BASE_H

class CResRef;

#include "shared/exo_string.h"


#include "shared/exo_linked_list.h"

#include "shared/exo_linked_list.h"



template <class T>
class CExoArrayList {
public:
    T* element;                                     // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08

    T& operator[](int i) { return element[i]; }
    int count(CExoString s);
    void Allocate(int n);
    void Add(T t)
    {
        if (num == array_size) {
            Allocate(array_size ? array_size * 2 : 16);
        }
        element[num++] = t;
    }
    void RemoveIndex(int i);                        // out of line (e.g. 0x0048DDE0)
    void Insert(T t, int pos);                      // out of line
    void AddUnique(T t);                            // out of line (CExoString: 0x005F7C10)
    void DelIndex(int i)
    {
        num--;
        for (; i < num; i++) {
            element[i] = element[i + 1];
        }
    }
    void SetSize(int n)
    {
        if (n == 0) {
            if (element) {
                delete[] element;
                element = 0;
                array_size = 0;
            }
        } else {
            Allocate(n);
        }
        num = n;
    }
};

#endif
