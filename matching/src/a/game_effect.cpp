// CGameEffect (0x00503590-0x00504830) and the action-target record
// at 0x005034D0. Names follow NWN's CGameEffect where the GFF labels agree.
#include <string.h>
#include "a/game_effect.h"

// FUNCTION: SWKOTOR 0x005034D0
CSWSActionTarget::CSWSActionTarget()
{
    m_fX = 0.0f;
    m_fY = 0.0f;
    m_fZ = 0.0f;
    m_nActionId = 0;
    m_nGroupId = 0;
    m_oidArea = 0x7f000000;
    m_oidTarget = 0x7f000000;
}

// FUNCTION: SWKOTOR 0x005034F0
int CSWSActionTarget::SaveData(CResGFF* pRes, CResStruct* pStruct)
{
    pRes->WriteFieldDWORD(pStruct, m_nActionId, "ActionId");
    pRes->WriteFieldWORD(pStruct, m_nGroupId, "GroupId");
    pRes->WriteFieldDWORD(pStruct, m_oidArea, "Target_Area");
    pRes->WriteFieldDWORD(pStruct, m_oidTarget, "Target_Object");
    pRes->WriteFieldFLOAT(pStruct, m_fX, "Target_LocX");
    pRes->WriteFieldFLOAT(pStruct, m_fY, "Target_LocY");
    pRes->WriteFieldFLOAT(pStruct, m_fZ, "Target_LocZ");
    return 1;
}

// FUNCTION: SWKOTOR 0x00503590
CGameEffect::~CGameEffect()
{
    if (m_pLinkLeft) {
        delete m_pLinkLeft;
        m_pLinkLeft = NULL;
    }
    if (m_pLinkRight) {
        delete m_pLinkRight;
        m_pLinkRight = NULL;
    }
    if (m_nParamInteger) {
        delete[] m_nParamInteger;
        m_nParamInteger = NULL;
    }
}

// FUNCTION: SWKOTOR 0x00503630
void CGameEffect::SetNumIntegers(int nNum)
{
    m_nNumIntegers = nNum;
    if (m_nParamInteger) {
        delete[] m_nParamInteger;
        m_nParamInteger = NULL;
    }
    m_nParamInteger = new int[m_nNumIntegers];
    memset(m_nParamInteger, 0, m_nNumIntegers * sizeof(int));
}

// FUNCTION: SWKOTOR 0x00503690
int CGameEffect::GetInteger(int nIndex)
{
    return m_nParamInteger[nIndex];
}

// FUNCTION: SWKOTOR 0x005036A0
void CGameEffect::SetInteger(int nIndex, int nValue)
{
    m_nParamInteger[nIndex] = nValue;
}

// FUNCTION: SWKOTOR 0x005036C0
float CGameEffect::GetFloat(int nIndex)
{
    return m_nParamFloat[nIndex];
}

// FUNCTION: SWKOTOR 0x005036D0
void CGameEffect::SetFloat(int nIndex, float fValue)
{
    m_nParamFloat[nIndex] = fValue;
}

// FUNCTION: SWKOTOR 0x005036F0
void CGameEffect::SetObjectID(int nIndex, unsigned long oid)
{
    m_oidParamObjectID[nIndex] = oid;
}

// FUNCTION: SWKOTOR 0x00503700
CExoString CGameEffect::GetString(int nIndex)
{
    return m_sParamString[nIndex];
}

// FUNCTION: SWKOTOR 0x00503730
void CGameEffect::SetString(int nIndex, const CExoString& s)
{
    m_sParamString[nIndex] = s;
}

// FUNCTION: SWKOTOR 0x00503750
void CGameEffect::SetExpiryTime(unsigned long nDay, unsigned long nTime)
{
    m_nExpiryCalendarDay = nDay;
    m_nExpiryTimeOfDay = nTime;
}

// FUNCTION: SWKOTOR 0x00503770
void CGameEffect::GetExpiryTime(unsigned long* pDay, unsigned long* pTime)
{
    *pDay = m_nExpiryCalendarDay;
    *pTime = m_nExpiryTimeOfDay;
}

