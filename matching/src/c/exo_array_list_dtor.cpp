// CExoArrayList<T> destructors for element types with destructors
// (b0060_client_dtors_forwarders). Kept apart from exo_input.h's
// CExoArrayList, whose users must not see a destructor.

class CExoString {
public:
    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04

    ~CExoString();                                  // 0x005E5C20
};

struct ExoLocString_st {
    int m_nLanguage;                                // 0x00
    CExoString m_sString;                           // 0x04

    ~ExoLocString_st();                             // 0x0059EA20
};

template <class T>
class CExoArrayList {
public:
    T* element;                                     // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08

    ~CExoArrayList()
    {
        if (element) {
            delete[] element;
        }
        element = 0;
    }
};

template class CExoArrayList<ExoLocString_st>;

// FUNCTION: SWKOTOR 0x005F7CD0 SYMBOL: ??1?$CExoArrayList@UExoLocString_st@@@@QAE@XZ
