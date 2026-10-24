// lane-m48-traps fork f-input: CClientExoAppInternal::ProcessInput 006227E0..00624454 (whole function), adapted from the
// lane-input r3 donor (lane-input/src/process_input.cpp, sha256 in ../donor/SHA256SUMS.txt; every retained line keeps the
// donor's original-address citations). D83 adaptation: the branches proven unreachable under menu state (see
// ../FORK.json traps[] for the gate cells, initial values and all writers) are replaced by calls to the shared fatal trap
// kotor_unreachable_trap(<first instruction of the unreachable region>, ...). No other behaviour is changed.
//  T-PI-1 00622872 PlayBackInputEvents     gate client+90h (event playback), only set by SetUpEventPlayBack ('-epf' argv)
//  T-PI-2 00622C18 RestoreCamera branch    gate CClientOptions+6Dh == 5 (camera mode; 3 on the menu)
//  T-PI-3 00622D09 world picking           gate CSWCModule+40h scene != 0 (0 until CSWCModule::LoadModule)
//  T-PI-4 006236BF hover reset             gate CGuiInGame+90h main interface != 0 (0 until CGuiInGame::Initialize)
//  T-PI-5 0062388A drive/camera keys       gate client+2A0h drive controller != 0 (0 until AttachPlayerGameObjectToArea)
//  T-PI-6 00623F2E drive update            gate GetPlayerCreature() != 0 (client+20h = 7F000000 on the menu)
//  T-PI-7 00623F81 mini-game input         gate CSWCModule+48h area != 0 (0 until CSWCModule::LoadModule)
// Also here: GetControlledCharacterDead 005F2ED0 (+ trap T-GCD-1 005F2EEA), the CExoArrayList<CExoInputEvent> helpers
// 0061FB50 / 005E0A30 / 00620480, the D83 function trap for CClientExoApp::setUpEventPlayBack 005ED830, the frame entry
// ABI adapter frame_R4_006227e0 (no address credit) and the cells 008338F0 / 008338F8.
#include "2ec18fd9a2-process_input.h"
#include "0e7d509bda-input_system.h"
#include "3509f7d710-process_input_consts.h"
#include "eddb76278d-allocator_live.h"
#include "36201186c4-window_shared_cells.h"
#include "fd249c55aa-kotor_unreachable_trap.h"
#include <cstring>

using namespace kotor_input_hover;
using namespace kotor_process_input;
namespace req = kotor_input_required;
namespace ids = kotor_process_input_ids;
using kotor_reconstruction::StartupString;

// ---- cells 008338F0 / 008338F8 (.bss words, PE initial value 0) ----
// 008338F0 mouse-look toggle: writers 00621AF2 (=0, quick load), 00621CE6 (= value, events DB/DC); readers 0062282B,
//          00623A37, 0061F8C6, 00620576. 008338F8 action-menu modifier: writer 00621CFF (events DD/DE).
namespace kotor_input_cells_required {
volatile std::int32_t word_008338f0 = 0;
volatile std::int32_t word_008338f8 = 0;
}
namespace cell = kotor_input_cells_required;

// ---- 005ED460 / 005ED470 CClientExoApp::GetCaptureEventFile / GetCaptureEvents (owner: sibling fork f-keys) ----
extern "C" void* __fastcall kotor_fastcall_005ed460(void* app, void*);
extern "C" int __fastcall kotor_fastcall_005ed470(void* app, void*);
// ---- in-game providers (claude-build: closes T-GCD-1, T-PI-2, T-PI-4) ----
extern "C" void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void* client_creature, void*);  // CSWCCreature::GetServerCreature (0816)
extern "C" int __fastcall recovered_creature_incapacitated_004ef890(void* creature, void*);                  // CSWSCreature::GetDeadTemp (0963)
extern "C" void __fastcall kotor_fastcall_00641dc0(void* module, void*);                                     // CSWCModule::RestoreCamera
extern "C" void __fastcall kotor_fastcall_006200e0(void* client, void*, int input_class, int flag);           // SetInputClass, RET 8
extern "C" void __fastcall kotor_fastcall_006222f0(void* client, void*, U32 id, const void* position, U32 flag);  // HandleMouseOver, RET 0Ch (W5 picking)
namespace kotor_input_cells_required { extern volatile int camera_restore_007a2288; }                          // [007A2288] (W1 b7)

