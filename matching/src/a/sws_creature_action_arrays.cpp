// Related creature action/effect list boundaries. Local declarations avoid
// adding constructors to the shared implicit array-list contract.
class CGameEffect;
template <class T> class CExoArrayList {
public:
    CExoArrayList(const CExoArrayList& other) {
        num = 0;
        array_size = 0;
        element = 0;
        for (int i = 0; i < other.num; i++) Add(other.element[i]);
    }
    void Allocate(int nSize);
    void Add(T t) {
        if (num == array_size) Allocate(array_size ? array_size*2 : 16);
        element[num++] = t;
    }
    T* element;
    int num, array_size;
};
template <class T> void CExoArrayList<T>::Allocate(int nSize)
{
    T* pOld = element;
    array_size = nSize;
    element = new T[nSize];
    for (int i = 0; i < num; i++) element[i] = pOld[i];
    if (pOld) delete[] pOld;
}
// FUNCTION: SWKOTOR 0x004FDD30 SYMBOL: ??0?$CExoArrayList@PAVCGameEffect@@@@QAE@ABV0@@Z
template CExoArrayList<CGameEffect*>::CExoArrayList(const CExoArrayList<CGameEffect*>& other);
