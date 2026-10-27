// Client creature appearance (CSWCCreatureAppearance, nwccreatureappearance.o).
// Names come from the Mac debug map where it has them; others are
// descriptive and marked as such.
#include <stdio.h>

#include "k/sw_c_creature_appearance.h"

#define K_APPEARANCE_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define K_APPEARANCE_MAX(a, b) (((a) > (b)) ? (a) : (b))

class CSWItem {
public:
    virtual ~CSWItem();
    char pad_0x4[0x10 - 0x4];
};

class CSWCItem : public CSWItem, public CSWCObject {
};

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;
    CExoArrayList()
    {
        num = 0;
        array_size = 0;
        element = 0;
    }
    ~CExoArrayList()
    {
        if (element)
            delete[] element;
    }
};

// FUNCTION: SWKOTOR 0x00696430
CSWCCreatureAppearance::CSWCCreatureAppearance(CSWCCreature* pCreature)
{
    m_oidHeadItem = g_oidInvalid;
    memset(m_anUnknown02, 0, sizeof(m_anUnknown02));
    m_nGender = 0xff;
    m_nHeadModel = 0xff;
    m_nHead = 0xff;
    m_nUnknown34 = 0xff;
    m_nBodyVariation = 0;
    m_nTextureVariation = 1;
    m_nAppearanceType = 0xffff;
    m_nUnknown14 = 0;
    m_nUnknown1D = 0;
    m_nUnknown1E = 0;
    m_nUnknown1F = 0;
    m_nUnknown20 = 0;
    m_nTail = 0;
    m_nWing = 0;
    m_fUnknown24 = 0.6f;
    m_fHeadHeight = 0.5f;
    m_nUnknown30 = 0;
    m_nAlignment = 50;
    m_pCreature = pCreature;
    m_nUnknown40 = 0;
    m_nUnknown44 = 0;
    m_nUnknown48 = 0;
    m_nUnknown4C = 0;
    m_nUnknown50 = 0;
    m_nUnknown54 = 0;
    m_bForceAnimate = 0;
    m_nUnknown38 = 0;
}

// FUNCTION: SWKOTOR 0x00696CA0
char CSWCCreatureAppearance::GetGenderChar()
{
    CExoString sValue;
    g_pRules->Get2DArrays()->GetGenderTable()->GetCExoStringEntry(m_nGender, "GENDER", &sValue);
    return sValue[0];
}

// FUNCTION: SWKOTOR 0x00696D40
unsigned char CSWCCreatureAppearance::GetAnimType(const CExoString& sModelType)
{
    if (sModelType.Find('S', 0) != -1) {
        return 11;
    }
    if (sModelType.Find("B", 0) != -1) {
        return 3;
    }
    if (sModelType.Find('F', 0) != -1 || sModelType.Find('L', 0) != -1) {
        return 2;
    }
    return 11;
}

// FUNCTION: SWKOTOR 0x00696E10
int CSWCCreatureAppearance::RequestSimple(CCAppearanceInfo* pInfo)
{
    CExoString sRace;
    CExoString sModelType;
    g_pRules->Get2DArrays()->GetAppearanceTable()->GetCExoStringEntry(pInfo->m_nAppearanceType, "RACE", &sRace);
    g_pRules->Get2DArrays()->GetAppearanceTable()->GetCExoStringEntry(pInfo->m_nAppearanceType, "MODELTYPE", &sModelType);
    unsigned char nAnimType = GetAnimType(sModelType);
    m_pCreature->RequestModel(sRace, "", nAnimType);
    return 1;
}

// FUNCTION: SWKOTOR 0x00696F60
int CSWCCreatureAppearance::RequestHeadAppearance(CCAppearanceInfo* pInfo)
{
    CExoString sModel;
    CExoString sTexture;
    CExoString sValue;
    g_pRules->Get2DArrays()->GetAppearanceTable()->GetCExoStringEntry(pInfo->m_nAppearanceType, "MODELTYPE", &sValue);
    if (GetAnimType(sValue) == 3 && pInfo->m_nHead != 0 && pInfo->m_oidHeadItem == g_oidInvalid) {
        sModel = "P";
        g_pRules->Get2DArrays()->GetGenderTable()->GetCExoStringEntry(pInfo->m_nGender == 1, "GENDER", &sValue);
        sModel = sModel + sValue;
        g_pRules->Get2DArrays()->GetAppearanceTable()->GetCExoStringEntry(pInfo->m_nAppearanceType, "RACE", &sValue);
        sModel = sModel + sValue;
        sTexture = sModel;
        sModel = sModel + CExoString(pInfo->m_nHeadModel);
        sModel = sModel + "_HEAD";
        sTexture = sTexture + "0_HEAD";
        char szNumber[4];
        sprintf(szNumber, "%03u", pInfo->m_nHead);
        sModel = sModel + szNumber;
        sTexture = sTexture + szNumber;
        g_pRules->Get2DArrays()->GetAppearanceTable()->GetCExoStringEntry(pInfo->m_nAppearanceType, "MODELTYPE",
                                                                         &sValue);
        unsigned char nAnimType = GetAnimType(sValue);
        m_pCreature->RequestModel(sModel, sTexture, nAnimType);
    }
    return 1;
}

