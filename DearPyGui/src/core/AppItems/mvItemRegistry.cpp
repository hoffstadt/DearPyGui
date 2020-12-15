#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvProfiler.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvAppItems.h"

namespace Marvel {

	mvItemRegistry::mvItemRegistry()
	{
		mvEventBus::Subscribe(this, 0, mvEVT_CATEGORY_ITEM);
		mvEventBus::Subscribe(this, mvEVT_END_FRAME);
		mvEventBus::Subscribe(this, mvEVT_PRE_RENDER_RESET);
		mvEventBus::Subscribe(this, mvEVT_RENDER);
		mvEventBus::Subscribe(this, mvEVT_ACTIVE_WINDOW);

		auto add_hidden_window = [&](mvRef<mvAppItem> item, const std::string& label) {
			m_backWindows.push_back(item);
			m_backWindows.back()->setLabel(label);
			m_backWindows.back()->hide();
		};

		add_hidden_window(CreateRef<mvAboutWindow>("about##standard"), "About Dear PyGui");
		add_hidden_window(CreateRef<mvDocWindow>("documentation##standard"), "Core Documentation");
		add_hidden_window(CreateRef<mvDebugWindow>("debug##standard"), "Dear PyGui Debug");
		add_hidden_window(CreateRef<mvMetricsWindow>("metrics##standard"), "Metrics");
		add_hidden_window(CreateRef<mvStyleWindow>("style##standard"), "Dear PyGui Style Editor");
		add_hidden_window(CreateRef<mvFileDialog>(), "FileDialog");
	}

	mvItemRegistry::~mvItemRegistry()
	{
		mvEventBus::UnSubscribe(this);
	}

	bool mvItemRegistry::onEvent(mvEvent& event)
	{
		mvEventDispatcher dispatcher(event);

		dispatcher.dispatch(BIND_EVENT_METH(mvItemRegistry::onDeleteItem), mvEVT_DELETE_ITEM);
		dispatcher.dispatch(BIND_EVENT_METH(mvItemRegistry::onMoveItem), mvEVT_MOVE_ITEM);
		dispatcher.dispatch(BIND_EVENT_METH(mvItemRegistry::onMoveItemUp), mvEVT_MOVE_ITEM_UP);
		dispatcher.dispatch(BIND_EVENT_METH(mvItemRegistry::onMoveItemDown), mvEVT_MOVE_ITEM_DOWN);
		dispatcher.dispatch(BIND_EVENT_METH(mvItemRegistry::onEndFrame), mvEVT_END_FRAME);
		dispatcher.dispatch(BIND_EVENT_METH(mvItemRegistry::onPreRenderReset), mvEVT_PRE_RENDER_RESET);
		dispatcher.dispatch(BIND_EVENT_METH(mvItemRegistry::onRender), mvEVT_RENDER);
		dispatcher.dispatch(BIND_EVENT_METH(mvItemRegistry::onActiveWindow), mvEVT_ACTIVE_WINDOW);

		return event.handled;
	}

	bool mvItemRegistry::onRender(mvEvent& event)
	{

		// resets app items states (i.e. hovered)
		for (auto window : m_frontWindows)
			window->draw();
		for (auto window : m_backWindows)
			window->draw();

		return false;
	}

	bool mvItemRegistry::onPreRenderReset(mvEvent& event)
	{

		// resets app items states (i.e. hovered)
		for (auto window : m_frontWindows)
			window->resetState();
		for (auto window : m_backWindows)
			window->resetState();

		return false;
	}

	bool mvItemRegistry::onEndFrame(mvEvent& event)
	{

		Py_BEGIN_ALLOW_THREADS
		mvApp::GetApp()->getItemRegistry().postDeleteItems();
		mvApp::GetApp()->getItemRegistry().postAddItems();
		mvApp::GetApp()->getItemRegistry().postAddPopups();
		mvApp::GetApp()->getItemRegistry().postMoveItems();

		Py_END_ALLOW_THREADS

		return false;
	}

	bool mvItemRegistry::onActiveWindow(mvEvent& event)
	{

		m_activeWindow = GetEString(event, "WINDOW");

		return false;
	}

	bool mvItemRegistry::onDeleteItem(mvEvent& event)
	{
		if (GetEBool(event, "CHILDREN_ONLY"))
			m_deleteChildrenQueue.push(GetEString(event, "ITEM"));
		else
			m_deleteQueue.push_back(GetEString(event, "ITEM"));

		return true;
	}

	bool mvItemRegistry::onMoveItem(mvEvent& event)
	{
		m_moveVec.push({ 
			GetEString(event, "ITEM"), 
			GetEString(event, "PARENT"), 
			GetEString(event, "BEFORE") });
		return true;
	}

