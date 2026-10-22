// CSWCMiniGame::AddEnemy: the one id-list Add with Allocate inlined, so it
// lives in its own unit (M_INLINE_ID_ALLOCATE).

#define M_INLINE_ID_ALLOCATE
#include "m/sw_mini_game_object.h"

// FUNCTION: SWKOTOR 0x00671E40
CSWMiniEnemy* CSWCMiniGame::AddEnemy(CExoArrayList<CResRef>* pModels,
                                     CExoArrayList<unsigned char>* pAttach, const CResRef& cTrack)
{
    int nModels = pModels->num;
    CSWMiniEnemy* pEnemy = new CSWMiniEnemy(this);
    char szModel[17];
    for (int i = 0; i < nModels; i++) {
        pModels->element[i].CopyToString(szModel);
        CAurObject* pModel = NewCAurObject(szModel, "", 0, 0);
        if (pModel) {
            pEnemy->AddModel(pModel, pAttach->element[i]);
        }
    }
    cTrack.CopyToString(szModel);
    CAurObject* pTrack = NewCAurObject(szModel, "", 0, 0);
    if (pTrack) {
        CSWMiniTrackPoint* pPoint = FindTrackPoint(cTrack);
        if (pPoint) {
            pTrack->SetLocalPosition(pPoint->m_vPos);
        }
        pTrack->SetScene(m_pScene->GetScene());
        pEnemy->SetModelIfChanged(pTrack, 0);
    }
    m_lstEnemies.Add(pEnemy->m_nId);
    return pEnemy;
}
