// CSWSArea GIT object-list loaders (0x00505360-0x0050579F).
#include "a/sws_area.h"

// FUNCTION: SWKOTOR 0x00504A70
int CSWSArea::LoadCreatures(CResGFF* pRes, CResStruct* pStruct, int bLoadObjectState, int bUseTemplates)
{
    CResList cList;
    if (pRes->GetList(&cList, pStruct, "Creature List")) {
        for (unsigned long i = 0; i < pRes->GetListCount(&cList); i++) {
            CResStruct cCreature;
            if (pRes->GetListElement(&cCreature, &cList, i) && pRes->GetStructID(&cCreature) == 4) {
                int bSuccess;
                CSWSCreature* pCreature;
                if (bUseTemplates) {
                    pCreature = new CSWSCreature(0x7f000000, 0);
                    if (pCreature == NULL) {
                        continue;
                    }
                    CResRef cDefault("");
                    CResRef cTemplate = pRes->ReadFieldCResRef(&cCreature, "TemplateResRef", bSuccess, cDefault);
                    if (!bSuccess || !pCreature->LoadFromTemplate(cTemplate, 1)) {
                        delete pCreature;
                        continue;
                    }
                }
                else {
                    unsigned long oid = pRes->ReadFieldDWORD(&cCreature, "ObjectId", bSuccess, 0x7f000000);
                    pCreature = new CSWSCreature(oid, 0);
                    if (pCreature == NULL) {
                        continue;
                    }
                    if (!pCreature->LoadCreature(pRes, &cCreature, bLoadObjectState)) {
                        delete pCreature;
                        continue;
                    }
                }
                Vector vPosition(0.0f, 0.0f, 0.0f);
                Vector vSafe(0.0f, 0.0f, 0.0f);
                vPosition.x = pRes->ReadFieldFLOAT(&cCreature, "XPosition", bSuccess, 0.0f);
                vPosition.y = pRes->ReadFieldFLOAT(&cCreature, "YPosition", bSuccess, 0.0f);
                vPosition.z = pRes->ReadFieldFLOAT(&cCreature, "ZPosition", bSuccess, 0.0f);
                CPathfindInformation* pPathfindInfo = pCreature->m_pPathfindInformation;
                if (ComputeSafeLocation(vPosition, 20.0f, pPathfindInfo, 1, &vSafe)) {
                    vPosition = vSafe;
                }
                pCreature->AddToArea(this, vPosition.x, vPosition.y, vPosition.z, bLoadObjectState, 0);
                pCreature->SetOrientation(Vector(pRes->ReadFieldFLOAT(&cCreature, "XOrientation", bSuccess, 0.0f),
                                                 pRes->ReadFieldFLOAT(&cCreature, "YOrientation", bSuccess, 0.0f),
                                                 pRes->ReadFieldFLOAT(&cCreature, "ZOrientation", bSuccess, 0.0f)));
            }
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00504DE0
int CSWSArea::LoadItems(CResGFF* pRes, CResStruct* pStruct, int bLoadObjectState, int bUseTemplates)
{
    int bSuccess;
    CResList cList;
    if (pRes->GetList(&cList, pStruct, "List")) {
        for (unsigned long i = 0; i < pRes->GetListCount(&cList); i++) {
            CResStruct cItem;
            if (pRes->GetListElement(&cItem, &cList, i) && pRes->GetStructID(&cItem) == 0) {
                unsigned long oid = pRes->ReadFieldDWORD(&cItem, "ObjectId", bSuccess, 0x7f000000);
                CSWSItem* pItem = new CSWSItem(oid);
                if (pItem) {
                    int bRead;
                    if (bUseTemplates) {
                        CResRef cDefault("");
                        CResRef cTemplate = pRes->ReadFieldCResRef(&cItem, "TemplateResRef", bRead, cDefault);
                        if (!bRead || !pItem->LoadFromTemplate(cTemplate)) {
                            delete pItem;
                            continue;
                        }
                    }
                    else if (!pItem->LoadItem(pRes, &cItem)) {
                        delete pItem;
                        continue;
                    }
                    {
                        Vector vOrientation(0.0f, 0.0f, 0.0f);
                        if (bLoadObjectState) {
                            pItem->LoadObjectState(pRes, &cItem);
                        }
                        pItem->AddToArea(this,
                                         pRes->ReadFieldFLOAT(&cItem, "XPosition", bRead, 0.0f),
                                         pRes->ReadFieldFLOAT(&cItem, "YPosition", bRead, 0.0f),
                                         pRes->ReadFieldFLOAT(&cItem, "ZPosition", bRead, 0.0f),
                                         !bLoadObjectState);
                        vOrientation.x = pRes->ReadFieldFLOAT(&cItem, "XOrientation", bRead, 0.0f);
                        vOrientation.y = pRes->ReadFieldFLOAT(&cItem, "YOrientation", bRead, 0.0f);
                        vOrientation.z = pRes->ReadFieldFLOAT(&cItem, "ZOrientation", bRead, 0.0f);
                        pItem->SetOrientation(vOrientation);
                    }
                }
            }
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00505060
int CSWSArea::LoadEncounters(CResGFF* pRes, CResStruct* pStruct, int bLoadObjectState, int bUseTemplates)
{
    CResList cList;
    if (pRes->GetList(&cList, pStruct, "Encounter List")) {
        for (unsigned long i = 0; i < pRes->GetListCount(&cList); i++) {
            CResStruct cEncounter;
            if (pRes->GetListElement(&cEncounter, &cList, i) && pRes->GetStructID(&cEncounter) == 7) {
                int bSuccess;
                unsigned long oid = pRes->ReadFieldDWORD(&cEncounter, "ObjectId", bSuccess, 0x7f000000);
                CSWSEncounter* pEncounter = new CSWSEncounter(oid);
                if (pEncounter == NULL) {
                    continue;
                }
                if (bUseTemplates) {
                    CResRef cDefault("");
                    CResRef cTemplate = pRes->ReadFieldCResRef(&cEncounter, "TemplateResRef", bSuccess, cDefault);
                    if (!bSuccess || !pEncounter->LoadFromTemplate(cTemplate, 0, NULL)) {
                        delete pEncounter;
                        continue;
                    }
                    float fX = pRes->ReadFieldFLOAT(&cEncounter, "XPosition", bSuccess, 0.0f);
                    float fY = pRes->ReadFieldFLOAT(&cEncounter, "YPosition", bSuccess, 0.0f);
                    float fZ = pRes->ReadFieldFLOAT(&cEncounter, "ZPosition", bSuccess, 0.0f);
                    pEncounter->SetPosition(Vector(fX, fY, fZ), 1, 1, 0);
                    CResList cSubList;
                    if (pRes->GetList(&cSubList, &cEncounter, "Geometry") &&
                        !pEncounter->LoadEncounterGeometry(pRes, cSubList)) {
                        delete pEncounter;
                        pEncounter = NULL;
                    }
                    if (pRes->GetList(&cSubList, &cEncounter, "SpawnPointList")) {
                        pEncounter->LoadEncounterSpawnPoints(pRes, cSubList);
                    }
                }
                else if (!pEncounter->LoadEncounter(pRes, &cEncounter)) {
                    delete pEncounter;
                    continue;
                }
                if (pEncounter) {
                    if (bLoadObjectState) {
                        pEncounter->LoadObjectState(pRes, &cEncounter);
                    }
                    pEncounter->AddToArea(this, pEncounter->m_vPosition.x, pEncounter->m_vPosition.y,
                                          pEncounter->m_vPosition.z, !bLoadObjectState);
                }
            }
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00505360
int CSWSArea::LoadWaypoints(CResGFF* pRes, CResStruct* pStruct, int bLoadObjectState, int bUseTemplates)
{
    CResList cList;
    if (pRes->GetList(&cList, pStruct, "WaypointList")) {
        for (unsigned long i = 0; i < pRes->GetListCount(&cList); i++) {
            CResStruct cWaypoint;
            if (pRes->GetListElement(&cWaypoint, &cList, i) && pRes->GetStructID(&cWaypoint) == 5) {
                int bSuccess;
                unsigned long oid = pRes->ReadFieldDWORD(&cWaypoint, "ObjectId", bSuccess, 0x7f000000);
                CSWSWaypoint* pWaypoint = new CSWSWaypoint(oid);
                if (pWaypoint) {
                    if (!pWaypoint->LoadWaypoint(pRes, &cWaypoint)) {
                        delete pWaypoint;
                    }
                    else {
                        if (bLoadObjectState) {
                            pWaypoint->LoadObjectState(pRes, &cWaypoint);
                        }
                        pWaypoint->AddToArea(this,
                                             pRes->ReadFieldFLOAT(&cWaypoint, "XPosition", bSuccess, 0.0f),
                                             pRes->ReadFieldFLOAT(&cWaypoint, "YPosition", bSuccess, 0.0f),
                                             pRes->ReadFieldFLOAT(&cWaypoint, "ZPosition", bSuccess, 0.0f),
                                             !bLoadObjectState);
                        Vector vPosition = pWaypoint->m_vPosition;
                        vPosition.z = ComputeHeight(vPosition, 1, NULL);
                        pWaypoint->SetPosition(vPosition, 1, 1, 0);
                    }
                }
            }
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00505560
int CSWSArea::LoadSounds(CResGFF* pRes, CResStruct* pStruct, int bLoadObjectState, int bUseTemplates)
{
    CSWSSoundObject* pSound;
    CResList cList;
    if (pRes->GetList(&cList, pStruct, "SoundList")) {
        unsigned long nCount = pRes->GetListCount(&cList);
        for (unsigned long i = 0; i < nCount; i++) {
            CResStruct cSound;
            if (pRes->GetListElement(&cSound, &cList, i) && pRes->GetStructID(&cSound) == 6) {
                int bSuccess;
                unsigned long oid = pRes->ReadFieldDWORD(&cSound, "ObjectId", bSuccess, 0x7f000000);
                pSound = new CSWSSoundObject(oid);
                if (bUseTemplates) {
                    CResRef cDefault("");
                    CResRef cTemplate = pRes->ReadFieldCResRef(&cSound, "TemplateResRef", bSuccess, cDefault);
                    if (bSuccess && pSound->LoadFromTemplate(cTemplate)) {
                        pSound->m_nGeneratedType =
                            (unsigned char)pRes->ReadFieldDWORD(&cSound, "GeneratedType", bSuccess, 0);
                        pSound->SetPosition(Vector(pRes->ReadFieldFLOAT(&cSound, "XPosition", bSuccess, 0.0f),
                                                   pRes->ReadFieldFLOAT(&cSound, "YPosition", bSuccess, 0.0f),
                                                   pRes->ReadFieldFLOAT(&cSound, "ZPosition", bSuccess, 0.0f)),
                                            1, 1, 0);
                        pSound->AddToArea(this, !bLoadObjectState);
                    }
                    else if (pSound) {
                        delete pSound;
                    }
                }
                else if (pSound->LoadSound(pRes, &cSound)) {
                    pSound->AddToArea(this, !bLoadObjectState);
                }
                else if (pSound) {
                    delete pSound;
                }
            }
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x005057A0
int CSWSArea::LoadStores(CResGFF* pRes, CResStruct* pStruct, int bLoadObjectState, int bUseTemplates)
{
    CResList cList;
    if (pRes->GetList(&cList, pStruct, "StoreList")) {
        for (unsigned long i = 0; i < pRes->GetListCount(&cList); i++) {
            CResStruct cStore;
            if (pRes->GetListElement(&cStore, &cList, i) && pRes->GetStructID(&cStore) == 0xb) {
                int bSuccess;
                unsigned long oid = pRes->ReadFieldDWORD(&cStore, "ObjectId", bSuccess, 0x7f000000);
                CSWSStore* pStore = new CSWSStore(oid);
                if (pStore == NULL) {
                    continue;
                }
                if (bUseTemplates) {
                    CResRef cDefault("");
                    CResRef cTemplate = pRes->ReadFieldCResRef(&cStore, "ResRef", bSuccess, cDefault);
                    if (!bSuccess || !pStore->LoadFromTemplate(cTemplate, 1)) {
                        delete pStore;
                        continue;
                    }
                }
                else if (!pStore->LoadStore(pRes, &cStore, 0)) {
                    delete pStore;
                    continue;
                }
                Vector vOrientation(pRes->ReadFieldFLOAT(&cStore, "XOrientation", bSuccess, 0.0f),
                                    pRes->ReadFieldFLOAT(&cStore, "YOrientation", bSuccess, 0.0f),
                                    pRes->ReadFieldFLOAT(&cStore, "ZOrientation", bSuccess, 0.0f));
                if (vOrientation.MagnitudeSquared() > 0.00001f) {
                    vOrientation = NormalizeVector(vOrientation);
                }
                else {
                    vOrientation = Vector(0.0f, 1.0f, 0.0f);
                }
                pStore->SetOrientation(vOrientation);
                if (bLoadObjectState) {
                    pStore->LoadObjectState(pRes, &cStore);
                }
                pStore->AddToArea(this,
                                  pRes->ReadFieldFLOAT(&cStore, "XPosition", bSuccess, 0.0f),
                                  pRes->ReadFieldFLOAT(&cStore, "YPosition", bSuccess, 0.0f),
                                  pRes->ReadFieldFLOAT(&cStore, "ZPosition", bSuccess, 0.0f),
                                  !bLoadObjectState);
            }
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00505AF0
int CSWSArea::LoadAreaEffects(CResGFF* pRes, CResStruct* pStruct, int bLoadObjectState, int bUseTemplates)
{
    CResList cList;
    if (pRes->GetList(&cList, pStruct, "AreaEffectList")) {
        for (unsigned long i = 0; i < pRes->GetListCount(&cList); i++) {
            CResStruct cEffect;
            if (pRes->GetListElement(&cEffect, &cList, i) && pRes->GetStructID(&cEffect) == 0xd) {
                int bSuccess;
                unsigned long oid = pRes->ReadFieldDWORD(&cEffect, "ObjectId", bSuccess, 0x7f000000);
                CSWSAreaOfEffectObject* pEffect = new CSWSAreaOfEffectObject(oid);
                if (pEffect == NULL) {
                    continue;
                }
                if (!pEffect->LoadEffect(pRes, &cEffect)) {
                    delete pEffect;
                    continue;
                }
                Vector vOrientation(pRes->ReadFieldFLOAT(&cEffect, "OrientationX", bSuccess, 0.0f),
                                    pRes->ReadFieldFLOAT(&cEffect, "OrientationY", bSuccess, 0.0f),
                                    pRes->ReadFieldFLOAT(&cEffect, "OrientationZ", bSuccess, 0.0f));
                if (vOrientation.MagnitudeSquared() > 0.0001f) {
                    vOrientation = NormalizeVector(vOrientation);
                }
                else {
                    vOrientation = Vector(0.0f, 1.0f, 0.0f);
                }
                pEffect->SetOrientation(vOrientation);
                if (bLoadObjectState) {
                    pEffect->LoadObjectState(pRes, &cEffect);
                }
                pEffect->AddToArea(this,
                                   pRes->ReadFieldFLOAT(&cEffect, "PositionX", bSuccess, 0.0f),
                                   pRes->ReadFieldFLOAT(&cEffect, "PositionY", bSuccess, 0.0f),
                                   pRes->ReadFieldFLOAT(&cEffect, "PositionZ", bSuccess, 0.0f),
                                   !bLoadObjectState);
            }
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0050A0E0
int CSWSArea::LoadDoors(CResGFF* pRes, CResStruct* pStruct, int bLoadObjectState, int bUseTemplates)
{
    CResList cList;
    if (pRes->GetList(&cList, pStruct, "Door List")) {
        for (unsigned long i = 0; i < pRes->GetListCount(&cList); i++) {
            CResStruct cDoor;
            if (pRes->GetListElement(&cDoor, &cList, i) && pRes->GetStructID(&cDoor) == 8) {
                int bSuccess;
                unsigned long oid = pRes->ReadFieldDWORD(&cDoor, "ObjectId", bSuccess, 0x7f000000);
                CSWSDoor* pDoor = new CSWSDoor(oid);
                if (pDoor == NULL) {
                    continue;
                }
                if (!pDoor->LoadDoorExternal(pRes, &cDoor, bUseTemplates)) {
                    delete pDoor;
                    continue;
                }
                float fBearing = pRes->ReadFieldFLOAT(&cDoor, "Bearing", bSuccess, 0.0f);
                Quaternion qOrientation(Vector(0.0f, 0.0f, 1.0f), fBearing);
                pDoor->m_fBearing = fBearing;
                pDoor->SetOrientation(qOrientation);
                if (bLoadObjectState) {
                    pDoor->LoadObjectState(pRes, &cDoor);
                }
                pDoor->AddToArea(this,
                                 pRes->ReadFieldFLOAT(&cDoor, "X", bSuccess, 0.0f),
                                 pRes->ReadFieldFLOAT(&cDoor, "Y", bSuccess, 0.0f),
                                 pRes->ReadFieldFLOAT(&cDoor, "Z", bSuccess, 0.0f),
                                 !bLoadObjectState);
                if (pDoor->m_bTrapFlag2E8) {
                    m_aTriggers.Add(pDoor->m_idSelf);
                }
            }
        }
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0050A7B0
int CSWSArea::LoadPlaceables(CResGFF* pRes, CResStruct* pStruct, int bLoadObjectState, int bUseTemplates)
{
    CResList cList;
    if (pRes->GetList(&cList, pStruct, "Placeable List")) {
        for (unsigned long i = 0; i < pRes->GetListCount(&cList); i++) {
            CResStruct cPlaceable;
            if (pRes->GetListElement(&cPlaceable, &cList, i) && pRes->GetStructID(&cPlaceable) == 9) {
                int bSuccess;
                unsigned long oid = pRes->ReadFieldDWORD(&cPlaceable, "ObjectId", bSuccess, 0x7f000000);
                CSWSPlaceable* pPlaceable = new CSWSPlaceable(oid);
                if (pPlaceable == NULL) {
                    continue;
                }
                if (bUseTemplates) {
                    CResRef cDefault("");
                    CResRef cTemplate = pRes->ReadFieldCResRef(&cPlaceable, "TemplateResRef", bSuccess, cDefault);
                    if (!bSuccess || !pPlaceable->LoadFromTemplate(cTemplate, 1)) {
                        delete pPlaceable;
                        continue;
                    }
                }
                else if (!pPlaceable->LoadPlaceable(pRes, &cPlaceable, 0)) {
                    delete pPlaceable;
                    continue;
                }
                float fBearing = pRes->ReadFieldFLOAT(&cPlaceable, "Bearing", bSuccess, 0.0f);
                pPlaceable->m_fBearing = fBearing;
                Quaternion qOrientation(Vector(0.0f, 0.0f, 1.0f), fBearing);
                pPlaceable->SetOrientation(qOrientation);
                if (bLoadObjectState) {
                    pPlaceable->LoadObjectState(pRes, &cPlaceable);
                }
                pPlaceable->AddToArea(this,
                                      Vector(pRes->ReadFieldFLOAT(&cPlaceable, "X", bSuccess, 0.0f),
                                             pRes->ReadFieldFLOAT(&cPlaceable, "Y", bSuccess, 0.0f),
                                             pRes->ReadFieldFLOAT(&cPlaceable, "Z", bSuccess, 0.0f)),
                                      bLoadObjectState);
                if (pPlaceable->m_bTrapFlag278) {
                    m_aTriggers.Add(pPlaceable->m_idSelf);
                }
            }
        }
    }
    return 1;
}
