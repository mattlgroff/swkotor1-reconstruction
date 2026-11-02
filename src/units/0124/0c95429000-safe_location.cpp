#include "0e76439f1a-safe_location.hpp"
#include "1a847f7684-w1_x87.hpp"
#include <cstring>
// Reconstructed from swkotor.exe (GOG, sha256 9c10e045...). Comments cite original PCs.
// Tier 7 (reached by W5's movement / door code): 0050CA10 CSWSArea::NoNonWalkPolysInDoors, 004C0FA0
// CSWSArea::ComputeSafeLocationInDirection, and the walkability leaves 00506400 / 00581A10 they need.
namespace w1::safe_location {
namespace {
constexpr U kDoorPad = 0x3c23d70au;     // [0073DFEC] 0.01f
constexpr U kFloorDrop = 0x3dcccccdu;   // [0073E798] 0.1f
constexpr double kStepBias = 0.01;      // [00743310] qword 3F847AE147AE147B
constexpr U kRayTop = 0x447a0000u, kRayBottom = 0xc47a0000u;   // 1000.0f / -1000.0f (00581A84 / 00581AA2)
// a2 (L3 t1): the compares are C++ on long double register values (fld dword ; fcomp dword ; fnstsw ; test ah,mask),
// unrounded and NaN-exact: lt is the ordered less (test ah,5 -> C0 alone), not_gt the negation of the ordered greater
// (test ah,41h -> C0 or C3 or unordered).
long double F(U u) { float f; std::memcpy(&f, &u, 4); return f; }   // fld dword
bool lt(U a, U b) { return F(a) < F(b); }                  // fcomp; test ah,5 -> C0 alone (ordered a < b)
bool not_gt(U a, U b) { return !(F(a) > F(b)); }           // fcomp; test ah,41h -> C0 or C3 or unordered
// NoNonWalkPolysInDoors per-box rejection on one axis (0050CAB4..0050CB24 pattern): both ends below the minimum, or
// both above the maximum.
bool outside(U lo, U hi, U box_min, U box_max) {
  if (lt(lo, box_min) && lt(hi, box_min)) return true;
  if (!not_gt(hi, box_max) && !not_gt(lo, box_max)) return true;
  return false;
}
}
U surface_material_walk_check_00581a10(U mesh, U x, U y, U z, U e, Ports& p) {
  // e == esp after `push esi` (frame 0x4C): +0x1C local point, +0x28 hit record (+0 -1, +0xC triangle = -1, rest 0),
  // +0x4C..+0x54 the by-value world point.
  p.write(e + 0x4c, x); p.write(e + 0x50, y); p.write(e + 0x54, z);
  p.call(0x596a00, mesh, {e + 0x1c, e + 0x4c});                     // CSWCollisionMesh::WorldToLocal(out, in)
  for (U o : {0x4u, 0x8u, 0xcu, 0x2cu, 0x30u, 0x44u, 0x38u, 0x3cu, 0x40u}) p.write(e + o, 0);
  U lx = p.read(e + 0x1c), ly = p.read(e + 0x20);
  p.write(e + 0x10, lx); p.write(e + 0x14, ly); p.write(e + 4, lx); p.write(e + 8, ly);
  p.write(e + 0x18, kRayTop); p.write(e + 0xc, kRayBottom);
  p.write(e + 0x34, 0xffffffffu); p.write(e + 0x28, 0xffffffffu);
  p.call(0x581530, mesh, {lx, ly, kRayTop, lx, ly, kRayBottom, e + 0x28});   // CheckAABBWalkable(from, to, hit)
  U tri = p.read(e + 0x34);
  if (tri == 0xffffffffu) return 0;
  return p.read(p.read(mesh + 0x64) + tri * 4);                     // surface material of the hit triangle
}
U position_walkable_00506400(U area, U x, U y, U z, U e, Ports& p) {
  // e == esp after `push esi`: +4 Walk value, +8 CExoString "Walk", +0x18.. by-value point (+0x20)
  p.write(e + 0x20, x); p.write(e + 0x24, y); p.write(e + 0x28, z);
  U room = p.call(0x4bb600, area, {e + 0x20, 0});                   // CSWSArea::GetRoom
  if (!room) return 0;
  U mat = p.call(0x581a10, p.read(room + 0x3c), {x, y, z});         // GetSurfaceMaterialWalkCheckOnly
  p.call(0x5e5a90, e + 8, {0x747670});                              // CExoString("Walk")
  U table = p.read(p.read(p.global(kRules) + 0xb8) + 0x14);         // surfacemat 2DA
  p.call(0x413660, table, {mat, e + 8, e + 4});                     // C2DA::GetINTEntry(row, column, &out)
  p.call(0x5e5c20, e + 8, {});
  return p.read(e + 4) ? 1u : 0u;
}
U no_nonwalk_polys_in_doors_0050ca10(U area, U a, U b, U zlo, U zhi, U radius, U object_id, U out_id, U list, U flag9, U flag10,
                                     Ports& p) {
  S n = static_cast<S>(p.read(area + 0x48));
  if (n <= 0) return 1;                                              // 0050CA35
  U ax = p.read(a), bx = p.read(b), ay = p.read(a + 4), by = p.read(b + 4);
  U minx = lt(ax, bx) ? ax : bx, maxx = lt(ax, bx) ? bx : ax;        // 0050CA3F..0050CA56
  U miny = lt(ay, by) ? ay : by, maxy = lt(ay, by) ? by : ay;
  U minx_r = x87::sub_float(minx, radius), miny_r = x87::sub_float(miny, radius);
  U maxx_r = x87::add_float(maxx, radius), maxy_r = x87::add_float(maxy, radius);
  bool allocated = false;
  for (S i = 0; i < static_cast<S>(p.read(area + 0x48)); ++i) {    // 0050CAB1..0050CC26
    U box = p.read(area + 0x4c) + static_cast<U>(i) * 0x18;
    U b0 = p.read(box), b1 = p.read(box + 4), b2 = p.read(box + 8), b3 = p.read(box + 0xc), b4 = p.read(box + 0x10), b5 = p.read(box + 0x14);
    if (outside(minx_r, maxx_r, b0, b3)) continue;
    if (outside(miny_r, maxy_r, b1, b4)) continue;
    if (outside(zlo, zhi, b2, b5)) continue;
    U id = p.read(p.read(area + 0x50) + static_cast<U>(i) * 4);
    U o = p.call(0x4ae750, server(p), {id});                          // CServerExoApp::GetGameObject
    if (!o || !p.vcall(o, 0x18, {})) continue;                        // doors only
    if (p.vcall(area, 4, {id, list})) continue;                       // vtable +4 (id, list): ignored
    if (p.call(0x4bbe20, area, {id, a, b, zlo, zhi, radius, object_id, flag9, 0})) continue;   // NoNoneWalkPolysInStaticObject
    p.write(out_id, p.read(p.read(area + 0x50) + static_cast<U>(i) * 4));   // 0050CC46..0050CC55: the blocking door
    if (flag10 != 1) return 0;
    if (!list) {                                                      // 0050CC5D..0050CC87: temporary CExoArrayList
      U mem = p.call(0x6fa7e6, 0, {0xc});
      if (mem) { p.write(mem + 4, 0); p.write(mem + 8, 0); p.write(mem, 0); }
      list = mem; allocated = true;
    }
    U r = p.vcall(area, 0x18, {a, b, zlo, zhi, radius, object_id, out_id, flag9, list, 1});   // vtable +0x18 = 0050C1E0 HandleTransparentDoors (10 args, ret 0x28)
    if (list && allocated) { p.call(0x6fa390, 0, {p.read(list)}); p.write(list, 0); p.call(0x6fa390, 0, {list}); }
    return r;
  }
  return 1;
}
U compute_safe_location_in_direction_004c0fa0(U area, U start, U dir, U max_dist, U creature, U check_doors, U out, U e,
                                               Ports& p) {
  // e == esp after the four pushes (frame 0x78). +0x14 loop index / room out, +0x18 count, +0x1C/+0x7C default height 0,
  // +0x20 door out, +0x24..+0x2C candidate, +0x30..+0x38 normalized direction, +0x3C..+0x44 forward point,
  // +0x48..+0x50 candidate before height, +0x54/+0x58 sideways unit, +0x64/+0x68 sideways offsets, +0x74 forward z,
  // +0x80 float count, the argument slots +0x7C..+0x90 as the original reuses them.
  p.write(area + 0x1ac, creature);
  U r = p.read(creature + 4);                                       // step (+4)
  U h = p.read(creature + 0x14);                                    // height (+0x14)
  p.write(e + 0x7c, 0);
  U px = x87::add_float(p.read(start), p.read(dir)), py = x87::add_float(p.read(start + 4), p.read(dir + 4));
  U pz = x87::add_float(p.read(dir + 8), p.read(start + 8));
  p.write(e + 0x24, px); p.write(e + 0x28, py); p.write(e + 0x2c, pz);
  auto height = [&](U default_slot) {                               // GetRoom + CSWSRoom::ComputeHeight (ST0) or the default
    U room = p.call(0x4bb600, area, {e + 0x24, default_slot == e + 0x7c ? e + 0x80 : e + 0x14});
    if (!room) { p.write(e + 0x2c, p.read(default_slot)); return; }
    p.call_st0(0x579430, room, {p.read(e + 0x24), p.read(e + 0x28), p.read(e + 0x2c), 0}, e + 0x98);
    x87::X80 x; for (U i = 0; i < 10; ++i) x.b[i] = static_cast<unsigned char>(p.read8(e + 0x98 + i));
    p.write(e + 0x2c, x87::x80_to_float(x));
  };
  auto accept = [&]() {                                             // 004C1058..004C1070 / 004C13B2..004C13CD
    p.write(out, p.read(e + 0x24)); p.write(out + 4, p.read(e + 0x28)); p.write(out + 8, p.read(e + 0x2c)); return 1u;
  };
  height(e + 0x7c);
  if (p.call(0x4be5e0, area, {e + 0x24, creature}) == 1) return accept();   // TestSafeLocationPoint
  bool doors = false;
  if (p.call(0x506400, area, {p.read(e + 0x24), p.read(e + 0x28), p.read(e + 0x2c)})) {   // PositionWalkable
    // 004C10AA..004C1127: the door box around the step point: B = (x - 0.01, y - 0.01, z), A = (x + 0.01, y + 0.01, z)
    U cx = p.read(e + 0x24), cy = p.read(e + 0x28), cz = p.read(e + 0x2c);
    p.write(e + 0x48, x87::add_float(cx, kDoorPad)); p.write(e + 0x4c, x87::add_float(cy, kDoorPad)); p.write(e + 0x50, cz);
    p.write(e + 0x3c, x87::sub_float(cx, kDoorPad)); p.write(e + 0x40, x87::sub_float(cy, kDoorPad)); p.write(e + 0x44, cz);
    p.write(e + 0x7c, r);
    doors = p.call(0x50ca10, area, {e + 0x3c, e + 0x48, cz, x87::add_float(h, cz), r, kInvalidObject, e + 0x20, 0, 1, 0}) != 0;
  }
  if (!doors) check_doors = 0;                                      // 004C1130
  p.write(e + 0x48, p.read(dir)); p.write(e + 0x4c, p.read(dir + 4)); p.write(e + 0x50, 0);
  p.call(0x4ab180, 0, {e + 0x30, e + 0x48});                        // normalize(out, in)
  p.write(e + 0x58, p.read(e + 0x30) ^ 0x80000000u);                // fchs (sideways = (n.y, -n.x))
  p.write(e + 0x54, p.read(e + 0x34));
  U d = r;
  p.write(e + 0x7c, d);
  if (lt(r, max_dist)) {
    do {
      S k = static_cast<S>(x87::ftol_add_double_div(d, kStepBias, r));   // 004C11A0..004C11B1
      p.write(e + 0x18, static_cast<U>(k));
      if (-k <= k) {
        p.write(e + 0x80, x87::int_to_float(k));
        U n3[3] = {p.read(e + 0x30), p.read(e + 0x34), p.read(e + 0x38)}, side[2] = {p.read(e + 0x54), p.read(e + 0x58)};
        U st[3] = {p.read(start), p.read(start + 4), p.read(start + 8)};
        for (S i = -k; i <= k; ++i) {
          p.write(e + 0x14, static_cast<U>(i));
          p.write(e + 0x1c, 0);
          x87::SafeStep s = x87::safe_direction_point(i, p.read(e + 0x80), side, n3, st);
          p.write(e + 0x64, s.oy); p.write(e + 0x68, s.oz); p.write(e + 0x74, s.fz_offset);
          p.write(e + 0x3c, s.f[0]); p.write(e + 0x40, s.f[1]); p.write(e + 0x44, s.f[2]);
          p.write(e + 0x48, s.g[0]); p.write(e + 0x4c, s.g[1]); p.write(e + 0x50, s.g[2]);
          p.write(e + 0x24, s.g[0]); p.write(e + 0x28, s.g[1]); p.write(e + 0x2c, s.g[2]);
          height(e + 0x1c);
          if (p.call(0x4be5e0, area, {e + 0x24, creature}) != 1) continue;
          U base = lt(p.read(e + 0x2c), p.read(start + 8)) ? p.read(e + 0x2c) : p.read(start + 8);   // 004C12D6..004C130B
          U lo = x87::sub_float(base, kFloorDrop), hi = x87::add_float(h, base);
          p.write(e + 0x14, lo); p.write(e + 0x10, hi);
          if (!check_doors) return accept();
          if (p.call(0x50ca10, area, {start, e + 0x24, lo, hi, r, kInvalidObject, e + 0x20, 0, 1, 0}) == 1) return accept();
        }
      }
      bool more = x87::add_store_below(d, r, max_dist);             // 004C1366..004C1381 (fst, then fcomp unrounded)
      p.write(e + 0x7c, d);
      if (!more) break;
    } while (true);
  }
  return p.call(0x4be860, area, {start, max_dist, creature, check_doors, out});   // CSWSArea::ComputeSafeLocation
}
}
