// NewController key evaluation / reporting and the MdlNode
// controller dispatch (0x00482BF0 .. 0x00485660).

#include <math.h>
#include "e/mdlanim.h"

extern float g_fPositionScale;      // 0x00798B38
extern int g_bReportKeys;           // 0x008302CC
extern float g_fAnimationLength;    // 0x008302D0
extern unsigned char g_bNoKeyReports;   // 0x008302C8
extern float g_fAnimationTime;      // 0x0078E574

static float s_fControlOld;         // 0x00830330
static float s_fControlNew;         // 0x00830334
static float s_fControlPrev;        // 0x0083032C

// FUNCTION: SWKOTOR 0x00482BF0
int NewController::GetFloatValue(float fTime, MdlNode* pNode, float* pOut)
{
    if (m_nColumns != 1)
        return 0;
    int nIndex;
    float fFrac;
    GetKeyIndex(fTime, pNode, &nIndex, &fFrac);
    if (nIndex == 0) {
        *pOut = pNode->m_lControllerData.element[m_nDataIndex];
        return 1;
    }
    if (nIndex == m_nNumKeys) {
        if (m_nFlags == 1)
            *pOut = pNode->m_lControllerData.element[m_nDataIndex + nIndex * 3 - 3];
        else
            *pOut = pNode->m_lControllerData.element[m_nDataIndex + m_nNumKeys - 1];
        return 1;
    }
    if (m_nFlags == 1) {
        int k = m_nDataIndex + nIndex * 3 - 3;
        *pOut = BezierInterpolate(pNode->m_lControllerData.element[k],
                                  pNode->m_lControllerData.element[k + 2],
                                  pNode->m_lControllerData.element[k + 3] + pNode->m_lControllerData.element[k],
                                  pNode->m_lControllerData.element[k + 4] + pNode->m_lControllerData.element[k + 2],
                                  fFrac);
    }
    else {
        *pOut = (1.0f - fFrac) * pNode->m_lControllerData.element[m_nDataIndex + nIndex - 1]
                + fFrac * pNode->m_lControllerData.element[m_nDataIndex + nIndex];
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00482D20
int NewController::GetVectorValue(float fTime, MdlNode* pNode, Vector* pOut)
{
    if (m_nColumns != 3)
        return 0;
    int nIndex;
    float fFrac;
    GetKeyIndex(fTime, pNode, &nIndex, &fFrac);
    static Vector s_vKey0(0.0f, 0.0f, 0.0f);
    static Vector s_vKey1(0.0f, 0.0f, 0.0f);
    float fScale = 1.0f;
    if (m_nType == 8)
        fScale = g_fPositionScale;

    int k;
    if (nIndex == 0) {
        k = m_nDataIndex;
        goto copy;
    }
    if (m_nFlags == 1) {
        if (nIndex == m_nNumKeys) {
            k = m_nDataIndex + nIndex * 9 - 9;
        copy:
            pOut->x = pNode->m_lControllerData.element[k++];
            pOut->y = pNode->m_lControllerData.element[k++];
            pOut->z = pNode->m_lControllerData.element[k];
            *pOut = *pOut * fScale;
            return 1;
        }
        k = m_nDataIndex + nIndex * 9 - 9;
        s_vKey0.x = pNode->m_lControllerData.element[k];
        s_vKey0.y = pNode->m_lControllerData.element[k + 1];
        s_vKey0.z = pNode->m_lControllerData.element[k + 2];
        s_vKey1.x = pNode->m_lControllerData.element[k + 9];
        s_vKey1.y = pNode->m_lControllerData.element[k + 10];
        s_vKey1.z = pNode->m_lControllerData.element[k + 11];
        float* pData = pNode->m_lControllerData.element;
        Vector vOut(pData[k + 6], pData[k + 7], pData[k + 8]);
        Vector vIn(pData[k + 12], pData[k + 13], pData[k + 14]);
        *pOut = BezierInterpolate(s_vKey0, s_vKey1, s_vKey0 + vOut, s_vKey1 + vIn, fFrac) * fScale;
        return 1;
    }
    if (nIndex == m_nNumKeys) {
        k = m_nDataIndex + nIndex * 3 - 3;
        goto copy;
    }
    k = m_nDataIndex + nIndex * 3 - 3;
    s_vKey0.x = pNode->m_lControllerData.element[k++];
    s_vKey0.y = pNode->m_lControllerData.element[k++];
    s_vKey0.z = pNode->m_lControllerData.element[k++];
    s_vKey1.x = pNode->m_lControllerData.element[k++];
    s_vKey1.y = pNode->m_lControllerData.element[k++];
    s_vKey1.z = pNode->m_lControllerData.element[k];
    *pOut = Lerp(s_vKey0, s_vKey1, fFrac) * fScale;
    return 1;
}

// FUNCTION: SWKOTOR 0x00483050
void NewController::GetQuaternionFromIndexLoc(int nIndex, MdlNode* pNode, Quaternion* pOut)
{
    CExoArrayList<float>& lData = pNode->m_lControllerData;
    int nColumns = m_nColumns;
    if (nColumns == 4) {
        int k = nIndex - 1 < 0 ? 0 : nIndex - 1;
        k = m_nDataIndex + k * 4;
        pOut->x = lData.element[k++];
        pOut->y = lData.element[k++];
        pOut->z = lData.element[k++];
        pOut->w = lData.element[k];
    }
    else if (nColumns == 2) {
        int k = nIndex - 1 < 0 ? 0 : nIndex - 1;
        unsigned long nPacked = *(unsigned long*)&lData.element[m_nDataIndex + k];
        float x = (int)(nPacked & 0x7ff) * (1.0f / 1023.0f) - 1.0f;
        pOut->x = x;
        float y = (int)((nPacked >> 11) & 0x7ff) * (1.0f / 1023.0f) - 1.0f;
        pOut->y = y;
        float z = (int)(nPacked >> 22) * (1.0f / 511.0f) - 1.0f;
        pOut->z = z;
        float fMag2 = x * x + y * y + z * z;
        if (fMag2 <= 1.0f) {
            pOut->w = (float)sqrt(1.0f - fMag2);
        }
        else {
            float fInv = 1.0f / (float)sqrt(fMag2);
            pOut->w = 0.0f;
            pOut->x = x * fInv;
            pOut->y = y * fInv;
            pOut->z = z * fInv;
        }
    }
}

// FUNCTION: SWKOTOR 0x004831B0
int NewController::GetQuaternionValue(float fTime, MdlNode* pNode, Quaternion* pOut)
{
    int nColumns = m_nColumns;
    if (nColumns != 4 && nColumns != 2)
        return 0;
    int nIndex;
    float fFrac;
    GetKeyIndex(fTime, pNode, &nIndex, &fFrac);
    static Quaternion s_qKey0;
    static Quaternion s_qKey1;
    if (nIndex == 0) {
        GetQuaternionFromIndexLoc(nIndex, pNode, pOut);
        return 1;
    }
    if (nIndex == m_nNumKeys) {
        GetQuaternionFromIndexLoc(m_nNumKeys, pNode, pOut);
        return 1;
    }
    GetQuaternionFromIndexLoc(nIndex, pNode, &s_qKey0);
    GetQuaternionFromIndexLoc(nIndex + 1, pNode, &s_qKey1);
    Quaternion q = Slerp(s_qKey0, s_qKey1, fFrac);
    *pOut = q;
    return 1;
}

// FUNCTION: SWKOTOR 0x00483330
void NewController::Control(Part* pPart, MdlNode* pNode, float fPrevTime, float fTime, float fBlend)
{
    int nColumns = m_nColumns;
    if (nColumns == 1) {
        float* pValue = (float*)((char*)pPart + m_nType);
        s_fControlOld = *pValue;
        GetFloatValue(fTime, pNode, &s_fControlNew);
        if (m_nAddOffset != -1)
            s_fControlNew += *(float*)(pPart->m_pBase + m_nAddOffset);
        if (fPrevTime < -95.0f) {
            *pValue = (1.0f - fBlend) * s_fControlOld + s_fControlNew * fBlend;
            return;
        }
        GetFloatValue(fPrevTime, pNode, &s_fControlPrev);
        *pValue = (1.0f - fBlend) * s_fControlPrev + s_fControlNew * fBlend;
    }
    else if (nColumns == 3) {
        Vector vPrev(0.0f, 0.0f, 0.0f);
        Vector vNew(0.0f, 0.0f, 0.0f);
        Vector* pValue = (Vector*)((char*)pPart + m_nType);
        Vector vOld = *pValue;
        GetVectorValue(fTime, pNode, &vNew);
        Vector* pAdd;
        if (m_nAddOffset != -1) {
            pAdd = (Vector*)(pPart->m_pBase + m_nAddOffset);
            vNew += *pAdd;
        }
        if (fPrevTime < -95.0f) {
            *pValue = Lerp(vOld, vNew, fBlend);
            return;
        }
        GetVectorValue(fPrevTime, pNode, &vPrev);
        if (m_nAddOffset != -1)
            vPrev += *pAdd;
        *pValue = Lerp(vPrev, vNew, fBlend);
    }
    else if (nColumns == 4 || nColumns == 2) {
        Quaternion qNew;
        Quaternion* pValue = (Quaternion*)((char*)pPart + m_nType);
        Quaternion qOld = *pValue;
        Quaternion qPrev;
        GetQuaternionValue(fTime, pNode, &qNew);
        if (fPrevTime < -95.0f) {
            *pValue = *Interpolate(&qPrev, &qOld, &qNew, fBlend);
            return;
        }
        GetQuaternionValue(fPrevTime, pNode, &qPrev);
        *pValue = *Interpolate(&qOld, &qPrev, &qNew, fBlend);
    }
}

// FUNCTION: SWKOTOR 0x00484EC0
void NewController::ReportKey(Part* pPart, MdlNode* pNode, int nKey, float fDelta)
{
    ReportKeyEvent event;
    event.fTime = fDelta;
    Gob* pGob = pPart->m_pGob;
    float fTime = pNode->m_lControllerData.element[m_nTimeIndex + nKey];
    int nColumns = m_nColumns;
    if (nColumns == 1)
        return;
    if (nColumns == 3) {
        Vector vPosition(0.0f, 0.0f, 0.0f);
        GetVectorValue(fTime, pNode, &vPosition);
        event.pPosition = &vPosition;
        event.bPosition = 1;
        Quaternion qOrientation;
        int bFound = 0;
        for (int i = 0; i < pNode->m_lControllers.num; i++) {
            if (&pNode->m_lControllers.element[i] != this && pNode->m_lControllers.element[i].m_nType == 0x14
                && (bFound = pNode->m_lControllers.element[i].GetQuaternionValue(fTime, pNode, &qOrientation)) != 0)
                break;
        }
        if (!bFound)
            qOrientation = pPart->m_qOrientation;
        event.pOrientation = &qOrientation;
        pGob->DoEvent("ReportKey", &event);
    }
    else if (nColumns == 4 || nColumns == 2) {
        Quaternion qOrientation;
        GetQuaternionValue(fTime, pNode, &qOrientation);
        event.pOrientation = &qOrientation;
        event.bPosition = 0;
        Vector vPosition(0.0f, 0.0f, 0.0f);
        for (int i = 0; i < pNode->m_lControllers.num; i++) {
            NewController* pController = &pNode->m_lControllers.element[i];
            if (pController != this && pController->m_nType == 8
                && pController->GetVectorValue(fTime, pNode, &vPosition)) {
                event.pPosition = &vPosition;
                break;
            }
        }
        pGob->DoEvent("ReportKey", &event);
    }
}

// FUNCTION: SWKOTOR 0x00485250
void NewController::ReportKeys(Part* pPart, MdlNode* pNode, float fStart, float fEnd)
{
    if (!pPart->m_pGob)
        return;
    if (fStart < 0.0f)
        fStart = 0.0f;
    if (fEnd > g_fAnimationLength)
        fEnd = g_fAnimationLength;
    int nTimeIndex = m_nTimeIndex;
    int nNumKeys = m_nNumKeys;
    ReportKeyEvent event;
    float fTotal = 0.0f;
    int nFirst;
    int nLast;
    for (nFirst = 0; nFirst < m_nNumKeys; nFirst++) {
        if (!(fStart > pNode->m_lControllerData.element[nTimeIndex + nFirst]))
            break;
    }
    for (nLast = 0; nLast < m_nNumKeys; nLast++) {
        if (!(fEnd > pNode->m_lControllerData.element[nTimeIndex + nLast]))
            break;
    }
    if (g_bNoKeyReports & 1)
        return;
    int i;
    float fDelta;
    if (fStart < fEnd) {
        for (i = nFirst; i < nLast; i++) {
            if (i == nFirst)
                fDelta = pNode->m_lControllerData.element[nTimeIndex + i] - fStart;
            else
                fDelta = pNode->m_lControllerData.element[nTimeIndex + i]
                         - pNode->m_lControllerData.element[nTimeIndex + i - 1];
            ReportKey(pPart, pNode, i, fDelta);
            fTotal += fDelta;
        }
    }
    else {
        float fWrap;
        if (nFirst == nNumKeys)
            fWrap = g_fAnimationLength - fStart;
        else
            fWrap = g_fAnimationLength - pNode->m_lControllerData.element[nTimeIndex + nNumKeys - 1];
        for (i = nFirst; i < m_nNumKeys; i++) {
            if (i == nFirst && i < nNumKeys)
                fDelta = pNode->m_lControllerData.element[i] - fStart;
            else
                fDelta = pNode->m_lControllerData.element[nTimeIndex + i]
                         - pNode->m_lControllerData.element[nTimeIndex + i - 1];
            ReportKey(pPart, pNode, i, fDelta);
            fTotal += fDelta;
        }
        for (i = 0; i < nLast; i++) {
            if (i == 0)
                fDelta = fWrap + pNode->m_lControllerData.element[nTimeIndex];
            else
                fDelta = pNode->m_lControllerData.element[nTimeIndex + i]
                         - pNode->m_lControllerData.element[nTimeIndex + i - 1];
            ReportKey(pPart, pNode, i, fDelta);
            fTotal += fDelta;
        }
    }
    event.fTime = g_fAnimationTime - fTotal;
    event.pOrientation = 0;
    event.pPosition = 0;
    event.bPosition = m_nColumns == 3;
    pPart->m_pGob->DoEvent("ReportKey", &event);
}

// FUNCTION: SWKOTOR 0x004854D0
void MdlNode::InternalControl(Part* pPart, float fPrevTime, float fTime, float fBlend)
{
    pPart->UpdateTransform();
    for (int i = 0; i < m_lControllers.num; i++) {
        m_lControllers.element[i].Control(pPart, this, fPrevTime, fTime, fBlend);
        if (g_bReportKeys) {
            if (fPrevTime < -95.0f)
                m_lControllers.element[i].ReportKeys(pPart, this, -100.0f - fPrevTime, fTime);
            else
                m_lControllers.element[i].ReportKeys(pPart, this, fPrevTime, fTime);
        }
    }
}