extern "C" double __fastcall kotor_fastcall_005df4d0(void*,void*,U32 event,U32 input_class) noexcept(false);
extern "C" unsigned __fastcall kotor_fastcall_005f2df0(void*,void*) noexcept(false);
extern "C" void __fastcall kotor_fastcall_00670fb0(void*,void*,void* axis,float delta) noexcept(false);
extern "C" void __fastcall kotor_fastcall_00671020(void*,void*,float x,float y,float delta) noexcept(false);
extern "C" void __fastcall kotor_fastcall_006710e0(void*,void*,float delta) noexcept(false);

namespace {
float f32(U32 bits) { float f; std::memcpy(&f, &bits, 4); return f; }
template<class T> T at(const void* base, U32 off) { T v; std::memcpy(&v, static_cast<const unsigned char*>(base) + off, sizeof(T)); return v; }
template<class T> void put(void* base, U32 off, T v) { std::memcpy(static_cast<unsigned char*>(base) + off, &v, sizeof(T)); }
ClientApp* app() { return static_cast<AppManager*>(kotor_menu_window_cells::manager_007a39fc)->client; }  // *(007A39FC)+4
Input* input() { return static_cast<Input*>(kotor_menu_window_cells::input_007a39e4); }                   // *(007A39E4)
OptionsView* options() { return reinterpret_cast<OptionsView*>(app()->GetClientOptions()); }             // 005ED700
bool fl_less(double a,double b) { return a < b; }
bool fl_c0_or_c3(double a,double b) { return !(a > b); }
// input-a2 (INPUT-PRECISION-PLAN): the two minigame poll sequences keep the original x87 data flow. The polled sum and the
// mouse quotient/product stay in the x87 register (no 64-bit spill, no SSE reciprocal) until the original FSTP32.
// Ordinary MSVC inline-assembly helpers (not naked, normal prologue/nonvolatile handling, noexcept(false)). The input cell
// [007A39E4] is reloaded before every poll, as the original does; PollInput 005DF4D0 is the selected typed double export
// (fastcall: ECX = input, EDX unused, stack = event, class; RET 8; result in ST0). Constants are the original words
// (-1.0f [00740CBC], 1.0f [0073D764], 20.0f [007A232C], which the original only ever reads).
//
// Axis pair, original 00623FAA..0062409D (Type2 x), 0062409D..00624190 (Type2 y), 006242FD..006243E8 (Type1):
//   each test = poll(first) FSTP32 s; poll(second) FADD s; then
//   T1: FCOMP -1; FNSTSW; TEST AH,5; JNP -> T3            (ordered sum < -1 goes to T3)
//   T2: FLD 1; FCOMP ST(1); FNSTSW; FSTP ST(0); TEST AH,5; JP -> T3; result = 1.0
//   T3: FCOMP -1; FNSTSW; TEST AH,5; JP -> T4; result = -1.0
//   T4: FSTP32 result (the unclamped sum, rounded once)
// Every test polls again (no caching), exactly 2..6 polls per axis as the original.
__declspec(noinline) void axis_pair_00623faa(U32 first_event, U32 second_event, float* result) noexcept(false) {
    void* volatile* const cell = &kotor_menu_window_cells::input_007a39e4;
    const float minus_one = f32(ids::minus_one_00740cbc), one = f32(ids::one_0073d764);
    float first;                                                        // original scratch [esp+18h]
    __asm {
        mov eax, cell
        mov ecx, [eax]
        push 1
        push first_event
        call kotor_fastcall_005df4d0
        fstp dword ptr first
        mov eax, cell
        mov ecx, [eax]
        push 1
        push second_event
        call kotor_fastcall_005df4d0
        fadd dword ptr first
        fcomp dword ptr minus_one
        fnstsw ax
        test ah, 5
        jnp test3
        mov eax, cell
        mov ecx, [eax]
        push 1
        push first_event
        call kotor_fastcall_005df4d0
        fstp dword ptr first
        mov eax, cell
        mov ecx, [eax]
        push 1
        push second_event
        call kotor_fastcall_005df4d0
        fadd dword ptr first
        fld dword ptr one
        fcomp st(1)
        fnstsw ax
        fstp st(0)
        test ah, 5
        jp test3
        mov eax, result
        mov dword ptr [eax], 0x3F800000
        jmp done
    test3:
        mov eax, cell
        mov ecx, [eax]
        push 1
        push first_event
        call kotor_fastcall_005df4d0
        fstp dword ptr first
        mov eax, cell
        mov ecx, [eax]
        push 1
        push second_event
        call kotor_fastcall_005df4d0
        fadd dword ptr first
        fcomp dword ptr minus_one
        fnstsw ax
        test ah, 5
        jp test4
        mov eax, result
        mov dword ptr [eax], 0xBF800000
        jmp done
    test4:
        mov eax, cell
        mov ecx, [eax]
        push 1
        push first_event
        call kotor_fastcall_005df4d0
        fstp dword ptr first
        mov eax, cell
        mov ecx, [eax]
        push 1
        push second_event
        call kotor_fastcall_005df4d0
        fadd dword ptr first
        mov eax, result
        fstp dword ptr [eax]
    done:
    }
}
// Mouse, original 006241E2..00624218: poll(17h) FDIV 20 -> FSTP32 x; poll(18h); FLD -1; FDIV 20; FMULP ST(1) -> FSTP32 y.
__declspec(noinline) void mouse_pair_006241e2(float* x, float* y) noexcept(false) {
    void* volatile* const cell = &kotor_menu_window_cells::input_007a39e4;
    const float minus_one = f32(ids::minus_one_00740cbc), divisor = f32(ids::mouse_divisor_007a232c);
    __asm {
        mov eax, cell
        mov ecx, [eax]
        push 1
        push 0x17
        call kotor_fastcall_005df4d0
        fdiv dword ptr divisor
        mov eax, x
        fstp dword ptr [eax]
        mov eax, cell
        mov ecx, [eax]
        push 1
        push 0x18
        call kotor_fastcall_005df4d0
        fld dword ptr minus_one
        fdiv dword ptr divisor
        fmulp st(1), st
        mov eax, y
        fstp dword ptr [eax]
    }
}
void* module_of(ClientInternal* self) { return self->get<void*>(0x18); }                                // 00644760 GetProperty0x18 (mov eax,[ecx+18h])
U32 snapshot_seconds_div() {                                       // GetWorldTimer / GetSnapshotTime / __aulldiv(.,1000)
    void* timer = req::get_world_timer_005ed4f0(app(), nullptr);
    return static_cast<U32>(kotor_menu_10_19::dependencies::unsigned_divide_006fb030(req::get_snapshot_time_004adc70(timer, nullptr), 1000u));
}
}

