// light_node.h -- LightNode
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/i/light_manager.h); the other copies' members fall on its members.
#ifndef SHARED_LIGHT_NODE_H
#define SHARED_LIGHT_NODE_H


class Scene;

class LightNode {
public:
    char pad_0x0[0x68];
    Scene * m_pScene;                            // 0x68
    char pad_0x6c[0x24];
    int m_nShadowPriority;                              // +0x90 (placeholder name)
    char pad_0x94[0xa8 - 0x94];
    int m_bNoFade;                                      // +0xa8 (placeholder name)

    LightNode* AsMdlNodeLight();                        // 0x0043E3D0
};

#endif
