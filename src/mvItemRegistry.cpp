#include "mvItemRegistry.h"
#include "mvProfiler.h"
#include "mvContext.h"
#include "mvAppItemCommons.h"
#include "mvPyUtils.h"
#include "mvToolManager.h"
#include "mvFontManager.h"

mvItemRegistry::mvItemRegistry()
{
    // We seldom need to do a GetItem(0), and an explicit check for uuid == 0 in GetItem
    // slows it down.  Instead of this, we add a fake entry to the allItems map,
    // mapping 0 to nullptr.  This entry will never go away because map items are only
    // removed in the mvAppItem destructor.
    allItems[0] = nullptr;
}

static b8
DoesAliasExist(mvItemRegistry& registry, const std::string& alias)
{
    if (registry.aliases.count(alias) != 0)
        return true;
    return false;
}

static void
DebugItem(const char* label, const char* item) {
    ImGui::Text("%s", label);
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%s", item);
}

static void
PushParent(mvItemRegistry& registry, mvAppItem* item)
{
    registry.containers.push(item);
}

static mvAppItem*
TopParent(mvItemRegistry& registry)
{
    if (!registry.containers.empty())
        return registry.containers.top();
    return nullptr;
}

static void
UpdateChildLocations(std::vector<std::shared_ptr<mvAppItem>>* children, i32 slots)
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

static b8
AddRuntimeChild(mvAppItem* rootitem, mvUUID before, std::shared_ptr<mvAppItem> item)
{
    if (before == 0)
    {
        i32 targetSlot = DearPyGui::GetEntityTargetSlot(item->type);
        item->info.location = (i32)rootitem->childslots[targetSlot].size();
        rootitem->childslots[targetSlot].push_back(item);
        item->info.parentPtr = rootitem;
        item->config.parent = rootitem->uuid;
        DearPyGui::OnChildAdded(rootitem, item);
        return true;
    }
    else
    {
        for (auto& childslot : rootitem->childslots)
        {
            for (auto it = childslot.begin(); it != childslot.end(); ++it)
            {
                auto child = *it;
                if (!child)
                    continue;

                if (child->uuid == before)
                {
                    childslot.insert(it, item);
                    item->info.parentPtr = rootitem;
                    item->config.parent = rootitem->uuid;
                    // TODO: this one can be optimized by updating locations in the tail
                    // of the `childslot` only
                    UpdateChildLocations(&childslot, 1);
                    DearPyGui::OnChildAdded(rootitem, item);
                    return true;
                }
            }
        }
    }
    IM_ASSERT(false && "We could not find `before` in its parent's childslots.");
    return false;
}

static b8
AddItemAfter(mvItemRegistry& registry, mvUUID prev, std::shared_ptr<mvAppItem> item)
{

    mvAppItem* prevItem = GetItem(registry, prev);
    if (!prevItem)
    {
        IM_ASSERT(false && "AddItemAfter could not find prev.");
        return false;
    }

    mvAppItem* parent = prevItem->info.parentPtr;
    if (parent)
    {
        for (auto& childset : parent->childslots)
        {
            for (auto it = childset.begin(); it != childset.end(); ++it)
            {
                if (it->get() == prevItem)
                {
                    ++it;
                    childset.insert(it, item);
                    item->info.parentPtr = parent;
                    item->config.parent = parent->uuid;
                    UpdateChildLocations(&childset, 1);
                    DearPyGui::OnChildAdded(parent, item);
                    return true;
                }
            }
        }
        // This must never happen
        IM_ASSERT(false && "Could not find item in parent's childslots.");
        return false;
    }
    
    assert(false);
    return false;
}

static std::vector<std::shared_ptr<mvAppItem>>&
GetRootsList(mvItemRegistry& registry, mvAppItemType type)
{
    switch (type)
    {
        case mvAppItemType::mvWindowAppItem: return registry.windowRoots;
        case mvAppItemType::mvColorMapRegistry: return registry.colormapRoots;
        case mvAppItemType::mvFileDialog: return registry.filedialogRoots;
        case mvAppItemType::mvStage: return registry.stagingRoots;
        case mvAppItemType::mvViewportMenuBar: return registry.viewportMenubarRoots;
        case mvAppItemType::mvFontRegistry: return registry.fontRegistryRoots;
        case mvAppItemType::mvHandlerRegistry: return registry.handlerRegistryRoots;
        case mvAppItemType::mvTextureRegistry: return registry.textureRegistryRoots;
        case mvAppItemType::mvValueRegistry: return registry.valueRegistryRoots;
        case mvAppItemType::mvTheme: return registry.themeRegistryRoots;
        case mvAppItemType::mvTemplateRegistry: return registry.itemTemplatesRoots;
        case mvAppItemType::mvItemHandlerRegistry: return registry.itemHandlerRegistryRoots;
        case mvAppItemType::mvViewportDrawlist: return registry.viewportDrawlistRoots;
    }
    // We must never end up here
    IM_ASSERT(false && "A root container does not have a corresponding list in GetRootsList.");
    return registry.windowRoots;
}

