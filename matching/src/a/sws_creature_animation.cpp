// CSWSCreature::SetAnimation (nwscreature.cpp).
#include "a/sws_creature.h"

struct CClientOptions {
    char pad_0x0[0x6d];
    unsigned char m_nDifficulty;                    // 0x6d (placeholder name)
};

class CSWCObject {
public:
    int AnimationFireAndForget(unsigned short nAnimation); // 0x0063C380
};

class CSWSObject {
public:
    void SetUpdateFlag(unsigned long nFlag);        // 0x004CC1F0
};

// FUNCTION: SWKOTOR 0x004F0D70
void CSWSCreature::SetAnimation(int nAnimation)
{
    if (nAnimation == 10001 || nAnimation == 10000) {
        switch (m_nAmbientAnimState) {
        case 2: nAnimation = 0x274a; break;
        case 5: nAnimation = 0x27a6; break;
        case 9: nAnimation = 0x28a0; break;
        case 10: nAnimation = 0x28a1; break;
        case 13: nAnimation = 0x28a2; break;
        case 4: nAnimation = 0x2716; break;
        case 3: nAnimation = 0x2718; break;
        case 14: nAnimation = 0x27ac; break;
        case 6: nAnimation = 0x2785; break;
        case 7: nAnimation = 0x278c; break;
        case 8: nAnimation = 0x2786; break;
        case 11: nAnimation = 0x2799; break;
        case 12: nAnimation = 0x279a; break;
        default: nAnimation = 10000 + (m_b4E0 != 0); break;
        }
    } else if (nAnimation == -1) {
        nAnimation = 10000;
    }
    unsigned long oidSelf = m_idSelf;
    if (GetServerExoApp()->IsInjured(oidSelf)) {
        if (GetClientExoApp()->GetClientOptions()->m_nDifficulty != 5) goto Injured;
        if (GetClientExoApp()->GetSWParty()->GetCharacter(0)) {
            CClientExoApp* pClient = GetClientExoApp();
            unsigned long oidCreature = m_idSelf;
            unsigned long oidLeader = pClient->GetSWParty()->GetCharacter(0)->m_idSelf;
            if (pClient->ServerToClientObjectId(oidCreature) != oidLeader) goto Injured;
        }
    }
    if ((nAnimation == 0x2712 || nAnimation == 0x2714) && (m_n9FC & 1)) nAnimation = 0x2795;
    goto Done;
Injured:
    if (nAnimation == 0x2712) nAnimation = 0x276d;
    else if (nAnimation == 0x2714) nAnimation = 0x276e;
Done:
    if ((GetDead() || GetDeadTemp()) && nAnimation != 0x2716 && nAnimation != 0x2718 && nAnimation != 0x27ac)
        return;
    CSWCCreature* pClientCreature;
    if (!GetClientObject() ||
        !(pClientCreature = reinterpret_cast<CGameObjectClient*>(GetClientObject())->AsSWCCreature()) ||
        !reinterpret_cast<CSWCObject*>(pClientCreature)->AnimationFireAndForget((unsigned short)nAnimation) ||
        m_nD4 != (unsigned long)nAnimation)
        reinterpret_cast<CSWSObject*>(this)->SetUpdateFlag(0);
    m_nD4 = nAnimation;
}