// ======== 0061FB50 CExoArrayList<CExoInputEvent>::delete(int index), thiscall RET 4 ========
void __fastcall kotor_input_required::event_list_delete_0061fb50(InputEventList* list, void*, int index) {
    list->count = list->count - 1;                                 // 0061FB51..57 (count decremented first)
    for (int i = index; i < list->count; ++i)                      // 0061FB5E..60 / 0061FB8D..96 (count reloaded)
        list->data[i] = list->data[i + 1];                         // 0061FB70..8A (4 dwords from element i+1)
}

// ======== 005E0A30 CExoArrayList<CExoInputEvent>::Add(CExoInputEvent), thiscall RET 10h ========
void __fastcall kotor_input_required::event_list_add_005e0a30(InputEventList* list, void*, InputEvent e) {
    if (list->count == list->capacity)                             // 005E0A33..39
        EventListAllocate005e0410(list, nullptr, list->capacity != 0 ? list->capacity * 2 : 0x10);  // 005E0A3B..4B
    list->data[list->count] = e;                                   // 005E0A50..72
    ++list->count;                                                 // 005E0A75
}

// ======== 00620480 CExoArrayList<CExoInputEvent>::Insert(CExoInputEvent, int index), thiscall RET 14h ========
// Observed order: append at the old end, then shift (old_count..index+1) up by one, then store at index.
void __fastcall kotor_input_required::event_list_insert_00620480(InputEventList* list, void*, InputEvent e, int index) {
    int old_count = list->count;                                   // 00620487 (EDI)
    if (old_count == list->capacity)                               // 00620483..8C
        EventListAllocate005e0410(list, nullptr, list->capacity != 0 ? list->capacity * 2 : 0x10);  // 0062048E..9E
    list->data[list->count] = e;                                   // 006204A3..C5
    ++list->count;                                                 // 006204C8
    for (; old_count > index; --old_count)                         // 006204CB..D1 / 006204FD..01
        list->data[old_count] = list->data[old_count - 1];         // 006204E0..FE
    list->data[index] = e;                                         // 00620505..25
}