// FUNCTION: SWKOTOR 0x00503790
void CGameEffect::SaveGameEffect(CResGFF* pRes, CResStruct* pStruct)
{
    CResList cList;
    CResStruct cElement;
    unsigned long nDay;
    unsigned long nTime;
    unsigned long i;

    pRes->WriteFieldDWORD64(pStruct, GetID(), "Id");
    pRes->WriteFieldWORD(pStruct, GetType(), "Type");
    pRes->WriteFieldWORD(pStruct, GetSubType(), "SubType");
    pRes->WriteFieldFLOAT(pStruct, GetDuration(), "Duration");
    pRes->WriteFieldBYTE(pStruct, GetSkipOnLoad(), "SkipOnLoad");
    GetExpiryTime(&nDay, &nTime);
    pRes->WriteFieldDWORD(pStruct, nDay, "ExpireDay");
    pRes->WriteFieldDWORD(pStruct, nTime, "ExpireTime");
    pRes->WriteFieldDWORD(pStruct, GetCreator(), "CreatorId");
    pRes->WriteFieldDWORD(pStruct, GetSpellId(), "SpellId");
    pRes->WriteFieldINT(pStruct, GetExpose(), "IsExposed");
    pRes->WriteFieldINT(pStruct, GetNumIntegers(), "NumIntegers");
    if (m_nNumIntegers > 0) {
        pRes->AddList(&cList, pStruct, "IntList");
        for (i = 0; i < (unsigned long)m_nNumIntegers; i++) {
            pRes->AddListElement(&cElement, &cList, 3);
            pRes->WriteFieldINT(&cElement, m_nParamInteger[i], "Value");
        }
    }
    pRes->AddList(&cList, pStruct, "FloatList");
    for (i = 0; i < 4; i++) {
        pRes->AddListElement(&cElement, &cList, 4);
        pRes->WriteFieldFLOAT(&cElement, GetFloat(i), "Value");
    }
    pRes->AddList(&cList, pStruct, "StringList");
    for (i = 0; i < 6; i++) {
        pRes->AddListElement(&cElement, &cList, 5);
        pRes->WriteFieldCExoString(&cElement, GetString(i), "Value");
    }
    pRes->AddList(&cList, pStruct, "ObjectList");
    for (i = 0; i < 4; i++) {
        pRes->AddListElement(&cElement, &cList, 6);
        pRes->WriteFieldDWORD(&cElement, m_oidParamObjectID[i], "Value");
    }
}

// FUNCTION: SWKOTOR 0x00503A00
void CGameEffect::SetCreator(unsigned long oid)
{
    CGameObject* pObject = GetServerExoApp()->GetGameObject(oid);
    m_oidCreator = oid;
    if (pObject && pObject->AsSWSObject()) {
        m_nSpellId = pObject->AsSWSObject()->GetLastSpellId();
        if (pObject->AsSWSAreaOfEffectObject()) {
            pObject->AsSWSAreaOfEffectObject();
            m_oidCreator = pObject->AsSWSAreaOfEffectObject()->m_oidCreator;
        }
    }
}

