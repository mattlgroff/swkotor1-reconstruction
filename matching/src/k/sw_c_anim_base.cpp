// CSWCAnimBase / CSWCAnimBaseTW model wrappers (b00698 neighbours,
// 0x0069C5D0-0x0069DE70).

#include "k/sw_c_anim_base.h"

#pragma auto_inline(off)

// FUNCTION: SWKOTOR 0x0069C5D0
void CSWCAnimBaseTW::UnloadModel()
{
    if (m_pModel2) {
        m_pModel2->AttachToObject(0, 0, 0);
        delete m_pModel2;
        m_pModel2 = 0;
    }
    if (m_pModel3) {
        m_pModel3->AttachToObject(0, 0, 0);
        delete m_pModel3;
        m_pModel3 = 0;
    }
    CSWCAnimBase::UnloadModel();
}

// FUNCTION: SWKOTOR 0x0069C640
void CSWCAnimBaseTW::UnloadModelPart(unsigned char nPart)
{
    if (nPart == 0xfd) {
        if (m_pModel2) {
            m_pModel2->AttachToObject(0, 0, 0);
            delete m_pModel2;
            m_pModel2 = 0;
            return;
        }
    }
    else if (nPart == 0xfc) {
        if (m_pModel3) {
            m_pModel3->AttachToObject(0, 0, 0);
            delete m_pModel3;
            m_pModel3 = 0;
            return;
        }
    }
    CSWCAnimBase::UnloadModelPart(nPart);
}

// FUNCTION: SWKOTOR 0x0069CD10
CSWCAnimBaseTW::~CSWCAnimBaseTW()
{
    UnloadModel();
}

// FUNCTION: SWKOTOR 0x0069CD60 SYMBOL: ??_GCSWCAnimBaseTW@@UAEPAXI@Z

// FUNCTION: SWKOTOR 0x0069CD80
void CSWCAnimBase::SetVisible(int bVisible)
{
    if (m_pModel) {
        m_pModel->SetVisible(bVisible);
    }
}

// FUNCTION: SWKOTOR 0x0069CDA0
CAurObject* CSWCAnimBase::GetModel(unsigned char nPart)
{
    if (nPart != 0xff) {
        return 0;
    }
    return m_pModel;
}