// Descriptive name: height of the HEAD_G node above the creature, or the
// stored default head height.
// FUNCTION: SWKOTOR 0x006973B0
float CSWCCreatureAppearance::GetHeadNodeHeight()
{
    Vector vPosition;
    Quaternion qOrientation;
    CAurObject* pModel = m_pCreature->GetModel(0xff, 1);
    if (pModel != 0) {
        pModel->GetNodePosition("HEAD_G", &vPosition, &qOrientation);
        if (vPosition.z != 0.0f) {
            float fHeight = vPosition.z - m_pCreature->m_vPosition.z;
            if (!(fHeight < 0.0f)) {
                return fHeight;
            }
        }
        return m_fHeadHeight;
    }
    return 0.0f;
}

// FUNCTION: SWKOTOR 0x00697460
CAurObject* CSWCCreatureAppearance::GetCameraFollowPart()
{
    CAurObject* pModel = m_pCreature->GetModel(0xff, 1);
    CAurPart* pPart = pModel->FindPart("CAMERAHOOK");
    if (pPart != 0) {
        return pPart->GetObject();
    }
    return pModel;
}

// FUNCTION: SWKOTOR 0x006974A0
int CSWCCreatureAppearance::GetFollowingBody()
{
    return m_pCreature->GetModel(0xff, 1)->FindPart("CAMERAHOOK") != 0;
}

// FUNCTION: SWKOTOR 0x006974D0
float CSWCCreatureAppearance::GetHeadHeightOffset()
{
    CAurObject* pModel = m_pCreature->GetModel(0xff, 1);
    if (pModel != 0) {
        if (pModel->FindPart("CAMERAHOOK") != 0) {
            Vector vPosition;
            Quaternion qOrientation;
            pModel->GetNodeTransform("CAMERAHOOK", &vPosition, &qOrientation);
            return vPosition.z;
        }
        return GetHeadNodeHeight();
    }
    return 0.0f;
}

// FUNCTION: SWKOTOR 0x00697580
float CSWCCreatureAppearance::GetCameraOffset()
{
    float fValue = 0.0f;
    if (g_pRules->Get2DArrays()->GetAppearanceTable()->GetFLOATEntry(m_nAppearanceType, g_nAppearanceColCameraOffset, &fValue)) {
        return fValue;
    }
    return 0.0f;
}

// Descriptive name: appearance.2da run rate scaled to the client's units.
// FUNCTION: SWKOTOR 0x006975D0
float CSWCCreatureAppearance::GetRunRate()
{
    float fValue;
    g_pRules->Get2DArrays()->GetAppearanceTable()->GetFLOATEntry(m_nAppearanceType, g_nAppearanceColRunRate, &fValue);
    return 0.37f * (fValue * 1000.0f);
}

