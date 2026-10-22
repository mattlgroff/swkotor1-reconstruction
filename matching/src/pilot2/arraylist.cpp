// CExoArrayList<MaxFace>::SetSize (pilot group 2).
// Needs exception handling: compile with --flags "-O2 -Gy -GX" so delete[]
// goes through `eh vector destructor iterator'.

struct MaxFace {
    char data[0x20];
    ~MaxFace();
};

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    void Allocate(int n);
    void SetSize(int n);
};

template <class T>
void CExoArrayList<T>::SetSize(int n)
{
    if (n == 0) {
        if (element) {
            delete[] element;
            element = 0;
        }
        array_size = 0;
    } else {
        Allocate(n);
    }
    num = n;
}

template class CExoArrayList<MaxFace>;

// FUNCTION: SWKOTOR 0x00464350
/* CExoArrayList<MaxFace>::SetSize -- annotation key: */ /* SetSize( */
