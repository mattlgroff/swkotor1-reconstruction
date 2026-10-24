// Lane PZ-A tier a1: Pazaak entry and exit - VM routine 364 PlayPazaak (00540FD0), CClientExoApp thunks
// (005EDDC0 / 005EDDD0) and CClientExoAppInternal::StartPazaakGame / EndPazaakGame (005F3810 / 005F3950),
// reconstructed from the original swkotor.exe (GOG 1.03) disassembly, read-only. Original PCs on every line.
// Recipe: W5 isolated /EHs (00540FD0 and 005F3810 each have one original unwind state).
// Globals are bound by the reconstruction's symbols; no original address is dereferenced.
#include "4b9fe8db02e2-claude_pazaak_pza.hpp"
#include <cstdint>

namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }  // [007A39FC] CAppManager* [0053]
namespace frame_control {
extern std::uint32_t message_manager_007a3a00;                                  // [007A3A00] CVirtualMachine* [0456]
extern std::uint32_t sound_manager_007a39ec;                                    // [007A39EC] CExoSound* [0673]
}
namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String* s, void*); } // CExoString() [menu20_bindings]

extern "C" {
extern const std::uint32_t kotor_rdata_sound_mode_0074c5ec;                     // [0074C5EC] = 0 (.rdata) [0673]
extern const std::uint32_t kotor_rdata_sound_mode_0074c5fc;                     // [0074C5FC] = 4 (.rdata) [0673]
void* __cdecl live_operator_new(unsigned size);                                 // operator new 006FA7E6 (game heap) [0004]
void __cdecl live_free(void* memory);                                           // free 006FA390 (game heap) [0004]
void* __fastcall kotor_fastcall_005ed8b0(void* client, void*);                  // CClientExoApp::GetSWParty [0707]
void* __fastcall kotor_fastcall_00635460(void* party, void*);                   // CSWParty::GetPlayerCharacter [0767]
std::uint32_t __fastcall kotor_fastcall_004aea30(void* server, void*, std::uint32_t id);  // ClientToServerObjectId RET 4 [0595]
void* __fastcall kotor_fastcall_004ae770(void* server, void*, std::uint32_t id);  // GetCreatureByGameObjectID RET 4 [0595]
unsigned __fastcall w2x_creature_gold_004edd60(void* creature, void*);          // CSWSCreature::GetGold [0886]
void __fastcall kotor_fastcall_004edd90(void* creature, void*, std::int32_t gold); // CSWSCreature::SetGold RET 4 [claude_w1_route_bodies]
void* __fastcall frame_R4_005ed690(void* client, void*);                        // CClientExoApp::GetInGameGui [0457]
void __fastcall kotor_fastcall_0062eeb0(void* in_game, void*, std::uint8_t kind, std::uint32_t value); // CGuiInGame::UpdateStatus RET 8 [src_hud_main_interface]
void* __fastcall async_assign_string_005e5c50(void* s, void*, void* other);     // CExoString::operator= RET 4 [0199]
char __fastcall c2da_current_string_index_005e5600(void* s, void*, int index);  // CExoString::operator[] RET 4 [0288]
void __fastcall frame_LogStringDtor_005e5c20(void* s, void*);                   // CExoString::~CExoString [0459]
void __fastcall frame_R4_005d5e80(void* sound, void*, std::uint32_t mode);      // CExoSound::SetSoundMode RET 4 [0709]
void __fastcall kotor_fastcall_006200e0(void* internal, void*, int input_class, int flag); // SetInputClass RET 8 [0298]
std::uint32_t __fastcall kotor_fastcall_0040bc70(void* manager, void*, void* panel, int layer, int flag); // CSWGuiManager::AddPanel RET 0Ch [0298]
void __fastcall kotor_fastcall_0040a140(void* manager, void*, int sound);       // CSWGuiManager::PlayGuiSound RET 4 [1205-30]
std::uint32_t __fastcall frame_R4_005d0fc0(void* vm, void*, void* script, std::uint32_t self_id, int flag); // CVirtualMachine::RunScript RET 0Ch [load_screen_thin]
std::uint32_t __fastcall recovered_vm_outer_pop_integer_005d1000(void* vm, void*, std::int32_t* out); // StackPopInteger RET 4 [vm_actions_t1]
std::uint32_t __fastcall recovered_vm_outer_pop_string_005d1080(void* vm, void*, void* out);          // StackPopString RET 4 [vm_actions_t1]
std::uint32_t __fastcall recovered_vm_outer_pop_object_005d10c0(void* vm, void*, std::uint32_t* out); // StackPopObject RET 4 [1196]
}

