#include "n/object_control_family.h"

// Descriptive names for two independently recovered event handlers.
// FUNCTION: SWKOTOR 0x0063D3F0
void CSWCObject::SetAnimationFromEvent(unsigned long nAnimation,int nEvent)
{
    m_nAnimationEvent = nEvent;
    if (nAnimation != m_nCurrentAnimation) {
        if (AnimationFireAndForget((unsigned short)nAnimation))
            AddAnimation((unsigned short)nAnimation,0);
        else
            SetAnimation((unsigned short)nAnimation,0,0);
    }
}

// FUNCTION: SWKOTOR 0x0063D850
void CSWCObject::OnAnimationChanged(unsigned long nOldAnimation,unsigned short nNewAnimation)
{
    int bShowItems = 1;
    if (AnimationHideEquippedItems(nNewAnimation))
        bShowItems = 0;
    if (AsSWCCreature() && AsSWCCreature()->m_pEquippedItem && bShowItems &&
        !g_pAppManager->m_pClientExoApp->GetInGameGui()->m_bItemsHidden)
        AsSWCCreature()->ShowEquippedItems();
}

// FUNCTION: SWKOTOR 0x0063F5E0
int CSWCObject::HasModel()
{
    if (m_pAnimationBase && m_pAnimationBase->GetModel(255))
        return 1;
    return 0;
}

// FUNCTION: SWKOTOR 0x0063CB70
int CSWCObject::SetEnvMap(int bEnable,int bStore,const CResRef& cEnvMap)
{
    CExoString sEnvMap;
    if (!m_pAnimationBase)
        return 0;
    CSWCModule* pModule = g_pAppManager->m_pClientExoApp->GetModule();
    if (cEnvMap == "DEFAULT") {
        if (pModule) {
            CSWCArea* pArea = pModule->m_pArea;
            if (pArea)
                CResRef(pArea->m_cEnvMap).CopyToString(sEnvMap);
        }
        if (!sEnvMap.GetLength())
            return 0;
    } else {
        cEnvMap.CopyToString(sEnvMap);
    }
    CResRef cMap(sEnvMap);
    return m_pAnimationBase->SetEnvMap(bEnable,bStore,cMap);
}

// FUNCTION: SWKOTOR 0x0063CCC0
int CSWCObject::SetEnvMapPart(unsigned char nPart,int bEnable,int bStore,const CResRef& cEnvMap)
{
    CExoString sEnvMap;
    if (!m_pAnimationBase)
        return 0;
    CSWCModule* pModule = g_pAppManager->m_pClientExoApp->GetModule();
    if (cEnvMap == "DEFAULT") {
        if (pModule) {
            CSWCArea* pArea = pModule->m_pArea;
            if (pArea)
                CResRef(pArea->m_cEnvMap).CopyToString(sEnvMap);
        }
        if (!sEnvMap.GetLength())
            return 0;
    } else {
        cEnvMap.CopyToString(sEnvMap);
    }
    CResRef cMap(sEnvMap);
    return m_pAnimationBase->SetEnvMapPart(nPart,bEnable,bStore,cMap);
}

extern "C" int __cdecl _strnicmp(const char*,const char*,unsigned int);
extern "C" int __cdecl _stricmp(const char*,const char*);
extern "C" int __cdecl sprintf(char*,const char*,...);

// FUNCTION: SWKOTOR 0x0063D330
void CSWCObject::OnAnimationEvent(const char* szModel,const char* szEvent)
{
    if (m_nPendingAnimation) {
        if (!_strnicmp(szEvent,"DONE",4)) {
            char szDone[64];
            {
                CExoString sAnimation = m_pAnimationBase->GetAnimationName(m_nCurrentAnimation);
                sprintf(szDone,"done%s",sAnimation.CStr());
            }
            if (!_stricmp(szEvent,szDone)) {
                m_nCurrentAnimation = m_nPendingAnimation;
                m_nPendingAnimation = 0;
            }
        }
    }
}

// FUNCTION: SWKOTOR 0x0063E510
void __cdecl ObjectAnimationCallback(const char* szModel,const char* szEvent,CSWCObject* pObject)
{
    pObject->OnAnimationEvent(szModel,szEvent);
}

// FUNCTION: SWKOTOR 0x0063CFD0
void CSWCObject::SetPortraitRef(const CResRef& cRef)
{
    CResRef cPortrait = m_cPortrait = cRef;
    SetPortraitResource(cPortrait,1);
}
