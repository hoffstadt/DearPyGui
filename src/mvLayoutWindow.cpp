#include "mvLayoutWindow.h"
#include <imnodes.h>
#include "mvContext.h"
#include "mvItemRegistry.h"

static void
DebugItem(const char* label, const char* item) {
    ImGui::Text("%s", label);
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%s", item);
}

mvLayoutWindow::mvLayoutWindow()
{
    m_windowflags = ImGuiWindowFlags_NoSavedSettings;
}

void mvLayoutWindow::renderRootCategory(const char* category, std::vector<std::shared_ptr<mvAppItem>>& roots)
{

    const auto node_flags = ImGuiTreeNodeFlags_OpenOnArrow | (roots.empty() ? ImGuiTreeNodeFlags_Leaf : 0);

    ImGui::PushID(&roots);
    const auto expanded = ImGui::TreeNodeEx(category, node_flags);

    if (expanded)
    {
        for (auto& root : roots)
            renderTreeNode(root);
        ImGui::TreePop();
    }

    ImGui::PopID();
}

void mvLayoutWindow::renderTreeNode(std::shared_ptr<mvAppItem>& item)
{

    // build up flags for current node
    const auto node_flags = ImGuiTreeNodeFlags_OpenOnArrow
        | ((item->uuid == m_selectedItem) ? ImGuiTreeNodeFlags_Selected : 0)
        | (DearPyGui::GetEntityDesciptionFlags(item->type) & MV_ITEM_DESC_CONTAINER ? 0 : ImGuiTreeNodeFlags_Leaf);

    // render this node
    ImGui::PushID(item.get());
    std::string labelToShow = DearPyGui::GetEntityTypeString(item->type);
    if (!item->config.alias.empty())
        labelToShow = item->config.alias;
    else if (!item->config.specifiedLabel.empty())
        labelToShow = item->config.specifiedLabel;


    if (!_imguiFilter.PassFilter(labelToShow.c_str()) && _startFiltering)
    {
        ImGui::PopID();
        return;
    }

    const auto expanded = ImGui::TreeNodeEx(labelToShow.c_str(), node_flags);

    if (item->uuid == m_selectedItem)
        _startFiltering = true;
        
    // processing for selecting node
    if (ImGui::IsItemClicked())
    {
        m_selectedItem = item->uuid;
        _itemref = item;
        m_dirtyNodes = true;
    }

    if (!(DearPyGui::GetEntityDesciptionFlags(item->type) & MV_ITEM_DESC_CONTAINER))
    {
        if(expanded)
            ImGui::TreePop();
        ImGui::PopID();
        if (item->uuid == m_selectedItem)
            _startFiltering = false;
        return;
    }

    if (expanded)
    {

        int i = 0;
        for (auto& childrenSet : item->childslots)
        {

            std::string title = "Child Slot: " + std::to_string(i++);

            if (_slots)
            {

                if (ImGui::TreeNodeEx(title.c_str(), childrenSet.empty() ? ImGuiTreeNodeFlags_Leaf : 0))
                {
                    for (auto& children : childrenSet)
                        if(children)
                            renderTreeNode(children);
                    ImGui::TreePop();
                }
            }
            else
            {
                for (auto& children : childrenSet)
                    if(children)
                        renderTreeNode(children);
            }
        }
        ImGui::TreePop();
    }

    ImGui::PopID();

    if (item->uuid == m_selectedItem)
        _startFiltering = false;

}

