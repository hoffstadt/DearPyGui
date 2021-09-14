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

	static void DebugItem(const char* label, const char* item) {
		ImGui::Text("%s", label);
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%s", item);
	}

	void mvAppItem::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{
		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Returns an item's configuration.";
			setup.category = { "App Item Operations" };
			setup.returnType = mvPyDataType::Dict;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "get_item_configuration", parser });
		}

		{

			mvPythonParserSetup setup;
			setup.about = "Returns an item types.";
			setup.category = { "App Item Operations" };
			setup.returnType = mvPyDataType::Dict;

			mvPythonParser parser = FinalizeParser(setup, {});
			parsers->insert({ "get_item_types", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });
			args.push_back({ mvPyDataType::UUID, "source" });
			args.push_back({ mvPyDataType::Integer, "slot" });

			mvPythonParserSetup setup;
			setup.about = "Sets an item's children.";
			setup.category = { "App Item Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "set_item_children", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Focuses an item.";
			setup.category = { "App Item Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "focus_item", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item"});
			args.push_back({ mvPyDataType::UUID, "font"});

			mvPythonParserSetup setup;
			setup.about = "Sets an item's font.";
			setup.category = { "Fonts", "App Item Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "set_item_font", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });
			args.push_back({ mvPyDataType::String, "alias" });

			mvPythonParserSetup setup;
			setup.about = "Sets an item's alias.";
			setup.category = { "App Item Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "set_item_alias", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Returns an item's alias.";
			setup.category = { "App Item Operations" };
			setup.returnType = mvPyDataType::String;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "get_item_alias", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });
			args.push_back({ mvPyDataType::UUID, "handler_registry" });

			mvPythonParserSetup setup;
			setup.about = "Binds an item handler registry to an item.";
			setup.category = { "App Item Operations", "Events"};

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "bind_item_handler_registry", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });
			args.push_back({ mvPyDataType::UUID, "theme" });

			mvPythonParserSetup setup;
			setup.about = "Binds a theme to an item.";
			setup.category = { "App Item Operations", "Themes" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "bind_item_theme", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Returns an item's state.";
			setup.category = { "App Item Operations" };
			setup.returnType = mvPyDataType::Dict;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "get_item_state", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Configures an item.";
			setup.category = { "App Item Operations" };
			setup.unspecifiedKwargs = true;
			setup.internal = true;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "configure_item", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Returns an item's information.";
			setup.category = { "App Item Operations" };
			setup.returnType = mvPyDataType::Dict;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "get_item_info", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Returns an item's value.";
			setup.category = { "App Item Operations" };
			setup.returnType = mvPyDataType::Any;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "get_value", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUIDList, "items" });

			mvPythonParserSetup setup;
			setup.about = "Returns values of a list of items.";
			setup.category = { "App Item Operations" };
			setup.returnType = mvPyDataType::Any;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "get_values", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });
			args.push_back({ mvPyDataType::Object, "value" });

			mvPythonParserSetup setup;
			setup.about = "Set's an item's value.";
			setup.category = { "App Item Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "set_value", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Resets an item's position after using 'set_item_pos'.";
			setup.category = { "App Item Operations" };

			mvPythonParser parser = FinalizeParser(setup, args);
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

	mvUUID mvAppItem::GetIDFromPyObject(PyObject* item)
	{

		if (item == nullptr)
			return 0;

		if (isPyObject_Int(item))
			return ToUUID(item);
		else if (isPyObject_String(item))
		{
			std::string alias = ToString(item);
			return GetIdFromAlias(*mvApp::GetApp()->itemRegistry, alias);
		}
		return 0;
	}

	mvAppItem::mvAppItem(mvUUID uuid)
	{
		_uuid = uuid;
		_internalLabel = "###" + std::to_string(_uuid);
	}

	void mvAppItem::renderDebugInfo()
	{
		static char ts[6] = "True";
		static char fs[6] = "False";

		std::string width = std::to_string(_width);
		std::string height = std::to_string(_height);

		std::string sizex = std::to_string(_state.rectSize.x);
		std::string sizey = std::to_string(_state.rectSize.y);

		ImGui::PushID(this);
		DebugItem("Label:", _specifiedLabel.c_str());
		DebugItem("ID:", std::to_string(_uuid).c_str());
		DebugItem("Alias:", _alias.c_str());
		DebugItem("Type:", getTypeString());
		DebugItem("Filter:", _filter.c_str());
		DebugItem("Payload Type:", _payloadType.c_str());
		DebugItem("Location:", std::to_string(_location).c_str());
		DebugItem("Track Offset:", std::to_string(_trackOffset).c_str());
		DebugItem("Container:", mvAppItem::DoesItemHaveFlag(this, MV_ITEM_DESC_CONTAINER) ? ts : fs);
		DebugItem("Width:", width.c_str());
		DebugItem("Height:", height.c_str());
		DebugItem("Size x:", sizex.c_str());
		DebugItem("Size y:", sizey.c_str());
		DebugItem("Show:", _show ? ts : fs);
		DebugItem("Enabled:", _enabled ? ts : fs);
		DebugItem("Tracked:", _tracked ? ts : fs);
		DebugItem("Callback:", _callback ? ts : fs);
		DebugItem("User Data:", _user_data ? ts : fs);
		DebugItem("Drop Callback:", _dropCallback ? ts : fs);
		DebugItem("Drag Callback:", _dragCallback ? ts : fs);

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Text("Bindings");
		ImGui::Separator();
		DebugItem("Theme Bound:", _theme ? ts : fs);
		DebugItem("Font Bound:", _font ? ts : fs);
		DebugItem("Handlers Bound:", _handlerRegistry ? ts : fs);

		int applicableState = getApplicableState();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Text("State");
		ImGui::Separator();
		if (applicableState & MV_STATE_VISIBLE) DebugItem("Item Visible:", IsItemVisible(_state, 1) ? ts : fs);
		if (applicableState & MV_STATE_HOVER) DebugItem("Item Hovered:", IsItemHovered(_state, 1) ? ts : fs);
		if (applicableState & MV_STATE_ACTIVE) DebugItem("Item Active:", IsItemActive(_state, 1) ? ts : fs);
		if (applicableState & MV_STATE_FOCUSED) DebugItem("Item Focused:", IsItemFocused(_state, 1) ? ts : fs);
		if (applicableState & MV_STATE_CLICKED)
		{
			DebugItem("Item Left Clicked:", IsItemLeftClicked(_state, 1) ? ts : fs);
			DebugItem("Item Right Clicked:", IsItemRightClicked(_state, 1) ? ts : fs);
			DebugItem("Item Middle Clicked:", IsItemMiddleClicked(_state, 1) ? ts : fs);
		}
		if (applicableState & MV_STATE_EDITED) DebugItem("Item Edited:", IsItemEdited(_state, 1) ? ts : fs);
		if (applicableState & MV_STATE_ACTIVATED) DebugItem("Item Activated:", IsItemActivated(_state, 1) ? ts : fs);
		if (applicableState & MV_STATE_DEACTIVATED) DebugItem("Item Deactivated:", IsItemDeactivated(_state, 1) ? ts : fs);
		if (applicableState & MV_STATE_DEACTIVATEDAE) DebugItem("Item DeactivatedAfterEdit:", IsItemDeactivatedAfterEdit(_state, 1) ? ts : fs);
		if (applicableState & MV_STATE_TOGGLED_OPEN) DebugItem("Item ToggledOpen:", IsItemToogledOpen(_state, 1) ? ts : fs);

		ImGui::PopID();

		renderSpecificDebugInfo();
	}

	void mvAppItem::renderDebugWindow()
	{

		ImGui::SetNextWindowSize(ImVec2(500.0f, 500.0f), ImGuiCond_FirstUseEver);

		if (!ImGui::Begin(_specifiedLabel.c_str(), &_showDebug))
		{
			ImGui::End();

			if (!_showDebug)
				RemoveDebugWindow(*mvApp::GetApp()->itemRegistry, _uuid);
			return;
		}

		renderDebugInfo();

		ImGui::End();

		if (!_showDebug)
			RemoveDebugWindow(*mvApp::GetApp()->itemRegistry, _uuid);
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
		setWidth(item->_width);
		setHeight(item->_height);
		//setPos(item->_state.pos);

		if (!item->_specifiedLabel.empty())
		{
			_specifiedLabel = item->_specifiedLabel;
			if (_useInternalLabel)
				_internalLabel = item->_specifiedLabel + "###" + std::to_string(_uuid);
			else
				_internalLabel = item->_specifiedLabel;
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
		_state.pos = {pos.x, pos.y };
	}

	void mvAppItem::registerWindowFocusing()
	{
		if (ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows))
		{

			// update mouse
			ImVec2 mousePos = ImGui::GetMousePos();
			float x = mousePos.x - ImGui::GetWindowPos().x;
			float y = mousePos.y - ImGui::GetWindowPos().y;
			mvInput::setMousePosition(x, y);

			if (mvApp::GetApp()->itemRegistry->activeWindow != _uuid)
			{
				mvApp::GetApp()->itemRegistry->activeWindow = _uuid;
				mvEventBus::Publish(mvEVT_CATEGORY_ITEM, mvEVT_ACTIVE_WINDOW, { CreateEventArgument("WINDOW", _uuid) });
			}

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
		ResetAppItemState(_state);
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
		else if (slot < 4)
		{
			_children[slot].clear();
			_children[slot].shrink_to_fit();
		}
		onChildrenRemoved();
	}

	void mvAppItem::setLabel(const std::string& value)
	{
		_specifiedLabel = value;
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
				DelaySearch(*mvApp::GetApp()->itemRegistry, this);
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
			CleanUpItem(*mvApp::GetApp()->itemRegistry, _uuid);
			RemoveAlias(*mvApp::GetApp()->itemRegistry, _alias, true);
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
		VerifyArgumentCount(mvApp::GetApp()->getParsers()[parser], args);
	}

	void mvAppItem::handleKeywordArgs(PyObject* dict, std::string parser)
	{
		if (dict == nullptr)
			return;

		if (VerifyKeywordArguments(mvApp::GetApp()->getParsers()[parser], dict))
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
				setDataSource(GetIdFromAlias(*mvApp::GetApp()->itemRegistry, alias));
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
			RemoveAlias(*mvApp::GetApp()->itemRegistry, _alias, true);
			
		_alias = value;

		// only register non-empty aliases
		if (!_alias.empty())
			AddAlias(*mvApp::GetApp()->itemRegistry, _alias, _uuid);
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
					parent = GetIdFromAlias(*mvApp::GetApp()->itemRegistry, ToString(item));
				else
					parent = ToUUID(item);
			}
			
			if (PyObject* item = PyDict_GetItemString(kwargs, "before"))
			{
				if (PyUnicode_Check(item))
					before = GetIdFromAlias(*mvApp::GetApp()->itemRegistry, ToString(item));
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

		auto children = GetItemChildren(*mvApp::GetApp()->itemRegistry, _uuid);
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

		PyDict_SetItemString(dict, "type", mvPyObject(ToPyString(getTypeString())));
		PyDict_SetItemString(dict, "target", mvPyObject(ToPyInt(getTarget())));

		if (_parentPtr)
			PyDict_SetItemString(dict, "parent", mvPyObject(ToPyUUID(_parentPtr->_uuid)));
		else
			PyDict_SetItemString(dict, "parent", mvPyObject(GetPyNone()));

		if (_theme)
			PyDict_SetItemString(dict, "theme", mvPyObject(ToPyUUID(_theme->_uuid)));
		else
			PyDict_SetItemString(dict, "theme", mvPyObject(GetPyNone()));

		if (_font)
			PyDict_SetItemString(dict, "font", mvPyObject(ToPyUUID(_font->_uuid)));
		else
			PyDict_SetItemString(dict, "font", mvPyObject(GetPyNone()));

		if(getDescFlags() & MV_ITEM_DESC_CONTAINER)
			PyDict_SetItemString(dict, "container", mvPyObject(ToPyBool(true)));
		else
			PyDict_SetItemString(dict, "container", mvPyObject(ToPyBool(false)));

		int applicableState = getApplicableState();
		PyDict_SetItemString(dict, "hover_handler_applicable", mvPyObject(ToPyBool(applicableState & MV_STATE_HOVER)));
		PyDict_SetItemString(dict, "active_handler_applicable", mvPyObject(ToPyBool(applicableState & MV_STATE_ACTIVE)));
		PyDict_SetItemString(dict, "focus_handler_applicable", mvPyObject(ToPyBool(applicableState & MV_STATE_FOCUSED)));
		PyDict_SetItemString(dict, "clicked_handler_applicable", mvPyObject(ToPyBool(applicableState & MV_STATE_CLICKED)));
		PyDict_SetItemString(dict, "visible_handler_applicable", mvPyObject(ToPyBool(applicableState & MV_STATE_VISIBLE)));
		PyDict_SetItemString(dict, "edited_handler_applicable", mvPyObject(ToPyBool(applicableState & MV_STATE_EDITED)));
		PyDict_SetItemString(dict, "activated_handler_applicable", mvPyObject(ToPyBool(applicableState & MV_STATE_ACTIVATED)));
		PyDict_SetItemString(dict, "deactivated_handler_applicable", mvPyObject(ToPyBool(applicableState & MV_STATE_DEACTIVATED)));
		PyDict_SetItemString(dict, "deactivatedae_handler_applicable", mvPyObject(ToPyBool(applicableState & MV_STATE_DEACTIVATEDAE)));
		PyDict_SetItemString(dict, "toggled_open_handler_applicable", mvPyObject(ToPyBool(applicableState & MV_STATE_TOGGLED_OPEN)));
		PyDict_SetItemString(dict, "resized_handler_applicable", mvPyObject(ToPyBool(applicableState & MV_STATE_RESIZED)));

	}

	void mvAppItem::getConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;

		// config py objects
		mvPyObject py_filter_key = ToPyString(_filter);
		mvPyObject py_payload_type = ToPyString(_payloadType);
		mvPyObject py_label = ToPyString(_specifiedLabel);
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

		if (!Parse((mvApp::GetApp()->getParsers())["get_item_configuration"], args, kwargs, __FUNCTION__, &itemraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);
		auto appitem = GetItem((*mvApp::GetApp()->itemRegistry), item);

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

		if (!Parse((mvApp::GetApp()->getParsers())["focus_item"], args, kwargs, __FUNCTION__, &itemraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);
		FocusItem(*mvApp::GetApp()->itemRegistry, item);

		return GetPyNone();
	}

	PyObject* mvAppItem::set_item_children(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;
		PyObject* sourceraw;
		int slot;

		if (!Parse((mvApp::GetApp()->getParsers())["set_item_children"], args, kwargs, __FUNCTION__,
			&itemraw, &sourceraw, &slot))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);
		mvUUID source = mvAppItem::GetIDFromPyObject(sourceraw);
		auto appitem = GetItem((*mvApp::GetApp()->itemRegistry), item);
		
		auto& staging = mvApp::GetApp()->itemRegistry->stagingRoots;

		bool stage_found = false;
		mvRef<mvAppItem> staging_container = nullptr;

		for (auto& stage : staging)
		{
			if(stage->_uuid == source)
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

		DeleteItem(*mvApp::GetApp()->itemRegistry, source);

		return GetPyNone();
	}

	PyObject* mvAppItem::bind_item_font(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;
		PyObject* fontraw;

		if (!Parse((mvApp::GetApp()->getParsers())["bind_item_font"], args, kwargs, __FUNCTION__,
			&itemraw, &fontraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);
		mvUUID font = mvAppItem::GetIDFromPyObject(fontraw);
		auto appitem = GetItem((*mvApp::GetApp()->itemRegistry), item);
		auto appfont = GetRefItem(*mvApp::GetApp()->itemRegistry, font);

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
				appfont->onBind(appitem);
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

		if (!Parse((mvApp::GetApp()->getParsers())["bind_item_theme"], args, kwargs, __FUNCTION__,
			&itemraw, &themeraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);
		mvUUID theme = mvAppItem::GetIDFromPyObject(themeraw);
		auto appitem = GetItem((*mvApp::GetApp()->itemRegistry), item);

		if (appitem)
		{
			if (theme == 0)
			{
				appitem->_theme = nullptr;
				return GetPyNone();
			}

			auto apptheme = GetRefItem(*mvApp::GetApp()->itemRegistry, theme);

			if (apptheme)
			{
				appitem->_theme = apptheme;
				apptheme->onBind(appitem);
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

	PyObject* mvAppItem::bind_item_handler_registry(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;
		PyObject* regraw;

		if (!Parse((mvApp::GetApp()->getParsers())["bind_item_handler_registry"], args, kwargs, __FUNCTION__,
			&itemraw, &regraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);
		mvUUID reg = mvAppItem::GetIDFromPyObject(regraw);
		auto appitem = GetItem((*mvApp::GetApp()->itemRegistry), item);

		if (appitem)
		{
			if (reg == 0)
			{
				appitem->_handlerRegistry = nullptr;
				return GetPyNone();
			}

			auto apptheme = GetRefItem(*mvApp::GetApp()->itemRegistry, reg);

			if (apptheme)
			{
				appitem->_handlerRegistry = apptheme;
				apptheme->onBind(appitem);
				return GetPyNone();
			}
			else
				mvThrowPythonError(mvErrorCode::mvItemNotFound, "bind_item_handler_registry",
					"Theme item not found: " + std::to_string(item), nullptr);
		}
		else
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "bind_item_handler_registry",
				"Item not found: " + std::to_string(item), nullptr);

		return GetPyNone();
	}

	PyObject* mvAppItem::reset_pos(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;

		if (!Parse((mvApp::GetApp()->getParsers())["reset_pos"], args, kwargs, __FUNCTION__,
			&itemraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);
		auto appitem = GetItem((*mvApp::GetApp()->itemRegistry), item);

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

		if (!Parse((mvApp::GetApp()->getParsers())["get_item_state"], args, kwargs, __FUNCTION__, &itemraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);
		auto appitem = GetItem((*mvApp::GetApp()->itemRegistry), item);

		PyObject* pdict = PyDict_New();

		if (appitem)
			FillAppItemState(pdict, appitem->_state, appitem->getApplicableState());
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

		if (!Parse((mvApp::GetApp()->getParsers())["get_item_info"], args, kwargs, __FUNCTION__, &itemraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);
		auto appitem = GetItem((*mvApp::GetApp()->itemRegistry), item);

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
		auto appitem = GetItem((*mvApp::GetApp()->itemRegistry), item);

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

		if (!Parse((mvApp::GetApp()->getParsers())["get_value"], args, kwargs, __FUNCTION__, &nameraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID name = mvAppItem::GetIDFromPyObject(nameraw);
		auto item = GetItem(*mvApp::GetApp()->itemRegistry, name);
		if (item)
			return item->getPyValue();

		return GetPyNone();
	}

	PyObject* mvAppItem::get_values(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* items;

		if (!Parse((mvApp::GetApp()->getParsers())["get_values"], args, kwargs, __FUNCTION__, &items))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		auto aitems = ToUUIDVect(items);
		PyObject* pyvalues = PyList_New(aitems.size());

		for (size_t i = 0; i < aitems.size(); i++)
		{
			auto item = GetItem(*mvApp::GetApp()->itemRegistry, aitems[i]);
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

		if (!Parse((mvApp::GetApp()->getParsers())["set_value"], args, kwargs, __FUNCTION__, &nameraw, &value))
			return GetPyNone();

		if (value)
			Py_XINCREF(value);

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID name = mvAppItem::GetIDFromPyObject(nameraw);

		auto item = GetItem(*mvApp::GetApp()->itemRegistry, name);
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

	PyObject* mvAppItem::set_item_alias(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;
		const char* alias;

		if (!Parse((mvApp::GetApp()->getParsers())["set_item_alias"], args, kwargs, __FUNCTION__,
			&itemraw, &alias))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);
		auto appitem = GetItem((*mvApp::GetApp()->itemRegistry), item);
		if (appitem)
			appitem->_alias = alias;
		return GetPyNone();
	}

	PyObject* mvAppItem::get_item_alias(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;

		if (!Parse((mvApp::GetApp()->getParsers())["get_item_alias"], args, kwargs, __FUNCTION__,
			&itemraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);
		auto appitem = GetItem((*mvApp::GetApp()->itemRegistry), item);
		if (appitem)
			return ToPyString(appitem->_alias);
		return GetPyNone();
	}
}
