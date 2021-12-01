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
#include "mvPythonTypeChecker.h"
#include "mvGlobalIntepreterLock.h"

namespace Marvel {

    mvItemRegistry::mvItemRegistry()
    {
        // prefill cached containers
        for (i32 i = 0; i < CachedContainerCount; i++)
        {
            cachedContainersID[i] = 0;
            cachedContainersPTR[i] = nullptr;
            cachedItemsID[i] = 0;
            cachedItemsPTR[i] = nullptr;
        }
    }

    void 
    InsertParser_mvItemRegistry(std::map<std::string, mvPythonParser>* parsers)
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
            args.push_back({ mvPyDataType::Callable, "callback" });

            mvPythonParserSetup setup;
            setup.about = "Captures the next item.";
            setup.category = { "Item Registry" };

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "capture_next_item", parser });
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
            args.reserve(3);
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
            args.reserve(3);
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
            args.reserve(3);
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
            args.reserve(3);
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
            args.push_back({ mvPyDataType::UUID, "font" });

            mvPythonParserSetup setup;
            setup.about = "Sets an item's font.";
            setup.category = { "Fonts", "App Item Operations" };

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "bind_item_font", parser });
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
            setup.category = { "App Item Operations", "Events" };

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

    mv_internal b8
    DoesAliasExist(mvItemRegistry& registry, const std::string& alias)
    {
        if (registry.aliases.count(alias) != 0)
            return true;
        return false;
    }

    mv_internal void
    DebugItem(const char* label, const char* item) {
        ImGui::Text("%s", label);
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%s", item);
    }

    mv_internal void
    UpdateLocations(std::vector<mvRef<mvAppItem>>* children, i32 slots)
    {
        for (i32 i = 0; i < slots; i++)
        {
            i32 index = 0;
            for (auto& child : children[i])
            {
                child->_location = index;
                index++;
            }
        }
    }

    mv_internal void
    PushParent(mvItemRegistry& registry, mvAppItem* item)
    {
        registry.containers.push(item);
    }

    mv_internal mvAppItem*
    PopParent(mvItemRegistry& registry)
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

    mv_internal mvAppItem*
    TopParent(mvItemRegistry& registry)
    {
        if (!registry.containers.empty())
            return registry.containers.top();
        return nullptr;
    }

    mv_internal void
    EmptyParents(mvItemRegistry& registry)
    {
        while (!registry.containers.empty())
            registry.containers.pop();

        MV_ITEM_REGISTRY_INFO("Container stack emptied.");
    }

    mv_internal void
    CacheItem(mvItemRegistry& registry, mvAppItem* item)
    {
        if (GetEntityDesciptionFlags(item->getType()) & MV_ITEM_DESC_CONTAINER)
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

    mv_internal b8
    DeleteRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID uuid)
    {
        b8 deletedItem = false;

        // try to delete build-in item
        for (auto& root : roots)
        {
            deletedItem = root->deleteChild(uuid);
            if (deletedItem)
                break;
        }

        if (deletedItem)
            return true;

        b8 rootDeleting = false;

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

    mv_internal b8
    MoveRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID uuid, mvRef<mvAppItem>& item)
    {
        
        for (auto& window : roots)
        {
            item = window->stealChild(uuid);
            if (item)
                return true;
        }
        return false;
    }

    mv_internal b8
    MoveUpRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID uuid)
    {
        for (auto& window : roots)
        {
            if (window->moveChildUp(uuid))
                return true;
        }
        return false;
    }

    mv_internal b8
    MoveDownRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID uuid)
    {
        for (auto& window : roots)
        {
            if (window->moveChildDown(uuid))
                return true;
        }
        return false;
    }

    mv_internal b8
    AddRuntimeChildRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID parent, mvUUID before, mvRef<mvAppItem> item)
    {
        for (auto& root : roots)
        {
            if (root->addRuntimeChild(parent, before, item))
                return true;
        }
        return false;

    }

    mv_internal b8
    AddItemAfterRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID prev, mvRef<mvAppItem> item)
    {
        for (auto& root : roots)
        {
            if (root->addChildAfter(prev, item))
                return true;
        }

        return false;
    }

    mv_internal b8
    AddItemAfter(mvItemRegistry& registry, mvUUID prev, mvRef<mvAppItem> item)
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
        else if (AddItemAfterRoot(registry.itemTemplatesRoots, prev, item)) return true;
        else if (AddItemAfterRoot(registry.itemHandlerRegistryRoots, prev, item)) return true;
        else if (AddItemAfterRoot(registry.viewportDrawlistRoots, prev, item)) return true;

        assert(false);
        return false;
    }

    mv_internal b8
    AddItem(mvItemRegistry& registry, mvRef<mvAppItem> item)
    {

        //MV_ITEM_REGISTRY_TRACE("Adding item: " + item->_name);

        mvAppItem* parentitem = TopParent(registry);
        item->_parentPtr = parentitem;
        parentitem->addItem(item);

        return true;
    }

    mv_internal b8
    AddRuntimeItem(mvItemRegistry& registry, mvUUID parent, mvUUID before, mvRef<mvAppItem> item)
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
        else if (AddRuntimeChildRoot(registry.itemTemplatesRoots, parent, before, item)) return true;
        else if (AddRuntimeChildRoot(registry.itemHandlerRegistryRoots, parent, before, item)) return true;
        else if (AddRuntimeChildRoot(registry.viewportDrawlistRoots, parent, before, item)) return true;

        return false;
    }

    mv_internal b8
    AddRoot(mvItemRegistry& registry, mvRef<mvAppItem> item)
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
        if (item->getType() == mvAppItemType::mvTemplateRegistry) registry.itemTemplatesRoots.push_back(item);
        if (item->getType() == mvAppItemType::mvItemHandlerRegistry) registry.itemHandlerRegistryRoots.push_back(item);
        if (item->getType() == mvAppItemType::mvViewportDrawlist) registry.viewportDrawlistRoots.push_back(item);

        return true;
    }

    mv_internal mvAppItem*
    GetItemRoot(mvItemRegistry& registry, std::vector<mvRef<mvAppItem>>& roots, mvUUID uuid)
    {
        for (auto root : roots)
        {
            if (root->_uuid == uuid)
            {
                CacheItem(registry, root.get());
                return root.get();
            }

            mvAppItem* child = root->getChild(uuid);
            if (child)
            {
                CacheItem(registry, child);
                registry.delayedSearch.clear();
                return child;
            }
        }

        return nullptr;
    }

    mv_internal mvRef<mvAppItem>
    GetRefItemRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID uuid)
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

    mv_internal void
    GetAllItemsRoot(std::vector<mvRef<mvAppItem>>& roots, std::vector<mvUUID>& childList)
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
                if (GetEntityDesciptionFlags(child->getType()) & MV_ITEM_DESC_CONTAINER)
                    ChildRetriever(child);
            }
            for (auto& child : children1)
            {
                childList.emplace_back(child->_uuid);
                if (GetEntityDesciptionFlags(child->getType()) & MV_ITEM_DESC_CONTAINER)
                    ChildRetriever(child);
            }
            for (auto& child : children2)
            {
                childList.emplace_back(child->_uuid);
                if (GetEntityDesciptionFlags(child->getType()) & MV_ITEM_DESC_CONTAINER)
                    ChildRetriever(child);
            }

        };

        for (auto& root : roots)
        {
            childList.emplace_back(root->_uuid);
            ChildRetriever(root);
        }
    }

    mv_internal std::vector<mvUUID>
    GetAllItems(mvItemRegistry& registry)
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
        GetAllItemsRoot(registry.itemTemplatesRoots, childList);
        GetAllItemsRoot(registry.itemHandlerRegistryRoots, childList);
        GetAllItemsRoot(registry.viewportDrawlistRoots, childList);

        return childList;
    }

    mv_internal std::vector<mvUUID>
    GetWindows(mvItemRegistry& registry)
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
        for (auto& root : registry.itemTemplatesRoots) childList.emplace_back(root->_uuid);
        for (auto& root : registry.itemHandlerRegistryRoots) childList.emplace_back(root->_uuid);
        for (auto& root : registry.viewportDrawlistRoots) childList.emplace_back(root->_uuid);

        return childList;
    }

    mv_internal void
    SetPrimaryWindow(mvItemRegistry& registry, mvUUID uuid, b8 value)
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

    mv_internal void
    UnstageItem(mvItemRegistry& registry, mvUUID uuid)
    {
        b8 item_found = false;
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

    mv_internal std::vector<std::vector<mvUUID>>
    GetItemChildren(mvItemRegistry& registry, mvUUID uuid)
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

    mv_internal void
    AddDebugWindow(mvItemRegistry& registry, mvRef<mvAppItem> item)
    {
        registry.debugWindows.push_back(item);
    }

    mv_internal void
    RemoveDebugWindow(mvItemRegistry& registry, mvUUID uuid)
    {
        // check if debug window exists
        b8 exists = false;
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

    mvUUID 
    GetIDFromPyObject(PyObject* item)
    {

        if (item == nullptr)
            return 0;

        if (isPyObject_Int(item))
            return ToUUID(item);
        else if (isPyObject_String(item))
        {
            std::string alias = ToString(item);
            return GetIdFromAlias(*GContext->itemRegistry, alias);
        }
        return 0;
    }

    mvAppItem* 
    GetItemRoot(mvItemRegistry& registry, mvUUID uuid)
    {
        mvAppItem* item = GetItem(registry, uuid);
        if (item)
        {
            if (item->_parentPtr)
            {
                mvAppItem* currentAncestor = item->_parentPtr;
                while (!(GetEntityDesciptionFlags(currentAncestor->getType()) & MV_ITEM_DESC_ROOT))
                    currentAncestor = currentAncestor->_parentPtr;

                return currentAncestor;
            }
        }
        return nullptr;
    }

    b8
    DeleteItem(mvItemRegistry& registry, mvUUID uuid, b8 childrenOnly, i32 slot)
    {

        MV_ITEM_REGISTRY_TRACE("Attempting to delete: " + std::to_string(uuid));

        CleanUpItem(registry, uuid);

        // delete item's children only
        if(childrenOnly)
        {
            auto item = GetItem(registry, uuid);
            if (item)
            {
                if (slot > -1 && slot < 4)
                {
                    item->_children[slot].clear();
                    item->_children[slot].shrink_to_fit();
                }
                else
                {
                    for(size_t i = 0; i < 4; i++)
                    {
                        item->_children[i].clear();
                        item->_children[i].shrink_to_fit();
                    }
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
        else if (DeleteRoot(registry.itemTemplatesRoots, uuid)) deletedItem = true;
        else if (DeleteRoot(registry.itemHandlerRegistryRoots, uuid)) deletedItem = true;
        else if (DeleteRoot(registry.viewportDrawlistRoots, uuid)) deletedItem = true;

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

    b8
    MoveItem(mvItemRegistry& registry, mvUUID uuid, mvUUID parent, mvUUID before)
    {

        MV_ITEM_REGISTRY_TRACE("Attempting to move: " + std::to_string(uuid));


        mvRef<mvAppItem> child = nullptr;

        b8 movedItem = false;

        if(registry.capturedItem)
        {
            if(registry.capturedItem->_uuid == uuid)
            {
                child = registry.capturedItem;
                movedItem = true;
                registry.capturedItem = nullptr;
            }
                
        }

        if(!movedItem)
        {
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
            else if (MoveRoot(registry.itemTemplatesRoots, uuid, child)) movedItem = true;
            else if (MoveRoot(registry.itemHandlerRegistryRoots, uuid, child)) movedItem = true;
            else if (MoveRoot(registry.viewportDrawlistRoots, uuid, child)) movedItem = true;
        }
        
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

    b8
    MoveItemUp(mvItemRegistry& registry, mvUUID uuid)
    {

        MV_ITEM_REGISTRY_TRACE("Attempting to move: " + std::to_string(uuid));

        b8 movedItem = false;

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
        else if (MoveUpRoot(registry.itemTemplatesRoots, uuid)) movedItem = true;
        else if (MoveUpRoot(registry.itemHandlerRegistryRoots, uuid)) movedItem = true;
        else if (MoveUpRoot(registry.viewportDrawlistRoots, uuid)) movedItem = true;

        if (!movedItem)
        {
            mvThrowPythonError(mvErrorCode::mvItemNotFound, "move_item",
                "Item not found: " + std::to_string(uuid), nullptr);
            MV_ITEM_REGISTRY_WARN("Could not move item, it was not found");
        }

        assert(movedItem && "Item to move not found");

        return movedItem;
    }

    b8
    MoveItemDown(mvItemRegistry& registry, mvUUID uuid)
    {

        MV_ITEM_REGISTRY_TRACE("Attempting to move: " + std::to_string(uuid));

        b8 movedItem = false;

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
        else if (MoveDownRoot(registry.itemTemplatesRoots, uuid)) movedItem = true;
        else if (MoveDownRoot(registry.itemHandlerRegistryRoots, uuid)) movedItem = true;
        else if (MoveDownRoot(registry.viewportDrawlistRoots, uuid)) movedItem = true;

        if (!movedItem)
        {
            mvThrowPythonError(mvErrorCode::mvItemNotFound, "move_item",
                "Item not found: " + std::to_string(uuid), nullptr);
            MV_ITEM_REGISTRY_WARN("Could not move item, it was not found");
        }

        assert(movedItem && "Item to move not found");

        return movedItem;
    }

    void 
    RenderItemRegistry(mvItemRegistry& registry)
    {

        // TODO: figure out why delayedSearch can
        //       still have values (sometimes).
        //       It should be empty after every search.
        if(!registry.delayedSearch.empty())
            registry.delayedSearch.clear();

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

        for (auto& root : registry.colormapRoots)
            root->draw(nullptr, 0.0f, 0.0f);

        for (auto& root : registry.windowRoots)
            root->draw(nullptr, 0.0f, 0.0f);

        for (auto& root : registry.viewportMenubarRoots)
            root->draw(nullptr, 0.0f, 0.0f);

        for (auto& root : registry.viewportDrawlistRoots)
            root->draw(nullptr, 0.0f, 0.0f);

        for (auto& root : registry.themeRegistryRoots)
        {
            if (root->_show)
                root->customAction();
        }

        for (auto& root : registry.debugWindows)
        {
            ImGui::SetNextWindowSize(ImVec2(500.0f, 500.0f), ImGuiCond_FirstUseEver);

            if (!ImGui::Begin(root->_specifiedLabel.c_str(), &root->_showDebug))
            {
                ImGui::End();

                if (!root->_showDebug)
                    RemoveDebugWindow(*GContext->itemRegistry, root->_uuid);
                return;
            }

            mv_local_persist char ts[6] = "True";
            mv_local_persist char fs[6] = "False";

            std::string width = std::to_string(root->_width);
            std::string height = std::to_string(root->_height);

            std::string sizex = std::to_string(root->_state.rectSize.x);
            std::string sizey = std::to_string(root->_state.rectSize.y);

            ImGui::PushID(root.get());
            DebugItem("Label:", root->_specifiedLabel.c_str());
            DebugItem("ID:", std::to_string(root->_uuid).c_str());
            DebugItem("Alias:", root->_alias.c_str());
            DebugItem("Type:", GetEntityTypeString(root->getType()));
            DebugItem("Filter:", root->_filter.c_str());
            DebugItem("Payload Type:", root->_payloadType.c_str());
            DebugItem("Location:", std::to_string(root->_location).c_str());
            DebugItem("Track Offset:", std::to_string(root->_trackOffset).c_str());
            DebugItem("Container:", GetEntityDesciptionFlags(root->getType()) & MV_ITEM_DESC_CONTAINER ? ts : fs);
            DebugItem("Width:", width.c_str());
            DebugItem("Height:", height.c_str());
            DebugItem("Size x:", sizex.c_str());
            DebugItem("Size y:", sizey.c_str());
            DebugItem("Show:", root->_show ? ts : fs);
            DebugItem("Enabled:", root->_enabled ? ts : fs);
            DebugItem("Tracked:", root->_tracked ? ts : fs);
            DebugItem("Callback:", root->_callback ? ts : fs);
            DebugItem("User Data:", root->_user_data ? ts : fs);
            DebugItem("Drop Callback:", root->_dropCallback ? ts : fs);
            DebugItem("Drag Callback:", root->_dragCallback ? ts : fs);

            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Text("Bindings");
            ImGui::Separator();
            DebugItem("Theme Bound:", root->_theme ? ts : fs);
            DebugItem("Font Bound:", root->_font ? ts : fs);
            DebugItem("Handlers Bound:", root->_handlerRegistry ? ts : fs);

            i32 applicableState = GetApplicableState(root->getType());
            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Text("State");
            ImGui::Separator();
            if (applicableState & MV_STATE_VISIBLE) DebugItem("Item Visible:", IsItemVisible(root->_state, 1) ? ts : fs);
            if (applicableState & MV_STATE_HOVER) DebugItem("Item Hovered:", IsItemHovered(root->_state, 1) ? ts : fs);
            if (applicableState & MV_STATE_ACTIVE) DebugItem("Item Active:", IsItemActive(root->_state, 1) ? ts : fs);
            if (applicableState & MV_STATE_FOCUSED) DebugItem("Item Focused:", IsItemFocused(root->_state, 1) ? ts : fs);
            if (applicableState & MV_STATE_CLICKED)
            {
                DebugItem("Item Left Clicked:", IsItemLeftClicked(root->_state, 1) ? ts : fs);
                DebugItem("Item Right Clicked:", IsItemRightClicked(root->_state, 1) ? ts : fs);
                DebugItem("Item Middle Clicked:", IsItemMiddleClicked(root->_state, 1) ? ts : fs);
            }
            if (applicableState & MV_STATE_EDITED) DebugItem("Item Edited:", IsItemEdited(root->_state, 1) ? ts : fs);
            if (applicableState & MV_STATE_ACTIVATED) DebugItem("Item Activated:", IsItemActivated(root->_state, 1) ? ts : fs);
            if (applicableState & MV_STATE_DEACTIVATED) DebugItem("Item Deactivated:", IsItemDeactivated(root->_state, 1) ? ts : fs);
            if (applicableState & MV_STATE_DEACTIVATEDAE) DebugItem("Item DeactivatedAfterEdit:", IsItemDeactivatedAfterEdit(root->_state, 1) ? ts : fs);
            if (applicableState & MV_STATE_TOGGLED_OPEN) DebugItem("Item ToggledOpen:", IsItemToogledOpen(root->_state, 1) ? ts : fs);

            ImGui::PopID();

            root->renderSpecificDebugInfo();

            ImGui::End();

            if (!root->_showDebug)
                RemoveDebugWindow(*GContext->itemRegistry, root->_uuid);
        }

    }

    void 
    ResetTheme(mvItemRegistry& registry)
    {
        for (auto& root : registry.themeRegistryRoots)
            root->_show = false;
    }

    void 
    DelaySearch(mvItemRegistry& registry, mvAppItem* item)
    {
        registry.delayedSearch.push_back(item);
    }

    mvAppItem* 
    GetItem(mvItemRegistry& registry, mvUUID uuid)
    {

        // check captured
        if(registry.capturedItem)
        {
            if(registry.capturedItem->_uuid == uuid)
                return registry.capturedItem.get();
        }

        // check cache
        for (i32 i = 0; i < registry.CachedContainerCount; i++)
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
        if (auto foundItem = GetItemRoot(registry, registry.itemTemplatesRoots, uuid)) return foundItem;
        if (auto foundItem = GetItemRoot(registry, registry.itemHandlerRegistryRoots, uuid)) return foundItem;
        if (auto foundItem = GetItemRoot(registry, registry.viewportDrawlistRoots, uuid)) return foundItem;

        for (auto delayedItem : registry.delayedSearch)
        {
            mvAppItem* child = delayedItem->getChild(uuid);
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

    mvRef<mvAppItem> 
    GetRefItem(mvItemRegistry& registry, mvUUID uuid)
    {

        // check captured
        if(registry.capturedItem)
        {
            if(registry.capturedItem->_uuid == uuid)
                return registry.capturedItem;
        }
        
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
        else if (auto foundItem = GetRefItemRoot(registry.itemTemplatesRoots, uuid)) return foundItem;
        else if (auto foundItem = GetRefItemRoot(registry.itemHandlerRegistryRoots, uuid)) return foundItem;
        else if (auto foundItem = GetRefItemRoot(registry.viewportDrawlistRoots, uuid)) return foundItem;

        return nullptr;
    }

    mvWindowAppItem* 
    GetWindow(mvItemRegistry& registry, mvUUID uuid)
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

    void 
    ClearItemRegistry(mvItemRegistry& registry)
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
        registry.itemTemplatesRoots.clear();
        registry.itemHandlerRegistryRoots.clear();
        registry.viewportDrawlistRoots.clear();
    }

    void 
    CleanUpItem(mvItemRegistry& registry, mvUUID uuid)
    {
        for (i32 i = 0; i < registry.CachedContainerCount; i++)
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

    b8
    AddItemWithRuntimeChecks(mvItemRegistry& registry, mvRef<mvAppItem> item, mvUUID parent, mvUUID before)
    {

        if(registry.captureCallback)
        {
  
            // this is a unique situation in that the caller always has the GIL
            registry.capturedItem = item;
            mvRunCallback(registry.captureCallback, registry.capturedItem->_uuid, nullptr, nullptr);
            Py_XDECREF(registry.captureCallback);
            registry.captureCallback = nullptr;
            return true;
        }

        if (GetEntityDesciptionFlags(item->getType()) & MV_ITEM_DESC_HANDLER && parent == 0)
            parent = item->_parent;

        if (item == nullptr)
            return false;

        // check if item is ok
        if (!item->_state.ok)
            return false;

        //---------------------------------------------------------------------------
        // STEP 0: updata "last" information
        //---------------------------------------------------------------------------
        if (GetEntityDesciptionFlags(item->getType()) & MV_ITEM_DESC_ROOT)
        {
            registry.lastRootAdded = item->_uuid;
            registry.lastContainerAdded = item->_uuid;
        }
        else if (GetEntityDesciptionFlags(item->getType()) & MV_ITEM_DESC_CONTAINER)
            registry.lastContainerAdded = item->_uuid;

        registry.lastItemAdded = item->_uuid;

        CacheItem(registry, item.get());

        //---------------------------------------------------------------------------
        // STEP 1: check if an item with this name exists (NO LONGER NEEDED)
        //---------------------------------------------------------------------------
        //if (getItem(item->_uuid))
        //{
        //  mvThrowPythonEr1ror(mvErrorCode::mvNon1e, "Item must have a unique name.");
        //  MV_ITEM_REGISTRY_WARN("Item must have a unique name.");
        //  assert(false);
        //  return false;
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
        if (GetEntityDesciptionFlags(item->getType()) & MV_ITEM_DESC_ROOT)
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

            mvAppItem* beforeItem = GetItem(registry, before);
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
            if (parentPtr)
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
        b8 isParentCompatible = false;
        const std::vector<std::pair<std::string, i32>>* allowableParents = &item->getAllowableParents();

        std::string acceptableParentTypes;

        for (const auto& compatibleParent : *allowableParents)
        {
            acceptableParentTypes.append(compatibleParent.first + "\n");
            if ((i32)parentPtr->getType() == compatibleParent.second)
            {
                isParentCompatible = true;
                break;
            }
        }

        if (!isParentCompatible)
        {
            if (allowableParents->empty())
            {
                mvThrowPythonError(mvErrorCode::mvIncompatibleParent, item->getCommand(),
                    "Incompatible parent. Item does can not have a parent.", item.get());
                return false;
            }

            if (!(*allowableParents)[0].second == (i32)mvAppItemType::All)
            {

                mvThrowPythonError(mvErrorCode::mvIncompatibleParent, item->getCommand(),
                    "Incompatible parent. Acceptable parents include:\t" + acceptableParentTypes, item.get());

                assert(false);
                return false;
            }
        }

        //---------------------------------------------------------------------------
        // STEP 6: check if parent accepts our item (this isn't duplicate STEP 3)
        //---------------------------------------------------------------------------
        b8 amICompatible = false;
        const std::vector<std::pair<std::string, i32>>* allowableChildren = &parentPtr->getAllowableChildren();

        std::string acceptableChildTypes;

        for (const auto& compatibleChildren : *allowableChildren)
        {
            acceptableChildTypes.append(compatibleChildren.first + "\n");
            if ((i32)item->getType() == compatibleChildren.second)
            {
                amICompatible = true;
                break;
            }
        }

        if (!amICompatible)
        {
            if (allowableChildren->empty())
            {
                mvThrowPythonError(mvErrorCode::mvIncompatibleChild, parentPtr->getCommand(),
                    "Incompatible child. Item does not accept children.", parentPtr);
                return false;
            }

            if (!(*allowableChildren)[0].second == (i32)mvAppItemType::All)
            {

                mvThrowPythonError(mvErrorCode::mvIncompatibleChild, parentPtr->getCommand(),
                    "Incompatible child. Acceptable children include:\t" + acceptableChildTypes, parentPtr);

                assert(false);
                return false;
            }
        }

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

    void 
    AddAlias(mvItemRegistry& registry, const std::string& alias, mvUUID id)
    {
        if (!GContext->IO.allowAliasOverwrites)
        {
            if (DoesAliasExist(registry, alias))
            {
                mvThrowPythonError(mvErrorCode::mvNone, "add alias",
                    "Alias already exists", nullptr);
                return;
            }
        }

        registry.aliases[alias] = id;

        mvAppItem* item = GetItem(registry, id);
        if (item)
            item->_alias = alias;
    }

    void 
    RemoveAlias(mvItemRegistry& registry, const std::string& alias, b8 itemTriggered)
    {

        if (alias.empty())
            return;

        if (!DoesAliasExist(registry, alias))
        {
            mvThrowPythonError(mvErrorCode::mvNone, "remove alias",
                "Alias does not exists", nullptr);
            return;
        }

        mvAppItem* item = GetItem(registry, registry.aliases[alias]);
        if (item)
            item->_alias.clear();

        if (itemTriggered)
        {
            if (!GContext->IO.manualAliasManagement)
                registry.aliases.erase(alias);
        }
        else
            registry.aliases.erase(alias);

    }

    mvUUID 
    GetIdFromAlias(mvItemRegistry& registry, const std::string& alias)
    {
        if (DoesAliasExist(registry, alias))
            return registry.aliases[alias];
        return 0;
    }

    void 
    TryBoundTemplateRegistry(mvItemRegistry& registry, mvAppItem* item)
    {
        if (registry.boundedTemplateRegistry)
        {
            for (auto& tempItem : registry.boundedTemplateRegistry->_children[GetEntityTargetSlot(item->getType())])
            {
                if (tempItem->getType() == item->getType())
                {
                    item->applyTemplate(tempItem.get());
                    return;
                }
            }
        }
    }

    std::tuple<mvUUID, mvUUID, std::string> 
    GetNameFromArgs(mvUUID& name, PyObject* args, PyObject* kwargs)
    {

        mvUUID parent = 0;
        mvUUID before = 0;
        std::string alias;

        if (kwargs)
        {
            if (PyObject* item = PyDict_GetItemString(kwargs, "parent"))
            {
                if (PyUnicode_Check(item))
                    parent = GetIdFromAlias(*GContext->itemRegistry, ToString(item));
                else
                    parent = ToUUID(item);
            }

            if (PyObject* item = PyDict_GetItemString(kwargs, "before"))
            {
                if (PyUnicode_Check(item))
                    before = GetIdFromAlias(*GContext->itemRegistry, ToString(item));
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

    mv_python_function
    pop_container_stack(PyObject* self, PyObject* args, PyObject* kwargs)
    {

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvAppItem* item = PopParent((*GContext->itemRegistry));
        if (item)
            return ToPyUUID(item->_uuid);
        else
            return GetPyNone();

    }

    mv_python_function
    empty_container_stack(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);
        EmptyParents((*GContext->itemRegistry));
        return GetPyNone();
    }

    mv_python_function
    top_container_stack(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvAppItem* item = TopParent((*GContext->itemRegistry));
        if (item)
            return ToPyUUID(item->_uuid);
        else
            return GetPyNone();
    }

    mv_python_function
    last_item(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        return ToPyUUID(GContext->itemRegistry->lastItemAdded);
    }

    mv_python_function
    last_container(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        return ToPyUUID(GContext->itemRegistry->lastContainerAdded);
    }

    mv_python_function
    last_root(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        return ToPyUUID(GContext->itemRegistry->lastRootAdded);
    }

    mv_python_function
    push_container_stack(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        PyObject* itemraw;

        if (!Parse((GetParsers())["push_container_stack"], args, kwargs, __FUNCTION__, &itemraw))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID item = GetIDFromPyObject(itemraw);

        mvAppItem* parent = GetItem((*GContext->itemRegistry), item);
        if (parent)
        {
            if (GetEntityDesciptionFlags(parent->getType()) & MV_ITEM_DESC_CONTAINER)
            {
                PushParent((*GContext->itemRegistry), parent);
                return ToPyBool(true);
            }
        }
        return ToPyBool(false);
    }

    mv_python_function
    set_primary_window(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        PyObject* itemraw;
        i32 value;

        if (!VerifyRequiredArguments(GetParsers()["set_primary_window"], args))
            return GetPyNone();

        if (!Parse((GetParsers())["set_primary_window"], args, kwargs, __FUNCTION__, &itemraw, &value))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID item = GetIDFromPyObject(itemraw);

        SetPrimaryWindow((*GContext->itemRegistry), item, value);

        return GetPyNone();
    }

    mv_python_function
    get_active_window(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        return ToPyUUID(GContext->itemRegistry->activeWindow);
    }

    mv_python_function
    move_item(PyObject* self, PyObject* args, PyObject* kwargs)
    {

        PyObject* itemraw;
        PyObject* parentraw = nullptr;
        PyObject* beforeraw = nullptr;

        if (!Parse((GetParsers())["move_item"], args, kwargs, __FUNCTION__,
            &itemraw, &parentraw, &beforeraw))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID item = GetIDFromPyObject(itemraw);
        mvUUID parent = GetIDFromPyObject(parentraw);
        mvUUID before = GetIDFromPyObject(beforeraw);

        MoveItem((*GContext->itemRegistry), item, parent, before);

        return GetPyNone();
    }

    mv_python_function
    delete_item(PyObject* self, PyObject* args, PyObject* kwargs)
    {

        PyObject* itemraw;
        b32 childrenOnly = false;
        i32 slot = -1;

        if (!Parse((GetParsers())["delete_item"], args, kwargs, __FUNCTION__, &itemraw, &childrenOnly, &slot))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID item = GetIDFromPyObject(itemraw);

        DeleteItem((*GContext->itemRegistry), item, childrenOnly, slot);

        return GetPyNone();

    }

    mv_python_function
    does_item_exist(PyObject* self, PyObject* args, PyObject* kwargs)
    {

        PyObject* itemraw;

        if (!Parse((GetParsers())["does_item_exist"], args, kwargs, __FUNCTION__, &itemraw))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID item = GetIDFromPyObject(itemraw);

        if (GetItem((*GContext->itemRegistry), item))
            return ToPyBool(true);
        return ToPyBool(false);
    }

    mv_python_function
    move_item_up(PyObject* self, PyObject* args, PyObject* kwargs)
    {

        PyObject* itemraw;

        if (!Parse((GetParsers())["move_item_up"], args, kwargs, __FUNCTION__, &itemraw))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID item = GetIDFromPyObject(itemraw);

        MoveItemUp((*GContext->itemRegistry), item);

        return GetPyNone();

    }

    mv_python_function
    move_item_down(PyObject* self, PyObject* args, PyObject* kwargs)
    {

        PyObject* itemraw;

        if (!Parse((GetParsers())["move_item_down"], args, kwargs, __FUNCTION__, &itemraw))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID item = GetIDFromPyObject(itemraw);

        MoveItemDown((*GContext->itemRegistry), item);

        return GetPyNone();
    }

    mv_python_function
    reorder_items(PyObject* self, PyObject* args, PyObject* kwargs)
    {

        PyObject* containerraw;
        i32 slot = 0;
        PyObject* new_order = nullptr;

        if (!Parse((GetParsers())["reorder_items"], args, kwargs, __FUNCTION__,
            &containerraw, &slot, &new_order))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        auto anew_order = ToUUIDVect(new_order);
        mvUUID container = GetIDFromPyObject(containerraw);

        mvAppItem* parent = GetItem((*GContext->itemRegistry), container);

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

    mv_python_function
    unstage(PyObject* self, PyObject* args, PyObject* kwargs)
    {

        PyObject* itemraw = nullptr;

        if (!Parse((GetParsers())["unstage"], args, kwargs, __FUNCTION__, &itemraw))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID item = GetIDFromPyObject(itemraw);

        UnstageItem((*GContext->itemRegistry), item);

        return GetPyNone();
    }

    mv_python_function
    show_item_debug(PyObject* self, PyObject* args, PyObject* kwargs)
    {

        PyObject* itemraw = nullptr;

        if (!Parse((GetParsers())["show_item_debug"], args, kwargs, __FUNCTION__, &itemraw))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID item = GetIDFromPyObject(itemraw);

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

    mv_python_function
    get_all_items(PyObject* self, PyObject* args, PyObject* kwargs)
    {

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        return ToPyList(GetAllItems((*GContext->itemRegistry)));
    }

    mv_python_function
    show_imgui_demo(PyObject* self, PyObject* args, PyObject* kwargs)
    {

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        GContext->itemRegistry->showImGuiDebug = true;
        return GetPyNone();
    }

    mv_python_function
    show_implot_demo(PyObject* self, PyObject* args, PyObject* kwargs)
    {

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        GContext->itemRegistry->showImPlotDebug = true;
        return GetPyNone();
    }

    mv_python_function
    get_windows(PyObject* self, PyObject* args, PyObject* kwargs)
    {

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        return ToPyList(GetWindows((*GContext->itemRegistry)));
    }

    mv_python_function
    add_alias(PyObject* self, PyObject* args, PyObject* kwargs)
    {

        const char* alias;
        PyObject* itemraw;

        if (!Parse((GetParsers())["add_alias"], args, kwargs, __FUNCTION__, &alias, &itemraw))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID item = GetIDFromPyObject(itemraw);

        AddAlias((*GContext->itemRegistry), alias, item);

        return GetPyNone();

    }

    mv_python_function
    remove_alias(PyObject* self, PyObject* args, PyObject* kwargs)
    {

        const char* alias;

        if (!Parse((GetParsers())["remove_alias"], args, kwargs, __FUNCTION__, &alias))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        RemoveAlias((*GContext->itemRegistry), alias);

        return GetPyNone();

    }

    mv_python_function
    does_alias_exist(PyObject* self, PyObject* args, PyObject* kwargs)
    {

        const char* alias;

        if (!Parse((GetParsers())["does_alias_exist"], args, kwargs, __FUNCTION__, &alias))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        bool result = DoesAliasExist((*GContext->itemRegistry), alias);

        return ToPyBool(result);
    }

    mv_python_function
    get_alias_id(PyObject* self, PyObject* args, PyObject* kwargs)
    {

        const char* alias;

        if (!Parse((GetParsers())["get_alias_id"], args, kwargs, __FUNCTION__, &alias))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID result = GetIdFromAlias((*GContext->itemRegistry), alias);

        return ToPyUUID(result);
    }

    mv_python_function
    get_aliases(PyObject* self, PyObject* args, PyObject* kwargs)
    {

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        std::vector<std::string> aliases;

        for (const auto& alias : GContext->itemRegistry->aliases)
            aliases.push_back(alias.first);

        return ToPyList(aliases);
    }

    mv_python_function
    bind_template_registry(PyObject* self, PyObject* args, PyObject* kwargs)
    {

        PyObject* itemraw;

        if (!Parse((GetParsers())["bind_template_registry"], args, kwargs, __FUNCTION__,
            &itemraw))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID item = GetIDFromPyObject(itemraw);


        if (item == 0)
            GContext->itemRegistry->boundedTemplateRegistry = nullptr;
        else
        {
            auto actualItem = GetRefItem((*GContext->itemRegistry), item);
            if (actualItem)
                GContext->itemRegistry->boundedTemplateRegistry = std::move(actualItem);
            else
            {
                mvThrowPythonError(mvErrorCode::mvItemNotFound, "bind_template_registry",
                    "Item not found: " + std::to_string(item), nullptr);
                return GetPyNone();
            }
        }

        return GetPyNone();
    }

    mv_python_function
    focus_item(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        PyObject* itemraw;

        if (!Parse((GetParsers())["focus_item"], args, kwargs, __FUNCTION__, &itemraw))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID item = GetIDFromPyObject(itemraw);

        if (!GContext->started)
        {
            for (size_t i = 0; i < GContext->itemRegistry->windowRoots.size(); i++)
            {
                if (GContext->itemRegistry->windowRoots[i]->_uuid == item)
                {
                    mvRef<mvAppItem> oldItem = GContext->itemRegistry->windowRoots.back();
                    GContext->itemRegistry->windowRoots[GContext->itemRegistry->windowRoots.size() - 1] = GContext->itemRegistry->windowRoots[i];
                    GContext->itemRegistry->windowRoots[i] = oldItem;
                    break;
                }
            }
        }

        mvAppItem* appitem = GetItem(*GContext->itemRegistry, item);

        if (appitem)
        {
            appitem->_focusNextFrame = true;
            if (auto parent = GetItemRoot(*GContext->itemRegistry, item))
                parent->_focusNextFrame = true;
        }
        else
            mvThrowPythonError(mvErrorCode::mvItemNotFound, "focus_item",
                "Item not found: " + std::to_string(item), nullptr);

        return GetPyNone();
    }

    mv_python_function
    get_item_info(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        PyObject* itemraw;

        if (!Parse((GetParsers())["get_item_info"], args, kwargs, __FUNCTION__, &itemraw))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID item = GetIDFromPyObject(itemraw);
        mvAppItem* appitem = GetItem((*GContext->itemRegistry), item);

        PyObject* pdict = PyDict_New();

        if (appitem)
        {

            std::string parserCommand = appitem->getCommand();

            auto children = GetItemChildren(*GContext->itemRegistry, appitem->_uuid);
            if (children.empty())
                PyDict_SetItemString(pdict, "children", mvPyObject(GetPyNone()));
            else
            {
                PyObject* pyChildren = PyDict_New();
                i32 i = 0;
                for (const auto& slot : children)
                {
                    PyDict_SetItem(pyChildren, ToPyInt(i), mvPyObject(ToPyList(slot)));
                    i++;
                }
                PyDict_SetItemString(pdict, "children", mvPyObject(pyChildren));
            }

            PyDict_SetItemString(pdict, "type", mvPyObject(ToPyString(GetEntityTypeString(appitem->getType()))));
            PyDict_SetItemString(pdict, "target", mvPyObject(ToPyInt(GetEntityTargetSlot(appitem->getType()))));

            if (appitem->_parentPtr)
                PyDict_SetItemString(pdict, "parent", mvPyObject(ToPyUUID(appitem->_parentPtr->_uuid)));
            else
                PyDict_SetItemString(pdict, "parent", mvPyObject(GetPyNone()));

            if (appitem->_theme)
                PyDict_SetItemString(pdict, "theme", mvPyObject(ToPyUUID(appitem->_theme->_uuid)));
            else
                PyDict_SetItemString(pdict, "theme", mvPyObject(GetPyNone()));

            if (appitem->_font)
                PyDict_SetItemString(pdict, "font", mvPyObject(ToPyUUID(appitem->_font->_uuid)));
            else
                PyDict_SetItemString(pdict, "font", mvPyObject(GetPyNone()));

            if (GetEntityDesciptionFlags(appitem->getType()) & MV_ITEM_DESC_CONTAINER)
                PyDict_SetItemString(pdict, "container", mvPyObject(ToPyBool(true)));
            else
                PyDict_SetItemString(pdict, "container", mvPyObject(ToPyBool(false)));

            i32 applicableState = GetApplicableState(appitem->getType());
            PyDict_SetItemString(pdict, "hover_handler_applicable", mvPyObject(ToPyBool(applicableState & MV_STATE_HOVER)));
            PyDict_SetItemString(pdict, "active_handler_applicable", mvPyObject(ToPyBool(applicableState & MV_STATE_ACTIVE)));
            PyDict_SetItemString(pdict, "focus_handler_applicable", mvPyObject(ToPyBool(applicableState & MV_STATE_FOCUSED)));
            PyDict_SetItemString(pdict, "clicked_handler_applicable", mvPyObject(ToPyBool(applicableState & MV_STATE_CLICKED)));
            PyDict_SetItemString(pdict, "visible_handler_applicable", mvPyObject(ToPyBool(applicableState & MV_STATE_VISIBLE)));
            PyDict_SetItemString(pdict, "edited_handler_applicable", mvPyObject(ToPyBool(applicableState & MV_STATE_EDITED)));
            PyDict_SetItemString(pdict, "activated_handler_applicable", mvPyObject(ToPyBool(applicableState & MV_STATE_ACTIVATED)));
            PyDict_SetItemString(pdict, "deactivated_handler_applicable", mvPyObject(ToPyBool(applicableState & MV_STATE_DEACTIVATED)));
            PyDict_SetItemString(pdict, "deactivatedae_handler_applicable", mvPyObject(ToPyBool(applicableState & MV_STATE_DEACTIVATEDAE)));
            PyDict_SetItemString(pdict, "toggled_open_handler_applicable", mvPyObject(ToPyBool(applicableState & MV_STATE_TOGGLED_OPEN)));
            PyDict_SetItemString(pdict, "resized_handler_applicable", mvPyObject(ToPyBool(applicableState & MV_STATE_RECT_SIZE)));

        }

        else
            mvThrowPythonError(mvErrorCode::mvItemNotFound, "get_item_info",
                "Item not found: " + std::to_string(item), nullptr);

        return pdict;
    }

    mv_python_function
    get_item_configuration(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        PyObject* itemraw;

        if (!Parse((GetParsers())["get_item_configuration"], args, kwargs, __FUNCTION__, &itemraw))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID item = GetIDFromPyObject(itemraw);
        mvAppItem* appitem = GetItem((*GContext->itemRegistry), item);

        PyObject* pdict = PyDict_New();

        if (appitem)
        {
            // config py objects
            mvPyObject py_filter_key = ToPyString(appitem->_filter);
            mvPyObject py_payload_type = ToPyString(appitem->_payloadType);
            mvPyObject py_label = ToPyString(appitem->_specifiedLabel);
            mvPyObject py_use_internal_label = ToPyBool(appitem->_useInternalLabel);
            mvPyObject py_source = ToPyUUID(appitem->_source);
            mvPyObject py_show = ToPyBool(appitem->_show);
            mvPyObject py_enabled = ToPyBool(appitem->_enabled);
            mvPyObject py_tracked = ToPyBool(appitem->_tracked);
            mvPyObject py_width = ToPyInt(appitem->_width);
            mvPyObject py_track_offset = ToPyFloat(appitem->_trackOffset);
            mvPyObject py_height = ToPyInt(appitem->_height);
            mvPyObject py_indent = ToPyInt((i32)appitem->_indent);

            PyDict_SetItemString(pdict, "filter_key", py_filter_key);
            PyDict_SetItemString(pdict, "payload_type", py_payload_type);
            PyDict_SetItemString(pdict, "label", py_label);
            PyDict_SetItemString(pdict, "use_internal_label", py_use_internal_label);
            PyDict_SetItemString(pdict, "source", py_source);
            PyDict_SetItemString(pdict, "show", py_show);
            PyDict_SetItemString(pdict, "enabled", py_enabled);
            PyDict_SetItemString(pdict, "tracked", py_tracked);
            PyDict_SetItemString(pdict, "width", py_width);
            PyDict_SetItemString(pdict, "track_offset", py_track_offset);
            PyDict_SetItemString(pdict, "height", py_height);
            PyDict_SetItemString(pdict, "indent", py_indent);

            if (appitem->_callback)
            {
                Py_XINCREF(appitem->_callback);
                PyDict_SetItemString(pdict, "callback", appitem->_callback);
            }
            else
                PyDict_SetItemString(pdict, "callback", GetPyNone());

            if (appitem->_dropCallback)
            {
                Py_XINCREF(appitem->_dropCallback);
                PyDict_SetItemString(pdict, "drop_callback", appitem->_dropCallback);
            }
            else
                PyDict_SetItemString(pdict, "drop_callback", GetPyNone());

            if (appitem->_dragCallback)
            {
                Py_XINCREF(appitem->_dragCallback);
                PyDict_SetItemString(pdict, "drag_callback", appitem->_dragCallback);
            }
            else
                PyDict_SetItemString(pdict, "drag_callback", GetPyNone());

            if (appitem->_user_data)
            {
                Py_XINCREF(appitem->_user_data);
                PyDict_SetItemString(pdict, "user_data", appitem->_user_data);
            }
            else
                PyDict_SetItemString(pdict, "user_data", GetPyNone());

            appitem->getSpecificConfiguration(pdict);
        }
        else
            mvThrowPythonError(mvErrorCode::mvItemNotFound, "get_item_configuration",
                "Item not found: " + std::to_string(item), nullptr);

        return pdict;
    }

    mv_python_function
    set_item_children(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        PyObject* itemraw;
        PyObject* sourceraw;
        i32 slot;

        if (!Parse((GetParsers())["set_item_children"], args, kwargs, __FUNCTION__,
            &itemraw, &sourceraw, &slot))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID item = GetIDFromPyObject(itemraw);
        mvUUID source = GetIDFromPyObject(sourceraw);
        mvAppItem* appitem = GetItem((*GContext->itemRegistry), item);

        auto& staging = GContext->itemRegistry->stagingRoots;

        b8 stage_found = false;
        mvRef<mvAppItem> staging_container = nullptr;

        for (auto& stage : staging)
        {
            if (stage->_uuid == source)
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
            auto& oldChildren = appitem->_children[slot];
            oldChildren.reserve(staging_container->_children[slot].size());
            oldChildren = std::move(staging_container->_children[slot]);
            for (auto& child : oldChildren)
            {
                child->_parent = item;
                child->_parentPtr = appitem;
            }
            UpdateLocations(appitem->_children, 4);
        }
        else
            mvThrowPythonError(mvErrorCode::mvItemNotFound, "set_item_children",
                "Item not found: " + std::to_string(item), nullptr);

        DeleteItem(*GContext->itemRegistry, source);

        return GetPyNone();
    }

    mv_python_function
    bind_item_font(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        PyObject* itemraw;
        PyObject* fontraw;

        if (!Parse((GetParsers())["bind_item_font"], args, kwargs, __FUNCTION__,
            &itemraw, &fontraw))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID item = GetIDFromPyObject(itemraw);
        mvUUID font = GetIDFromPyObject(fontraw);
        mvAppItem* appitem = GetItem((*GContext->itemRegistry), item);
        auto appfont = GetRefItem(*GContext->itemRegistry, font);

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

    mv_python_function
    bind_item_theme(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        PyObject* itemraw;
        PyObject* themeraw;

        if (!Parse((GetParsers())["bind_item_theme"], args, kwargs, __FUNCTION__,
            &itemraw, &themeraw))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID item = GetIDFromPyObject(itemraw);
        mvUUID theme = GetIDFromPyObject(themeraw);
        mvAppItem* appitem = GetItem((*GContext->itemRegistry), item);

        if (appitem)
        {
            if (theme == 0)
            {
                appitem->_theme = nullptr;
                return GetPyNone();
            }

            auto apptheme = GetRefItem(*GContext->itemRegistry, theme);

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

    mv_python_function
    bind_item_handler_registry(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        PyObject* itemraw;
        PyObject* regraw;

        if (!Parse((GetParsers())["bind_item_handler_registry"], args, kwargs, __FUNCTION__,
            &itemraw, &regraw))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID item = GetIDFromPyObject(itemraw);
        mvUUID reg = GetIDFromPyObject(regraw);
        mvAppItem* appitem = GetItem((*GContext->itemRegistry), item);

        if (appitem)
        {
            if (reg == 0)
            {
                appitem->_handlerRegistry = nullptr;
                return GetPyNone();
            }

            auto apptheme = GetRefItem(*GContext->itemRegistry, reg);

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

    mv_python_function
    reset_pos(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        PyObject* itemraw;

        if (!Parse((GetParsers())["reset_pos"], args, kwargs, __FUNCTION__,
            &itemraw))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID item = GetIDFromPyObject(itemraw);
        mvAppItem* appitem = GetItem((*GContext->itemRegistry), item);

        if (appitem)
            appitem->_dirtyPos = false;
        else
            mvThrowPythonError(mvErrorCode::mvItemNotFound, "reset_pos",
                "Item not found: " + std::to_string(item), nullptr);

        return GetPyNone();
    }

    mv_python_function
    get_item_state(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        PyObject* itemraw;

        if (!Parse((GetParsers())["get_item_state"], args, kwargs, __FUNCTION__, &itemraw))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID item = GetIDFromPyObject(itemraw);
        mvAppItem* appitem = GetItem((*GContext->itemRegistry), item);

        PyObject* pdict = PyDict_New();

        if (appitem)
            FillAppItemState(pdict, appitem->_state, GetApplicableState(appitem->getType()));
        else
            mvThrowPythonError(mvErrorCode::mvItemNotFound, "get_item_state",
                "Item not found: " + std::to_string(item), nullptr);

        return pdict;
    }

    mv_python_function
    get_item_types(PyObject* self, PyObject* args, PyObject* kwargs)
    {

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        PyObject* pdict = PyDict_New();
        #define X(el) PyDict_SetItemString(pdict, #el, PyLong_FromLong((int)mvAppItemType::el));
        MV_ITEM_TYPES
        #undef X

        return pdict;
    }

    mv_python_function
    configure_item(PyObject* self, PyObject* args, PyObject* kwargs)
    {

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID item = GetIDFromPyObject(PyTuple_GetItem(args, 0));
        mvAppItem* appitem = GetItem((*GContext->itemRegistry), item);

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

    mv_python_function
    get_value(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        PyObject* nameraw;

        if (!Parse((GetParsers())["get_value"], args, kwargs, __FUNCTION__, &nameraw))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID name = GetIDFromPyObject(nameraw);
        mvAppItem* item = GetItem(*GContext->itemRegistry, name);
        if (item)
            return item->getPyValue();

        return GetPyNone();
    }

    mv_python_function
    get_values(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        PyObject* items;

        if (!Parse((GetParsers())["get_values"], args, kwargs, __FUNCTION__, &items))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        auto aitems = ToUUIDVect(items);
        PyObject* pyvalues = PyList_New(aitems.size());

        for (size_t i = 0; i < aitems.size(); i++)
        {
            auto item = GetItem(*GContext->itemRegistry, aitems[i]);
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

    mv_python_function
    set_value(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        PyObject* nameraw;
        PyObject* value;

        if (!Parse((GetParsers())["set_value"], args, kwargs, __FUNCTION__, &nameraw, &value))
            return GetPyNone();

        if (value)
            Py_XINCREF(value);

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID name = GetIDFromPyObject(nameraw);

        mvAppItem* item = GetItem(*GContext->itemRegistry, name);
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

    mv_python_function
    set_item_alias(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        PyObject* itemraw;
        const char* alias;

        if (!Parse((GetParsers())["set_item_alias"], args, kwargs, __FUNCTION__,
            &itemraw, &alias))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID item = GetIDFromPyObject(itemraw);
        mvAppItem* appitem = GetItem((*GContext->itemRegistry), item);
        if (appitem)
            appitem->_alias = alias;
        return GetPyNone();
    }

    mv_python_function
    get_item_alias(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        PyObject* itemraw;

        if (!Parse((GetParsers())["get_item_alias"], args, kwargs, __FUNCTION__,
            &itemraw))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvUUID item = GetIDFromPyObject(itemraw);
        mvAppItem* appitem = GetItem((*GContext->itemRegistry), item);
        if (appitem)
            return ToPyString(appitem->_alias);
        return GetPyNone();
    }

    mv_python_function
    capture_next_item(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        PyObject* callable;

        if (!Parse((GetParsers())["capture_next_item"], args, kwargs, __FUNCTION__,
            &callable))
            return GetPyNone();

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        if(GContext->itemRegistry->captureCallback)
            Py_XDECREF(GContext->itemRegistry->captureCallback);
        
        Py_XINCREF(callable);
        if(callable == Py_None)
            GContext->itemRegistry->captureCallback = nullptr;
        else
            GContext->itemRegistry->captureCallback = callable;
        
        return GetPyNone();
    }
}
