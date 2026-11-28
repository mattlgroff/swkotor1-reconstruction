// Worker J matching headers: CERFFile writer / reader (0x005DCFC0-0x005DE44B)
// and the CExoFile / CExoLinkedList callees it uses.
#ifndef J_ERF_H
#define J_ERF_H

#include <string.h>
#include "j/exo_string.h"

class CExoFile {
public:
    CExoFile(const CExoString& sFileName, unsigned short nResType, const CExoString& sMode); // 0x005E68D0
    ~CExoFile();                                    // 0x005E69E0
    unsigned long GetOffset();                      // 0x005E6940
    unsigned long GetSize();                        // 0x005E6950
    unsigned long Read(void* pBuffer, unsigned long nSize, unsigned long nCount);   // 0x005E6960
    unsigned long Read(CExoString& sString, unsigned long nSize);                   // 0x005E6970
    unsigned long Write(const void* pBuffer, unsigned long nSize, unsigned long nCount); // 0x005E69A0
    int IsOpen();                                   // 0x005E6A10
    int Flush();                                    // 0x005E6A20
    int Seek(long nOffset, int nFrom);              // 0x005E6AA0
    int SeekEnd();                                  // 0x005E6AF0

    void* m_pFileInternal;
};


#include "shared/exo_linked_list.h"




template <class T>
class CExoArrayList {
public:
    CExoArrayList()
    {
        num = 0;
        array_size = 0;
        element = 0;
    }
    ~CExoArrayList()
    {
        if (element) {
            delete[] element;
        }
        element = 0;
    }
    T& operator[](int i) { return element[i]; }

    T* element;                                     // 0x00
    int num;                                        // 0x04
    int array_size;                                 // 0x08
};

class CExoBase {
public:
    void GetDirectoryList(CExoArrayList<CExoString>* pList, const CExoString& sDirectory,
                          unsigned short nResType, int bSort, int bFullPath);   // 0x005E6640
    unsigned short GetResTypeFromExtension(const CExoString& sExtension);   // 0x005E6670
};


typedef void (*ERFProgressCallback)(int nCurrent, int nTotal, void* pParam1, void* pParam2);

// Raw resource handed to AddResource (CRes accessors, not inlined).
class CERFRawResource {
public:
    void* GetData();                                // 0x004064A0 (+0x10)
    unsigned long GetSize();                        // 0x00644760 (+0x18)
};

class CResGFF {
public:
    void Pack(int a, int b);                        // 0x00412DB0
    unsigned long GetTotalSize();                   // 0x00411540
    int WriteGFFData(CExoFile* pFile, unsigned long& nSize);   // 0x004113D0
};

#include "shared/app_manager.h"

#include "shared/globals.h"

// Localized string table entry (0x0C bytes).
#include "shared/erf_string.h"

// Key list entry (0x18 bytes).
class CERFKey {
public:
    CERFKey()
    {
        memset(m_cResRef, 0, 16);
        memset(&m_nUnused, 0, 2);
        m_nResID = 0;
        m_nResType = 0xFFFF;
    }
    ~CERFKey();
    int Write(CExoFile* pFile);

    char m_cResRef[16];                             // 0x00
    unsigned long m_nResID;                         // 0x10
    unsigned short m_nResType;                      // 0x14
    unsigned short m_nUnused;                       // 0x16
};

// Resource list entry (0x0C bytes).
class CERFRes {
public:
    CERFRes()
    {
        m_nOffset = 0;
        m_nSize = 0;
        m_pData = 0;
    }
    ~CERFRes()
    {
        if (m_pData) {
            delete[] m_pData;
            m_pData = 0;
        }
        m_nOffset = 0;
        m_nSize = 0;
    }
    int CopyToFile(CExoFile* pSrc, CExoFile* pDst, int nUnused);
    int Write(CExoFile* pFile, unsigned long nEntryOffset);

    unsigned long m_nOffset;                        // 0x00
    unsigned long m_nSize;                          // 0x04
    char* m_pData;                                  // 0x08
};

class CERFFile {
public:
    CERFFile();
    ~CERFFile();
    int Create(CExoString& sFileName);
    int WriteHeader();
    int Finish();
    int Read(CExoString& sFileName);
    int ReadHeader(CExoString& sFileName);          // 0x005DCE50
    int Reset();
    int WriteStringTable();
    int ReserveEntries(unsigned long nEntries);
    int AddFile(CExoString& sFileName, int nUnused);
    int AddResource(const char* szResRef, unsigned short nResType, void* pResource, int bGFF,
                    int nUnused);
    void ExtractAll(CExoString& sFileName, CExoString& sDirectory, int nUnused,
                   ERFProgressCallback pCallback, void* pParam1, void* pParam2);
    void AddDirectory(CExoString& sDirectory, int nUnused, ERFProgressCallback pCallback,
                      void* pParam1, void* pParam2);

    // ERF header, written as one 0xA0-byte block.
    unsigned long m_nFileType;                      // 0x00
    unsigned long m_nVersion;                       // 0x04
    unsigned long m_nLanguageCount;                 // 0x08
    unsigned long m_nLocalizedStringSize;           // 0x0c
    unsigned long m_nEntryCount;                    // 0x10
    unsigned long m_nOffsetToLocalizedString;       // 0x14
    unsigned long m_nOffsetToKeyList;               // 0x18
    unsigned long m_nOffsetToResourceList;          // 0x1c
    unsigned long m_nBuildYear;                     // 0x20
    unsigned long m_nBuildDay;                      // 0x24
    unsigned long m_nDescriptionStrRef;             // 0x28
    char m_pReserved[0x74];                         // 0x2c

    CExoLinkedList<CERFString> m_lStrings;          // 0xa0
    CExoLinkedList<CERFKey> m_lKeys;                // 0xa4
    CExoLinkedList<CERFRes> m_lResources;           // 0xa8
    CExoFile* m_pFile;                              // 0xac
    unsigned long m_nB0;                            // 0xb0
    unsigned long m_nB4;                            // 0xb4
    unsigned long m_nB8;                            // 0xb8
    unsigned long m_nMaxEntries;                    // 0xbc
    unsigned long m_nCurrentEntry;                  // 0xc0
    unsigned long m_nC4;                            // 0xc4
    CExoString m_sC8;                               // 0xc8
};

#endif
