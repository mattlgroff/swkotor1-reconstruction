// CSWSArea::LoadTriggers (0x0050A350): GIT "TriggerList" loader.
#include "a/sws_area.h"

// FUNCTION: SWKOTOR 0x0050A350
int CSWSArea::LoadTriggers(CResGFF* pRes, CResStruct* pStruct, int bLoadObjectState,
                           int bUseTemplates)
{
    CResList cList;
    if (pRes->GetList(&cList, pStruct, "TriggerList")) {
        for (unsigned long i = 0; i < pRes->GetListCount(&cList); i++) {
            CResStruct cTrigger;
            if (pRes->GetListElement(&cTrigger, &cList, i) && pRes->GetStructID(&cTrigger) == 1) {
                int bIdSuccess;
                unsigned long oid = pRes->ReadFieldDWORD(&cTrigger, "ObjectId", bIdSuccess, 0x7f000000);
                int bSuccess;
                CSWSTrigger* pTrigger = new CSWSTrigger(oid);
                if (pTrigger == NULL) {
                    continue;
                }
                float fX, fY, fZ;
                if (bUseTemplates) {
                    CResRef cDefault("");
                    CResRef cTemplate = pRes->ReadFieldCResRef(&cTrigger, "TemplateResRef", bSuccess, cDefault);
                    if (!bSuccess || !pTrigger->LoadFromTemplate(cTemplate)) {
                        delete pTrigger;
                        continue;
                    }
                    CExoString sLinkedToModule;
                    CResRef cModuleDefault("");
                    CResRef cModule = pRes->ReadFieldCResRef(&cTrigger, "LinkedToModule", bSuccess,
                                                             cModuleDefault);
                    cModule.CopyToString(sLinkedToModule);
                    pTrigger->m_lsTransitionDestination =
                        pRes->ReadFieldCExoLocString(&cTrigger, "TransitionDestination", bSuccess,
                                                     pTrigger->m_lsTransitionDestination);
                    {
                        CExoString sDefault("");
                        pTrigger->m_sLinkedTo = pRes->ReadFieldCExoString(&cTrigger, "LinkedTo",
                                                                          bSuccess, sDefault);
                    }
                    pTrigger->m_sLinkedToModule = sLinkedToModule;
                    pTrigger->m_nLinkedToFlags = pRes->ReadFieldBYTE(&cTrigger, "LinkedToFlags", bSuccess, 0);
                    fX = pRes->ReadFieldFLOAT(&cTrigger, "XPosition", bSuccess, 0.0f);
                    fY = pRes->ReadFieldFLOAT(&cTrigger, "YPosition", bSuccess, 0.0f);
                    fZ = pRes->ReadFieldFLOAT(&cTrigger, "ZPosition", bSuccess, 0.0f);
                    pTrigger->SetPosition(Vector(fX, fY, fZ), 1, 1, 0);
                    CResList cGeometry;
                    if (pRes->GetList(&cGeometry, &cTrigger, "Geometry")) {
                        pTrigger->LoadGeometry(pRes, &cGeometry);
                    }
                } else {
                    if (!pTrigger->LoadTrigger(pRes, &cTrigger)) {
                        delete pTrigger;
                        continue;
                    }
                    fX = pRes->ReadFieldFLOAT(&cTrigger, "XPosition", bSuccess, 0.0f);
                    fY = pRes->ReadFieldFLOAT(&cTrigger, "YPosition", bSuccess, 0.0f);
                    fZ = pRes->ReadFieldFLOAT(&cTrigger, "ZPosition", bSuccess, 0.0f);
                }
                if (bLoadObjectState) {
                    pTrigger->LoadObjectState(pRes, &cTrigger);
                }
                pTrigger->AddToArea(this, fX, fY, fZ, !bLoadObjectState);
                if (pTrigger->m_bIsTrap) {
                    m_aTriggers.Add(pTrigger->m_idSelf);
                }
            }
        }
    }
    return 1;
}
