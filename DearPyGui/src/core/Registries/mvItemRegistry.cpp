#include "mvItemRegistry.h"
#include "PythonUtilities/mvPythonExceptions.h"
#include "mvProfiler.h"
#include "mvApp.h"
#include "Registries/mvItemRegistry.h"
#include "core/AppItems/mvAppItems.h"

namespace Marvel {

	mvItemRegistry* mvItemRegistry::s_instance = nullptr;

	mvItemRegistry* mvItemRegistry::GetItemRegistry()
	{
		if (s_instance)
			return s_instance;

		s_instance = new mvItemRegistry();
		return s_instance;
	}

	mvItemRegistry::mvItemRegistry()
	{
		mvEventBus::Subscribe(this, 0, mvEVT_CATEGORY_ITEM);
		mvEventBus::Subscribe(this, mvEVT_END_FRAME);
		mvEventBus::Subscribe(this, mvEVT_PRE_RENDER_RESET);
		mvEventBus::Subscribe(this, mvEVT_RENDER);

		auto add_hidden_window = [&](mvAppItem* item, const std::string& label) {
			m_backWindows.push_back(item);
			m_backWindows.back()->setLabel(label);
			m_backWindows.back()->hide();
		};

		add_hidden_window(new mvAboutWindow("about##standard"), "About Dear PyGui");
		add_hidden_window(new mvDocWindow("documentation##standard"), "Core Documentation");
		add_hidden_window(new mvDebugWindow("debug##standard"), "Dear PyGui Debug");
		add_hidden_window(new mvMetricsWindow("metrics##standard"), "Metrics");
		add_hidden_window(new mvStyleWindow("style##standard"), "Dear PyGui Style Editor");
		add_hidden_window(new mvFileDialog(), "FileDialog");
	}

	void mvItemRegistry::resetWindowStates()
	{
		// resets app items states (i.e. hovered)
		for (auto window : m_frontWindows)
			window->resetState();
		for (auto window : m_backWindows)
			window->resetState();
	}

	void mvItemRegistry::draw()
	{
		// resets app items states (i.e. hovered)
		for (auto window : m_frontWindows)
			window->draw();
		for (auto window : m_backWindows)
			window->draw();
	}

	bool mvItemRegistry::onEvent(mvEvent& event)
	{
		mvEventDispatcher dispatcher(event);

		dispatcher.dispatch(BIND_EVENT_METH(mvItemRegistry::onAddItem), mvEVT_ADD_ITEM);
		dispatcher.dispatch(BIND_EVENT_METH(mvItemRegistry::onDeleteItem), mvEVT_DELETE_ITEM);
		dispatcher.dispatch(BIND_EVENT_METH(mvItemRegistry::onMoveItem), mvEVT_MOVE_ITEM);
		dispatcher.dispatch(BIND_EVENT_METH(mvItemRegistry::onMoveItemUp), mvEVT_MOVE_ITEM_UP);
		dispatcher.dispatch(BIND_EVENT_METH(mvItemRegistry::onMoveItemDown), mvEVT_MOVE_ITEM_DOWN);
		dispatcher.dispatch(BIND_EVENT_METH(mvItemRegistry::onEndFrame), mvEVT_END_FRAME);
		dispatcher.dispatch(BIND_EVENT_METH(mvItemRegistry::onPreRenderReset), mvEVT_PRE_RENDER_RESET);
		dispatcher.dispatch(BIND_EVENT_METH(mvItemRegistry::onRender), mvEVT_RENDER);

		return event.handled;
	}

	bool mvItemRegistry::onRender(mvEvent& event)
	{
		draw();
		return false;
	}

	bool mvItemRegistry::onPreRenderReset(mvEvent& event)
	{
		resetWindowStates();
		return false;
	}

	bool mvItemRegistry::onEndFrame(mvEvent& event)
	{

		Py_BEGIN_ALLOW_THREADS
		mvItemRegistry::GetItemRegistry()->postDeleteItems();
		mvItemRegistry::GetItemRegistry()->postAddItems();
		mvItemRegistry::GetItemRegistry()->postAddPopups();
		mvItemRegistry::GetItemRegistry()->postMoveItems();

		Py_END_ALLOW_THREADS

		return false;
	}

