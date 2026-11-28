// sw_gui_target_action_list.h -- CSWGuiTargetActionList
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/k/sw_gui_main_interface.h); the other copies' members fall on its members.
#ifndef SHARED_SW_GUI_TARGET_ACTION_LIST_H
#define SHARED_SW_GUI_TARGET_ACTION_LIST_H


struct CSWGuiTargetAction;

struct CSWGuiTargetActionList {
    CSWGuiTargetActionList();                       // 0x005A76D0 (CExoArrayList)
    ~CSWGuiTargetActionList();                      // 0x005FBFB0
    CSWGuiTargetAction* m_pData;                    // 0x00
    int m_nCount;                                   // 0x04
    int m_nAlloc;                                   // 0x08
};

#endif