extern const int g_nEffectTrueType74667C;  // 0x0074667C
extern const int g_nEffectTrueType746684;  // 0x00746684
extern const int g_nEffectTrueType746698;  // 0x00746698
extern const int g_nEffectTrueType7466AC;  // 0x007466AC
extern const int g_nEffectTrueType7466B8;  // 0x007466B8
extern const int g_nEffectTrueType7466C4;  // 0x007466C4
extern const int g_nEffectTrueType7466D0;  // 0x007466D0
extern const int g_nEffectTrueType7466D4;  // 0x007466D4
extern const int g_nEffectTrueType7466DC;  // 0x007466DC
extern const int g_nEffectTrueType7466E0;  // 0x007466E0
extern const int g_nEffectTrueType7466F8;  // 0x007466F8
extern const int g_nEffectTrueType7466FC;  // 0x007466FC
extern const int g_nEffectTrueType74669C;  // 0x0074669C
extern const int g_nEffectTrueType746690;  // 0x00746690
extern const int g_nEffectTrueType746730;  // 0x00746730
extern const int g_nEffectTrueType746734;  // 0x00746734
extern const int g_nEffectTrueType746680;  // 0x00746680
extern const int g_nEffectTrueType746688;  // 0x00746688
extern const int g_nEffectTrueType74670C;  // 0x0074670C
extern const int g_nEffectTrueType746710;  // 0x00746710
extern const int g_nEffectTrueType7466A4;  // 0x007466A4
extern const int g_nEffectTrueType7466A8;  // 0x007466A8
extern const int g_nEffectTrueType7466B0;  // 0x007466B0
extern const int g_nEffectTrueType7466B4;  // 0x007466B4
extern const int g_nEffectTrueType7466BC;  // 0x007466BC
extern const int g_nEffectTrueType7466C0;  // 0x007466C0
extern const int g_nEffectTrueType74673C;  // 0x0074673C
extern const int g_nEffectTrueType746740;  // 0x00746740
extern const int g_nEffectTrueType7466EC;  // 0x007466EC
extern const int g_nEffectTrueType7466F0;  // 0x007466F0
extern const int g_nEffectTrueType7466E4;  // 0x007466E4
extern const int g_nEffectTrueType7466E8;  // 0x007466E8
extern const int g_nEffectTrueType746700;  // 0x00746700
extern const int g_nEffectTrueType746704;  // 0x00746704
extern const int g_nEffectTrueType746758;  // 0x00746758
extern const int g_nEffectTrueType74675C;  // 0x0074675C
extern const int g_nEffectTrueType746738;  // 0x00746738
extern const int g_nEffectTrueType7467A4;  // 0x007467A4
extern const int g_nEffectTrueType746748;  // 0x00746748
extern const int g_nEffectTrueType746770;  // 0x00746770
extern const int g_nEffectTrueType7467C4;  // 0x007467C4
extern const int g_nEffectTrueType746774;  // 0x00746774
extern const int g_nEffectTrueType746778;  // 0x00746778
extern const int g_nEffectTrueType74679C;  // 0x0074679C
extern const int g_nEffectTrueType746794;  // 0x00746794
extern const int g_nEffectTrueType74677C;  // 0x0074677C
extern const int g_nEffectTrueType7467A0;  // 0x007467A0
extern const int g_nEffectTrueType746780;  // 0x00746780
extern const int g_nEffectTrueType74674C;  // 0x0074674C
extern const int g_nEffectTrueType746798;  // 0x00746798
extern const int g_nEffectTrueType7467A8;  // 0x007467A8
extern const int g_nEffectTrueType7467AC;  // 0x007467AC
extern const int g_nEffectTrueType746744;  // 0x00746744
extern const int g_nEffectTrueType74680C;  // 0x0074680C
extern const int g_nEffectTrueType7466F4;  // 0x007466F4
extern const int g_nEffectTrueType746808;  // 0x00746808
extern const int g_nEffectTrueType746810;  // 0x00746810
extern const int g_nEffectTrueType746818;  // 0x00746818

