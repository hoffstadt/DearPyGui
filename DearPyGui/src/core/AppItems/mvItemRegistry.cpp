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
			mvPythonParser parser(mvPyDataType::String);
			parser.finalize();
			parsers->insert({ "pop_container_stack", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.addArg<mvPyDataType::String>("container");
			parser.addArg<mvPyDataType::Integer>("slot");
			parser.addArg<mvPyDataType::StringList>("new_order");
			parser.finalize();
			parsers->insert({ "reorder_items", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.addArg<mvPyDataType::StringList>("items");
			parser.finalize();
			parsers->insert({ "unstage_items", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.addArg<mvPyDataType::StringList>("items");
			parser.finalize();
			parsers->insert({ "stage_items", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::Bool);
			parser.addArg<mvPyDataType::String>("item");
			parser.finalize();
			parsers->insert({ "push_container_stack", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::String);
			parser.finalize();
			parsers->insert({ "top_container_stack", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::String);
			parser.finalize();
			parsers->insert({ "empty_container_stack", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.addArg<mvPyDataType::String>("item");
			parser.addArg<mvPyDataType::String>("parent", mvArgType::KEYWORD_ARG, "''");
			parser.addArg<mvPyDataType::String>("before", mvArgType::KEYWORD_ARG, "''");
			parser.finalize();
			parsers->insert({ "move_item", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::StringList);
			parser.finalize();
			parsers->insert({ "get_windows", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::StringList);
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
			parser.addArg<mvPyDataType::String>("item");
			parser.addArg<mvPyDataType::Bool>("children_only", mvArgType::KEYWORD_ARG, "False");
			parser.finalize();
			parsers->insert({ "delete_item", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::Bool);
			parser.addArg<mvPyDataType::String>("item");
			parser.finalize();
			parsers->insert({ "does_item_exist", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.addArg<mvPyDataType::String>("item");
			parser.finalize();
			parsers->insert({ "move_item_up", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.addArg<mvPyDataType::String>("item");
			parser.finalize();
			parsers->insert({ "move_item_down", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::String);
			parser.finalize();
			parsers->insert({ "get_active_window", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.addArg<mvPyDataType::String>("window");
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
		
	}

	mvValueVariant mvItemRegistry::getValue(const std::string& name)
	{
		mvRef<mvAppItem> item = getItem(name);
		if (item)
			return item->getValue();
		return nullptr;
	}

	bool mvItemRegistry::deleteItem(const std::string& name, bool childrenOnly)
	{

		MV_ITEM_REGISTRY_TRACE("Attempting to delete: " + name);

		// check staging first
		if (m_stagingArea.count(name) != 0)
		{
			if (childrenOnly)
				m_stagingArea[name]->deleteChildren();
			else
				m_stagingArea.erase(name);
			MV_ITEM_REGISTRY_INFO(name + " found and deleted.");
			return true;
		}

		// delete item's children only
		if(childrenOnly)
		{
			auto item = getItem(name);
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
			deletedItem = window->deleteChild(name);
			if (deletedItem)
				break;
		}

		bool rootDeleting = false;

		// check if attempting to delete a window
		for (auto& window : m_roots)
		{
			if (window->m_name == name)
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
				if (window->m_name == name)
				{
					deletedItem = true;
					continue;
				}
				m_roots.push_back(window);
			}
		}

		if (deletedItem)
		{
			MV_ITEM_REGISTRY_INFO(name + " found and deleted.");
		}
		else
			mvThrowPythonError(1001, name + " not deleted because it was not found");

		assert(deletedItem && "Item to delete not found");
		return deletedItem;
	}

	bool mvItemRegistry::moveItem(const std::string& name, const std::string& parent, const std::string& before)
	{

		MV_ITEM_REGISTRY_TRACE("Attempting to move: " + name);

		mvRef<mvAppItem> child = nullptr;

		bool movedItem = false;

		for (auto& window : m_roots)
		{
			child = window->stealChild(name);
			if (child)
				break;
		}

		if (m_stagingArea.count(name) != 0)
		{
			child = m_stagingArea[name];
			m_stagingArea.erase(name);
		}

		if (child == nullptr)
		{
			mvThrowPythonError(1000, name + " not moved because it was not found");
			MV_ITEM_REGISTRY_WARN("Could not move item, it was not found");
		}


		if (child)
			addRuntimeItem(parent, before, child);

		assert(movedItem && "Item to move not found");

		return movedItem;
	}

	bool mvItemRegistry::moveItemUp(const std::string& name)
	{

		MV_ITEM_REGISTRY_TRACE("Attempting to move: " + name);

		bool movedItem = false;

		for (auto window : m_roots)
		{
			movedItem = window->moveChildUp(name);
			if (movedItem)
				break;
		}

		if (!movedItem)
		{
			mvThrowPythonError(1000, name + " not moved because it was not found");
			MV_ITEM_REGISTRY_WARN("Could not move item, it was not found");
		}

		assert(movedItem && "Item to move not found");

		return movedItem;
	}

	bool mvItemRegistry::moveItemDown(const std::string& name)
	{

		MV_ITEM_REGISTRY_TRACE("Attempting to move: " + name);

		bool movedItem = false;

		for (auto window : m_roots)
		{
			movedItem = window->moveChildDown(name);
			if (movedItem)
				break;
		}

		if (!movedItem)
		{
			mvThrowPythonError(1000, name + " not moved because it was not found");
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
			window->draw(nullptr, 0.0f, 0.0f);

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

		m_activeWindow = GetEString(event, "WINDOW");

		MV_ITEM_REGISTRY_INFO("Active window changed to: " + m_activeWindow);

		return false;
	}

	bool mvItemRegistry::addRuntimeItem(const std::string& parent, const std::string& before, mvRef<mvAppItem> item)
	{
		MV_ITEM_REGISTRY_TRACE("Attempting to add new widget: ", item->m_name);

		// add runtime items
		bool addedItem = false;

		for (auto window : m_roots)
		{
			addedItem = window->addRuntimeChild(parent, before, item);
			if (addedItem)
				break;
		}
		assert(addedItem);
		return addedItem;
	}

	bool mvItemRegistry::addItemAfter(const std::string& prev, mvRef<mvAppItem> item)
	{
		MV_ITEM_REGISTRY_TRACE("Attempting to add new widget after: ", item->m_name);

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

	void mvItemRegistry::pushParent(mvRef<mvAppItem> item)
	{
		m_containers.push(item);
	}

	void mvItemRegistry::setStagingMode(bool value)
	{
		m_staging = value;
	}

	mvRef<mvAppItem> mvItemRegistry::popParent()
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

	mvRef<mvAppItem> mvItemRegistry::topParent()
	{
		if (!m_containers.empty())
			return m_containers.top();
		return nullptr;
	}

	mvRef<mvAppItem> mvItemRegistry::getItem(const std::string& name)
	{
		mvRef<mvAppItem> item = nullptr;

		for (auto& window : m_roots)
		{
			if (window->m_name == name)
				return window;

			auto child = window->getChild(name);
			if (child)
				return child;
		}

		if(m_stagingArea.count(name) != 0)
			return m_stagingArea[name];

		//assert(false && "Item not found.");

		return nullptr;
	}

	mvWindowAppItem* mvItemRegistry::getWindow(const std::string& name)
	{

		mvRef<mvAppItem> item = getItem(name);
		if (item == nullptr)
		{
			assert(false && "Window not found.");
			return nullptr;
		}

		if (item->getType() == mvAppItemType::mvWindowAppItem)
			return static_cast<mvWindowAppItem*>(item.get());

		assert(false && "Item is not a window not found.");
		return nullptr;
	}

	bool mvItemRegistry::addItem(mvRef<mvAppItem> item)
	{

		MV_ITEM_REGISTRY_TRACE("Adding item: " + item->m_name);

		mvRef<mvAppItem> parentitem = topParent();
		item->m_parentPtr = parentitem.get();
		parentitem->addItem(item);

		return true;
	}

	bool mvItemRegistry::addWindow(mvRef<mvAppItem> item)
	{
		MV_ITEM_REGISTRY_INFO("Adding window: " + item->m_name);
		m_roots.push_back(item);
		return true;
	}

	void mvItemRegistry::clearRegistry()
	{
		MV_ITEM_REGISTRY_INFO("Clearing item registry.");
		m_roots.clear();
	}

	bool mvItemRegistry::addItemWithRuntimeChecks(mvRef<mvAppItem> item, const char* parent, const char* before)
	{
		MV_ITEM_REGISTRY_TRACE("Adding runtime item: " + item->m_name);

		if (item == nullptr)
			return false;

		// check if item is ok
		if (!item->getState().isOk())
			return false;

		//---------------------------------------------------------------------------
		// STEP 0: check if an item with this name exists
		//---------------------------------------------------------------------------
		if (getItem(item->m_name))
		{
			mvThrowPythonError(1000, "Item must have a unique name.");
			MV_ITEM_REGISTRY_WARN("Item must have a unique name.");
			//assert(false);
			return false;
		}

		enum class AddTechnique
		{
			NONE, STAGE, BEFORE, PARENT, STACK
		};
		AddTechnique technique = AddTechnique::NONE;

		//std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());

		//---------------------------------------------------------------------------
		// STEP 1: empty parent stack if mistakes were made
		//---------------------------------------------------------------------------
		if (mvAppItem::DoesItemHaveFlag(item.get(), MV_ITEM_DESC_ROOT) && topParent() != nullptr)
		{
			mvThrowPythonError(1000, "Container stack not empty.");
			emptyParents();
			MV_ITEM_REGISTRY_ERROR("Container stack not empty when adding " + item->m_name);
			assert(false);
		}

		//---------------------------------------------------------------------------
		// STEP 2: handle root case
		//---------------------------------------------------------------------------
		if (mvAppItem::DoesItemHaveFlag(item.get(), MV_ITEM_DESC_ROOT))
		{
			if (m_staging)
			{
				m_stagingArea[item->getName()] = item;
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
		mvRef<mvAppItem> parentPtr = nullptr;
		if (!std::string(before).empty())
		{
			parentPtr = getItem(before);
			technique = AddTechnique::BEFORE;
		}

		else if (!std::string(parent).empty())
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
				m_stagingArea[item->getName()] = item;
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
			return addRuntimeItem(parent, before, item); // same for run/compile time

		//---------------------------------------------------------------------------
		// STEP 9: handle "stack" style adding
		//---------------------------------------------------------------------------
		if(mvApp::IsAppStarted())
			return addRuntimeItem(parentPtr->m_name, "", item);
		return addItem(item);
	}

	std::string mvItemRegistry::getItemParentName(const std::string& name)
	{
		mvRef<mvAppItem> item = getItem(name);
		if (item)
			return item->m_parentPtr->m_name;
		
		mvThrowPythonError(1000, name + ": item not found");
		assert(false && "Item not found.");

		return "";
	}

	std::vector<std::vector<std::string>> mvItemRegistry::getItemChildren(const std::string& name)
	{

		mvRef<mvAppItem> item = getItem(name);

		std::vector<std::vector<std::string>> childList;

		if (item)
		{
			for (auto& children : item->m_children)
			{
				std::vector<std::string> childSlot;
				for (auto& child : children)
					childSlot.emplace_back(child->m_name);
				childList.push_back(childSlot);
			}

		}
		else
		{
			mvThrowPythonError(1000, name + ": item not found");
			assert(false && "Item not found.");
		}

		return childList;
	}

	std::vector<std::string> mvItemRegistry::getAllItems()
	{

		std::vector<std::string> childList;

		// to help recursively retrieve children
		std::function<void(mvRef<mvAppItem>)> ChildRetriever;
		ChildRetriever = [&childList, &ChildRetriever](mvRef<mvAppItem> item) {
			auto& children0 = item->m_children[0];
			auto& children1 = item->m_children[1];
			auto& children2 = item->m_children[2];
			for (auto& child : children0)
			{
				childList.emplace_back(child->m_name);
				if (mvAppItem::DoesItemHaveFlag(child.get(), MV_ITEM_DESC_CONTAINER))
					ChildRetriever(child);
			}
			for (auto& child : children1)
			{
				childList.emplace_back(child->m_name);
				if (mvAppItem::DoesItemHaveFlag(child.get(), MV_ITEM_DESC_CONTAINER))
					ChildRetriever(child);
			}
			for (auto& child : children2)
			{
				childList.emplace_back(child->m_name);
				if (mvAppItem::DoesItemHaveFlag(child.get(), MV_ITEM_DESC_CONTAINER))
					ChildRetriever(child);
			}

		};

		for (auto& window : m_roots)
		{
			childList.emplace_back(window->m_name);
			ChildRetriever(window);
		}

		return childList;
	}

	std::vector<std::string> mvItemRegistry::getWindows()
	{

		std::vector<std::string> childList;
		for (auto& window : m_roots)
			childList.emplace_back(window->m_name);

		return childList;
	}

	void mvItemRegistry::setPrimaryWindow(const std::string& name, bool value)
	{

		mvWindowAppItem* window = getWindow(name);

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
			if (window->m_name != name)
				static_cast<mvWindowAppItem*>(window.get())->setWindowAsMainStatus(false);
		}

	}

	void mvItemRegistry::unstageItem(const std::string& name)
	{

		if (m_stagingArea.count(name) != 0)
		{
			mvRef<mvAppItem> item = m_stagingArea[name];
			m_stagingArea.erase(name);
			if (item->getType() == mvAppItemType::mvStagingContainer)
			{
				for (auto& children : item->m_children)
				{
					for (auto& child : children)
						addItemWithRuntimeChecks(child, "", "");
				}
			}
			else
				addItemWithRuntimeChecks(item, "", "");
		}
		else
		{
			mvThrowPythonError(1000, "Staged item does not exist.");
			assert(false);
		}

	}

	void mvItemRegistry::stageItem(const std::string& name)
	{
		mvRef<mvAppItem> child;

		for (auto& window : m_roots)
		{
			child = window->stealChild(name);
			if (child)
				break;
		}

		if (child == nullptr)
		{
			mvThrowPythonError(1000, name + " not staged because it was not found");
			MV_ITEM_REGISTRY_WARN("Could not stage item, it was not found");
			return;
		}

		m_stagingArea[child->getName()] = child;

	}

	PyObject* mvItemRegistry::pop_container_stack(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto item = mvApp::GetApp()->getItemRegistry().popParent();
		if (item)
			return ToPyString(item->getName());
		else
			return GetPyNone();
	}

	PyObject* mvItemRegistry::empty_container_stack(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		mvApp::GetApp()->getItemRegistry().emptyParents();
		return GetPyNone();
	}

	PyObject* mvItemRegistry::top_container_stack(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto item = mvApp::GetApp()->getItemRegistry().topParent();
		if (item)
			return ToPyString(item->getName());
		else
			return GetPyNone();
	}

	PyObject* mvItemRegistry::push_container_stack(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["push_container_stack"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto parent = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (parent)
		{
			if (mvAppItem::DoesItemHaveFlag(parent.get(), MV_ITEM_DESC_CONTAINER))
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

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		mvApp::GetApp()->getItemRegistry().setStagingMode((bool)mode);
		return GetPyNone();
	}

	PyObject* mvItemRegistry::set_primary_window(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;
		int value;

		if (!(mvApp::GetApp()->getParsers())["set_primary_window"].parse(args, kwargs, __FUNCTION__, &item, &value))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		mvApp::GetApp()->getItemRegistry().setPrimaryWindow(item, value);

		return GetPyNone();
	}

	PyObject* mvItemRegistry::get_active_window(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		return ToPyString(mvApp::GetApp()->getItemRegistry().getActiveWindow());
	}

	PyObject* mvItemRegistry::move_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* item;
		const char* parent = "";
		const char* before = "";

		if (!(mvApp::GetApp()->getParsers())["move_item"].parse(args, kwargs, __FUNCTION__,
			&item, &parent, &before))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		mvApp::GetApp()->getItemRegistry().moveItem(item, parent, before);

		return GetPyNone();
	}

	PyObject* mvItemRegistry::delete_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* item;
		int childrenOnly = false;

		if (!(mvApp::GetApp()->getParsers())["delete_item"].parse(args, kwargs, __FUNCTION__, &item, &childrenOnly))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		mvApp::GetApp()->getItemRegistry().deleteItem(item, childrenOnly);

		return GetPyNone();

	}

	PyObject* mvItemRegistry::does_item_exist(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* item;

		if (!(mvApp::GetApp()->getParsers())["does_item_exist"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		if (mvApp::GetApp()->getItemRegistry().getItem(item))
			return ToPyBool(true);
		return ToPyBool(false);
	}

	PyObject* mvItemRegistry::move_item_up(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* item;

		if (!(mvApp::GetApp()->getParsers())["move_item_up"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		mvApp::GetApp()->getItemRegistry().moveItemUp(item);

		return GetPyNone();

	}

	PyObject* mvItemRegistry::move_item_down(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* item;

		if (!(mvApp::GetApp()->getParsers())["move_item_down"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		mvApp::GetApp()->getItemRegistry().moveItemDown(item);

		return GetPyNone();
	}

	PyObject* mvItemRegistry::reorder_items(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* container;
		int slot = 0;
		PyObject* new_order = nullptr;

		if (!(mvApp::GetApp()->getParsers())["reorder_items"].parse(args, kwargs, __FUNCTION__, 
			&container, &slot, &new_order))
			return GetPyNone();

		auto anew_order = ToStringVect(new_order);
		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());

		auto parent = mvApp::GetApp()->getItemRegistry().getItem(container);

		std::vector<mvRef<mvAppItem>>& children = parent->getChildren(slot);

		std::vector<mvRef<mvAppItem>> newchildren;
		newchildren.reserve(children.size());

		// todo: better sorting algorithm
		for (const auto& item : anew_order)
		{
			for (auto& child : children)
			{
				if (child->getName() == item)
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

		auto aitems = ToStringVect(items);
		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		for(const auto& item : aitems)
			mvApp::GetApp()->getItemRegistry().unstageItem(item);


		return GetPyNone();
	}

	PyObject* mvItemRegistry::stage_items(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* items = nullptr;

		if (!(mvApp::GetApp()->getParsers())["stage_items"].parse(args, kwargs, __FUNCTION__, &items))
			return GetPyNone();

		auto aitems = ToStringVect(items);
		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		for (const auto& item : aitems)
			mvApp::GetApp()->getItemRegistry().stageItem(item);

		return GetPyNone();
	}

	PyObject* mvItemRegistry::get_all_items(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		return ToPyList(mvApp::GetApp()->getItemRegistry().getAllItems());
	}

	PyObject* mvItemRegistry::get_windows(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		return ToPyList(mvApp::GetApp()->getItemRegistry().getWindows());
	}
}
