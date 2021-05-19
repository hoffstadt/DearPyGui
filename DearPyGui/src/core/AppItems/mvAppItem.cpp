#include "mvAppItem.h"
#include "mvApp.h"
#include "mvInput.h"
#include "mvItemRegistry.h"
#include "mvCore.h"
#include "mvPythonExceptions.h"
#include "mvGlobalIntepreterLock.h"
#include "mvAppItemCommons.h"

namespace Marvel{

	void mvAppItem::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		{
			mvPythonParser parser(mvPyDataType::Dict);
			parser.addArg<mvPyDataType::String>("item");
			parser.finalize();
			parsers->insert({ "get_item_configuration", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.addArg<mvPyDataType::String>("item");
			parser.finalize();
			parsers->insert({ "focus_item", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::Dict);
			parser.addArg<mvPyDataType::String>("item");
			parser.finalize();
			parsers->insert({ "get_item_state", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.addArg<mvPyDataType::String>("item");
			parser.addKwargs();
			parser.finalize();
			parsers->insert({ "configure_item", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::Dict);
			parser.addArg<mvPyDataType::String>("item");
			parser.finalize();
			parsers->insert({ "get_item_info", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::Any);
			parser.addArg<mvPyDataType::String>("item");
			parser.finalize();
			parsers->insert({ "get_value", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::Any);
			parser.addArg<mvPyDataType::StringList>("items");
			parser.finalize();
			parsers->insert({ "get_values", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.addArg<mvPyDataType::String>("item");
			parser.addArg<mvPyDataType::Object>("value");
			parser.finalize();
			parsers->insert({ "set_value", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.addArg<mvPyDataType::String>("item");
			parser.addArg<mvPyDataType::Float>("x");
			parser.addArg<mvPyDataType::Float>("y");
			parser.finalize();
			parsers->insert({ "set_item_pos", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None);
			parser.addArg<mvPyDataType::String>("item");
			parser.finalize();
			parsers->insert({ "reset_pos", parser });
		}

	}

	std::vector<mvRef<mvAppItem>>& mvAppItem::getChildren(int slot) 
	{ 
		return m_children[slot];
	}

	void mvAppItem::setChildren(int slot, std::vector<mvRef<mvAppItem>> children)
	{
		m_children[slot] = children;
	}

	bool mvAppItem::DoesItemHaveFlag(mvAppItem* item, int flag)
	{
		return item->getDescFlags() & flag;
	}

	void mvAppItem::AddCommonArgs(mvPythonParser& parser, CommonParserArgs args)
	{

		if(args & MV_PARSER_ARG_ID)            parser.addArg<mvPyDataType::String>("id", mvArgType::KEYWORD_ARG);
		if(args & MV_PARSER_ARG_WIDTH)         parser.addArg<mvPyDataType::Integer>("width", mvArgType::KEYWORD_ARG, "0");
		if(args & MV_PARSER_ARG_HEIGHT)        parser.addArg<mvPyDataType::Integer>("height", mvArgType::KEYWORD_ARG, "0");
		if(args & MV_PARSER_ARG_INDENT)        parser.addArg<mvPyDataType::Integer>("indent", mvArgType::KEYWORD_ARG, "-1");
		if(args & MV_PARSER_ARG_PARENT)        parser.addArg<mvPyDataType::String>("parent", mvArgType::KEYWORD_ARG, "''", "Parent to add this item to. (runtime adding)");
		if(args & MV_PARSER_ARG_BEFORE)        parser.addArg<mvPyDataType::String>("before", mvArgType::KEYWORD_ARG, "''", "This item will be displayed before the specified item in the parent.");
		if(args & MV_PARSER_ARG_LABEL)         parser.addArg<mvPyDataType::String>("label", mvArgType::KEYWORD_ARG, "''", "Overrides 'name' as label");
		if(args & MV_PARSER_ARG_SOURCE)        parser.addArg<mvPyDataType::String>("source", mvArgType::KEYWORD_ARG, "''", "Overrides 'name' as value storage key");		
		if(args & MV_PARSER_ARG_CALLBACK)      parser.addArg<mvPyDataType::Callable>("callback", mvArgType::KEYWORD_ARG, "None", "Registers a callback");
		if(args & MV_PARSER_ARG_CALLBACK_DATA) parser.addArg<mvPyDataType::Object>("callback_data", mvArgType::KEYWORD_ARG, "None", "Callback data");
		if(args & MV_PARSER_ARG_SHOW)          parser.addArg<mvPyDataType::Bool>("show", mvArgType::KEYWORD_ARG, "True", "Attempt to render");
		if(args & MV_PARSER_ARG_ENABLED)       parser.addArg<mvPyDataType::Bool>("enabled", mvArgType::KEYWORD_ARG, "True");

	}

	mvAppItem::mvAppItem(const std::string& name)
	{
		m_name = name;
		m_label = name + " ###" + name;
		m_specificedlabel = name;
	}

	bool mvAppItem::preDraw()
	{
		if (!m_show)
			return false;

		if (m_focusNextFrame)
		{
			ImGui::SetKeyboardFocusHere();
			m_focusNextFrame = false;
		}

		m_previousCursorPos = ImGui::GetCursorPos();
		if (m_dirtyPos)
			ImGui::SetCursorPos(m_state.getItemPos());

		m_state.setPos({ ImGui::GetCursorPosX(), ImGui::GetCursorPosY() });

		// set item width
		if (m_width != 0)
			ImGui::SetNextItemWidth((float)m_width);

		if (m_indent > 0.0f)
			ImGui::Indent(m_indent);

		return true;
	}

	void mvAppItem::postDraw()
	{
		if (m_dirtyPos)
			ImGui::SetCursorPos(m_previousCursorPos);

		if(m_indent > 0.0f)
			ImGui::Unindent(m_indent);

		m_state.update();
	}

	mvAppItem* mvAppItem::getRoot() const
	{
		if (m_parentPtr)
		{
			mvAppItem* item = m_parentPtr;
			while (!DoesItemHaveFlag(item, MV_ITEM_DESC_ROOT))
				item = item->m_parentPtr;

			return item;
		}
		return nullptr;
	}

	void mvAppItem::setFont(const std::string& name, int size, ImFont* font) 
	{
		m_font = font; 
		m_fontName = name;
		m_fontSize = size;
	}

	void mvAppItem::setPos(const ImVec2& pos)
	{
		m_dirtyPos = true;
		m_state.setPos({ pos.x, pos.y });
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

	void mvAppItem::setCallback(PyObject* callback)
	{ 

		if (callback == Py_None)
		{
			m_callback = nullptr;
			return;
		}

		m_callback = callback;
	}

	void mvAppItem::setCallbackData(PyObject* data)
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
		for (auto& childset : m_children)
		{
			for (auto& child : childset)
				child->resetState();
		}
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

		for (auto& childset : m_children)
		{
			if (operation(childset))
				return true;
		}

		return false;


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

		for (auto& childset : m_children)
		{
			if (operation(childset))
				return true;
		}

		return false;

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
					onChildAdd(item);
					item->m_parentPtr = this;
					return true;
				}

				else
				{
					// check children
					for (auto& childslot : m_children)
					{
						for (auto& child : childslot)
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
			}

			else
			{
				bool beforeFound = false;

				// check children
				for (auto& childslot : m_children)
				{
					for (auto& child : childslot)
					{

						if (child->m_name == before)
						{
							beforeFound = true;
							break;
						}
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
						{
							children.push_back(item);
							onChildAdd(item);
						}
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

		return operation(m_children[item->getTarget()]);
	}

	bool mvAppItem::addItem(mvRef<mvAppItem> item)
	{

		m_children[item->getTarget()].push_back(item);
		onChildAdd(item);

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
			for (auto& childslot : m_children)
			{
				for (auto& child : childslot)
				{

					if (child->m_name == prev)
					{
						item->m_parentPtr = this;
						prevFound = true;
						break;
					}

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
					{
						children.push_back(item);
						onChildAdd(item);
					}
				}

				return true;
			}


			// check children
			for (auto& childslot : m_children)
			{
				for (auto& child : childslot)
				{
					if (DoesItemHaveFlag(child.get(), MV_ITEM_DESC_CONTAINER))
					{
						// parent found
						if (child->addChildAfter(prev, item))
							return true;
					}
				}
			}

			return false;
		};

		return operation(m_children[item->getTarget()]);
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
						onChildRemoved(item);
						continue;
					}

					children.push_back(item);
				}
			}

			return itemDeleted;
		};

		for (auto& childset : m_children)
		{
			if (operation(childset))
				return true;
		}

		return false;
	}

	void mvAppItem::deleteChildren()
	{
		for (auto& childset : m_children)
			childset.clear();
		onChildrenRemoved();
	}

	void mvAppItem::setLabel(const std::string& value)
	{
		m_specificedlabel = value;
		m_label = value + " ###" + m_name;
	}

	mvRef<mvAppItem> mvAppItem::stealChild(const std::string& name)
	{
		mvRef<mvAppItem> stolenChild = nullptr;

		for (auto& childset : m_children)
		{
			bool childfound = false;

			for (auto& item : childset)
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
				std::vector<mvRef<mvAppItem>> oldchildren = childset;

				childset.clear();

				for (auto& item : oldchildren)
				{
					if (item->m_name == name)
					{
						stolenChild = item;
						onChildRemoved(item);
						continue;
					}

					childset.push_back(item);
				}

				return stolenChild;
			}

			
			//return static_cast<mvRef<mvAppItem>>(CreateRef<mvButton>("Not possible"));
		}

		return stolenChild;
	}

	mvRef<mvAppItem> mvAppItem::getChild(const std::string& name)
	{
		for (auto& childset : m_children)
		{
			for (auto& item : childset)
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

	PyObject* mvAppItem::getCallback(bool ignore_enabled)
	{
		if (m_enabled)
			return m_callback;

		return ignore_enabled ? m_callback : nullptr;
		
	}

	bool mvAppItem::isThemeColorCacheValid() const
	{
		return !m_theme_color_dirty;
	}

	bool mvAppItem::isThemeDisabledColorCacheValid() const
	{
		return !m_theme_disabled_color_dirty;
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

		for (auto& child : m_children[1])
			child->inValidateThemeColorCache();
	}

	void mvAppItem::inValidateThemeDisabledColorCache()
	{
		m_theme_disabled_color_dirty = true;
		m_cached_disabled_colors.clear();

		for (auto& child : m_children[1])
			child->inValidateThemeDisabledColorCache();
	}

	void mvAppItem::inValidateThemeStyleCache()
	{
		m_theme_style_dirty = true;
		m_cached_styles.clear();
		m_cached_styles2.clear();

		for (auto& child : m_children[1])
			child->inValidateThemeStyleCache();
	}

	void mvAppItem::inValidateThemeFontCache()
	{
		m_theme_font_dirty = true;
		m_cachefont = nullptr;

		for (auto& childset : m_children)
		{
			for (auto& child : childset)
				child->inValidateThemeFontCache();
		}
	}

	void mvAppItem::setThemeColorCacheValid()
	{
		m_theme_color_dirty = false;
	}

	void mvAppItem::setThemeDisabledColorCacheValid()
	{
		m_theme_disabled_color_dirty = false;
	}

	void mvAppItem::setThemeStyleCacheValid()
	{
		m_theme_style_dirty = false;
	}

	void mvAppItem::setThemeFontCacheValid()
	{
		m_theme_font_dirty = false;
	}

	mvThemeColors& mvAppItem::getCachedThemeColors()
	{
		return m_cached_colors;
	}

	mvThemeColors& mvAppItem::getCachedThemeDisabledColors()
	{
		return m_cached_disabled_colors;
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

	void mvAppItem::checkArgs(PyObject* args, PyObject* kwargs)
	{
		mvAppItemType type = getType();
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

		mvApp::GetApp()->getParsers()[parserCommand].verifyArgumentCount(args);

	}

	void mvAppItem::handleKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		if (PyArg_ValidateKeywordArguments(dict) == 0)
		{
			assert(false);
			mvThrowPythonError(1000, "Dictionary keywords must be strings");
			return;
		}

		if (PyObject* item = PyDict_GetItemString(dict, "label"))
		{
			const std::string label = ToString(item);
			if(!label.empty())
				setLabel(label);
		}

		if (PyObject* item = PyDict_GetItemString(dict, "width")) setWidth(ToInt(item));
		if (PyObject* item = PyDict_GetItemString(dict, "height")) setHeight(ToInt(item));
		if (PyObject* item = PyDict_GetItemString(dict, "indent")) m_indent = (float)ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "show")) 
		{
			m_show = ToBool(item);
			if (m_show)
				show();
		}
		if (PyObject* item = PyDict_GetItemString(dict, "source")) setDataSource(ToString(item));
		if (PyObject* item = PyDict_GetItemString(dict, "enabled")) setEnabled(ToBool(item));
		if (PyObject* item = PyDict_GetItemString(dict, "default_value")) setPyValue(item);

		if (PyObject* item = PyDict_GetItemString(dict, "callback"))
		{
			if (m_callback)
				Py_XDECREF(m_callback);

			Py_XINCREF(item);
			setCallback(item);
		}

		if (PyObject* item = PyDict_GetItemString(dict, "callback_data"))
		{
			if (m_callback_data)
				Py_XDECREF(m_callback_data);
			
			Py_XINCREF(item);
			setCallbackData(item);
		}

		handleSpecificKeywordArgs(dict);
	}

	std::pair<std::string, std::string> mvAppItem::GetNameFromArgs(std::string& name, PyObject* args, PyObject* kwargs)
	{

		std::string parent;
		std::string before;

		if (kwargs)
		{
			if (PyObject* item = PyDict_GetItemString(kwargs, "parent")) parent = ToString(item);
			if (PyObject* item = PyDict_GetItemString(kwargs, "before")) before = ToString(item);
			if (PyObject* item = PyDict_GetItemString(kwargs, "id"))
			{
				auto id = ToString(item);
				if (!id.empty())
					name = id;
			};
		}

		return std::make_pair(parent, before);
	}

	void mvAppItem::getItemInfo(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		std::string parserCommand;

		constexpr_for<1, (int)mvAppItemType::ItemTypeCount, 1>(
			[&](auto i) {
				using item_type = typename mvItemTypeMap<i>::type;
				mvAppItemType ait = mvItemTypeReverseMap<item_type>::type;
				if (getType() == ait)
				{
					parserCommand = item_type::s_internal_id;
					return;
				}
			});

		auto children = mvApp::GetApp()->getItemRegistry().getItemChildren(m_name);
		if (children.empty())
			PyDict_SetItemString(dict, "children", GetPyNone());
		else
		{
			PyObject* pyChildren = PyDict_New();
			int i = 0;
			for (const auto& slot : children)
			{
				//PyDict_SetItemString(pyChildren, std::to_string(i).c_str(), ToPyList(slot));
				PyDict_SetItem(pyChildren, ToPyInt(i), ToPyList(slot));
				i++;
			}
			PyDict_SetItemString(dict, "children", pyChildren);
		}

		PyDict_SetItemString(dict, "type", ToPyString(parserCommand));

		if (m_parentPtr)
			PyDict_SetItemString(dict, "parent", ToPyString(m_parentPtr->getName()));
		else
			PyDict_SetItemString(dict, "parent", GetPyNone());

		if(getDescFlags() == MV_ITEM_DESC_CONTAINER)
			PyDict_SetItemString(dict, "container", ToPyBool(true));
		else
			PyDict_SetItemString(dict, "container", ToPyBool(false));

	}

	void mvAppItem::getConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		PyDict_SetItemString(dict, "label", ToPyString(m_specificedlabel));
		PyDict_SetItemString(dict, "source", ToPyString(m_source));
		PyDict_SetItemString(dict, "show", ToPyBool(m_show));
		PyDict_SetItemString(dict, "enabled", ToPyBool(m_enabled));
		PyDict_SetItemString(dict, "width", ToPyInt(m_width));
		PyDict_SetItemString(dict, "height", ToPyInt(m_height));
		PyDict_SetItemString(dict, "indent", ToPyInt(m_indent));

		if (m_callback)
		{
			Py_XINCREF(m_callback);
			PyDict_SetItemString(dict, "callback", m_callback);
		}
		else
			PyDict_SetItemString(dict, "callback", GetPyNone());

		if (m_callback_data)
		{
			Py_XINCREF(m_callback_data);
			PyDict_SetItemString(dict, "callback_data", m_callback_data);
		}
		else
			PyDict_SetItemString(dict, "callback_data", GetPyNone());
	}

	PyObject* mvAppItem::get_item_configuration(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["get_item_configuration"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();


		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		PyObject* pdict = PyDict_New();

		if (appitem)
		{
			appitem->getConfiguration(pdict);
			appitem->getSpecificConfiguration(pdict);
		}
		else
			mvThrowPythonError(1000, item + std::string(" item was not found"));

		return pdict;
	}

	PyObject* mvAppItem::focus_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["focus_item"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();


		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
		{
			appitem->m_focusNextFrame = true;
			if (auto parent = appitem->getRoot())
				parent->m_focusNextFrame = true;
		}
		else
			mvThrowPythonError(1000, item + std::string(" item was not found"));

		return GetPyNone();
	}

	PyObject* mvAppItem::set_item_pos(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;
		float x, y;

		if (!(mvApp::GetApp()->getParsers())["set_item_pos"].parse(args, kwargs, __FUNCTION__, 
			&item, &x, &y))
			return GetPyNone();


		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
			appitem->setPos({ x, y });
		else
			mvThrowPythonError(1000, item + std::string(" item was not found"));

		return GetPyNone();
	}

	PyObject* mvAppItem::reset_pos(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["reset_pos"].parse(args, kwargs, __FUNCTION__,
			&item))
			return GetPyNone();


		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
			appitem->m_dirtyPos = false;
		else
			mvThrowPythonError(1000, item + std::string(" item was not found"));

		return GetPyNone();
	}

	PyObject* mvAppItem::get_item_state(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["get_item_state"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();


		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		PyObject* pdict = PyDict_New();

		if (appitem)
			appitem->getState().getState(pdict);

		else
			mvThrowPythonError(1000, item + std::string(" item was not found"));

		return pdict;
	}

	PyObject* mvAppItem::get_item_info(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		const char* item;

		if (!(mvApp::GetApp()->getParsers())["get_item_info"].parse(args, kwargs, __FUNCTION__, &item))
			return GetPyNone();


		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		PyObject* pdict = PyDict_New();

		if (appitem)
			appitem->getItemInfo(pdict);

		else
			mvThrowPythonError(1000, item + std::string(" item was not found"));

		return pdict;
	}

	PyObject* mvAppItem::configure_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		std::string item = ToString(PyTuple_GetItem(args, 0));

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
		{
			appitem->checkArgs(nullptr, kwargs);
			appitem->handleKeywordArgs(kwargs);
		}
		else
			mvThrowPythonError(1000, item + std::string(" item was not found"));

		return GetPyNone();
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

	PyObject* mvAppItem::get_values(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* items;

		if (!(mvApp::GetApp()->getParsers())["get_values"].parse(args, kwargs, __FUNCTION__, &items))
			return GetPyNone();

		auto aitems = ToStringVect(items);
		PyObject* pyvalues = PyList_New(aitems.size());

		std::lock_guard<std::mutex> lk(mvApp::GetApp()->getMutex());
		for (int i = 0; i<aitems.size(); i++)
		{
			auto item = mvApp::GetApp()->getItemRegistry().getItem(aitems[i]);
			if (item)
				PyList_SetItem(pyvalues, i, item->getPyValue());
			else
				PyList_SetItem(pyvalues, i, GetPyNone());
		}

		return pyvalues;
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

}
