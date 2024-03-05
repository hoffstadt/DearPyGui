#pragma once

#include <stack>
#include <vector>
#include <unordered_map>
#include <queue>
#include <string>
#include <mutex>
#include <map>
#include "mvAppItem.h"
#include "mvPyUtils.h"

//-----------------------------------------------------------------------------
// forward declarations
//-----------------------------------------------------------------------------
struct mvItemRegistry;
class mvWindowAppItem;

//-----------------------------------------------------------------------------
// public API
//-----------------------------------------------------------------------------

void             RenderItemRegistry(mvItemRegistry& registry);

// cleanup
void             ClearItemRegistry(mvItemRegistry& registry);
void             CleanUpItem      (mvItemRegistry& registry, mvUUID uuid);
b8               DeleteItem       (mvItemRegistry& registry, mvUUID uuid, b8 childrenOnly = false, i32 slot = -1);

// aliases
void             AddAlias      (mvItemRegistry& registry, const std::string& alias, mvUUID id);
void             RemoveAlias   (mvItemRegistry& registry, const std::string& alias, b8 itemTriggered = false);
mvUUID           GetIdFromAlias(mvItemRegistry& registry, const std::string& alias);

// item movement
b8               MoveItem    (mvItemRegistry& registry, mvUUID uuid, mvUUID parent, mvUUID before);
b8               MoveItemUp  (mvItemRegistry& registry, mvUUID uuid);
b8               MoveItemDown(mvItemRegistry& registry, mvUUID uuid);

// item retrieval
mvUUID           GetIDFromPyObject(PyObject* item);
mvAppItem*       GetItem        (mvItemRegistry& registry, mvUUID uuid);
std::shared_ptr<mvAppItem> GetRefItem     (mvItemRegistry& registry, mvUUID uuid);
mvWindowAppItem* GetWindow      (mvItemRegistry& registry, mvUUID uuid);
mvAppItem*       GetItemRoot    (mvItemRegistry& registry, mvUUID uuid);

// item operations
void             DelaySearch             (mvItemRegistry& registry, mvAppItem* item);
b8               AddItemWithRuntimeChecks(mvItemRegistry& registry, std::shared_ptr<mvAppItem> item, mvUUID parent, mvUUID before);
void             ResetTheme              (mvItemRegistry& registry);

//-----------------------------------------------------------------------------
// mvItemRegistry
//     - Responsibilities:
//         * holds widget hierarchy roots (windows)
//         * keeping up with the active window 
//         * managing the parent deduction system (parent stack) 
//         * acts as the interface for accessing widgets
//         * creates the "standard" windows on startup (debug, about, etc.)
//-----------------------------------------------------------------------------

struct mvItemRegistry
{

    static constexpr i32 CachedContainerCount = 25;

    // caching
    mvUUID     lastItemAdded = 0;
    mvUUID     lastContainerAdded = 0;
    mvUUID     lastRootAdded = 0;
    i32        cachedContainerIndex = 0;
    i32        cachedItemsIndex = 0;
    mvUUID     cachedItemsID[CachedContainerCount];
    mvAppItem* cachedItemsPTR[CachedContainerCount];
    mvUUID     cachedContainersID[CachedContainerCount];
    mvAppItem* cachedContainersPTR[CachedContainerCount];

    // misc
    std::stack<mvAppItem*>                  containers;      // parent stack, top of stack becomes widget's parent
    std::unordered_map<std::string, mvUUID> aliases;
    std::vector<mvAppItem*>                 delayedSearch;
    b8                                      showImGuiDebug = false;
    b8                                      showImPlotDebug = false;
    std::vector<std::shared_ptr<mvAppItem>> debugWindows;
    std::shared_ptr<mvAppItem>              capturedItem = nullptr;
    mvPyObjectStrictPtr                     captureCallback = nullptr;
    mvPyObjectStrictPtr                     captureCallbackUserData = nullptr;

    // roots
    std::vector<std::shared_ptr<mvAppItem>> colormapRoots;
    std::vector<std::shared_ptr<mvAppItem>> filedialogRoots;
    std::vector<std::shared_ptr<mvAppItem>> stagingRoots;
    std::vector<std::shared_ptr<mvAppItem>> viewportMenubarRoots;
    std::vector<std::shared_ptr<mvAppItem>> windowRoots;
    std::vector<std::shared_ptr<mvAppItem>> fontRegistryRoots;
    std::vector<std::shared_ptr<mvAppItem>> handlerRegistryRoots;
    std::vector<std::shared_ptr<mvAppItem>> itemHandlerRegistryRoots;
    std::vector<std::shared_ptr<mvAppItem>> textureRegistryRoots;
    std::vector<std::shared_ptr<mvAppItem>> valueRegistryRoots;
    std::vector<std::shared_ptr<mvAppItem>> themeRegistryRoots;
    std::vector<std::shared_ptr<mvAppItem>> itemTemplatesRoots;
    std::vector<std::shared_ptr<mvAppItem>> viewportDrawlistRoots;

    mvItemRegistry();
};