// FUNCTION: SWKOTOR 0x0069CDC0 SYMBOL: ?ReplaceTexture@CSWCAnimBase@@UAEHABVCResRef@@0H@Z
int CSWCAnimBase::ReplaceTexture(const CResRef& cOld, const CResRef& cNew, int bStore)
{
    if (!m_pModel) {
        return 0;
    }
    char szOld[20];
    char szNew[20];
    cOld.CopyToString(szOld);
    cNew.CopyToString(szNew);
    _strlwr(szNew);
    _strlwr(szOld);
    m_pModel->ReplaceTexture(szOld, szNew);
    if (bStore) {
        m_cReplaceTexture = cNew;
        m_bReplaceTextureIndexed = 0;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0069CE50 SYMBOL: ?ReplaceTexture@CSWCAnimBase@@UAEHEABVCResRef@@0H@Z
int CSWCAnimBase::ReplaceTexture(unsigned char nPart, const CResRef& cOld, const CResRef& cNew, int bStore)
{
    if (m_pModel && nPart == 0xff) {
        char szOld[20];
        char szNew[20];
        cOld.CopyToString(szOld);
        cNew.CopyToString(szNew);
        _strlwr(szNew);
        _strlwr(szOld);
        m_pModel->ReplaceTexture(szOld, szNew);
        if (bStore) {
            m_cReplaceTexture = cNew;
            m_bReplaceTextureIndexed = 0;
        }
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0069CEF0 SYMBOL: ?ReplaceTexture@CSWCAnimBase@@UAEHEABVCResRef@@0HPAGHH@Z
int CSWCAnimBase::ReplaceTexture(unsigned char nPart, const CResRef& cOld, const CResRef& cNew, int nCount, unsigned short* pIndices, int bStore, int nStart)
{
    if (m_pModel && nPart == 0xff) {
        char szOld[20];
        char szNew[20];
        cOld.CopyToString(szOld);
        cNew.CopyToString(szNew);
        _strlwr(szOld);
        _strlwr(szNew);
        m_pModel->ReplaceTextureIndexed(szOld, szNew, nCount, pIndices, nStart);
        if (bStore) {
            m_cReplaceTexture = cNew;
            m_bReplaceTextureIndexed = 1;
            m_nTextureCount = (unsigned char)nCount;
            m_nTextureStart = (unsigned char)nStart;
            memcpy(m_aTextureIndices, pIndices + nStart, nCount * 2);
        }
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0069CFD0 SYMBOL: ?ReplaceTexture@CSWCAnimBase@@UAEHABVCResRef@@0HPAGHH@Z
int CSWCAnimBase::ReplaceTexture(const CResRef& cOld, const CResRef& cNew, int nCount, unsigned short* pIndices, int bStore, int nStart)
{
    if (!m_pModel) {
        return 0;
    }
    char szOld[20];
    char szNew[20];
    cOld.CopyToString(szOld);
    cNew.CopyToString(szNew);
    _strlwr(szOld);
    _strlwr(szNew);
    m_pModel->ReplaceTextureIndexed(szOld, szNew, nCount, pIndices, nStart);
    if (bStore) {
        m_cReplaceTexture = cNew;
        m_bReplaceTextureIndexed = 1;
        m_nTextureCount = (unsigned char)nCount;
        m_nTextureStart = (unsigned char)nStart;
        memcpy(m_aTextureIndices, pIndices + nStart, nCount * 2);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0069D0A0
int CSWCAnimBase::RestoreTexture()
{
    if (!m_pModel) {
        return 0;
    }
    if (m_bReplaceTextureIndexed) {
        CResRef cEmpty("");
        ReplaceTexture(0xff, cEmpty, m_cReplaceTexture, m_nTextureCount, m_aTextureIndices, 0, m_nTextureStart);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0069D100
int CSWCAnimBase::RestoreTexturePart(unsigned char nPart)
{
    if (m_pModel && nPart == 0xff) {
        if (m_bReplaceTextureIndexed) {
            CResRef cEmpty("");
            ReplaceTexture(0xff, cEmpty, m_cReplaceTexture, m_nTextureCount, m_aTextureIndices, 0, m_nTextureStart);
            return 1;
        }
        m_pModel->RestoreTextures();
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0069D180
float CSWCAnimBase::GetAnimationEventTime2(int nAnim)
{
    CExoString sAnim = GetAnimationName(nAnim);
    if (m_pModel) {
        float fTime;
        float fOther;
        m_pModel->GetEventTime(sAnim.CStr(), &fTime, &fOther);
        if (fTime > 0.0f) {
            return fTime * 1000.0f;
        }
    }
    return 0.0f;
}

// FUNCTION: SWKOTOR 0x0069D250
float CSWCAnimBase::GetAnimationEventTime(int nAnim)
{
    CExoString sAnim = GetAnimationName(nAnim);
    if (m_pModel) {
        float fTime;
        float fOther;
        m_pModel->GetEventTime(sAnim.CStr(), &fOther, &fTime);
        if (fTime > 0.0f) {
            return fTime * 1000.0f;
        }
    }
    return 0.0f;
}

// FUNCTION: SWKOTOR 0x0069D320
int CSWCAnimBase::HasAnimationLength(int nAnim)
{
    if (!m_pModel) {
        return 0;
    }
    CExoString sAnim = GetAnimationName(nAnim);
    float fTime;
    float fOther;
    m_pModel->GetEventTime(sAnim.CStr(), &fTime, &fOther);
    return fTime > 0.0f;
}

// FUNCTION: SWKOTOR 0x0069D3E0
int CSWCAnimBase::HasAnimation(int nAnim)
{
    if (!m_pModel) {
        return 0;
    }
    CExoString sAnim = GetAnimationName(nAnim);
    float fTime;
    float fOther;
    return m_pModel->GetEventTime(sAnim.CStr(), &fOther, &fTime) != 0;
}

// FUNCTION: SWKOTOR 0x0069D490
int CSWCAnimBase::SetModelV128(CAurObject* pModel, int n)
{
    if (!pModel) {
        return 0;
    }
    pModel->v128(n);
    return 1;
}

// FUNCTION: SWKOTOR 0x0069D4C0
int CSWCAnimBase::SetModelV12C(CAurObject* pModel, int n)
{
    if (!pModel) {
        return 0;
    }
    pModel->v12C(n);
    return 1;
}

// FUNCTION: SWKOTOR 0x0069A3A0
int CSWCAnimBase::SetModelV128Self(int n)
{
    return SetModelV128(m_pModel, n);
}

// FUNCTION: SWKOTOR 0x0069A3C0
int CSWCAnimBase::SetModelV12CSelf(int n)
{
    return SetModelV12C(m_pModel, n);
}

// FUNCTION: SWKOTOR 0x0069D4F0
int CSWCAnimBase::PlayAnim(int nAnim, float fSpeed, int a, int b)
{
    return PlayAnimOnModel(m_pModel, nAnim, fSpeed, a, b);
}

// FUNCTION: SWKOTOR 0x0069D520
int CSWCAnimBase::PlayAnimPart(unsigned char nPart, int nAnim, float fSpeed, int a, int b)
{
    if (nPart != 0xff) {
        return 0;
    }
    return PlayAnimOnModel(m_pModel, nAnim, fSpeed, a, b);
}

// FUNCTION: SWKOTOR 0x0069D540
int CSWCAnimBase::SetTalk(int a, int b, int c, int d)
{
    if (m_pModel) {
        if (a && b) {
            m_pModel->SetLight("talk", a, b, c, d);
        }
        else {
            m_pModel->ClearLight("talk");
        }
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0069D5A0
int CSWCAnimBase::PlayAnimOnPart(CAurObject* pModel, unsigned char nPart, int nAnim, float fSpeed, int a, int b)
{
    if (nPart != 0xff) {
        return 0;
    }
    return PlayAnimOnModel(pModel, nAnim, fSpeed, a, b);
}

// FUNCTION: SWKOTOR 0x0069D5D0
int CSWCAnimBase::SetFacing(const Vector& vDirection)
{
    if (!m_pModel) {
        return 0;
    }
    m_pModel->SetOrientation(EulerToQuaternion(GetYaw(vDirection), 0, 0));
    return 1;
}

// FUNCTION: SWKOTOR 0x0069D640
int CSWCAnimBase::SetOrientation(const Quaternion& q)
{
    if (!m_pModel) {
        return 0;
    }
    m_pModel->SetOrientation(q);
    return 1;
}

// FUNCTION: SWKOTOR 0x0069D690
int CSWCAnimBase::SetPosition(const Vector& v)
{
    if (!m_pModel) {
        return 0;
    }
    m_pModel->SetPosition(v);
    return 1;
}

// FUNCTION: SWKOTOR 0x0069D6E0
void CSWCAnimBase::SetFlag130(int b)
{
    if (m_pModel) {
        if (b) {
            m_pModel->v130();
        }
        else {
            m_pModel->v134();
        }
    }
}

// FUNCTION: SWKOTOR 0x0069D710
void CSWCAnimBase::SetFlag138(int b)
{
    if (m_pModel) {
        if (b) {
            m_pModel->v138();
        }
        else {
            m_pModel->v13C();
        }
    }
}

// FUNCTION: SWKOTOR 0x0069D740
void CSWCAnimBase::UnloadModel()
{
    if (m_pModel) {
        SetVisible(0);
        delete m_pModel;
        m_pModel = 0;
    }
}

// FUNCTION: SWKOTOR 0x0069D770
void CSWCAnimBase::UnloadModelPart(unsigned char nPart)
{
    if (nPart == 0xff && m_pModel) {
        SetVisible(0);
        delete m_pModel;
        m_pModel = 0;
    }
}

// FUNCTION: SWKOTOR 0x0069D7B0
int CSWCAnimBase::SetEnvMap(int bEnable, int bStore, const CResRef& cEnvMap)
{
    char szEnvMap[20];
    if (!m_pModel) {
        return 0;
    }
    if (bEnable) {
        cEnvMap.CopyToString(szEnvMap);
        m_pModel->SetEnvMap(szEnvMap);
    }
    else {
        m_pModel->SetEnvMap("");
    }
    if (bStore) {
        m_cEnvMap = cEnvMap;
        m_bEnvMap = bEnable;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0069D830
int CSWCAnimBase::SetEnvMapPart(unsigned char nPart, int bEnable, int bStore, const CResRef& cEnvMap)
{
    char szEnvMap[20];
    if (m_pModel && nPart == 0xff) {
        if (bEnable) {
            cEnvMap.CopyToString(szEnvMap);
            m_pModel->SetEnvMap(szEnvMap);
        }
        else {
            m_pModel->SetEnvMap("");
        }
        if (bStore) {
            m_cEnvMap = cEnvMap;
            m_bEnvMap = bEnable;
        }
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0069D8B0
int CSWCAnimBase::RestoreEnvMap()
{
    if (!m_pModel) {
        return 0;
    }
    if (m_bEnvMap) {
        char szEnvMap[20];
        m_cEnvMap.CopyToString(szEnvMap);
        m_pModel->SetEnvMap(szEnvMap);
        return 1;
    }
    m_pModel->SetEnvMap("");
    return 1;
}

// FUNCTION: SWKOTOR 0x0069D910
int CSWCAnimBase::HasAnimationLength2(int nAnim)
{
    CExoString sAnim;
    sAnim = GetAnimationName(nAnim);
    if (!m_pModel) {
        return 0;
    }
    float fTime;
    float fOther;
    m_pModel->GetEventTime(sAnim.CStr(), &fOther, &fTime);
    return fTime >= 0.0f;
}

// FUNCTION: SWKOTOR 0x0069DA00
int CSWCAnimBase::CreateShadow(const CExoString& sModel)
{
    m_pShadow = NewCAurObject(sModel.CStr(), "shadowblob", 0, 0);
    if (!m_pShadow) {
        return 0;
    }
    m_pShadow->vBC();
    if (m_pModel) {
        m_pShadow->AttachToObject(m_pModel, "root", 3);
        m_bShadowAttached = 1;
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0069DA70
void CSWCAnimBase::DestroyShadow()
{
    if (m_pShadow) {
        m_pShadow->AttachToObject(0, 0, 0);
        m_pShadow->SetVisible(0);
        delete m_pShadow;
        m_pShadow = 0;
        m_bShadowAttached = 0;
    }
}

// FUNCTION: SWKOTOR 0x0069DAC0
void CSWCAnimBase::AttachShadow(int bForce)
{
    if (m_pShadow && m_pModel && (bForce || !m_bShadowAttached)) {
        m_pShadow->AttachToObject(m_pModel, "root", 3);
        m_bShadowAttached = 1;
    }
}

// FUNCTION: SWKOTOR 0x0069DB10
void CSWCAnimBase::DetachShadow()
{
    if (m_pShadow && m_bShadowAttached) {
        m_pShadow->AttachToObject(0, 0, 0);
        m_pShadow->SetVisible(0);
        m_bShadowAttached = 0;
    }
}

// FUNCTION: SWKOTOR 0x0069DB50
void CSWCAnimBase::SetShadowOrientation(const Quaternion& q)
{
    Quaternion qNode;
    Vector vNode;
    if (m_pShadow) {
        m_pShadow->GetNodePosition("root", &vNode, &qNode);
        m_pShadow->SetNodePosition("root", vNode, q);
    }
}

// FUNCTION: SWKOTOR 0x0069DC10
void CSWCAnimBase::ModelVC0()
{
    if (m_pModel) {
        m_pModel->vC0();
    }
}

// FUNCTION: SWKOTOR 0x0069DC30
void CSWCAnimBase::ModelVBC()
{
    if (m_pModel) {
        m_pModel->vBC();
    }
}

// FUNCTION: SWKOTOR 0x0069DC50
Quaternion CSWCAnimBase::GetOrientation()
{
    if (!m_pModel) {
        return Quaternion();
    }
    return m_pModel->GetOrientation();
}

// FUNCTION: SWKOTOR 0x0069DC90
void CSWCAnimBase::UpdateTilt()
{
    if (m_pModel) {
        float fPitch = GetPitch(m_pModel->GetOrientation());
        float fRoll = GetRoll(m_pModel->GetOrientation());
        m_pModel->SetOrientation(EulerToQuaternion(fPitch, fRoll, m_fTiltSpeed));
    }
}

// FUNCTION: SWKOTOR 0x0069DD30
// NONMATCH: 9 diff insns. Same code, but the original places the -1.0 tail
// before the f tail; tried ternary / if-chain / clamp-variable forms.
void CSWCAnimBase::SetGroundTiltAmount(float f)
{
    m_fTiltSpeed = (!(-1.0f > f) ? (1.0f < f ? 1.0f : f) : -1.0f) * g_fGroundTiltScale;
}

// FUNCTION: SWKOTOR 0x0069DD90
int CSWCAnimBase::SetSkin(const CResRef& cSkin, int n)
{
    if (!m_pModel) {
        return 0;
    }
    char szSkin[20];
    cSkin.CopyToString(szSkin);
    _strlwr(szSkin);
    m_pModel->v150(szSkin, n);
    return 1;
}

// FUNCTION: SWKOTOR 0x0069DDF0
int CSWCAnimBase::ClearSkin()
{
    if (!m_pModel) {
        return 0;
    }
    m_pModel->v154();
    return 1;
}

// FUNCTION: SWKOTOR 0x0069DE10
int CSWCAnimBase::SetFlag144(int b)
{
    if (!m_pModel) {
        return 0;
    }
    m_pModel->v144(b != 0);
    return 1;
}

// FUNCTION: SWKOTOR 0x0069DE40
int CSWCAnimBase::HasModel(int n)
{
    return m_pModel != 0;
}

// FUNCTION: SWKOTOR 0x0069DE50
void CSWCAnimBase::ModelVF8()
{
    if (m_pModel) {
        m_pModel->vF8();
    }
}

// FUNCTION: SWKOTOR 0x0069DE70
void CSWCAnimBase::ModelVFC()
{
    if (m_pModel) {
        m_pModel->vFC();
    }
}

// FUNCTION: SWKOTOR 0x0069E140
CSWCAnimBase::~CSWCAnimBase()
{
    if (m_pShadow) {
        m_pShadow->AttachToObject(0, 0, 0);
        m_pShadow->SetVisible(0);
        delete m_pShadow;
        m_pShadow = 0;
        m_bShadowAttached = 0;
    }
    DestroyModelInline();
}

// FUNCTION: SWKOTOR 0x0069BCB0
CAurObject* CSWCAnimBaseTW::GetModel(unsigned char nPart)
{
    switch (nPart) {
    case 0xfc:
        return m_pModel3;
    case 0xfd:
        return m_pModel2;
    }
    return CSWCAnimBase::GetModel(nPart);
}

// FUNCTION: SWKOTOR 0x0069BE20 SYMBOL: ?ReplaceTexture@CSWCAnimBaseTW@@UAEHABVCResRef@@0H@Z
int CSWCAnimBaseTW::ReplaceTexture(const CResRef& cOld, const CResRef& cNew, int bStore)
{
    char szOld[20];
    char szNew[20];
    if (m_pModel2) {
        cOld.CopyToString(szOld);
        cNew.CopyToString(szNew);
        _strlwr(szNew);
        _strlwr(szOld);
        m_pModel2->ReplaceTexture(szOld, szNew);
        if (bStore) {
            m_cReplaceTexture2 = cNew;
            m_bReplaceTextureIndexed2 = 0;
        }
    }
    if (m_pModel3) {
        cOld.CopyToString(szOld);
        cNew.CopyToString(szNew);
        _strlwr(szNew);
        _strlwr(szOld);
        m_pModel3->ReplaceTexture(szOld, szNew);
        if (bStore) {
            m_cReplaceTexture3 = cNew;
            m_bReplaceTextureIndexed3 = 0;
        }
    }
    return CSWCAnimBase::ReplaceTexture(cOld, cNew, bStore);
}

// FUNCTION: SWKOTOR 0x0069BF30 SYMBOL: ?ReplaceTexture@CSWCAnimBaseTW@@UAEHEABVCResRef@@0H@Z
int CSWCAnimBaseTW::ReplaceTexture(unsigned char nPart, const CResRef& cOld, const CResRef& cNew, int bStore)
{
    char szOld[20];
    char szNew[20];
    if (nPart == 0xfd) {
        if (!m_pModel2) {
            return 0;
        }
        cOld.CopyToString(szOld);
        cNew.CopyToString(szNew);
        _strlwr(szOld);
        _strlwr(szNew);
        m_pModel2->ReplaceTexture(szOld, szNew);
        if (bStore) {
            m_cReplaceTexture2 = cNew;
            m_bReplaceTextureIndexed2 = 0;
        }
    }
    else if (nPart == 0xfc) {
        if (!m_pModel3) {
            return 0;
        }
        cOld.CopyToString(szOld);
        cNew.CopyToString(szNew);
        _strlwr(szOld);
        _strlwr(szNew);
        m_pModel3->ReplaceTexture(szOld, szNew);
        if (bStore) {
            m_cReplaceTexture3 = cNew;
            m_bReplaceTextureIndexed3 = 0;
        }
    }
    else {
        return CSWCAnimBase::ReplaceTexture(0xff, cOld, cNew, bStore);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0069C090 SYMBOL: ?ReplaceTexture@CSWCAnimBaseTW@@UAEHEABVCResRef@@0HPAGHH@Z
int CSWCAnimBaseTW::ReplaceTexture(unsigned char nPart, const CResRef& cOld, const CResRef& cNew, int nCount, unsigned short* pIndices, int bStore, int nStart)
{
    char szOld[20];
    char szNew[20];
    if (nPart == 0xfd) {
        if (!m_pModel2) {
            return 0;
        }
        cOld.CopyToString(szOld);
        cNew.CopyToString(szNew);
        _strlwr(szOld);
        _strlwr(szNew);
        m_pModel2->ReplaceTextureIndexed(szOld, szNew, nCount, pIndices, nStart);
        if (bStore) {
            m_cReplaceTexture2 = cNew;
            m_bReplaceTextureIndexed2 = 1;
            m_nTextureCount2 = (unsigned char)nCount;
            m_nTextureStart2 = (unsigned char)nStart;
            memcpy(m_aTextureIndices2, pIndices + nStart, nCount * 2);
        }
        return 1;
    }
    if (nPart == 0xfc) {
        if (!m_pModel3) {
            return 0;
        }
        cOld.CopyToString(szOld);
        cNew.CopyToString(szNew);
        _strlwr(szOld);
        _strlwr(szNew);
        m_pModel3->ReplaceTextureIndexed(szOld, szNew, nCount, pIndices, nStart);
        if (bStore) {
            m_cReplaceTexture3 = cNew;
            m_bReplaceTextureIndexed3 = 1;
            m_nTextureCount3 = (unsigned char)nCount;
            m_nTextureStart3 = (unsigned char)nStart;
            memcpy(m_aTextureIndices3, pIndices + nStart, nCount * 2);
        }
        return 1;
    }
    return CSWCAnimBase::ReplaceTexture(0xff, cOld, cNew, nCount, pIndices, bStore, nStart);
}

// FUNCTION: SWKOTOR 0x0069C260 SYMBOL: ?ReplaceTexture@CSWCAnimBaseTW@@UAEHABVCResRef@@0HPAGHH@Z
int CSWCAnimBaseTW::ReplaceTexture(const CResRef& cOld, const CResRef& cNew, int nCount, unsigned short* pIndices, int bStore, int nStart)
{
    char szOld[20];
    char szNew[20];
    if (m_pModel2) {
        cOld.CopyToString(szOld);
        cNew.CopyToString(szNew);
        _strlwr(szOld);
        _strlwr(szNew);
        m_pModel2->ReplaceTextureIndexed(szOld, szNew, nCount, pIndices, nStart);
        if (bStore) {
            m_cReplaceTexture2 = cNew;
            m_bReplaceTextureIndexed2 = 1;
            m_nTextureCount2 = (unsigned char)nCount;
            m_nTextureStart2 = (unsigned char)nStart;
            memcpy(m_aTextureIndices2, pIndices + nStart, nCount * 2);
        }
    }
    if (m_pModel3) {
        cOld.CopyToString(szOld);
        cNew.CopyToString(szNew);
        _strlwr(szOld);
        _strlwr(szNew);
        m_pModel3->ReplaceTextureIndexed(szOld, szNew, nCount, pIndices, nStart);
        if (bStore) {
            m_cReplaceTexture3 = cNew;
            m_bReplaceTextureIndexed3 = 1;
            m_nTextureCount3 = (unsigned char)nCount;
            m_nTextureStart3 = (unsigned char)nStart;
            memcpy(m_aTextureIndices3, pIndices + nStart, nCount * 2);
        }
    }
    return CSWCAnimBase::ReplaceTexture(cOld, cNew, nCount, pIndices, bStore, nStart);
}

// FUNCTION: SWKOTOR 0x0069C400
int CSWCAnimBaseTW::RestoreTexture()
{
    if (m_pModel2 && m_bReplaceTextureIndexed2) {
        CResRef cEmpty("");
        ReplaceTexture(0xfd, cEmpty, m_cReplaceTexture2, m_nTextureCount2, m_aTextureIndices2, 0, m_nTextureStart2);
    }
    if (m_pModel3 && m_bReplaceTextureIndexed3) {
        CResRef cEmpty("");
        ReplaceTexture(0xfc, cEmpty, m_cReplaceTexture3, m_nTextureCount3, m_aTextureIndices3, 0, m_nTextureStart3);
    }
    return CSWCAnimBase::RestoreTexture();
}

// FUNCTION: SWKOTOR 0x0069C4C0
int CSWCAnimBaseTW::RestoreTexturePart(unsigned char nPart)
{
    if (nPart == 0xfd) {
        if (m_pModel2) {
            if (m_bReplaceTextureIndexed2) {
                CResRef cEmpty("");
                ReplaceTexture(0xfd, cEmpty, m_cReplaceTexture2, m_nTextureCount2, m_aTextureIndices2, 0, m_nTextureStart2);
            }
            else {
                m_pModel2->RestoreTextures();
            }
            return 1;
        }
    }
    else if (nPart == 0xfc) {
        if (m_pModel3) {
            if (m_bReplaceTextureIndexed3) {
                CResRef cEmpty("");
                ReplaceTexture(0xfc, cEmpty, m_cReplaceTexture3, m_nTextureCount3, m_aTextureIndices3, 0, m_nTextureStart3);
            }
            else {
                m_pModel3->RestoreTextures();
            }
            return 1;
        }
    }
    else {
        return CSWCAnimBase::RestoreTexturePart(nPart);
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0069C6D0
int CSWCAnimBaseTW::SetEnvMap(int bEnable, int bStore, const CResRef& cEnvMap)
{
    char szEnvMap[20];
    if (bEnable) {
        cEnvMap.CopyToString(szEnvMap);
        if (m_pModel2) {
            m_pModel2->SetEnvMap(szEnvMap);
        }
        if (m_pModel3) {
            m_pModel3->SetEnvMap(szEnvMap);
        }
    }
    else {
        if (m_pModel2) {
            m_pModel2->SetEnvMap("");
        }
        if (m_pModel3) {
            m_pModel3->SetEnvMap("");
        }
    }
    if (bStore) {
        m_cEnvMap2 = cEnvMap;
        m_bEnvMap2 = bEnable;
        m_cEnvMap3 = cEnvMap;
        m_bEnvMap3 = bEnable;
    }
    return CSWCAnimBase::SetEnvMap(bEnable, bStore, cEnvMap);
}

// FUNCTION: SWKOTOR 0x0069C790
int CSWCAnimBaseTW::SetEnvMapPart(unsigned char nPart, int bEnable, int bStore, const CResRef& cEnvMap)
{
    char szEnvMap[20];
    if (nPart == 0xfd) {
        if (m_pModel2) {
            if (bEnable) {
                cEnvMap.CopyToString(szEnvMap);
                m_pModel2->SetEnvMap(szEnvMap);
            }
            else {
                m_pModel2->SetEnvMap("");
            }
            if (bStore) {
                m_cEnvMap2 = cEnvMap;
                m_bEnvMap2 = bEnable;
            }
        }
    }
    else if (nPart == 0xfc) {
        if (m_pModel3) {
            if (bEnable) {
                cEnvMap.CopyToString(szEnvMap);
                m_pModel3->SetEnvMap(szEnvMap);
            }
            else {
                m_pModel3->SetEnvMap("");
            }
            if (bStore) {
                m_cEnvMap3 = cEnvMap;
                m_bEnvMap3 = bEnable;
            }
        }
    }
    else {
        return CSWCAnimBase::SetEnvMapPart(nPart, bEnable, bStore, cEnvMap);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x0069C8A0
int CSWCAnimBaseTW::RestoreEnvMap()
{
    char szEnvMap[20];
    if (m_pModel2) {
        if (m_bEnvMap2) {
            m_cEnvMap2.CopyToString(szEnvMap);
            m_pModel2->SetEnvMap(szEnvMap);
        }
        else {
            m_pModel2->SetEnvMap("");
        }
    }
    if (m_pModel3) {
        if (m_bEnvMap3) {
            m_cEnvMap3.CopyToString(szEnvMap);
            m_pModel3->SetEnvMap(szEnvMap);
        }
        else {
            m_pModel3->SetEnvMap("");
        }
    }
    return CSWCAnimBase::RestoreEnvMap();
}

// FUNCTION: SWKOTOR 0x0069C940
int CSWCAnimBaseTW::PlayAnimOnModel(CAurObject* pModel, int nAnim, float fSpeed, int a, int b)
{
    if (fSpeed == 0.0f) {
        fSpeed = 1.0f;
    }
    if (m_pModel2 || m_pModel3) {
        CExoString sAnim = GetAnimationName(nAnim);
        if (m_pModel2) {
            if (!m_pModel2->PlayAnimation(sAnim.CStr(), fSpeed, a, b)) {
                m_pModel2->PlayAnimation("default", fSpeed, a, b);
            }
        }
        if (m_pModel3) {
            if (!m_pModel3->PlayAnimation(sAnim.CStr(), fSpeed, a, b)) {
                m_pModel3->PlayAnimation("default", fSpeed, a, b);
            }
        }
    }
    return CSWCAnimBase::PlayAnimOnModel(pModel, nAnim, fSpeed, a, b);
}

// FUNCTION: SWKOTOR 0x0069CA70
int CSWCAnimBaseTW::PlayAnimOnPart(CAurObject* pModel, unsigned char nPart, int nAnim, float fSpeed, int a, int b)
{
    CExoString sAnim = GetAnimationName(nAnim);
    if (nPart == 0xff) {
        return CSWCAnimBase::PlayAnimOnModel(pModel, nAnim, fSpeed, a, b);
    }
    if (nPart == 0xfd) {
        if (!m_pModel2) {
            return 0;
        }
        int bResult = m_pModel2->PlayAnimation(sAnim.CStr(), fSpeed, a, b);
        if (!bResult) {
            m_pModel2->PlayAnimation("default", fSpeed, a, b);
        }
        return bResult;
    }
    if (nPart == 0xfc && m_pModel3) {
        int bResult = m_pModel3->PlayAnimation(sAnim.CStr(), fSpeed, a, b);
        if (!bResult) {
            m_pModel3->PlayAnimation("default", fSpeed, a, b);
        }
        return bResult;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0069BCE0
int CSWCAnimBaseTW::LoadModel(const CResRef& cResRef, unsigned char nPart)
{
    char szModel[20];
    switch (nPart) {
    case 0xff:
        if (!CSWCAnimBase::LoadModel(cResRef, 0xff)) {
            return 0;
        }
        if (m_pModel2) {
            m_pModel2->AttachToObject(m_pModel, "tail", 0);
        }
        if (m_pModel3) {
            m_pModel3->AttachToObject(m_pModel, "wings", 0);
        }
        return 1;
    case 0xfd:
        cResRef.CopyToString(szModel);
        m_pModel2 = NewCAurObject(szModel, "tail", 0, 0);
        if (!m_pModel2) {
            return 0;
        }
        m_cReplaceTexture2 = cResRef;
        m_bReplaceTextureIndexed2 = 0;
        m_pModel2->AttachToObject(m_pModel, "tail", 0);
        return 1;
    case 0xfc:
        cResRef.CopyToString(szModel);
        m_pModel3 = NewCAurObject(szModel, "wings", 0, 0);
        if (!m_pModel3) {
            return 0;
        }
        m_cReplaceTexture3 = cResRef;
        m_bReplaceTextureIndexed3 = 0;
        m_pModel3->AttachToObject(m_pModel, "wings", 0);
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0069E1F0
int CSWCAnimBase::LoadModel(const CResRef& cResRef, unsigned char nPart)
{
    Quaternion qOrientation;
    Vector vPosition;
    if (nPart != 0xff) {
        return 0;
    }
    char szModel[20];
    cResRef.CopyToString(szModel);
    m_pModel = NewCAurObject(szModel, "body", 0, 0);
    if (!m_pModel) {
        return 0;
    }
    if (m_pModel->GetNodeTransform("handconjure", &vPosition, &qOrientation)) {
        if (qOrientation.x != 0.0f || qOrientation.y != 0.0f || qOrientation.z != 0.0f || qOrientation.w != 1.0f) {
            CExoString sMessage;
            sMessage.Format("CSWCAnimBase::LoadModel(): handconjure node of %s is rotated", szModel);
        }
    }
    if (m_pModel->GetNodeTransform("headconjure", &vPosition, &qOrientation)) {
        if (qOrientation.x != 0.0f || qOrientation.y != 0.0f || qOrientation.z != 0.0f || qOrientation.w != 1.0f) {
            CExoString sMessage;
            sMessage.Format("CSWCAnimBase::LoadModel(): headconjure node of %s is rotated", szModel);
        }
    }
    m_cReplaceTexture = cResRef;
    m_bReplaceTextureIndexed = 0;
    AttachShadow(0);
    return 1;
}

// FUNCTION: SWKOTOR 0x0069CBD0
CSWCAnimBaseTW::CSWCAnimBaseTW()
{
    m_nAnimType = 0xb;
    m_pModel2 = 0;
    m_pModel3 = 0;
    m_cReplaceTexture2 = "";
    m_bReplaceTextureIndexed2 = 0;
    m_nTextureCount2 = 0;
    m_nTextureStart2 = 0;
    memset(m_aTextureIndices2, 0, sizeof(m_aTextureIndices2));
    m_cReplaceTexture3 = "";
    m_bReplaceTextureIndexed3 = 0;
    m_nTextureCount3 = 0;
    m_nTextureStart3 = 0;
    memset(m_aTextureIndices3, 0, sizeof(m_aTextureIndices3));
    m_cEnvMap2 = "";
    m_bEnvMap2 = 0;
    m_cEnvMap3 = "";
    m_bEnvMap3 = 0;
}

// The Vector array is constructed by an inline loop (not the vector
// constructor iterator) only with inlining enabled.
#pragma auto_inline(on)

// FUNCTION: SWKOTOR 0x0069DFB0
CSWCAnimBase::CSWCAnimBase()
{
    m_pModel = 0;
    m_nAnimType = 0;
    m_nC5 = 0;
    m_nC6 = 0;
    m_nC7 = 0;
    m_cReplaceTexture = "";
    m_bReplaceTextureIndexed = 0;
    m_nTextureCount = 0;
    m_nTextureStart = 0;
    memset(m_aTextureIndices, 0, sizeof(m_aTextureIndices));
    for (int i = 0; i < 5; i++) {
        m_a90[i] = 0;
        m_aA4[i] = 0;
    }
    m_cEnvMap = "";
    m_bEnvMap = 0;
    m_fC0 = 1.0f;
    m_nC8 = 0;
    m_nCC = 0;
    m_nD0 = 1;
    m_pShadow = 0;
    m_bShadowAttached = 0;
    m_fTiltSpeed = 0;
    m_nDC = 0;
    m_qE0 = Quaternion(g_vZAxis, 0.0f);
}
