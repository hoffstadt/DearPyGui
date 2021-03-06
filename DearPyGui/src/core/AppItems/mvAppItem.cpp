#include "mvAppItem.h"
#include "mvApp.h"
#include "mvInput.h"
#include "mvItemRegistry.h"

namespace Marvel{

	mvAppItem::mvAppItem(const std::string& name)
	{
		m_core_config.name = name;
		m_label = name + "###" + name;
		m_core_config.label = name;
		m_state.setParent(this);
	}

	mvAppItem::mvAppItem(const mvAppItemConfig& config)
	{
		m_core_config.name = config.name;
		m_label = config.name + "###" + config.name;
		m_core_config.label = config.name;
		m_state.setParent(this);

		if (m_label != "__DearPyGuiDefault")
		{
			m_label = config.label + "###" + config.label;
			m_core_config.label = config.label;
		}
	}

	void mvAppItem::updateCoreConfig()
	{

		setLabel(m_core_config.label);
		setWidth(m_core_config.width);
		setHeight(m_core_config.height);
		setDataSource(m_core_config.source);
		setEnabled(m_core_config.enabled);
	}

	mvAppItemConfig& mvAppItem::getCoreConfig()
	{
		return m_core_config;
	}

	void mvAppItem::registerWindowFocusing()
	{
		if (ImGui::IsWindowFocused())
		{

			// update mouse
			ImVec2 mousePos = ImGui::GetMousePos();
			float x = mousePos.x - ImGui::GetWindowPos().x;
			float y = mousePos.y - ImGui::GetWindowPos().y;
			mvInput::setMousePosition(x, y);

			if (mvApp::GetApp()->getItemRegistry().getActiveWindow() != m_core_config.name)
				mvEventBus::Publish(mvEVT_CATEGORY_ITEM, mvEVT_ACTIVE_WINDOW, { CreateEventArgument("WINDOW", m_core_config.name) });

		}
	}

	void mvAppItem::setCallback(mvCallable callback)
	{ 
#ifndef MV_CPP
		if (callback == Py_None)
		{
			m_core_config.callback = nullptr;
			return;
		}
#endif // !MV_CPP

		m_core_config.callback = callback;
	}

	void mvAppItem::setCallbackData(mvCallableData data)
	{
		if (data == Py_None)
		{
			m_core_config.callback_data = nullptr;
			return;
		}
		m_core_config.callback_data = data;
	}

	void mvAppItem::resetState()
	{
		m_state.reset();
		for (auto& item : m_children)
			item->resetState();

	}

	bool  mvAppItem::moveChildUp(const std::string& name)
	{
		bool found = false;
		int index = 0;

		// check children
		for (size_t i = 0; i<m_children.size(); i++)
		{

			if (m_children[i]->m_core_config.name == name)
			{
				found = true;
				index = (int)i;
				break;
			}

			if (m_children[i]->getDescription().container)
			{
				found = m_children[i]->moveChildUp(name);
				if (found)
					return true;
			}

		}

		if (found)
		{
			if (index > 0)
			{
				auto upperitem = m_children[index - 1];
				auto loweritem = m_children[index];

				m_children[index] = upperitem;
				m_children[index-1] = loweritem;
			}

			return true;
		}

		return false;

	}

	bool  mvAppItem::moveChildDown(const std::string& name)
	{
		bool found = false;
		size_t index = 0;

		// check children
		for (size_t i = 0; i < m_children.size(); i++)
		{

			if (m_children[i]->m_core_config.name == name)
			{
				found = true;
				index = i;
				break;
			}

			if (m_children[i]->getDescription().container)
			{
				found = m_children[i]->moveChildDown(name);
				if (found)
					return true;
			}

		}

		if (found)
		{
			if (index < m_children.size()-1)
			{
				auto upperitem = m_children[index];
				auto loweritem = m_children[index+1];

				m_children[index] = loweritem;
				m_children[index + 1] = upperitem;
			}

			return true;
		}

		return false;
	}

