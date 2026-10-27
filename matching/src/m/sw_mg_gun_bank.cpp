// CSWMGGunBank (vtable 0x00752AE4) and CSWMGTargettingGunBank (vtable
// 0x00752AF0): setters, constructors, gun list helpers and the death
// callbacks (0x00673840-0x00674A10). The CSWMiniHitInfo constructor is
// inlined into CSWMGGunBank's constructor here.

#define M_INLINE_MINI_HIT_INFO
#define M_INLINE_TARGETTING_INFO
#include "m/sw_mini_game_object.h"

// FUNCTION: SWKOTOR 0x00673840
float CSWMGGunBank::SetSpeed(float f)
{
    if (f >= 0.0) {
        m_cHit.m_fSpeed = f;
    }
    return m_cHit.m_fSpeed;
}

// FUNCTION: SWKOTOR 0x00673860
float CSWMGGunBank::SetLifespan(float f)
{
    if (f > 0.0) {
        m_cHit.m_fLifespan = f;
    }
    return m_cHit.m_fLifespan;
}

// FUNCTION: SWKOTOR 0x00673880
int CSWMGGunBank::SetTargetMask(int n)
{
    if (n & 0x11111110) {
        char sz[256];
        sprintf(sz, "CSWMGGunBank: Bullet Target Type (%d) is invalid", n);
        return m_cHit.m_nTargetMask;
    }
    m_cHit.m_nTargetMask = n;
    return n;
}

// FUNCTION: SWKOTOR 0x006738D0
void CSWMGGunBank::OnFire()
{
    CResRef cScript = m_pOwner->GetScript(7);
    m_pOwner->SetHitInfo2(m_cHit);
    if (cScript.IsValid()) {
        CExoString sScript;
        cScript.CopyToString(sScript);
        unsigned long nId = m_pOwner->m_nId;
        g_pVirtualMachine->RunScript(&sScript, nId, 1);
    }
}

// FUNCTION: SWKOTOR 0x00673970
float CSWMGTargettingGunBank::SetSensingRadius(float f)
{
    if (f >= 0.0) {
        m_cInfo.m_fSensingRadius = f;
    }
    return m_cInfo.m_fSensingRadius;
}

// FUNCTION: SWKOTOR 0x00673990
float CSWMGTargettingGunBank::SetVertSpread(float f)
{
    if (f >= 0.0 || f == -1.0) {
        m_cInfo.m_fVertSpread = f;
    }
    return m_cInfo.m_fVertSpread;
}

// FUNCTION: SWKOTOR 0x006739C0
float CSWMGTargettingGunBank::SetHorizSpread(float f)
{
    if (f >= 0.0 || f == -1.0) {
        m_cInfo.m_fHorizSpread = f;
    }
    return m_cInfo.m_fHorizSpread;
}

// FUNCTION: SWKOTOR 0x006739F0
float CSWMGTargettingGunBank::SetInaccuracy(float f)
{
    if (f >= 0.0) {
        m_cInfo.m_fInaccuracy = f;
    }
    return m_cInfo.m_fInaccuracy;
}

// FUNCTION: SWKOTOR 0x00673F50
float CSWMGGunBank::SetRateOfFire(float f)
{
    if (f >= 0.0) {
        int nCount = m_lstGuns.num;
        m_cHit.m_fRateOfFire = f;
        for (int i = 0; i < nCount; i++) {
            CAurEmitter* pEmitter = (CAurEmitter*)m_lstGuns.element[i]->GetUserData(-1);
            switch (pEmitter->GetType()) {
            case 0xaaaa:
                pEmitter->SetBirthrateB(f);
                break;
            case 0xaaad:
                pEmitter->SetBirthrateA(f);
                break;
            }
        }
    }
    return m_cHit.m_fRateOfFire;
}

// FUNCTION: SWKOTOR 0x00673FD0
void CSWMGGunBank::Reset()
{
    int nCount = m_lstGuns.num;
    for (int i = 0; i < nCount; i++) {
        CAurObject* pGun = m_lstGuns.element[i];
        pGun->PlayAnimation("damage", 1.0f, 1, 0);
        pGun->PlayAnimation("ready", 1.0f, 0x20, 0);
    }
}

// FUNCTION: SWKOTOR 0x00674020
CAurObject* CSWMGGunBank::RemoveGun(CAurObject* pGun)
{
    int i = m_lstGuns.IndexOf(pGun);
    if (i == -1) {
        return 0;
    }
    pGun->AttachToObject(0, 0, 0);
    m_lstGuns.DelIndexInline(i);
    return pGun;
}

// FUNCTION: SWKOTOR 0x00674090
void CSWMGGunBank::LoadResources()
{
    int nCount = m_lstGuns.num;
    for (int i = 0; i < nCount; i++) {
        CAurEmitter* pEmitter = (CAurEmitter*)m_lstGuns.element[i]->GetUserData(0xaaad);
        if (pEmitter) {
            pEmitter->Restart();
        }
    }
}

