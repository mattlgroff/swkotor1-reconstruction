// Part range query, using the part's position.
struct Vector { float x, y, z; };
float VectorMagnitude(Vector* pVector); // 0x004AB100
class PartTriMesh {
public:
    char pad_0x0[8];
    Vector m_vPosition;
    int WithinRange(float x, float y, float z, float range);
};

// A named magnitude local keeps the stack cleanup after the x87 add.
// FUNCTION: SWKOTOR 0x0046F1A0
int PartTriMesh::WithinRange(float x, float y, float z, float range)
{
    Vector distance;
    distance.x = m_vPosition.x - x;
    distance.y = m_vPosition.y - y;
    distance.z = m_vPosition.z - z;
    float fMag = VectorMagnitude(&distance);
    return fMag < 2.0f + range ? 1 : 0;
}
