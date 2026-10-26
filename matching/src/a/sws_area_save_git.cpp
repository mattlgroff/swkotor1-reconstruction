// CSWSArea::SaveGIT (0x0050BA00): sorts the area objects by kind (player
// creatures go to the caller's list), builds the "GIT " V2.0 GFF and adds
// it to the save ERF as <area resref>.git.
#include "a/sws_area.h"

class CERFFile {
public:
    int AddResource(const char* szName, unsigned short nType, void* pData, int bOwned,
                    int nSize);                     // 0x005DDF30
};

class CSWSCreatureStatsPC {
public:
    char pad_0x0[0x6c];
    int m_bIsPC;                                    // 0x6c
};

class CSWSCreatureWithStats : public CSWSObject {
public:
    char pad_0x9c[0xa74 - 0x9c];
    CSWSCreatureStatsPC* m_pStats;                  // 0xa74
};

// FUNCTION: SWKOTOR 0x0050BA00
void CSWSArea::SaveGIT(CERFFile* pERF, CExoString* pName, CExoArrayList<unsigned long>* pPlayers)
{
    CExoString sUnused;
    CGameObjectArray* pArray = GetServerExoApp()->GetObjectArray();
    CExoArrayList<unsigned long> aCreatures;
    CExoArrayList<unsigned long> aItems;
    CExoArrayList<unsigned long> aDoors;
    CExoArrayList<unsigned long> aTriggers;
    CExoArrayList<unsigned long> aEncounters;
    CExoArrayList<unsigned long> aWaypoints;
    CExoArrayList<unsigned long> aSounds;
    CExoArrayList<unsigned long> aPlaceables;
    CExoArrayList<unsigned long> aStores;
    CExoArrayList<unsigned long> aAreaEffects;
    CResGFF* pGFF = new CResGFF;
    CResStruct* pRoot = new CResStruct;
    {
        CExoString sVersion("V2.0");
        CExoString sType("GIT ");
        pGFF->CreateGFFFile(pRoot, sType, sVersion);
    }
    for (int i = 0; i < m_aGameObjects.num; i++) {
        CGameObject* pObject;
        if (pArray->GetGameObject(m_aGameObjects.element[i], &pObject) != g_nGameObjectFound) {
            continue;
        }
        if (pObject->AsSWSCreature()) {
            if (((CSWSCreatureWithStats*)pObject->AsSWSCreature())->m_pStats->m_bIsPC) {
                pPlayers->AddInline(m_aGameObjects.element[i]);
            } else {
                aCreatures.AddInline(m_aGameObjects.element[i]);
            }
        } else if (pObject->AsSWSItem()) {
            aItems.AddInline(m_aGameObjects.element[i]);
        } else if (pObject->AsSWSDoor()) {
            aDoors.AddInline(m_aGameObjects.element[i]);
        } else if (pObject->AsSWSTrigger()) {
            aTriggers.AddInline(m_aGameObjects.element[i]);
        } else if (pObject->AsSWSEncounter()) {
            aEncounters.Add(m_aGameObjects.element[i]);
        } else if (pObject->AsSWSWaypoint()) {
            aWaypoints.Add(m_aGameObjects.element[i]);
        } else if (pObject->AsSWSSoundObject()) {
            aSounds.Add(m_aGameObjects.element[i]);
        } else if (pObject->AsSWSPlaceable()) {
            aPlaceables.Add(m_aGameObjects.element[i]);
        } else if (pObject->AsSWSStore()) {
            aStores.Add(m_aGameObjects.element[i]);
        } else if (pObject->AsSWSAreaOfEffectObject()) {
            aAreaEffects.Add(m_aGameObjects.element[i]);
        }
    }
    m_cScriptVarTable.SaveVarTable(pGFF, pRoot);
    m_cVarTable.SaveVarTable(pGFF, pRoot);
    pGFF->WriteFieldBYTE(pRoot, m_nCurrentWeather, "CurrentWeather");
    pGFF->WriteFieldBYTE(pRoot, (unsigned char)m_bWeatherStarted, "WeatherStarted");
    pGFF->WriteFieldBYTE(pRoot, (unsigned char)m_bTransPending, "TransPending");
    pGFF->WriteFieldBYTE(pRoot, m_nTransPendNextID, "TransPendNextID");
    pGFF->WriteFieldBYTE(pRoot, m_nTransPendCurrID, "TransPendCurrID");
    SaveCreatures(pGFF, pRoot, &aCreatures);
    SaveItems(pGFF, pRoot, &aItems);
    SaveDoors(pGFF, pRoot, &aDoors);
    SaveTriggers(pGFF, pRoot, &aTriggers);
    SaveEncounters(pGFF, pRoot, &aEncounters);
    SaveWaypoints(pGFF, pRoot, &aWaypoints);
    SaveSounds(pGFF, pRoot, &aSounds);
    SavePlaceables(pGFF, pRoot, &aPlaceables);
    SaveStores(pGFF, pRoot, &aStores);
    SaveAreaEffects(pGFF, pRoot, &aAreaEffects);
    SaveProperties(pGFF, pRoot);
    SaveMaps(pGFF, pRoot);
    SavePlaceableCameras(pGFF, pRoot);
    CExoString sResName;
    m_cResRef.CopyToString(sResName);
    pERF->AddResource(sResName.CStr(), 0x7e7, pGFF, 1, -1);
    delete pGFF;
    delete pRoot;
}
