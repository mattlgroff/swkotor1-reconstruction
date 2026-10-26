# Bark and streaming sound ABI recovery

The Mac debug map identifies CSWGuiBarkBubble::SetBark as taking
`(unsigned long, const CExoString&, const CResRef&, int)` and identifies both
CExoStreamingSoundSource::Play and its internal counterpart as taking
`(const CResRef&, unsigned short, int, int, int)` (2/2 builds). Earlier partial
headers modeled the resource reference as an integer. These declarations now
match the supported signatures; their native call chain forwards the address
of the resource reference. Both affected translation units remain byte
identical in their individual comparisons.
