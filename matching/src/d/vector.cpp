// out-of-line Vector default constructor (used by the vector
// constructor iterator 0x00401130 for `new Vector[n]`).

class Vector {
public:
    float x, y, z;
    Vector();
};

// FUNCTION: SWKOTOR 0x00409e70
Vector::Vector()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}
