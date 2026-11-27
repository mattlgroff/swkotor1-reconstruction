// sw_party_client.h -- CSWPartyClient
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (src/c/sw_c_creature_state_family.cpp); the other copies' members fall on its members.
#ifndef SHARED_SW_PARTY_CLIENT_H
#define SHARED_SW_PARTY_CLIENT_H


class CSWPartyClient {
public:
    int SetPlayerCreature(unsigned long oid);       // 0x006364C0 (returns int: mov eax, ebp)
    int RemovePartyMember(unsigned long oid); // 0x006349f0, descriptive callee name.
    void SetPC(int nIndex);
};

#endif
