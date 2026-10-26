// Scene visibility-file loader (0x004568D0).
#include <stdio.h>
template<class T> class CExoArrayList { public: T* element; int num, array_size; };
class CAurRoom { public: char pad_0x0[0x5c]; CExoArrayList<CAurRoom*> m_lVisible; };
class AurRes;
void* AurResGet(const char*, const char*, int*, char); // 0x0044C740
void AurResClose(AurRes*, int); // 0x0044C8F0
char* AurResGetNextLine(); // 0x0044BFA0
class Scene {
public:
    virtual void V000();
    virtual void V004();
    virtual void V008();
    virtual void V00C();
    virtual void V010();
    virtual void V014();
    virtual void V018();
    virtual void V01C();
    virtual void V020();
    virtual void V024();
    virtual void V028();
    virtual void V02C();
    virtual void V030();
    virtual void V034();
    virtual void V038();
    virtual void V03C();
    virtual void V040();
    virtual void V044();
    virtual void V048();
    virtual void V04C();
    virtual void V050();
    virtual void V054();
    virtual void V058();
    virtual void V05C();
    virtual void V060();
    virtual void V064();
    virtual void V068();
    virtual void V06C();
    virtual void V070();
    virtual CAurRoom* FindRoom(char* name); // +0x74
    char pad_0x4[0xc4];
    CExoArrayList<CAurRoom*> m_lRooms; // +0xc8
    void SetVisibility(CAurRoom*, CAurRoom*, int); // 0x00454940
    int LoadVisibility(char* name);
};

// FUNCTION: SWKOTOR 0x004568D0
int Scene::LoadVisibility(char* name)
{
    for (int i = 0; i < m_lRooms.num; i++)
        m_lRooms.element[i]->m_lVisible.num = 0;
    AurRes* pRes = (AurRes*)AurResGet(name, ".vis", 0, 1);
    if (!pRes) return 0;
    char visibleName[0x40];
    char roomName[0x40];
    char* line = AurResGetNextLine();
    while (line) {
        roomName[0] = 0;
        int count = 0;
        sscanf(line, "%s%d", roomName, &count);
        CAurRoom* room = FindRoom(roomName);
        if (room && count != 0) {
            for (int k = 0; k < count; k++) {
                visibleName[0] = 0;
                char* visibleLine = AurResGetNextLine();
                sscanf(visibleLine, "%s", visibleName);
                CAurRoom* visible = FindRoom(visibleName);
                if (visible) SetVisibility(room, visible, 1);
            }
        }
        line = AurResGetNextLine();
    }
    AurResClose(pRes, 0);
    return 1;
}