// FUNCTION: SWKOTOR 0x006740D0
// NONMATCH (5 diff insns): stack frame only. The original shares the
// initial CResRef::operator= return temporary with the gun-emitter case's
// (frame 0x48); ours gives each its own slot (0x58). Scoping the first
// assignment, an early return and C-style declarations were tried.
// When the old gun has no emitter the original uses pNewGun uninitialized.
void CSWMGGunBank::SetGunModel(CResRef* pGunModel)
{
    m_cGunModel = *pGunModel;
    int nCount = m_lstGuns.num;
    for (int i = 0; i < nCount; i++) {
        CAurObject* pTarget = 0;
        CAurObject* pOldGun = m_lstGuns.element[0];
        CAurObject* pNewGun;
        CAurEmitter* pEmitter = (CAurEmitter*)pOldGun->GetUserData(-1);
        if (pEmitter) {
            switch (pEmitter->GetType()) {
            case 0xaaaa:
                pTarget = ((CAurTargettingEmitter*)pEmitter)->m_pTarget;
            case 0xaaad: {
                CAurObject* pModel = pEmitter->m_pModel;
                CAurPart* pPart = pEmitter->m_pPart;
                if (pModel && pPart) {
                    char szModel[17];
                    m_cGunModel.CopyToString(szModel);
                    pNewGun = NewCAurObject(szModel, "", 0, 0);
                    switch (pEmitter->GetType()) {
                    case 0xaaad: {
                        CAurGunEmitter* pOld = (CAurGunEmitter*)pEmitter;
                        CAurGunEmitter* pNew = new CAurGunEmitter(pNewGun, pModel, pPart);
                        pNew->SetBirthrateA(pOld->GetBirthrate());
                        pNew->m_cSound = pOld->m_cSound;
                        pNewGun->SetBehavior(pNew);
                        pNewGun->AddEventCallback(GunEventCallback, "*", this, 9999.0f, 0);
                        break;
                    }
                    case 0xaaaa: {
                        CAurTargettingEmitter* pOld = (CAurTargettingEmitter*)pEmitter;
                        CAurTargettingEmitter* pNew =
                            new CAurTargettingEmitter(pNewGun, pModel, pPart, pTarget);
                        pNew->m_cInfo = pOld->m_cInfo;
                        pNew->m_fBump = pOld->GetBump();
                        pNew->m_vOffset = pOld->m_vOffset;
                        pNew->SetBirthrateB(pOld->GetBirthrate());
                        pNew->m_cSound = pOld->m_cSound;
                        pNewGun->SetBehavior(pNew);
                        pNewGun->AddEventCallback(GunEventCallback, "*", this, 9999.0f, 0);
                        break;
                    }
                    }
                    pNewGun->AttachToPart(pModel, pPart, 0);
                }
                break;
            }
            }
        }
        pNewGun->PlayAnimation("ready", 1.0f, 0, 0);
        m_lstGuns.DelIndexInline(0);
        delete pOldGun;
        m_lstGuns.AddInline(pNewGun);
    }
}

// FUNCTION: SWKOTOR 0x006746D0
CSWMGGunBank::CSWMGGunBank(CSWTrackFollower* pOwner, CResRef* pBulletModel, CResRef* pGunModel,
                           CSWMiniHitInfo* pHit)
{
    m_cBulletModel = *pBulletModel;
    SetGunModel(pGunModel);
    m_cHit.m_nDamage = pHit->m_nDamage;
    SetSpeed(pHit->m_fSpeed);
    SetRateOfFire(pHit->m_fRateOfFire);
    SetLifespan(pHit->m_fLifespan);
    SetTargetMask(pHit->m_nTargetMask);
    m_cHit.m_cSound = pHit->m_cSound;
    m_pOwner = pOwner;
}

// FUNCTION: SWKOTOR 0x00674800
CSWMGGunBank::~CSWMGGunBank()
{
    ClearGuns();
}

// FUNCTION: SWKOTOR 0x00674860
CSWMGTargettingGunBank::CSWMGTargettingGunBank(CSWTrackFollower* pOwner, CResRef* pBulletModel,
                                               CResRef* pGunModel, CSWMiniHitInfo* pHit,
                                               CSWMGTargettingInfo* pInfo,
                                               CSWTrackFollower* pTarget)
    : CSWMGGunBank(pOwner, pBulletModel, pGunModel, pHit)
{
    SetSensingRadius(pInfo->m_fSensingRadius);
    SetHorizSpread(pInfo->m_fHorizSpread);
    SetVertSpread(pInfo->m_fVertSpread);
    SetInaccuracy(pInfo->m_fInaccuracy);
    m_pTarget = pTarget;
}

