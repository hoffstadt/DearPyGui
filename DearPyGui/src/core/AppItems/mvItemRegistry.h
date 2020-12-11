#pragma once

#include <stack>
#include <vector>
#include <queue>
#include <string>
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

        struct OrderedItem
        {
            mvAppItem* item;   // new item to add
            std::string prev;  // what item to add item after
        };

        struct NewRuntimeItem
        {
            mvAppItem* item;    // new item to add
            std::string before; // what item to add new item before
            std::string parent; // what parent to add item to (if not using before)
        };

        struct StolenChild
        {
            std::string item;   // item to steal
            std::string parent; // what item to add stolen item before
            std::string before; // what parent to add item to (if not using before)
        };

	public:

        mvItemRegistry();
        ~mvItemRegistry();

        //-----------------------------------------------------------------------------
        // Event Handling
        //-----------------------------------------------------------------------------
        bool                     onEvent         (mvEvent& event) override;
        bool                     onDeleteItem    (mvEvent& event);
        bool                     onMoveItem      (mvEvent& event);
        bool                     onMoveItemUp    (mvEvent& event);
        bool                     onMoveItemDown  (mvEvent& event);
        bool                     onEndFrame      (mvEvent& event);
        bool                     onRender        (mvEvent& event);
        bool                     onPreRenderReset(mvEvent& event);                     
        bool                     onActiveWindow  (mvEvent& event);                     

        //-----------------------------------------------------------------------------
        // AppItem Operations
        //-----------------------------------------------------------------------------
        bool                     isItemToBeDeleted (const std::string& name) const;
        bool                     addItem           (mvAppItem* item);
        bool                     addItemAfter      (const std::string& prev, mvAppItem* item);
        bool                     addWindow         (mvAppItem* item);
        bool                     addRuntimeItem    (const std::string& parent, const std::string& before, mvAppItem* item);
        mvAppItem*               getItem           (const std::string& name, bool ignoreRuntime = false);
        mvAppItem*               getItemAsync      (const std::string& name, bool ignoreRuntime = false); // allows item to be retrieved outside main thread
        mvAppItem*               getRuntimeItem    (const std::string& name);
        mvWindowAppItem*         getWindow         (const std::string& name);
        std::vector<mvAppItem*>& getFrontWindows   () { return m_frontWindows; }
        std::vector<mvAppItem*>& getBackWindows    () { return m_backWindows; }
        const std::string&       getActiveWindow() const { return m_activeWindow; }
        bool                     addItemWithRuntimeChecks(mvAppItem* item, const char* parent, const char* before);
        
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
        void                     pushParent  (mvAppItem* item); // pushes parent onto stack
        void                     emptyParents();                // empties parent stack
        mvAppItem*               popParent   ();                // pop parent off stack and return it
        mvAppItem*               topParent   ();                // returns top parent without popping

    private:

        void                     postDeleteItems();
        void                     postAddItems   ();
        void                     postAddPopups  ();
        void                     postMoveItems  ();
        void                     clearRegistry  ();

	private:

		std::stack<mvAppItem*>  m_parents;
		std::vector<mvAppItem*> m_frontWindows;
		std::vector<mvAppItem*> m_backWindows;
        std::string             m_activeWindow;

        // runtime widget modifications
        std::queue<std::string>     m_deleteChildrenQueue;
        std::vector<std::string>    m_deleteQueue;
        std::queue<std::string>     m_upQueue;
        std::queue<std::string>     m_downQueue;
        std::vector<NewRuntimeItem> m_newItemVec;
        std::vector<OrderedItem>    m_orderedVec;
        std::queue<StolenChild>     m_moveVec;

	};

}