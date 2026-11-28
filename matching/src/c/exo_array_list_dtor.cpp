// CExoArrayList<T> destructors for element types with destructors
// (b0060_client_dtors_forwarders). Kept apart from exo_input.h's
// CExoArrayList, whose users must not see a destructor.

#include "shared/exo_string.h"

#include "shared/exo_loc_string_st.h"

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
