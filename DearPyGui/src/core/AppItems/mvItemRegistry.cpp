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
			mvPythonParser parser(mvPyDataType::UUID);
			parser.finalize();
			parsers->insert({ "pop_container_stack", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.addArg<mvPyDataType::UUID>("container");
			parser.addArg<mvPyDataType::Integer>("slot");
			parser.addArg<mvPyDataType::UUIDList>("new_order");
			parser.finalize();
			parsers->insert({ "reorder_items", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.addArg<mvPyDataType::UUIDList>("items");
			parser.finalize();
			parsers->insert({ "unstage_items", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.addArg<mvPyDataType::UUIDList>("items");
			parser.finalize();
			parsers->insert({ "stage_items", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::Bool);
			parser.addArg<mvPyDataType::UUID>("item");
			parser.finalize();
			parsers->insert({ "push_container_stack", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::UUID);
			parser.finalize();
			parsers->insert({ "top_container_stack", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::UUID);
			parser.finalize();
			parsers->insert({ "last_item", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::UUID);
			parser.finalize();
			parsers->insert({ "last_container", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::UUID);
			parser.finalize();
			parsers->insert({ "last_root", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.finalize();
			parsers->insert({ "empty_container_stack", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.addArg<mvPyDataType::UUID>("item");
			parser.addArg<mvPyDataType::UUID>("parent", mvArgType::KEYWORD_ARG, "0");
			parser.addArg<mvPyDataType::UUID>("before", mvArgType::KEYWORD_ARG, "0");
			parser.finalize();
			parsers->insert({ "move_item", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::UUIDList);
			parser.finalize();
			parsers->insert({ "get_windows", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::UUIDList);
			parser.finalize();
			parsers->insert({ "get_all_items", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.addArg<mvPyDataType::Bool>("mode");
			parser.finalize();
			parsers->insert({ "set_staging_mode", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.addArg<mvPyDataType::UUID>("item");
			parser.addArg<mvPyDataType::Bool>("children_only", mvArgType::KEYWORD_ARG, "False");
			parser.finalize();
			parsers->insert({ "delete_item", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::Bool);
			parser.addArg<mvPyDataType::UUID>("item");
			parser.finalize();
			parsers->insert({ "does_item_exist", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.addArg<mvPyDataType::UUID>("item");
			parser.finalize();
			parsers->insert({ "move_item_up", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.addArg<mvPyDataType::UUID>("item");
			parser.finalize();
			parsers->insert({ "move_item_down", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::UUID);
			parser.finalize();
			parsers->insert({ "get_active_window", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
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
			m_cachedContainersID[i] = 0;
			m_cachedContainersPTR[i] = nullptr;
			m_cachedItemsID[i] = 0;
			m_cachedItemsPTR[i] = nullptr;
		}
		
	}

	mvValueVariant mvItemRegistry::getValue(mvUUID uuid)
	{
		mvAppItem* item = getItem(uuid);
		if (item)
			return item->getValue();
		return nullptr;
	}

	bool mvItemRegistry::deleteItem(mvUUID uuid, bool childrenOnly)
	{

		MV_ITEM_REGISTRY_TRACE("Attempting to delete: " + uuid);

		// check staging first
		if (m_stagingArea.count(uuid) != 0)
		{
			if (childrenOnly)
				m_stagingArea[uuid]->deleteChildren();
			else
				m_stagingArea.erase(uuid);
			MV_ITEM_REGISTRY_INFO(uuid + " found and deleted.");
			return true;
		}

		// delete item's children only
		if(childrenOnly)
		{
			auto item = getItem(uuid);
			if (item)
			{
				item->deleteChildren();
				MV_ITEM_REGISTRY_INFO("Item found and it's children deleted.");
				return true;
			}
		}

		bool deletedItem = false;

		// try to delete build-in item
		for (auto& window : m_roots)
		{
			deletedItem = window->deleteChild(uuid);
			if (deletedItem)
				break;
		}

		bool rootDeleting = false;

		// check if attempting to delete a window
		for (auto& window : m_roots)
		{
			if (window->m_uuid == uuid)
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
			std::vector<mvRef<mvAppItem>> oldwindows = m_roots;

			m_roots.clear();

			for (auto& window : oldwindows)
			{
				if (window->m_uuid == uuid)
				{
					deletedItem = true;
					continue;
				}
				m_roots.push_back(window);
			}
		}

		if (deletedItem)
		{
			MV_ITEM_REGISTRY_INFO(uuid + " found and deleted.");
		}
		else
			mvThrowPythonError(1001, uuid + " not deleted because it was not found");

		assert(deletedItem && "Item to delete not found");
		return deletedItem;
	}

	bool mvItemRegistry::moveItem(mvUUID uuid, mvUUID parent, mvUUID before)
	{

		MV_ITEM_REGISTRY_TRACE("Attempting to move: " + uuid);

		mvRef<mvAppItem> child = nullptr;

		bool movedItem = false;

		for (auto& window : m_roots)
		{
			child = window->stealChild(uuid);
			if (child)
				break;
		}

		if (m_stagingArea.count(uuid) != 0)
		{
			child = m_stagingArea[uuid];
			m_stagingArea.erase(uuid);
		}

		if (child == nullptr)
		{
			mvThrowPythonError(1000, uuid + " not moved because it was not found");
			MV_ITEM_REGISTRY_WARN("Could not move item, it was not found");
		}


		if (child)
			addRuntimeItem(parent, before, child);

		assert(movedItem && "Item to move not found");

		return movedItem;
	}

	bool mvItemRegistry::moveItemUp(mvUUID uuid)
	{

		MV_ITEM_REGISTRY_TRACE("Attempting to move: " + uuid);

		bool movedItem = false;

		for (auto window : m_roots)
		{
			movedItem = window->moveChildUp(uuid);
			if (movedItem)
				break;
		}

		if (!movedItem)
		{
			mvThrowPythonError(1000, uuid + " not moved because it was not found");
			MV_ITEM_REGISTRY_WARN("Could not move item, it was not found");
		}

		assert(movedItem && "Item to move not found");

		return movedItem;
	}

	bool mvItemRegistry::moveItemDown(mvUUID uuid)
	{

		MV_ITEM_REGISTRY_TRACE("Attempting to move: " + uuid);

		bool movedItem = false;

		for (auto window : m_roots)
		{
			movedItem = window->moveChildDown(uuid);
			if (movedItem)
				break;
		}

		if (!movedItem)
		{
			mvThrowPythonError(1000, uuid + " not moved because it was not found");
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

		for (auto& window : m_roots)
		{
			if (!window->preDraw())
				continue;

			if(window->m_show || mvAppItem::DoesItemHaveFlag(window.get(), MV_ITEM_DESC_ALWAYS_DRAW))
				window->draw(nullptr, 0.0f, 0.0f);

			window->postDraw();
		}

		return false;
	}

	bool mvItemRegistry::onPreRenderReset(mvEvent& event)
	{

		// resets app items states (i.e. hovered)
		for (auto& window : m_roots)
			window->resetState();

		return false;
	}

	bool mvItemRegistry::onActiveWindow(mvEvent& event)
	{

		m_activeWindow = GetEUUID(event, "WINDOW");

		MV_ITEM_REGISTRY_INFO("Active window changed to: " + m_activeWindow);

		return false;
	}

	bool mvItemRegistry::addRuntimeItem(mvUUID parent, mvUUID before, mvRef<mvAppItem> item)
	{
		//MV_ITEM_REGISTRY_TRACE("Attempting to add new widget: ", item->m_name);


		if (m_stagingArea.count(parent) != 0)
		{
			m_stagingArea[parent]->addItem(item);
			return true;
		}

		else if (m_stagingArea.count(before) != 0)
		{
			m_stagingArea[before]->addItem(item);
			return true;
		}

		if (m_staging)
		{
			for (auto& stagingItem : m_stagingArea)
			{
				if (stagingItem.second->addRuntimeChild(parent, before, item))
					return true;
			}

		}

		for (auto& window : m_roots)
		{
			if(window->addRuntimeChild(parent, before, item))
				return true;
		}

		assert(false);
		return false;
	}

	bool mvItemRegistry::addItemAfter(mvUUID prev, mvRef<mvAppItem> item)
	{
		//MV_ITEM_REGISTRY_TRACE("Attempting to add new widget after: ", item->m_name);

		bool addedItem = false;

		for (auto& window : m_roots)
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
		m_containers.push(item);
	}

	void mvItemRegistry::setStagingMode(bool value)
	{
		m_staging = value;
	}

	mvAppItem* mvItemRegistry::popParent()
	{
		if (m_containers.empty())
		{
			mvThrowPythonError(1000, "No container to pop.");
			MV_ITEM_REGISTRY_WARN("No container to pop.");
			assert(false);
			return nullptr;
		}

		auto item = m_containers.top();
		m_containers.pop();
		return item;
	}

	void mvItemRegistry::emptyParents()
	{
		while (!m_containers.empty())
			m_containers.pop();

		MV_ITEM_REGISTRY_INFO("Container stack emptied.");
	}

	mvAppItem* mvItemRegistry::topParent()
	{
		if (!m_containers.empty())
			return m_containers.top();
		return nullptr;
	}

	void mvItemRegistry::cacheItem(mvAppItem* item)
	{
		if (mvAppItem::DoesItemHaveFlag(item, MV_ITEM_DESC_CONTAINER))
		{
		m_cachedContainersID[m_cachedContainerIndex] = item->getUUID();
		m_cachedContainersPTR[m_cachedContainerIndex] = item;
		m_cachedContainerIndex++;
		if (m_cachedContainerIndex == CachedContainerCount)
			m_cachedContainerIndex = 0;
		}

		m_cachedItemsID[m_cachedItemsIndex] = item->getUUID();
		m_cachedItemsPTR[m_cachedItemsIndex] = item;
		m_cachedItemsIndex++;
		if (m_cachedItemsIndex == CachedContainerCount)
			m_cachedItemsIndex = 0;
	}

	void mvItemRegistry::delaySearch(mvAppItem* item)
	{
		m_delayedSearch.push_back(item);
	}

	mvAppItem* mvItemRegistry::getItem(mvUUID uuid)
	{
		mvRef<mvAppItem> item = nullptr;

		// check cache first
		for (int i = 0; i < CachedContainerCount; i++)
		{
			if (m_cachedContainersID[i] == uuid)
				return m_cachedContainersPTR[i];
			if (m_cachedItemsID[i] == uuid)
				return m_cachedItemsPTR[i];
		}

		if (m_stagingArea.count(uuid) != 0)
			return m_stagingArea[uuid].get();

		if (m_staging)
		{
			for (auto& stagingItem : m_stagingArea)
			{
				if (auto child = stagingItem.second->getChild(uuid))
				{
					m_delayedSearch.clear();
					cacheItem(child);
					return child;
				}
			}

		}

		for (auto& window : m_roots)
		{
			if (window->m_uuid == uuid)
			{
				cacheItem(window.get());
				return window.get();
			}

			auto child = window->getChild(uuid);
			if (child)
			{
				cacheItem(child);
				m_delayedSearch.clear();
				return child;
			}
		}

		for (auto delayedItem : m_delayedSearch)
		{
			auto child = delayedItem->getChild(uuid);
			if (child)
			{
				cacheItem(child);
				m_delayedSearch.clear();
				return child;
			}
		}

		m_delayedSearch.clear();

		return nullptr;
	}

	mvRef<mvAppItem> mvItemRegistry::getRefItem(mvUUID uuid)
	{
		mvRef<mvAppItem> item = nullptr;

		for (auto& window : m_roots)
		{
			if (window->m_uuid == uuid)
				return window;

			auto child = window->getChildRef(uuid);
			if (child)
				return child;
		}

		if (m_stagingArea.count(uuid) != 0)
			return m_stagingArea[uuid];

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

		//MV_ITEM_REGISTRY_TRACE("Adding item: " + item->m_name);

		mvAppItem* parentitem = topParent();
		item->m_parentPtr = parentitem;
		parentitem->addItem(item);

		return true;
	}

	bool mvItemRegistry::addWindow(mvRef<mvAppItem> item)
	{
		//MV_ITEM_REGISTRY_INFO("Adding window: " + item->m_name);
		m_roots.push_back(item);
		return true;
	}

	void mvItemRegistry::clearRegistry()
	{
		MV_ITEM_REGISTRY_INFO("Clearing item registry.");
		m_roots.clear();
	}

	void mvItemRegistry::cleanUpItem(mvUUID uuid)
	{
		for (int i = 0; i < CachedContainerCount; i++)
		{
			if (m_cachedContainersID[i] == uuid)
			{
				m_cachedContainersID[i] = 0;
				m_cachedContainersPTR[i] = nullptr;
			}

			if (m_cachedItemsID[i] == uuid)
			{
				m_cachedItemsID[i] = 0;
				m_cachedItemsPTR[i] = nullptr;
			}
		}
	}

	bool mvItemRegistry::addItemWithRuntimeChecks(mvRef<mvAppItem> item, mvUUID parent, mvUUID before)
	{
		//MV_ITEM_REGISTRY_TRACE("Adding runtime item: " + item->m_name);

		if (mvAppItem::DoesItemHaveFlag(item.get(), MV_ITEM_DESC_HANDLER) && parent == 0)
			parent = item->m_parent;

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
			m_lastRootAdded = item->getUUID();
			m_lastContainerAdded = item->getUUID();
		}
		else if (mvAppItem::DoesItemHaveFlag(item.get(), MV_ITEM_DESC_CONTAINER))
			m_lastContainerAdded = item->getUUID();

		m_lastItemAdded = item->getUUID();

		cacheItem(item.get());

		//---------------------------------------------------------------------------
		// STEP 1: check if an item with this name exists (NO LONGER NEEDED)
		//---------------------------------------------------------------------------
		//if (getItem(item->getUUID()))
		//{
		//	mvThrowPythonError(1000, "Item must have a unique name.");
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
			if (m_staging)
			{
				m_stagingArea[item->getUUID()] = item;
				return true;
			}

			else if (item->getType() == mvAppItemType::mvStagingContainer)
			{
				mvThrowPythonError(1000, "Staging container can only be adding in staging mode.");
				return false;
			}

			if (mvApp::IsAppStarted())
			{
				m_roots.push_back(item);
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
				parentPtr = beforeItem->m_parentPtr;
			technique = AddTechnique::BEFORE;
		}

		else if (parent > 0)
		{
			parentPtr = getItem(parent);
			technique = AddTechnique::PARENT;
		}

		else
		{
			parentPtr = topParent();
			technique = AddTechnique::STACK;
		}

		//---------------------------------------------------------------------------
		// STEP 4: check if parent was deduced
		//---------------------------------------------------------------------------
		if (parentPtr == nullptr)
		{
			if (m_staging)
			{
				m_stagingArea[item->getUUID()] = item;
				return true;
			}

			mvThrowPythonError(1000, "Parent could not be deduced.");
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
			for (auto& children : item->m_children)
			{
				std::vector<mvUUID> childSlot;
				for (auto& child : children)
					childSlot.emplace_back(child->m_uuid);
				childList.push_back(childSlot);
			}

		}
		else
		{
			mvThrowPythonError(1000, uuid + ": item not found");
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
			auto& children0 = item->m_children[0];
			auto& children1 = item->m_children[1];
			auto& children2 = item->m_children[2];
			for (auto& child : children0)
			{
				childList.emplace_back(child->m_uuid);
				if (mvAppItem::DoesItemHaveFlag(child.get(), MV_ITEM_DESC_CONTAINER))
					ChildRetriever(child);
			}
			for (auto& child : children1)
			{
				childList.emplace_back(child->m_uuid);
				if (mvAppItem::DoesItemHaveFlag(child.get(), MV_ITEM_DESC_CONTAINER))
					ChildRetriever(child);
			}
			for (auto& child : children2)
			{
				childList.emplace_back(child->m_uuid);
				if (mvAppItem::DoesItemHaveFlag(child.get(), MV_ITEM_DESC_CONTAINER))
					ChildRetriever(child);
			}

		};

		for (auto& window : m_roots)
		{
			childList.emplace_back(window->m_uuid);
			ChildRetriever(window);
		}

		return childList;
	}

	std::vector<mvUUID> mvItemRegistry::getWindows()
	{

		std::vector<mvUUID> childList;
		for (auto& window : m_roots)
			childList.emplace_back(window->m_uuid);

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
			mvThrowPythonError(1000, "Window does not exist.");
			assert(false);
		}

		// reset other windows
		for (auto& window : m_roots)
		{
			if (window->m_uuid != uuid)
				static_cast<mvWindowAppItem*>(window.get())->setWindowAsMainStatus(false);
		}

	}

	void mvItemRegistry::unstageItem(mvUUID uuid)
	{

		if (m_stagingArea.count(uuid) != 0)
		{
			mvRef<mvAppItem> item = m_stagingArea[uuid];
			m_stagingArea.erase(uuid);
			if (item->getType() == mvAppItemType::mvStagingContainer)
			{
				for (auto& children : item->m_children)
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
			mvThrowPythonError(1000, "Staged item does not exist.");
			assert(false);
		}

	}

	void mvItemRegistry::stageItem(mvUUID uuid)
	{
		mvRef<mvAppItem> child;

		for (auto& window : m_roots)
		{
			child = window->stealChild(uuid);
			if (child)
				break;
		}

		if (child == nullptr)
		{
			mvThrowPythonError(1000, uuid + " not staged because it was not found");
			MV_ITEM_REGISTRY_WARN("Could not stage item, it was not found");
			return;
		}

		m_stagingArea[child->getUUID()] = child;

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
		return ToPyUUID(mvApp::GetApp()->getItemRegistry().m_lastItemAdded);
	}

	PyObject* mvItemRegistry::last_container(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		return ToPyUUID(mvApp::GetApp()->getItemRegistry().m_lastContainerAdded);
	}

	PyObject* mvItemRegistry::last_root(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		return ToPyUUID(mvApp::GetApp()->getItemRegistry().m_lastRootAdded);
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

		if (!(mvApp::GetApp()->getParsers())["delete_item"].parse(args, kwargs, __FUNCTION__, &item, &childrenOnly))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		mvApp::GetApp()->getItemRegistry().deleteItem(item, childrenOnly);

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

	PyObject* mvItemRegistry::get_windows(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);
		return ToPyList(mvApp::GetApp()->getItemRegistry().getWindows());
	}
}
