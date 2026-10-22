// CExoResMan (pilot). Byte-matching reconstruction for MSVC 7.0 /O2.

class CExoResMan {
public:
    int Directory(unsigned long a, int b, int c, int d, int e);
    int Leaf9460(unsigned long a, int b, int c);
    int Leaf9480(unsigned long a, int b, int c);
};

// FUNCTION: SWKOTOR 0x00409460
int CExoResMan::Leaf9460(unsigned long a, int b, int c)
{
    return Directory(a, 1, 0, c, b);
}

// FUNCTION: SWKOTOR 0x00409480
int CExoResMan::Leaf9480(unsigned long a, int b, int c)
{
    return Directory(a, b, 1, 0, c);
}
