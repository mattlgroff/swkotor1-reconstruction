# Worker K round 1 — placeable / door lifecycle and actions

Branch `codex/worker-k-r1`, base `5edb148`, owned range 00680000–0069FFFF.
Read matching agreements, workflow audit and old worker K notes. Prefetched all
136 eligible candidates with todo.py; selected the related CSWCPlaceable /
CSWCDoor family, including adjacent unlisted callbacks discovered in executable.
Current shared base declarations remain untouched; extended layouts are isolated.

Counts pending canonical comparison. Screened: 136. No screened address is
counted as an implementation or exact match. No shared reports modified.

Checkpoint 1: actual attempts 10, NEW exact 8, retained misses 2. Exact:
6825F0, 682620, 682660 (new unlisted callback), 683D00, 683D40, 683D90,
683E00, 683E50. Misses 6826A0 / 683E90: known tutorial-dispatch register /
global-load scheduling; three normal shapes tried, defer. The speed/animation
members are +48/+4C. Animation action IDs are full-word arguments (original
passes them directly to GetAnimationLength without narrowing); SetAnimation
uses a short and stores it. Invert SetOpenState's zero test to get the zero
arm after the positive-range arm. A named GetMessage local orders its call
before loading the object ID. The ShowTutorialWindow wrapper has an existing
void/int semantic conflict in symbols; callers here consume EAX, retained int
caller view and deferred ABI correction rather than changing shared contracts.

Checkpoint 2: cumulative actual attempts 17, NEW exact 15, retained misses 2.
Added exact 682BE0 / 6831B0 / 683200 / 683630 / 6836A0 / 683BB0 / 683F90.
Lifecycle now covers both full destructors and door constructor; placeable
scalar deleting destructor compiled separately so it calls the full destructor.
Inline module GetArea preserves ECX for UnloadLight (direct field read gives
EAX plus a move). A local app pointer before the timer getter matches ctor
argument scheduling, as predicted by old notes. IsHostile deliberately performs
the repeated server lookup, including the original null-result dereference
edge; caching the first lookup would alter observable calls.

Checkpoint 3: cumulative actual attempts 23, NEW exact 21, retained misses 2.
Six additional exact functions: 682270, 6823C0 (unlisted GetName), 682DD0
(unlisted lock action), 683F00, 6836C0, 684CD0 (unlisted door AI update).
Both AI functions share a typed macro, preserving object-array status global,
world-time output order and destruction after successful array removal. Load
object ID into a named local before retrieving the array to match EAX/EDX.
GetAIList inline accessor yields the original LEA; a direct empty list member
expression produced MOV/ADD. Canonical GetServerObject symbol retained via a
polymorphic link base at offset zero; a nonpolymorphic empty base would receive
an MSVC7 +4 adjustment and is incorrect. All lifecycle members were rechecked.

## Final lifecycle/action/model family result

- Screened 136 eligible in-range addresses; actual new annotations/attempts 28.
- NEW exact 23; retained NEW NONMATCH 5; prior NONMATCH promotions 0.
- Whole K folder canonical `-q -j4` result: **323/355**, from **300/327**.
  Compared a private `/tmp/worker-k-final-status.csv` to checked-in baseline:
  no prior MATCH regressions and no changed old NONMATCH distances.
- No shared reports, symbols, assets, VS2022 source, old-repository files or
  out-of-range addresses changed. No push. Local checkpoint commits only.

NEW exact addresses: 00682270, 006823C0, 006823F0, 006825F0, 00682620,
00682660, 00682BE0, 00682DD0, 006831B0, 00683200, 00683630, 006836A0,
006836C0, 00683BB0, 00683D00, 00683D40, 00683D90, 00683E00, 00683E50,
00683F00, 00683F90, 006842E0, 00684CD0.

Retained misses: 006826A0 (6), 00682C60 (17), 00682E20 (19),
00682F20 (11), 00683E90 (8). 2DA misses retain the best of three normal
receiver/temporary shapes; no instruction-copying, raw-address loads or
artificial signature tricks were used. Strings were read from original image:
SetAppearance uses `HitCheck` / `NonCull`, shadow uses `ShadowSize` and the
three `Fx_Shadow_*` literals. GetCanSeeHeight uses `CanSeeHeight`.

Both model loaders now match. Preserve argument order `(resref, part, unused)`;
the third stack word is unused. Door's +D0 write occurs before the null test,
as in the original. The model event callback is cdecl with event second and
client-door receiver third. Placeable's implicit CExoString suffix argument
and Quaternion-before-Vector construction reproduce all temporaries and EH
cleanup in its 512-byte loader. Model APIs use the established CAnimBase
(0x14) / CSWCAnimBase (0xF0) hierarchy; class-size assertions also preserve
CSWCObject 0xFC, placeable 0x150 and door 0x144. SetAnimation consumes an int
animation ID for the canonical int/float PlayAnim interface and stores its low
word, matching the original full stack-word argument reads. CanPerformAction
is the GUI elapsed-action timer gate at 0062F9A0; FadeObject at 0063DF60
advances the fade and reports timed deletion, rather than a simple getter.

No V4 differential promotion is claimed: this matching-only family depends on
original app/timer/model virtual interfaces and compiler EH runtime. Byte
identity and original side effects/ABI were checked with the original toolchain.

### Next coherent follow-on queue

The lifecycle, model allocation/teardown and compact action-dispatch workset is
complete, with the five honest misses retained. Larger animation/light/menu
work remains screened but **not counted as attempted**: LoadLight 006826E0,
project-target 00682FF0, placeable UpdateAnimation 006832A0, target menu
006837D0, door UpdateAnimation 00684030 and interaction 00684660. The two
animation methods need a shared next-animation record and audio contract;
original short output slots retain unwritten high bytes, and the sound API
005D5E10 has conflicting typed signatures in existing symbols. Keep those
contracts honest rather than manufacture high-word values or a false call
signature. Original disassembly of these functions was screened; no code is
being claimed for them. Subsequent separate family: area VFX lifecycle or
client player appearance; do not jump into stale 006A-range queues.
