// CSWSMessage::WriteCExoLocStringServer (round s9): write a localised string
// in the receiving player's language, or its StrRef.
#include "b/sws_message.h"
#include "b/exo_loc_string.h"

// FUNCTION: SWKOTOR 0x005785C0
void CSWSMessage::WriteCExoLocStringServer(const CExoLocString& sString, unsigned char nGender)
{
    CExoString sText;
    if (g_pAppManager->m_pServerExoApp) {
        CExoLocString& sLocString = const_cast<CExoLocString&>(sString);
        int nLanguage = g_pAppManager->m_pServerExoApp->GetPlayerLanguage(m_nPlayerId);
        int nModuleLanguage = g_pAppManager->m_pServerExoApp->GetModuleLanguage();
        if (sLocString.GetStringLoc(nLanguage, sText, nGender)) {
            if (sText.GetLength() + m_nWriteBufferPtr + 4 > m_nWriteBufferSize) {
                ExtendWriteBuffer(sText.GetLength() + 4);
            }
            WriteBOOL(0);
            WriteCExoString(sText, 0x20);
        } else if (nLanguage != 0 && sLocString.GetStringLoc(0, sText, nGender)) {
            if (sText.GetLength() + m_nWriteBufferPtr + 4 > m_nWriteBufferSize) {
                ExtendWriteBuffer(sText.GetLength() + 4);
            }
            WriteBOOL(0);
            WriteCExoString(sText, 0x20);
        } else if (nModuleLanguage != nLanguage && nModuleLanguage != 0 &&
                   sLocString.GetStringLoc(nModuleLanguage, sText, nGender)) {
            if (sText.GetLength() + m_nWriteBufferPtr + 4 > m_nWriteBufferSize) {
                ExtendWriteBuffer(sText.GetLength() + 4);
            }
            WriteBOOL(0);
            WriteCExoString(sText, 0x20);
        } else {
            unsigned char nFoundGender;
            if (sLocString.GetString(0, &nLanguage, sText, &nFoundGender)) {
                if (sText.GetLength() + m_nWriteBufferPtr + 4 > m_nWriteBufferSize) {
                    ExtendWriteBuffer(sText.GetLength() + 4);
                }
                WriteBOOL(0);
                WriteCExoString(sText, 0x20);
            } else if (sLocString.m_nStrRef != 0xffffffff) {
                if (m_nWriteBufferPtr + 9 > m_nWriteBufferSize) {
                    ExtendWriteBuffer(9);
                }
                WriteBOOL(1);
                WriteBYTE(nGender, 8);
                WriteDWORD(sLocString.m_nStrRef, 0x20);
            } else {
                if (sText.GetLength() + m_nWriteBufferPtr > m_nWriteBufferSize) {
                    ExtendWriteBuffer(sText.GetLength());
                }
                WriteBOOL(0);
                WriteCExoString(sText, 0x20);
            }
        }
    }
}
