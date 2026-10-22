// CExoArrayList<CExoArrayList<CAurTriangleBin*>> instance
// (0x00455580 .. 0x00456A00).

class CAurTriangleBin;

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList();                                    // 0x005A76D0 (T*)
    ~CExoArrayList();                                   // 0x0047ABC0 (T*)
    CExoArrayList<T>& operator=(const CExoArrayList<T>& o);    // 0x0046BC80 (T*)
    void Allocate(int n);
    void SetSize(int n);
};

typedef CExoArrayList<CAurTriangleBin*> TriangleBinList;

// FUNCTION: SWKOTOR 0x00455580 SYMBOL: ??1?$CExoArrayList@V?$CExoArrayList@PAVCAurTriangleBin@@@@@@QAE@XZ
template <>
CExoArrayList<TriangleBinList>::~CExoArrayList()
{
    if (element)
        delete[] element;
}

// FUNCTION: SWKOTOR 0x004555B0 SYMBOL: ?Allocate@?$CExoArrayList@V?$CExoArrayList@PAVCAurTriangleBin@@@@@@QAEXH@Z
template <>
void CExoArrayList<TriangleBinList>::Allocate(int n)
{
    TriangleBinList* old = element;
    array_size = n;
    element = new TriangleBinList[n];
    for (int i = 0; i < num; i++)
        element[i] = old[i];
    if (old)
        delete[] old;
}

// FUNCTION: SWKOTOR 0x00456A00 SYMBOL: ?SetSize@?$CExoArrayList@V?$CExoArrayList@PAVCAurTriangleBin@@@@@@QAEXH@Z
template <>
void CExoArrayList<TriangleBinList>::SetSize(int n)
{
    if (n == 0) {
        if (element) {
            delete[] element;
            element = 0;
        }
        array_size = 0;
    }
    else
        Allocate(n);
    num = n;
}
