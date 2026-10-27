// CSWVirtualMachineCommands::ExecuteCommandGetFirstItemInInventory (round s10):
// GetFirstItemInInventory (0x153) and GetNextItemInInventory share this
// handler. Creatures and placeables step into container items: the
// iterator keeps an index into the main repository, an index into the
// open container and the container's id.
#include "b/vm_commands.h"
#include "b/sws_item.h"

extern const unsigned long OBJECT_INVALID;                  // 0x00746644

// Iterator fields (names guessed).
struct CSWSCreatureInventoryIterView {
    char pad_0x0[0xa34];
    unsigned short m_nInventoryIndex;                       // 0xa34
    unsigned short m_nContainerIndex;                       // 0xa36
    unsigned long m_oidContainer;                           // 0xa38
};

struct CSWSPlaceableInventoryIterView {
    char pad_0x0[0x36c];
    CItemRepository* m_pItemRepository;                     // 0x36c
    unsigned short m_nInventoryIndex;                       // 0x370
    unsigned short m_nContainerIndex;                       // 0x372
    unsigned long m_oidContainer;                           // 0x374
};

struct CSWSItemInventoryIterView {
    char pad_0x0[0x26c];
    CItemRepository* m_pRepository;                         // 0x26c
    char pad_0x270[0x298 - 0x270];
    unsigned short m_nInventoryIndex;                       // 0x298
};

struct CSWSStoreInventoryIterView {
    char pad_0x0[0x254];
    unsigned short m_nInventoryIndex;                       // 0x254
};

#define CREATURE_ITER(p) ((CSWSCreatureInventoryIterView*)(p)->AsSWSCreature())
#define PLACEABLE_ITER(p) ((CSWSPlaceableInventoryIterView*)(p)->AsSWSPlaceable())
#define ITEM_ITER(p) ((CSWSItemInventoryIterView*)(p)->AsSWSItem())
#define STORE_ITER(p) ((CSWSStoreInventoryIterView*)(p)->AsSWSStore())

// NONMATCH (109): register swap only; the original keeps the index in edi
// and the creature/placeable pointer in ebx, cl swaps them.
// FUNCTION: SWKOTOR 0x00549B50
int CSWVirtualMachineCommands::ExecuteCommandGetFirstItemInInventory(int nCommandId, int nParameters)
{
    unsigned long oidObject;
    unsigned long oidResult = INVALID_OBJECT_ID;
    CItemRepository* pMainRepository = NULL;
    unsigned short nIndex = 0;
    if (!g_pVirtualMachine->StackPopObject(&oidObject)) {
        return VIRTUAL_MACHINE_ERROR_STACK_UNDERFLOW;
    }
    CGameObject* pObject = g_pAppManager->GetServerExoApp()->GetGameObject(oidObject);
    if (pObject) {
        CItemRepository* pRepository = NULL;
        if (pObject->AsSWSCreature()) {
            CSWSCreature* pCreature = pObject->AsSWSCreature();
            CSWSCreatureInventoryIterView* pIter = (CSWSCreatureInventoryIterView*)pCreature;
            pRepository = pCreature->GetItemRepository(1);
            pMainRepository = pRepository;
            if (nCommandId == 0x153) {
                pIter->m_oidContainer = OBJECT_INVALID;
                pIter->m_nContainerIndex = 0;
                pIter->m_nInventoryIndex = 1;
            } else {
                CSWSItem* pContainer = g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(pIter->m_oidContainer);
                if (pContainer && pContainer->GetPossessor() == pCreature->m_idSelf) {
                    pRepository = pContainer->m_pRepository;
                    nIndex = pIter->m_nContainerIndex++;
                } else {
                    pIter->m_nContainerIndex = 0;
                    pIter->m_oidContainer = OBJECT_INVALID;
                    nIndex = pIter->m_nInventoryIndex++;
                }
            }
        } else if (pObject->AsSWSPlaceable()) {
            CSWSPlaceableInventoryIterView* pIter = PLACEABLE_ITER(pObject);
            pRepository = pIter->m_pItemRepository;
            pMainRepository = pRepository;
            if (nCommandId == 0x153) {
                pIter->m_oidContainer = OBJECT_INVALID;
                pIter->m_nContainerIndex = 0;
                pIter->m_nInventoryIndex = 1;
            } else {
                CSWSItem* pContainer = g_pAppManager->GetServerExoApp()->GetItemByGameObjectID(pIter->m_oidContainer);
                if (pContainer && pContainer->GetPossessor() == ((CGameObject*)pIter)->m_idSelf) {
                    pRepository = pContainer->m_pRepository;
                    nIndex = pIter->m_nContainerIndex++;
                } else {
                    pIter->m_nContainerIndex = 0;
                    pIter->m_oidContainer = OBJECT_INVALID;
                    nIndex = pIter->m_nInventoryIndex++;
                }
            }
        } else if (pObject->AsSWSItem()) {
            pRepository = ITEM_ITER(pObject)->m_pRepository;
            if (nCommandId != 0x153) {
                nIndex = ITEM_ITER(pObject)->m_nInventoryIndex;
            }
            ITEM_ITER(pObject)->m_nInventoryIndex = nIndex + 1;
        } else if (pObject->AsSWSStore()) {
            CSWSStoreInventoryIterView* pIter = STORE_ITER(pObject);
            unsigned short nStoreIndex;
            if (nCommandId == 0x153) {
                nStoreIndex = 0;
            } else {
                nStoreIndex = pIter->m_nInventoryIndex;
            }
            pIter->m_nInventoryIndex = nStoreIndex + 1;
        }
        if (pRepository) {
            if ((unsigned long)pRepository->m_aItems.num <= nIndex && pRepository != pMainRepository) {
                pRepository = pMainRepository;
                if (pObject->AsSWSCreature()) {
                    nIndex = CREATURE_ITER(pObject)->m_nInventoryIndex;
                    CREATURE_ITER(pObject)->m_nInventoryIndex = nIndex + 1;
                    CREATURE_ITER(pObject)->m_oidContainer = OBJECT_INVALID;
                    CREATURE_ITER(pObject)->m_nContainerIndex = 0;
                } else if (pObject->AsSWSPlaceable()) {
                    nIndex = PLACEABLE_ITER(pObject)->m_nInventoryIndex;
                    PLACEABLE_ITER(pObject)->m_nInventoryIndex = nIndex + 1;
                    PLACEABLE_ITER(pObject)->m_oidContainer = OBJECT_INVALID;
                    PLACEABLE_ITER(pObject)->m_nContainerIndex = 0;
                }
            }
            if ((unsigned long)pRepository->m_aItems.num > nIndex) {
                CSWSItem* pItem = pRepository->GetItemAt(nIndex);
                if (pItem) {
                    CSWBaseItem* pBaseItem = g_pRules->m_pBaseItemArray->GetBaseItem(pItem->m_nBaseItem);
                    if (pBaseItem && pBaseItem->m_bContainer) {
                        if (pObject->AsSWSCreature()) {
                            CREATURE_ITER(pObject)->m_oidContainer = pItem->GetId();
                            CREATURE_ITER(pObject)->m_nContainerIndex = 0;
                        } else if (pObject->AsSWSPlaceable()) {
                            PLACEABLE_ITER(pObject)->m_oidContainer = pItem->GetId();
                            PLACEABLE_ITER(pObject)->m_nContainerIndex = 0;
                        }
                    }
                    oidResult = pItem->m_idSelf;
                }
            }
        }
    }
    return g_pVirtualMachine->StackPushObject(oidResult) ? 0 : VIRTUAL_MACHINE_ERROR_STACK_OVERFLOW;
}
