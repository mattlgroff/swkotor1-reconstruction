// Scene cap bucket and static mesh shadow passes
// (0x00455A40, 0x00456250).

#include "h/scene.h"

// Pointer list whose copy constructor lives in another unit (0x0046D250).
class IntList {
public:
    int* element;
    int num;
    int array_size;

    IntList(const IntList& o);              // 0x0046D250
    ~IntList() { if (element) delete[] element; }
};

class CapMaterial {
public:
    void BindTexture0();                    // 0x0047ADF0
    void SetBlendingMode(int nMode);        // 0x0047AF70
};

class CapPart {
public:
    virtual void V000(); virtual void V004(); virtual void V008(); virtual void V00C();
    virtual void V010(); virtual void V014(); virtual void V018(); virtual void V01C();
    virtual void V020(); virtual void V024(); virtual void V028(); virtual void V02C();
    virtual void V030(); virtual void V034(); virtual void V038(); virtual void V03C();
    virtual void V040(); virtual void V044(); virtual void V048(); virtual void V04C();
    virtual void V050(); virtual void V054(); virtual void V058(); virtual void V05C();
    virtual void Prepare();                 // +0x60
    virtual void Draw(int n);               // +0x64
    char pad_0x4[0x40];
    CapMaterial* m_pMaterial;               // +0x44
};

IntList* AurTextureGetOrdering();           // 0x00421970

extern CExoArrayList<CapPart*> g_aCapBuckets[];     // 0x007FC008
extern CExoArrayList<CapPart*> g_lStaticMeshes;     // 0x007FBF84

// FUNCTION: SWKOTOR 0x00455A40
void Scene::DoCapBucket()
{
    IntList lOrder(*AurTextureGetOrdering());
    for (int i = 0; i < lOrder.num; i++) {
        int n = lOrder.element[i];
        if (g_aCapBuckets[n].num) {
            CapMaterial* pMaterial = g_aCapBuckets[n].element[0]->m_pMaterial;
            pMaterial->BindTexture0();
            pMaterial->SetBlendingMode(0);
            for (int j = 0; j < g_aCapBuckets[n].num; j++) {
                CapPart* pPart = g_aCapBuckets[n].element[j];
                pPart->Prepare();
                pPart->Draw(0);
            }
        }
    }
    for (int k = 0; k < g_aCapBuckets[0].num; k++) {
        CapPart* pPart = g_aCapBuckets[0].element[k];
        if (k == 0)
            pPart->m_pMaterial->BindTexture0();
        pPart->Prepare();
        pPart->Draw(0);
    }
}

// FUNCTION: SWKOTOR 0x00456250
void Scene::DoStaticMeshShadows()
{
    void* pSun = m_pLights->m_pSun;
    IntList lLights(*m_pLights->GetLights(1));
    if (lLights.num == 1 && pSun != (void*)lLights.element[0])
        return;
    for (int i = 0; i < g_lStaticMeshes.num; i++)
        g_lStaticMeshes.element[i]->Draw(0);
}
