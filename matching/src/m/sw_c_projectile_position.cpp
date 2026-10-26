// Folded position setters used by client projectile and server object code.
class Vector { public: float x, y, z; };
class CSWCProjectile {
public:
    char pad_0x00[0x114];
    Vector m_vTargetPosition;
    char pad_0x120[0x90];
    Vector m_vSourcePosition;
    void SetTargetPosition(const Vector& value);
    void SetSourcePosition(const Vector& value);
};

// FUNCTION: SWKOTOR 0x0064B4D0
void CSWCProjectile::SetTargetPosition(const Vector& value)
{
    m_vTargetPosition = value;
}

// FUNCTION: SWKOTOR 0x0064B4F0
void CSWCProjectile::SetSourcePosition(const Vector& value)
{
    m_vSourcePosition = value;
}
