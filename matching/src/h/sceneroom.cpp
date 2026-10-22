// Scene room lookup and .VIS writer (0x00452B10-0x00452CC0) and
// the tile part classifier ProcessTilePart (0x004528B0).

#include "h/scene.h"

struct FILE;
extern "C" int __cdecl _stricmp(const char*, const char*);
extern "C" int __cdecl sprintf(char*, const char*, ...);
extern "C" int __cdecl fprintf(FILE*, const char*, ...);
extern "C" FILE* __cdecl fopen(const char*, const char*);
extern "C" int __cdecl fclose(FILE*);
extern "C" char* __cdecl strcpy(char*, const char*);
extern "C" char* __cdecl strcat(char*, const char*);
#pragma intrinsic(strcpy, strcat)

// FUNCTION: SWKOTOR 0x00452B10
CAurRoom* Scene::FindRoom(char* szName)
{
    for (int i = 0; i < m_lRooms.num; i++) {
        if (!_stricmp(szName, m_lRooms.element[i]->m_szName))
            return m_lRooms.element[i];
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x00452B70
int Scene::WriteVisibility(char* szDir, char* szName)
{
    if (!szDir || !*szDir)
        szDir = ".";
    char szPath[0x100];
    sprintf(szPath, "%s/%s.VIS", szDir, szName);
    FILE* pFile = fopen(szPath, "w");
    if (!pFile)
        return 0;
    for (int i = 0; i < m_lRooms.num; i++) {
        fprintf(pFile, "%s %d\n", m_lRooms.element[i]->m_szName, m_lRooms.element[i]->m_lVisible.num);
        for (int j = 0; j < m_lRooms.element[i]->m_lVisible.num; j++)
            fprintf(pFile, "  %s\n", m_lRooms.element[i]->m_lVisible.element[j]->m_szName);
    }
    fclose(pFile);
    return 1;
}

// FUNCTION: SWKOTOR 0x00452C60
CAurRoom* Scene::GetRoom(int i)
{
    if (i >= 0 && i < m_lRooms.num)
        return m_lRooms.element[i];
    return 0;
}

// FUNCTION: SWKOTOR 0x00452C90
void Scene::SetCurrentRoom(int i)
{
    if (i >= 0 && i < m_lRooms.num)
        m_pCurrentRoom = m_lRooms.element[i];
    else
        m_pCurrentRoom = 0;
}

class TileNameTable {
public:
    char m_pad00[0xb8];
    CExoArrayList<char*> m_lNames;          // +0xb8
};

class TileSuperModel {
public:
    char m_pad00[0x84];
    TileNameTable* m_pTable;                // +0x84
};

class TileModel {
public:
    TileNameTable* GetNameTable();          // 0x0043E1C0
    TileSuperModel* GetSuperModel();        // 0x0043E1D0
};

class TileNode {
public:
    char m_pad00[4];
    int m_nNameIndex;                       // +0x04
    TileModel* m_pModel;                    // +0x08

    char* GetNameOutOfLine();               // 0x00438A00
    char* GetName()
    {
        if (m_pModel->GetNameTable())
            return m_pModel->GetNameTable()->m_lNames[m_nNameIndex];
        return m_pModel->GetSuperModel()->m_pTable->m_lNames[m_nNameIndex];
    }
};

class TilePart {
public:
    virtual void V00(); virtual void V04(); virtual void V08(); virtual void V0C();
    virtual void V10(); virtual void V14(); virtual void V18(); virtual void V1C();
    virtual void V20();
    virtual void* GetMesh();                // +0x24
    TileNode* m_pNode;                      // +0x04
};

struct TileParts {
    void* m_pMainLight1;                    // +0x00
    void* m_pMainLight2;                    // +0x04
    TilePart* m_pSecondLight1;              // +0x08
    TilePart* m_pSecondLight2;              // +0x0c
};

// FUNCTION: SWKOTOR 0x004528B0
bool ProcessTilePart(TileParts* pOut, TilePart* pPart, char* szBase)
{
    char szName[0x40];
    strcpy(szName, szBase);
    strcat(szName, "ml1");
    if (!_stricmp(pPart->m_pNode->GetName(), szName)) {
        pOut->m_pMainLight1 = pPart->GetMesh();
        return true;
    }
    strcpy(szName, szBase);
    strcat(szName, "ml2");
    if (!_stricmp(pPart->m_pNode->GetNameOutOfLine(), szName)) {
        pOut->m_pMainLight2 = pPart->GetMesh();
        return true;
    }
    strcpy(szName, szBase);
    strcat(szName, "sl1");
    if (!_stricmp(pPart->m_pNode->GetNameOutOfLine(), szName)) {
        pOut->m_pSecondLight1 = pPart;
        return true;
    }
    strcpy(szName, szBase);
    strcat(szName, "sl2");
    if (!_stricmp(pPart->m_pNode->GetNameOutOfLine(), szName)) {
        pOut->m_pSecondLight2 = pPart;
        return true;
    }
    return false;
}
