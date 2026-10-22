// Small server helpers: CSWGlobalVariableTable dtor, CScriptLocation::Equals,
// CServerAIMaster::AppendToScriptsRun.
#include "b/global_variable_table.h"
#include "b/sws_item.h"
#include "b/vector.h"

// FUNCTION: SWKOTOR 0x00528DE0
CSWGlobalVariableTable::~CSWGlobalVariableTable()
{
}

// FUNCTION: SWKOTOR 0x0052BF80
int CScriptLocation::Equals(CScriptLocation* pOther)
{
    if (*(Vector*)m_vPosition == *(Vector*)pOther->m_vPosition &&
        *(Vector*)m_vOrientation == *(Vector*)pOther->m_vOrientation) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0052BFC0
void CServerAIMaster::AppendToScriptsRun(const CExoString& sScript)
{
    CExoString sSeparator(",");
    m_sScriptsRun = m_sScriptsRun + sScript + sSeparator;
}