// FUNCTION: SWKOTOR 0x00503A70
int CGameEffect::GetScriptEffectType()
{
    int nResult = 0;

    if (m_nType == g_nEffectTrueType74667C) {
        nResult = 0;
    }
    else if (m_nType == g_nEffectTrueType746684) {
        nResult = 1;
    }
    else if (m_nType == g_nEffectTrueType746698) {
        nResult = 3;
    }
    else if (m_nType == g_nEffectTrueType7466AC) {
        nResult = 7;
    }
    else if (m_nType == g_nEffectTrueType7466B8) {
        nResult = 9;
    }
    else if (m_nType == g_nEffectTrueType7466C4) {
        nResult = 0xb;
    }
    else if (m_nType == g_nEffectTrueType7466D0) {
        nResult = 0xd;
    }
    else if (m_nType == g_nEffectTrueType7466D4) {
        nResult = 0xf;
    }
    else if (m_nType == g_nEffectTrueType7466DC) {
        nResult = 0x11;
    }
    else if (m_nType == g_nEffectTrueType7466E0) {
        nResult = 0x12;
    }
    else if (m_nType == g_nEffectTrueType7466F8) {
        nResult = 0x14;
    }
    else if (m_nType == g_nEffectTrueType7466FC) {
        nResult = 0x15;
    }
    else if (m_nType == g_nEffectTrueType74669C) {
        switch (m_nParamInteger[0]) {
        case 1: nResult = 0x18; break;
        case 2: nResult = 0x19; break;
        case 5: nResult = 0x1b; break;
        case 4: nResult = 0x1d; break;
        case 6: nResult = 0x1e; break;
        case 3: nResult = 0x23; break;
        }
    }
    else if (m_nType == g_nEffectTrueType746708) {
        nResult = 0x1f;
    }
    else if (m_nType == g_nEffectTrueType746690) {
        nResult = 0x20;
    }
    else if (m_nType == g_nEffectTrueType746730) {
        nResult = 0x21;
    }
    else if (m_nType == g_nEffectTrueType746734) {
        nResult = 0x22;
    }
    else if (m_nType == g_nEffectTrueType746680) {
        nResult = 0x24;
    }
    else if (m_nType == g_nEffectTrueType746688) {
        nResult = 0x25;
    }
    else if (m_nType == g_nEffectTrueType74670C) {
        nResult = 0x26;
    }
    else if (m_nType == g_nEffectTrueType746710) {
        nResult = 0x27;
    }
    else if (m_nType == g_nEffectTrueType7466A4) {
        nResult = 0x28;
    }
    else if (m_nType == g_nEffectTrueType7466A8) {
        nResult = 0x29;
    }
    else if (m_nType == g_nEffectTrueType7466B0) {
        nResult = 0x2a;
    }
    else if (m_nType == g_nEffectTrueType7466B4) {
        nResult = 0x2b;
    }
    else if (m_nType == g_nEffectTrueType7466BC) {
        nResult = 0x2c;
    }
    else if (m_nType == g_nEffectTrueType7466C0) {
        nResult = 0x2d;
    }
    else if (m_nType == g_nEffectTrueType74673C) {
        nResult = 0x2e;
    }
    else if (m_nType == g_nEffectTrueType746740) {
        nResult = 0x2f;
    }
    else if (m_nType == g_nEffectTrueType7466EC) {
        nResult = 0x30;
    }
    else if (m_nType == g_nEffectTrueType7466F0) {
        nResult = 0x31;
    }
    else if (m_nType == g_nEffectTrueType7466E4) {
        nResult = 0x32;
    }
    else if (m_nType == g_nEffectTrueType7466E8) {
        nResult = 0x33;
    }
    else if (m_nType == g_nEffectTrueType746700) {
        nResult = 0x34;
    }
    else if (m_nType == g_nEffectTrueType746704) {
        nResult = 0x35;
    }
    else if (m_nType == g_nEffectTrueType746758) {
        nResult = 0x36;
    }
    else if (m_nType == g_nEffectTrueType74675C) {
        nResult = 0x37;
    }
    else if (m_nType == g_nEffectTrueType746738) {
        nResult = 0x38;
    }
    else if (m_nType == g_nEffectTrueType7467A4) {
        nResult = 0x3a;
    }
    else if (m_nType == g_nEffectTrueType746748) {
        nResult = 0x3b;
    }
    else if (m_nType == g_nEffectTrueType746770) {
        nResult = 0x3c;
    }
    else if (m_nType == g_nEffectTrueType7467C4) {
        nResult = 0x3d;
    }
    else if (m_nType == g_nEffectTrueType746774) {
        nResult = 0x3e;
    }
    else if (m_nType == g_nEffectTrueType746778) {
        nResult = 0x3f;
    }
    else if (m_nType == g_nEffectTrueType74679C) {
        nResult = 0x40;
    }
    else if (m_nType == g_nEffectTrueType746794) {
        nResult = 0x41;
    }
    else if (m_nType == g_nEffectTrueType74677C) {
        nResult = 0x42;
    }
    else if (m_nType == g_nEffectTrueType7467A0) {
        nResult = 0x43;
    }
    else if (m_nType == g_nEffectTrueType746780) {
        nResult = 0x44;
    }
    else if (m_nType == g_nEffectTrueType74674C) {
        nResult = 0x45;
    }
    else if (m_nType == g_nEffectTrueType746798) {
        nResult = 0x46;
    }
    else if (m_nType == g_nEffectTrueType7467A8) {
        nResult = 0x47;
    }
    else if (m_nType == g_nEffectTrueType7467AC) {
        nResult = 0x48;
    }
    else if (m_nType == g_nEffectTrueType746744) {
        nResult = 0x49;
    }
    else if (m_nType == g_nEffectTrueType74680C) {
        nResult = 0x4a;
    }
    else if (m_nType == g_nEffectTrueType7466F4) {
        nResult = 0x4b;
    }
    else if (m_nType == g_nEffectTrueType746808) {
        nResult = 0x4c;
    }
    else if (m_nType == g_nEffectTrueType746810) {
        nResult = 0x4d;
    }
    else if (m_nType == g_nEffectTrueType746818) {
        nResult = 0x4e;
    }
    return nResult;
}

