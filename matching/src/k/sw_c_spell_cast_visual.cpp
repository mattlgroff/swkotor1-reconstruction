// Spell-cast model slots recovered from the canonical client binary.
#include <string.h>

#include "k/sw_c_anim_base.h"

// Caster whose model the visuals attach to (only the GetModel slot is used).
class CExoSound {
public:
    int Play3DOneShotSound(const CResRef& cSound, Vector vPosition, float fRange, unsigned char nPriority,
                           unsigned long nDelay, unsigned char nVolume, float fPitch,
                           float fVariance);  // 0x005D5E10
};

extern CExoSound* g_pExoSound;                      // 0x007A39EC

class CSWCCreature {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0C();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1C();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2C();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3C();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4C();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5C();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6C();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7C();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8C();
    virtual void v90(); virtual void v94();
    virtual CAurObject* GetModel(unsigned char nPart, int b);  // 0x98

    char pad_0x4[0x24 - 0x4];
    Vector m_vPosition;                             // 0x24
};

class CSWCSpellCastVisual {
public:
    CAurObject* m_pVisual[6];
    unsigned long m_anUnknown18[3];                 // 0x18 conjure / cast timers, 0x20 active
    int m_bFadeDone[3];                             // 0x24, set by the "donefade" callback

    CSWCSpellCastVisual();
    ~CSWCSpellCastVisual();
    void UnloadConjureVisual();
    void UnloadCastVisual();
    int LoadConjureVisual(CResRef cHead, CResRef cHand, CResRef cGround, CResRef cSound1, CResRef cSound2,
                          CSWCCreature* pCreature);
    int LoadCastVisual(CResRef cHead, CResRef cHand, CResRef cGround, CSWCCreature* pCreature);
    void Update(unsigned long nDelta);
};

// FUNCTION: SWKOTOR 0x00695CE0
CSWCSpellCastVisual::CSWCSpellCastVisual()
{
    m_pVisual[0] = 0;
    m_pVisual[1] = 0;
    m_pVisual[2] = 0;
    m_pVisual[3] = 0;
    m_pVisual[4] = 0;
    m_pVisual[5] = 0;
    m_anUnknown18[0] = 0;
    m_anUnknown18[1] = 0;
    m_anUnknown18[2] = 0;
    m_bFadeDone[0] = 0;
    m_bFadeDone[1] = 0;
    m_bFadeDone[2] = 0;
}

// FUNCTION: SWKOTOR 0x00695D10
void CSWCSpellCastVisual::UnloadConjureVisual()
{
    if (m_pVisual[0] != 0) {
        m_pVisual[0]->AttachToObject(0, 0, 0);
        if (m_pVisual[0] != 0) delete m_pVisual[0];
        m_pVisual[0] = 0;
    }
    if (m_pVisual[1] != 0) {
        m_pVisual[1]->AttachToObject(0, 0, 0);
        if (m_pVisual[1] != 0) delete m_pVisual[1];
        m_pVisual[1] = 0;
    }
    if (m_pVisual[2] != 0) {
        m_pVisual[2]->AttachToObject(0, 0, 0);
        if (m_pVisual[2] != 0) delete m_pVisual[2];
        m_pVisual[2] = 0;
    }
}

// FUNCTION: SWKOTOR 0x00695D90
void CSWCSpellCastVisual::UnloadCastVisual()
{
    if (m_pVisual[3] != 0) {
        m_pVisual[3]->AttachToObject(0, 0, 0);
        if (m_pVisual[3] != 0) delete m_pVisual[3];
        m_pVisual[3] = 0;
    }
    if (m_pVisual[4] != 0) {
        m_pVisual[4]->AttachToObject(0, 0, 0);
        if (m_pVisual[4] != 0) delete m_pVisual[4];
        m_pVisual[4] = 0;
    }
    if (m_pVisual[5] != 0) {
        m_pVisual[5]->AttachToObject(0, 0, 0);
        if (m_pVisual[5] != 0) delete m_pVisual[5];
        m_pVisual[5] = 0;
    }
}

