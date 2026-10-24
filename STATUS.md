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

Known math defects include loss of x87 intermediate precision and double rounding in trigonometric helpers used by camera, animation, particle and other engine code, including turret orientation. Reviewed x87 angle-math tiers remain **unbuilt in any playable candidate**. They cover 68 audited transcendental sites (36 changed and 32 reported already exact), plus replacement of modern `std::acos` in particle motion and correction of its pi/2 constant. Their isolated comparison tests and reviews do not establish gameplay correctness. Those inline-assembly tiers are reference material for the requested plain-C++ math work and are not included in this runtime integration. The checkpoint records the C++ math lane as paused; the discrepancies remain open work.

Other remaining functions include unused Neverwinter multiplayer remnants, debug tools and cheats; their classifications and any intentional omissions still need confirmation.

The source builds with the Windows toolchain in [BUILDING.md](BUILDING.md). 

Planned documentation covers reconstruction provenance, compatibility and intentional omissions.
