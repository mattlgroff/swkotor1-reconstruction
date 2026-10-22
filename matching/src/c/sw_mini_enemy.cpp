// CSWMiniEnemy (b0062 message leaves).

class CResStruct;

class CResGFF {
public:
    unsigned char ReadFieldBYTE(CResStruct* pStruct, char* szField, int& bSuccess, unsigned char nDefault);   // 0x00411A60
};

class CSWMiniObject {
public:
    int Load(CResGFF* pGFF, CResStruct* pStruct);   // 0x0066FFF0
};

class CSWMiniEnemy : public CSWMiniObject {
public:
    int Load(CResGFF* pGFF, CResStruct* pStruct);

    char pad_0x0[0x1a0];
    unsigned char m_nEnemyField;                    // 0x1a0
};

// FUNCTION: SWKOTOR 0x006705F0
int CSWMiniEnemy::Load(CResGFF* pGFF, CResStruct* pStruct)
{
    if (!CSWMiniObject::Load(pGFF, pStruct)) {
        return 0;
    }
    int bSuccess;
    m_nEnemyField = pGFF->ReadFieldBYTE(pStruct, "Field", bSuccess, 0);
    return 1;
}
