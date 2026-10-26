// Check for text in the player's, default, or module language.
#include "b/sws_message.h"
#include "b/exo_loc_string.h"

// NONMATCH (34): register choices and early branch layout.
// FUNCTION: SWKOTOR 0x0056AC20
int CSWSMessage::HasValidLocalizedString(CExoLocString* pString, unsigned char nGender)
{
    CExoString sText;
    CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
    if (!pServer) return 0;
    int nLanguage = pServer->GetPlayerLanguage(m_nPlayerId);
    int nModuleLanguage = g_pAppManager->GetServerExoApp()->GetModuleLanguage();
    int bFound = pString->GetStringLoc(nLanguage, sText, nGender);
    if (!bFound) {
        if (nLanguage != 0) bFound = pString->GetStringLoc(0, sText, nGender);
        if (!bFound) {
            if (nModuleLanguage != nLanguage && nModuleLanguage != 0)
                bFound = pString->GetStringLoc(nModuleLanguage, sText, nGender);
            if (!bFound) {
                bFound = pString->GetString(0, &nLanguage, sText, &nGender);
                if (!bFound && pString->m_nStrRef != 0xffffffff) return 1;
            }
        }
    }
    return sText.GetLength() > 0;
}
