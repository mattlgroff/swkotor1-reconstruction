// CServerExoAppInternal members that pass a CResRef temporary by
// value (built in place, so CResRef's copy constructor is declared here).
#define I_CRESREF_DECLARED_COPY
#include "i/server_exo_app.h"

// FUNCTION: SWKOTOR 0x004B20E0
int CServerExoAppInternal::IncludeModuleInSave(const CExoString& sModule)
{
    int nInclude;
    C2DA* p2DA = new C2DA(CResRef("modulesave"), 0);
    if (p2DA == NULL) {
        return 1;
    }
    if (!p2DA->Load2DArray()) {
        delete p2DA;
        return 1;
    }
    bool bFailed;
    {
        CExoString sColumn("IncludeInSave");
        bFailed = !p2DA->GetINTEntry(sModule, sColumn, &nInclude);
    }
    if (bFailed) {
        delete p2DA;
        return 1;
    }
    delete p2DA;
    return nInclude != 0;
}
