// tag_msg.h -- tagMSG
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/d/app_misc.cpp); the other copies' members fall on its members.
#ifndef SHARED_TAG_MSG_H
#define SHARED_TAG_MSG_H


struct tagMSG {
    void* hwnd;
    unsigned int message;
    unsigned int wParam;
    long lParam;
    unsigned long time;
    long pt_x;
    long pt_y;
};

#endif
