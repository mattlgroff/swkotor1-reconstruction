// CExoPackedFile virtual resource-file accessor.

class CExoPackedFile {
public:
    virtual ~CExoPackedFile();
    virtual void AddRefs();
    virtual void AddAsyncRefs();
    virtual int CloseFile();
    virtual int CloseAsyncFile();
    virtual void DelRefs();
    virtual void DelAsyncRefs();
    virtual void* GetFile();
    virtual void* GetAsyncFile();                    // vtable slot +0x20

    char m_pad_04[0x14];
    void* m_pAsyncFile;                              // +0x18
};

// FUNCTION: SWKOTOR 0x00644760
void* CExoPackedFile::GetAsyncFile()
{
    return m_pAsyncFile;
}
