#include "mvLayoutWindow.h"
#include <imnodes.h>
#include "mvContext.h"
#include "mvItemRegistry.h"

namespace Marvel {

    mv_internal void
    DebugItem(const char* label, const char* item) {
        ImGui::Text("%s", label);
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%s", item);
    }

    mvLayoutWindow::mvLayoutWindow()
    {
        m_windowflags = ImGuiWindowFlags_NoSavedSettings;
    }

    void mvLayoutWindow::renderRootCategory(const char* category, std::vector<mvRef<mvAppItem>>& roots)
    {

        const auto node_flags = ImGuiTreeNodeFlags_OpenOnArrow | (roots.empty() ? ImGuiTreeNodeFlags_Leaf : 0);

        ImGui::PushID(&roots);
        const auto expanded = ImGui::TreeNodeEx(category, node_flags);

        if (expanded)
        {
            for (auto& root : roots)
                renderTreeNode(root.get());
            ImGui::TreePop();
        }

        ImGui::PopID();
    }

    void mvLayoutWindow::renderTreeNode(mvAppItem* item)
    {

        // build up flags for current node
        const auto node_flags = ImGuiTreeNodeFlags_OpenOnArrow
            | ((item->_uuid == m_selectedItem) ? ImGuiTreeNodeFlags_Selected : 0)
            | (item->getDescFlags() & MV_ITEM_DESC_CONTAINER ? 0 : ImGuiTreeNodeFlags_Leaf);

        // render this node
        ImGui::PushID(item);
        std::string labelToShow = item->getTypeString();
        if (!item->_alias.empty())
            labelToShow = item->_alias;
        else if (!item->_specifiedLabel.empty())
            labelToShow = item->_specifiedLabel;


        if (!_imguiFilter.PassFilter(labelToShow.c_str()) && _startFiltering)
        {
            ImGui::PopID();
            return;
        }

        const auto expanded = ImGui::TreeNodeEx(labelToShow.c_str(), node_flags);

        if (item->_uuid == m_selectedItem)
            _startFiltering = true;
        
        // processing for selecting node
        if (ImGui::IsItemClicked())
        {
            m_selectedItem = item->_uuid;
            _itemref = item;
            m_dirtyNodes = true;
        }

        if (!(item->getDescFlags() & MV_ITEM_DESC_CONTAINER))
        {
            if(expanded)
                ImGui::TreePop();
            ImGui::PopID();
            if (item->_uuid == m_selectedItem)
                _startFiltering = false;
            return;
        }

        if (expanded)
        {

            int i = 0;
            for (auto& childrenSet : item->_children)
            {

                std::string title = "Child Slot: " + std::to_string(i++);

                if (_slots)
                {

                    if (ImGui::TreeNodeEx(title.c_str(), childrenSet.empty() ? ImGuiTreeNodeFlags_Leaf : 0))
                    {
                        for (auto& children : childrenSet)
                            renderTreeNode(children.get());
                        ImGui::TreePop();
                    }
                }
                else
                {
                    for (auto& children : childrenSet)
                        renderTreeNode(children.get());
                }
            }
            ImGui::TreePop();
        }

        ImGui::PopID();

        if (item->_uuid == m_selectedItem)
            _startFiltering = false;

    }

