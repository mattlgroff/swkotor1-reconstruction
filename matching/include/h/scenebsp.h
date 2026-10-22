// SceneBSP room / node helpers (0x0046A610-0x0046E290).
#ifndef MATCHING_H_SCENEBSP_H
#define MATCHING_H_SCENEBSP_H

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    CExoArrayList() { num = 0; array_size = 0; element = 0; }
    ~CExoArrayList() { if (element) delete[] element; }
    void Allocate(int n);                       // 0x005E03C0 (pointer lists)
    T& operator[](int i) { return element[i]; }
    void Add(T value)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 8);
        element[num++] = value;
    }
    void AddB(T value)
    {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 8);
        int n = num;
        element[n] = value;
        num = n + 1;
    }
    void AddOutOfLine(T value);                 // 0x00444FE0 (pointer lists)
    // Add with Allocate expanded (copy constructor / operator=).
    __forceinline void AddCopy(T value)
    {
        if (num == array_size) {
            int n = array_size ? array_size * 2 : 8;
            T* old = element;
            array_size = n;
            element = new T[n];
            for (int i = 0; i < num; i++)
                element[i] = old[i];
            if (old)
                delete[] old;
        }
        element[num++] = value;
    }
    int Contains(T value)
    {
        int n = 0;
        for (int i = 0; i < num; i++)
            if (element[i] == value)
                n++;
        return n;
    }
    void AddUnique(T value);                    // 0x0046BD10
    void Remove(T value);                       // 0x00445080
    void RemoveNoThrow(T value) throw();        // 0x00445080 (same body)
    // Remove with the body expanded (SafePointer host destructor).
    __forceinline void RemoveInline(T value)
    {
        int i;
        for (i = num - 1; i >= 0; i--) {
            if (element[i] == value)
                break;
        }
        num--;
        for (; i < num; i++)
            element[i] = element[i + 1];
    }
    __forceinline void AddUniqueOutOfLine(T value)
    {
        if (!Contains(value))
            AddOutOfLine(value);
    }
    int ContainsOutOfLine(T value);             // 0x004AF240
    __forceinline void RemoveIfPresent(T value)
    {
        if (Contains(value))
            Remove(value);
    }
    CExoArrayList(const CExoArrayList<T>& o)
    {
        num = 0;
        array_size = 0;
        element = 0;
        for (int i = 0; i < o.num; i++)
            AddCopy(o.element[i]);
    }
    CExoArrayList<T>& operator=(const CExoArrayList<T>& o)
    {
        num = 0;
        for (int i = 0; i < o.num; i++)
            AddCopy(o.element[i]);
        return *this;
    }
};

class Vector {
public:
    float x, y, z;

    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    Vector operator+(const Vector& o) const { return Vector(x + o.x, y + o.y, z + o.z); }
    Vector operator*(float f) const { return Vector(x * f, y * f, z * f); }
    Vector operator-(const Vector& o) const { return Vector(x - o.x, y - o.y, z - o.z); }
    int IsZero();                           // 0x0046A620
};

class Quaternion {
public:
    float w, x, y, z;

    Quaternion() {}
    Quaternion(float fW, float fX, float fY, float fZ) { z = fZ; y = fY; x = fX; w = fW; }
};

Vector operator*(const Quaternion& q, const Vector& v);     // 0x004A9BB0

struct Plane {
    Vector n;
    float d;
};

float SpherePlaneSide(Vector* pCenter, float fRadius, Plane* pPlane);  // 0x004A9710
float BoxPlaneSide(Vector* pMin, Vector* pMax, Plane* pPlane);         // 0x004AAF50

class nodedata;

// Tree node returned by the tile lookups; +0x58 is its room data.
class BSPNode {
public:
    char pad_0x0[0x10];
    int m_n10;                              // +0x10
    int m_n14;                              // +0x14
    char pad_0x18[4];
    Vector m_vMin;                          // +0x1c
    Vector m_vMax;                          // +0x28
    char pad_0x34[0x24];
    nodedata* m_pData;                      // +0x58
    CExoArrayList<BSPNode*> m_lVisible;     // +0x5c
};

class TileData;
class Gob;
class Part;
class Light;
class Emitter;
class EmitterBox;

class nodedata {
public:
    CExoArrayList<Part*> m_lParts;          // +0x00
    CExoArrayList<Gob*> m_lGobs;            // +0x0c
    CExoArrayList<Part*> m_lStatic;         // +0x18
    CExoArrayList<Part*> m_lDynamic;        // +0x24
    CExoArrayList<Part*> m_lLights30;       // +0x30
    CExoArrayList<Part*> m_lLights3C;       // +0x3c
    CExoArrayList<Part*> m_lParts48;        // +0x48
    CExoArrayList<EmitterBox*> m_lEmitterBoxes;   // +0x54
    CExoArrayList<Part*> m_lParts60;        // +0x60
    void* m_p6C;                            // +0x6c
    TileData* m_pTileData;                  // +0x70
    CExoArrayList<void*> m_lObjects;        // +0x74
    CExoArrayList<void*> m_lList80;         // +0x80
    CExoArrayList<void*> m_lEmitters;       // +0x8c

    nodedata();
    ~nodedata();
    void AddObject(void* p);
    void AddEmitter(void* p);
};

BSPNode* FindTile(void* pTiles, void* pKey, int n);     // 0x004AD060
BSPNode* FindTileBox(void* pTiles, Vector* pMin, Vector* pMax);    // 0x004AD0D0

// Back-pointer holder: the target keeps a list of the SafePointers that
// reference it.
template <class T>
class SafePointer {
public:
    T* m_p;

    SafePointer<T>& operator=(T* p);        // 0x0046E240 (T = Wind)
};

class PartDynamic;

class Wind : public CExoArrayList<void*> {
public:
    void AddPart(PartDynamic* pPart);       // 0x004A6470
};

#endif