	bool mvItemRegistry::onMoveItemUp(mvEvent& event)
	{
		m_upQueue.push(GetEString(event, "ITEM"));
		return true;
	}

	bool mvItemRegistry::onMoveItemDown(mvEvent& event)
	{
		m_downQueue.push(GetEString(event, "ITEM"));
		return true;
	}

	bool mvItemRegistry::addRuntimeItem(const std::string& parent, const std::string& before, mvRef<mvAppItem> item)
	{
		if (!mvApp::GetApp()->checkIfMainThread())
			return false;

		m_newItemVec.push_back({ item, before, parent });

		return true;
	}

	bool mvItemRegistry::addItemAfter(const std::string& prev, mvRef<mvAppItem> item)
	{
		if (!mvApp::GetApp()->checkIfMainThread())
			return false;

		m_orderedVec.push_back({ item, prev });
		return true;
	}

	void mvItemRegistry::pushParent(mvRef<mvAppItem> item)
	{
		m_parents.push(item);
	}

	mvRef<mvAppItem> mvItemRegistry::popParent()
	{
		if (m_parents.empty())
		{
			ThrowPythonException("No parent to pop.");
			return nullptr;
		}

		auto item = m_parents.top();
		m_parents.pop();
		return item;
	}

	void mvItemRegistry::emptyParents()
	{
		while (!m_parents.empty())
			m_parents.pop();
	}

	mvRef<mvAppItem> mvItemRegistry::topParent()
	{
		if (!m_parents.empty())
			return m_parents.top();
		return nullptr;
	}

	mvRef<mvAppItem> mvItemRegistry::getItem(const std::string& name, bool ignoreRuntime)
	{
		if (!mvApp::GetApp()->checkIfMainThread())
			return nullptr;

		return getItemAsync(name, ignoreRuntime);
	}

	mvRef<mvAppItem> mvItemRegistry::getItemAsync(const std::string& name, bool ignoreRuntime)
	{

		mvRef<mvAppItem> item = nullptr;

		if (!ignoreRuntime)
			item = getRuntimeItem(name);

		if (item)
			return item;

		for (auto window : m_frontWindows)
		{
			if (window->m_name == name)
				return window;

			auto child = window->getChild(name);
			if (child)
				return child;
		}

		for (auto window : m_backWindows)
		{
			if (window->m_name == name)
				return window;

			auto child = window->getChild(name);
			if (child)
				return child;
		}

		return nullptr;
	}

	mvRef<mvAppItem> mvItemRegistry::getRuntimeItem(const std::string& name)
	{

		for (auto& item : m_newItemVec)
		{

			if (item.item->m_name == name)
				return item.item;
		}

		for (auto& item : m_orderedVec)
		{

			if (item.item->m_name == name)
				return item.item;
		}

		return nullptr;
	}

	mvWindowAppItem* mvItemRegistry::getWindow(const std::string& name)
	{
		if (!mvApp::GetApp()->checkIfMainThread())
			return nullptr;

		mvRef<mvAppItem> item = getRuntimeItem(name);
		if (item == nullptr)
			item = getItem(name);
		if (item == nullptr)
			return nullptr;

		if (item->getType() == mvAppItemType::Window)
			return static_cast<mvWindowAppItem*>(item.get());

		return nullptr;
	}

	bool mvItemRegistry::isItemToBeDeleted(const std::string& name) const
	{
		for (const auto& item : m_deleteQueue)
		{
			if (name == item)
				return true;
		}

		return false;
	}

	bool mvItemRegistry::addItem(mvRef<mvAppItem> item)
	{
		if (!mvApp::GetApp()->checkIfMainThread())
			return false;

		static int count = 0;
		count++;

		if (!item->getDescription().duplicatesAllowed)
		{
			if (getItem(item->m_name))
			{
				std::string message = item->m_name + " " + std::to_string(count);
				ThrowPythonException(message + ": Items of this type must have unique names");
				return false;
			}
		}

		mvRef<mvAppItem> parentitem = topParent();
		if (parentitem == nullptr)
		{
			std::string message = item->m_name;
			ThrowPythonException(message + ": Parent for this item not found or the parent stack is empty.");
			return false;
		}

		item->m_parent = parentitem.get();
		parentitem->m_children.emplace_back(item);

		return true;
	}

	bool mvItemRegistry::addWindow(mvRef<mvAppItem> item)
	{
		if (!mvApp::GetApp()->checkIfMainThread())
			return false;

		m_frontWindows.push_back(item);
		return true;
	}

