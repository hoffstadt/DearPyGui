#include "mvLayoutWindow.h"
#include "mvApp.h"
#include "mvItemRegistry.h"

namespace Marvel {

    static void DebugItem(const char* label, const char* item)
    {
        ImGui::Text("%s", label);
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%s", item);
    }

    static void DebugItem(const char* label, float x)
    {
        ImGui::Text("%s", label);
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%s", std::to_string(x).c_str());
    }

    static void DebugItem(const char* label, float x, float y)
    {
        ImGui::Text("%s", label);
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "%s", (std::to_string(x) + ", " + std::to_string(y)).c_str());
    }

    mvLayoutWindow::mvLayoutWindow()
    {
        m_windowflags = ImGuiWindowFlags_NoSavedSettings;
    }

    void mvLayoutWindow::renderNode(mvAppItem* item)
    {

        // build up flags for current node
        const auto node_flags = ImGuiTreeNodeFlags_OpenOnArrow
            | ((item->getName() == m_selectedItem) ? ImGuiTreeNodeFlags_Selected : 0)
            | (mvAppItem::DoesItemHaveFlag(item, MV_ITEM_DESC_CONTAINER) ? 0 : ImGuiTreeNodeFlags_Leaf);

        // render this node
        ImGui::PushID(item);
        const auto expanded = ImGui::TreeNodeEx(m_showLabels ? item->m_specificedlabel.c_str() : 
            item->m_name.c_str(), node_flags);
        
        // processing for selecting node
        if (ImGui::IsItemClicked())
            m_selectedItem = item->m_name;

        if (!mvAppItem::DoesItemHaveFlag(item, MV_ITEM_DESC_CONTAINER))
        {
            if(expanded)
                ImGui::TreePop();
            ImGui::PopID();
            return;
        }

        if (expanded)
        {

            int i = 0;
            for (auto& childrenSet : item->m_children)
            {

                std::string title = "Child Slot: " + std::to_string(i++);
  
                if (ImGui::TreeNodeEx(title.c_str(), childrenSet.empty() ? ImGuiTreeNodeFlags_Leaf : 0))
                {
                    for (auto& children : childrenSet)
                        renderNode(children.get());
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }

        ImGui::PopID();

    }

    void mvLayoutWindow::drawWidgets()
    {
        // left side
        static char ts[6] = "True";
        static char fs[6] = "False";

		auto selectedItem = mvApp::GetApp()->getItemRegistry().getItem(m_selectedItem);
		std::string parentName;

		if (selectedItem == nullptr)
			selectedItem = mvApp::GetApp()->getItemRegistry().getRoots()[0];

		if (selectedItem->m_parentPtr)
			parentName = selectedItem->m_parentPtr->m_name;

		std::string width = std::to_string(selectedItem->m_width);
		std::string height = std::to_string(selectedItem->m_height);

		std::string sizex = std::to_string(selectedItem->getState().getItemRectSize().x);
		std::string sizey = std::to_string(selectedItem->getState().getItemRectSize().y);

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
					m_selectedItem = "";
				});
		}
		ImGui::SameLine();
		if (ImGui::Button("Show"))
			mvApp::GetApp()->getItemRegistry().getItem(m_selectedItem)->show();
		ImGui::SameLine();
		if (ImGui::Button("Hide"))
			mvApp::GetApp()->getItemRegistry().getItem(m_selectedItem)->hide();

		ImGui::PushItemWidth(200);
		DebugItem("Item Name:", m_selectedItem.c_str());
		DebugItem("Item Label:", selectedItem->m_specificedlabel.c_str());
		//DebugItem("Item Type:", selectedItem->getStringType().c_str());
		DebugItem("Container:", mvAppItem::DoesItemHaveFlag(selectedItem.get(), MV_ITEM_DESC_CONTAINER) ? ts : fs);
		DebugItem("Item Parent:", parentName.c_str());
		DebugItem("Item Width:", width.c_str());
		DebugItem("Item Height:", height.c_str());
		DebugItem("Item Size x:", sizex.c_str());
		DebugItem("Item Size y:", sizey.c_str());
		DebugItem("Item Show:", selectedItem->m_show ? ts : fs);
		DebugItem("Item Visible:", selectedItem->getState().isItemVisible() ? ts : fs);
		DebugItem("Item Hovered:", selectedItem->getState().isItemHovered() ? ts : fs);
		DebugItem("Item Active:", selectedItem->getState().isItemActive() ? ts : fs);
		DebugItem("Item Focused:", selectedItem->getState().isItemFocused() ? ts : fs);
		DebugItem("Item Clicked:", selectedItem->getState().isItemClicked() ? ts : fs);
		DebugItem("Item Edited:", selectedItem->getState().isItemEdited() ? ts : fs);
		DebugItem("Item Activated:", selectedItem->getState().isItemActivated() ? ts : fs);
		DebugItem("Item Deactivated:", selectedItem->getState().isItemDeactivated() ? ts : fs);
		DebugItem("Item DeactivatedAfterEdit:", selectedItem->getState().isItemDeactivatedAfterEdit() ? ts : fs);
		DebugItem("Item ToggledOpen:", selectedItem->getState().isItemToogledOpen() ? ts : fs);
		ImGui::EndGroup();
		ImGui::PopItemWidth();
		ImGui::SameLine();

        // right side
        ImGui::BeginGroup();
        ImGui::Checkbox("Show Label", &m_showLabels);

        ImGui::BeginChild("TreeChild", ImVec2(-1.0f, -1.0f), true);

        for (auto& window : mvApp::GetApp()->getItemRegistry().getRoots())
            renderNode(window.get());

        ImGui::EndChild();
        ImGui::EndGroup();
    }

}
