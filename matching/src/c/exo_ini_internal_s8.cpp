// CExoIniInternal::ReadIniEntry, worker C s8 (exoiniinternal.o;
// translation-unit-local layout).

#include <stdio.h>
#include <string.h>

class CExoString {
public:
    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04

    CExoString();                                   // 0x005B3190
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const char* s);           // 0x005E5140
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    int operator==(const char* s) const;            // 0x005E5290
    char* CStr() const;                             // 0x005E5670
    int GetLength() const;                          // 0x005E5790
    int CompareNoCase(const char* s) const;         // 0x005E58C0
    CExoString Left(int nCount) const;              // 0x005E5F70
    CExoString Right(int nCount) const;             // 0x005E6170
};

typedef void* CExoLinkedListPosition;

struct CExoLinkedListNode {
    CExoLinkedListNode* pPrev;                      // 0x00
    CExoLinkedListNode* pNext;                      // 0x04
    void* pObject;                                  // 0x08
};

class CExoLinkedListInternal {
public:
    CExoLinkedListNode* pHead;                      // 0x00
    CExoLinkedListNode* pTail;                      // 0x04
    unsigned long nCount;                           // 0x08

    void* GetNext(CExoLinkedListPosition& pos);     // 0x005E9B90
    CExoLinkedListPosition AddTail(void* p);        // 0x005E9DC0
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedListInternal* m_pcExoLinkedListInternal;   // 0x00

    CExoLinkedListPosition GetHeadPos() { return m_pcExoLinkedListInternal->pHead; }
    T* GetHead()
    {
        void* p = m_pcExoLinkedListInternal->pHead;
        if (p) {
            p = ((CExoLinkedListNode*)p)->pObject;
        }
        return (T*)p;
    }
    T* GetNext(CExoLinkedListPosition& pos) { return (T*)m_pcExoLinkedListInternal->GetNext(pos); }
    CExoLinkedListPosition AddTail(T* p) { return m_pcExoLinkedListInternal->AddTail(p); }
};

class CFileSection {
public:
    CExoString m_sName;                             // 0x00
    CExoLinkedList<CExoString> m_lstKeys;           // 0x08

    CFileSection();                                 // 0x005EA600
};

class CExoIniInternal {
public:
    CExoLinkedList<CFileSection> m_lstSections;     // 0x00
    CExoString m_sFileName;                         // 0x04

    void Destroy();                                 // 0x005EA650
    int ReadIniEntry(CExoString& sValue, const CExoString& sFile, const CExoString& sSection,
                     const CExoString& sKey);
};

// NONMATCH (41 diff insns): register allocation only (same size and
// layout). The original reloads the list head for the iteration position
// after testing GetHead() instead of sharing the load, and sets -1 with
// `or edx, -1` in the fseek offset.
// FUNCTION: SWKOTOR 0x005EA6C0
int CExoIniInternal::ReadIniEntry(CExoString& sValue, const CExoString& sFile, const CExoString& sSection,
                                  const CExoString& sKey)
{
    char szLine[0x200];
    char szFile[0x60];
    char szSection[0x60];
    char szKey[0x60];
    sprintf(szFile, "%s", sFile.CStr());
    sprintf(szSection, "%s", sSection.CStr());
    sprintf(szKey, "%s", sKey.CStr());
    if (!m_sFileName.CompareNoCase(szFile)) {
        Destroy();
    }
    if (!m_lstSections.GetHead()) {
        m_sFileName = sFile;
        FILE* fp = fopen(szFile, "r");
        if (!fp) {
            return 0;
        }
        while (fgets(szLine, 0x200, fp)) {
            CFileSection* pSection = new CFileSection;
            pSection->m_sName = szLine;
            m_lstSections.AddTail(pSection);
            if (szLine[0] == '[') {
                while (fgets(szLine, 0x200, fp)) {
                    if (szLine[0] == '[') {
                        fseek(fp, -(long)(strlen(szLine) + 1), SEEK_CUR);
                        break;
                    }
                    CExoString* pKey = new CExoString;
                    *pKey = szLine;
                    pSection->m_lstKeys.AddTail(pKey);
                }
            }
        }
        fclose(fp);
    }
    CExoLinkedListPosition pos = m_lstSections.GetHeadPos();
    CFileSection* pSection = m_lstSections.GetHead();
    while (pSection) {
        strcpy(szLine, pSection->m_sName.CStr());
        char* p = strchr(szLine, ']');
        if (p) {
            *p = 0;
        }
        p = strchr(szLine, '[');
        if (!p) {
            p = szLine;
        }
        if (!strcmp(p + 1, szSection)) {
            CExoLinkedListPosition posKey = pSection->m_lstKeys.GetHeadPos();
            CExoString* pKey = pSection->m_lstKeys.GetHead();
            while (pKey) {
                strcpy(szLine, pKey->CStr());
                p = strchr(szLine, '=');
                if (p) {
                    *p = 0;
                }
                if (!strcmp(szLine, szKey)) {
                    sValue = strchr(pKey->CStr(), '=') + 1;
                    if (sValue.Right(1) == "\n") {
                        sValue = sValue.Left(sValue.GetLength() - 1);
                    }
                    return 1;
                }
                pKey = pSection->m_lstKeys.GetNext(posKey);
            }
            return 0;
        }
        pSection = m_lstSections.GetNext(pos);
    }
    return 0;
}
