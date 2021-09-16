#include "mvLayoutWindow.h"
#include <imnodes.h>
#include "mvContext.h"
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

        if (!item->getDescFlags() & MV_ITEM_DESC_CONTAINER)
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
        _itemref->renderDebugInfo();
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
