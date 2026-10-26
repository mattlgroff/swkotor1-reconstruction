// Scene virtual forwarding slot.
class Gob;
class Scene {
public:
    virtual void V000();
    virtual void V004();
    virtual void V008();
    virtual void V00C();
    virtual void V010();
    virtual void V014();
    virtual void V018();
    virtual void V01C();
    virtual void V020();
    virtual void V024();
    virtual void V028();
    virtual void V02C();
    virtual void V030();
    virtual void V034();
    virtual void V038();
    virtual void V03C();
    virtual void V040();
    virtual void V044();
    virtual void V048();
    virtual void V04C();
    virtual void V050();
    virtual void V054();
    virtual void V058();
    virtual void V05C();
    virtual void V060();
    virtual void V064();
    virtual void V068();
    virtual void V06C();
    virtual void V070();
    virtual void V074();
    virtual void V078();
    virtual void V07C();
    virtual void V080();
    virtual void V084();
    virtual void V088();
    virtual void V08C();
    virtual void V090();
    virtual void V094();
    virtual void V098();
    virtual void V09C();
    virtual void V0A0();
    virtual void V0A4();
    virtual void V0A8();
    virtual void V0AC();
    virtual void ChildAdd(Gob*); // +0xb0, 0x00455670
    void AddObject(Gob*);
};

// FUNCTION: SWKOTOR 0x00458BD0
void Scene::AddObject(Gob* p) { ChildAdd(p); }
