// CSWSObject one-liner conversations: RunDialogOneLiner (0x004CB220).
#include "i/sws_object.h"

// FUNCTION: SWKOTOR 0x004CB220
int CSWSObject::RunDialogOneLiner(const CExoString& sDialog, unsigned long oidTarget)
{
    CResRef cDialog;
    CSWSDialog cDialogData;
    CExoLocString sText;
    CResRef cSound;
    CResRef cScript;
    unsigned char nGender = g_nDefaultGender;
    CGameObject* pObject;
    CGameObjectArray* pArray = g_pAppManager->m_pServerExoApp->GetObjectArray();
    CSWSMessage* pMessage = g_pAppManager->m_pServerExoApp->GetNWSMessage();

    if (sDialog == "") {
        cDialog = GetDialogResref();
    } else {
        cDialog = sDialog;
    }
    if (cDialog == "") {
        return 0;
    }

    CResGFF* pGFF = new CResGFF(0x7ed, "DLG ", cDialog);
    if (!pGFF->m_bLoaded) {
        delete pGFF;
        return 0;
    }
    if (!cDialogData.LoadDialog(pGFF, 0)) {
        delete pGFF;
        return 0;
    }
    delete pGFF;

    if (cDialogData.GetStartEntryOneLiner(this, sText, cSound, cScript)) {
        if (pArray->GetGameObject(oidTarget, &pObject) == GAME_OBJECT_VALID &&
            pObject->AsSWSObject()) {
            nGender = pObject->AsSWSObject()->GetGender();
        }
        pMessage->SendServerToPlayerChatMultiLangMessage(8, m_idSelf, sText, oidTarget,
                                                        nGender, NULL, 0, 0, cSound,
                                                        0x7f000000, 0);
        cDialogData.RunScript(this, cScript);
        return 1;
    }
    return 0;
}