	bool mvItemRegistry::onAddItem(mvEvent& event)
	{

		mvAppItem* item = GetEPtr<mvAppItem*>(event, "ITEM");

		return addItem(item);
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

	bool mvItemRegistry::addRuntimeItem(const std::string& parent, const std::string& before, mvAppItem* item)
	{
		if (!mvApp::GetApp()->checkIfMainThread())
			return false;

		m_newItemVec.push_back({ item, before, parent });

		return true;
	}

	bool mvItemRegistry::addItemAfter(const std::string& prev, mvAppItem* item)
	{
		if (!mvApp::GetApp()->checkIfMainThread())
			return false;

		m_orderedVec.push_back({ item, prev });
		return true;
	}

	void mvItemRegistry::pushParent(mvAppItem* item)
	{
		m_parents.push(item);
	}

	mvAppItem* mvItemRegistry::popParent()
	{
		if (m_parents.empty())
		{
			ThrowPythonException("No parent to pop.");
			return nullptr;
		}

		mvAppItem* item = m_parents.top();
		m_parents.pop();
		return item;
	}

	void mvItemRegistry::emptyParents()
	{
		while (!m_parents.empty())
			m_parents.pop();
	}

	mvAppItem* mvItemRegistry::topParent()
	{
		if (m_parents.size() != 0)
			return m_parents.top();
		return nullptr;
	}

	mvAppItem* mvItemRegistry::getItem(const std::string& name, bool ignoreRuntime)
	{
		if (!mvApp::GetApp()->checkIfMainThread())
			return nullptr;

		return getItemAsync(name, ignoreRuntime);
	}

	mvAppItem* mvItemRegistry::getItemAsync(const std::string& name, bool ignoreRuntime)
	{

		mvAppItem* item = nullptr;

		if (!ignoreRuntime)
			item = getRuntimeItem(name);

		if (item)
			return item;

		for (auto window : m_frontWindows)
		{
			if (window->getName() == name)
				return window;

			auto child = window->getChild(name);
			if (child)
				return child;
		}

		for (auto window : m_backWindows)
		{
			if (window->getName() == name)
				return window;

			auto child = window->getChild(name);
			if (child)
				return child;
		}

		return nullptr;
	}

	mvAppItem* mvItemRegistry::getRuntimeItem(const std::string& name)
	{

		for (auto& item : m_newItemVec)
		{

			if (item.item->getName() == name)
				return item.item;
		}

		for (auto& item : m_orderedVec)
		{

			if (item.item->getName() == name)
				return item.item;
		}

		return nullptr;
	}

	mvWindowAppitem* mvItemRegistry::getWindow(const std::string& name)
	{
		if (!mvApp::GetApp()->checkIfMainThread())
			return nullptr;

		mvAppItem* item = getRuntimeItem(name);
		if (item == nullptr)
			item = getItem(name);
		if (item == nullptr)
			return nullptr;

		if (item->getType() == mvAppItemType::Window)
			return static_cast<mvWindowAppitem*>(item);

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

	bool mvItemRegistry::addItem(mvAppItem* item)
	{
		if (!mvApp::GetApp()->checkIfMainThread())
			return false;

		static int count = 0;
		count++;

		if (!item->getDescription().duplicatesAllowed)
		{
			if (getItem(item->getName()))
			{
				std::string message = item->getName() + " " + std::to_string(count);
				ThrowPythonException(message + ": Items of this type must have unique names");
				delete item;
				item = nullptr;
				return false;
			}
		}

		mvAppItem* parentitem = topParent();
		if (parentitem == nullptr)
		{
			std::string message = item->getName();
			ThrowPythonException(message + ": Parent for this item not found or the parent stack is empty.");
			delete item;
			item = nullptr;
			return false;
		}

		item->setParent(parentitem);
		parentitem->addChild(item);

		return true;
	}

	bool mvItemRegistry::addWindow(mvAppItem* item)
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
				if (window->getName() == item)
				{
					frontWindowDeleting = true;
					break;
				}
			}

			for (auto window : m_backWindows)
			{
				if (window->getName() == item)
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
				std::vector<mvAppItem*> oldwindows = m_frontWindows;

				m_frontWindows.clear();

				for (auto window : oldwindows)
				{
					if (window->getName() == item)
					{
						delete window;
						window = nullptr;
						deletedItem = true;
						continue;
					}
					m_frontWindows.push_back(window);
				}
			}

			if (backWindowDeleting)
			{
				std::vector<mvAppItem*> oldwindows = m_backWindows;

				m_backWindows.clear();

				for (auto window : oldwindows)
				{
					if (window->getName() == item)
					{
						delete window;
						window = nullptr;
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
					if (getItem(newItem.item->getName(), true))
					{
						std::string message = newItem.item->getName();
						ThrowPythonException(message + ": Items of this type must have unique names");
						delete newItem.item;
						newItem.item = nullptr;
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
					ThrowPythonException(newItem.item->getName() + " not added because its parent was not found");
					delete newItem.item;
					newItem.item = nullptr;
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

				if (getItem(popup.item->getName(), true))
				{
					std::string message = popup.item->getName();
					ThrowPythonException(message + ": Items of this type must have unique names");
					delete popup.item;
					popup.item = nullptr;
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
					ThrowPythonException(popup.item->getName() + " not added because its parent was not found");
					delete popup.item;
					popup.item = nullptr;
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

				mvAppItem* child = nullptr;

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
		for (auto window : m_frontWindows)
		{
			delete window;
			window = nullptr;
		}
		m_frontWindows.clear();

		for (auto window : m_backWindows)
		{
			delete window;
			window = nullptr;
		}
		m_backWindows.clear();
	}

}