static void
AddRoot(mvItemRegistry& registry, std::shared_ptr<mvAppItem> item)
{
    auto& roots = GetRootsList(registry, item->type);
    roots.push_back(item);
}

static void
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

    std::vector<std::shared_ptr<mvAppItem>> oldWindows = registry.debugWindows;
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

// Disconnects the item from its parent by removing it from the parent's childslot.
// For root items, removes it from the "parent" list of roots.
// When called on an item that's in the tree, must never return false.  The return
// value is only intended to catch internal errors when the item registry is broken.
// Note: it does *not* reset item->info.parentPtr!
static bool
RemoveItemFromTree(mvItemRegistry& registry, mvAppItem* item)
{
    mvAppItem* parent = item->info.parentPtr;
    if (parent)
    {
        for (auto& childset : parent->childslots)
        {
            for (auto it = childset.begin(); it != childset.end(); ++it)
            {
                if (it->get() == item)
                {
                    auto child = *it;
                    childset.erase(it);
                    // TODO: this one can be optimized by updating locations in the tail
                    // of the `childset` only
                    UpdateChildLocations(&childset, 1);
                    DearPyGui::OnChildRemoved(parent, child);
                    return true;
                }
            }
        }
        // This must never happen
        IM_ASSERT(false && "Could not find item in parent's childslots.");
        return false;
    }

    // Now this is a root item
    IM_ASSERT((DearPyGui::GetEntityDesciptionFlags(item->type) & MV_ITEM_DESC_ROOT) && "Item is neither a child nor a root.");

    auto& roots = GetRootsList(registry, item->type);
    for (auto it = roots.begin(); it != roots.end(); ++it)
    {
        if (it->get() == item)
        {
            roots.erase(it);
            return true;
        }
    }

    // This must never happen
    IM_ASSERT(false && "Could not find a root item in the corresponding roots list.");
    return false;
}

b8
DeleteItem(mvItemRegistry& registry, mvUUID uuid, b8 childrenOnly, i32 slot)
{
    mvAppItem* item = GetItem(registry, uuid);
    if (!item)
    {
        mvThrowPythonError(mvErrorCode::mvItemNotFound, "delete_item",
            "Item not found: " + std::to_string(uuid), nullptr);
        return false;
    }

    // delete item's children only
    if (childrenOnly)
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

        return true;
    }

    // See if it is the captured item
    if (registry.capturedItem && registry.capturedItem.get() == item)
    {
        registry.capturedItem = nullptr;
        return true;
    }

    // Now this is just a regular item in the tree
    RemoveDebugWindow(registry, uuid);
    if (!RemoveItemFromTree(registry, item))
    {
        mvThrowPythonError(mvErrorCode::mvItemNotFound, "delete_item",
            "Unable to delete item: " + std::to_string(uuid), item);
        return false;
    }

    return true;
}

b8
MoveItem(mvItemRegistry& registry, mvUUID uuid, mvUUID parent, mvUUID before)
{

    std::shared_ptr<mvAppItem> child = nullptr;

    if (registry.capturedItem && registry.capturedItem->uuid == uuid)
    {
        child = registry.capturedItem;
        registry.capturedItem = nullptr;
    }
    else
    {
        child = GetRefItem(registry, uuid);
        if (child == nullptr)
        {
            mvThrowPythonError(mvErrorCode::mvItemNotFound, "move_item",
                "Item not found: " + std::to_string(uuid), nullptr);
            return false;
        }
        // Remove it from its current location in the tree
        RemoveItemFromTree(registry, child.get());
    }

    // Resetting parent item or otherwise AddItemWithRuntimeChecks might fall back
    // to the previously stored value.
    child->config.parent = 0;

    // TODO: AddItemWithRuntimeChecks will refuse to add items with state.ok == false,
    // which includes fonts and textures that failed to load.  Fix this.
    return AddItemWithRuntimeChecks(registry, child, parent, before);
}