// ======== 005F2ED0 CClientExoAppInternal::GetControlledCharacterDead (thiscall, RET; ECX unused) ========
// GetCharacter(GetSWParty(), 0) -> GetServerCreature -> tail JMP CSWSCreature::GetDeadTemp 004EF890.
// T-GCD-1 005F2EEA: reached only with a party member 0 (CSWParty count [party+0] == 0 on the menu: ctor 00636471;
// the only increment is CSWParty::AddCharacter 0063672F, callers SetPlayerCreature / UnloadModule / CrownFutureLeader /
// CSWCCreature::SetInParty - all module/creature paths).
extern "C" int __fastcall kotor_fastcall_005f2ed0(void* self, void*) {
    (void)self;
    void* party = req::get_sw_party_005ed8b0(app(), nullptr);      // 005F2ED0..DA (push 0 at 005F2ED8)
    void* member = req::get_character_006346c0(party, nullptr, 0); // 005F2EDF..E1
    if (member == nullptr) return 0;                               // 005F2EE6..E8 -> 005F2EFC
    void* creature = kotor_m15a_creature_get_server_creature_0060fb20(member, nullptr);  // 005F2EEA..EC
    if (creature == nullptr) return 0;                             // 005F2EF1..F3 -> 005F2EFC
    return recovered_creature_incapacitated_004ef890(creature, nullptr);  // 005F2EF5..F7 (tail JMP 004EF890)
}

// ======== 005ED830 CClientExoApp::setUpEventPlayBack (D83 FUNCTION TRAP) ========
// Original: mov ecx,[ecx+4]; jmp 005F5910 SetUpEventPlayBack (event playback file parser 0061FBA0 + playback 006224E0).
// Only caller: checkCommandLineArgs 004017E8, reached only when an argv element matches "-epf" (_strncmp 004017A7,
// literal 0073D5F8). The run configuration launches without arguments.
extern "C" void __fastcall kotor_fastcall_005ed830(void* app, void*, void* name) {
    (void)app; (void)name;
    kotor_unreachable_trap(0x005ED830ul, "CClientExoApp::setUpEventPlayBack@005ED830 (-epf event playback)");
}

// ======== 005E30E0 CExoRawInputInternal::GetJoystickBuffer (D83 FUNCTION TRAP) ----
// Only caller: CExoInputInternal::GetEvents 005E2603..3C, loop d = 2 .. device_count-1 with device_count =
// [raw+18h] + 2 (005E2234..3A). [raw+18h] (joystick count) is written only by CExoRawInputInternal ctor 005E3F6D (= 0);
// no other store to raw+18h exists in 005E2000..005E5000 (tools/writers.py), so the loop body never runs.
int RawInput::GetJoystickBuffer(U32 joystick, DeviceBuffer* out) {
    (void)joystick; (void)out;
    kotor_unreachable_trap(0x005E30E0ul, "CExoRawInputInternal::GetJoystickBuffer@005E30E0 (joystick count is always 0)");
}

// ======== frame entry ABI adapter (consumer frame-gui/source/entry_providers.h:8) â€” no address credit ========
extern "C" void __fastcall frame_R4_006227e0(void* self, void*, float delta) {
    static_cast<ClientInternal*>(self)->ProcessInput(delta);
}

