#pragma once

#include <stack>
#include <vector>
#include <unordered_map>
#include <queue>
#include <string>
#include <mutex>
#include <map>
#include "mvEvents.h"
#include "mvAppItem.h"
#include "cpp.hint"
#include "mvPyObject.h"

namespace Marvel {

    //-----------------------------------------------------------------------------
    // Forward Declarations
    //-----------------------------------------------------------------------------
    class mvAppItem;
    class mvWindowAppItem;

    //-----------------------------------------------------------------------------
    // mvItemRegistry
    //     - Responsibilities:
    //         * holds widget hierarchy roots (windows)
    //         * keeping up with the active window 
    //         * managing the parent deduction system (parent stack) 
    //         * acts as the interface for accessing widgets
    //         * creates the "standard" windows on startup (debug, about, etc.)
    //-----------------------------------------------------------------------------
	class mvItemRegistry : public mvEventHandler
	{

        friend class mvApp;
        friend class mvLayoutWindow;

    public:

        static constexpr int CachedContainerCount = 25;

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_CREATE_COMMAND(move_item);
        MV_CREATE_COMMAND(delete_item);
        MV_CREATE_COMMAND(does_item_exist);
        MV_CREATE_COMMAND(move_item_down);
        MV_CREATE_COMMAND(move_item_up);
        MV_CREATE_COMMAND(get_windows);
        MV_CREATE_COMMAND(get_all_items);
        MV_CREATE_COMMAND(get_active_window);
        MV_CREATE_COMMAND(set_primary_window);
        MV_CREATE_COMMAND(push_container_stack);
        MV_CREATE_COMMAND(pop_container_stack);
        MV_CREATE_COMMAND(top_container_stack);
        MV_CREATE_COMMAND(empty_container_stack);
        MV_CREATE_COMMAND(last_item);
        MV_CREATE_COMMAND(last_container);
        MV_CREATE_COMMAND(last_root);
        MV_CREATE_COMMAND(unstage);
        MV_CREATE_COMMAND(reorder_items);
        MV_CREATE_COMMAND(show_imgui_demo);
        MV_CREATE_COMMAND(show_implot_demo);
        MV_CREATE_COMMAND(add_alias);
        MV_CREATE_COMMAND(remove_alias);
        MV_CREATE_COMMAND(does_alias_exist);
        MV_CREATE_COMMAND(get_alias_id);
        MV_CREATE_COMMAND(get_aliases);
        MV_CREATE_COMMAND(bind_template_registry);

        MV_START_COMMANDS
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
        MV_END_COMMANDS

	public:

        mvItemRegistry();

        //-----------------------------------------------------------------------------
        // Event Handling
        //-----------------------------------------------------------------------------
        bool onEvent         (mvEvent& event) override;
        bool onRender        (mvEvent& event);                 
        bool onActiveWindow  (mvEvent& event);                     

        //-----------------------------------------------------------------------------
        // Widget Operations
        //-----------------------------------------------------------------------------
        bool                           focusItem         (mvUUID uuid);
        bool                           deleteItem        (mvUUID uuid, bool childrenOnly = false, int slot = -1);
        bool                           moveItem          (mvUUID uuid, mvUUID parent, mvUUID before);
        bool                           moveItemUp        (mvUUID uuid);
        bool                           moveItemDown      (mvUUID uuid);
        void                           clearRegistry     ();
        mvAppItem*                     getItem           (mvUUID uuid);
        mvRef<mvAppItem>               getRefItem        (mvUUID uuid);
        mvWindowAppItem*               getWindow         (mvUUID uuid);
        std::vector<mvRef<mvAppItem>>& getFontRegistries ()       { return _fontRegistryRoots; }
        std::vector<mvRef<mvAppItem>>& getStaging ()       { return _stagingRoots; }
        mvUUID                         getActiveWindow   () const { return _activeWindow; }
        bool                           addItemWithRuntimeChecks(mvRef<mvAppItem> item, mvUUID parent, mvUUID before);
        void                           cacheItem(mvAppItem* item);
        void                           cleanUpItem(mvUUID uuid);

        //-----------------------------------------------------------------------------
        // Pools and Config
        //-----------------------------------------------------------------------------
        std::vector<mvRef<mvAppItem>>& getItemPools() { return _itemPoolRoots; }
        mvRef<mvAppItem>               getItemFromPool(mvAppItemType itemType);
        bool                           skipRequiredArgs() const { return _skipRequiredArgs; }
        bool                           skipPositionalArgs() const { return _skipPositionalArgs; }
        bool                           skipKeywordArgs() const { return _skipKeywordArgs; }
        void                           tryBoundTemplateRegistry(mvAppItem* item) const;

