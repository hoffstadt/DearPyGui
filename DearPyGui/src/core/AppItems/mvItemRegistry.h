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

    // item pools
    mvRef<mvAppItem> GetItemFromPool(mvItemRegistry& registry, mvAppItemType itemType);

    // item movement
    b8               MoveItem    (mvItemRegistry& registry, mvUUID uuid, mvUUID parent, mvUUID before);
    b8               MoveItemUp  (mvItemRegistry& registry, mvUUID uuid);
    b8               MoveItemDown(mvItemRegistry& registry, mvUUID uuid);

    // item retrieval
    mvUUID           GetIDFromPyObject(PyObject* item);
    mvAppItem*       GetItem        (mvItemRegistry& registry, mvUUID uuid);
    mvRef<mvAppItem> GetRefItem     (mvItemRegistry& registry, mvUUID uuid);
    mvWindowAppItem* GetWindow      (mvItemRegistry& registry, mvUUID uuid);
    mvAppItem*       GetItemRoot    (mvItemRegistry& registry, mvUUID uuid);

    // item operations
    void             DelaySearch             (mvItemRegistry& registry, mvAppItem* item);
    b8               AddItemWithRuntimeChecks(mvItemRegistry& registry, mvRef<mvAppItem> item, mvUUID parent, mvUUID before);
    void             ResetTheme              (mvItemRegistry& registry);
    void             TryBoundTemplateRegistry(mvItemRegistry& registry, mvAppItem* item);

    // retrieves parent, before, uuid from user input when creating an item
    std::tuple<mvUUID, mvUUID, std::string> GetNameFromArgs(mvUUID& name, PyObject* args, PyObject* kwargs);

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
        mvUUID                                  activeWindow = 0;
        std::vector<mvAppItem*>                 delayedSearch;
        b8                                      showImGuiDebug = false;
        b8                                      showImPlotDebug = false;
        mvRef<mvAppItem>                        boundedTemplateRegistry;
        std::vector<mvRef<mvAppItem>>           debugWindows;
        mvRef<mvAppItem>                        capturedItem = nullptr;
        PyObject*                               captureCallback = nullptr;

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
        std::vector<mvRef<mvAppItem>> viewportDrawlistRoots;

        mvItemRegistry()
        {
            // prefill cached containers
            for (i32 i = 0; i < CachedContainerCount; i++)
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

    MV_CREATE_FREE_COMMAND(capture_next_item);
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
    MV_CREATE_FREE_COMMAND(focus_item);
    MV_CREATE_FREE_COMMAND(get_item_info);
    MV_CREATE_FREE_COMMAND(get_item_configuration);
    MV_CREATE_FREE_COMMAND(get_item_state);
    MV_CREATE_FREE_COMMAND(configure_item);
    MV_CREATE_FREE_COMMAND(get_item_types);
    MV_CREATE_FREE_COMMAND(get_value);
    MV_CREATE_FREE_COMMAND(get_values);
    MV_CREATE_FREE_COMMAND(set_value);
    MV_CREATE_FREE_COMMAND(reset_pos);
    MV_CREATE_FREE_COMMAND(set_item_children);
    MV_CREATE_FREE_COMMAND(bind_item_handler_registry);
    MV_CREATE_FREE_COMMAND(bind_item_font);
    MV_CREATE_FREE_COMMAND(bind_item_theme);
    MV_CREATE_FREE_COMMAND(set_item_alias);
    MV_CREATE_FREE_COMMAND(get_item_alias);

    MV_START_FREE_COMMANDS(mvItemRegistryCommands)
        MV_ADD_COMMAND(focus_item);
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
        MV_ADD_COMMAND(get_item_info);
        MV_ADD_COMMAND(set_item_alias);
        MV_ADD_COMMAND(get_item_alias);
        MV_ADD_COMMAND(get_item_types);
        MV_ADD_COMMAND(get_item_configuration);
        MV_ADD_COMMAND(get_item_state);
        MV_ADD_COMMAND(configure_item);
        MV_ADD_COMMAND(get_value);
        MV_ADD_COMMAND(get_values);
        MV_ADD_COMMAND(set_value);
        MV_ADD_COMMAND(reset_pos);
        MV_ADD_COMMAND(set_item_children);
        MV_ADD_COMMAND(bind_item_handler_registry);
        MV_ADD_COMMAND(bind_item_font);
        MV_ADD_COMMAND(bind_item_theme);
        MV_ADD_COMMAND(capture_next_item);
    MV_END_COMMANDS

}
