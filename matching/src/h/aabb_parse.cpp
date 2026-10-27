// ASCII model AABB tree reader (0x00463C20).

extern "C" int __cdecl sscanf(const char*, const char*, ...);

char* AurResGetNextLine();                  // 0x0044BFA0

class Vector {
public:
    float x, y, z;

    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
};

inline Vector operator+(const Vector& a, const Vector& b)
{
    Vector v;
    v.x = a.x + b.x;
    v.y = a.y + b.y;
    v.z = a.z + b.z;
    return v;
}

inline Vector operator-(const Vector& a, const Vector& b)
{
    Vector v;
    v.x = a.x - b.x;
    v.y = a.y - b.y;
    v.z = a.z - b.z;
    return v;
}

inline char* SkipBlanks(char* s)
{
    if (!s)
        return 0;
    while (*s == ' ' || *s == '\t')
        s++;
    return s;
}

inline float Abs(float f)
{
    return f < 0.0f ? -f : f;
}

struct AABBNode {
    Vector vMin;                            // +0x00
    Vector vMax;                            // +0x0c
    AABBNode* pLeft;                        // +0x18
    AABBNode* pRight;                       // +0x1c
    int nFace;                              // +0x20
    int nFlags;                             // +0x24

    AABBNode() : vMin(0.0f, 0.0f, 0.0f), vMax(0.0f, 0.0f, 0.0f)
    {
        pRight = 0;
        pLeft = 0;
        nFace = -1;
        nFlags = 0;
    }
};

// Reads one node line ("min max face [flags]") and, for an interior node
// (face -1), its two children from the following lines. Without explicit
// flags the split axis is the dominant axis between the child centres.
// FUNCTION: SWKOTOR 0x00463C20
void Parse_AABB(char* s, AABBNode** ppNode)
{
    s = SkipBlanks(s);
    int nFlags;
    int nFace;
    Vector vMin(0.0f, 0.0f, 0.0f);
    Vector vMax(0.0f, 0.0f, 0.0f);
    int n = sscanf(s, "%f %f %f %f %f %f %d %d", &vMin.x, &vMin.y, &vMin.z,
                   &vMax.x, &vMax.y, &vMax.z, &nFace, &nFlags);
    *ppNode = new AABBNode;
    (*ppNode)->nFace = nFace;
    (*ppNode)->vMin = vMin;
    (*ppNode)->vMax = vMax;
    if (n == 7) {
        vMin = Vector(vMin.x - 0.01f, vMin.y - 0.01f, vMin.z - 0.01f);
        vMax = Vector(vMax.x + 0.01f, vMax.y + 0.01f, vMax.z + 0.01f);
    }
    if ((*ppNode)->nFace != -1)
        return;
    char* szLine = AurResGetNextLine();
    Parse_AABB(szLine, &(*ppNode)->pLeft);
    szLine = AurResGetNextLine();
    Parse_AABB(szLine, &(*ppNode)->pRight);
    if (n == 7) {
        AABBNode* p = *ppNode;
        Vector c0 = p->pLeft->vMin + p->pLeft->vMax;
        Vector c1 = p->pRight->vMin + p->pRight->vMax;
        vMin = c1 - c0;
        if (Abs(vMin.x) > Abs(vMin.y) && Abs(vMin.x) > Abs(vMin.z))
            p->nFlags = 1 << (vMin.x > 0.0f ? 0 : 3);
        else if (Abs(vMin.y) > Abs(vMin.z))
            p->nFlags = 1 << (vMin.y > 0.0f ? 1 : 4);
        else
            p->nFlags = 1 << (vMin.z > 0.0f ? 2 : 5);
    } else {
        (*ppNode)->nFlags = nFlags;
    }
}
