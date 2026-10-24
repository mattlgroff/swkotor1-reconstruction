// 0044F3D0 Quaternion::xdir — private full leaf.
// Original: ECX = {w,x,y,z}; stack Vector3* out; RET 4; EAX = out.
// Canonical export: @quaternion_xdir_0044f3d0@12.
//
// Source order follows the original operation families:
//   A  = X*Z - Y*W, A2 = A+A          (X*Z first, Y*W second)
//   B  = Y*X + W*Z, B2 = B+B          (Y*X first, W*Z second)
//   yy = Y*Y first, zz = Z*Z second, s = yy+zz, c = 1-(s+s)
//   store out->x=c, out->y=B2, out->z=A2
// All quaternion reads happen before the first store, so out may alias q.
// Ordinary double intermediates model the original PC53 add/mul/sub domain;
// this is not an all-FP-state or byte-identity claim. Emitted order is an
// object-review gate.

struct Vector3 {
    float x, y, z;
};

struct Quaternion {
    float w, x, y, z;
};

extern "C" Vector3* __fastcall quaternion_xdir_0044f3d0(
    const Quaternion* q, void*, Vector3* out) {
    const float w = q->w;
    const float x = q->x;
    const float y = q->y;
    const float z = q->z;

    const double xz = static_cast<double>(x) * static_cast<double>(z);
    const double yw = static_cast<double>(y) * static_cast<double>(w);
    const double a = xz - yw;
    const double a2 = a + a;

    const double yx = static_cast<double>(y) * static_cast<double>(x);
    const double wz = static_cast<double>(w) * static_cast<double>(z);
    const double b = yx + wz;
    const double b2 = b + b;

    const double yy = static_cast<double>(y) * static_cast<double>(y);
    const double zz = static_cast<double>(z) * static_cast<double>(z);
    const double s = yy + zz;
    const double c = 1.0 - (s + s);

    out->x = static_cast<float>(c);
    out->y = static_cast<float>(b2);
    out->z = static_cast<float>(a2);
    return out;
}
