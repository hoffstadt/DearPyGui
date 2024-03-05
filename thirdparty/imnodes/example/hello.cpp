#include "node_editor.h"
#include <imnodes.h>
#include <imgui.h>

namespace example
{
namespace
{
class HelloWorldNodeEditor
{
public:
    void show()
    {
        ImGui::Begin("simple node editor");

        ImNodes::BeginNodeEditor();
        ImNodes::BeginNode(1);

        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("simple node :)");
        ImNodes::EndNodeTitleBar();

        ImNodes::BeginInputAttribute(2);
        ImGui::Text("input");
        ImNodes::EndInputAttribute();

        ImNodes::BeginOutputAttribute(3);
        ImGui::Indent(40);
        ImGui::Text("output");
        ImNodes::EndOutputAttribute();

        ImNodes::EndNode();
        ImNodes::EndNodeEditor();

        ImGui::End();
    }
};

static HelloWorldNodeEditor editor;
} // namespace

void NodeEditorInitialize() { ImNodes::SetNodeGridSpacePos(1, ImVec2(200.0f, 200.0f)); }

void NodeEditorShow() { editor.show(); }

void NodeEditorShutdown() {}

} // namespace example
