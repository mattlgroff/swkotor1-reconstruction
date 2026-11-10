// CExoIniInternal::ReadIniEntry, worker C s8 (exoiniinternal.o;
// translation-unit-local layout).

#include <stdio.h>
#include <string.h>

class CExoString {
public:
    char* m_sString;                                // 0x00
    unsigned long m_nBufferLength;                  // 0x04

    CExoString();                                   // 0x005B3190
    CExoString(const char* s);                      // 0x005E5A90
    ~CExoString();                                  // 0x005E5C20
    CExoString& operator=(const char* s);           // 0x005E5140
    CExoString& operator=(const CExoString& s);     // 0x005E5C50
    int operator==(const char* s) const;            // 0x005E5290
    char* CStr() const;                             // 0x005E5670
    int GetLength() const;                          // 0x005E5790
    int CompareNoCase(const char* s) const;         // 0x005E58C0
    CExoString Left(int nCount) const;              // 0x005E5F70
    CExoString Right(int nCount) const;             // 0x005E6170
    CExoString operator+(const CExoString& s) const;    // 0x005E5D10
    void __cdecl Format(const char* sFormat, ...);  // 0x005E5680
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
    CExoLinkedListPosition AddHead(void* p);        // 0x005E9D70
    void* GetHead()
    {
        CExoLinkedListNode* pNode = (CExoLinkedListNode*)pHead;
        if (!pNode) {
            return 0;
        }
        return pNode->pObject;
    }
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedListInternal* m_pcExoLinkedListInternal;   // 0x00

    CExoLinkedListPosition GetHeadPos() { return m_pcExoLinkedListInternal->pHead; }
    T* GetHead()
    {
        CExoLinkedListInternal* pInternal = m_pcExoLinkedListInternal;
        return (T*)pInternal->GetHead();
    }
    T* GetNext(CExoLinkedListPosition& pos) { return (T*)m_pcExoLinkedListInternal->GetNext(pos); }
    CExoLinkedListPosition AddTail(T* p) { return m_pcExoLinkedListInternal->AddTail(p); }
    CExoLinkedListPosition AddHead(T* p) { return m_pcExoLinkedListInternal->AddHead(p); }
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
    int WriteIniEntry(const CExoString& sValue, const CExoString& sFile, const CExoString& sSection,
                      const CExoString& sKey);
};

// NONMATCH (11 diff insns): the original keeps the raw `new CExoString`
// pointer in esi instead of the constructor's return, plus one lea/inc swap.
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
    CFileSection* pSection = m_lstSections.GetHead();
    CExoLinkedListPosition pos = m_lstSections.GetHeadPos();
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
            CExoString* pKey = pSection->m_lstKeys.GetHead();
            CExoLinkedListPosition posKey = pSection->m_lstKeys.GetHeadPos();
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

// FUNCTION: SWKOTOR 0x005EAB00
// NONMATCH (169 diff insns): same statements and EH states; the frame is one
// slot short (the original gives the error string its own slot next to the
// `new` temporaries) and `this`/section/key registers rotate (ebp/edi/ebx).
int CExoIniInternal::WriteIniEntry(const CExoString& sValue, const CExoString& sFile,
                                   const CExoString& sSection, const CExoString& sKey)
{
    char szLine[0x200];
    char szFile[0x60];
    char szSection[0x60];
    char szKey[0x60];
    char szValue[0x200];
    sprintf(szFile, "%s", sFile.CStr());
    sprintf(szSection, "%s", sSection.CStr());
    sprintf(szKey, "%s", sKey.CStr());
    sprintf(szValue, "%s", sValue.CStr());
    if (!m_sFileName.CompareNoCase(szFile)) {
        Destroy();
    }
    if (!m_lstSections.GetHead()) {
        m_sFileName = sFile;
        FILE* fp = fopen(szFile, "r");
        if (!fp) {
            fp = fopen(szFile, "w");
            if (!fp) {
                return 0;
            }
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
    CFileSection* pSection = m_lstSections.GetHead();
    CExoLinkedListPosition pos = m_lstSections.GetHeadPos();
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
            break;
        }
        pSection = m_lstSections.GetNext(pos);
    }
    if (pSection) {
        CExoString* pKey = pSection->m_lstKeys.GetHead();
        CExoLinkedListPosition posKey = pSection->m_lstKeys.GetHeadPos();
        char* pEquals;
        while (pKey) {
            strcpy(szLine, pKey->CStr());
            pEquals = strchr(szLine, '=');
            if (pEquals) {
                *pEquals = 0;
            }
            if (!strcmp(szLine, szKey)) {
                break;
            }
            pKey = pSection->m_lstKeys.GetNext(posKey);
        }
        if (pKey) {
            if (pEquals) {
                strcpy(pEquals + 1, szValue);
                *pEquals = '=';
            } else {
                strcat(szLine, "=");
                strcat(szLine, szValue);
            }
            *pKey = szLine;
            *pKey = *pKey + "\n";
        } else {
            CExoString* pNew = new CExoString;
            *pNew = *pNew + szKey;
            *pNew = *pNew + "=";
            *pNew = *pNew + szValue;
            *pNew = *pNew + "\n";
            pSection->m_lstKeys.AddHead(pNew);
        }
    } else {
        CFileSection* pNew = new CFileSection;
        pNew->m_sName = "\n[";
        pNew->m_sName = pNew->m_sName + szSection;
        pNew->m_sName = pNew->m_sName + "]\n";
        CExoString* pKey = new CExoString;
        *pKey = szKey;
        *pKey = *pKey + "=";
        *pKey = *pKey + szValue;
        *pKey = *pKey + "\n";
        pNew->m_lstKeys.AddHead(pKey);
        m_lstSections.AddTail(pNew);
    }
    FILE* fp = fopen(szFile, "w");
    if (!fp) {
        CExoString sError;
        sError.Format("ERROR: %s file is marked read only! Are you sure that you are running from the "
                      "install location and not from the network?", szFile);
        sError.CStr();
        return 0;
    }
    pSection = m_lstSections.GetHead();
    pos = m_lstSections.GetHeadPos();
    while (pSection) {
        fprintf(fp, "%s", pSection->m_sName.CStr());
        CExoString* pKey = pSection->m_lstKeys.GetHead();
        CExoLinkedListPosition posKey = pSection->m_lstKeys.GetHeadPos();
        while (pKey) {
            fprintf(fp, "%s", pKey->CStr());
            pKey = pSection->m_lstKeys.GetNext(posKey);
        }
        pSection = m_lstSections.GetNext(pos);
    }
    fclose(fp);
    return 1;
}
