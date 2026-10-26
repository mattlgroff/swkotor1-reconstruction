# Project status

The game is reported playable end to end **as far as we know**. This is a qualified report from manual play, not exhaustive full-game verification. Known math discrepancies remain under investigation. Function-matching statistics do not measure gameplay coverage.

The private reconstruction checkpoints reported these results on 24 October 2026:

- **Swoop, candidate c64:** three Taris races, including a win and story progression. Tatooine and Manaan races were not covered. Reloading saves created during this session was not tested.
- **Ebon Hawk turret, c64:** boarding, the escape cutscene, turret combat, a win followed by the Leviathan cutscene and onward travel to Dantooine; a loss produced the expected game-over behavior. Random hyperspace turret encounters were not covered.
- **Pazaak, c65:** Garouk deck purchase (96 to 46 credits), the lesson, side-deck and card controls, AI turns, set/match completion, Niklos wagered losses, a mid-match forfeit, cancellation at the wager popup without charge, reload, and a wagered win (46 to 51 credits). The report recorded no traps or crashes in these runs.
- **Lip sync, c65:** the user confirmed Canderous's mouth animation after the launcher bound the LIPS resource alias.

These results belong to the private candidates, not a fresh test of this public integration. The integrated public source rebuilt successfully on 24 October 2026 from public base `7be96d85d95a5296770f7445c82b83f62731ca79` with this patch, using the toolchain in [BUILDING.md](BUILDING.md) and four compiler jobs. All 1,921 units compiled and linked (exit 0). There were no linker warnings; one unused-function compiler warning came from an unchanged public source file and recipe.

The resulting executable SHA-256 is `daec6bf0399d0986cbc55a82ef9bdceb65b53070b2cf61efef32afd6515f4a99`. Linked-image checks passed 12 minigame tables, 28 SWMG bindings and seven Pazaak tables with zero mismatches; removing the Pazaak provider made the link fail as expected. These checks do not establish gameplay behavior.

This exact public binary was launched under a debugger from an isolated runtime copy on 24 October 2026. Process/window metadata and the loader report confirmed window creation and middleware loading. It was subsequently launched normally and handed over for manual testing. Automated menu, Pazaak, swoop, turret and lip-sync checks remain **UNRUN**; manual gameplay results have not been reported. No public minigame smoke-test pass or full playthrough is claimed.

The subsequent 24 October SFX patch corrects `UpdateSoundEngine` (005F5370): camera orientation writes a 16-byte quaternion, but the listener previously supplied a 12-byte vector scratch buffer. The buffer is now a 16-byte vector/quaternion union with a correctly typed output pointer. The private c66 report records independent review and user confirmation of restored combat sounds in the Taris Bendak arena. That is historical private-build evidence; it is not an audio test of the rebuilt public executable.

The public SFX source rebuilt successfully from base `e904e6287845f12c72fe7950327485aa114760bc` with the same documented toolchain and four compiler jobs: all 1,921 units compiled and linked (exit 0). Executable SHA-256: `c2a93275133a6a3c3074e353db6a99dc3a3bb3e335d19d302094e435561b267d`. This successor has not been launched or checked for audible SFX here; public audio verification remains **UNRUN**. Publication does not imply a manual gameplay pass.

The c67 math integration brings in the reviewed x87 trig tiers and rounding changes. The private audit classifies 68 transcendental sites: 15 changed in the first tier, 21 in the second, and 32 already exact and unchanged. Two additional particle acos sites change outside that count. The four rounding-conversion sites now use an authored `FloatToInt` helper following the current x87 rounding mode, replacing the floor/fmod emulation. These counts describe the audited scope, not all floating-point arithmetic or a proof of universal numerical equivalence.

