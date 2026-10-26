// CSWCCreatureStats name and portrait helpers (nwccreaturestats.o).

#include "m/sw_portrait.h"

// FUNCTION: SWKOTOR 0x00647B80
void CSWCCreatureStats::SetPortraitId(unsigned short nId)
{
    CSWPortrait cPortrait;
    if (cPortrait.SetPortraitId(nId)) {
        m_cPortrait = cPortrait.GetPortrait(0);
    }
    m_nPortraitId = nId;
}

// FUNCTION: SWKOTOR 0x00647C10
int CSWCCreatureStats::CheckCustomPortrait(CResRef* pResRef, int bFemale)
{
    CSWPortrait cPortrait;
    cPortrait.m_cBaseResRef = *pResRef;
    if (g_pExoResMan->Exists(cPortrait.GetPortrait(3), 3, NULL)) {
        return 1;
    }
    if (bFemale) {
        *pResRef = "po_hu_f_99_";
    } else {
        *pResRef = "po_hu_m_99_";
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00647CE0
CExoString CSWCCreatureStats::GetFullName()
{
    if (m_sFirstName.GetLength() > 0 && m_sLastName.GetLength() > 0) {
        return m_sFirstName + CExoString(" ") + m_sLastName;
    }
    if (m_sFirstName.GetLength() > 0) {
        return m_sFirstName;
    }
    if (m_sLastName.GetLength() > 0) {
        return m_sLastName;
    }
    return CExoString("");
}

// Descriptive name: creature size letter (creaturesize column) to radius.
// The original never initialises the result for an unknown letter; MSVC7
// gives it the dead argument slot, so the pointer bits are returned.
// FUNCTION: SWKOTOR 0x00647DD0
float CSWCCreatureStats::GetCreatureSize(CExoString* pSize)
{
    float fSize;
    if (*pSize == "T" || *pSize == "P") {
        fSize = g_pRules->m_fCreatureSizeTiny;
    } else if (*pSize == "S") {
        fSize = g_pRules->m_fCreatureSizeSmall;
    } else if (*pSize == "M") {
        fSize = g_pRules->m_fCreatureSizeMedium;
    } else if (*pSize == "L") {
        fSize = g_pRules->m_fCreatureSizeLarge;
    } else if (*pSize == "W") {
        fSize = g_pRules->m_fCreatureSizeHuge;
    }
    return fSize;
}

// FUNCTION: SWKOTOR 0x00647E80
CResRef CSWCCreatureStats::GetPortrait(int bUseAlignment)
{
    CExoString sResRef;
    C2DA* pPortraits = g_pRules->Get2DArrays()->GetPortraits();
    if (pPortraits == NULL) {
        return m_cPortrait;
    }
    if (!bUseAlignment) {
        return m_cPortrait;
    }
    sResRef = "";
    if (m_nGoodEvil <= 10) {
        if (!pPortraits->GetCExoStringEntry(m_nPortraitId, "BaseResRefVVVE", &sResRef)) {
            return m_cPortrait;
        }
    } else if (m_nGoodEvil <= 20) {
        if (!pPortraits->GetCExoStringEntry(m_nPortraitId, "BaseResRefVVE", &sResRef)) {
            return m_cPortrait;
        }
    } else if (m_nGoodEvil <= 30) {
        if (!pPortraits->GetCExoStringEntry(m_nPortraitId, "BaseResRefVE", &sResRef)) {
            return m_cPortrait;
        }
    } else if (m_nGoodEvil <= 40) {
        if (!pPortraits->GetCExoStringEntry(m_nPortraitId, "BaseResRefE", &sResRef)) {
            return m_cPortrait;
        }
    } else {
        return m_cPortrait;
    }
    return CResRef(sResRef);
}
