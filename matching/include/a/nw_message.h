// CNWMessage / CSWMessage stream helpers and CSWCCMessageData
// (0x004D60F0-0x004D7780). Self-contained declarations.
#ifndef A_NW_MESSAGE_H
#define A_NW_MESSAGE_H

#include <stddef.h>
#include <string.h>

#include "shared/exo_string.h"

class CResRef {
public:
    CResRef(const char* s);                         // 0x00406D80
    CResRef(const CResRef& r);                      // 0x00406DA0
    char* GetResRef();                              // 0x00641DB0 (returns this)
    char m_resRef[16];
};

#include "shared/vector.h"

struct CResStruct {
    unsigned long m_nStructIndex;
};

struct CResList {
    unsigned long m_nStructIndex;
    char m_pLabel[16];
};

class CResGFF {
public:
    int AddList(CResList* pList, CResStruct* pStruct, char* szLabel);               // 0x00412450
    int AddListElement(CResStruct* pStruct, CResList* pList, unsigned long nType);  // 0x004124E0
    int WriteFieldBYTE(CResStruct* pStruct, unsigned char nValue, char* szLabel);   // 0x00412620
    int WriteFieldDWORD(CResStruct* pStruct, unsigned long nValue, char* szLabel);  // 0x00412760
    int WriteFieldINT(CResStruct* pStruct, int nValue, char* szLabel);              // 0x004127B0
    int WriteFieldFLOAT(CResStruct* pStruct, float fValue, char* szLabel);          // 0x00412870
    int WriteFieldCExoString(CResStruct* pStruct, const CExoString& sValue, char* szLabel); // 0x00412970
};

template <class T> class CExoArrayList {
public:
    CExoArrayList(int nSize = 0)
    {
        num = 0;
        array_size = nSize;
        element = NULL;
        if (array_size) {
            element = new T[array_size];
        }
    }
    ~CExoArrayList() { delete[] element; element = 0; }
    void Allocate(int n);                           // <unsigned long> 0x005E03C0
    void SetSize(int nSize)                         // <CExoString> 0x00405950
    {
        if (nSize == 0) {
            if (element) {
                delete[] element;
                element = NULL;
                array_size = 0;
            }
        }
        else {
            Allocate(nSize);
        }
        num = nSize;
    }
    T* element;
    int num;
    int array_size;
};

#include "shared/globals.h"

// vtable 0x007465A4
#include "shared/nw_message.h"

#include "shared/sw_message.h"

class CSWCCMessageData {
public:
    CSWCCMessageData();
    ~CSWCCMessageData();
    int GetInteger(int nIndex);
    float GetFloat(int nIndex);
    unsigned long GetObjectID(int nIndex);
    CExoString GetString(int nIndex);
    void SetInteger(int nIndex, int nValue);
    void SetFloat(int nIndex, float fValue);
    void SetObjectID(int nIndex, unsigned long oidValue);
    void SetString(int nIndex, const CExoString& sValue);
    int SaveData(CResGFF* pRes, CResStruct* pStruct);

    unsigned char m_nType;                      // 0x00
    CExoArrayList<int> m_lstIntegers;           // 0x04
    CExoArrayList<float> m_lstFloats;           // 0x10
    CExoArrayList<CExoString> m_lstStrings;     // 0x1c
    CExoArrayList<unsigned long> m_lstObjects;  // 0x28
};

#endif
