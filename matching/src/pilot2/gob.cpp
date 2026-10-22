// Gob::IsInCutscene (pilot group 2). The function is 16 bytes; the 736-byte
// extent in the function table spans the following small Gob accessors too.
class Gob {
public:
    char pad_0x00[0x14];
    unsigned char m_bInCutscene;
    unsigned char IsInCutscene();
};
// FUNCTION: SWKOTOR 0x0043E8A0
unsigned char Gob::IsInCutscene() { return m_bInCutscene; }
