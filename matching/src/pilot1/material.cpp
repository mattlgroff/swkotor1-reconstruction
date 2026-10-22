// Pilot group 1: Material.

void* LoadResourceFile(char* szName, char* szExt, int* pSize, int bFlag);   // 0x0044C740
char* GetResourceData(int nOffset, void* pRes);                             // 0x0044BD10
char* GetNextResourceLine();                                                // 0x0044BFA0
void FreeResourceFile(void* pRes, int nSize);                               // 0x0044C8F0

class MaterialTexture {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual char IsTxiLoaded();                  // 0x34
    virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual int GetTxiSize();                    // 0x90
    virtual char* GetTxiData();                  // 0x94
    virtual void v38(); virtual void v39();
    virtual void SetTxiData(char* pData, int nSize);  // 0xa0
    virtual void SetTxiLoaded(int bLoaded);      // 0xa4
};

class Material {
public:
    MaterialTexture* m_pTexture;    // 0x0
    void ParseField(char* szLine);  // 0x0047ABD0
    void Init(char* szName);
};

// FUNCTION: SWKOTOR 0x0047B110
void Material::Init(char* szName)
{
    int nSize = 0;
    char szLine[0x2000];

    if (m_pTexture && m_pTexture->IsTxiLoaded()) {
        char* pData = m_pTexture->GetTxiData();
        int nLeft = m_pTexture->GetTxiSize();
        if (pData && nLeft > 0) {
            do {
                int n = 0;
                do {
                    szLine[n] = *pData;
                    n++;
                    pData++;
                } while (n < nLeft && szLine[n - 1] != '\n');
                nLeft -= n;
                char* p;
                if (nLeft < 0) {
                    p = 0;
                }
                else {
                    szLine[n] = 0;
                    p = szLine;
                    while (*p == ' ' || *p == '\t') {
                        p++;
                    }
                    ParseField(p);
                }
                if (!nLeft || !p) {
                    return;
                }
            } while (nLeft > 0);
        }
        return;
    }

    void* pRes = LoadResourceFile(szName, ".txi", &nSize, 1);
    if (!pRes) {
        m_pTexture->SetTxiLoaded(1);
        return;
    }
    int nLen = nSize < 0 ? -1 - nSize : nSize;
    if (nLen > 0) {
        MaterialTexture* pTexture = m_pTexture;
        pTexture->SetTxiData(GetResourceData(0, pRes), nLen);
        char* p = GetNextResourceLine();
        while (p) {
            while (*p == ' ' || *p == '\t') {
                p++;
            }
            ParseField(p);
            p = GetNextResourceLine();
        }
    }
    FreeResourceFile(pRes, nSize);
    if (m_pTexture) {
        m_pTexture->SetTxiLoaded(1);
    }
}
