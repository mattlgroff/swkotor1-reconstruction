#include "c/client_exo_app.h"

// FUNCTION: SWKOTOR 0x005F4100
void CClientExoAppInternal::DelayNextEnemyAutoPause()
{
    if (m_fUnknown390 <= 0.0f) {
        m_fUnknown38c = 5.0f;
    }
}

// FUNCTION: SWKOTOR 0x005F9AA0
void CClientExoAppInternal::AddOtherModelToIgnoreList(unsigned long oid,
                                                      CExoArrayList<CGob*>* pList)
{
    CGameObject* pObject = g_pAppManager->GetClientExoApp()->GetGameObject(oid);
    if (pObject == 0) return;
    CSWCObject* pSubObject = pObject->AsSWCObject();
    if (pSubObject == 0) return;
    CGob* pModel = pSubObject->FindPart(0xff, 1);
    if (pModel != 0) pList->Add(pModel);
}
