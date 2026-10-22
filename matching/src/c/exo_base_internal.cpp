// CExoBaseInternal, the wide-to-multibyte copy helper and
// CExoArrayList<FILETIME>::Insert (b005e0 / b005e7).

class CExoString {
public:
    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04

    ~CExoString();                                  // 0x005E5C20
    int CompareNoCase(const CExoString& s) const;   // 0x005E6450
};

class CExoBaseInternal {
public:
    int m_nResTypes;                                // 0x00
    unsigned short* m_pResTypes;                    // 0x04
    CExoString* m_pResTypeExtensions;               // 0x08

    CExoBaseInternal();
    ~CExoBaseInternal();
    void InitResTypes();                            // 0x005E6D20
    unsigned short GetResTypeFromExtension(const CExoString& sExtension);
};

// FUNCTION: SWKOTOR 0x005E8BA0
CExoBaseInternal::CExoBaseInternal()
{
    InitResTypes();
}

// FUNCTION: SWKOTOR 0x005E8BB0
CExoBaseInternal::~CExoBaseInternal()
{
    if (m_pResTypes) {
        delete m_pResTypes;
    }
    if (m_pResTypeExtensions) {
        delete[] m_pResTypeExtensions;
    }
}

// FUNCTION: SWKOTOR 0x005E7A40
unsigned short CExoBaseInternal::GetResTypeFromExtension(const CExoString& sExtension)
{
    for (int i = 0; i < m_nResTypes; i++) {
        if (m_pResTypeExtensions[i].CompareNoCase(sExtension)) {
            return m_pResTypes[i];
        }
    }
    return m_pResTypes[0x57];
}

extern "C" unsigned int __cdecl wcslen(const unsigned short* s);    // 0x006FD83E
extern "C" int __cdecl wctomb(char* mb, unsigned short wc);         // 0x006FD817

// FUNCTION: SWKOTOR 0x005E8B20
// Wide to multibyte copy (no terminator); characters that do not convert and
// control characters 1..31 become 0x7f.
int __stdcall ExoWideToMultiByte(char* sDest, const unsigned short* sSource, unsigned int nMax)
{
    int nOut = 0;
    unsigned int nLength = wcslen(sSource);
    for (unsigned int i = 0; i < (nMax < nLength ? nMax : nLength); i++) {
        char* p = sDest + nOut;
        int n = wctomb(p, sSource[i]);
        if (n != -1 && (sSource[i] < 1 || sSource[i] > 0x1f)) {
            nOut += n;
        } else {
            *p = 0x7f;
            nOut++;
        }
    }
    return 1;
}

struct _FILETIME {
    unsigned long dwLowDateTime;
    unsigned long dwHighDateTime;
};

template <class T>
class CExoArrayList {
public:
    T* element;                                     // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08

    void Allocate(int n);                           // 0x004C4F50 for _FILETIME
    void Insert(T t, int pos);
};

// FUNCTION: SWKOTOR 0x005E8BF0 SYMBOL: ?Insert@?$CExoArrayList@U_FILETIME@@@@QAEXU_FILETIME@@H@Z
template <class T>
void CExoArrayList<T>::Insert(T t, int pos)
{
    int i = num;
    if (num == array_size) {
        Allocate(array_size ? array_size * 2 : 16);
    }
    element[num] = t;
    num++;
    for (; i > pos; i--) {
        element[i] = element[i - 1];
    }
    element[pos] = t;
}

template class CExoArrayList<_FILETIME>;
