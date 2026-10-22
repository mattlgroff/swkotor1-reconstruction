// Mini-game objects, round 2: CSWMiniGameObject ctor / dtor, CSWTrackFollower
// lifecycle and hook / gun-bank helpers, CSWMiniPlayer and CSWMiniObstacle
// slots, and the anim-event callbacks (0x0066C540-0x0066E270).

#include "m/sw_mini_game_object.h"

#pragma auto_inline(off)

// FUNCTION: SWKOTOR 0x0066C540
CSWMiniGameObject::CSWMiniGameObject(int nType)
{
    m_nId = g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->GetFreeSlot();
    g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->m_pObjects[m_nId] = this;
    m_nObjectType = nType;
    m_n0C = 0;
    m_pRef18 = 0;
    m_nTarget4C = 0xff;
    m_nTarget50 = 0xff;
}

// FUNCTION: SWKOTOR 0x0066C610
// NONMATCH (4 diff insns): in the inlined SafePointer destructor the
// original computes the list `this` (`add ecx, 4`) before pushing the
// argument; plain, local-pointer and RemoveRef-helper forms all push first.
CSWMiniGameObject::~CSWMiniGameObject()
{
    g_pAppManager->GetClientExoApp()->GetMiniGameObjects()->m_pObjects[m_nId] = 0;
}

// FUNCTION: SWKOTOR 0x0066C6A0
void StartTrackCallback(CAurObject* pModel, char* szEvent, void* pUser)
{
    Vector vPos;
    Quaternion qOri;
    pModel->GetNode("modelhook")->GetTransform(&vPos, &qOri);
    ((CSWMiniPlayer*)pUser)->m_vStartTrack = vPos;
}

// FUNCTION: SWKOTOR 0x0066C720
int IsGunBankPart(CAurPart* pPart)
{
    return !_strnicmp(pPart->GetPartName(), "gunbank", 7);
}

// FUNCTION: SWKOTOR 0x0066CE60
void CSWMiniPlayer::SetBank(int n, int bRelative)
{
    char sz[12];
    if (bRelative) {
        m_nBank += n;
    }
    else {
        if (m_nBank == n) {
            return;
        }
        m_nBank = n;
    }
    if (m_nBank < -10) {
        m_nBank = -10;
    }
    else if (m_nBank > 10) {
        m_nBank = 10;
    }
    if (m_nBank) {
        sprintf(sz, "Bank%c_%02d", m_nBank < 0 ? 'L' : 'R', abs(m_nBank));
    }
    else {
        sprintf(sz, "Ready_01");
    }
    PlayAnimation(sz, 1.0f, 0x10);
}

// FUNCTION: SWKOTOR 0x0066CF20
void CSWMiniPlayer::SetAxisModeV(int n)
{
    if (n >= -1 && n != 0) {
        m_nAxisMode = n;
    }
}

// FUNCTION: SWKOTOR 0x0066D0B0
int CSWTrackFollower::LoadFromGFF(CResGFF* pGFF, CResStruct* pStruct)
{
    CResStruct cScripts;
    if (pGFF->GetStructFromStruct(&cScripts, pStruct, "Scripts")) {
        LoadScripts(pGFF, (int)pStruct, &cScripts);
    }
    RunCreateScript();
    return 1;
}

// FUNCTION: SWKOTOR 0x0066D100
CResRef CSWMiniObstacle::GetScript(int nScript)
{
    if (nScript >= 0 && nScript < 5) {
        return m_aScripts[nScript];
    }
    return CResRef();
}

// FUNCTION: SWKOTOR 0x0066D150
void CSWMiniObstacle::SetScript(int nScript, const CResRef& cScript)
{
    if (nScript >= 0 && nScript < 5) {
        m_aScripts[nScript] = cScript;
    }
}

// FUNCTION: SWKOTOR 0x0066D180
void CSWMiniObstacle::LoadScripts(CResGFF* pGFF, int nUnused, CResStruct* pStruct)
{
    CSWMiniGameObject::LoadScripts(pGFF, nUnused, pStruct);
}