	void mvItemRegistry::postDeleteItems()
	{
		MV_PROFILE_FUNCTION()

			// delete items from the delete queue
			while (!m_deleteChildrenQueue.empty())
			{
				auto item = getItem(m_deleteChildrenQueue.front());
				if (item)
					item->deleteChildren();
				m_deleteChildrenQueue.pop();
			}

		// delete items from the delete queue
		for(auto& item : m_deleteQueue)
		{
			bool deletedItem = false;

			// try to delete item
			for (auto window : m_frontWindows)
			{
				deletedItem = window->deleteChild(item);
				if (deletedItem)
					break;
			}

			if (!deletedItem)
			{
				for (auto window : m_backWindows)
				{
					deletedItem = window->deleteChild(item);
					if (deletedItem)
						break;
				}
			}

			bool frontWindowDeleting = false;
			bool backWindowDeleting = false;

			// check if attempting to delete a window
			for (auto window : m_frontWindows)
			{
				if (window->m_name == item)
				{
					frontWindowDeleting = true;
					break;
				}
			}

			for (auto window : m_backWindows)
			{
				if (window->m_name == item)
				{
					backWindowDeleting = true;
					break;
				}
			}

			// delete window and update window vector
			// this should be changed to a different data
			// structure
			if (frontWindowDeleting)
			{
				std::vector<mvRef<mvAppItem>> oldwindows = m_frontWindows;

				m_frontWindows.clear();

				for (auto window : oldwindows)
				{
					if (window->m_name == item)
					{
						deletedItem = true;
						continue;
					}
					m_frontWindows.push_back(window);
				}
			}

			if (backWindowDeleting)
			{
				std::vector<mvRef<mvAppItem>> oldwindows = m_backWindows;

				m_backWindows.clear();

				for (auto window : oldwindows)
				{
					if (window->m_name == item)
					{
						deletedItem = true;
						continue;
					}
					m_backWindows.push_back(window);
				}
			}

			if (!deletedItem)
				ThrowPythonException(item + " not deleted because it was not found");

			
		}
		m_deleteQueue.clear();
	}

	void mvItemRegistry::postAddItems()
	{
		MV_PROFILE_FUNCTION()

			// add runtime items
			for (auto& newItem : m_newItemVec)
			{

				bool addedItem = false;

				if (!newItem.item->getDescription().duplicatesAllowed)
				{
					if (getItem(newItem.item->m_name, true))
					{
						std::string message = newItem.item->m_name;
						ThrowPythonException(message + ": Items of this type must have unique names");
						continue;
					}
				}

				if (newItem.item->getDescription().root)
				{
					m_frontWindows.push_back(newItem.item);
					continue;
				}

				for (auto window : m_frontWindows)
				{
					addedItem = window->addRuntimeChild(newItem.parent, newItem.before, newItem.item);
					if (addedItem)
						break;
				}

				if (!addedItem)
				{
					for (auto otherItems : m_orderedVec)
					{
						addedItem = otherItems.item->addRuntimeChild(newItem.parent, newItem.before, newItem.item);
						if (addedItem)
							break;
					}
				}

				if (!addedItem)
				{
					ThrowPythonException(newItem.item->m_name + " not added because its parent was not found");
				}

			}

		m_newItemVec.clear();
	}

	void mvItemRegistry::postAddPopups()
	{
		MV_PROFILE_FUNCTION()

			// add popup items
			for (auto& popup : m_orderedVec)
			{

				bool addedItem = false;

				if (getItem(popup.item->m_name, true))
				{
					std::string message = popup.item->m_name;
					ThrowPythonException(message + ": Items of this type must have unique names");
					continue;
				}

				for (auto window : m_frontWindows)
				{
					addedItem = window->addChildAfter(popup.prev, popup.item);
					if (addedItem)
						break;
				}

				if (!addedItem)
				{
					ThrowPythonException(popup.item->m_name + " not added because its parent was not found");
				}

			}
		m_orderedVec.clear();
	}

	void mvItemRegistry::postMoveItems()
	{
		MV_PROFILE_FUNCTION()

			// move
			while (!m_moveVec.empty())
			{
				StolenChild childrequest = m_moveVec.front();
				m_moveVec.pop();

				mvRef<mvAppItem> child = nullptr;

				bool movedItem = false;

				for (auto window : m_frontWindows)
				{
					child = window->stealChild(childrequest.item);
					if (child)
						break;
				}

				if (child == nullptr)
					ThrowPythonException(childrequest.item + " not moved because it was not found");

				if (child)
					addRuntimeItem(childrequest.parent, childrequest.before, child);
			}

		// move items up
		while (!m_upQueue.empty())
		{
			std::string& itemname = m_upQueue.front();

			bool movedItem = false;

			for (auto window : m_frontWindows)
			{
				movedItem = window->moveChildUp(itemname);
				if (movedItem)
					break;
			}

			if (!movedItem)
				ThrowPythonException(itemname + " not moved because it was not found");

			m_upQueue.pop();
		}

		// move items down
		while (!m_downQueue.empty())
		{
			std::string& itemname = m_downQueue.front();

			bool movedItem = false;

			for (auto window : m_frontWindows)
			{
				movedItem = window->moveChildDown(itemname);
				if (movedItem)
					break;
			}

			if (!movedItem)
				ThrowPythonException(itemname + " not moved because it was not found");

			m_downQueue.pop();
		}
	}

