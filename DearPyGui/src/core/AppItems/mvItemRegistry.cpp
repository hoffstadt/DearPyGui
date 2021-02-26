#include "mvItemRegistry.h"
#include "mvProfiler.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvAppItems.h"

namespace Marvel {

	mvItemRegistry::mvItemRegistry()
	{
		mvEventBus::Subscribe(this, 0, mvEVT_CATEGORY_ITEM);
		mvEventBus::Subscribe(this, mvEVT_PRE_RENDER_RESET);
		mvEventBus::Subscribe(this, mvEVT_RENDER);
		mvEventBus::Subscribe(this, mvEVT_ACTIVE_WINDOW);

		auto add_hidden_window = [&](mvRef<mvAppItem> item, const std::string& label) {
			m_backWindows.push_back(item);
			m_backWindows.back()->setLabel(label);
			m_backWindows.back()->hide();
		};

#ifndef MV_CPP
		add_hidden_window(CreateRef<mvDocWindow>("documentation##standard"), "Core Documentation");
		add_hidden_window(CreateRef<mvDebugWindow>("debug##standard"), "Dear PyGui Debug");
#endif // !MV_CPP

		add_hidden_window(CreateRef<mvAboutWindow>("about##standard"), "About Dear PyGui");
		add_hidden_window(CreateRef<mvMetricsWindow>("metrics##standard"), "Metrics");
		add_hidden_window(CreateRef<mvStyleWindow>("style##standard"), "Dear PyGui Style Editor");
		add_hidden_window(CreateRef<mvFileDialog>(), "FileDialog");
	}

	mvItemRegistry::~mvItemRegistry()
	{
		mvEventBus::UnSubscribe(this);
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

		// delete items that are parent only
		if(childrenOnly)
		{
			auto item = getItem(name);
			if (item)
			{
				item->deleteChildren();
				return true;
			}
		}

		bool deletedItem = false;

		// try to delete item
		for (auto window : m_frontWindows)
		{
			deletedItem = window->deleteChild(name);
			if (deletedItem)
				break;
		}

		if (!deletedItem)
		{
			for (auto window : m_backWindows)
			{
				deletedItem = window->deleteChild(name);
				if (deletedItem)
					break;
			}
		}

		bool frontWindowDeleting = false;
		bool backWindowDeleting = false;

		// check if attempting to delete a window
		for (auto window : m_frontWindows)
		{
			if (window->m_core_config.name == name)
			{
				frontWindowDeleting = true;
				break;
			}
		}

		for (auto window : m_backWindows)
		{
			if (window->m_core_config.name == name)
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
				if (window->m_core_config.name == name)
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
				if (window->m_core_config.name == name)
				{
					deletedItem = true;
					continue;
				}
				m_backWindows.push_back(window);
			}
		}

		if (!deletedItem)
			ThrowPythonException(name + " not deleted because it was not found");

