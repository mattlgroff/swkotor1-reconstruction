// Save the explored-area bitmap from the active server module.
#include "a/sws_area.h"

// FUNCTION: SWKOTOR 0x005061D0
void CSWSArea::SaveMaps(CResGFF* pRes, CResStruct* pStruct)
{
    CSWSModule* pModule = GetServerExoApp()->GetModule();
    if (pModule == NULL) {
        return;
    }

    CSWSAreaMap* pAreaMap = pModule->m_pAreaMap;
    if (pAreaMap != NULL && pAreaMap->m_nWords > 0) {

        unsigned long nWords = 0;
        int nWidth = 0;
        int nHeight = 0;
        unsigned long* pData = pAreaMap->GetExploredData(&nWords, &nWidth, &nHeight);

        CResStruct cAreaMap;
        pRes->AddStructToStruct(&cAreaMap, pStruct, "AreaMap", 0x65);
        pRes->WriteFieldINT(&cAreaMap, nWidth, "AreaMapResX");
        pRes->WriteFieldINT(&cAreaMap, nHeight, "AreaMapResY");
        pRes->WriteFieldDWORD(&cAreaMap, nWords * 4, "AreaMapDataSize");
        pRes->WriteFieldVOID(&cAreaMap, pData, nWords * 4, "AreaMapData");
    }
}
