// Client projectile CSWCProjectile (nwcprojectile.o): model and target
// setup, and the per-path-type behaviour-list builders called by SetMode
// 0x006D6EE0. Builder names give the path type SetMode dispatches on
// (descriptive); only the spiral builder has an attested name.
#include "l/sw_c_projectile.h"

// FUNCTION: SWKOTOR 0x006D2C30
void CSWCProjectile::SetDuration(unsigned long nDuration)
{
    m_nTotalDuration = nDuration;
    m_nDuration = nDuration;
}

// FUNCTION: SWKOTOR 0x006D2C50
int CSWCProjectile::LoadModel(const CResRef& cResRef, unsigned char nPart, int n)
{
    if (m_pAnimationBase == 0) {
        m_pAnimationBase = new CSWCAnimBaseProjectile;
        if (m_pAnimationBase == 0) {
            return 0;
        }
    }
    if (!m_pAnimationBase->LoadModel(cResRef, nPart)) {
        return 0;
    }
    CAurObject* pModel = m_pAnimationBase->GetModel(nPart);
    pModel->SetClientObject(this);
    pModel->SetObjectType(OBJECT_TYPE_PROJECTILE);
    m_pAnimationBase->AttachShadow();
    return 1;
}

// FUNCTION: SWKOTOR 0x006D2D30
void CSWCProjectile::ReleaseModel()
{
    if (m_pGob) {
        m_pGob->AttachToObject(0, 0, 0);
        m_pGob->SetVisible(0);
        delete m_pGob;
        m_pGob = 0;
    }
    if (m_pAnimationBase) {
        delete m_pMuzzleFlash;
        m_pMuzzleFlash = 0;
        m_pAnimationBase->UnloadModel();
        delete m_pAnimationBase;
        m_pAnimationBase = 0;
    }
}

// FUNCTION: SWKOTOR 0x006D2DB0
void CSWCProjectile::UnloadModel(int nPart)
{
    if (m_pGob) {
        m_pGob->AttachToObject(0, 0, 0);
        m_pGob->SetVisible(0);
        delete m_pGob;
        m_pGob = 0;
    }
    if (m_pAnimationBase) {
        m_pAnimationBase->UnloadModelPart(nPart);
    }
}

// FUNCTION: SWKOTOR 0x006D41B0
void CSWCProjectile::SetSpellId(int nSpell)
{
    CSWSpell* pSpell = g_pRules->m_pSpellArray->GetSpell(nSpell);
    if (pSpell == 0 && nSpell != 11000) {
        return;
    }
    m_nSpellId = nSpell;
    if (nSpell == 11000) {
        m_cImpactModel = "c_cow_dead";
        return;
    }
    m_cModel = pSpell->m_cProjectileModel;
    m_cImpactModel = pSpell->m_cProjectileModel;
    m_n168 = 0;
    m_n1A1 = pSpell->m_n11D;
}

