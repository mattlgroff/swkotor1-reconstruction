// Scene ordered Gob list (+0x54); names describe the ordering operations.
class Gob;
struct Vector { float x, y, z; };
extern int g_nGeomFadeMode; // 0x0078E3A0

template<class T> class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;
    void Remove(T value);                    // 0x00445080
    void Insert(T value, int index);         // 0x00445020
    void Allocate(int capacity);             // 0x005E03C0
    int IndexOf(T value) {
        for (int i = 0; i < num; i++)
            if (element[i] == value) return i;
        return -1;
    }
    void Add(T value) {
        if (num == array_size)
            Allocate(array_size ? array_size * 2 : 8);
        element[num++] = value;
    }
};

class Scene {
public:
    char pad_0x0[0x54];
    CExoArrayList<Gob*> m_lGobOrder;
    int m_bFogEnabled;
    float m_fFogStart, m_fFogEnd;
    int m_nFogMode;
    float m_fFogDensity;
    Vector m_vFogColor;
    char pad_0x80[0x28];
    Vector m_vSceneFocus;
    void RenderGobFirst(Gob* gob);
    void RenderGobLast(Gob* gob);
    void RenderGobAfter(Gob* gob, Gob* anchor);
    void RenderGobBefore(Gob* gob, Gob* anchor);
    void SetFog(int enabled);
    void SetFogColor(const Vector* color);
    void GetFogRange(float* start, float* end);
    void SetFogRange(float start, float end);
    bool IsGeomFadeEnabled();
    void SetSceneFocus(Vector focus);
};

// FUNCTION: SWKOTOR 0x00453E00
void Scene::RenderGobFirst(Gob* gob)
{
    m_lGobOrder.Remove(gob);
    m_lGobOrder.Insert(gob, 0);
}

// FUNCTION: SWKOTOR 0x00453E20
void Scene::RenderGobLast(Gob* gob)
{
    m_lGobOrder.Remove(gob);
    m_lGobOrder.Add(gob);
}

// FUNCTION: SWKOTOR 0x00453E70
void Scene::RenderGobAfter(Gob* gob, Gob* anchor)
{
    CExoArrayList<Gob*>* list = &m_lGobOrder;
    int index = list->IndexOf(anchor);
    list->Remove(gob);
    list->Insert(gob, index + 1);
}

// FUNCTION: SWKOTOR 0x00453EC0
void Scene::RenderGobBefore(Gob* gob, Gob* anchor)
{
    CExoArrayList<Gob*>* list = &m_lGobOrder;
    int index = list->IndexOf(anchor);
    list->Remove(gob);
    list->Insert(gob, index);
}

// FUNCTION: SWKOTOR 0x00458B40
void Scene::SetFogColor(const Vector* color)
{
    m_vFogColor = *color;
}

// FUNCTION: SWKOTOR 0x00458B60
void Scene::GetFogRange(float* start, float* end)
{
    *start = m_fFogStart;
    *end = m_fFogEnd;
}

// FUNCTION: SWKOTOR 0x00458B80
void Scene::SetFogRange(float start, float end)
{
    m_fFogStart = start;
    m_fFogEnd = end;
}

// FUNCTION: SWKOTOR 0x00458BC0
bool Scene::IsGeomFadeEnabled()
{
    return g_nGeomFadeMode == 1 ? true : false;
}

// FUNCTION: SWKOTOR 0x00458BA0
void Scene::SetSceneFocus(Vector focus)
{
    m_vSceneFocus = focus;
}

// FUNCTION: SWKOTOR 0x00458B30
void Scene::SetFog(int enabled) { m_bFogEnabled = enabled; }