		return deletedItem;
	}

	bool mvItemRegistry::moveItem(const std::string& name, const std::string& parent, const std::string& before)
	{

		mvRef<mvAppItem> child = nullptr;

		bool movedItem = false;

		for (auto window : m_frontWindows)
		{
			child = window->stealChild(name);
			if (child)
				break;
		}

		if (child == nullptr)
			ThrowPythonException(name + " not moved because it was not found");

		if (child)
			addRuntimeItem(parent, before, child);

		return movedItem;
	}

	bool mvItemRegistry::moveItemUp(const std::string& name)
	{

		bool movedItem = false;

		for (auto window : m_frontWindows)
		{
			movedItem = window->moveChildUp(name);
			if (movedItem)
				break;
		}

		if (!movedItem)
			ThrowPythonException(name + " not moved because it was not found");

		return movedItem;
	}

	bool mvItemRegistry::moveItemDown(const std::string& name)
	{

		bool movedItem = false;

		for (auto window : m_frontWindows)
		{
			movedItem = window->moveChildDown(name);
			if (movedItem)
				break;
		}

		if (!movedItem)
			ThrowPythonException(name + " not moved because it was not found");

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

		MV_PROFILE_FUNCTION();

		// resets app items states (i.e. hovered)
		for (auto window : m_frontWindows)
			window->draw();
		for (auto window : m_backWindows)
			window->draw();

		return false;
	}

	bool mvItemRegistry::onPreRenderReset(mvEvent& event)
	{

		MV_PROFILE_FUNCTION();

		// resets app items states (i.e. hovered)
		for (auto window : m_frontWindows)
			window->resetState();
		for (auto window : m_backWindows)
			window->resetState();

		return false;
	}

	bool mvItemRegistry::onActiveWindow(mvEvent& event)
	{

		m_activeWindow = GetEString(event, "WINDOW");

		return false;
	}

	bool mvItemRegistry::addRuntimeItem(const std::string& parent, const std::string& before, mvRef<mvAppItem> item)
	{
		// add runtime items
		bool addedItem = false;

		if (!item->getDescription().duplicatesAllowed)
		{
			if (getItem(item->m_core_config.name))
			{
				std::string message = item->m_core_config.name;
				ThrowPythonException(message + ": Items of this type must have unique names");
				return false;
			}
		}

		if (item->getDescription().root)
		{
			m_frontWindows.push_back(item);
			return true;
		}

		for (auto window : m_frontWindows)
		{
			addedItem = window->addRuntimeChild(parent, before, item);
			if (addedItem)
				break;
		}

		if (!addedItem)
		{
			ThrowPythonException(item->m_core_config.name + " not added because its parent was not found");
		}

		return addedItem;
	}

	bool mvItemRegistry::addItemAfter(const std::string& prev, mvRef<mvAppItem> item)
	{
		// add popup items
		bool addedItem = false;

		if (getItem(item->m_core_config.name))
		{
			std::string message = item->m_core_config.name;
			ThrowPythonException(message + ": Items of this type must have unique names");
			return false;
		}

		for (auto window : m_frontWindows)
		{
			addedItem = window->addChildAfter(prev, item);
			if (addedItem)
				break;
		}

		if (!addedItem)
		{
			ThrowPythonException(item->m_core_config.name + " not added because its parent was not found");
		}
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

	mvRef<mvAppItem> mvItemRegistry::getItem(const std::string& name)
	{
		mvRef<mvAppItem> item = nullptr;

		for (auto window : m_frontWindows)
		{
			if (window->m_core_config.name == name)
				return window;

			auto child = window->getChild(name);
			if (child)
				return child;
		}

		for (auto window : m_backWindows)
		{
			if (window->m_core_config.name == name)
				return window;

			auto child = window->getChild(name);
			if (child)
				return child;
		}

		return nullptr;
	}

	mvWindowAppItem* mvItemRegistry::getWindow(const std::string& name)
	{

		mvRef<mvAppItem> item = getItem(name);
		if (item == nullptr)
			return nullptr;

		if (item->getType() == mvAppItemType::Window)
			return static_cast<mvWindowAppItem*>(item.get());

		return nullptr;
	}

	bool mvItemRegistry::addItem(mvRef<mvAppItem> item)
	{

		static int count = 0;
		count++;

		if (!item->getDescription().duplicatesAllowed)
		{
			if (getItem(item->m_core_config.name))
			{
				std::string message = item->m_core_config.name + " " + std::to_string(count);
				ThrowPythonException(message + ": Items of this type must have unique names");
				return false;
			}
		}

		mvRef<mvAppItem> parentitem = topParent();
		if (parentitem == nullptr)
		{
			std::string message = item->m_core_config.name;
			ThrowPythonException(message + ": Parent for this item not found or the parent stack is empty.");
			return false;
		}

		item->m_parent = parentitem.get();
		parentitem->m_children.emplace_back(item);

		return true;
	}

	bool mvItemRegistry::addWindow(mvRef<mvAppItem> item)
	{

		m_frontWindows.push_back(item);
		return true;
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

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());

		// remove bad parent stack item
		if (item->getDescription().root && topParent() != nullptr)
		{
			emptyParents();

			ThrowPythonException("Parent stack not empty. Adding window will empty the parent stack. Don't forget to end container types.");
		}

		if (item->getType() != mvAppItemType::Node && topParent() != nullptr)
		{
			if (topParent()->getType() == mvAppItemType::NodeEditor)
			{
				ThrowPythonException("Node editor children must be nodes only.");
				return false;
			}
		}

		if (item->getType() != mvAppItemType::NodeAttribute && topParent() != nullptr)
		{
			if (topParent()->getType() == mvAppItemType::Node)
			{
				ThrowPythonException("Node children must be nodes attributes only.");
				return false;
			}
		}

		if (item->getType() == mvAppItemType::Popup || item->getType() == mvAppItemType::Tooltip)
		{
			addItemAfter(parent, item);
			return true;
		}

		// window runtime adding
		if (item->getDescription().root && mvApp::IsAppStarted())
			addRuntimeItem("", "", item);

		// window compile adding
		else if (item->getDescription().root)
			addWindow(item);

		// typical run time adding
		else if ((!std::string(parent).empty() || !std::string(before).empty()) && mvApp::IsAppStarted())
			addRuntimeItem(parent, before, item);

		// adding without specifying before or parent, instead using parent stack
		else if (std::string(parent).empty() && std::string(before).empty() && mvApp::IsAppStarted() && topParent() != nullptr)
			addRuntimeItem(topParent()->m_core_config.name, before, item);

		// adding without specifying before or parent, but with empty stack (add to main window)
		else if (std::string(parent).empty() && std::string(before).empty() && mvApp::IsAppStarted())
		{
			ThrowPythonException("Parent stack is empty. You must specify 'before' or 'parent' widget.");
			return false;
		}

		// adding normally but using the runtime style of adding
		else if (!std::string(parent).empty() && !mvApp::IsAppStarted())
			addRuntimeItem(parent, before, item);

		// typical adding before runtime
		else if (std::string(parent).empty() && !mvApp::IsAppStarted() && std::string(before).empty())
			addItem(item);


		return true;
	}

	std::string mvItemRegistry::getItemParentName(const std::string& name)
	{
		mvRef<mvAppItem> item = getItem(name);
		if (item)
			return item->m_parent->m_core_config.name;
		
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
				childList.emplace_back(child->m_core_config.name);
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
				childList.emplace_back(child->m_core_config.name);
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
			childList.emplace_back(window->m_core_config.name);

		for (auto window : m_backWindows)
			childList.emplace_back(window->m_core_config.name);

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
			ThrowPythonException("Window does not exists.");

		// reset other windows
		for (auto window : m_frontWindows)
		{
			if (window->m_core_config.name != name)
				static_cast<mvWindowAppItem*>(window.get())->setWindowAsMainStatus(false);
		}

		mvAppLog::Focus();
	}
}