	bool mvAppItem::addRuntimeChild(const std::string& parent, const std::string& before, mvRef<mvAppItem> item)
	{
		if (before.empty() && parent.empty())
			return false;

		//this is the container, add item to beginning.
		if (before.empty())
		{
			if (parent == m_core_config.name)
			{
				m_children.push_back(item);
				item->m_parent = this;
				return true;
			}

			else
			{
				// check children
				for (auto& child : m_children)
				{
					if (child->getDescription().container)
					{
						// parent found
						if (child->addRuntimeChild(parent, before, item))
							return true;
					}
				}
			}
		}

		else
		{
			bool beforeFound = false;

			// check children
			for (auto& child : m_children)
			{

				if (child->m_core_config.name == before)
				{
					beforeFound = true;
					break;
				}

			}

			// after item is in this container
			if (beforeFound)
			{
				item->m_parent = this;

				std::vector<mvRef<mvAppItem>> oldchildren = m_children;
				m_children.clear();

				for (auto child : oldchildren)
				{
					if (child->m_core_config.name == before)
						m_children.push_back(item);
					m_children.push_back(child);

				}

				return true;
			}
		}

		// check children
		for (auto& child : m_children)
		{
			if (child->getDescription().container)
			{
				// parent found
				if (child->addRuntimeChild(parent, before, item))
					return true;
			}
		}

		return false;
	}

	bool mvAppItem::addChildAfter(const std::string& prev, mvRef<mvAppItem> item)
	{
		if (prev.empty())
			return false;


		bool prevFound = false;

		// check children
		for (auto& child : m_children)
		{

			if (child->m_core_config.name == prev)
			{
				item->m_parent = this;
				prevFound = true;
				break;
			}

		}

		// prev item is in this container
		if (prevFound)
		{
			//item->setParent(this);

			std::vector<mvRef<mvAppItem>> oldchildren = m_children;
			m_children.clear();

			for (auto& child : oldchildren)
			{
				m_children.push_back(child);
				if (child->m_core_config.name == prev)
					m_children.push_back(item);
			}

			return true;
		}
		

		// check children
		for (auto& child : m_children)
		{
			if (child->getDescription().container)
			{
				// parent found
				if (child->addChildAfter(prev, item))
					return true;
			}
		}

		return false;
	}

	bool mvAppItem::deleteChild(const std::string& name)
	{

		bool childfound = false;
		bool itemDeleted = false;

		for (auto& item : m_children)
		{
			if (item->m_core_config.name == name)
			{
				childfound = true;
				break;
			}

			if (item->getDescription().container)
			{
				itemDeleted = item->deleteChild(name);
				if (itemDeleted)
					break;
			}
		}

		if (childfound)
		{
			std::vector<mvRef<mvAppItem>> oldchildren = m_children;

			m_children.clear();

			for (auto& item : oldchildren)
			{
				if (item->m_core_config.name == name)
				{
					itemDeleted = true;
					continue;
				}

				m_children.push_back(item);
			}
		}

		return itemDeleted;
	}

	void mvAppItem::deleteChildren()
	{
		m_children.clear();
	}

	void mvAppItem::setLabel(const std::string& value)
	{
		m_core_config.label = value;
		m_label = value + "###" + m_core_config.name;
	}

	mvRef<mvAppItem> mvAppItem::stealChild(const std::string& name)
	{
		mvRef<mvAppItem> stolenChild = nullptr;

		bool childfound = false;

		for (auto& item : m_children)
		{
			if (item->m_core_config.name == name)
			{
				childfound = true;
				break;
			}

			if (item->getDescription().container)
			{
				stolenChild = item->stealChild(name);
				if (stolenChild)
					return stolenChild;
			}
		}

		if (childfound)
		{
			std::vector<mvRef<mvAppItem>> oldchildren = m_children;

			m_children.clear();

			for (auto& item : oldchildren)
			{
				if (item->m_core_config.name == name)
				{
					stolenChild = item;
					continue;
				}

				m_children.push_back(item);
			}
		}

		return stolenChild;
	}

	mvRef<mvAppItem> mvAppItem::getChild(const std::string& name)
	{
		for (auto& item : m_children)
		{
			if (item->m_core_config.name == name)
				return item;

			if (item->getDescription().container)
			{
				auto child = item->getChild(name);
				if (child)
					return child;
			}
		}

		return nullptr;
	}

