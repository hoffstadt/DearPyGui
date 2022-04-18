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
PushParent(mvItemRegistry& registry, mvAppItem* item)
{
    registry.containers.push(item);
}

mv_internal mvAppItem*
TopParent(mvItemRegistry& registry)
{
    if (!registry.containers.empty())
        return registry.containers.top();
    return nullptr;
}

mv_internal void
CacheItem(mvItemRegistry& registry, mvAppItem* item)
{
    if (DearPyGui::GetEntityDesciptionFlags(item->type) & MV_ITEM_DESC_CONTAINER)
    {
        registry.cachedContainersID[registry.cachedContainerIndex] = item->uuid;
        registry.cachedContainersPTR[registry.cachedContainerIndex] = item;
        registry.cachedContainerIndex++;
        if (registry.cachedContainerIndex == registry.CachedContainerCount)
            registry.cachedContainerIndex = 0;
    }

    registry.cachedItemsID[registry.cachedItemsIndex] = item->uuid;
    registry.cachedItemsPTR[registry.cachedItemsIndex] = item;
    registry.cachedItemsIndex++;
    if (registry.cachedItemsIndex == registry.CachedContainerCount)
        registry.cachedItemsIndex = 0;
}

mv_internal void
UpdateChildLocations(std::vector<mvRef<mvAppItem>>* children, i32 slots)
{
    for (i32 i = 0; i < slots; i++)
    {
        i32 index = 0;
        for (auto& child : children[i])
        {
            if (child)
            {
                child->info.location = index;
                index++;
            }
        }
    }
}

mv_internal b8
DeleteChild(mvAppItem* item, mvUUID uuid)
{
    for (auto& childset : item->childslots)
    {
        b8 childfound = false;
        b8 itemDeleted = false;

        for (auto& child : childset)
        {
            if (child)
            {
                if (child->uuid == uuid)
                {
                    childfound = true;
                    break;
                }

                itemDeleted = DeleteChild(child.get(), uuid);
                if (itemDeleted)
                    break;
            }
        }

        if (childfound)
        {
            std::vector<mvRef<mvAppItem>> oldchildren = childset;

            childset.clear();

            for (auto& child : oldchildren)
            {
                if (child)
                {
                    if (child->uuid == uuid)
                    {
                        itemDeleted = true;
                        DearPyGui::OnChildRemoved(item, child);
                        continue;
                    }
                }

                childset.push_back(child);
            }
        }

        if (itemDeleted)
        {
            UpdateChildLocations(item->childslots, 4);
            return true;
        }
    }

    return false;
}

mv_internal b8
DeleteRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID uuid)
{
    b8 deletedItem = false;

    // try to delete build-in item
    for (auto& root : roots)
    {
        deletedItem = DeleteChild(root.get(), uuid);
        if (deletedItem)
            break;
    }

    if (deletedItem)
        return true;

    b8 rootDeleting = false;

    // check if attempting to delete a window
    for (auto& window : roots)
    {
        if (window->uuid == uuid)
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
            if (root->uuid == uuid)
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

mv_internal mvRef<mvAppItem>
StealChild(mvAppItem* item, mvUUID uuid)
{
    mvRef<mvAppItem> stolenChild = nullptr;

    for (auto& childset : item->childslots)
    {
        b8 childfound = false;

        for (auto& child : childset)
        {
            if (!child)
                continue;

            if (child->uuid == uuid)
            {
                childfound = true;
                break;
            }

            if (DearPyGui::GetEntityDesciptionFlags(child->type) & MV_ITEM_DESC_CONTAINER)
            {
                stolenChild = StealChild(child.get(), uuid);
                if (stolenChild)
                    return stolenChild;
            }
        }

        if (childfound)
        {
            std::vector<mvRef<mvAppItem>> oldchildren = childset;

            childset.clear();

            for (auto& child : oldchildren)
            {
                if (child->uuid == uuid)
                {
                    stolenChild = child;
                    DearPyGui::OnChildRemoved(item, child);
                    continue;
                }

                childset.push_back(child);
            }

            UpdateChildLocations(item->childslots, 4);

            return stolenChild;
        }


        //return static_cast<mvRef<mvAppItem>>(CreateRef<mvButton>("Not possible"));
    }

    return stolenChild;
}

mv_internal b8
MoveRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID uuid, mvRef<mvAppItem>& item)
{
        
    for (auto& window : roots)
    {
        item = StealChild(window.get(), uuid);
        if (item)
            return true;
    }
    return false;
}

mv_internal b8
MoveChildUp(mvAppItem* item, mvUUID uuid)
{
    b8 found = false;
    i32 index = 0;

    for (auto& childset : item->childslots)
    {
        // check children
        for (size_t i = 0; i < childset.size(); i++)
        {

            if (childset[i]->uuid == uuid)
            {
                found = true;
                index = (i32)i;
                break;
            }

            if (DearPyGui::GetEntityDesciptionFlags(childset[i]->type) & MV_ITEM_DESC_CONTAINER)
            {
                found = MoveChildUp(childset[i].get(), uuid);
                if (found)
                    return true;
            }

        }

        if (found)
        {
            if (index > 0)
            {
                auto upperitem = childset[index - 1];
                auto loweritem = childset[index];

                childset[index] = upperitem;
                childset[index - 1] = loweritem;

                UpdateChildLocations(item->childslots, 4);
            }

            return true;
        }
    }

    return false;
}

mv_internal b8
MoveUpRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID uuid)
{
    for (auto& window : roots)
    {
        if (MoveChildUp(window.get(), uuid))
            return true;
    }
    return false;
}