// FUNCTION: SWKOTOR 0x0066D2F0
CAurObject* CSWTrackFollower::SetModel(CAurObject* pModel, int n)
{
    if (m_pModel && m_pModel != pModel) {
        delete m_pModel;
    }
    m_pModel = pModel;
    m_nFlags &= ~1;
    if (pModel) {
        CAurNode* pNode = pModel->GetNode("modelhook");
        Vector vPos;
        Quaternion qOri;
        pNode->GetTransform(&vPos, &qOri);
        if (AsMiniPlayer()) {
            m_pModel->AddEventCallback(StartTrackCallback, "starttrack", this, 9999.0f, 0);
        }
        int nCount = m_lstHooked.num;
        for (int i = 0; i < nCount; i++) {
            CSWMiniHookEntry e = m_lstHooked.element[i];
            if (e.m_bAttach) {
                e.m_pModel->AttachToObject(m_pModel, "modelhook", n);
            }
            else {
                e.m_pModel->AttachToObject(m_pModel, "modelhook", 0);
            }
            e.m_pModel->SetLocalPosition(vPos);
        }
        if (AsMiniPlayer() && ((CSWMiniPlayer*)this)->m_pHook) {
            ((CSWMiniPlayer*)this)->m_pHook->AttachToObject(m_pModel, "modelhook", n);
        }
    }
    return m_pModel;
}

// FUNCTION: SWKOTOR 0x0066D490
void CSWTrackFollower::Reset()
{
    PlayAnimation("damage", 1.0f, 1);
    PlayAnimation("Ready_01", 1.0f, 0x20);
    int nCount = m_lstGunBanks.num;
    for (int i = 0; i < nCount; i++) {
        m_lstGunBanks.element[i]->Reset();
    }
}

