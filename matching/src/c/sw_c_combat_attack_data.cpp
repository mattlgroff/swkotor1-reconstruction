// CSWCCombatAttackData initialization and memberwise copying. The copy helper
// name is descriptive: unlike a constructor, its EAX is not the destination.
struct Vector {
    float x, y, z;
    Vector() { x = 0.0f; y = 0.0f; z = 0.0f; }
    Vector(float fx, float fy, float fz) { x = fx; y = fy; z = fz; }
};
class CSWCCombatAttackData {
public:
    unsigned char m_nAttackType, m_nResult;
    char pad_0x2[2];
    unsigned long m_oidAttacker;
    unsigned short m_nAnimation, m_nReaction;
    unsigned char m_nFlags;
    char pad_0xd[3];
    int m_nDamage;
    unsigned long m_oidTarget;
    unsigned short m_n18, m_n1a, m_n1c, m_n1e;
    int m_n20, m_n24;
    unsigned char m_nHand;
    char pad_0x29[3];
    Vector m_vPosition;
    int m_n38, m_n3c, m_n40, m_n44;
    CSWCCombatAttackData();
    void CopyFrom(const CSWCCombatAttackData& source);
};

// FUNCTION: SWKOTOR 0x0060B630
CSWCCombatAttackData::CSWCCombatAttackData()
{
    m_oidAttacker = 0x7f000000;
    m_oidTarget = 0x7f000000;
    m_nAttackType = 0;
    m_nResult = 0;
    m_nAnimation = 0;
    m_nReaction = 0;
    m_nFlags = 0;
    m_n1e = 0;
    m_nDamage = 0;
    m_n18 = 0;
    m_n1a = 0;
    m_n1c = 0;
    m_vPosition = Vector(0.0f, 0.0f, 0.0f);
    m_n38 = -1;
    m_n3c = 0;
    m_n44 = 0;
    m_n20 = 0;
    m_n24 = 0;
    m_nHand = 0;
    m_n40 = 0;
}

// FUNCTION: SWKOTOR 0x0060B6B0
void CSWCCombatAttackData::CopyFrom(const CSWCCombatAttackData& source)
{
    m_nAttackType = source.m_nAttackType;
    m_nResult = source.m_nResult;
    m_oidAttacker = source.m_oidAttacker;
    m_oidTarget = source.m_oidTarget;
    m_nAnimation = source.m_nAnimation;
    m_nReaction = source.m_nReaction;
    m_nFlags = source.m_nFlags;
    m_nDamage = source.m_nDamage;
    m_n1e = source.m_n1e;
    m_n18 = source.m_n18;
    m_n1a = source.m_n1a;
    m_n1c = source.m_n1c;
    m_vPosition = source.m_vPosition;
    m_n38 = source.m_n38;
    m_n3c = source.m_n3c;
    m_n44 = source.m_n44;
    m_n20 = source.m_n20;
    m_n24 = source.m_n24;
    m_nHand = source.m_nHand;
    m_n40 = source.m_n40;
}
