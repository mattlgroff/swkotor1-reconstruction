// CScriptEvent (0x004D7270-0x004D79A2). Self-contained declarations.
#ifndef A_SCRIPT_EVENT_H
#define A_SCRIPT_EVENT_H

#include <stddef.h>

class CExoString {
public:
    CExoString();                                   // 0x005B3190
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    int operator!=(const CExoString& s) const;      // 0x005E5310
    char* m_sString;
    unsigned long m_nBufferLength;
};

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
    int WriteFieldWORD(CResStruct* pStruct, unsigned short nValue, char* szLabel);  // 0x004126C0
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
    ~CExoArrayList() { delete[] element; element = NULL; }
    // <int>/<float> fold to 0x005E03C0, <CExoString> is 0x00405080.
    void Allocate(int nSize)
    {
        T* pOld = element;
        array_size = nSize;
        element = new T[nSize];
        for (int i = 0; i < num; i++) {
            element[i] = pOld[i];
        }
        if (pOld) {
            delete[] pOld;
        }
    }
    void SetSize(int nSize)
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

class CScriptEvent {
public:
    CScriptEvent();
    ~CScriptEvent();
    int operator==(const CScriptEvent& e);
    int SaveEvent(CResGFF* pRes, CResStruct* pStruct);
    void CopyScriptEvent(CScriptEvent* pSource);

    unsigned short m_nType;                     // 0x00
    CExoArrayList<int> m_lstIntegers;           // 0x04
    CExoArrayList<float> m_lstFloats;           // 0x10
    CExoArrayList<CExoString> m_lstStrings;     // 0x1c
    CExoArrayList<unsigned long> m_lstObjects;  // 0x28
};

#endif
