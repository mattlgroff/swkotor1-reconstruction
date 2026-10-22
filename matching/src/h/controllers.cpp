// controller builders of the ASCII model reader
// (0x00464E40 AddGenericControllerSingle ..).

#define H_ALLOCATE_OUT_OF_LINE
#include "h/texctl.h"

void ParseGenericKey(char* s, AurGenericKey* pKey);     // 0x00463B30

// Controller header (0x10 bytes) appended to the node's controller list.
struct NewController {
    int nType;                  // +0x00
    short nRow;                 // +0x04
    short nRows;                // +0x06
    short nTimeIndex;           // +0x08
    short nDataIndex;           // +0x0a
    unsigned char nColumns;     // +0x0c
    char pad_0xd[3];
};

extern int g_nControllers;      // 0x007FBAE8

// Add with the growth expanded (operator new / delete).
__forceinline void AddFloat(CExoArrayList<float>* pList, float f)
{
    if (pList->num == pList->array_size)
        pList->AllocateInline(pList->array_size ? pList->array_size * 2 : 8);
    pList->element[pList->num++] = f;
}

// NONMATCH (13 insns): the AddFloat temporary goes to nRow's argument slot
// (original: nType's) and the final size++ uses edx (original: ecx).
// FUNCTION: SWKOTOR 0x00464E40
int AddGenericControllerSingle(int nUnused, char* szText,
                               CExoArrayList<NewController>* pControllers,
                               CExoArrayList<float>* pData, int nType, short nRow)
{
    NewController ctl;
    AurGenericKey key;
    ctl.nType = nType;
    ctl.nRow = nRow;
    ctl.nRows = 1;
    ParseGenericKey(szText, &key);
    if (key.nNumValues == 4) {
        static Vector s_vAxis;
        s_vAxis.x = key.fValues[0];
        s_vAxis.y = key.fValues[1];
        s_vAxis.z = key.fValues[2];
        Quaternion q(s_vAxis, key.fValues[3]);
        key.fValues[0] = q.x;
        key.fValues[1] = q.y;
        key.fValues[2] = q.z;
        key.fValues[3] = q.w;
    }
    for (int i = key.nNumValues - 1; i >= 0; i--)
        key.fValues[i + 1] = key.fValues[i];
    key.nNumValues++;
    key.fValues[0] = 0.0f;
    switch (key.nNumValues - 1) {
    case 1:
        ctl.nColumns = 1;
        break;
    case 3:
        ctl.nColumns = 3;
        break;
    case 4:
        ctl.nColumns = 4;
        break;
    }
    ctl.nTimeIndex = (short)pData->num;
    AddFloat(pData, 0.0f);
    ctl.nDataIndex = (short)pData->num;
    for (int j = 1; j < key.nNumValues; j++)
        AddFloat(pData, key.fValues[j]);
    if (pControllers->num == pControllers->array_size)
        pControllers->Allocate(pControllers->array_size ? pControllers->array_size * 2 : 8);
    pControllers->element[pControllers->num] = ctl;
    pControllers->num++;
    return ++g_nControllers;
}

void ParseList(char* s, CExoArrayList<AurGenericKey>* pList);  // 0x00464440
void CullUnnecessaryKeys(CExoArrayList<AurGenericKey>* pList);  // 0x00463F80
void CompressQuaternionKey(CExoArrayList<AurGenericKey>* pList);    // 0x00464B50

class LoadingModel {
public:
    char pad_0x0[0x50];
    unsigned short m_nFlags;            // +0x50

    int NoCompress() { return (m_nFlags >> 9) & 1; }
};

extern LoadingModel* g_pLoadingModel;   // 0x007FBAE4
extern int g_bCullKeys;                 // 0x0078E590
extern int g_bCompressQuaternions;      // 0x0078E588

// NONMATCH (99 insns): first draft. The original keeps the zero in ebp,
// copies the time value through an integer register, tests the model flag
// as `movzx; shr 9; test cl, 1` (ours `test byte [+0x51], 2`) and counts
// both key loops down.
// FUNCTION: SWKOTOR 0x00465090
void AddGenericController(int nUnused, char* szText,
                          CExoArrayList<NewController>* pControllers,
                          CExoArrayList<float>* pData, int nType, short nRow)
{
    NewController ctl;
    CExoArrayList<AurGenericKey> lKeys;
    ctl.nType = nType;
    ctl.nRow = nRow;
    ParseList(szText, &lKeys);
    ctl.nRows = (short)lKeys.num;
    if (!lKeys.num)
        return;
    switch (lKeys.element[0].nNumValues - 1) {
    case 1:
        ctl.nColumns = 1;
        break;
    case 3:
        ctl.nColumns = 3;
        break;
    case 4:
        ctl.nColumns = 4;
        if (g_bCullKeys) {
            CullUnnecessaryKeys(&lKeys);
            ctl.nRows = (short)lKeys.num;
        }
        if (g_bCompressQuaternions && !g_pLoadingModel->NoCompress()) {
            CompressQuaternionKey(&lKeys);
            ctl.nColumns = 2;
        }
        break;
    }
    ctl.nTimeIndex = (short)pData->num;
    int i;
    for (i = 0; i < lKeys.num; i++)
        pData->AddInline(lKeys.element[i].fValues[0]);
    ctl.nDataIndex = (short)pData->num;
    for (i = 0; i < lKeys.num; i++)
        for (int j = 1; j < lKeys.element[i].nNumValues; j++)
            AddFloat(pData, lKeys.element[i].fValues[j]);
    g_nControllers += lKeys.num;
    pControllers->AddInline(ctl);
}