// FUNCTION: SWKOTOR 0x00503E40 SYMBOL: ??0CGameEffect@@QAE@H@Z
CGameEffect::CGameEffect(int bCreateNewID)
{
    if (bCreateNewID == 1) {
        m_nID = g_nNextEffectID;
        g_nNextEffectID++;
    }
    else {
        m_nID = 0;
    }
    m_bExpose = bCreateNewID;
    m_bSkipOnLoad = 0;
    m_nType = 0;
    m_nSubType = 0;
    m_fDuration = 0.0f;
    m_nExpiryCalendarDay = 0;
    m_nExpiryTimeOfDay = 0;
    m_oidCreator = 0x7f000000;
    m_pLinkLeft = NULL;
    m_pLinkRight = NULL;
    m_n24 = -1;
    m_nSpellId = 0xffffffff;
    m_nParamInteger = NULL;
    SetNumIntegers(8);
    memset(m_nParamFloat, 0, sizeof(m_nParamFloat));
    for (int j = 0; j < 6; j++) {
        m_sParamString[j] = "";
    }
    for (int k = 0; k < 4; k++) {
        m_oidParamObjectID[k] = 0x7f000000;
    }
}

// FUNCTION: SWKOTOR 0x00503F70 SYMBOL: ??0CGameEffect@@QAE@PAV0@@Z
CGameEffect::CGameEffect(CGameEffect* pCopy)
{
    m_nID = 0;
    m_bExpose = 0;
    m_nType = 0;
    m_nSubType = 0;
    m_fDuration = 0.0f;
    m_nExpiryCalendarDay = 0;
    m_nExpiryTimeOfDay = 0;
    m_oidCreator = 0x7f000000;
    m_pLinkLeft = NULL;
    m_pLinkRight = NULL;
    m_nParamInteger = NULL;
    m_nSpellId = 0xffffffff;
    m_n24 = 0;
    SetNumIntegers(pCopy->m_nNumIntegers);
    memset(m_nParamFloat, 0, sizeof(m_nParamFloat));
    for (int j = 0; j < 6; j++) {
        m_sParamString[j] = "";
    }
    for (int k = 0; k < 4; k++) {
        m_oidParamObjectID[k] = 0x7f000000;
    }
    m_nID = pCopy->m_nID;
    m_nSubType = pCopy->m_nSubType;
    m_fDuration = pCopy->m_fDuration;
    m_nExpiryCalendarDay = pCopy->m_nExpiryCalendarDay;
    m_nExpiryTimeOfDay = pCopy->m_nExpiryTimeOfDay;
    m_oidCreator = pCopy->m_oidCreator;
    m_nSpellId = pCopy->m_nSpellId;
    m_bSkipOnLoad = 0;
}