void mvLayoutWindow::drawWidgets()
{
        
	mvUUID parentName = 0;

    if (_itemref == nullptr && GContext->itemRegistry->windowRoots.size() > 0)
        _itemref = GContext->itemRegistry->windowRoots[0];
    else if(_itemref == nullptr)
        return;

    if (_itemref->info.parentPtr)
        parentName = _itemref->info.parentPtr->uuid;

    // left side
    ImGui::BeginGroup();

    if (ImGui::ArrowButton("Move Up", ImGuiDir_Up))
    {
        std::lock_guard<std::recursive_mutex> lk(GContext->mutex);
        mvSubmitCallback([&]()
            {
                MoveItemUp(*GContext->itemRegistry, m_selectedItem);
            });
    }

    ImGui::SameLine();
    if (ImGui::ArrowButton("Move Down", ImGuiDir_Down))
    {
        std::lock_guard<std::recursive_mutex> lk(GContext->mutex);
        mvSubmitCallback([&]()
            {
                MoveItemDown(*GContext->itemRegistry, m_selectedItem);
            });
    }
    ImGui::SameLine();
    if (ImGui::Button("Delete"))
    {
        std::lock_guard<std::recursive_mutex> lk(GContext->mutex);
        mvSubmitCallback([&]()
            {
                DeleteItem(*GContext->itemRegistry, m_selectedItem, false);
                m_selectedItem = 0;
            });

        _itemref = nullptr;
        _itemref = GContext->itemRegistry->windowRoots[0];
    }
    ImGui::SameLine();
    if (ImGui::Button("Show"))
    {
        std::lock_guard<std::recursive_mutex> lk(GContext->mutex);
        mvAppItem* tempItem = GetItem(*GContext->itemRegistry, m_selectedItem);
        tempItem->config.show = true;
        tempItem->info.shownLastFrame = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Hide"))
    {
        std::lock_guard<std::recursive_mutex> lk(GContext->mutex);
        mvAppItem* tempItem = GetItem(*GContext->itemRegistry, m_selectedItem);
        tempItem->config.show = false;
        tempItem->info.hiddenLastFrame = true;
    }
    ImGui::SameLine();
    ImGui::Checkbox("Show Slots###layout", &_slots);

    ImGui::BeginChild("###layoutwindow", ImVec2(400, 0));
    static char ts[6] = "True";
    static char fs[6] = "False";

    std::string width = std::to_string(_itemref->config.width);
    std::string height = std::to_string(_itemref->config.height);

    std::string sizex = std::to_string(_itemref->state.rectSize.x);
    std::string sizey = std::to_string(_itemref->state.rectSize.y);

    ImGui::PushID(_itemref.get());
    DebugItem("Label:", _itemref->config.specifiedLabel.c_str());
    DebugItem("ID:", std::to_string(_itemref->uuid).c_str());
    DebugItem("Alias:", _itemref->config.alias.c_str());
    DebugItem("Type:", DearPyGui::GetEntityTypeString(_itemref->type));
    DebugItem("Filter:", _itemref->config.filter.c_str());
    DebugItem("Payload Type:", _itemref->config.payloadType.c_str());
    DebugItem("Location:", std::to_string(_itemref->info.location).c_str());
    DebugItem("Track Offset:", std::to_string(_itemref->config.trackOffset).c_str());
    DebugItem("Container:", DearPyGui::GetEntityDesciptionFlags(_itemref->type) & MV_ITEM_DESC_CONTAINER ? ts : fs);
    DebugItem("Width:", width.c_str());
    DebugItem("Height:", height.c_str());
    DebugItem("Size x:", sizex.c_str());
    DebugItem("Size y:", sizey.c_str());
    DebugItem("Show:", _itemref->config.show ? ts : fs);
    DebugItem("Enabled:", _itemref->config.enabled ? ts : fs);
    DebugItem("Tracked:", _itemref->config.tracked ? ts : fs);
    DebugItem("Callback:", _itemref->config.callback ? ts : fs);
    DebugItem("User Data:", _itemref->config.user_data ? ts : fs);
    DebugItem("Drop Callback:", _itemref->config.dropCallback ? ts : fs);
    DebugItem("Drag Callback:", _itemref->config.dragCallback ? ts : fs);

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Text("Bindings");
    ImGui::Separator();
    DebugItem("Theme Bound:", _itemref->theme ? ts : fs);
    DebugItem("Font Bound:", _itemref->font ? ts : fs);
    DebugItem("Handlers Bound:", _itemref->handlerRegistry ? ts : fs);

    int applicableState = DearPyGui::GetApplicableState(_itemref->type);
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Text("State");
    ImGui::Separator();
    if (applicableState & MV_STATE_VISIBLE) DebugItem("Item Visible:", IsItemVisible(_itemref->state, 1) ? ts : fs);
    if (applicableState & MV_STATE_HOVER) DebugItem("Item Hovered:", IsItemHovered(_itemref->state, 1) ? ts : fs);
    if (applicableState & MV_STATE_ACTIVE) DebugItem("Item Active:", IsItemActive(_itemref->state, 1) ? ts : fs);
    if (applicableState & MV_STATE_FOCUSED) DebugItem("Item Focused:", IsItemFocused(_itemref->state, 1) ? ts : fs);
    if (applicableState & MV_STATE_CLICKED)
    {
        DebugItem("Item Left Clicked:", IsItemLeftClicked(_itemref->state, 1) ? ts : fs);
        DebugItem("Item Right Clicked:", IsItemRightClicked(_itemref->state, 1) ? ts : fs);
        DebugItem("Item Middle Clicked:", IsItemMiddleClicked(_itemref->state, 1) ? ts : fs);
    }
    if (applicableState & MV_STATE_EDITED) DebugItem("Item Edited:", IsItemEdited(_itemref->state, 1) ? ts : fs);
    if (applicableState & MV_STATE_ACTIVATED) DebugItem("Item Activated:", IsItemActivated(_itemref->state, 1) ? ts : fs);
    if (applicableState & MV_STATE_DEACTIVATED) DebugItem("Item Deactivated:", IsItemDeactivated(_itemref->state, 1) ? ts : fs);
    if (applicableState & MV_STATE_DEACTIVATEDAE) DebugItem("Item DeactivatedAfterEdit:", IsItemDeactivatedAfterEdit(_itemref->state, 1) ? ts : fs);
    if (applicableState & MV_STATE_TOGGLED_OPEN) DebugItem("Item ToggledOpen:", IsItemToogledOpen(_itemref->state, 1) ? ts : fs);

    ImGui::PopID();
    ImGui::EndChild();

    ImGui::EndGroup();
    ImGui::SameLine();


    // right side
    ImGui::BeginGroup();
    _imguiFilter.Draw();
    _startFiltering = false;
    ImGui::BeginChild("TreeChild", ImVec2(-1.0f, -1.0f), ImGuiChildFlags_Border);
    renderRootCategory("Windows", GContext->itemRegistry->windowRoots);
    renderRootCategory("Themes", GContext->itemRegistry->themeRegistryRoots);
    renderRootCategory("Template Registries", GContext->itemRegistry->itemTemplatesRoots);
    renderRootCategory("Staging Containers", GContext->itemRegistry->stagingRoots);
    renderRootCategory("Texture Registries", GContext->itemRegistry->textureRegistryRoots);
    renderRootCategory("Font Registries", GContext->itemRegistry->fontRegistryRoots);
    renderRootCategory("Item Handler Registries", GContext->itemRegistry->itemHandlerRegistryRoots);
    renderRootCategory("Handler Registries", GContext->itemRegistry->handlerRegistryRoots);
    renderRootCategory("Value Registries", GContext->itemRegistry->valueRegistryRoots);
    renderRootCategory("Colormap Registries", GContext->itemRegistry->colormapRoots);
    renderRootCategory("File Dialogs", GContext->itemRegistry->filedialogRoots);
    renderRootCategory("Viewport Menubars", GContext->itemRegistry->viewportMenubarRoots);
    renderRootCategory("Viewport Drawlists", GContext->itemRegistry->viewportDrawlistRoots);
    ImGui::EndChild();
    ImGui::EndGroup();


}