// mdl_face_indices.h -- MdlFaceIndices
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/e/mdlanim.h); the other copies' members fall on its members.
#ifndef SHARED_MDL_FACE_INDICES_H
#define SHARED_MDL_FACE_INDICES_H


struct MdlFaceIndices {
    int* pPos;          // +0x00
    int* pTex;          // +0x04 [layer * 3 + corner]
    int* p08;
    int* p0c;
    int* pNormal;       // +0x10
};

#endif
