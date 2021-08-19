#include "mvAppItem.h"
#include "mvApp.h"
#include "mvInput.h"
#include "mvItemRegistry.h"
#include "mvCore.h"
#include "mvPythonExceptions.h"
#include "mvGlobalIntepreterLock.h"
#include "mvAppItemCommons.h"
#include "mvLog.h"
#include "mvPythonTypeChecker.h"
#include "mvPyObject.h"

namespace Marvel{

	void mvAppItem::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		{
			mvPythonParser parser(mvPyDataType::Dict, "Undocumented", { "App Item Operations" });
			parser.addArg<mvPyDataType::UUID>("item");
			parser.finalize();
			parsers->insert({ "get_item_configuration", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::Dict, "Undocumented", { "App Item Operations" });
			parser.finalize();
			parsers->insert({ "get_item_types", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::Dict, "Undocumented", { "App Item Operations" });
			parser.addArg<mvPyDataType::UUID>("item");
			parser.addArg<mvPyDataType::UUID>("source");
			parser.addArg<mvPyDataType::Integer>("slot");
			parser.finalize();
			parsers->insert({ "set_item_children", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Undocumented", { "App Item Operations" });
			parser.addArg<mvPyDataType::UUID>("item");
			parser.finalize();
			parsers->insert({ "focus_item", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Undocumented", { "Fonts", "App Item Operations" });
			parser.addArg<mvPyDataType::UUID>("item");
			parser.addArg<mvPyDataType::UUID>("font");
			parser.finalize();
			parsers->insert({ "set_item_font", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Sets the item's alias.", { "App Item Operations" });
			parser.addArg<mvPyDataType::UUID>("item");
			parser.addArg<mvPyDataType::String>("alias");
			parser.finalize();
			parsers->insert({ "set_item_alias", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::String, "Gets the item's alias.", { "App Item Operations" });
			parser.addArg<mvPyDataType::UUID>("item");
			parser.finalize();
			parsers->insert({ "get_item_alias", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Undocumented", { "Themes", "App Item Operations" });
			parser.addArg<mvPyDataType::UUID>("item");
			parser.addArg<mvPyDataType::UUID>("theme");
			parser.finalize();
			parsers->insert({ "bind_item_theme", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Undocumented", { "Themes", "App Item Operations" });
			parser.addArg<mvPyDataType::UUID>("item");
			parser.addArg<mvPyDataType::UUID>("theme");
			parser.finalize();
			parsers->insert({ "bind_item_disabled_theme", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Undocumented", { "Themes", "App Item Operations" });
			parser.addArg<mvPyDataType::Integer>("item");
			parser.addArg<mvPyDataType::UUID>("theme");
			parser.finalize();
			parsers->insert({ "bind_item_type_disabled_theme", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Undocumented", { "Themes", "App Item Operations" });
			parser.addArg<mvPyDataType::Integer>("item");
			parser.addArg<mvPyDataType::UUID>("theme");
			parser.finalize();
			parsers->insert({ "bind_item_type_theme", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::Dict, "Undocumented", { "App Item Operations" });
			parser.addArg<mvPyDataType::UUID>("item");
			parser.finalize();
			parsers->insert({ "get_item_state", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Undocumented", { "App Item Operations" });
			parser.addArg<mvPyDataType::UUID>("item");
			parser.addKwargs();
			parser.makeInternal();
			parser.finalize();
			parsers->insert({ "configure_item", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::Dict, "Undocumented", { "App Item Operations" });
			parser.addArg<mvPyDataType::UUID>("item");
			parser.finalize();
			parsers->insert({ "get_item_info", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::Any, "Undocumented", { "App Item Operations" });
			parser.addArg<mvPyDataType::UUID>("item");
			parser.finalize();
			parsers->insert({ "get_value", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::Any, "Undocumented", { "App Item Operations" });
			parser.addArg<mvPyDataType::UUIDList>("items");
			parser.finalize();
			parsers->insert({ "get_values", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Undocumented", { "App Item Operations" });
			parser.addArg<mvPyDataType::UUID>("item");
			parser.addArg<mvPyDataType::Object>("value");
			parser.finalize();
			parsers->insert({ "set_value", parser });
		}

		{
			mvPythonParser parser(mvPyDataType::None, "Undocumented", { "App Item Operations" });
			parser.addArg<mvPyDataType::UUID>("item");
			parser.finalize();
			parsers->insert({ "reset_pos", parser });
		}

	}

	std::vector<mvRef<mvAppItem>>& mvAppItem::getChildren(int slot) 
	{ 
		return _children[slot];
	}

	void mvAppItem::setChildren(int slot, std::vector<mvRef<mvAppItem>> children)
	{
		_children[slot] = children;
	}

	bool mvAppItem::DoesItemHaveFlag(mvAppItem* item, int flag)
	{
		return item->getDescFlags() & flag;
	}

	void mvAppItem::AddCommonArgs(mvPythonParser& parser, CommonParserArgs args)
	{

		parser.addArg<mvPyDataType::String>("label", mvArgType::KEYWORD_ARG, "None", "Overrides 'name' as label.");
		parser.addArg<mvPyDataType::Object>("user_data", mvArgType::KEYWORD_ARG, "None", "User data for callbacks.");
		parser.addArg<mvPyDataType::Bool>("use_internal_label", mvArgType::KEYWORD_ARG, "True", "Use generated internal label instead of user specified (appends ### uuid).");
	
		if(args & MV_PARSER_ARG_ID)            parser.addArg<mvPyDataType::UUID>("tag", mvArgType::KEYWORD_ARG, "0", "Unique id used to programmatically refer to the item.If label is unused this will be the label.");
		if(args & MV_PARSER_ARG_WIDTH)         parser.addArg<mvPyDataType::Integer>("width", mvArgType::KEYWORD_ARG, "0", "Width of the item.");
		if(args & MV_PARSER_ARG_HEIGHT)        parser.addArg<mvPyDataType::Integer>("height", mvArgType::KEYWORD_ARG, "0", "Height of the item.");
		if(args & MV_PARSER_ARG_INDENT)        parser.addArg<mvPyDataType::Integer>("indent", mvArgType::KEYWORD_ARG, "-1", "Offsets the widget to the right the specified number multiplied by the indent style.");
		if(args & MV_PARSER_ARG_PARENT)        parser.addArg<mvPyDataType::UUID>("parent", mvArgType::KEYWORD_ARG, "0", "Parent to add this item to. (runtime adding)");
		if(args & MV_PARSER_ARG_BEFORE)        parser.addArg<mvPyDataType::UUID>("before", mvArgType::KEYWORD_ARG, "0", "This item will be displayed before the specified item in the parent.");
		if(args & MV_PARSER_ARG_SOURCE)        parser.addArg<mvPyDataType::UUID>("source", mvArgType::KEYWORD_ARG, "0", "Overrides 'id' as value storage key.");
		if(args & MV_PARSER_ARG_PAYLOAD_TYPE)  parser.addArg<mvPyDataType::String>("payload_type", mvArgType::KEYWORD_ARG, "'$$DPG_PAYLOAD'", "Sender string type must be the same as the target for the target to run the payload_callback.");		
		if(args & MV_PARSER_ARG_CALLBACK)      parser.addArg<mvPyDataType::Callable>("callback", mvArgType::KEYWORD_ARG, "None", "Registers a callback.");
		if(args & MV_PARSER_ARG_DRAG_CALLBACK) parser.addArg<mvPyDataType::Callable>("drag_callback", mvArgType::KEYWORD_ARG, "None", "Registers a drag callback for drag and drop.");
		if(args & MV_PARSER_ARG_DROP_CALLBACK) parser.addArg<mvPyDataType::Callable>("drop_callback", mvArgType::KEYWORD_ARG, "None", "Registers a drop callback for drag and drop.");
		if(args & MV_PARSER_ARG_SHOW)          parser.addArg<mvPyDataType::Bool>("show", mvArgType::KEYWORD_ARG, "True", "Attempt to render widget.");
		if(args & MV_PARSER_ARG_ENABLED)       parser.addArg<mvPyDataType::Bool>("enabled", mvArgType::KEYWORD_ARG, "True", "Turns off functionality of widget and applies the disabled theme.");
		if(args & MV_PARSER_ARG_POS)		   parser.addArg<mvPyDataType::IntList>("pos", mvArgType::KEYWORD_ARG, "[]", "Places the item relative to window coordinates, [0,0] is top left.");
		if(args & MV_PARSER_ARG_FILTER)		   parser.addArg<mvPyDataType::String>("filter_key", mvArgType::KEYWORD_ARG, "''", "Used by filter widget.");
		if(args & MV_PARSER_ARG_SEARCH_DELAY)  parser.addArg<mvPyDataType::Bool>("delay_search", mvArgType::KEYWORD_ARG, "False", "Delays searching container for specified items until the end of the app. Possible optimization when a container has many children that are not accessed often.");
		
		if (args & MV_PARSER_ARG_TRACKED)
		{
			parser.addArg<mvPyDataType::Bool>("tracked", mvArgType::KEYWORD_ARG, "False", "Scroll tracking");
			parser.addArg<mvPyDataType::Float>("track_offset", mvArgType::KEYWORD_ARG, "0.5", "0.0f:top, 0.5f:center, 1.0f:bottom");
		}

	}

	mvUUID mvAppItem::GetIDFromPyObject(PyObject* item)
	{

		if (item == nullptr)
			return 0;

		if (isPyObject_Int(item))
			return ToUUID(item);
		else if (isPyObject_String(item))
		{
			std::string alias = ToString(item);
			return mvApp::GetApp()->getItemRegistry().getIdFromAlias(alias);
		}
		return 0;
	}

	mvAppItem::mvAppItem(mvUUID uuid)
	{
		_uuid = uuid;
		_internalLabel = "###" + std::to_string(_uuid);
	}

	void mvAppItem::applyTemplate(mvAppItem* item)
	{
		_useInternalLabel = item->_useInternalLabel;
		_tracked = item->_tracked;
		_trackOffset = item->_trackOffset;
		_searchLast = item->_searchLast;
		_indent = item->_indent;
		_show = item->_show;
		_filter = item->_filter;
		_payloadType = item->_payloadType;
		_enabled = item->_enabled;
		_source = item->_source;
		_font = item->_font;
		_theme = item->_theme;
		_disabledTheme = item->_disabledTheme;
		setWidth(item->_width);
		setHeight(item->_height);
		//setPos(item->_state.getItemPos());

		if (!item->_specificedlabel.empty())
		{
			_specificedlabel = item->_specificedlabel;
			if (_useInternalLabel)
				_internalLabel = item->_specificedlabel + "###" + std::to_string(_uuid);
			else
				_internalLabel = item->_specificedlabel;
		}

		if (_enabled) _enabledLastFrame = true;
		else _disabledLastFrame = true;

		if (item->_callback)
		{
			Py_XINCREF(item->_callback);
			setCallback(item->_callback);
		}

		if (item->_dragCallback)
		{
			Py_XINCREF(item->_dragCallback);
			setDragCallback(item->_dragCallback);
		}

		if (item->_dropCallback)
		{
			Py_XINCREF(item->_dropCallback);
			setDropCallback(item->_dropCallback);
		}

		if (item->_user_data)
		{
			Py_XINCREF(item->_user_data);
			setCallbackData(item->_user_data);
		}

		applySpecificTemplate(item);
	}

	bool  mvAppItem::moveChildUp(mvUUID uuid)
	{
		bool found = false;
		int index = 0;

		auto operation = [&](std::vector<mvRef<mvAppItem>>& children)
		{
			// check children
			for (size_t i = 0; i < children.size(); i++)
			{

				if (children[i]->_uuid == uuid)
				{
					found = true;
					index = (int)i;
					break;
				}

				if (mvAppItem::DoesItemHaveFlag(children[i].get(), MV_ITEM_DESC_CONTAINER))
				{
					found = children[i]->moveChildUp(uuid);
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

					updateLocations();
				}

				return true;
			}

			return false;
		};

		for (auto& childset : _children)
		{
			if (operation(childset))
				return true;
		}

		return false;


	}

	bool  mvAppItem::moveChildDown(mvUUID uuid)
	{
		bool found = false;
		size_t index = 0;

		auto operation = [&](std::vector<mvRef<mvAppItem>>& children)
		{
			// check children
			for (size_t i = 0; i < children.size(); i++)
			{

				if (children[i]->_uuid == uuid)
				{
					found = true;
					index = i;
					break;
				}

				if (mvAppItem::DoesItemHaveFlag(children[i].get(), MV_ITEM_DESC_CONTAINER))
				{
					found = children[i]->moveChildDown(uuid);
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

					updateLocations();
				}

				return true;
			}

			return false;
		};

		for (auto& childset : _children)
		{
			if (operation(childset))
				return true;
		}

		return false;

	}

	bool mvAppItem::isParentCompatible(mvAppItemType type) 
	{ 

		const std::vector<std::pair<std::string, int>>* allowableParents = &getAllowableParents();

		std::string acceptableTypes;

		for (const auto& compatibleParent : *allowableParents)
		{
			acceptableTypes.append(compatibleParent.first + "\n");
			if ((int)type == compatibleParent.second)
				return true;
		}

		if (allowableParents->empty())
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleParent, getCommand(),
				"Incompatible parent. Item does can not have a parent.", this);
			return false;
		}

		if ((*allowableParents)[0].second == (int)mvAppItemType::All)
			return true;

		mvThrowPythonError(mvErrorCode::mvIncompatibleParent, getCommand(),
			"Incompatible parent. Acceptable parents include:\t" + acceptableTypes, this);

		assert(false);
		return false;
	}

	bool mvAppItem::canChildBeAdded(mvAppItemType type)
	{ 
		const std::vector<std::pair<std::string, int>>* allowableChildren = &getAllowableChildren();

		std::string acceptableTypes;

		for (const auto& compatibleChildren : *allowableChildren)
		{
			acceptableTypes.append(compatibleChildren.first + "\n");
			if ((int)type == compatibleChildren.second)
				return true;
		}

		if (allowableChildren->empty())
		{
			mvThrowPythonError(mvErrorCode::mvIncompatibleChild, getCommand(),
				"Incompatible child. Item does not accept children.", this);
			return false;
		}

		if ((*allowableChildren)[0].second == (int)mvAppItemType::All)
			return true;

		mvThrowPythonError(mvErrorCode::mvIncompatibleChild, getCommand(),
			"Incompatible child. Acceptable children include:\t" + acceptableTypes, this);

		assert(false);
		return false;
	}

	bool mvAppItem::preDraw()
	{
		if (!_show)
			return false;

		if (_focusNextFrame)
		{
			ImGui::SetKeyboardFocusHere();
			_focusNextFrame = false;
		}

		_previousCursorPos = ImGui::GetCursorPos();
		if (_dirtyPos)
			ImGui::SetCursorPos(_state.getItemPos());

		_state.setPos({ ImGui::GetCursorPosX(), ImGui::GetCursorPosY() });

		// set item width
		if (_width != 0)
			ImGui::SetNextItemWidth((float)_width);

		if (_indent > 0.0f)
			ImGui::Indent(_indent);

		if (_font)
		{
			ImFont* fontptr = static_cast<mvFont*>(_font.get())->getFontPtr();
			ImGui::PushFont(fontptr);
		}


		if (_enabled)
		{
			if (auto classTheme = getClassTheme())
			{
				static_cast<mvTheme*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);
			}

			if (_theme)
			{
				static_cast<mvTheme*>(_theme.get())->draw(nullptr, 0.0f, 0.0f);
			}
		}
		else
		{
			if (auto classTheme = getClassDisabledTheme())
			{
				static_cast<mvTheme*>(classTheme.get())->draw(nullptr, 0.0f, 0.0f);
			}

			if (_disabledTheme)
			{
				static_cast<mvTheme*>(_disabledTheme.get())->draw(nullptr, 0.0f, 0.0f);
			}
		}

		return true;
	}

	void mvAppItem::postDraw()
	{

		if (_dirtyPos)
			ImGui::SetCursorPos(_previousCursorPos);

		if(_indent > 0.0f)
			ImGui::Unindent(_indent);

		_state.update();

		if (_font)
		{
			ImGui::PopFont();
		}

		if (_enabled)
		{
			if (auto classTheme = getClassTheme())
			{
				static_cast<mvTheme*>(classTheme.get())->customAction();
			}

			if (_theme)
			{
				static_cast<mvTheme*>(_theme.get())->customAction();
			}
		}
		else
		{
			if (auto classTheme = getClassDisabledTheme())
			{
				static_cast<mvTheme*>(classTheme.get())->customAction();
			}

			if (_disabledTheme)
			{
				static_cast<mvTheme*>(_disabledTheme.get())->customAction();
			}
		}

		// event handlers
		for (auto& item : _children[3])
		{
			if (!item->preDraw())
				continue;

			item->draw(nullptr, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
		}

		// drag drop
		for (auto& item : _children[4])
		{
			if (!item->preDraw())
				continue;

			item->draw(nullptr, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());
		}

		if (_dropCallback)
		{
			ScopedID id(_uuid);
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(_payloadType.c_str()))
				{
					auto payloadActual = static_cast<const mvDragPayload*>(payload->Data);
					if(_alias.empty())
						mvApp::GetApp()->getCallbackRegistry().addCallback(getDropCallback(), _uuid, payloadActual->getDragData(), nullptr);
					else
						mvApp::GetApp()->getCallbackRegistry().addCallback(getDropCallback(), _alias, payloadActual->getDragData(), nullptr);
				}

				ImGui::EndDragDropTarget();
			}
		}
	}

	void mvAppItem::setPayloadType(const std::string& payloadType)
	{
		_payloadType = payloadType;
	}

	mvAppItem* mvAppItem::getRoot() const
	{
		if (_parentPtr)
		{
			mvAppItem* item = _parentPtr;
			while (!DoesItemHaveFlag(item, MV_ITEM_DESC_ROOT))
				item = item->_parentPtr;

			return item;
		}
		return nullptr;
	}

	void mvAppItem::setPos(const ImVec2& pos)
	{
		_dirtyPos = true;
		_state.setPos({ pos.x, pos.y });
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

			if (mvApp::GetApp()->getItemRegistry().getActiveWindow() != _uuid)
				mvEventBus::Publish(mvEVT_CATEGORY_ITEM, mvEVT_ACTIVE_WINDOW, { CreateEventArgument("WINDOW", _uuid) });

		}
	}

	void mvAppItem::setCallback(PyObject* callback)
	{ 

		if (callback == Py_None)
		{
			_callback = nullptr;
			return;
		}

		_callback = callback;
	}

	void mvAppItem::setDragCallback(PyObject* callback)
	{

		if (callback == Py_None)
		{
			_dragCallback = nullptr;
			return;
		}

		_dragCallback = callback;
	}

	void mvAppItem::setDropCallback(PyObject* callback)
	{

		if (callback == Py_None)
		{
			_dropCallback = nullptr;
			return;
		}

		_dropCallback = callback;
	}

	void mvAppItem::setCallbackData(PyObject* data)
	{
		if (data == Py_None)
		{
			_user_data = nullptr;
			return;
		}
		_user_data = data;
	}

	void mvAppItem::resetState()
	{
		_state.reset();
		for (auto& childset : _children)
		{
			for (auto& child : childset)
				child->resetState();
		}
	}

	bool mvAppItem::addRuntimeChild(mvUUID parent, mvUUID before, mvRef<mvAppItem> item)
	{
		if (before == 0 && parent == 0)
			return false;


		for (auto& children : _children)
		{
			//this is the container, add item to end.
			if (before == 0)
			{

				if (_uuid == parent)
				{
					item->_location = _children[item->getTarget()].size();
					_children[item->getTarget()].push_back(item);
					onChildAdd(item);
					item->_parentPtr = this;
					item->_parent = _uuid;
					return true;
				}

				// check children
				for (auto& childslot : _children)
				{
					for (auto& child : childslot)
					{
						if (mvAppItem::DoesItemHaveFlag(child.get(), MV_ITEM_DESC_CONTAINER) || mvAppItem::DoesItemHaveFlag(item.get(), MV_ITEM_DESC_HANDLER))
						{
							// parent found
							if (child->addRuntimeChild(parent, before, item))
								return true;
						}
					}
				}
			}

			// this is the container, add item to beginning.
			else
			{
				bool beforeFound = false;

				// check children
				for (auto& child : children)
				{

					if (child->_uuid == before)
					{
						beforeFound = true;
						break;
					}
				}


				// after item is in this container
				if (beforeFound)
				{
					item->_parentPtr = this;

					std::vector<mvRef<mvAppItem>> oldchildren = children;
					children.clear();

					for (auto& child : oldchildren)
					{
						if (child->_uuid == before)
						{
							children.push_back(item);
							onChildAdd(item);
						}
						children.push_back(child);

					}

					updateLocations();

					return true;
				}
			}

			// check children
			for (auto& child : children)
			{
				if (mvAppItem::DoesItemHaveFlag(child.get(), MV_ITEM_DESC_CONTAINER) || mvAppItem::DoesItemHaveFlag(item.get(), MV_ITEM_DESC_HANDLER))
				{
					// parent found
					if (child->addRuntimeChild(parent, before, item))
						return true;
				}
			}

		};

		return false;
	}

	bool mvAppItem::addItem(mvRef<mvAppItem> item)
	{
		item->_location = _children[item->getTarget()].size();
		_children[item->getTarget()].push_back(item);
		onChildAdd(item);

		return true;
	}

	bool mvAppItem::addChildAfter(mvUUID prev, mvRef<mvAppItem> item)
	{
		if (prev == 0)
			return false;


		auto operation = [&](std::vector<mvRef<mvAppItem>>& children)
		{
			bool prevFound = false;

			// check children
			for (auto& childslot : _children)
			{
				for (auto& child : childslot)
				{

					if (child->_uuid == prev)
					{
						item->_parentPtr = this;
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
					if (child->_uuid == prev)
					{
						children.push_back(item);
						onChildAdd(item);
					}
				}

				return true;
			}


			// check children
			for (auto& childslot : _children)
			{
				for (auto& child : childslot)
				{
					// parent found
					if (child->addChildAfter(prev, item))
						return true;
				}
			}

			return false;
		};

		// todo: fix this
		return operation(_children[item->getTarget()]);
	}

	bool mvAppItem::deleteChild(mvUUID uuid)
	{

		auto operation = [&](std::vector<mvRef<mvAppItem>>& children)
		{
			bool childfound = false;
			bool itemDeleted = false;

			for (auto& item : children)
			{
				if (item->_uuid == uuid)
				{
					childfound = true;
					break;
				}

				itemDeleted = item->deleteChild(uuid);
				if (itemDeleted)
					break;
			}

			if (childfound)
			{
				std::vector<mvRef<mvAppItem>> oldchildren = children;

				children.clear();

				for (auto& item : oldchildren)
				{
					if (item->_uuid == uuid)
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

		for (auto& childset : _children)
		{
			if (operation(childset))
			{
				updateLocations();
				return true;
			}
		}

		return false;
	}

	void mvAppItem::updateLocations()
	{
		for (auto& childset : _children)
		{
			int index = 0;
			for (auto& child : childset)
			{
				child->_location = index;
				index++;
			}
		}
	}

	void mvAppItem::deleteChildren(int slot)
	{
		if (slot < 0)
		{
			for (auto& childset : _children)
			{
				childset.clear();
				childset.shrink_to_fit();
			}
		}
		else if (slot < 5)
		{
			_children[slot].clear();
			_children[slot].shrink_to_fit();
		}
		onChildrenRemoved();
	}

	void mvAppItem::setLabel(const std::string& value)
	{
		_specificedlabel = value;
		if (_useInternalLabel)
			_internalLabel = value + "###" + std::to_string(_uuid);
		else
			_internalLabel = value;
	}

	void mvAppItem::setFilter(const std::string& value)
	{
		_filter = value;
	}

	mvRef<mvAppItem> mvAppItem::stealChild(mvUUID uuid)
	{
		mvRef<mvAppItem> stolenChild = nullptr;

		for (auto& childset : _children)
		{
			bool childfound = false;

			for (auto& item : childset)
			{
				if (item->_uuid == uuid)
				{
					childfound = true;
					break;
				}

				if (DoesItemHaveFlag(item.get(), MV_ITEM_DESC_CONTAINER))
				{
					stolenChild = item->stealChild(uuid);
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
					if (item->_uuid == uuid)
					{
						stolenChild = item;
						onChildRemoved(item);
						continue;
					}

					childset.push_back(item);
				}

				updateLocations();

				return stolenChild;
			}


			//return static_cast<mvRef<mvAppItem>>(CreateRef<mvButton>("Not possible"));
		}

		return stolenChild;
	}

	mvAppItem* mvAppItem::getChild(mvUUID uuid)
	{

		if (_uuid == uuid)
			return this;

		if (_searchLast)
		{
			if (_searchDelayed)
				_searchDelayed = false;
			else
			{
				_searchDelayed = true;
				mvApp::GetApp()->getItemRegistry().delaySearch(this);
				return nullptr;
			}
		}

		for (auto& childset : _children)
		{
			for (auto& item : childset)
			{
				if (item->_uuid == uuid)
					return item.get();

				auto child = item->getChild(uuid);
				if (child)
					return child;
			}
		}

		return nullptr;
	}

	mvRef<mvAppItem> mvAppItem::getChildRef(mvUUID uuid)
	{

		for (auto& childset : _children)
		{
			for (auto& item : childset)
			{
				if (item->_uuid == uuid)
					return item;

				auto child = item->getChildRef(uuid);
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
		if (_callback)
			Py_DECREF(_callback);
		if (_user_data)
			Py_DECREF(_user_data);

		// in case item registry is destroyed
		if (mvApp::IsAppStarted())
		{
			mvApp::GetApp()->getItemRegistry().cleanUpItem(_uuid);
			mvApp::GetApp()->getItemRegistry().removeAlias(_alias, true);
		}
	}

	PyObject* mvAppItem::getCallback(bool ignore_enabled)
	{
		if (_enabled)
			return _callback;

		return ignore_enabled ? _callback : nullptr;
		
	}

	void mvAppItem::checkArgs(PyObject* args, PyObject* kwargs, std::string parser)
	{
		mvApp::GetApp()->getParsers()[parser].verifyArgumentCount(args);
	}

	void mvAppItem::handleKeywordArgs(PyObject* dict, std::string parser)
	{
		if (dict == nullptr)
			return;

		if (mvApp::GetApp()->getParsers()[parser].verifyKeywordArguments(dict))
			return;

		if (PyArg_ValidateKeywordArguments(dict) == 0)
		{
			assert(false);
			mvThrowPythonError(mvErrorCode::mvNone, "Dictionary keywords must be strings");
			return;
		}

		if (PyObject* item = PyDict_GetItemString(dict, "use_internal_label")) _useInternalLabel = ToBool(item); // must be before label

		if (PyObject* item = PyDict_GetItemString(dict, "label"))
		{
			if (item != Py_None)
			{
				const std::string label = ToString(item);
				setLabel(label);
			}
		}

		
		if (PyObject* item = PyDict_GetItemString(dict, "width")) setWidth(ToInt(item));
		if (PyObject* item = PyDict_GetItemString(dict, "height")) setHeight(ToInt(item));
		if (PyObject* item = PyDict_GetItemString(dict, "pos")) {
			std::vector<float> position = ToFloatVect(item);
			if (!position.empty())
				setPos(mvVec2{ position[0], position[1] });
		}
		if (PyObject* item = PyDict_GetItemString(dict, "indent")) _indent = (float)ToInt(item);
		if (PyObject* item = PyDict_GetItemString(dict, "show")) 
		{
			_show = ToBool(item);
			if (_show)
				show();
			else
				hide();
		}

		if (PyObject* item = PyDict_GetItemString(dict, "filter_key")) _filter = ToString(item);
		if (PyObject* item = PyDict_GetItemString(dict, "payload_type")) setPayloadType(ToString(item));
		if (PyObject* item = PyDict_GetItemString(dict, "source"))
		{
			if (isPyObject_Int(item))
				setDataSource(ToUUID(item));
			else if (isPyObject_String(item))
			{
				std::string alias = ToString(item);
				setDataSource(mvApp::GetApp()->getItemRegistry().getIdFromAlias(alias));
			}
		}
		if (PyObject* item = PyDict_GetItemString(dict, "enabled")) setEnabled(ToBool(item));
		if (PyObject* item = PyDict_GetItemString(dict, "tracked")) _tracked = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "delay_search")) _searchLast = ToBool(item);
		if (PyObject* item = PyDict_GetItemString(dict, "track_offset"))
		{
			_trackOffset = ToFloat(item);
		}
		if (PyObject* item = PyDict_GetItemString(dict, "default_value"))
		{
			if(_source == 0)
				setPyValue(item);
		}

		if (PyObject* item = PyDict_GetItemString(dict, "callback"))
		{
			if (_callback)
				Py_XDECREF(_callback);

			Py_XINCREF(item);
			setCallback(item);
		}

		if (PyObject* item = PyDict_GetItemString(dict, "drag_callback"))
		{
			if (_dragCallback)
				Py_XDECREF(_dragCallback);

			Py_XINCREF(item);
			setDragCallback(item);
		}

		if (PyObject* item = PyDict_GetItemString(dict, "drop_callback"))
		{
			if (_dropCallback)
				Py_XDECREF(_dropCallback);

			Py_XINCREF(item);
			setDropCallback(item);
		}

		if (PyObject* item = PyDict_GetItemString(dict, "user_data"))
		{
			if (_user_data)
				Py_XDECREF(_user_data);
			
			Py_XINCREF(item);
			setCallbackData(item);
		}

		handleSpecificKeywordArgs(dict);
	}

	void mvAppItem::setEnabled(bool value) 
	{ 
		if (_enabled == value)
			return;

		_enabled = value; 
		
		if (value)
			_enabledLastFrame = true;
		else
			_disabledLastFrame = true; 
	}

	bool mvAppItem::shouldFocusNextFrame() const 
	{ 
		return _focusNextFrame; 
	}

	bool mvAppItem::wasShownLastFrameReset() 
	{ 
		bool result = _shownLastFrame;
		_shownLastFrame = false; 
		return result; 
	}

	bool mvAppItem::wasHiddenLastFrameReset() 
	{ 
		bool result = _hiddenLastFrame;
		_hiddenLastFrame = false; 
		return result; 
	}

	bool mvAppItem::wasEnabledLastFrameReset() 
	{ 
		bool result = _enabledLastFrame; 
		_enabledLastFrame = false;
		return result; 
	}

	bool mvAppItem::wasDisabledLastFrameReset() 
	{
		bool result = _disabledLastFrame;
		_disabledLastFrame = false; 
		return result; 
	}

	void mvAppItem::setAlias(const std::string& value)
	{
		// remove old alias from registry
		if (!_alias.empty())
			mvApp::GetApp()->getItemRegistry().removeAlias(_alias, true);
			
		_alias = value;

		// only register non-empty aliases
		if (!_alias.empty())
			mvApp::GetApp()->getItemRegistry().addAlias(_alias, _uuid);
	}

	void mvAppItem::setWidth(int width) 
	{
		_dirty_size = true;  
		_width = width; 
	}

	void mvAppItem::setHeight(int height) 
	{ 
		_dirty_size = true;  
		_height = height; 
	}

	void mvAppItem::hide() 
	{ 
		_show = false; 
		_hiddenLastFrame = true; 
	}

	void mvAppItem::show() 
	{
		_show = true; 
		_shownLastFrame = true; 
	}

	void mvAppItem::setDataSource(mvUUID value)
	{
		_source = value; 
	}

	void mvAppItem::focus() 
	{ 
		_focusNextFrame = true; 
	}

	void mvAppItem::unfocus() 
	{ 
		_focusNextFrame = false; 
	}

	mvAppItemState& mvAppItem::getState() 
	{ 
		return _state; 
	}

	mvAppItem* mvAppItem::getParent() 
	{ 
		return _parentPtr; 
	}

	int mvAppItem::getLocation() const 
	{
		return _location; 
	}

	void mvAppItem::requestAltCustomAction() 
	{ 
		_triggerAlternativeAction = true; 
	}

	bool mvAppItem::isAltCustomActionRequested() const 
	{ 
		return _triggerAlternativeAction; 
	}

	std::tuple<mvUUID, mvUUID, std::string> mvAppItem::GetNameFromArgs(mvUUID& name, PyObject* args, PyObject* kwargs)
	{

		mvUUID parent = 0;
		mvUUID before = 0;
		std::string alias;

		if (kwargs)
		{
			if (PyObject* item = PyDict_GetItemString(kwargs, "parent"))
			{
				if (PyUnicode_Check(item))
					parent = mvApp::GetApp()->getItemRegistry().getIdFromAlias(ToString(item));
				else
					parent = ToUUID(item);
			}
			
			if (PyObject* item = PyDict_GetItemString(kwargs, "before"))
			{
				if (PyUnicode_Check(item))
					before = mvApp::GetApp()->getItemRegistry().getIdFromAlias(ToString(item));
				else
					before = ToUUID(item);
			}

			if (PyObject* item = PyDict_GetItemString(kwargs, "tag"))
			{
				if (PyUnicode_Check(item))
					alias = ToString(item);
				else
				{
					auto id = ToUUID(item);
					if (id > 0)
						name = id;
				}
			};
		}

		return std::make_tuple(parent, before, alias);
	}

	void mvAppItem::getItemInfo(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		std::string parserCommand = getCommand();

		auto children = mvApp::GetApp()->getItemRegistry().getItemChildren(_uuid);
		if (children.empty())
			PyDict_SetItemString(dict, "children", mvPyObject(GetPyNone()));
		else
		{
			PyObject* pyChildren = PyDict_New();
			int i = 0;
			for (const auto& slot : children)
			{
				PyDict_SetItem(pyChildren, ToPyInt(i), mvPyObject(ToPyList(slot)));
				i++;
			}
			PyDict_SetItemString(dict, "children", mvPyObject(pyChildren));
		}

		PyDict_SetItemString(dict, "type", mvPyObject(ToPyString(parserCommand)));
		PyDict_SetItemString(dict, "target", mvPyObject(ToPyInt(getTarget())));

		if (_parentPtr)
			PyDict_SetItemString(dict, "parent", mvPyObject(ToPyUUID(_parentPtr->getUUID())));
		else
			PyDict_SetItemString(dict, "parent", mvPyObject(GetPyNone()));

		if (_theme)
			PyDict_SetItemString(dict, "theme", mvPyObject(ToPyUUID(_theme->getUUID())));
		else
			PyDict_SetItemString(dict, "theme", mvPyObject(GetPyNone()));

		if (_font)
			PyDict_SetItemString(dict, "font", mvPyObject(ToPyUUID(_font->getUUID())));
		else
			PyDict_SetItemString(dict, "font", mvPyObject(GetPyNone()));

		if (_disabledTheme)
			PyDict_SetItemString(dict, "disabled_theme", mvPyObject(ToPyUUID(_disabledTheme->getUUID())));
		else
			PyDict_SetItemString(dict, "disabled_theme", mvPyObject(GetPyNone()));

		if(getDescFlags() & MV_ITEM_DESC_CONTAINER)
			PyDict_SetItemString(dict, "container", mvPyObject(ToPyBool(true)));
		else
			PyDict_SetItemString(dict, "container", mvPyObject(ToPyBool(false)));

	}

	void mvAppItem::getConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		// config py objects
		mvPyObject py_filter_key = ToPyString(_filter);
		mvPyObject py_payload_type = ToPyString(_payloadType);
		mvPyObject py_label = ToPyString(_specificedlabel);
		mvPyObject py_use_internal_label = ToPyBool(_useInternalLabel);
		mvPyObject py_source = ToPyUUID(_source);
		mvPyObject py_show = ToPyBool(_show);
		mvPyObject py_enabled = ToPyBool(_enabled);
		mvPyObject py_tracked = ToPyBool(_tracked);
		mvPyObject py_width = ToPyInt(_width);
		mvPyObject py_track_offset = ToPyFloat(_trackOffset);
		mvPyObject py_height = ToPyInt(_height);
		mvPyObject py_indent = ToPyInt(_indent);

		PyDict_SetItemString(dict, "filter_key", py_filter_key);
		PyDict_SetItemString(dict, "payload_type", py_payload_type);
		PyDict_SetItemString(dict, "label", py_label);
		PyDict_SetItemString(dict, "use_internal_label", py_use_internal_label);
		PyDict_SetItemString(dict, "source", py_source);
		PyDict_SetItemString(dict, "show", py_show);
		PyDict_SetItemString(dict, "enabled", py_enabled);
		PyDict_SetItemString(dict, "tracked", py_tracked);
		PyDict_SetItemString(dict, "width", py_width);
		PyDict_SetItemString(dict, "track_offset", py_track_offset);
		PyDict_SetItemString(dict, "height", py_height);
		PyDict_SetItemString(dict, "indent", py_indent);

		if (_callback)
		{
			Py_XINCREF(_callback);
			PyDict_SetItemString(dict, "callback", _callback);
		}
		else
			PyDict_SetItemString(dict, "callback", GetPyNone());

		if (_dropCallback)
		{
			Py_XINCREF(_dropCallback);
			PyDict_SetItemString(dict, "drop_callback", _dropCallback);
		}
		else
			PyDict_SetItemString(dict, "drop_callback", GetPyNone());

		if (_dragCallback)
		{
			Py_XINCREF(_dragCallback);
			PyDict_SetItemString(dict, "drag_callback", _dragCallback);
		}
		else
			PyDict_SetItemString(dict, "drag_callback", GetPyNone());

		if (_user_data)
		{
			Py_XINCREF(_user_data);
			PyDict_SetItemString(dict, "user_data", _user_data);
		}
		else
			PyDict_SetItemString(dict, "user_data", GetPyNone());
	}

	void mvAppItem::setPoolInfo(mvUUID pool, mvUUID itemSet)
	{
		_pool = pool;
		_itemSet = itemSet;
	}

	std::pair<mvUUID, mvUUID> mvAppItem::getPoolInfo() const
	{
		return std::make_pair(_pool, _itemSet);
	}

	PyObject* mvAppItem::get_item_configuration(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;

		if (!(mvApp::GetApp()->getParsers())["get_item_configuration"].parse(args, kwargs, __FUNCTION__, &itemraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		PyObject* pdict = PyDict_New();

		if (appitem)
		{
			appitem->getConfiguration(pdict);
			appitem->getSpecificConfiguration(pdict);
		}
		else
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "get_item_configuration",
				"Item not found: " + std::to_string(item), nullptr);

		return pdict;
	}

	PyObject* mvAppItem::focus_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;

		if (!(mvApp::GetApp()->getParsers())["focus_item"].parse(args, kwargs, __FUNCTION__, &itemraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);
		mvApp::GetApp()->getItemRegistry().focusItem(item);

		return GetPyNone();
	}

	PyObject* mvAppItem::set_item_children(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;
		PyObject* sourceraw;
		int slot;

		if (!(mvApp::GetApp()->getParsers())["set_item_children"].parse(args, kwargs, __FUNCTION__,
			&itemraw, &sourceraw, &slot))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);
		mvUUID source = mvAppItem::GetIDFromPyObject(sourceraw);
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);
		
		auto& staging = mvApp::GetApp()->getItemRegistry().getStaging();

		bool stage_found = false;
		mvRef<mvAppItem> staging_container = nullptr;

		for (auto& stage : staging)
		{
			if(stage->getUUID() == source)
			{ 
				staging_container = stage;
				stage_found = true;
				break;
			}
		}

		if (!stage_found)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "set_item_children",
				"Stage item not found: " + std::to_string(item), nullptr);
			assert(false);
			return GetPyNone();
		}

		
		if (appitem)
		{
			auto& oldChildren = appitem->getChildren(slot);
			oldChildren.reserve(staging_container->_children[slot].size());
			oldChildren = std::move(staging_container->_children[slot]);
			for (auto& child : oldChildren)
			{
				child->_parent = item;
				child->_parentPtr = appitem;
			}

			appitem->updateLocations();
			
		}
		else
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "set_item_children",
				"Item not found: " + std::to_string(item), nullptr);

		mvApp::GetApp()->getItemRegistry().deleteItem(source);

		return GetPyNone();
	}

	PyObject* mvAppItem::bind_item_font(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;
		PyObject* fontraw;

		if (!(mvApp::GetApp()->getParsers())["bind_item_font"].parse(args, kwargs, __FUNCTION__,
			&itemraw, &fontraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);
		mvUUID font = mvAppItem::GetIDFromPyObject(fontraw);
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);
		auto appfont = mvApp::GetApp()->getItemRegistry().getRefItem(font);

		if (appitem)
		{
			if (font == 0)
			{
				appitem->_font = nullptr;
				return GetPyNone();
			}

			if (appfont)
			{
				appitem->_font = appfont;
			}
			else
			{
				mvThrowPythonError(mvErrorCode::mvItemNotFound, "bind_item_font",
					"Font item not found: " + std::to_string(item), nullptr);
			}
		}
		else
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "bind_item_font",
				"Item not found: " + std::to_string(item), nullptr);

		return GetPyNone();
	}

	PyObject* mvAppItem::bind_item_theme(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;
		PyObject* themeraw;

		if (!(mvApp::GetApp()->getParsers())["bind_item_theme"].parse(args, kwargs, __FUNCTION__,
			&itemraw, &themeraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);
		mvUUID theme = mvAppItem::GetIDFromPyObject(themeraw);
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
		{
			if (theme == 0)
			{
				appitem->_theme = nullptr;
				return GetPyNone();
			}

			auto apptheme = mvApp::GetApp()->getItemRegistry().getRefItem(theme);

			if (apptheme)
			{
				appitem->_theme = apptheme;
				return GetPyNone();
			}
			else
				mvThrowPythonError(mvErrorCode::mvItemNotFound, "bind_item_theme",
					"Theme item not found: " + std::to_string(item), nullptr);
		}
		else
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "bind_item_theme",
				"Item not found: " + std::to_string(item), nullptr);

		return GetPyNone();
	}

	PyObject* mvAppItem::bind_item_disabled_theme(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;
		PyObject* themeraw;

		if (!(mvApp::GetApp()->getParsers())["bind_item_disabled_theme"].parse(args, kwargs, __FUNCTION__,
			&itemraw, &themeraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);
		mvUUID theme = mvAppItem::GetIDFromPyObject(themeraw);
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
		{
			if (theme == 0)
			{
				appitem->_theme = nullptr;
				return GetPyNone();
			}

			auto apptheme = mvApp::GetApp()->getItemRegistry().getRefItem(theme);

			if (apptheme)
			{
				appitem->_disabledTheme = apptheme;
				return GetPyNone();
			}
			else
				mvThrowPythonError(mvErrorCode::mvItemNotFound, "bind_item_disabled_theme",
					"Disabled theme item not found: " + std::to_string(item), nullptr);
		}
		else
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "bind_item_disabled_theme",
				"Item not found: " + std::to_string(item), nullptr);

		return GetPyNone();
	}

	PyObject* mvAppItem::reset_pos(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;

		if (!(mvApp::GetApp()->getParsers())["reset_pos"].parse(args, kwargs, __FUNCTION__,
			&itemraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
			appitem->_dirtyPos = false;
		else
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "reset_pos",
				"Item not found: " + std::to_string(item), nullptr);

		return GetPyNone();
	}

	PyObject* mvAppItem::get_item_state(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;

		if (!(mvApp::GetApp()->getParsers())["get_item_state"].parse(args, kwargs, __FUNCTION__, &itemraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		PyObject* pdict = PyDict_New();

		if (appitem)
			appitem->getState().getState(pdict);

		else
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "get_item_state",
				"Item not found: " + std::to_string(item), nullptr);

		return pdict;
	}

	PyObject* mvAppItem::get_item_types(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		PyObject* pdict = PyDict_New();
		constexpr_for<1, (int)mvAppItemType::ItemTypeCount, 1>(
			[&](auto i) {

				using item_type = typename mvItemTypeMap<i>::type;
				PyDict_SetItemString(pdict, mvItemTypeMap<i>::s_class, PyLong_FromLong(item_type::s_internal_type));
			});

		return pdict;
	}

	PyObject* mvAppItem::get_item_info(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;

		if (!(mvApp::GetApp()->getParsers())["get_item_info"].parse(args, kwargs, __FUNCTION__, &itemraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		PyObject* pdict = PyDict_New();

		if (appitem)
			appitem->getItemInfo(pdict);

		else
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "get_item_info",
				"Item not found: " + std::to_string(item), nullptr);

		return pdict;
	}

	PyObject* mvAppItem::configure_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(PyTuple_GetItem(args, 0));
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);

		if (appitem)
		{
			//appitem->checkArgs(args, kwargs);
			appitem->handleKeywordArgs(kwargs, appitem->getCommand());
		}
		else
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "configure_item",
				"Item not found: " + std::to_string(item), nullptr);

		return GetPyNone();
	}

	PyObject* mvAppItem::get_value(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* nameraw;

		if (!(mvApp::GetApp()->getParsers())["get_value"].parse(args, kwargs, __FUNCTION__, &nameraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID name = mvAppItem::GetIDFromPyObject(nameraw);
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

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		auto aitems = ToUUIDVect(items);
		PyObject* pyvalues = PyList_New(aitems.size());

		for (size_t i = 0; i < aitems.size(); i++)
		{
			auto item = mvApp::GetApp()->getItemRegistry().getItem(aitems[i]);
			if (item)
				PyList_SetItem(pyvalues, i, item->getPyValue());
			else
			{
				mvThrowPythonError(mvErrorCode::mvItemNotFound, "get_values",
					"Item not found: " + std::to_string(aitems[i]), nullptr);
				PyList_SetItem(pyvalues, i, GetPyNone());
			}
		}

		return pyvalues;
	}

	PyObject* mvAppItem::set_value(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* nameraw;
		PyObject* value;

		if (!(mvApp::GetApp()->getParsers())["set_value"].parse(args, kwargs, __FUNCTION__, &nameraw, &value))
			return GetPyNone();

		if (value)
			Py_XINCREF(value);

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID name = mvAppItem::GetIDFromPyObject(nameraw);

		auto item = mvApp::GetApp()->getItemRegistry().getItem(name);
		if (item)
			item->setPyValue(value);
		else
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "set_value",
				"Item not found: " + std::to_string(name), nullptr);
		}

		Py_XDECREF(value);

		return GetPyNone();
	}

	PyObject* mvAppItem::bind_item_type_theme(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int item;
		PyObject* themeraw;

		if (!(mvApp::GetApp()->getParsers())["bind_item_type_theme"].parse(args, kwargs, __FUNCTION__,
			&item, &themeraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID theme = mvAppItem::GetIDFromPyObject(themeraw);

		// reset
		if (theme == 0)
		{
			constexpr_for<1, (int)mvAppItemType::ItemTypeCount, 1>(
				[&](auto i) {
					using item_type = typename mvItemTypeMap<i>::type;
					if(i == item)
						item_type::s_class_theme = nullptr;
				});
		}
		else
		{
			auto themeitem = mvApp::GetApp()->getItemRegistry().getRefItem(theme);
			if (themeitem)
			{
				constexpr_for<1, (int)mvAppItemType::ItemTypeCount, 1>(
					[&](auto i) {
						using item_type = typename mvItemTypeMap<i>::type;
						if (i == item)
							item_type::s_class_theme = themeitem;
					});
			}
			else
			{
				mvThrowPythonError(mvErrorCode::mvItemNotFound, "bind_item_type_theme",
					"Theme item not found: " + std::to_string(item), nullptr);;
			}
		}

		return GetPyNone();
	}

	PyObject* mvAppItem::bind_item_type_disabled_theme(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		int item;
		PyObject* themeraw;

		if (!(mvApp::GetApp()->getParsers())["set_item_type_disabled_theme"].parse(args, kwargs, __FUNCTION__,
			&item, &themeraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID theme = mvAppItem::GetIDFromPyObject(themeraw);

		// reset
		if (theme == 0)
		{
			constexpr_for<1, (int)mvAppItemType::ItemTypeCount, 1>(
				[&](auto i) {
					using item_type = typename mvItemTypeMap<i>::type;
					if (i == item)
						item_type::s_class_theme = nullptr;
				});
		}
		else
		{
			auto themeitem = mvApp::GetApp()->getItemRegistry().getRefItem(theme);
			if (themeitem)
			{
				constexpr_for<1, (int)mvAppItemType::ItemTypeCount, 1>(
					[&](auto i) {
						using item_type = typename mvItemTypeMap<i>::type;
						if (i == item)
							item_type::s_class_disabled_theme = themeitem;
					});
			}
			else
			{
				mvThrowPythonError(mvErrorCode::mvItemNotFound, "set_item_type_disabled_theme",
					"Disabled theme item not found: " + std::to_string(item), nullptr);
			}
		}

		return GetPyNone();
	}

	PyObject* mvAppItem::set_item_alias(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;
		const char* alias;

		if (!(mvApp::GetApp()->getParsers())["set_item_alias"].parse(args, kwargs, __FUNCTION__,
			&itemraw, &alias))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (appitem)
			appitem->_alias = alias;
		return GetPyNone();
	}

	PyObject* mvAppItem::get_item_alias(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;

		if (!(mvApp::GetApp()->getParsers())["get_item_alias"].parse(args, kwargs, __FUNCTION__,
			&itemraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);
		auto appitem = mvApp::GetApp()->getItemRegistry().getItem(item);
		if (appitem)
			return ToPyString(appitem->_alias);
		return GetPyNone();
	}
}
