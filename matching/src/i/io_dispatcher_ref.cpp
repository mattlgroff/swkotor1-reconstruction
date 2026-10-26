// IODispatcher::GetRef (aurio.o, 0x004A0580); name from the Mac debug map.
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
        element = 0;
        if (array_size)
            element = new T[array_size];
    }
    ~CExoArrayList()
    {
        if (element)
            delete[] element;
    }
};

class IODispatcher {
public:
    static IODispatcher* GetRef();

    CExoArrayList<void*> m_lRequests;                   // +0x00
};

// FUNCTION: SWKOTOR 0x004A0580
IODispatcher* IODispatcher::GetRef()
{
    static IODispatcher s_dispatcher;
    return &s_dispatcher;
}
