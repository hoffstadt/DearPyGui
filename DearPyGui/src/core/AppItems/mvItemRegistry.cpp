#include "mvItemRegistry.h"
#include "mvProfiler.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvAppItemCommons.h"
#include "mvLog.h"
#include "mvPythonExceptions.h"
#include "mvToolManager.h"
#include "mvPythonExceptions.h"
#include "mvToolManager.h"
#include "mvFontManager.h"

namespace Marvel {

	void mvItemRegistry::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::String, "alias" });
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Adds an alias.";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::None;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "add_alias", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::String, "alias" });

			mvPythonParserSetup setup;
			setup.about = "Removes an alias.";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::None;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "remove_alias", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::String, "alias" });

			mvPythonParserSetup setup;
			setup.about = "Checks if an alias exist.";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::Bool;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "does_alias_exist", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::String, "alias" });

			mvPythonParserSetup setup;
			setup.about = "Returns the ID associated with an alias.";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::UUID;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "get_alias_id", parser });
		}

		{

			mvPythonParserSetup setup;
			setup.about = "Pops the top item off the parent stack and return its ID.";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::UUID;

			mvPythonParser parser = FinalizeParser(setup, {});
			parsers->insert({ "pop_container_stack", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Shows the imgui demo.";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::None;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "show_imgui_demo", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Shows the implot demo.";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::None;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "show_implot_demo", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "container" });
			args.push_back({ mvPyDataType::Integer, "slot" });
			args.push_back({ mvPyDataType::UUIDList, "new_order" });

			mvPythonParserSetup setup;
			setup.about = "Reorders an item's children.";
			setup.category = { "App Item Operations" };
			setup.returnType = mvPyDataType::None;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "reorder_items", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "template_registry" });

			mvPythonParserSetup setup;
			setup.about = "Binds a global template registry.";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::None;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "bind_template_registry", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Unstages an item.";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::None;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "unstage", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Shows an item's debug window";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::None;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "show_item_debug", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Pushes an item onto the container stack.";
			setup.category = { "Item Registry"};
			setup.returnType = mvPyDataType::Bool;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "push_container_stack", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Returns the item on the top of the container stack.";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::UUID;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "top_container_stack", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Returns the last item added.";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::UUID;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "last_item", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Returns the last container item added.";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::UUID;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "last_container", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Returns the last root added (registry or window).";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::UUID;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "last_root", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Emptyes the container stack.";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::None;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "empty_container_stack", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });
			args.push_back({ mvPyDataType::UUID, "parent", mvArgType::KEYWORD_ARG, "0"});
			args.push_back({ mvPyDataType::UUID, "before", mvArgType::KEYWORD_ARG, "0"});

			mvPythonParserSetup setup;
			setup.about = "Moves an item to a new location.";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::None;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "move_item", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Returns all windows.";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::UUIDList;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "get_windows", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Returns all items.";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::UUIDList;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "get_all_items", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Returns all aliases.";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::StringList;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "get_aliases", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });
			args.push_back({ mvPyDataType::Bool, "children_only", mvArgType::KEYWORD_ARG, "False" });
			args.push_back({ mvPyDataType::Integer, "slot", mvArgType::KEYWORD_ARG, "-1" });

			mvPythonParserSetup setup;
			setup.about = "Deletes an item..";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::None;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "delete_item", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Checks if an item exist..";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::Bool;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "does_item_exist", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Moves an item up.";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::None;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "move_item_up", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Moves an item down.";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::None;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "move_item_down", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Returns the active window.";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::UUID;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "get_active_window", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "window" });
			args.push_back({ mvPyDataType::Bool, "value" });

			mvPythonParserSetup setup;
			setup.about = "Sets the primary window.";
			setup.category = { "Item Registry" };
			setup.returnType = mvPyDataType::None;

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "set_primary_window", parser });
		}

	}

	mvItemRegistry::mvItemRegistry()
	{
		mvEventBus::Subscribe(this, 0, mvEVT_CATEGORY_ITEM);
		mvEventBus::Subscribe(this, mvEVT_RENDER);
		mvEventBus::Subscribe(this, mvEVT_ACTIVE_WINDOW);

		// prefill cached containers
		for (int i = 0; i < CachedContainerCount; i++)
		{
			_cachedContainersID[i] = 0;
			_cachedContainersPTR[i] = nullptr;
			_cachedItemsID[i] = 0;
			_cachedItemsPTR[i] = nullptr;
		}
		
		_valueRegistryRoots.reserve(10 * 100000);
	}

	mvRef<mvAppItem> mvItemRegistry::getItemFromPool(mvAppItemType itemType)
	{
		for (auto& pool : _itemPoolRoots)
		{
			auto item = static_cast<mvItemPool*>(pool.get())->getItem(itemType);
			if (item)
				return item;
		}

		return nullptr;
	}

	bool mvItemRegistry::focusItem(mvUUID uuid)
	{

		if (!mvApp::IsAppStarted())
		{
			for (size_t i = 0; i < _windowRoots.size(); i++)
			{
				if (_windowRoots[i]->_uuid == uuid)
				{
					mvRef<mvAppItem> oldItem = _windowRoots.back();
					_windowRoots[_windowRoots.size() - 1] = _windowRoots[i];
					_windowRoots[i] = oldItem;
					return true;
				}
			}
		}

		auto appitem = getItem(uuid);

		if (appitem)
		{
			appitem->focus();
			if (auto parent = appitem->getRoot())
				parent->focus();
			return true;
		}
		else
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "focus_item",
				"Item not found: " + std::to_string(uuid), nullptr);
		return false;
	}

	bool mvItemRegistry::addRuntimeChildRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID parent, mvUUID before, mvRef<mvAppItem> item)
	{
		for (auto& root : roots)
		{
			if (root->addRuntimeChild(parent, before, item))
				return true;
		}
		return false;

	}

	bool mvItemRegistry::deleteRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID uuid)
	{
		bool deletedItem = false;

		// try to delete build-in item
		for (auto& root : roots)
		{
			deletedItem = root->deleteChild(uuid);
			if (deletedItem)
				break;
		}

		if (deletedItem)
			return true;

		bool rootDeleting = false;

		// check if attempting to delete a window
		for (auto& window : roots)
		{
			if (window->_uuid == uuid)
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
			std::vector<mvRef<mvAppItem>> oldroots = roots;

			roots.clear();

			for (auto& root : oldroots)
			{
				if (root->_uuid == uuid)
				{
					deletedItem = true;
					continue;
				}
				roots.push_back(root);
			}

			return true;
		}

		return false;
	}

	bool mvItemRegistry::moveRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID uuid, mvRef<mvAppItem>& item)
	{
		for (auto& window : roots)
		{
			item = window->stealChild(uuid);
			if (item)
				return true;
		}
		return false;
	}

	bool mvItemRegistry::moveUpRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID uuid)
	{
		for (auto& window : roots)
		{
			if (window->moveChildUp(uuid))
				return true;
		}
		return false;
	}
	
	bool mvItemRegistry::moveDownRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID uuid)
	{
		for (auto& window : roots)
		{
			if (window->moveChildDown(uuid))
				return true;
		}
		return false;
	}

	bool mvItemRegistry::deleteItem(mvUUID uuid, bool childrenOnly, int slot)
	{

		MV_ITEM_REGISTRY_TRACE("Attempting to delete: " + std::to_string(uuid));

		cleanUpItem(uuid);

		// delete item's children only
		if(childrenOnly)
		{
			auto item = getItem(uuid);
			if (item)
			{
				item->deleteChildren(slot);
				MV_ITEM_REGISTRY_INFO("Item found and it's children deleted.");
				return true;
			}
		}

		bool deletedItem = false;

		if (deleteRoot(_colormapRoots, uuid)) deletedItem = true;
		else if (deleteRoot(_filedialogRoots, uuid)) deletedItem = true;
		else if (deleteRoot(_stagingRoots, uuid)) deletedItem = true;
		else if (deleteRoot(_viewportMenubarRoots, uuid)) deletedItem = true;
		else if (deleteRoot(_fontRegistryRoots, uuid)) deletedItem = true;
		else if (deleteRoot(_handlerRegistryRoots, uuid)) deletedItem = true;
		else if (deleteRoot(_textureRegistryRoots, uuid)) deletedItem = true;
		else if (deleteRoot(_valueRegistryRoots, uuid)) deletedItem = true;
		else if (deleteRoot(_windowRoots, uuid)) deletedItem = true;
		else if (deleteRoot(_themeRegistryRoots, uuid)) deletedItem = true;
		else if (deleteRoot(_itemPoolRoots, uuid)) deletedItem = true;
		else if (deleteRoot(_itemTemplatesRoots, uuid)) deletedItem = true;
		else if (deleteRoot(_itemHandlerRegistryRoots, uuid)) deletedItem = true;

		if (deletedItem)
		{
			MV_ITEM_REGISTRY_INFO(std::to_string(uuid) + " found and deleted.");
			removeDebugWindow(uuid);
		}
		else
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "delete_item",
				"Item not found: " + std::to_string(uuid), nullptr);

		assert(deletedItem && "Item to delete not found");
		return deletedItem;
	}

	bool mvItemRegistry::moveItem(mvUUID uuid, mvUUID parent, mvUUID before)
	{

		MV_ITEM_REGISTRY_TRACE("Attempting to move: " + std::to_string(uuid));

		mvRef<mvAppItem> child = nullptr;

		bool movedItem = false;

		if (moveRoot(_colormapRoots, uuid, child)) movedItem = true;
		else if (moveRoot(_filedialogRoots, uuid, child)) movedItem = true;
		else if (moveRoot(_stagingRoots, uuid, child)) movedItem = true;
		else if (moveRoot(_viewportMenubarRoots, uuid, child)) movedItem = true;
		else if (moveRoot(_fontRegistryRoots, uuid, child)) movedItem = true;
		else if (moveRoot(_handlerRegistryRoots, uuid, child)) movedItem = true;
		else if (moveRoot(_textureRegistryRoots, uuid, child)) movedItem = true;
		else if (moveRoot(_valueRegistryRoots, uuid, child)) movedItem = true;
		else if (moveRoot(_windowRoots, uuid, child)) movedItem = true;
		else if (moveRoot(_themeRegistryRoots, uuid, child)) movedItem = true;
		else if (moveRoot(_itemPoolRoots, uuid, child)) movedItem = true;
		else if (moveRoot(_itemTemplatesRoots, uuid, child)) movedItem = true;
		else if (moveRoot(_itemHandlerRegistryRoots, uuid, child)) movedItem = true;

		if (child == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "move_item",
				"Item not found: " + std::to_string(uuid), nullptr);
			MV_ITEM_REGISTRY_WARN("Could not move item, it was not found");
		}

		if (child)
			addRuntimeItem(parent, before, child);

		return movedItem;
	}

	bool mvItemRegistry::moveItemUp(mvUUID uuid)
	{

		MV_ITEM_REGISTRY_TRACE("Attempting to move: " + std::to_string(uuid));

		bool movedItem = false;

		if (moveUpRoot(_colormapRoots, uuid)) movedItem = true;
		else if (moveUpRoot(_filedialogRoots, uuid)) movedItem = true;
		else if (moveUpRoot(_stagingRoots, uuid)) movedItem = true;
		else if (moveUpRoot(_viewportMenubarRoots, uuid)) movedItem = true;
		else if (moveUpRoot(_fontRegistryRoots, uuid)) movedItem = true;
		else if (moveUpRoot(_handlerRegistryRoots, uuid)) movedItem = true;
		else if (moveUpRoot(_textureRegistryRoots, uuid)) movedItem = true;
		else if (moveUpRoot(_valueRegistryRoots, uuid)) movedItem = true;
		else if (moveUpRoot(_windowRoots, uuid)) movedItem = true;
		else if (moveUpRoot(_themeRegistryRoots, uuid)) movedItem = true;
		else if (moveUpRoot(_itemPoolRoots, uuid)) movedItem = true;
		else if (moveUpRoot(_itemTemplatesRoots, uuid)) movedItem = true;
		else if (moveUpRoot(_itemHandlerRegistryRoots, uuid)) movedItem = true;

		if (!movedItem)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "move_item",
				"Item not found: " + std::to_string(uuid), nullptr);
			MV_ITEM_REGISTRY_WARN("Could not move item, it was not found");
		}

		assert(movedItem && "Item to move not found");

		return movedItem;
	}

	bool mvItemRegistry::moveItemDown(mvUUID uuid)
	{

		MV_ITEM_REGISTRY_TRACE("Attempting to move: " + std::to_string(uuid));

		bool movedItem = false;

		if (moveDownRoot(_colormapRoots, uuid)) movedItem = true;
		else if (moveDownRoot(_filedialogRoots, uuid)) movedItem = true;
		else if (moveDownRoot(_stagingRoots, uuid)) movedItem = true;
		else if (moveDownRoot(_viewportMenubarRoots, uuid)) movedItem = true;
		else if (moveDownRoot(_fontRegistryRoots, uuid)) movedItem = true;
		else if (moveDownRoot(_handlerRegistryRoots, uuid)) movedItem = true;
		else if (moveDownRoot(_textureRegistryRoots, uuid)) movedItem = true;
		else if (moveDownRoot(_valueRegistryRoots, uuid)) movedItem = true;
		else if (moveDownRoot(_windowRoots, uuid)) movedItem = true;
		else if (moveDownRoot(_themeRegistryRoots, uuid)) movedItem = true;
		else if (moveDownRoot(_itemPoolRoots, uuid)) movedItem = true;
		else if (moveDownRoot(_itemTemplatesRoots, uuid)) movedItem = true;
		else if (moveDownRoot(_itemHandlerRegistryRoots, uuid)) movedItem = true;

		if (!movedItem)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "move_item",
				"Item not found: " + std::to_string(uuid), nullptr);
			MV_ITEM_REGISTRY_WARN("Could not move item, it was not found");
		}

		assert(movedItem && "Item to move not found");

		return movedItem;
	}

	bool mvItemRegistry::onEvent(mvEvent& event)
	{
		mvEventDispatcher dispatcher(event);

		dispatcher.dispatch(BIND_EVENT_METH(mvItemRegistry::onRender), mvEVT_RENDER);
		dispatcher.dispatch(BIND_EVENT_METH(mvItemRegistry::onActiveWindow), mvEVT_ACTIVE_WINDOW);

		return event.handled;
	}

	bool mvItemRegistry::onRender(mvEvent& event)
	{

		MV_PROFILE_SCOPE("Rendering")

		if(_showImGuiDebug)
			ImGui::ShowDemoWindow(&_showImGuiDebug);
		if(_showImPlotDebug)
			ImPlot::ShowDemoWindow(&_showImPlotDebug);

		if (mvToolManager::GetFontManager()._resetDefault)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.FontDefault = nullptr;
			mvToolManager::GetFontManager()._resetDefault = false;
		}

		for (auto& root : _fontRegistryRoots)
		{
			if (root->_show)
				root->draw(nullptr, 0.0f, 0.0f);
		}

		if (mvToolManager::GetFontManager()._newDefault)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.FontDefault = nullptr;

			for (auto& root : _fontRegistryRoots)
			{
				for (auto& font : root->_children[1])
				{
					if (static_cast<mvFont*>(font.get())->_default)
					{
						io.FontDefault = static_cast<mvFont*>(font.get())->getFontPtr();
						break;
					}
				}
			}

			mvToolManager::GetFontManager()._newDefault = false;
		}

		for (auto& root : _handlerRegistryRoots)
		{
			if (root->_show)
				root->draw(nullptr, 0.0f, 0.0f);
		}

		for (auto& root : _textureRegistryRoots)
			root->draw(nullptr, 0.0f, 0.0f);

		for (auto& root : _filedialogRoots)
			root->draw(nullptr, 0.0f, 0.0f);

		for (auto& root : _themeRegistryRoots)
		{
			if(root->_show)
				root->draw(nullptr, 0.0f, 0.0f);
		}

		for (auto& root : _windowRoots)
			root->draw(nullptr, 0.0f, 0.0f);

		for (auto& root : _colormapRoots)
		{
			if (root->isAltCustomActionRequested())
				root->alternativeCustomAction();
			root->draw(nullptr, 0.0f, 0.0f);
		}

		for (auto& root : _viewportMenubarRoots)
			root->draw(nullptr, 0.0f, 0.0f);

		for (auto& root : _themeRegistryRoots)
		{
			if (root->_show)
				root->customAction();
		}

		for (auto& root : _debugWindows)
			root->renderDebugWindow();

		return false;
	}

	void mvItemRegistry::resetTheme()
	{
		for (auto& root : _themeRegistryRoots)
			root->_show = false;
	}

	bool mvItemRegistry::onActiveWindow(mvEvent& event)
	{

		_activeWindow = GetEUUID(event, "WINDOW");

		MV_ITEM_REGISTRY_INFO("Active window changed to: " + std::to_string(_activeWindow));

		return false;
	}

	bool mvItemRegistry::addItemAfterRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID prev, mvRef<mvAppItem> item)
	{
		for (auto& root : roots)
		{
			if (root->addChildAfter(prev, item))
				return true;
		}

		return false;
	}

	bool mvItemRegistry::addRuntimeItem(mvUUID parent, mvUUID before, mvRef<mvAppItem> item)
	{

		if (addRuntimeChildRoot(_colormapRoots, parent, before, item)) return true;
		else if (addRuntimeChildRoot(_filedialogRoots, parent, before, item)) return true;
		else if (addRuntimeChildRoot(_stagingRoots, parent, before, item)) return true;
		else if (addRuntimeChildRoot(_viewportMenubarRoots, parent, before, item)) return true;
		else if (addRuntimeChildRoot(_fontRegistryRoots, parent, before, item)) return true;
		else if (addRuntimeChildRoot(_handlerRegistryRoots, parent, before, item)) return true;
		else if (addRuntimeChildRoot(_textureRegistryRoots, parent, before, item)) return true;
		else if (addRuntimeChildRoot(_valueRegistryRoots, parent, before, item)) return true;
		else if (addRuntimeChildRoot(_windowRoots, parent, before, item)) return true;
		else if (addRuntimeChildRoot(_themeRegistryRoots, parent, before, item)) return true;
		else if (addRuntimeChildRoot(_itemPoolRoots, parent, before, item)) return true;
		else if (addRuntimeChildRoot(_itemTemplatesRoots, parent, before, item)) return true;
		else if (addRuntimeChildRoot(_itemHandlerRegistryRoots, parent, before, item)) return true;

		return false;
	}

	bool mvItemRegistry::addItemAfter(mvUUID prev, mvRef<mvAppItem> item)
	{

		if (addItemAfterRoot(_colormapRoots, prev, item)) return true;
		else if (addItemAfterRoot(_filedialogRoots, prev, item)) return true;
		else if (addItemAfterRoot(_stagingRoots, prev, item)) return true;
		else if (addItemAfterRoot(_viewportMenubarRoots, prev, item)) return true;
		else if (addItemAfterRoot(_fontRegistryRoots, prev, item)) return true;
		else if (addItemAfterRoot(_handlerRegistryRoots, prev, item)) return true;
		else if (addItemAfterRoot(_textureRegistryRoots, prev, item)) return true;
		else if (addItemAfterRoot(_valueRegistryRoots, prev, item)) return true;
		else if (addItemAfterRoot(_windowRoots, prev, item)) return true;
		else if (addItemAfterRoot(_themeRegistryRoots, prev, item)) return true;
		else if (addItemAfterRoot(_itemPoolRoots, prev, item)) return true;
		else if (addItemAfterRoot(_itemTemplatesRoots, prev, item)) return true;
		else if (addItemAfterRoot(_itemHandlerRegistryRoots, prev, item)) return true;

		assert(false);
		return false;
	}

	void mvItemRegistry::pushParent(mvAppItem* item)
	{
		_containers.push(item);
	}

	mvAppItem* mvItemRegistry::popParent()
	{
		if (_containers.empty())
		{
			mvThrowPythonError(mvErrorCode::mvContainerStackEmpty, "No container to pop.");
			MV_ITEM_REGISTRY_WARN("No container to pop.");
			assert(false);
			return nullptr;
		}

		auto item = _containers.top();
		_containers.pop();
		return item;
	}

	void mvItemRegistry::emptyParents()
	{
		while (!_containers.empty())
			_containers.pop();

		MV_ITEM_REGISTRY_INFO("Container stack emptied.");
	}

	mvAppItem* mvItemRegistry::topParent()
	{
		if (!_containers.empty())
			return _containers.top();
		return nullptr;
	}

	void mvItemRegistry::cacheItem(mvAppItem* item)
	{
		if (mvAppItem::DoesItemHaveFlag(item, MV_ITEM_DESC_CONTAINER))
		{
		_cachedContainersID[_cachedContainerIndex] = item->_uuid;
		_cachedContainersPTR[_cachedContainerIndex] = item;
		_cachedContainerIndex++;
		if (_cachedContainerIndex == CachedContainerCount)
			_cachedContainerIndex = 0;
		}

		_cachedItemsID[_cachedItemsIndex] = item->_uuid;
		_cachedItemsPTR[_cachedItemsIndex] = item;
		_cachedItemsIndex++;
		if (_cachedItemsIndex == CachedContainerCount)
			_cachedItemsIndex = 0;
	}

	void mvItemRegistry::delaySearch(mvAppItem* item)
	{
		_delayedSearch.push_back(item);
	}

	mvAppItem* mvItemRegistry::getItemRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID uuid)
	{
		for (auto root : roots)
		{
			if (root->_uuid == uuid)
			{
				cacheItem(root.get());
				return root.get();
			}

			auto child = root->getChild(uuid);
			if (child)
			{
				cacheItem(child);
				_delayedSearch.clear();
				return child;
			}
		}

		return nullptr;
	}

	mvAppItem* mvItemRegistry::getItem(mvUUID uuid)
	{

		// check cache first
		for (int i = 0; i < CachedContainerCount; i++)
		{
			if (_cachedContainersID[i] == uuid)
				return _cachedContainersPTR[i];
			if (_cachedItemsID[i] == uuid)
				return _cachedItemsPTR[i];
		}

		if (auto foundItem = getItemRoot(_colormapRoots, uuid)) return foundItem;
		if (auto foundItem = getItemRoot(_colormapRoots, uuid)) return foundItem;
		if (auto foundItem = getItemRoot(_filedialogRoots, uuid)) return foundItem;
		if (auto foundItem = getItemRoot(_stagingRoots, uuid)) return foundItem;
		if (auto foundItem = getItemRoot(_viewportMenubarRoots, uuid)) return foundItem;
		if (auto foundItem = getItemRoot(_fontRegistryRoots, uuid)) return foundItem;
		if (auto foundItem = getItemRoot(_handlerRegistryRoots, uuid)) return foundItem;
		if (auto foundItem = getItemRoot(_textureRegistryRoots, uuid)) return foundItem;
		if (auto foundItem = getItemRoot(_valueRegistryRoots, uuid)) return foundItem;
		if (auto foundItem = getItemRoot(_windowRoots, uuid)) return foundItem;
		if (auto foundItem = getItemRoot(_themeRegistryRoots, uuid)) return foundItem;
		if (auto foundItem = getItemRoot(_itemPoolRoots, uuid)) return foundItem;
		if (auto foundItem = getItemRoot(_itemTemplatesRoots, uuid)) return foundItem;
		if (auto foundItem = getItemRoot(_itemHandlerRegistryRoots, uuid)) return foundItem;

		for (auto delayedItem : _delayedSearch)
		{
			auto child = delayedItem->getChild(uuid);
			if (child)
			{
				cacheItem(child);
				_delayedSearch.clear();
				return child;
			}
		}

		_delayedSearch.clear();

		return nullptr;
	}

	mvRef<mvAppItem> mvItemRegistry::getRefItemRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID uuid)
	{

		for (auto& root : roots)
		{
			if (root->_uuid == uuid)
				return root;

			auto child = root->getChildRef(uuid);
			if (child)
				return child;
		}

		return nullptr;
	}

	mvRef<mvAppItem> mvItemRegistry::getRefItem(mvUUID uuid)
	{

		if (auto foundItem = getRefItemRoot(_colormapRoots, uuid)) return foundItem;
		else if (auto foundItem = getRefItemRoot(_filedialogRoots, uuid)) return foundItem;
		else if (auto foundItem = getRefItemRoot(_stagingRoots, uuid)) return foundItem;
		else if (auto foundItem = getRefItemRoot(_viewportMenubarRoots, uuid)) return foundItem;
		else if (auto foundItem = getRefItemRoot(_fontRegistryRoots, uuid)) return foundItem;
		else if (auto foundItem = getRefItemRoot(_handlerRegistryRoots, uuid)) return foundItem;
		else if (auto foundItem = getRefItemRoot(_textureRegistryRoots, uuid)) return foundItem;
		else if (auto foundItem = getRefItemRoot(_valueRegistryRoots, uuid)) return foundItem;
		else if (auto foundItem = getRefItemRoot(_windowRoots, uuid)) return foundItem;
		else if (auto foundItem = getRefItemRoot(_themeRegistryRoots, uuid)) return foundItem;
		else if (auto foundItem = getRefItemRoot(_itemPoolRoots, uuid)) return foundItem;
		else if (auto foundItem = getRefItemRoot(_itemTemplatesRoots, uuid)) return foundItem;
		else if (auto foundItem = getRefItemRoot(_itemHandlerRegistryRoots, uuid)) return foundItem;

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

		//MV_ITEM_REGISTRY_TRACE("Adding item: " + item->_name);

		mvAppItem* parentitem = topParent();
		item->_parentPtr = parentitem;
		parentitem->addItem(item);

		return true;
	}

	bool mvItemRegistry::addRoot(mvRef<mvAppItem> item)
	{

		if (item->getType() == mvAppItemType::mvWindowAppItem) _windowRoots.push_back(item);
		if (item->getType() == mvAppItemType::mvColorMapRegistry) _colormapRoots.push_back(item);
		if (item->getType() == mvAppItemType::mvFileDialog) _filedialogRoots.push_back(item);
		if (item->getType() == mvAppItemType::mvStage) _stagingRoots.push_back(item);
		if (item->getType() == mvAppItemType::mvViewportMenuBar) _viewportMenubarRoots.push_back(item);
		if (item->getType() == mvAppItemType::mvFontRegistry) _fontRegistryRoots.push_back(item);
		if (item->getType() == mvAppItemType::mvHandlerRegistry) _handlerRegistryRoots.push_back(item);
		if (item->getType() == mvAppItemType::mvTextureRegistry) _textureRegistryRoots.push_back(item);
		if (item->getType() == mvAppItemType::mvValueRegistry) _valueRegistryRoots.push_back(item);
		if (item->getType() == mvAppItemType::mvTheme) _themeRegistryRoots.push_back(item);
		if (item->getType() == mvAppItemType::mvItemPool) _itemPoolRoots.push_back(item);
		if (item->getType() == mvAppItemType::mvTemplateRegistry) _itemTemplatesRoots.push_back(item);
		if (item->getType() == mvAppItemType::mvItemHandlerRegistry) _itemHandlerRegistryRoots.push_back(item);

		return true;
	}

	void mvItemRegistry::clearRegistry()
	{
		MV_ITEM_REGISTRY_INFO("Clearing item registry.");
		_colormapRoots.clear();
		_filedialogRoots.clear();
		_stagingRoots.clear();
		_viewportMenubarRoots.clear();
		_windowRoots.clear();
		_fontRegistryRoots.clear();
		_handlerRegistryRoots.clear();
		_textureRegistryRoots.clear();
		_valueRegistryRoots.clear();
		_themeRegistryRoots.clear();
		_itemPoolRoots.clear();
		_itemTemplatesRoots.clear();
		_itemHandlerRegistryRoots.clear();
	}

	void mvItemRegistry::cleanUpItem(mvUUID uuid)
	{
		for (int i = 0; i < CachedContainerCount; i++)
		{
			if (_cachedContainersID[i] == uuid)
			{
				_cachedContainersID[i] = 0;
				_cachedContainersPTR[i] = nullptr;
			}

			if (_cachedItemsID[i] == uuid)
			{
				_cachedItemsID[i] = 0;
				_cachedItemsPTR[i] = nullptr;
			}
		}
	}

	bool mvItemRegistry::addItemWithRuntimeChecks(mvRef<mvAppItem> item, mvUUID parent, mvUUID before)
	{
		//MV_ITEM_REGISTRY_TRACE("Adding runtime item: " + item->_name);

		if (mvAppItem::DoesItemHaveFlag(item.get(), MV_ITEM_DESC_HANDLER) && parent == 0)
			parent = item->_parent;

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
			_lastRootAdded = item->_uuid;
			_lastContainerAdded = item->_uuid;
		}
		else if (mvAppItem::DoesItemHaveFlag(item.get(), MV_ITEM_DESC_CONTAINER))
			_lastContainerAdded = item->_uuid;

		_lastItemAdded = item->_uuid;

		cacheItem(item.get());

		//---------------------------------------------------------------------------
		// STEP 1: check if an item with this name exists (NO LONGER NEEDED)
		//---------------------------------------------------------------------------
		//if (getItem(item->_uuid))
		//{
		//	mvThrowPythonEr1ror(mvErrorCode::mvNon1e, "Item must have a unique name.");
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

			if (mvApp::IsAppStarted())
			{
				addRoot(item);
				return true;
			}
			return addRoot(item);
		}
			
		//---------------------------------------------------------------------------
		// STEP 3: attempt to deduce parent
		//---------------------------------------------------------------------------
		mvAppItem* parentPtr = nullptr;
		if (before > 0)
		{

			auto beforeItem = getItem(before);
			if (beforeItem)
				parentPtr = beforeItem->_parentPtr;
			technique = AddTechnique::BEFORE;
		}

		else if (parent > MV_RESERVED_UUID_start + MV_RESERVED_UUIDs)
		{
			parentPtr = getItem(parent);
			technique = AddTechnique::PARENT;
		}

		else if (parent == 0)
		{
			parentPtr = topParent();
			technique = AddTechnique::STACK;
		}

		// reserved uuid case
		else
		{
			parentPtr = getItem(parent);
			if(parentPtr)
				technique = AddTechnique::PARENT;

			// revert to stack operation (reserved uuid not used)
			else
			{
				parentPtr = topParent();
				technique = AddTechnique::STACK;
			}
		}

		//---------------------------------------------------------------------------
		// STEP 4: check if parent was deduced
		//---------------------------------------------------------------------------
		if (parentPtr == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvParentNotDeduced, "add_*", "Parent could not be deduced.", item.get());
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
		// STEP 7: add items who require "after" adding (tooltip)
		//---------------------------------------------------------------------------
		if (item->getType() == mvAppItemType::mvTooltip)
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
			return parentPtr->addRuntimeChild(parentPtr->_uuid, 0, item);
		return addItem(item);
	}

	std::vector<std::vector<mvUUID>> mvItemRegistry::getItemChildren(mvUUID uuid)
	{

		mvAppItem* item = getItem(uuid);

		std::vector<std::vector<mvUUID>> childList;

		if (item)
		{
			for (auto& children : item->_children)
			{
				std::vector<mvUUID> childSlot;
				for (auto& child : children)
					childSlot.emplace_back(child->_uuid);
				childList.push_back(childSlot);
			}

		}
		else
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "get_item_children",
				"Item not found: " + std::to_string(uuid), nullptr);
			assert(false && "Item not found.");
		}

		return childList;
	}

	void mvItemRegistry::getAllItemsRoot(std::vector<mvRef<mvAppItem>>& roots, std::vector<mvUUID>& childList)
	{
		// to help recursively retrieve children
		std::function<void(mvRef<mvAppItem>)> ChildRetriever;
		ChildRetriever = [&childList, &ChildRetriever](mvRef<mvAppItem> item) {
			auto& children0 = item->_children[0];
			auto& children1 = item->_children[1];
			auto& children2 = item->_children[2];
			for (auto& child : children0)
			{
				childList.emplace_back(child->_uuid);
				if (mvAppItem::DoesItemHaveFlag(child.get(), MV_ITEM_DESC_CONTAINER))
					ChildRetriever(child);
			}
			for (auto& child : children1)
			{
				childList.emplace_back(child->_uuid);
				if (mvAppItem::DoesItemHaveFlag(child.get(), MV_ITEM_DESC_CONTAINER))
					ChildRetriever(child);
			}
			for (auto& child : children2)
			{
				childList.emplace_back(child->_uuid);
				if (mvAppItem::DoesItemHaveFlag(child.get(), MV_ITEM_DESC_CONTAINER))
					ChildRetriever(child);
			}

		};

		for (auto& root : roots)
		{
			childList.emplace_back(root->_uuid);
			ChildRetriever(root);
		}
	}

	std::vector<mvUUID> mvItemRegistry::getAllItems()
	{

		std::vector<mvUUID> childList;

		getAllItemsRoot(_colormapRoots, childList);
		getAllItemsRoot(_filedialogRoots, childList);
		getAllItemsRoot(_stagingRoots, childList);
		getAllItemsRoot(_viewportMenubarRoots, childList);
		getAllItemsRoot(_windowRoots, childList);
		getAllItemsRoot(_fontRegistryRoots, childList);
		getAllItemsRoot(_handlerRegistryRoots, childList);
		getAllItemsRoot(_textureRegistryRoots, childList);
		getAllItemsRoot(_valueRegistryRoots, childList);
		getAllItemsRoot(_themeRegistryRoots, childList);
		getAllItemsRoot(_itemPoolRoots, childList);
		getAllItemsRoot(_itemTemplatesRoots, childList);
		getAllItemsRoot(_itemHandlerRegistryRoots, childList);

		return childList;
	}

	std::vector<mvUUID> mvItemRegistry::getWindows()
	{

		std::vector<mvUUID> childList;
		for (auto& root : _colormapRoots) childList.emplace_back(root->_uuid);
		for (auto& root : _filedialogRoots) childList.emplace_back(root->_uuid);
		for (auto& root : _stagingRoots) childList.emplace_back(root->_uuid);
		for (auto& root : _viewportMenubarRoots) childList.emplace_back(root->_uuid);
		for (auto& root : _windowRoots) childList.emplace_back(root->_uuid);
		for (auto& root : _fontRegistryRoots) childList.emplace_back(root->_uuid);
		for (auto& root : _handlerRegistryRoots) childList.emplace_back(root->_uuid);
		for (auto& root : _textureRegistryRoots) childList.emplace_back(root->_uuid);
		for (auto& root : _valueRegistryRoots) childList.emplace_back(root->_uuid);
		for (auto& root : _themeRegistryRoots) childList.emplace_back(root->_uuid);
		for (auto& root : _itemPoolRoots) childList.emplace_back(root->_uuid);
		for (auto& root : _itemTemplatesRoots) childList.emplace_back(root->_uuid);
		for (auto& root : _itemHandlerRegistryRoots) childList.emplace_back(root->_uuid);

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
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "set_primary_window",
				"Item not found: " + std::to_string(uuid), nullptr);
			assert(false);
		}

		// reset other windows
		for (auto& window : _windowRoots)
		{
			if (window->_uuid != uuid)
				static_cast<mvWindowAppItem*>(window.get())->setWindowAsMainStatus(false);
		}

	}

	void mvItemRegistry::unstageItem(mvUUID uuid)
	{
		bool item_found = false;
		for (auto& item : _stagingRoots)
		{
			if (item->_uuid == uuid && item->getType() == mvAppItemType::mvStage)
			{
				for (auto& children : item->_children)
				{
					for (auto& child : children)
						addItemWithRuntimeChecks(child, 0, 0);
				}

				item_found = true;
				break;
			}
		}

		if (item_found)
		{
			cleanUpItem(uuid);
			return;
		}

		mvThrowPythonError(mvErrorCode::mvItemNotFound, "unstage",
			"Stage not found: " + std::to_string(uuid), nullptr);
		assert(false);

	}

	void mvItemRegistry::addAlias(const std::string& alias, mvUUID id)
	{
		if (!_allowAliasOverwrites)
		{
			if (doesAliasExist(alias))
			{
				mvThrowPythonError(mvErrorCode::mvNone, "add alias",
					"Alias already exists", nullptr);
				return;
			}
		}

		_aliases[alias] = id;

		auto item = getItem(id);
		if (item)
			item->_alias = alias;
	}

	void mvItemRegistry::removeAlias(const std::string& alias, bool itemTriggered)
	{

		if (alias.empty())
			return;

		if (!doesAliasExist(alias))
		{
			mvThrowPythonError(mvErrorCode::mvNone, "remove alias",
				"Alias does not exists", nullptr);
			return;
		}

		auto item = getItem(_aliases[alias]);
		if (item)
			item->_alias = "";

		if (itemTriggered)
		{
			if (!_manualAliasManagement)
				_aliases.erase(alias);
		}
		else
			_aliases.erase(alias);

	}

	bool mvItemRegistry::doesAliasExist(const std::string& alias)
	{
		if (_aliases.count(alias) != 0)
			return true;
		return false;
	}

	mvUUID mvItemRegistry::getIdFromAlias(const std::string& alias)
	{
		if (doesAliasExist(alias))
			return _aliases[alias];
		return 0;
	}

	void mvItemRegistry::tryBoundTemplateRegistry(mvAppItem* item) const
	{
		if (_boundedTemplateRegistry)
		{
			for (auto& tempItem : _boundedTemplateRegistry->_children[item->getTarget()])
			{
				if (tempItem->getType() == item->getType())
				{
					item->applyTemplate(tempItem.get());
					return;
				}
			}
		}
	}

	void mvItemRegistry::addDebugWindow(mvRef<mvAppItem> item)
	{
		_debugWindows.push_back(item);
	}

	void mvItemRegistry::removeDebugWindow(mvUUID uuid)
	{
		// check if debug window exists
		bool exists = false;
		for (const auto& debug : _debugWindows)
		{
			if (debug->_uuid == uuid)
			{
				exists = true;
				break;
			}
		}

		// not found
		if (!exists)
			return;

		std::vector<mvRef<mvAppItem>> oldWindows = _debugWindows;
		_debugWindows.clear();

		for (auto& debug : oldWindows)
		{
			if (debug->_uuid != uuid)
				_debugWindows.push_back(debug);
		}
	}

	PyObject* mvItemRegistry::pop_container_stack(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		auto item = mvApp::GetApp()->getItemRegistry().popParent();
		if (item)
			return ToPyUUID(item->_uuid);
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
			return ToPyUUID(item->_uuid);
		else
			return GetPyNone();
	}

	PyObject* mvItemRegistry::last_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		return ToPyUUID(mvApp::GetApp()->getItemRegistry()._lastItemAdded);
	}

	PyObject* mvItemRegistry::last_container(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		return ToPyUUID(mvApp::GetApp()->getItemRegistry()._lastContainerAdded);
	}

	PyObject* mvItemRegistry::last_root(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		return ToPyUUID(mvApp::GetApp()->getItemRegistry()._lastRootAdded);
	}

	PyObject* mvItemRegistry::push_container_stack(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;

		if (!Parse((mvApp::GetApp()->getParsers())["push_container_stack"], args, kwargs, __FUNCTION__, &itemraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);

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

	PyObject* mvItemRegistry::set_primary_window(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;
		int value;

		if (!VerifyRequiredArguments(mvApp::GetApp()->getParsers()["set_primary_window"], args))
			return GetPyNone();

		if (!Parse((mvApp::GetApp()->getParsers())["set_primary_window"], args, kwargs, __FUNCTION__, &itemraw, &value))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);

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

		PyObject* itemraw;
		PyObject* parentraw = nullptr;
		PyObject* beforeraw = nullptr;

		if (!Parse((mvApp::GetApp()->getParsers())["move_item"], args, kwargs, __FUNCTION__,
			&itemraw, &parentraw, &beforeraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);
		mvUUID parent = mvAppItem::GetIDFromPyObject(parentraw);
		mvUUID before = mvAppItem::GetIDFromPyObject(beforeraw);

		mvApp::GetApp()->getItemRegistry().moveItem(item, parent, before);

		return GetPyNone();
	}

	PyObject* mvItemRegistry::delete_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw;
		int childrenOnly = false;
		int slot = -1;

		if (!Parse((mvApp::GetApp()->getParsers())["delete_item"], args, kwargs, __FUNCTION__, &itemraw, &childrenOnly, &slot))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);

		mvApp::GetApp()->getItemRegistry().deleteItem(item, childrenOnly, slot);

		return GetPyNone();

	}

	PyObject* mvItemRegistry::does_item_exist(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw;

		if (!Parse((mvApp::GetApp()->getParsers())["does_item_exist"], args, kwargs, __FUNCTION__, &itemraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);

		if (mvApp::GetApp()->getItemRegistry().getItem(item))
			return ToPyBool(true);
		return ToPyBool(false);
	}

	PyObject* mvItemRegistry::move_item_up(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw;

		if (!Parse((mvApp::GetApp()->getParsers())["move_item_up"], args, kwargs, __FUNCTION__, &itemraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);

		mvApp::GetApp()->getItemRegistry().moveItemUp(item);

		return GetPyNone();

	}

	PyObject* mvItemRegistry::move_item_down(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw;

		if (!Parse((mvApp::GetApp()->getParsers())["move_item_down"], args, kwargs, __FUNCTION__, &itemraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);

		mvApp::GetApp()->getItemRegistry().moveItemDown(item);

		return GetPyNone();
	}

	PyObject* mvItemRegistry::reorder_items(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* containerraw;
		int slot = 0;
		PyObject* new_order = nullptr;

		if (!Parse((mvApp::GetApp()->getParsers())["reorder_items"], args, kwargs, __FUNCTION__,
			&containerraw, &slot, &new_order))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		auto anew_order = ToUUIDVect(new_order);
		mvUUID container = mvAppItem::GetIDFromPyObject(containerraw);

		auto parent = mvApp::GetApp()->getItemRegistry().getItem(container);

		std::vector<mvRef<mvAppItem>>& children = parent->getChildren(slot);

		std::vector<mvRef<mvAppItem>> newchildren;
		newchildren.reserve(children.size());

		// todo: better sorting algorithm
		for (const auto& item : anew_order)
		{
			for (auto& child : children)
			{
				if (child->_uuid == item)
				{
					newchildren.emplace_back(child);
					break;
				}
			}
		}
		children = newchildren;
		return GetPyNone();
	}

	PyObject* mvItemRegistry::unstage(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw = nullptr;

		if (!Parse((mvApp::GetApp()->getParsers())["unstage"], args, kwargs, __FUNCTION__, &itemraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);

		mvApp::GetApp()->getItemRegistry().unstageItem(item);

		return GetPyNone();
	}

	PyObject* mvItemRegistry::show_item_debug(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw = nullptr;

		if (!Parse((mvApp::GetApp()->getParsers())["show_item_debug"], args, kwargs, __FUNCTION__, &itemraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);

		auto actualItem = mvApp::GetApp()->getItemRegistry().getRefItem(item);
		if (actualItem)
		{
			actualItem->_showDebug = true;
			mvApp::GetApp()->getItemRegistry().addDebugWindow(actualItem);
		}
		else
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "show_item_debug",
				"Item not found: " + std::to_string(item), nullptr);
		}

		return GetPyNone();
	}

	PyObject* mvItemRegistry::get_all_items(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		return ToPyList(mvApp::GetApp()->getItemRegistry().getAllItems());
	}

	PyObject* mvItemRegistry::show_imgui_demo(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvApp::GetApp()->getItemRegistry()._showImGuiDebug = true;
		return GetPyNone();
	}

	PyObject* mvItemRegistry::show_implot_demo(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvApp::GetApp()->getItemRegistry()._showImPlotDebug = true;
		return GetPyNone();
	}

	PyObject* mvItemRegistry::get_windows(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		return ToPyList(mvApp::GetApp()->getItemRegistry().getWindows());
	}

	PyObject* mvItemRegistry::add_alias(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* alias;
		PyObject* itemraw;

		if (!Parse((mvApp::GetApp()->getParsers())["add_alias"], args, kwargs, __FUNCTION__, &alias, &itemraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);

		mvApp::GetApp()->getItemRegistry().addAlias(alias, item);

		return GetPyNone();

	}

	PyObject* mvItemRegistry::remove_alias(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* alias;

		if (!Parse((mvApp::GetApp()->getParsers())["remove_alias"], args, kwargs, __FUNCTION__, &alias))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvApp::GetApp()->getItemRegistry().removeAlias(alias);

		return GetPyNone();

	}

	PyObject* mvItemRegistry::does_alias_exist(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* alias;

		if (!Parse((mvApp::GetApp()->getParsers())["does_alias_exist"], args, kwargs, __FUNCTION__, &alias))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		bool result = mvApp::GetApp()->getItemRegistry().doesAliasExist(alias);

		return ToPyBool(result);
	}

	PyObject* mvItemRegistry::get_alias_id(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* alias;

		if (!Parse((mvApp::GetApp()->getParsers())["get_alias_id"], args, kwargs, __FUNCTION__, &alias))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID result = mvApp::GetApp()->getItemRegistry().getIdFromAlias(alias);

		return ToPyUUID(result);
	}

	PyObject* mvItemRegistry::get_aliases(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		std::vector<std::string> aliases;

		for (const auto& alias : mvApp::GetApp()->getItemRegistry()._aliases)
			aliases.push_back(alias.first);

		return ToPyList(aliases);
	}

	PyObject* mvItemRegistry::bind_template_registry(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw;

		if (!Parse((mvApp::GetApp()->getParsers())["bind_template_registry"], args, kwargs, __FUNCTION__,
			&itemraw))
			return GetPyNone();

		if (!mvApp::s_manualMutexControl) std::lock_guard<std::mutex> lk(mvApp::s_mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);


		if (item == 0)
			mvApp::GetApp()->getItemRegistry()._boundedTemplateRegistry = nullptr;
		else
		{
			auto actualItem = mvApp::GetApp()->getItemRegistry().getRefItem(item);
			if (actualItem)
				mvApp::GetApp()->getItemRegistry()._boundedTemplateRegistry = actualItem;
			else
			{
				mvThrowPythonError(mvErrorCode::mvItemNotFound, "bind_template_registry",
					"Item not found: " + std::to_string(item), nullptr);
				return GetPyNone();
			}
		}

		return GetPyNone();
	}

}
