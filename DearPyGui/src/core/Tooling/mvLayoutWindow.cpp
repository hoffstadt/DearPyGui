#include "mvLayoutWindow.h"
#include <imnodes.h>
#include "mvApp.h"
#include "mvItemRegistry.h"

namespace Marvel {

    static void DebugItem(const char* label, const char* item) {
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
            | ((item->getUUID() == m_selectedItem) ? ImGuiTreeNodeFlags_Selected : 0)
            | (mvAppItem::DoesItemHaveFlag(item, MV_ITEM_DESC_CONTAINER) ? 0 : ImGuiTreeNodeFlags_Leaf);

        // render this node
        ImGui::PushID(item);
        std::string labelToShow = item->getTypeString();
        if (!item->_alias.empty())
            labelToShow = item->_alias;
        else if (!item->_specificedlabel.empty())
            labelToShow = item->_specificedlabel;


        if (!_imguiFilter.PassFilter(labelToShow.c_str()) && _startFiltering)
        {
            ImGui::PopID();
            return;
        }

        const auto expanded = ImGui::TreeNodeEx(labelToShow.c_str(), node_flags);

        if (item->getUUID() == m_selectedItem)
            _startFiltering = true;
        
        // processing for selecting node
        if (ImGui::IsItemClicked())
        {
            m_selectedItem = item->_uuid;
            _itemref = item;
            m_dirtyNodes = true;
        }

        if (!mvAppItem::DoesItemHaveFlag(item, MV_ITEM_DESC_CONTAINER))
        {
            if(expanded)
                ImGui::TreePop();
            ImGui::PopID();
            if (item->getUUID() == m_selectedItem)
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

        if (item->getUUID() == m_selectedItem)
            _startFiltering = false;

    }

    void mvLayoutWindow::drawWidgets()
    {
        // left side
        static char ts[6] = "True";
        static char fs[6] = "False";

		mvUUID parentName = 0;

		if (_itemref == nullptr)
            _itemref = mvApp::GetApp()->getItemRegistry()._windowRoots[0].get();

		if (_itemref->_parentPtr)
			parentName = _itemref->_parentPtr->_uuid;

		std::string width = std::to_string(_itemref->_width);
		std::string height = std::to_string(_itemref->_height);

		std::string sizex = std::to_string(_itemref->getState().getItemRectSize().x);
		std::string sizey = std::to_string(_itemref->getState().getItemRectSize().y);

		ImGui::BeginGroup();

		if (ImGui::ArrowButton("Move Up", ImGuiDir_Up))
			mvApp::GetApp()->getCallbackRegistry().submitCallback([&]()
				{
					mvApp::GetApp()->getItemRegistry().moveItemUp(m_selectedItem);
				});

		ImGui::SameLine();
		if (ImGui::ArrowButton("Move Down", ImGuiDir_Down))
			mvApp::GetApp()->getCallbackRegistry().submitCallback([&]()
				{
					mvApp::GetApp()->getItemRegistry().moveItemDown(m_selectedItem);
				});
		ImGui::SameLine();
		if (ImGui::Button("Delete"))
		{
			mvApp::GetApp()->getCallbackRegistry().submitCallback([&]()
				{
					mvApp::GetApp()->getItemRegistry().deleteItem(m_selectedItem, false);
					m_selectedItem = 0;
                    _itemref = nullptr;
				});
		}
		ImGui::SameLine();
		if (ImGui::Button("Show"))
			mvApp::GetApp()->getItemRegistry().getItem(m_selectedItem)->show();
		ImGui::SameLine();
		if (ImGui::Button("Hide"))
			mvApp::GetApp()->getItemRegistry().getItem(m_selectedItem)->hide();
        ImGui::SameLine();
        ImGui::Checkbox("Show Slots###layout", &_slots);

        ImGui::BeginChild("###layoutwindow", ImVec2(400, 0));
		DebugItem("Label:", _itemref->_specificedlabel.c_str());
		DebugItem("ID:", std::to_string(_itemref->getUUID()).c_str());
		DebugItem("Alias:", _itemref->getAlias().c_str());
		DebugItem("Filter:", _itemref->getFilter().c_str());
		DebugItem("Payload Type:", _itemref->_payloadType.c_str());
        DebugItem("Location:", std::to_string(_itemref->_location).c_str());
        DebugItem("Track Offset:", std::to_string(_itemref->_trackOffset).c_str());
		DebugItem("Container:", mvAppItem::DoesItemHaveFlag(_itemref, MV_ITEM_DESC_CONTAINER) ? ts : fs);
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
		DebugItem("Disabled Theme Bound:", _itemref->_disabledTheme ? ts : fs);
		DebugItem("Font Bound:", _itemref->_disabledTheme ? ts : fs);
		DebugItem("Handlers Bound:", _itemref->_handlerRegistry ? ts : fs);

        int applicableState = _itemref->getApplicableState();
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Text("State");
        ImGui::Separator();
        if(applicableState & MV_STATE_VISIBLE) DebugItem("Item Visible:", _itemref->getState().isItemVisible(1) ? ts : fs);
		if(applicableState & MV_STATE_HOVER) DebugItem("Item Hovered:", _itemref->getState().isItemHovered(1) ? ts : fs);
		if(applicableState & MV_STATE_ACTIVE) DebugItem("Item Active:", _itemref->getState().isItemActive(1) ? ts : fs);
		if(applicableState & MV_STATE_FOCUSED) DebugItem("Item Focused:", _itemref->getState().isItemFocused(1) ? ts : fs);
		if(applicableState & MV_STATE_CLICKED) DebugItem("Item Left Clicked:", _itemref->getState().isItemLeftClicked(1) ? ts : fs);
		if(applicableState & MV_STATE_CLICKED) DebugItem("Item Right Clicked:", _itemref->getState().isItemRightClicked(1) ? ts : fs);
		if(applicableState & MV_STATE_CLICKED) DebugItem("Item Middle Clicked:", _itemref->getState().isItemMiddleClicked(1) ? ts : fs);
		if(applicableState & MV_STATE_EDITED) DebugItem("Item Edited:", _itemref->getState().isItemEdited(1) ? ts : fs);
		if(applicableState & MV_STATE_ACTIVATED) DebugItem("Item Activated:", _itemref->getState().isItemActivated(1) ? ts : fs);
		if(applicableState & MV_STATE_DEACTIVATED) DebugItem("Item Deactivated:", _itemref->getState().isItemDeactivated(1) ? ts : fs);
		if(applicableState & MV_STATE_DEACTIVATEDAE) DebugItem("Item DeactivatedAfterEdit:", _itemref->getState().isItemDeactivatedAfterEdit(1) ? ts : fs);
		if(applicableState & MV_STATE_TOGGLED_OPEN) DebugItem("Item ToggledOpen:", _itemref->getState().isItemToogledOpen(1) ? ts : fs);
        ImGui::EndChild();
		ImGui::EndGroup();
		ImGui::SameLine();

        // right side

        ImGui::BeginGroup();
        _imguiFilter.Draw();
        _startFiltering = false;
        ImGui::BeginChild("TreeChild", ImVec2(-1.0f, -1.0f), true);
        renderRootCategory("Windows", mvApp::GetApp()->getItemRegistry()._windowRoots);
        renderRootCategory("Themes", mvApp::GetApp()->getItemRegistry()._themeRegistryRoots);
        renderRootCategory("Template Registries", mvApp::GetApp()->getItemRegistry()._itemTemplatesRoots);
        renderRootCategory("Staging Containers", mvApp::GetApp()->getItemRegistry()._stagingRoots);
        renderRootCategory("Texture Registries", mvApp::GetApp()->getItemRegistry()._textureRegistryRoots);
        renderRootCategory("Font Registries", mvApp::GetApp()->getItemRegistry()._fontRegistryRoots);
        renderRootCategory("Item Handler Registries", mvApp::GetApp()->getItemRegistry()._itemHandlerRegistryRoots);
        renderRootCategory("Handler Registries", mvApp::GetApp()->getItemRegistry()._handlerRegistryRoots);
        renderRootCategory("Value Registries", mvApp::GetApp()->getItemRegistry()._valueRegistryRoots);
        renderRootCategory("Colormap Registries", mvApp::GetApp()->getItemRegistry()._colormapRoots);
        renderRootCategory("File Dialogs", mvApp::GetApp()->getItemRegistry()._filedialogRoots);
        renderRootCategory("Viewport Menubars", mvApp::GetApp()->getItemRegistry()._viewportMenubarRoots);
        ImGui::EndChild();
        ImGui::EndGroup();


    }

}