    void mvLayoutWindow::drawWidgets()
    {
        
		mvUUID parentName = 0;

		if (_itemref == nullptr)
            _itemref = GContext->itemRegistry->windowRoots[0].get();

		if (_itemref->_parentPtr)
			parentName = _itemref->_parentPtr->_uuid;

        // left side
		ImGui::BeginGroup();

		if (ImGui::ArrowButton("Move Up", ImGuiDir_Up))
			GContext->callbackRegistry->submitCallback([&]()
				{
					MoveItemUp(*GContext->itemRegistry, m_selectedItem);
				});

		ImGui::SameLine();
		if (ImGui::ArrowButton("Move Down", ImGuiDir_Down))
			GContext->callbackRegistry->submitCallback([&]()
				{
					MoveItemDown(*GContext->itemRegistry, m_selectedItem);
				});
		ImGui::SameLine();
		if (ImGui::Button("Delete"))
		{
			GContext->callbackRegistry->submitCallback([&]()
				{
					DeleteItem(*GContext->itemRegistry, m_selectedItem, false);
					m_selectedItem = 0;
				});

            _itemref = nullptr;
            _itemref = GContext->itemRegistry->windowRoots[0].get();
		}
		ImGui::SameLine();
        if (ImGui::Button("Show"))
        {
            mvAppItem* tempItem = GetItem(*GContext->itemRegistry, m_selectedItem);
            tempItem->_show = true;
            tempItem->_shownLastFrame = true;
        }
		ImGui::SameLine();
        if (ImGui::Button("Hide"))
        {
            mvAppItem* tempItem = GetItem(*GContext->itemRegistry, m_selectedItem);
            tempItem->_show = false;
            tempItem->_hiddenLastFrame = true;
        }
        ImGui::SameLine();
        ImGui::Checkbox("Show Slots###layout", &_slots);

        ImGui::BeginChild("###layoutwindow", ImVec2(400, 0));
        mv_local_persist char ts[6] = "True";
        mv_local_persist char fs[6] = "False";

        std::string width = std::to_string(_itemref->_width);
        std::string height = std::to_string(_itemref->_height);

        std::string sizex = std::to_string(_itemref->_state.rectSize.x);
        std::string sizey = std::to_string(_itemref->_state.rectSize.y);

        ImGui::PushID(_itemref);
        DebugItem("Label:", _itemref->_specifiedLabel.c_str());
        DebugItem("ID:", std::to_string(_itemref->_uuid).c_str());
        DebugItem("Alias:", _itemref->_alias.c_str());
        DebugItem("Type:", _itemref->getTypeString());
        DebugItem("Filter:", _itemref->_filter.c_str());
        DebugItem("Payload Type:", _itemref->_payloadType.c_str());
        DebugItem("Location:", std::to_string(_itemref->_location).c_str());
        DebugItem("Track Offset:", std::to_string(_itemref->_trackOffset).c_str());
        DebugItem("Container:", _itemref->getDescFlags() & MV_ITEM_DESC_CONTAINER ? ts : fs);
        DebugItem("Width:", width.c_str());
        DebugItem("Height:", height.c_str());
        DebugItem("Size x:", sizex.c_str());
        DebugItem("Size y:", sizey.c_str());
        DebugItem("Show:", _itemref->_show ? ts : fs);
        DebugItem("Enabled:", _itemref->_enabled ? ts : fs);
        DebugItem("Tracked:", _itemref->_tracked ? ts : fs);
        DebugItem("Callback:", _itemref->_callback ? ts : fs);
        DebugItem("User Data:", _itemref->_user_data ? ts : fs);
        DebugItem("Drop Callback:", _itemref->_dropCallback ? ts : fs);
        DebugItem("Drag Callback:", _itemref->_dragCallback ? ts : fs);

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Text("Bindings");
        ImGui::Separator();
        DebugItem("Theme Bound:", _itemref->_theme ? ts : fs);
        DebugItem("Font Bound:", _itemref->_font ? ts : fs);
        DebugItem("Handlers Bound:", _itemref->_handlerRegistry ? ts : fs);

        int applicableState = _itemref->getApplicableState();
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Text("State");
        ImGui::Separator();
        if (applicableState & MV_STATE_VISIBLE) DebugItem("Item Visible:", IsItemVisible(_itemref->_state, 1) ? ts : fs);
        if (applicableState & MV_STATE_HOVER) DebugItem("Item Hovered:", IsItemHovered(_itemref->_state, 1) ? ts : fs);
        if (applicableState & MV_STATE_ACTIVE) DebugItem("Item Active:", IsItemActive(_itemref->_state, 1) ? ts : fs);
        if (applicableState & MV_STATE_FOCUSED) DebugItem("Item Focused:", IsItemFocused(_itemref->_state, 1) ? ts : fs);
        if (applicableState & MV_STATE_CLICKED)
        {
            DebugItem("Item Left Clicked:", IsItemLeftClicked(_itemref->_state, 1) ? ts : fs);
            DebugItem("Item Right Clicked:", IsItemRightClicked(_itemref->_state, 1) ? ts : fs);
            DebugItem("Item Middle Clicked:", IsItemMiddleClicked(_itemref->_state, 1) ? ts : fs);
        }
        if (applicableState & MV_STATE_EDITED) DebugItem("Item Edited:", IsItemEdited(_itemref->_state, 1) ? ts : fs);
        if (applicableState & MV_STATE_ACTIVATED) DebugItem("Item Activated:", IsItemActivated(_itemref->_state, 1) ? ts : fs);
        if (applicableState & MV_STATE_DEACTIVATED) DebugItem("Item Deactivated:", IsItemDeactivated(_itemref->_state, 1) ? ts : fs);
        if (applicableState & MV_STATE_DEACTIVATEDAE) DebugItem("Item DeactivatedAfterEdit:", IsItemDeactivatedAfterEdit(_itemref->_state, 1) ? ts : fs);
        if (applicableState & MV_STATE_TOGGLED_OPEN) DebugItem("Item ToggledOpen:", IsItemToogledOpen(_itemref->_state, 1) ? ts : fs);

        ImGui::PopID();
        ImGui::EndChild();

		ImGui::EndGroup();
		ImGui::SameLine();

        // right side
        ImGui::BeginGroup();
        _imguiFilter.Draw();
        _startFiltering = false;
        ImGui::BeginChild("TreeChild", ImVec2(-1.0f, -1.0f), true);
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
        ImGui::EndChild();
        ImGui::EndGroup();


    }

}
