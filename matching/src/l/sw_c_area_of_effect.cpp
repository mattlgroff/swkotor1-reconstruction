// The client area-of-effect object CSWCAreaOfEffectObject (vtable
// 0x00757E28): model teardown, per-instance animation, duration and move
// timer bookkeeping.
#include "l/sw_c_area_of_effect.h"

static inline CClientExoApp* GetClientExoApp()
{
    return g_pAppManager->m_pClientExoApp;
}

// FUNCTION: SWKOTOR 0x006D90D0
void CSWCAreaOfEffectObject::ReleaseModel()
{
    if (m_pnAnimationLengths) {
        delete[] m_pnAnimationLengths;
        m_pnAnimationLengths = 0;
    }
    if (m_pnAnimations) {
        delete[] m_pnAnimations;
        m_pnAnimations = 0;
    }
    if (m_pbPlaced) {
        delete[] m_pbPlaced;
        m_pbPlaced = 0;
    }
    if (m_pnMoveTimers) {
        delete[] m_pnMoveTimers;
        m_pnMoveTimers = 0;
    }
    if (m_pnDurations) {
        delete[] m_pnDurations;
        m_pnDurations = 0;
    }
    if (m_pAnimationBase) {
        for (int i = 0; i < m_nInstances; i++) {
            m_pAnimationBase[i].UnloadModel();
        }
        delete[] m_pAnimationBase;
        m_pAnimationBase = 0;
    }
    if (m_pVisualEffect) {
        delete m_pVisualEffect;
        m_pVisualEffect = 0;
    }
}

// FUNCTION: SWKOTOR 0x006D91D0
void CSWCAreaOfEffectObject::UnloadModel(int nPart)
{
    if ((unsigned char)nPart == 0xff) {
        ReleaseModel();
    }
}

// FUNCTION: SWKOTOR 0x006D91F0
int CSWCAreaOfEffectObject::SetAnimation(unsigned short nAnim, unsigned char nInstance)
{
    if (!m_pAnimationBase) {
        return 0;
    }
    m_pnAnimations[nInstance] = nAnim;
    if (nAnim == 1) {
        m_pnAnimationLengths[nInstance] = (int)m_pAnimationBase[nInstance].GetAnimationLength(1);
        if (m_pnAnimationLengths[nInstance] == 0) {
            m_pnAnimationLengths[nInstance] = 1;
        }
        return m_pAnimationBase[nInstance].PlayAnimation(1, 1.0f, 1, 0);
    }
    if (nAnim == 3) {
        m_pnAnimationLengths[nInstance] = (int)m_pAnimationBase[nInstance].GetAnimationLength(3);
        if (m_pnAnimationLengths[nInstance] == 0) {
            m_pnAnimationLengths[nInstance] = 1;
        }
        return m_pAnimationBase[nInstance].PlayAnimation(3, 1.0f, 1, 0);
    }
    m_pnAnimationLengths[nInstance] = 0;
    return m_pAnimationBase[nInstance].PlayAnimation(nAnim, 1.0f, 0, 0);
}

// FUNCTION: SWKOTOR 0x006D9300
void CSWCAreaOfEffectObject::ResetInstanceDuration(int nInstance)
{
    if (m_pAnimationBase) {
        int nTotal = 0;
        unsigned char nGroup;
        for (nGroup = 0; nGroup < 3; nGroup++) {
            nTotal += m_anGroupInstances[nGroup];
            if (nInstance < nTotal) {
                break;
            }
        }
        if (nGroup != 3) {
            m_pnDurations[nInstance] = m_anGroupDurations[nGroup];
        }
    }
}

// FUNCTION: SWKOTOR 0x006D9350
void CSWCAreaOfEffectObject::StopEffect()
{
    CExoString sCessation;
    if (g_pRules->Get2DArrays()->GetVisualEffects()->GetCExoStringEntry(m_nEffectRow, "SoundCessation",
                                                                        &sCessation)) {
        sCessation = "";
    }
    g_pExoSound->Play3DOneShotSound(sCessation, m_vPosition, 1.5f, 0xc, 0, 0, 0.0f, 0.0f);
    if (m_pSound) {
        m_pSound->Stop();
        if (m_pSound) {
            delete m_pSound;
        }
        m_pSound = 0;
    }
    m_bEnding = 1;
    if (m_pVisualEffect) {
        m_pVisualEffect->SetAnimation(3);
    }
    for (int i = 0; i < m_nInstances; i++) {
        SetAnimation(3, (unsigned char)i);
        m_pnDurations[i] = 0;
    }
    m_nFinishTimer = 3000;
}