// FUNCTION: SWKOTOR 0x00695E10
void CSWCSpellCastVisual::Update(unsigned long nDelta)
{
    if (m_bFadeDone[0] && m_pVisual[0]) {
        m_pVisual[0]->AttachToObject(0, 0, 0);
        if (m_pVisual[0] != 0) delete m_pVisual[0];
        m_pVisual[0] = 0;
    }
    if (m_bFadeDone[1] && m_pVisual[1]) {
        m_pVisual[1]->AttachToObject(0, 0, 0);
        if (m_pVisual[1] != 0) delete m_pVisual[1];
        m_pVisual[1] = 0;
    }
    if (m_bFadeDone[2] && m_pVisual[2]) {
        m_pVisual[2]->AttachToObject(0, 0, 0);
        if (m_pVisual[2] != 0) delete m_pVisual[2];
        m_pVisual[2] = 0;
    }
    if (!m_anUnknown18[2]) {
        return;
    }
    if (!m_anUnknown18[0] && !m_anUnknown18[1]) {
        m_anUnknown18[2] = 0;
        return;
    }
    if (m_anUnknown18[0] > 0) {
        if (m_anUnknown18[0] <= nDelta) {
            UnloadConjureVisual();
            m_anUnknown18[0] = 0;
            return;
        }
        m_anUnknown18[0] -= nDelta;
        return;
    }
    if (m_anUnknown18[1] > 0) {
        if (m_anUnknown18[1] <= nDelta) {
            UnloadCastVisual();
            m_anUnknown18[1] = 0;
            m_anUnknown18[2] = 0;
            return;
        }
        m_anUnknown18[1] -= nDelta;
    }
}

// FUNCTION: SWKOTOR 0x00695F20
CSWCSpellCastVisual::~CSWCSpellCastVisual()
{
    UnloadConjureVisual();
    UnloadCastVisual();
}

// FUNCTION: SWKOTOR 0x00696250
int CSWCSpellCastVisual::LoadCastVisual(CResRef cHead, CResRef cHand, CResRef cGround, CSWCCreature* pCreature)
{
    char szModel[17];
    UnloadCastVisual();
    cHead.CopyToString(szModel);
    if (strlen(szModel) != 0) {
        m_pVisual[3] = NewCAurObject(szModel, "castvisual", 0, 0);
        if (m_pVisual[3] != 0) {
            m_pVisual[3]->AttachToObject(pCreature->GetModel(0xff, 1), "headconjure", 0);
            m_pVisual[3]->PlayAnimation("cast01", 1.0f, 1, 0);
        }
    }
    cHand.CopyToString(szModel);
    if (strlen(szModel) != 0) {
        m_pVisual[4] = NewCAurObject(szModel, "castvisual", 0, 0);
        if (m_pVisual[4] != 0) {
            m_pVisual[4]->AttachToObject(pCreature->GetModel(0xff, 1), "handconjure", 0);
            m_pVisual[4]->PlayAnimation("cast01", 1.0f, 1, 0);
        }
    }
    cGround.CopyToString(szModel);
    if (strlen(szModel) != 0) {
        m_pVisual[5] = NewCAurObject(szModel, "castgroundvisual", 0, 0);
        if (m_pVisual[5] != 0) {
            m_pVisual[5]->AttachToObject(pCreature->GetModel(0xff, 1), "root", 0);
            m_pVisual[5]->PlayAnimation("cast01", 1.0f, 1, 0);
        }
    }
    if (m_pVisual[1] != 0) {
        m_pVisual[1]->PlayAnimation("fade", 1.0f, 1, 0);
    }
    if (m_pVisual[0] != 0) {
        m_pVisual[0]->PlayAnimation("fade", 1.0f, 1, 0);
    }
    if (m_pVisual[2] != 0) {
        m_pVisual[2]->PlayAnimation("fade", 1.0f, 1, 0);
    }
    return 1;
}

