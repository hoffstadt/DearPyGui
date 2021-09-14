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
            | ((item->_uuid == m_selectedItem) ? ImGuiTreeNodeFlags_Selected : 0)
            | (mvAppItem::DoesItemHaveFlag(item, MV_ITEM_DESC_CONTAINER) ? 0 : ImGuiTreeNodeFlags_Leaf);

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

        if (!mvAppItem::DoesItemHaveFlag(item, MV_ITEM_DESC_CONTAINER))
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
            _itemref = mvApp::GetApp()->getItemRegistry()._windowRoots[0].get();

		if (_itemref->_parentPtr)
			parentName = _itemref->_parentPtr->_uuid;

        // left side
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
				});

            _itemref = nullptr;
            _itemref = mvApp::GetApp()->getItemRegistry()._windowRoots[0].get();
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
        _itemref->renderDebugInfo();
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
