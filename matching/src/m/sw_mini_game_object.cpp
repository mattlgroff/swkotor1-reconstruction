// Mini-game objects: CSWMiniGameObjectArray, CSWMiniGameObject and the
// CSWTrackFollower accessors (0x0066BF10-0x0066E8F2, 0x00673A10).

#include "m/sw_mini_game_object.h"

#pragma auto_inline(off)

// FUNCTION: SWKOTOR 0x00673A10
CSWMiniHitInfo::CSWMiniHitInfo()
{
    m_nDamage = 0;
    m_nTargetMask = 0;
    m_fSpeed = 0.0f;
    m_fRateOfFire = 0.0f;
    m_fLifespan = 0.0f;
}

// FUNCTION: SWKOTOR 0x0066BF10
CSWMiniGameObjectArray::CSWMiniGameObjectArray()
{
    for (int i = 0; i < 0xff; i++) {
        m_pObjects[i] = 0;
    }
    m_nNext = 0;
}

// FUNCTION: SWKOTOR 0x0066BF30
CSWMiniGameObject* CSWMiniGameObjectArray::GetObject(unsigned long nId, unsigned char nType)
{
    if (nId < 0xff) {
        CSWMiniGameObject* pObject = m_pObjects[nId];
        if (pObject) {
            switch (nType) {
            case 0:
                return pObject;
            case 4:
                return pObject->AsTrackFollower();
            case 3:
                return (CSWMiniGameObject*)pObject->AsMiniObstacle();
            case 1:
                return (CSWMiniGameObject*)pObject->AsMiniPlayer();
            case 2:
                return (CSWMiniGameObject*)pObject->AsMiniEnemy();
            case 5: {
                CSWMiniEnemyHead* pEnemy = pObject->AsMiniEnemy();
                if (pEnemy && pEnemy->m_bActive) {
                    return (CSWMiniGameObject*)pEnemy;
                }
                break;
            }
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0066C060
int CSWMiniGameObjectArray::GetFreeSlot()
{
    int nStart = m_nNext;
    while (m_pObjects[m_nNext]) {
        m_nNext++;
        if (m_nNext == 0xff) {
            m_nNext = 0;
        }
        if (m_nNext == nStart) {
            return 0xff;
        }
    }
    return m_nNext;
}

// FUNCTION: SWKOTOR 0x0066C0A0
void CSWMiniGameObject::RunHeartbeatScript(float fDelta)
{
    CResRef cScript = GetScript(4);
    if (cScript.IsValid()) {
        CExoString sScript;
        cScript.CopyToString(sScript);
        unsigned long nId = m_nId;
        g_pVirtualMachine->RunScript(&sScript, nId, 1);
    }
}

// FUNCTION: SWKOTOR 0x0066C130
void CSWMiniGameObject::ValidateTarget4C()
{
    if (!g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetObject(m_nTarget4C, 4)) {
        m_nTarget4C = 0xff;
    }
}

// FUNCTION: SWKOTOR 0x0066C160
void CSWMiniGameObject::ValidateTarget50()
{
    if (!g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetObject(m_nTarget50, 4)) {
        m_nTarget50 = 0xff;
    }
}

// FUNCTION: SWKOTOR 0x0066C2D0
void CSWMiniGameObject::RunHitFollowerScript(int bHit)
{
    if (!bHit) {
        CResRef cScript = GetScript(2);
        if (cScript.IsValid()) {
            CExoString sScript;
            cScript.CopyToString(sScript);
            unsigned long nId = m_nId;
            g_pVirtualMachine->RunScript(&sScript, nId, 1);
        }
    }
}

// FUNCTION: SWKOTOR 0x0066C370
void CSWMiniGameObject::RunCreateScript()
{
    CResRef cScript;
    if (AsTrackFollower()) {
        cScript = GetScript(0);
        if (cScript.IsValid()) {
            CExoString sScript;
            cScript.CopyToString(sScript);
            unsigned long nId = m_nId;
            g_pVirtualMachine->RunScript(&sScript, nId, 1);
        }
    }
}

// FUNCTION: SWKOTOR 0x0066C420
void CSWMiniGameObject::LoadScripts(CResGFF* pGFF, int nUnused, CResStruct* pStruct)
{
    int bSuccess;
    {
        CResRef cDefault("");
        SetScript(0, pGFF->ReadFieldCResRef(pStruct, "OnCreate", bSuccess, cDefault));
    }
    {
        CResRef cDefault("");
        SetScript(1, pGFF->ReadFieldCResRef(pStruct, "OnHitBullet", bSuccess, cDefault));
    }
    {
        CResRef cDefault("");
        SetScript(2, pGFF->ReadFieldCResRef(pStruct, "OnHitFollower", bSuccess, cDefault));
    }
    {
        CResRef cDefault("");
        SetScript(3, pGFF->ReadFieldCResRef(pStruct, "OnAnimEvent", bSuccess, cDefault));
    }
    {
        CResRef cDefault("");
        SetScript(4, pGFF->ReadFieldCResRef(pStruct, "OnHeartbeat", bSuccess, cDefault));
    }
}

// FUNCTION: SWKOTOR 0x0066C740
void CSWTrackFollower::LoadScripts(CResGFF* pGFF, int nUnused, CResStruct* pStruct)
{
    int bSuccess;
    CSWMiniGameObject::LoadScripts(pGFF, nUnused, pStruct);
    {
        CResRef cDefault("");
        SetScript(5, pGFF->ReadFieldCResRef(pStruct, "OnDamage", bSuccess, cDefault));
    }
    {
        CResRef cDefault("");
        SetScript(6, pGFF->ReadFieldCResRef(pStruct, "OnDeath", bSuccess, cDefault));
    }
    {
        CResRef cDefault("");
        SetScript(7, pGFF->ReadFieldCResRef(pStruct, "OnFire", bSuccess, cDefault));
    }
    {
        CResRef cDefault("");
        SetScript(8, pGFF->ReadFieldCResRef(pStruct, "OnHitObstacle", bSuccess, cDefault));
    }
    {
        CResRef cDefault("");
        SetScript(9, pGFF->ReadFieldCResRef(pStruct, "OnTrackLoop", bSuccess, cDefault));
    }
}

// FUNCTION: SWKOTOR 0x0066C870
float CSWTrackFollower::SetSpeed(float f)
{
    m_fSpeed = f;
    if (m_pModel && (m_nFlags & 1)) {
        m_pModel->PlayAnimation("track", f * 0.01f, m_nAxisMode != 1 ? 0x14 : 0x11, 0);
    }
    return m_fSpeed;
}

// FUNCTION: SWKOTOR 0x0066C8D0
float CSWTrackFollower::SetF84(float f)
{
    if (f >= 0.0f) {
        m_f84 = f;
    }
    return m_f84;
}

// FUNCTION: SWKOTOR 0x0066C900
int CSWTrackFollower::SetN8C(int n)
{
    if (n > 0) {
        m_n8C = n;
    }
    return m_n8C;
}

// FUNCTION: SWKOTOR 0x0066C920
float CSWTrackFollower::SetFA0(float f)
{
    if (f >= 0.0f) {
        m_fA0 = f;
    }
    return m_fA0;
}

// FUNCTION: SWKOTOR 0x0066C950
int CSWTrackFollower::SetN90(int n)
{
    if (n > 0) {
        m_n90 = n;
    }
    return m_n90;
}

// FUNCTION: SWKOTOR 0x0066C970
CResRef CSWTrackFollower::GetScript(int nScript)
{
    if (nScript >= 0 && nScript < 10) {
        return m_aScripts[nScript];
    }
    return CResRef();
}

// FUNCTION: SWKOTOR 0x0066C9C0
void CSWTrackFollower::SetScript(int nScript, const CResRef& cScript)
{
    if (nScript >= 0 && nScript < 10) {
        m_aScripts[nScript] = cScript;
    }
}

// FUNCTION: SWKOTOR 0x0066C9F0
int CSWTrackFollower::GetSound15C(int n)
{
    if (n >= 0 && n < 2) {
        return m_aSounds[n].m_n04;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0066CA10
void CSWTrackFollower::SetSound15C(int n, int v)
{
    if (n >= 0 && n < 2) {
        m_aSounds[n].m_n04 = v;
    }
}

// FUNCTION: SWKOTOR 0x0066CA30
int CSWTrackFollower::GetSound160(int n)
{
    if (n >= 0 && n < 2) {
        return m_aSounds[n].m_n08;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0066CA50
void CSWTrackFollower::SetSound160(int n, int v)
{
    if (n >= 0 && n < 2) {
        m_aSounds[n].m_n08 = v;
    }
}

// FUNCTION: SWKOTOR 0x0066CA70
int CSWTrackFollower::GetSoundVolume(int n)
{
    if (n >= 0 && n < 2) {
        return m_aSounds[n].m_nVolume;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0066CA90
void CSWTrackFollower::SetSoundVolume(int n, int v)
{
    if (n >= 0 && n < 2) {
        m_aSounds[n].m_nVolume = v;
        if (n < 1) {
            m_apSources[n]->SetVolume(v);
        }
    }
}

// FUNCTION: SWKOTOR 0x0066CAD0
CExoString CSWTrackFollower::GetModelName()
{
    if (m_pModel) {
        return CExoString(m_pModel->GetName());
    }
    return CExoString("");
}

// FUNCTION: SWKOTOR 0x0066CB10
void CSWTrackFollower::ValidateTarget170()
{
    if (!g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetObject(m_nTarget170, 3)) {
        m_nTarget170 = 0xff;
    }
}

// FUNCTION: SWKOTOR 0x0066CB40
void CSWMiniPlayer::ClampPosition(Vector* pPos)
{
    if (m_vClamp.x != 0.0f) {
        if (m_pMiniGame->m_nMode == 2) {
            if (pPos->x < 0.0f) {
                pPos->x += 360.0f;
            }
            else if (pPos->x > 359.0f) {
                pPos->x -= 360.0f;
            }
        }
    }
    else if (m_vMin.x + m_vOffset.x < pPos->x) {
        pPos->x = m_vMin.x + m_vOffset.x;
    }
    else if (m_vMax.x + m_vOffset.x > pPos->x) {
        pPos->x = m_vMax.x + m_vOffset.x;
    }
    if (m_vClamp.y != 0.0f) {
        if (m_pMiniGame->m_nMode == 2) {
            if (pPos->y < 0.0f) {
                pPos->y += 360.0f;
            }
            else if (pPos->y > 359.0f) {
                pPos->y -= 360.0f;
            }
        }
    }
    else if (m_vMin.y + m_vOffset.y < pPos->y) {
        pPos->y = m_vMin.y + m_vOffset.y;
    }
    else if (m_vMax.y + m_vOffset.y > pPos->y) {
        pPos->y = m_vMax.y + m_vOffset.y;
    }
    if (m_vClamp.z != 0.0f) {
        if (m_pMiniGame->m_nMode == 2) {
            if (pPos->z < 0.0f) {
                pPos->z += 360.0f;
            }
            else if (pPos->z > 359.0f) {
                pPos->z -= 360.0f;
            }
        }
    }
    else if (m_vMin.z + m_vOffset.z < pPos->z) {
        pPos->z = m_vMin.z + m_vOffset.z;
    }
    else if (m_vMax.z + m_vOffset.z > pPos->z) {
        pPos->z = m_vMax.z + m_vOffset.z;
    }
}

// FUNCTION: SWKOTOR 0x0066CD30
// NONMATCH (6 diff insns): the original reuses vVel's dead stack slot for the
// conjugated-quaternion temporary (frame 0x34, ours 0x40), and multiplies the
// projection's x term as delta.x * axis.x. Inline Dot/operator forms and a
// block-scoped vVel were tried.
Vector CSWMiniPlayer::GetInertia(const Vector& vPrevPos, const Vector& vPos, const Quaternion& qOri,
                                 float fDelta)
{
    Vector vAxis = m_pMiniGame->GetAxis((Quaternion*)&qOri);
    Vector vVel = (vPos - vPrevPos) / fDelta;
    Vector vDelta = m_v228 - vVel;
    vDelta -= vAxis * (vDelta.x * vAxis.x + vDelta.y * vAxis.y + vDelta.z * vAxis.z);
    m_v228 = vVel;
    float fScale = g_fMiniInertiaScale;
    return (-qOri * vDelta) * fScale;
}

// FUNCTION: SWKOTOR 0x0066CCF0
Vector CSWMiniPlayer::SetPosition(const Vector& v)
{
    m_vPosition = v;
    ClampPosition(&m_vPosition);
    return m_vPosition;
}

// FUNCTION: SWKOTOR 0x0066CF40
float CSWMiniPlayer::SetF1D8(float f)
{
    if (f >= 0.0f) {
        m_f1D8 = f;
    }
    return m_f1D8;
}

// FUNCTION: SWKOTOR 0x0066CF70
float CSWMiniPlayer::SetF1DC(float f)
{
    if (f >= 0.0f) {
        m_f1DC = f;
    }
    return m_f1DC;
}

// FUNCTION: SWKOTOR 0x0066CFA0
float CSWMiniPlayer::SetF1E0(float f)
{
    if (f >= 0.0f) {
        m_f1E0 = f;
    }
    return m_f1E0;
}

// FUNCTION: SWKOTOR 0x0066CFD0
Vector CSWMiniPlayer::SetOffset(const Vector& v)
{
    m_vOffset = v;
    if (!(m_nFlags & 1)) {
        m_vPosition = v;
    }
    return m_vOffset;
}

// FUNCTION: SWKOTOR 0x0066D030
void CSWMiniPlayer::SetHook(CAurObject* pHook, unsigned char bAttach)
{
    int nNode;
    if (bAttach) {
        nNode = m_pMiniGame->GetHookNode();
    }
    else {
        nNode = 0;
    }
    if (m_pHook) {
        delete m_pHook;
    }
    m_pHook = pHook;
    m_bHookAttached = bAttach;
    if (pHook && m_pModel) {
        pHook->AttachToObject(m_pModel, "modelhook", nNode);
    }
}

// FUNCTION: SWKOTOR 0x0066D090
void CSWTrackFollower::SetAxisMode(int n)
{
    if (abs(n) == 1) {
        m_nAxisMode = n;
    }
}

// FUNCTION: SWKOTOR 0x0066D240
CSWMGGunBank* CSWTrackFollower::GetGunBank(int n)
{
    if (n >= 0 && n < m_lstGunBanks.num) {
        return m_lstGunBanks.element[n];
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0066D260
CAurObject* CSWTrackFollower::GetHookedModel(int n)
{
    if (n >= m_lstHooked.num) {
        return 0;
    }
    return m_lstHooked.element[n].m_pModel;
}

// FUNCTION: SWKOTOR 0x0066D280
CAurObject* CSWTrackFollower::RemoveHookedModel(CAurObject* p)
{
    for (int i = 0; i < m_lstHooked.num; i++) {
        if (m_lstHooked.element[i].m_pModel == p) {
            m_lstHooked.DelIndex(i);
            p->AttachToObject(0, 0, 0);
            return p;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0066D440
void CSWTrackFollower::StopTrackAnimation()
{
    if (m_pModel && (m_nFlags & 1)) {
        m_pModel->PlayAnimation("track", 0.0f, m_nAxisMode == 1 ? 0x11 : 0x14, 0);
        m_nFlags &= ~1;
        if (m_apSources[0]) {
            m_apSources[0]->Stop();
        }
    }
}

// FUNCTION: SWKOTOR 0x0066E280
CAurObject* CSWTrackFollower::SetModelIfChanged(CAurObject* pModel, int n)
{
    if (pModel != m_pModel) {
        return SetModel(pModel, n);
    }
    return m_pModel;
}

// FUNCTION: SWKOTOR 0x0066E8E0
void CSWTrackFollower::SetN19C(int n)
{
    m_n19C = n;
    ApplyDamage(0);
}

#pragma auto_inline(on)
