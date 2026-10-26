// CSWPortrait (nwportrait.o): portrait resref lookup from portraits.2da.

#include "m/sw_portrait.h"

// FUNCTION: SWKOTOR 0x00678EC0
CSWPortrait::CSWPortrait()
{
    m_cBaseResRef = "";
}

// FUNCTION: SWKOTOR 0x00678EF0
CResRef CSWPortrait::GetPortrait(unsigned char nSize)
{
    CResRef cResRef = m_cBaseResRef;
    if (*cResRef.GetResRef() == 0) {
        return CResRef("");
    }
    switch (nSize) {
    case 1:
        cResRef += "T";
        break;
    case 2:
        cResRef += "S";
        break;
    case 3:
        cResRef += "M";
        break;
    case 4:
        cResRef += "L";
        break;
    case 5:
        cResRef += "H";
        break;
    }
    return cResRef;
}

// FUNCTION: SWKOTOR 0x006790D0
int CSWPortrait::SetPortraitId(unsigned short nId)
{
    CExoString sResRef;
    CExoString sPrefix;
    if (nId >= 0xfffe) {
        return 0;
    }
    if (g_pAppManager->m_pClientExoApp->GetClientOptions()->m_nGoreLevel == 0) {
        if (!g_pRules->Get2DArrays()->GetPortraits()->GetCExoStringEntry(nId, "LowGore", &sResRef)) {
            if (!g_pRules->Get2DArrays()->GetPortraits()->GetCExoStringEntry(nId, "BaseResRef", &sResRef)) {
                sPrefix = "";
                sResRef = "";
            }
        }
    } else {
        if (!g_pRules->Get2DArrays()->GetPortraits()->GetCExoStringEntry(nId, "BaseResRef", &sResRef)) {
            sPrefix = "";
            sResRef = "";
        }
    }
    sResRef = sPrefix + sResRef;
    m_cBaseResRef = sResRef;
    return 1;
}
