// GUI skill-rank bit updates share the creature statistics layout.
class CSWRules { public: char pad_000[0xab]; unsigned char m_nNumSkills;
 unsigned char GetNumSkills() { return m_nNumSkills; } };
extern CSWRules* g_pRules;
class CSWCCreatureStats { public: char pad_000[0x88]; char* m_pSkillRanks;
 char GetBaseSkillRank(unsigned short skill) { if (skill >= g_pRules->GetNumSkills()) return 0; return m_pSkillRanks[skill]; }
 void SetBaseSkillRank(unsigned short skill, unsigned char rank) { if (skill < g_pRules->GetNumSkills()) m_pSkillRanks[skill] = rank; }
};
class CSWCCreature { public: char pad_000[0x2f8]; CSWCCreatureStats* m_pStats;
 CSWCCreatureStats* GetStats() { return m_pStats; } };
class CSWCMessage { public: void SetGuiSkillRank(CSWCCreature*,unsigned short,unsigned short,unsigned short); };
// FUNCTION: SWKOTOR 0x0064CE90
void CSWCMessage::SetGuiSkillRank(CSWCCreature* creature,unsigned short value,unsigned short mask,unsigned short skill)
{
 if (value & mask) {
  if (creature->GetStats()->GetBaseSkillRank(skill) <= 0) creature->GetStats()->SetBaseSkillRank(skill,1);
 } else {
  if (creature->GetStats()->GetBaseSkillRank(skill) > 0) creature->GetStats()->SetBaseSkillRank(skill,0);
 }
}
