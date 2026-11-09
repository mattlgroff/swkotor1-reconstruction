// Player-to-server level-up message.
#include "b/sws_message.h"
#include "b/sws_creature.h"
#include "b/sws_player.h"
#include "b/sw_rules.h"

// Class entry of the rules (0x1a0 bytes).
class CSWClass {
public:
    char pad_0x0[0x54];
    unsigned char m_nHitDie;                                // 0x54 (name guessed)
    unsigned char m_nForceDie;                              // 0x55 (name guessed)
    char pad_0x56[0x1a0 - 0x56];
};

// One level's choices (0x30 bytes).
class CSWLevelStats {
public:
    CSWLevelStats();                                        // 0x005CB1B0
    ~CSWLevelStats();                                       // 0x005CB270
    void SetSkillRank(unsigned short nSkill, unsigned char nRank); // 0x005CB120
    void AddFeat(unsigned short nFeat);                     // 0x005CB150

    CExoArrayList<unsigned long> m_aSpellsAdded;            // 0x00 (name guessed)
    CExoArrayList<unsigned long> m_aSpellsRemoved;          // 0x0c (name guessed)
    char pad_0x18[0x28 - 0x18];
    unsigned short m_nSkillPoints;                          // 0x28
    unsigned char m_nAbilityGain;                           // 0x2a
    unsigned char m_nHitDie;                                // 0x2b
    unsigned char m_nClass;                                 // 0x2c
    unsigned char m_nForceDie;                              // 0x2d
};

// FUNCTION: SWKOTOR 0x00527560
int CSWSMessage::HandlePlayerToServerLevelUpMessage(CSWSPlayer* pPlayer, unsigned char nMinor)
{
    int nResult = 0;
    if (MessageReadOverflow(1)) {
        return 0;
    }
    if (nMinor == 0) {
        CSWSCreature* pCreature = pPlayer->GetGameObject()->AsSWSCreature();
        if (!pCreature) {
            return 0;
        }
        CSWLevelStats* pLevelStats = new CSWLevelStats;
        pLevelStats->m_nClass = ReadBYTE(8);
        if ((pCreature->GetStats()->GetLevel(0) + 1) % 4 == 0) {
            pLevelStats->m_nAbilityGain = ReadBYTE(8);
        }
        CSWClass* pClass = &g_pRules->m_pClasses[pLevelStats->m_nClass];
        pLevelStats->m_nHitDie = pClass->m_nHitDie;
        pLevelStats->m_nForceDie = pClass->m_nForceDie;
        unsigned long nSkillMask = ReadDWORD(0x20);
        unsigned char i;
        for (i = 0; i < g_pRules->m_nNumSkills; i++) {
            if (nSkillMask & (1 << i)) {
                pLevelStats->SetSkillRank(i, ReadBYTE(8));
            }
        }
        pLevelStats->m_nSkillPoints = ReadWORD(0x10);
        unsigned char nCount = ReadBYTE(8);
        for (; nCount != 0; nCount--) {
            pLevelStats->AddFeat(ReadWORD(0x10));
        }
        nCount = ReadBYTE(8);
        for (; nCount != 0; nCount--) {
            pLevelStats->m_aSpellsAdded.Add(ReadDWORD(0x20));
        }
        nCount = ReadBYTE(8);
        for (; nCount != 0; nCount--) {
            pLevelStats->m_aSpellsRemoved.Add(ReadDWORD(0x20));
        }
        if (MessageReadOverflow(1) || MessageReadUnderflow(1)) {
            return 0;
        }
        int bValid = pCreature->GetStats()->ValidateLevelUp(pLevelStats);
        if (!bValid) {
            delete pLevelStats;
        }
        nResult = SendServerToPlayerLevelUp_Confirmation(pPlayer->m_nPlayerID, bValid);
        if (pCreature->GetStats()->CanLevelUp()) {
            pCreature->SendFeedbackMessage(0xb, NULL);
        }
    }
    return nResult;
}