namespace {
constexpr std::int32_t k_pop_failed = -2001;          // 0xFFFFF82F
constexpr std::uint32_t k_invalid = 0x7f000000u;      // OBJECT_INVALID
constexpr unsigned k_start_panel_size = 0x8928;       // CSWGuiPazaakStart (005F38C0 push 8928h)
inline unsigned char* bytes(void* p) { return static_cast<unsigned char*>(p); }
template <class T> inline T& at(void* p, unsigned offset) { return *reinterpret_cast<T*>(bytes(p) + offset); }
inline void* app_manager() { return kotor_menu_window_cells::manager_007a39fc; }   // re-read at every use
inline void* client_app() { return at<void*>(app_manager(), 4); }                  // [[007A39FC]+4]
inline void* server_app() { return at<void*>(app_manager(), 8); }                  // [[007A39FC]+8]
inline void* vm() { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_control::message_manager_007a3a00)); }
inline void* sound() { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_control::sound_manager_007a39ec)); }
// CClientExoAppInternal fields (constructed by 005FBFE0, +74 destroyed by 005FC6F0)
constexpr unsigned k_running = 0x70, k_end_script = 0x74, k_wager = 0x7c, k_result = 0x80, k_deck = 0x84, k_gui = 0x274;
// Player creature: GetCreatureByGameObjectID(ClientToServerObjectId(GetSWParty()->GetPlayerCharacter()->[+4]))
// (005F383C..71 / 005F3976..A7). The client player object is not null-checked (original).
void* player_creature() {
    void* const party = kotor_fastcall_005ed8b0(client_app(), nullptr);
    void* const pc = kotor_fastcall_00635460(party, nullptr);
    const std::uint32_t id = kotor_fastcall_004aea30(server_app(), nullptr, at<std::uint32_t>(pc, 4));
    return kotor_fastcall_004ae770(server_app(), nullptr, id);
}
struct StringGuard {                                                               // one CExoString unwind state
    void* s;
    ~StringGuard() { frame_LogStringDtor_005e5c20(s, nullptr); }
};
}  // namespace

