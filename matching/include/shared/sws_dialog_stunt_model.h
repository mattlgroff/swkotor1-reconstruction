// sws_dialog_stunt_model.h -- CSWSDialogStuntModel
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/f/dialog_server.cpp); the other copies' members fall on its members.
#ifndef SHARED_SWS_DIALOG_STUNT_MODEL_H
#define SHARED_SWS_DIALOG_STUNT_MODEL_H

#include "shared/exo_string.h"
#include "shared/res_ref.h"

class CSWSDialogStuntModel {
public:
    CSWSDialogStuntModel();
    ~CSWSDialogStuntModel();
    CExoString m_sParticipant;       // +0x0
    CResRef m_cStuntModel;           // +0x8
};

#endif
