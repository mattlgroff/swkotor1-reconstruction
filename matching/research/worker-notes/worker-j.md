# Worker j notes

## Audit fixes 2026-11-10

The relocation audit (audit-s1) found MATCH functions in j with wrong targets
or literals that matchcmp masks:
- CNetLayer::HandleBNCSMessage: all five password tests call
  CExoString::operator==(const char*) (0x005E5290), not operator!=
  (0x005E5390); j/exo_string.h had 0x005E5290 under the wrong operator.
  The branch bytes are identical either way, so only the callee shows it.
- CERFFile::AddFile splits the name at ":" (alias separator) and
  CERFFile::AddDirectory formats "%s%s"; the source had "\\" and "%s\\%s".
- CVirtualMachineInternal Save/LoadScriptSituation: the GFF field for the
  script's +0x24 value is "CRC" (0x0074C4E4), not "LoadedFromSave".
- ~CExoStreamingSoundSourceInternal closes the +0x38 member with fclose
  (0x006FACF0); it is a FILE*, not a buffer freed with free.
Lesson: read string literals and operator overloads from the exe and
mac_names.csv, not from neighbouring headers.
