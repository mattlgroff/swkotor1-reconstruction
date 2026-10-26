// Camera follow behavior; CAurCamera virtual slot +0x7c.
class Gob { public: char pad[4]; };
class CAurBehavior { public: char pad[0x14]; };
class CAurBehaviorCameraFollow : public CAurBehavior {
public:
    CAurBehaviorCameraFollow(Gob*, Gob*); // 0x0049FE80
    char pad_follow[0x5c];
};
class CAurCamera {
public:
    virtual void V00();
    virtual void V04();
    virtual void V08();
    virtual void V0C();
    virtual void V10();
    virtual void V14();
    virtual void V18();
    virtual void V1C();
    virtual void V20();
    virtual void V24();
    virtual void V28();
    virtual void V2C();
    virtual void V30();
    virtual void V34();
    virtual void V38();
    virtual void V3C();
    virtual void V40();
    virtual void V44();
    virtual void V48();
    virtual void V4C();
    virtual void V50();
    virtual void V54();
    virtual void V58();
    virtual void V5C();
    virtual void V60();
    virtual void V64();
    virtual void V68();
    virtual void V6C();
    virtual void V70();
    virtual void V74();
    virtual void V78();
    virtual void SetAttachment(CAurBehavior*); // +0x7c
};
class Camera : public CAurCamera, public Gob {
public:
    void SetFollow(Gob* target);
};

// FUNCTION: SWKOTOR 0x0045C0C0
void Camera::SetFollow(Gob* target)
{
    if (target)
        SetAttachment(new CAurBehaviorCameraFollow(this, target));
    else
        SetAttachment(0);
}
