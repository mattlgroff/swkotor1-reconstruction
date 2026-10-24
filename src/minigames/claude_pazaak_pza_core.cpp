// Lane PZ-A tier a1: CSWPazaak / CPazaakPlayer rules, AI and shuffling (006E4360..006E4C60, 0067CAE0, 0067CB10),
// reconstructed from the original swkotor.exe (GOG 1.03) disassembly, read-only. Original PCs on every line.
// Every random draw goes through the game CRT rand 006FC447 (crt_rand_006fc447, shared holdrand stream), never host rand.
// Integer only; no x87. Recipe: W5 isolated /EHs (LoadInitialSideDeck has two original unwind states).
#include "4b9fe8db02e2-claude_pazaak_pza.hpp"
#include <cstring>

using pza::Card;
using pza::Player;
using pza::Pazaak;

namespace menu_resources {
struct Ref { unsigned char bytes[16]; Ref* construct_00406d80(const char* text); };   // CResRef(const char*) [0030]
}
namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String* s, void*); } // CExoString() [menu20_bindings]

extern "C" {
int __cdecl crt_rand_006fc447();                                                     // _rand 006FC447 [0479]
struct PzaRef16 { unsigned char bytes[16]; };
void* __fastcall c2da_current_construct_00413cc0(void* table, void*, PzaRef16 name, unsigned flag); // C2DA(CResRef,int) RET 14h [0377]
int __fastcall c2da_current_load_004143b0(void* table, void*);                       // C2DA::Load2DArray [0380]
int __fastcall c2da_current_get_string_entry_00413190(void* table, void*, int row, int column, void* out); // RET 0Ch [0390]
void __fastcall c2da_current_unload_004139e0(void* table, void*);                    // C2DA::Unload2DArray [0377]
void __fastcall c2da_current_destroy_00413d80(void* table, void*);                   // C2DA::~C2DA [0377]
char __fastcall c2da_current_string_index_005e5600(void* s, void*, int index);       // CExoString::operator[] RET 4 [0288]
void __fastcall frame_LogStringDtor_005e5c20(void* s, void*);                        // CExoString::~CExoString [0459]

// [007A26B8] int[28] card value table (.data, read only by 006E4360 and 006E4910; no writer in the original).
// Index = card code: 0..5 +1..+6, 6..11 -1..-6, 12..17 +1..+6 (flip; negated when flag != 0), 18..27 1..10.
extern const int kotor_pazaak_card_values_007a26b8[28] = {
    1, 2, 3, 4, 5, 6, -1, -2, -3, -4, -5, -6, 1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
}

namespace {
constexpr int k_bust = -1000000;           // 0xFFF0BDC0
constexpr unsigned k_invalid = 0x7f000000u; // OBJECT_INVALID
const char k_pazaak_decks_007588a4[] = "PazaakDecks";
inline Player* player_of(void* p) { return static_cast<Player*>(p); }
inline Pazaak* pazaak_of(void* p) { return static_cast<Pazaak*>(p); }
// card contribution as GetTotal / FindPotentialSidecardTotal compute it (006E4373..A0 and 006E4971..97)
inline int card_value(int value, int flag) {
    if (value < 0) return 0;                                                          // test/jge; xor ecx,ecx
    if (value >= 12 && value < 18 && flag != 0) return -kotor_pazaak_card_values_007a26b8[value]; // jl/jge/test; neg
    return kotor_pazaak_card_values_007a26b8[value];
}
}  // namespace

extern "C" {

// ================================================================== 006E4360 CPazaakPlayer::GetTotal() RET 0
std::int32_t __fastcall kotor_fastcall_006e4360(void* self, void*) {
    Player* const p = player_of(self);
    int total = 0;                                                                    // 006E4361
    for (int i = 0; i < 9; ++i) {                                                     // 006E4366..006E4570 (unrolled x9)
        const int value = p->board[i].value;                                          // 006E4370
        if (value == -1) break;                                                       // 006E4373..76 -> 006E4576
        total += card_value(value, p->board[i].flag);                                 // 006E437C..A7
    }
    return total;                                                                     // 006E4576..78
}

// ================================================================== 006E4580 CPazaakPlayer::ChooseSidedeck(CPazaakCard* deck10) RET 4
// Moves 4 random cards of the 10 into hand[0..3]; the vacated slot gets the last live card, the last becomes {-1,0}.
void __fastcall kotor_fastcall_006e4580(void* self, void*, void* cards10) {
    Player* const p = player_of(self);
    Card* const cards = static_cast<Card*>(cards10);
    int n = 10;                                                                       // 006E4588 ebp
    for (int k = 0; k < 4; ++k, --n) {                                                // 006E4592 / 006E45D7..DD
        const int r = crt_rand_006fc447() % n;                                        // 006E45A0..A6 cdq/idiv
        p->hand[k].value = cards[r].value;                                            // 006E45AE..B1
        p->hand[k].flag = cards[r].flag;                                              // 006E45B4..B8
        cards[r].value = cards[n - 1].value;                                          // 006E45BB..BE
        cards[r].flag = cards[n - 1].flag;                                            // 006E45C1..C7
        cards[n - 1].value = -1;                                                      // 006E45CB
        cards[n - 1].flag = 0;                                                        // 006E45D0
    }
}

// ================================================================== 006E45F0 CPazaakPlayer::AddGameCard(CPazaakCard) RET 8
void __fastcall kotor_fastcall_006e45f0(void* self, void*, std::int32_t value, std::int32_t flag) {
    Player* const p = player_of(self);
    int i = 0;                                                                        // 006E45F3
    for (; i < 9; ++i) {                                                              // 006E45F8..04
        if (p->board[i].value == -1) {
            p->board[i].value = value;                                                // 006E4610
            p->board[i].flag = flag;                                                  // 006E4614
            break;
        }
    }
    if (i >= 8 || kotor_fastcall_006e4360(self, nullptr) == 20) p->stood = 1;         // 006E4618..29 (9th card or 20)
}

// ================================================================== 006E4640 CSWPazaak::ClearGameBoard() RET 0
void __fastcall kotor_fastcall_006e4640(void* self, void*) {
    Pazaak* const z = pazaak_of(self);
    for (int i = 0; i < 9; ++i) {                                                     // 006E4640..0006E4714
        z->human.board[i].value = -1; z->ai.board[i].value = -1;
        z->human.board[i].flag = 0; z->ai.board[i].flag = 0;
    }
    z->human.stood = 0;                                                               // 006E471A [+70h]
    z->ai.stood = 0;                                                                  // 006E471D [+E0h]
}

// ================================================================== 006E4730 CSWPazaak::ShuffleDeck() RET 0 (40 rand draws)
void __fastcall kotor_fastcall_006e4730(void* self, void*) {
    Pazaak* const z = pazaak_of(self);
    Card pool[40];                                                                    // [esp+1Ch], 140h bytes
    for (int i = 0; i < 40; ++i) pool[i].flag = 0;                                    // 006E4740..5A
    for (int i = 0; i < 40; ++i) {                                                    // 006E475C..7E
        pool[i].flag = 0;
        pool[i].value = i % 10 + 18;                                                  // cdq/idiv 0Ah; add edx,12h
    }
    int n = 40;                                                                       // 006E4780 ebx
    for (int k = 0; k < 40; ++k, --n) {                                               // 006E47A0..D9
        const int r = crt_rand_006fc447() % n;                                        // 006E47A0..A6
        z->deck[k].value = pool[r].value;                                             // 006E47A8..AC
        pool[r].value = pool[n - 1].value;                                            // 006E47AE..B0
        z->deck[k].flag = pool[r].flag;                                               // 006E47BE..C1
        pool[r].flag = pool[n - 1].flag;                                              // 006E47C4..C7
        pool[n - 1].value = -1;                                                       // 006E47CA..CD
        pool[n - 1].flag = 0;                                                         // 006E47D0..D4
    }
    z->deck_index = 39;                                                               // 006E47E2
}

// ================================================================== 006E4800 CSWPazaak::DrawCard() -> CPazaakCard (hidden return) RET 4
void* __fastcall kotor_fastcall_006e4800(void* self, void*, void* out_card) {
    Pazaak* const z = pazaak_of(self);
    if (z->deck_index < 0) kotor_fastcall_006e4730(self, nullptr);                    // 006E4803..0D
    const int index = z->deck_index;                                                  // 006E4812 (re-read)
    Card* const out = static_cast<Card*>(out_card);
    out->value = z->deck[index].value;                                                // 006E4818..23
    out->flag = z->deck[index].flag;                                                  // 006E4825..2C
    --z->deck_index;                                                                  // 006E482F
    return out_card;                                                                  // EAX = [esp+8]
}

// ================================================================== 006E4840 CSWPazaak::CanUsePlayerSidedeckCard(int) RET 4
std::int32_t __fastcall kotor_fastcall_006e4840(void* self, void*, std::int32_t hand_index) {
    return pazaak_of(self)->human.hand[hand_index].value >= 0 ? 1 : 0;                // 006E4845..4D setge
}

// ================================================================== 006E4860 CSWPazaak::UsePlayerSidedeckCard(int) RET 4
std::int32_t __fastcall kotor_fastcall_006e4860(void* self, void*, std::int32_t hand_index) {
    Pazaak* const z = pazaak_of(self);
    Card& card = z->human.hand[hand_index];
    if (card.value < 0) return 0;                                                     // 006E4868..74
    kotor_fastcall_006e45f0(&z->human, nullptr, card.value, card.flag);               // 006E4877..84
    card.value = -1;                                                                  // 006E488E
    card.flag = 0;                                                                    // 006E4892
    return 1;                                                                         // 006E4897
}

// ================================================================== 006E48A0 CSWPazaak::DetermineWinner() RET 0
// 0 = not both standing, 1 = player wins, 2 = opponent wins, 3 = tie. A bust forces both players to stand.
std::int32_t __fastcall kotor_fastcall_006e48a0(void* self, void*) {
    Pazaak* const z = pazaak_of(self);
    int human = kotor_fastcall_006e4360(&z->human, nullptr);                          // 006E48A4..AF
    int ai = kotor_fastcall_006e4360(&z->ai, nullptr);                                // 006E48AC..B1
    if (human > 20 || ai > 20) { z->human.stood = 1; z->ai.stood = 1; }               // 006E48B6..C8
    if (z->human.stood == 0 || z->ai.stood == 0) return 0;                            // 006E48CE..DD -> 006E4906
    if (human > 20) human = k_bust;                                                   // 006E48DF..E4
    if (ai > 20) ai = k_bust;                                                         // 006E48E9..EE
    if (human > ai) return 1;                                                         // 006E48F3..F5
    return (human >= ai ? 1 : 0) + 2;                                                 // 006E48F7..FE
}

// ================================================================== 006E4910 CSWPazaak::FindPotentialSidecardTotal(CPazaakPlayer*, int* index, int lower) RET 0Ch
// Scans the 4 hand cards (flip cards in both signs: negative pass first) for the best total: lower == 0 -> highest
// total above the current (a tie only replaces a flip-card best); lower != 0 -> lowest non-bust total below it.
// Side effect (original): every flip card in the hand is left with flag 0 (its last pass), then the chosen card's
// flag is set to the sign that produced the best total. *index is written only when a card was chosen.
std::int32_t __fastcall kotor_fastcall_006e4910(void*, void*, std::uint32_t player_arg, std::uint32_t index_arg,
                                                std::uint32_t lower_arg) {
    Player* const p = reinterpret_cast<Player*>(static_cast<std::uintptr_t>(player_arg));
    int* const out_index = reinterpret_cast<int*>(static_cast<std::uintptr_t>(index_arg));
    const int lower = static_cast<int>(lower_arg);
    const int base = kotor_fastcall_006e4360(p, nullptr);                             // 006E491B..25 [esp+20h]
    int best = base > 20 ? k_bust : base;                                             // 006E4922..2B eax
    int best_index = -1;                                                              // 006E4932 [esp+18h]
    int best_flag = 0;                                                                // 006E493A [esp+1Ch]
    int best_is_flip = 0;                                                             // 006E493E [esp+14h]
    for (int i = 0; i < 4; ++i) {                                                     // 006E4930 ebx .. 006E4A07
        Card& card = p->hand[i];
        const int code = card.value;                                                  // 006E4942 esi (read once)
        int passes = 2;                                                               // 006E4944 [esp+10h]
        do {
            int flag = 0;                                                             // 006E4950 edx
            if (code >= 12 && code < 18) {                                            // 006E4952..5A
                flag = passes == 2 ? 1 : 0;                                           // 006E495C..61 sete
                card.flag = flag;                                                     // 006E4964
            } else {
                passes = 1;                                                           // 006E4969
            }
            int total = card_value(card.value, card.flag) + base;                     // 006E4971..A2 (re-read [edi])
            if (total > 20) total = k_bust;                                           // 006E49A4..A9
            bool take;
            if (lower == 0) {                                                         // 006E49AE..B4
                take = total > best || (total == best && best_is_flip != 0);          // 006E49B6..D0
            } else if (total < best) {                                                // 006E49BA..BC
                take = total > k_bust;                                                // 006E49BE..C4 (jg)
            } else {
                take = total == best && best_is_flip != 0;                            // 006E49C8..D0
            }
            if (take) {                                                               // 006E49D2
                best = total;
                best_index = i;
                best_flag = flag;
                best_is_flip = code >= 12 && code < 18 ? 1 : 0;                       // 006E49DF..EE
            }
        } while (--passes != 0);                                                      // 006E49F6..FA
    }
    if (best_index >= 0) {                                                            // 006E4A0D..13
        *out_index = best_index;                                                      // 006E4A1D
        p->hand[best_index].flag = best_flag;                                         // 006E4A23
    }
    return best;                                                                      // EAX
}

// ================================================================== 006E4A40 CSWPazaak::LoadInitialSideDeck(int deck, CPazaakCard* out10) RET 8
// pazaakdecks.2da row `deck` (deck < 0: rand() % rows, only after a successful load), columns 0..9 decoded from the
// cell text "<sign><digit>": value = digit - '1', '-' adds 6, '*' adds 12; flag 0. A failed load leaves out10 untouched.
void __fastcall kotor_fastcall_006e4a40(void*, void*, std::int32_t deck, void* out_cards10) {
    menu_resources::Ref name;                                                         // outgoing CResRef temporary (POD)
    name.construct_00406d80(k_pazaak_decks_007588a4);                                 // 006E4A64..72 "PazaakDecks"
    PzaRef16 by_value;
    std::memcpy(by_value.bytes, name.bytes, 16);
    alignas(8) unsigned char table[0x54];                                             // C2DA at [esp+18h] (0x54, operator new size 006DF5E4)
    c2da_current_construct_00413cc0(table, nullptr, by_value, 0);                     // 006E4A77..7B
    struct TableGuard {                                                               // EH state 0: ~C2DA (00737560)
        unsigned char* t;
        ~TableGuard() { c2da_current_destroy_00413d80(t, nullptr); }                   // 006E4B26..32
    } table_guard{table};
    if (c2da_current_load_004143b0(table, nullptr) != 0) {                            // 006E4A80..8F
        alignas(4) unsigned char text[8];                                             // CExoString at [esp+10h]
        kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(text), nullptr);                // 006E4A95..99
        struct StringGuard {                                                          // EH state 1: ~CExoString (00737568)
            unsigned char* s;
            ~StringGuard() { frame_LogStringDtor_005e5c20(s, nullptr); }               // 006E4B18..21
        } string_guard{text};
        if (deck < 0) {                                                               // 006E4A9E..A6
            int rows;
            std::memcpy(&rows, table + 0x24, 4);                                      // 006E4AA8 C2DA row count
            deck = crt_rand_006fc447() % rows;                                        // 006E4AAC..B4
        }
        Card* const out = static_cast<Card*>(out_cards10);
        for (int column = 0; column < 10; ++column) {                                 // 006E4AB7..0D
            (void)c2da_current_get_string_entry_00413190(table, nullptr, deck, column, text); // 006E4AC0..CE
            int value = static_cast<signed char>(c2da_current_string_index_005e5600(text, nullptr, 1)) - 0x31; // 006E4AD3..E7
            const char sign = c2da_current_string_index_005e5600(text, nullptr, 0);   // 006E4AE1..EA
            if (sign == '*') value += 12;                                             // 006E4AEF..F1 / 006E4AFC
            else if (sign == '-') value += 6;                                         // 006E4AF3..F7
            out[column].value = value;                                                // 006E4B01
            out[column].flag = 0;                                                     // 006E4B03
        }
        c2da_current_unload_004139e0(table, nullptr);                                 // 006E4B0F..13
    }
}

// ================================================================== 006E4B50 CPazaakPlayer::CPazaakPlayer() RET 0
void* __fastcall kotor_fastcall_006e4b50(void* self, void*) {
    Player* const p = player_of(self);
    for (int i = 0; i < 4; ++i) p->hand[i].flag = 0;                                  // 006E4B53..66
    for (int i = 0; i < 9; ++i) p->board[i].flag = 0;                                 // 006E4B68..76
    p->stood = 0;                                                                     // 006E4B78
    p->sets = 0;                                                                      // 006E4B7B
    for (int i = 0; i < 4; ++i) { p->hand[i].value = -1; p->hand[i].flag = 0; }       // 006E4B7E..A1
    for (int i = 0; i < 9; ++i) { p->board[i].value = -1; p->board[i].flag = 0; }     // 006E4BA4..EF
    return self;                                                                      // EAX = this
}

// ================================================================== 006E4C00 CSWPazaak::CSWPazaak() RET 0
// The deck card values are left as the allocation held them (only their flags are zeroed); deck_index -1 forces a
// shuffle before the first draw.
void* __fastcall kotor_fastcall_006e4c00(void* self, void*) noexcept(false) {   // h1: constructors are noexcept(false)
    Pazaak* const z = pazaak_of(self);
    kotor_fastcall_006e4b50(&z->human, nullptr);                                      // 006E4C03..06
    kotor_fastcall_006e4b50(&z->ai, nullptr);                                         // 006E4C0B..0E
    for (int i = 0; i < 40; ++i) z->deck[i].flag = 0;                                 // 006E4C13..2A
    z->wager = 0;                                                                     // 006E4C2C
    z->opponent = k_invalid;                                                          // 006E4C32
    z->deck_index = -1;                                                               // 006E4C39
    z->ai_state = 0;                                                                  // 006E4C43
    return self;                                                                      // 006E4C4D
}

// ================================================================== 006E4C60 CSWPazaak::PlayAITurn(CPazaakPlayer* ai, CPazaakPlayer* human, int state) RET 0Ch
// Returns the next AI state: 0 nothing to do, 1 drew a card, 2 played a side card, 3 stood, 4 turn over.
// Jump table 006E4E38: state 0 -> 006E4CB6, 1 -> 006E4CEA, 2 -> 006E4DDD, 3/4 -> 006E4CAA; above 4 (unsigned) -> 4.
// The original passes its own first-argument stack slot as FindPotentialSidecardTotal's index output; only the second
// call's output is read (the slot is reset to -1 just before it), so a local slot is equivalent.
std::int32_t __fastcall kotor_fastcall_006e4c60(void* self, void*, void* ai_arg, void* human_arg, std::int32_t state) {
    Player* const ai = player_of(ai_arg);
    Player* const human = player_of(human_arg);
    const std::uint32_t ai_word = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(ai));
    int slot = static_cast<int>(ai_word);                                             // [esp+20h] (first argument)
    const std::uint32_t slot_word = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(&slot));
    int human_total = kotor_fastcall_006e4360(human, nullptr);                        // 006E4C6D..79 [esp+10h]
    if (human_total > 20) {                                                           // 006E4C76..7D
        human_total = k_bust;                                                         // 006E4C7F..87
        human->stood = 1;                                                             // 006E4C8B (the opponent's flag)
    }
    if (static_cast<std::uint32_t>(state) > 4) return 4;                              // 006E4C96..99 ja -> 006E4E28
    auto play_card = [&](int index) -> std::int32_t {                                 // 006E4D7F..A7
        kotor_fastcall_006e45f0(ai, nullptr, ai->hand[index].value, ai->hand[index].flag);
        ai->hand[index].value = -1;                                                   // 006E4D8F..92
        ai->hand[index].flag = 0;                                                     // 006E4D95..97
        return 2;
    };
    auto stand_or_end = [&]() -> std::int32_t {                                       // 006E4DD5 (also state 2 at 006E4DDD)
        int total = kotor_fastcall_006e4360(ai, nullptr);                             // 006E4DDD..DF
        if (total > 20) total = k_bust;                                               // 006E4DE4..E9
        if (human->stood != 0 && total < human_total) return 4;                       // 006E4DEE..F7
        if (total >= 18) { ai->stood = 1; return 3; }                                 // 006E4DF9..FC / 006E4DFE
        if (kotor_fastcall_006e4910(self, nullptr, ai_word, slot_word, 1) >= 16) {    // 006E4E14..26
            ai->stood = 1;                                                            // 006E4DFE
            return 3;
        }
        return 4;                                                                     // 006E4E28
    };
    switch (state) {                                                                  // 006E4CA3
    case 0: {                                                                         // 006E4CB6
        if (ai->stood != 0) return 0;                                                 // 006E4CBA..BF -> 006E4CAA
        Card card;                                                                    // [esp+14h]
        Card* const drawn = static_cast<Card*>(kotor_fastcall_006e4800(self, nullptr, &card)); // 006E4CC1..C8
        kotor_fastcall_006e45f0(ai, nullptr, drawn->value, drawn->flag);              // 006E4CCD..D6
        return 1;                                                                     // 006E4CDE
    }
    case 1: {                                                                         // 006E4CEA
        const int ai_total = kotor_fastcall_006e4360(ai, nullptr);                    // 006E4CEA..EC [esp+14h] (raw)
        int may_wait = 1;                                                             // 006E4CF8..01 [esp+28h]
        if (human->sets + 1 >= 3) may_wait = 0;                                       // 006E4CF1..07
        if (human->stood != 0 && ai_total > human_total && ai_total <= 20) {          // 006E4D0F..1D
            ai->stood = 1;                                                            // 006E4D20
            return 3;
        }
        const int lowest = kotor_fastcall_006e4910(self, nullptr, ai_word, slot_word, 1);  // 006E4D31..3A ebx
        slot = -1;                                                                    // 006E4D4B
        const int highest = kotor_fastcall_006e4910(self, nullptr, ai_word, slot_word, 0); // 006E4D3F..53 eax
        const int index = slot;                                                       // 006E4D5B edi
        if (highest >= 18 && highest >= human_total) {                                // 006E4D58..65
            bool play = false;
            if (highest >= 20) play = true;                                           // 006E4D67..6A
            else if (highest >= 19 && lowest > 11) play = true;                       // 006E4D6C..74
            else if (lowest > 13) play = true;                                        // 006E4D76..79
            if (play && index >= 0) return play_card(index);                          // 006E4D7B..7D
        }
        if (ai_total > 20) {                                                          // 006E4DAA..AF
            if (may_wait != 0 && human_total >= 18 && highest < human_total) return 4; // 006E4DB1..C4
            if (index >= 0) return play_card(index);                                  // 006E4DC6..D3
        }
        return stand_or_end();                                                        // 006E4DD5
    }
    case 2:
        return stand_or_end();                                                        // 006E4DDD
    default:
        return 0;                                                                     // 006E4CAA (states 3, 4)
    }
}

// ================================================================== 0067CAE0 CSWPazaak::DrawPlayerCard() RET 0
void __fastcall kotor_fastcall_0067cae0(void* self, void*) {
    Card card;                                                                        // [esp+4]
    Card* const drawn = static_cast<Card*>(kotor_fastcall_006e4800(self, nullptr, &card)); // 0067CAE4..EB
    kotor_fastcall_006e45f0(&pazaak_of(self)->human, nullptr, drawn->value, drawn->flag);  // 0067CAF0..FA
}

// ================================================================== 0067CB10 CSWPazaak::DoAITurn() RET 0 (EAX = new state)
std::int32_t __fastcall kotor_fastcall_0067cb10(void* self, void*) {
    Pazaak* const z = pazaak_of(self);
    const std::int32_t state = kotor_fastcall_006e4c60(self, nullptr, &z->ai, &z->human, z->ai_state); // 0067CB13..24
    z->ai_state = state;                                                              // 0067CB29
    return state;                                                                     // EAX
}

}  // extern "C"