The L1 component is an original-derived local dependency, generated automatically from the supported executable during the build. It supplies the selected original CRT math implementations and authored x87 adapters. Generated code, tables and objects are not distributed in this repository. The existing reconstructed acos/asin/pow wrappers retain their callers, and other modern CRT math calls remain. See [BUILDING.md](BUILDING.md#local-original-derived-math-dependency). This does not add matches to the separate original-compiler matching track.

The private c67 report records finite numerical comparisons, binding and constant checks, and an unchanged private stopper census; it explicitly reports no live run. Those results are historical evidence, not public gameplay verification. Full-arithmetic reconstruction remains future work.

The public c67 integration rebuilt on 25 October 2026 from base `e1aaf19243b9355e57cda38108e629de06bdea09`: all 1,921 compilation units plus three local L1 objects linked successfully (exit 0). Executable SHA-256: `58f19fef528b089dbda073d8369aaa05a63d7fa50c9cb555f675af020369e35a`. The local L1 comparison passed 2,806 instructions, 183 relocated fields, 508 internal branches and 15,372 data bytes under the documented rebinding rules. A bounded fixture passed 12 basic math/initialization cases and 24 FloatToInt cases across four rounding modes; this does not repeat the private large-scale numerical suite or execute every audited game site.

This public c67 executable has not been launched or gameplay-tested here. Camera, turret, animation, particle, model-loading and other live paths remain **UNRUN** for this binary. The source and build checks do not establish a full playthrough or universal numerical equivalence.

## c68 integration

The c68 checkpoint adds the end-credits `StopSounds` call, restores the sound-play length return through the reviewed callers, removes the wrong-ABI string-list Add duplicate and the added `SetEventDescriptions` exit guard, and restores the original startup `HD0:TEMP` removal operation. Much of the private harness cleanup was already unnecessary in the public build: its selected window, gamma, capture and event-creation providers already call the real APIs, and the private frame-capture and strict-window harness layers are absent. Those existing public behaviors are preserved rather than credited as new fixes. Remaining fatal binding checks are not silently removed. This is the faithful reconstruction track, not the separate Reforged modernization plan.

The private c68 report records user checks of keyboard input after resolution changes, dialogue skip, Inventory, 3D combat sounds, fullscreen/resolution changes and Alt-Tab, and starting end credits without the former trap. Gamma could not be assessed with HDR enabled. The relative movie-path correction for dark-side ending music was made afterward and was **not rerun live** in that report. These are historical private-candidate observations, not tests of this public executable.

The public runtime instructions retain the player's graphics preferences unless windowed mode is explicitly selected, and specify `MOVIES=.\movies` for the original ending-music path comparison. They continue to use an isolated runtime with copied resources and `HD0=.\`; no installation or saves are modified. The private launcher and its junction-based layout are not substituted for this public setup.

The c67 x87 changes and automatic local original-derived CRT dependency remain in place. C68's L5 math harness is test infrastructure, not a new full-arithmetic runtime implementation or a byte-identical matching gain. Known NaN-only differences in the affected sound unit and previously documented ownership/exception-path limitations remain; this integration does not claim to close them.

The public c68 integration rebuilt on 26 October 2026 from base `e6cd312e538be656fefdefe5dbf819e4506a29c5`, which retains the published c67 changes and subsequent matching work. The canonical `build.py --target game` build used the documented toolchain, a fresh output directory and four compiler jobs: all 1,921 units plus the local L1 math objects linked successfully (exit 0). Executable SHA-256: `1f52dd1d0d0d7869501df39116d8b36e5352cef7e0ace993e137baba6b522a91`. Source pins and original input hashes were unchanged by the build.

Focused public checks passed: the obsolete string-list ABI export is absent, sound-play return providers are selected, and the credits call reaches the existing ambient StopSounds implementation. The affected sound unit emits x87 arithmetic without SSE-register instructions. Twelve minigame tables, 28 resolver bindings and seven Pazaak tables had no mismatches; removing the Pazaak provider failed the link. The same public per-symbol stopper census decreased from 373 to 372, with the credits trap removed; these counts differ from the private candidate's census and are not gameplay coverage. There were no linker warnings and one unchanged unused-function compiler warning.

Regression checks passed 12 bounded math/initialization fixtures, 24 FloatToInt cases, local-math omission/provider/input rejection controls, 70 synthetic Python tests, and isolated runtime-directory preparation checks. All 22 affected unchanged c67 units first reproduced their prior executable COFF code and relocation counts. These checks do not repeat the private large-scale ABI differential suite or establish universal numerical equivalence.

This public c68 executable has not been launched or gameplay-tested here. Credits, ending music, sound timing, input/resolution changes, gamma and other live paths remain **UNRUN** for this binary. Publication does not imply that the private results have been repeated publicly.

## Intentionally excluded

Developer Tools, dead/unused Neverwinter multiplayer code, and Cheat Codes are intentionally outside this project's normal single-player gameplay scope. These are deliberate scope exclusions, not unfinished normal single-player gameplay features. This scope decision does not establish that every address in those categories is unreachable or correctly classified, and it does not remove unresolved functions from the recorded stopper census or matching metrics. No implemented code is removed by this documentation change.

The source builds with the Windows toolchain in [BUILDING.md](BUILDING.md). 

Planned documentation covers reconstruction provenance, compatibility and intentional omissions.