	mvAppItem::~mvAppItem()
	{
		deleteChildren();

#ifndef MV_CPP		 
		mvGlobalIntepreterLock gil;
		if (m_core_config.callback)
			Py_DECREF(m_core_config.callback);
		if (m_core_config.callback_data)
			Py_DECREF(m_core_config.callback_data);
#endif
	}

	mvCallable mvAppItem::getCallback(bool ignore_enabled)
	{
		if (m_core_config.enabled)
			return m_core_config.callback;
#ifdef MV_CPP	
		return m_core_config.callback;
#else
		return ignore_enabled ? m_core_config.callback : nullptr;
#endif
		
	}

#ifndef MV_CPP

	void mvAppItem::checkConfigDict(PyObject* dict)
	{
#ifndef MV_CPP	
		if (dict == nullptr)
			return;

		auto configKeys = ToStringVect(PyDict_Keys(dict));
		auto parserKeywords = (mvApp::GetApp()->getParsers())[getParserCommand()].getKeywords();
		if (parserKeywords.empty())
		{
			ThrowPythonException("\"" + m_core_config.name + "\" could not find a parser that matched \"" + getParserCommand() + "\".");
			return;
		}
		for (const auto& key : configKeys)
		{
			size_t i = 0;
			while (i < parserKeywords.size() - 1)
			{
				if (key == parserKeywords[i])
				{
					break;
				}
				i++;
			}
			if (i == parserKeywords.size() - 1)
			{
				ThrowPythonException("\"" + key + "\" configuration does not exist in \"" + m_core_config.name + "\".");
			}
		}
#endif
	}

	void mvAppItem::setConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

#ifndef MV_CPP	
		if (PyObject* item = PyDict_GetItemString(dict, "name")) m_core_config.name = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "label")) setLabel(ToString(item));
		if (PyObject* item = PyDict_GetItemString(dict, "width")) setWidth(ToInt(item));
		if (PyObject* item = PyDict_GetItemString(dict, "height")) setHeight(ToInt(item));
		if (PyObject* item = PyDict_GetItemString(dict, "show")) m_core_config.show = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "source")) setDataSource(ToString(item));
		if (PyObject* item = PyDict_GetItemString(dict, "enabled")) setEnabled(ToBool(item));
#endif
	}

	void mvAppItem::getConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;
#ifndef MV_CPP	
		PyDict_SetItemString(dict, "name", ToPyString(m_core_config.name));
		PyDict_SetItemString(dict, "label", ToPyString(m_core_config.label));
		PyDict_SetItemString(dict, "source", ToPyString(m_core_config.source));
		PyDict_SetItemString(dict, "show", ToPyBool(m_core_config.show));
		PyDict_SetItemString(dict, "enabled", ToPyBool(m_core_config.enabled));
		PyDict_SetItemString(dict, "width", ToPyInt(m_core_config.width));
		PyDict_SetItemString(dict, "height", ToPyInt(m_core_config.height));
