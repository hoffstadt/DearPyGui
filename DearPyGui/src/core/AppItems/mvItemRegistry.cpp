#include "mvItemRegistry.h"
#include "mvProfiler.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvAppItemCommons.h"
#include "mvLog.h"
#include "mvPythonExceptions.h"
#include "mvToolManager.h"

namespace Marvel {

	void mvItemRegistry::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		{
			mvPythonParser parser(mvPyDataType::UUID, "Undocumented", { "Item Registry" });
			parser.finalize();
			parsers->insert({ "pop_container_stack", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Undocumented", { "Item Registry" });
			parser.finalize();
			parsers->insert({ "show_imgui_demo", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Undocumented", { "Item Registry" });
			parser.finalize();
			parsers->insert({ "show_implot_demo", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Undocumented", { "Item Registry" });
			parser.addArg<mvPyDataType::UUID>("container");
			parser.addArg<mvPyDataType::Integer>("slot");
			parser.addArg<mvPyDataType::UUIDList>("new_order");
			parser.finalize();
			parsers->insert({ "reorder_items", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Undocumented", { "Item Registry" });
			parser.addArg<mvPyDataType::UUIDList>("items");
			parser.finalize();
			parsers->insert({ "unstage_items", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Undocumented", { "Item Registry" });
			parser.addArg<mvPyDataType::UUIDList>("items");
			parser.finalize();
			parsers->insert({ "stage_items", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::Bool, "Undocumented", { "Item Registry" });
			parser.addArg<mvPyDataType::UUID>("item");
			parser.finalize();
			parsers->insert({ "push_container_stack", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::UUID, "Undocumented", { "Item Registry" });
			parser.finalize();
			parsers->insert({ "top_container_stack", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::UUID, "Undocumented", { "Item Registry" });
			parser.finalize();
			parsers->insert({ "last_item", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::UUID, "Undocumented", { "Item Registry" });
			parser.finalize();
			parsers->insert({ "last_container", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::UUID, "Undocumented", { "Item Registry" });
			parser.finalize();
			parsers->insert({ "last_root", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Undocumented", { "Item Registry" });
			parser.finalize();
			parsers->insert({ "empty_container_stack", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Undocumented", { "Item Registry" });
			parser.addArg<mvPyDataType::UUID>("item");
			parser.addArg<mvPyDataType::UUID>("parent", mvArgType::KEYWORD_ARG, "0");
			parser.addArg<mvPyDataType::UUID>("before", mvArgType::KEYWORD_ARG, "0");
			parser.finalize();
			parsers->insert({ "move_item", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::UUIDList, "Undocumented", { "Item Registry" });
			parser.finalize();
			parsers->insert({ "get_windows", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::UUIDList, "Undocumented", { "Item Registry" });
			parser.finalize();
			parsers->insert({ "get_all_items", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Undocumented", { "Item Registry" });
			parser.addArg<mvPyDataType::Bool>("mode");
			parser.finalize();
			parsers->insert({ "set_staging_mode", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Undocumented", { "Item Registry" });
			parser.addArg<mvPyDataType::UUID>("item");
			parser.addArg<mvPyDataType::Bool>("children_only", mvArgType::KEYWORD_ARG, "False");
			parser.addArg<mvPyDataType::Integer>("slot", mvArgType::KEYWORD_ARG, "-1");
			parser.finalize();
			parsers->insert({ "delete_item", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::Bool, "Undocumented", { "Item Registry" });
			parser.addArg<mvPyDataType::UUID>("item");
			parser.finalize();
			parsers->insert({ "does_item_exist", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Undocumented", { "Item Registry" });
			parser.addArg<mvPyDataType::UUID>("item");
			parser.finalize();
			parsers->insert({ "move_item_up", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Undocumented", { "Item Registry" });
			parser.addArg<mvPyDataType::UUID>("item");
			parser.finalize();
			parsers->insert({ "move_item_down", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::UUID, "Undocumented", { "Item Registry" });
			parser.finalize();
			parsers->insert({ "get_active_window", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Undocumented", { "Item Registry" });
			parser.addArg<mvPyDataType::UUID>("window");
			parser.addArg<mvPyDataType::Bool>("value");
			parser.finalize();
			parsers->insert({ "set_primary_window", parser });
		}

	}

	mvItemRegistry::mvItemRegistry()
	{
		mvEventBus::Subscribe(this, 0, mvEVT_CATEGORY_ITEM);
		mvEventBus::Subscribe(this, mvEVT_PRE_RENDER_RESET);
		mvEventBus::Subscribe(this, mvEVT_RENDER);
		mvEventBus::Subscribe(this, mvEVT_ACTIVE_WINDOW);

		// prefill cached containers
		for (int i = 0; i < CachedContainerCount; i++)
		{
			_cachedContainersID[i] = 0;
			_cachedContainersPTR[i] = nullptr;
			_cachedItemsID[i] = 0;
			_cachedItemsPTR[i] = nullptr;
		}
		
	}

	mvValueVariant mvItemRegistry::getValue(mvUUID uuid)
	{
		mvAppItem* item = getItem(uuid);
		if (item)
			return item->getValue();
		return nullptr;
	}

	bool mvItemRegistry::focusItem(mvUUID uuid)
	{

		if (!mvApp::IsAppStarted())
		{
			for (size_t i = 0; i < _roots.size(); i++)
			{
				if (_roots[i]->getUUID() == uuid)
				{
					mvRef<mvAppItem> oldItem = _roots.back();
					_roots[_roots.size() - 1] = _roots[i];
					_roots[i] = oldItem;
					return true;
				}
			}
		}

		auto appitem = getItem(uuid);

		if (appitem)
		{
			appitem->focus();
			if (auto parent = appitem->getRoot())
				parent->focus();
			return true;
		}
		else
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "focus_item",
				"Item not found: " + std::to_string(uuid), nullptr);
		return false;
	}

	bool mvItemRegistry::deleteItem(mvUUID uuid, bool childrenOnly, int slot)
	{

		MV_ITEM_REGISTRY_TRACE("Attempting to delete: " + std::to_string(uuid));

		cleanUpItem(uuid);

		// check staging first
		if (_stagingArea.count(uuid) != 0)
		{
			if (childrenOnly)
				_stagingArea[uuid]->deleteChildren(slot);
			else
				_stagingArea.erase(uuid);
			MV_ITEM_REGISTRY_INFO(std::to_string(uuid) + " found and deleted.");
			return true;
		}

		// delete item's children only
		if(childrenOnly)
		{
			auto item = getItem(uuid);
			if (item)
			{
				item->deleteChildren(slot);
				MV_ITEM_REGISTRY_INFO("Item found and it's children deleted.");
				return true;
			}
		}

		bool deletedItem = false;

		// try to delete build-in item
		for (auto& window : _roots)
		{
			deletedItem = window->deleteChild(uuid);
			if (deletedItem)
				break;
		}

		bool rootDeleting = false;

		// check if attempting to delete a window
		for (auto& window : _roots)
		{
			if (window->_uuid == uuid)
			{
				rootDeleting = true;
				break;
			}
		}

		// delete window and update window vector
		// this should be changed to a different data
		// structure
		if (rootDeleting)
		{
			std::vector<mvRef<mvAppItem>> oldwindows = _roots;

			_roots.clear();

			for (auto& window : oldwindows)
			{
				if (window->_uuid == uuid)
				{
					deletedItem = true;
					continue;
				}
				_roots.push_back(window);
			}
		}

		if (deletedItem)
		{
			MV_ITEM_REGISTRY_INFO(std::to_string(uuid) + " found and deleted.");
		}
		else
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "delete_item",
				"Item not found: " + std::to_string(uuid), nullptr);

		assert(deletedItem && "Item to delete not found");
		return deletedItem;
	}

	bool mvItemRegistry::moveItem(mvUUID uuid, mvUUID parent, mvUUID before)
	{

		MV_ITEM_REGISTRY_TRACE("Attempting to move: " + std::to_string(uuid));

		mvRef<mvAppItem> child = nullptr;

		bool movedItem = false;

		for (auto& window : _roots)
		{
			child = window->stealChild(uuid);
			if (child)
			{
				movedItem = true;
				break;
			}
		}

		if (_stagingArea.count(uuid) != 0)
		{
			child = _stagingArea[uuid];
			_stagingArea.erase(uuid);
		}

		if (child == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "move_item",
				"Item not found: " + std::to_string(uuid), nullptr);
			MV_ITEM_REGISTRY_WARN("Could not move item, it was not found");
		}


		if (child)
			addRuntimeItem(parent, before, child);


		return movedItem;
	}

	bool mvItemRegistry::moveItemUp(mvUUID uuid)
	{

		MV_ITEM_REGISTRY_TRACE("Attempting to move: " + std::to_string(uuid));

		bool movedItem = false;

		for (auto window : _roots)
		{
			movedItem = window->moveChildUp(uuid);
			if (movedItem)
				break;
		}

		if (!movedItem)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "move_item",
				"Item not found: " + std::to_string(uuid), nullptr);
			MV_ITEM_REGISTRY_WARN("Could not move item, it was not found");
		}

		assert(movedItem && "Item to move not found");

		return movedItem;
	}

	bool mvItemRegistry::moveItemDown(mvUUID uuid)
	{

		MV_ITEM_REGISTRY_TRACE("Attempting to move: " + std::to_string(uuid));

		bool movedItem = false;

		for (auto window : _roots)
		{
			movedItem = window->moveChildDown(uuid);
			if (movedItem)
				break;
		}

		if (!movedItem)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "move_item",
				"Item not found: " + std::to_string(uuid), nullptr);
			MV_ITEM_REGISTRY_WARN("Could not move item, it was not found");
		}

		assert(movedItem && "Item to move not found");

		return movedItem;
	}

	bool mvItemRegistry::onEvent(mvEvent& event)
	{
		mvEventDispatcher dispatcher(event);

		dispatcher.dispatch(BIND_EVENT_METH(mvItemRegistry::onPreRenderReset), mvEVT_PRE_RENDER_RESET);
		dispatcher.dispatch(BIND_EVENT_METH(mvItemRegistry::onRender), mvEVT_RENDER);
		dispatcher.dispatch(BIND_EVENT_METH(mvItemRegistry::onActiveWindow), mvEVT_ACTIVE_WINDOW);

		return event.handled;
	}

	bool mvItemRegistry::onRender(mvEvent& event)
	{

		MV_PROFILE_SCOPE("Rendering")

		if(_showImGuiDebug)
			ImGui::ShowDemoWindow(&_showImGuiDebug);
		if(_showImPlotDebug)
			ImPlot::ShowDemoWindow(&_showImPlotDebug);

		// todo: roots really needs to be split
		for (auto& root : _roots)
		{
			if (!root->preDraw())
				continue;

			if (root->isAltCustomActionRequested())
				root->alternativeCustomAction();

			if(root->_show || mvAppItem::DoesItemHaveFlag(root.get(), MV_ITEM_DESC_ALWAYS_DRAW) || root->getType() == mvAppItemType::mvWindowAppItem)
				root->draw(nullptr, 0.0f, 0.0f);

			root->postDraw();
		}

		return false;
	}

	bool mvItemRegistry::onPreRenderReset(mvEvent& event)
	{

		// resets app items states (i.e. hovered)
		for (auto& window : _roots)
			window->resetState();

		return false;
	}

	bool mvItemRegistry::onActiveWindow(mvEvent& event)
	{

		_activeWindow = GetEUUID(event, "WINDOW");

		MV_ITEM_REGISTRY_INFO("Active window changed to: " + std::to_string(_activeWindow));

		return false;
	}

	bool mvItemRegistry::addRuntimeItem(mvUUID parent, mvUUID before, mvRef<mvAppItem> item)
	{
		//MV_ITEM_REGISTRY_TRACE("Attempting to add new widget: ", item->_name);


		if (_stagingArea.count(parent) != 0)
		{
			_stagingArea[parent]->addItem(item);
			return true;
		}

		else if (_stagingArea.count(before) != 0)
		{
			_stagingArea[before]->addItem(item);
			return true;
		}

		if (_staging)
		{
			for (auto& stagingItem : _stagingArea)
			{
				if (stagingItem.second->addRuntimeChild(parent, before, item))
					return true;
			}

		}

		for (auto& window : _roots)
		{
			if(window->addRuntimeChild(parent, before, item))
				return true;
		}

		return false;
	}

	bool mvItemRegistry::addItemAfter(mvUUID prev, mvRef<mvAppItem> item)
	{
		//MV_ITEM_REGISTRY_TRACE("Attempting to add new widget after: ", item->_name);

		bool addedItem = false;

		for (auto& window : _roots)
		{
			addedItem = window->addChildAfter(prev, item);
			if (addedItem)
				break;
		}

		assert(addedItem);
		return addedItem;
	}

	void mvItemRegistry::pushParent(mvAppItem* item)
	{
		_containers.push(item);
	}

	void mvItemRegistry::setStagingMode(bool value)
	{
		_staging = value;
	}

	mvAppItem* mvItemRegistry::popParent()
	{
		if (_containers.empty())
		{
			mvThrowPythonError(mvErrorCode::mvContainerStackEmpty, "No container to pop.");
			MV_ITEM_REGISTRY_WARN("No container to pop.");
			assert(false);
			return nullptr;
		}

		auto item = _containers.top();
		_containers.pop();
		return item;
	}

	void mvItemRegistry::emptyParents()
	{
		while (!_containers.empty())
			_containers.pop();

		MV_ITEM_REGISTRY_INFO("Container stack emptied.");
	}

	mvAppItem* mvItemRegistry::topParent()
	{
		if (!_containers.empty())
			return _containers.top();
		return nullptr;
	}

	void mvItemRegistry::cacheItem(mvAppItem* item)
	{
		if (mvAppItem::DoesItemHaveFlag(item, MV_ITEM_DESC_CONTAINER))
		{
		_cachedContainersID[_cachedContainerIndex] = item->getUUID();
		_cachedContainersPTR[_cachedContainerIndex] = item;
		_cachedContainerIndex++;
		if (_cachedContainerIndex == CachedContainerCount)
			_cachedContainerIndex = 0;
		}

		_cachedItemsID[_cachedItemsIndex] = item->getUUID();
		_cachedItemsPTR[_cachedItemsIndex] = item;
		_cachedItemsIndex++;
		if (_cachedItemsIndex == CachedContainerCount)
			_cachedItemsIndex = 0;
	}

	void mvItemRegistry::delaySearch(mvAppItem* item)
	{
		_delayedSearch.push_back(item);
	}

	mvAppItem* mvItemRegistry::getItem(mvUUID uuid)
	{
		mvRef<mvAppItem> item = nullptr;

		// check cache first
		for (int i = 0; i < CachedContainerCount; i++)
		{
			if (_cachedContainersID[i] == uuid)
				return _cachedContainersPTR[i];
			if (_cachedItemsID[i] == uuid)
				return _cachedItemsPTR[i];
		}

		if (_stagingArea.count(uuid) != 0)
			return _stagingArea[uuid].get();

		if (_staging)
		{
			for (auto& stagingItem : _stagingArea)
			{
				if (auto child = stagingItem.second->getChild(uuid))
				{
					_delayedSearch.clear();
					cacheItem(child);
					return child;
				}
			}

		}

		for (auto& window : _roots)
		{
			if (window->_uuid == uuid)
			{
				cacheItem(window.get());
				return window.get();
			}

			auto child = window->getChild(uuid);
			if (child)
			{
				cacheItem(child);
				_delayedSearch.clear();
				return child;
			}
		}

		for (auto delayedItem : _delayedSearch)
		{
			auto child = delayedItem->getChild(uuid);
			if (child)
			{
				cacheItem(child);
				_delayedSearch.clear();
				return child;
			}
		}

		_delayedSearch.clear();

		return nullptr;
	}

	mvRef<mvAppItem> mvItemRegistry::getRefItem(mvUUID uuid)
	{
		mvRef<mvAppItem> item = nullptr;

		for (auto& window : _roots)
		{
			if (window->_uuid == uuid)
				return window;

			auto child = window->getChildRef(uuid);
			if (child)
				return child;
		}

		if (_stagingArea.count(uuid) != 0)
			return _stagingArea[uuid];

		//assert(false && "Item not found.");

		return nullptr;
	}

	mvWindowAppItem* mvItemRegistry::getWindow(mvUUID uuid)
	{

		mvAppItem* item = getItem(uuid);
		if (item == nullptr)
		{
			assert(false && "Window not found.");
			return nullptr;
		}

		if (item->getType() == mvAppItemType::mvWindowAppItem)
			return static_cast<mvWindowAppItem*>(item);

		assert(false && "Item is not a window not found.");
		return nullptr;
	}

	bool mvItemRegistry::addItem(mvRef<mvAppItem> item)
	{

		//MV_ITEM_REGISTRY_TRACE("Adding item: " + item->_name);

		mvAppItem* parentitem = topParent();
		item->_parentPtr = parentitem;
		parentitem->addItem(item);

		return true;
	}

	bool mvItemRegistry::addWindow(mvRef<mvAppItem> item)
	{
		//MV_ITEM_REGISTRY_INFO("Adding window: " + item->_name);
		_roots.push_back(item);
		return true;
	}

	void mvItemRegistry::clearRegistry()
	{
		MV_ITEM_REGISTRY_INFO("Clearing item registry.");
		_roots.clear();
	}

	void mvItemRegistry::cleanUpItem(mvUUID uuid)
	{
		for (int i = 0; i < CachedContainerCount; i++)
		{
			if (_cachedContainersID[i] == uuid)
			{
				_cachedContainersID[i] = 0;
				_cachedContainersPTR[i] = nullptr;
			}

			if (_cachedItemsID[i] == uuid)
			{
				_cachedItemsID[i] = 0;
				_cachedItemsPTR[i] = nullptr;
			}
		}
	}

	bool mvItemRegistry::addItemWithRuntimeChecks(mvRef<mvAppItem> item, mvUUID parent, mvUUID before)
	{
		//MV_ITEM_REGISTRY_TRACE("Adding runtime item: " + item->_name);

		if (mvAppItem::DoesItemHaveFlag(item.get(), MV_ITEM_DESC_HANDLER) && parent == 0)
			parent = item->_parent;

		if (item == nullptr)
			return false;

		// check if item is ok
		if (!item->getState().isOk())
			return false;

		
		//---------------------------------------------------------------------------
		// STEP 0: updata "last" information
		//---------------------------------------------------------------------------
		if (mvAppItem::DoesItemHaveFlag(item.get(), MV_ITEM_DESC_ROOT))
		{
			_lastRootAdded = item->getUUID();
			_lastContainerAdded = item->getUUID();
		}
		else if (mvAppItem::DoesItemHaveFlag(item.get(), MV_ITEM_DESC_CONTAINER))
			_lastContainerAdded = item->getUUID();

		_lastItemAdded = item->getUUID();

		cacheItem(item.get());

		//---------------------------------------------------------------------------
		// STEP 1: check if an item with this name exists (NO LONGER NEEDED)
		//---------------------------------------------------------------------------
		//if (getItem(item->getUUID()))
		//{
		//	mvThrowPythonEr1ror(mvErrorCode::mvNon1e, "Item must have a unique name.");
		//	MV_ITEM_REGISTRY_WARN("Item must have a unique name.");
		//	assert(false);
		//	return false;
		//}

		enum class AddTechnique
		{
			NONE, STAGE, BEFORE, PARENT, STACK
		};
		AddTechnique technique = AddTechnique::NONE;

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		//---------------------------------------------------------------------------
		// STEP 2: handle root case
		//---------------------------------------------------------------------------
		if (mvAppItem::DoesItemHaveFlag(item.get(), MV_ITEM_DESC_ROOT))
		{
			if (_staging)
			{
				_stagingArea[item->getUUID()] = item;
				return true;
			}

			else if (item->getType() == mvAppItemType::mvStagingContainer)
			{
				mvThrowPythonError(mvErrorCode::mvStagingModeOff, "Staging container can only be adding in staging mode.");
				return false;
			}

			if (mvApp::IsAppStarted())
			{
				_roots.push_back(item);
				return true;
			}
			return addWindow(item);
		}
			
		//---------------------------------------------------------------------------
		// STEP 3: attempt to deduce parent
		//---------------------------------------------------------------------------
		mvAppItem* parentPtr = nullptr;
		if (before > 0)
		{

			auto beforeItem = getItem(before);
			if (beforeItem)
				parentPtr = beforeItem->_parentPtr;
			technique = AddTechnique::BEFORE;
		}

		else if (parent > MV_RESERVED_UUID_start + MV_RESERVED_UUIDs)
		{
			parentPtr = getItem(parent);
			technique = AddTechnique::PARENT;
		}

		else if (parent == 0)
		{
			parentPtr = topParent();
			technique = AddTechnique::STACK;
		}

		// reserved uuid case
		else
		{
			parentPtr = getItem(parent);
			if(parentPtr)
				technique = AddTechnique::PARENT;

			// revert to stack operation (reserved uuid not used)
			else
			{
				parentPtr = topParent();
				technique = AddTechnique::STACK;
			}
		}

		//---------------------------------------------------------------------------
		// STEP 4: check if parent was deduced
		//---------------------------------------------------------------------------
		if (parentPtr == nullptr)
		{
			if (_staging)
			{
				_stagingArea[item->getUUID()] = item;
				return true;
			}

			mvThrowPythonError(mvErrorCode::mvParentNotDeduced, "add_*", "Parent could not be deduced.", item.get());
			MV_ITEM_REGISTRY_ERROR("Parent could not be deduced.");
			assert(false);
			return false;
		}

		//---------------------------------------------------------------------------
		// STEP 5: check if parent is a compatible type
		//---------------------------------------------------------------------------
		if (!item->isParentCompatible(parentPtr->getType()))
			return false;

		//---------------------------------------------------------------------------
		// STEP 6: check if parent accepts our item (this isn't duplicate STEP 3 ha)
		//---------------------------------------------------------------------------
		if (!parentPtr->canChildBeAdded(item->getType()))
			return false;

		//---------------------------------------------------------------------------
		// STEP 7: add items who require "after" adding (i.e. popup, tooltip)
		//---------------------------------------------------------------------------
		if (mvAppItem::DoesItemHaveFlag(item.get(), MV_ITEM_DESC_AFTER))
			return addItemAfter(parent, item);

		//---------------------------------------------------------------------------
		// STEP 8: handle "before" and "after" style adding
		//---------------------------------------------------------------------------
		if (technique == AddTechnique::BEFORE || technique == AddTechnique::PARENT)
			return parentPtr->addRuntimeChild(parent, before, item); // same for run/compile time

		//---------------------------------------------------------------------------
		// STEP 9: handle "stack" style adding
		//---------------------------------------------------------------------------
		if(mvApp::IsAppStarted())
			return parentPtr->addRuntimeChild(parentPtr->getUUID(), 0, item);
		return addItem(item);
	}

	std::vector<std::vector<mvUUID>> mvItemRegistry::getItemChildren(mvUUID uuid)
	{

		mvAppItem* item = getItem(uuid);

		std::vector<std::vector<mvUUID>> childList;

		if (item)
		{
			for (auto& children : item->_children)
			{
				std::vector<mvUUID> childSlot;
				for (auto& child : children)
					childSlot.emplace_back(child->_uuid);
				childList.push_back(childSlot);
			}

		}
		else
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "get_item_children",
				"Item not found: " + std::to_string(uuid), nullptr);
			assert(false && "Item not found.");
		}

		return childList;
	}

	std::vector<mvUUID> mvItemRegistry::getAllItems()
	{

		std::vector<mvUUID> childList;

		// to help recursively retrieve children
		std::function<void(mvRef<mvAppItem>)> ChildRetriever;
		ChildRetriever = [&childList, &ChildRetriever](mvRef<mvAppItem> item) {
			auto& children0 = item->_children[0];
			auto& children1 = item->_children[1];
			auto& children2 = item->_children[2];
			for (auto& child : children0)
			{
				childList.emplace_back(child->_uuid);
				if (mvAppItem::DoesItemHaveFlag(child.get(), MV_ITEM_DESC_CONTAINER))
					ChildRetriever(child);
			}
			for (auto& child : children1)
			{
				childList.emplace_back(child->_uuid);
				if (mvAppItem::DoesItemHaveFlag(child.get(), MV_ITEM_DESC_CONTAINER))
					ChildRetriever(child);
			}
			for (auto& child : children2)
			{
				childList.emplace_back(child->_uuid);
				if (mvAppItem::DoesItemHaveFlag(child.get(), MV_ITEM_DESC_CONTAINER))
					ChildRetriever(child);
			}

		};

		for (auto& window : _roots)
		{
			childList.emplace_back(window->_uuid);
			ChildRetriever(window);
		}

		return childList;
	}

	std::vector<mvUUID> mvItemRegistry::getWindows()
	{

		std::vector<mvUUID> childList;
		for (auto& window : _roots)
			childList.emplace_back(window->_uuid);

		return childList;
	}

	void mvItemRegistry::setPrimaryWindow(mvUUID uuid, bool value)
	{

		mvWindowAppItem* window = getWindow(uuid);

		if (window)
		{
			if (window->getWindowAsMainStatus() == value)
				return;
			else
				window->setWindowAsMainStatus(value);
		}
		else
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "set_primary_window",
				"Item not found: " + std::to_string(uuid), nullptr);
			assert(false);
		}

		// reset other windows
		for (auto& window : _roots)
		{
			if (window->_uuid != uuid && window->getType() == mvAppItemType::mvWindowAppItem)
				static_cast<mvWindowAppItem*>(window.get())->setWindowAsMainStatus(false);
		}

	}

	void mvItemRegistry::unstageItem(mvUUID uuid)
	{

		if (_stagingArea.count(uuid) != 0)
		{
			mvRef<mvAppItem> item = _stagingArea[uuid];
			_stagingArea.erase(uuid);
			cleanUpItem(uuid);
			if (item->getType() == mvAppItemType::mvStagingContainer)
			{
				for (auto& children : item->_children)
				{
					for (auto& child : children)
						addItemWithRuntimeChecks(child, 0, 0);
				}
			}
			else
				addItemWithRuntimeChecks(item, 0, 0);
		}
		else
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "unstage_item",
				"Staged item not found: " + std::to_string(uuid), nullptr);
			assert(false);
		}

	}

	void mvItemRegistry::stageItem(mvUUID uuid)
	{
		mvRef<mvAppItem> child;

		for (auto& window : _roots)
		{
			child = window->stealChild(uuid);
			if (child)
				break;
		}

		if (child == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "stage_item",
				"Item not found: " + std::to_string(uuid), nullptr);
			MV_ITEM_REGISTRY_WARN("Could not stage item, it was not found");
			return;
		}

		_stagingArea[child->getUUID()] = child;

	}

	PyObject* mvItemRegistry::pop_container_stack(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		auto item = mvApp::GetApp()->getItemRegistry().popParent();
		if (item)
			return ToPyUUID(item->getUUID());
		else
			return GetPyNone();

	}

	PyObject* mvItemRegistry::empty_container_stack(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		mvApp::GetApp()->getItemRegistry().emptyParents();
		return GetPyNone();
	}

	PyObject* mvItemRegistry::top_container_stack(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		auto item = mvApp::GetApp()->getItemRegistry().topParent();
		if (item)
			return ToPyUUID(item->getUUID());
		else
			return GetPyNone();
	}

	PyObject* mvItemRegistry::last_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		return ToPyUUID(mvApp::GetApp()->getItemRegistry()._lastItemAdded);
	}

	PyObject* mvItemRegistry::last_container(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		return ToPyUUID(mvApp::GetApp()->getItemRegistry()._lastContainerAdded);
	}

	PyObject* mvItemRegistry::last_root(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		return ToPyUUID(mvApp::GetApp()->getItemRegistry()._lastRootAdded);
	}

	PyObject* mvItemRegistry::push_container_stack(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvUUID item;

		if (!(mvApp::GetApp()->getParsers())["push_container_stack"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		auto parent = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (parent)
		{
			if (mvAppItem::DoesItemHaveFlag(parent, MV_ITEM_DESC_CONTAINER))
			{
				mvApp::GetApp()->getItemRegistry().pushParent(parent);
				return ToPyBool(true);
			}
		}
		return ToPyBool(false);
	}

	PyObject* mvItemRegistry::set_staging_mode(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int mode;

		if (!(mvApp::GetApp()->getParsers())["set_staging_mode"].parse(args, kwargs, __FUNCTION__, &mode))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		mvApp::GetApp()->getItemRegistry().setStagingMode((bool)mode);
		return GetPyNone();
	}

	PyObject* mvItemRegistry::set_primary_window(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		mvUUID item;
		int value;

		if (!mvApp::GetApp()->getParsers()["set_primary_window"].verifyRequiredArguments(args))
			return GetPyNone();

		if (!(mvApp::GetApp()->getParsers())["set_primary_window"].parse(args, kwargs, __FUNCTION__, &item, &value))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		mvApp::GetApp()->getItemRegistry().setPrimaryWindow(item, value);

		return GetPyNone();
	}

	PyObject* mvItemRegistry::get_active_window(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		return ToPyUUID(mvApp::GetApp()->getItemRegistry().getActiveWindow());
	}

	PyObject* mvItemRegistry::move_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		mvUUID item;
		mvUUID parent = 0;
		mvUUID before = 0;

		if (!(mvApp::GetApp()->getParsers())["move_item"].parse(args, kwargs, __FUNCTION__,
			&item, &parent, &before))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		mvApp::GetApp()->getItemRegistry().moveItem(item, parent, before);

		return GetPyNone();
	}

	PyObject* mvItemRegistry::delete_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		mvUUID item;
		int childrenOnly = false;
		int slot = -1;

		if (!(mvApp::GetApp()->getParsers())["delete_item"].parse(args, kwargs, __FUNCTION__, &item, &childrenOnly, &slot))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		mvApp::GetApp()->getItemRegistry().deleteItem(item, childrenOnly, slot);

		return GetPyNone();

	}

	PyObject* mvItemRegistry::does_item_exist(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		mvUUID item;

		if (!(mvApp::GetApp()->getParsers())["does_item_exist"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		if (mvApp::GetApp()->getItemRegistry().getItem(item))
			return ToPyBool(true);
		return ToPyBool(false);
	}

	PyObject* mvItemRegistry::move_item_up(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		mvUUID item;

		if (!(mvApp::GetApp()->getParsers())["move_item_up"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		mvApp::GetApp()->getItemRegistry().moveItemUp(item);

		return GetPyNone();

	}

	PyObject* mvItemRegistry::move_item_down(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		mvUUID item;

		if (!(mvApp::GetApp()->getParsers())["move_item_down"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		mvApp::GetApp()->getItemRegistry().moveItemDown(item);

		return GetPyNone();
	}

	PyObject* mvItemRegistry::reorder_items(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		mvUUID container;
		int slot = 0;
		PyObject* new_order = nullptr;

		if (!(mvApp::GetApp()->getParsers())["reorder_items"].parse(args, kwargs, __FUNCTION__, 
			&container, &slot, &new_order))
			return GetPyNone();

		auto anew_order = ToUUIDVect(new_order);
		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		auto parent = mvApp::GetApp()->getItemRegistry().getItem(container);

		std::vector<mvRef<mvAppItem>>& children = parent->getChildren(slot);

		std::vector<mvRef<mvAppItem>> newchildren;
		newchildren.reserve(children.size());

		// todo: better sorting algorithm
		for (const auto& item : anew_order)
		{
			for (auto& child : children)
			{
				if (child->getUUID() == item)
				{
					newchildren.emplace_back(child);
					break;
				}
			}
		}
		children = newchildren;
		return GetPyNone();
	}

	PyObject* mvItemRegistry::unstage_items(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* items = nullptr;

		if (!(mvApp::GetApp()->getParsers())["unstage_items"].parse(args, kwargs, __FUNCTION__, &items))
			return GetPyNone();

		auto aitems = ToUUIDVect(items);
		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		for(const auto& item : aitems)
			mvApp::GetApp()->getItemRegistry().unstageItem(item);


		return GetPyNone();
	}

	PyObject* mvItemRegistry::stage_items(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* items = nullptr;

		if (!(mvApp::GetApp()->getParsers())["stage_items"].parse(args, kwargs, __FUNCTION__, &items))
			return GetPyNone();

		auto aitems = ToUUIDVect(items);
		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		for (const auto& item : aitems)
			mvApp::GetApp()->getItemRegistry().stageItem(item);

		return GetPyNone();
	}

	PyObject* mvItemRegistry::get_all_items(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		return ToPyList(mvApp::GetApp()->getItemRegistry().getAllItems());
	}

	PyObject* mvItemRegistry::show_imgui_demo(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		mvApp::GetApp()->getItemRegistry()._showImGuiDebug = true;
		return GetPyNone();
	}

	PyObject* mvItemRegistry::show_implot_demo(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		mvApp::GetApp()->getItemRegistry()._showImPlotDebug = true;
		return GetPyNone();
	}

	PyObject* mvItemRegistry::get_windows(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		return ToPyList(mvApp::GetApp()->getItemRegistry().getWindows());
	}
}
