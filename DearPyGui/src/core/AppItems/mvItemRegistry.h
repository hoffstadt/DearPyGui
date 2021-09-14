#pragma once

#include <stack>
#include <vector>
#include <unordered_map>
#include <queue>
#include <string>
#include <mutex>
#include <map>
#include "mvAppItem.h"
#include "cpp.hint"
#include "mvPyObject.h"

namespace Marvel {

    //-----------------------------------------------------------------------------
    // forward declarations
    //-----------------------------------------------------------------------------
    struct mvItemRegistry;

    //-----------------------------------------------------------------------------
    // public API
    //-----------------------------------------------------------------------------

    void                             RenderItemRegistry(mvItemRegistry& registry);

    // cleanup
    void                             ClearItemRegistry(mvItemRegistry& registry);
    void                             CleanUpItem      (mvItemRegistry& registry, mvUUID uuid);
    bool                             DeleteItem       (mvItemRegistry& registry, mvUUID uuid, bool childrenOnly = false, int slot = -1);

    // aliases
    void                             AddAlias      (mvItemRegistry& registry, const std::string& alias, mvUUID id);
    void                             RemoveAlias   (mvItemRegistry& registry, const std::string& alias, bool itemTriggered = false);
    bool                             DoesAliasExist(mvItemRegistry& registry, const std::string& alias);
    mvUUID                           GetIdFromAlias(mvItemRegistry& registry, const std::string& alias);

    // item pools
    mvRef<mvAppItem>                 GetItemFromPool(mvItemRegistry& registry, mvAppItemType itemType);

    // item movement
    bool                             MoveItem    (mvItemRegistry& registry, mvUUID uuid, mvUUID parent, mvUUID before);
    bool                             MoveItemUp  (mvItemRegistry& registry, mvUUID uuid);
    bool                             MoveItemDown(mvItemRegistry& registry, mvUUID uuid);

    // item retrieval
    mvAppItem*                       GetItem        (mvItemRegistry& registry, mvUUID uuid);
    mvRef<mvAppItem>                 GetRefItem     (mvItemRegistry& registry, mvUUID uuid);
    mvWindowAppItem*                 GetWindow      (mvItemRegistry& registry, mvUUID uuid);
    std::vector<std::vector<mvUUID>> GetItemChildren(mvItemRegistry& registry, mvUUID uuid);

    // item operations
    bool                             FocusItem               (mvItemRegistry& registry, mvUUID uuid);
    void                             DelaySearch             (mvItemRegistry& registry, mvAppItem* item);
    bool                             AddItemWithRuntimeChecks(mvItemRegistry& registry, mvRef<mvAppItem> item, mvUUID parent, mvUUID before);
    void                             ResetTheme              (mvItemRegistry& registry);
    void                             AddDebugWindow          (mvItemRegistry& registry, mvRef<mvAppItem> item);
    void                             RemoveDebugWindow       (mvItemRegistry& registry, mvUUID uuid);
    void                             TryBoundTemplateRegistry(mvItemRegistry& registry, mvAppItem* item);

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

        static constexpr int CachedContainerCount = 25;

        // python user config
        bool allowAliasOverwrites = false;
        bool manualAliasManagement = false;
        bool skipRequiredArgs = false;
        bool skipPositionalArgs = false;
        bool skipKeywordArgs = false;

        // caching
        mvUUID     lastItemAdded = 0;
        mvUUID     lastContainerAdded = 0;
        mvUUID     lastRootAdded = 0;
        int        cachedContainerIndex = 0;
        int        cachedItemsIndex = 0;
        mvUUID     cachedItemsID[CachedContainerCount];
        mvAppItem* cachedItemsPTR[CachedContainerCount];
        mvUUID     cachedContainersID[CachedContainerCount];
        mvAppItem* cachedContainersPTR[CachedContainerCount];

        // misc
        std::stack<mvAppItem*>                  containers;      // parent stack, top of stack becomes widget's parent
        std::unordered_map<std::string, mvUUID> aliases;
        mvUUID                                  activeWindow = 0;
        std::vector<mvAppItem*>                 delayedSearch;
        bool                                    showImGuiDebug = false;
        bool                                    showImPlotDebug = false;
        mvRef<mvAppItem>                        boundedTemplateRegistry;
        std::vector<mvRef<mvAppItem>>           debugWindows;

