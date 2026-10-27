// Check for text in the player's, default, or module language.
#include "b/sws_message.h"
#include "b/exo_loc_string.h"

// FUNCTION: SWKOTOR 0x0056AC20
int CSWSMessage::HasValidLocalizedString(CExoLocString* pString, unsigned char nGender)
{
    CExoString sText;
    CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
    if (!pServer) return 0;
    int nLanguage = pServer->GetPlayerLanguage(m_nPlayerId);
    int nModuleLanguage = g_pAppManager->GetServerExoApp()->GetModuleLanguage();
    if (!pString->GetStringLoc(nLanguage, sText, nGender) &&
        !(nLanguage != 0 && pString->GetStringLoc(0, sText, nGender)) &&
        !(nModuleLanguage != nLanguage && nModuleLanguage != 0 && pString->GetStringLoc(nModuleLanguage, sText, nGender))) {
        unsigned char nFoundGender;
        if (!pString->GetString(0, &nLanguage, sText, &nFoundGender) && pString->m_nStrRef != 0xffffffff) {
            return 1;
        }
    }
    if (sText.GetLength() > 0) {
        return 1;
    }
    return 0;
}
