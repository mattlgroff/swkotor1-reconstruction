// CExoIniInternal::ReadIniEntry, worker C s8 (exoiniinternal.o;
// translation-unit-local layout).

#include <stdio.h>
#include <string.h>

#include "shared/exo_string.h"


#include "shared/exo_linked_list.h"

#include "shared/exo_linked_list.h"



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
