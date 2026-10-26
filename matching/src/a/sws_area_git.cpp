// CSWSArea area/GIT loading drivers (0x0050DD80, 0x0050E190).
#include "a/sws_area.h"

// FUNCTION: SWKOTOR 0x0050DD80
int CSWSArea::LoadGIT(int bLoadObjectState)
{
    if (!g_pExoResMan->Exists(m_cResRef, 0x7e7, NULL)) {
        return 0;
    }
    CResGFF* pGIT = new CResGFF(0x7e7, "GIT ", m_cResRef);
    if (!pGIT->m_bLoaded94) {
        delete pGIT;
        m_pGIT->Release();
        return 0;
    }
    CResStruct cTop;
    pGIT->GetTopLevelStruct(&cTop);
    int bSuccess;
    if (bLoadObjectState) {
        m_cScriptVarTable.LoadVarTable(pGIT, &cTop);
        m_cVarTable.LoadVarTable(pGIT, &cTop);
        m_nCurrentWeather = pGIT->ReadFieldBYTE(&cTop, "CurrentWeather", bSuccess, 0);
        m_bWeatherStarted = pGIT->ReadFieldBYTE(&cTop, "WeatherStarted", bSuccess, 0);
        if (m_nAreaFlags & 1) {
            m_nCurrentWeather = 0xff;
            m_bWeatherStarted = 0;
        }
    }
    int bUseTemplates = pGIT->ReadFieldBYTE(&cTop, "UseTemplates", bSuccess, 0);
    LoadCreatures(pGIT, &cTop, bLoadObjectState, bUseTemplates);
    LoadItems(pGIT, &cTop, bLoadObjectState, bUseTemplates);
    LoadDoors(pGIT, &cTop, bLoadObjectState, bUseTemplates);
    LoadTriggers(pGIT, &cTop, bLoadObjectState, bUseTemplates);
    LoadEncounters(pGIT, &cTop, bLoadObjectState, bUseTemplates);
    LoadWaypoints(pGIT, &cTop, bLoadObjectState, bUseTemplates);
    LoadSounds(pGIT, &cTop, bLoadObjectState, bUseTemplates);
    LoadPlaceables(pGIT, &cTop, bLoadObjectState, bUseTemplates);
    LoadStores(pGIT, &cTop, bLoadObjectState, bUseTemplates);
    LoadAreaEffects(pGIT, &cTop, bLoadObjectState, bUseTemplates);
    LoadProperties(pGIT, &cTop);
    LoadMaps(pGIT, &cTop);
    LoadPlaceableCameras(pGIT, &cTop);
    delete pGIT;
    return 1;
}

// FUNCTION: SWKOTOR 0x0050E190
int CSWSArea::LoadArea(int bLoadObjectState)
{
    if (!m_pGIT->Demand()) {
        return 0;
    }
    CResStruct cTop;
    m_pGIT->GetTopLevelStruct(&cTop);
    LoadAreaHeader(&cTop);
    LoadRoomInfo(&cTop);
    LoadGIT(bLoadObjectState);
    LoadPathPoints(&cTop);
    m_pGIT->Release();
    GetServerExoApp()->GetModule()->AddObjectToLookupTable(m_sTag, m_idSelf);
    m_nMapGrid223 = (unsigned char)ceil(m_cValue8.m_n4 * m_cValue8.m_n0 / 8.0f);
    return 1;
}