// FUNCTION: SWKOTOR 0x00697610
void CSWCCreatureAppearance::GetBTypeModelAndTextureResRef(CCAppearanceInfo* pInfo, CResRef* pModel,
                                                           CResRef* pTexture)
{
    char szModelColumn[] = "ModelA";
    char szTextureColumn[] = "TEXA";
    int nBody = K_APPEARANCE_MAX(1, K_APPEARANCE_MIN(10, pInfo->m_nBodyVariation));
    int nTexture = K_APPEARANCE_MAX(1, pInfo->m_nTextureVariation);
    if (pInfo->m_nUnknown14 != 0) {
        szTextureColumn[3] = szModelColumn[5] = (char)(nBody + 0x40);
    }
    CExoString sValue;
    if (pModel != 0) {
        if (!g_pRules->Get2DArrays()->GetAppearanceTable()->GetCExoStringEntry(pInfo->m_nAppearanceType, szModelColumn,
                                                      &sValue)) {
            sValue = "";
        }
        *pModel = sValue;
    }
    if (pTexture != 0) {
        if (g_pRules->Get2DArrays()->GetAppearanceTable()->GetCExoStringEntry(pInfo->m_nAppearanceType, szTextureColumn,
                                                     &sValue)) {
            CResRef cTexture;
            CExoString sName;
            sName.Format("%s%02d", sValue.CStr(), nTexture);
            cTexture = sName;
            if (!g_pExoResMan->Exists(cTexture, 3, 0) && !g_pExoResMan->Exists(cTexture, 0xbbf, 0)) {
                sName.Format("%s%02d", sValue.CStr(), 1);
                cTexture = sName;
            }
            *pTexture = cTexture;
        }
        else {
            *pTexture = "";
        }
    }
}

// Advance one externally driven animation step on a model.
#define K_ANIMATE_MODEL_ONCE(pModel)       \
    do {                                   \
        (pModel)->BeginExternalAnimation(); \
        (pModel)->AnimateExternal(1.0f);   \
        (pModel)->EndExternalAnimation();  \
    } while (0)

// FUNCTION: SWKOTOR 0x006978A0
void CSWCCreatureAppearance::ForceAnimate()
{
    if (m_bForceAnimate == 0) {
        return;
    }
    CAurObject* pModel = m_pCreature->GetModel(0xff, 1);
    if (pModel != 0) {
        K_ANIMATE_MODEL_ONCE(pModel);
    }
    pModel = m_pCreature->GetModel(0xfe, 1);
    if (pModel != 0) {
        K_ANIMATE_MODEL_ONCE(pModel);
        CSWCItem* pItem = (CSWCItem*)g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(m_oidHeadItem);
        if (pItem != 0) {
            pModel = pItem->GetModel(0xff, 1);
            if (pModel != 0) {
                K_ANIMATE_MODEL_ONCE(pModel);
            }
        }
    }
    CSWCItem* pItem = (CSWCItem*)g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(
        m_pCreature->GetEquippedItemID(0x10));
    if (pItem != 0) {
        pModel = pItem->GetModel(0xff, 1);
        if (pModel != 0) {
            K_ANIMATE_MODEL_ONCE(pModel);
        }
    }
    pItem = (CSWCItem*)g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(
        m_pCreature->GetEquippedItemID(0x20));
    if (pItem != 0) {
        pModel = pItem->GetModel(0xff, 1);
        if (pModel != 0) {
            K_ANIMATE_MODEL_ONCE(pModel);
        }
    }
}

// FUNCTION: SWKOTOR 0x00697A20
int CSWCCreatureAppearance::CreateHeadAppearance(CCAppearanceInfo* pInfo)
{
    CExoString sModelType;
    g_pRules->Get2DArrays()->GetAppearanceTable()->GetCExoStringEntry(pInfo->m_nAppearanceType, g_nAppearanceColModelType, &sModelType);
    if (GetAnimType(sModelType) != 3) {
        m_nHead = pInfo->m_nHead;
        m_pCreature->UnloadModel(0xfe);
        return 1;
    }
    return CreateHead(3, pInfo);
}

