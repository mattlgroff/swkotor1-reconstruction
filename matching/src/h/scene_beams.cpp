// Scene static beam bucket.
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
    virtual void V24();
    virtual void V28();
    virtual void V2C();
    virtual void V30();
    virtual void V34();
    virtual void V38();
    virtual void V3C();
    virtual void V40();
    virtual void V44();
    virtual void V48();
    virtual void V4C();
    virtual void V50();
    virtual void V54();
    virtual void V58();
    virtual void V5C();
    virtual void V60();
    virtual void Draw(int); // +0x64
};
template<class T> class CExoArrayList { public: T* element; int num; int array_size; };
extern int g_bStaticMeshBeams; // 0x0078E3D0, descriptive
extern CExoArrayList<Part*> g_l7FBF6C; // 0x007FBF6C
void SetBeaming(); // 0x00471450
class Scene { public: void DoStaticMeshBeams(); };

// FUNCTION: SWKOTOR 0x00450970
void Scene::DoStaticMeshBeams()
{
    if (!g_bStaticMeshBeams) return;
    SetBeaming();
    for (int i = 0; i < g_l7FBF6C.num; i++)
        g_l7FBF6C.element[i]->Draw(0);
}