        //-----------------------------------------------------------------------------
        // Aliases
        //-----------------------------------------------------------------------------
        void addAlias        (const std::string& alias, mvUUID id);
        void removeAlias     (const std::string& alias, bool itemTriggered = false);
        bool doesAliasExist  (const std::string& alias);
        mvUUID getIdFromAlias(const std::string& alias);
        
        // called by python interface
        std::vector<mvUUID>              getAllItems       ();
        std::vector<mvUUID>              getWindows        ();
        std::vector<std::vector<mvUUID>> getItemChildren   (mvUUID uuid);
        void                             setPrimaryWindow  (mvUUID uuid, bool value);
        void                             unstageItem       (mvUUID uuid);

        // hacky
        void delaySearch(mvAppItem* item);

        //-----------------------------------------------------------------------------
        // Parent stack operations
        //     - used for automatic parent deduction
        //-----------------------------------------------------------------------------
        void       pushParent  (mvAppItem* item); // pushes parent onto stack
        void       emptyParents();                      // empties parent stack
        mvAppItem* popParent   ();                      // pop parent off stack and return it
        mvAppItem* topParent   ();                      // returns top parent without popping

    private:

        bool addItem       (mvRef<mvAppItem> item);
        bool addItemAfter  (mvUUID prev, mvRef<mvAppItem> item); // for popups/tooltips
        bool addRoot       (mvRef<mvAppItem> item);
        bool addRuntimeItem(mvUUID parent, mvUUID before, mvRef<mvAppItem> item);

        // need to clean this up but this delegates taskes to the specific root types
        bool deleteRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID uuid);
        bool moveRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID uuid, mvRef<mvAppItem>& item);
        bool moveUpRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID uuid);
        bool moveDownRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID uuid);
        bool addRuntimeChildRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID parent, mvUUID before, mvRef<mvAppItem> item);
        bool addItemAfterRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID prev, mvRef<mvAppItem> item);
        mvAppItem* getItemRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID uuid);
        mvRef<mvAppItem> getRefItemRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID uuid);
        void getAllItemsRoot(std::vector<mvRef<mvAppItem>>& roots, std::vector<mvUUID>& childList);
	
private:

        // caching
        mvUUID                                       _lastItemAdded = 0;
        mvUUID                                       _lastContainerAdded = 0;
        mvUUID                                       _lastRootAdded = 0;
        mvUUID                                       _cachedItemsID[CachedContainerCount];
        mvAppItem*                                   _cachedItemsPTR[CachedContainerCount];
        mvUUID                                       _cachedContainersID[CachedContainerCount];
        mvAppItem*                                   _cachedContainersPTR[CachedContainerCount];
        int                                          _cachedContainerIndex = 0;
        int                                          _cachedItemsIndex = 0;

		std::stack<mvAppItem*>                       _containers;      // parent stack, top of stack becomes widget's parent
        std::unordered_map<std::string, mvUUID>      _aliases;
        mvUUID                                       _activeWindow = 0;
        std::vector<mvAppItem*>                      _delayedSearch;
        bool                                         _showImGuiDebug = false;
        bool                                         _showImPlotDebug = false;

        // roots
        std::vector<mvRef<mvAppItem>> _colormapRoots;
        std::vector<mvRef<mvAppItem>> _filedialogRoots;
        std::vector<mvRef<mvAppItem>> _stagingRoots;
        std::vector<mvRef<mvAppItem>> _viewportMenubarRoots;
        std::vector<mvRef<mvAppItem>> _windowRoots;
        std::vector<mvRef<mvAppItem>> _fontRegistryRoots;
        std::vector<mvRef<mvAppItem>> _handlerRegistryRoots;
        std::vector<mvRef<mvAppItem>> _textureRegistryRoots;
        std::vector<mvRef<mvAppItem>> _valueRegistryRoots;
        std::vector<mvRef<mvAppItem>> _themeRegistryRoots;
        std::vector<mvRef<mvAppItem>> _itemPoolRoots;
        std::vector<mvRef<mvAppItem>> _itemTemplatesRoots;

        // bound registries
        mvRef<mvAppItem> _boundedTemplateRegistry;

        // user config
        bool _allowAliasOverwrites = false;
        bool _manualAliasManagement = false;
        bool _skipRequiredArgs = false;
        bool _skipPositionalArgs = false;
        bool _skipKeywordArgs = false;
	};

}