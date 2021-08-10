#include "mvTreeNode.h"
#include "core/mvInput.h"
#include "mvApp.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	void mvTreeNode::InsertParser(std::map<std::string, mvPythonParser>* parsers)
	{

		mvPythonParser parser(mvPyDataType::UUID, "Adds a tree node to add items to. Must be closed with the end command.", { "Containers", "Widgets" }, true);
		mvAppItem::AddCommonArgs(parser, (CommonParserArgs)(
			MV_PARSER_ARG_ID |
			MV_PARSER_ARG_INDENT |
			MV_PARSER_ARG_PARENT |
			MV_PARSER_ARG_BEFORE |
			MV_PARSER_ARG_SHOW |
			MV_PARSER_ARG_FILTER |
			MV_PARSER_ARG_DROP_CALLBACK |
			MV_PARSER_ARG_DRAG_CALLBACK |
			MV_PARSER_ARG_PAYLOAD_TYPE |
			MV_PARSER_ARG_SEARCH_DELAY |
			MV_PARSER_ARG_TRACKED |
			MV_PARSER_ARG_POS)
		);

		parser.addArg<mvPyDataType::Bool>("default_open", mvArgType::KEYWORD_ARG, "False", "Sets the tree node open by default.");
		parser.addArg<mvPyDataType::Bool>("open_on_double_click", mvArgType::KEYWORD_ARG, "False", "Need double-click to open node.");
		parser.addArg<mvPyDataType::Bool>("open_on_arrow", mvArgType::KEYWORD_ARG, "False", "Only open when clicking on the arrow part.");
		parser.addArg<mvPyDataType::Bool>("leaf", mvArgType::KEYWORD_ARG, "False", "No collapsing, no arrow (use as a convenience for leaf nodes).");
		parser.addArg<mvPyDataType::Bool>("bullet", mvArgType::KEYWORD_ARG, "False", "Display a bullet instead of arrow.");
		parser.addArg<mvPyDataType::Bool>("selectable", mvArgType::KEYWORD_ARG, "False", "Makes the tree selectable.");

		parser.finalize();

		parsers->insert({ s_command, parser });
	}
	mvTreeNode::mvTreeNode(mvUUID uuid)
		: mvAppItem(uuid)
	{
	}

	void mvTreeNode::draw(ImDrawList* drawlist, float x, float y)
	{
		ScopedID id(_uuid);

		ImGui::BeginGroup();
		
		if (*_value && _selectable)
			_flags |= ImGuiTreeNodeFlags_Selected;
		else
			_flags &= ~ImGuiTreeNodeFlags_Selected;

		bool expanded = ImGui::TreeNodeEx(_internalLabel.c_str(), _flags);

		if (ImGui::IsItemClicked())
			*_value = !*_value;

		if (!expanded)
		{
			ImGui::EndGroup();
			return;
		}

		for (auto& item : _children[1])
		{
			if (!item->preDraw())
				continue;

			item->draw(drawlist, ImGui::GetCursorPosX(), ImGui::GetCursorPosY());

			item->postDraw();
		}
		ImGui::TreePop();
		ImGui::EndGroup();

	}

	PyObject* mvTreeNode::getPyValue()
	{
		return ToPyBool(*_value);
	}

	void mvTreeNode::setPyValue(PyObject* value)
	{
		*_value = ToBool(value);
	}

	void mvTreeNode::setDataSource(mvUUID dataSource)
	{
		if (dataSource == _source) return;
		_source = dataSource;

		mvAppItem* item = mvApp::GetApp()->getItemRegistry().getItem(dataSource);
		if (!item)
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotFound, "set_value",
				"Source item not found: " + std::to_string(dataSource), this);
			return;
		}
		if (item->getValueType() != getValueType())
		{
			mvThrowPythonError(mvErrorCode::mvSourceNotCompatible, "set_value",
				"Values types do not match: " + std::to_string(dataSource), this);
			return;
		}
		_value = *static_cast<std::shared_ptr<bool>*>(item->getValue());
	}

	void mvTreeNode::handleSpecificKeywordArgs(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		if (PyObject* item = PyDict_GetItemString(dict, "selectable")) _selectable = ToBool(item);

		// helper for bit flipping
		auto flagop = [dict](const char* keyword, int flag, int& flags)
		{
			if (PyObject* item = PyDict_GetItemString(dict, keyword)) ToBool(item) ? flags |= flag : flags &= ~flag;
		};

		// flags
		flagop("default_open", ImGuiTreeNodeFlags_DefaultOpen, _flags);
		flagop("open_on_double_click", ImGuiTreeNodeFlags_OpenOnDoubleClick, _flags);
		flagop("open_on_arrow", ImGuiTreeNodeFlags_OpenOnArrow, _flags);
		flagop("leaf", ImGuiTreeNodeFlags_Leaf, _flags);
		flagop("bullet", ImGuiTreeNodeFlags_Bullet, _flags);
	}

	void mvTreeNode::getSpecificConfiguration(PyObject* dict)
	{
		if (dict == nullptr)
			return;
		 
		PyDict_SetItemString(dict, "selectable", ToPyBool(_selectable));

		// helper to check and set bit
		auto checkbitset = [dict](const char* keyword, int flag, const int& flags)
		{
			PyDict_SetItemString(dict, keyword, ToPyBool(flags & flag));
		};

		// flags
		checkbitset("default_open", ImGuiTreeNodeFlags_DefaultOpen, _flags);
		checkbitset("open_on_double_click", ImGuiTreeNodeFlags_OpenOnDoubleClick, _flags);
		checkbitset("open_on_arrow", ImGuiTreeNodeFlags_OpenOnArrow, _flags);
		checkbitset("leaf", ImGuiTreeNodeFlags_Leaf, _flags);
		checkbitset("bullet", ImGuiTreeNodeFlags_Bullet, _flags);

	}

}