#include "mvItemRegistry.h"
#include "mvProfiler.h"
#include "mvContext.h"
#include "mvItemRegistry.h"
#include "mvAppItemCommons.h"
#include "mvLog.h"
#include "mvPythonExceptions.h"
#include "mvToolManager.h"
#include "mvPythonExceptions.h"
#include "mvToolManager.h"
#include "mvFontManager.h"

namespace Marvel {

	void InsertParser_mvItemRegistry(std::map<std::string, mvPythonParser>* parsers)
	{

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::String, "alias" });
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Adds an alias.";
			setup.category = { "Item Registry" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "add_alias", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::String, "alias" });

			mvPythonParserSetup setup;
			setup.about = "Removes an alias.";
			setup.category = { "Item Registry" };

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

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "show_imgui_demo", parser });
		}

		{
			std::vector<mvPythonDataElement> args;

			mvPythonParserSetup setup;
			setup.about = "Shows the implot demo.";
			setup.category = { "Item Registry" };

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

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "reorder_items", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "template_registry" });

			mvPythonParserSetup setup;
			setup.about = "Binds a global template registry.";
			setup.category = { "Item Registry" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "bind_template_registry", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Unstages an item.";
			setup.category = { "Item Registry" };

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "unstage", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Shows an item's debug window";
			setup.category = { "Item Registry" };

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

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "move_item_up", parser });
		}

		{
			std::vector<mvPythonDataElement> args;
			args.push_back({ mvPyDataType::UUID, "item" });

			mvPythonParserSetup setup;
			setup.about = "Moves an item down.";
			setup.category = { "Item Registry" };

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

			mvPythonParser parser = FinalizeParser(setup, args);
			parsers->insert({ "set_primary_window", parser });
		}

	}

	static void PushParent(mvItemRegistry& registry, mvAppItem* item)
	{
		registry.containers.push(item);
	}

	static mvAppItem* PopParent(mvItemRegistry& registry)
	{
		if (registry.containers.empty())
		{
			mvThrowPythonError(mvErrorCode::mvContainerStackEmpty, "No container to pop.");
			MV_ITEM_REGISTRY_WARN("No container to pop.");
			assert(false);
			return nullptr;
		}

		auto item = registry.containers.top();
		registry.containers.pop();
		return item;
	}

	static mvAppItem* TopParent(mvItemRegistry& registry)
	{
		if (!registry.containers.empty())
			return registry.containers.top();
		return nullptr;
	}

	static void EmptyParents(mvItemRegistry& registry)
	{
		while (!registry.containers.empty())
			registry.containers.pop();

		MV_ITEM_REGISTRY_INFO("Container stack emptied.");
	}

	static void CacheItem(mvItemRegistry& registry, mvAppItem* item)
	{
		if (item->getDescFlags() & MV_ITEM_DESC_CONTAINER)
		{
			registry.cachedContainersID[registry.cachedContainerIndex] = item->_uuid;
			registry.cachedContainersPTR[registry.cachedContainerIndex] = item;
			registry.cachedContainerIndex++;
			if (registry.cachedContainerIndex == registry.CachedContainerCount)
				registry.cachedContainerIndex = 0;
		}

		registry.cachedItemsID[registry.cachedItemsIndex] = item->_uuid;
		registry.cachedItemsPTR[registry.cachedItemsIndex] = item;
		registry.cachedItemsIndex++;
		if (registry.cachedItemsIndex == registry.CachedContainerCount)
			registry.cachedItemsIndex = 0;
	}

	static bool DeleteRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID uuid)
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

	static bool MoveRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID uuid, mvRef<mvAppItem>& item)
	{
		for (auto& window : roots)
		{
			item = window->stealChild(uuid);
			if (item)
				return true;
		}
		return false;
	}

	static bool MoveUpRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID uuid)
	{
		for (auto& window : roots)
		{
			if (window->moveChildUp(uuid))
				return true;
		}
		return false;
	}

	static bool MoveDownRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID uuid)
	{
		for (auto& window : roots)
		{
			if (window->moveChildDown(uuid))
				return true;
		}
		return false;
	}

	static bool AddRuntimeChildRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID parent, mvUUID before, mvRef<mvAppItem> item)
	{
		for (auto& root : roots)
		{
			if (root->addRuntimeChild(parent, before, item))
				return true;
		}
		return false;

	}

	static bool AddItemAfterRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID prev, mvRef<mvAppItem> item)
	{
		for (auto& root : roots)
		{
			if (root->addChildAfter(prev, item))
				return true;
		}

		return false;
	}

	static bool AddItemAfter(mvItemRegistry& registry, mvUUID prev, mvRef<mvAppItem> item)
	{

		if (AddItemAfterRoot(registry.colormapRoots, prev, item)) return true;
		else if (AddItemAfterRoot(registry.filedialogRoots, prev, item)) return true;
		else if (AddItemAfterRoot(registry.stagingRoots, prev, item)) return true;
		else if (AddItemAfterRoot(registry.viewportMenubarRoots, prev, item)) return true;
		else if (AddItemAfterRoot(registry.fontRegistryRoots, prev, item)) return true;
		else if (AddItemAfterRoot(registry.handlerRegistryRoots, prev, item)) return true;
		else if (AddItemAfterRoot(registry.textureRegistryRoots, prev, item)) return true;
		else if (AddItemAfterRoot(registry.valueRegistryRoots, prev, item)) return true;
		else if (AddItemAfterRoot(registry.windowRoots, prev, item)) return true;
		else if (AddItemAfterRoot(registry.themeRegistryRoots, prev, item)) return true;
		else if (AddItemAfterRoot(registry.itemPoolRoots, prev, item)) return true;
		else if (AddItemAfterRoot(registry.itemTemplatesRoots, prev, item)) return true;
		else if (AddItemAfterRoot(registry.itemHandlerRegistryRoots, prev, item)) return true;

		assert(false);
		return false;
	}

	static bool AddItem(mvItemRegistry& registry, mvRef<mvAppItem> item)
	{

		//MV_ITEM_REGISTRY_TRACE("Adding item: " + item->_name);

		mvAppItem* parentitem = TopParent(registry);
		item->_parentPtr = parentitem;
		parentitem->addItem(item);

		return true;
	}

	static bool AddRuntimeItem(mvItemRegistry& registry, mvUUID parent, mvUUID before, mvRef<mvAppItem> item)
	{

		if (AddRuntimeChildRoot(registry.colormapRoots, parent, before, item)) return true;
		else if (AddRuntimeChildRoot(registry.filedialogRoots, parent, before, item)) return true;
		else if (AddRuntimeChildRoot(registry.stagingRoots, parent, before, item)) return true;
		else if (AddRuntimeChildRoot(registry.viewportMenubarRoots, parent, before, item)) return true;
		else if (AddRuntimeChildRoot(registry.fontRegistryRoots, parent, before, item)) return true;
		else if (AddRuntimeChildRoot(registry.handlerRegistryRoots, parent, before, item)) return true;
		else if (AddRuntimeChildRoot(registry.textureRegistryRoots, parent, before, item)) return true;
		else if (AddRuntimeChildRoot(registry.valueRegistryRoots, parent, before, item)) return true;
		else if (AddRuntimeChildRoot(registry.windowRoots, parent, before, item)) return true;
		else if (AddRuntimeChildRoot(registry.themeRegistryRoots, parent, before, item)) return true;
		else if (AddRuntimeChildRoot(registry.itemPoolRoots, parent, before, item)) return true;
		else if (AddRuntimeChildRoot(registry.itemTemplatesRoots, parent, before, item)) return true;
		else if (AddRuntimeChildRoot(registry.itemHandlerRegistryRoots, parent, before, item)) return true;

		return false;
	}

	static bool AddRoot(mvItemRegistry& registry, mvRef<mvAppItem> item)
	{

		if (item->getType() == mvAppItemType::mvWindowAppItem) registry.windowRoots.push_back(item);
		if (item->getType() == mvAppItemType::mvColorMapRegistry) registry.colormapRoots.push_back(item);
		if (item->getType() == mvAppItemType::mvFileDialog) registry.filedialogRoots.push_back(item);
		if (item->getType() == mvAppItemType::mvStage) registry.stagingRoots.push_back(item);
		if (item->getType() == mvAppItemType::mvViewportMenuBar) registry.viewportMenubarRoots.push_back(item);
		if (item->getType() == mvAppItemType::mvFontRegistry) registry.fontRegistryRoots.push_back(item);
		if (item->getType() == mvAppItemType::mvHandlerRegistry) registry.handlerRegistryRoots.push_back(item);
		if (item->getType() == mvAppItemType::mvTextureRegistry) registry.textureRegistryRoots.push_back(item);
		if (item->getType() == mvAppItemType::mvValueRegistry) registry.valueRegistryRoots.push_back(item);
		if (item->getType() == mvAppItemType::mvTheme) registry.themeRegistryRoots.push_back(item);
		if (item->getType() == mvAppItemType::mvItemPool) registry.itemPoolRoots.push_back(item);
		if (item->getType() == mvAppItemType::mvTemplateRegistry) registry.itemTemplatesRoots.push_back(item);
		if (item->getType() == mvAppItemType::mvItemHandlerRegistry) registry.itemHandlerRegistryRoots.push_back(item);

		return true;
	}

	static mvAppItem* GetItemRoot(mvItemRegistry& registry, std::vector<mvRef<mvAppItem>>& roots, mvUUID uuid)
	{
		for (auto root : roots)
		{
			if (root->_uuid == uuid)
			{
				CacheItem(registry, root.get());
				return root.get();
			}

			auto child = root->getChild(uuid);
			if (child)
			{
				CacheItem(registry, child);
				registry.delayedSearch.clear();
				return child;
			}
		}

		return nullptr;
	}

	static mvRef<mvAppItem> GetRefItemRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID uuid)
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

	static void GetAllItemsRoot(std::vector<mvRef<mvAppItem>>& roots, std::vector<mvUUID>& childList)
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
				if (child->getDescFlags() & MV_ITEM_DESC_CONTAINER)
					ChildRetriever(child);
			}
			for (auto& child : children1)
			{
				childList.emplace_back(child->_uuid);
				if (child->getDescFlags() & MV_ITEM_DESC_CONTAINER)
					ChildRetriever(child);
			}
			for (auto& child : children2)
			{
				childList.emplace_back(child->_uuid);
				if (child->getDescFlags() & MV_ITEM_DESC_CONTAINER)
					ChildRetriever(child);
			}

		};

		for (auto& root : roots)
		{
			childList.emplace_back(root->_uuid);
			ChildRetriever(root);
		}
	}

	static std::vector<mvUUID> GetAllItems(mvItemRegistry& registry)
	{

		std::vector<mvUUID> childList;

		GetAllItemsRoot(registry.colormapRoots, childList);
		GetAllItemsRoot(registry.filedialogRoots, childList);
		GetAllItemsRoot(registry.stagingRoots, childList);
		GetAllItemsRoot(registry.viewportMenubarRoots, childList);
		GetAllItemsRoot(registry.windowRoots, childList);
		GetAllItemsRoot(registry.fontRegistryRoots, childList);
		GetAllItemsRoot(registry.handlerRegistryRoots, childList);
		GetAllItemsRoot(registry.textureRegistryRoots, childList);
		GetAllItemsRoot(registry.valueRegistryRoots, childList);
		GetAllItemsRoot(registry.themeRegistryRoots, childList);
		GetAllItemsRoot(registry.itemPoolRoots, childList);
		GetAllItemsRoot(registry.itemTemplatesRoots, childList);
		GetAllItemsRoot(registry.itemHandlerRegistryRoots, childList);

		return childList;
	}

	static std::vector<mvUUID> GetWindows(mvItemRegistry& registry)
	{

		std::vector<mvUUID> childList;
		for (auto& root : registry.colormapRoots) childList.emplace_back(root->_uuid);
		for (auto& root : registry.filedialogRoots) childList.emplace_back(root->_uuid);
		for (auto& root : registry.stagingRoots) childList.emplace_back(root->_uuid);
		for (auto& root : registry.viewportMenubarRoots) childList.emplace_back(root->_uuid);
		for (auto& root : registry.windowRoots) childList.emplace_back(root->_uuid);
		for (auto& root : registry.fontRegistryRoots) childList.emplace_back(root->_uuid);
		for (auto& root : registry.handlerRegistryRoots) childList.emplace_back(root->_uuid);
		for (auto& root : registry.textureRegistryRoots) childList.emplace_back(root->_uuid);
		for (auto& root : registry.valueRegistryRoots) childList.emplace_back(root->_uuid);
		for (auto& root : registry.themeRegistryRoots) childList.emplace_back(root->_uuid);
		for (auto& root : registry.itemPoolRoots) childList.emplace_back(root->_uuid);
		for (auto& root : registry.itemTemplatesRoots) childList.emplace_back(root->_uuid);
		for (auto& root : registry.itemHandlerRegistryRoots) childList.emplace_back(root->_uuid);

		return childList;
	}

	static void SetPrimaryWindow(mvItemRegistry& registry, mvUUID uuid, bool value)
	{

		mvWindowAppItem* window = GetWindow(registry, uuid);

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
		for (auto& window : registry.windowRoots)
		{
			if (window->_uuid != uuid)
				static_cast<mvWindowAppItem*>(window.get())->setWindowAsMainStatus(false);
		}

	}

	static void UnstageItem(mvItemRegistry& registry, mvUUID uuid)
	{
		bool item_found = false;
		for (auto& item : registry.stagingRoots)
		{
			if (item->_uuid == uuid && item->getType() == mvAppItemType::mvStage)
			{
				for (auto& children : item->_children)
				{
					for (auto& child : children)
						AddItemWithRuntimeChecks(registry, child, 0, 0);
				}

				item_found = true;
				break;
			}
		}

		if (item_found)
		{
			CleanUpItem(registry, uuid);
			return;
		}

		mvThrowPythonError(mvErrorCode::mvItemNotFound, "unstage",
			"Stage not found: " + std::to_string(uuid), nullptr);
		assert(false);

	}

	mvRef<mvAppItem> GetItemFromPool(mvItemRegistry& registry, mvAppItemType itemType)
	{
		for (auto& pool : registry.itemPoolRoots)
		{
			auto item = static_cast<mvItemPool*>(pool.get())->getItem(itemType);
			if (item)
				return item;
		}

		return nullptr;
	}

	std::vector<std::vector<mvUUID>> GetItemChildren(mvItemRegistry& registry, mvUUID uuid)
	{

		mvAppItem* item = GetItem(registry, uuid);

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

	mvAppItem* GetItemRoot(mvItemRegistry& registry, mvUUID uuid)
	{
		mvAppItem* item = GetItem(registry, uuid);
		if (item)
		{
			if (item->_parentPtr)
			{
				mvAppItem* currentAncestor = item->_parentPtr;
				while (!(currentAncestor->getDescFlags() & MV_ITEM_DESC_ROOT))
					currentAncestor = currentAncestor->_parentPtr;

				return currentAncestor;
			}
		}
		return nullptr;
	}

	bool FocusItem(mvItemRegistry& registry, mvUUID uuid)
	{

		if (!GContext->started)
		{
			for (size_t i = 0; i < registry.windowRoots.size(); i++)
			{
				if (registry.windowRoots[i]->_uuid == uuid)
				{
					mvRef<mvAppItem> oldItem = registry.windowRoots.back();
					registry.windowRoots[registry.windowRoots.size() - 1] = registry.windowRoots[i];
					registry.windowRoots[i] = oldItem;
					return true;
				}
			}
		}

		auto appitem = GetItem(registry, uuid);

		if (appitem)
		{
			appitem->_focusNextFrame = true;
			if (auto parent = GetItemRoot(registry, uuid))
				parent->_focusNextFrame = true;
			return true;
		}
		else
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "focus_item",
				"Item not found: " + std::to_string(uuid), nullptr);
		return false;
	}

	bool DeleteItem(mvItemRegistry& registry, mvUUID uuid, bool childrenOnly, int slot)
	{

		MV_ITEM_REGISTRY_TRACE("Attempting to delete: " + std::to_string(uuid));

		CleanUpItem(registry, uuid);

		// delete item's children only
		if(childrenOnly)
		{
			auto item = GetItem(registry, uuid);
			if (item)
			{
				if (slot < 4)
				{
					item->_children[slot].clear();
					item->_children[slot].shrink_to_fit();
				}
				item->onChildrenRemoved();
				MV_ITEM_REGISTRY_INFO("Item found and it's children deleted.");
				return true;
			}
		}

		bool deletedItem = false;

		if (DeleteRoot(registry.colormapRoots, uuid)) deletedItem = true;
		else if (DeleteRoot(registry.filedialogRoots, uuid)) deletedItem = true;
		else if (DeleteRoot(registry.stagingRoots, uuid)) deletedItem = true;
		else if (DeleteRoot(registry.viewportMenubarRoots, uuid)) deletedItem = true;
		else if (DeleteRoot(registry.fontRegistryRoots, uuid)) deletedItem = true;
		else if (DeleteRoot(registry.handlerRegistryRoots, uuid)) deletedItem = true;
		else if (DeleteRoot(registry.textureRegistryRoots, uuid)) deletedItem = true;
		else if (DeleteRoot(registry.valueRegistryRoots, uuid)) deletedItem = true;
		else if (DeleteRoot(registry.windowRoots, uuid)) deletedItem = true;
		else if (DeleteRoot(registry.themeRegistryRoots, uuid)) deletedItem = true;
		else if (DeleteRoot(registry.itemPoolRoots, uuid)) deletedItem = true;
		else if (DeleteRoot(registry.itemTemplatesRoots, uuid)) deletedItem = true;
		else if (DeleteRoot(registry.itemHandlerRegistryRoots, uuid)) deletedItem = true;

		if (deletedItem)
		{
			MV_ITEM_REGISTRY_INFO(std::to_string(uuid) + " found and deleted.");
			RemoveDebugWindow(registry, uuid);
		}
		else
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "delete_item",
				"Item not found: " + std::to_string(uuid), nullptr);

		assert(deletedItem && "Item to delete not found");
		return deletedItem;
	}

	bool MoveItem(mvItemRegistry& registry, mvUUID uuid, mvUUID parent, mvUUID before)
	{

		MV_ITEM_REGISTRY_TRACE("Attempting to move: " + std::to_string(uuid));

		mvRef<mvAppItem> child = nullptr;

		bool movedItem = false;

		if (MoveRoot(registry.colormapRoots, uuid, child)) movedItem = true;
		else if (MoveRoot(registry.filedialogRoots, uuid, child)) movedItem = true;
		else if (MoveRoot(registry.stagingRoots, uuid, child)) movedItem = true;
		else if (MoveRoot(registry.viewportMenubarRoots, uuid, child)) movedItem = true;
		else if (MoveRoot(registry.fontRegistryRoots, uuid, child)) movedItem = true;
		else if (MoveRoot(registry.handlerRegistryRoots, uuid, child)) movedItem = true;
		else if (MoveRoot(registry.textureRegistryRoots, uuid, child)) movedItem = true;
		else if (MoveRoot(registry.valueRegistryRoots, uuid, child)) movedItem = true;
		else if (MoveRoot(registry.windowRoots, uuid, child)) movedItem = true;
		else if (MoveRoot(registry.themeRegistryRoots, uuid, child)) movedItem = true;
		else if (MoveRoot(registry.itemPoolRoots, uuid, child)) movedItem = true;
		else if (MoveRoot(registry.itemTemplatesRoots, uuid, child)) movedItem = true;
		else if (MoveRoot(registry.itemHandlerRegistryRoots, uuid, child)) movedItem = true;

		if (child == nullptr)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "move_item",
				"Item not found: " + std::to_string(uuid), nullptr);
			MV_ITEM_REGISTRY_WARN("Could not move item, it was not found");
		}

		if (child)
			AddRuntimeItem(registry, parent, before, child);

		return movedItem;
	}

	bool MoveItemUp(mvItemRegistry& registry, mvUUID uuid)
	{

		MV_ITEM_REGISTRY_TRACE("Attempting to move: " + std::to_string(uuid));

		bool movedItem = false;

		if (MoveUpRoot(registry.colormapRoots, uuid)) movedItem = true;
		else if (MoveUpRoot(registry.filedialogRoots, uuid)) movedItem = true;
		else if (MoveUpRoot(registry.stagingRoots, uuid)) movedItem = true;
		else if (MoveUpRoot(registry.viewportMenubarRoots, uuid)) movedItem = true;
		else if (MoveUpRoot(registry.fontRegistryRoots, uuid)) movedItem = true;
		else if (MoveUpRoot(registry.handlerRegistryRoots, uuid)) movedItem = true;
		else if (MoveUpRoot(registry.textureRegistryRoots, uuid)) movedItem = true;
		else if (MoveUpRoot(registry.valueRegistryRoots, uuid)) movedItem = true;
		else if (MoveUpRoot(registry.windowRoots, uuid)) movedItem = true;
		else if (MoveUpRoot(registry.themeRegistryRoots, uuid)) movedItem = true;
		else if (MoveUpRoot(registry.itemPoolRoots, uuid)) movedItem = true;
		else if (MoveUpRoot(registry.itemTemplatesRoots, uuid)) movedItem = true;
		else if (MoveUpRoot(registry.itemHandlerRegistryRoots, uuid)) movedItem = true;

		if (!movedItem)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "move_item",
				"Item not found: " + std::to_string(uuid), nullptr);
			MV_ITEM_REGISTRY_WARN("Could not move item, it was not found");
		}

		assert(movedItem && "Item to move not found");

		return movedItem;
	}

	bool MoveItemDown(mvItemRegistry& registry, mvUUID uuid)
	{

		MV_ITEM_REGISTRY_TRACE("Attempting to move: " + std::to_string(uuid));

		bool movedItem = false;

		if (MoveDownRoot(registry.colormapRoots, uuid)) movedItem = true;
		else if (MoveDownRoot(registry.filedialogRoots, uuid)) movedItem = true;
		else if (MoveDownRoot(registry.stagingRoots, uuid)) movedItem = true;
		else if (MoveDownRoot(registry.viewportMenubarRoots, uuid)) movedItem = true;
		else if (MoveDownRoot(registry.fontRegistryRoots, uuid)) movedItem = true;
		else if (MoveDownRoot(registry.handlerRegistryRoots, uuid)) movedItem = true;
		else if (MoveDownRoot(registry.textureRegistryRoots, uuid)) movedItem = true;
		else if (MoveDownRoot(registry.valueRegistryRoots, uuid)) movedItem = true;
		else if (MoveDownRoot(registry.windowRoots, uuid)) movedItem = true;
		else if (MoveDownRoot(registry.themeRegistryRoots, uuid)) movedItem = true;
		else if (MoveDownRoot(registry.itemPoolRoots, uuid)) movedItem = true;
		else if (MoveDownRoot(registry.itemTemplatesRoots, uuid)) movedItem = true;
		else if (MoveDownRoot(registry.itemHandlerRegistryRoots, uuid)) movedItem = true;

		if (!movedItem)
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "move_item",
				"Item not found: " + std::to_string(uuid), nullptr);
			MV_ITEM_REGISTRY_WARN("Could not move item, it was not found");
		}

		assert(movedItem && "Item to move not found");

		return movedItem;
	}

	void RenderItemRegistry(mvItemRegistry& registry)
	{

		MV_PROFILE_SCOPE("Rendering")

		if(registry.showImGuiDebug)
			ImGui::ShowDemoWindow(&registry.showImGuiDebug);
		if(registry.showImPlotDebug)
			ImPlot::ShowDemoWindow(&registry.showImPlotDebug);

		if (mvToolManager::GetFontManager()._resetDefault)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.FontDefault = nullptr;
			mvToolManager::GetFontManager()._resetDefault = false;
		}

		for (auto& root : registry.fontRegistryRoots)
		{
			if (root->_show)
				root->draw(nullptr, 0.0f, 0.0f);
		}

		if (mvToolManager::GetFontManager()._newDefault)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.FontDefault = nullptr;

			for (auto& root : registry.fontRegistryRoots)
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

		for (auto& root : registry.handlerRegistryRoots)
		{
			if (root->_show)
				root->draw(nullptr, 0.0f, 0.0f);
		}

		for (auto& root : registry.textureRegistryRoots)
			root->draw(nullptr, 0.0f, 0.0f);

		for (auto& root : registry.filedialogRoots)
			root->draw(nullptr, 0.0f, 0.0f);

		for (auto& root : registry.themeRegistryRoots)
		{
			if(root->_show)
				root->draw(nullptr, 0.0f, 0.0f);
		}

		for (auto& root : registry.windowRoots)
			root->draw(nullptr, 0.0f, 0.0f);

		for (auto& root : registry.colormapRoots)
		{
			if (root->_triggerAlternativeAction)
				root->alternativeCustomAction();
			root->draw(nullptr, 0.0f, 0.0f);
		}

		for (auto& root : registry.viewportMenubarRoots)
			root->draw(nullptr, 0.0f, 0.0f);

		for (auto& root : registry.themeRegistryRoots)
		{
			if (root->_show)
				root->customAction();
		}

		for (auto& root : registry.debugWindows)
			root->renderDebugWindow();

	}

	void ResetTheme(mvItemRegistry& registry)
	{
		for (auto& root : registry.themeRegistryRoots)
			root->_show = false;
	}

	void DelaySearch(mvItemRegistry& registry, mvAppItem* item)
	{
		registry.delayedSearch.push_back(item);
	}

	mvAppItem* GetItem(mvItemRegistry& registry, mvUUID uuid)
	{

		// check cache first
		for (int i = 0; i < registry.CachedContainerCount; i++)
		{
			if (registry.cachedContainersID[i] == uuid)
				return registry.cachedContainersPTR[i];
			if (registry.cachedItemsID[i] == uuid)
				return registry.cachedItemsPTR[i];
		}

		if (auto foundItem = GetItemRoot(registry, registry.colormapRoots, uuid)) return foundItem;
		if (auto foundItem = GetItemRoot(registry, registry.colormapRoots, uuid)) return foundItem;
		if (auto foundItem = GetItemRoot(registry, registry.filedialogRoots, uuid)) return foundItem;
		if (auto foundItem = GetItemRoot(registry, registry.stagingRoots, uuid)) return foundItem;
		if (auto foundItem = GetItemRoot(registry, registry.viewportMenubarRoots, uuid)) return foundItem;
		if (auto foundItem = GetItemRoot(registry, registry.fontRegistryRoots, uuid)) return foundItem;
		if (auto foundItem = GetItemRoot(registry, registry.handlerRegistryRoots, uuid)) return foundItem;
		if (auto foundItem = GetItemRoot(registry, registry.textureRegistryRoots, uuid)) return foundItem;
		if (auto foundItem = GetItemRoot(registry, registry.valueRegistryRoots, uuid)) return foundItem;
		if (auto foundItem = GetItemRoot(registry, registry.windowRoots, uuid)) return foundItem;
		if (auto foundItem = GetItemRoot(registry, registry.themeRegistryRoots, uuid)) return foundItem;
		if (auto foundItem = GetItemRoot(registry, registry.itemPoolRoots, uuid)) return foundItem;
		if (auto foundItem = GetItemRoot(registry, registry.itemTemplatesRoots, uuid)) return foundItem;
		if (auto foundItem = GetItemRoot(registry, registry.itemHandlerRegistryRoots, uuid)) return foundItem;

		for (auto delayedItem : registry.delayedSearch)
		{
			auto child = delayedItem->getChild(uuid);
			if (child)
			{
				CacheItem(registry, child);
				registry.delayedSearch.clear();
				return child;
			}
		}

		registry.delayedSearch.clear();

		return nullptr;
	}

	mvRef<mvAppItem> GetRefItem(mvItemRegistry& registry, mvUUID uuid)
	{

		if (auto foundItem = GetRefItemRoot(registry.colormapRoots, uuid)) return foundItem;
		else if (auto foundItem = GetRefItemRoot(registry.filedialogRoots, uuid)) return foundItem;
		else if (auto foundItem = GetRefItemRoot(registry.stagingRoots, uuid)) return foundItem;
		else if (auto foundItem = GetRefItemRoot(registry.viewportMenubarRoots, uuid)) return foundItem;
		else if (auto foundItem = GetRefItemRoot(registry.fontRegistryRoots, uuid)) return foundItem;
		else if (auto foundItem = GetRefItemRoot(registry.handlerRegistryRoots, uuid)) return foundItem;
		else if (auto foundItem = GetRefItemRoot(registry.textureRegistryRoots, uuid)) return foundItem;
		else if (auto foundItem = GetRefItemRoot(registry.valueRegistryRoots, uuid)) return foundItem;
		else if (auto foundItem = GetRefItemRoot(registry.windowRoots, uuid)) return foundItem;
		else if (auto foundItem = GetRefItemRoot(registry.themeRegistryRoots, uuid)) return foundItem;
		else if (auto foundItem = GetRefItemRoot(registry.itemPoolRoots, uuid)) return foundItem;
		else if (auto foundItem = GetRefItemRoot(registry.itemTemplatesRoots, uuid)) return foundItem;
		else if (auto foundItem = GetRefItemRoot(registry.itemHandlerRegistryRoots, uuid)) return foundItem;

		return nullptr;
	}

	mvWindowAppItem* GetWindow(mvItemRegistry& registry, mvUUID uuid)
	{

		mvAppItem* item = GetItem(registry, uuid);
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

	void ClearItemRegistry(mvItemRegistry& registry)
	{
		MV_ITEM_REGISTRY_INFO("Clearing item registry.");
		registry.colormapRoots.clear();
		registry.filedialogRoots.clear();
		registry.stagingRoots.clear();
		registry.viewportMenubarRoots.clear();
		registry.windowRoots.clear();
		registry.fontRegistryRoots.clear();
		registry.handlerRegistryRoots.clear();
		registry.textureRegistryRoots.clear();
		registry.valueRegistryRoots.clear();
		registry.themeRegistryRoots.clear();
		registry.itemPoolRoots.clear();
		registry.itemTemplatesRoots.clear();
		registry.itemHandlerRegistryRoots.clear();
	}

	void CleanUpItem(mvItemRegistry& registry, mvUUID uuid)
	{
		for (int i = 0; i < registry.CachedContainerCount; i++)
		{
			if (registry.cachedContainersID[i] == uuid)
			{
				registry.cachedContainersID[i] = 0;
				registry.cachedContainersPTR[i] = nullptr;
			}

			if (registry.cachedItemsID[i] == uuid)
			{
				registry.cachedItemsID[i] = 0;
				registry.cachedItemsPTR[i] = nullptr;
			}
		}
	}

	bool AddItemWithRuntimeChecks(mvItemRegistry& registry, mvRef<mvAppItem> item, mvUUID parent, mvUUID before)
	{
		//MV_ITEM_REGISTRY_TRACE("Adding runtime item: " + item->_name);

		if (item->getDescFlags() & MV_ITEM_DESC_HANDLER && parent == 0)
			parent = item->_parent;

		if (item == nullptr)
			return false;

		// check if item is ok
		if (!item->_state.ok)
			return false;

		//---------------------------------------------------------------------------
		// STEP 0: updata "last" information
		//---------------------------------------------------------------------------
		if (item->getDescFlags() & MV_ITEM_DESC_ROOT)
		{
			registry.lastRootAdded = item->_uuid;
			registry.lastContainerAdded = item->_uuid;
		}
		else if (item->getDescFlags() & MV_ITEM_DESC_CONTAINER)
			registry.lastContainerAdded = item->_uuid;

		registry.lastItemAdded = item->_uuid;

		CacheItem(registry, item.get());

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

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		//---------------------------------------------------------------------------
		// STEP 2: handle root case
		//---------------------------------------------------------------------------
		if (item->getDescFlags() & MV_ITEM_DESC_ROOT)
		{

			if (GContext->started)
			{
				AddRoot(registry, item);
				return true;
			}
			return AddRoot(registry, item);
		}
			
		//---------------------------------------------------------------------------
		// STEP 3: attempt to deduce parent
		//---------------------------------------------------------------------------
		mvAppItem* parentPtr = nullptr;
		if (before > 0)
		{

			auto beforeItem = GetItem(registry, before);
			if (beforeItem)
				parentPtr = beforeItem->_parentPtr;
			technique = AddTechnique::BEFORE;
		}

		else if (parent > MV_RESERVED_UUID_start + MV_RESERVED_UUIDs)
		{
			parentPtr = GetItem(registry, parent);
			technique = AddTechnique::PARENT;
		}

		else if (parent == 0)
		{
			parentPtr = TopParent(registry);
			technique = AddTechnique::STACK;
		}

		// reserved uuid case
		else
		{
			parentPtr = GetItem(registry, parent);
			if(parentPtr)
				technique = AddTechnique::PARENT;

			// revert to stack operation (reserved uuid not used)
			else
			{
				parentPtr = TopParent(registry);
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
			return AddItemAfter(registry, parent, item);

		//---------------------------------------------------------------------------
		// STEP 8: handle "before" and "after" style adding
		//---------------------------------------------------------------------------
		if (technique == AddTechnique::BEFORE || technique == AddTechnique::PARENT)
			return parentPtr->addRuntimeChild(parent, before, item); // same for run/compile time

		//---------------------------------------------------------------------------
		// STEP 9: handle "stack" style adding
		//---------------------------------------------------------------------------
		if(GContext->started)
			return parentPtr->addRuntimeChild(parentPtr->_uuid, 0, item);
		return AddItem(registry, item);
	}

	void AddAlias(mvItemRegistry& registry, const std::string& alias, mvUUID id)
	{
		if (!registry.allowAliasOverwrites)
		{
			if (DoesAliasExist(registry, alias))
			{
				mvThrowPythonError(mvErrorCode::mvNone, "add alias",
					"Alias already exists", nullptr);
				return;
			}
		}

		registry.aliases[alias] = id;

		auto item = GetItem(registry, id);
		if (item)
			item->_alias = alias;
	}

	void RemoveAlias(mvItemRegistry& registry, const std::string& alias, bool itemTriggered)
	{

		if (alias.empty())
			return;

		if (!DoesAliasExist(registry, alias))
		{
			mvThrowPythonError(mvErrorCode::mvNone, "remove alias",
				"Alias does not exists", nullptr);
			return;
		}

		auto item = GetItem(registry, registry.aliases[alias]);
		if (item)
			item->_alias = "";

		if (itemTriggered)
		{
			if (!registry.manualAliasManagement)
				registry.aliases.erase(alias);
		}
		else
			registry.aliases.erase(alias);

	}

	bool DoesAliasExist(mvItemRegistry& registry, const std::string& alias)
	{
		if (registry.aliases.count(alias) != 0)
			return true;
		return false;
	}

	mvUUID GetIdFromAlias(mvItemRegistry& registry, const std::string& alias)
	{
		if (DoesAliasExist(registry, alias))
			return registry.aliases[alias];
		return 0;
	}

	void TryBoundTemplateRegistry(mvItemRegistry& registry, mvAppItem* item)
	{
		if (registry.boundedTemplateRegistry)
		{
			for (auto& tempItem : registry.boundedTemplateRegistry->_children[item->getTarget()])
			{
				if (tempItem->getType() == item->getType())
				{
					item->applyTemplate(tempItem.get());
					return;
				}
			}
		}
	}

	void AddDebugWindow(mvItemRegistry& registry, mvRef<mvAppItem> item)
	{
		registry.debugWindows.push_back(item);
	}

	void RemoveDebugWindow(mvItemRegistry& registry, mvUUID uuid)
	{
		// check if debug window exists
		bool exists = false;
		for (const auto& debug : registry.debugWindows)
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

		std::vector<mvRef<mvAppItem>> oldWindows = registry.debugWindows;
		registry.debugWindows.clear();

		for (auto& debug : oldWindows)
		{
			if (debug->_uuid != uuid)
				registry.debugWindows.push_back(debug);
		}
	}

	PyObject* pop_container_stack(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		auto item = PopParent((*GContext->itemRegistry));
		if (item)
			return ToPyUUID(item->_uuid);
		else
			return GetPyNone();

	}

	PyObject* empty_container_stack(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);
		EmptyParents((*GContext->itemRegistry));
		return GetPyNone();
	}

	PyObject* top_container_stack(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		auto item = TopParent((*GContext->itemRegistry));
		if (item)
			return ToPyUUID(item->_uuid);
		else
			return GetPyNone();
	}

	PyObject* last_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		return ToPyUUID(GContext->itemRegistry->lastItemAdded);
	}

	PyObject* last_container(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		return ToPyUUID(GContext->itemRegistry->lastContainerAdded);
	}

	PyObject* last_root(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		return ToPyUUID(GContext->itemRegistry->lastRootAdded);
	}

	PyObject* push_container_stack(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;

		if (!Parse((GetParsers())["push_container_stack"], args, kwargs, __FUNCTION__, &itemraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);

		auto parent = GetItem((*GContext->itemRegistry), item);
		if (parent)
		{
			if (parent->getDescFlags() & MV_ITEM_DESC_CONTAINER)
			{
				PushParent((*GContext->itemRegistry), parent);
				return ToPyBool(true);
			}
		}
		return ToPyBool(false);
	}

	PyObject* set_primary_window(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		PyObject* itemraw;
		int value;

		if (!VerifyRequiredArguments(GetParsers()["set_primary_window"], args))
			return GetPyNone();

		if (!Parse((GetParsers())["set_primary_window"], args, kwargs, __FUNCTION__, &itemraw, &value))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);

		SetPrimaryWindow((*GContext->itemRegistry), item, value);

		return GetPyNone();
	}

	PyObject* get_active_window(PyObject* self, PyObject* args, PyObject* kwargs)
	{
		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		return ToPyUUID(GContext->itemRegistry->activeWindow);
	}

	PyObject* move_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw;
		PyObject* parentraw = nullptr;
		PyObject* beforeraw = nullptr;

		if (!Parse((GetParsers())["move_item"], args, kwargs, __FUNCTION__,
			&itemraw, &parentraw, &beforeraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);
		mvUUID parent = mvAppItem::GetIDFromPyObject(parentraw);
		mvUUID before = mvAppItem::GetIDFromPyObject(beforeraw);

		MoveItem((*GContext->itemRegistry), item, parent, before);

		return GetPyNone();
	}

	PyObject* delete_item(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw;
		int childrenOnly = false;
		int slot = -1;

		if (!Parse((GetParsers())["delete_item"], args, kwargs, __FUNCTION__, &itemraw, &childrenOnly, &slot))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);

		DeleteItem((*GContext->itemRegistry), item, childrenOnly, slot);

		return GetPyNone();

	}

	PyObject* does_item_exist(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw;

		if (!Parse((GetParsers())["does_item_exist"], args, kwargs, __FUNCTION__, &itemraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);

		if (GetItem((*GContext->itemRegistry), item))
			return ToPyBool(true);
		return ToPyBool(false);
	}

	PyObject* move_item_up(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw;

		if (!Parse((GetParsers())["move_item_up"], args, kwargs, __FUNCTION__, &itemraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);

		MoveItemUp((*GContext->itemRegistry), item);

		return GetPyNone();

	}

	PyObject* move_item_down(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw;

		if (!Parse((GetParsers())["move_item_down"], args, kwargs, __FUNCTION__, &itemraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);

		MoveItemDown((*GContext->itemRegistry), item);

		return GetPyNone();
	}

	PyObject* reorder_items(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* containerraw;
		int slot = 0;
		PyObject* new_order = nullptr;

		if (!Parse((GetParsers())["reorder_items"], args, kwargs, __FUNCTION__,
			&containerraw, &slot, &new_order))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		auto anew_order = ToUUIDVect(new_order);
		mvUUID container = mvAppItem::GetIDFromPyObject(containerraw);

		auto parent = GetItem((*GContext->itemRegistry), container);

		std::vector<mvRef<mvAppItem>>& children = parent->_children[slot];

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

	PyObject* unstage(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw = nullptr;

		if (!Parse((GetParsers())["unstage"], args, kwargs, __FUNCTION__, &itemraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);

		UnstageItem((*GContext->itemRegistry), item);

		return GetPyNone();
	}

	PyObject* show_item_debug(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw = nullptr;

		if (!Parse((GetParsers())["show_item_debug"], args, kwargs, __FUNCTION__, &itemraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);

		auto actualItem = GetRefItem((*GContext->itemRegistry), item);
		if (actualItem)
		{
			actualItem->_showDebug = true;
			AddDebugWindow((*GContext->itemRegistry), actualItem);
		}
		else
		{
			mvThrowPythonError(mvErrorCode::mvItemNotFound, "show_item_debug",
				"Item not found: " + std::to_string(item), nullptr);
		}

		return GetPyNone();
	}

	PyObject* get_all_items(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		return ToPyList(GetAllItems((*GContext->itemRegistry)));
	}

	PyObject* show_imgui_demo(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		GContext->itemRegistry->showImGuiDebug = true;
		return GetPyNone();
	}

	PyObject* show_implot_demo(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		GContext->itemRegistry->showImPlotDebug = true;
		return GetPyNone();
	}

	PyObject* get_windows(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		return ToPyList(GetWindows((*GContext->itemRegistry)));
	}

	PyObject* add_alias(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* alias;
		PyObject* itemraw;

		if (!Parse((GetParsers())["add_alias"], args, kwargs, __FUNCTION__, &alias, &itemraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);

		AddAlias((*GContext->itemRegistry), alias, item);

		return GetPyNone();

	}

	PyObject* remove_alias(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* alias;

		if (!Parse((GetParsers())["remove_alias"], args, kwargs, __FUNCTION__, &alias))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		RemoveAlias((*GContext->itemRegistry), alias);

		return GetPyNone();

	}

	PyObject* does_alias_exist(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* alias;

		if (!Parse((GetParsers())["does_alias_exist"], args, kwargs, __FUNCTION__, &alias))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		bool result = DoesAliasExist((*GContext->itemRegistry), alias);

		return ToPyBool(result);
	}

	PyObject* get_alias_id(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		const char* alias;

		if (!Parse((GetParsers())["get_alias_id"], args, kwargs, __FUNCTION__, &alias))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID result = GetIdFromAlias((*GContext->itemRegistry), alias);

		return ToPyUUID(result);
	}

	PyObject* get_aliases(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		std::vector<std::string> aliases;

		for (const auto& alias : GContext->itemRegistry->aliases)
			aliases.push_back(alias.first);

		return ToPyList(aliases);
	}

	PyObject* bind_template_registry(PyObject* self, PyObject* args, PyObject* kwargs)
	{

		PyObject* itemraw;

		if (!Parse((GetParsers())["bind_template_registry"], args, kwargs, __FUNCTION__,
			&itemraw))
			return GetPyNone();

		if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

		mvUUID item = mvAppItem::GetIDFromPyObject(itemraw);


		if (item == 0)
			GContext->itemRegistry->boundedTemplateRegistry = nullptr;
		else
		{
			auto actualItem = GetRefItem((*GContext->itemRegistry), item);
			if (actualItem)
				GContext->itemRegistry->boundedTemplateRegistry = actualItem;
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
