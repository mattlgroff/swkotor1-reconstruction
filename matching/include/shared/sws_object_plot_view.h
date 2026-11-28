// sws_object_plot_view.h -- CSWSObjectPlotView
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/b/vm_commands_get_object.cpp); the other copies' members fall on its members.
#ifndef SHARED_SWS_OBJECT_PLOT_VIEW_H
#define SHARED_SWS_OBJECT_PLOT_VIEW_H


struct CSWSObjectPlotView {
    char pad_0x0[0xf8];
    int m_bPlotObject;                                      // 0xf8
};

#endif
