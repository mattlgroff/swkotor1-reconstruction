// CSWSDoor / CSWSPlaceable / CSWSTrigger orientation, opening and bounds
// (round s8, 0x00584C20..0x0058EAE0).

#include <math.h>

class Vector {
public:
    Vector() {}
    Vector(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }
    Vector operator-(const Vector &v) const { Vector r; r.x = x - v.x; r.y = y - v.y; r.z = z - v.z; return r; }
    int IsZero() const { return x == 0.0f && y == 0.0f && z == 0.0f; }
    float MagnitudeSquared() const { return x * x + y * y + z * z; }
    float x, y, z;
};

class Quaternion {
public:
    float w, x, y, z;
};

Vector Normalize(const Vector &v);

class CExoString {
public:
    CExoString();
    ~CExoString();
    char *m_sString;
    unsigned long m_nBufferLength;
};

class CSWSObject;
class CSWSDoor;

class CGameObject {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual CSWSObject *AsSWSObject();         // +0x10
    virtual void v14();
    virtual CSWSDoor *AsSWSDoor();             // +0x18

    unsigned long m_idSelf;          // +0x4
    unsigned char m_nObjectType;     // +0x8
    char pad_0x9[0x3];
};

class CServerExoApp {
public:
    CGameObject *GetGameObject(unsigned long id);
};

class CAppManager {
public:
    char pad_0x0[0x8];
    CServerExoApp *m_pServerExoApp;  // +0x8
    CServerExoApp *GetServerExoApp() { return m_pServerExoApp; }
};

extern CAppManager *g_pAppManager;

class CVirtualMachine {
public:
    int RunScript(CExoString *psFileName, unsigned long oid, int bFlag);
};

extern CVirtualMachine *g_pVirtualMachine;

class CSWSArea {
public:
    int PositionWalkable(Vector vPosition);
};

class CSWSObject : public CGameObject {
public:
    void SetOrientation(const Vector &v);
    CSWSArea *GetArea();

    char pad_0xc[0x84];
    Vector m_vPosition;              // +0x90
    Vector m_vOrientation;           // +0x9c
};

class CSWCollisionMesh {
public:
    Vector GetWorldPoint(const Vector &v);
    Vector GetUsePoint(int n) { return GetWorldPoint(m_avUse[n]); }
    int GetMeshBoundingBox(Vector &vMin, Vector &vMax);
    void RenderDebug(int bFill, unsigned long nColor1, unsigned long nColor2);

    char pad_0x0[0x20];
    Vector m_vPosition;              // +0x20
    char pad_0x2c[0xc];
    Quaternion m_qOrientation;       // +0x38
    void SetOrientation(Quaternion q) { m_qOrientation = q; }
    char pad_0x48[0x28];
    Vector m_avUse[2];               // +0x70
};

extern int g_nDoorStateClosed;
extern int g_nDoorStateOpen1;
extern int g_nDoorStateOpen2;
extern int g_nDoorDebugMode;

class CSWSDoor : public CSWSObject {
public:
    void SetOpenState(unsigned char nState, int bNextOnly);
    CSWSObject *GetLinkedObject();
    void OpenDoor(unsigned long oidOpener);
    void SetOrientation(Quaternion q);
    int InDoorArea(Vector v);
    Vector GetActionPoint(int nPoint, unsigned char nIndex);
    Vector GetNearestActionPoint(const Vector &vFrom, int bClosedOnly);
    int CheckPlacement(Vector vPosition, Vector vUnused, Vector *pMin, Vector *pMax);

    char pad_0xa8[0x2c];
    int m_nAnimation;                // +0xd4
    char pad_0xd8[0x150];
    CExoString m_sScripts[15];       // +0x228
    char pad_0x2a0[0x7c];
    unsigned long m_oidLastOpened;   // +0x31c
    char pad_0x320[0xc];
    CSWCollisionMesh *m_apWalkMeshes[3]; // +0x32c
    char pad_0x338[0x18];
    Vector m_avAreaPoints[4];        // +0x350
    int m_nAreaPoints;               // +0x380
    char pad_0x384[0x2c];
    Quaternion m_qOrientation;       // +0x3b0
};

