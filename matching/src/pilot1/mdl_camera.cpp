// Pilot group 1: MdlNodeCamera. The original's extent for the ctor also
// covers the following (unlisted) function at 0x0044A2A0, so this file is
// compiled without /Gy to lay both out contiguously in one .text section.

class MdlNode {
public:
    unsigned short type;    // 0x0
    char pad_0x2[0x4e];
    MdlNode(char* name);
};

class MdlNodeCamera : public MdlNode {
public:
    MdlNodeCamera(char* name);
};

class Model {
public:
    char* GetName();
    MdlNode* NewCameraNode();
};

// FUNCTION: SWKOTOR 0x0044A2A0
MdlNode* Model::NewCameraNode()
{
    return new MdlNodeCamera(GetName());
}

// FUNCTION: SWKOTOR 0x0044A280
MdlNodeCamera::MdlNodeCamera(char* name)
    : MdlNode(name)
{
    type = 9;
}
