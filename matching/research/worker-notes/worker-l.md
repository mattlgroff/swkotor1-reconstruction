# Worker L matching notes

Range: 0x006A0000–0x006EFFFF. Read Kotor RE's historical worker-l notes and
current matching headers; its old next queues include many now-owned targets.

## Current checkpoint, 2026-10-26

- Accepted control-pointer array IndexOf at 0x006C2A40 is a 48-byte exact
  template instantiation; control and panel pointer versions share the code.
- Character panel dispatch 0x006AE980/0x006B0B90/0x006B2220 and generated
  deleting wrapper 0x006B2200 are exact. The regular destructor is external.
  An unannotated constructor emits the vtable/wrapper without an invented
  empty regular destructor. Do not count that constructor as reconstructed.
- CAurEmitter::SetBirthrateA/B at 0x006DAF60/0x006DAFA0 are retained two-diff
  attempts. Rate/timer fields are +0x4C/+0x50 and +0x74/+0x78. Ordered
  timer<old-rate and timer!=0 guard timer -= old-rate - new-rate. The x87
  arithmetic matches; the integer load moves across the second branch.
  Five bounded shape checks found no improvement; defer these scheduling
  ties and continue the wider pool.

## Next family

Screen at least 100 eligible range addresses, then process a complete
related GUI family using known panel headers. Compact ownership anchors
include inventory ctor 0x006B34C0, regular dtor 0x006B40C0, deleting wrapper
0x006B4B90, Draw 0x006B4BB0; equip ctor 0x006BA980, regular dtor
0x006B8840, deleting wrapper 0x006B9450, Draw 0x006BB510 (UpdateInventory
then base Draw); and graphics-advanced ctor 0x006E16E0, regular dtor
0x006E1F40, deleting wrapper 0x006E1F20. Confirm names, call targets and
current source ownership. Compiler-generated wrappers may reference
externally declared real destructors; do not substitute fake callee labels
or copy instruction sequences.