b8
MoveItemUp(mvItemRegistry& registry, mvUUID uuid)
{
    mvAppItem* item = GetItem(registry, uuid);
    if (!item)
    {
        mvThrowPythonError(mvErrorCode::mvItemNotFound, "move_item_up",
            "Item not found: " + std::to_string(uuid), nullptr);
        return false;
    }

    mvAppItem* parent = item->info.parentPtr;
    if (parent)
    {
        for (auto& childset : parent->childslots)
        {
            for (auto it = childset.begin(), prev_it = childset.begin(); it != childset.end(); ++it)
            {
                if (it->get() == item)
                {
                    if (it != childset.begin())
                    {
                        // Swapping locations so that we don't need to recalculate them on all children
                        std::swap((*prev_it)->info.location, (*it)->info.location);
                        // Now swap the child items themselves
                        std::iter_swap(prev_it, it);
                    }
                    return true;
                }
                prev_it = it;
            }
        }
        // This must never happen
        IM_ASSERT(false && "Could not find item in parent's childslots.");
        mvThrowPythonError(mvErrorCode::mvItemNotFound, "move_item_up",
            "Unable to move item: " + std::to_string(uuid), item);
        return false;
    }

    // This is probably a root item, but we don't support moving roots up/down.
    // It has never worked on roots, and actually has little sense.
    mvThrowPythonError(mvErrorCode::mvItemNotFound, "move_item_up",
        "move_item_up for root items is not supported", item);
    return false;
}

b8
MoveItemDown(mvItemRegistry& registry, mvUUID uuid)
{
    mvAppItem* item = GetItem(registry, uuid);
    if (!item)
    {
        mvThrowPythonError(mvErrorCode::mvItemNotFound, "move_item_down",
            "Item not found: " + std::to_string(uuid), nullptr);
        return false;
    }

    mvAppItem* parent = item->info.parentPtr;
    if (parent)
    {
        for (auto& childset : parent->childslots)
        {
            for (auto it = childset.begin(); it != childset.end(); ++it)
            {
                if (it->get() == item)
                {
                    auto next_it = std::next(it);
                    if (next_it != childset.end())
                    {
                        // Swapping locations so that we don't need to recalculate them on all children
                        std::swap((*it)->info.location, (*next_it)->info.location);
                        // Now swap the child items themselves
                        std::iter_swap(it, next_it);
                    }
                    return true;
                }
            }
        }
        // This must never happen
        IM_ASSERT(false && "Could not find item in parent's childslots.");
        mvThrowPythonError(mvErrorCode::mvItemNotFound, "move_item_down",
            "Unable to move item: " + std::to_string(uuid), item);
        return false;
    }

    // This is probably a root item, but we don't support moving roots up/down.
    // It has never worked on roots, and actually has little sense.
    mvThrowPythonError(mvErrorCode::mvItemNotFound, "move_item_down",
        "move_item_down for root items is not supported", item);
    return false;
}

b8
ReorderChildren(mvItemRegistry& registry, mvUUID parent, i32 slot, const std::vector<mvUUID>& new_order)
{
	mvAppItem* parentItem = GetItem(registry, parent);
	if (parentItem == nullptr)
	{
		mvThrowPythonError(mvErrorCode::mvItemNotFound, "reorder_items",
			"Item not found: " + std::to_string(parent), nullptr);
		return false;
	}

	std::vector<std::shared_ptr<mvAppItem>>& children = parentItem->childslots[slot];

	std::vector<std::shared_ptr<mvAppItem>> newchildren;
	newchildren.reserve(children.size());

	// todo: better sorting algorithm
	for (const auto& item : new_order)
	{
		for (auto& child : children)
		{
			if (child->uuid == item)
			{
				newchildren.emplace_back(child);
				break;
			}
		}
	}
	children = newchildren;
    UpdateChildLocations(&children, 1);
	return true;
}

void 
RenderItemRegistry(mvItemRegistry& registry)
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

    for (auto& root : registry.themeRegistryRoots)
    {
        if(root->config.show)
            ((mvTheme*)root.get())->push_theme_components();
    }

    for (auto& root : registry.filedialogRoots)
        root->draw(nullptr, 0.0f, 0.0f);

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

        static char ts[6] = "True";
        static char fs[6] = "False";

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
        DebugItem("User Data:", *(root->config.user_data) ? ts : fs);
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

mvAppItem* 
GetItem(mvItemRegistry& registry, mvUUID uuid)
{
    // check captured
    if (registry.capturedItem && registry.capturedItem->uuid == uuid)
        return registry.capturedItem.get();

    auto found = registry.allItems.find(uuid);
    return found != registry.allItems.end()? found->second : nullptr;
}

std::shared_ptr<mvAppItem>
GetRefItem(mvItemRegistry& registry, mvUUID uuid)
{
    mvAppItem* item = GetItem(registry, uuid);
    return item? item->shared_from_this() : nullptr;
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

    assert(false && "Item is not a window.");
    return nullptr;
}

