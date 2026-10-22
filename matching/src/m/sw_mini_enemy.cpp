// CSWMiniEnemy (vtable 0x007528A8): gun-bank loading and the small slots,
// plus the CSWMGTargettingInfo limits block.

#include "m/sw_mini_game_object.h"



// FUNCTION: SWKOTOR 0x0066F5F0
// NONMATCH (7 diff insns): register choice only; the original loads the
// player (m_pMiniGame->m_pPlayer) and pInfo through edx and pHit through ecx
// before the first push. An inline GetPlayer() getter is worse (12).
CSWMGGunBank* CSWMiniEnemy::CreateGunBank(int nBank, CResRef* pBulletModel, CResRef* pGunModel,
                                          CSWMiniHitInfo* pHit, CSWMGTargettingInfo* pInfo,
                                          CResRef* pFireSound)
{
    if (nBank < 0) {
        return 0;
    }
    CSWMGGunBank* pBank;
    if (nBank >= m_lstGunBanks.num) {
        for (int i = m_lstGunBanks.num; i < nBank; i++) {
            m_lstGunBanks.Add(0);
        }
        pBank = new CSWMGTargettingGunBank(this, pBulletModel, pGunModel, pHit, pInfo,
                                           m_pMiniGame->m_pPlayer);
        pBank->m_cFireSound = *pFireSound;
        m_lstGunBanks.Add(pBank);
    }
    else if (m_lstGunBanks.element[nBank] == 0) {
        pBank = new CSWMGTargettingGunBank(this, pBulletModel, pGunModel, pHit, pInfo,
                                           m_pMiniGame->m_pPlayer);
        m_lstGunBanks.Insert(pBank, nBank);
        m_lstGunBanks.DelIndex(nBank + 1);
    }
    else {
        char sz[64];
        sprintf(sz, "CSWMiniEnemy: Recreating gun bank %d", nBank);
        return 0;
    }
    return pBank;
}

#pragma auto_inline(off)

// FUNCTION: SWKOTOR 0x0066FB20
int CSWMiniEnemy::LoadGunBank(CResGFF* pGFF, CResStruct* pStruct)
{
    CResRef cBulletModel;
    CResRef cGunModel;
    CResRef cFireSound;
    CSWMiniHitInfo cHit;
    CSWMGTargettingInfo cInfo;
    int bSuccess;
    CResStruct cBullet;
    int nBank = pGFF->ReadFieldDWORD(pStruct, "BankID", bSuccess, 0xffffffff);
    if (nBank == -1) {
        return 0;
    }
    {
        CResRef cDefault("");
        cGunModel = pGFF->ReadFieldCResRef(pStruct, "Gun_Model", bSuccess, cDefault);
    }
    if (!cGunModel.IsValid()) {
        return 0;
    }
    cInfo.m_fSensingRadius = pGFF->ReadFieldFLOAT(pStruct, "Sensing_Radius", bSuccess, 0.0f);
    if (!bSuccess) {
        return 0;
    }
    cInfo.m_fHorizSpread = pGFF->ReadFieldFLOAT(pStruct, "Horiz_Spread", bSuccess, 0.0f);
    if (!bSuccess) {
        return 0;
    }
    cInfo.m_fVertSpread = pGFF->ReadFieldFLOAT(pStruct, "Vert_Spread", bSuccess, 0.0f);
    if (!bSuccess) {
        return 0;
    }
    cInfo.m_fInaccuracy = pGFF->ReadFieldFLOAT(pStruct, "Inaccuracy", bSuccess, 0.0f);
    bSuccess = pGFF->GetStructFromStruct(&cBullet, pStruct, "Bullet");
    if (!bSuccess) {
        return 0;
    }
    {
        CResRef cDefault("");
        cBulletModel = pGFF->ReadFieldCResRef(&cBullet, "Bullet_Model", bSuccess, cDefault);
    }
    if (!cBulletModel.IsValid()) {
        return 0;
    }
    cHit.m_nDamage = pGFF->ReadFieldDWORD(&cBullet, "Damage", bSuccess, 0);
    if (!bSuccess) {
        return 0;
    }
    cHit.m_fLifespan = pGFF->ReadFieldFLOAT(&cBullet, "Lifespan", bSuccess, 0.0f);
    if (!bSuccess) {
        return 0;
    }
    cHit.m_fRateOfFire = pGFF->ReadFieldFLOAT(&cBullet, "Rate_Of_Fire", bSuccess, 0.0f);
    if (!bSuccess) {
        return 0;
    }
    cHit.m_fSpeed = pGFF->ReadFieldFLOAT(&cBullet, "Speed", bSuccess, 0.0f);
    if (!bSuccess) {
        return 0;
    }
    cHit.m_nTargetMask = pGFF->ReadFieldDWORD(&cBullet, "Target_Type", bSuccess, 0);
    if (!bSuccess) {
        return 0;
    }
    {
        CResRef cDefault("");
        cHit.m_cSound = pGFF->ReadFieldCResRef(&cBullet, "Collision_Sound", bSuccess, cDefault);
    }
    {
        CResRef cDefault("");
        cFireSound = pGFF->ReadFieldCResRef(pStruct, "Fire_Sound", bSuccess, cDefault);
    }
    CreateGunBank(nBank, &cBulletModel, &cGunModel, &cHit, &cInfo, &cFireSound);
    return 1;
}

// FUNCTION: SWKOTOR 0x00673A30
CSWMGTargettingInfo::CSWMGTargettingInfo()
{
    m_fHorizSpread = 0.0f;
    m_fVertSpread = 0.0f;
    m_fSensingRadius = 0.0f;
    m_fInaccuracy = 0.0f;
}

#pragma auto_inline(on)

// FUNCTION: SWKOTOR 0x00670B10
int CSWMiniEnemy::GetTargetMask()
{
    return 2;
}

// Not in the original: makes this unit emit the implicit destructor and
// ??_G below.
CSWMiniEnemy* NewMiniEnemyForDtor(CSWCMiniGame* pGame)
{
    return new CSWMiniEnemy(pGame);
}

// FUNCTION: SWKOTOR 0x006714D0 SYMBOL: ??1CSWMiniEnemy@@UAE@XZ

// FUNCTION: SWKOTOR 0x006714B0 SYMBOL: ??_GCSWMiniEnemy@@UAEPAXI@Z
