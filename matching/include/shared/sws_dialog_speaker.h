// sws_dialog_speaker.h -- CSWSDialogSpeaker
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/f/dialog_server.cpp); the other copies' members fall on its members.
#ifndef SHARED_SWS_DIALOG_SPEAKER_H
#define SHARED_SWS_DIALOG_SPEAKER_H

#include "shared/exo_string.h"

class CSWSDialogSpeaker {
public:
    CSWSDialogSpeaker();
    ~CSWSDialogSpeaker();
    unsigned long m_oidSpeaker;      // +0x0
    CExoString m_sTag;               // +0x4
};

#endif
