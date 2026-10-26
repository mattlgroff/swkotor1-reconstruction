// Unique insertion into the object-ID array list.
template <class T> class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;
    void Allocate(int n); // 0x005E03C0
    int Contains(T value) {
        int count = 0;
        for (int i = 0; i < num; ++i)
            if (element[i] == value) ++count;
        return count;
    }
    void AddUnique(T value);
};

// FUNCTION: SWKOTOR 0x006449D0 SYMBOL: ?AddUnique@?$CExoArrayList@K@@QAEXK@Z
template <class T> void CExoArrayList<T>::AddUnique(T value)
{
    if (Contains(value)) return;
    if (num == array_size) Allocate(array_size ? array_size * 2 : 16);
    element[num++] = value;
}

template void CExoArrayList<unsigned long>::AddUnique(unsigned long);