extern "C" {

// ================================================================== 005F3810 CClientExoAppInternal::StartPazaakGame RET 14h
// (int deck, CExoString* end_script, int max_wager, int tutorial, OBJECT_ID opponent)
void __fastcall kotor_fastcall_005f3810(void* self, void*, std::int32_t deck, void* end_script, std::int32_t max_wager,
                                        std::int32_t tutorial, std::uint32_t opponent) {
    if (at<std::int32_t>(self, k_running) != 0) return;                           // 005F3829..2F
    at<std::int32_t>(self, k_running) = 1;                                        // 005F3835
    void* const creature = player_creature();                                     // 005F383C..71
    std::int32_t wager = max_wager;                                               // 005F3873 edi
    if (creature != nullptr && wager > 0) {                                       // 005F3871..7B (jle)
        const std::int32_t gold = static_cast<std::int32_t>(w2x_creature_gold_004edd60(creature, nullptr)); // 005F387D..7F
        if (!(wager < gold)) wager = gold;                                        // 005F3884..88 (jl, signed)
    }
    at<std::int32_t>(self, k_result) = 0;                                         // 005F3892
    at<std::int32_t>(self, k_wager) = wager;                                      // 005F389C
    (void)async_assign_string_005e5c50(bytes(self) + k_end_script, nullptr, end_script); // 005F388A..9F
    at<std::int32_t>(self, k_deck) = deck;                                        // 005F38A4..A8
    frame_R4_005d5e80(sound(), nullptr, kotor_rdata_sound_mode_0074c5fc);         // 005F38AE..BB
    void* const memory = live_operator_new(k_start_panel_size);                   // 005F38C0..C5
    void* panel = nullptr;                                                        // 005F38FB
    if (memory != nullptr) {                                                      // 005F38D1..DB
        try {                                                                     // EH state 0: free the allocation (00725E10)
            panel = kotor_fastcall_00681a90(memory, nullptr, at<void*>(self, k_gui), wager, deck, tutorial, opponent); // 005F38DD..F2
        } catch (...) {
            live_free(memory);
            throw;
        }
    }
    kotor_fastcall_006200e0(self, nullptr, 2, 1);                                 // 005F38FD..0B
    (void)kotor_fastcall_0040bc70(at<void*>(self, k_gui), nullptr, panel, 3, 1);  // 005F3910..1B
    kotor_fastcall_0040a140(at<void*>(self, k_gui), nullptr, 4);                  // 005F3920..28
}

// ================================================================== 005F3950 CClientExoAppInternal::EndPazaakGame RET 10h
// (int won, int wager, int restart, OBJECT_ID opponent)
void __fastcall kotor_fastcall_005f3950(void* self, void*, std::int32_t won, std::int32_t wager, std::int32_t restart,
                                        std::uint32_t opponent) {
    at<std::int32_t>(self, k_result) = won;                                       // 005F3961
    at<std::int32_t>(self, k_wager) = wager;                                      // 005F3967
    kotor_fastcall_006200e0(self, nullptr, 0, 1);                                 // 005F395D..6A
    at<std::int32_t>(self, k_running) = 0;                                        // 005F396F
    void* const creature = player_creature();                                     // 005F3976..AC
    if (creature != nullptr && wager > 0) {                                       // 005F39AE..B4 (jle)
        const std::uint32_t gold = w2x_creature_gold_004edd60(creature, nullptr); // 005F39B7..BE ebp
        std::uint32_t updated;
        if (won == 0) {                                                           // 005F39C0..C6
            const std::uint32_t remaining = gold - static_cast<std::uint32_t>(wager); // 005F39C8..CA
            updated = static_cast<std::int32_t>(remaining) <= 0 ? 0u : remaining; // 005F39CC..D6 setle/dec/and
        } else {
            updated = static_cast<std::uint32_t>(wager) + gold;                   // 005F39DA
        }
        if (updated != gold) {                                                    // 005F39DC..DE
            kotor_fastcall_004edd90(creature, nullptr, static_cast<std::int32_t>(updated)); // 005F39E0..E3
            void* const in_game = frame_R4_005ed690(client_app(), nullptr);       // 005F39E8..F5
            kotor_fastcall_0062eeb0(in_game, nullptr, 1, updated - gold);         // 005F39F0..FC
        }
    }
    if (restart != 0) {                                                           // 005F3A02..08
        kotor_fastcall_005f3810(self, nullptr, at<std::int32_t>(self, k_deck), bytes(self) + k_end_script, 0, 1,
                                opponent);                                        // 005F3A0A..20
        return;
    }
    if (vm() != nullptr &&                                                        // 005F3A2B..32
        c2da_current_string_index_005e5600(bytes(self) + k_end_script, nullptr, 0) != 0) { // 005F3A34..42
        (void)frame_R4_005d0fc0(vm(), nullptr, bytes(self) + k_end_script, k_invalid, 0); // 005F3A44..52 (no owner)
    }
    kotor_fastcall_0040a140(at<void*>(self, k_gui), nullptr, 5);                  // 005F3A57..5F
    frame_R4_005d5e80(sound(), nullptr, kotor_rdata_sound_mode_0074c5ec);         // 005F3A64..71
}

// ================================================================== 005EDDC0 CClientExoApp::StartPazaakGame RET 14h
void __fastcall kotor_fastcall_005eddc0(void* self, void*, std::int32_t deck, void* end_script, std::int32_t max_wager,
                                        std::int32_t tutorial, std::uint32_t opponent) {
    kotor_fastcall_005f3810(at<void*>(self, 4), nullptr, deck, end_script, max_wager, tutorial, opponent); // mov ecx,[ecx+4]; jmp
}

// ================================================================== 005EDDD0 CClientExoApp::EndPazaakGame RET 10h
void __fastcall kotor_fastcall_005eddd0(void* self, void*, std::int32_t won, std::int32_t wager, std::int32_t restart,
                                        std::uint32_t opponent) {
    kotor_fastcall_005f3950(at<void*>(self, 4), nullptr, won, wager, restart, opponent);  // mov ecx,[ecx+4]; jmp
}

// ================================================================== 00540FD0 ExecuteCommandPlayPazaak (routine 364) RET 8
// PlayPazaak(int nOpponentPazaakDeck, string sEndScript, int nMaxWager, int bShowTutorial = FALSE,
//            object oOpponent = OBJECT_INVALID). Pop order deck, end script, max wager, [tutorial], [opponent].
int __fastcall kotor_fastcall_00540fd0(void*, void*, int, unsigned argc) {
    std::int32_t deck = 0;                                                        // [esp+0Ch]
    if (recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &deck) == 0) return k_pop_failed; // 00540FEE..FA
    alignas(4) unsigned char end_script[8];                                       // CExoString [esp+10h]
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(end_script), nullptr);          // 00540FFC..5410005
    StringGuard guard{end_script};                                                // EH state 0 (0071D250)
    std::int32_t max_wager = 0;                                                   // [esp+8]
    if (recovered_vm_outer_pop_string_005d1080(vm(), nullptr, end_script) == 0 ||   // 00541005..1F
        recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &max_wager) == 0) // 00541021..33
        return k_pop_failed;                                                      // 00541035..46
    std::int32_t tutorial = 0;                                                    // 00541064
    if (static_cast<int>(argc) >= 4 &&                                            // 0054105D..6C (jl, signed)
        recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, &tutorial) == 0)  // 0054106E..80
        return k_pop_failed;                                                      // 00541082..93
    std::uint32_t opponent = k_invalid;                                           // 005410AD
    if (static_cast<int>(argc) >= 5 &&                                            // 005410AA..B5
        recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &opponent) == 0)   // 005410B7..C9
        return k_pop_failed;
    kotor_fastcall_005eddc0(client_app(), nullptr, deck, end_script, max_wager, tutorial != 0 ? 1 : 0,
                            opponent);                                            // 005410CB..F4
    return 0;                                                                     // 005410F9..0E (string destroyed)
}

}  // extern "C"
