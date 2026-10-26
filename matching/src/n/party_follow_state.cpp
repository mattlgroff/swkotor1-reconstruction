// CSWParty follow-point state. The fixed member stride, follow-state offset,
// vector/index members, and follow-path pointer are confirmed in the binary.
struct NPartyVector {
    float x;
    float y;
    float z;
};

class CSWFollowPath {
public:
    // Inferred name; the binary confirms thiscall, four stack args, RET 10h.
    int GetFollowPoint(float fDistance, NPartyVector* pPoint,
                       unsigned long* pPointIndex, int nSide);
};

class CSWPartyMember {
public:
    unsigned long m_oid;
    unsigned long m_nFollowState;                    // record +0x04
    char pad_0x8[0x20];
    NPartyVector m_vFollowPoint;                     // record +0x28
    unsigned long m_nFollowPointIndex;               // record +0x34
    char pad_0x38[0x50];
};

class CSWParty {
public:
    int m_nMemberCount;
    char pad_0x4[0x20];
    CSWPartyMember m_aMembers[3];                    // +0x24, stride 0x88
    char pad_0x1bc[4];
    float m_fFollowDistance;                         // +0x1C0
    char pad_0x1c4[0x118];
    CSWFollowPath m_FollowPath;                       // +0x2DC, embedded object

    // Inferred name; thiscall float result is returned through x87 ST(0).
    float GetFollowDistance();                       // 0x00634A70
    void RecalculateFollowPoint();
    void FollowStateChanged();
};

// FUNCTION: SWKOTOR 0x006359C0
void CSWParty::RecalculateFollowPoint()
{
    NPartyVector point;
    unsigned long nPointIndex;
    point.x = 0.0f;
    point.y = 0.0f;
    point.z = 0.0f;
    if (m_FollowPath.GetFollowPoint(GetFollowDistance() - m_fFollowDistance,
                                    &point, &nPointIndex, 1)) {
        m_aMembers[1].m_vFollowPoint = point;
        m_aMembers[1].m_nFollowPointIndex = nPointIndex;
    }
    if (m_FollowPath.GetFollowPoint(GetFollowDistance() + m_fFollowDistance,
                                    &point, &nPointIndex, 2)) {
        m_aMembers[2].m_vFollowPoint = point;
        m_aMembers[2].m_nFollowPointIndex = nPointIndex;
    }
}

// FUNCTION: SWKOTOR 0x006369D0
void CSWParty::FollowStateChanged()
{
    RecalculateFollowPoint();
    for (int nMember = 0; nMember < 3; ++nMember) {
        switch (m_aMembers[nMember].m_nFollowState) {
        case 1:
            m_aMembers[nMember].m_nFollowState = 6;
            break;
        case 2:
            m_aMembers[nMember].m_nFollowState = 9;
            break;
        case 3:
            m_aMembers[nMember].m_nFollowState = 7;
            break;
        case 4:
            m_aMembers[nMember].m_nFollowState = 8;
            break;
        default:
            break;
        }
    }
}
