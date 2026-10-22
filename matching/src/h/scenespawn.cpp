// tile part spawning (0x00454A20).

#include "h/scene.h"

extern "C" char* __cdecl strcpy(char*, const char*);
extern "C" char* __cdecl strcat(char*, const char*);
extern "C" int __cdecl sprintf(char*, const char*, ...);
extern "C" double __cdecl floor(double);
#pragma intrinsic(strcpy, strcat)

class MdlNode;

class Model {
public:
    char m_pad00[8];
    char m_szName[0x20];            // +0x08
    MdlNode* m_pRoot;               // +0x28
};

class SpawnPart;

class SpawnObject {
public:
    char m_pad00[0x6c];
    SpawnPart* m_pPart;             // +0x6c
};

Model* FindModel(char* szName);                                         // 0x00464110
MdlNode* findnode(MdlNode* pRoot, char* szName);                        // 0x00452000
SpawnObject* NewCAurObject(char* szModel, char* szName, int a, int b);  // 0x00449CC0
SpawnPart* CreateInstanceTree(MdlNode* pRoot, int n);                   // 0x00449410
void PartInitMaterial(int n, SpawnPart* pPart, char* szName);           // 0x00442D70
void AurTextureSetModeStatic();                                         // 0x004204B0
void AurTextureSetModeDynamic();                                        // 0x004204C0
void SetPartTile(SpawnPart* pPart, int nTile);                          // 0x00452500
void CollectTileParts(SpawnPart* pPart);                                // 0x00454470
void PlaceTileParts(SpawnObject* pObject, SpawnPart* pPart, int n, Vector vPos,
                    Quaternion qOri, int a, int b);                     // 0x00453F10

// FUNCTION: SWKOTOR 0x00454A20
SpawnObject* SpawnPartsForTile(char* szModel, Vector* pPos, Quaternion* pOri, int nTile,
                               int bNoTileNode, int n)
{
    Model* pModel = FindModel(szModel);
    if (!pModel)
        return 0;
    AurTextureSetModeStatic();
    SpawnObject* pObject = 0;
    SpawnPart* pPart;
    char szName[0x40];
    if (!bNoTileNode) {
        strcpy(szName, pModel->m_szName);
        strcat(szName, "a");
        if (findnode(pModel->m_pRoot, szName)) {
            pObject = NewCAurObject(szModel, "", 0, 0);
            pPart = pObject->m_pPart;
            goto place;
        }
    }
    pPart = CreateInstanceTree(pModel->m_pRoot, 0);
    sprintf(szName, "(%d,%d)", (int)floor(pPos->x), (int)floor(pPos->y));
    PartInitMaterial(0, pPart, szName);
place:
    SetPartTile(pPart, nTile);
    CollectTileParts(pPart);
    PlaceTileParts(pObject, pPart, n, *pPos, *pOri, 0, 0);
    AurTextureSetModeDynamic();
    return pObject;
}