	void mvItemRegistry::clearRegistry()
	{
		m_frontWindows.clear();
		m_backWindows.clear();
	}

	bool mvItemRegistry::addItemWithRuntimeChecks(mvRef<mvAppItem> item, const char* parent, const char* before)
	{

		if (item == nullptr)
			return false;

		// remove bad parent stack item
		if (item->getDescription().root && topParent() != nullptr)
		{
			emptyParents();
			ThrowPythonException("Parent stack not empty. Adding window will empty the parent stack. Don't forget to end container types.");
		}

		if (item->getType() == mvAppItemType::Popup || item->getType() == mvAppItemType::Tooltip)
			return addItemAfter(parent, item);

		// window runtime adding
		if (item->getDescription().root && mvApp::IsAppStarted())
			return addRuntimeItem("", "", item);

		// window compile adding
		else if (item->getDescription().root)
			return addWindow(item);

		// typical run time adding
		else if ((!std::string(parent).empty() || !std::string(before).empty()) && mvApp::IsAppStarted())
			return addRuntimeItem(parent, before, item);

		// adding without specifying before or parent, instead using parent stack
		else if (std::string(parent).empty() && std::string(before).empty() && mvApp::IsAppStarted() && topParent() != nullptr)
			return addRuntimeItem(topParent()->m_name, before, item);

		// adding without specifying before or parent, but with empty stack (add to main window)
		else if (std::string(parent).empty() && std::string(before).empty() && mvApp::IsAppStarted())
		{
			ThrowPythonException("Parent stack is empty. You must specify 'before' or 'parent' widget.");
			return false;
		}

		// adding normally but using the runtime style of adding
		else if (!std::string(parent).empty() && !mvApp::IsAppStarted())
			return addRuntimeItem(parent, before, item);

		// typical adding before runtime
		else if (std::string(parent).empty() && !mvApp::IsAppStarted() && std::string(before).empty())
			return addItem(item);


		return false;
	}

	std::string mvItemRegistry::getItemParentName(const std::string& name)
	{
		mvRef<mvAppItem> item = getItem(name);
		if (item)
			return item->m_parent->m_name;
		
		return "";
	}

	std::vector<std::string> mvItemRegistry::getItemChildren(const std::string& name)
	{

		mvRef<mvAppItem> item = getItem(name);

		std::vector<std::string> childList;

		if (item)
		{
			auto children = item->m_children;
			for (auto child : children)
				childList.emplace_back(child->m_name);
		}

		return childList;
	}

	std::vector<std::string> mvItemRegistry::getAllItems()
	{

		std::vector<std::string> childList;

		// to help recursively retrieve children
		std::function<void(mvRef<mvAppItem>)> ChildRetriever;
		ChildRetriever = [&childList, &ChildRetriever](mvRef<mvAppItem> item) {
			auto children = item->m_children;
			for (auto child : children)
			{
				childList.emplace_back(child->m_name);
				if (child->getDescription().container)
					ChildRetriever(child);
			}

		};

		for (auto window : m_frontWindows)
			ChildRetriever(window);
		for (auto window : m_backWindows)
			ChildRetriever(window);

		return childList;
	}

	std::vector<std::string> mvItemRegistry::getWindows()
	{
		std::vector<std::string> childList;
		for (auto window : m_frontWindows)
			childList.emplace_back(window->m_name);

		for (auto window : m_backWindows)
			childList.emplace_back(window->m_name);

		return childList;
	}

	void mvItemRegistry::setPrimaryWindow(const std::string& name, bool value)
	{

		// reset other windows
		for (auto window : m_frontWindows)
		{
			if (window->m_name != name)
				static_cast<mvWindowAppItem*>(window.get())->setWindowAsMainStatus(false);
		}

		mvAppLog::Focus();

		mvWindowAppItem* window = mvApp::GetApp()->getItemRegistry().getWindow(name);

		if (window)
			window->setWindowAsMainStatus(value);
		else
			ThrowPythonException("Window does not exists.");

	}
}