// CScriptLocation / CScriptTalent (0x004CA7A0-0x004CABF4) and the
// cdecl beam-effect test 0x004CA960.
#include "i/sws_object.h"

// FUNCTION: SWKOTOR 0x004CA7A0
CScriptLocation::CScriptLocation()
{
    m_vPosition.x = 0.0f;
    m_vPosition.y = 0.0f;
    m_vPosition.z = 0.0f;
    m_vOrientation.x = 0.0f;
    m_vOrientation.y = 0.0f;
    m_vOrientation.z = 0.0f;
}

// FUNCTION: SWKOTOR 0x004CA7C0
void CScriptLocation::CopyScriptLocation(CScriptLocation* pOther)
{
    if (pOther) {
        m_vOrientation = pOther->m_vOrientation;
        m_vPosition = pOther->m_vPosition;
    }
}

// FUNCTION: SWKOTOR 0x004CA800
int CScriptLocation::LoadLocation(CResGFF* pRes, CResStruct* pStruct)
{
    int bSuccess;
    Vector v;
    v.x = pRes->ReadFieldFLOAT(pStruct, "OrientationX", bSuccess, 0.0f);
    v.y = pRes->ReadFieldFLOAT(pStruct, "OrientationY", bSuccess, 0.0f);
    v.z = pRes->ReadFieldFLOAT(pStruct, "OrientationZ", bSuccess, 0.0f);
    m_vOrientation = v;
    v.x = pRes->ReadFieldFLOAT(pStruct, "PositionX", bSuccess, 0.0f);
    v.y = pRes->ReadFieldFLOAT(pStruct, "PositionY", bSuccess, 0.0f);
    v.z = pRes->ReadFieldFLOAT(pStruct, "PositionZ", bSuccess, 0.0f);
    m_vPosition = v;
    return 1;
}

// FUNCTION: SWKOTOR 0x004CA8E0
int CScriptLocation::SaveLocation(CResGFF* pRes, CResStruct* pStruct)
{
    pRes->WriteFieldFLOAT(pStruct, m_vOrientation.x, "OrientationX");
    pRes->WriteFieldFLOAT(pStruct, m_vOrientation.y, "OrientationY");
    pRes->WriteFieldFLOAT(pStruct, m_vOrientation.z, "OrientationZ");
    pRes->WriteFieldFLOAT(pStruct, m_vPosition.x, "PositionX");
    pRes->WriteFieldFLOAT(pStruct, m_vPosition.y, "PositionY");
    pRes->WriteFieldFLOAT(pStruct, m_vPosition.z, "PositionZ");
    return 1;
}

// FUNCTION: SWKOTOR 0x004CA960
int FUN_004ca960(unsigned short nVisualEffect)
{
    if (nVisualEffect == 0x7f5 || nVisualEffect == 0x7f6 || nVisualEffect == 0x1770 ||
        nVisualEffect == 0x7ea || nVisualEffect == 0x7eb || nVisualEffect == 0x7ec ||
        nVisualEffect == 0x801 || nVisualEffect == 0x802 || nVisualEffect == 0x803 ||
        nVisualEffect == 0x804 || nVisualEffect == 0x805 || nVisualEffect == 0xfc5 ||
        nVisualEffect == 0x7ed || nVisualEffect == 0x80d || nVisualEffect == 0x811 ||
        nVisualEffect == 0x812) {
        return 1;
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x004CA9E0
CScriptTalent::CScriptTalent()
{
    m_nId = -1;
    m_nType = -1;
    m_nMultiClass = 0;
    m_oidItem = 0x7f000000;
    m_nItemPropertyIndex = -1;
    m_nCasterLevel = 0xff;
    m_nMetaType = 0xff;
}

// FUNCTION: SWKOTOR 0x004CAA10
void CScriptTalent::CopyScriptTalent(CScriptTalent* pOther)
{
    if (pOther) {
        m_nId = pOther->m_nId;
        m_nType = pOther->m_nType;
        m_nMultiClass = pOther->m_nMultiClass;
        m_oidItem = pOther->m_oidItem;
        m_nItemPropertyIndex = pOther->m_nItemPropertyIndex;
        m_nCasterLevel = pOther->m_nCasterLevel;
        m_nMetaType = pOther->m_nMetaType;
    }
}

// FUNCTION: SWKOTOR 0x004CAA50
int CScriptTalent::SaveTalent(CResGFF* pRes, CResStruct* pStruct)
{
    pRes->WriteFieldINT(pStruct, m_nId, "ID");
    pRes->WriteFieldINT(pStruct, m_nType, "Type");
    pRes->WriteFieldBYTE(pStruct, m_nMultiClass, "MultiClass");
    pRes->WriteFieldDWORD(pStruct, m_oidItem, "Item");
    pRes->WriteFieldINT(pStruct, m_nItemPropertyIndex, "ItemPropertyIndex");
    pRes->WriteFieldBYTE(pStruct, m_nCasterLevel, "CasterLevel");
    pRes->WriteFieldBYTE(pStruct, m_nMetaType, "MetaType");
    return 1;
}

// FUNCTION: SWKOTOR 0x004CAAF0
int CScriptTalent::LoadTalent(CResGFF* pRes, CResStruct* pStruct)
{
    int bSuccess;
    m_nId = pRes->ReadFieldINT(pStruct, "ID", bSuccess, 0);
    m_nType = pRes->ReadFieldINT(pStruct, "Type", bSuccess, 0);
    m_nMultiClass = pRes->ReadFieldBYTE(pStruct, "MultiClass", bSuccess, 0);
    m_oidItem = pRes->ReadFieldDWORD(pStruct, "Item", bSuccess, 0);
    m_nItemPropertyIndex = pRes->ReadFieldINT(pStruct, "ItemPropertyIndex", bSuccess, 0);
    m_nCasterLevel = pRes->ReadFieldBYTE(pStruct, "CasterLevel", bSuccess, 0);
    m_nMetaType = pRes->ReadFieldBYTE(pStruct, "MetaType", bSuccess, 0);
    return 1;
}

// FUNCTION: SWKOTOR 0x004CABB0
int CScriptTalent::Equals(CScriptTalent* pOther)
{
    if (m_nType == pOther->m_nType && m_nId == pOther->m_nId &&
        m_nMultiClass == pOther->m_nMultiClass && m_oidItem == pOther->m_oidItem &&
        m_nItemPropertyIndex == pOther->m_nItemPropertyIndex &&
        m_nCasterLevel == pOther->m_nCasterLevel && m_nMetaType == pOther->m_nMetaType) {
        return 1;
    }
    return 0;
}
