#include "m/sw_mini_game_object.h"

// FUNCTION: SWKOTOR 0x0066BFB0
CSWMiniGameObject* CSWMiniGameObjectArray::GetObjectByTag(const CExoString& sTag)
{
    for (int i = 0; i < 0xff; ++i) {
        if (m_pObjects[i] && sTag == m_pObjects[i]->GetModelName()) {
            return m_pObjects[i];
        }
    }
    return 0;
}
