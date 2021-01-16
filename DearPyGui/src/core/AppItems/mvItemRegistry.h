#pragma once

#include <stack>
#include <vector>
#include <queue>
#include <string>
#include <mutex>
#include "mvEvents.h"

namespace Marvel {

    //-----------------------------------------------------------------------------
    // Forward Declarations
    //-----------------------------------------------------------------------------
    class mvAppItem;
    class mvWindowAppItem;

    //-----------------------------------------------------------------------------
    // mvItemRegistry
    //-----------------------------------------------------------------------------
	class mvItemRegistry : public mvEventHandler
	{

        friend class mvApp;

	public:

        mvItemRegistry();
        ~mvItemRegistry();

        bool deleteItem  (const std::string& name, bool childrenOnly = false);
        bool moveItem    (const std::string& name, const std::string& parent, const std::string& before);
        bool moveItemUp  (const std::string& name);
        bool moveItemDown(const std::string& name);

        //-----------------------------------------------------------------------------
        // Event Handling
        //-----------------------------------------------------------------------------
        bool                     onEvent         (mvEvent& event) override;
        bool                     onRender        (mvEvent& event);
        bool                     onPreRenderReset(mvEvent& event);                     
        bool                     onActiveWindow  (mvEvent& event);                     

        //-----------------------------------------------------------------------------
        // AppItem Operations
        //-----------------------------------------------------------------------------
        mvRef<mvAppItem>               getItem           (const std::string& name);
        mvWindowAppItem*               getWindow         (const std::string& name);
        std::vector<mvRef<mvAppItem>>& getFrontWindows   () { return m_frontWindows; }
        std::vector<mvRef<mvAppItem>>& getBackWindows    () { return m_backWindows; }
        const std::string&             getActiveWindow() const { return m_activeWindow; }
        bool                           addItemWithRuntimeChecks(mvRef<mvAppItem> item, const char* parent, const char* before);
        
        // called by python interface
        std::vector<std::string> getAllItems       ();
        std::vector<std::string> getWindows        ();
        std::vector<std::string> getItemChildren   (const std::string& name);
        std::string              getItemParentName (const std::string& name);
        void                     setPrimaryWindow  (const std::string& name, bool value);

        //-----------------------------------------------------------------------------
        // Parent stack operations
        //     - used for automatic parent deduction
        //-----------------------------------------------------------------------------
        void                     pushParent  (mvRef<mvAppItem> item); // pushes parent onto stack
        void                     emptyParents();                // empties parent stack
        mvRef<mvAppItem>         popParent   ();                // pop parent off stack and return it
        mvRef<mvAppItem>         topParent   ();                // returns top parent without popping

    private:

        void                     clearRegistry();
        bool                     addItem(mvRef<mvAppItem> item);
        bool                     addItemAfter(const std::string& prev, mvRef<mvAppItem> item);
        bool                     addWindow(mvRef<mvAppItem> item);
        bool                     addRuntimeItem(const std::string& parent, const std::string& before, mvRef<mvAppItem> item);


	private:

		std::stack<mvRef<mvAppItem>>  m_parents;
		std::vector<mvRef<mvAppItem>> m_frontWindows;
		std::vector<mvRef<mvAppItem>> m_backWindows;
        std::string                   m_activeWindow;

	};

}