// FUNCTION: SWKOTOR 0x00589C70
void CSWSDoor::OpenDoor(unsigned long oidOpener)
{
    int nSide = 2;
    CGameObject *pOpener = g_pAppManager->m_pServerExoApp->GetGameObject(oidOpener);
    if (pOpener) {
        Vector &vPos = pOpener->AsSWSObject()->m_vPosition;
        Vector vDiff(vPos.x - m_vPosition.x, vPos.y - m_vPosition.y, vPos.z - m_vPosition.z);
        if (vDiff.x * m_vOrientation.x + vDiff.y * m_vOrientation.y + vDiff.z * m_vOrientation.z > 0.0f)
            nSide = 1;
    }
    m_oidLastOpened = oidOpener;
    SetOpenState(nSide, 1);
    CSWSObject *pLinked = GetLinkedObject();
    if (pLinked && pLinked->AsSWSDoor())
        pLinked->AsSWSDoor()->SetOpenState(nSide, 1);
    g_pVirtualMachine->RunScript(&m_sScripts[0], m_idSelf, 1);
}

// FUNCTION: SWKOTOR 0x005897D0
int CSWSDoor::InDoorArea(Vector v)
{
    int nCrossings = 0;
    for (int i = 0; i < m_nAreaPoints; i++) {
        int nLast = m_nAreaPoints - 1;
        int j;
        if (i != nLast)
            j = i + 1;
        else
            j = 0;
        if (m_avAreaPoints[j].x != m_avAreaPoints[i].x) {
            float fY1 = m_avAreaPoints[i].y;
            float fDY = m_avAreaPoints[j].y - fY1;
            float fDX = m_avAreaPoints[j].x - m_avAreaPoints[i].x;
            float fY = fY1 - (fDY / fDX) * (m_avAreaPoints[i].x - v.x);
            if (fY > v.y) {
                if (fabs(fDY) > fabs(fDX)) {
                    if (m_avAreaPoints[j].y > m_avAreaPoints[i].y) {
                        if (fY >= m_avAreaPoints[i].y && fY < m_avAreaPoints[j].y)
                            nCrossings++;
                    } else if (m_avAreaPoints[i].y > m_avAreaPoints[j].y && fY > m_avAreaPoints[j].y && fY <= m_avAreaPoints[i].y)
                        nCrossings++;
                } else {
                    if (m_avAreaPoints[j].x > m_avAreaPoints[i].x) {
                        if (v.x >= m_avAreaPoints[i].x && v.x < m_avAreaPoints[j].x)
                            nCrossings++;
                    } else if (m_avAreaPoints[i].x > m_avAreaPoints[j].x && v.x > m_avAreaPoints[j].x && v.x <= m_avAreaPoints[i].x)
                        nCrossings++;
                }
            }
        }
        if (m_avAreaPoints[j].x == v.x && (m_avAreaPoints[j].y > v.y || m_avAreaPoints[i].y >= v.y))
            nCrossings++;
    }
    return nCrossings % 2 == 1;
}

