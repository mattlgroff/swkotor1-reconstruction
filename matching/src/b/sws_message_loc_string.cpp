// Localised first/last name text in a player's language (round s9).
#include "b/sws_message.h"
#include "b/exo_loc_string.h"

// FUNCTION: SWKOTOR 0x00578870
int CSWSMessage::GetLocStringServer(unsigned long nPlayerId, CExoLocString& sFirst, CExoLocString& sLast,
                                    CExoString& sOut, unsigned char nGender)
{
    CExoString sFirstText;
    CExoString sLastText;
    sOut = "";
    CServerExoApp* pServer = g_pAppManager->GetServerExoApp();
    if (!pServer) {
        return 0;
    }
    int nLanguage = pServer->GetPlayerLanguage(nPlayerId);
    sFirst.GetString(nLanguage, &sFirstText, nGender);
    sLast.GetString(nLanguage, &sLastText, nGender);
    if (sLastText.IsEmpty() || sFirstText.IsEmpty()) {
        sOut = sFirstText + sLastText;
    } else {
        CExoString sSpace(" ");
        sOut = sFirstText + sSpace + sLastText;
    }
    return 1;
}

struct STR_RES {
    CExoString sString;                                     // 0x00
    CResRef cSoundResRef;                                   // 0x08
    float fSoundLength;                                     // 0x18
};

class CTlkTable {
public:
    int Fetch(unsigned long nStrRef, STR_RES& strRes, unsigned char nGender); // 0x0041E550
};

extern CTlkTable* g_pTlkTable;                              // 0x007A3A08

// The player's language, then the default and module languages, then the
// first stored entry; a bare StrRef is fetched from the talk table and its
// sound length is added to fSeconds.
// FUNCTION: SWKOTOR 0x00578A20
int CSWSMessage::GetLocStringServerTime(unsigned long nPlayerId, CExoLocString& sString, CExoString& sOut,
                                        float& fSeconds, unsigned char nGender)
{
    CExoString sFirstText;
    CExoString sLastText;
    STR_RES strRes;
    sOut = "";
    fSeconds = 0.0f;
    if (!g_pAppManager->GetServerExoApp()) {
        return 0;
    }
    int nLanguage = g_pAppManager->GetServerExoApp()->GetPlayerLanguage(nPlayerId);
    int nModuleLanguage = g_pAppManager->GetServerExoApp()->GetModuleLanguage();
    unsigned char nFoundGender;
    if (!sString.GetStringLoc(nLanguage, sOut, nGender) &&
        !(nLanguage != 0 && sString.GetStringLoc(0, sOut, nGender)) &&
        !(nModuleLanguage != nLanguage && nModuleLanguage != 0 && sString.GetStringLoc(nModuleLanguage, sOut, nGender)) &&
        !sString.GetString(0, &nLanguage, sOut, &nFoundGender) &&
        sString.m_nStrRef != 0xffffffff) {
        g_pTlkTable->Fetch(sString.m_nStrRef, strRes, nFoundGender);
        sOut = strRes.sString;
        fSeconds = strRes.fSoundLength + fSeconds;
    }
    return 1;
}
