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
        bool                           deleteItem        (const std::string& name, bool childrenOnly = false);
        bool                           moveItem          (const std::string& name, const std::string& parent, const std::string& before);
        bool                           moveItemUp        (const std::string& name);
        bool                           moveItemDown      (const std::string& name);
        void                           clearRegistry     ();
        mvValueVariant                 getValue          (const std::string& name);
        mvAppItem*                     getItem           (const std::string& name);
        mvRef<mvAppItem>               getRefItem        (const std::string& name);
        mvWindowAppItem*               getWindow         (const std::string& name);
        std::vector<mvRef<mvAppItem>>& getRoots          ()       { return m_roots; }
        const std::string&             getActiveWindow   () const { return m_activeWindow; }
        bool                           addItemWithRuntimeChecks(mvRef<mvAppItem> item, const char* parent, const char* before);
        
        // called by python interface
        std::vector<std::string>              getAllItems       ();
        std::vector<std::string>              getWindows        ();
        std::vector<std::vector<std::string>> getItemChildren   (const std::string& name);
        std::string                           getItemParentName (const std::string& name);
        void                                  setPrimaryWindow  (const std::string& name, bool value);
        void                                  stageItem         (const std::string& name);
        void                                  unstageItem       (const std::string& name);
        void                                  setStagingMode    (bool value);

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
        bool                           addItemAfter  (const std::string& prev, mvRef<mvAppItem> item); // for popups/tooltips
        bool                           addWindow     (mvRef<mvAppItem> item);
        bool                           addRuntimeItem(const std::string& parent, const std::string& before, mvRef<mvAppItem> item);


	private:

		std::stack<mvAppItem*>                            m_containers;      // parent stack, top of stack becomes widget's parent
		std::vector<mvRef<mvAppItem>>                     m_roots;
        std::unordered_map<std::string, mvRef<mvAppItem>> m_stagingArea;
        std::string                                       m_activeWindow;
        bool                                              m_staging = false;
        std::string                                       m_lastItemAdded;
        std::string                                       m_lastContainerAdded;
        std::string                                       m_lastRootAdded;


	};

}