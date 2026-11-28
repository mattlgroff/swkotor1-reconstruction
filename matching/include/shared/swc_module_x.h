// swc_module_x.h -- CSWCModuleX
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/c/gui_in_game_dialog.cpp); the other copies' members fall on its members.
#ifndef SHARED_SWC_MODULE_X_H
#define SHARED_SWC_MODULE_X_H

#include "shared/swc_area_x.h"

class CCameraX;
class CResRefX;

class CSWCModuleX {
public:
    char pad_0x0[0x40];
    CCameraX* m_pCamera;                                // 0x40
    char pad_0x44[0x4];
    CSWCAreaX* m_pArea;                                 // 0x48
    void SetCameraModel(CResRefX* p);                   // 0x00640F70
    float SetAnimatedCamera(unsigned short n, int a, unsigned long oid);  // 0x00641010
    CSWSAreaX* GetServerArea()
    {
        if (m_pArea) {
            return m_pArea->GetServerArea();
        }
        return 0;
    }
};

#endif
