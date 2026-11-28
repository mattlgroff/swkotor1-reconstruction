// render_window.h -- CRenderWindow
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/d/app_misc.cpp); the other copies' members fall on its members.
#ifndef SHARED_RENDER_WINDOW_H
#define SHARED_RENDER_WINDOW_H


class CRenderWindow {
public:
    virtual void Slot00();
    virtual int IsFullScreen();     // 0x04
    virtual void Slot08();
    virtual void Slot0c();
    virtual void Slot10();
    virtual void Slot14();
    virtual void Slot18();
    virtual void Slot1c();
    virtual void Slot20();
    virtual void SetGamma(float f, int bApply); // 0x24
};

#endif
