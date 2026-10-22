// CSWCMiniGame (vtable 0x00752914): object lists, axis helpers, the
// destructor, plus the small hit-info / explosion helpers it uses
// (0x00670B10-0x006737E0).

#include "m/sw_mini_game_object.h"

// FUNCTION: SWKOTOR 0x00670B20
void ExplosionDoneCallback(CAurObject* pModel, char* szEvent, void* pUser)
{
    if (pModel) {
        delete pModel;
    }
}

// FUNCTION: SWKOTOR 0x00670B30
void CSWCMiniGame::GetClipRange(float* pNear, float* pFar)
{
    if (pNear) {
        *pNear = m_fNear;
    }
    if (pFar) {
        *pFar = m_fFar;
    }
}

// FUNCTION: SWKOTOR 0x00670B50
void CSWCMiniGame::SetClipRange(float fA, float fB)
{
    if (fA > fB) {
        m_fNear = fB;
        m_fFar = fA;
    }
    else if (fA < fB) {
        m_fNear = fA;
        m_fFar = fB;
    }
    else {
        m_fNear = fA;
        m_fFar = fB;
    }
    if (m_pCamera) {
        m_pCamera->SetClipRange(m_fNear, m_fFar);
    }
}

// FUNCTION: SWKOTOR 0x00670BB0
Vector CSWCMiniGame::GetAxis(Quaternion* pOri)
{
    switch (m_nAxisType) {
    case 1:
        return pOri->GetXAxis();
    case 2:
        return pOri->GetYAxis();
    case 3:
        return pOri->ToDirection();
    }
    return Vector();
}