void 
ClearItemRegistry(mvItemRegistry& registry)
{
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

b8
AddItemWithRuntimeChecks(mvItemRegistry& registry, std::shared_ptr<mvAppItem> item, mvUUID parent, mvUUID before)
{

    if(registry.captureCallback)
    {
  
        // this is a unique situation in that the caller always has the GIL
        registry.capturedItem = item;
        // resetting captureCallback in advance in order to avoid recursion (if the callback
        // attempts to add another item or move an item)
        mvPyObject captureCallback(std::move(registry.captureCallback));
        registry.captureCallback = nullptr;
        mvRunCallback(captureCallback, nullptr, registry.capturedItem->uuid);
        return true;
    }

    if (item == nullptr)
        return false;

    // check if item is ok
    if (!item->state.ok)
        return false;

    mvPySafeLockGuard lk(GContext->mutex);

    //---------------------------------------------------------------------------
    // STEP 0: update "last" information
    //---------------------------------------------------------------------------
    if (DearPyGui::GetEntityDesciptionFlags(item->type) & MV_ITEM_DESC_ROOT)
    {
        registry.lastRootAdded = item->uuid;
        registry.lastContainerAdded = item->uuid;
    }
    else if (DearPyGui::GetEntityDesciptionFlags(item->type) & MV_ITEM_DESC_CONTAINER)
        registry.lastContainerAdded = item->uuid;

    registry.lastItemAdded = item->uuid;

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

    //---------------------------------------------------------------------------
    // STEP 2: handle root case
    //---------------------------------------------------------------------------
    if (DearPyGui::GetEntityDesciptionFlags(item->type) & MV_ITEM_DESC_ROOT)
    {
        AddRoot(registry, item);
        return true;
    }

    //---------------------------------------------------------------------------
    // STEP 3: attempt to deduce parent
    //---------------------------------------------------------------------------
    if (item->type == mvAppItemType::mvTooltip && parent == 0)
        parent = item->config.parent;

    mvAppItem* parentPtr = nullptr;
    if (before > 0)
    {
        // Adding it before the "before" item - let's find it!
        mvAppItem* beforeItem = GetItem(registry, before);
        if (beforeItem == nullptr)
        {
            mvThrowPythonError(mvErrorCode::mvItemNotFound, "add_*", "Item not found: " + std::to_string(parent), nullptr);
            IM_ASSERT(false && "The 'before' item could not be found.");
            return false;
        }
        // We'll need the parent anyway in order to check compatibility
        parentPtr = beforeItem->info.parentPtr;
    }

    else if (parent >= MV_START_UUID)
    {
        // Append to children of the specified parent
        parentPtr = GetItem(registry, parent);
    }

    else if (parent == 0)
    {
        // Take the parent from stack and add to the end
        parentPtr = TopParent(registry);
    }

    // reserved uuid case
    else
    {
        parentPtr = GetItem(registry, parent);
        if (!parentPtr)
        {
            // revert to stack operation (reserved uuid not used)
            parentPtr = TopParent(registry);
        }
    }

    //---------------------------------------------------------------------------
    // STEP 4: check if parent was deduced
    //---------------------------------------------------------------------------
    if (parentPtr == nullptr)
    {
        mvThrowPythonError(mvErrorCode::mvParentNotDeduced, "add_*", "Parent could not be deduced.", item.get());
        IM_ASSERT(false && "Parent could not be deduced.");
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

        if ((*allowableParents)[0].second != (i32) mvAppItemType::All)
        {

            mvThrowPythonError(mvErrorCode::mvIncompatibleParent, GetEntityCommand(item->type),
                "Incompatible parent. Acceptable parents include:\t" + acceptableParentTypes, item.get());

            IM_ASSERT(false && "Incompatible parent.");
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

        if ((*allowableChildren)[0].second != (i32)mvAppItemType::All)
        {

            mvThrowPythonError(mvErrorCode::mvIncompatibleChild, GetEntityCommand(parentPtr->type),
                "Incompatible child. Acceptable children include:\t" + acceptableChildTypes, parentPtr);

            IM_ASSERT(false && "Incompatible child.");
            return false;
        }
    }

    //---------------------------------------------------------------------------
    // STEP 7: add items who require "after" adding (tooltip)
    //---------------------------------------------------------------------------
    if (item->type == mvAppItemType::mvTooltip)
    {
        mvAppItem* prevItemParent = parentPtr->info.parentPtr;
        if (prevItemParent && prevItemParent->type == mvAppItemType::mvTable)
        {
            prevItemParent->childslots[2][parentPtr->info.location] = item;
            return true;
        }
        else
        {
            return AddItemAfter(registry, parent, item);
        }
    }

    //---------------------------------------------------------------------------
    // STEP 8: Either append it to parent's children or insert before the "before"
    //---------------------------------------------------------------------------
    return AddRuntimeChild(parentPtr, before, item);
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