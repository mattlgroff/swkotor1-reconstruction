// CExoResMan::ServiceFromEncapsulated (pilot group 2).

typedef void* CExoLinkedListPosition;

class CExoLinkedListInternal {
public:
    CExoLinkedListPosition pHead;

    void* GetAtPos(CExoLinkedListPosition pos);         // 0x005E9B60
    void* GetNext(CExoLinkedListPosition& pos);         // 0x005E9B90
};

template <class T>
class CExoLinkedList {
public:
    CExoLinkedListInternal* m_pcExoLinkedListInternal;

    CExoLinkedListPosition GetHeadPos() { return m_pcExoLinkedListInternal->pHead; }
    T* GetAtPos(CExoLinkedListPosition pos) { return (T*)m_pcExoLinkedListInternal->GetAtPos(pos); }
    T* GetNext(CExoLinkedListPosition& pos) { return (T*)m_pcExoLinkedListInternal->GetNext(pos); }
};

class CExoEncapsulatedFile {
public:
    virtual void Unknown00();
    virtual int OpenFile();                                         // 0x04
    virtual int OpenAsyncFile();                                    // 0x08
    virtual void Unknown0C();
    virtual void Unknown10();
    virtual int CloseFile();                                        // 0x14
    virtual int CloseAsyncFile();                                   // 0x18
    virtual void Unknown1C();
    virtual void* GetAsyncFile();                                   // 0x20
    virtual unsigned long GetResourceSize(unsigned long nEntry);    // 0x24
    virtual void Unknown28();
    virtual void Unknown2C();
    virtual void Unknown30();
    virtual unsigned long ReadResource(unsigned long nID, void* pData, unsigned long nSize, unsigned long nOffset);      // 0x34
    virtual void ReadResourceAsync(unsigned long nID, void* pData, unsigned long nSize, unsigned long nOffset);         // 0x38
};

class CExoKeyTable {
public:
    char pad_0x00[0x28];
    unsigned long m_nTableID : 28;          // 0x28
    char pad_0x2c[0x4];
    CExoEncapsulatedFile** m_ppFile;        // 0x30
};

class CRes {
public:
    virtual void Unknown00();
    virtual void Unknown04();
    virtual void Unknown08();
    virtual void Unknown0C();
    virtual int OnResourceServiced();       // 0x10

    char pad_0x04[0x4];
    unsigned long m_nID;                    // 0x08
    unsigned long m_nStatus;                // 0x0c
    void* m_pResource;                      // 0x10
    char pad_0x14[0x4];
    unsigned long m_nSize;                  // 0x18
};

class CExoResMan {
public:
    char pad_0x00[0x18];
    CExoLinkedList<CExoKeyTable> m_lstEncapsulatedFiles;   // 0x18
    char pad_0x1c[0x10];
    void* m_pAsyncFile;                         // 0x2c

    int ReadRaw(CRes* pRes);                    // 0x004077B0

    int ServiceFromEncapsulated(CRes* pRes, int bAsync);
};

// BEST ATTEMPT (not matching): everything but block layout matches. The
// original keeps the head==NULL exit on the loop's own return block and
// aligns the loop head (lea esp,[esp]); we emit the exit at the shared
// tail epilogue instead, shifting every later offset.
// FUNCTION: SWKOTOR 0x00407BD0
int CExoResMan::ServiceFromEncapsulated(CRes* pRes, int bAsync)
{
    if (pRes == 0) {
        return 0;
    }
    if (pRes->m_nStatus & 4) {
        return 0;
    }

    CExoLinkedListPosition pos = m_lstEncapsulatedFiles.GetHeadPos();
    if (pos) {
        CExoKeyTable* pTable = m_lstEncapsulatedFiles.GetAtPos(pos);
        while (pos) {
            if (pTable && pTable->m_nTableID == ((pRes->m_nID >> 14) & 0x3f)) {
                if (bAsync) {
                    (*pTable->m_ppFile)->OpenAsyncFile();
                    unsigned long nSize = (*pTable->m_ppFile)->GetResourceSize(pRes->m_nID & 0x3fff);
                    pRes->m_nSize = nSize;
                    if (!ReadRaw(pRes)) {
                        (*pTable->m_ppFile)->CloseAsyncFile();
                        return 0;
                    }
                    m_pAsyncFile = (*pTable->m_ppFile)->GetAsyncFile();
                    (*pTable->m_ppFile)->ReadResourceAsync(pRes->m_nID, pRes->m_pResource, nSize, 0);
                    return 1;
                }
                (*pTable->m_ppFile)->OpenFile();
                unsigned long nSize = (*pTable->m_ppFile)->GetResourceSize(pRes->m_nID & 0x3fff);
                pRes->m_nSize = nSize;
                if (!ReadRaw(pRes) ||
                    (*pTable->m_ppFile)->ReadResource(pRes->m_nID, pRes->m_pResource, nSize, 0) != nSize) {
                    (*pTable->m_ppFile)->CloseFile();
                    return 0;
                }
                (*pTable->m_ppFile)->CloseFile();
                int nResult = pRes->OnResourceServiced();
                pRes->m_nStatus = (pRes->m_nStatus & ~4) | (nResult ? 4 : 0);
                return nResult;
            }
            pTable = m_lstEncapsulatedFiles.GetNext(pos);
        }
    }
    return 0;
}