// FUNCTION: SWKOTOR 0x0066D4E0
int CSWTrackFollower::HitCheckBullet(CAurObject* pBullet)
{
    CSWMiniBulletInfo* pInfo = pBullet->GetUserData(0xaaab);
    if (pInfo && pInfo->m_pOwner == this) {
        return 0;
    }
    int nCount = m_lstHooked.num;
    if (nCount > 0) {
        for (int i = 0; i < nCount; i++) {
            if (m_pMiniGame->HitCheckBullet(pBullet, m_lstHooked.element[i].m_pModel, this)) {
                return 1;
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0066D550
void CSWTrackFollower::PlayAnimation(const char* szAnim, float fSpeed, int nFlags)
{
    int nCount = m_lstHooked.num;
    for (int i = 0; i < nCount; i++) {
        m_lstHooked.element[i].m_pModel->PlayAnimation(szAnim, fSpeed, nFlags, 0);
    }
    if (AsMiniPlayer() && AsMiniPlayer()->m_pHook) {
        AsMiniPlayer()->GetHook()->PlayAnimation(szAnim, fSpeed, nFlags, 0);
    }
}

// FUNCTION: SWKOTOR 0x0066D5D0
Vector CSWTrackFollower::GetHookPosition()
{
    Vector v;
    if (m_lstHooked.num) {
        v = m_lstHooked.element[0].m_pModel->GetPosition();
    }
    return v;
}

// FUNCTION: SWKOTOR 0x0066DC50
void CSWTrackFollower::LoadGunBanks()
{
    int nCount = m_lstGunBanks.num;
    for (int i = 0; i < nCount; i++) {
        m_lstGunBanks.element[i]->LoadResources();
    }
}

// FUNCTION: SWKOTOR 0x0066DC80
CExoString CSWMiniObstacle::GetModelName()
{
    if (m_pModel) {
        return CExoString(m_pModel->GetName());
    }
    return CExoString("");
}

// FUNCTION: SWKOTOR 0x0066DCC0 SYMBOL: ?Remove@?$CExoArrayList@PAVCSWMGGunBank@@@@QAEXPAVCSWMGGunBank@@@Z
template <class T>
void CExoArrayList<T>::Remove(T t) throw()
{
    int i;
    for (i = 0; i < num; i++) {
        if (element[i] == t) {
            break;
        }
    }
    num--;
    for (; i < num; i++) {
        element[i] = element[i + 1];
    }
}

template void CExoArrayList<CSWMGGunBank*>::Remove(CSWMGGunBank* t);

// FUNCTION: SWKOTOR 0x0066DD10 SYMBOL: ?Add@?$CExoArrayList@UCSWMiniHookEvent@@@@QAEXUCSWMiniHookEvent@@@Z
template void CExoArrayList<CSWMiniHookEvent>::Add(CSWMiniHookEvent t);

// FUNCTION: SWKOTOR 0x0066DE50
void TrackFollowerPartDie(CAurObject* pPart, void* pUnused, CSWTrackFollower* pFollower)
{
    if (!pFollower) {
        if (pPart) {
            delete pPart;
        }
        return;
    }
    pFollower->RemoveHookedModel(pPart);
    if (pPart) {
        delete pPart;
    }
    if (pFollower->m_lstHooked.num == 0) {
        CSWCMiniGame* pGame = pFollower->m_pMiniGame;
        if (pFollower->m_nFlags & 2) {
            int nCount = pFollower->m_lstGunBanks.num;
            for (int i = 0; i < nCount; i++) {
                pFollower->m_lstGunBanks.element[i]->m_pOwner = 0;
            }
        }
        if (pGame->m_pPlayer == (CSWMiniPlayer*)pFollower) {
            pGame->SetPlayer(0);
            return;
        }
        pGame->RemoveObject(pFollower);
        delete pFollower;
    }
}

// FUNCTION: SWKOTOR 0x0066E110
void CSWTrackFollower::RemoveGunBank(CSWMGGunBank* p)
{
    if (m_nFlags & 2) {
        m_lstGunBanks.Remove(p);
    }
}

// FUNCTION: SWKOTOR 0x0066E1F0
CAurObject* CSWTrackFollower::AddModel(CAurObject* p, unsigned char bAttach)
{
    if (!p) {
        return 0;
    }
    for (int i = 0; i < m_lstHooked.num; i++) {
        if (m_lstHooked.element[i].m_pModel == p) {
            return p;
        }
    }
    CSWMiniHookEntry e;
    e.m_pModel = p;
    e.m_bAttach = bAttach;
    m_lstHooked.Add(e);
    if (m_pModel) {
        p->AttachToObject(m_pModel, "modelhook", 0);
    }
    return p;
}

// FUNCTION: SWKOTOR 0x0066E2A0
// NONMATCH (11 diff insns): only the 16-byte copy of m_acSoundNames[0]
// differs; the original copies dword by dword (load/store pairs), ours
// interleaves the loads. memcpy was worse.
void CSWTrackFollower::OnDeath(int bSkipScript)
{
    if (!bSkipScript) {
        CResRef cScript = GetScript(6);
        if (cScript.IsValid()) {
            CExoString sScript;
            cScript.CopyToString(sScript);
            g_pVirtualMachine->RunScript(&sScript, m_nId, 1);
            return;
        }
    }
    StopTrackAnimation();
    CResRef cSound = m_acSoundNames[0];
    if (cSound.IsValid()) {
        unsigned char nGroup = AsMiniPlayer() ? 0x17 : 0x18;
        if (cSound == "mgs_accelpad") {
            g_pExoSound->PlayOneShotSound(cSound, nGroup, 0, g_nMiniGameAccelVolume, 0, 0);
        }
        else {
            g_pExoSound->PlayOneShotSound(cSound, nGroup, 0, 100, 0, 0);
        }
    }
    int i;
    for (i = m_lstHooked.num - 1; i >= 0; i--) {
        CAurObject* p = m_lstHooked.element[i].m_pModel;
        p->PlayAnimation(0, 1.0f, 0, 0);
        if (p->PlayAnimation("die", 1.0f, 1, 0)) {
            p->AddEventCallback((AurEventCallback)TrackFollowerPartDie, "donedie", this, 9999.0f, 0);
        }
        else {
            RemoveHookedModel(p);
            delete p;
        }
    }
    int nCount = m_lstGunBanks.num;
    for (i = 0; i < nCount; i++) {
        m_lstGunBanks.element[i]->Die();
    }
    if (m_lstHooked.num == 0) {
        if (m_pMiniGame->m_pPlayer == (CSWMiniPlayer*)this) {
            m_pMiniGame->SetPlayer(0);
        }
        else {
            m_n0C = 1;
        }
    }
    m_pMiniGame->RemoveObject(this);
}

// FUNCTION: SWKOTOR 0x0066E490
int CSWTrackFollower::AttachGunParts(CAurObject* pModel)
{
    CExoLocalList<CAurPart*> lstParts;
    pModel->FindParts(IsGunBankPart, &lstParts);
    int nAttached = 0;
    for (int i = 0; i < lstParts.num; i++) {
        int nBank;
        if (sscanf(lstParts.element[i]->GetPartName(), "gunbank%d-", &nBank) != 1) {
            char sz[128];
            sprintf(sz, "CSWTrackFollower: Could not find bank ID in attach part name %s.",
                    lstParts.element[i]->GetPartName());
        }
        else if (nBank < 0) {
            char sz[128];
            sprintf(sz, "CSWTrackFollower: Bank ID %d < 0.", nBank);
        }
        else if (nBank < m_lstGunBanks.num) {
            m_lstGunBanks.element[nBank]->AttachGun(pModel, lstParts.element[i]);
            nAttached++;
        }
        else {
            char sz[128];
            sprintf(sz, "CSWTrackFollower: Trying to attach gun to a non-existent bank ID: %d", nBank);
        }
    }
    return nAttached;
}

// FUNCTION: SWKOTOR 0x0066E5C0
void CSWTrackFollower::OnHitObstacle(int bSkipScript)
{
    if (!bSkipScript) {
        CResRef cScript = GetScript(8);
        if (cScript.IsValid()) {
            CExoString sScript;
            cScript.CopyToString(sScript);
            g_pVirtualMachine->RunScript(&sScript, m_nId, 1);
            return;
        }
    }
    int nCount = m_lstHooked.num;
    for (int i = 0; i < nCount; i++) {
        m_lstHooked.element[i].m_pModel->SetBehavior(0);
    }
    if (!(m_nFlags & 2)) {
        m_nFlags |= 2;
        OnDeath(0);
    }
}

// FUNCTION: SWKOTOR 0x0066E6B0
int CSWTrackFollower::DoObstacleHitCheck(CSWMiniObstacle* pObstacle)
{
    CAurObject* pModel = pObstacle->m_pModel;
    if (pModel) {
        CAurNode* pNode = pModel->GetNode("hitbump");
        if (pNode && m_lstHooked.num) {
            CAurObject* pHook = m_lstHooked.element[0].m_pModel;
            Vector vFrom = pHook->GetHitOrigin();
            Vector vTo = pHook->GetPosition();
            if (pModel->HitTest(pNode, vFrom, vTo, 0, &g_vMiniHitPoint)) {
                m_nTarget170 = pObstacle->m_nId;
                OnHitObstacle(0);
                pObstacle->m_nTarget50 = m_nId;
                pObstacle->RunHitFollowerScript(0);
                return 1;
            }
        }
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0066E7A0
void CSWTrackFollower::ApplyDamage(int bSkipScript)
{
    if (!bSkipScript) {
        CResRef cScript = GetScript(5);
        if (cScript.IsValid()) {
            CExoString sScript;
            cScript.CopyToString(sScript);
            g_pVirtualMachine->RunScript(&sScript, m_nId, 1);
            return;
        }
    }
    m_n8C += m_n19C;
    if (m_n19C < 0) {
        if (m_n8C <= 0) {
            if (!(m_nFlags & 2)) {
                m_nFlags |= 2;
                OnDeath(0);
            }
        }
        else {
            PlayAnimation("damage", 1.0f, 1);
            PlayAnimation("Ready_01", 1.0f, 0x20);
        }
    }
    else {
        m_n8C = m_n8C < m_n90 ? m_n8C : m_n90;
    }
}

// FUNCTION: SWKOTOR 0x0066E900
CExoSoundSource* CSWTrackFollower::SetSound(int n, const CResRef& cRes)
{
    if (n < 0) {
        return 0;
    }
    if (n < 1) {
        if (m_apSources[n]) {
            int bPlaying = m_apSources[n]->IsHardwarePlayingSoundPleaseDontUseThis();
            if (bPlaying) {
                m_apSources[n]->Stop();
            }
            if (cRes.IsValid()) {
                m_apSources[n]->SetResRef(cRes, 1);
                m_apSources[n]->Set3D(1);
                m_apSources[n]->SetPosition(GetHookPosition(), 0);
                m_apSources[n]->SetVolume((unsigned char)m_aSounds[n].m_nVolume);
                if (AsMiniPlayer()) {
                    m_apSources[n]->SetPriorityGroup(0x17);
                }
                else {
                    m_apSources[n]->SetPriorityGroup(0x18);
                }
                if (bPlaying) {
                    m_apSources[n]->Play();
                }
            }
            else {
                if (m_apSources[n]) {
                    delete m_apSources[n];
                }
                m_apSources[n] = 0;
            }
        }
        else if (cRes.IsValid()) {
            m_apSources[n] = new CExoSoundSource(cRes);
            m_apSources[n]->Set3D(1);
            m_apSources[n]->SetPosition(GetHookPosition(), 0);
            m_apSources[n]->SetVolume((unsigned char)m_aSounds[n].m_nVolume);
            if (AsMiniPlayer()) {
                m_apSources[n]->SetPriorityGroup(0x17);
            }
            else {
                m_apSources[n]->SetPriorityGroup(0x18);
            }
        }
        return m_apSources[n];
    }
    if (n < 2) {
        m_acSoundNames[n - 1] = cRes;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0066F7E0
void CSWTrackFollower::LoadSounds(CResGFF* pGFF, int nUnused, CResStruct* pStruct)
{
    int bSuccess;
    CExoSoundSource* pSource;
    {
        CResRef cDefault("");
        pSource = SetSound(0, pGFF->ReadFieldCResRef(pStruct, "Engine", bSuccess, cDefault));
    }
    if (pSource) {
        pSource->SetLooping(1);
    }
    {
        CResRef cDefault("");
        SetSound(1, pGFF->ReadFieldCResRef(pStruct, "Death", bSuccess, cDefault));
    }
}

// FUNCTION: SWKOTOR 0x0066F440
CSWMGGunBank* CSWMiniPlayer::CreateGunBank(int nBank, CResRef* pBulletModel, CResRef* pGunModel,
                                           CSWMiniHitInfo* pHit, CResRef* pFireSound)
{
    if (nBank < 0) {
        return 0;
    }
    CSWMGGunBank* pBank;
    if (nBank >= m_lstGunBanks.num) {
        for (int i = m_lstGunBanks.num; i < nBank; i++) {
            m_lstGunBanks.Add(0);
        }
        pBank = new CSWMGGunBank(this, pBulletModel, pGunModel, pHit);
        pBank->m_cFireSound = *pFireSound;
        m_lstGunBanks.Add(pBank);
    }
    else if (m_lstGunBanks.element[nBank] == 0) {
        pBank = new CSWMGGunBank(this, pBulletModel, pGunModel, pHit);
        pBank->m_cFireSound = *pFireSound;
        m_lstGunBanks.Insert(pBank, nBank);
        m_lstGunBanks.DelIndex(nBank + 1);
    }
    else {
        char sz[64];
        sprintf(sz, "CSWMiniPlayer: Recreating gun bank %d", nBank);
        return 0;
    }
    return pBank;
}

// FUNCTION: SWKOTOR 0x0066F890
int CSWMiniPlayer::LoadGunBank(CResGFF* pGFF, CResStruct* pStruct)
{
    CResRef cBulletModel;
    CResRef cGunModel;
    CResRef cFireSound;
    CSWMiniHitInfo cHit;
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
    CreateGunBank(nBank, &cBulletModel, &cGunModel, &cHit, &cFireSound);
    return 1;
}

// FUNCTION: SWKOTOR 0x0066FFF0
// NONMATCH (11 diff insns): stack slots only. The original keeps the list
// element CResStruct in the dead pGFF argument slot and the list count in
// the frame; ours swaps them (declaration orders and shared-struct forms
// tried).
int CSWTrackFollower::Load(CResGFF* pGFF, CResStruct* pStruct)
{
    int bSuccess;
    int nCount;
    CResList cList;
    int n = pGFF->ReadFieldDWORD(pStruct, "Hit_Points", bSuccess, 0);
    if (n > 0) {
        m_n8C = n;
    }
    n = pGFF->ReadFieldDWORD(pStruct, "Max_HPs", bSuccess, 0);
    if (n > 0) {
        m_n90 = n;
    }
    float f = pGFF->ReadFieldFLOAT(pStruct, "Bump_Damage", bSuccess, -1.0f);
    if (f >= 0.0f) {
        m_f84 = f;
    }
    SetAxisModeV(pGFF->ReadFieldINT(pStruct, "Sphere_Radius", bSuccess, -10));
    m_n94 = pGFF->ReadFieldINT(pStruct, "Invince_Period", bSuccess, 0);
    f = pGFF->ReadFieldFLOAT(pStruct, "Invince_Period", bSuccess, 0.0f);
    if (f >= 0.0f) {
        m_fA0 = f;
    }
    bSuccess = pGFF->GetList(&cList, pStruct, "Gun_Banks");
    if (bSuccess) {
        nCount = pGFF->GetListCount(&cList);
        int i;
        for (i = 0; i < nCount; i++) {
            CResStruct cBank;
            bSuccess = pGFF->GetListElement(&cBank, &cList, i);
            if (bSuccess) {
                LoadGunBank(pGFF, &cBank);
            }
        }
        if (nCount) {
            nCount = m_lstHooked.num;
            for (i = 0; i < nCount; i++) {
                AttachGunParts(m_lstHooked.element[i].m_pModel);
            }
        }
    }
    CResStruct cStruct;
    if (pGFF->GetStructFromStruct(&cStruct, pStruct, "Scripts")) {
        LoadScripts(pGFF, (int)pStruct, &cStruct);
    }
    if (pGFF->GetStructFromStruct(&cStruct, pStruct, "Sounds")) {
        LoadSounds(pGFF, (int)pStruct, &cStruct);
    }
    RunCreateScript();
    return 1;
}

#pragma auto_inline(on)

// FUNCTION: SWKOTOR 0x0066E130
void CSWTrackFollower::RunHeartbeatScript(float fDelta)
{
    CSWMiniGameObject::RunHeartbeatScript(fDelta);
    if (m_f9C > 0.0f) {
        float f = m_f9C - fDelta;
        m_f9C = f > 0.0f ? f : 0.0f;
    }
    if (m_apSources[0]) {
        m_apSources[0]->SetPosition(GetHookPositionInline(), 0);
    }
}

struct CSWMiniHookEventSrc {
    void* m_p00;
    Vector* m_pPos;
    Quaternion* m_pOri;
    unsigned char m_bSecond;
};

// FUNCTION: SWKOTOR 0x0066DD60
void HookEventCallback(void* pA, void* pB, CSWMiniHookEventSrc* pSrc)
{
    CSWCMiniGame* pGame = g_pAppManager->GetClientExoApp()->GetMiniGame();
    if (pGame->m_bActive) {
        CSWMiniPlayer* pPlayer = pGame->m_pPlayer;
        CSWMiniHookEvent e;
        e.m_p00 = pSrc->m_p00;
        e.m_bHasPos = pSrc->m_pPos != 0;
        e.m_bHasOri = pSrc->m_pOri != 0;
        if (pSrc->m_pOri) {
            e.m_qOri = *pSrc->m_pOri;
        }
        if (pSrc->m_pPos) {
            e.m_vPos = *pSrc->m_pPos;
        }
        if (pSrc->m_bSecond) {
            pPlayer->m_lstEvents1A0.Add(e);
        }
        else {
            pPlayer->m_lstEvents1AC.Add(e);
        }
    }
}

// FUNCTION: SWKOTOR 0x0066FE50
// NONMATCH (8 diff insns): in the "loop" branch the original spills
// m_fSpeed into the dead pFollower argument slot before SetSpeed; float
// locals and an inline getter get a new frame slot instead (frame 0x38).
void TrackFollowerAnimEvent(CAurObject* pModel, char* szEvent, CSWTrackFollower* pFollower)
{
    if (!_strnicmp(szEvent, "custom_", 7)) {
        szEvent += 7;
        CResRef cScript = pFollower->GetScript(3);
        CExoString sEvent(szEvent);
        pFollower->m_pRef18.Set((CSWMiniRefTarget*)pModel);
        pFollower->m_sTag = sEvent;
        if (cScript.IsValid()) {
            CExoString sScript;
            cScript.CopyToString(sScript);
            unsigned long nId = pFollower->m_nId;
            g_pVirtualMachine->RunScript(&sScript, nId, 1);
        }
    }
    else if (!_strnicmp(szEvent, "loop", 4)) {
        CSWTrackFollower* p = pFollower->AsTrackFollower();
        if (p) {
            if (p->m_nAxisMode != -1) {
                p->SetAxisModeV(p->m_nAxisMode - 1);
                p->SetSpeed(p->m_fSpeed);
            }
            CResRef cScript = p->GetScript(9);
            if (cScript.IsValid()) {
                CExoString sScript;
                cScript.CopyToString(sScript);
                unsigned long nId = p->m_nId;
                g_pVirtualMachine->RunScript(&sScript, nId, 1);
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x006701C0
// NONMATCH (12 diff insns): the original sets the running flag with
// `or dword ptr [esi+0x88], 1`; ours loads / ors / stores it around the
// m_apSources[0] load (|= and the bitfield give the same).
void CSWTrackFollower::Go()
{
    if (m_pModel && !(m_nFlags & 1)) {
        int nCount = m_lstHooked.num;
        int nFlags = 4;
        if (m_nAxisMode == 1) {
            nFlags = 5;
        }
        for (int i = 0; i < nCount; i++) {
            CAurObject* pHooked = m_lstHooked.element[i].m_pModel;
            pHooked->AddEventCallback((AurEventCallback)TrackFollowerAnimEvent, "*", this, 9999.0f, 0);
            pHooked->PlayAnimation("Ready_01", 1.0f, 0, 0);
        }
        if (AsMiniPlayer()) {
            m_pModel->AddEventCallback((AurEventCallback)HookEventCallback, "ReportKey", 0, 9999.0f, 0);
            nFlags |= 0x100;
        }
        m_pModel->AddEventCallback((AurEventCallback)TrackFollowerAnimEvent, "*", this, 9999.0f, 0);
        m_pModel->PlayAnimation("track", m_fSpeed * 0.01f, nFlags, 0);
        m_bTrackActive = 1;
        CExoSoundSource* pSource = m_apSources[0];
        if (pSource && !pSource->IsHardwarePlayingSoundPleaseDontUseThis()) {
            pSource->SetPosition(GetHookPosition(), 0);
            pSource->Play();
        }
    }
}
