// The control and panel pointer instantiations share this implementation
// through identical COMDAT folding.  Existing callers establish the template
// name; the canonical x86 bounds the search by the signed element count.
class CSWGuiControl;

template <class T> class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    int IndexOf(T value);
};

// FUNCTION: SWKOTOR 0x006C2A40 SYMBOL: ?IndexOf@?$CExoArrayList@PAVCSWGuiControl@@@@QAEHPAVCSWGuiControl@@@Z
template <class T> int CExoArrayList<T>::IndexOf(T value)
{
    for (int i = 0; i < num; ++i) {
        if (element[i] == value) {
            return i;
        }
    }
    return -1;
}

template int CExoArrayList<CSWGuiControl*>::IndexOf(CSWGuiControl*);
