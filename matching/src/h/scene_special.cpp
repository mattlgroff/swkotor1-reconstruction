// Scene registration of light and emitter parts below a part tree
// (0x004531A0 / 0x00453550). The original inlines each recursion five
// levels deep.

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;

    void AddUnique(T value);                // 0x0046BD10 (pointer lists)
    void Remove(T value);                   // 0x00445080 (pointer lists)
};

class Part {
public:
    virtual void V00();
    virtual void V04();
    virtual void V08();
    virtual void V0C();
    virtual void V10();
    virtual void V14();
    virtual void V18();
    virtual void V1C();
    virtual void V20();
    virtual Part* AsLight();                // +0x24
    virtual void V28();
    virtual void V2C();
    virtual void V30();
    virtual Part* AsEmitter();              // +0x34
    char pad_0x4[0x2c];
    CExoArrayList<Part*> m_lChildren;       // +0x30
};

class SceneLights {
public:
    virtual void V00();
    virtual void V04();
    virtual void V08();
    virtual void AddLight(Part* pLight);                // +0x0c
    virtual void RemoveLight(Part* pLight, int n);      // +0x10
};

class Model;
class Scene;
void SceneBSPAddPart(Scene* pScene, Part* pPart, Model* pModel);    // 0x0046E290
void SceneBSPRemovePart(Scene* pScene, Part* pPart);                // 0x0046C510

class Scene {
public:
    char pad_0x0[0x80];
    SceneLights* m_pLights;                 // +0x80
    char pad_0x84[0x10];
    CExoArrayList<Part*> m_lEmitters;       // +0x94

    void ListSpecialPartsR(Part* pPart);
    void UnlistSpecialParts(Part* pPart);
};

#pragma inline_recursion(on)
#pragma inline_depth(6)

// FUNCTION: SWKOTOR 0x004531A0
void Scene::ListSpecialPartsR(Part* pPart)
{
    if (!pPart)
        return;
    Part* pLight = pPart->AsLight();
    if (pLight) {
        m_pLights->AddLight(pLight);
        SceneBSPAddPart(this, pLight, 0);
    }
    Part* pEmitter = pPart->AsEmitter();
    if (pEmitter) {
        SceneBSPAddPart(this, pEmitter, 0);
        m_lEmitters.AddUnique(pEmitter);
    }
    for (int i = 0; i < pPart->m_lChildren.num; i++)
        ListSpecialPartsR(pPart->m_lChildren.element[i]);
}

// FUNCTION: SWKOTOR 0x00453550
void Scene::UnlistSpecialParts(Part* pPart)
{
    if (!pPart)
        return;
    Part* pLight = pPart->AsLight();
    if (pLight) {
        m_pLights->RemoveLight(pLight, 0);
        SceneBSPRemovePart(this, pLight);
    }
    Part* pEmitter = pPart->AsEmitter();
    if (pEmitter) {
        m_lEmitters.Remove(pEmitter);
        SceneBSPRemovePart(this, pEmitter);
    }
    for (int i = 0; i < pPart->m_lChildren.num; i++)
        UnlistSpecialParts(pPart->m_lChildren.element[i]);
}