// FUNCTION: SWKOTOR 0x00697B00
int CSWCCreatureAppearance::CreateTailAppearance(CCAppearanceInfo* pInfo)
{
    CExoString sModelType;
    g_pRules->Get2DArrays()->GetAppearanceTable()->GetCExoStringEntry(pInfo->m_nAppearanceType, g_nAppearanceColModelType, &sModelType);
    GetAnimType(sModelType);
    if (sModelType.Find('T', 0) == -1) {
        m_nTail = pInfo->m_nTail;
        m_pCreature->UnloadModel(0xfd);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00697BC0
int CSWCCreatureAppearance::CreateWingAppearance(CCAppearanceInfo* pInfo)
{
    CExoString sModelType;
    g_pRules->Get2DArrays()->GetAppearanceTable()->GetCExoStringEntry(pInfo->m_nAppearanceType, g_nAppearanceColModelType, &sModelType);
    GetAnimType(sModelType);
    if (sModelType.Find('W', 0) == -1) {
        m_nWing = pInfo->m_nWing;
        m_pCreature->UnloadModel(0xfc);
    }
    return 1;
}

// FUNCTION: SWKOTOR 0x00697C80
int CSWCCreatureAppearance::RequestBType(CCAppearanceInfo* pInfo)
{
    CResRef cModel;
    GetBTypeModelAndTextureResRef(pInfo, &cModel, 0);
    CResRef cTexture("");
    m_pCreature->RequestModel(cModel, cTexture, 3);
    return 1;
}

// FUNCTION: SWKOTOR 0x00698A40
int CSWCCreatureAppearance::RequestBodyAppearance(CCAppearanceInfo* pInfo)
{
    CExoString sModelType;
    g_pRules->Get2DArrays()->GetAppearanceTable()->GetCExoStringEntry(pInfo->m_nAppearanceType, "MODELTYPE", &sModelType);
    int nResult;
    switch (GetAnimType(sModelType)) {
    case 2:
    case 11:
        nResult = RequestSimple(pInfo);
        break;
    case 3:
        nResult = RequestBType(pInfo);
        break;
    default:
        nResult = 0;
        break;
    }
    return nResult;
}

// FUNCTION: SWKOTOR 0x00698B10
int CSWCCreatureAppearance::CreateBodyAppearance(CCAppearanceInfo* pInfo)
{
    CExoString sModelType;
    g_pRules->Get2DArrays()->GetAppearanceTable()->GetFLOATEntry(pInfo->m_nAppearanceType,
                                                                 g_nAppearanceColUnknown20D8, &m_fUnknown58);
    g_pRules->Get2DArrays()->GetAppearanceTable()->GetFLOATEntry(pInfo->m_nAppearanceType,
                                                                 g_nAppearanceColUnknown20DC, &m_fUnknown5C);
    g_pRules->Get2DArrays()->GetAppearanceTable()->GetFLOATEntry(pInfo->m_nAppearanceType,
                                                                 g_nAppearanceColUnknown20E0, &m_fUnknown60);
    g_pRules->Get2DArrays()->GetAppearanceTable()->GetFLOATEntry(pInfo->m_nAppearanceType,
                                                                 g_nAppearanceColUnknown20E4, &m_fUnknown64);
    if (m_fUnknown58 == 0.0f) {
        m_fUnknown58 = 25.0f;
    }
    if (m_fUnknown5C == 0.0f) {
        m_fUnknown5C = 6.0f;
    }
    if (m_fUnknown60 == 0.0f) {
        m_fUnknown60 = 2.0f;
    }
    if (m_fUnknown64 == 0.0f) {
        m_fUnknown64 = 4.0f;
    }
    CSWSCreature* pServerCreature = m_pCreature->AsServerCreature();
    if (pServerCreature != 0) {
        m_fUnknown5C *= pServerCreature->GetMovementRateFactor();
    }
    float fValue;
    g_pRules->Get2DArrays()->GetAppearanceTable()->GetFLOATEntry(pInfo->m_nAppearanceType, g_nAppearanceColHitRadius,
                                                                 &fValue);
    m_fUnknown24 = fValue;
    g_pRules->Get2DArrays()->GetAppearanceTable()->GetCExoStringEntry(pInfo->m_nAppearanceType,
                                                                      g_nAppearanceColModelType, &sModelType);
    unsigned char nAnimType = GetAnimType(sModelType);
    int bResult;
    switch (nAnimType) {
    case 2:
    case 11:
        bResult = CreateSimple(nAnimType, pInfo);
        break;
    case 3:
        bResult = CreateBTypeBody(nAnimType, pInfo);
        break;
    default:
        bResult = 0;
        break;
    }
    g_pRules->Get2DArrays()->GetAppearanceTable()->GetFLOATEntry(pInfo->m_nAppearanceType, g_nAppearanceColHeight,
                                                                 &fValue);
    m_fHeadHeight = fValue;
    g_pRules->Get2DArrays()->GetAppearanceTable()->GetINTEntry(pInfo->m_nAppearanceType,
                                                               g_nAppearanceColUnknown20F0, &m_nUnknown38);
    if (bResult) {
        if ((nAnimType == 2 || nAnimType == 3) && sModelType.Find("L", 0) == -1) {
            if (m_pCreature->GetAnimBase() != 0) {
                m_pCreature->GetAnimBase()->SetCheckItems(0);
            }
            m_pCreature->SetLightingFlag(1);
        }
        else {
            if (m_pCreature->GetAnimBase() != 0) {
                m_pCreature->GetAnimBase()->SetCheckItems(1);
            }
            m_pCreature->SetLightingFlag(2);
        }
        m_pCreature->UpdateWeaponClass();
    }
    return bResult;
}

// FUNCTION: SWKOTOR 0x00698550
int CSWCCreatureAppearance::CreateSimple(unsigned char nAnimType, CCAppearanceInfo* pInfo)
{
    CResRef cModel;
    CResRef cEnvMap;
    CExoString sRace;
    CExoString sValue;
    g_pRules->Get2DArrays()->GetAppearanceTable()->GetCExoStringEntry(pInfo->m_nAppearanceType, "RACE", &sRace);
    g_pRules->Get2DArrays()->GetAppearanceTable()->GetCExoStringEntry(pInfo->m_nAppearanceType, "ENVMAP", &sValue);
    int bHasEnvMap;
    if (sValue.GetLength() > 0) {
        cEnvMap = sValue;
        bHasEnvMap = 1;
    }
    else {
        bHasEnvMap = 0;
    }
    cModel = sRace;
    if (m_nAppearanceType == pInfo->m_nAppearanceType) {
        return 1;
    }
    // Unused in the original: constructed and destroyed around the model swap.
    CExoArrayList<CAurObject*> lUnused;
    CAurObject* pModel = m_pCreature->GetModel(0xfb, 1);
    if (pModel != 0) {
        pModel->SetVisible(0);
        pModel->AttachToObject(0, 0, 0);
    }
    pModel = m_pCreature->GetModel(0xf7, 1);
    if (pModel != 0) {
        pModel->SetVisible(0);
        pModel->AttachToObject(0, 0, 0);
    }
    m_pCreature->UnloadAttachedModel();
    int bReattach = m_pCreature->Unknown11C();
    m_pCreature->UnloadModel(0xff);
    m_pCreature->ResetAppearanceState();
    if (!m_pCreature->LoadModel(cModel, 0xff, nAnimType)) {
        return 0;
    }
    if (bReattach) {
        m_pCreature->Unknown118();
    }
    if (bHasEnvMap) {
        m_pCreature->SetEnvMapPart(0xff, 1, 1, cEnvMap);
    }
    else {
        m_pCreature->SetEnvMapPart(0xff, 0, 1, "DEFAULT");
    }
    if (g_pAppManager->GetClientExoApp()->GetPlayerCreature() == m_pCreature) {
        m_pCreature->GetModel(0xff, 1)->SetUnknown40(0);
    }
    *(CCAppearanceInfo*)this = *pInfo;
    m_nUnknown40 = 1;
    m_nUnknown44 = 1;
    m_nUnknown48 = 1;
    CExoString sModelType;
    g_pRules->Get2DArrays()->GetAppearanceTable()->GetCExoStringEntry(pInfo->m_nAppearanceType, "MODELTYPE",
                                                                      &sModelType);
    nAnimType = GetAnimType(sModelType);
    if ((nAnimType == 2 || nAnimType == 3) && sModelType.Find('L', 0) == -1) {
        if (m_pCreature->GetAnimBase() != 0) {
            m_pCreature->GetAnimBase()->SetCheckItems(0);
        }
        m_pCreature->SetLightingFlag(1);
    }
    else {
        if (m_pCreature->GetAnimBase() != 0) {
            m_pCreature->GetAnimBase()->SetCheckItems(1);
        }
        m_pCreature->SetLightingFlag(2);
    }
    if (nAnimType != 3 &&
        g_pRules->Get2DArrays()->GetAppearanceTable()->GetCExoStringEntry(pInfo->m_nAppearanceType, "RaceTex",
                                                                          &sValue)) {
        if (g_pExoResMan->Exists(sValue, 3, 0) || g_pExoResMan->Exists(sValue, 0xbbf, 0)) {
            m_pCreature->ReplaceTextures("", sValue, 1);
        }
    }
    m_pCreature->UpdateWeaponClass();
    SwitchWeaponEvent(0, "AppearanceForceUpdate", m_pCreature);
    m_bForceAnimate = g_pAppManager->GetClientExoApp()->GetPausedByCombat();
    return 1;
}

// FUNCTION: SWKOTOR 0x00697CE0
int CSWCCreatureAppearance::CreateBTypeBody(unsigned char nAnimType, CCAppearanceInfo* pInfo)
{
    CResRef cModel;
    CResRef cTexture;
    CResRef cEnvMap;
    CExoString sRace;
    CExoString sEnvMap;
    CExoString sUnused;
    char szMessage[128];
    g_pRules->Get2DArrays()->GetAppearanceTable()->GetCExoStringEntry(pInfo->m_nAppearanceType, "RACE", &sRace);
    g_pRules->Get2DArrays()->GetAppearanceTable()->GetCExoStringEntry(pInfo->m_nAppearanceType, "ENVMAP", &sEnvMap);
    int bHasEnvMap;
    if (sEnvMap.GetLength() > 0) {
        cEnvMap = sEnvMap;
        bHasEnvMap = 1;
    }
    else {
        bHasEnvMap = 0;
    }
    GetBTypeModelAndTextureResRef(pInfo, &cModel, &cTexture);
    if (m_nAppearanceType == pInfo->m_nAppearanceType && m_nUnknown14 == pInfo->m_nUnknown14 &&
        m_nBodyVariation == pInfo->m_nBodyVariation && m_nTextureVariation == pInfo->m_nTextureVariation &&
        m_nGender == pInfo->m_nGender) {
        return 1;
    }
    m_nBodyVariation = pInfo->m_nBodyVariation;
    m_nTextureVariation = pInfo->m_nTextureVariation;
    m_nAppearanceType = pInfo->m_nAppearanceType;
    m_nUnknown14 = pInfo->m_nUnknown14;
    m_nGender = pInfo->m_nGender;
    CAurObject* pHead = m_pCreature->GetModel(0xfe, 1);
    if (pHead != 0) {
        pHead->SetUnknown58(0);
    }
    int bReattach = m_pCreature->Unknown11C();
    m_pCreature->UnloadModel(0xff);
    if (!m_pCreature->LoadModel(cModel, 0xff, nAnimType)) {
        sprintf(szMessage, "CSWCCreatureAppearance::CreateBTypeBody(): Failed to load model '%s'.",
                cModel.GetResRefStr());
        return 0;
    }
    CAurObject* pModel = m_pCreature->GetModel(0xff, 1);
    if (pModel != 0) {
        pModel->SetPosition(m_pCreature->m_vPosition);
    }
    if (bReattach) {
        m_pCreature->Unknown118();
    }
    if (bHasEnvMap) {
        m_pCreature->SetEnvMapPart(0xff, 1, 1, cEnvMap);
    }
    else {
        m_pCreature->SetEnvMapPart(0xff, 0, 1, "DEFAULT");
    }
    if (g_pAppManager->m_pServerExoApp == 0 ||
        g_pAppManager->GetClientExoApp()->IsPlayerCreature(m_pCreature)) {
        m_pCreature->GetModel(0xff, 1)->SetUnknown40(0);
    }
    if (cTexture != "") {
        m_pCreature->ReplaceTexture(0xff, "", cTexture, 1);
    }
    m_nAppearanceType = pInfo->m_nAppearanceType;
    m_nUnknown40 = 1;
    m_nUnknown44 = 1;
    m_nUnknown48 = 1;
    SwitchWeaponEvent(0, "AppearanceForceUpdate", m_pCreature);
    if (pHead != 0) {
        CAurObject* pBody = m_pCreature->GetModel(0xff, 1);
        pHead->AttachToObject(pBody, "HeadHook", 0);
    }
    m_nUnknown50 = 1;
    m_bForceAnimate = g_pAppManager->GetClientExoApp()->GetPausedByCombat();
    return 1;
}

// FUNCTION: SWKOTOR 0x00698150
void CSWCCreatureAppearance::SetAppearanceAlignment(short nAlignment)
{
    int bHeadTexture;
    if (m_nUnknown50 == 0 && m_nAlignment == nAlignment) {
        return;
    }
    m_nAlignment = nAlignment;
    if (m_pCreature == 0 || m_pCreature->GetModel(0xfe, 0) == 0) {
        return;
    }
    CExoString sHeadTexture;
    CExoString sBodyTexture;
    bHeadTexture = 0;
    C2DA* pTable = g_pRules->Get2DArrays()->GetAppearanceTable();
    int bBodyTexture = 0;
    if (pTable == 0) {
        return;
    }
    m_nUnknown50 = 0;
    int nHead;
    if (m_nUnknown34 != 0xff) {
        nHead = m_nUnknown34;
        m_nUnknown40 = 1;
    }
    else if (m_nUnknown30 != 0) {
        pTable->GetINTEntry(m_nAppearanceType, g_nAppearanceColUnknown20C4, &nHead);
    }
    else {
        pTable->GetINTEntry(m_nAppearanceType, g_nAppearanceColUnknown2114, &nHead);
    }
    if (nAlignment <= 10) {
        bHeadTexture = g_pRules->Get2DArrays()->GetHeadTable()->GetCExoStringEntry(nHead, "HEADTEXVVVE", &sHeadTexture);
    }
    else if (nAlignment <= 20) {
        bHeadTexture = g_pRules->Get2DArrays()->GetHeadTable()->GetCExoStringEntry(nHead, "HEADTEXVVE", &sHeadTexture);
    }
    else if (nAlignment <= 30) {
        bHeadTexture = g_pRules->Get2DArrays()->GetHeadTable()->GetCExoStringEntry(nHead, "HEADTEXVE", &sHeadTexture);
    }
    else if (nAlignment <= 40) {
        bHeadTexture = g_pRules->Get2DArrays()->GetHeadTable()->GetCExoStringEntry(nHead, "HEADTEXE", &sHeadTexture);
    }
    if (m_nUnknown14 == 0 && nAlignment <= 30) {
        bBodyTexture = g_pRules->Get2DArrays()->GetAppearanceTable()->GetCExoStringEntry(m_nAppearanceType, "TEXAEVIL",
                                                                                         &sBodyTexture);
    }
    if (bHeadTexture) {
        if (g_pExoResMan->Exists(sHeadTexture, 3, 0) || g_pExoResMan->Exists(sHeadTexture, 0xbbf, 0)) {
            m_pCreature->ReplaceTexture(0xfe, "", sHeadTexture, 1);
        }
    }
    else {
        m_pCreature->RestoreTexturePart(0xfe);
    }
    if (bBodyTexture) {
        char szTexture[32];
        sprintf(szTexture, "%s01", sBodyTexture.CStr());
        sBodyTexture = szTexture;
        if (g_pExoResMan->Exists(sBodyTexture, 3, 0) || g_pExoResMan->Exists(sBodyTexture, 0xbbf, 0)) {
            m_pCreature->ReplaceTexture(0xff, "", sBodyTexture, 1);
        }
    }
    else {
        CResRef cTexture;
        GetBTypeModelAndTextureResRef(this, 0, &cTexture);
        if (cTexture != "") {
            m_pCreature->ReplaceTexture(0xff, "", cTexture, 1);
        }
        if (g_pAppManager->m_pServerExoApp == 0 ||
            g_pAppManager->GetClientExoApp()->IsPlayerCreature(m_pCreature)) {
            m_pCreature->GetModel(0xff, 1)->SetUnknown40(0);
        }
    }
}

// FUNCTION: SWKOTOR 0x006964C0
int CSWCCreatureAppearance::CreateHead(unsigned char nAnimType, CCAppearanceInfo* pInfo)
{
    CResRef cModel;
    CResRef cTexture;
    CResRef cEnvMap;
    CExoString sModel;
    CExoString sTexture;
    CExoString sValue;
    if (pInfo->m_nHead == 0 && pInfo->m_oidHeadItem == g_oidInvalid) {
        m_pCreature->UnloadAttachedModel();
        m_pCreature->UnloadModel(0xfe);
        m_pCreature->ResetAppearanceState();
        m_oidHeadItem = pInfo->m_oidHeadItem;
        m_nHead = pInfo->m_nHead;
        return 1;
    }
    g_pRules->Get2DArrays()->GetAppearanceTable()->GetCExoStringEntry(pInfo->m_nAppearanceType,
                                                                      g_nAppearanceColEnvMap, &sValue);
    int bHasEnvMap;
    char szNumber[4];
    if (sValue.GetLength() > 0) {
        cEnvMap = sValue;
        bHasEnvMap = 1;
    }
    else {
        bHasEnvMap = 0;
    }
    if (nAnimType == 3) {
        int nHead;
        if (pInfo->m_nUnknown34 != 0xff) {
            nHead = pInfo->m_nUnknown34;
            m_nUnknown40 = 1;
        }
        else {
            g_pRules->Get2DArrays()->GetAppearanceTable()->GetINTEntry(
                pInfo->m_nAppearanceType,
                pInfo->m_nUnknown30 ? g_nAppearanceColUnknown20C4 : g_nAppearanceColUnknown2114, &nHead);
        }
        g_pRules->Get2DArrays()->GetHeadTable()->GetCExoStringEntry(nHead, "HEAD", &sModel);
        if (sModel.IsEmpty()) {
            return 0;
        }
        cModel = sModel;
        cTexture = sModel;
    }
    else {
        sModel = "P";
        g_pRules->Get2DArrays()->GetGenderTable()->GetCExoStringEntry(pInfo->m_nGender == 1, g_nGenderColGender,
                                                                      &sValue);
        sModel = sModel + sValue;
        g_pRules->Get2DArrays()->GetAppearanceTable()->GetCExoStringEntry(pInfo->m_nAppearanceType,
                                                                          g_nAppearanceColRace, &sValue);
        sModel = sModel + sValue;
        sTexture = sModel;
        sModel = sModel + CExoString(pInfo->m_nHeadModel);
        sModel = sModel + "_HEAD";
        sTexture = sTexture + "0_HEAD";
        sprintf(szNumber, "%03u", pInfo->m_nHead);
        sModel = sModel + szNumber;
        sTexture = sTexture + szNumber;
        cModel = sModel;
        cTexture = sTexture;
    }
    if (m_nUnknown40 == 0 && m_nAppearanceType == pInfo->m_nAppearanceType && m_nGender == pInfo->m_nGender &&
        m_nHead == pInfo->m_nHead && m_oidHeadItem == pInfo->m_oidHeadItem &&
        m_nUnknown34 == pInfo->m_nUnknown34 && m_nUnknown30 == pInfo->m_nUnknown30) {
        if (bHasEnvMap) {
            m_pCreature->SetEnvMapPart(0xfe, 1, 1, cEnvMap);
        }
        else {
            m_pCreature->SetEnvMapPart(0xfe, 0, 1, "DEFAULT");
        }
        return 1;
    }
    m_pCreature->UnloadAttachedModel();
    m_pCreature->UnloadModel(0xfe);
    m_pCreature->ResetAppearanceState();
    m_nHead = pInfo->m_nHead;
    m_nUnknown34 = pInfo->m_nUnknown34;
    m_nUnknown30 = pInfo->m_nUnknown30;
    if (!m_pCreature->LoadModel(cModel, 0xfe, nAnimType)) {
        return 0;
    }
    if (nAnimType == 1 || nAnimType == 3) {
        if (m_nUnknown40 != 0 || m_oidHeadItem != pInfo->m_oidHeadItem) {
            CSWCItem* pOldItem = (CSWCItem*)g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(m_oidHeadItem);
            if (pOldItem != 0) {
                CSWCObject* pOldObject = pOldItem;
                CAurObject* pOldModel = pOldObject->GetModel(0xff, 1);
                if (pOldModel != 0) {
                    pOldModel->AttachToObject(0, 0, 0);
                    pOldModel->SetAlpha(1.0f, 1);
                    pOldModel->SetUnknown40(1);
                }
                if (pOldItem->m_pArea != 0) {
                    pOldObject->AttachToScene(0);
                }
            }
            if (pInfo->m_oidHeadItem != g_oidInvalid) {
                CSWCItem* pNewItem =
                    (CSWCItem*)g_pAppManager->GetClientExoApp()->GetItemByGameObjectID(pInfo->m_oidHeadItem);
                if (pNewItem == 0) {
                    return 0;
                }
                CAurObject* pHeadModel = m_pCreature->GetModel(0xfe, 1);
                CAurObject* pItemModel = pNewItem->GetModel(0xff, 1);
                if (pItemModel != 0) {
                    pItemModel->AttachToObject(pHeadModel, "GoggleHook", 0);
                    pItemModel->PlayAnimation("default", 1.0f, 0, 0);
                }
            }
        }
        m_pCreature->GetAnimBase()->SetHeadItem(pInfo->m_oidHeadItem);
    }
    m_oidHeadItem = pInfo->m_oidHeadItem;
    if (g_pAppManager->GetClientExoApp()->GetPlayerCreature() == m_pCreature) {
        m_pCreature->GetModel(0xfe, 1)->SetUnknown40(0);
    }
    if (bHasEnvMap) {
        m_pCreature->SetEnvMapPart(0xfe, 1, 1, cEnvMap);
    }
    else {
        m_pCreature->SetEnvMapPart(0xfe, 0, 1, "DEFAULT");
    }
    m_nUnknown50 = 1;
    m_nUnknown40 = 0;
    m_bForceAnimate = g_pAppManager->GetClientExoApp()->GetPausedByCombat();
    return 1;
}
