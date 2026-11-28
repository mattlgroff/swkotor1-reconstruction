// scene_lights.h -- SceneLights
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/h/scene_buckets.cpp); the other copies' members fall on its members.
#ifndef SHARED_SCENE_LIGHTS_H
#define SHARED_SCENE_LIGHTS_H

#include "shared/vector.h"

template <class T> class CExoArrayList;
class Light;
class Part;

class SceneLights {
public:
    virtual void PrioritizeLights(Vector* pos, int n);          // +0x00
    virtual void V04();
    virtual void ResetLights();                                 // +0x08, descriptive
    virtual void AddLight(Part* pLight);                         // +0x0c
    virtual void RemoveLight(Part* pLight, int n);               // +0x10
    virtual void V14(); virtual void V18(); virtual void V1C();
    // MSVC places overloaded virtuals in reverse declaration order.
    virtual void EnableHighestPriorityLights(int n, int mask);  // +0x24
    virtual void EnableHighestPriorityLights(int n);            // +0x20
    virtual void AddFocusLight(int n, Vector pos);             // +0x28, descriptive
    virtual void V2C(); virtual void V30(); virtual void V34();
    virtual void SetupLightsForThisGob(Vector* pos, float radius); // +0x38
    CExoArrayList<Light*>* GetLights(int nType);                // 0x004A4B10
    char pad_0x4[0xdc];
    int m_nE0;                                  // +0xe0
    char pad_0xe4[8];
    int m_nEC;                                  // +0xec
};

#endif