        // roots
        std::vector<mvRef<mvAppItem>> colormapRoots;
        std::vector<mvRef<mvAppItem>> filedialogRoots;
        std::vector<mvRef<mvAppItem>> stagingRoots;
        std::vector<mvRef<mvAppItem>> viewportMenubarRoots;
        std::vector<mvRef<mvAppItem>> windowRoots;
        std::vector<mvRef<mvAppItem>> fontRegistryRoots;
        std::vector<mvRef<mvAppItem>> handlerRegistryRoots;
        std::vector<mvRef<mvAppItem>> itemHandlerRegistryRoots;
        std::vector<mvRef<mvAppItem>> textureRegistryRoots;
        std::vector<mvRef<mvAppItem>> valueRegistryRoots;
        std::vector<mvRef<mvAppItem>> themeRegistryRoots;
        std::vector<mvRef<mvAppItem>> itemPoolRoots;
        std::vector<mvRef<mvAppItem>> itemTemplatesRoots;

        mvItemRegistry()
        {
            // prefill cached containers
            for (int i = 0; i < CachedContainerCount; i++)
            {
                cachedContainersID[i] = 0;
                cachedContainersPTR[i] = nullptr;
                cachedItemsID[i] = 0;
                cachedItemsPTR[i] = nullptr;
            }
        }
    };

    //-----------------------------------------------------------------------------
    // Python Parsing
    //-----------------------------------------------------------------------------

    void InsertParser_mvItemRegistry(std::map<std::string, mvPythonParser>* parsers);

    MV_CREATE_FREE_COMMAND(move_item);
    MV_CREATE_FREE_COMMAND(delete_item);
    MV_CREATE_FREE_COMMAND(does_item_exist);
    MV_CREATE_FREE_COMMAND(move_item_down);
    MV_CREATE_FREE_COMMAND(move_item_up);
    MV_CREATE_FREE_COMMAND(get_windows);
    MV_CREATE_FREE_COMMAND(get_all_items);
    MV_CREATE_FREE_COMMAND(get_active_window);
    MV_CREATE_FREE_COMMAND(set_primary_window);
    MV_CREATE_FREE_COMMAND(push_container_stack);
    MV_CREATE_FREE_COMMAND(pop_container_stack);
    MV_CREATE_FREE_COMMAND(top_container_stack);
    MV_CREATE_FREE_COMMAND(empty_container_stack);
    MV_CREATE_FREE_COMMAND(last_item);
    MV_CREATE_FREE_COMMAND(last_container);
    MV_CREATE_FREE_COMMAND(last_root);
    MV_CREATE_FREE_COMMAND(unstage);
    MV_CREATE_FREE_COMMAND(reorder_items);
    MV_CREATE_FREE_COMMAND(show_imgui_demo);
    MV_CREATE_FREE_COMMAND(show_implot_demo);
    MV_CREATE_FREE_COMMAND(add_alias);
    MV_CREATE_FREE_COMMAND(remove_alias);
    MV_CREATE_FREE_COMMAND(does_alias_exist);
    MV_CREATE_FREE_COMMAND(get_alias_id);
    MV_CREATE_FREE_COMMAND(get_aliases);
    MV_CREATE_FREE_COMMAND(bind_template_registry);
    MV_CREATE_FREE_COMMAND(show_item_debug);

    MV_START_FREE_COMMANDS(mvItemRegistryCommands)
        MV_ADD_COMMAND(bind_template_registry);
        MV_ADD_COMMAND(get_aliases);
        MV_ADD_COMMAND(add_alias);
        MV_ADD_COMMAND(remove_alias);
        MV_ADD_COMMAND(does_alias_exist);
        MV_ADD_COMMAND(get_alias_id);
        MV_ADD_COMMAND(move_item);
        MV_ADD_COMMAND(delete_item);
        MV_ADD_COMMAND(does_item_exist);
        MV_ADD_COMMAND(move_item_down);
        MV_ADD_COMMAND(move_item_up);
        MV_ADD_COMMAND(get_windows);
        MV_ADD_COMMAND(get_all_items);
        MV_ADD_COMMAND(get_active_window);
        MV_ADD_COMMAND(set_primary_window);
        MV_ADD_COMMAND(push_container_stack);
        MV_ADD_COMMAND(pop_container_stack);
        MV_ADD_COMMAND(top_container_stack);
        MV_ADD_COMMAND(empty_container_stack);
        MV_ADD_COMMAND(last_item);
        MV_ADD_COMMAND(last_container);
        MV_ADD_COMMAND(last_root);
        MV_ADD_COMMAND(unstage);
        MV_ADD_COMMAND(reorder_items);
        MV_ADD_COMMAND(show_imgui_demo);
        MV_ADD_COMMAND(show_implot_demo);
        MV_ADD_COMMAND(show_item_debug);
    MV_END_COMMANDS

}