// Scalar deleting destructors for small panel classes.

class CSWGuiMapHider {
public:
    __declspec(noinline) virtual ~CSWGuiMapHider();
};

class CSWGuiInGameMap {
public:
    __declspec(noinline) virtual ~CSWGuiInGameMap();
};

class CSWGuiInGameGalaxyMap {
public:
    __declspec(noinline) virtual ~CSWGuiInGameGalaxyMap();
};

// FUNCTION: SWKOTOR 0x00695160 SYMBOL: ??_GCSWGuiMapHider@@UAEPAXI@Z
CSWGuiMapHider::~CSWGuiMapHider() {}

// FUNCTION: SWKOTOR 0x00695BD0 SYMBOL: ??_GCSWGuiInGameMap@@UAEPAXI@Z
CSWGuiInGameMap::~CSWGuiInGameMap() {}

// FUNCTION: SWKOTOR 0x006958A0 SYMBOL: ??_GCSWGuiInGameGalaxyMap@@UAEPAXI@Z
CSWGuiInGameGalaxyMap::~CSWGuiInGameGalaxyMap() {}
