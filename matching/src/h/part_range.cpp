// Part range query, using the part's position.
struct Vector { float x, y, z; };
float VectorMagnitude(Vector* pVector); // 0x004AB100
class PartTriMesh {
public:
    char pad_0x0[8];
    Vector m_vPosition;
    int WithinRange(float x, float y, float z, float range);
};

// NONMATCH (1): stack argument cleanup is scheduled before the x87 add.
// FUNCTION: SWKOTOR 0x0046F1A0
int PartTriMesh::WithinRange(float x, float y, float z, float range)
{
    Vector distance;
    distance.x = m_vPosition.x - x;
    distance.y = m_vPosition.y - y;
    distance.z = m_vPosition.z - z;
    return VectorMagnitude(&distance) < 2.0f + range ? 1 : 0;
}
