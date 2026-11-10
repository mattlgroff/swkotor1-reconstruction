# Worker e notes

## Audit fixes 2026-11-10 (fix2-s1)

The relocation audit (audit-s1) found MATCH functions in e whose masked
relocations pointed at the wrong target. Folder stays at 189/222.

- anim.cpp AnimTreeRun::~AnimTreeRun: `delete m_pTree` must call
  MaxTree::operator delete (0x0044AEC0), not the global one; declared the
  class operator on the local MaxTree.
- behavior.cpp: the hand-written stand-ins `thunk_FUN_0049fce0` (0x004A0030)
  and `behavior_attach_scalar_delete_004a0010` were the compiler-generated
  `??1CAurBehaviorAttachTranslate` (a jmp to ~CAurBehaviorAttach) and
  `??_GCAurBehaviorAttachTranslate` (folded with the Rotate/NoOrientation
  copies; vtable 0x007438B8 is set by the Translate ctor 0x0049FDC0). The
  stand-in ??_G called the base dtor directly instead of 0x004A0030. Both
  are now annotated SYMBOL lines on the generated functions; likewise
  0x004A0040 is `??_GCAurBehaviorCameraFollow`. All MATCH.
- anim.cpp MakeInlinePartList CExoArrayList<Part*> Allocate/Add rows left
  as is: the scene-side Part list calls 0x005E03C0 / 0x00444FE0, and the
  conflict comes from g/aur_array.cpp annotating 0x00437D60 / 0x00438B40 as
  the Part* instances (see worker-h.md, same date).

Lesson: a hand-written `__fastcall` stand-in for a compiler-generated
destructor can match while calling the wrong callee; annotate the
generated ??_G / ??1 symbol instead.