extern "C" void __fastcall w5_process_input_drive_0062388a(void* client_internal, void*, float dt);   // W5: 0062388A..00623F1F
extern "C" void __fastcall w5_process_input_picking_00622d09(void* client_internal, void*);   // W5 t3: 00622D09..006236FD
void ClientInternal::ProcessInput(float delta) {
    (void)delta;                                                           // [esp+134h]
    StartupString text88;                                                  // [esp+88h] 0062280E
    int client_result = 0;                                                 // [esp+2Ch] 0062280A
    if (get<U32>(0x2BC) != 0) return;                                      // 00622813..20 -> 00624425
    int mouse_look = cell::word_008338f0;                                  // [esp+48h] 0062282B..34
    {
        OptionsView* o = options();                                        // 00622838
        const U32 mode = get<U32>(0x9C);
        if ((o->flags08 & 2) && mode != 2 && mode != 3)                    // 0062283D..51
            mouse_look = (cell::word_008338f0 == 0) ? 1 : 0;               // 00622853..5C (EDI reused)
    }
    if (req::get_capture_event_play_back_005ed480(app(), nullptr)) {      // 00622860..70
        // T-PI-1: client+90h is 0 (ctor 005FC313); its only non-zero writer is SetUpEventPlayBack 005F5942/005F594A,
        // reached only through 005ED830 (D83 trap above, argv "-epf").
        kotor_unreachable_trap(0x00622872ul, "CClientExoAppInternal::ProcessInput@00622872 PlayBackInputEvents (event playback)");
    }
    {
        const int diff = req::get_snapshot_time_difference_004adc80(get<void*>(0x2C), nullptr);  // 0062287E..81
        const float scaled = static_cast<float>(static_cast<double>(diff) * f32(ids::timer_scale_007439bc));  // 00622890..9A
        input()->UpdateMouseDelta(scaled);                                 // 006228A2..A3
    }
    if (input()->GetActive()) {                                            // 006228A8..B5
        int count = 0;                                                     // [esp+28h]
        InputEventList list = input()->GetEvents(&count, static_cast<int>(get<U32>(0x9C)));  // 006228BB..D2 ([esp+64h])
        {   // ---- event reordering pass (006228D7..00622B0D) ----
            const U32 mode = get<U32>(0x9C);
            if (at<int>(get<void*>(0x40), 0x34) == 1 || mode == 2 || mode == 3) {   // 006228D7..F6
                int seen_gui_button = 0;                                   // [esp+20h]
                for (int i = 0; i < count; ++i) {                          // [esp+10h], offsets [esp+14h]/EDI
                    InputEvent* ev = &list.data[i];
                    if (ev->value == 0) continue;                          // 00622920..28
                    const unsigned char n = get<unsigned char>(0x414);     // 0062292E
                    if (n < 10) {                                          // 00622934..37
                        set<int>(0x3EC + n * 4u, ev->desc->id);            // 00622939..45
                        set<unsigned char>(0x414, static_cast<unsigned char>(n + 1));  // 0062294C
                    } else {
                        for (U32 k = 0; k < 9; ++k) set<U32>(0x3EC + k * 4, get<U32>(0x3F0 + k * 4));  // 00622954..BE (shift down)
                        set<int>(0x410, list.data[i].desc->id);            // 006229B4..CB
                    }
                    ev = &list.data[i];                                    // 006229D1..D5 (EBP)
                    const int id = ev->desc->id;                           // 006229D8..DB
                    const U32 idx = static_cast<U32>(id - 1);              // 006229DE
                    // table 00624460: indices 0,1,9,10,0x26..0x2D,0x34,0x35 -> 006229F2
                    const bool gui_button = idx <= 0x35 && (idx <= 1 || idx == 9 || idx == 10 || (idx >= 0x26 && idx <= 0x2D) || idx >= 0x34);
                    if (gui_button) {
                        if (seen_gui_button) {                             // 006229F2..F8
                            req::event_list_delete_0061fb50(&list, nullptr, i);  // 006229FA..A03
                            --i; --count;                                  // 00622A10..20
                            continue;
                        }
                        seen_gui_button = 1;                               // 00622A29
                    }
                    const int id2 = ev->desc->id;                          // 00622A31 (reloaded)
                    if (static_cast<U32>(id2) < 0x31u || static_cast<U32>(id2) > 0x32u) continue;  // 00622A34..40 (unsigned)
                    int insert_at = -1;                                    // EBX 00622A4E
                    for (int j = i + 1; j < count; ++j) {                  // 00622A46..86
                        const InputEvent* p = &list.data[j];
                        if (p->value != 0) {
                            const U32 pid = static_cast<U32>(p->desc->id);
                            if (pid >= 0x2Fu && pid <= 0x30u) insert_at = j + 1;  // 00622A69..79 (LEA EBX,[EDX+1])
                        }
                    }
                    if (insert_at == -1) continue;                         // 00622A88..8B
                    const InputEvent copy = *ev;                           // 00622A8D..AE (by-value copy)
                    if (insert_at >= list.count)                           // 00622A8D..99 (list count field)
                        req::event_list_add_005e0a30(&list, nullptr, copy);              // 00622AB5
                    else
                        req::event_list_insert_00620480(&list, nullptr, copy, insert_at); // 00622AD7
                    req::event_list_delete_0061fb50(&list, nullptr, i);    // 00622ADC..E5 (i, count not adjusted)
                }
            }
        }
        // ---- dispatch pass (00622B0F..00622CD6) ----
        for (int i = 0; i < count; ++i) {                                  // [esp+10h], EBX = i*16
            if (kotor_fastcall_005ed470(app(), nullptr) &&
                kotor_fastcall_005ed460(app(), nullptr)) {                 // 00622B20..41
                void* file = kotor_fastcall_005ed460(app(), nullptr);      // 00622B43..4E
                const U32 seconds = snapshot_seconds_div();                // 00622B59..70
                input()->WriteEventToFile(seconds, i, file);               // 00622B75..7C (time, index, file)
            }
            const InputEvent* ev = &list.data[i];
            const int id = ev->desc->id;                                   // 00622B81..89 (EDI)
            const int value = ev->value;                                   // 00622B8F (EBP)
            bool gui_event;
            if (id >= 0x27 && id <= 0x40) gui_event = true;                // 00622B8C..97 (signed)
            else if (at<int>(req::get_in_game_gui_005ed690(app(), nullptr), 0x34) == 1) gui_event = false;  // 00622B99..AB
            else gui_event = (id >= 0xB4 && id <= 0xBB) || id == 0xCE || id == 0xDF || id == 0xF3 || id == 0xF4 ||
                             (id >= 0xFE && id <= 0x106);              // 00622BB1..EF
            if (gui_event) {
                if (value != 0 && at<unsigned char>(get<void*>(0x4), 0x6D) == 5 &&           // 00622BF1..FC
                    at<void*>(req::get_in_game_gui_005ed690(app(), nullptr), 0xDC) == nullptr) {  // 00622BFE..14
                    // T-PI-2: CClientOptions+6Dh camera mode is 3 on the menu (ctor 0061DB79; CGuiInGame ctor
                    // SetCameraMode(3) 0062FFB6); every other SetCameraMode 0061B6F0 caller is a CSWCModule camera
                    // method / LoadModule / HandleServerToPlayerCamera (module loaded).
                    kotor_input_cells_required::camera_restore_007a2288 = 1;       // 00622C18
                    kotor_fastcall_00641dc0(module_of(this), nullptr);             // 00622C16..29 GetProperty0x18 -> RestoreCamera
                    kotor_fastcall_006200e0(this, nullptr, 0, 1);                  // 00622C2E..34 SetInputClass(0,1); jmp 00622C70
                } else {
                    const U32 mode = get<U32>(0x9C);
                    if (at<int>(get<void*>(0x40), 0x34) == 1 || mode == 2 || mode == 3)     // 00622C3B..52
                        get<Manager*>(0x274)->HandleInputEvent(id, value);                  // 00622C54..5C
                }
            } else {
                client_result = HandleInputEvent(id, value);               // 00622C63..6C
            }
            const U32 mode = get<U32>(0x9C);
            if ((mouse_look == 0 || mode == 2) && mode != 4) {             // 00622C70..88
                client_result = 0;                                         // 00622C9E
                get<Manager*>(0x274)->HandleMouseMove(mouse_x, mouse_y);   // 00622C8A..A6
            } else if (client_result && kotor_input_cells::gui_manager_007a39f4) {  // 00622CAD..BD
                gui_manager()->ResetToolTipTimer();                        // 00622CBF
            }
        }
        // ---- mouse-over / picking (00622CDC..006236FD) ----
        if (!module_of(this) || !at<void*>(module_of(this), 0x40) || mouse_look) {  // 00622CDC..D03
            void* in_game = get<void*>(0x40);                              // 006236AE
            if (in_game && at<void*>(in_game, 0x90)) {                     // 006236B1..BD
                // T-PI-4: CGuiInGame+90h (main interface) is 0 on the menu: ctor 006300E7; set only by
                // CGuiInGame::Initialize 0063304D (<- InitializeInGameGUI <- HandleServerToPlayerModule 00668575) and
                // ResetInterfaceForSize 0062F65B (D83 trap, lane main).
                const float zero_position[3] = {0.0f, 0.0f, 0.0f};             // [esp+50h..58h] 006236CD..DD
                kotor_fastcall_006222f0(this, nullptr, 0x7F000000u, zero_position, 0);  // 006236BF..E5
                put<U32>(at<void*>(get<void*>(0x40), 0x90), 0x5CAC, 0x7F000000u);       // 006236EA..F3
            }
        } else {
            // T-PI-3: CSWCModule+40h (scene) is 0 on the menu: ctor 00644009; written only by CSWCModule::LoadModule
            // 0063F6E0/0063F8B4/0063F9A5 (single caller HandleServerToPlayerModule 0066855F) and ~CSWCModule 00641D37.
            w5_process_input_picking_00622d09(this, nullptr);                // 00622D09..006236FD (lane W5 t3 helper)
        }
        live_free(list.data);                                              // 006236FD..0A (bare _free, null-safe)
    }
    // ---- mouse-look clears the GUI hover (00623718..00623769) ----
    if (mouse_look) {
        Control* over = get<Manager*>(0x274)->mouse_over;                  // 0062371C..22
        if (over) {
            Control* parent = over->GetSelectableParent();                 // 00623729..2B
            (over->*(over->vtable->handle_mouse_over))(0);                 // 00623730..38
            (over->*(over->cvt()->handle_focus_change))(0);                // 0062373B..41
            if (parent) {
                (parent->*(parent->vtable->handle_mouse_over))(0);         // 00623748..4F
                (parent->*(parent->cvt()->handle_focus_change))(0);        // 00623752..59
            }
            get<Manager*>(0x274)->mouse_over = nullptr;                    // 0062375C..62
        }
    }
    StartupString text94;                                                  // [esp+94h] 0062376B..72
    req::get_player_creature_005ed540(app(), nullptr);                     // 00623777..88 (result unused)
    if (kotor_fastcall_005ed460(app(), nullptr)) {                         // 0062378D..9D
        GameObject* pc = static_cast<GameObject*>(req::get_player_creature_005ed540(app(), nullptr));  // 006237A3..AB
        if (pc) {                                                          // 006237B0..B2
            const Vector3 p = pc->position;                                // 006237B8..C0
            const U32 seconds = snapshot_seconds_div();                    // 006237ED..1A
            kotor_reconstruction::StartupFormat005e5680(&text94, ids::capture_format_0074ea00, seconds, 0x58,
                static_cast<double>(p.x), static_cast<double>(p.y), static_cast<double>(p.z));  // 006237C3..2D
            req::file_write_string_005e6a70(kotor_fastcall_005ed460(app(), nullptr), nullptr, &text94);  // 00623832..4C
        }
    }
    void* pc0 = req::get_character_006346c0(req::get_sw_party_005ed8b0(app(), nullptr), nullptr, 0);  // 00623851..66 (EDI)
    (void)pc0;
    if (!kotor_fastcall_005f2ed0(this, nullptr) && get<void*>(0x2A0)) {  // 00623866..84
        // T-PI-5: client+2A0h (drive controller) is 0 on the menu: ctor 005FC4C6; only writer
        // AttachPlayerGameObjectToArea 005F5DF9 (D83 trap 005F5CF0, lane main; callers SetInputClass 0062019E class 0/1
        // with a pending player id, CClientExoApp 005ED723).
        w5_process_input_drive_0062388a(this, nullptr, delta);             // 0062388A..00623F1F (lane W5 helper)
    }
    // ---- drive update / mini-game (00623F23..0062440C) ----
    if (req::get_player_creature_005f1420(this, nullptr)) {               // 00623F23..2C
        // T-PI-6: client+20h player id is 7F000000 (ctor 005FC216; writers SetPlayerCreature / UnloadModule /
        // CrownFutureLeader / DontUseThisSetPlayerCreature / ParseEventCaptureEventFile); GetCreatureByGameObjectID
        // 005F1180 finds no creature for it (no game objects on the menu).
        void* control = get<void*>(0x2A0);                                 // 00623F2E
        using Control = int(__fastcall*)(void*, void*, float);             // vtbl+28h CSWPlayerControlCamRelative::Control 00679940 (W5)
        (*reinterpret_cast<Control*>(*static_cast<char**>(control) + 0x28))(control, nullptr, delta); // 00623F34..3E
        void* drive_module = get<void*>(0x18);                             // 00623F41
        if (!drive_module) return;                                         // 00623F44..46
        void* drive_area = at<void*>(drive_module, 0x48);                  // 00623F4C
        if (!drive_area) return;                                           // 00623F4F..51
        void* mini = at<void*>(drive_area, 0x264);                       // 00623F57 single load
        if (!mini) return;
        kotor_fastcall_006710e0(mini, nullptr, delta);                     // 00623F65 -> 0062440C
        return;
    }
    void* module = get<void*>(0x18);                                       // 00623F6B
    if (!module) return;
    void* area = at<void*>(module, 0x48);                                  // 00623F76
    if (!area) return;                                                     // 00623F7B -> 00624411
    void* mini = at<void*>(area, 0x264);                                  // 00623F81 single load
    if (!mini) return;
    const int kind = at<int>(mini, 0x80);                                  // 00623F8F
    const double minus_one = f32(ids::minus_one_00740cbc), one = f32(ids::one_0073d764);
    if (kind == 2) {                                                       // 00623FA3
        float x;                                                           // [esp+14h]
        axis_pair_00623faa(0x11E, 0x11B, &x);                              // 00623FAA..0062409D
        float y;                                                           // [esp+10h]
        axis_pair_00623faa(0x11D, 0x11A, &y);                              // 0062409D..00624190
        float y_axis = y;                                                  // 00624190..C1
        if (app()->GetClientOptions() && (options()->flags08 & 8u)) y_axis = -y;  // 00624199..B9 (FCHS)
        Vector3 axis{x, 0.0f, y_axis};                                     // [esp+4Ch] 006241C1..D5
        kotor_fastcall_00670fb0(mini, nullptr, &axis, delta);            // 006241C9..DD
        float mx, my;                                                      // [esp+14h], [esp+10h]
        mouse_pair_006241e2(&mx, &my);                                     // 006241E2..00624218
        if (!fl_c0_or_c3(minus_one, mx)) mx = -1.0f;                       // 0062421C..2B / 00624248
        else if (fl_less(one, mx)) mx = 1.0f;                              // 0062422D..3E
        if (!fl_c0_or_c3(minus_one, my)) my = -1.0f;                       // 00624250..5F / 0062427C
        else if (fl_less(one, my)) my = 1.0f;                              // 00624261..72
        if (app()->GetClientOptions() && (options()->flags08 & 8u)) my = -my;  // 0062428D..AE
        kotor_fastcall_00671020(mini, nullptr, mx, my, delta);          // 006242B2..BF
        if (!kotor_fastcall_005f2df0(this, nullptr)) {          // 006242C4..CD
            Manager* m = get<Manager*>(0x274);                             // 006242D3
            const int half_h = static_cast<int>(m->height) / 2;            // 006242D9..E6 (CDQ/SUB/SAR)
            const int half_w = static_cast<int>(m->width) / 2;             // 006242E9..F0
            reinterpret_cast<kotor_menu20::Input*>(input())->set_mouse_005df5d0(half_w, half_h);  // 006242DD..F3
        }
    } else if (kind == 1) {                                                // 00623F9D -> 006242FD
        float v;                                                           // FSTP32 [esp+30h] at 006243E9
        axis_pair_00623faa(0x11E, 0x11B, &v);                              // 006242FD..006243E8 (FLD 1 / FLD -1 / sum -> FSTP32)
        Vector3 axis{v, 0.0f, 0.0f};                                       // [esp+30h] 006243E8..FC
        kotor_fastcall_00670fb0(mini, nullptr, &axis, delta);            // 006243ED..404
    }
    kotor_fastcall_006710e0(mini, nullptr, delta);                           // 00624409..0C
}                                                                          // 00624411..37 ~CExoString x2
