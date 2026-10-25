#ifndef I_WIND_MANAGER_H
#define I_WIND_MANAGER_H

#include "i/vector_math.h"

class PartEmitter {
public:
    void CalculateRadius(); // 0x0048E1B0
};

template <class T>
class PointerList {
public:
    void AddUnique(T value);
    void Remove(T value);
    T* element;
    int num;
    int array_size;
};

struct PointSourceWind {
    unsigned int words[6];
};

class PointSourceWindList {
public:
    void Add(PointSourceWind value);
    void Allocate(int count);
    PointSourceWind* element;
    int num;
    int array_size;
};

class WindManager {
public:
    void SetMaximumDeviation(float fMinimum, float fMaximum); // 0x004A57A0
    void SetGlobalWind(Vector vWind);                          // 0x004A5900
    void RegisterWindyEmitter(PartEmitter* pEmitter);          // 0x004A6470
    void DeregisterWindyEmitter(PartEmitter* pEmitter);        // 0x004A6480
    void AddPointSourceWind(Vector position, unsigned int a, unsigned int b, unsigned int c); // 0x004A6510
    Vector* GetGlobalPointWind(Vector* out, const Vector* point, float time); // 0x004A6440
    Vector* PointSourceWindVector(Vector* out, Vector point); // 0x004A5C80

    char pad_0x00[0x0c];
    PointSourceWindList m_lPointSourceWinds;                   // +0x0c
    Vector m_vGlobalWind;                                      // +0x18
    char pad_0x24[0x814];
    float m_fMinimumDeviation;                                 // +0x838
    float m_fMaximumDeviation;                                 // +0x83c
    char pad_0x840[0x0c];
    PointerList<PartEmitter*> m_lEmitters;                     // +0x84c
};

#endif
