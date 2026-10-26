// CExoArrayList<CExoInputEvent>::Insert (clientinput.o instantiation).
struct CExoInputEvent { unsigned long m_nEvent,m_nData,m_nX,m_nY; };

template <class T>
class CExoArrayList {
public:
    T* element; int num; int array_size;
    void Allocate(int n); // <CExoInputEvent>: 0x005E0410
    void Add(T t)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 16);
        element[num] = t;
        num++;
    }
    void Insert(T t,int nIndex)
    {
        int i = num;
        Add(t);
        for (; i > nIndex; i--)
            element[i] = element[i - 1];
        element[nIndex] = t;
    }
};

template void CExoArrayList<CExoInputEvent>::Insert(CExoInputEvent t,int nIndex);

// FUNCTION: SWKOTOR 0x00620480 SYMBOL: ?Insert@?$CExoArrayList@UCExoInputEvent@@@@QAEXUCExoInputEvent@@H@Z
