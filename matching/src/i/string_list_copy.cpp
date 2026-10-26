// CExoStringMovieList constructor (0x004B71E0): copies a CExoString
// list (name from the caller-side symbol in symbols.csv).
class CExoString {
public:
    CExoString(const CExoString& s);                // 0x005E5B00
    ~CExoString();                                  // 0x005E5C20
    char* m_sString;
    unsigned long m_nBufferLength;
};

template <class T>
class CExoArrayList {
public:
    CExoArrayList()
    {
        num = 0;
        array_size = 0;
        element = 0;
    }
    void Add(T t);                                  // <CExoString>: 0x004059A0
    T* element;
    int num;
    int array_size;
};

class CExoStringMovieList {
public:
    CExoStringMovieList(CExoArrayList<CExoString>* pList);
    CExoArrayList<CExoString> m_lstMovies;          // 0x00 (placeholder name)
};

// FUNCTION: SWKOTOR 0x004B71E0
CExoStringMovieList::CExoStringMovieList(CExoArrayList<CExoString>* pList)
{
    for (int i = 0; i < pList->num; i++) {
        m_lstMovies.Add(pList->element[i]);
    }
}
