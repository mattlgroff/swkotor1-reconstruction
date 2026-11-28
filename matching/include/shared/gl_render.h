// gl_render.h -- GLRender
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/h/gob_partdraw.cpp); the other copies' members fall on its members.
#ifndef SHARED_GL_RENDER_H
#define SHARED_GL_RENDER_H

#include "shared/vector.h"

class Quaternion;

class GLRender {
public:
    static void Transform(const Vector* pPos, const Quaternion* pOri, int bForce);  // 0x0046F060

    // declared by other copies
    static void ActivateTextureStage(int nStage);
};

#endif