mv_internal b8
MoveChildDown(mvAppItem* item, mvUUID uuid)
{
    b8 found = false;
    size_t index = 0;

    for (auto& childset : item->childslots)
    {
        // check children
        for (size_t i = 0; i < childset.size(); i++)
        {

            if (childset[i]->uuid == uuid)
            {
                found = true;
                index = i;
                break;
            }

            if (DearPyGui::GetEntityDesciptionFlags(childset[i]->type) & MV_ITEM_DESC_CONTAINER)
            {
                found = MoveChildDown(childset[i].get(), uuid);
                if (found)
                    return true;
            }

        }

        if (found)
        {
            if (index < childset.size() - 1)
            {
                auto upperitem = childset[index];
                auto loweritem = childset[index + 1];

                childset[index] = loweritem;
                childset[index + 1] = upperitem;

                UpdateChildLocations(item->childslots, 4);
            }

            return true;
        }


    }

    return false;
}

mv_internal b8
MoveDownRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID uuid)
{
    for (auto& window : roots)
    {
        if (MoveChildDown(window.get(), uuid))
            return true;
    }
    return false;
}

mv_internal b8
AddRuntimeChild(mvAppItem* rootitem, mvUUID parent, mvUUID before, mvRef<mvAppItem> item)
{
    if (before == 0 && parent == 0)
        return false;

    for (auto& children : rootitem->childslots)
    {
        //this is the container, add item to end.
        if (before == 0)
        {

            if (rootitem->uuid == parent)
            {
                i32 targetSlot = DearPyGui::GetEntityTargetSlot(item->type);
                item->info.location = (i32)rootitem->childslots[targetSlot].size();
                rootitem->childslots[targetSlot].push_back(item);
                DearPyGui::OnChildAdded(rootitem, item);
                item->info.parentPtr = rootitem;
                item->config.parent = rootitem->uuid;
                return true;
            }

            // check children
            for (auto& childslot : rootitem->childslots)
            {
                for (auto& child : childslot)
                {

                    if (!child)
                        continue;

                    if (DearPyGui::GetEntityDesciptionFlags(child->type) & MV_ITEM_DESC_CONTAINER
                        || DearPyGui::GetEntityDesciptionFlags(item->type) & MV_ITEM_DESC_HANDLER)
                    {
                        // parent found
                        if (AddRuntimeChild(child.get(), parent, before, item))
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
                if (!child)
                    continue;

                if (child->uuid == before)
                {
                    beforeFound = true;
                    break;
                }
            }


            // after item is in this container
            if (beforeFound)
            {
                item->info.parentPtr = rootitem;

                std::vector<mvRef<mvAppItem>> oldchildren = children;
                children.clear();

                for (auto& child : oldchildren)
                {
                    if (!child)
                        continue;

                    if (child->uuid == before)
                    {
                        children.push_back(item);
                        DearPyGui::OnChildAdded(rootitem, item);
                    }
                    children.push_back(child);

                }

                UpdateChildLocations(rootitem->childslots, 4);

                return true;
            }
        }

        // check children
        for (auto& child : children)
        {
            if (!child)
                continue;

            if (DearPyGui::GetEntityDesciptionFlags(child->type) & MV_ITEM_DESC_CONTAINER
                || DearPyGui::GetEntityDesciptionFlags(item->type) & MV_ITEM_DESC_HANDLER)
            {
                // parent found
                if (AddRuntimeChild(child.get(), parent, before, item))
                    return true;
            }
        }

    };

    return false;
}

mv_internal b8
AddRuntimeChildRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID parent, mvUUID before, mvRef<mvAppItem> item)
{
    for (auto& root : roots)
    {
        if (AddRuntimeChild(root.get(), parent, before, item))
            return true;
    }
    return false;

}

mv_internal b8
AddChildAfter(mvAppItem* parent, mvUUID prev, mvRef<mvAppItem> item)
{
    if (prev == 0)
        return false;

    b8 prevFound = false;

    // check children
    for (auto& childslot : parent->childslots)
    {
        for (auto& child : childslot)
        {
            if (!child)
                continue;

            if (child->uuid == prev)
            {
                item->info.parentPtr = parent;
                prevFound = true;
                break;
            }

        }
    }

    // prev item is in this container
    if (prevFound)
    {
        i32 targetSlot = DearPyGui::GetEntityTargetSlot(item->type);
        std::vector<mvRef<mvAppItem>> oldchildren = parent->childslots[targetSlot];
        parent->childslots[targetSlot].clear();

        for (auto& child : oldchildren)
        {
            parent->childslots[targetSlot].push_back(child);
            if (child->uuid == prev)
            {
                parent->childslots[targetSlot].push_back(item);
                DearPyGui::OnChildAdded(parent, item);
            }
        }

        return true;
    }


    // check children
    for (auto& childslot : parent->childslots)
    {
        for (auto& child : childslot)
        {
            if (!child)
                continue;

            // parent found
            if (AddChildAfter(child.get(), prev, item))
                return true;
        }
    }

    return false;
}

mv_internal b8
AddItemAfterRoot(std::vector<mvRef<mvAppItem>>& roots, mvUUID prev, mvRef<mvAppItem> item)
{
    for (auto& root : roots)
    {
        if (AddChildAfter(root.get(), prev, item))
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
    mvAppItem* parentitem = TopParent(registry);
    item->info.parentPtr = parentitem;
    i32 targetSlot = DearPyGui::GetEntityTargetSlot(item->type);
    item->info.location = (i32)parentitem->childslots[targetSlot].size();
    parentitem->childslots[targetSlot].push_back(item);
    DearPyGui::OnChildAdded(parentitem, item);
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

    if (item->type == mvAppItemType::mvWindowAppItem) registry.windowRoots.push_back(item);
    if (item->type == mvAppItemType::mvColorMapRegistry) registry.colormapRoots.push_back(item);
    if (item->type == mvAppItemType::mvFileDialog) registry.filedialogRoots.push_back(item);
    if (item->type == mvAppItemType::mvStage) registry.stagingRoots.push_back(item);
    if (item->type == mvAppItemType::mvViewportMenuBar) registry.viewportMenubarRoots.push_back(item);
    if (item->type == mvAppItemType::mvFontRegistry) registry.fontRegistryRoots.push_back(item);
    if (item->type == mvAppItemType::mvHandlerRegistry) registry.handlerRegistryRoots.push_back(item);
    if (item->type == mvAppItemType::mvTextureRegistry) registry.textureRegistryRoots.push_back(item);
    if (item->type == mvAppItemType::mvValueRegistry) registry.valueRegistryRoots.push_back(item);
    if (item->type == mvAppItemType::mvTheme) registry.themeRegistryRoots.push_back(item);
    if (item->type == mvAppItemType::mvTemplateRegistry) registry.itemTemplatesRoots.push_back(item);
    if (item->type == mvAppItemType::mvItemHandlerRegistry) registry.itemHandlerRegistryRoots.push_back(item);
    if (item->type == mvAppItemType::mvViewportDrawlist) registry.viewportDrawlistRoots.push_back(item);

    return true;
}

mv_internal mvAppItem*
GetChild(mvAppItem* rootitem, mvUUID uuid)
{

    if (rootitem->uuid == uuid)
        return rootitem;

    if (rootitem->config.searchLast)
    {
        if (rootitem->config.searchDelayed)
            rootitem->config.searchDelayed = false;
        else
        {
            rootitem->config.searchDelayed = true;
            DelaySearch(*GContext->itemRegistry, rootitem);
        }
    }

    for (auto& childset : rootitem->childslots)
    {
        for (auto& childitem : childset)
        {
            if (!childitem)
                continue;

            if (childitem->uuid == uuid)
                return childitem.get();

            auto child = GetChild(childitem.get(), uuid);
            if (child)
                return child;
        }
    }

    return nullptr;
}

mv_internal mvRef<mvAppItem>
GetChildRef(mvAppItem* rootitem, mvUUID uuid)
{

    for (auto& childset : rootitem->childslots)
    {
        for (auto& item : childset)
        {

            if (!item)
                continue;

            if (item->uuid == uuid)
                return item;

            auto child = GetChildRef(item.get(), uuid);
            if (child)
                return child;
        }
    }

    return nullptr;
}

mv_internal mvAppItem*
GetItemRoot(mvItemRegistry& registry, std::vector<mvRef<mvAppItem>>& roots, mvUUID uuid)
{
    for (auto& root : roots)
    {
        if (root->uuid == uuid)
        {
            CacheItem(registry, root.get());
            return root.get();
        }

        mvAppItem* child = GetChild(root.get(), uuid);
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
        if (root->uuid == uuid)
            return root;

        auto child = GetChildRef(root.get(), uuid);
        if (child)
            return child;
    }

    return nullptr;
}

mv_internal void
RemoveDebugWindow(mvItemRegistry& registry, mvUUID uuid)
{
    // check if debug window exists
    b8 exists = false;
    for (const auto& debug : registry.debugWindows)
    {
        if (debug->uuid == uuid)
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
        if (debug->uuid != uuid)
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
        if (item->info.parentPtr)
        {
            mvAppItem* currentAncestor = item->info.parentPtr;
            while (!(DearPyGui::GetEntityDesciptionFlags(currentAncestor->type) & MV_ITEM_DESC_ROOT))
                currentAncestor = currentAncestor->info.parentPtr;

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
                item->childslots[slot].clear();
            }
            else
            {
                for(size_t i = 0; i < 4; i++)
                {
                    item->childslots[i].clear();
                }
            }
                
            if(item->type == mvAppItemType::mvTable)
                static_cast<mvTable*>(item)->onChildrenRemoved();

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
        if(registry.capturedItem->uuid == uuid)
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
        if (root->config.show)
            root->draw(nullptr, 0.0f, 0.0f);
    }

    if (mvToolManager::GetFontManager()._newDefault)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.FontDefault = nullptr;

        for (auto& root : registry.fontRegistryRoots)
        {
            for (auto& font : root->childslots[1])
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
        if (root->config.show)
            root->draw(nullptr, 0.0f, 0.0f);
    }

    for (auto& root : registry.textureRegistryRoots)
        root->draw(nullptr, 0.0f, 0.0f);

    for (auto& root : registry.filedialogRoots)
        root->draw(nullptr, 0.0f, 0.0f);

    for (auto& root : registry.themeRegistryRoots)
    {
        if(root->config.show)
            ((mvTheme*)root.get())->push_theme_components();
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
        if (root->config.show)
            ((mvTheme*)root.get())->pop_theme_components();
    }

    for (auto& root : registry.debugWindows)
    {
        ImGui::SetNextWindowSize(ImVec2(500.0f, 500.0f), ImGuiCond_FirstUseEver);

        if (!ImGui::Begin(root->config.specifiedLabel.c_str(), &root->info.showDebug))
        {
            ImGui::End();

            if (!root->info.showDebug)
                RemoveDebugWindow(*GContext->itemRegistry, root->uuid);
            return;
        }

        mv_local_persist char ts[6] = "True";
        mv_local_persist char fs[6] = "False";

        std::string width = std::to_string(root->config.width);
        std::string height = std::to_string(root->config.height);

        std::string sizex = std::to_string(root->state.rectSize.x);
        std::string sizey = std::to_string(root->state.rectSize.y);

        ImGui::PushID(root.get());
        DebugItem("Label:", root->config.specifiedLabel.c_str());
        DebugItem("ID:", std::to_string(root->uuid).c_str());
        DebugItem("Alias:", root->config.alias.c_str());
        DebugItem("Type:", DearPyGui::GetEntityTypeString(root->type));
        DebugItem("Filter:", root->config.filter.c_str());
        DebugItem("Payload Type:", root->config.payloadType.c_str());
        DebugItem("Location:", std::to_string(root->info.location).c_str());
        DebugItem("Track Offset:", std::to_string(root->config.trackOffset).c_str());
        DebugItem("Container:", DearPyGui::GetEntityDesciptionFlags(root->type) & MV_ITEM_DESC_CONTAINER ? ts : fs);
        DebugItem("Width:", width.c_str());
        DebugItem("Height:", height.c_str());
        DebugItem("Size x:", sizex.c_str());
        DebugItem("Size y:", sizey.c_str());
        DebugItem("Show:", root->config.show ? ts : fs);
        DebugItem("Enabled:", root->config.enabled ? ts : fs);
        DebugItem("Tracked:", root->config.tracked ? ts : fs);
        DebugItem("Callback:", root->config.callback ? ts : fs);
        DebugItem("User Data:", root->config.user_data ? ts : fs);
        DebugItem("Drop Callback:", root->config.dropCallback ? ts : fs);
        DebugItem("Drag Callback:", root->config.dragCallback ? ts : fs);

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Text("Bindings");
        ImGui::Separator();
        DebugItem("Theme Bound:", root->theme ? ts : fs);
        DebugItem("Font Bound:", root->font ? ts : fs);
        DebugItem("Handlers Bound:", root->handlerRegistry ? ts : fs);

        i32 applicableState = DearPyGui::GetApplicableState(root->type);
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Text("State");
        ImGui::Separator();
        if (applicableState & MV_STATE_VISIBLE) DebugItem("Item Visible:", IsItemVisible(root->state, 1) ? ts : fs);
        if (applicableState & MV_STATE_HOVER) DebugItem("Item Hovered:", IsItemHovered(root->state, 1) ? ts : fs);
        if (applicableState & MV_STATE_ACTIVE) DebugItem("Item Active:", IsItemActive(root->state, 1) ? ts : fs);
        if (applicableState & MV_STATE_FOCUSED) DebugItem("Item Focused:", IsItemFocused(root->state, 1) ? ts : fs);
        if (applicableState & MV_STATE_CLICKED)
        {
            DebugItem("Item Left Clicked:", IsItemLeftClicked(root->state, 1) ? ts : fs);
            DebugItem("Item Right Clicked:", IsItemRightClicked(root->state, 1) ? ts : fs);
            DebugItem("Item Middle Clicked:", IsItemMiddleClicked(root->state, 1) ? ts : fs);
        }
        if (applicableState & MV_STATE_EDITED) DebugItem("Item Edited:", IsItemEdited(root->state, 1) ? ts : fs);
        if (applicableState & MV_STATE_ACTIVATED) DebugItem("Item Activated:", IsItemActivated(root->state, 1) ? ts : fs);
        if (applicableState & MV_STATE_DEACTIVATED) DebugItem("Item Deactivated:", IsItemDeactivated(root->state, 1) ? ts : fs);
        if (applicableState & MV_STATE_DEACTIVATEDAE) DebugItem("Item DeactivatedAfterEdit:", IsItemDeactivatedAfterEdit(root->state, 1) ? ts : fs);
        if (applicableState & MV_STATE_TOGGLED_OPEN) DebugItem("Item ToggledOpen:", IsItemToogledOpen(root->state, 1) ? ts : fs);

        ImGui::PopID();

        ImGui::End();

        if (!root->info.showDebug)
            RemoveDebugWindow(*GContext->itemRegistry, root->uuid);
    }

}

void 
ResetTheme(mvItemRegistry& registry)
{
    for (auto& root : registry.themeRegistryRoots)
        root->config.show = false;
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
        if(registry.capturedItem->uuid == uuid)
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
        mvAppItem* child = GetChild(delayedItem, uuid);
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
        if(registry.capturedItem->uuid == uuid)
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

    if (item->type == mvAppItemType::mvWindowAppItem)
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
        mvRunCallback(registry.captureCallback, registry.capturedItem->uuid, nullptr, nullptr);
        Py_XDECREF(registry.captureCallback);
        registry.captureCallback = nullptr;
        return true;
    }

    if (DearPyGui::GetEntityDesciptionFlags(item->type) & MV_ITEM_DESC_HANDLER && parent == 0)
        parent = item->config.parent;

    if (item == nullptr)
        return false;

    // check if item is ok
    if (!item->state.ok)
        return false;

    //---------------------------------------------------------------------------
    // STEP 0: updata "last" information
    //---------------------------------------------------------------------------
    if (DearPyGui::GetEntityDesciptionFlags(item->type) & MV_ITEM_DESC_ROOT)
    {
        registry.lastRootAdded = item->uuid;
        registry.lastContainerAdded = item->uuid;
    }
    else if (DearPyGui::GetEntityDesciptionFlags(item->type) & MV_ITEM_DESC_CONTAINER)
        registry.lastContainerAdded = item->uuid;

    registry.lastItemAdded = item->uuid;

    CacheItem(registry, item.get());

    //---------------------------------------------------------------------------
    // STEP 1: check if an item with this name exists (NO LONGER NEEDED)
    //---------------------------------------------------------------------------
    //if (getItem(item->uuid))
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
    if (DearPyGui::GetEntityDesciptionFlags(item->type) & MV_ITEM_DESC_ROOT)
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
            parentPtr = beforeItem->info.parentPtr;
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
    const std::vector<std::pair<std::string, i32>>* allowableParents = &DearPyGui::GetAllowableParents(item->type);

    std::string acceptableParentTypes;

    for (const auto& compatibleParent : *allowableParents)
    {
        acceptableParentTypes.append(compatibleParent.first + "\n");
        if ((i32)parentPtr->type == compatibleParent.second)
        {
            isParentCompatible = true;
            break;
        }
    }

    if (!isParentCompatible)
    {
        if (allowableParents->empty())
        {
            mvThrowPythonError(mvErrorCode::mvIncompatibleParent, GetEntityCommand(item->type),
                "Incompatible parent. Item does can not have a parent.", item.get());
            return false;
        }

        if (!(*allowableParents)[0].second == (i32)mvAppItemType::All)
        {

            mvThrowPythonError(mvErrorCode::mvIncompatibleParent, GetEntityCommand(item->type),
                "Incompatible parent. Acceptable parents include:\t" + acceptableParentTypes, item.get());

            assert(false);
            return false;
        }
    }

    //---------------------------------------------------------------------------
    // STEP 6: check if parent accepts our item (this isn't duplicate STEP 3)
    //---------------------------------------------------------------------------
    b8 amICompatible = false;
    const std::vector<std::pair<std::string, i32>>* allowableChildren = &DearPyGui::GetAllowableChildren(parentPtr->type);

    std::string acceptableChildTypes;

    for (const auto& compatibleChildren : *allowableChildren)
    {
        acceptableChildTypes.append(compatibleChildren.first + "\n");
        if ((i32)item->type == compatibleChildren.second)
        {
            amICompatible = true;
            break;
        }
    }

    if (!amICompatible)
    {
        if (allowableChildren->empty())
        {
            mvThrowPythonError(mvErrorCode::mvIncompatibleChild, GetEntityCommand(parentPtr->type),
                "Incompatible child. Item does not accept children.", parentPtr);
            return false;
        }

        if (!(*allowableChildren)[0].second == (i32)mvAppItemType::All)
        {

            mvThrowPythonError(mvErrorCode::mvIncompatibleChild, GetEntityCommand(parentPtr->type),
                "Incompatible child. Acceptable children include:\t" + acceptableChildTypes, parentPtr);

            assert(false);
            return false;
        }
    }

    //---------------------------------------------------------------------------
    // STEP 7: add items who require "after" adding (tooltip)
    //---------------------------------------------------------------------------
    if (item->type == mvAppItemType::mvTooltip)
    {
        if (parentPtr->info.parentPtr->type == mvAppItemType::mvTable)
        {
            parentPtr->info.parentPtr->childslots[2][parentPtr->info.location] = item;
            return true;
        }
        else
        {
            return AddItemAfter(registry, parent, item);
        }
    }

    //---------------------------------------------------------------------------
    // STEP 8: handle "before" and "after" style adding
    //---------------------------------------------------------------------------
    if (technique == AddTechnique::BEFORE || technique == AddTechnique::PARENT)
        return AddRuntimeChild(parentPtr, parent, before, item); // same for run/compile time

    //---------------------------------------------------------------------------
    // STEP 9: handle "stack" style adding
    //---------------------------------------------------------------------------
    if(GContext->started)
        return AddRuntimeChild(parentPtr, parentPtr->uuid, 0, item);
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
        item->config.alias = alias;
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
        item->config.alias.clear();

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