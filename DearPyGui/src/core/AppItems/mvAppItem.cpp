#include "mvAppItem.h"
#include "mvApp.h"
#include "mvInput.h"
#include "mvItemRegistry.h"
#include "mvCore.h"
#include "mvAppItemCommons.h"

namespace Marvel{

	void mvAppItem::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		parsers->insert({ "get_item_callback_data", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns an item' callback data", "Any", "Widget Commands") });

		parsers->insert({ "set_item_callback_data", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
		}, "Sets an item's callback data if applicable.", "None", "Widget Commands") });

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

		parsers->insert({ "get_item_children1", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns a list of an item's children.", "List[str]", "Widget Commands") });

		parsers->insert({ "get_item_callback", mvPythonParser({
			{mvPythonDataType::String, "item"}
		}, "Returns an item' callback", "Callable", "Widget Commands") });

		parsers->insert({ "get_item_parent", mvPythonParser({
		{mvPythonDataType::String, "item"}
		}, "Returns an item's parent.", "str", "Widget Commands") });

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

		parsers->insert({ "set_item_callback", mvPythonParser({
			{mvPythonDataType::String, "item"},
			{mvPythonDataType::Callable, "callback", "Registers a callback"},
			{mvPythonDataType::KeywordOnly},
			{mvPythonDataType::Object, "callback_data", "Callback data", "None"},
		}, "Sets an item's callback if applicable.", "None", "Widget Commands") });

	}

	bool mvAppItem::DoesItemHaveFlag(mvAppItem* item, int flag)
	{
		return item->getDescFlags() & flag;
	}

	mvAppItem::mvAppItem(const std::string& name)
	{
		m_name = name;
		m_label = name + " ###" + name;
		m_label = name;
		m_state.setParent(this);
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

			if (mvApp::GetApp()->getItemRegistry().getActiveWindow() != m_name)
				mvEventBus::Publish(mvEVT_CATEGORY_ITEM, mvEVT_ACTIVE_WINDOW, { CreateEventArgument("WINDOW", m_name) });

		}
	}

	void mvAppItem::setCallback(mvCallable callback)
	{ 

		if (callback == Py_None)
		{
			m_callback = nullptr;
			return;
		}

		m_callback = callback;
	}

	void mvAppItem::setCallbackData(mvCallableData data)
	{
		if (data == Py_None)
		{
			m_callback_data = nullptr;
			return;
		}
		m_callback_data = data;
	}

	void mvAppItem::resetState()
	{
		m_state.reset();
		for (auto& item : m_children0)
			item->resetState();
		for (auto& item : m_children1)
			item->resetState();

	}

	bool  mvAppItem::moveChildUp(const std::string& name)
	{
		bool found = false;
		int index = 0;

		auto operation = [&](std::vector<mvRef<mvAppItem>>& children)
		{
			// check children
			for (size_t i = 0; i < children.size(); i++)
			{

				if (children[i]->m_name == name)
				{
					found = true;
					index = (int)i;
					break;
				}

				if (mvAppItem::DoesItemHaveFlag(children[i].get(), MV_ITEM_DESC_CONTAINER))
				{
					found = children[i]->moveChildUp(name);
					if (found)
						return true;
				}

			}

			if (found)
			{
				if (index > 0)
				{
					auto upperitem = children[index - 1];
					auto loweritem = children[index];

					children[index] = upperitem;
					children[index - 1] = loweritem;
				}

				return true;
			}

			return false;
		};

		if (operation(m_children0))
			return true;
		return operation(m_children1);


	}

	bool  mvAppItem::moveChildDown(const std::string& name)
	{
		bool found = false;
		size_t index = 0;

		auto operation = [&](std::vector<mvRef<mvAppItem>>& children)
		{
			// check children
			for (size_t i = 0; i < children.size(); i++)
			{

				if (children[i]->m_name == name)
				{
					found = true;
					index = i;
					break;
				}

				if (mvAppItem::DoesItemHaveFlag(children[i].get(), MV_ITEM_DESC_CONTAINER))
				{
					found = children[i]->moveChildDown(name);
					if (found)
						return true;
				}

			}

			if (found)
			{
				if (index < children.size() - 1)
				{
					auto upperitem = children[index];
					auto loweritem = children[index + 1];

					children[index] = loweritem;
					children[index + 1] = upperitem;
				}

				return true;
			}

			return false;
		};

		if (operation(m_children0))
			return true;
		return operation(m_children1);

	}