#endif
	}

	void AddItemCommands(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "get_item_callback_data", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns an item' callback data", "Any", "Widget Commands") });

		parsers->insert({ "set_item_callback_data", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
		}, "Sets an item's callback data if applicable.", "None", "Widget Commands") });

		parsers->insert({ "move_item", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::String, "parent", "", "''"},
			{mvPythonDataType::String, "before", "", "''"}
		}, "Moves an existing item.", "None", "Widget Commands") });

		parsers->insert({ "get_item_type", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns an item's type", "str", "Widget Commands") });

		parsers->insert({ "get_item_configuration", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns an items configuration", "dict", "Widget Commands") });

		parsers->insert({ "configure_item", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::Kwargs, "**Kwargs"},
		}, "Configures an item", "None", "Widget Commands") });

		parsers->insert({ "get_windows", mvPythonParser({
		}, "Returns a list of windows.", "List[str]", "Widget Commands") });

		parsers->insert({ "get_all_items", mvPythonParser({
		}, "Returns a list of all items.", "List[str]", "Widget Commands") });

		parsers->insert({ "get_item_children", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns a list of an item's children.", "List[str]", "Widget Commands") });

		parsers->insert({ "set_item_label", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::String, "label"},
		}, "Sets an item's label.", "None", "Widget Commands") });

		parsers->insert({ "get_item_label", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Gets an item's label.", "str", "Widget Commands") });

		parsers->insert({ "delete_item", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Bool, "children_only", "delete children only", "False"}
		}, "Deletes an item if it exists.", "None", "Widget Commands") });

		parsers->insert({ "does_item_exist", mvPythonParser({
		{mvPythonDataType::String, "item"},
		}, "Checks if item exists.", "bool", "Widget Commands") });

		parsers->insert({ "move_item_up", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Moves item up if possible and if it exists.", "None", "Widget Commands") });

		parsers->insert({ "move_item_down", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Moves item down if possible and if it exists.", "None", "Widget Commands") });

		parsers->insert({ "get_item_callback", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns an item' callback", "Callable", "Widget Commands") });

		parsers->insert({ "get_item_height", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns an item's height.", "float", "Widget Commands") });

		parsers->insert({ "get_item_width", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns an item's width.", "float", "Widget Commands") });

		parsers->insert({ "get_item_parent", mvPythonParser({
		{mvPythonDataType::String, "item"}
		}, "Returns an item's parent.", "str", "Widget Commands") });

		parsers->insert({ "get_item_tip", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns an item's tip.", "str", "Widget Commands") });

		parsers->insert({ "is_item_hovered", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item is hovered.", "bool", "Widget Commands") });

		parsers->insert({ "is_item_shown", mvPythonParser({
		{mvPythonDataType::String, "item"},
		}, "Checks if an item is shown.", "bool", "Widget Commands") });

		parsers->insert({ "is_item_active", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item is active.", "bool", "Widget Commands") });

		parsers->insert({ "is_item_focused", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item is focused.", "bool", "Widget Commands") });

		parsers->insert({ "is_item_clicked", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item is clicked.", "bool", "Widget Commands") });

		parsers->insert({ "is_item_container", mvPythonParser({
		{mvPythonDataType::String, "item"},
		}, "Checks if an item is container.", "bool", "Widget Commands") });

		parsers->insert({ "is_item_visible", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item is visible on screen.", "bool", "Widget Commands") });

		parsers->insert({ "is_item_edited", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item has been edited.", "bool", "Widget Commands") });

		parsers->insert({ "is_item_activated", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item has been activated.", "bool", "Widget Commands") });

		parsers->insert({ "is_item_deactivated", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item has been deactivated.", "bool", "Widget Commands") });

		parsers->insert({ "is_item_deactivated_after_edit", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item has been deactivated after editing.", "bool", "Widget Commands") });

		parsers->insert({ "is_item_toggled_open", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Checks if an item is toggled.", "bool", "Widget Commands") });

		parsers->insert({ "get_item_rect_min", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Returns an item's minimum allowable size. [width, height]", "[float, float]", "Widget Commands") });

		parsers->insert({ "get_item_rect_max", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Returns an item's maximum allowable size. [width, height]", "[float, float]", "Widget Commands") });

		parsers->insert({ "get_item_rect_size", mvPythonParser({
			{mvPythonDataType::String, "item"},
		}, "Returns an item's current size. [width, height]", "[float, float]", "Widget Commands") });

		parsers->insert({ "get_value", mvPythonParser({
			{mvPythonDataType::String, "name"}
		}, "Returns an item's value or None if there is none.", "Any", "Widget Commands") });

		parsers->insert({ "set_value", mvPythonParser({
			{mvPythonDataType::String, "name"},
			{mvPythonDataType::Object, "value"}
		}, "Sets an item's value if applicable.", "bool", "Widget Commands") });

		parsers->insert({ "show_item", mvPythonParser({
			{mvPythonDataType::String, "name"}
		}, "Shows an item if it was hidden.", "None", "Widget Commands") });

		parsers->insert({ "set_item_callback", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::Callable, "callback", "Registers a callback"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
		}, "Sets an item's callback if applicable.", "None", "Widget Commands") });

		parsers->insert({ "set_item_tip", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::String, "tip"}
		}, "Sets a simple tooltip for an item.", "None", "Widget Commands") });

		parsers->insert({ "set_item_width", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::Integer, "width"}
		}, "Sets an item's width.", "None", "Widget Commands") });

		parsers->insert({ "set_item_height", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::Integer, "height"}
		}, "Sets an item's height if applicable.", "None", "Widget Commands") });
	}

	PyObject* move_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* item;
		const char* parent = "";
		const char* before = "";

		if (!(mvApp::GetApp()->getParsers())["move_item"].parse(args, kwargs, __FUNCTION__,
			&item, &parent, &before))
			return GetPyNone();

		mvApp::GetApp()->getCallbackRegistry().submit([item, parent, before]()
			{mvApp::GetApp()->getItemRegistry().moveItem(item, parent, before); });

		return GetPyNone();
	}

	PyObject* get_item_configuration(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["get_item_configuration"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();


		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		PyObject* pdict = PyDict_New();

		if (std::string(item) == "logger##standard")
			mvAppLog::GetConfigDict(pdict);

		if (appitem)
		{
			appitem->getConfigDict(pdict);
			appitem->getExtraConfigDict(pdict);
		}
		else
			ThrowPythonException(item + std::string(" item was not found"));

		return pdict;
	}

	PyObject* configure_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		std::string item = ToString(PyTuple_GetItem(args, 0));

		if (std::string(item) == "logger##standard")
		{
			mvAppLog::SetConfigDict(kwargs);
			return GetPyNone();
		}

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
		{
			appitem->checkConfigDict(kwargs);
			appitem->setConfigDict(kwargs);
			appitem->setExtraConfigDict(kwargs);
		}
		else
			ThrowPythonException(item + std::string(" item was not found"));

		return GetPyNone();
	}

	PyObject* delete_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* item;
		int childrenOnly = false;

		if (!(mvApp::GetApp()->getParsers())["delete_item"].parse(args, kwargs, __FUNCTION__, &item, &childrenOnly))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		mvApp::GetApp()->getItemRegistry().deleteItem(item, childrenOnly);

		return GetPyNone();

	}

	PyObject* does_item_exist(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* item;

		if (!(mvApp::GetApp()->getParsers())["does_item_exist"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		if (mvApp::GetApp()->getItemRegistry().getItem(item))
			return ToPyBool(true);
		return ToPyBool(false);
	}

	PyObject* move_item_up(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* item;

		if (!(mvApp::GetApp()->getParsers())["move_item_up"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		mvApp::GetApp()->getCallbackRegistry().submit([item]()
			{mvApp::GetApp()->getItemRegistry().moveItemUp(item); });

		return GetPyNone();

	}

	PyObject* move_item_down(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* item;

		if (!(mvApp::GetApp()->getParsers())["move_item_down"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		mvApp::GetApp()->getCallbackRegistry().submit([item]()
			{mvApp::GetApp()->getItemRegistry().moveItemDown(item); });

		return GetPyNone();
	}

	PyObject* get_item_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["get_item_callback"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
		{
			PyObject* callback = appitem->getCallback();
			if (callback)
			{
				Py_XINCREF(callback);
				return callback;
			}

		}

		return GetPyNone();
	}

	PyObject* get_item_children(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["get_item_children"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		auto children = mvApp::GetApp()->getItemRegistry().getItemChildren(item);

		if (!children.empty())
			return ToPyList(children);

		return GetPyNone();
	}

	PyObject* get_all_items(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		return ToPyList(mvApp::GetApp()->getItemRegistry().getAllItems());
	}

	PyObject* get_windows(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		return ToPyList(mvApp::GetApp()->getItemRegistry().getWindows());
	}

	PyObject* get_item_parent(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["get_item_parent"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		std::string parent = mvApp::GetApp()->getItemRegistry().getItemParentName(item);

		if (!parent.empty())
			return ToPyString(parent);

		return GetPyNone();
	}

	PyObject* is_item_hovered(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["is_item_hovered"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (appitem)
			return ToPyBool(appitem->getState().isItemHovered());
		return ToPyBool(false);
	}

	PyObject* is_item_shown(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["is_item_shown"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (appitem)
			return ToPyBool(appitem->isShown());
		return ToPyBool(false);
	}

	PyObject* is_item_active(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["is_item_active"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (appitem)
			return ToPyBool(appitem->getState().isItemActive());
		return ToPyBool(false);
	}

	PyObject* is_item_focused(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["is_item_focused"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (appitem)
			return ToPyBool(appitem->getState().isItemFocused());
		return ToPyBool(false);
	}

	PyObject* is_item_clicked(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["is_item_clicked"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (appitem)
			return ToPyBool(appitem->getState().isItemClicked());
		return ToPyBool(false);
	}

	PyObject* is_item_container(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["is_item_container"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (appitem)
			return ToPyBool(appitem->getDescription().container);
		return ToPyBool(false);

	}

	PyObject* is_item_visible(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["is_item_visible"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (appitem)
			return ToPyBool(appitem->getState().isItemVisible());
		return ToPyBool(false);
	}

	PyObject* is_item_edited(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["is_item_edited"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (appitem)
			return ToPyBool(appitem->getState().isItemEdited());
		return ToPyBool(false);
	}

	PyObject* is_item_activated(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["is_item_activated"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (appitem)
			return ToPyBool(appitem->getState().isItemActivated());
		return ToPyBool(false);
	}

	PyObject* is_item_deactivated(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["is_item_deactivated"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (appitem)
			return ToPyBool(appitem->getState().isItemDeactivated());
		return ToPyBool(false);
	}

	PyObject* is_item_deactivated_after_edit(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["is_item_deactivated_after_edit"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (appitem)
			return ToPyBool(appitem->getState().isItemDeactivatedAfterEdit());
		return ToPyBool(false);
	}

	PyObject* is_item_toggled_open(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["is_item_toggled_open"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
			return ToPyBool(appitem->getState().isItemToogledOpen());


		return ToPyBool(false);
	}

	PyObject* get_item_rect_min(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["get_item_rect_min"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (appitem)
		{
			mvVec2 value = appitem->getState().getItemRectMin();
			return ToPyPair(value.x, value.y);
		}
		return ToPyPair(0.0f, 0.0f);
	}

	PyObject* get_item_rect_max(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["get_item_rect_max"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();


		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (appitem)
		{
			mvVec2 value = appitem->getState().getItemRectMax();
			return ToPyPair(value.x, value.y);
		}
		return ToPyPair(0.0f, 0.0f);
	}

	PyObject* get_item_rect_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["get_item_rect_size"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
		{
			mvVec2 value = appitem->getState().getItemRectSize();
			return ToPyPair(value.x, value.y);
		}
		return ToPyPair(0.0f, 0.0f);
	}

	PyObject* get_value(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if (!(mvApp::GetApp()->getParsers())["get_value"].parse(args, kwargs, __FUNCTION__, &name))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		auto item = mvApp::GetApp()->getItemRegistry().getItem(name);
		if (item)
			return item->getPyValue();

		return GetPyNone();

	}

	PyObject* set_value(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* value;

		if (!(mvApp::GetApp()->getParsers())["set_value"].parse(args, kwargs, __FUNCTION__, &name, &value))
			return GetPyNone();

		if (value)
			Py_XINCREF(value);

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());

		auto item = mvApp::GetApp()->getItemRegistry().getItem(name);
		if (item)
			item->setPyValue(value);

		Py_XDECREF(value);

		return GetPyNone();
	}

	PyObject* set_item_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;
		PyObject* callback_data = nullptr;
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["set_item_callback"].parse(args, kwargs, __FUNCTION__, &item, &callback, &callback_data))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
		{
			if (appitem->getCallback() != callback)
				Py_XINCREF(callback);
			appitem->setCallback(callback);

			if (callback_data)
			{
				if (appitem->getCallbackData() != callback_data)
					Py_XINCREF(callback_data);
				appitem->setCallbackData(callback_data);
			}
		}

		return GetPyNone();
	}

	PyObject* get_item_type(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if (!(mvApp::GetApp()->getParsers())["get_item_type"].parse(args, kwargs, __FUNCTION__, &name))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(name);

		return ToPyString(appitem->getStringType());
	}

	PyObject* get_item_callback_data(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["get_item_callback_data"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
		{
			PyObject* callback = appitem->getCallbackData();
			if (callback)
			{
				Py_XINCREF(callback);
				return callback;
			}

		}

		return GetPyNone();
	}

	PyObject* set_item_callback_data(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* data;
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["set_item_callback_data"].parse(args, kwargs, __FUNCTION__, &item, &data))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
		{
			if (appitem->getCallbackData() != data)
				Py_XINCREF(data);
			appitem->setCallbackData(data);
		}

		return GetPyNone();
	}

#endif // !MV_CPP
}
