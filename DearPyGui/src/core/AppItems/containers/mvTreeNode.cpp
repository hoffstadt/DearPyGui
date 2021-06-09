#include "mvTreeNode.h"
#include "core/mvInput.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvImGuiThemeScope.h"
#include "mvFontScope.h"

namespace Marvel {
	void mvTreeNode::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Undocumented function", { "Containers", "Widgets" });
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_LABEL |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_POS)
		);

		parser.addArg<mvPyDataType::Bool>("default_open", mvArgType::KEYWORD_ARG, "False");
		parser.addArg<mvPyDataType::Bool>("open_on_double_click", mvArgType::KEYWORD_ARG, "False", "Need double-click to open node");
		parser.addArg<mvPyDataType::Bool>("open_on_arrow", mvArgType::KEYWORD_ARG, "False", "Only open when clicking on the arrow part.");
		parser.addArg<mvPyDataType::Bool>("leaf", mvArgType::KEYWORD_ARG, "False", "No collapsing, no arrow (use as a convenience for leaf nodes).");
		parser.addArg<mvPyDataType::Bool>("bullet", mvArgType::KEYWORD_ARG, "False", "Display a bullet instead of arrow");
		parser.addArg<mvPyDataType::Bool>("selectable", mvArgType::KEYWORD_ARG, "False", "Makes the tree selectable");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}
	mvTreeNode::mvTreeNode(mvUUID uuid)
		: mvBoolPtrBase(uuid)
	{
	}

	void mvTreeNode::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(m_uuid);
		mvImGuiThemeScope scope(this);
		mvFontScope fscope(this);

		ImGui::BeginGroup();
		
		if (*m_value && m_selectable)
			m_flags |= ImGuiTreeNodeFlags_Selected;
		else
			m_flags &= ~ImGuiTreeNodeFlags_Selected;

		bool expanded = ImGui::TreeNodeEx(m_label.c_str(), m_flags);

		if (ImGui::IsItemClicked())
			*m_value = !*m_value;

		if (!expanded)
		{
			ImGui::EndGroup();
			return;
		}

		//we do this so that the children dont get the theme
		scope.cleanup();

		for (auto& item : m_children[1])
		{
			if (!item->preDraw())
				continue;

			item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

			item->postDraw();
		}
		ImGui::TreePop();
		ImGui::EndGroup();

	}

	void mvTreeNode::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "selectable")) m_selectable = ToBool(item);

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// flags
		flagop("default_open", ImGuiTreeNodeFlags_DefaultOpen, m_flags);
		flagop("open_on_double_click", ImGuiTreeNodeFlags_OpenOnDoubleClick, m_flags);
		flagop("open_on_arrow", ImGuiTreeNodeFlags_OpenOnArrow, m_flags);
		flagop("leaf", ImGuiTreeNodeFlags_Leaf, m_flags);
		flagop("bullet", ImGuiTreeNodeFlags_Bullet, m_flags);
	}

	void mvTreeNode::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "selectable", ToPyBool(m_selectable));

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		// flags
		checkbitset("default_open", ImGuiTreeNodeFlags_DefaultOpen, m_flags);
		checkbitset("open_on_double_click", ImGuiTreeNodeFlags_OpenOnDoubleClick, m_flags);
		checkbitset("open_on_arrow", ImGuiTreeNodeFlags_OpenOnArrow, m_flags);
		checkbitset("leaf", ImGuiTreeNodeFlags_Leaf, m_flags);
		checkbitset("bullet", ImGuiTreeNodeFlags_Bullet, m_flags);

	}

}