// NONMATCH (82): logic and calls line up; the original spills each
// difference vector to memory (fst) before squaring, cl here keeps it on x87.
// FUNCTION: SWKOTOR 0x00589240
Vector CSWSDoor::GetNearestActionPoint(const Vector &vFrom, int bClosedOnly)
{
    int nState = m_nAnimation;
    Vector vBest(0.0f, 0.0f, 0.0f);
    float fBest = 1000.0f;
    CSWSArea *pArea = GetArea();
    if (bClosedOnly)
        nState = 0x2726;
    Vector vPoint0 = GetActionPoint(nState, 0);
    Vector vPoint1 = GetActionPoint(nState, 1);
    float fDist0 = (vFrom - vPoint0).MagnitudeSquared();
    float fDist1 = (vFrom - vPoint1).MagnitudeSquared();
    if (fDist0 < fDist1 && pArea && pArea->PositionWalkable(vPoint0)) {
        vBest = vPoint0;
        fBest = fDist0;
    } else if (pArea && pArea->PositionWalkable(vPoint1)) {
        vBest = vPoint1;
        fBest = fDist1;
    }
    if (nState != 0x2726) {
        Vector vClosed0 = m_apWalkMeshes[g_nDoorStateClosed]->GetWorldPoint(m_apWalkMeshes[g_nDoorStateClosed]->m_avUse[0]);
        Vector vClosed1 = m_apWalkMeshes[g_nDoorStateClosed]->GetWorldPoint(m_apWalkMeshes[g_nDoorStateClosed]->m_avUse[1]);
        float fClosed0 = (vFrom - vClosed0).MagnitudeSquared();
        float fClosed1 = (vFrom - vClosed1).MagnitudeSquared();
        if (fClosed0 < fBest && pArea && pArea->PositionWalkable(vClosed0))
            vBest = vClosed0;
        else if (fClosed1 < fBest && pArea && pArea->PositionWalkable(vClosed1))
            vBest = vClosed1;
    }
    return vBest;
}

// FUNCTION: SWKOTOR 0x00589980
int CSWSDoor::CheckPlacement(Vector vPosition, Vector vUnused, Vector *pMin, Vector *pMax)
{
    Vector vMin(0.0f, 0.0f, 0.0f);
    Vector vMax(0.0f, 0.0f, 0.0f);
    int bResult = 1;
    for (int i = g_nDoorStateClosed; i <= g_nDoorStateOpen2; i++) {
        m_apWalkMeshes[i]->m_vPosition = vPosition;
        m_apWalkMeshes[i]->SetOrientation(m_qOrientation);
        bResult &= m_apWalkMeshes[i]->GetMeshBoundingBox(vMin, vMax);
        if (g_nDoorDebugMode) {
            if (i == 0) {
                Vector v1 = m_apWalkMeshes[g_nDoorStateClosed]->GetWorldPoint(m_apWalkMeshes[g_nDoorStateClosed]->m_avUse[0]);
                Vector v2 = m_apWalkMeshes[g_nDoorStateClosed]->GetWorldPoint(m_apWalkMeshes[g_nDoorStateClosed]->m_avUse[1]);
                g_nDoorDebugMode = 2;
                m_apWalkMeshes[0]->RenderDebug(0, 0xffff0000, 0xff00ff00);
            } else if (i == 1) {
                Vector v1 = m_apWalkMeshes[g_nDoorStateOpen1]->GetWorldPoint(m_apWalkMeshes[g_nDoorStateOpen1]->m_avUse[0]);
                v1.z += 0.2f;
                Vector v2 = m_apWalkMeshes[g_nDoorStateOpen1]->GetWorldPoint(m_apWalkMeshes[g_nDoorStateOpen1]->m_avUse[1]);
                v2.z += 0.2f;
                g_nDoorDebugMode = 3;
                m_apWalkMeshes[1]->RenderDebug(0, 0xffff0000, 0xff00ff00);
            } else if (i == 2) {
                Vector v1 = m_apWalkMeshes[g_nDoorStateOpen2]->GetWorldPoint(m_apWalkMeshes[g_nDoorStateOpen2]->m_avUse[0]);
                v1.z += 0.4f;
                Vector v2 = m_apWalkMeshes[g_nDoorStateOpen2]->GetWorldPoint(m_apWalkMeshes[g_nDoorStateOpen2]->m_avUse[1]);
                v2.z += 0.4f;
                g_nDoorDebugMode = 4;
                m_apWalkMeshes[2]->RenderDebug(0, 0xffff0000, 0xff00ff00);
            }
        }
        if (i == g_nDoorStateClosed) {
            *pMin = vMin;
            *pMax = vMax;
        } else {
            if (vMin.x < pMin->x)
                pMin->x = vMin.x;
            if (vMin.y < pMin->y)
                pMin->y = vMin.y;
            if (vMin.z < pMin->z)
                pMin->z = vMin.z;
            if (vMax.x > pMax->x)
                pMax->x = vMax.x;
            if (vMax.y > pMax->y)
                pMax->y = vMax.y;
            if (vMax.z > pMax->z)
                pMax->z = vMax.z;
        }
    }
    return bResult;
}

