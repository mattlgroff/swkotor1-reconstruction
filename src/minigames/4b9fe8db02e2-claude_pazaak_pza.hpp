#pragma once
// Lane PZ-A (Pazaak rules, AI, shuffling, VM entry, client-app start/end), KOTOR 1 GOG 1.03 swkotor.exe.
// Builds against the shared contract claude_pazaak_decls.hpp (PZ-B tier h1, copied byte-exact into contracts/;
// sha256 7AE5B691EF45C01286C48DE117E660235D0EF4E1BB0D2FAA8904F5231D01B009). Export names and @N come from it.
//
// One deviation from h1 (unchanged since h0), requested as a header amendment (INTEGRATION.md): 0067CB10 CSWPazaak::DoAITurn returns the
// new AI state in EAX (0067CB29 mov [esi+22Ch],eax; the caller DoGameSequence consumes it at 00680335 mov edi,eax).
// h1 declares it void. The decorated name @kotor_fastcall_0067cb10@8 is unchanged; only the C++ return type differs,
// so this header renames h1's declaration while including it and declares the int-returning form.
#define kotor_fastcall_0067cb10 kotor_fastcall_0067cb10_h1_declared_void
#include "602f99a6ff9a-claude_pazaak_decls.hpp"
#undef kotor_fastcall_0067cb10
#include <cstddef>
#include <cstdint>

extern "C" std::int32_t __fastcall kotor_fastcall_0067cb10(void* pazaak, void*);   // DoAITurn RET 0, EAX = new AI state

namespace pza {
// Layout (from the original instructions, checked against h1):
//   CPazaakCard   8 bytes {+0 i32 value (card code), +4 i32 flag (flip sign: nonzero = negative)}
//   CPazaakPlayer 0x70    {+0 hand[4], +0x20 board[9], +0x68 stood, +0x6C sets won}
//   CSWPazaak     0x230   {+0 wager, +4 opponent, +8 human, +0x78 AI, +0xE8 deck[40], +0x228 deck index, +0x22C AI state}
struct Card { std::int32_t value; std::int32_t flag; };
struct Player { Card hand[4]; Card board[9]; std::int32_t stood; std::int32_t sets; };
struct Pazaak {
    std::int32_t wager; std::uint32_t opponent; Player human; Player ai;
    Card deck[40]; std::int32_t deck_index; std::int32_t ai_state;
};
static_assert(sizeof(Card) == 8, "CPazaakCard");
static_assert(sizeof(Player) == pazaak::player::kSize && offsetof(Player, hand) == pazaak::player::kHand &&
              offsetof(Player, board) == pazaak::player::kBoard && offsetof(Player, stood) == pazaak::player::kStanding &&
              offsetof(Player, sets) == pazaak::player::kSetsWon, "CPazaakPlayer vs h1");
static_assert(sizeof(Pazaak) == pazaak::core::kSize && offsetof(Pazaak, wager) == pazaak::core::kWager &&
              offsetof(Pazaak, opponent) == pazaak::core::kOpponent && offsetof(Pazaak, human) == pazaak::core::kPlayer &&
              offsetof(Pazaak, ai) == pazaak::core::kNpc && offsetof(Pazaak, deck) == pazaak::core::kDeck &&
              offsetof(Pazaak, deck_index) == pazaak::core::kDeckIndex && offsetof(Pazaak, ai_state) == pazaak::core::kAiState,
              "CSWPazaak vs h1");
}  // namespace pza
