# Worker f notes (bucket 0x00580000-0x005CFFFF)

Older history for this bucket (rounds r1-r11, written in the Kotor RE
checkout) is in `docs/batches/worker-f-notes.md` there.

## 2026-10-26 round s1: CSWRoomSurfaceMesh / CSWCollisionMesh (walkmesh)

Folder `matching/src/f`: 604/651 -> 637/688 (kotor-match -q -j 2).

New files:

- `include/f/room_surface_mesh.h`: shared layout for CSWCollisionMesh
  (vtable slots 0-10), CSWRoomSurfaceMesh (+0x88 adjacency, +0x8c perimeter
  edge list, +0x9c room-end list, +0xac AABB list, each with an external flag;
  +0xbc crossing points, +0xc8 crossing data, +0xd4 AABB root, +0xd8..0xe4
  LineOfSight / Walk / WalkCheck / all surface masks), the 0x88-byte BWM header
  (CSWWalkMeshHeader), CSWCollisionResult, CResBWM / CResMDL.
- `src/f/room_surface_mesh.cpp` (26 annotated, 21 exact).
- `src/f/collision_mesh.cpp` (10 annotated, 10 exact).
- `src/f/collision_result.cpp` (out-of-line CSWCollisionResult ctor, exact).

Exact (32 new): 0x00580340 performance-counter timer, 0x00580720 AABB node
ctor, 0x005807C0 room LoadMeshBinary (hidden), 0x005808E0 BWM header ctor,
0x00580620 GetAdjacentFaces, 0x00580BE0 GetPerimeterVertex, 0x00580C60
GetEdgeVertices, 0x00580D50 WeldEdgeVertices, 0x00581320 CSWCollisionResult
ctor, 0x00581370 room SaveBinaryMesh (hidden), 0x00581610 CheckAABBAll,
0x00581720 FindTriangle, 0x00581930 / 0x00581A10 GetSurfaceMaterial(+
WalkCheckOnly), 0x00581B30 ComputeHeightAABB, 0x00581D60
~CExoArrayList<AABBNode>, 0x00581FE0 ~CSWRoomSurfaceMesh, 0x00582180 GetResRef
and 0x005821A0-0x005821D0 mask getters (hidden, inside the dtor's range);
CSWCollisionMesh 0x00596670 LoadMesh, 0x00596890 base LoadMeshText (hidden),
0x005968A0 GetLine, 0x00596AA0 GetWorldPoint, 0x00596C10 ComputeNormals,
0x00596D10 GetMeshBoundingBox, 0x00596E40 GetMeshOutline, 0x00597090
GetVertex(int, float*, float*, float*), 0x00599ED0 TransformToWorld,
0x0059A040 SaveBinaryMesh.
Promoted: 0x005806C0 GetTriangleIndices (surfmesh.cpp, was NONMATCH 4).

NONMATCH kept: 0x00580AF0 TransformAABBNode (4: `[edi+edx]` vs `[edx+edi]`
base/index order on the second node access), 0x00580E90 SetAdjacency (3: the
spilled vDiff2 components sit above the named Vectors instead of below them),
0x00582AE0 ctor (41: the original's `m_lAABB = <empty list>` temporary leaves
no destructor code and stores a constant 0 flag; the per-row
GetINTEntry calls load the 2DA pointer into eax before the pushes),
0x00580920 CheckAABBNode (51: merged `return 0` block placement and
register roles), 0x00581D90 AddCrossing (64: unrolled search loop; register
allocation and FP term order).

Lessons:

- `p[nFace * 3 + k]` written with an explicit `int i = nFace * 3;` and
  `((int *)p)[i + k]` gives the original `lea eax,[eax+eax*2]` +
  `[base+eax*4+k]`; struct indexing (`p[n].a[k]`) gives `shl` + `[esi+edx]`.
  Fixed GetAdjacentFaces, GetTriangleIndices and halved CheckAABBNode.
- Vector `operator+` member: MSVC swaps commutative fadd operands, so write
  `m_vOffset + v` to get `fld v; fadd offset` (TransformToWorld,
  GetWorldPoint `m_vPosition + q * (v + m_vOffset)`). The rule is not uniform
  (TransformAABBNode needs the two different spellings for max and min).
- A local that is only assigned from a by-value return: `Vector v;` (zeroing
  default ctor) followed by `v = F();` inside the loop gives the original
  temp-then-copy; the dead zero stores are removed (GetMeshBoundingBox).
- Several identical getters folded by the linker must be declared as distinct
  methods (CResBWM::GetBWMSize vs CResMDL::GetMDLSize), otherwise cl tail-merges
  the two LoadMeshText call sites (LoadMesh).
- Two temporaries passed by value (`CheckAABBAll(Vector(...), Vector(...))`)
  are built right-to-left; named `vStart` / `vEnd` locals give the original
  slot order.
- Loop-invariant expressions written inside the loop body are hoisted into
  both the unrolled and the remainder preheaders, like the original; the
  search loop only unrolls with the bound in a local (`int nCount = num`).
- An inline in-class CExoArrayList::Insert / Add / Allocate is inlined for
  `int` but stays a call for `Vector` (0x00581C30), as in the original.
- The plane-distance dot product slot order changed with operand order
  (`vNormal.x * v0.x + ...`) even though the emitted FP order did not.
- Commits from a worktree need `git -c user.name=... -c user.email=...`
  (no global identity on this Mac).

Next in range (walkmesh family): 0x005803C0 hidden adjacency builder
(~0x220 bytes, 7-int edge records, heavily strength-reduced), 0x005821E0
border-loop builder (1104), 0x00582630 IsPointInPolygon (1200), 0x00583E30
ComputeClippedLineSegment (1008), 0x00584220 ClippedLineSegmentWalkable
(1616), 0x00582D70 room LoadMeshText (4288), 0x005972B0 NoNonWalkPolys
(11296); retries of the five NONMATCHes above. Other queue from r11 still
applies (dialog, combat, store, faction 0x005BFCC0).
