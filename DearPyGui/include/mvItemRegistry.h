#pragma once

#include <stack>
#include <vector>
#include <queue>
#include <string>

namespace Marvel {

    //-----------------------------------------------------------------------------
    // Forward Declarations
    //-----------------------------------------------------------------------------
    class mvAppItem;
    class mvWindowAppitem;

    //-----------------------------------------------------------------------------
    // mvItemRegistry
    //-----------------------------------------------------------------------------
	class mvItemRegistry
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

        //-----------------------------------------------------------------------------
        // AppItem Operations
        //-----------------------------------------------------------------------------
        bool                     addItem           (mvAppItem* item);
        bool                     addItemAfter      (const std::string& prev, mvAppItem* item);
        bool                     addWindow         (mvAppItem* item);
        bool                     addRuntimeItem    (const std::string& parent, const std::string& before, mvAppItem* item);
        bool                     moveItem          (const std::string& name, const std::string& parent, const std::string& before);
        void                     deleteItem        (const std::string& name);
        void                     deleteItemChildren(const std::string& name);
        void                     moveItemUp        (const std::string& name);
        void                     moveItemDown      (const std::string& name);
        mvAppItem*               getItem           (const std::string& name, bool ignoreRuntime = false);
        mvAppItem*               getItemAsync      (const std::string& name, bool ignoreRuntime = false); // allows item to be retrieved outside main thread
        mvAppItem*               getRuntimeItem    (const std::string& name);
        mvWindowAppitem*         getWindow         (const std::string& name);
        std::vector<mvAppItem*>& getWindows        () { return m_windows; }

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
		std::vector<mvAppItem*> m_windows;

        // runtime widget modifications
        std::queue<std::string>     m_deleteChildrenQueue;
        std::queue<std::string>     m_deleteQueue;
        std::queue<std::string>     m_upQueue;
        std::queue<std::string>     m_downQueue;
        std::vector<NewRuntimeItem> m_newItemVec;
        std::vector<OrderedItem>    m_orderedVec;
        std::queue<StolenChild>     m_moveVec;

	};

}