	bool mvAppItem::addRuntimeChild(const std::string& parent, const std::string& before, mvRef<mvAppItem> item)
	{
		if (before.empty() && parent.empty())
			return false;


		auto operation = [&](std::vector<mvRef<mvAppItem>>& children)
		{
			//this is the container, add item to beginning.
			if (before.empty())
			{
				if (parent == m_name)
				{
					children.push_back(item);
					item->m_parentPtr = this;
					return true;
				}

				else
				{
					// check children
					for (auto& child : children)
					{
						if (mvAppItem::DoesItemHaveFlag(child.get(), MV_ITEM_DESC_CONTAINER))
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
				for (auto& child : children)
				{

					if (child->m_name == before)
					{
						beforeFound = true;
						break;
					}

				}

				// after item is in this container
				if (beforeFound)
				{
					item->m_parentPtr = this;

					std::vector<mvRef<mvAppItem>> oldchildren = children;
					children.clear();

					for (auto child : oldchildren)
					{
						if (child->m_name == before)
							children.push_back(item);
						children.push_back(child);

					}

					return true;
				}
			}

			// check children
			for (auto& child : children)
			{
				if (mvAppItem::DoesItemHaveFlag(child.get(), MV_ITEM_DESC_CONTAINER))
				{
					// parent found
					if (child->addRuntimeChild(parent, before, item))
						return true;
				}
			}

			return false;
		};

		if (operation(m_children0))
			return true;
		return operation(m_children1);
	}

	bool mvAppItem::addItem(mvRef<mvAppItem> item)
	{
		if (item->getTarget() == 0)
			m_children0.push_back(item);
		else
			m_children1.push_back(item);

		return true;
	}

	bool mvAppItem::addChildAfter(const std::string& prev, mvRef<mvAppItem> item)
	{
		if (prev.empty())
			return false;


		auto operation = [&](std::vector<mvRef<mvAppItem>>& children)
		{
			bool prevFound = false;

			// check children
			for (auto& child : children)
			{

				if (child->m_name == prev)
				{
					item->m_parentPtr = this;
					prevFound = true;
					break;
				}

			}

			// prev item is in this container
			if (prevFound)
			{
				//item->setParent(this);

				std::vector<mvRef<mvAppItem>> oldchildren = children;
				children.clear();

				for (auto& child : oldchildren)
				{
					children.push_back(child);
					if (child->m_name == prev)
						children.push_back(item);
				}

				return true;
			}


			// check children
			for (auto& child : children)
			{
				if (DoesItemHaveFlag(child.get(), MV_ITEM_DESC_CONTAINER))
				{
					// parent found
					if (child->addChildAfter(prev, item))
						return true;
				}
			}

			return false;
		};

		if (operation(m_children0))
			return true;
		return operation(m_children1);
	}

	bool mvAppItem::deleteChild(const std::string& name)
	{

		auto operation = [&](std::vector<mvRef<mvAppItem>>& children)
		{
			bool childfound = false;
			bool itemDeleted = false;

			for (auto& item : children)
			{
				if (item->m_name == name)
				{
					childfound = true;
					break;
				}

				if (DoesItemHaveFlag(item.get(), MV_ITEM_DESC_CONTAINER))
				{
					itemDeleted = item->deleteChild(name);
					if (itemDeleted)
						break;
				}
			}

			if (childfound)
			{
				std::vector<mvRef<mvAppItem>> oldchildren = children;

				children.clear();

				for (auto& item : oldchildren)
				{
					if (item->m_name == name)
					{
						itemDeleted = true;
						continue;
					}

					children.push_back(item);
				}
			}

			return itemDeleted;
		};

		if (operation(m_children0))
			return true;
		return operation(m_children1);
	}

	void mvAppItem::deleteChildren()
	{
		m_children0.clear();
		m_children1.clear();
	}

	void mvAppItem::setLabel(const std::string& value)
	{
		m_specificedlabel = value;
		m_label = value + " ###" + m_name;
	}

	mvRef<mvAppItem> mvAppItem::stealChild(const std::string& name)
	{
		mvRef<mvAppItem> stolenChild = nullptr;

		auto operation = [&](std::vector<mvRef<mvAppItem>>& children)
		{
			bool childfound = false;

			for (auto& item : children)
			{
				if (item->m_name == name)
				{
					childfound = true;
					break;
				}

				if (DoesItemHaveFlag(item.get(), MV_ITEM_DESC_CONTAINER))
				{
					stolenChild = item->stealChild(name);
					if (stolenChild)
						return stolenChild;
				}
			}

			if (childfound)
			{
				std::vector<mvRef<mvAppItem>> oldchildren = children;

				children.clear();

				for (auto& item : oldchildren)
				{
					if (item->m_name == name)
					{
						stolenChild = item;
						continue;
					}

					children.push_back(item);
				}
			}


		};


		if (operation(m_children0))
			return stolenChild;
		return operation(m_children1);

	}

	mvRef<mvAppItem> mvAppItem::getChild(const std::string& name)
	{
		for (auto& item : m_children0)
		{
			if (item->m_name == name)
				return item;

			if (DoesItemHaveFlag(item.get(), MV_ITEM_DESC_CONTAINER))
			{
				auto child = item->getChild(name);
				if (child)
					return child;
			}
		}

		for (auto& item : m_children1)
		{
			if (item->m_name == name)
				return item;

			if (DoesItemHaveFlag(item.get(), MV_ITEM_DESC_CONTAINER))
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
 
		mvGlobalIntepreterLock gil;
		if (m_callback)
			Py_DECREF(m_callback);
		if (m_callback_data)
			Py_DECREF(m_callback_data);
	}

	mvCallable mvAppItem::getCallback(bool ignore_enabled)
	{
		if (m_enabled)
			return m_callback;

		return ignore_enabled ? m_callback : nullptr;
		
	}

	bool mvAppItem::isThemeColorCacheValid() const
	{
		return !m_theme_color_dirty;
	}

	bool mvAppItem::isThemeStyleCacheValid() const
	{
		return !m_theme_style_dirty;
	}

	bool mvAppItem::isThemeFontCacheValid() const
	{
		return !m_theme_font_dirty;
	}

	void mvAppItem::inValidateThemeColorCache()
	{
		m_theme_color_dirty = true;
		m_cached_colors.clear();

		for (auto& child : m_children1)
			child->inValidateThemeColorCache();
	}

	void mvAppItem::inValidateThemeStyleCache()
	{
		m_theme_style_dirty = true;
		m_cached_styles.clear();
		m_cached_styles2.clear();

		for (auto& child : m_children1)
			child->inValidateThemeStyleCache();
	}

	void mvAppItem::inValidateThemeFontCache()
	{
		m_theme_font_dirty = true;
		m_cached_font = nullptr;

		for (auto& child : m_children1)
			child->inValidateThemeFontCache();
	}

	void mvAppItem::setThemeColorCacheValid()
	{
		m_theme_color_dirty = false;
	}

	void mvAppItem::setThemeStyleCacheValid()
	{
		m_theme_style_dirty = false;
	}

	void mvAppItem::setThemeFontCacheValid()
	{
		m_theme_font_dirty = false;
	}

	ImFont* mvAppItem::getCachedFont()
	{
		return m_cached_font;
	}

	mvThemeColors& mvAppItem::getCachedThemeColors()
	{
		return m_cached_colors;
	}

	std::unordered_map<ImGuiStyleVar, float>& mvAppItem::getCachedThemeStyles()
	{
		return m_cached_styles;
	}

	std::unordered_map<ImGuiStyleVar, float>& mvAppItem::getCachedThemeStyles1()
	{
		return m_cached_styles1;
	}

	std::unordered_map<ImGuiStyleVar, float>& mvAppItem::getCachedThemeStyles2()
	{
		return m_cached_styles2;
	}

	void mvAppItem::checkConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		auto configKeys = ToStringVect(PyDict_Keys(dict));

		static std::string base_keyword1 = "name";
		static std::string base_keyword2 = "label";
		static std::string base_keyword3 = "source";
		static std::string base_keyword4 = "tip";
		static std::string base_keyword5 = "show";
		static std::string base_keyword6 = "enabled";
		static std::string base_keyword7 = "width";
		static std::string base_keyword8 = "height";

		std::string parserCommand;

		constexpr_for<1, (int)mvAppItemType::ItemTypeCount, 1>(
			[&](auto i) {
				using item_type = typename mvItemTypeMap<i>::type;
				mvAppItemType ait = mvItemTypeReverseMap<item_type>::type;
				if (getType() == ait)
				{
					parserCommand = item_type::s_command;
					return;
				}
			});

		const auto& parserKeywordsOrig = mvApp::GetApp()->getParsers()[parserCommand].getKeywords();
		std::vector<std::string> parserKeywords;
		parserKeywords.reserve(parserKeywordsOrig.size() + 8);
		for (int i = 0; i < parserKeywordsOrig.size(); i++)
			if(parserKeywordsOrig[i])
				parserKeywords.push_back(std::string(parserKeywordsOrig[i]));

		parserKeywords.push_back(base_keyword1);
		parserKeywords.push_back(base_keyword2);
		parserKeywords.push_back(base_keyword3);
		parserKeywords.push_back(base_keyword4);
		parserKeywords.push_back(base_keyword5);
		parserKeywords.push_back(base_keyword6);
		parserKeywords.push_back(base_keyword7);
		parserKeywords.push_back(base_keyword8);

		for (const auto& key : configKeys)
		{
			size_t i = 0;
			while (i < parserKeywords.size())
			{
				if (key == parserKeywords[i])
				{
					break;
				}
				i++;
			}
			if (i == parserKeywords.size())
			{
				ThrowPythonException("\"" + key + "\" configuration does not exist in \"" + m_name + "\".");
			}
		}
	}

	void mvAppItem::setConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyObject* item = PyDict_GetItemString(dict, "name")) m_name = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "label")) setLabel(ToString(item));
		if (PyObject* item = PyDict_GetItemString(dict, "width")) setWidth(ToInt(item));
		if (PyObject* item = PyDict_GetItemString(dict, "height")) setHeight(ToInt(item));
		if (PyObject* item = PyDict_GetItemString(dict, "show")) m_show = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "source")) setDataSource(ToString(item));
		if (PyObject* item = PyDict_GetItemString(dict, "enabled")) setEnabled(ToBool(item));
	}

	void mvAppItem::getConfigDict(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "name", ToPyString(m_name));
		PyDict_SetItemString(dict, "label", ToPyString(m_specificedlabel));
		PyDict_SetItemString(dict, "source", ToPyString(m_source));
		PyDict_SetItemString(dict, "show", ToPyBool(m_show));
		PyDict_SetItemString(dict, "enabled", ToPyBool(m_enabled));
		PyDict_SetItemString(dict, "width", ToPyInt(m_width));
		PyDict_SetItemString(dict, "height", ToPyInt(m_height));

	}

	PyObject* mvAppItem::get_item_configuration(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["get_item_configuration"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();


		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
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

	PyObject* mvAppItem::configure_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		std::string item = ToString(PyTuple_GetItem(args, 0));

		if (std::string(item) == "logger##standard")
		{
			mvAppLog::SetConfigDict(kwargs);
			return GetPyNone();
		}

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
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

	PyObject* mvAppItem::get_item_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["get_item_callback"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
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

	PyObject* mvAppItem::get_item_children(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["get_item_children1"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto children = mvApp::GetApp()->getItemRegistry().getItemChildren(item);

		if (!children.empty())
			return ToPyList(children);

		return GetPyNone();
	}

	PyObject* mvAppItem::get_item_parent(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["get_item_parent"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());

		std::string parent = mvApp::GetApp()->getItemRegistry().getItemParentName(item);

		if (!parent.empty())
			return ToPyString(parent);

		return GetPyNone();
	}

	PyObject* mvAppItem::is_item_hovered(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["is_item_hovered"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (appitem)
			return ToPyBool(appitem->getState().isItemHovered());
		return ToPyBool(false);
	}

	PyObject* mvAppItem::is_item_shown(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["is_item_shown"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (appitem)
			return ToPyBool(appitem->isShown());
		return ToPyBool(false);
	}

	PyObject* mvAppItem::is_item_active(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["is_item_active"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (appitem)
			return ToPyBool(appitem->getState().isItemActive());
		return ToPyBool(false);
	}

	PyObject* mvAppItem::is_item_focused(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["is_item_focused"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (appitem)
			return ToPyBool(appitem->getState().isItemFocused());
		return ToPyBool(false);
	}

	PyObject* mvAppItem::is_item_clicked(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["is_item_clicked"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (appitem)
			return ToPyBool(appitem->getState().isItemClicked());
		return ToPyBool(false);
	}

	PyObject* mvAppItem::is_item_container(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["is_item_container"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (appitem)
			return ToPyBool(DoesItemHaveFlag(appitem.get(), MV_ITEM_DESC_CONTAINER));
		return ToPyBool(false);

	}

	PyObject* mvAppItem::is_item_visible(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["is_item_visible"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (appitem)
			return ToPyBool(appitem->getState().isItemVisible());
		return ToPyBool(false);
	}

	PyObject* mvAppItem::is_item_edited(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["is_item_edited"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (appitem)
			return ToPyBool(appitem->getState().isItemEdited());
		return ToPyBool(false);
	}

	PyObject* mvAppItem::is_item_activated(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["is_item_activated"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (appitem)
			return ToPyBool(appitem->getState().isItemActivated());
		return ToPyBool(false);
	}

	PyObject* mvAppItem::is_item_deactivated(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["is_item_deactivated"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (appitem)
			return ToPyBool(appitem->getState().isItemDeactivated());
		return ToPyBool(false);
	}

	PyObject* mvAppItem::is_item_deactivated_after_edit(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["is_item_deactivated_after_edit"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (appitem)
			return ToPyBool(appitem->getState().isItemDeactivatedAfterEdit());
		return ToPyBool(false);
	}

	PyObject* mvAppItem::is_item_toggled_open(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["is_item_toggled_open"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
			return ToPyBool(appitem->getState().isItemToogledOpen());


		return ToPyBool(false);
	}

	PyObject* mvAppItem::get_item_rect_min(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["get_item_rect_min"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (appitem)
		{
			mvVec2 value = appitem->getState().getItemRectMin();
			return ToPyPair(value.x, value.y);
		}
		return ToPyPair(0.0f, 0.0f);
	}

	PyObject* mvAppItem::get_item_rect_max(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["get_item_rect_max"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();


		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (appitem)
		{
			mvVec2 value = appitem->getState().getItemRectMax();
			return ToPyPair(value.x, value.y);
		}
		return ToPyPair(0.0f, 0.0f);
	}

	PyObject* mvAppItem::get_item_rect_size(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["get_item_rect_size"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
		{
			mvVec2 value = appitem->getState().getItemRectSize();
			return ToPyPair(value.x, value.y);
		}
		return ToPyPair(0.0f, 0.0f);
	}

	PyObject* mvAppItem::get_value(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if (!(mvApp::GetApp()->getParsers())["get_value"].parse(args, kwargs, __FUNCTION__, &name))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto item = mvApp::GetApp()->getItemRegistry().getItem(name);
		if (item)
			return item->getPyValue();

		return GetPyNone();

	}

	PyObject* mvAppItem::set_value(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;
		PyObject* value;

		if (!(mvApp::GetApp()->getParsers())["set_value"].parse(args, kwargs, __FUNCTION__, &name, &value))
			return GetPyNone();

		if (value)
			Py_XINCREF(value);

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());

		auto item = mvApp::GetApp()->getItemRegistry().getItem(name);
		if (item)
			item->setPyValue(value);

		Py_XDECREF(value);

		return GetPyNone();
	}

	PyObject* mvAppItem::set_item_callback(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* callback;
		PyObject* callback_data = nullptr;
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["set_item_callback"].parse(args, kwargs, __FUNCTION__, &item, &callback, &callback_data))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
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

	PyObject* mvAppItem::get_item_type(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* name;

		if (!(mvApp::GetApp()->getParsers())["get_item_type"].parse(args, kwargs, __FUNCTION__, &name))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(name);

		std::string parserCommand;

		constexpr_for<1, (int)mvAppItemType::ItemTypeCount, 1>(
			[&](auto i) {
				using item_type = typename mvItemTypeMap<i>::type;
				mvAppItemType ait = mvItemTypeReverseMap<item_type>::type;
				if (appitem->getType() == ait)
				{
					parserCommand = item_type::s_internal_id;
					return;
				}
			});

		return ToPyString(parserCommand);
	}

	PyObject* mvAppItem::get_item_callback_data(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["get_item_callback_data"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
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

	PyObject* mvAppItem::set_item_callback_data(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* data;
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["set_item_callback_data"].parse(args, kwargs, __FUNCTION__, &item, &data))
			return GetPyNone();

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
		{
			if (appitem->getCallbackData() != data)
				Py_XINCREF(data);
			appitem->setCallbackData(data);
		}

		return GetPyNone();
	}

}
