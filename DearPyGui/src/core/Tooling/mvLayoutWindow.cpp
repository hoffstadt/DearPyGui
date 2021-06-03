#include "mvLayoutWindow.h"
#include <imnodes.h>
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

    void mvLayoutWindow::renderTreeNode(mvAppItem* item)
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
        {
            m_selectedItem = item->m_name;
            m_dirtyNodes = true;
        }

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
                        renderTreeNode(children.get());
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }

        ImGui::PopID();

    }
    
    bool mvLayoutWindow::renderParentNode(mvAppItem* item, int nodeId, int slotId)
    {

        mvAppItem* parent = item->getParent();

        if (parent == nullptr)
            return false;

        if (m_dirtyNodes)
            imnodes::SetNodeGridSpacePos(nodeId, ImVec2(10.0f, 10.0f));

        imnodes::BeginNode(nodeId);

        imnodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(m_showLabels ? parent->m_specificedlabel.c_str() : parent->m_name.c_str());
        imnodes::EndNodeTitleBar();

        for (int i = 0; i < 4; i++)
        {
            if (i == item->getTarget())
            {
                imnodes::BeginOutputAttribute(slotId);
                ImGui::Text("Child Slot %d", item->getTarget());
                imnodes::EndOutputAttribute();
            }
            else
            {
                imnodes::BeginStaticAttribute(-1);
                ImGui::Text("Child Slot %d", i);
                imnodes::EndStaticAttribute();
            }
        }

        imnodes::EndNode();

        if (nodeId == m_selectedId)
        {
            m_selectedItem = parent->m_name;
            m_selectedId = -2;
        }

        return true;
    }

    bool mvLayoutWindow::renderChildAttr(mvAppItem* item, int slot, int slotId)
    {
        if (item->getChildren(slot).empty())
        {
            imnodes::BeginStaticAttribute(slotId);
            ImGui::Text("Child Slot %d", slot);
            imnodes::EndStaticAttribute();
        }
        else
        {
            imnodes::BeginOutputAttribute(slotId);
            ImGui::Text("Child Slot %d", slot);
            imnodes::EndOutputAttribute();
        }

        return !item->getChildren(slot).empty();
    }

    void mvLayoutWindow::renderChildNodes(mvAppItem* item, int slot, int& link, int& node, int startAttrId, int parentAttrId)
    {
        int current_x = 500 + 200 * slot;
        int current_y = 100;

        for (int i = 0; i < item->getChildren(slot).size(); i++)
        {
            mvAppItem* child = item->getChildren(slot)[i].get();

            imnodes::BeginNode(++node);

            if (m_dirtyNodes)
                imnodes::SetNodeGridSpacePos(node, ImVec2(current_x, current_y));

            imnodes::BeginNodeTitleBar();
            ImGui::TextUnformatted(m_showLabels ? child->m_specificedlabel.c_str() : child->m_name.c_str());
            imnodes::EndNodeTitleBar();

            imnodes::BeginInputAttribute(parentAttrId + i + 1);
            ImGui::Text("Parent");
            imnodes::EndInputAttribute();

            if (node == m_selectedId)
            {
                m_selectedItem = child->m_name;
                m_selectedId = -2;
            }

            imnodes::EndNode();

            current_y += 70 + 10;

            imnodes::Link(link++, parentAttrId, parentAttrId + i + 1);
        }
    }

    void mvLayoutWindow::renderNode(mvAppItem* item)
    {
        // dummy ids
        int linkId = 0;
        int nodeId = 1000;
        
        // attribute ids
        int parentParentAttrId = 2000;
        int currentItemParentAttrId = 3000;        
        int slot0AttrId = 4000;
        int slot1AttrId = 5000;
        int slot2AttrId = 6000;
        int slot3AttrId = 7000;

        imnodes::BeginNodeEditor();
        imnodes::ClearNodeSelection();

        bool hasParent = renderParentNode(item, ++nodeId, parentParentAttrId);

        imnodes::PushColorStyle(imnodes::ColorStyle_NodeOutline, mvColor(0, 255, 0, 255));
        imnodes::BeginNode(++nodeId);

        if (m_dirtyNodes)
            imnodes::SetNodeGridSpacePos(nodeId, ImVec2(100.0f, 300.0f));

        imnodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(m_showLabels ? item->m_specificedlabel.c_str() : item->m_name.c_str());
        imnodes::EndNodeTitleBar();

        if (hasParent)
        {
            imnodes::BeginInputAttribute(currentItemParentAttrId);
            ImGui::Text("Parent");
            imnodes::EndInputAttribute();
        }

        bool hasSlot0Children = renderChildAttr(item, 0, slot0AttrId);
        bool hasSlot1Children = renderChildAttr(item, 1, slot1AttrId);
        bool hasSlot2Children = renderChildAttr(item, 2, slot2AttrId);
        bool hasSlot3Children = renderChildAttr(item, 3, slot2AttrId);

        imnodes::EndNode();
        imnodes::PopColorStyle();

        if (nodeId == m_selectedId)
        {
            m_selectedItem = item->m_name;
            m_selectedId = -1;
        }

        if (hasSlot0Children) renderChildNodes(item, 0, linkId, nodeId, slot0AttrId, slot0AttrId);
        if (hasSlot1Children) renderChildNodes(item, 1, linkId, nodeId, slot1AttrId, slot1AttrId);
        if (hasSlot2Children) renderChildNodes(item, 2, linkId, nodeId, slot2AttrId, slot2AttrId);
        if (hasSlot3Children) renderChildNodes(item, 3, linkId, nodeId, slot3AttrId, slot3AttrId);

        if(hasParent)
            imnodes::Link(linkId++, parentParentAttrId, currentItemParentAttrId);

        imnodes::EndNodeEditor();

        if (imnodes::NumSelectedNodes() > 0)
        {
            int* selected_nodes = new int[imnodes::NumSelectedNodes()];
            imnodes::GetSelectedNodes(selected_nodes);
            m_selectedId = selected_nodes[0];
            delete[] selected_nodes;
        }

        if (m_selectedId == -1)
            m_dirtyNodes = false;
        else if (m_selectedId == -2)
        {
            m_dirtyNodes = true;
            m_selectedId = -1;
        }
        else
            m_dirtyNodes = true;
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
        ImGui::SameLine();
        ImGui::Checkbox("Node View", &m_nodeView);

        if (m_nodeView)
        {
            renderNode(selectedItem.get());
        }
        else
        {
            ImGui::BeginChild("TreeChild", ImVec2(-1.0f, -1.0f), true);

            for (auto& window : mvApp::GetApp()->getItemRegistry().getRoots())
                renderTreeNode(window.get());

            ImGui::EndChild();
        }
        ImGui::EndGroup();
    }

}