// FUNCTION: SWKOTOR 0x00670C10
int CSWCMiniGame::GetHookNode()
{
    switch (m_nMode) {
    case 1:
        return 1;
    case 2:
        return 2;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00670FB0
void CSWCMiniGame::UpdateAxis(Vector* pVec, float fDelta)
{
    switch (m_nMode) {
    case 1:
        pVec->y = pVec->y * -1.0f;
        pVec->z = pVec->z * -1.0f;
        break;
    case 2: {
        float fX = -pVec->x;
        float fZ = -pVec->z;
        pVec->x = fZ;
        pVec->z = fX;
        pVec->y = pVec->y * -1.0f;
        break;
    }
    default:
        return;
    }
    switch (m_nAxis) {
    case 0:
        UpdateAxisLinear(pVec, fDelta);
        return;
    case 1:
        UpdateAxisRK4(pVec, fDelta);
        return;
    }
}

// FUNCTION: SWKOTOR 0x00671460
void CSWMiniGameObject::SetHitInfo(const CSWMiniHitInfo& cHit)
{
    m_cHit = cHit;
}

// FUNCTION: SWKOTOR 0x006714E0
void CSWCMiniGame::Go()
{
    CSWMiniGameObjectArray* pObjects = g_pAppManager->GetClientExoApp()->GetMiniGameObjects();
    if (m_pPlayer) {
        m_pPlayer->Go();
    }
    for (int i = m_lstEnemies.num - 1; i >= 0; i--) {
        CSWTrackFollower* pEnemy = (CSWTrackFollower*)pObjects->GetObject(m_lstEnemies.element[i], 2);
        if (pEnemy) {
            pEnemy->Go();
        }
        else {
            m_lstEnemies.DelIndexInline(i);
        }
    }
    if (m_pMusic) {
        m_pMusic->Play(m_cMusic, 8, 0, 1, 0);
    }
}

// FUNCTION: SWKOTOR 0x00671570
CSWMiniGameObject* CSWCMiniGame::GetEnemy(int i)
{
    if (i >= m_lstEnemies.num) {
        return 0;
    }
    return g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetObject(
        m_lstEnemies.element[i], 2);
}

// FUNCTION: SWKOTOR 0x006715B0
CSWTrackFollower* CSWCMiniGame::RemoveObject(CSWTrackFollower* p)
{
    unsigned long nId = p->m_nId;
    int i = m_lstEnemies.IndexOf(nId);
    if (i == -1) {
        return 0;
    }
    m_lstEnemies.DelIndexInline(i);
    return p;
}

// FUNCTION: SWKOTOR 0x00671610
CSWMiniTrackPoint* CSWCMiniGame::GetTrackPoint(int i)
{
    if (i >= m_lstTrackPoints.num) {
        return 0;
    }
    return m_lstTrackPoints.element[i];
}

// FUNCTION: SWKOTOR 0x00671630
CSWMiniGameObject* CSWCMiniGame::GetObstacle(int i)
{
    if (i >= m_lstObstacles.num) {
        return 0;
    }
    return g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetObject(
        m_lstObstacles.element[i], 3);
}

// FUNCTION: SWKOTOR 0x00671670
CSWMiniCamera* CSWCMiniGame::SetCamera(CSWMiniCamera* pCamera)
{
    m_pCamera = pCamera;
    if (pCamera && m_pPlayer && m_pPlayer->m_pModel) {
        switch (m_nMode) {
        case 1: {
            int nCount = m_pPlayer->m_lstHooked.num;
            CAurNode* pNode = 0;
            CAurObject* pHooked;
            for (int i = 0; i < nCount; i++) {
                if (pNode) {
                    break;
                }
                pHooked = m_pPlayer->GetHookedModel(i);
                if (pHooked) {
                    pNode = pHooked->GetNode("camerahook");
                }
            }
            if (pNode) {
                m_pCamera->SetBehavior(new CAurBehaviorAttach(m_pCamera->GetObject(), pHooked,
                                                              (CAurPart*)pNode));
            }
            else {
                CAurObject* pModel = m_pPlayer->m_pModel;
                if (pModel) {
                    pNode = pModel->GetNode("camerahook");
                    if (pNode) {
                        m_pCamera->SetBehavior(new CAurBehaviorAttach(m_pCamera->GetObject(),
                                                                      pModel, (CAurPart*)pNode));
                    }
                }
            }
            break;
        }
        case 2:
            pCamera->AttachTo(m_pPlayer->m_pHook, "camerahook", 0);
            break;
        }
        m_pCamera->SetClipRange(m_fNear, m_fFar);
        m_pCamera->SetFieldOfView(m_f70);
    }
    return m_pCamera;
}

// FUNCTION: SWKOTOR 0x006717F0
CSWMiniPlayer* CSWCMiniGame::SetPlayer(CSWMiniPlayer* p)
{
    if (m_pPlayer) {
        delete m_pPlayer;
    }
    m_pPlayer = p;
    SetCamera(m_pCamera);
    return m_pPlayer;
}

// FUNCTION: SWKOTOR 0x00671820
CSWMiniPlayer* CSWCMiniGame::CreatePlayer(CExoArrayList<CResRef>* pModels,
                                          CExoArrayList<unsigned char>* pAttach,
                                          const CResRef& cTrack)
{
    int nModels = pModels->num;
    if (m_pPlayer) {
        delete m_pPlayer;
    }
    m_pPlayer = new CSWMiniPlayer(this);
    char szModel[17];
    for (int i = 0; i < nModels; i++) {
        pModels->element[i].CopyToString(szModel);
        CAurObject* pModel = NewCAurObject(szModel, "", 0, 0);
        if (pModel) {
            m_pPlayer->AddModel(pModel, pAttach->element[i]);
        }
    }
    cTrack.CopyToString(szModel);
    CAurObject* pTrack = NewCAurObject(szModel, "", 0, 0);
    if (pTrack) {
        CSWMiniTrackPoint* pPoint = FindTrackPoint(cTrack);
        if (pPoint) {
            pTrack->SetLocalPosition(pPoint->m_vPos);
        }
        pTrack->SetScene(m_pScene->GetScene());
        m_pPlayer->SetModelIfChanged(pTrack, GetHookNode());
    }
    SetCamera(m_pCamera);
    return m_pPlayer;
}

// FUNCTION: SWKOTOR 0x006719D0
CSWMiniPlayer* CSWCMiniGame::CreatePlayerWithHook(CExoArrayList<CResRef>* pModels,
                                                  CExoArrayList<unsigned char>* pAttach,
                                                  const CResRef& cTrack, const CResRef& cHook,
                                                  unsigned char bAttach)
{
    int nModels = pModels->num;
    if (m_pPlayer) {
        delete m_pPlayer;
    }
    m_pPlayer = new CSWMiniPlayer(this);
    char szModel[17];
    for (int i = 0; i < nModels; i++) {
        pModels->element[i].CopyToString(szModel);
        CAurObject* pModel = NewCAurObject(szModel, "", 0, 0);
        if (pModel) {
            m_pPlayer->AddModel(pModel, pAttach->element[i]);
        }
    }
    cTrack.CopyToString(szModel);
    CAurObject* pTrack = NewCAurObject(szModel, "", 0, 0);
    if (pTrack) {
        CSWMiniTrackPoint* pPoint = FindTrackPoint(cTrack);
        if (pPoint) {
            pTrack->SetLocalPosition(pPoint->m_vPos);
        }
        pTrack->SetScene(m_pScene->GetScene());
        m_pPlayer->SetModelIfChanged(pTrack, GetHookNode());
    }
    cHook.CopyToString(szModel);
    CAurObject* pHook = NewCAurObject(szModel, "", 0, 0);
    if (pHook) {
        pHook->SetScene(m_pScene->GetScene());
        m_pPlayer->SetHook(pHook, bAttach);
    }
    SetCamera(m_pCamera);
    return m_pPlayer;
}

// FUNCTION: SWKOTOR 0x00671BC0
int CSWCMiniGame::SetLateralAxis(int n)
{
    if (n >= 0 && n <= 3) {
        m_nLateralAxis = n;
    }
    return m_nLateralAxis;
}

// FUNCTION: SWKOTOR 0x00671BE0
int CSWCMiniGame::SetVerticalAxis(int n)
{
    if (n >= 0 && n <= 3) {
        m_nVerticalAxis = n;
    }
    return m_nVerticalAxis;
}

// FUNCTION: SWKOTOR 0x00671C00 SYMBOL: ?Add@?$CExoArrayList@K@@QAEXK@Z
template void CExoArrayList<unsigned long>::Add(unsigned long t);

// FUNCTION: SWKOTOR 0x00672040
int CSWCMiniGame::ClearEnemies()
{
    int nCount = m_lstEnemies.num;
    CSWMiniGameObjectArray* pObjects = g_pAppManager->GetClientExoApp()->GetMiniGameObjects();
    for (int i = 0; i < nCount; i++) {
        CSWTrackFollower* pEnemy = (CSWTrackFollower*)pObjects->GetObject(m_lstEnemies.element[i], 2);
        if (pEnemy) {
            delete pEnemy;
        }
    }
    if (m_lstEnemies.element) {
        delete[] m_lstEnemies.element;
        m_lstEnemies.element = 0;
        m_lstEnemies.array_size = 0;
    }
    m_lstEnemies.num = 0;
    return nCount;
}

// FUNCTION: SWKOTOR 0x006721F0
int CSWCMiniGame::ClearTrackPoints()
{
    int nCount = m_lstTrackPoints.num;
    for (int i = nCount - 1; i >= 0; i--) {
        delete m_lstTrackPoints.element[i];
    }
    if (m_lstTrackPoints.element) {
        delete[] m_lstTrackPoints.element;
        m_lstTrackPoints.element = 0;
        m_lstTrackPoints.array_size = 0;
    }
    m_lstTrackPoints.num = 0;
    return nCount;
}

// FUNCTION: SWKOTOR 0x006720C0
CSWMiniTrackPoint* CSWCMiniGame::AddTrackPoint(const CResRef& cName, const Vector& vPos)
{
    int nCount = m_lstTrackPoints.num;
    for (int i = 0; i < nCount; i++) {
        CSWMiniTrackPoint* pPoint = m_lstTrackPoints.element[i];
        if (pPoint->m_cName == cName) {
            return pPoint;
        }
    }
    CSWMiniTrackPoint* pPoint = new CSWMiniTrackPoint;
    pPoint->m_cName = cName;
    pPoint->m_vPos = vPos;
    m_lstTrackPoints.Add(pPoint);
    return pPoint;
}

// FUNCTION: SWKOTOR 0x00672240
CSWMiniObstacle* CSWCMiniGame::AddObstacle(const CResRef& cModel, const Vector& vPos)
{
    CSWMiniObstacle* pObstacle;
    char szModel[17];
    cModel.CopyToString(szModel);
    CAurObject* pModel = NewCAurObject(szModel, "", 0, 0);
    if (pModel) {
        pModel->SetLocalPosition(vPos);
        pModel->SetScene(m_pScene->GetScene());
        pModel->PlayAnimation("ready", 1.0f, 0, 0);
        pObstacle = new CSWMiniObstacle(this, pModel);
        m_lstObstacles.Add(pObstacle->m_nId);
        return pObstacle;
    }
    return pObstacle;
}

// FUNCTION: SWKOTOR 0x00672350
int CSWCMiniGame::ClearObstacles()
{
    int nCount = m_lstObstacles.num;
    CSWMiniGameObjectArray* pObjects = g_pAppManager->GetClientExoApp()->GetMiniGameObjects();
    for (int i = 0; i < nCount; i++) {
        CSWMiniObstacle* pObstacle = (CSWMiniObstacle*)pObjects->GetObject(m_lstObstacles.element[i], 3);
        if (pObstacle) {
            delete pObstacle;
        }
    }
    if (m_lstObstacles.element) {
        delete[] m_lstObstacles.element;
        m_lstObstacles.element = 0;
        m_lstObstacles.array_size = 0;
    }
    m_lstObstacles.num = 0;
    return nCount;
}

// FUNCTION: SWKOTOR 0x00672EB0
CSWCMiniGame::~CSWCMiniGame()
{
    SetPlayer(0);
    ClearEnemies();
    ClearTrackPoints();
    ClearObstacles();
    if (m_pMusic) {
        delete m_pMusic;
    }
}

// FUNCTION: SWKOTOR 0x00672FA0
void CSWCMiniGame::AddBullet(CAurObject* pBullet)
{
    m_lstBullets.Add(new CAurObjectRef(pBullet));
}

// FUNCTION: SWKOTOR 0x006730A0
int CSWCMiniGame::HitCheckBullet(CAurObject* pBullet, CAurObject* pHook, CSWMiniGameObject* p)
{
    CSWMiniBulletInfo* pInfo = pBullet->GetUserData(0xaaab);
    if (pInfo) {
        CAurNode* pNode = pHook->GetNode("hitbullet");
        if (pNode && pHook->HitTest(pNode, pBullet->GetHitOrigin(), pBullet->GetPosition(), 0,
                                    &g_vMiniHitPoint)) {
            p->SetHitInfo(pInfo->m_cHit);
            p->m_nTarget4C = pInfo->m_pOwner->m_nId;
            p->m_s44 = g_szMiniHitPart;
            p->m_vSoundPos = g_vMiniHitPos;
            p->RunHitBulletScript(0);
            if (pBullet->PlayAnimation("explode", 1.0f, 3, 0)) {
                CSWMiniExplosion* pExplosion = new CSWMiniExplosion;
                pExplosion->m_pModel = pBullet;
                pExplosion->m_qOri = DirectionToQuaternion(g_vMiniHitNormal);
                pExplosion->m_qOri =
                    Quaternion(Vector(1.0f, 0.0f, 0.0f), 1.5707964f) * pExplosion->m_qOri;
                pExplosion->m_vPos = g_vMiniHitPos;
                pBullet->SetBehavior(0);
                pBullet->AddEventCallback(ExplosionDoneCallback, "doneexplode", 0, 9999.0f, 0);
                m_lstEffects.Add(pExplosion);
                return 1;
            }
            delete pBullet;
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x006732F0
void CSWCMiniGame::DoHitCheck()
{
    int nBullets = m_lstBullets.num;
    CSWMiniGameObjectArray* pObjects = g_pAppManager->GetClientExoApp()->GetMiniGameObjects();
    int i;
    int j;
    for (i = nBullets - 1; i >= 0; i--) {
        CAurObject* pBullet = m_lstBullets.element[i]->m_p;
        if (pBullet) {
            if (m_pPlayer && m_pPlayer->HitCheckBullet(pBullet)) {
                RemoveBullet(i);
                continue;
            }
            for (j = m_lstEnemies.num - 1; j >= 0; j--) {
                CSWTrackFollower* pEnemy =
                    (CSWTrackFollower*)pObjects->GetObject(m_lstEnemies.element[j], 2);
                if (pEnemy) {
                    if (pEnemy->HitCheckBullet(pBullet)) {
                        RemoveBullet(i);
                        goto next;
                    }
                }
                else {
                    m_lstEnemies.DelIndexInline(j);
                }
            }
            for (j = m_lstObstacles.num - 1; j >= 0; j--) {
                CSWMiniObstacle* pObstacle =
                    (CSWMiniObstacle*)pObjects->GetObject(m_lstObstacles.element[j], 3);
                if (pObstacle) {
                    if (HitCheckBullet(pBullet, pObstacle->m_pModel, pObstacle)) {
                        RemoveBullet(i);
                        goto next;
                    }
                }
                else {
                    m_lstObstacles.DelIndexInline(j);
                }
            }
        }
        else {
            RemoveBullet(i);
        }
    next:;
    }
    for (j = m_lstObstacles.num - 1; j >= 0; j--) {
        if (m_pPlayer && !m_pPlayer->m_bDead) {
            CSWMiniObstacle* pObstacle =
                (CSWMiniObstacle*)pObjects->GetObject(m_lstObstacles.element[j], 3);
            if (pObstacle) {
                m_pPlayer->DoObstacleHitCheck(pObstacle);
            }
            else {
                m_lstObstacles.DelIndexInline(j);
            }
        }
    }
    if (m_pPlayer && !m_pPlayer->m_bDead) {
        for (int k = 0; k < 3; k++) {
            if (m_pPlayer && !m_pPlayer->DoFollowerHitCheck(&m_lstEnemies, k == 0)) {
                break;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x006735D0
void CSWCMiniGame::Update(float fDelta)
{
    CSWMiniGameObjectArray* pObjects = g_pAppManager->GetClientExoApp()->GetMiniGameObjects();
    int i;
    for (i = 0; i < 0xff; i++) {
        CSWMiniGameObject* pObject = pObjects->GetObject(i, 0);
        if (pObject && pObject->m_n0C) {
            delete pObject;
        }
    }
    if (m_pPlayer) {
        m_pPlayer->RunHeartbeatScript(fDelta);
    }
    for (i = m_lstEnemies.num - 1; i >= 0; i--) {
        CSWMiniGameObject* pEnemy = pObjects->GetObject(m_lstEnemies.element[i], 2);
        if (pEnemy) {
            pEnemy->RunHeartbeatScript(fDelta);
        }
        else {
            m_lstEnemies.DelIndexInline(i);
        }
    }
    for (i = m_lstObstacles.num - 1; i >= 0; i--) {
        CSWMiniGameObject* pObstacle = pObjects->GetObject(m_lstObstacles.element[i], 3);
        if (pObstacle) {
            pObstacle->RunHeartbeatScript(fDelta);
        }
        else {
            m_lstObstacles.DelIndexInline(i);
        }
    }
    for (i = m_lstEffects.num - 1; i >= 0; i--) {
        CSWMiniExplosion* pExplosion = m_lstEffects.element[i];
        CAurObject* pModel = pExplosion->m_pModel;
        pModel->SetLocalPosition(pExplosion->m_vPos);
        pModel->SetOrientation(pExplosion->m_qOri);
        delete pExplosion;
        m_lstEffects.DelIndexInline(i);
    }
    if (m_pPlayer->m_bTrackActive) {
        DoHitCheck();
    }
    if (m_pPlayer && m_pPlayer->m_lstHooked.num) {
        CSWMiniRoom* pRoom = m_pScene->GetRoomAt(m_pPlayer->GetHookPosition());
        if (pRoom) {
            m_pRoom = pRoom;
        }
    }
    if (m_pScene && m_pScene->m_pScene && m_pRoom) {
        m_pScene->m_pScene->SetRoom(m_pRoom->m_pRoomData);
    }
}

// FUNCTION: SWKOTOR 0x00673060 SYMBOL: ??_GCSWCMiniGame@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x00673080
CSWMiniExplosion::CSWMiniExplosion()
{
}

// FUNCTION: SWKOTOR 0x006737E0
void CSWTrackFollower::SetHitInfo2(const CSWMiniHitInfo& cHit)
{
    m_cHit2 = cHit;
}