// FUNCTION: SWKOTOR 0x006D4250
int CSWCProjectile::PlayAnimation(unsigned short nAnim)
{
    int bResult = 0;
    if (m_pAnimationBase) {
        bResult = m_pAnimationBase->PlayAnimation(nAnim, 1.0f, 0, 0);
        if (bResult) {
            m_nCurrentAnimation = nAnim;
            return 1;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x006D42A0
void CSWCProjectile::SetImpactInfo(unsigned char a, unsigned long b, unsigned short c)
{
    m_nImpactA = a;
    m_nImpactB = b;
    m_nImpactC = c;
}

// FUNCTION: SWKOTOR 0x006D42D0
void CSWCProjectile::SetTarget(unsigned long oidTarget, const CExoString& sNode)
{
    CGameObject* pObject = g_pAppManager->m_pClientExoApp->GetGameObject(oidTarget);
    CSWCPlaceable* pPlaceable = 0;
    CSWCDoor* pDoor = 0;
    if (pObject == 0) {
        m_oidTarget = OBJECT_INVALID;
    }
    else {
        pPlaceable = pObject->AsSWCPlaceable();
        pDoor = pObject->AsSWCDoor();
    }
    if (pPlaceable) {
        CExoString sSuffix("_impact");
        m_sTargetNode = pPlaceable->m_sModelName + sSuffix;
    }
    else if (pDoor) {
        CExoString sSuffix("impc");
        m_sTargetNode = pDoor->m_sModelName + sSuffix;
    }
    else if (sNode == "") {
        m_sTargetNode = "impact";
    }
    else {
        m_sTargetNode = sNode;
    }
    m_oidTarget = oidTarget;
}

// FUNCTION: SWKOTOR 0x006D4440
void CSWCProjectile::AttachMuzzleFlash(const CExoString& sModel, CAurObject* pTo,
                                       const CExoString& sNode)
{
    if (sModel.IsEmpty()) {
        return;
    }
    if (pTo == 0) {
        return;
    }
    m_pMuzzleFlash = NewCAurObject(sModel.CStr(), "muzzleflash", 0, 0);
    m_pMuzzleFlash->AttachToObject(pTo, sNode.CStr(), 3);
    m_pMuzzleFlash->PlayAnimation("impact", 1.0f, 1, 0);
}

// FUNCTION: SWKOTOR 0x006D4520
void CSWCProjectile::ClearBehaviorList()
{
    while (!m_lBehaviors.IsEmpty()) {
        CSWCProjectileBehavior* pBehavior = m_lBehaviors.RemoveHead();
        if (pBehavior) {
            delete pBehavior;
        }
    }
}

// FUNCTION: SWKOTOR 0x006D4550
void CSWCProjectile::SetBehaviorListType1()
{
    Vector vTarget;
    if (m_oidTarget == OBJECT_INVALID) {
        vTarget = m_vTargetPosition;
    }
    else if (g_pAppManager->m_pClientExoApp->GetGameObject(m_oidTarget)) {
        vTarget = Vector(0.0f, 0.0f, 0.0f);
    }
    else {
        vTarget = m_vTargetPosition;
    }
    CSWCProjectileBehavior* pBehavior = new CSWCProjectileBehavior;
    pBehavior->m_vStart = vTarget;
    pBehavior->m_nType = 1;
    pBehavior->m_vEnd = vTarget;
    pBehavior->m_nState = 0;
    pBehavior->m_nDuration = m_nDuration;
    pBehavior->m_nDelay = 0;
    pBehavior->m_oidTarget = m_oidTarget;
    pBehavior->m_nFlags = 0;
    pBehavior->m_fSpeed = -1.0f;
    m_lBehaviors.AddTail(pBehavior);
}

// FUNCTION: SWKOTOR 0x006D4640
void CSWCProjectile::SetBehaviorListType2()
{
    Vector vTarget;
    if (m_oidTarget == OBJECT_INVALID) {
        vTarget = m_vTargetPosition;
    }
    else if (g_pAppManager->m_pClientExoApp->GetGameObject(m_oidTarget)) {
        vTarget = Vector(0.0f, 0.0f, 0.0f);
    }
    else {
        vTarget = m_vTargetPosition;
    }
    CSWCProjectileBehavior* pBehavior = new CSWCProjectileBehavior;
    pBehavior->m_vStart = vTarget;
    pBehavior->m_nType = 2;
    pBehavior->m_vEnd = vTarget;
    pBehavior->m_nState = 0;
    pBehavior->m_nDuration = m_nDuration;
    pBehavior->m_nDelay = 0;
    pBehavior->m_oidTarget = m_oidTarget;
    pBehavior->m_nFlags = 0;
    pBehavior->m_fSpeed = -1.0f;
    m_lBehaviors.AddTail(pBehavior);
}

// FUNCTION: SWKOTOR 0x006D4930
void CSWCProjectile::SetBehaviorListType4()
{
    Vector vTarget;
    if (m_oidTarget == OBJECT_INVALID) {
        vTarget = m_vTargetPosition;
    }
    else if (g_pAppManager->m_pClientExoApp->GetGameObject(m_oidTarget)) {
        vTarget = Vector(0.0f, 0.0f, 0.0f);
    }
    else {
        vTarget = m_vTargetPosition;
    }
    CSWCProjectileBehavior* pBehavior = new CSWCProjectileBehavior;
    pBehavior->m_vStart = vTarget;
    pBehavior->m_nType = 4;
    pBehavior->m_vEnd = vTarget;
    pBehavior->m_nState = 0;
    pBehavior->m_nDuration = m_nDuration;
    pBehavior->m_nDelay = 0;
    pBehavior->m_oidTarget = m_oidTarget;
    pBehavior->m_nFlags = 0;
    pBehavior->m_fSpeed = -1.0f;
    m_lBehaviors.AddTail(pBehavior);
}

// FUNCTION: SWKOTOR 0x006D4A20
void CSWCProjectile::SetBehaviorListType9()
{
    Vector vTarget;
    if (m_oidTarget == OBJECT_INVALID) {
        vTarget = m_vTargetPosition;
    }
    else if (g_pAppManager->m_pClientExoApp->GetGameObject(m_oidTarget)) {
        vTarget = Vector(0.0f, 0.0f, 0.0f);
    }
    else {
        vTarget = m_vTargetPosition;
    }
    CSWCProjectileBehavior* pBehavior = new CSWCProjectileBehavior;
    pBehavior->m_vStart = vTarget;
    pBehavior->m_nType = 9;
    pBehavior->m_vEnd = vTarget;
    pBehavior->m_nState = 0;
    pBehavior->m_nDuration = m_nDuration;
    pBehavior->m_nDelay = 0;
    pBehavior->m_oidTarget = m_oidTarget;
    pBehavior->m_nFlags = 0;
    pBehavior->m_fSpeed = -1.0f;
    m_lBehaviors.AddTail(pBehavior);
}

// FUNCTION: SWKOTOR 0x006D4B10
void CSWCProjectile::SetBehaviorListType5()
{
    Vector vTarget;
    if (m_oidTarget == OBJECT_INVALID) {
        vTarget = m_vTargetPosition;
    }
    else if (g_pAppManager->m_pClientExoApp->GetGameObject(m_oidTarget)) {
        vTarget = Vector(0.0f, 0.0f, 0.0f);
    }
    else {
        vTarget = m_vTargetPosition;
    }
    CSWCProjectileBehavior* pBehavior = new CSWCProjectileBehavior;
    pBehavior->m_vStart = vTarget;
    pBehavior->m_nType = 5;
    pBehavior->m_vEnd = vTarget;
    pBehavior->m_nState = 0;
    pBehavior->m_nDuration = m_nDuration;
    pBehavior->m_nDelay = 0;
    pBehavior->m_oidTarget = m_oidTarget;
    pBehavior->m_nFlags = 0;
    pBehavior->m_fSpeed = -1.0f;
    m_lBehaviors.AddTail(pBehavior);
}

// FUNCTION: SWKOTOR 0x006D4730
void CSWCProjectile::SetBehaviorListType3()
{
    Vector vTarget;
    if (m_oidTarget == OBJECT_INVALID) {
        vTarget = m_vTargetPosition;
    }
    else if (g_pAppManager->m_pClientExoApp->GetGameObject(m_oidTarget)) {
        vTarget = Vector(0.0f, 0.0f, 0.0f);
    }
    else {
        vTarget = m_vTargetPosition;
    }

    CSWCProjectileBehavior* pBehavior = new CSWCProjectileBehavior;
    pBehavior->m_nType = 1;
    pBehavior->m_vStart = m_vPosition;
    pBehavior->m_vStart.z += 30.0f;
    pBehavior->m_vEnd = vTarget;
    pBehavior->m_nState = 0;
    pBehavior->m_nDuration = m_nDuration / 2;
    pBehavior->m_nDelay = 0;
    pBehavior->m_oidTarget = OBJECT_INVALID;
    pBehavior->m_nFlags = 0;
    pBehavior->m_fSpeed = -1.0f;
    m_lBehaviors.AddTail(pBehavior);

    pBehavior = new CSWCProjectileBehavior;
    pBehavior->m_vStart = vTarget;
    pBehavior->m_vStart.z += 30.0f;
    pBehavior->m_vEnd = vTarget;
    pBehavior->m_nType = 11;
    pBehavior->m_nState = 0;
    pBehavior->m_nDuration = 0;
    pBehavior->m_nDelay = 0;
    pBehavior->m_oidTarget = m_oidTarget;
    pBehavior->m_nFlags = 0;
    pBehavior->m_fSpeed = -1.0f;
    m_lBehaviors.AddTail(pBehavior);

    pBehavior = new CSWCProjectileBehavior;
    pBehavior->m_vStart = vTarget;
    pBehavior->m_vEnd = vTarget;
    pBehavior->m_nType = 1;
    pBehavior->m_nState = 0;
    pBehavior->m_nDuration = m_nDuration - m_nDuration / 2;
    pBehavior->m_nDelay = 0;
    pBehavior->m_oidTarget = m_oidTarget;
    pBehavior->m_nFlags = 0;
    pBehavior->m_fSpeed = -1.0f;
    m_lBehaviors.AddTail(pBehavior);
}

// FUNCTION: SWKOTOR 0x006D4FE0
void CSWCProjectile::SetBehaviorListType7()
{
    Vector vTarget;
    if (m_oidTarget == OBJECT_INVALID) {
        vTarget = m_vTargetPosition;
    }
    else if (g_pAppManager->m_pClientExoApp->GetGameObject(m_oidTarget)) {
        vTarget = Vector(0.0f, 0.0f, 0.0f);
    }
    else {
        vTarget = m_vTargetPosition;
    }
    CSWCProjectileBehavior* pBehavior = new CSWCProjectileBehavior;
    pBehavior->m_vStart = vTarget;
    pBehavior->m_nType = 7;
    pBehavior->m_vEnd = Vector(0.0f, 0.0f, 0.0f);
    pBehavior->m_nState = 0;
    pBehavior->m_nDuration = m_nDuration;
    pBehavior->m_nDelay = 0;
    pBehavior->m_oidTarget = m_oidTarget;
    pBehavior->m_nFlags = 0;
    pBehavior->m_fSpeed = -1.0f;
    m_lBehaviors.AddTail(pBehavior);
}

// FUNCTION: SWKOTOR 0x006D5500
void CSWCProjectile::SetBehaviorListType10()
{
    Vector vTarget;
    if (m_oidTarget == OBJECT_INVALID) {
        vTarget = m_vTargetPosition;
    }
    else if (g_pAppManager->m_pClientExoApp->GetGameObject(m_oidTarget)) {
        vTarget = Vector(0.0f, 0.0f, 0.0f);
    }
    else {
        vTarget = m_vTargetPosition;
    }
    CSWCProjectileBehavior* pBehavior = new CSWCProjectileBehavior;
    pBehavior->m_vStart = vTarget;
    pBehavior->m_nType = 12;
    pBehavior->m_vEnd = vTarget;
    pBehavior->m_nState = 0;
    pBehavior->m_nDuration = m_nDuration;
    pBehavior->m_nDelay = 0;
    pBehavior->m_oidTarget = m_oidTarget;
    pBehavior->m_nFlags = 0;
    pBehavior->m_fSpeed = -1.0f;
    m_lBehaviors.AddTail(pBehavior);
}

// FUNCTION: SWKOTOR 0x006D50F0
void CSWCProjectile::SetBehaviorListType14()
{
    Vector vTarget;
    if (m_oidTarget == OBJECT_INVALID) {
        vTarget = m_vTargetPosition;
    }
    else if (g_pAppManager->m_pClientExoApp->GetGameObject(m_oidTarget) == 0) {
        vTarget = m_vTargetPosition;
    }

    CSWCProjectileBehavior* pBehavior = new CSWCProjectileBehavior;
    pBehavior->m_nType = 1;
    pBehavior->m_oidTarget = m_oidTarget;
    pBehavior->m_vStart = vTarget;
    pBehavior->m_vEnd = vTarget;
    pBehavior->m_nState = 0;
    pBehavior->m_nDuration = m_nDuration;
    pBehavior->m_nDelay = 0;
    pBehavior->m_nFlags = 1;
    pBehavior->m_fSpeed = 0.0f;
    m_lBehaviors.AddTail(pBehavior);

    pBehavior = new CSWCProjectileBehavior;
    pBehavior->m_nType = 1;
    pBehavior->m_oidTarget = 0x7f000000;
    pBehavior->m_vStart = m_vSourcePosition;
    pBehavior->m_vEnd = m_vSourcePosition;
    pBehavior->m_nState = 0;
    pBehavior->m_nDuration = m_nSourceDuration;
    pBehavior->m_nDelay = 0;
    pBehavior->m_nFlags = 1;
    pBehavior->m_fSpeed = 0.0f;
    m_lBehaviors.AddTail(pBehavior);
}

// FUNCTION: SWKOTOR 0x006D5250
void CSWCProjectile::SetBehaviorListType15()
{
    Vector vTarget;
    if (m_oidTarget == OBJECT_INVALID) {
        vTarget = m_vTargetPosition;
    }
    else if (g_pAppManager->m_pClientExoApp->GetGameObject(m_oidTarget) == 0) {
        vTarget = m_vTargetPosition;
    }

    CSWCProjectileBehavior* pBehavior = new CSWCProjectileBehavior;
    pBehavior->m_nType = 1;
    pBehavior->m_oidTarget = m_oidTarget;
    pBehavior->m_vStart = vTarget;
    pBehavior->m_vEnd = vTarget;
    pBehavior->m_nState = 0;
    pBehavior->m_nDuration = m_nDuration;
    pBehavior->m_nDelay = 0;
    pBehavior->m_nFlags = 1;
    pBehavior->m_fSpeed = 0.0f;
    m_lBehaviors.AddTail(pBehavior);

    pBehavior = new CSWCProjectileBehavior;
    pBehavior->m_nType = 1;
    pBehavior->m_oidTarget = m_oidSource;
    pBehavior->m_vStart = vTarget;
    pBehavior->m_vEnd = vTarget;
    pBehavior->m_nState = 0;
    pBehavior->m_nDuration = m_nDuration;
    pBehavior->m_nDelay = 0;
    pBehavior->m_nFlags = 1;
    pBehavior->m_fSpeed = 0.0f;
    m_lBehaviors.AddTail(pBehavior);
}

// FUNCTION: SWKOTOR 0x006D53A0
void CSWCProjectile::SetBehaviorListType16()
{
    Vector vTarget;
    if (m_oidTarget == OBJECT_INVALID) {
        vTarget = m_vTargetPosition;
    }
    else if (g_pAppManager->m_pClientExoApp->GetGameObject(m_oidTarget) == 0) {
        vTarget = m_vTargetPosition;
    }

    CSWCProjectileBehavior* pBehavior = new CSWCProjectileBehavior;
    pBehavior->m_nType = 1;
    pBehavior->m_oidTarget = m_oidTarget;
    pBehavior->m_vStart = vTarget;
    pBehavior->m_vEnd = vTarget;
    pBehavior->m_nState = 0;
    pBehavior->m_nDuration = m_nDuration;
    pBehavior->m_nDelay = 0;
    pBehavior->m_nFlags = 1;
    pBehavior->m_fSpeed = GetSourceSpeed();
    m_lBehaviors.AddTail(pBehavior);

    pBehavior = new CSWCProjectileBehavior;
    pBehavior->m_nType = 1;
    pBehavior->m_oidTarget = 0x7f000000;
    pBehavior->m_vStart = m_vSourcePosition;
    pBehavior->m_vEnd = m_vSourcePosition;
    pBehavior->m_nState = 0;
    pBehavior->m_nDuration = m_nSourceDuration;
    pBehavior->m_nDelay = 0;
    pBehavior->m_nFlags = 1;
    pBehavior->m_fSpeed = 0.0f;
    m_lBehaviors.AddTail(pBehavior);
}

// FUNCTION: SWKOTOR 0x006D4C00
void CSWCProjectile::SetBehaviorListSpiral()
{
    Vector vSide;
    Vector vDir = m_vTargetPosition - m_vPosition;
    m_nSide = rand() % 2;
    if (m_nSide) {
        vSide.x = -vDir.y;
        vSide.y = vDir.x;
    }
    else {
        vSide.x = vDir.y;
        vSide.y = -vDir.x;
    }
    vSide.z = 0.0f;
    if (vSide.y * vSide.y + vSide.x * vSide.x <= 0.00001f) {
        vSide = Vector(0.0f, 1.0f, 0.0f);
    }
    vSide = NormalizeVector(vSide) * 2.0f;
    if (m_oidTarget == OBJECT_INVALID) {
        vSide = vSide + m_vTargetPosition;
        vSide.z = m_vPosition.z;
    }

    CSWCProjectileBehavior* pBehavior;
    if (m_nDuration - 2500 != 0) {
        pBehavior = new CSWCProjectileBehavior;
        pBehavior->m_nType = 10;
        pBehavior->m_vStart = vSide;
        if (m_oidTarget == OBJECT_INVALID) {
            pBehavior->m_vEnd = m_vTargetPosition;
        }
        else {
            pBehavior->m_vEnd = Vector(0.0f, 0.0f, 0.0f);
        }
        pBehavior->m_nState = 0;
        pBehavior->m_nDuration = m_nDuration - 2500;
        pBehavior->m_nDelay = 0;
        pBehavior->m_oidTarget = m_oidTarget;
        pBehavior->m_nFlags = 0;
        pBehavior->m_fSpeed = 2.0f;
        m_lBehaviors.AddTail(pBehavior);
    }
    else if (m_oidTarget == OBJECT_INVALID) {
        SetPosition(vSide);
    }
    else {
        CGameObject* pObject;
        CGameObjectArray* pObjectArray = g_pAppManager->m_pClientExoApp->GetGameObjectArray();
        if (pObjectArray->GetGameObject(m_oidTarget, &pObject) == GAME_OBJECT_VALID) {
            Vector vPosition;
            vPosition = vSide + ((CSWCObject*)pObject)->m_vPosition;
            vPosition.z = m_vPosition.z;
            SetPosition(vPosition);
        }
    }

    pBehavior = new CSWCProjectileBehavior;
    pBehavior->m_nType = 6;
    if (m_oidTarget == OBJECT_INVALID) {
        pBehavior->m_vStart = m_vTargetPosition;
        pBehavior->m_vEnd = m_vTargetPosition;
    }
    else {
        pBehavior->m_vStart = Vector(0.0f, 0.0f, 0.0f);
        pBehavior->m_vEnd = Vector(0.0f, 0.0f, 0.0f);
    }
    pBehavior->m_nState = 0;
    pBehavior->m_nDuration = 2500;
    pBehavior->m_nDelay = 0;
    pBehavior->m_oidTarget = m_oidTarget;
    pBehavior->m_nFlags = 0;
    pBehavior->m_fSpeed = -1.0f;
    m_lBehaviors.AddTail(pBehavior);
}

// FUNCTION: SWKOTOR 0x006D55F0
void CSWCProjectile::SetBehaviorListType50()
{
    Vector vTarget;
    if (m_oidTarget == OBJECT_INVALID) {
        vTarget = m_vTargetPosition;
    }
    else if (g_pAppManager->m_pClientExoApp->GetGameObject(m_oidTarget)) {
        vTarget = Vector(0.0f, 0.0f, 0.0f);
    }
    else {
        vTarget = m_vTargetPosition;
    }

    CSWCProjectileBehavior* pBehavior = new CSWCProjectileBehavior;
    pBehavior->m_vStart = vTarget;
    pBehavior->m_nType = 2;
    pBehavior->m_vEnd = m_vTargetPosition;
    pBehavior->m_nState = 0;
    pBehavior->m_nDuration = m_nDuration;
    pBehavior->m_nDelay = m_nDuration * 6 / 10;
    pBehavior->m_oidTarget = m_oidTarget;
    pBehavior->m_nFlags = 0;
    pBehavior->m_fSpeed = -1.0f;
    m_lBehaviors.AddTail(pBehavior);

    pBehavior = new CSWCProjectileBehavior;
    pBehavior->m_vStart = vTarget;
    pBehavior->m_nType = 4;
    pBehavior->m_vEnd = m_vTargetPosition;
    pBehavior->m_nState = 3;
    pBehavior->m_nDuration = m_nDuration * 6 / 10;
    pBehavior->m_nDelay = 0;
    pBehavior->m_oidTarget = m_oidTarget;
    pBehavior->m_nFlags = 9;
    pBehavior->m_fSpeed = -1.0f;
    m_lBehaviors.AddTail(pBehavior);
}

// FUNCTION: SWKOTOR 0x006D6EE0
void CSWCProjectile::SetMode(int nMode)
{
    m_nMode = nMode;
    if (m_oidTarget != OBJECT_INVALID) {
        CGameObject* pObject;
        CGameObjectArray* pObjectArray = g_pAppManager->m_pClientExoApp->GetGameObjectArray();
        if (pObjectArray->GetGameObject(m_oidTarget, &pObject) == GAME_OBJECT_VALID) {
            m_vTargetPosition = ((CSWCObject*)pObject)->m_vPosition;
        }
    }
    ClearBehaviorList();
    switch (nMode) {
    case 1:
        SetBehaviorListType1();
        break;
    case 2:
        SetBehaviorListType2();
        break;
    case 3:
        SetBehaviorListType3();
        break;
    case 4:
        SetBehaviorListType4();
        break;
    case 9:
        SetBehaviorListType9();
        break;
    case 5:
        SetBehaviorListType5();
        break;
    case 6:
        SetBehaviorListSpiral();
        break;
    case 7:
        SetBehaviorListType7();
        break;
    case 8:
        SetBehaviorListType8();
        break;
    case 10:
        SetBehaviorListType10();
        break;
    case 50:
        SetBehaviorListType50();
        break;
    case 11:
        SetBehaviorListType11(1);
        break;
    case 12:
        SetBehaviorListType11(0);
        break;
    case 13:
        SetBehaviorListType13();
        break;
    case 14:
        SetBehaviorListType14();
        break;
    case 15:
        SetBehaviorListType15();
        break;
    case 16:
        SetBehaviorListType16();
        break;
    default:
        return;
    }
    m_pBehavior = m_lBehaviors.RemoveHead();
    m_nDuration = m_nTotalDuration = m_pBehavior->m_nDuration;
    m_vTargetPosition = m_pBehavior->m_vStart;
    SetProjectileVelAndAccel(m_vOrientation);
}

// ---------------------------------------------------------------- lifetime

// FUNCTION: SWKOTOR 0x006D57B0
CSWCProjectile::CSWCProjectile()
    : CSWCObject(OBJECT_TYPE_PROJECTILE)
{
    unsigned long nDay;
    unsigned long nTime;
    g_pAppManager->m_pClientExoApp->GetWorldTimer()->GetWorldTime(&nDay, &nTime);
    m_nCreateDay = nDay;
    m_vPosition = Vector(0.0f, 0.0f, 0.0f);
    m_nCreateTime = nTime;
    m_vTargetPosition = Vector(0.0f, 0.0f, 0.0f);
    m_v178 = Vector(0.0f, 0.0f, 0.0f);
    m_n15C = 1;
    m_n160 = 0;
    m_n164 = 0;
    m_n168 = 0;
    m_nMode = 0;
    m_v184 = Vector(0.0f, 0.0f, 0.0f);
    m_n154 = 1;
    m_n170 = 1;
    m_n150 = 0;
    m_nTotalDuration = 0;
    m_nDuration = 0;
    g_pAppManager->m_pClientExoApp->GetGameObjectArray()->AddInternalObject(&m_idSelf, this, 0);
    g_pAppManager->m_pClientExoApp->GetClientAIMaster()->AddObject(this);
    m_vVelocity = Vector(0.0f, 0.0f, 0.0f);
    m_pBehavior = 0;
    m_nSpellId = 0;
    m_pGob = 0;
    m_nSide = 0;
    m_n1A1 = 0;
    m_nImpactA = 0;
    m_nImpactC = 0;
    m_nImpactB = 0;
    m_n158 = 0;
    m_sTargetNode = "impact";
    m_oidTarget = 0x7f000000;
    m_oidSource = 0x7f000000;
    m_n1A4 = 0;
    m_n1A8 = 0;
    m_pMuzzleFlash = 0;
    m_nSourceDuration = 0;
    m_fSourceSpeed = 0.0f;
}

// FUNCTION: SWKOTOR 0x006D5A40
CSWCProjectile::~CSWCProjectile()
{
    g_pAppManager->m_pClientExoApp->GetGameObjectArray()->Delete(m_idSelf);
    ClearBehaviorList();
    ReleaseModel();
}

// FUNCTION: SWKOTOR 0x006D5AF0 SYMBOL: ??_GCSWCProjectile@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x006D44B0 SYMBOL: ??0?$CExoLinkedList@UCSWCProjectileBehavior@@@@QAE@XZ
