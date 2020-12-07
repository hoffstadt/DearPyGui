#pragma once

#include <stack>
#include <vector>
#include <queue>
#include <string>
#include "core/mvEvents.h"

namespace Marvel {

    //-----------------------------------------------------------------------------
    // Forward Declarations
    //-----------------------------------------------------------------------------
    class mvAppItem;
    class mvWindowAppitem;

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

        bool onEvent(mvEvent& event) override;

        // mostly for testing new event bus system
        bool onDeleteItem  (mvEvent& event);
        bool onMoveItem    (mvEvent& event);
        bool onMoveItemUp  (mvEvent& event);
        bool onMoveItemDown(mvEvent& event);
        bool onAddItem     (mvEvent& event);

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
        mvWindowAppitem*         getWindow         (const std::string& name);
        std::vector<mvAppItem*>& getFrontWindows        () { return m_frontWindows; }
        std::vector<mvAppItem*>& getBackWindows        () { return m_backWindows; }

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