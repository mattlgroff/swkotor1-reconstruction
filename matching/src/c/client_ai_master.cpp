// CClientAIMaster (b005f3_client_internal_lists).

class CClientObject;

class CClientAIList {
public:
    ~CClientAIList();                               // 0x00604A40
    int AddObject(CClientObject* pObject);          // 0x006047A0
    int RemoveObject(CClientObject* pObject);       // 0x00604810
    char pad_0x0[0x10];
};

class CClientAIMaster {
public:
    ~CClientAIMaster();
    int AddObject(CClientObject* pObject);
    int RemoveObject(CClientObject* pObject);

    int m_nUnknown00;                               // 0x00
    CClientAIList m_lstActions;                     // 0x04
};

// FUNCTION: SWKOTOR 0x00604B40
CClientAIMaster::~CClientAIMaster()
{
}

// FUNCTION: SWKOTOR 0x00604A20
int CClientAIMaster::AddObject(CClientObject* pObject)
{
    return m_lstActions.AddObject(pObject);
}

// FUNCTION: SWKOTOR 0x00604A30
int CClientAIMaster::RemoveObject(CClientObject* pObject)
{
    return m_lstActions.RemoveObject(pObject);
}