// FUNCTION: SWKOTOR 0x006D9520
void CSWCAreaOfEffectObject::SetMoveTimer(unsigned long nTimer, CAurObject* pModel)
{
    if (m_pAnimationBase) {
        int i;
        for (i = 0; i < m_nInstances; i++) {
            if (m_pAnimationBase[i].GetModel(0xff) == pModel) {
                break;
            }
        }
        if (i != m_nInstances) {
            m_pnMoveTimers[i] = nTimer;
        }
    }
}

// The inlined copy of CSWCArea::GetEnvMap (0x0063BFA0).
static inline CResRef GetAreaEnvMap(CSWCArea* pArea)
{
    return pArea->m_cEnvMap;
}

// FUNCTION: SWKOTOR 0x006D9590
int CSWCAreaOfEffectObject::SetEnvMap(int bEnable, int bStore, const CResRef& cEnvMap)
{
    CExoString sEnvMap;
    if (!m_pAnimationBase) {
        return 0;
    }
    CSWCModule* pModule = g_pAppManager->m_pClientExoApp->GetModule();
    if (cEnvMap == "DEFAULT") {
        CSWCArea* pArea;
        if (pModule && (pArea = pModule->m_pArea) != 0) {
            GetAreaEnvMap(pArea).CopyToString(sEnvMap);
        }
        if (sEnvMap.GetLength() == 0) {
            return 0;
        }
    } else {
        cEnvMap.CopyToString(sEnvMap);
    }
    for (int i = 0; i < m_nInstances; i++) {
        CResRef cResRef(sEnvMap);
        m_pAnimationBase[i].SetEnvMap(bEnable, bStore, cResRef);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x006D96E0
int CSWCAreaOfEffectObject::SetEnvMapPart(unsigned char nPart, int bEnable, int bStore, const CResRef& cEnvMap)
{
    CExoString sEnvMap;
    if (!m_pAnimationBase || nPart != 0xff) {
        return 0;
    }
    CSWCModule* pModule = g_pAppManager->m_pClientExoApp->GetModule();
    if (cEnvMap == "DEFAULT") {
        if (pModule && pModule->m_pArea) {
            pModule->m_pArea->GetEnvMap().CopyToString(sEnvMap);
        }
        if (sEnvMap.GetLength() == 0) {
            return 0;
        }
    } else {
        cEnvMap.CopyToString(sEnvMap);
    }
    for (int i = 0; i < m_nInstances; i++) {
        CResRef cResRef(sEnvMap);
        m_pAnimationBase[i].SetEnvMap(bEnable, bStore, cResRef);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x006D9820
CSWCAreaOfEffectObject::~CSWCAreaOfEffectObject()
{
    if (m_pSound) {
        delete m_pSound;
    }
    ReleaseModel();
}

// FUNCTION: SWKOTOR 0x006D9890 SYMBOL: ??_GCSWCAreaOfEffectObject@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x006DA330
void CSWCAreaOfEffectObject::UpdateInstances()
{
    int nDone = 0;
    if (m_pAnimationBase) {
        for (int i = 0; i < m_nInstances; i++) {
            if (m_pnDurations[i] == 0) {
                if (m_bEnding) {
                    nDone++;
                } else if (m_pnMoveTimers[i] > 0) {
                    if (m_pnMoveTimers[i] <= m_nDeltaTime) {
                        if (PlaceInstance(i)) {
                            if (!m_pbPlaced[i]) {
                                m_pAnimationBase[i].AttachToScene(m_pScene);
                                m_pbPlaced[i] = 1;
                            }
                            SetAnimation(1, i);
                            ResetInstanceDuration(i);
                            m_pnMoveTimers[i] = 0;
                            if (*m_cImpactSound.GetResRef() && rand() % 100 < m_nImpactSoundChance &&
                                m_pAnimationBase[i].GetModel(0xff)) {
                                g_pExoSound->Play3DOneShotSound(m_cImpactSound,
                                                                m_pAnimationBase[i].GetModel(0xff)->GetPosition(),
                                                                1.5f, 0xc, 0, 0, 0.0f, 0.0f);
                            }
                        }
                    } else {
                        m_pnMoveTimers[i] -= m_nDeltaTime;
                    }
                }
            } else if (m_pnAnimations[i] == 2) {
                if (m_pnDurations[i] <= m_nDeltaTime) {
                    SetAnimation(3, i);
                    m_pnDurations[i] = 0;
                } else {
                    m_pnDurations[i] -= m_nDeltaTime;
                }
            }
        }
    }
    int bVisualDone = 1;
    if (m_pVisualEffect) {
        bVisualDone = m_pVisualEffect->m_bDone;
    }
    if (m_bEnding && bVisualDone && nDone == m_nInstances) {
        m_bFinished = 1;
    }
}

// FUNCTION: SWKOTOR 0x006DA9B0
void CSWCAreaOfEffectObject::AIUpdate()
{
    unsigned long nDay;
    unsigned long nTime;
    unsigned long nDeltaDay;
    unsigned long nDeltaTime;
    GetClientExoApp()->GetWorldTimer()->GetWorldTime(&nDay, &nTime);
    GetClientExoApp()->GetWorldTimer()->SubtractWorldTimes(nDay, nTime, m_nCreateDay, m_nCreateTime, &nDeltaDay,
                                                           &nDeltaTime);
    m_nDeltaTime = nDeltaTime;
    if (m_oidTarget != 0x7f000000) {
        CGameObject* pObject = GetClientExoApp()->GetGameObject(m_oidTarget);
        if (pObject) {
            CSWCObject* pTarget = pObject->AsSWCObject();
            SetPosition(pTarget->m_vPosition);
        }
    }
    if (!m_bSoundStarted) {
        CExoString sImpact;
        CExoString sDuration;
        if (!g_pRules->Get2DArrays()->GetVisualEffects()->GetCExoStringEntry(m_nEffectRow, "SoundImpact", &sImpact)) {
            sImpact = "";
        }
        if (!g_pRules->Get2DArrays()->GetVisualEffects()->GetCExoStringEntry(m_nEffectRow, "SoundDuration",
                                                                             &sDuration)) {
            sDuration = "";
        }
        g_pExoSound->Play3DOneShotSound(sImpact, m_vPosition, 1.5f, 0xc, 0, 0, 0.0f, 0.0f);
        m_pSound = new CExoSoundSource(sDuration);
        if (m_pSound) {
            m_pSound->Set3D(1);
            m_pSound->SetPriorityGroup(0xc);
            m_pSound->SetLooping(1);
            m_pSound->SetPosition(m_vPosition, 1.5f);
            m_pSound->Play();
        }
        m_bSoundStarted = 1;
    }
    for (unsigned char i = 0; i < m_nInstances; i++) {
        if (m_pnAnimationLengths[i] > 0) {
            if (m_pnAnimationLengths[i] > m_nDeltaTime) {
                m_pnAnimationLengths[i] -= m_nDeltaTime;
            } else {
                m_pnAnimationLengths[i] = 0;
                if (m_pnAnimations[i] == 1) {
                    if (m_pAnimationBase) {
                        m_pnAnimations[i] = 2;
                        m_pnAnimationLengths[i] = 0;
                        m_pAnimationBase[i].PlayAnimation(2, 1.0f, 0, 0);
                    }
                } else if (m_pnAnimations[i] == 3) {
                    SetMoveTimer(1, m_pAnimationBase[i].GetModel(0xff));
                }
            }
        }
    }
    if (m_bFinished) {
        if (m_nFinishTimer <= m_nDeltaTime) {
            m_nFinishTimer = 0;
            AttachToScene(0);
            unsigned long oid = m_idSelf;
            g_pAppManager->m_pClientExoApp->GetGameObjectArray()->Delete(oid);
            delete this;
            return;
        }
        m_nFinishTimer -= m_nDeltaTime;
    } else {
        UpdateInstances();
    }
    if (m_pVisualEffect) {
        if (!m_n150) {
            m_pVisualEffect->AttachToScene(m_pScene);
            m_n150 = 1;
        }
        m_pVisualEffect->AIUpdate();
    }
    if (m_pSound) {
        m_pSound->SetPosition(m_vPosition, 1.5f);
    }
    m_nCreateDay = nDay;
    m_nCreateTime = nTime;
}

// FUNCTION: SWKOTOR 0x006D8310
CSWCAreaOfEffectObject::CSWCAreaOfEffectObject()
    : CSWCObject(0xb)
{
    unsigned long nDay;
    unsigned long nTime;
    GetClientExoApp()->GetWorldTimer()->GetWorldTime(&nDay, &nTime);
    m_nCreateDay = nDay;
    m_nCreateTime = nTime;
    m_vOrientation = Vector(0.0f, 1.0f, 0.0f);
    m_vPosition = Vector(0.0f, 0.0f, 0.0f);
    m_n124 = 0;
    m_n128 = 0;
    m_n12C = 0;
    m_n13F = 0;
    for (unsigned char i = 0; i < 3; i++) {
        m_anGroupFlags[i] = 0;
        m_anGroupInstances[i] = 0;
        m_anGroupDurations[i] = 0;
    }
    m_pnDurations = 0;
    m_nFinishTimer = 0;
    m_bFinished = 0;
    m_nInstances = 0;
    m_bEnding = 0;
    m_pbPlaced = 0;
    m_pScene = 0;
    m_pnAnimationLengths = 0;
    m_pnAnimations = 0;
    m_pnMoveTimers = 0;
    m_pVisualEffect = 0;
    m_n150 = 0;
    m_pSound = 0;
    m_cImpactSound = "";
    m_nImpactSoundChance = 0;
    m_n16C = 0;
    m_bSoundStarted = 0;
    m_oidTarget = 0x7f000000;
    m_nEffectRow = -1;
}