// FUNCTION: SWKOTOR 0x00508A30
int CSWSArea::AIUpdate()
{
    unsigned long nDay;
    unsigned long nTime;
    unsigned long nDiffDays;
    unsigned long nDiffTime;
    GetServerExoApp()->GetWorldTimer()->GetWorldTime(&nDay, &nTime);
    GetServerExoApp()->GetWorldTimer()->SubtractWorldTimes(nDay, nTime, m_nLastWeatherDay, m_nLastWeatherTime,
                                                           &nDiffDays, &nDiffTime);
    m_nLastWeatherDay = nDay;
    m_nLastWeatherTime = nTime;
    if (m_nWeatherType == 0xff && !(m_nAreaFlags & 1)) {
        if (m_nWeatherTimer == 0) {
            int nRandom = rand() % 100;
            int bChanged = 0;
            if (m_nChanceRain > 0) {
                if (nRandom < m_nChanceRain) {
                    if (!m_bWeatherStarted) {
                        m_nCurrentWeather = 1;
                        m_bWeatherStarted = 1;
                        bChanged = 1;
                    }
                }
                else if (m_bWeatherStarted) {
                    m_nCurrentWeather = 1;
                    m_bWeatherStarted = 0;
                    bChanged = 1;
                }
            }
            else if (m_nChanceSnow > 0) {
                if (nRandom < m_nChanceSnow) {
                    if (!m_bWeatherStarted) {
                        m_nCurrentWeather = 2;
                        m_bWeatherStarted = 1;
                        bChanged = 1;
                    }
                }
                else if (m_bWeatherStarted) {
                    m_nCurrentWeather = 2;
                    m_bWeatherStarted = 0;
                    bChanged = 1;
                }
            }
            if (bChanged) {
                SetCurrentWeather(m_nCurrentWeather, m_bWeatherStarted);
            }
            m_nWeatherTimer = 300000;
        }
        else if (m_nWeatherTimer <= nDiffTime) {
            m_nWeatherTimer = 0;
        }
        else {
            m_nWeatherTimer -= nDiffTime;
        }
    }
    GetServerExoApp()->GetWorldTimer()->SubtractWorldTimes(nDay, nTime, m_nLastHeartbeatDay,
                                                           m_nLastHeartbeatTime, &nDiffDays, &nDiffTime);
    if (nDiffTime >= 6000) {
        if (m_nLastHeartbeatTime) {
            g_pVirtualMachine->RunScript(&m_sScriptOnHeartbeat, m_idSelf, 1);
        }
        m_nLastHeartbeatDay = nDay;
        m_nLastHeartbeatTime = nTime;
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00508400
int CSWSArea::LoadPathPoints(CResStruct* pStruct)
{
    int bResult = 1;
    if (g_pExoResMan->Exists(m_cResRef, 0xbbb, NULL)) {
        CResGFF* pPath = new CResGFF(0xbbb, "PTH ", m_cResRef);
        if (!pPath->m_bLoaded94) {
            delete pPath;
            return 0;
        }
        CResStruct cTop;
        CResList cList;
        CResStruct cElement;
        int bSuccess;
        pPath->GetTopLevelStruct(&cTop);
        if (pPath->GetList(&cList, &cTop, "Path_Points")) {
            m_nPathPoints = pPath->GetListCount(&cList);
            if (m_nPathPoints == 0) {
                delete pPath;
                return 0;
            }
            m_pPathPoints = new CSWSPathPoint[m_nPathPoints];
            for (unsigned long i = 0; i < m_nPathPoints; i++) {
                if (pPath->GetListElement(&cElement, &cList, i) && pPath->GetStructID(&cElement) == 2) {
                    m_pPathPoints[i].m_fX = pPath->ReadFieldFLOAT(&cElement, "X", bSuccess, 0.0f);
                    bResult &= bSuccess;
                    m_pPathPoints[i].m_fY = pPath->ReadFieldFLOAT(&cElement, "Y", bSuccess, 0.0f);
                    bResult &= bSuccess;
                    m_pPathPoints[i].m_nConnections =
                        pPath->ReadFieldDWORD(&cElement, "Conections", bSuccess, 0);
                    bResult &= bSuccess;
                    m_pPathPoints[i].m_nFirstConnection =
                        pPath->ReadFieldDWORD(&cElement, "First_Conection", bSuccess, 0);
                    bResult &= bSuccess;
                }
            }
        }
        if (pPath->GetList(&cList, &cTop, "Path_Conections")) {
            m_nPathConnections = pPath->GetListCount(&cList);
            m_pPathConnections = new unsigned long[m_nPathConnections];
            for (unsigned long i = 0; i < m_nPathConnections; i++) {
                if (pPath->GetListElement(&cElement, &cList, i) && pPath->GetStructID(&cElement) == 3) {
                    m_pPathConnections[i] = pPath->ReadFieldDWORD(&cElement, "Destination", bSuccess, 0);
                    bResult &= bSuccess;
                }
            }
        }
        delete pPath;
    }
    return bResult;
}