// FUNCTION: SWKOTOR 0x00504090
void CGameEffect::CopyEffect(CGameEffect* pFrom, int bSkipIntegers)
{
    if (pFrom == NULL) {
        return;
    }
    if (m_nID == 0) {
        m_nID = pFrom->m_nID;
    }
    m_nType = pFrom->m_nType;
    m_nSubType = pFrom->m_nSubType;
    m_bExpose = pFrom->m_bExpose;
    m_fDuration = pFrom->m_fDuration;
    m_nExpiryCalendarDay = pFrom->m_nExpiryCalendarDay;
    m_nExpiryTimeOfDay = pFrom->m_nExpiryTimeOfDay;
    m_oidCreator = pFrom->m_oidCreator;
    m_nSpellId = pFrom->m_nSpellId;
    m_n24 = pFrom->m_n24;
    if (pFrom->m_pLinkLeft) {
        m_pLinkLeft = new CGameEffect(0);
        m_pLinkLeft->CopyEffect(pFrom->m_pLinkLeft, 0);
    }
    if (pFrom->m_pLinkRight) {
        m_pLinkRight = new CGameEffect(0);
        m_pLinkRight->CopyEffect(pFrom->m_pLinkRight, 0);
    }
    SetNumIntegers(pFrom->m_nNumIntegers);
    if (bSkipIntegers) {
        return;
    }
    for (int i = 0; i < m_nNumIntegers; i++) {
        m_nParamInteger[i] = pFrom->m_nParamInteger[i];
    }
    m_nParamFloat[0] = pFrom->m_nParamFloat[0];
    m_nParamFloat[1] = pFrom->m_nParamFloat[1];
    m_nParamFloat[2] = pFrom->m_nParamFloat[2];
    m_nParamFloat[3] = pFrom->m_nParamFloat[3];
    m_oidParamObjectID[0] = pFrom->m_oidParamObjectID[0];
    m_oidParamObjectID[1] = pFrom->m_oidParamObjectID[1];
    m_oidParamObjectID[2] = pFrom->m_oidParamObjectID[2];
    m_oidParamObjectID[3] = pFrom->m_oidParamObjectID[3];
    for (int j = 0; j < 6; j++) {
        m_sParamString[j] = pFrom->m_sParamString[j];
    }
}

// FUNCTION: SWKOTOR 0x00504240
void CGameEffect::UpdateLinked()
{
    if (m_pLinkLeft) {
        switch (m_nSubType & 0x18) {
        case 0x08:
            m_pLinkLeft->m_nSubType = (m_pLinkLeft->m_nSubType & 0xffef) | 0x08;
            break;
        case 0x10:
            m_pLinkLeft->m_nSubType = (m_pLinkLeft->m_nSubType & 0xfff7) | 0x10;
            break;
        case 0x18:
            m_pLinkLeft->m_nSubType |= 0x18;
            break;
        }
        m_pLinkLeft->UpdateLinked();
        m_pLinkLeft->SetCreator(m_oidCreator);
        m_pLinkLeft->SetDuration(GetDuration());
        m_pLinkLeft->m_nSubType ^= (m_pLinkLeft->m_nSubType ^ m_nSubType) & 7;
        m_pLinkLeft->m_nExpiryCalendarDay = m_nExpiryCalendarDay;
        m_pLinkLeft->m_nExpiryTimeOfDay = m_nExpiryTimeOfDay;
        m_pLinkLeft->m_nSpellId = m_nSpellId;
        m_pLinkLeft->m_nID = m_nID;
    }
    if (m_pLinkRight) {
        switch (m_nSubType & 0x18) {
        case 0x08:
            m_pLinkRight->m_nSubType = (m_pLinkRight->m_nSubType & 0xffef) | 0x08;
            break;
        case 0x10:
            m_pLinkRight->m_nSubType = (m_pLinkRight->m_nSubType & 0xfff7) | 0x10;
            break;
        case 0x18:
            m_pLinkRight->m_nSubType |= 0x18;
            break;
        }
        m_pLinkRight->UpdateLinked();
        m_pLinkRight->SetCreator(m_oidCreator);
        m_pLinkRight->SetDuration(GetDuration());
        m_pLinkRight->m_nSubType ^= (m_pLinkRight->m_nSubType ^ m_nSubType) & 7;
        m_pLinkRight->m_nExpiryCalendarDay = m_nExpiryCalendarDay;
        m_pLinkRight->m_nExpiryTimeOfDay = m_nExpiryTimeOfDay;
        m_pLinkRight->m_nSpellId = m_nSpellId;
        m_pLinkRight->m_nID = m_nID;
    }
}

