#include "node_editor.h"
#include <imnodes.h>
#include <imgui.h>
#include <SDL_scancode.h>

#include <algorithm>
#include <vector>

namespace example
{
namespace
{
struct Node
{
    int   id;
    float value;

    Node(const int i, const float v) : id(i), value(v) {}
};

struct Link
{
    int id;
    int start_attr, end_attr;
};

struct Editor
{
    ImNodesEditorContext* context = nullptr;
    std::vector<Node>     nodes;
    std::vector<Link>     links;
    int                   current_id = 0;
};

void show_editor(const char* editor_name, Editor& editor)
{
    ImNodes::EditorContextSet(editor.context);

    ImGui::Begin(editor_name);
    ImGui::TextUnformatted("A -- add node");

    ImNodes::BeginNodeEditor();

    if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) &&
        ImNodes::IsEditorHovered() && ImGui::IsKeyReleased(ImGuiKey_A))
    {
        const int node_id = ++editor.current_id;
        ImNodes::SetNodeScreenSpacePos(node_id, ImGui::GetMousePos());
        ImNodes::SnapNodeToGrid(node_id);
        editor.nodes.push_back(Node(node_id, 0.f));
    }

    for (Node& node : editor.nodes)
    {
        ImNodes::BeginNode(node.id);

        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("node");
        ImNodes::EndNodeTitleBar();

        ImNodes::BeginInputAttribute(node.id << 8);
        ImGui::TextUnformatted("input");
        ImNodes::EndInputAttribute();

        ImNodes::BeginStaticAttribute(node.id << 16);
        ImGui::PushItemWidth(120.0f);
        ImGui::DragFloat("value", &node.value, 0.01f);
        ImGui::PopItemWidth();
        ImNodes::EndStaticAttribute();

        ImNodes::BeginOutputAttribute(node.id << 24);
        const float text_width = ImGui::CalcTextSize("output").x;
        ImGui::Indent(120.f + ImGui::CalcTextSize("value").x - text_width);
        ImGui::TextUnformatted("output");
        ImNodes::EndOutputAttribute();

        ImNodes::EndNode();
    }

    for (const Link& link : editor.links)
    {
        ImNodes::Link(link.id, link.start_attr, link.end_attr);
    }

    ImNodes::EndNodeEditor();

    {
        Link link;
        if (ImNodes::IsLinkCreated(&link.start_attr, &link.end_attr))
        {
            link.id = ++editor.current_id;
            editor.links.push_back(link);
        }
    }

    {
        int link_id;
        if (ImNodes::IsLinkDestroyed(&link_id))
        {
            auto iter = std::find_if(
                editor.links.begin(), editor.links.end(), [link_id](const Link& link) -> bool {
                    return link.id == link_id;
                });
            assert(iter != editor.links.end());
            editor.links.erase(iter);
        }
    }

    ImGui::End();
}

Editor editor1;
Editor editor2;
} // namespace

void NodeEditorInitialize()
{
    editor1.context = ImNodes::EditorContextCreate();
    editor2.context = ImNodes::EditorContextCreate();
    ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);

    ImNodesIO& io = ImNodes::GetIO();
    io.LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;
    io.MultipleSelectModifier.Modifier = &ImGui::GetIO().KeyCtrl;

    ImNodesStyle& style = ImNodes::GetStyle();
    style.Flags |= ImNodesStyleFlags_GridLinesPrimary | ImNodesStyleFlags_GridSnapping;
}

void NodeEditorShow()
{
    show_editor("editor1", editor1);
    show_editor("editor2", editor2);
}

void NodeEditorShutdown()
{
    ImNodes::PopAttributeFlag();
    ImNodes::EditorContextFree(editor1.context);
    ImNodes::EditorContextFree(editor2.context);
}
} // namespace example