// NONMATCH (2): x87 operand order of q.z * q.w only (fld/fmul swapped).
// FUNCTION: SWKOTOR 0x005896E0
void CSWSDoor::SetOrientation(Quaternion q)
{
    m_qOrientation = q;
    Vector v((q.x * q.y - q.z * q.w) * 2.0f, 1.0f - (q.x * q.x + q.z * q.z) * 2.0f, 0.0f);
    if (v.x * v.x + v.y * v.y > 0.0001f)
        v = Normalize(v);
    else
        v = Vector(0.0f, 1.0f, 0.0f);
    CSWSObject::SetOrientation(v);
}

// ---------------------------------------------------------------------------

class CSWSPlaceable : public CSWSObject {
public:
    void SetOrientation(const Quaternion &q);
    Vector GetNearestActionPoint(const Vector &v);

    char pad_0xa8[0x2f8];
    CSWCollisionMesh m_cCollisionMesh;   // +0x3a0
    Quaternion m_qOrientation;       // +0x428
};

// NONMATCH (50): the original rematerialises &m_avUse[n] for every call and
// returns through a temporary; cl here keeps the addresses in ebx/ebp.
// FUNCTION: SWKOTOR 0x00584C20
Vector CSWSPlaceable::GetNearestActionPoint(const Vector &v)
{
    CSWCollisionMesh *pMesh = &m_cCollisionMesh;
    if (pMesh->GetUsePoint(0).IsZero())
        return m_vPosition;
    Vector vDiff1 = v - pMesh->GetUsePoint(0);
    Vector vDiff2 = v - pMesh->GetUsePoint(1);
    if (!pMesh->GetUsePoint(1).IsZero() && !(vDiff2.MagnitudeSquared() > vDiff1.MagnitudeSquared()))
        return pMesh->GetUsePoint(1);
    return pMesh->GetUsePoint(0);
}

// FUNCTION: SWKOTOR 0x005853C0
void CSWSPlaceable::SetOrientation(const Quaternion &q)
{
    m_qOrientation = q;
    Vector v;
    v.x = (q.y * q.x - q.z * q.w) * 2.0f;
    v.z = 0.0f;
    v.y = 1.0f - (q.z * q.z + q.x * q.x) * 2.0f;
    v = Normalize(v);
    CSWSObject::SetOrientation(v);
}

// ---------------------------------------------------------------------------

class CSWSTrigger : public CSWSObject {
public:
    void GetBoundingBox(float *pMinX, float *pMinY, float *pMaxX, float *pMaxY);
    int InTrigger(Vector *pPoint);
    int GetClosestPointWithinTrigger(Vector *pPoint, Vector *pResult);

    char pad_0xa8[0x1dc];
    int m_nVertices;                 // +0x284
    Vector *m_pvVertices;            // +0x288
    char pad_0x28c[0x8];
    int m_nOutlineVertices;          // +0x294
    int *m_pnOutlineVertices;        // +0x298
    char pad_0x29c[0x20];
    int m_bCircular;                 // +0x2bc
    char pad_0x2c0[0x28];
    float m_fMinX;                   // +0x2e8
    float m_fMaxX;                   // +0x2ec
    float m_fMinY;                   // +0x2f0
    float m_fMaxY;                   // +0x2f4
};