// FUNCTION: SWKOTOR 0x005043A0
void CGameEffect::LoadGameEffect(CResGFF* pRes, CResStruct* pStruct)
{
    int bSuccess;
    CResStruct cElement;
    CExoString sValue;
    CResList cList;
    unsigned long nIndex;

    m_nID = pRes->ReadFieldDWORD64(pStruct, "Id", bSuccess, 0);
    SetType(pRes->ReadFieldWORD(pStruct, "Type", bSuccess, 0));
    m_nSubType = pRes->ReadFieldWORD(pStruct, "SubType", bSuccess, 0);
    m_fDuration = pRes->ReadFieldFLOAT(pStruct, "Duration", bSuccess, 0.0f);
    SetSkipOnLoad(pRes->ReadFieldBYTE(pStruct, "SkipOnLoad", bSuccess, 0));
    unsigned long nDay = pRes->ReadFieldDWORD(pStruct, "ExpireDay", bSuccess, 0);
    unsigned long nTime = pRes->ReadFieldDWORD(pStruct, "ExpireTime", bSuccess, 0);
    SetExpiryTime(nDay, nTime);
    SetCreator(pRes->ReadFieldDWORD(pStruct, "CreatorId", bSuccess, 0));
    m_nSpellId = pRes->ReadFieldDWORD(pStruct, "SpellId", bSuccess, 0);
    m_bExpose = pRes->ReadFieldINT(pStruct, "IsExposed", bSuccess, 0);
    SetNumIntegers(pRes->ReadFieldINT(pStruct, "NumIntegers", bSuccess, 0));

    if ((GetType() == g_nEffectTrueType746714 || GetType() == g_nEffectTrueType746708) &&
        m_nNumIntegers < 0x15) {
        unsigned long nOld = m_nNumIntegers - 6;
        if (pRes->GetList(&cList, pStruct, "IntList") == 1) {
            nIndex = 0;
            unsigned long nList = 0;
            while (nIndex < 0x15) {
                if (nIndex < nOld) {
                    if (nList < pRes->GetListCount(&cList)) {
                        pRes->GetListElement(&cElement, &cList, nList);
                        m_nParamInteger[nList] = pRes->ReadFieldINT(&cElement, "Value", bSuccess, 0);
                        nList++;
                        nIndex++;
                    }
                }
                else if (nIndex < 0xf) {
                    m_nParamInteger[nList] = 0;
                    nIndex++;
                }
                else {
                    if (nList < pRes->GetListCount(&cList)) {
                        pRes->GetListElement(&cElement, &cList, nList);
                        m_nParamInteger[nList] = pRes->ReadFieldINT(&cElement, "Value", bSuccess, 0);
                        nList++;
                        nIndex++;
                    }
                }
            }
        }
        SetNumIntegers(0x15);
    }
    else if (m_nNumIntegers > 0) {
        if (pRes->GetList(&cList, pStruct, "IntList") == 1) {
            for (nIndex = 0; nIndex < pRes->GetListCount(&cList); nIndex++) {
                pRes->GetListElement(&cElement, &cList, nIndex);
                m_nParamInteger[nIndex] = pRes->ReadFieldINT(&cElement, "Value", bSuccess, 0);
            }
        }
    }
    if (pRes->GetList(&cList, pStruct, "FloatList") == 1) {
        for (nIndex = 0; nIndex < 4; nIndex++) {
            pRes->GetListElement(&cElement, &cList, nIndex);
            m_nParamFloat[nIndex] = pRes->ReadFieldFLOAT(&cElement, "Value", bSuccess, 0.0f);
        }
    }
    if (pRes->GetList(&cList, pStruct, "StringList") == 1) {
        for (nIndex = 0; nIndex < 6; nIndex++) {
            pRes->GetListElement(&cElement, &cList, nIndex);
            {
                CExoString sDefault("");
                sValue = pRes->ReadFieldCExoString(&cElement, "Value", bSuccess, sDefault);
            }
            m_sParamString[nIndex] = sValue;
        }
    }
    if (pRes->GetList(&cList, pStruct, "ObjectList") == 1) {
        for (nIndex = 0; nIndex < 4; nIndex++) {
            pRes->GetListElement(&cElement, &cList, nIndex);
            m_oidParamObjectID[nIndex] = pRes->ReadFieldDWORD(&cElement, "Value", bSuccess, 0);
        }
    }
}

// FUNCTION: SWKOTOR 0x00504810
void CGameEffect::SetLinked(CGameEffect* pLeft, CGameEffect* pRight)
{
    m_pLinkLeft = pLeft;
    m_pLinkRight = pRight;
    UpdateLinked();
}
