// CSWSSysAdmin lifetime (own TU so the CSWSClient base ctor / dtor stay calls).

class CSWSClient {
public:
    CSWSClient(unsigned long nPlayerId);
    virtual ~CSWSClient();

    unsigned long m_nPlayerId;       // +0x04
    int m_n08;                       // +0x08
};

class CSWSSysAdmin : public CSWSClient {
public:
    CSWSSysAdmin(unsigned long nPlayerId);
    virtual ~CSWSSysAdmin();
};

// FUNCTION: SWKOTOR 0x005802F0
CSWSSysAdmin::CSWSSysAdmin(unsigned long nPlayerId) : CSWSClient(nPlayerId)
{
}

// FUNCTION: SWKOTOR 0x00580310
CSWSSysAdmin::~CSWSSysAdmin()
{
}

// FUNCTION: SWKOTOR 0x00580320 SYMBOL: ??_GCSWSSysAdmin@@UAEPAXI@Z
