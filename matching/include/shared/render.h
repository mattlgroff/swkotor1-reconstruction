// render.h -- CRender
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/c/client_exo_app_internal_s5.cpp); the other copies' members fall on its members.
#ifndef SHARED_RENDER_H
#define SHARED_RENDER_H


class CRender {
public:
    virtual void v00();
    virtual int GetFullScreen();                    // 0x04 (descriptive)
    virtual void v08(); virtual void v0C(); virtual void v10(); virtual void v14();
    virtual void ReleaseDevice();                   // 0x18 (descriptive)
    virtual void V1C();                                          // +0x1c
    virtual void V20();                                          // +0x20
    virtual void SetGamma(float f);                              // +0x24
};

#endif