// FUNCTION: SWKOTOR 0x00674930
void GunDie(CAurObject* pModel, char* szEvent, CSWMGGunBank* pBank)
{
    if (!pBank) {
        if (pModel) {
            delete pModel;
        }
        return;
    }
    pBank->RemoveGun(pModel);
    if (pModel) {
        delete pModel;
    }
    if (pBank->m_lstGuns.num == 0) {
        if (pBank->m_pOwner) {
            pBank->m_pOwner->RemoveGunBank(pBank);
        }
        delete pBank;
    }
}

// FUNCTION: SWKOTOR 0x00674990
void CSWMGGunBank::Die()
{
    for (int i = m_lstGuns.num - 1; i >= 0; i--) {
        CAurObject* pGun = m_lstGuns.element[i];
        pGun->PlayAnimation(0, 1.0f, 0, 0);
        if (pGun->PlayAnimation("die", 1.0f, 1, 0)) {
            pGun->AddEventCallback((AurEventCallback)GunDie, "donedie", this, 9999.0f, 0);
        }
        else {
            RemoveGun(pGun);
            delete pGun;
        }
    }
}

// FUNCTION: SWKOTOR 0x006743B0
CAurObject* CSWMGGunBank::AttachGun(CAurObject* pModel, CAurPart* pPart)
{
    if (m_cGunModel.IsValid() && pModel && pPart) {
        char szModel[17];
        m_cGunModel.CopyToString(szModel);
        CAurObject* pGun = NewCAurObject(szModel, "", 0, 0);
        if (pGun) {
            CAurGunEmitter* pEmitter = new CAurGunEmitter(pGun, pModel, pPart);
            pEmitter->SetBirthrateA(m_cHit.m_fRateOfFire);
            pEmitter->m_cSound = m_cFireSound;
            pGun->SetBehavior(pEmitter);
            pGun->AddEventCallback(GunEventCallback, "*", this, 9999.0f, 0);
            pGun->PlayAnimation("ready", 1.0f, 0, 0);
            m_lstGuns.Add(pGun);
            return pGun;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x006744D0
int CSWMGGunBank::ClearGuns()
{
    int nCount = m_lstGuns.num;
    for (int i = nCount - 1; i >= 0; i--) {
        m_lstGuns.element[i]->AttachToObject(0, 0, 0);
        if (m_lstGuns.element[i]) {
            delete m_lstGuns.element[i];
        }
    }
    if (m_lstGuns.element) {
        delete[] m_lstGuns.element;
        m_lstGuns.element = 0;
        m_lstGuns.array_size = 0;
    }
    m_lstGuns.num = 0;
    return nCount;
}

// FUNCTION: SWKOTOR 0x00674530
CAurObject* CSWMGTargettingGunBank::AttachGun(CAurObject* pModel, CAurPart* pPart)
{
    if (m_cGunModel.IsValid() && pModel && pPart) {
        char szModel[17];
        m_cGunModel.CopyToString(szModel);
        CAurObject* pGun = NewCAurObject(szModel, "", 0, 0);
        if (pGun) {
            CAurTargettingEmitter* pEmitter =
                new CAurTargettingEmitter(pGun, pModel, pPart, m_pTarget->GetHookedModel(0));
            pEmitter->SetBirthrateB(m_cHit.m_fRateOfFire);
            pEmitter->m_cInfo = m_cInfo;
            pEmitter->m_fBump = m_pTarget->GetF84();
            pEmitter->m_cSound = m_cFireSound;
            if (m_pTarget->AsMiniPlayer()) {
                pEmitter->m_vOffset = m_pTarget->AsMiniPlayer()->m_v214;
            }
            pGun->SetBehavior(pEmitter);
            pGun->AddEventCallback(GunEventCallback, "*", this, 9999.0f, 0);
            pGun->PlayAnimation("ready", 1.0f, 0, 0);
            m_lstGuns.Add(pGun);
            return pGun;
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00674A10
// NONMATCH (8 diff insns): register choice only; the original loads the
// attach part / model into edx / ecx for the CAurBehaviorAttach arguments,
// ours into ecx / edx.
void CSWMGTargettingGunBank::Die()
{
    int nCount = m_lstGuns.num;
    for (int i = nCount - 1; i >= 0; i--) {
        CAurObject* pGun = m_lstGuns.element[i];
        CAurEmitter* pEmitter = (CAurEmitter*)pGun->GetUserData(0xaaaa);
        if (pEmitter && pEmitter->m_pModel && pEmitter->m_pPart) {
            pGun->SetBehavior(new CAurBehaviorAttach(pGun, pEmitter->m_pModel, pEmitter->m_pPart));
            pGun->PlayAnimation(0, 1.0f, 0, 0);
            if (pGun->PlayAnimation("die", 1.0f, 1, 0)) {
                pGun->AddEventCallback((AurEventCallback)GunDie, "donedie", this, 9999.0f, 0);
            }
            else {
                RemoveGun(pGun);
                delete pGun;
            }
        }
        else {
            RemoveGun(pGun);
            delete pGun;
        }
    }
}