// FUNCTION: SWKOTOR 0x0058CC60
void CSWSTrigger::GetBoundingBox(float *pMinX, float *pMinY, float *pMaxX, float *pMaxY)
{
    if (m_fMinX == m_fMaxX && m_fMinY == m_fMaxY) {
        Vector *pVertices = m_pvVertices;
        m_fMinX = pVertices[0].x;
        m_fMinY = pVertices[0].y;
        m_fMaxX = pVertices[0].x;
        m_fMaxY = pVertices[0].y;
        for (int i = 1; i < m_nVertices; i++) {
            if (pVertices[i].x < m_fMinX)
                m_fMinX = pVertices[i].x;
            if (pVertices[i].y < m_fMinY)
                m_fMinY = pVertices[i].y;
            if (pVertices[i].x > m_fMaxX)
                m_fMaxX = pVertices[i].x;
            if (pVertices[i].y > m_fMaxY)
                m_fMaxY = pVertices[i].y;
        }
    }
    *pMinX = m_fMinX;
    *pMinY = m_fMinY;
    *pMaxX = m_fMaxX;
    *pMaxY = m_fMaxY;
}

extern float g_fTriggerRadius;

// FUNCTION: SWKOTOR 0x0058CE40
int CSWSTrigger::InTrigger(Vector *pPoint)
{
    if (m_bCircular) {
        Vector vDiff(pPoint->x - m_vPosition.x, pPoint->y - m_vPosition.y, pPoint->z - m_vPosition.z);
        if (vDiff.y * vDiff.y + vDiff.x * vDiff.x + vDiff.z * vDiff.z <= g_fTriggerRadius * g_fTriggerRadius)
            return 1;
        return 0;
    }
    int nCrossings = 0;
    for (int i = 0; i < m_nOutlineVertices; i++) {
        int nIndex = m_pnOutlineVertices[i];
        int nLast = m_nOutlineVertices - 1;
        int j;
        if (i != nLast)
            j = i + 1;
        else
            j = 0;
        Vector *pB = &m_pvVertices[m_pnOutlineVertices[j]];
        Vector *pA = &m_pvVertices[nIndex];
        if (pB->x != pA->x) {
            float fX = pPoint->x;
            float fY1 = pA->y;
            float fDY = pB->y - fY1;
            float fDX = pB->x - pA->x;
            float fY = fY1 - (pA->x - pPoint->x) * (fDY / fDX);
            if (fY > pPoint->y) {
                if (fabs(fDY) > fabs(fDX)) {
                    if (pB->y > pA->y) {
                        if (fY >= pA->y && fY < pB->y)
                            nCrossings++;
                    } else if (pA->y > pB->y && fY > pB->y && fY <= pA->y)
                        nCrossings++;
                } else {
                    if (pB->x > pA->x) {
                        if (fX >= pA->x && fX < pB->x)
                            nCrossings++;
                    } else if (pA->x > pB->x && fX > pB->x && fX <= pA->x)
                        nCrossings++;
                }
            }
        }
        if (pB->x == pPoint->x && (pB->y > pPoint->y || pA->y >= pPoint->y))
            nCrossings++;
    }
    return nCrossings % 2 == 1;
}

// FUNCTION: SWKOTOR 0x0058D8C0
int CSWSTrigger::GetClosestPointWithinTrigger(Vector *pPoint, Vector *pResult)
{
    float fMinX, fMinY, fMaxX, fMaxY;
    float fBest = 1.0e11f;
    Vector vBest(0.0f, 0.0f, 0.0f);
    Vector vTest;
    GetBoundingBox(&fMinX, &fMinY, &fMaxX, &fMaxY);
    float fStepX = (fMaxX - fMinX) * 0.2f;
    float fStepY = (fMaxY - fMinY) * 0.2f;
    for (float fX = fMinX; fX <= fMaxX; fX += fStepX) {
        for (float fY = fMinY; fY <= fMaxY; fY += fStepY) {
            vTest = Vector(fX, fY, 0.0f);
            float fDist = Vector(pPoint->x - vTest.x, pPoint->y - vTest.y, pPoint->z - vTest.z).MagnitudeSquared();
            if (fDist < fBest && InTrigger(&vTest) == 1) {
                vBest = vTest;
                fBest = fDist;
            }
        }
    }
    if (fBest < 1.0e10f) {
        *pResult = vBest;
        return 1;
    }
    return 0;
}
