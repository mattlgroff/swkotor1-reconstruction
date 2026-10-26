// CSWSArea GIT/ARE property, explored-map and placeable-camera
// serialisation (0x00505DA0-0x005063BF).
#include "a/sws_area.h"

// FUNCTION: SWKOTOR 0x00505DA0
int CSWSArea::LoadMaps(CResGFF* pRes, CResStruct* pStruct)
{
    int bReturn = 1;
    CResStruct cAreaMap;
    if (pRes->GetStructFromStruct(&cAreaMap, pStruct, "AreaMap")) {
        int bSuccess;
        int nWidth = pRes->ReadFieldINT(&cAreaMap, "AreaMapResX", bSuccess, 0);
        int nHeight = pRes->ReadFieldINT(&cAreaMap, "AreaMapResY", bSuccess, 0);
        unsigned long nSize = pRes->ReadFieldDWORD(&cAreaMap, "AreaMapDataSize", bSuccess, 0);
        unsigned long* pData = new unsigned long[nSize];
        pRes->ReadFieldVOID(&cAreaMap, pData, nSize, "AreaMapData", bSuccess, NULL);
        if (bSuccess) {
            CSWSModule* pModule = GetServerExoApp()->GetModule();
            if (pModule->m_pAreaMap != NULL) {
                pModule->m_pAreaMap->SetExploredData(pData, nSize / 4, nWidth, nHeight);
            }
        }
        else {
            bReturn = 0;
        }
        if (pData != NULL) {
            delete[] pData;
        }
        return bReturn;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00505EB0
int CSWSArea::LoadPlaceableCameras(CResGFF* pRes, CResStruct* pStruct)
{
    int bSuccess = 1;
    Vector vDefault;
    vDefault.x = 0.0f;
    vDefault.y = 0.0f;
    vDefault.z = 0.0f;
    Quaternion qDefault;
    qDefault.z = 0.0f;
    qDefault.y = 0.0f;
    qDefault.x = 0.0f;
    qDefault.w = 1.0f;

    CResList cList;
    pRes->GetList(&cList, pStruct, "CameraList");
    int nCount = pRes->GetListCount(&cList);
    if (nCount > 50) {
        return 0;
    }
    g_pAppManager->m_pClientExoApp->GetInGameGui()->SetPlaceableCameraCount(nCount);
    for (int i = 0; i < nCount; i++) {
        CResStruct cCamera;
        pRes->GetListElement(&cCamera, &cList, i);
        int nCameraID = pRes->ReadFieldINT(&cCamera, "CameraID", bSuccess, -1);
        Vector vPosition = pRes->ReadFieldVector(&cCamera, "Position", bSuccess, vDefault);
        Quaternion qOrientation = pRes->ReadFieldQuaternion(&cCamera, "Orientation", bSuccess, qDefault);
        float fPitch = pRes->ReadFieldFLOAT(&cCamera, "Pitch", bSuccess, 0.0f);
        float fHeight = pRes->ReadFieldFLOAT(&cCamera, "Height", bSuccess, 0.0f);
        float fFieldOfView = pRes->ReadFieldFLOAT(&cCamera, "FieldOfView", bSuccess, 55.0f);
        float fMicRange = pRes->ReadFieldFLOAT(&cCamera, "MicRange", bSuccess, 0.0f);
        CClientExoApp* pClient = g_pAppManager->m_pClientExoApp;
        pClient->GetInGameGui()->SetPlaceableCamera(
            i, nCameraID, &vPosition, &qOrientation, fPitch, fHeight, fFieldOfView, fMicRange);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00506090
void CSWSArea::SaveProperties(CResGFF* pRes, CResStruct* pStruct)
{
    CResStruct cProperties;
    pRes->AddStructToStruct(&cProperties, pStruct, "AreaProperties", 0x64);
    m_pAmbientSound->Save(pRes, &cProperties);
    pRes->WriteFieldBYTE(&cProperties, (unsigned char)m_bUnescapable, "Unescapable");
    pRes->WriteFieldBYTE(&cProperties, (unsigned char)m_nRestrictMode, "RestrictMode");
    pRes->WriteFieldDWORD(&cProperties, m_nStealthXPMax, "StealthXPMax");
    pRes->WriteFieldDWORD(&cProperties, m_nStealthXPCurrent, "StealthXPCurrent");
    pRes->WriteFieldDWORD(&cProperties, m_nStealthXPLoss, "StealthXPLoss");
    pRes->WriteFieldBYTE(&cProperties, (unsigned char)m_bStealthXPEnabled, "StealthXPEnabled");
    pRes->WriteFieldBYTE(&cProperties, (unsigned char)m_bTransPending, "TransPending");
    pRes->WriteFieldBYTE(&cProperties, m_nTransPendNextID, "TransPendNextID");
    pRes->WriteFieldBYTE(&cProperties, m_nTransPendCurrID, "TransPendCurrID");
    pRes->WriteFieldDWORD(&cProperties, m_nSunFogColor, "SunFogColor");
}

// FUNCTION: SWKOTOR 0x005062A0
void CSWSArea::SavePlaceableCameras(CResGFF* pRes, CResStruct* pStruct)
{
    int i;
    CGuiInGame* pGui = g_pAppManager->m_pClientExoApp->GetInGameGui();
    CResList cList;
    CResStruct cCamera;
    pRes->AddList(&cList, pStruct, "CameraList");
    for (i = 0; i < pGui->m_nPlaceableCameras; i++) {
        CSWPlaceableCamera* pCamera = pGui->GetPlaceableCamera(i);
        if (pCamera != NULL) {
            pRes->AddListElement(&cCamera, &cList, 0xe);
            pRes->WriteFieldINT(&cCamera, pCamera->m_nCameraID, "CameraID");
            pRes->WriteFieldVector(&cCamera, pCamera->m_vPosition, "Position");
            pRes->WriteFieldQuaternion(&cCamera, pCamera->m_qOrientation, "Orientation");
            pRes->WriteFieldFLOAT(&cCamera, pCamera->m_fPitch, "Pitch");
            pRes->WriteFieldFLOAT(&cCamera, pCamera->m_fHeight, "Height");
            pRes->WriteFieldFLOAT(&cCamera, pCamera->m_fFieldOfView, "FieldOfView");
            pRes->WriteFieldFLOAT(&cCamera, pCamera->m_fMicRange, "MicRange");
        }
    }
}