// Not in the function table. Model event callback for "donefade"
// (descriptive name): flags whichever conjure visual finished fading.
// FUNCTION: SWKOTOR 0x00695EF0
void ConjureVisualDoneFadeCallback(CAurObject* pObject, char* szEvent, void* pUserData)
{
    CSWCSpellCastVisual* pVisual = (CSWCSpellCastVisual*)pUserData;
    if (pVisual != 0) {
        if (pObject == pVisual->m_pVisual[0]) {
            pVisual->m_bFadeDone[0] = 1;
        }
        if (pObject == pVisual->m_pVisual[1]) {
            pVisual->m_bFadeDone[1] = 1;
        }
        if (pObject == pVisual->m_pVisual[2]) {
            pVisual->m_bFadeDone[2] = 1;
        }
    }
}

// FUNCTION: SWKOTOR 0x00695F30
int CSWCSpellCastVisual::LoadConjureVisual(CResRef cHead, CResRef cHand, CResRef cGround, CResRef cSound1,
                                           CResRef cSound2, CSWCCreature* pCreature)
{
    CExoString sModel;
    UnloadConjureVisual();
    cHead.CopyToString(sModel);
    if (sModel.GetLength() > 0) {
        m_pVisual[0] = NewCAurObject(sModel.CStr(), "conjurevisual", 0, 0);
        if (m_pVisual[0] != 0) {
            m_bFadeDone[0] = 0;
            m_pVisual[0]->AddEventCallback(ConjureVisualDoneFadeCallback, "donefade", this, 9999.0f, 0);
            m_pVisual[0]->AttachToObject(pCreature->GetModel(0xff, 1), "headconjure", 0);
            m_pVisual[0]->PlayAnimation("conjure01", 1.0f, 0, 0);
        }
    }
    cHand.CopyToString(sModel);
    if (sModel.GetLength() > 0) {
        m_pVisual[1] = NewCAurObject(sModel.CStr(), "conjurevisual", 0, 0);
        if (m_pVisual[1] != 0) {
            m_bFadeDone[1] = 0;
            m_pVisual[1]->AddEventCallback(ConjureVisualDoneFadeCallback, "donefade", this, 9999.0f, 0);
            m_pVisual[1]->AttachToObject(pCreature->GetModel(0xff, 1), "handconjure", 0);
            m_pVisual[1]->PlayAnimation("conjure01", 1.0f, 0, 0);
        }
    }
    cGround.CopyToString(sModel);
    if (sModel.GetLength() > 0) {
        m_pVisual[2] = NewCAurObject(sModel.CStr(), "conjuregroundvisual", 0, 0);
        if (m_pVisual[2] != 0) {
            m_bFadeDone[2] = 0;
            m_pVisual[2]->AddEventCallback(ConjureVisualDoneFadeCallback, "donefade", this, 9999.0f, 0);
            m_pVisual[2]->AttachToObject(pCreature->GetModel(0xff, 1), "root", 0);
            m_pVisual[2]->PlayAnimation("conjure01", 1.0f, 0, 0);
        }
    }
    Quaternion qOrientation;
    Vector vPosition;
    CAurObject* pModel = pCreature->GetModel(0xff, 1);
    if (pModel == 0 || !pModel->GetNodePosition("handconjure", &vPosition, &qOrientation)) {
        vPosition = pCreature->m_vPosition;
    }
    if (*cSound1.GetResRef() != 0) {
        g_pExoSound->Play3DOneShotSound(cSound1, vPosition, 1.5f, 13, 0, 0, 0.0f, 0.0f);
    }
    if (*cSound2.GetResRef() != 0) {
        g_pExoSound->Play3DOneShotSound(cSound2, vPosition, 1.5f, 13, 0, 0, 0.0f, 0.0f);
    }
    return 1;
}
