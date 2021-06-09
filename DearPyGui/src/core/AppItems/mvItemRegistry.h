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

    public:

        static constexpr int CachedContainerCount = 25;

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_CREATE_EXTRA_COMMAND(move_item);
        MV_CREATE_EXTRA_COMMAND(delete_item);
        MV_CREATE_EXTRA_COMMAND(does_item_exist);
        MV_CREATE_EXTRA_COMMAND(move_item_down);
        MV_CREATE_EXTRA_COMMAND(move_item_up);
        MV_CREATE_EXTRA_COMMAND(get_windows);
        MV_CREATE_EXTRA_COMMAND(get_all_items);
        MV_CREATE_EXTRA_COMMAND(get_active_window);
        MV_CREATE_EXTRA_COMMAND(set_primary_window);
        MV_CREATE_EXTRA_COMMAND(push_container_stack);
        MV_CREATE_EXTRA_COMMAND(pop_container_stack);
        MV_CREATE_EXTRA_COMMAND(top_container_stack);
        MV_CREATE_EXTRA_COMMAND(empty_container_stack);
        MV_CREATE_EXTRA_COMMAND(last_item);
        MV_CREATE_EXTRA_COMMAND(last_container);
        MV_CREATE_EXTRA_COMMAND(last_root);
        MV_CREATE_EXTRA_COMMAND(set_staging_mode);
        MV_CREATE_EXTRA_COMMAND(stage_items);
        MV_CREATE_EXTRA_COMMAND(unstage_items);
        MV_CREATE_EXTRA_COMMAND(reorder_items);

        MV_START_EXTRA_COMMANDS
            MV_ADD_EXTRA_COMMAND(move_item);
            MV_ADD_EXTRA_COMMAND(delete_item);
            MV_ADD_EXTRA_COMMAND(does_item_exist);
            MV_ADD_EXTRA_COMMAND(move_item_down);
            MV_ADD_EXTRA_COMMAND(move_item_up);
            MV_ADD_EXTRA_COMMAND(get_windows);
            MV_ADD_EXTRA_COMMAND(get_all_items);
            MV_ADD_EXTRA_COMMAND(get_active_window);
            MV_ADD_EXTRA_COMMAND(set_primary_window);
            MV_ADD_EXTRA_COMMAND(push_container_stack);
            MV_ADD_EXTRA_COMMAND(pop_container_stack);
            MV_ADD_EXTRA_COMMAND(top_container_stack);
            MV_ADD_EXTRA_COMMAND(empty_container_stack);
            MV_ADD_EXTRA_COMMAND(last_item);
            MV_ADD_EXTRA_COMMAND(last_container);
            MV_ADD_EXTRA_COMMAND(last_root);
            MV_ADD_EXTRA_COMMAND(set_staging_mode);
            MV_ADD_EXTRA_COMMAND(stage_items);
            MV_ADD_EXTRA_COMMAND(unstage_items);
            MV_ADD_EXTRA_COMMAND(reorder_items);
        MV_END_EXTRA_COMMANDS

	public:

        mvItemRegistry();

        //-----------------------------------------------------------------------------
        // Event Handling
        //-----------------------------------------------------------------------------
        bool                           onEvent         (mvEvent& event) override;
        bool                           onRender        (mvEvent& event);
        bool                           onPreRenderReset(mvEvent& event);                     
        bool                           onActiveWindow  (mvEvent& event);                     

        //-----------------------------------------------------------------------------
        // Widget Operations
        //-----------------------------------------------------------------------------
        bool                           deleteItem        (mvUUID uuid, bool childrenOnly = false);
        bool                           moveItem          (mvUUID uuid, mvUUID parent, mvUUID before);
        bool                           moveItemUp        (mvUUID uuid);
        bool                           moveItemDown      (mvUUID uuid);
        void                           clearRegistry     ();
        mvValueVariant                 getValue          (mvUUID uuid);
        mvAppItem*                     getItem           (mvUUID uuid);
        mvRef<mvAppItem>               getRefItem        (mvUUID uuid);
        mvWindowAppItem*               getWindow         (mvUUID uuid);
        std::vector<mvRef<mvAppItem>>& getRoots          ()       { return m_roots; }
        mvUUID                         getActiveWindow   () const { return m_activeWindow; }
        bool                           addItemWithRuntimeChecks(mvRef<mvAppItem> item, mvUUID parent, mvUUID before);
        void                           cleanUpItem(mvUUID uuid);
        
        // called by python interface
        std::vector<mvUUID>              getAllItems       ();
        std::vector<mvUUID>              getWindows        ();
        std::vector<std::vector<mvUUID>> getItemChildren   (mvUUID uuid);
        void                            setPrimaryWindow  (mvUUID uuid, bool value);
        void                            stageItem         (mvUUID uuid);
        void                            unstageItem       (mvUUID uuid);
        void                            setStagingMode    (bool value);
        std::unordered_map<mvUUID, mvRef<mvAppItem>>& getStaging() { return m_stagingArea; }

        //-----------------------------------------------------------------------------
        // Parent stack operations
        //     - used for automatic parent deduction
        //-----------------------------------------------------------------------------
        void                           pushParent  (mvAppItem* item); // pushes parent onto stack
        void                           emptyParents();                      // empties parent stack
        mvAppItem*                     popParent   ();                      // pop parent off stack and return it
        mvAppItem*                     topParent   ();                      // returns top parent without popping

    private:

        bool                           addItem       (mvRef<mvAppItem> item);
        bool                           addItemAfter  (mvUUID prev, mvRef<mvAppItem> item); // for popups/tooltips
        bool                           addWindow     (mvRef<mvAppItem> item);
        bool                           addRuntimeItem(mvUUID parent, mvUUID before, mvRef<mvAppItem> item);


	private:

        // caching
        mvUUID                                       m_lastItemAdded = 0;
        mvUUID                                       m_lastContainerAdded = 0;
        mvUUID                                       m_lastRootAdded = 0;
        mvUUID                                       m_cachedItemsID[CachedContainerCount];
        mvAppItem*                                   m_cachedItemsPTR[CachedContainerCount];
        mvUUID                                       m_cachedContainersID[CachedContainerCount];
        mvAppItem*                                   m_cachedContainersPTR[CachedContainerCount];
        int                                          m_cachedContainerIndex = 0;
        int                                          m_cachedItemsIndex = 0;

		std::stack<mvAppItem*>                       m_containers;      // parent stack, top of stack becomes widget's parent
		std::vector<mvRef<mvAppItem>>                m_roots;
        std::unordered_map<mvUUID, mvRef<mvAppItem>> m_stagingArea;
        mvUUID                                       m_activeWindow = 0;
        bool                                         m_staging = false;




	};

}